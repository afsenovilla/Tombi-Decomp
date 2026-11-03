#!/usr/bin/env python3
"""Every text a translation patch (*.tombipatch.json, tools/text_patch.py) can change, as one list of entries by category, for the
text editor (tools/text_editor.py).  No GUI here; everything is read from the player's own disc (image or extracted folder).

An entry is one translatable text of the chosen game language, with the places it is written to (its "targets"):
  wfm      dialogues of the *.WFM files (text_core.Project edits): the area dialogues (MFNT), the signs, the system
           popups and item names of group 0 (the same text in every MFNT file is ONE entry), the event titles (CFNT999,
           the same in every area: one entry)
  menu     the "menus" section of the patch: title / options screen (tools/menu_text.py), memory card screen and the save
           list's place names (tools/memcard_text.py), the sub-screen lists: item names, event names, warp place names, event
           hints, item descriptions (tools/subscreen_text.py), the pause menus (tools/pause_text.py, Spanish only)
  jp       a line of the Japanese release (tools/jp_text.py) with no PAL counterpart: read only

Catalog(source, lang, base_patch=None, improved=None) builds the list; .text(e) / .original(e) / .improved(e) / .set_text(e, t);
.other(e, letter) the same entry in another PAL language; .check(e, text) the validator's messages; .to_patch() the patch.

  text_catalog.py stats DISC [LANG]     prints the number of entries per category
"""
import copy, json, os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import text_core as C
import text_patch as TP
import wfm_py as W
import title_layout as TL
import menu_text as MT
import memcard_text as MC
import pause_text as PT
import subscreen_text as SS

LANG_ORDER = "BFGIHSPWD"     # --lang n of the port: n = index of the letter (B English 0 ... S Spanish 5 ... D Danish 8)
CATEGORIES = ["dialogue", "sign", "system", "item_name", "item_desc", "event_title", "event_name", "event_hint",
              "place", "memcard", "title_menu", "pause", "jp_only"]
SIGN_TABLE = 0x80079EBC      # MAIN0 sign table (u16 k, u16 dx) x 37 and the button-config variants (tools/dialogue_refs.py)
SIGN_CFG = {2: 0x80079E8C, 3: 0x80079E98, 4: 0x80079EA4, 6: 0x80079EB0}


class Entry:
    __slots__ = ("eid", "cat", "group", "kind", "targets", "key", "label", "note", "jp")

    def __init__(self, eid, cat, group, kind, targets, key, label="", note=""):
        self.eid, self.cat, self.group, self.kind, self.targets, self.key = eid, cat, group, kind, targets, key
        self.label, self.note, self.jp = label, note, None

    def __repr__(self):
        return "<Entry %s %s>" % (self.eid, self.cat)


def lang_n(letter):
    return LANG_ORDER.index(letter)


def wfm_path(path, letter):
    """The same WFM file in another language (AREA05/MFNT050S.WFM -> AREA05/MFNT050B.WFM)."""
    return path[:-5] + letter + path[-4:]


def _menu_file(tmpl, letter):
    return tmpl.replace("{n}", str(lang_n(letter)))


def _ok_place(t):
    letters = sum(c.isalpha() for c in t)
    return letters >= 3 and re.fullmatch(r"[\w \n¿?¡!.,'\-]+", t) is not None


class Catalog:
    def __init__(self, source, lang="S", base_patch=None, improved=None, log=lambda *a: None):
        """source: text_core.Source; lang: the PAL language letter edited; base_patch: the translation being edited (a patch
        dict, None = a new one); improved: the improved-translation patch dict shown beside (Spanish), or None."""
        self.src, self.lang, self.log = source, lang, log
        self.proj = C.Project(source)
        self.proj.langs = set(self.proj.languages())
        self.proj.link = False
        self.improved = improved
        self.extra = {}            # keys of the patch the editor does not touch (portGfx, notes ...)
        self._menu_cache, self._files = {}, {}
        if base_patch:
            self.load_patch(base_patch)
        self.entries = []
        self.by_id = {}
        self._build()

    # ---- the patch being edited
    def load_patch(self, patch):
        self.proj.edits, self.proj.menus = {}, {}
        TP.apply_to_project(self.proj, patch)
        for path, d in (patch.get("edits") or {}).items():     # kept as they are (see to_patch), also those equal to the disc
            if path in self.src.files and isinstance(d, dict):
                for k, t in d.items():
                    if str(k).isdigit() and int(k) < self.proj.count(path):
                        self.proj.edits.setdefault(path, {}).setdefault(int(k), t)
        self.proj.langs = set(self.proj.languages())
        self.proj.menus = copy.deepcopy(patch.get("menus") or {})
        self.extra = {k: v for k, v in patch.items() if k in ("title", "author", "notes", "portGfx")}

    def to_patch(self, title=None, author=None):
        p = TP.make(self.proj, author if author is not None else self.extra.get("author", ""),
                    title if title is not None else self.extra.get("title", ""), self.extra.get("notes", ""))
        # every edit of the project, also one equal to the disc text that came with the loaded patch (an event title kept
        # as it is still rewrites its layout copies in the overlays, text_core._title_patches)
        p["edits"] = {path: {str(i): t for i, t in sorted(e.items())} for path, e in sorted(self.proj.edits.items()) if e}
        if self.extra.get("portGfx"):
            p["portGfx"] = self.extra["portGfx"]
        menus = {f: s for f, s in ((f, {k: v for k, v in s.items() if v}) for f, s in self.proj.menus.items()) if s}
        if menus:
            p["menus"] = menus
        else:
            p.pop("menus", None)
        return p

    def n_changed(self):
        return sum(1 for e in self.entries if e.kind != "jp" and self.changed(e))

    # ---- disc files
    def read(self, name):
        if name not in self._files:
            try:
                self._files[name] = self.src.read_any(name)
            except (OSError, KeyError):
                self._files[name] = None
        return self._files[name]

    def _groups(self, path):
        """group bases of a WFM (header bytes 16..143, tools/dialogue_refs.py): [base of group 0, 1, ...]"""
        b = self.read(path)
        if b is None:
            return [0]
        t = struct.unpack_from("<64H", b, 16)
        return [0] + [x for x in t[1:] if x] if t else [0]

    def _signs(self):
        """(k, dx) pairs of the MAIN0 sign table."""
        exe = self.read("EXE/MAIN0.EXE")
        if not exe or exe[:8] != b"PS-X EXE":
            return set()
        t = struct.unpack_from("<I", exe, 0x18)[0]
        off = lambda a: a - t + 0x800
        out = set()
        for i in range(37):
            k, dx = struct.unpack_from("<HH", exe, off(SIGN_TABLE + 8 * i))
            if i in SIGN_CFG:
                out |= {(k, struct.unpack_from("<H", exe, off(SIGN_CFG[i] + 2 * b))[0]) for b in range(6)}
            else:
                out.add((k, dx))
        return out

    # ---- menus of one language: {(section, ordinal): (original text, [(file, patch section, patch key)])}
    def menu_items(self, letter):
        if letter in self._menu_cache:
            return self._menu_cache[letter]
        out = {}
        n = lang_n(letter)
        o = "SYSTEM/O%dPTSUB0.BIN" % n
        d = self.read(o)
        if d:
            try:
                L = MT.Layout(d)
                for i in (0, 1):
                    out[("title", i)] = (MT.decode(L.string(L.title[i])), [(o, "title", str(i))])
                for i, a in enumerate(L.options):
                    t = MT.decode(L.string(a))
                    if t.strip() and not re.fullmatch(r"(\{-?\d+\}| )*", t):
                        out[("options", i)] = (t, [(o, "options", str(i))])
            except MT.MenuError as e:
                self.log("%s: %s" % (o, e))
        m = "SYSTEM/D%dSPSUB2.BIN" % n
        d = self.read(m)
        if d:
            try:
                L = MC.Layout(d)
                for i, a in enumerate(L.messages):
                    t = MC.decode(L.string(a))
                    if t.strip():
                        out[("messages", i)] = (t, [(m, "messages", str(i))])
                for i, a in enumerate(L.locations):
                    t = MC.decode(L.string(a))
                    if t.strip():
                        out[("locations", i)] = (t, [(m, "locations", str(i))])
            except MC.MemcardError as e:
                self.log("%s: %s" % (m, e))
        subs = ["SYSTEM/D%dSPSUB.BIN" % n, "SYSTEM/D%dSPSUB3.BIN" % n]
        texts, events, places = {}, {}, {}
        for f in subs + [m]:
            d = self.read(f)
            if not d:
                continue
            texts[f] = [t for _, t in SS.dump(d)]
            if f == m:
                continue
            try:
                base = SS._base(d, 0)
                words = SS._words(d)
            except SS.SubscreenError as e:
                self.log("%s: %s" % (f, e))
                continue
            try:
                s = dict(SS.dump(d))
                events[f] = [s.get(words[i] - base) for i in SS._event_table(d, base)]
            except SS.SubscreenError as e:
                self.log("%s: %s" % (f, e))
            try:
                ps = {}
                for w in set(words):
                    q = w - base
                    if 0 <= q < len(d) and not q & 1:
                        g = SS._place_at(d, q)
                        if g:
                            t = MC.decode(list(g))
                            if _ok_place(t):
                                ps[q] = t
                places[f] = [ps[q] for q in sorted(ps)]
            except SS.SubscreenError as e:
                self.log("%s: %s" % (f, e))
        main = subs[0] if subs[0] in texts else subs[1] if subs[1] in texts else None
        if main:
            ev = [t for t in events.get(main, []) if t]
            evset = set(ev)
            seen = set()
            k = 0
            for t in texts[main]:
                if t in seen or t in evset:
                    continue
                seen.add(t)
                tg = [(f, "itemNames", t) for f in subs + [m] if t in texts.get(f, ())]
                out[("listNames", k)] = (t, tg)
                k += 1
            seen = set()
            for t in ev:
                if t in seen:
                    continue
                seen.add(t)
                out[("eventNames", len(seen) - 1)] = (t, [(f, "eventNames", t) for f in subs if t in (events.get(f) or ())])
            seen, k = set(), 0
            for f in (subs[1], subs[0]):
                for t in places.get(f, ()):
                    if t in seen:
                        continue
                    seen.add(t)
                    out[("placeNames", k)] = (t, [(g, "placeNames", t) for g in subs if t in (places.get(g) or ())])
                    k += 1
        for f in subs:
            d = self.read(f)
            if not d:
                continue
            for a, b in SS._wfm_spans(d):
                try:
                    w = W.decode(d[a:b])
                except Exception:      # noqa: BLE001
                    continue
                sec = {200: "eventDescriptions", 205: "itemDescriptions"}.get(len(w.dialogues))
                if sec is None or (sec == "eventDescriptions" and f != subs[0]):
                    continue
                self._files[(f, sec)] = (w, a, b)
                for i, dl in enumerate(w.dialogues):
                    t = W.words_to_text(w, dl.words, dl.font)
                    if not C.plain(t):
                        continue
                    key = (sec, i)
                    if key in out:
                        out[key][1].append((f, sec, str(i)))
                    else:
                        out[key] = (t, [(f, sec, str(i))])
        if lang_n(letter) in PT.STRIPS:
            e = "EXE/MAIN%d.EXE" % n
            d = self.read(e)
            if d:
                seen = []
                for _, _, _, s in PT.entries(d):
                    end = d.index(b"\xff", s)
                    try:
                        t = PT.decode(d[s:end], n)
                    except (KeyError, IndexError):
                        continue
                    if t.strip() and t not in seen:
                        seen.append(t)
                for k, t in enumerate(seen):
                    out[("pause", k)] = (t, [(e, "pause", t)])
        self._menu_cache[letter] = out
        return out

    # ---- the list
    def _build(self):
        L = self.lang
        signs = self._signs()
        names = [n for n in self.src.names() if C.lang_of(n) == L]
        menu = self.menu_items(L)
        item_names = {t for (s, _), (t, _) in menu.items() if s == "listNames"}
        by_key = {}

        def add(key, cat, group, path, idx, label):
            e = by_key.get(key)
            if e is None:
                e = Entry("w%d" % len(self.entries), cat, group, "wfm", [], key, label)
                by_key[key] = e
                self.entries.append(e)
            e.targets.append((path, idx))
            return e
        for path in names:
            base = os.path.basename(path).upper()
            area = path.split("/")[0]
            try:
                cnt = self.proj.count(path)
            except C.CoreError as ex:
                self.log(str(ex))
                continue
            if base.startswith("CFNT"):
                for i in range(cnt):
                    t = self.proj.original(path, i)
                    add(("title", base, i, t), "event_title", "banner" if i == 0 else "titles", path, i, "#%d" % i)
                continue
            groups = self._groups(path)
            for i in range(cnt):
                t = self.proj.original(path, i)
                k = max(j for j, b in enumerate(groups) if b <= i)
                dx = i - groups[k]
                if k == 0:
                    if not C.plain(t):
                        continue
                    if i <= 2:
                        add(("verb", i), "system", "verbs", path, i, "#%d" % i)
                    else:
                        cat = "item_name" if C.plain(t, 999) in item_names else "system"
                        add(("g0", t), cat, "dialogue_names" if cat == "item_name" else "popups", path, i, "#%d" % i)
                    continue
                if not C.plain(t):
                    continue            # an empty slot: nothing to translate
                cat = "sign" if (k, dx) in signs else "dialogue"
                add(("dlg", base, i, t), cat, area, path, i, "%s #%d" % (base[:-4], i))
        for key, (t, tg) in sorted(menu.items(), key=lambda kv: (kv[0][0], kv[0][1])):
            sec = key[0]
            cat, group = {"title": ("title_menu", "title"), "options": ("title_menu", "options"),
                          "messages": ("memcard", "messages"), "locations": ("place", "save_list"),
                          "listNames": ("item_name", "list_names"), "eventNames": ("event_name", "event_list"),
                          "placeNames": ("place", "warp_list"), "eventDescriptions": ("event_hint", "event_hints"),
                          "itemDescriptions": ("item_desc", "item_desc"), "pause": ("pause", "pause")}[sec]
            e = Entry("m:%s:%d" % key, cat, group, "menu", None, key, "%s %d" % key)
            self.entries.append(e)
        # port-only extra strings that exist in the patches (optionsAfter: the controls screen title of --lang 5)
        for p in (self.improved, {"menus": self.proj.menus}):
            for f, spec in ((p or {}).get("menus") or {}).items():
                if f == "SYSTEM/O%dPTSUB0.BIN" % lang_n(L):
                    for k in (spec.get("optionsAfter") or {}):
                        eid = "m:optionsAfter:%s" % k
                        if not any(e.eid == eid for e in self.entries):
                            self.entries.append(Entry(eid, "title_menu", "options", "menu", None, ("optionsAfter", int(k)),
                                                      "optionsAfter %s" % k))
        self.by_id = {e.eid: e for e in self.entries}

    def add_japanese(self, jp):
        """jp: JpIndex. Attaches the JP lines to the entries and adds the JP-only lines as read-only entries."""
        self.entries = [e for e in self.entries if e.kind != "jp"]
        for e in self.entries:
            e.jp = None
            if e.kind == "wfm":
                lines = []
                for p, i in e.targets:
                    for t in jp.lines.get("%s#%d" % (p[:-5], i), ()):
                        if t not in lines:
                            lines.append(t)
                e.jp = lines or None
            elif e.key[0] in ("eventDescriptions", "itemDescriptions"):
                e.jp = jp.lines.get("%s#%d" % e.key) or None
        for k, (tag, i, t, note) in enumerate(jp.solo):
            e = Entry("j%d" % k, "jp_only", tag.split("_")[0], "jp", [], (tag, i), "%s #%d" % (tag, i), note)
            e.jp = [t]
            self.entries.append(e)
        self.by_id = {e.eid: e for e in self.entries}

    # ---- texts
    def _menu_targets(self, e, letter=None):
        letter = letter or self.lang
        item = self.menu_items(letter).get(e.key)
        if item is None and e.key[0] == "optionsAfter":
            return "", [("SYSTEM/O%dPTSUB0.BIN" % lang_n(letter), "optionsAfter", str(e.key[1]))]
        return item if item else (None, [])

    @staticmethod
    def _spec_get(menus, f, sec, key):
        v = ((menus or {}).get(f) or {}).get(sec, {}).get(key)
        if sec == "title" and isinstance(v, dict):
            v = v.get("text")
        return v

    def original(self, e, letter=None):
        if e.kind == "wfm":
            p, i = e.targets[0]
            p = wfm_path(p, letter) if letter else p
            try:
                return self.proj.original(p, i) if i < self.proj.count(p) else None
            except (C.CoreError, KeyError):
                return None
        if e.kind == "jp":
            return ""
        return self._menu_targets(e, letter)[0]

    def text(self, e):
        if e.kind == "wfm":
            return self.proj.text(*e.targets[0])
        if e.kind == "jp":
            return ""
        orig, tg = self._menu_targets(e)
        for f, sec, key in tg:
            v = self._spec_get(self.proj.menus, f, sec, key)
            if v is not None:
                return v
        return orig if orig is not None else ""

    def improved_text(self, e):
        """The text of the improved translation, or None when there is none (other languages, JP-only lines)."""
        if not self.improved or e.kind == "jp":
            return None
        if e.kind == "wfm":
            p, i = e.targets[0]
            if C.lang_of(p) not in {C.lang_of(x) for x in self.improved.get("edits", {})} | {"S"}:
                return None
            v = self.improved.get("edits", {}).get(p, {}).get(str(i))
            return v if v is not None else self.proj.original(p, i)
        orig, tg = self._menu_targets(e)
        for f, sec, key in tg:
            v = self._spec_get(self.improved.get("menus"), f, sec, key)
            if v is not None:
                return v
        return orig

    def other(self, e, letter):
        """The original text of the entry in another PAL language (None when that file / string does not exist)."""
        if e.kind == "jp":
            return None
        if e.kind == "wfm":
            return self.original(e, letter)
        try:
            return self._menu_targets(e, letter)[0]
        except Exception:      # noqa: BLE001
            return None

    def changed(self, e):
        if e.kind == "jp":
            return False
        o = self.original(e)
        return C.norm(self.text(e)) != C.norm(o or "")

    def set_text(self, e, text):
        if e.kind == "jp":
            return
        if e.kind == "wfm":
            for p, i in e.targets:
                if self.proj.text(p, i) != text:
                    self.proj.set_text(p, i, text)
            return
        orig, tg = self._menu_targets(e)
        same = orig is not None and C.norm(text) == C.norm(orig) and e.key[0] != "optionsAfter"
        for f, sec, key in tg:
            spec = self.proj.menus.setdefault(f, {})
            d = spec.setdefault(sec, {})
            if same:
                d.pop(key, None)
            elif sec == "title":
                d[key] = self._title_spec(f, int(key), text, d.get(key))
            elif sec == "itemNames" and re.search(r"D\dSPSUB2\.BIN$", f):
                try:            # the memory card overlay's copy can only change in place (no pointers to repack)
                    SS._in_place(self.read(f), {key: text})
                    d[key] = text
                except SS.SubscreenError:
                    d.pop(key, None)
            else:
                d[key] = text
            if not d:
                spec.pop(sec, None)
            if not spec:
                self.proj.menus.pop(f, None)

    def _title_spec(self, f, i, text, old):
        """{"text", "pal"} of a title string: the palette bytes follow the drawn glyphs (one each); a new length stretches the
        original palette over the new glyphs."""
        out = {"text": text}
        try:
            g = [c for c in MT.encode(text) if c > 0]
            if old and isinstance(old, dict) and len(old.get("pal") or ()) == len(g):
                out["pal"] = old["pal"]
                return out
            L = MT.Layout(self.read(f))
            og = [c for c in L.string(L.title[i]) if c > 0]
            p = L.off(L.title_pal[i])
            pal = list(L.d[p:p + len(og)])
            if pal and len(g) != len(pal):
                out["pal"] = [pal[min(len(pal) - 1, k * len(pal) // max(1, len(g)))] for k in range(len(g))]
        except MT.MenuError:
            pass
        return out

    # ---- checks
    def check(self, e, text):
        """[(level, message)] for the text of entry e: level "err" (the game cannot show it as written), "warn", "info"."""
        import check_text_patch as K
        if e.kind == "jp":
            return []
        out = []
        if e.kind == "wfm":
            # a character no font has stops the encoder: report it, then check the rest with '?' in its place (the box
            # sizes and pages still matter)
            out, probe = [], text
            for _ in range(8):
                try:
                    self.proj.words(e.targets[0][0], e.targets[0][1], probe)
                    break
                except W.UnknownGlyph as ex:
                    ch = getattr(ex, "char", None) or (re.search(r"'(.)' \(U\+", str(ex)) or [None, None])[1]
                    if not ch or ch == "?":
                        break
                    out.append(("err", "encode: %s" % ex))
                    probe = probe.replace(ch, "?")
                except W.WfmError:
                    break
            K.VERB_FIRST = bool(self.proj.verb_first)
            K.NOTES.clear()
            seen = {m for _, m in out}
            text = probe
            for p, i in e.targets[:1] + [t for t in e.targets[1:] if self.proj.original(*t) != self.proj.original(*e.targets[0])]:
                for m in K.check_entry(self.proj, p, i, text):
                    if m not in seen:
                        seen.add(m)
                        out.append(("err", m))
            for n in K.NOTES:
                out.append(("info", n.split(": ", 1)[-1]))
            K.NOTES.clear()
            cut = self.proj.title_console_cut(e.targets[0][0], e.targets[0][1], text)
            if cut:
                out.append(("info", "console: %d / %d" % cut))
            return out
        sec = e.key[0]
        orig, tg = self._menu_targets(e)
        for f, psec, key in tg:
            data = self.read(f)
            if data is None:
                continue
            try:
                if psec in ("title", "options", "optionsAfter"):
                    spec = copy.deepcopy(self.proj.menus.get(f) or {})
                    v = self._title_spec(f, int(key), text, (spec.get("title") or {}).get(key)) if psec == "title" else text
                    spec.setdefault(psec, {})[key] = v
                    if psec == "optionsAfter" and key not in (spec.get("options") or {}):
                        pass
                    MT.apply(data, {k: spec[k] for k in ("title", "options", "optionsAfter") if k in spec})
                elif psec in ("messages", "locations"):
                    spec = copy.deepcopy(self.proj.menus.get(f) or {})
                    spec.setdefault(psec, {})[key] = text
                    MC.apply(data, {k: spec[k] for k in ("messages", "locations") if k in spec})
                elif psec == "itemNames":
                    if re.search(r"D\dSPSUB2\.BIN$", f):
                        try:
                            SS._in_place(data, {key: text})
                        except SS.SubscreenError:
                            out.append(("info", "memcard"))
                        continue
                    SS.encode(text)
                    try:
                        SS._in_place(data, {key: text})
                    except SS.SubscreenError:
                        SS.apply_names(data, {key: text})
                elif psec == "eventNames":
                    SS.encode(text)
                    SS.apply_event_names(data, {key: text})
                elif psec == "placeNames":
                    MC.encode(text)
                    SS.apply_places(data, {key: text})
                elif psec in ("eventDescriptions", "itemDescriptions"):
                    out += self._check_desc(f, psec, int(key), text, orig)
                    edits = dict((self.proj.menus.get(f) or {}).get(psec) or {})
                    edits[key] = text
                    SS.apply_descriptions(data, edits, 200 if psec == "eventDescriptions" else 205, self.proj.pool())
                elif psec == "pause":
                    PT.apply(data, {"pause": {key: text}}, lang_n(self.lang))
            except (MT.MenuError, MC.MemcardError, SS.SubscreenError, PT.PauseError, W.WfmError) as ex:
                m = "%s: %s" % (os.path.basename(f), ex)
                if ("err", m) not in out:
                    out.append(("err", m))
        return out

    def _check_desc(self, f, sec, i, text, orig):
        import check_text_patch as K
        import text_preview as P
        w = self._desc_bank(f, sec)
        if w is None:
            return []
        out = []
        try:
            wc = C.clone(w)
            words = W.text_to_words(wc, text, w.dialogues[i].font, self.proj.pool())
            ow = W.text_to_words(C.clone(w), orig, w.dialogues[i].font, self.proj.pool()) if orig else []
            detail = []
            new = K.problems_of(wc, words, detail) - (K.problems_of(C.clone(w), ow) - {"wide"} if ow else set())
            if new:
                out.append(("err", "preview: " + ", ".join(sorted(new)) + (" (" + "; ".join(detail) + ")" if detail else "")))
        except W.WfmError as ex:
            out.append(("err", str(ex)))
        if orig and K.tags(text) != K.tags(orig):
            out.append(("err", "control codes differ: %s vs %s" % (K.tags(text), K.tags(orig))))
        return out

    def _desc_bank(self, f, sec):
        if (f, sec) not in self._files:
            self.menu_items(self.lang)
        v = self._files.get((f, sec))
        return v[0] if v else None

    # ---- preview
    def preview(self, e, text, scale=2):
        """(PIL image, warnings) of how the box will look, or (None, []) when the entry has no WFM preview."""
        import text_preview as P
        if e.kind == "wfm":
            p, i = e.targets[0]
            for _ in range(8):          # unknown characters drawn as '?' (check() reports them)
                try:
                    words, wc = self.proj.words(p, i, text)
                    break
                except W.UnknownGlyph as ex:
                    ch = getattr(ex, "char", None) or (re.search(r"'(.)' \(U\+", str(ex)) or [None, None])[1]
                    if not ch or ch == "?":
                        raise
                    text = text.replace(ch, "?")
            else:
                return None, []
            return P.render(wc, words, scale=scale, title=self.proj.title_lines(p, i, text))
        if e.kind == "menu" and e.key[0] in ("eventDescriptions", "itemDescriptions"):
            tg = self._menu_targets(e)[1]
            w = self._desc_bank(tg[0][0], e.key[0]) if tg else None
            if w is None:
                return None, []
            wc = C.clone(w)
            words = W.text_to_words(wc, text, w.dialogues[e.key[1]].font, self.proj.pool())
            return P.render(wc, words, scale=scale)
        return None, []

    def check_all(self, progress=None):
        """{eid: [(level, msg)]} of the changed entries with problems, plus the whole patch encoded like the port's tools do
        (Project.encode_edited) under the key "*"."""
        out = {}
        todo = [e for e in self.entries if e.kind != "jp" and self.changed(e)]
        for k, e in enumerate(todo):
            if progress:
                progress(k, len(todo))
            try:
                m = [x for x in self.check(e, self.text(e)) if x[0] == "err"]
            except Exception as ex:      # noqa: BLE001
                m = [("err", str(ex))]
            if m:
                out[e.eid] = m
        files, errors = self.proj.encode_edited()
        if errors:
            out["*"] = [("err", x) for x in errors]
        return out


# ---- Japanese ----------------------------------------------------------------------------------------------------

class JpIndex:
    """JP lines keyed by PAL target ("AREA05/MFNT050#12", "eventDescriptions#3") and the lines with no PAL counterpart."""
    VERSION = 1

    def __init__(self, lines=None, solo=None):
        self.lines = lines or {}
        self.solo = solo or []

    @classmethod
    def build(cls, jp_disc, pal_disc):
        import jp_text as J
        r = J.align_disc(jp_disc, pal_disc, "B")
        lines, solo = {}, []
        for c in r["containers"]:
            if c["copy_of"]:
                continue
            m = c["pal"]
            for i, t in enumerate(c["texts"]):
                if not J.real(t):
                    continue
                js = c["map"].get(i) if m else None
                if m and js:
                    for j in js:
                        if "#" in m:
                            sec = {205: "itemDescriptions", 200: "eventDescriptions"}.get(len(r["pal_texts"][m]))
                            if sec:
                                lines.setdefault("%s#%d" % (sec, j), []).append(t)
                        else:
                            lines.setdefault("%s#%d" % (m[:-5], j), []).append(t)
                else:
                    solo.append((c["tag"], i, t, "nopal" if m else "noblock"))
        return cls(lines, solo)

    def save(self, path):
        json.dump({"version": self.VERSION, "lines": self.lines, "solo": self.solo}, open(path, "w", encoding="utf-8"),
                  ensure_ascii=False)

    @classmethod
    def load(cls, path):
        d = json.load(open(path, encoding="utf-8"))
        if d.get("version") != cls.VERSION:
            raise ValueError("old cache")
        return cls(d["lines"], [tuple(x) for x in d["solo"]])


def main():
    if len(sys.argv) >= 3 and sys.argv[1] == "stats":
        cat = Catalog(C.Source(sys.argv[2]), sys.argv[3] if len(sys.argv) > 3 else "S")
        import collections
        c = collections.Counter(e.cat for e in cat.entries)
        for k in CATEGORIES:
            print("%-12s %5d" % (k, c.get(k, 0)))
        print("total", len(cat.entries))
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
