#!/usr/bin/env python3
"""Logic of the Tomba! text editor (no GUI): open a disc image or extracted fs, edit dialogues, build the disc.

  Source      a raw/cooked .bin image or a folder with the disc files; lists and reads the *.WFM files
  Project     decoded files + the edits (path -> {dialogue index: text}); JSON project, TXT/JSON export and import,
              validation, search, encoding of the edited WFM files and the disc build (tools/mkdisc.py --relayout)

Command line (used by the tests; the GUI is tools/text_editor.py):
  text_core.py render SRC PATH IDX [-o OUT.png] [--scale N]     draw one dialogue (PATH like AREA05/MFNT050S.WFM)
  text_core.py export SRC OUT.json|OUT.txt [--all-languages]    every dialogue as text
  text_core.py build  SRC PROJECT.json|IMPORT.json|IMPORT.txt -o OUT.bin [--work DIR] [--disc IMAGE]
"""
import argparse, contextlib, io, json, os, re, shutil, struct, sys, tempfile, unicodedata
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n, text_strings
from i18n import tr

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wfm_py as W
import title_layout as TL
import verb_order as VO
import menu_text as MT
import memcard_text as MC

LANG_MAIN = "S"          # the PAL Spanish disc uses the *S.WFM files; the other languages are still on the disc
PROJECT_VERSION = 1


class CoreError(Exception):
    pass


LANG_NAMES = {"B": "Inglés", "D": "Danés", "F": "Francés", "G": "Alemán", "H": "Neerlandés", "I": "Italiano",
              "P": "Portugués", "S": "Español", "W": "Sueco"}


def lang_of(path):
    """Language letter of a WFM file (the last letter of its name: MFNT050S -> S)."""
    return os.path.splitext(os.path.basename(path))[0][-1:].upper()


def is_main_lang(path):
    return lang_of(path) == LANG_MAIN


def norm(text):
    """Cosmetic line feeds do not count."""
    return text.replace("\r", "").replace("\n", "")


# ---- sources -----------------------------------------------------------------------------------------------------

class Source:
    def __init__(self, path):
        path = os.path.abspath(path)
        self.path = path
        self.image = None
        self.root = None
        if os.path.isdir(path):
            self.kind = "folder"
            self.root = path if os.path.isdir(os.path.join(path, "AREA00")) or not os.path.isdir(os.path.join(path, "fs")) \
                else os.path.join(path, "fs")
            self.files = {}
            for d, _, fs in os.walk(self.root):
                for f in fs:
                    if f.upper().endswith(".WFM"):
                        full = os.path.join(d, f)
                        self.files[os.path.relpath(full, self.root).replace(os.sep, "/")] = full
            sib = os.path.join(os.path.dirname(self.root), "Tombi.bin")
            if os.path.isfile(sib):
                self.image = sib
        elif os.path.isfile(path):
            import cdfiles
            self.kind = "image"
            self.image = path
            with open(path, "rb") as f:
                f.seek(16 * 2352 + 25)
                raw = f.read(5) == b"CD001"
                f.seek(16 * 2048 + 1)
                cooked = f.read(5) == b"CD001"
            if not (raw or cooked):
                raise CoreError(tr("tc.notiso", path))
            self.sector, self.off = (2352, 24) if raw else (2048, 0)
            lay = cdfiles.layout_from_image(path)
            self.layout = lay
            self.files = {k: v for k, v in lay.items() if k.upper().endswith(".WFM")}
        else:
            raise CoreError(tr("tc.nofile", path))
        if not self.files:
            raise CoreError(tr("tc.nowfm", path))

    def names(self):
        return sorted(self.files)

    def all_names(self):
        """Every file name of the disc (the overlays that hold the event title layout are among them)."""
        if self.kind == "image":
            return list(self.layout)
        if not hasattr(self, "_all"):
            self._all = [os.path.relpath(os.path.join(d, f), self.root).replace(os.sep, "/")
                         for d, _, fs in os.walk(self.root) for f in fs if f.upper().endswith(".BIN")]
        return self._all

    def read(self, name):
        if self.kind == "folder":
            return open(self.files[name], "rb").read()
        lba, size = self.files[name]
        out = bytearray()
        with open(self.image, "rb") as f:
            for i in range((size + 2047) // 2048):
                f.seek((lba + i) * self.sector + self.off)
                out += f.read(2048)
        return bytes(out[:size])

    def read_any(self, name):
        """Any file of the disc (used to take the executables)."""
        if self.kind == "folder":
            return open(os.path.join(self.root, name), "rb").read()
        lba, size = self.layout[name]
        out = bytearray()
        with open(self.image, "rb") as f:
            for i in range((size + 2047) // 2048):
                f.seek((lba + i) * self.sector + self.off)
                out += f.read(2048)
        return bytes(out[:size])


# ---- project -----------------------------------------------------------------------------------------------------

def plain(text, n=60):
    """One-line summary of a dialogue text for lists."""
    t = re.sub(r"\{[^}]*\}", " ", text.replace("\n", ""))
    t = re.sub(r"\s+", " ", t).strip()
    return t if len(t) <= n else t[:n - 1] + "…"


def fold(s):
    return "".join(c for c in unicodedata.normalize("NFD", s.casefold()) if unicodedata.category(c) != "Mn")


class Project:
    def __init__(self, source):
        self.src = source
        self.edits = {}            # path -> {idx: text}
        self._wfm = {}
        self._orig = {}
        self._pool = None
        self._layouts = {}
        self.link = True           # editing one entry edits its copies in the other areas
        self.undo_stack = []
        self.langs = {LANG_MAIN}   # languages shown / exported / imported / built (saved in the project file)
        self.glyph_shift = {}      # {height: {char: dx}}: glyph bitmaps moved inside their cell in the edited files (W.shift_glyphs)
        self.menus = {}            # title / options menu texts of SYSTEM/OnPTSUB0.BIN (menu_text.py; saved in the project file)
        self.verb_first = False    # item popup order "Obtienes: Rana" (verb_order.py patch of EXE/MAINn.EXE; saved in the project file)

    def languages(self):
        """Language letters present on the disc, in LANG_NAMES order (unknown ones last)."""
        have = {lang_of(n) for n in self.src.names()}
        return sorted(have, key=lambda l: (list(LANG_NAMES).index(l) if l in LANG_NAMES else 99, l))

    def selected(self, path):
        return lang_of(path) in self.langs

    # decoded files
    def wfm(self, path):
        if path not in self._wfm:
            try:
                self._wfm[path] = W.decode(self.src.read(path))
            except Exception as e:
                raise CoreError("%s: %s" % (path, e))
        return self._wfm[path]

    def count(self, path):
        return len(self.wfm(path).dialogues)

    def layout(self, path):
        """Event title layout (title_layout.Layout) of a CFNT file: where each letter goes, read from the overlays; None when the
        file is not a title font or the overlays are not on this source."""
        if not TL.is_title_file(path):
            return None
        if path not in self._layouts:
            try:
                self._layouts[path] = TL.load_layout(self.src.read_any, self.src.all_names(), path, self.wfm(path))
            except Exception:       # noqa: BLE001  no overlays (folder with only WFM files): titles without spacing
                self._layouts[path] = None
        return self._layouts[path]

    def original(self, path, idx):
        key = (path, idx)
        if key not in self._orig:
            w = self.wfm(path)
            d = w.dialogues[idx]
            lay = self.layout(path)
            lines = lay.lines(idx) if lay else None
            nvis = sum(map(len, lines)) if lines else 0
            if lines and (nvis == len(d.words) or (0 < nvis < len(d.words) and len(lay.recs[idx]) == len(d.words))):
                toks = [W.words_to_text(w, [v], d.font) for v in d.words[:nvis]]     # hidden surplus tiles: padding
                self._orig[key] = TL.lines_to_text(toks, lines)
            else:
                words = d.words
                b = self.banner(path, idx)
                if b is not None and 0 < b.get("shown", len(words)) < len(words):
                    words = words[:b["shown"]]      # a shorter banner is padded to K words; the hidden tiles are not text
                self._orig[key] = W.words_to_text(w, words, d.font)
        return self._orig[key]

    def title_lines(self, path, idx, text):
        """Lines of an event title as the text says ([[gap before each letter]]), or None if the dialogue is not a title."""
        lay = self.layout(path)
        if lay is None or idx not in lay.recs:
            if TL.is_title_file(path):
                return self._single_line(text)      # no layout list (CFNT999 #0, #1, ...): one centred line, spaces as gaps
            return None
        return self._effective_lines(lay, idx, text)

    @staticmethod
    def _single_line(text):
        """One centred line for a title without layout list; {NL} counts as a space, so the result is always a single line."""
        lines = TL.text_to_lines(text)[0]
        flat = []
        for l in lines:
            flat += [1 if flat and j == 0 else g for j, g in enumerate(l)]
        return [flat] if flat else []

    @staticmethod
    def _effective_lines(lay, idx, text):
        """Lines the text asks for; a text without any space or {NL} (projects saved before the layout was known) keeps the original."""
        lines = TL.text_to_lines(text)[0]
        orig = lay.lines(idx)
        if orig and not re.search(r" |\{NL\}", text) and (len(orig) > 1 or any(g for l in orig for g in l)):
            n = sum(map(len, lines))
            if n == sum(map(len, orig)):
                return orig
        return lines

    def banner(self, path, idx):
        """Banner tables (title_layout.banner_info) when (path, idx) is dialogue 0 of a CFNT999 file (the "completed" banner), else None."""
        if idx != 0 or not TL.is_title_file(path):
            return None
        lang = lang_of(path)
        key = ("banner", lang)
        if key not in self._layouts:
            try:
                self._layouts[key] = TL.banner_info(self.src.read_any, lang)
            except Exception:       # noqa: BLE001  no executables on this source
                self._layouts[key] = None
        return self._layouts[key]

    def title_problem(self, path, idx, text):
        """Readable problem of an event title text (the number of letters is fixed by the layout), or None."""
        lay = self.layout(path)
        if lay is None or idx not in lay.recs:
            b = self.banner(path, idx)
            if b is None:
                return None
            n = TL.text_to_lines(text)[1]
            if n > b["k"]:
                return tr("tc.titlelen", n, b["k"])
            return tr("tc.titleempty") if n == 0 else None
        n = TL.text_to_lines(text)[1]
        want = len(lay.recs[idx])
        if n > want:
            return tr("tc.titlelen", n, want)
        if n == 0:
            return tr("tc.titleempty")
        return None

    def title_max(self, path, idx):
        """Most letters the title can have (the size of its layout list), or None when it has no list."""
        lay = self.layout(path)
        if lay is None or idx not in lay.recs:
            b = self.banner(path, idx)
            return b["k"] if b else None
        return len(lay.recs[idx])

    def title_hidden(self, path, idx, text):
        """Surplus tiles of a title shorter than its layout list (they are put off screen)."""
        want = self.title_max(path, idx)
        return 0 if want is None else max(0, want - TL.text_to_lines(text)[1])

    def text(self, path, idx):
        return self.edits.get(path, {}).get(idx, self.original(path, idx))

    def edited(self, path, idx):
        return idx in self.edits.get(path, {})

    def set_text(self, path, idx, text):
        if norm(text) == norm(self.original(path, idx)):
            e = self.edits.get(path)
            if e and idx in e:
                del e[idx]
                if not e:
                    del self.edits[path]
        else:
            self.edits.setdefault(path, {})[idx] = text

    # linked copies: the same file name exists in several areas (CFNT999S.WFM x19 ...) with identical texts
    def copies(self, path, idx):
        """Paths (the file itself included) with the same file name, same index and same original text, in the selected languages."""
        base = os.path.basename(path).upper()
        orig = self.original(path, idx)
        out = []
        for n in self.src.names():
            if os.path.basename(n).upper() == base and self.selected(n):
                try:
                    if idx < self.count(n) and (n == path or self.original(n, idx) == orig):
                        out.append(n)
                except CoreError:
                    pass
        return out if path in out else [path]

    def set_text_linked(self, path, idx, text):
        """set_text on the file and, when self.link, on all its copies. Returns the paths changed."""
        paths = self.copies(path, idx) if self.link else [path]
        for n in paths:
            self.set_text(n, idx, text)
        return paths

    # find and replace
    @staticmethod
    def _finder(find, case, word, tags):
        pat = re.escape(find)
        if word:
            pat = r"(?<!\w)" + pat + r"(?!\w)"
        rx = re.compile(pat, 0 if case else re.I)

        def sub(text, repl):
            if tags:
                return rx.sub(lambda m: repl, text)
            parts = re.split(r"(\{[^}]*\})", text)       # tags {..} are left alone
            return "".join(x if x.startswith("{") and x.endswith("}") else rx.sub(lambda m: repl, x) for x in parts)
        return sub

    def replace_all(self, find, repl, case=False, word=False, tags=False, only=None, all_langs=False):
        """Find and replace over the texts (current text of every entry). only = [(path, idx)] limits it. Applies what is valid and
        remembers the previous state for undo_replace. Returns (entries changed, [problems])."""
        if not find:
            return 0, []
        sub = self._finder(find, case, word, tags)
        todo, problems, before = [], [], {}
        names = [n for n in self.src.names() if all_langs or self.selected(n)]
        keys = only if only is not None else [(n, i) for n in names for i in range(self.count(n))]
        for n, i in keys:
            old = self.text(n, i)
            new = sub(old, repl)
            if new == old:
                continue
            prob = self.validate(n, i, new)
            if prob:
                problems.append("%s #%d: %s" % (n, i, prob))
                continue
            todo.append((n, i, new))
        for n, i, new in todo:
            if all_langs and not self.selected(n):
                self.langs.add(lang_of(n))
            before[(n, i)] = self.edits.get(n, {}).get(i)
            self.set_text(n, i, new)
        if todo:
            self.undo_stack.append(before)
        return len(todo), problems

    def undo_replace(self):
        """Reverts the last replace_all; returns the number of entries restored (0 = nothing to undo)."""
        if not self.undo_stack:
            return 0
        for (n, i), t in self.undo_stack.pop().items():
            if t is None:
                e = self.edits.get(n)
                if e and i in e:
                    del e[i]
                    if not e:
                        del self.edits[n]
            else:
                self.edits.setdefault(n, {})[i] = t
        return 1

    def n_edits(self):
        return sum(len(v) for v in self.edits.values())

    # glyph pool (characters a file lacks are borrowed from other files)
    def pool(self):
        if self._pool is None:
            self._pool = W.GlyphPool()
            for n in self.src.names():
                try:
                    self._pool.add(self.wfm(n))     # every language: a 16 px 'W' may only exist in another one
                except CoreError:
                    pass
        return self._pool

    def words(self, path, idx, text):
        """Encodes text for dialogue idx -> (words, clone of the wfm that has the glyphs it needs). Raises WfmError;
        characters the file lacks are borrowed from other files (UnknownGlyph only if no file has them)."""
        base = self.wfm(path)
        font = base.dialogues[idx].font
        wc = clone(base)
        words = W.text_to_words(wc, text, font, self.pool())
        W.shift_glyphs(wc, self.glyph_shift)
        return words, wc

    def validate(self, path, idx, text):
        try:
            self.words(path, idx, text)
        except W.WfmError as e:
            return str(e)
        return self.title_problem(path, idx, text)

    # search
    def search(self, query, edited_only=False):
        q = fold(query)
        res = []
        for n in self.src.names():
            if not self.selected(n):
                continue
            for i in range(self.count(n)):
                if edited_only and not self.edited(n, i):
                    continue
                if not q or q in fold(self.text(n, i)):
                    res.append((n, i))
        return res

    # project file
    def to_json(self):
        return {"version": PROJECT_VERSION, "disc": os.path.basename(self.src.path), "languages": sorted(self.langs),
                "link": self.link, "verbFirst": self.verb_first, **({"menus": self.menus} if self.menus else {}), "edits": {p: {str(i): t for i, t in sorted(e.items())} for p, e in sorted(self.edits.items())}}

    def load_edits(self, data, replace=True):
        """data: {"edits": {path: {idx: text}}} (project) or {path: {idx: text}} (export). Returns (applied, problems)."""
        ed = data.get("edits") if isinstance(data.get("edits"), dict) else data
        if replace:
            self.edits = {}
            if isinstance(data.get("link"), bool):
                self.link = data["link"]
            if isinstance(data.get("verbFirst"), bool):
                self.verb_first = data["verbFirst"]
            self.menus = dict(data["menus"]) if isinstance(data.get("menus"), dict) else {}
            if isinstance(data.get("languages"), list):
                self.langs = {str(l).upper() for l in data["languages"]} & set(self.languages()) or {LANG_MAIN}
        applied, problems = 0, []
        for path, d in ed.items():
            if path not in self.src.files:
                problems.append(tr("tc.nofilein", path))
                continue
            if not self.selected(path):
                problems.append(tr("tc.langskip", path))
                continue
            if not isinstance(d, dict):
                continue
            for k, t in d.items():
                try:
                    i = int(k)
                    if not 0 <= i < self.count(path):
                        raise ValueError
                except ValueError:
                    problems.append(tr("tc.nodlg", path, k))
                    continue
                before = self.text(path, i)
                self.set_text(path, i, t)
                applied += norm(before) != norm(t)
        return applied, problems

    def export_json(self):
        out = {}
        for n in self.src.names():
            if self.selected(n):
                out[n] = {str(i): self.text(n, i) for i in range(self.count(n))}
        return out

    def export_txt(self):
        lines = ["# Tombi! textos. Cada diálogo empieza con '=== ARCHIVO #n ==='; los saltos de línea reales se ignoran,",
                 "# usa {NL} {CLEAR} {WAIT} ... (ver ayuda del editor).", ""]
        for n in self.src.names():
            if self.selected(n):
                for i in range(self.count(n)):
                    lines.append("=== %s #%d ===" % (n, i))
                    lines.append(self.text(n, i).rstrip("\n"))
                    lines.append("")
        return "\n".join(lines)

    def import_txt(self, txt):
        data, cur = {}, None
        for ln in txt.splitlines():
            m = re.fullmatch(r"=== (\S+) #(\d+) ===", ln)
            if m:
                cur = (m.group(1), m.group(2))
                data.setdefault(cur[0], {})[cur[1]] = []
            elif cur is not None:
                data[cur[0]][cur[1]].append(ln)
        out = {p: {i: "\n".join(l).strip("\n") for i, l in d.items()} for p, d in data.items()}
        return self.load_edits(out, replace=False)

    # encoding
    def encode_edited(self):
        """{path: bytes} for every edited file, and a list of readable errors (nothing is returned if there are any)."""
        files, errors = {}, []
        for path, e in sorted(self.edits.items()):
            if not self.selected(path):
                continue
            w = clone(self.wfm(path))
            for idx, text in sorted(e.items()):
                try:
                    d = w.dialogues[idx]
                    words = W.text_to_words(w, text, d.font, self.pool())
                    lay = self.layout(path)
                    if lay is not None and idx in lay.recs and words and len(words) < len(lay.recs[idx]):
                        # shorter title: the engine reads one glyph per record of the list, so pad (the extra tiles are off screen)
                        words = words + [words[0]] * (len(lay.recs[idx]) - len(words))
                    if idx == 0 and words and (lay is None or idx not in lay.recs) and self.banner(path, 0) is not None and len(words) < self.banner(path, 0)["k"]:
                        # shorter "completed" banner: keep dialogue 0 at K glyph words like the original (a shorter dialogue 0
                        # left the native port on NOW LOADING); _banner_patches only shows the first letters, the rest go off screen
                        words = words + [words[0]] * (self.banner(path, 0)["k"] - len(words))
                    d.words = words
                except W.WfmError as ex:
                    errors.append("%s #%d: %s" % (path, idx, ex))
            if not errors:
                try:
                    W.shift_glyphs(w, self.glyph_shift)
                    files[path] = W.encode(w)
                except W.WfmError as ex:
                    errors.append("%s: %s" % (path, ex))
        self._title_patches(files, errors)
        self._banner_patches(files, errors)
        self._verb_patches(files, errors)
        self._menu_patches(files, errors)
        if not errors:
            # the area load lists keep their own size of every file: a WFM that changed size without its entry being
            # updated gets its tail overwritten by the next file in memory (tools/load_lists.py)
            import load_lists
            files.update(load_lists.fix(self.src.read_any, self.src.all_names(), files))
        return ({} if errors else files), errors

    def _menu_patches(self, files, errors):
        """Title screen / options menu texts (not WFM: glyph strings in SYSTEM/OnPTSUB0.BIN, tools/menu_text.py) and the
        memory card screen's (SYSTEM/DnSPSUB2.BIN, tools/memcard_text.py)."""
        for path, spec in sorted(self.menus.items()):
            mod = MC if re.search(r"D\dSPSUB2\.BIN$", path, re.I) else MT
            try:
                data = files[path] if path in files else self.src.read_any(path)
                new = mod.apply(data, spec)
            except (OSError, KeyError):
                errors.append(tr("tc.nofilein", path))
                continue
            except (MT.MenuError, MC.MemcardError) as ex:
                errors.append("%s: %s" % (path, ex))
                continue
            if new != data:
                files[path] = new

    def _title_patches(self, files, errors):
        """Event titles: the spacing and line breaks of the edited titles go into the overlays that hold the layout (same number of
        records, so nothing moves). Adds the changed overlays to files."""
        new = {}                   # language letter -> (wfm, {dialogue: records})
        for path, e in sorted(self.edits.items()):
            lay = self.layout(path) if self.selected(path) else None
            if lay is None:
                continue
            lang = lang_of(path)
            for idx, text in sorted(e.items()):
                if idx not in lay.recs:
                    continue
                prob = self.title_problem(path, idx, text)
                if prob:
                    errors.append("%s #%d: %s" % (path, idx, prob))
                    continue
                lines = self._effective_lines(lay, idx, text)
                if lines == lay.lines(idx):
                    continue                    # same spacing as the original: nothing to write
                old = lay.recs[idx]
                recs = TL.pad_records(TL.lines_to_records(lines, [r[2] for r in old]), old)
                new.setdefault(lang, (self.wfm(path), {}, {}))[2].update(lay.recs)
                prev = new[lang][1]
                if idx in prev and prev[idx] != recs:
                    errors.append("%s #%d: %s" % (path, idx, tr("tc.titleconflict")))
                prev[idx] = recs
        if errors:
            return
        for lang, (w, recs, ref) in new.items():
            try:
                files.update(TL.patch_files(self.src.read_any, self.src.all_names(), w, lang, recs, ref))
            except (OSError, KeyError) as ex:
                errors.append("%s: %s" % (lang, ex))


    def verb_deltas(self, lang):
        """(d0, d1): how many px wider (negative: narrower) the verb boxes of a language got with the edits. printInfoMessage adds a
        constant to the item name width (`W + 0x60` obtained = dialogue 2, `+ 0x58` used / equipped = dialogues 0 / 1); the box of a
        dialogue is its widest line + 16, and the constant must follow the widest verb over the language's MFNT files."""
        import text_preview
        old, new = {0: [], 1: []}, {0: [], 1: []}
        for path in self.src.names():
            if lang_of(path) != lang or not os.path.basename(path).startswith("MFNT"):
                continue
            w = self.wfm(path)
            if len(w.dialogues) < 3:
                continue
            for i in (0, 1, 2):
                k = 0 if i == 2 else 1
                o = text_preview.fit_box(w, w.dialogues[i].words)[0]
                n = o
                if i in self.edits.get(path, {}):
                    try:
                        words, wc = self.words(path, i, self.edits[path][i])
                        n = text_preview.fit_box(wc, words)[0]
                    except W.WfmError:
                        pass                    # reported by encode_edited
                old[k].append(o)
                new[k].append(n)
        return tuple(max(new[k]) - max(old[k]) if old[k] else 0 for k in (0, 1))

    def _verb_patches(self, files, errors):
        """Item popup ("Rana Ganada"): with verb_first the executable of every selected language draws the verb text (dialogue 2 of
        the MFNT file) before the item name, like "Usado(a) Rana" already does (tools/verb_order.py, same size). Whenever the verb
        dialogues 0 / 1 / 2 changed width, the width constants of the box follow. Adds the executables."""
        for lang in sorted(self.langs):
            if lang not in TL.LANG_N:
                continue
            d0, d1 = self.verb_deltas(lang)
            if not self.verb_first and not d0 and not d1:
                continue
            name = TL.exe_name(lang)
            try:
                data = files[name] if name in files else self.src.read_any(name)
            except (OSError, KeyError):
                continue                    # no executable of this language on the source
            new = VO.patch(data) if self.verb_first else data
            if new is not None and (d0 or d1):
                new = VO.adjust(new, d0, d1)
            if new is None:
                errors.append(tr("tc.verbnf", name))
            elif new != data:
                files[name] = new

    def _banner_patches(self, files, errors):
        """Dialogue 0 of CFNT999 (the banner shown when an event is completed): the engine takes its glyphs and positions from tables of
        EXE/MAINn.EXE, not from the text, so they are rewritten to match the edited text. Adds the changed executables to files."""
        for path, e in sorted(self.edits.items()):
            if 0 not in e or not self.selected(path) or self.banner(path, 0) is None:
                continue
            text = e[0]
            prob = self.title_problem(path, 0, text)
            if prob:
                errors.append("%s #0: %s" % (path, prob))
                continue
            lang = lang_of(path)
            if path not in files:
                continue
            words = W.decode(files[path]).dialogues[0].words        # the words as stored in the encoded file
            gaps = [g for l in self._single_line(text) for g in l]
            words = words[:len(gaps)]                               # drop the padding words (encode_edited)
            if len(gaps) != len(words):
                errors.append("%s #0: %s" % (path, tr("tc.titleempty")))
                continue
            # the glyph ids of the edited WFM (files[path]) are the ones the tables must point to; every language has one executable
            try:
                new = TL.banner_patch(self.src.read_any, lang, words, gaps)
            except (OSError, KeyError) as ex:
                errors.append("%s: %s" % (lang, ex))
                continue
            files.update(new)


def clone(w):
    c = W.Wfm()
    c.__dict__.update(w.__dict__)
    c.glyphs = list(w.glyphs)
    c.charidx = {k: list(v) for k, v in w.charidx.items()}
    c.dialogues = [W.Dialogue(d.words, d.term, d.font) for d in w.dialogues]
    return c


# ---- disc build ----------------------------------------------------------------------------------------------------

def have_mkpsxiso():
    import mkdisc
    try:
        mkdisc.tool("mkpsxiso")
        mkdisc.tool("dumpsxiso")
        return True
    except SystemExit:
        return False


def build_disc(proj, out_bin, work, disc=None, log=print):
    """Writes the edited WFM files and builds out_bin (+ .cue) with tools/mkdisc.py --relayout. Returns the list of files.
    See build_report for the details (output path, size, verification)."""
    return build_report(proj, out_bin, work, disc, log)["files"]


def build_report(proj, out_bin, work, disc=None, log=print):
    """Like build_disc but returns {"files", "out", "cue", "size", "verified"}. The image is built under a temporary name next to
    out_bin, read back (every written file must be on it byte for byte) and only then moved onto out_bin, so an old or locked
    out_bin can never be silently kept, and out_bin may be the image that was opened."""
    import mkdisc
    files, errors = proj.encode_edited()
    if errors:
        raise CoreError(tr("tc.cantbuild") + "\n" + "\n".join(errors[:20]))
    if not files:
        raise CoreError(tr("tc.nochanges"))
    disc = disc or proj.src.image
    if not disc or not os.path.isfile(disc):
        raise CoreError(tr("tc.needimage"))
    out_bin = os.path.abspath(out_bin)
    work = os.path.abspath(work)
    fs = os.path.join(work, "fs")
    shutil.rmtree(fs, ignore_errors=True)
    for name, data in files.items():
        p = os.path.join(fs, *name.split("/"))
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "wb").write(data)
    src = Source(disc)
    for name in sorted(src.layout):
        if re.fullmatch(r"EXE/MAIN\d\.EXE", name) and name not in files:
            p = os.path.join(fs, *name.split("/"))
            os.makedirs(os.path.dirname(p), exist_ok=True)
            open(p, "wb").write(src.read_any(name))
    # build inside the work folder (mkpsxiso already writes there; it could not create "~build-N.bin" next to the output on
    # Windows: "Cannot open or create output image file"), then move the checked image onto out_bin
    # mkpsxiso.exe could not create its image under the user's Desktop either (Windows protected folders block unknown
    # programs; the editor itself may write there): build in the system temp folder, then move the image with Python
    tmpdir = tempfile.mkdtemp(prefix="tombi-iso-")
    tmp_bin = os.path.join(tmpdir, "build.bin")
    tmp_cue = os.path.splitext(tmp_bin)[0] + ".cue"
    tmp_lay = os.path.splitext(tmp_bin)[0] + ".layout.json"
    ns = argparse.Namespace(fs=fs, disc=disc, work=os.path.join(work, "mkdisc"), out=tmp_bin, relayout=True)
    buf = io.StringIO()
    try:
        try:
            with contextlib.redirect_stdout(buf), contextlib.redirect_stderr(buf):
                mkdisc.build(ns)
        except SystemExit as e:
            raise CoreError(tr("tc.mkfail", e, buf.getvalue()[-1500:]))
        log(next((l for l in buf.getvalue().splitlines() if l.startswith("built")), ""))
        # read back: the written files must be on the new image
        new = Source(tmp_bin)
        for name, data in files.items():
            if re.fullmatch(r"EXE/MAIN\d\.EXE", name):        # relayout rewrites the file tables of the executables: compare the rest
                ok = name in new.layout and new.layout[name][1] == len(data)
                if ok:
                    f0, f1 = TL.banner_find(data), TL.banner_find(new.read_any(name))
                    ok = f0 is not None and f0 == f1 and new.read_any(name)[f0[0]:f0[2] + 4 * f0[1]] == data[f0[0]:f0[2] + 4 * f0[1]]
                    if ok:
                        r = slice(VO.FUNC, VO.FUNC + VO.FUNC_LEN)
                        ok = new.read_any(name)[r] == data[r]
                if not ok:
                    raise CoreError(tr("tc.verify", name))
                continue
            if name not in new.layout or new.layout[name][1] != len(data) or new.read_any(name) != data:
                raise CoreError(tr("tc.verify", name))
        try:
            part = out_bin + ".part"
            shutil.move(tmp_bin, part)             # may cross drives (work folder vs output), unlike os.replace
            os.replace(part, out_bin)              # same folder: atomic, the old image stays until the new one is complete
        except OSError as e:
            raise CoreError(tr("tc.cantwrite", out_bin, e))
    finally:
        shutil.rmtree(tmpdir, ignore_errors=True)
        for t in (tmp_bin, tmp_cue, tmp_lay, out_bin + ".part"):
            if os.path.exists(t):
                try:
                    os.remove(t)
                except OSError:
                    pass
    cue = os.path.splitext(out_bin)[0] + ".cue"
    try:
        open(cue, "w", newline="\r\n").write('FILE "%s" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n' % os.path.basename(out_bin))
    except OSError:
        cue = None
    return {"files": sorted(files), "out": out_bin, "cue": cue, "size": os.path.getsize(out_bin), "verified": True}


def write_patched(proj, out_dir):
    files, errors = proj.encode_edited()
    if errors:
        raise CoreError("\n".join(errors[:20]))
    for name, data in files.items():
        p = os.path.join(out_dir, *name.split("/"))
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "wb").write(data)
    return sorted(files)


# ---- command line --------------------------------------------------------------------------------------------------

def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("render")
    r.add_argument("src"); r.add_argument("path"); r.add_argument("idx", type=int)
    r.add_argument("-o", "--out", default="preview.png"); r.add_argument("--scale", type=int, default=2)
    r.add_argument("--text", help="render this text instead of the dialogue's own")
    e = sub.add_parser("export")
    e.add_argument("src"); e.add_argument("out"); e.add_argument("--all-languages", action="store_true"); e.add_argument("--languages", help="letters, e.g. SB")
    b = sub.add_parser("build")
    b.add_argument("src"); b.add_argument("project"); b.add_argument("-o", "--out", required=True)
    b.add_argument("--work"); b.add_argument("--disc")
    b.add_argument("--verb-first", action="store_true", help='item popup order "Obtienes: Rana" (patches EXE/MAINn.EXE, tools/verb_order.py)')
    a = p.parse_args()
    proj = Project(Source(a.src))
    if a.cmd == "export":
        if a.all_languages:
            proj.langs = set(proj.languages())
        elif a.languages:
            proj.langs = set(a.languages.upper())
    if a.cmd == "render":
        import text_preview
        text = a.text if a.text is not None else proj.text(a.path, a.idx)
        words, wc = proj.words(a.path, a.idx, text)
        im, warns = text_preview.render(wc, words, scale=a.scale, title=proj.title_lines(a.path, a.idx, text))
        prob = proj.title_problem(a.path, a.idx, text)
        if prob:
            warns.append(prob)
        im.save(a.out)
        for w in warns:
            print(w)
        print(proj.original(a.path, a.idx))
    elif a.cmd == "export":
        if a.out.lower().endswith(".json"):
            json.dump(proj.export_json(), open(a.out, "w", encoding="utf-8"), ensure_ascii=False, indent=1)
        else:
            open(a.out, "w", encoding="utf-8").write(proj.export_txt())
    else:
        if a.project.lower().endswith(".txt"):
            ap, pr = proj.import_txt(open(a.project, encoding="utf-8").read())
        else:
            ap, pr = proj.load_edits(json.load(open(a.project, encoding="utf-8")))
        proj.verb_first = proj.verb_first or a.verb_first
        print("applied %d edits; problems: %s" % (ap, pr))
        work = a.work or os.path.join(os.path.dirname(os.path.abspath(a.out)), "tombi-text-work")
        print("changed:", build_disc(proj, a.out, work, a.disc))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (CoreError, W.WfmError) as e:
        sys.exit("error: %s" % e)
