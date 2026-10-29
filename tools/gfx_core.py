#!/usr/bin/env python3
"""Tombi! (PAL) graphics editor, logic without any window (tools/gfx_editor.py is the GUI, exe tombi-gfx.exe).

Opens a disc image (or the extracted fs folder), lists every editable graphic (static screens, the UiSpriteDef table of MAIN0, the GRPX pages
of the load lists, WPP sprite frames, CLUT palettes), decodes pages with the right palette, imports/exports indexed PNG, edits palette
entries, keeps all edits in memory (undo), writes/reads mods (mods/<name>/mod.json + PNGs, the format of tools/mod.py) and builds a disc.

  python3 tools/gfx_core.py list [DISC|FS]            the tree
  python3 tools/gfx_core.py render DISC ITEM OUT.png  ITEM = a name printed by `list` (e.g. screen:title_landscape, ui:1, page:SYSTEM/A10003.GAM)

Edits are bytes of the *unpacked* resource (GAM files are repacked with tools/gam_py.py when a mod/disc is written).  A 4 bpp page stores
palette indices only; the palette is a separate CLUT record (PalRef = file, first word, colour count) that the sprite/screen points at.
UiSpriteDef (MAIN0 0x80078808): the table is `{u16 u, v, w, h; s16 clutX, clutY}` per entry; it holds only 3 plausible entries
(0 = unused placeholder, 1 = TRY, 2 = AGAIN), the bytes after them are other data."""
import contextlib, fnmatch, io, json, os, re, shutil, struct, sys, tempfile
from array import array

HERE = os.path.dirname(os.path.abspath(__file__))
BUNDLE = getattr(sys, "_MEIPASS", HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import i18n
from i18n import tr

UI_TABLE_VADDR = 0x80078808
UI_PAGE = "SYSTEM/A10003.GAM"                      # page the TRY / AGAIN sprites are cut from (tpage 0x14, resident SYSTEM list 0)
UI_SCREEN_POS = {1: (104, 96), 2: (160, 96)}      # FUN_800212b4 ("drawNowLoading", really the TRY AGAIN prompt): screen position of the sprites
UI_NAMES = {0: "UI 0", 1: "TRY", 2: "AGAIN"}


class CoreError(Exception):
    pass


def err(key, *a):
    return CoreError(tr(key, *a))


# ------------------------------------------------------------------ disc / file system
class Disc:
    """A disc image (files are extracted on demand into a cache folder) or an extracted fs folder."""

    def __init__(self, path, cache=None):
        import cdfiles
        path = os.path.abspath(path)
        self.path, self.image, self.layout, self.tmp = path, None, None, None
        if os.path.isdir(path):
            self.fs = path if os.path.isdir(os.path.join(path, "AREA00")) or not os.path.isdir(os.path.join(path, "fs")) else os.path.join(path, "fs")
            sib = os.path.join(os.path.dirname(self.fs), "Tombi.bin")
            if os.path.isfile(sib):
                self.image = sib
        elif os.path.isfile(path):
            with open(path, "rb") as f:
                f.seek(16 * 2352 + 25)
                raw = f.read(5) == b"CD001"
                f.seek(16 * 2048 + 1)
                cooked = f.read(5) == b"CD001"
            if not (raw or cooked):
                raise err("c.notiso", path)
            self.sector, self.off = (2352, 24) if raw else (2048, 0)
            self.image = path
            self.layout = cdfiles.layout_from_image(path)
            self.tmp = cache or tempfile.mkdtemp(prefix="tombi-gfx-")
            self.fs = os.path.join(self.tmp, "fs")
            os.makedirs(self.fs, exist_ok=True)
            for n in sorted(self.layout):
                if n.startswith(("SYS/", "EXE/")):
                    self.ensure(n)
        else:
            raise err("c.nofile", path)
        if not os.path.isdir(os.path.join(self.fs, "SYS")):
            raise err("c.nofs", self.fs)
        self._env()

    def ensure(self, name):
        """Extracts one file of the image into the cache (no-op for a folder)."""
        p = os.path.join(self.fs, *name.split("/"))
        if os.path.isfile(p) or self.layout is None or name not in self.layout:
            return p
        lba, size = self.layout[name]
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(self.image, "rb") as f, open(p + ".part", "wb") as o:
            left = size
            for i in range((size + 2047) // 2048):
                f.seek((lba + i) * self.sector + self.off)
                o.write(f.read(2048)[:left])
                left -= 2048
        os.replace(p + ".part", p)
        return p

    def raw(self, name):
        """The file as stored on the disc (GAM still packed)."""
        return open(self.ensure(name), "rb").read()

    def _env(self):
        os.environ["TOMBI_FS"] = self.fs
        if self.image:
            os.environ["TOMBI_IMG"] = self.image
        import area000 as A, vram
        A.FS = self.fs
        A.IMG = self.image or ""
        A._LISTS = A._REC = None
        A._BLOCKS.clear()
        vram._RES.clear()
        vram._CTX.clear()
        orig = getattr(A, "_orig_read_resource", None) or A.read_resource
        A._orig_read_resource = orig

        def read_resource(name, fs=None):
            for ext in ("", ".GAM", ".000"):
                if os.path.isfile(os.path.join(A.FS, name + ext)) or (self.layout and (name + ext) in self.layout):
                    self.ensure(name + ext)
                    break
            return orig(name, A.FS)
        A.read_resource = read_resource
        # load_lists / load_records bind FS as a default argument at import time: call them with the current folder
        for fn in ("load_lists", "load_records"):
            o = getattr(A, "_orig_" + fn, None) or getattr(A, fn)
            setattr(A, "_orig_" + fn, o)
            setattr(A, fn, (lambda o: (lambda fs=None: o(A.FS)))(o))

    def close(self):
        if self.tmp and os.path.isdir(self.tmp):
            shutil.rmtree(self.tmp, ignore_errors=True)


def hashlib_md5(words):
    import hashlib
    return hashlib.md5(struct.pack("<%dH" % len(words), *words)).hexdigest()[:8]


# ------------------------------------------------------------------ pixel formats
def decode_plane(data, w_words, h, bpp):
    """Index plane (bytearray, one byte per pixel; 16 bpp: array('H') of words) of a page of w_words x h words."""
    if bpp == 8:
        return bytearray(data[:w_words * h * 2])
    if bpp == 4:
        raw = bytes(data[:w_words * h * 2])
        n = bytearray(len(raw) * 2)
        n[0::2] = raw.translate(bytes(i & 15 for i in range(256)))
        n[1::2] = raw.translate(bytes(i >> 4 for i in range(256)))
        return n
    a = array("H")
    a.frombytes(bytes(data[:w_words * h * 2]))
    return a


def encode_plane(plane, bpp):
    if bpp == 8:
        return bytes(plane)
    if bpp == 4:
        return bytes(a | b << 4 for a, b in zip(plane[0::2], plane[1::2]))
    return plane.tobytes()


def rgb_to_word(r, g, b):
    return (r * 31 + 127) // 255 | ((g * 31 + 127) // 255) << 5 | ((b * 31 + 127) // 255) << 10


def word_to_rgb(w):
    import vram
    return tuple(vram.RGB[w & 0x7FFF])


# ------------------------------------------------------------------ palettes
class PalRef:
    """n colours at word `off` of the CLUT file `file`."""
    __slots__ = ("file", "off", "n")

    def __init__(self, file, off, n):
        self.file, self.off, self.n = file, off, n

    def key(self):
        return (self.file, self.off, self.n)

    def __eq__(self, o):
        return isinstance(o, PalRef) and self.key() == o.key()

    def __hash__(self):
        return hash(self.key())

    def __repr__(self):
        return "PalRef(%r, %d, %d)" % self.key()


class Rect:
    def __init__(self, label, x, y, w, h, pal=None, group="", item=None):
        self.label, self.x, self.y, self.w, self.h, self.pal, self.group, self.item = label, x, y, w, h, pal, group, item


# ------------------------------------------------------------------ screens (tools/screens.py)
def screens_list():
    import screens
    out = []
    for name, pages, clutf, crop, opaque in screens.STATIC:
        out.append(dict(name=name, pages=pages, clut=clutf, crop=crop, opaque=opaque, bpp=8))
    return out


EXTRA_SCREENS = [   # (name, page, load list, index, palette PalRef args (file, first word, count), crop)
    # NOW LOADING: A00900 (SYS list 9, 256 x 72, 4 bpp) holds its own palettes in its last row (y 71: 3 x 16 words at 16, 32, 48); UiSpriteDef 0 (w112 h71) names CLUT (16,71) = the first one (ground truth); the TRY AGAIN CLUT gives the same letters but muddy pigs/block
    ("now_loading", "SYSTEM/A00900.000", "SYS", 9, ("SYSTEM/A00900.000", 71 * 64 + 16, 16), (0, 0, 256, 64)),
    # TRY AGAIN: the two UiSpriteDef sprites (184,192 40x16 and 184,208 56x16) of the HUD page; CLUT (352,509) = row 29 of the area CLUT strips
    ("try_again", "SYSTEM/A10003.GAM", "SYS", 0, ("AREA00/CLUT01.GAM", 29 * 256 + 224, 16), (184, 192, 56, 32)),
]


# ------------------------------------------------------------------ the project
class Core:
    def __init__(self, path, cache=None):
        self.disc = Disc(path, cache)
        import area000 as A, vram
        self.A, self.vram = A, vram
        self.edits = {}            # resource name (or "<wpp file>#<frame>") -> bytes
        self.undo_stack = []       # [{key: previous bytes or None}]
        self.version = 0
        self.palette_fills = {}    # PalRef key -> {index: estimated word} for colours missing from a backdrop CLUT
        self.pal_choice = {}       # page name -> PalRef last used (saved with the mod)
        self.lists = A.load_lists()
        if not self.lists:
            raise err("c.nolists")
        self.truth()
        self._cl = None
        self._wpp = {}
        self._cap_pals = {}        # read-only palettes seen in the port capture that no CLUT file holds (set at run time)

    # ---- data
    def original(self, key):
        if "#" in key:
            name, n = key.split("#")
            return self.wpp(name)[2]["frame_%04d.4bpp" % int(n)]
        d = self.vram.resource(key)
        if d is None:
            raise err("c.nores", key)
        return d

    def get(self, key):
        return self.edits.get(key) if key in self.edits else self.original(key)

    def edit(self, changes):
        """Applies {key: new bytes} as one undo step."""
        prev, ch = {}, {}
        for k, v in changes.items():
            v = bytes(v)
            old = self.get(k)
            if len(v) != len(old):
                raise err("c.size", k, len(v), len(old))
            if v != old:
                prev[k] = self.edits.get(k)
                ch[k] = v
        if not ch:
            return False
        for k, v in ch.items():
            if v == self.original(k):
                self.edits.pop(k, None)
            else:
                self.edits[k] = v
        self.undo_stack.append(prev)
        self.version += 1
        return True

    def undo(self):
        if not self.undo_stack:
            return False
        for k, v in self.undo_stack.pop().items():
            if v is None:
                self.edits.pop(k, None)
            else:
                self.edits[k] = v
        self.version += 1
        return True

    def revert(self, key):
        if key in self.edits:
            self.edit({key: self.original(key)})

    def edited(self):
        return sorted(self.edits)

    # ---- load list catalogue
    def area_name(self, ld):
        return "SYSTEM" if ld == "SYS" else "AREA" + ld[2:]

    def sys_list_no(self, lst):
        if getattr(self, "_sysno", None) is None:
            self._sysno = {id(l): k for k, l in enumerate(self.lists["SYS"])}
        return self._sysno.get(id(lst))

    def backdrop(self, k):
        import palinfer
        return palinfer.backdrop(self, k)

    def grpx(self, lst):
        pr = self.vram.pair_cluts(lst)
        out = []
        k = self.sys_list_no(lst)
        bd = self.backdrop(k) if k is not None and k >= 9 else None
        for i, r in enumerate(lst):
            if self.vram.is_grpx(r):
                bpp = 16 if self.vram.is_clut_rec(r) else (8 if (i in pr or bd) else 4)
                out.append((i, r, bpp, lst[pr[i]] if i in pr else None))
        return out

    def page_info(self, name, ld=None, idx=None):
        """dict(name, w, h, x, y, bpp, clut record, ld, idx) of a GRPX record (first list that names it, or the given one)."""
        for l, lists in self.lists.items():
            if ld not in (None, l):
                continue
            for k, lst in enumerate(lists):
                if idx not in (None, k):
                    continue
                for i, r, bpp, cl in self.grpx(lst):
                    if r["name"] == name:
                        return dict(name=name, w=r["w"], h=r["h"], x=r["arg"] & 0xFFFF, y=r["arg"] >> 16, bpp=bpp, clut=cl, ld=l, idx=k)
        raise err("c.nopage", name)

    def clut_users(self, name):
        """([(ld, idx)], [page names]) of the load lists that load the CLUT file `name`, and the pages that use it."""
        if self._cl is None:
            self._cl = {}
            for ld, lists in self.lists.items():
                for k, lst in enumerate(lists):
                    gr = self.grpx(lst)
                    for i, r, bpp, cl in gr:
                        if bpp == 16:
                            e = self._cl.setdefault(r["name"], ([], set()))
                            e[0].append((ld, k))
                            for j, r2, b2, c2 in gr:
                                if b2 == 8 and c2 is not None and c2["name"] == r["name"]:
                                    e[1].add(r2["name"])
                                elif b2 == 4 and r["w"] != 256:
                                    e[1].add(r2["name"])
                                elif b2 == 4 and r["w"] == 256 and r["h"] > 1:
                                    e[1].add(r2["name"])
        l, p = self._cl.get(name, ([], set()))
        return l, sorted(p)

    # ---- palettes
    def pal_words(self, ref):
        if ref.file.startswith("capture:"):
            return list(self._cap_pals.get(ref.file, [0] * ref.n))
        d = self.get(ref.file)
        n = min(ref.n, max(0, len(d) // 2 - ref.off))
        ws = list(struct.unpack_from("<%dH" % n, d, ref.off * 2)) + [0] * (ref.n - n)
        fill = self.palette_fills.get(ref.key())      # backdrop colours the CLUT file leaves empty (see palinfer.backdrop)
        if fill:
            for i, v in fill.items():
                if i < len(ws) and ws[i] == 0:
                    ws[i] = v
        return ws

    def pal_rgb(self, ref):
        import vram
        return [bytes(vram.RGB[w & 0x7FFF]) for w in self.pal_words(ref)]

    def set_color(self, ref, i, rgb):
        """Sets colour i of the palette from an 8 bit RGB; keeps the STP bit of the word it replaces (a PNG cannot carry it)."""
        words = self.pal_words(ref)
        old = words[i]
        v = rgb_to_word(*rgb) | (old & 0x8000)
        if v == 0:
            v = 0x8000
        return self.set_word(ref, i, v)

    def set_word(self, ref, i, v):
        if ref.file.startswith("capture:"):
            raise err("c.capro")
        d = bytearray(self.get(ref.file))
        struct.pack_into("<H", d, (ref.off + i) * 2, v & 0xFFFF)
        return self.edit({ref.file: d})

    def clut_lookup(self, x, y, n, order=None):
        """PalRef of the CLUT record that holds the VRAM colours (x, y) (x in words): searched in `order` ([(ld, idx)]) then everywhere."""
        seq = list(order or [])
        seq += [(ld, k) for ld in self.lists for k in range(len(self.lists[ld]))]
        for ld, k in seq:
            for i, r, bpp, cl in self.grpx(self.lists[ld][k]):
                if bpp == 16:
                    rx, ry = r["arg"] & 0xFFFF, r["arg"] >> 16
                    if ry <= y < ry + r["h"] and rx <= x and x + n <= rx + r["w"]:
                        return PalRef(r["name"], (y - ry) * r["w"] + (x - rx), n)
        return None

    def palette_candidates(self, bpp, ld, idx):
        """[(label, PalRef)] palettes of the CLUT records of the load list (+ area list 0 + resident SYSTEM list 0)."""
        order = [(ld, idx)]
        if ld != "SYS" and idx != 0:
            order.append((ld, 0))
        if ld != "SYS" or idx != 0:
            order.append(("SYS", 0))
        seen, out = set(), []
        recs = [(r, l, k) for l, k in order for i, r, b, cl in self.grpx(self.lists[l][k]) if b == 16]
        if bpp == 4:
            recs.sort(key=lambda t: t[0]["w"] == 256)      # 16 colour strips first, 256 colour blocks (rows of sub-palettes) after
        for r, l, k in recs:
            if True:
                if r["name"] in seen:
                    continue
                seen.add(r["name"])
                w, h = r["w"], r["h"]
                short = r["name"].rsplit("/", 1)[-1]
                if bpp == 8 and w == 256:
                    out += [(tr("pal.row", short, y), PalRef(r["name"], y * 256, 256)) for y in range(h)]
                elif bpp == 4:
                    per = w // 16
                    for y in range(h):
                        for s in range(per):
                            ref = PalRef(r["name"], y * w + s * 16, 16)
                            if any(self.pal_words(ref)):
                                out.append((tr("pal.sub", short, y, s) if per > 1 else tr("pal.row", short, y), ref))
        return out

    def _rows_of(self, rec, bpp):
        """[(label, PalRef)] of one CLUT record for a 4 / 8 bpp view (8 bpp: 256 colour rows; 4 bpp: non-empty 16 colour sub-palettes)."""
        w, h, name = rec["w"], rec["h"], rec["name"]
        short = name.rsplit("/", 1)[-1]
        out = []
        if bpp == 8:
            if w == 256:
                out = [(tr("pal.row", short, y), PalRef(name, y * 256, 256)) for y in range(h)]
        else:
            per = w // 16
            for y in range(h):
                for k in range(per):
                    ref = PalRef(name, y * w + k * 16, 16)
                    if any(self.pal_words(ref)):
                        out.append((tr("pal.sub", short, y, k) if per > 1 else tr("pal.row", short, y), ref))
        return out

    def palette_groups(self, s):
        """Every palette usable for the 4 / 8 bpp surface `s`: [(group label, [(label, PalRef)] or a callable returning [(file, [(label, PalRef)])])]:
        the inferred one, CLUT records of the list, palettes embedded in the page, CLUT rows resident in VRAM at that point,
        and (lazily) every CLUT file of the disc grouped by file."""
        bpp = s.bpp
        if bpp == 16:
            return []
        groups, seen = [], set()
        if s.inferred:
            groups.append((tr("pg.inferred"), [(tr("pal.inf", self._ref_label(s.inferred)), s.inferred)]))
            seen.add(s.inferred)

        def add(label, items):
            items = [(l, r) for l, r in items if r not in seen]
            seen.update(r for l, r in items)
            if items:
                groups.append((label, items))
        add(tr("pg.list"), self.palette_candidates(bpp, s.ld, s.idx))
        emb = []
        if bpp == 4 and s.layers:
            name = s.layers[0][0]
            try:
                p = self.page_info(name)
                d = self.get(name)
                w, h = p["w"], p["h"]
                if h >= 9 and not any(d[(h - 3) * w * 2:(h - 1) * w * 2]):
                    for k in range(w // 16):
                        ref = PalRef(name, (h - 1) * w + k * 16, 16)
                        if any(self.pal_words(ref)):
                            emb.append((tr("pal.emb", name.rsplit("/", 1)[-1], h - 1, k), ref))
            except Exception:
                pass
        add(tr("pg.embedded"), emb)
        res = []
        order = [("SYS", 0)]
        if s.ld != "SYS" and s.ld in self.lists:
            order += [(s.ld, 0)] + [("SYS", k) for k in self.vram.sys_lists_for(s.ld, s.idx)] + [(s.ld, s.idx)]
        elif s.idx:
            order.append(("SYS", s.idx))
        done = set()
        for l, k in order:
            if (l, k) in done or k >= len(self.lists[l]):
                continue
            done.add((l, k))
            for i, r, b, cl in self.grpx(self.lists[l][k]):
                if b == 16:
                    res += self._rows_of(r, bpp)
        add(tr("pg.resident"), res)

        def disc():
            out, names = [], set()
            for l in self.lists:
                for lst in self.lists[l]:
                    for i, r, b, cl in self.grpx(lst):
                        if b == 16 and r["name"] not in names:
                            names.add(r["name"])
                            rows = self._rows_of(r, bpp)
                            if rows:
                                out.append((r["name"], rows))
            return out
        groups.append((tr("pg.disc"), disc))
        return groups

    def _ref_label(self, ref):
        return "%s  %s" % (ref.file.rsplit("/", 1)[-1], "w%d" % ref.off)

    # ---- surfaces
    def surface_for(self, item):
        """Surface for ('page', name, ld, idx) | ('screen', name) | ('ui', id) | ('wppframe', file, n) | ('clut', name)."""
        k = item[0]
        if k == "page":
            return self._page_surface(item[1], item[2], item[3])
        if k == "screen":
            return self._screen_surface(item[1])
        if k == "ui":
            s = self._page_surface(UI_PAGE, "SYS", 0)
            for r in s.rects:
                if r.item == ("ui", item[1]):
                    s.selected = r
                    s.pal = r.pal or s.pal
            return s
        if k == "wppframe":
            return self._wpp_surface(item[1], item[2])
        if k == "clut":
            return self._clut_surface(item[1])
        if k == "composed":
            return self._composed_surface(item[1], item[2], item[3])
        raise err("c.badkind", k)

    # ---- ground truth: the port capture (tools/gfx_capture.py)
    def truth(self):
        if not hasattr(self, "_truth"):
            try:
                import gfx_capture
                self._truth = gfx_capture.truth(getattr(self, "capture_path", None))
                self.vram.TRUTH = self._truth       # SPR parts the game was seen drawing are shown as it drew them
            except Exception:
                self._truth = None
        return self._truth

    def captured_page(self, name):
        """(bit depth, [entries]) of the draws the game was seen making from page `name`, or (None, [])."""
        t = self.truth()
        if not t:
            return None, []
        ents = t.pages(self).get(name) or []
        if not ents:
            return None, []
        wt = {}
        for e in ents:
            wt[e[0]] = wt.get(e[0], 0) + e[-1]
        return (4, 8)[max(wt, key=wt.get)], ents

    def captured_ref(self, cx, cy, words):
        """PalRef for a captured palette: the CLUT record that holds it when its words match (editable), else a read-only copy."""
        n = len(words)
        ref = self.clut_lookup(cx, cy, n)
        if ref and list(self.pal_words(ref)) == list(words):
            return ref
        for r2 in self._cap_scan(n):
            if list(self.pal_words(r2)) == list(words):
                return r2
        key = "capture:%d,%d:%s" % (cx, cy, hashlib_md5(words))
        self._cap_pals[key] = list(words)
        return PalRef(key, 0, n)

    def _cap_scan(self, n):
        return []

    def _page_surface(self, name, ld, idx, crop=None, pal=None, title=None):
        p = dict(self.page_info(name, ld, idx))
        cbpp, cents = self.captured_page(name) if crop is None and pal is None else (None, [])
        if cbpp and cbpp != p["bpp"]:
            p["bpp"] = cbpp
        s = Surface(self, title or name, p["bpp"])
        s.layers = [(name, 0, p["w"], p["h"])]
        s.pageinfo = p
        s.crop = crop or (0, 0, p["w"] * (4, 2, 1)[(4, 8, 16).index(p["bpp"])] * 1, p["h"])
        s.W, s.H = s.crop[2], s.crop[3]
        s.ld, s.idx = ld, idx
        s.opaque = False
        s.key = name
        if p["bpp"] == 8 and p["clut"]:
            s.pal = PalRef(p["clut"]["name"], 0, 256)
        elif p["bpp"] == 8 and ld == "SYS" and self.backdrop(idx):
            bd = self.backdrop(idx)
            s.pal = bd["ref"]
            s.pal_note = tr("l.deduced", bd["label"], "backdrop")
        elif p["bpp"] == 4:
            s.pal = pal
            if not s.pal:
                try:
                    import palinfer
                    g = palinfer.infer(self, name, ld, idx)
                except Exception:
                    g = None
                if g:
                    s.pal, s.pal_note = g.ref, tr("l.deduced", g.label, g.method)
                else:
                    s.pal = (s.candidates()[:1] or [(None, None)])[0][1]
        if cents:
            # what the game draws from this page (port capture): each sprite rectangle with its own palette, the most used one as default
            n = 16 if p["bpp"] == 4 else 256
            wt, seen = {}, {}
            for dep, cx, cy, x, y, w, h, hx, k in sorted(cents, key=lambda e: e[-1]):     # heaviest last: it wins where draws overlap
                if (4, 8)[dep] != p["bpp"]:
                    continue
                words = [int(hx[i:i + 4], 16) for i in range(0, len(hx), 4)][:n]
                ref = seen.get(hx) or seen.setdefault(hx, self.captured_ref(cx, cy, words))
                wt[ref] = wt.get(ref, 0) + k
                s.rects.append(Rect("", x, y, w, h, ref, "capture"))
            if wt:
                s.pal = max(wt, key=wt.get)
                s.pal_note = tr("l.captured")
                s.multi_pal = len(wt) > 1
        s.inferred = s.pal
        s.apply_choice()
        if name == UI_PAGE:
            s.rects = self.ui_rects()
            if crop:    # rects are in page coordinates; the surface shows the crop, so move them to its origin and drop the outside ones
                cx, cy, cw, ch = crop
                keep = []
                for r in s.rects:
                    if r.x < cx + cw and r.x + r.w > cx and r.y < cy + ch and r.y + r.h > cy:
                        r.x, r.y = r.x - cx, r.y - cy
                        keep.append(r)
                s.rects = keep
        return s

    def title_screen(self):
        """The title background as the game shows it (O0PTSUB0, frame 1300 of the port): the 320 x 240 landscape (A00200, CLUT A00201)
        at y 8 of the 320 x 256 PAL frame, the TOMBI! logo of A00202 (CLUT A00203) cut in two strips (page px 8,0 240x112 at 16,24 and
        8,112 48x112 at 256,24, the draws of the capture); the PRESS START text is not part of it.  PIL RGBA image."""
        from PIL import Image
        out = Image.new("RGBA", (320, 256), (0, 0, 0, 255))
        out.alpha_composite(self._screen_surface("title_landscape").render(), (0, 8))
        logo = self._screen_surface("title_logo_page").render()
        out.alpha_composite(logo.crop((8, 0, 248, 112)), (16, 24))
        out.alpha_composite(logo.crop((8, 112, 56, 224)), (256, 24))
        return out

    def _screen_surface(self, name):
        if name == "title_screen":      # composed, read only: edit title_landscape / title_logo_page
            return ComposedSurface(self, name, self.title_screen())
        if name.startswith("backdrop_L"):
            return self._backdrop_surface(int(name[10:]))
        for sc in screens_list():
            if sc["name"] == name:
                break
        else:
            for nm, page, ld, idx, pal, crop in EXTRA_SCREENS:
                if nm == name:
                    return self._page_surface(page, ld, idx, crop, PalRef(*pal), name)
            raise err("c.badkind", name)
        import screens
        layers = [(f, x * 2 - min(p[1] for p in sc["pages"]) * 2, screens._pagew(f, sc["pages"]), self.page_info(f)["h"]) for f, x in sc["pages"]]
        s = Surface(self, name, 8)
        s.layers = layers
        full_w = max(l[1] + l[2] * 2 for l in layers)
        full_h = max(l[3] for l in layers)
        s.crop = sc["crop"] or (0, 0, full_w, full_h)
        s.W, s.H = s.crop[2], s.crop[3]
        s.pal = PalRef(sc["clut"], 0, 256)
        s.key = sc["pages"][0][0]
        s.inferred = s.pal
        s.apply_choice()
        s.opaque = sc["opaque"]
        s.ld, s.idx = "SYS", 0
        s.pageinfo = self.page_info(sc["pages"][0][0])
        s.ld, s.idx = s.pageinfo["ld"], s.pageinfo["idx"]
        return s

    def _backdrop_surface(self, k):
        bd = self.backdrop(k)
        if not bd:
            raise err("c.badkind", "backdrop_L%d" % k)
        recs = [self.page_info(n, "SYS", k) for n in bd["pages"]]
        s = Surface(self, "backdrop_L%d" % k, 8)
        # The 320 px picture is stored as page 0 (256 px) + the first 64 px of page 1: the game draws that strip at the LEFT edge.  When page 0
        # is narrower than the picture, the sheet is rebuilt as [strip of page 1][page 0][rest of page 1][other pages] so the picture reads continuous.
        p0 = recs[0]
        strip = 320 - p0["w"] * 2
        if len(recs) > 1 and 0 < strip < recs[1]["w"] * 2:
            p1 = recs[1]
            x = strip
            s.layers = [(p1["name"], 0, p1["w"], p1["h"], 0, strip), (p0["name"], x, p0["w"], p0["h"])]
            x += p0["w"] * 2
            s.layers.append((p1["name"], x, p1["w"], p1["h"], strip, p1["w"] * 2 - strip))
            x += p1["w"] * 2 - strip
            for p in recs[2:]:
                s.layers.append((p["name"], x, p["w"], p["h"]))
                x += p["w"] * 2
        else:
            x, s.layers = 0, []
            for p in recs:
                s.layers.append((p["name"], x, p["w"], p["h"]))
                x += p["w"] * 2
        s.crop = (0, 0, x, max(p["h"] for p in recs))
        s.W, s.H = s.crop[2], s.crop[3]
        s.pal = bd["ref"]
        s.key = recs[0]["name"]
        s.inferred = s.pal
        s.apply_choice()
        s.pal_note = tr("l.deduced", bd["label"], "backdrop")
        s.opaque = True            # room pictures are opaque: palette word 0 is black
        s.pageinfo = recs[0]
        s.ld, s.idx = "SYS", k
        return s

    def composed_image(self):
        """Disc image the composed room views are captured from (the capture runs the game, so an extracted folder is not enough)."""
        return self.disc.image if self.disc.image and os.path.isfile(self.disc.image) else None

    def composed_exe(self):
        return self.disc.ensure("EXE/MAIN0.EXE")

    def _composed_surface(self, area, section, kind):
        """Read-only surface of a captured room view (tools/gfx_composed.py); the picture must be in the cache."""
        from PIL import Image
        import gfx_composed as GC
        img = self.composed_image()
        p = GC.cached(img, area, section, kind) if img else None
        if not p:
            raise err("c.nocomposed", area, section)
        return ComposedSurface(self, "room_%02d_%d_%s" % (area, section, kind), Image.open(p).convert("RGBA"))

    def _composed_nodes(self, N):
        import gfx_composed as GC
        out = []
        for a, n in GC.rooms():
            kids = []
            for s in range(n):
                if GC.unreachable(a, s):
                    continue
                kids.append(N(tr("t.room", s), ("composed", a, s, "composed"), id="composed:%d:%d" % (a, s)))
                kids.append(N(tr("t.roomsky", s), ("composed", a, s, "sky"), id="composedsky:%d:%d" % (a, s)))
            out.append(N("AREA%02d" % a, children=kids))
        return N(tr("t.composed"), children=out)

    def _clut_surface(self, name):
        p = self.page_info(name)
        s = Surface(self, name, 16)
        s.layers = [(name, 0, p["w"], p["h"])]
        s.pageinfo = p
        s.crop = (0, 0, p["w"], p["h"])
        s.W, s.H = p["w"], p["h"]
        s.ld, s.idx = p["ld"], p["idx"]
        s.opaque = False
        s.is_clut = True
        return s

    def _wpp_surface(self, name, n):
        man, files, frames = self.wpp(name)
        key = "%s#%d" % (name, n)
        s = Surface(self, "%s  #%d" % (name, n), 4)
        d = self.get(key)
        w, h = struct.unpack_from("<HH", d)
        s.layers = [(key, 0, w, h)]
        s.crop = (0, 0, w * 4, h)
        s.W, s.H = w * 4, h
        s.opaque = False
        s.ld, s.idx = (self.A.find_list(name) or ("SYS", 0, None))[:2]
        s.header = 4
        pal = self.wpp_palette(name, n)
        s.pal = pal
        s.wpp = True
        # the SPR table that cuts these frames (same frame count) gives each part its own CLUT: Tombi's upper body uses row 480 of the
        # area CLUT, the lower body row 481 (port capture); show each part with its palette
        try:
            for i, p in enumerate(self.wpp_parts(name, n)):
                c = self.vram.wpp_clut(p[2])
                ref = self.clut_lookup((c & 0x3F) * 16, c >> 6 & 0x1FF, 16, [(s.ld, s.idx), (s.ld, 0)])
                if ref:
                    u, v = min(p[0], p[3], p[6], p[10]), min(p[1], p[4], p[7], p[11])
                    s.rects.append(Rect("", u, v, p[8], p[9], ref, "part"))
            if len({r.pal for r in s.rects}) > 1:
                s.multi_pal = True
        except Exception:
            pass
        return s

    def wpp_parts(self, name, n):
        """Parts of frame n of the SPR table linked to the WPP file `name` (same frame count in its load list), [] when none."""
        c = self.__dict__.setdefault("_wppspr", {})
        if name not in c:
            man, files, frames = self.wpp(name)
            cnt = len([k for k in files if k.startswith("frame_")])
            lst = self.A.find_list(name)
            tab = None
            if lst:
                for r, d in self.A.list_blocks(lst[0], lst[1]):
                    if (r["kind"] & 0xF0) == 0x30 and d and len(d) >= 4 and struct.unpack_from("<H", d, 2)[0] // 4 == cnt:
                        tab = self.vram.parse_spr(d)
                        break
            c[name] = tab
        tab = c[name]
        return [p for p in tab[n] if p[8] and p[9]] if tab and n < len(tab) else []

    def anim_owner(self, owner):
        """(owner dict, frame source) of one animation owner of tools/anim.py: "tombi" or an enemy/NPC/object handler id "aNN_<handler>"; cached."""
        c = getattr(self, "_anim", None)
        if c is None:
            c = self._anim = {}
        if owner not in c:
            import anim
            if owner == "tombi":
                c[owner] = (anim.tombi_owner(self.disc.raw), anim.Sprites())
            elif owner.startswith("unnamed:"):      # an unnamed X000 function that selects scripts itself (tools/unnamed.py)
                import unnamed
                e = next(x for x in self.unnamed_entries() if x["prog"] == "X000" and x["address"] == owner[8:])
                ow = unnamed.synthetic_owner(e, self.disc.raw)
                c[owner] = (ow, anim.owner_source(ow))
            else:
                ow = next(o for o in anim.object_owners(self.disc.raw, int(owner[1:3])) if o["owner"] == owner)
                c[owner] = (ow, anim.owner_source(ow))
        return c[owner]

    def unnamed_entries(self):
        """The functions of notes/unnamed.json (tools/unnamed.py; bundled in the exe), for the "Sin nombre" node."""
        if not hasattr(self, "_unnamed"):
            import unnamed
            self._unnamed = unnamed.load()
        return self._unnamed

    def _unnamed_nodes(self):
        """"Sin nombre": one group per program (MAIN0, X000, the SYSTEM overlays), one node per generic-named function; the editor
        (tools/names_panel.py) rewrites the node text with the user's name."""
        import unnamed
        N = lambda text, item=None, children=None, lazy=None, id=None, open=False: dict(text=text, item=item, children=children or [], lazy=lazy, id=id, open=open)
        prob, groups = [], {}
        for e in self.unnamed_entries():
            n = N(e["name"], ("unnamed", unnamed.key_of(e)))
            if e.get("note"):
                prob.append(n)
            else:
                groups.setdefault(e["prog"], []).append(n)
        return [N(tr("un.probably", len(prob)), children=prob, open=True),
                N(tr("un.others", sum(map(len, groups.values()))), children=[N(tr("un.prog", prog, len(kids)), children=kids) for prog, kids in groups.items()])]

    def _anim_nodes(self):
        """"Animaciones": Tombi, then the enemies / NPCs / objects of each area (the scripts of every placed object handler)."""
        import anim
        N = lambda text, item=None, children=None, lazy=None, id=None: dict(text=text, item=item, children=children or [], lazy=lazy, id=id)

        def owner_kids(owner):
            ow = self.anim_owner(owner)[0]
            return [N("%02d  %s" % (a["id"], a["name"]), ("anim", owner, a["id"]), id="anim:%s:%d" % (owner, a["id"]))
                    for a in ow["animations"] if a.get("alias_of") is None and a.get("renderable", True)]

        def area_kids(area):
            import labels
            out = []
            for ow in anim.object_owners(self.disc.raw, area):
                kids = [a for a in ow["animations"] if a.get("renderable", True)]
                if kids:
                    out.append(N("%s  (%s, %d)" % (ow["name"], ",".join(ow["types"]), len(kids)), lazy=lambda o=ow["owner"]: owner_kids(o), id="animowner:" + ow["owner"]))
            return out
        out = [N("Tombi", lazy=lambda: owner_kids("tombi"), id="animowner:tombi")]
        for ld in sorted(self.lists):
            if ld.startswith("AR"):
                a = int(ld[2:])
                out.append(N(tr("t.animarea", a), lazy=lambda a=a: area_kids(a), id="animarea:%d" % a))
        return out

    def wpp(self, name):
        if name not in self._wpp:
            import wpp
            d = self.vram.resource(name)
            rec = wpp.is_wpp(name)
            if d is None or not rec:
                raise err("c.notwpp", name)
            self._wpp[name] = wpp.unpack_wpp(d, name, rec)
        return self._wpp[name]

    def wpp_palette(self, name, n):
        import wpp
        man, files, frames = self.wpp(name)
        info = wpp.frame_palettes(name, frames).get("frame_%04d.4bpp" % n)
        if info:
            ref = self.clut_lookup(info["vram"][0], info["vram"][1], 16)
            if ref:
                return ref
            return PalRef(info["clut"], info["row"] * 256 + info["x0"], 16)
        cr = self.A.find_clut(name)
        return PalRef(cr["name"], 0, 16) if cr else None

    # ---- UiSpriteDef
    def ui_table(self):
        """[(id, u, v, w, h, clutX, clutY)] of the MAIN0 table, up to the first implausible entry."""
        d = self.disc.raw("EXE/MAIN0.EXE")
        o = UI_TABLE_VADDR - 0x80010000 + 0x800
        out = []
        for i in range(64):
            u, v, w, h, cx, cy = struct.unpack_from("<4H2h", d, o + 12 * i)
            ok = 0 < w <= 256 and 0 < h <= 256 and u + w <= 256 and v + h <= 256 and 0 <= cy < 512 and 0 <= cx < 1024 and cx % 16 == 0
            if i == 0:
                ok = ok and h == 71     # entry 0 = {0, 0, 112, 71, 16, 71}: placeholder
            if not ok:
                break
            out.append((i, u, v, w, h, cx, cy))
        return out

    def ui_rects(self):
        out = []
        for i, u, v, w, h, cx, cy in self.ui_table():
            ref = self.clut_lookup(cx, cy, 16, [("AR00", 1), ("SYS", 0), ("SYS", 8)])
            lab = "%d  %s" % (i, UI_NAMES.get(i, "UI %d" % i))
            out.append(Rect(lab, u, v, w, h, ref, "ui", ("ui", i)))
        return out

    def preview_context(self, ui_id=None):
        """320x240 black canvas with the sprite(s) at their screen position (TRY AGAIN: ids 1 and 2). Returns a PIL image."""
        from PIL import Image
        s = self._page_surface(UI_PAGE, "SYS", 0)
        im = Image.new("RGBA", (320, 240), (0, 0, 0, 255))
        for i, u, v, w, h, cx, cy in self.ui_table():
            if i not in UI_SCREEN_POS or (ui_id is not None and i != ui_id):
                continue
            ref = self.clut_lookup(cx, cy, 16, [("AR00", 1), ("SYS", 0), ("SYS", 8)])
            sub = s.render(ref, (u, v, w, h))
            im.alpha_composite(sub, UI_SCREEN_POS[i])
        return im

    # ---- catalogue / tree
    def tree(self):
        """[node] with node = dict(text, item, children|lazy)."""
        N = lambda text, item=None, children=None, lazy=None, id=None: dict(text=text, item=item, children=children or [], lazy=lazy, id=id)
        root = []
        scr = ([N("title_screen", ("screen", "title_screen"))] + [N(sc["name"], ("screen", sc["name"])) for sc in screens_list()]
               + [N(e[0], ("screen", e[0])) for e in EXTRA_SCREENS])
        for n in scr:
            n["id"] = "screen:" + n["item"][1]
        import palinfer
        scr.append(N(tr("t.backdrops"), children=[N("L%d  %s" % (k, palinfer.backdrop(self, k)["name"]), ("screen", "backdrop_L%d" % k), id="backdrop:L%d" % k)
                                                 for k in palinfer.backdrop_lists(self)]))
        root.append(N(tr("t.screens"), children=scr))
        root.append(self._composed_nodes(N))
        ui = [N(tr("t.uitable"), children=[N("%d  %s  (%d,%d %dx%d)" % (i, UI_NAMES.get(i, "UI %d" % i), u, v, w, h), ("ui", i), id="ui:%d" % i)
                                           for i, u, v, w, h, cx, cy in self.ui_table()])]
        for nm in ("SYSTEM/A00000.GAM", UI_PAGE):
            ui.append(N(nm.split("/")[-1] + "  " + tr("t.hudpage"), ("page", nm, "SYS", 0), id="page:" + nm))
        root.append(N(tr("t.ui"), children=ui))
        pg = []
        for ld in sorted(self.lists, key=lambda x: (x != "SYS", x)):
            grp = N(self.area_name(ld), children=[])
            for k, lst in enumerate(self.lists[ld]):
                pages = [(r, bpp) for i, r, bpp, cl in self.grpx(lst) if bpp != 16]
                if not pages:
                    continue
                grp["children"].append(N(tr("t.list", k), id="list:%s:%d" % (ld, k), children=[
                    N("%s  (%dx%d, %d bpp)" % (r["name"].split("/")[-1], r["w"] * (4 if bpp == 4 else 2), r["h"], bpp), ("page", r["name"], ld, k), id="page:" + r["name"])
                    for r, bpp in pages]))
            pg.append(grp)
        root.append(N(tr("t.pages"), children=pg))
        root.append(N(tr("t.sprites"), lazy=self._sprite_nodes))
        root.append(N(tr("t.anims"), lazy=self._anim_nodes))
        root.append(N(tr("t.unnamed"), lazy=self._unnamed_nodes))
        root.append(self._model_nodes(N))
        pal = []
        for ld in sorted(self.lists, key=lambda x: (x != "SYS", x)):
            names = []
            for lst in self.lists[ld]:
                for i, r, bpp, cl in self.grpx(lst):
                    if bpp == 16 and r["name"] not in names:
                        names.append(r["name"])
            if names:
                pal.append(N(self.area_name(ld), children=[N(n.split("/")[-1], ("clut", n), id="clut:" + n) for n in sorted(names)]))
        root.append(N(tr("t.palettes"), children=pal))
        return root

    def _model_nodes(self, N):
        """Tree node "Modelos 3D": WMD rooms (kind 0x50) per area and the object model tables (kind 0x51), previewed by tools/model_viewer.py."""
        import model_view as MV
        rooms = {}
        for ld, i, nm in MV.rooms():
            rooms.setdefault(ld, []).append(N("%s  (%s)" % (nm.split("/")[-1].split(".")[0], tr("t.list", i)), ("model3d", "room", ld, i, None), id="room:%s:%d" % (ld, i)))
        objs = [N(tr("m.set", self.area_name(ld), i, n), id="objset:%s:%d" % (ld, i),
                  lazy=(lambda ld=ld, i=i, n=n: [N(tr("m.obj", k), ("model3d", "obj", ld, i, k), id="obj:%s:%d:%d" % (ld, i, k)) for k in range(n)]))
                for ld, i, n in MV.objects()]
        return N(tr("t.models"), children=[N(tr("t.m.rooms"), children=[N(self.area_name(ld), children=c) for ld, c in sorted(rooms.items())]),
                                          N(tr("t.m.objs"), children=objs)])

    def _sprite_nodes(self):
        import wpp
        N = lambda text, item=None, children=None, lazy=None, id=None: dict(text=text, item=item, children=children or [], lazy=lazy, id=id)
        out = []
        seen = set()
        for ld, lists in sorted(self.lists.items()):
            for lst in lists:
                for r in lst:
                    nm = r["name"]
                    if (r["kind"] & 0xF0) == 0x60 and r["type"] != 0xFFFFFFFF and nm.endswith(".000") and nm not in seen and wpp.is_wpp(nm):
                        seen.add(nm)
                        out.append(N(nm, lazy=(lambda nm=nm: self._frame_nodes(nm)), id="wppfile:" + nm))
        return out

    def _frame_nodes(self, nm):
        man, files, frames = self.wpp(nm)
        out = []
        for k in sorted(frames):
            n = int(k.split("frame_")[1][:4])
            w, h = struct.unpack_from("<HH", frames[k])
            out.append(dict(text="#%d  (%dx%d)" % (n, w * 4, h), item=("wppframe", nm, n), children=[], lazy=None, id="wpp:%s:frame:%d" % (nm, n)))
        return out

    def item_id(self, item):
        """Stable label id (tools/labels.py) of a tree item, or None."""
        k = item[0]
        return {"page": lambda: "page:" + item[1], "screen": lambda: ("backdrop:L" + item[1][10:]) if item[1].startswith("backdrop_L") else "screen:" + item[1],
                "ui": lambda: "ui:%d" % item[1], "wppframe": lambda: "wpp:%s:frame:%d" % (item[1], item[2]), "clut": lambda: "clut:" + item[1],
                "anim": lambda: "anim:%s:%d" % (item[1], item[2]),
                "model3d": lambda: ("room:%s:%d" % item[2:4]) if item[1] == "room" else "obj:%s:%d:%d" % item[2:5]}[k]() if k in ("page", "screen", "ui", "wppframe", "clut", "anim", "model3d") else None

    def item_name(self, item):
        return ":".join(str(x) for x in (item[0],) + tuple(item[1:2]))

    # ---- mods
    def used_palette(self, name):
        p = self.page_info(name)
        if p["bpp"] == 4:
            return self.pal_choice.get(name) or (self._page_surface(name, p["ld"], p["idx"]).pal)
        if p["bpp"] == 8:
            return self.pal_choice.get(name) or (PalRef(p["clut"]["name"], 0, 256) if p["clut"] else (self.backdrop(p["idx"]) or {}).get("ref") if p["ld"] == "SYS" else None)
        return None

    def save_mod(self, folder, name=None, description=""):
        """Writes folder/mod.json + PNGs for every edit.  Returns the list of PNG files."""
        from PIL import Image
        os.makedirs(folder, exist_ok=True)
        patchs, files = [], []
        for key in self.edited():
            if "#" in key:
                fname, n = key.split("#")
                n = int(n)
                s = self._wpp_surface(fname, n)
                png = "%s_frame%04d.png" % (fname.replace("/", "_").rsplit(".", 1)[0], n)
                s.export_png(os.path.join(folder, png))
                patchs.append({"file": png, "targets": [{"pattern": fname, "frame": n}]})
            else:
                p = self.page_info(key)
                png = key.replace("/", "_").rsplit(".", 1)[0] + ".png"
                if p["bpp"] == 16:
                    self.write_rgba(os.path.join(folder, png), key, p["w"], p["h"])
                else:
                    s = self._page_surface(key, p["ld"], p["idx"])
                    s.export_png(os.path.join(folder, png))
                patchs.append({"file": png, "targets": [{"pattern": key}]})
            files.append(png)
        cfg = {"name": name or os.path.basename(os.path.abspath(folder)), "description": description, "version": "1.0", "patchs": patchs,
               "tombi_gfx": {"palettes": {k: list(v.key()) for k, v in self.pal_choice.items()}}}
        json.dump(cfg, open(os.path.join(folder, "mod.json"), "w", encoding="utf-8"), indent=1, ensure_ascii=False)
        return files

    def write_rgba(self, path, key, w, h):
        from PIL import Image
        import vram
        ws = struct.unpack("<%dH" % (w * h), self.get(key)[:w * h * 2])
        raw = b"".join(vram.RGB[x & 0x7FFF] + (b"\0" if x == 0 else b"\xff") for x in ws)
        Image.frombytes("RGBA", (w, h), raw).save(path)

    def load_mod(self, folder):
        """Applies mod.json of `folder` to the in-memory edits (one undo step). Returns the number of resources changed."""
        import mod
        cfg = json.load(open(os.path.join(folder, "mod.json"), encoding="utf-8"))
        names = {r["name"] for lists in self.lists.values() for lst in lists for r in lst if self.vram.is_grpx(r)}
        for lists in self.lists.values():
            for lst in lists:
                for r in lst:
                    if (r["kind"] & 0xF0) == 0x60:
                        names.add(r["name"])
        changes = {}
        for p in cfg.get("patchs", []):
            png = os.path.join(folder, p["file"])
            for t in p["targets"]:
                hits = sorted(n for n in names if fnmatch.fnmatch(n, t["pattern"]))
                if not hits:
                    raise err("c.nomatch", t["pattern"])
                for h in hits:
                    try:
                        if t.get("frame") is not None:
                            key = "%s#%d" % (h, t["frame"])
                            d = self.get(key)
                            w, hh = struct.unpack_from("<HH", d)
                            changes[key] = struct.pack("<HH", w, hh) + mod.png_words(png, w, hh, 4)
                        else:
                            pi = self.page_info(h)
                            changes[h] = mod.png_words(png, pi["w"], pi["h"], pi["bpp"], self.get(h) if pi["bpp"] == 16 else None)
                    except SystemExit as e:
                        raise CoreError(str(e))
        for k, v in cfg.get("tombi_gfx", {}).get("palettes", {}).items():
            self.pal_choice[k] = PalRef(*v)
        self.edit(changes)
        return len(changes)

    # ---- building
    def changed_files(self):
        """{path: bytes as stored on the disc} for every edited file (GAM repacked, WPP re-encoded)."""
        import gam_py, wpp
        out = {}
        wppf = {}
        for key, v in self.edits.items():
            if "#" in key:
                wppf.setdefault(key.split("#")[0], {})[int(key.split("#")[1])] = v
            else:
                out[key] = v
        for name, fr in wppf.items():
            man, files, frames = self.wpp(name)
            f = dict(files)
            for n, v in fr.items():
                f["frame_%04d.4bpp" % n] = v
            new = wpp.pack_wpp(man, f)
            if len(new) != len(self.original(name)):
                raise err("c.wppsize", name, len(new), len(self.original(name)))
            out[name] = new
        res = {}
        for name, d in out.items():
            if name.upper().endswith(".GAM"):
                d = gam_py.pack(d, self.disc.raw(name))
            res[name] = d
        return res

    def write_files(self, out_dir):
        files = self.changed_files()
        for name, data in files.items():
            p = os.path.join(out_dir, *name.split("/"))
            os.makedirs(os.path.dirname(p), exist_ok=True)
            open(p, "wb").write(data)
        return sorted(files)

    def have_mkpsxiso(self):
        import mkdisc
        try:
            mkdisc.tool("mkpsxiso")
            mkdisc.tool("dumpsxiso")
            return True
        except SystemExit:
            return False

    def build_disc(self, out_bin, work, log=print):
        """Builds out_bin (+ .cue) from the original image with the edits (tools/mkdisc.py --relayout)."""
        import argparse, mkdisc
        files = self.changed_files()
        if not files:
            raise err("c.nochanges")
        if not self.disc.image:
            raise err("c.needimage")
        need = 2 * os.path.getsize(self.disc.image) + (64 << 20)
        probe = os.path.abspath(os.path.dirname(os.path.abspath(out_bin)))
        os.makedirs(probe, exist_ok=True)
        free = shutil.disk_usage(probe).free
        if free < need:
            raise err("c.space", need >> 20, free >> 20)
        work = os.path.abspath(work)
        fs = os.path.join(work, "fs")
        shutil.rmtree(fs, ignore_errors=True)
        for name, data in files.items():
            p = os.path.join(fs, *name.split("/"))
            os.makedirs(os.path.dirname(p), exist_ok=True)
            open(p, "wb").write(data)
        import cdfiles
        lay = cdfiles.layout_from_image(self.disc.image)
        for name in sorted(lay):
            if re.fullmatch(r"EXE/MAIN\d\.EXE", name):
                p = os.path.join(fs, *name.split("/"))
                os.makedirs(os.path.dirname(p), exist_ok=True)
                open(p, "wb").write(self.disc.raw(name) if self.disc.layout else open(os.path.join(self.disc.fs, name), "rb").read())
        ns = argparse.Namespace(fs=fs, disc=self.disc.image, work=os.path.join(work, "mkdisc"), out=os.path.abspath(out_bin), relayout=True)
        buf = io.StringIO()
        try:
            with contextlib.redirect_stdout(buf), contextlib.redirect_stderr(buf):
                mkdisc.build(ns)
        except SystemExit as e:
            raise CoreError(tr("c.mkfail", e, buf.getvalue()[-1500:]))
        log(next((l for l in buf.getvalue().splitlines() if l.startswith("built")), ""))
        return sorted(files)


# ------------------------------------------------------------------ surfaces
class Surface:
    """A picture made of one or more pages (a screen), a crop of it, or a page: index plane, palette, rects, import/export."""

    def __init__(self, core, title, bpp):
        self.core, self.title, self.bpp = core, title, bpp
        self.layers, self.crop, self.W, self.H = [], (0, 0, 0, 0), 0, 0
        self.pal = None
        self.rects = []
        self.selected = None
        self.pal_note = ""
        self.opaque = False
        self.is_clut = False
        self.wpp = False
        self.header = 0
        self.ld, self.idx = "SYS", 0
        self.pageinfo = None
        self.key = None            # Core.pal_choice key (name of the first page)
        self.inferred = None       # the palette the editor deduced (before any manual choice)
        self.multi_pal = False     # rects carry their own palettes (captured from the game): render each with it

    def apply_choice(self):
        c = self.core.pal_choice.get(self.key)
        if c and self.bpp != 16 and c.n == (16 if self.bpp == 4 else 256):
            self.pal = c

    def choose(self, ref):
        """Manual palette choice (remembered per entry; choosing the inferred one goes back to automatic)."""
        self.pal = ref
        if self.key is None:
            return
        if ref == self.inferred:
            self.core.pal_choice.pop(self.key, None)
        else:
            self.core.pal_choice[self.key] = ref

    def candidates(self):
        if self.bpp == 16:
            return []
        return self.core.palette_candidates(self.bpp, self.ld, self.idx)

    def _layer_planes(self):
        out = []
        for lay in self.layers:
            name, x0, w, h = lay[:4]
            d = self.core.get(name)
            if self.header:
                d = d[self.header:]
            full = w * (4 if self.bpp == 4 else 2 if self.bpp == 8 else 1)
            sx, sw = lay[4:6] if len(lay) > 4 else (0, full)     # optional source column window (pixels): a layer can show part of a page
            out.append((name, x0, w, h, decode_plane(d, w, h, self.bpp), sx, sw))
        return out

    def plane(self):
        """The cropped W x H plane (bytearray of indices, or array('H') for 16 bpp)."""
        cx, cy, cw, ch = self.crop
        lp = self._layer_planes()
        full_w = max(l[1] + l[6] for l in lp)
        full_h = max(l[3] for l in lp)
        full = (array("H", bytes(2 * full_w * full_h)) if self.bpp == 16 else bytearray(full_w * full_h))
        for name, x0, w, h, pl, sx, sw in lp:
            wp = w * (4 if self.bpp == 4 else 2 if self.bpp == 8 else 1)
            for y in range(h):
                full[y * full_w + x0:y * full_w + x0 + sw] = pl[y * wp + sx:y * wp + sx + sw]
        out = full[:0]
        rows = [full[(cy + y) * full_w + cx:(cy + y) * full_w + cx + cw] for y in range(ch)]
        res = (array("H") if self.bpp == 16 else bytearray())
        for r in rows:
            res += r
        return res

    def put(self, x, y, w, h, flat, label=""):
        """Writes a w x h block of indices at (x, y) of this surface (crop coordinates) into the pages. One undo step."""
        cx, cy, cw, ch = self.crop
        X, Y = x + cx, y + cy
        changes = {}
        touched = {}
        for name, x0, lw, lh, pl, sx, sw in self._layer_planes():
            wp = lw * (4 if self.bpp == 4 else 2 if self.bpp == 8 else 1)
            ax, bx = max(X, x0), min(X + w, x0 + sw)
            ay, by = max(Y, 0), min(Y + h, lh)
            if ax >= bx or ay >= by:
                continue
            pl = touched.setdefault(name, pl)
            for yy in range(ay, by):
                src = (yy - Y) * w + (ax - X)
                pl[yy * wp + sx + (ax - x0):yy * wp + sx + (bx - x0)] = flat[src:src + (bx - ax)]
        for name, pl in touched.items():
            data = encode_plane(pl, self.bpp)
            if self.header:
                data = self.core.get(name)[:self.header] + data
            changes[name] = data
        return self.core.edit(changes)

    def palette_rgba(self, ref=None, n=None):
        """(palette words) for the surface (or `ref`)."""
        ref = ref or self.pal
        return self.core.pal_words(ref) if ref else []

    def render(self, ref=None, region=None):
        """RGBA PIL image of the surface (or of the region (x, y, w, h)); index/word 0 is transparent unless the surface is opaque."""
        from PIL import Image
        import vram
        pl = self.plane()
        W, H = self.W, self.H
        if self.bpp == 16:
            raw = b"".join(vram.RGB[v & 0x7FFF] + (b"\xff" if self.opaque or v else b"\0") for v in pl)
            im = Image.frombytes("RGBA", (W, H), raw)
        else:
            ws = self.palette_rgba(ref)
            ws = ws + [0] * (256 - len(ws)) if len(ws) < 256 else ws
            pal = b"".join(vram.RGB[w & 0x7FFF] for w in ws[:256])
            alpha = bytes(255 if (self.opaque or w) else 0 for w in ws[:256])
            im = Image.frombytes("P", (W, H), bytes(pl))
            im.putpalette(pal)
            rgba = im.convert("RGBA")
            lut = Image.frombytes("L", (W, H), bytes(pl).translate(alpha + bytes(256 - len(alpha))))
            rgba.putalpha(lut)
            im = rgba
            if self.multi_pal and ref is None:
                im = self._multi(im, pl, W, H)
        if region:
            x, y, w, h = region
            im = im.crop((x, y, x + w, y + h))
        return im

    def _multi(self, im, pl, W, H):
        """Paints every rect that has its own palette with it (the sprites as the game draws them)."""
        from PIL import Image
        import vram
        luts = {}
        for r in self.rects:
            if not r.pal:
                continue
            if r.pal not in luts:
                ws = self.core.pal_words(r.pal)
                ws = (ws + [0] * 256)[:256]
                luts[r.pal] = [vram.RGB[w & 0x7FFF] + (b"\xff" if (self.opaque or w) else b"\0") for w in ws]
            lut = luts[r.pal]
            x0, y0, x1, y1 = max(r.x, 0), max(r.y, 0), min(r.x + r.w, W), min(r.y + r.h, H)
            if x0 >= x1 or y0 >= y1:
                continue
            raw = b"".join(lut[pl[y * W + x]] for y in range(y0, y1) for x in range(x0, x1))
            im.paste(Image.frombytes("RGBA", (x1 - x0, y1 - y0), raw), (x0, y0))
        return im

    def indexed(self, region=None, ref=None):
        """Indexed PIL image (mode P, real palette, tRNS for word 0) of the surface or a region."""
        from PIL import Image
        import vram
        if self.bpp == 16:
            return self.render(ref, region)
        pl = self.plane()
        W, H = self.W, self.H
        im = Image.frombytes("P", (W, H), bytes(pl))
        ws = self.palette_rgba(ref)
        n = 16 if self.bpp == 4 else 256
        ws = (ws + [0] * n)[:n]
        im.putpalette(b"".join(vram.RGB[w & 0x7FFF] for w in ws))
        im.info["transparency"] = bytes(255 if (self.opaque or w) else 0 for w in ws)
        if region:
            x, y, w, h = region
            im = im.crop((x, y, x + w, y + h))
            im.info["transparency"] = bytes(255 if (self.opaque or w_) else 0 for w_ in ws)
        return im

    def export_png(self, path, region=None, ref=None):
        im = self.indexed(region, ref)
        if self.bpp == 16:
            im.save(path)
        else:
            # keep the palette to its real size (16 or 256 entries) and index 0..N-1 unchanged
            im.save(path, transparency=im.info.get("transparency"), optimize=False)
        return path

    def export_bytes(self, region=None, ref=None):
        b = io.BytesIO()
        im = self.indexed(region, ref)
        im.save(b, "PNG", transparency=im.info.get("transparency")) if self.bpp != 16 else im.save(b, "PNG")
        return b.getvalue()

    def import_png(self, path, region=None, ref=None):
        """Imports a PNG into the region (default: the whole surface). Returns a report dict; warnings = [(text)]."""
        from PIL import Image
        import vram
        x, y, w, h = region or (0, 0, self.W, self.H)
        im = Image.open(path)
        im.load()
        warn = []
        if im.width > w or im.height > h:
            raise err("c.bigimg", im.width, im.height, w, h)
        if (im.width, im.height) != (w, h):
            warn.append(tr("w.smaller", im.width, im.height, w, h))
            w, h = im.width, im.height
        rep = dict(direct=False, colours=0, limit=0, approx=False, transparent=0, maxerr=0, warnings=warn)
        if self.bpp == 16:
            rgba = im.convert("RGBA")
            ws = array("H")
            old = self.plane()
            for k, (r, g, b, a) in enumerate(rgba.getdata()):
                if a == 0:
                    ws.append(0)
                    continue
                v = rgb_to_word(r, g, b)
                oi = (y + k // w) * self.W + x + k % w
                ws.append(v | (old[oi] & 0x8000) if v | (old[oi] & 0x8000) else 0x8000)
            self.put(x, y, w, h, ws, "import")
            return rep
        ref = ref or self.pal
        ws = self.palette_rgba(ref)
        n = 16 if self.bpp == 4 else 256
        ws = (ws + [0] * n)[:n]
        pal_rgb = [bytes(vram.RGB[v & 0x7FFF]) for v in ws]
        rep["limit"] = n
        if im.mode == "P":
            ip = im.getpalette() or []
            ip = [bytes(ip[i * 3:i * 3 + 3]) for i in range(len(ip) // 3)]
            data = im.tobytes()
            used = sorted(set(data))
            if max(used) < n and all(i < len(ip) and ip[i] == pal_rgb[i] for i in used):
                rep["direct"] = True
                idx = bytearray(data)
                rep["colours"] = len(used)
                self.put(x, y, w, h, idx, "import")
                self._check0(ws, warn)
                return rep
            warn.append(tr("w.palmismatch"))
        rgba = im.convert("RGBA")
        zero = [i for i, v in enumerate(ws) if v == 0]
        t_idx = zero[0] if zero else 0
        if not zero:
            warn.append(tr("w.nozero"))
        cand = [i for i in range(n) if ws[i] != 0]
        cache, out, uniq = {}, bytearray(), set()
        maxerr = 0
        for r, g, b, a in rgba.getdata():
            if a < 128:
                out.append(t_idx)
                rep["transparent"] += 1
                continue
            key = (r, g, b)
            if key not in cache:
                best = min(cand, key=lambda i: (pal_rgb[i][0] - r) ** 2 + (pal_rgb[i][1] - g) ** 2 + (pal_rgb[i][2] - b) ** 2) if cand else 0
                e = sum((pal_rgb[best][c] - key[c]) ** 2 for c in range(3)) ** 0.5
                cache[key] = (best, e)
            i, e = cache[key]
            uniq.add(key)
            out.append(i)
            maxerr = max(maxerr, e)
        rep["colours"] = len(uniq)
        rep["maxerr"] = round(maxerr, 1)
        rep["approx"] = maxerr > 6
        if len(uniq) > len(cand):
            warn.append(tr("w.morecolours", len(uniq), n))
        elif rep["approx"]:
            warn.append(tr("w.approx", round(maxerr)))
        self.put(x, y, w, h, out, "import")
        self._check0(ws, warn)
        return rep

    def _check0(self, ws, warn):
        if ws and ws[0] != 0:
            warn.append(tr("w.index0"))

    def hit(self, x, y):
        for r in reversed(self.rects):
            if r.x <= x < r.x + r.w and r.y <= y < r.y + r.h:
                return r
        return None


class ComposedSurface(Surface):
    """Read-only RGB picture (a captured room view): no palette, no pages, nothing to import."""

    def __init__(self, core, title, image):
        super().__init__(core, title, 16)
        self.image = image
        self.W, self.H = image.size
        self.crop = (0, 0, self.W, self.H)
        self.opaque = True

    def render(self, ref=None, region=None):
        im = self.image
        if region:
            x, y, w, h = region
            im = im.crop((x, y, x + w, y + h))
        return im

    def indexed(self, region=None, ref=None):
        return self.render(None, region).convert("RGB")

    def import_png(self, path, region=None, ref=None):
        raise err("c.composedro")


# ------------------------------------------------------------------ strings
S_ES = {
    "l.deduced": "paleta deducida: {0} ({1})", "l.captured": "paletas capturadas del juego (cada sprite con la suya)",
    "c.capro": "Esta paleta solo existe en tiempo de ejecución (capturada del juego): no hay archivo que editar.",
    "c.notiso": "{0} no es una imagen de disco ISO9660 (.bin de 2352 bytes por sector o .iso).", "c.nofile": "No existe {0}.",
    "c.nofs": "No se encuentra la carpeta SYS en {0}: ¿es el disco de Tombi! (PAL)?", "c.nolists": "No se pudieron leer las listas de carga (SYS/LD0*.BIN).",
    "c.nocomposed": "La vista compuesta de AREA{0:02d} sala {1} aún no está generada.", "c.composedro": "La vista compuesta es solo de lectura.",
    "c.nores": "No se pudo leer {0}.", "c.size": "{0}: tamaño {1} distinto del original {2}.", "c.nopage": "No hay ninguna página llamada {0}.",
    "c.badkind": "Elemento desconocido: {0}", "c.notwpp": "{0} no es un archivo de sprites WPP.", "c.nomatch": "El patrón {0} no coincide con ningún archivo.",
    "c.wppsize": "{0}: el archivo de sprites recodificado mide {1} bytes y el original {2}; el cambio no cabe (usa menos detalle/colores repetidos).",
    "c.nochanges": "No hay cambios que aplicar.", "c.needimage": "Hace falta la imagen original (.bin) para generar la ISO: ábrela en lugar de la carpeta.",
    "c.space": "Poco espacio libre: la ISO necesita unos {0} MB y hay {1} MB.", "c.mkfail": "mkpsxiso falló: {0}\n{1}",
    "c.bigimg": "La imagen ({0}x{1}) es mayor que el destino ({2}x{3}).",
    "lb.rename": "Renombrar…", "lb.name": "Nombre:", "lb.note": "Nota:", "lb.cancel": "Cancelar", "lb.export": "Exportar etiquetas", "lb.import": "Importar etiquetas",
    "lb.exported": "{0} etiqueta(s) exportadas a {1}", "lb.imported": "{0} etiqueta(s) importadas",
    "pg.inferred": "Deducida", "pg.list": "Registros CLUT de la lista actual", "pg.embedded": "Paletas incrustadas en la página",
    "pg.resident": "Filas CLUT residentes en la VRAM", "pg.disc": "Todos los CLUT del disco (por archivo)", "pal.inf": "{0}  (deducida)",
    "pal.emb": "{0}  fila {1} · paleta {2}", "l.nowpal": "Paleta: {0}", "l.autotag": "  (deducida)", "l.palhint": "Arriba/Abajo: cambiar de paleta",
    "pal.row": "{0}  fila {1}", "pal.sub": "{0}  fila {1} · paleta {2}",
    "t.screens": "Pantallas", "t.backdrops": "Fondos de sala (mapas)", "t.ui": "Interfaz / HUD", "t.uitable": "Tabla UiSpriteDef (MAIN0)", "t.hudpage": "(página HUD)", "t.pages": "Páginas por lista de carga",
    "t.list": "Lista {0}", "t.anims": "Animaciones", "t.animarea": "Enemigos / NPC / objetos: área {0}", "t.sprites": "Sprites (WPP)", "t.palettes": "Paletas (CLUT)",
    "t.unnamed": "Sin nombre", "un.prog": "{0}  ({1} funciones)", "un.probably": "Probablemente  ({0}: con nota, para revisar)", "un.others": "Resto  ({0})",
    "w.smaller": "La imagen ({0}x{1}) es menor que el destino ({2}x{3}): se coloca arriba a la izquierda.",
    "w.palmismatch": "El PNG es indexado pero su paleta no coincide con la actual: se ajusta al color más cercano.",
    "w.nozero": "La paleta no tiene ningún color 0x0000 (transparente): los píxeles transparentes usan el índice 0.",
    "w.morecolours": "La imagen usa {0} colores y la paleta solo admite {1}: se aproximaron al más cercano.",
    "w.approx": "Algunos colores se aproximaron (error máximo {0} de 441).",
    "w.index0": "El índice 0 de esta paleta no es transparente (palabra distinta de 0): se verá como color opaco.",
}
S_EN = {
    "l.deduced": "palette inferred: {0} ({1})", "l.captured": "palettes captured from the game (each sprite with its own)",
    "c.capro": "This palette only exists at run time (captured from the game): there is no file to edit.",
    "c.notiso": "{0} is not an ISO9660 disc image (.bin with 2352-byte sectors or .iso).", "c.nofile": "{0} does not exist.",
    "c.nofs": "No SYS folder found in {0}: is it the Tombi! (PAL) disc?", "c.nolists": "The load lists (SYS/LD0*.BIN) could not be read.",
    "c.nocomposed": "The composed view of AREA{0:02d} room {1} is not generated yet.", "c.composedro": "The composed view is read-only.",
    "c.nores": "Could not read {0}.", "c.size": "{0}: size {1} differs from the original {2}.", "c.nopage": "There is no page named {0}.",
    "c.badkind": "Unknown item: {0}", "c.notwpp": "{0} is not a WPP sprite file.", "c.nomatch": "Pattern {0} matches no file.",
    "c.wppsize": "{0}: the re-encoded sprite file is {1} bytes, the original {2}; the change does not fit (use less detail / repeated colours).",
    "c.nochanges": "There are no changes to apply.", "c.needimage": "The original image (.bin) is needed to build the ISO: open it instead of the folder.",
    "c.space": "Not enough free space: the ISO needs about {0} MB and {1} MB are free.", "c.mkfail": "mkpsxiso failed: {0}\n{1}",
    "c.bigimg": "The image ({0}x{1}) is larger than the target ({2}x{3}).",
    "lb.rename": "Rename…", "lb.name": "Name:", "lb.note": "Note:", "lb.cancel": "Cancel", "lb.export": "Export labels", "lb.import": "Import labels",
    "lb.exported": "{0} label(s) exported to {1}", "lb.imported": "{0} label(s) imported",
    "pg.inferred": "Inferred", "pg.list": "CLUT records of the current list", "pg.embedded": "Palettes embedded in the page",
    "pg.resident": "CLUT rows resident in VRAM", "pg.disc": "All CLUTs on the disc (by file)", "pal.inf": "{0}  (inferred)",
    "pal.emb": "{0}  row {1} · palette {2}", "l.nowpal": "Palette: {0}", "l.autotag": "  (inferred)", "l.palhint": "Up/Down: cycle palettes",
    "pal.row": "{0}  row {1}", "pal.sub": "{0}  row {1} · palette {2}",
    "t.screens": "Screens", "t.backdrops": "Room backdrops (maps)", "t.ui": "Interface / HUD", "t.uitable": "UiSpriteDef table (MAIN0)", "t.hudpage": "(HUD page)", "t.pages": "Pages by load list",
    "t.list": "List {0}", "t.anims": "Animations", "t.animarea": "Enemies / NPCs / objects: area {0}", "t.sprites": "Sprites (WPP)", "t.palettes": "Palettes (CLUT)",
    "t.unnamed": "Unnamed", "un.prog": "{0}  ({1} functions)", "un.probably": "Probably  ({0}: with a note, to review)", "un.others": "Others  ({0})",
    "w.smaller": "The image ({0}x{1}) is smaller than the target ({2}x{3}): placed at the top left.",
    "w.palmismatch": "The PNG is indexed but its palette differs from the current one: matched to the nearest colour.",
    "w.nozero": "The palette has no 0x0000 (transparent) colour: transparent pixels use index 0.",
    "w.morecolours": "The image uses {0} colours and the palette only holds {1}: they were approximated to the nearest.",
    "w.approx": "Some colours were approximated (largest error {0} of 441).",
    "w.index0": "Index 0 of this palette is not transparent (non-zero word): it will show as an opaque colour.",
}
i18n.add({"es": S_ES, "en": S_EN})


# ------------------------------------------------------------------ command line
def _walk(nodes, depth=0, out=None):
    out = [] if out is None else out
    for n in nodes:
        ch = n["children"] or (n["lazy"]() if n["lazy"] and depth < 1 else [])
        out.append("  " * depth + n["text"] + ("   [%s]" % ":".join(map(str, n["item"])) if n["item"] else ""))
        _walk(ch, depth + 1, out)
    return out


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="c", required=True)
    s = sub.add_parser("list")
    s.add_argument("disc")
    s = sub.add_parser("render")
    s.add_argument("disc")
    s.add_argument("item", help="screen:NAME | ui:ID | page:NAME[:LD:IDX] | clut:NAME")
    s.add_argument("out")
    a = ap.parse_args(argv)
    core = Core(a.disc)
    try:
        return _main(core, a)
    finally:
        core.disc.close()      # the extraction cache of a disc image is a temp folder: never leave it behind


def _main(core, a):
    if a.c == "list":
        print("\n".join(_walk(core.tree())))
        return 0
    p = a.item.split(":")
    item = (p[0], p[1] if p[0] != "ui" else int(p[1])) + tuple((p[2], int(p[3])) if len(p) > 3 else (("SYS", 0) if p[0] == "page" else ()))
    sf = core.surface_for(item)
    im = sf.render()
    im.save(a.out)
    print(a.out, im.size)
    return 0


if __name__ == "__main__":
    sys.exit(main())
