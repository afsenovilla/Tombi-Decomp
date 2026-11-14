#!/usr/bin/env python3
"""Pixel pop-in check for --widescreen (docs/PORT.md "Widescreen", "Pop-in check").

  build/port2/tombi2 ... --widescreen 16:9 --sweep-x 1700:4:X1,X0 --sweep-fly -30 \
      --dump-frames RUN/fr --dump-every 2 --dump-from 1700 --log-pop RUN/pop.txt
  python3 tools/ws_popscan.py RUN [--f0 1760] [--png 8] [-v]

Unlike tools/ws_popcheck.py (one box per object / background block), this looks at the pixels, so it cannot miss a
piece drawn in part (a polygon culled inside a block, a sub-block selection, a layer switched by depth...).  Each
dumped frame is cut into 8x8 tiles; a tile of frame f is "explained" when some shift (dx, dy) of the previous dump
matches it (mean |diff| below --thr; the camera's vertical step is found first, then dx within +-rx and dy within
+-ry around it, at half resolution), and the same backwards.  Unexplained tiles are content that appeared (forward)
or vanished (backward) without moving in.  Left out: the columns at the frame edges (scrolling in, --edge), the HUD
rows, and the boxes of the player and of the objects up to 128 columns wide (sprites animate; --log-pop gives them).
Clusters of --minclu tiles at least --minw tiles wide are events, tagged "strip" (touching the side strips) or "4:3".
Animated scenery (flowers, water, fast near layers) still gives events: look at the --png overlays (RUN/scan/,
previous dump left, this one right, red = appeared, magenta = vanished)."""
import sys, os, argparse, collections
import numpy as np
from PIL import Image
from multiprocessing import Pool

ap = argparse.ArgumentParser()
ap.add_argument('run')
ap.add_argument('--f0', type=int, default=1700)
ap.add_argument('--f1', type=int, default=10 ** 9)
ap.add_argument('--thr', type=float, default=70)
ap.add_argument('--rx', type=int, default=24)
ap.add_argument('--ry', type=int, default=8)
ap.add_argument('--edge', type=int, default=24, help='columns at the frame edges left out (scroll-in)')
ap.add_argument('--png', type=int, default=0, help='write overlays of the N worst frames')
ap.add_argument('--minclu', type=int, default=4)
ap.add_argument('--minw', type=int, default=3, help='tiles: a cluster narrower is a disocclusion sliver')
ap.add_argument('-v', action='store_true')
a = ap.parse_args()
T = 8

def load_boxes(path):
    box = collections.defaultdict(list); cam = {}
    if not os.path.exists(path): return box, cam
    f = None
    for ln in open(path):
        t = ln.split()
        if t[0] == 'F': f = int(t[1]); cam[f] = int(t[3])
        elif t[0] == 'O' and t[2] != '07':
            b = tuple(map(int, t[5:9]))
            if t[1] == 'player' or b[1] - b[0] <= 128: box[f].append(b)   # sprites animate; big pieces are scenery
    return box, cam

def img(f):
    return np.asarray(Image.open('%s/fr/frame_%06d.png' % (a.run, f)).convert('RGB'), dtype=np.int16)

def half(x):
    H, W, _ = x.shape
    return x[:H // 2 * 2, :W // 2 * 2].reshape(H // 2, 2, W // 2, 2, 3).mean((1, 3)).astype(np.float32)

def global_dy(cur, prev):
    """the camera's vertical step: best dy (half-res px) of the whole picture, coarse"""
    H, W, _ = cur.shape
    best, bdy = 1e18, 0
    c = cur[24:H - 24, W // 4: 3 * W // 4]
    for dy in range(-20, 21):
        p = prev[24 + dy:H - 24 + dy, W // 4: 3 * W // 4]
        best_dx = min(np.abs(c[:, 12:-12] - p[:, 12 + dx: p.shape[1] - 12 + dx]).mean() for dx in range(-12, 13, 2))
        if best_dx < best: best, bdy = best_dx, dy
    return bdy

def unexplained(cur, prev, gdy=None):
    # half resolution: 4x4 tiles there are the 8x8 tiles of the frame; shifts in steps of 2 px
    cur, prev = half(cur), half(prev)
    T = 4
    if gdy is None: gdy = global_dy(cur, prev)
    H, W, _ = cur.shape; R, Ry = a.rx // 2, a.ry // 2
    Y0 = Ry + abs(gdy)
    pp = np.full((H + 2 * Y0, W + 2 * R, 3), -2000, np.float32)
    pp[Y0:Y0 + H, R:R + W] = prev
    th, tw = H // T, W // T
    best = np.full((th, tw), 1e9)
    c = cur[:th * T, :tw * T]
    for dy in range(gdy - Ry, gdy + Ry + 1):
        for dx in range(-R, R + 1):
            p = pp[Y0 + dy:Y0 + dy + th * T, R + dx:R + dx + tw * T]
            d = np.abs(c - p).sum(2)
            m = d.reshape(th, T, tw, T).mean((1, 3))
            np.minimum(best, m, out=best)
    return best > a.thr, gdy

def mask_for(boxes, pad, H, W):
    th, tw = H // T, W // T
    m = np.zeros((th, tw), bool)
    for x0, x1, y0, y1 in boxes:
        X0, X1 = (x0 + pad) // T - 1, (x1 + pad) // T + 1
        Y0, Y1 = (y0 + 8) // T - 1, (y1 + 8) // T + 1     # frame rows: game y + 8 (256-line dump)
        m[max(0, Y0):max(0, Y1 + 1), max(0, X0):max(0, X1 + 1)] = True
    return m

def clusters(u):
    seen = np.zeros_like(u); out = []
    for y, x in zip(*np.nonzero(u)):
        if seen[y, x]: continue
        st = [(y, x)]; seen[y, x] = True; cl = []
        while st:
            cy, cx = st.pop(); cl.append((cy, cx))
            for ny, nx in ((cy + 1, cx), (cy - 1, cx), (cy, cx + 1), (cy, cx - 1)):
                if 0 <= ny < u.shape[0] and 0 <= nx < u.shape[1] and u[ny, nx] and not seen[ny, nx]:
                    seen[ny, nx] = True; st.append((ny, nx))
        out.append(cl)
    return out

frames = sorted(int(n[6:12]) for n in os.listdir(a.run + '/fr') if n.startswith('frame_'))
frames = [f for f in frames if a.f0 <= f <= a.f1]
box, cam = load_boxes(a.run + '/pop.txt')

def work(i):
    f0, f1 = frames[i - 1], frames[i]
    A, B = img(f0), img(f1)
    H, W, _ = B.shape; pad = (W - 320) // 2
    if np.abs(A - B).mean() > 120: return f1, None          # cut / fade
    fwd, g = unexplained(B, A); bwd, _ = unexplained(A, B, -g)
    th, tw = fwd.shape
    keep = np.ones((th, tw), bool)
    e = (a.edge + T - 1) // T
    keep[:, :e] = False; keep[:, tw - e:] = False
    keep[:48 // T] = False; keep[th - 40 // T:] = False       # HUD rows
    keep &= ~mask_for(box.get(f0, []) + box.get(f1, []), pad, H, W)
    res = []
    for kind, u in (('in', fwd & keep), ('out', bwd & keep)):
        for cl in clusters(u):
            xs = [x for _, x in cl]; ys = [y for y, _ in cl]
            if len(cl) < a.minclu or max(xs) - min(xs) + 1 < a.minw: continue
            x0, x1 = min(xs) * T - pad, (max(xs) + 1) * T - pad
            where = 'strip' if (x0 < 0 or x1 > 320) else '4:3'
            res.append((kind, where, len(cl), x0, x1, min(ys) * T - 8, (max(ys) + 1) * T - 8))
    return f1, res

if __name__ == '__main__':
    with Pool(4) as p:
        R = p.map(work, range(1, len(frames)))
    tot = collections.Counter(); score = []
    for f, res in R:
        if res is None: continue
        for r in res:
            tot[r[1]] += 1
            if a.v: print('  %d %s %s tiles %d x %d..%d y %d..%d' % ((f,) + r))
        score.append((sum(r[2] for r in res), f))
    print('%s: %d frame pairs, events strip %d, 4:3 %d' % (a.run, len(R), tot['strip'], tot['4:3']))
    score.sort(reverse=True)
    print('worst:', ' '.join('%d(%d)' % (f, s) for s, f in score[:10] if s))
    if a.png:
        from PIL import ImageDraw
        os.makedirs(a.run + '/scan', exist_ok=True)
        RR = dict(R)
        for s, f in score[:a.png]:
            if not s: continue
            i = frames.index(f)
            A = Image.open('%s/fr/frame_%06d.png' % (a.run, frames[i - 1])).convert('RGB')
            B = Image.open('%s/fr/frame_%06d.png' % (a.run, f)).convert('RGB')
            pad = (B.width - 320) // 2
            C = Image.new('RGB', (B.width * 2 + 4, B.height + 12)); C.paste(A, (0, 12)); C.paste(B, (B.width + 4, 12))
            d = ImageDraw.Draw(C)
            d.text((2, 0), 'frame %d' % frames[i - 1], fill=(255, 255, 255)); d.text((B.width + 6, 0), 'frame %d' % f, fill=(255, 255, 255))
            for kind, where, n, x0, x1, y0, y1 in RR[f]:
                ox = B.width + 4 if kind == 'in' else 0
                d.rectangle([ox + x0 + pad, 12 + y0 + 8, ox + x1 + pad - 1, 12 + y1 + 8 - 1], outline=(255, 0, 0) if kind == 'in' else (255, 0, 255))
            C.save('%s/scan/pop_%06d.png' % (a.run, f))
