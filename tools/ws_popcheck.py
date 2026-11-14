#!/usr/bin/env python3
"""Pop-in check for --widescreen (docs/PORT.md "Widescreen", "Pop-in check").

  build/port2/tombi2 ... --widescreen 16:9 --log-pop pop.txt
  python3 tools/ws_popcheck.py pop.txt [--margin 6] [-v]

Reads the per-frame draw ranges --log-pop writes (port2/runtime/ws_poplog.c: every object and background block the
scene drew, the screen-X / Y range of its primitives) and lists what appeared or vanished inside the frame instead of
crossing its edge: an item drawn on frame f but not on f - 1 (or the reverse) whose range lies wholly inside the
visible frame: its part inside the frame (clipped to it) is at least `margin` columns wide on its first (last)
frame, where something scrolling in shows a sliver as wide as one frame's scroll.  Each event is tagged by where it happened: "strip" (it reaches
into the side strips the widescreen frame adds: x < 0 or x >= 320) or "4:3" (wholly inside the 4:3 picture, where a
PS1 would show it too).  Frames where most of the scene changes at once (room change, camera cut) are skipped."""
import argparse, collections, sys


def load(path):
    frames = collections.OrderedDict()
    pad = 0
    cur = None
    for ln in open(path):
        t = ln.split()
        if not t:
            continue
        if t[0] == 'F':
            cur = {}
            frames[int(t[1])] = cur
            pad = int(t[2])
        elif t[0] == 'O' and cur is not None:
            key = ('O', t[1], t[2], t[3], t[4])            # pointer, category, type, subtype
            cur[key] = tuple(map(int, t[5:9]))
        elif t[0] == 'B' and cur is not None:
            cur[('B', t[1])] = tuple(map(int, t[2:6]))
    return frames, pad


def events(frames, pad, margin=16, ytop=0, ybot=240):
    fr = list(frames.items())
    out = []
    for (f0, a), (f1, b) in zip(fr, fr[1:]):
        if f1 != f0 + 1:
            continue
        ka, kb = set(a), set(b)
        if len(ka ^ kb) > 0.5 * max(1, len(ka | kb)) and len(ka | kb) > 6:
            continue                                         # scene change
        for kind, keys, src in (('in', kb - ka, b), ('out', ka - kb, a)):
            for k in keys:
                x0, x1, y0, y1 = src[k]
                if min(y1, ybot) - max(y0, ytop) < 4:
                    continue                                 # above / below the picture, or a sliver at its edge
                c0, c1 = max(x0, -pad), min(x1, 320 + pad)
                if c1 - c0 < margin:
                    continue                                 # a sliver at the frame edge: scrolled in / out
                where = 'strip' if (c0 < 0 or c1 > 320) else '4:3'
                out.append((f1, kind, where, k, (x0, x1, y0, y1)))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('log')
    ap.add_argument('--margin', type=int, default=16, help='a part narrower than this (columns) counts as scrolling in')
    ap.add_argument('-v', action='store_true', help='list every event')
    a = ap.parse_args()
    frames, pad = load(a.log)
    ev = events(frames, pad, a.margin)
    cnt = collections.Counter((e[2], e[1], e[3][0] + (':%s/%s' % (e[3][3], e[3][4]) if e[3][0] == 'O' else '')) for e in ev)
    print('%s: %d frames, pad %d, %d events' % (a.log, len(frames), pad, len(ev)))
    for (where, kind, what), n in sorted(cnt.items()):
        print('  %-5s %-3s %-10s %d' % (where, kind, what, n))
    if a.v:
        for f, kind, where, k, r in ev:
            print('  %d %s %s %s %s' % (f, kind, where, ' '.join(k), r))
    return 0


if __name__ == '__main__':
    sys.exit(main())
