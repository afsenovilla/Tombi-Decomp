# FFT scan: for every word offset (strides 1,2,4,16), weighted per-channel affine fit (gain clamped 0.5..2) of the
# 256-entry window to the custom CLUT; plus best 16-entry block per CLUT row anywhere (raw SSE).
import json, numpy as np, sys
T = json.load(open("/dev/shm/clut2/targets.json"))
W = np.memmap("/dev/shm/clut2/corpus.u16", np.uint16, "r"); N = len(W)
L = 1 << 20; STR = (1, 2, 4, 16); MAXM = 255 * 16 + 1
pages = {}
for k, v in T.items():
    w = np.array(v["weight"]); w[[s not in ("clutgen", "fmv") for s in v["src"]]] = 0; w /= w.sum()
    t = np.array([int(x, 16) for x in v["clut15"]]); tc = np.stack([(t >> s) & 31 for s in (0, 5, 10)], 1).astype(float)
    St = (w[:, None] * tc).sum(0); Stt = (w[:, None] * tc ** 2).sum(0)
    ker = {}
    for s in STR:
        kw = np.zeros(L); kw[np.arange(256) * s] = w
        kt = [np.zeros(L) for _ in range(3)]
        for c in range(3): kt[c][np.arange(256) * s] = w * tc[:, c]
        ker[s] = (np.conj(np.fft.rfft(kw)).astype(np.complex64), [np.conj(np.fft.rfft(x)).astype(np.complex64) for x in kt])
    blk = []
    for r in range(16):
        wr = w[r*16:(r+1)*16]
        if wr.sum() == 0: blk.append(None); continue
        wr = wr / wr.sum(); kw = np.zeros(L); kw[:16] = wr
        kt = [np.zeros(L) for _ in range(3)]
        for c in range(3): kt[c][:16] = wr * tc[r*16:(r+1)*16, c]
        blk.append((np.conj(np.fft.rfft(kw)).astype(np.complex64), [np.conj(np.fft.rfft(x)).astype(np.complex64) for x in kt], (wr[:, None] * tc[r*16:(r+1)*16] ** 2).sum()))
    pages[k] = dict(St=St, Stt=Stt, ker=ker, blk=blk, best={s: [] for s in STR}, bbest=[(1e9, -1)] * 16)
step = L - MAXM
for o0 in range(0, N, step):
    x = np.zeros(L); seg = np.asarray(W[o0:o0 + L]).astype(np.int64); n = len(seg)
    xc = [((seg >> s) & 31).astype(float) for s in (0, 5, 10)]
    F = [np.fft.rfft(np.pad(a, (0, L - n))) for a in xc]
    F2 = [np.fft.rfft(np.pad(a * a, (0, L - n))) for a in xc]
    Fall2 = F2[0] + F2[1] + F2[2]
    valid = min(step, n)
    for k, P in pages.items():
        for s in STR:
            kw, kt = P["ker"][s]
            res = np.zeros(valid); raw = np.zeros(valid); ok = np.ones(valid, bool)
            for c in range(3):
                Sx = np.fft.irfft(F[c] * kw, L)[:valid]; Sxx = np.fft.irfft(F2[c] * kw, L)[:valid]
                Sxt = np.fft.irfft(F[c] * kt[c], L)[:valid]
                vx = Sxx - Sx * Sx; cov = Sxt - Sx * P["St"][c]; vt = P["Stt"][c] - P["St"][c] ** 2
                ok &= vx > 4.0  # window std >= 2 levels in every channel
                a = np.clip(cov / np.maximum(vx, 1e-9), 0.5, 2.0)
                res += vt - 2 * a * cov + a * a * vx
                raw += Sxx - 2 * Sxt + P["Stt"][c]
            res[~ok] = 1e9
            lim = n - 255 * s
            if lim < valid: res[max(lim, 0):] = 1e9
            top = np.argpartition(res, 50)[:50]
            P["best"][s] += [(float(res[i]), float(raw[i]), o0 + int(i)) for i in top if res[i] < 1e8]
            P["best"][s] = sorted(P["best"][s])[:400]
        for r, b in enumerate(P["blk"]):
            if b is None: continue
            kw, kt, stt = b
            sse = np.fft.irfft(Fall2 * kw, L)[:valid] + stt
            for c in range(3): sse -= 2 * np.fft.irfft(F[c] * kt[c], L)[:valid]
            lim = n - 15
            if lim < valid: sse[max(lim, 0):] = 1e9
            i = int(np.argmin(sse))
            if sse[i] < P["bbest"][r][0]: P["bbest"][r] = (float(sse[i]), o0 + i)
    print(o0, file=sys.stderr, flush=True)
json.dump({k: dict(best=P["best"], bbest=P["bbest"]) for k, P in pages.items()}, open("/dev/shm/clut2/affine.json", "w"))
