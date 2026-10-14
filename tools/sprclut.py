#!/usr/bin/env python3
"""Tombi! (PAL) per frame texture page offset and CLUT of SPR frames, as the game code sets them (used by tools/vram.py).

An SPR part is a POLY_FT4 template `u0 v0 clut | u1 v1 tpage | ...`.  `FUN_80052db8` (src/FUN_80052db8.c), the drawer of every object that
uses an SPR table, copies it into the primitive and then

    p->tpage += o->w1e;            per object texture page offset
    if (o->b0d & 1) p->clut = o->w08;   per object CLUT replaces the one of the template
    SetSemiTrans(p, o->b0d >> 7);

so the same frame can come out of different pages with different palettes.  The table alone does not say which; the object init code does.

1. Items (`initItemObject`, src/initItemObject.c, MAIN0 0x8003C980).  Every pick-up / reward / inventory icon object is set up from a
   descriptor `DAT_8007B14C[DAT_8007B084[subtype]]` = `{b0, b1, b2 = w1e (tpage offset), b3, b4 = SPR_DATA index (table = load kind
   0x30 + b4), b5 = b0d (bit 0: use w08), b6 = CLUT mode, b7 = anim mode; s16 cx, cy; 4 box bytes; void **anims}`:
     b6 0: w08 = GetClut(cx, cy)          1: GetClut(cx, cy + variant)        3: GetClut(cx, cy + 0/1/2) (progress flags)
     b6 2: w08 = GetClut(DAT_8007B2D4[variant].x, .y)   (money bags: {x, y, value, pad} records)
     b7 0: o->anim = anims[0]             1: anims[variant]
   variant = TObj.b0c & 0x7F (placement data); we take variant 0, the one every placed item has unless the map says otherwise.
   The animation scripts (tools/anim.py `decode_script`) give the frame numbers each descriptor draws.

2. Tables shared by objects with different offsets (the resident HUD/item table SPR_DATA[4] is cut from `A10003` (tpage 0x14) and `A00000`
   (tpage 0x15)): every 4 bpp page of the resident list comes with its own 16 colour palette strips (SYS list 0: `A00000` -> `A00001`,
   `A00002`; `A10003` -> `A10004`, `A00005`: a 16 x n record belongs to the page record before it).  A part whose template CLUT lies in the
   strip of one page is drawn from that page, which fixes the offset of frames no descriptor names (`strip_owner`).

  tools/sprclut.py items        list the item descriptors and the frames / offset / CLUT they give
"""
import os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)

ITEM_INDEX = 0x8007B084      # u8 per subtype (200) -> descriptor number
ITEM_DESCS = 0x8007B14C      # Desc * per descriptor
ITEM_CLUTPOS = 0x8007B2D4    # {s16 x, y, value, pad} per money bag variant (b6 == 2)
N_SUBTYPES = 0xC8
OVERLAY_BASE = 0x800E8028


def clut_word(x, y):
    return (x >> 4) & 0x3F | (y & 0x1FF) << 6


class _Mem:
    """MAIN0.EXE (+ the area overlay X0nn.BIN at 0x800E8028) as a read-only memory."""

    def __init__(self, reader, area=None):
        d = reader("EXE/MAIN0.EXE")
        self.d, self.base = d, struct.unpack_from("<I", d, 0x18)[0]
        self.ov = None
        if area is not None:
            try:
                self.ov = reader("AREA%02d/X0%02d.BIN" % (area, area))
            except OSError:
                self.ov = None

    def _at(self, a, n):
        o = a - self.base + 0x800
        if 0x800 <= o and o + n <= len(self.d):
            return self.d, o
        if self.ov is not None and 0 <= a - OVERLAY_BASE and a - OVERLAY_BASE + n <= len(self.ov):
            return self.ov, a - OVERLAY_BASE
        raise KeyError(hex(a))

    def u8(self, a):
        b, o = self._at(a, 1)
        return b[o]

    def u16(self, a):
        b, o = self._at(a, 2)
        return struct.unpack_from("<H", b, o)[0]

    def s16(self, a):
        b, o = self._at(a, 2)
        return struct.unpack_from("<h", b, o)[0]

    def u32(self, a):
        b, o = self._at(a, 4)
        return struct.unpack_from("<I", b, o)[0]


def _reader(fs=None):
    if fs is None:
        import area000 as A
        fs = A.FS
    return lambda name: open(os.path.join(fs, name), "rb").read()


def item_descs(reader=None, area=None):
    """[dict(desc, subtypes, base, table, b0d, clut, mode, anim_mode, anims)] of the item descriptors (initItemObject)."""
    m = _Mem(reader or _reader(), area)
    idx = [m.u8(ITEM_INDEX + i) for i in range(N_SUBTYPES)]
    out = []
    for k in range(max(idx) + 1):
        p = m.u32(ITEM_DESCS + 4 * k)
        b = [m.u8(p + i) for i in range(8)]
        cx, cy = m.s16(p + 8), m.s16(p + 10)
        mode = b[6]
        if mode == 2:
            cx, cy = m.s16(ITEM_CLUTPOS), m.s16(ITEM_CLUTPOS + 2)
        out.append(dict(desc=k, subtypes=[i for i, v in enumerate(idx) if v == k], base=b[2], table=0x30 + b[4], b0d=b[5],
                        clut=clut_word(cx, cy) if b[5] & 1 else None, mode=mode, anim_mode=b[7], anims=m.u32(p + 0x10)))
    return out, m


def item_frames(kind, reader=None, area=None, nframes=None):
    """{frame: dict(base, clut (None = keep the template's), semi, descs)} for SPR table `kind` (0x30 + SPR_DATA index) from the item
    descriptors whose variant-0 animation script draws that frame.  Scripts in the area overlay are read from X0nn.BIN of `area`."""
    import anim
    descs, m = item_descs(reader, area)
    out = {}
    for d in descs:
        if d["table"] != kind:
            continue
        try:
            s = m.u32(d["anims"])
            sc = anim.decode_script(m, s)
        except (KeyError, struct.error):
            continue
        frames = sorted({e["frame"] for e in sc["entries"]})
        if not frames or (nframes is not None and max(frames) >= nframes) or any(e["dur"] == 0 for e in sc["entries"][:-1]):
            continue          # not a script of this overlay (the descriptor belongs to another area)
        for f in frames:
            r = out.get(f)
            if r is None:
                out[f] = dict(base=d["base"], clut=d["clut"], semi=d["b0d"] >> 7, descs=[d["desc"]])
            elif (r["base"], r["clut"]) == (d["base"], d["clut"]):
                r["descs"].append(d["desc"])
            else:
                r.setdefault("other", []).append(dict(desc=d["desc"], base=d["base"], clut=d["clut"]))
    return out


def strip_pages(recs):
    """{(x, y, w, h) of a 16 x n palette strip: (x, y, w, h) of the page record it belongs to}; recs = Vram.recs (load order)."""
    out, page = {}, None
    for name, x, y, w, h, kind in recs:
        if kind == "page":
            page = (x, y, w, h)
        elif w == 16 and page is not None:
            out[(x, y, w, h)] = page
    return out


def strip_owner(recs, clut):
    """VRAM rectangle (x, y, w, h) of the page whose palette strip holds the CLUT word, or None (later records win, as in VRAM)."""
    x, y = (clut & 0x3F) * 16, clut >> 6 & 0x1FF
    own = None
    for (sx, sy, sw, sh), pg in strip_pages(recs).items():
        if sx <= x < sx + sw and sy <= y < sy + sh:
            own = pg
    return own


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=("items",))
    ap.add_argument("--fs")
    ap.add_argument("--area", type=int)
    a = ap.parse_args(argv)
    rd = _reader(a.fs) if a.fs else None
    descs, _ = item_descs(rd, a.area)
    for d in descs:
        c = d["clut"]
        print("desc %2d subtypes %-14s table 0x%02X tpage +0x%02X clut %s anims %08X" % (
            d["desc"], ",".join(map(str, d["subtypes"][:4])), d["table"], d["base"],
            "(%d,%d)" % ((c & 63) * 16, c >> 6) if c is not None else "template", d["anims"]))
    for kind in (0x33, 0x34):
        fr = item_frames(kind, rd, a.area)
        print("table 0x%02X: %d frames named by items: %s" % (kind, len(fr), sorted(fr)))


if __name__ == "__main__":
    main()
