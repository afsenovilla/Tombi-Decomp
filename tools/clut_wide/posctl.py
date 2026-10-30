# positive control: a real 256-colour TIM CLUT, perturbed like an FMV-derived CLUT, must still be found by the metrics
import sys, json, numpy as np
sys.path.insert(0, "tools"); import clut_catalog as CC, area000 as A
sys.path.insert(0, "/dev/shm/clut2"); from look import W, idx, names
T = json.load(open("/dev/shm/clut2/targets.json"))
# pick real 8bpp TIM CLUTs (w=256) with >= 200 distinct colours
cands = []
for h, ph, s, n in idx:
    if ph: continue
    d = np.asarray(W[s:s+n]).tobytes()
    for (o, x, y, w, hh, words) in CC.tims(d):
        if w == 256 and len(set(words[:256])) >= 200:
            cands.append((names[h][0], o, s + (o + 20)//2, words[:256]))
print(len(cands), [c[:3] for c in cands[:8]])
json.dump(cands[:6], open("/dev/shm/clut2/posctl_cands.json", "w"))
