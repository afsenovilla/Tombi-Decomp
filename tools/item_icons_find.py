#!/usr/bin/env python3
"""Where a demo / prototype disc keeps the inventory item icons, found by their pixels: every 16 x 16 icon cell of the
retail PAL item page (SYSTEM/A00300.000 words 64..127, cell n at (n % 15 * 16 + 8, n / 15 * 16 + 8), 4 bpp) is looked
for in every file of the demo (GAM files unpacked too), at any page width, and the pages that hold them are drawn.
Reads only; the PNGs it writes are disc graphics and never belong in the repo (CI keeps them as run artifacts).

  python3 -I tools/item_icons_find.py DEMO_DIR [--pal game/disc/fs] [-o out] [--min-rows 12]

Prints one line per retail item (id, name, file, offset of the cell's first row, page width, rows that matched) and,
per page found, writes out/<file>_<width>.png (the whole page, grey) and out/<file>_<width>_items.png (the matched
cells in their retail colours, labelled with the item id) plus the cells of that page no retail icon matched."""
import argparse, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import area000      # noqa: E402
import tombi_save   # noqa: E402

PAL_STRIDE = 384                 # bytes per row of A00300.000 (192 words)
PAL_ITEM_X = 128                 # byte offset of VRAM word 64 in a row
CLUT_BLOCK, CLUT_COL, CLUTS_OFF = 0x98B8, 16, 0x21C60   # D0SPSUB: 48 x 31 colour block at (288, 480); item rows at x 304
STRIDES = [w * 2 for w in (32, 48, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 512, 640, 768, 1024)]


def rgb(v):
    return ((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31, 0 if v == 0 else 255)


def pal_cells(fs):
    """{n: [16 rows of 8 bytes]} of the retail item icons, empty cells left out; and {n: 16 rgba} their palettes."""
    pg = open(os.path.join(fs, "SYSTEM", "A00300.000"), "rb").read()
    sub = open(os.path.join(fs, "SYSTEM", "D0SPSUB.BIN"), "rb").read()
    cells, pals = {}, {}
    for n in range(160):
        u0, v0 = n % 15 * 16 + 8, n // 15 * 16 + 8
        rows = [pg[(v0 + y) * PAL_STRIDE + PAL_ITEM_X + u0 // 2:][:8] for y in range(16)]
        if any(any(r) for r in rows):
            cells[n] = rows
            row = sub[CLUTS_OFF + n]
            o = CLUT_BLOCK + (row * 48 + CLUT_COL) * 2
            pals[n] = [rgb(struct.unpack_from("<H", sub, o + 2 * i)[0]) for i in range(16)]
    return cells, pals, tombi_save.item_names(sub)


def demo_files(root):
    for dp, _, fn in os.walk(root):
        for f in sorted(fn):
            p = os.path.join(dp, f)
            if os.path.getsize(p) > 24 << 20 or f.upper().endswith((".STR", ".XA", ".DA")):   # movies / audio
                continue
            d = open(p, "rb").read()
            rel = os.path.relpath(p, root)
            yield rel, d
            if d[:4] == b"GAM\0":
                try:
                    yield rel + "~", area000.gam_unpack(d)
                except Exception:
                    pass


def key_row(rows):
    """the row of the cell with most distinct bytes (least likely to be found by chance), and its index"""
    k = max(range(2, 14), key=lambda y: (len(set(rows[y])), -abs(y - 8)))
    return k, bytes(rows[k])


def find(cells, files, min_rows):
    """{n: (file, offset of row 0, stride, rows matched)} best hit per item, and the files' data"""
    hits, data = {}, {}
    for rel, d in files:
        data[rel] = d
        for n, rows in cells.items():
            ky, key = key_row(rows)
            i = d.find(key)
            while i >= 0:
                for st in STRIDES:
                    base = i - ky * st
                    if base < 0:
                        continue
                    m = sum(1 for y in range(16) if d[base + y * st:base + y * st + 8] == bytes(rows[y]))
                    if m >= min_rows and m > hits.get(n, (0, 0, 0, 0))[3]:
                        hits[n] = (rel, base, st, m)
                i = d.find(key, i + 1)
    return hits, data


def draw(out, rel, d, st, hits, cells, pals, names):
    from PIL import Image, ImageDraw
    tag = rel.replace(os.sep, "_").replace("~", "_unpacked")
    h = len(d) // st
    img = Image.new("RGB", (st * 2, h))
    px = img.load()
    for y in range(h):
        for x in range(st * 2):
            v = d[y * st + x // 2] >> 4 * (x & 1) & 15
            px[x, y] = (v * 17, v * 17, v * 17)
    dr = ImageDraw.Draw(img)
    mine = {n: hh for n, hh in hits.items() if hh[0] == rel and hh[2] == st}
    for n, (_, base, _, _) in mine.items():
        x, y = base % st * 2, base // st
        dr.rectangle([x - 1, y - 1, x + 16, y + 16], outline=(255, 0, 0))
    img.save(os.path.join(out, "%s_%d.png" % (tag, st * 2)))
    # every cell of the grid the matched icons lie on (16 px steps from the first one), in page order: the matched
    # ones in their retail colours, labelled with the item id, the others (other art, or empty) in grey, labelled "?"
    order = sorted(mine, key=lambda n: (mine[n][1] // st, mine[n][1] % st))
    x0, y0 = min(mine[n][1] % st for n in order), min(mine[n][1] // st for n in order)
    x1, y1 = max(mine[n][1] % st for n in order), max(mine[n][1] // st for n in order)
    at = {(mine[n][1] % st, mine[n][1] // st): n for n in order}
    cells = []
    for y in range(y0, y1 + 1, 16):
        for x in range(x0, x1 + 1, 8):
            base = y * st + x
            if (x, y) in at or any(d[base + yy * st:base + yy * st + 8].strip(b"\0") for yy in range(16)):
                cells.append((base, at.get((x, y))))
    cols, S = 15, 3
    sheet = Image.new("RGBA", (cols * 20 * S, ((len(cells) + cols - 1) // cols) * 26 * S), (40, 40, 40, 255))
    sp = sheet.load()
    dr = ImageDraw.Draw(sheet)
    grey = [(v * 17, v * 17, v * 17, 255 if v else 0) for v in range(16)]
    for k, (base, n) in enumerate(cells):
        cx, cy = k % cols * 20 * S + 2 * S, k // cols * 26 * S + 2 * S
        P = pals[n] if n is not None else grey
        for y in range(16):
            for x in range(16):
                c = P[d[base + y * st + x // 2] >> 4 * (x & 1) & 15]
                if c[3]:
                    for dy in range(S):
                        for dx in range(S):
                            sp[cx + x * S + dx, cy + y * S + dy] = c
        dr.text((cx, cy + 16 * S + 1), str(n) if n is not None else "?", fill=(255, 255, 255, 255))
    path = os.path.join(out, "%s_%d_items.png" % (tag, st * 2))
    sheet.save(path)
    return order, [base for base, n in cells if n is None], path


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("demo")
    ap.add_argument("--pal", default=os.path.join(os.path.dirname(HERE), "game", "disc", "fs"))
    ap.add_argument("-o", "--out", default="out")
    ap.add_argument("--min-rows", type=int, default=12)
    ap.add_argument("--b64", action="store_true", help="print the labelled sheets as base64 too (for a CI log)")
    a = ap.parse_args(argv)
    cells, pals, names = pal_cells(a.pal)
    hits, data = find(cells, demo_files(a.demo), a.min_rows)
    os.makedirs(a.out, exist_ok=True)
    print("retail item icons: %d; found on the disc: %d" % (len(cells), len(hits)))
    for n in sorted(cells):
        if n in hits:
            rel, base, st, m = hits[n]
            print("%3d %-28s %s +0x%X  page %d px wide, cell (%d, %d), %d/16 rows" % (n, names.get(n, ""), rel, base, st * 2, base % st * 2, base // st, m))
        else:
            print("%3d %-28s -" % (n, names.get(n, "")))
    pages = sorted({(hh[0], hh[2]) for hh in hits.values()})
    for rel, st in pages:
        order, other, path = draw(a.out, rel, data[rel], st, hits, cells, pals, names)
        print("page %s (%d px wide): %d icons, in page order: %s" % (rel, st * 2, len(order), " ".join(str(n) for n in order)))
        print("  cells of that grid with other art: %s" % " ".join("+0x%X (%d, %d)" % (b, b % st * 2, b // st) for b in other))
        if a.b64:
            import base64
            print("  %s as base64:\n%s" % (os.path.basename(path), base64.b64encode(open(path, "rb").read()).decode()))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
