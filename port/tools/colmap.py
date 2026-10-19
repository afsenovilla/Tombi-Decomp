#!/usr/bin/env python3
"""Draw the static collision records of the current section from a `tombi --dump-ram FRAME:PATH` image.

    python3 port/tools/colmap.py RAMDUMP OUT.png [X0 X1 Y0 Y1] [mark x,y ...]

Layout (src/getCollisionPlaneAt.c, port/runtime/main0_handwritten.c func_800406E8 / func_800416B8):
  blob b = *(u32 *)0x1F800308 (AREA00 sections 0-2 also use *(u32 *)0x1F80030C); b+8: u16 offsets of one
  column table per depth plane (90 units of the object's depth, D_8007B460[area][section] is the origin);
  column c = (x - origin.x) / 8: list = rowtable + u16[rowtable + c*2]: u16 count, then records of four
  halfwords: flags, y, dy, (rise << 8) | (span << 4) | start (start/span within the 8-pixel column).
  Flag bit 0 = floor (the player's y is 20 above it), bit 4 = slope data, bits 2/3 = wall for movement
  left/right.  Platforms that are objects (pillars, beams, nets, bushes) are not in these lists.
The picture draws each depth plane in its own colour, prints the merged floor runs, and marks the given
positions (e.g. the player's x,y from `-v`)."""
import struct
import sys

from PIL import Image, ImageDraw

d = open(sys.argv[1], 'rb').read()
SP = d[0x200000:]


def r8(a): return d[a - 0x80000000]
def r16(a): return struct.unpack('<H', d[a - 0x80000000:a - 0x80000000 + 2])[0]
def s16(a): v = r16(a); return v - 0x10000 if v >= 0x8000 else v
def r32(a): return struct.unpack('<I', d[a - 0x80000000:a - 0x80000000 + 4])[0]
def sp32(o): return struct.unpack('<I', SP[o:o + 4])[0]


area = r16(0x8009C960); sec = r16(0x8009C962)
x0, x1 = s16(0x800A457C), s16(0x800A457E)
tabs = r32(0x8007B460 + area * 4)
tx, ty = s16(tabs + sec * 4), s16(tabs + sec * 4 + 2)
lim = r8(r32(0x8007B55C + area * 4) + sec)
blobs = [sp32(0x308)] + ([sp32(0x30C)] if area == 0 and sec != 3 else [])
nrows = (r16(blobs[0] + 8) - 8) // 2 if r16(blobs[0] + 8) > 8 else lim + 1   # plane 0's table follows the offsets
print(f"area {area} section {sec} x {x0}..{x1} origin ({tx},{ty}) planes {nrows} blobs {[hex(b) for b in blobs]}")

args = sys.argv[3:]
if len(args) >= 4 and all(a.lstrip('-').isdigit() for a in args[:4]):
    X0, X1, Y0, Y1 = map(int, args[:4]); marks = args[4:]
else:
    X0, X1, Y0, Y1 = x0 - 64, x1 + 64, -1600, 200; marks = args
img = Image.new('RGB', (X1 - X0, Y1 - Y0), (20, 20, 30))
dr = ImageDraw.Draw(img)
def P(x, y): return (x - X0, y - Y0)
for yy in range(Y0 - Y0 % 100, Y1, 100):
    dr.line([P(X0, yy), P(X1, yy)], fill=(40, 40, 60)); dr.text(P(X0 + 2, yy), str(yy), fill=(90, 90, 120))
for xx in range(X0 - X0 % 200, X1, 200):
    dr.line([P(xx, Y0), P(xx, Y1)], fill=(40, 40, 60)); dr.text(P(xx, Y0 + 2), str(xx), fill=(90, 90, 120))

COLS = [(255, 255, 255), (255, 120, 120), (120, 255, 120), (120, 160, 255), (255, 255, 120), (255, 120, 255)]
segs = []
for b in blobs:
    for r in range(nrows):
        rowt = b + r16(b + 8 + r * 2)
        for x in range(max(x0, 0) - max(x0, 0) % 8, x1, 8):
            c = (x - tx) // 8
            if c < 0: continue
            lst = rowt + r16(rowt + c * 2)
            n = s16(lst)
            if n <= 0 or n > 64: continue
            col = COLS[r % len(COLS)]
            for i in range(n):
                rec = lst + 2 + i * 8
                fl, ry, dy, w = r16(rec), s16(rec + 2), s16(rec + 4), r16(rec + 6)
                start, span = w & 15, (w >> 4) & 15
                xa, xb = x + start, x + start + span
                if fl & 0x10:
                    dr.line([P(xa, ry), P(xb, ry + dy)], fill=tuple(v // 3 for v in col))
                else:
                    dr.line([P(xa, ry), P(xb, ry + dy)], fill=col, width=2)
                    segs.append((r, xa, ry, xb, ry + dy, fl))
segs.sort()
runs = []
for r, xa, ya, xb, yb, fl in segs:
    if runs and runs[-1][0] == r and abs(runs[-1][2] - xa) <= 4 and abs(runs[-1][4] - ya) <= 6 and runs[-1][5] == fl:
        runs[-1] = (r, runs[-1][1], xb, runs[-1][3], yb, fl)
    else:
        runs.append((r, xa, xb, ya, yb, fl))
for r, xa, xb, ya, yb, fl in runs:
    if xb - xa >= 16: print(f"plane {r}: x {xa}..{xb} y {ya}..{yb} flags {fl:#x}")
for m in marks:
    mx, my = map(int, m.split(','))
    dr.ellipse([P(mx - 4, my - 4), P(mx + 4, my + 4)], outline=(255, 255, 0))
    dr.text(P(mx + 6, my - 6), m, fill=(255, 255, 0))
img.save(sys.argv[2])
print("saved", sys.argv[2], img.size)
