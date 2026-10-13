#!/usr/bin/env python3
"""Generates tools/icons/{text,assets,gfx}.ico (+ .png, 64 px) with Pillow: original 16x16 pixel art scaled up with nearest
neighbour.  text = speech bubble with a "T", assets = picture frame with a sun and mountains, gfx = paint palette with a brush.  No game graphics."""
import os, struct, io
from PIL import Image

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "icons")
PAL = {".": (0, 0, 0, 0), "k": (28, 32, 48, 255), "w": (250, 246, 232, 255), "b": (70, 130, 220, 255), "y": (255, 205, 60, 255),
       "g": (80, 170, 90, 255), "d": (50, 110, 60, 255), "s": (150, 200, 250, 255), "o": (220, 150, 40, 255),
       "t": (214, 170, 110, 255), "r": (220, 60, 60, 255), "n": (120, 80, 40, 255), "h": (200, 140, 90, 255)}
TEXT = [
    "................",
    ".kkkkkkkkkkkkkk.",
    ".kwwwwwwwwwwwwk.",
    ".kwwbbbbbbbbwwk.",
    ".kwwbbbbbbbbwwk.",
    ".kwwwwbbbbwwwwk.",
    ".kwwwwbbbbwwwwk.",
    ".kwwwwbbbbwwwwk.",
    ".kwwwwbbbbwwwwk.",
    ".kwwwwbbbbwwwwk.",
    ".kwwwwwwwwwwwwk.",
    ".kkkkkkwwkkkkkk.",
    "......kwwk......",
    ".....kwwk.......",
    ".....kkk........",
    "................",
]
ASSETS = [
    "................",
    ".kkkkkkkkkkkkkk.",
    ".kooooooooooook.",
    ".kossssssssssok.",
    ".kossssyyssssok.",
    ".kosssyyyyssook.",
    ".kossssyyssssok.",
    ".kosssssssggsok.",
    ".kossssggggggok.",
    ".kosgggggdddgok.",
    ".kosggddddddgok.",
    ".kooooooooooook.",
    ".kkkkkkkkkkkkkk.",
    "................",
    "................",
    "................",
]
GFX = [   # palette with colour dots and a paintbrush
    "................",
    "...kkkkkkk....nn",
    "..kttttttkk..nhn",
    ".kttrrtttttkknhn",
    ".kttrrttbbtkkhn.",
    ".kttttttbbtkhn..",
    ".kkttttttttkn...",
    "kttkkttggttk.k..",
    "ktkwwkttggtkyk..",
    "ktkwwkttttkyyk..",
    "kttkkttttkk.kk..",
    ".kttttttkk......",
    "..kkttkk........",
    "....kkk.........",
    "................",
    "................",
]


def render(rows):
    im = Image.new("RGBA", (16, 16))
    for y, r in enumerate(rows):
        for x, c in enumerate(r):
            im.putpixel((x, y), PAL[c])
    return im


def write_ico(path, im16, sizes=(16, 32, 48, 256)):
    pngs = []
    for s in sizes:
        b = io.BytesIO()
        im16.resize((s, s), Image.NEAREST).save(b, "PNG")
        pngs.append((s, b.getvalue()))
    head = struct.pack("<HHH", 0, 1, len(pngs))
    off = 6 + 16 * len(pngs)
    ents, blobs = b"", b""
    for s, d in pngs:
        ents += struct.pack("<BBBBHHII", s % 256, s % 256, 0, 0, 1, 32, len(d), off + len(blobs))
        blobs += d
    open(path, "wb").write(head + ents + blobs)


if __name__ == "__main__":
    os.makedirs(OUT, exist_ok=True)
    for name, rows in (("text", TEXT), ("assets", ASSETS), ("gfx", GFX)):
        im = render(rows)
        write_ico(os.path.join(OUT, name + ".ico"), im)
        im.resize((64, 64), Image.NEAREST).save(os.path.join(OUT, name + ".png"))
        print("icons/%s.ico/.png" % name)
