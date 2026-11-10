#!/usr/bin/env python3
"""The game's title logo as a PNG with transparency, read from the player's own disc: for the banner of the 3DS title
(.github/workflows/3ds.yml).  Nothing of it is in the repository.

  tools/n3ds_banner_logo.py FS_DIR OUT.png

SYSTEM/A00202.000 is the title's texture page (256 x 256, 8 bits per pixel, raw: the game uploads it to VRAM 256,0) and
SYSTEM/A00203.000 its 256 colours (15-bit words, 0 = transparent).  The title screen draws the logo as two sprites of that page
side by side (240 x 112 from texel 8,0 and 48 x 112 from 8,112): put together the same way here.
"""
import os, struct, sys, zlib


def find(fs, name):
    for root, _, files in os.walk(fs):
        for f in files:
            if f.upper() == name and os.path.basename(root).upper() == "SYSTEM":
                return os.path.join(root, f)
    sys.exit("not found on the disc: SYSTEM/" + name)


def png(path, w, h, rgba):
    def chunk(t, d):
        return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d))
    raw = b"".join(b"\0" + bytes(rgba[y * w * 4:(y + 1) * w * 4]) for y in range(h))
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0)) + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def main():
    fs, out = sys.argv[1], sys.argv[2]
    pix = open(find(fs, "A00202.000"), "rb").read()
    pal = struct.unpack("<256H", open(find(fs, "A00203.000"), "rb").read()[:512])
    if len(pix) != 65536:
        sys.exit("unexpected size of A00202.000")
    rgba = []
    for c in pal:
        rgba.append((0, 0, 0, 0) if c == 0 else ((c & 31) * 255 // 31, (c >> 5 & 31) * 255 // 31, (c >> 10 & 31) * 255 // 31, 255))
    W, H = 240 + 48, 112          # the title draws two sprites side by side: 240 x 112 from (8, 0), 48 x 112 from (8, 112)
    img = bytearray(W * H * 4)
    for y in range(H):
        for x in range(W):
            i = pix[y * 256 + 8 + x] if x < 240 else pix[(112 + y) * 256 + 8 + (x - 240)]
            img[(y * W + x) * 4:(y * W + x) * 4 + 4] = bytes(rgba[i])
    # crop to what is drawn
    xs = [x for y in range(H) for x in range(W) if img[(y * W + x) * 4 + 3]]
    ys = [y for y in range(H) for x in range(W) if img[(y * W + x) * 4 + 3]]
    x0, x1, y0, y1 = min(xs), max(xs) + 1, min(ys), max(ys) + 1
    crop = bytearray()
    for y in range(y0, y1):
        crop += img[(y * W + x0) * 4:(y * W + x1) * 4]
    png(out, x1 - x0, y1 - y0, crop)
    print("%s: %d x %d" % (out, x1 - x0, y1 - y0))


if __name__ == "__main__":
    main()
