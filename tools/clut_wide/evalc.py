import sys, json, os, glob, numpy as np
sys.path.insert(0, "/dev/shm/clut2"); from look import W, where
T = json.load(open("/dev/shm/clut2/targets.json"))
def tgt(k):
    v = T[k]; w = np.array(v["weight"]); w[[s not in ("clutgen", "fmv") for s in v["src"]]] = 0
    t = np.array([int(x, 16) for x in v["clut15"]]); return w, t
def ch(x): x = np.asarray(x, np.int64); return np.stack([(x >> s) & 31 for s in (0, 5, 10)], 1).astype(float)
def metrics(t, w, win):
    m = w > 0; wn = w / w.sum(); X, Tc = ch(win), ch(t)
    def stats(Y):
        d = np.abs(Y - Tc).max(1)
        return dict(mean=float((wn * d).sum()), **{"w%d" % k: float(wn[d <= k].sum()) for k in (6, 9, 12)},
                    **{"u%d" % k: float((d[m] <= k).mean()) for k in (6, 9, 12)})
    raw = stats(X)
    Y = X.copy(); fit = []
    for c in range(3):
        sx, st = (wn * X[:, c]).sum(), (wn * Tc[:, c]).sum()
        vx = (wn * (X[:, c] - sx) ** 2).sum(); cov = (wn * (X[:, c] - sx) * (Tc[:, c] - st)).sum()
        a = float(np.clip(cov / vx, 0.5, 2.0)) if vx > 0 else 1.0; b = st - a * sx
        Y[:, c] = np.clip(np.round(a * X[:, c] + b), 0, 31); fit.append((round(a, 2), round(b, 1)))
    return raw, stats(Y), fit
def window(o, s): return np.asarray(W[o:o + 255 * s + 1:s])
