#!/usr/bin/env python3
"""ws_seam.py: an objective seam metric for the widescreen room strips (docs/PORT.md "Room picture strips").

Input: an image laid out as [left strip | 320-column picture | right strip] (the standalone render of
port2/runtime/ws_backdrop.c with -DWS_BACKDROP_MAIN, or a game frame with --y0 16 for the picture rows).  The left
side is measured mirrored, so "picture column 316-319" always means the 4 picture columns next to the join and
"strip column 0-3" the 4 strip columns next to it.

Per row of a band:
  join   = max over R, G, B of |mean(picture cols 316-319) - mean(strip cols 0-3)|
  pstep  = the picture's own typical step in that row: the 90th percentile, over the band's step range (default
           cols 280-319), of the same measure between neighbouring 4-column groups
  hstep  = the largest step between neighbouring 4-column groups inside strip cols 0-19 (the hand-over: a pale or
           dark column a few columns into the strip shows as a line too); compared with the picture's largest step
           in the same row
  grad   = mean |horizontal luminance difference| over picture cols 312-319 vs strip cols 0-7
Per band (texture): picture cols tx0-319 against the same number of strip columns (at most 16): mean luminance,
contrast (std), gradient energy, and the stripe period (the lag 3-12 of the most prominent peak of the pooled
autocorrelation of the row-demeaned luminance) with its strength.

A band PASSES when
  - the join step is within the picture's own variation: >= 80 % of its rows have join <= pstep + 2 and the band's
    median join <= its median pstep;
  - the hand-over is too: median hstep <= 1.25 x the median of the picture's largest step over the same range;
  - the texture matches: |d mean| <= max(8, 0.35 * picture std) (8: one 5-bit colour step), contrast ratio 0.7-1.4,
    gradient ratio 0.6-1.6, and where
    the picture has a clear stripe period (peak prominence >= 0.25) the strip has one within 1 column and at least half
    as strong.

The noise floor: --calib CUT measures the picture against itself, split at picture column CUT (the picture's
columns up to CUT as "the picture", the next ones as "the strip"): what a perfect continuation scores.  A busy,
non-stationary picture fails some texture checks against itself; read the table with that in mind.

Usage: python3 tools/ws_seam.py IMG --pad 53 [--side right|left] [--y0 0] [--bands SPEC] [--rows] [--calib CUT]
  SPEC: "name:y0-y1[:tx0[:sx0-sx1]];..." (rows [y0, y1), texture from picture col tx0 (default 304), steps over
  picture cols [sx0, sx1) (default 280-320); left side in mirrored columns).  The default bands are AREA02 s1's.
"""
import argparse
import sys

import numpy as np
from PIL import Image

AREA02S1_RIGHT = "lit-sph:0-16:316;canopy:16-78;trunks:78-110;fence:110-137:314:186-226;ledge:137-156:314:150-216;" \
                 "bank:156-181:314:150-216;ground:181-192:316:0-100;front:192-224"
AREA02S1_LEFT = "canopy:0-78;trunks:78-108;fence:108-138;ledge:138-156;bank:156-181;ground:181-195;front:195-224"


def lum(a):
    return a[..., 0] * 0.299 + a[..., 1] * 0.587 + a[..., 2] * 0.114


def period(L):
    """the stripe period: the lag 3-12 of the most prominent local peak of the pooled autocorrelation of the rows
    (each row minus its mean); strength = the peak's height over the lowest value at a shorter lag"""
    L = L - L.mean(axis=1, keepdims=True)
    den = (L * L).sum()
    if den <= 0 or L.shape[1] < 8:
        return 0, 0.0
    n = min(13, L.shape[1] - 3)
    c = [1.0] + [(L[:, :-lag] * L[:, lag:]).sum() * L.shape[1] / (L.shape[1] - lag) / den for lag in range(1, n + 1)]
    best, bc = 0, 0.0
    for lag in range(3, n):
        if c[lag] > c[lag - 1] and c[lag] >= c[lag + 1]:
            pr = c[lag] - min(c[1:lag])
            if pr > bc:
                best, bc = lag, pr
    return best, bc


def groups(row, x0, x1):
    """means of the 4-column groups [x0, x0+4), ... up to x1 (row: (W, 3))"""
    return np.array([row[x:x + 4].mean(axis=0) for x in range(x0, x1 - 3, 4)])


def gsteps(g):
    return np.abs(np.diff(g, axis=0)).max(axis=1) if len(g) > 1 else np.zeros(1)


def parse_bands(spec):
    out = []
    for part in spec.split(';'):
        f = part.split(':')
        y0, y1 = (int(v) for v in f[1].split('-'))
        tx0 = int(f[2]) if len(f) > 2 and f[2] else 304
        sx = tuple(int(v) for v in f[3].split('-')) if len(f) > 3 else (280, 320)
        out.append((f[0], y0, y1, tx0, sx))
    return out


def measure(img, pad, side, bands, y0=0, rows=False, out=sys.stdout):
    a = np.asarray(img.convert('RGB'), dtype=np.float64)
    if side == 'left':
        a = a[:, ::-1]
    P = a[y0:y0 + 224, pad:pad + 320]
    S = a[y0:y0 + 224, pad + 320:pad + 320 + pad]
    res = []
    print(f"{'band':8s} {'rows':>7s} | {'join':>5s} {'pstep':>5s} {'ok%':>4s} | {'hstep':>5s} {'pmax':>5s} | "
          f"{'mean P/S':>11s} {'std P/S':>11s} {'grad P/S':>11s} {'period P/S':>17s} | verdict", file=out)
    for name, b0, b1, tx0, (sx0, sx1) in bands:
        js, ps, hs, pm, okr = [], [], [], [], 0
        for y in range(b0, b1):
            pr, sr = P[y], S[y]
            j = np.abs(pr[316:320].mean(axis=0) - sr[0:4].mean(axis=0)).max()
            st = gsteps(groups(pr, sx0, sx1))
            p90 = float(np.percentile(st, 90))
            h = gsteps(groups(sr, 0, 20)).max()
            js.append(j); ps.append(p90); hs.append(h); pm.append(st.max())
            okr += j <= p90 + 2
            if rows:
                print(f"   row {y:3d}: join {j:5.1f} pstep {p90:5.1f} hstep {h:5.1f} pmax {st.max():5.1f}"
                      f"{'  <-- step' if j > p90 + 2 else ''}", file=out)
        n = b1 - b0
        tw = min(320 - tx0, 16)
        LP, LS = lum(P[b0:b1, 320 - tw:320]), lum(S[b0:b1, 0:tw])
        mP, mS, sP, sS = LP.mean(), LS.mean(), LP.std(), LS.std()
        gP, gS = np.abs(np.diff(LP, axis=1)).mean(), np.abs(np.diff(LS, axis=1)).mean()
        perP, cP = period(LP)
        perS, cS = period(LS)
        jm, pmed, hm, pmm = float(np.median(js)), float(np.median(ps)), float(np.median(hs)), float(np.median(pm))
        why = []
        if okr < 0.8 * n or jm > pmed:
            why.append('join step')
        if hm > 1.25 * pmm:
            why.append('hand-over step')
        if abs(mP - mS) > max(8.0, 0.35 * sP):
            why.append('tone')
        if not (0.7 <= (sS + 1) / (sP + 1) <= 1.4):
            why.append('contrast')
        if not (0.6 <= (gS + 1) / (gP + 1) <= 1.6):
            why.append('texture energy')
        if cP >= 0.25 and (abs(perS - perP) > 1 or cS < 0.5 * cP):
            why.append('stripe period')
        v = 'PASS' if not why else 'FAIL: ' + ', '.join(why)
        print(f"{name:8s} {b0:3d}-{b1:3d} | {jm:5.1f} {pmed:5.1f} {100 * okr / n:4.0f} | {hm:5.1f} {pmm:5.1f} | "
              f"{mP:5.1f}/{mS:5.1f} {sP:5.1f}/{sS:5.1f} {gP:5.1f}/{gS:5.1f} {perP:2d}({cP:.2f})/{perS:2d}({cS:.2f}) | {v}",
              file=out)
        res.append((name, not why))
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('img')
    ap.add_argument('--pad', type=int, required=True)
    ap.add_argument('--side', choices=('right', 'left'), default='right')
    ap.add_argument('--y0', type=int, default=0, help='the picture\'s first row in the image (16 in a game frame)')
    ap.add_argument('--bands', help='band spec (default: AREA02 s1)')
    ap.add_argument('--rows', action='store_true', help='print every row')
    ap.add_argument('--calib', type=int, help='measure the picture against itself split at this column')
    o = ap.parse_args()
    spec = o.bands or (AREA02S1_RIGHT if o.side == 'right' else AREA02S1_LEFT)
    img = Image.open(o.img)
    if o.calib:
        a = np.asarray(img.convert('RGB'))
        if o.side == 'left':
            a = a[:, ::-1]
        pic = a[o.y0:o.y0 + 224, o.pad:o.pad + 320]
        c, n = o.calib, min(o.pad, 320 - o.calib)
        q = np.zeros((224, 320 + 2 * n, 3), np.uint8)
        q[:, n + 320 - c:n + 320] = pic[:, :c]
        q[:, n + 320:] = pic[:, c:c + n]
        img, o.pad, o.y0, o.side = Image.fromarray(q), n, 0, 'right'
    r = measure(img, o.pad, o.side, parse_bands(spec), o.y0, o.rows)
    print(f"{sum(ok for _, ok in r)}/{len(r)} bands pass")


if __name__ == '__main__':
    main()
