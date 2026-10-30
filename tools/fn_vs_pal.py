#!/usr/bin/env python3
"""Lists the functions of another build (beta/demo exe or overlay) that have no masked-hash twin in PAL, with the best
PAL similarity. Reads only; prints a table.

  python3 -I tools/xmatch.py TARGET --csv m.csv
  python3 -I tools/fn_vs_pal.py TARGET m.csv --vram 0x80010000 --text 0x80016070 0x8007555C [--min 48] [--top 60]

The gaps between the xmatch hits are cut at stack-frame prologues (addiu sp,sp,-N); leaf functions with no frame stay
glued to the previous piece. Similarity = Jaccard of masked 3-word shingles against every PAL function (0..100).
"""
import argparse, collections, csv, os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from xmatch import mask, words, pal_funcs


def main():
    p = argparse.ArgumentParser()
    p.add_argument("target"); p.add_argument("csv")
    p.add_argument("--vram", type=lambda x: int(x, 0), required=True)
    p.add_argument("--text", nargs=2, type=lambda x: int(x, 0), help="first/last vram of code")
    p.add_argument("--also", nargs="*", default=[], help="other binaries to search for an exact masked twin (e.g. JP retail)")
    p.add_argument("--min", type=int, default=48); p.add_argument("--top", type=int, default=60)
    a = p.parse_args()
    b = open(a.target, "rb").read()
    if b[:8] == b"PS-X EXE": b = b[0x800:]
    w = words(b); m = [mask(x) for x in w]
    lo = (a.text[0] - a.vram) // 4 if a.text else 0
    hi = (a.text[1] - a.vram) // 4 if a.text else len(w)
    cov = bytearray(len(w))
    for r in csv.DictReader(open(a.csv)):
        if os.path.basename(r["target"]) != os.path.basename(a.target): continue
        o = int(r["offset"], 16) // 4
        cov[o:o + int(r["size"]) // 4] = b"\1" * (int(r["size"]) // 4)
    gaps, i = [], lo
    while i < hi:
        if cov[i]: i += 1; continue
        j = i
        while j < hi and not cov[j]: j += 1
        gaps.append((i, j)); i = j
    funcs = []
    for g0, g1 in gaps:
        starts = [g0] + [k for k in range(g0 + 1, g1) if w[k] >> 16 == 0x27BD and (w[k] & 0x8000)]
        starts.append(g1)
        for s, e in zip(starts, starts[1:]):
            funcs.append((s, e))
    # merge tiny leading pieces (alignment nops) and drop pure padding
    funcs = [(s, e) for s, e in funcs if any(w[s:e]) and (e - s) * 4 >= a.min]
    pal = pal_funcs(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
    def sh(seq): return {tuple(seq[k:k + 3]) for k in range(len(seq) - 2)}
    idx, psh = collections.defaultdict(list), []
    for n, (mod, name, ad, sz, mw, fb) in enumerate(pal):
        s = sh(mw); psh.append(s)
        for t in s: idx[t].append(n)
    oth = []
    for f in a.also:
        ob = open(f, "rb").read()
        if ob[:8] == b"PS-X EXE": ob = ob[0x800:]
        om = [mask(x) for x in words(ob)]
        oi = collections.defaultdict(list)
        for k in range(len(om) - 4): oi[tuple(om[k:k + 4])].append(k)
        oth.append((os.path.basename(f), om, oi))
    def twin(seq):
        r = []
        for nm, om, oi in oth:
            for k in oi.get(tuple(seq[:4]), ()):
                if om[k:k + len(seq)] == seq: r.append(nm); break
        return ",".join(r)
    rows = []
    for s, e in funcs:
        ts = sh(m[s:e]); cnt = collections.Counter()
        for t in ts:
            for n in idx.get(t, ()): cnt[n] += 1
        best, bs = None, 0.0
        for n, c in cnt.most_common(8):
            j = c / (len(ts) + len(psh[n]) - c)
            if j > bs: best, bs = n, j
        rows.append((a.vram + s * 4, (e - s) * 4, int(bs * 100), pal[best][1] if best is not None else "-", twin(m[s:e])))
    rows.sort(key=lambda r: -r[1])
    print("gap functions >= %d bytes: %d, total %d bytes" % (a.min, len(rows), sum(r[1] for r in rows)))
    print("addr        size  sim  best PAL")
    for ad, sz, sm, nm, tw in rows[:a.top]: print("0x%08X %6d %4d  %s  %s" % (ad, sz, sm, nm, tw))
    print("sim>=60:", sum(1 for r in rows if r[2] >= 60), "of", len(rows))


if __name__ == "__main__":
    main()
