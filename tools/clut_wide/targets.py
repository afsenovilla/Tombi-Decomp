import sys, json, os, numpy as np
sys.path.insert(0, "tools"); sys.argv=["x"]
import civ_rebuild as C
L = json.load(open(C.LAYOUT, encoding="utf-8"))
C.COMMUNITY.update(C.clutgen_samples("/dev/shm/clutgen"))
out = {}
for grp in C.PAGE_SETS:
    Q, src, st = C.derive_clut(grp[0], L, {}, os.path.join(C.OUT, "fmv"))
    w = np.zeros(256)
    for n in grp: w += np.bincount(C.page8(n, L).ravel(), minlength=256)
    w[0] = 0
    out[grp[0]] = dict(pages=grp, clut15=st["clut15"], src=src, weight=w.tolist())
    print(grp, int((w>0).sum()), {s: src.count(s) for s in set(src)})
json.dump(out, open("/dev/shm/clut2/targets.json", "w"))
