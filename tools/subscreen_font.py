#!/usr/bin/env python3
"""Two TrueType fonts from the Select sub-screen's lettering: "Tombi Plank" (the wooden tabs' font) and "Tombi Leaf" (the
leaf buttons'), built on your own machine from your own disc (nothing of the disc ships: the repo holds coordinates and
our own letters only).  Every letter the nine language pages hold (tools/subscreen_glyphs.json), our hand-drawn ones for
the rest (tools/subscreen_extra_glyphs.py) and á é í ó ú ñ with their capitals, as pixel outlines (one texel = 64 units,
1024 units per em), the ink only: colour and shading are yours to add.

  python3 tools/subscreen_font.py --data data -o fonts/            -> fonts/TombiPlank.ttf, fonts/TombiLeaf.ttf
  python3 tools/subscreen_font.py --disc Tombi.bin -o fonts/

Needs fontTools (pip install fonttools)."""
import argparse, json, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import subscreen_glyphs as G
import subscreen_extra_glyphs as E

PX = 64                         # font units per texel
ACCENTS = {"á": ("a", "acute"), "é": ("e", "acute"), "í": ("i", "acute"), "ó": ("o", "acute"), "ú": ("u", "acute"),
           "Á": ("A", "acute"), "É": ("E", "acute"), "Í": ("I", "acute"), "Ó": ("O", "acute"), "Ú": ("U", "acute"),
           "ñ": ("n", "tilde"), "Ñ": ("N", "tilde")}
NAMES = {"tab": ("Tombi Plank", "TombiPlank"), "leaf": ("Tombi Leaf", "TombiLeaf")}


def disc_ink(table, pages, cleans, fn, ch):
    """{(x, y)} of a disc letter's ink, x from its left, y from its baseline (negative above); None if not there"""
    g = table["fonts"][fn]["glyphs"].get(ch)
    if not g:
        return None
    tx = G.glyph_texels(pages[g["file"]], fn, g["slot"], g, cleans[(fn, g["slot"])])
    ink = {(x, y) for (x, y), t in tx.items() if t in G.FONTS[fn]["text"]}
    x0 = min(x for x, _ in ink)
    return {(x - x0, y - g["baseline"]) for x, y in ink}


def extra_ink(fn, ch):
    tbl = E.TAB if fn == "tab" else E.LEAF
    if ch not in tbl:
        return None
    bl, rows = tbl[ch]
    return {(x, y - bl) for y, r in enumerate(rows) for x, c in enumerate(r) if c == "#"}


def ink_of(table, pages, cleans, fn, ch):
    if ch in ACCENTS:
        base, mark = ACCENTS[ch]
        ink = ink_of(table, pages, cleans, fn, base)
        if ink is None:
            return None
        ink = set(ink)
        if base in "ij":                     # the dot gives way to the mark
            comps = []
            pts = set(ink)
            while pts:
                stack = [pts.pop()]; comp = set()
                while stack:
                    x, y = stack.pop(); comp.add((x, y))
                    for dx in (-1, 0, 1):
                        for dy in (-1, 0, 1):
                            if (x + dx, y + dy) in pts:
                                pts.discard((x + dx, y + dy)); stack.append((x + dx, y + dy))
                comps.append(comp)
            ink = max(comps, key=len)
        rows = E.ACUTE if mark == "acute" else E.TILDE
        top = min(y for _, y in ink); x0 = min(x for x, _ in ink); x1 = max(x for x, _ in ink)
        mx = x0 + (x1 - x0 + 1 - len(rows[0])) // 2 + 1; my = top - 1 - len(rows)
        ink |= {(mx + x, my + y) for y, r in enumerate(rows) for x, c in enumerate(r) if c == "#"}
        return ink
    ink = disc_ink(table, pages, cleans, fn, ch)
    return ink if ink is not None else extra_ink(fn, ch)


def outline(pen, ink):
    """One rectangle per horizontal run of texels (y up in font units; texel row y covers -(y+1)*PX .. -y*PX)."""
    for y in sorted({y for _, y in ink}):
        xs = sorted(x for x, yy in ink if yy == y)
        runs, start = [], xs[0]
        for a, b in zip(xs, xs[1:] + [None]):
            if b != a + 1:
                runs.append((start, a)); start = b
        for x0, x1 in runs:   # rows overlap by a hair so rasterisers draw no seam between them
            l, r, bottom, top = x0 * PX, (x1 + 1) * PX, -(y + 1) * PX - 2, -y * PX + 2
            pen.moveTo((l, bottom)); pen.lineTo((l, top)); pen.lineTo((r, top)); pen.lineTo((r, bottom)); pen.closePath()


def build_font(table, pages, cleans, fn, out_dir):
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    chars = sorted(set(table["fonts"][fn]["glyphs"]) | set(E.TAB if fn == "tab" else E.LEAF))
    chars = [c for c in chars if c.strip()] + list(ACCENTS)
    glyphs, metrics, cmap, order = {".notdef": TTGlyphPen(None).glyph(), "space": TTGlyphPen(None).glyph()}, {".notdef": (6 * PX, 0), "space": (4 * PX, 0)}, {0x20: "space"}, [".notdef", "space"]
    for ch in chars:
        ink = ink_of(table, pages, cleans, fn, ch)
        if not ink:
            continue
        name = "uni%04X" % ord(ch)
        pen = TTGlyphPen(None)
        outline(pen, ink)
        glyphs[name] = pen.glyph(); metrics[name] = ((max(x for x, _ in ink) + 2) * PX, 0); cmap[ord(ch)] = name; order.append(name)
    full, ps = NAMES[fn]
    fb = FontBuilder(1024, isTTF=True)
    fb.setupGlyphOrder(order); fb.setupCharacterMap(cmap); fb.setupGlyf(glyphs); fb.setupHorizontalMetrics(metrics)
    fb.setupHorizontalHeader(ascent=15 * PX, descent=-5 * PX)
    fb.setupNameTable({"familyName": full, "styleName": "Regular", "fullName": full, "psName": ps,
                       "uniqueFontIdentifier": ps + ";Tombi-Decomp", "version": "Version 1.0",
                       "description": "Lettering of Tombi!'s Select sub-screen, built from the owner's own disc; missing letters drawn by the Tombi-Decomp project"})
    fb.setupOS2(sTypoAscender=15 * PX, sTypoDescender=-5 * PX, usWinAscent=15 * PX, usWinDescent=5 * PX, sxHeight=9 * PX, sCapHeight=13 * PX)
    fb.setupPost()
    path = os.path.join(out_dir, ps + ".ttf")
    fb.save(path)
    return path, len(order) - 2


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--data", default=os.path.join(os.path.dirname(HERE), "data"))
    ap.add_argument("--disc")
    ap.add_argument("-o", "--out", default=".", help="output folder")
    a = ap.parse_args(argv)
    try:
        import fontTools  # noqa: F401
    except ImportError:
        print("fontTools is needed: pip install fonttools", file=sys.stderr); return 2
    pages = G.read_pages(a.data, a.disc)
    if not pages:
        print("no sub-screen pages found (data/SYSTEM/A?0301.000)", file=sys.stderr); return 1
    table = json.load(open(os.path.join(HERE, "subscreen_glyphs.json")))
    _, cleans = G.build(pages)
    os.makedirs(a.out, exist_ok=True)
    for fn in ("tab", "leaf"):
        path, n = build_font(table, pages, cleans, fn, a.out)
        print("%s: %d letters" % (path, n))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
