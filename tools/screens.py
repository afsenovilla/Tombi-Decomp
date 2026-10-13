#!/usr/bin/env python3
"""Tombi! (PAL) static full-screen images as PNG (title screen, logos, "presents" screen, world map, ...).

Unlike the page dumps of tools/vram.py (OUT/vram: every GRPX record as one VRAM page), these are the pictures the game shows as they are meant to
be seen: the right bit depth, the real width/height (not the 256 word LoadImage pitch), strips of one picture stitched, the CLUT the load list pairs
with them.  Every image is an 8 bpp page of the SYSTEM load lists (`SYS/LD0SYS.BIN`) with a 256 colour CLUT record (256 x 1 words) that
the game uploads next to it; `arg = y << 16 | x` of the page record says where in VRAM it lives, which gives the stitching order.

  tools/screens.py OUT/screens [--fs DIR]
"""
import os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vram

RGB = vram.RGB

# name -> (pages [(file, VRAM x in words)], CLUT file, (x, y, w, h) crop in pixels or None, opaque)
# w/h of the crops are the real picture sizes: the landscape is stored as 256 word rows (512 px pitch) but only 160 words (320 px) x 240 rows
# are image data; the PAL game shows 640 x 512 (Sony screen = full size, 320 words x 512 rows).
STATIC = [
    ("sony_computer_entertainment_presents", [("SYSTEM/A09400.GAM", 0)], "SYSTEM/A09401.000", None, True),
    ("whoopee_camp_logo", [("SYSTEM/A00100.GAM", 0)], "SYSTEM/A00101.000", (0, 0, 576, 128), True),
    ("title_landscape", [("SYSTEM/A00200.GAM", 0)], "SYSTEM/A00201.000", (0, 0, 320, 240), True),
    ("title_logo_page", [("SYSTEM/A00202.000", 256)], "SYSTEM/A00203.000", (0, 0, 256, 224), False),
    ("worldmap_texture", [("SYSTEM/A00701.000", 192)], "SYSTEM/A00702.000", None, True),
    ("menu_atlas_save_load", [("SYSTEM/A00400.000", 64), ("SYSTEM/A00401.000", 256)], "SYSTEM/A00402.000", None, False),
    ("menu_atlas_info", [("SYSTEM/A00500.000", 64), ("SYSTEM/A00501.000", 256)], "SYSTEM/A00502.000", None, False),
]
# 4 bpp font/HUD sheet of the title list, one tile per palette of the 16 x n strips that come with it
FONT_SHEET = ("title_font_sheet", "SYSTEM/A00204.000", 64, ["SYSTEM/A00205.000", "SYSTEM/A00206.000"], (0, 0, 256, 160))


def _words(d):
    return memoryview(bytes(d[:len(d) // 2 * 2])).cast("H")


def render(pages, clut, crop, opaque):
    """pages = [(bytes, x words, w words, h)], clut = 256 words.  Returns PNG bytes."""
    x0 = min(p[1] for p in pages)
    x1 = max(p[1] + p[2] for p in pages)
    h = max(p[3] for p in pages)
    W = (x1 - x0) * 2
    rows = [bytearray(W) for _ in range(h)]
    for d, x, w, ph in pages:
        for y in range(ph):
            rows[y][(x - x0) * 2:(x - x0 + w) * 2] = d[y * w * 2:(y + 1) * w * 2]
    cx, cy, cw, ch = crop or (0, 0, W, h)
    rgb, a = [], []
    for y in range(cy, cy + ch):
        r = rows[y][cx:cx + cw]
        if opaque:
            rgb.append(b"".join(RGB[clut[i]] for i in r))
        else:
            rgb.append(b"".join(RGB[clut[i]] + (b"\0" if clut[i] == 0 else b"\xff") for i in r))
    return (vram.png_rgb if opaque else vram.png_rgba)(cw, ch, rgb)


def font_sheet(name, page, wwords, strips, crop):
    d = vram.resource(page)
    if d is None:
        return None
    cx, cy, cw, ch = crop
    pals = []
    for s in strips:
        c = vram.resource(s)
        if c is None:
            continue
        ws = _words(c)
        pals += [list(ws[i:i + 16]) for i in range(0, len(ws) - 15, 16)]
    if not pals:
        return None
    cols = 4
    nrows = (len(pals) + cols - 1) // cols
    W, H = cw * cols, ch * nrows
    img = [bytearray(W * 4) for _ in range(H)]
    bg = (64, 64, 96, 255)
    for r in img:
        r[:] = bytes(bg) * W
    for k, p in enumerate(pals):
        ox, oy = (k % cols) * cw, (k // cols) * ch
        for y in range(ch):
            row = d[(cy + y) * wwords * 2:(cy + y + 1) * wwords * 2]
            line = img[oy + y]
            for x in range(cw):
                b = row[(cx + x) >> 1]
                v = p[(b >> 4) if (cx + x) & 1 else b & 15]
                if v:
                    line[(ox + x) * 4:(ox + x) * 4 + 4] = RGB[v] + b"\xff"
    return vram.png_rgba(W, H, [bytes(r) for r in img])


def export_screens(out, log=lambda m: None):
    """Writes OUT/<name>.png for every static screen whose files exist; returns how many."""
    os.makedirs(out, exist_ok=True)
    n = 0
    for name, pages, clutf, crop, opaque in STATIC:
        c = vram.resource(clutf)
        ps = []
        for f, x in pages:
            d = vram.resource(f)
            if d is None or c is None:
                ps = None
                break
            h = len(d) // 2 // _pagew(f, pages)
            ps.append((d, x, _pagew(f, pages), h))
        if not ps:
            continue
        data = render(ps, list(_words(c)[:256]), crop, opaque)
        vram.write(os.path.join(out, name + ".png"), data)
        n += 1
        log(name)
    fs = font_sheet(*FONT_SHEET)
    if fs:
        vram.write(os.path.join(out, FONT_SHEET[0] + ".png"), fs)
        n += 1
    return n


def _pagew(f, pages):
    """Width in words of a page record (from the SYSTEM load lists; the record says w x h, the file is exactly w*h*2 bytes after unpacking)."""
    for lst in vram.A.load_lists().get("SYS", []):
        for r in lst:
            if r["name"] == f and vram.is_grpx(r):
                return r["w"]
    raise KeyError(f)


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("--fs")
    ap.add_argument("--img")
    a = ap.parse_args(argv)
    if a.fs:
        vram.A.FS = os.path.abspath(a.fs)
    if a.img:
        vram.A.IMG = os.path.abspath(a.img)
    print(export_screens(a.out, log=print), "screens")


if __name__ == "__main__":
    main()
