#!/usr/bin/env python3
"""Frame tables of the animated Civilization residents in the port (port2/runtime/main.c, port_civ_* tables).

Each frame id of the I0NFO2 anim scripts maps to a figure of the surviving 4 bpp page (tools/civ_sheets.py comps()),
optionally mirrored. The anchor is the column under the centre of the head (mean x of the top 12 opaque rows), so
the figure does not sway when an arm or a leg sticks out of its box; the bottom of the box stands on the ground.

  python3 tools/civ_anim.py        prints the C tables (boxes + anchors) from your own disc (game/disc/fs)
Nothing from the disc is stored: the tables are coordinates only.
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import civ_sheets as S

# figure numbers in reading order of the sheets (A06100 / A07600 comps indices, A08000 sheet order); "-n" = mirrored
WORKER = [0, 1, 2, 3, 4, 5, -3, -4, -5, 6, 7, 8, 9, 10, 11, 12, 13, 14, -12, -13, -14, 15, 16, 19, 17, 18, 20, 21, 22, 23]
WOMAN = [12, 18, 22, 0, 1, 2, '-0', -1, -2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -9, -10, -11, 13, 14, 15, 18, 20, 16]
BOY_COMPS = [23, 25, 26, 27, 28, 30, 31, 35]                     # sheet figures 1..8 of the boy (A07600)
BOY = [1, 3, 1, 1, 2, 2, 1, -2, -2, 3, 4, 5, 6, 3, 4, 1, 2, 2, 1, -2, -2, 1, 1, 3, 3, 1, 1]
IRON = [7, 13, 20] + list(range(7, 13)) + list(range(13, 19)) + list(range(1, 7)) + [27, 28, 29, 24, 25, 26, 21, 22, 23]


def anchor(p, box):
    x0, y0, x1, y1 = box
    xs = []
    for y in range(y0, y1 + 1):
        row = [x for x in range(x0, x1 + 1) if p[y][x]]
        if row:
            xs += row
            if (y - y0) >= 12 and xs: break
    return round(sum(xs) / len(xs)) - x0


def table(name, page, seq, pick):
    p = S.page(page)
    out = []
    for k in seq:
        fl = isinstance(k, str) or k < 0
        box = pick(abs(int(k)))
        out.append('{%d,%d,%d,%d,%d,%d}' % (box[0], box[1], box[2] - box[0] + 1, box[3] - box[1] + 1, fl, anchor(p, box)))
    print('%s[%d] = {\n    %s};' % (name, len(out), ', '.join(out)))


def main():
    a = S.comps(S.page('A06100')); table('port_civ_worker', 'A06100', WORKER, lambda k: a[k])
    b = S.comps(S.page('A07600')); table('port_civ_woman', 'A07600', WOMAN, lambda k: b[k])
    bo = [c for i, c in sorted(enumerate(b), key=lambda t: (t[1][1] // 40, t[1][0])) if i in BOY_COMPS]
    table('port_civ_boy', 'A07600', BOY, lambda k: bo[k - 1])
    c = [c for i, c in sorted(enumerate(S.comps(S.page('A08000'))), key=lambda t: (t[1][1] // 40, t[1][0]))]
    table('port_civ_iron', 'A08000', IRON, lambda k: c[k - 1])


if __name__ == '__main__':
    main()
