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
    # NOW LOADING: A00900 (SYS list 9, 256 x 72, 4 bpp) holds its own palettes in its last row (y 71: 3 x 16 words at 16, 32, 48)
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
        self.pal_choice = {}       # page name -> PalRef last used (saved with the mod)
        self.lists = A.load_lists()
        if not self.lists:
            raise err("c.nolists")
        self._cl = None
        self._wpp = {}

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
        d = self.get(ref.file)
        n = min(ref.n, max(0, len(d) // 2 - ref.off))
        return list(struct.unpack_from("<%dH" % n, d, ref.off * 2)) + [0] * (ref.n - n)

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
        raise err("c.badkind", k)

    def _page_surface(self, name, ld, idx, crop=None, pal=None, title=None):
        p = self.page_info(name, ld, idx)
        s = Surface(self, title or name, p["bpp"])
        s.layers = [(name, 0, p["w"], p["h"])]
        s.pageinfo = p
        s.crop = crop or (0, 0, p["w"] * (4, 2, 1)[(4, 8, 16).index(p["bpp"])] * 1, p["h"])
        s.W, s.H = s.crop[2], s.crop[3]
        s.ld, s.idx = ld, idx
        s.opaque = False
        if p["bpp"] == 8 and p["clut"]:
            s.pal = self.pal_choice.get(name) or PalRef(p["clut"]["name"], 0, 256)
        elif p["bpp"] == 8 and ld == "SYS" and self.backdrop(idx):
            bd = self.backdrop(idx)
            s.pal = self.pal_choice.get(name) or bd["ref"]
            s.pal_note = tr("l.deduced", bd["label"], "backdrop")
        elif p["bpp"] == 4:
            s.pal = self.pal_choice.get(name) or pal
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
        if pal:
            s.pal = pal
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

    def _screen_surface(self, name):
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
        x0 = min(p["x"] for p in recs)
        s = Surface(self, "backdrop_L%d" % k, 8)
        s.layers = [(p["name"], (p["x"] - x0) * 2, p["w"], p["h"]) for p in recs]
        s.crop = (0, 0, max((p["x"] - x0 + p["w"]) * 2 for p in recs), max(p["h"] for p in recs))
        s.W, s.H = s.crop[2], s.crop[3]
        s.pal = self.pal_choice.get(recs[0]["name"]) or bd["ref"]
        s.pal_note = tr("l.deduced", bd["label"], "backdrop")
        s.opaque = False
        s.pageinfo = recs[0]
        s.ld, s.idx = "SYS", k
        return s

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
        return s

    def anim_data(self):
        """(catalogue, anim.Sprites) of Tombi's animations (tools/anim.py), cached."""
        if not getattr(self, "_anim", None):
            import anim
            self._anim = (anim.catalog(self.disc.raw), anim.Sprites())
        return self._anim

    def _anim_nodes(self):
        cat = self.anim_data()[0]
        return [dict(text="%02d  %s" % (a["id"], a["name"]), item=("anim", a["id"]), children=[], lazy=None)
                for a in cat if a["alias_of"] is None]

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
        N = lambda text, item=None, children=None, lazy=None: dict(text=text, item=item, children=children or [], lazy=lazy)
        root = []
        scr = [N(sc["name"], ("screen", sc["name"])) for sc in screens_list()] + [N(e[0], ("screen", e[0])) for e in EXTRA_SCREENS]
        import palinfer
        scr.append(N(tr("t.backdrops"), children=[N("L%d  %s" % (k, palinfer.backdrop(self, k)["name"]), ("screen", "backdrop_L%d" % k))
                                                 for k in palinfer.backdrop_lists(self)]))
        root.append(N(tr("t.screens"), children=scr))
        ui = [N(tr("t.uitable"), children=[N("%d  %s  (%d,%d %dx%d)" % (i, UI_NAMES.get(i, "UI %d" % i), u, v, w, h), ("ui", i))
                                           for i, u, v, w, h, cx, cy in self.ui_table()])]
        for nm in ("SYSTEM/A00000.GAM", UI_PAGE):
            ui.append(N(nm.split("/")[-1] + "  " + tr("t.hudpage"), ("page", nm, "SYS", 0)))
        root.append(N(tr("t.ui"), children=ui))
        pg = []
        for ld in sorted(self.lists, key=lambda x: (x != "SYS", x)):
            grp = N(self.area_name(ld), children=[])
            for k, lst in enumerate(self.lists[ld]):
                pages = [(r, bpp) for i, r, bpp, cl in self.grpx(lst) if bpp != 16]
                if not pages:
                    continue
                grp["children"].append(N(tr("t.list", k), children=[
                    N("%s  (%dx%d, %d bpp)" % (r["name"].split("/")[-1], r["w"] * (4 if bpp == 4 else 2), r["h"], bpp), ("page", r["name"], ld, k))
                    for r, bpp in pages]))
            pg.append(grp)
        root.append(N(tr("t.pages"), children=pg))
        root.append(N(tr("t.sprites"), lazy=self._sprite_nodes))
        root.append(N(tr("t.anims"), lazy=self._anim_nodes))
        pal = []
        for ld in sorted(self.lists, key=lambda x: (x != "SYS", x)):
            names = []
            for lst in self.lists[ld]:
                for i, r, bpp, cl in self.grpx(lst):
                    if bpp == 16 and r["name"] not in names:
                        names.append(r["name"])
            if names:
                pal.append(N(self.area_name(ld), children=[N(n.split("/")[-1], ("clut", n)) for n in sorted(names)]))
        root.append(N(tr("t.palettes"), children=pal))
        return root

    def _sprite_nodes(self):
        import wpp
        N = lambda text, item=None, children=None, lazy=None: dict(text=text, item=item, children=children or [], lazy=lazy)
        out = []
        seen = set()
        for ld, lists in sorted(self.lists.items()):
            for lst in lists:
                for r in lst:
                    nm = r["name"]
                    if (r["kind"] & 0xF0) == 0x60 and r["type"] != 0xFFFFFFFF and nm.endswith(".000") and nm not in seen and wpp.is_wpp(nm):
                        seen.add(nm)
                        out.append(N(nm, lazy=(lambda nm=nm: self._frame_nodes(nm))))
        return out

    def _frame_nodes(self, nm):
        man, files, frames = self.wpp(nm)
        out = []
        for k in sorted(frames):
            n = int(k.split("_")[1][:4])
            w, h = struct.unpack_from("<HH", frames[k])
            out.append(dict(text="#%d  (%dx%d)" % (n, w * 4, h), item=("wppframe", nm, n), children=[], lazy=None))
        return out

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

    def candidates(self):
        if self.bpp == 16:
            return []
        return self.core.palette_candidates(self.bpp, self.ld, self.idx)

    def _layer_planes(self):
        out = []
        for name, x0, w, h in self.layers:
            d = self.core.get(name)
            if self.header:
                d = d[self.header:]
            out.append((name, x0, w, h, decode_plane(d, w, h, self.bpp)))
        return out

    def plane(self):
        """The cropped W x H plane (bytearray of indices, or array('H') for 16 bpp)."""
        cx, cy, cw, ch = self.crop
        lp = self._layer_planes()
        pw = lambda l: l[2] * (4 if self.bpp == 4 else 2 if self.bpp == 8 else 1)
        full_w = max(l[1] + pw(l) for l in lp)
        full_h = max(l[3] for l in lp)
        full = (array("H", bytes(2 * full_w * full_h)) if self.bpp == 16 else bytearray(full_w * full_h))
        for name, x0, w, h, pl in lp:
            wp = pw((name, x0, w))
            for y in range(h):
                full[y * full_w + x0:y * full_w + x0 + wp] = pl[y * wp:(y + 1) * wp]
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
        for name, x0, lw, lh, pl in self._layer_planes():
            wp = lw * (4 if self.bpp == 4 else 2 if self.bpp == 8 else 1)
            ax, bx = max(X, x0), min(X + w, x0 + wp)
            ay, by = max(Y, 0), min(Y + h, lh)
            if ax >= bx or ay >= by:
                continue
            for yy in range(ay, by):
                src = (yy - Y) * w + (ax - X)
                pl[yy * wp + (ax - x0):yy * wp + (bx - x0)] = flat[src:src + (bx - ax)]
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
        if region:
            x, y, w, h = region
            im = im.crop((x, y, x + w, y + h))
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


# ------------------------------------------------------------------ strings
S_ES = {
    "l.deduced": "paleta deducida: {0} ({1})",
    "c.notiso": "{0} no es una imagen de disco ISO9660 (.bin de 2352 bytes por sector o .iso).", "c.nofile": "No existe {0}.",
    "c.nofs": "No se encuentra la carpeta SYS en {0}: ¿es el disco de Tombi! (PAL)?", "c.nolists": "No se pudieron leer las listas de carga (SYS/LD0*.BIN).",
    "c.nores": "No se pudo leer {0}.", "c.size": "{0}: tamaño {1} distinto del original {2}.", "c.nopage": "No hay ninguna página llamada {0}.",
    "c.badkind": "Elemento desconocido: {0}", "c.notwpp": "{0} no es un archivo de sprites WPP.", "c.nomatch": "El patrón {0} no coincide con ningún archivo.",
    "c.wppsize": "{0}: el archivo de sprites recodificado mide {1} bytes y el original {2}; el cambio no cabe (usa menos detalle/colores repetidos).",
    "c.nochanges": "No hay cambios que aplicar.", "c.needimage": "Hace falta la imagen original (.bin) para generar la ISO: ábrela en lugar de la carpeta.",
    "c.space": "Poco espacio libre: la ISO necesita unos {0} MB y hay {1} MB.", "c.mkfail": "mkpsxiso falló: {0}\n{1}",
    "c.bigimg": "La imagen ({0}x{1}) es mayor que el destino ({2}x{3}).",
    "pal.row": "{0}  fila {1}", "pal.sub": "{0}  fila {1} · paleta {2}",
    "t.screens": "Pantallas", "t.backdrops": "Fondos de sala (mapas)", "t.ui": "Interfaz / HUD", "t.uitable": "Tabla UiSpriteDef (MAIN0)", "t.hudpage": "(página HUD)", "t.pages": "Páginas por lista de carga",
    "t.list": "Lista {0}", "t.anims": "Animaciones (Tombi)", "t.sprites": "Sprites (WPP)", "t.palettes": "Paletas (CLUT)",
    "w.smaller": "La imagen ({0}x{1}) es menor que el destino ({2}x{3}): se coloca arriba a la izquierda.",
    "w.palmismatch": "El PNG es indexado pero su paleta no coincide con la actual: se ajusta al color más cercano.",
    "w.nozero": "La paleta no tiene ningún color 0x0000 (transparente): los píxeles transparentes usan el índice 0.",
    "w.morecolours": "La imagen usa {0} colores y la paleta solo admite {1}: se aproximaron al más cercano.",
    "w.approx": "Algunos colores se aproximaron (error máximo {0} de 441).",
    "w.index0": "El índice 0 de esta paleta no es transparente (palabra distinta de 0): se verá como color opaco.",
}
S_EN = {
    "l.deduced": "palette inferred: {0} ({1})",
    "c.notiso": "{0} is not an ISO9660 disc image (.bin with 2352-byte sectors or .iso).", "c.nofile": "{0} does not exist.",
    "c.nofs": "No SYS folder found in {0}: is it the Tombi! (PAL) disc?", "c.nolists": "The load lists (SYS/LD0*.BIN) could not be read.",
    "c.nores": "Could not read {0}.", "c.size": "{0}: size {1} differs from the original {2}.", "c.nopage": "There is no page named {0}.",
    "c.badkind": "Unknown item: {0}", "c.notwpp": "{0} is not a WPP sprite file.", "c.nomatch": "Pattern {0} matches no file.",
    "c.wppsize": "{0}: the re-encoded sprite file is {1} bytes, the original {2}; the change does not fit (use less detail / repeated colours).",
    "c.nochanges": "There are no changes to apply.", "c.needimage": "The original image (.bin) is needed to build the ISO: open it instead of the folder.",
    "c.space": "Not enough free space: the ISO needs about {0} MB and {1} MB are free.", "c.mkfail": "mkpsxiso failed: {0}\n{1}",
    "c.bigimg": "The image ({0}x{1}) is larger than the target ({2}x{3}).",
    "pal.row": "{0}  row {1}", "pal.sub": "{0}  row {1} · palette {2}",
    "t.screens": "Screens", "t.backdrops": "Room backdrops (maps)", "t.ui": "Interface / HUD", "t.uitable": "UiSpriteDef table (MAIN0)", "t.hudpage": "(HUD page)", "t.pages": "Pages by load list",
    "t.list": "List {0}", "t.anims": "Animations (Tombi)", "t.sprites": "Sprites (WPP)", "t.palettes": "Palettes (CLUT)",
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
