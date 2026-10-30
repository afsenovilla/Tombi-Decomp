import sys, json, glob, numpy as np
sys.path.insert(0, "/dev/shm/clut2"); sys.path.insert(0, "/home/user/Tombi-Decomp/tools")
from evalc import *
from PIL import Image, ImageDraw
OUTD = "/tmp/claude-0/-home-user-Tombi-Decomp/7d8ccfcf-89bb-5621-9386-2224d5fdc0d9/scratchpad/clut2"
os.makedirs(OUTD, exist_ok=True)
AF = json.load(open("/dev/shm/clut2/affine.json"))
summary = {}
for k in T:
    w, t = tgt(k); C = set()
    for f in glob.glob("/dev/shm/clut2/res_%s_*.tsv" % k):
        for ln in open(f):
            a = ln.split("\t"); C.add((int(a[2]), int(a[1])))
    for s, lst in AF[k]["best"].items():
        for r in lst[:150]: C.add((r[2], int(s)))
    rows = []
    for o, s in C:
        raw, cor, fit = metrics(t, w, window(o, s))
        rows.append((cor["w6"] + cor["w9"] + raw["w6"], o, s, raw, cor, fit))
    rows.sort(key=lambda r: -r[0])
    # block assembly
    bb = AF[k]["bbest"]; asm = t.copy()
    for r, (sse, o) in enumerate(bb):
        if o >= 0: asm[r*16:(r+1)*16] = W[o:o+16]
    braw, bcor, _ = metrics(t, w, asm)
    summary[k] = dict(top=[dict(where=where(o), word=o, stride=s, raw=raw, cor=cor, fit=fit) for _, o, s, raw, cor, fit in rows[:8]],
                      blocks=dict(raw=braw, cor=bcor, srcs=[(where(o)[0][0], where(o)[2], round(sse, 1)) for sse, o in bb if o >= 0]))
    print("==", k, "candidates", len(rows))
    for _, o, s, raw, cor, fit in rows[:8]:
        nm, ph, bo = where(o)
        print(" %-34s off 0x%06X stride %2d | raw mean %.1f w6/9/12 %.0f/%.0f/%.0f%% u %.0f/%.0f/%.0f%% | cor mean %.1f w6/9/12 %.0f/%.0f/%.0f%% u %.0f/%.0f/%.0f%% fit %s" % (
            nm[0][:34], bo, s, raw["mean"], 100*raw["w6"], 100*raw["w9"], 100*raw["w12"], 100*raw["u6"], 100*raw["u9"], 100*raw["u12"],
            cor["mean"], 100*cor["w6"], 100*cor["w9"], 100*cor["w12"], 100*cor["u6"], 100*cor["u9"], 100*cor["u12"], fit))
    print(" blocks: raw mean %.1f w6/9/12 %.0f/%.0f/%.0f  files %s" % (braw["mean"], 100*braw["w6"], 100*braw["w9"], 100*braw["w12"], sorted(set(x[0] for x in summary[k]["blocks"]["srcs"]))[:6]))
    # render custom | top1 | top2
    import civ_rebuild as CR
    L = json.load(open(CR.LAYOUT, encoding="utf-8"))
    pg = np.hstack([CR.page8(n, L) for n in T[k]["pages"]])
    def rgb(words): c = ch(words); return np.clip(np.round(c * 255 / 31), 0, 255).astype(np.uint8)
    pals = [("custom FMV-derived", rgb(t))] + [("%s 0x%X s%d" % (where(r[1])[0][0], where(r[1])[2], r[2]), rgb(window(r[1], r[2]))) for r in rows[:2]]
    H, Wd = pg.shape; im = Image.new("RGB", (Wd, (H + 14) * 3), (20, 20, 20)); dr = ImageDraw.Draw(im)
    for i, (lab, P) in enumerate(pals):
        im.paste(Image.fromarray(P[pg]), (0, i * (H + 14) + 14)); dr.text((2, i * (H + 14)), lab, fill=(255, 255, 255))
    im.save(os.path.join(OUTD, "cmp_%s.png" % k))
json.dump(summary, open(os.path.join(OUTD, "summary.json"), "w"), indent=1, default=str)
