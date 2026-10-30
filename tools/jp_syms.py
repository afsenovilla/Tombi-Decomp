#!/usr/bin/env python3
"""PAL names onto the Japanese retail disc (SLPS-01144) using the Ghidra function list of tools/ghidra_pipeline_jp.sh.

  python3 -I tools/jp_syms.py extract            game_jp/ <- SLPS_011.44, SYSTEM/X*.BIN, INFO2 from game_jp/disc/tomba_jp.bin
  python3 -I tools/jp_syms.py prep IN_DIR        <tag>.csv (names) + <tag>.funcs (starts) for the Ghidra apply step,
                                                 from config/slps01144/symbol_addrs_*.txt (xmatch/jpdemo_syms output)
  python3 -I tools/jp_syms.py match OUT_DIR [--md FILE] [--csv FILE]
                                                 OUT_DIR = Ghidra export dir with <tag>.funcs.csv (ExportFuncs.java).
                                                 Pass 1 exact masked words (relocations zeroed, like xmatch.py);
                                                 pass 2 same masked sequence ignoring nothing but jal/lui/mem offsets and
                                                 allowing an equal-size PAL twin; pass 3 fuzzy (difflib on masked words,
                                                 size within 30%, unique best >= 0.80); pass 4 call-graph propagation
                                                 (callees of a matched pair paired in call order).
Needs the PAL files only for `match`. Prints coverage per program and writes the unmatched list.
"""
import argparse, collections, csv, difflib, os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from xmatch import mask, words

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JP = os.path.join(ROOT, "game_jp")
OVL_BASE, SHARED = 0x800E9AF8, 0x2B680
# tag -> (file, vram of file offset 0, first offset that belongs to the program, symbol files)
PROGS = {"slps01144": ("SLPS_011.44", 0x80010000, 0x800, ["slps01144"]), "x00": ("SYSTEM/X00.BIN", OVL_BASE, 0, ["x00"]),
         "info2": ("SYSTEM/INFO2.BIN", OVL_BASE, 0, [])}
for n in (10, 11, 16, 17):
    PROGS["x%02d" % n] = ("SYSTEM/X%02d.BIN" % n, OVL_BASE, SHARED, ["x%02d" % n])
GENERIC = re.compile(r"(func|FUN|sub|LAB)_[0-9A-Fa-f]{8}(_\w+)?$")


def extract():
    import cdfiles, jp_text
    d = jp_text.read_disc(os.path.join(JP, "disc", "tomba_jp.bin"))
    for tag, (f, _, _, _) in PROGS.items():
        p = os.path.join(JP, f); os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "wb").write(d[f]); print(p, len(d[f]))


def symfile(name):
    out = []
    p = os.path.join(ROOT, "config", "slps01144", "symbol_addrs_%s.txt" % name)
    for l in open(p):
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func(?:.*size:0x([0-9A-Fa-f]+))?", l)
        if m: out.append((int(m.group(2), 16), m.group(1), int(m.group(3) or "0", 16), "PAL " in l))
    return out


def prep(d):
    os.makedirs(d, exist_ok=True)
    for tag, (f, vram, start, syms) in PROGS.items():
        rows = []
        for s in (["x00"] if tag.startswith("x") and tag != "x00" else []) + syms:
            rows += [r for r in symfile(s) if s != "x00" or tag == "x00" or r[0] < OVL_BASE + SHARED]  # X00's area part is not shared
        if tag == "info2": rows = []
        rows.sort()
        with open(os.path.join(d, tag + ".csv"), "w") as o:
            o.write("address,name,confidence,comment\n")
            for a, n, sz, pal in rows: o.write("%08x,%s,%s,%s\n" % (a, n, "alta" if pal else "baja", "JP retail"))
        open(os.path.join(d, tag + ".funcs"), "w").write("".join("%08x\n" % a for a, *_ in rows))
        print(tag, len(rows), "symbols")


def pal_side():
    """[(module, name, addr, size, masked words, raw bytes, callees)] for every PAL function with a size."""
    specs = [("main0", "game/MAIN0.EXE", 0x80010000, 0x800)]
    for f in sorted(os.listdir(os.path.join(ROOT, "config"))):
        m = re.fullmatch(r"((?:x0\d\d)|i0nfo2)\.yaml", f)
        if m:
            y = open(os.path.join(ROOT, "config", f)).read()
            specs.append((m.group(1), re.search(r"target_path:\s*(\S+)", y).group(1),
                          int(re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", y).group(1), 16), 0))
    out = []
    for mod, path, vram, o0 in specs:
        sym = os.path.join(ROOT, "config", "symbol_addrs_%s.txt" % mod)
        if not os.path.exists(os.path.join(ROOT, path)) or not os.path.exists(sym): continue
        b = open(os.path.join(ROOT, path), "rb").read()
        for a, n, s, _ in symfile_pal(sym):
            off = a - vram + o0
            if s >= 8 and off >= 0 and off + s <= len(b):
                fb = b[off:off + s]
                out.append([mod, n, a, s, tuple(mask(w) for w in words(fb)), fb, jals(fb)])
    return out


def symfile_pal(p):
    for l in open(p):
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func.*size:0x([0-9A-Fa-f]+)", l)
        if m: yield int(m.group(2), 16), m.group(1), int(m.group(3), 16), 0


def jals(fb):
    return [((w & 0x3FFFFFF) << 2) | 0x80000000 for w in words(fb) if w >> 26 == 3]


def load_jp(outdir):
    res = {}
    for tag, (f, vram, start, _) in PROGS.items():
        p = os.path.join(outdir, tag + ".funcs.csv")
        if not os.path.exists(p): continue
        b = open(os.path.join(JP, f), "rb").read()
        lo = vram + (start - (0x800 if tag == "slps01144" else 0)) if tag != "slps01144" else 0x80010000
        fl = []
        for r in csv.DictReader(open(p)):
            a, s = int(r["address"], 16), int(r["size"])
            off = a - vram + (0x800 if tag == "slps01144" else 0)
            if tag.startswith("x") and tag != "x00" and a < OVL_BASE + SHARED: continue   # shared code counted in x00
            if s >= 8 and off >= 0 and off + s <= len(b):
                fb = b[off:off + s]
                fl.append([tag, a, s, tuple(mask(w) for w in words(fb)), fb, jals(fb), r["name"]])
        res[tag] = fl
    return res


def match(outdir, md, csvp):
    pal = pal_side()
    named = [p for p in pal if not GENERIC.match(p[1])]
    print("PAL functions %d, with real name %d" % (len(pal), len(named)))
    jp = load_jp(outdir)
    byseq = collections.defaultdict(list)
    for p in named: byseq[p[4]].append(p)
    result = {}      # (tag, addr) -> (palname, palmod, paladdr, how)
    palused = collections.Counter()
    allf = {(f[0], f[1]): f for fl in jp.values() for f in fl}
    # shared X00 code is the same address in every overlay; a PAL address -> JP address map feeds the call graph
    jp_by_addr = {}
    for (tag, a), f in allf.items(): jp_by_addr.setdefault(a, f)
    pal_by_addr = {}
    for p in named: pal_by_addr.setdefault((p[0] == "main0", p[2]), p)
    # pass 0: names already in config/slps01144/symbol_addrs_*.txt with a `PAL <module> 0x<addr>` comment (xmatch output)
    for tag in jp:
        for sn in (["x00"] if tag.startswith("x") and tag != "x00" else []) + PROGS[tag][3]:
            for l in open(os.path.join(ROOT, "config", "slps01144", "symbol_addrs_%s.txt" % sn)):
                m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*PAL (\w+) 0x([0-9A-Fa-f]+)", l)
                if m and (tag, int(m.group(2), 16)) in allf and not GENERIC.match(m.group(1)):
                    result[(tag, int(m.group(2), 16))] = (m.group(1), m.group(3), int(m.group(4), 16), "symfile")
    # pass 1: exact masked sequence; several PAL twins -> first by size order (all identical code), flagged "twin"
    for (tag, a), f in allf.items():
        c = byseq.get(f[3])
        if c and (tag, a) not in result:
            p = c[0]
            result[(tag, a)] = (p[1], p[0], p[2], "exact" if len(c) == 1 else "exact-twin%d" % len(c))
    # pass 3: fuzzy on the rest
    bysize = sorted(named, key=lambda p: p[3])
    import bisect
    sizes = [p[3] for p in bysize]
    for (tag, a), f in allf.items():
        if (tag, a) in result or f[2] < 24: continue
        lo, hi = bisect.bisect_left(sizes, int(f[2] * 0.7)), bisect.bisect_right(sizes, int(f[2] * 1.3))
        best, bs, second = None, 0, 0
        jh = set(f[3])
        for p in bysize[lo:hi]:
            if len(jh & set(p[4])) < 0.6 * max(len(jh), 1): continue
            r = difflib.SequenceMatcher(None, f[3], p[4], autojunk=False).ratio()
            if r > bs: best, bs, second = p, r, bs
            elif r > second: second = r
        if best and bs >= 0.80 and bs - second >= 0.03:
            result[(tag, a)] = (best[1], best[0], best[2], "fuzzy%.2f" % bs)
    # pass 4: call graph propagation (callees paired in call order when both sides have the same callee count)
    palf = {(p[0], p[2]): p for p in pal}
    for _ in range(4):
        new = 0
        for (tag, a), (pn, pm, pa, how) in list(result.items()):
            f = allf[(tag, a)]; p = palf.get((pm, pa))
            if not p or len(f[5]) != len(p[6]) or not f[5]: continue
            for ja, pca in zip(f[5], p[6]):
                jt = jp_by_addr.get(ja)
                if jt is None or (jt[0], jt[1]) in result: continue
                pc = palf.get(("main0" if 0x80010000 <= pca < 0x800E0000 and pm == "main0" else pm, pca)) or \
                     palf.get(("main0", pca)) if 0x80010000 <= pca < 0x800E0000 else palf.get((pm, pca))
                if pc and not GENERIC.match(pc[1]) and abs(pc[3] - jt[2]) <= max(16, jt[2] // 4):
                    result[(jt[0], jt[1])] = (pc[1], pc[0], pc[2], "callgraph"); new += 1
        if not new: break
    # pass 5: structural similarity (opcode/funct 4-grams, Dice) for the rest: JP code that is a variant of a PAL function
    def shape(seq): return [(w >> 26) << 6 | ((w & 0x3F) if w >> 26 == 0 else 0) for w in seq]
    def grams(sh): return collections.Counter(tuple(sh[i:i + 4]) for i in range(len(sh) - 3))
    pg = {id(p): grams(shape(p[4])) for p in named if p[3] >= 32}
    post = collections.defaultdict(list)
    for p in named:
        if id(p) in pg:
            for g in pg[id(p)]: post[g].append(p)
    weak = {}
    for (tag, a), f in allf.items():
        if (tag, a) in result or f[2] < 32: continue
        fg = grams(shape(f[3])); votes = collections.Counter()
        for g, c in fg.items():
            pl = post.get(g)
            if pl and len(pl) < 200:
                for p in pl: votes[id(p)] += min(c, pg[id(p)][g])
        cand = {id(p): p for pl in [post[g] for g in list(fg)[:400] if g in post and len(post[g]) < 200] for p in pl}
        best, bs, second = None, 0.0, 0.0
        nf = sum(fg.values())
        for pid, v in votes.most_common(8):
            p = cand.get(pid)
            if p is None: continue
            d = 2.0 * v / (nf + sum(pg[pid].values()))
            if d > bs: best, bs, second = p, d, bs
            elif d > second: second = d
        if best is not None:
            if bs >= 0.70 and bs - second >= 0.03: result[(tag, a)] = (best[1], best[0], best[2], "similar%.2f" % bs)
            else: weak[(tag, a)] = (best[1], best[0], best[2], bs)
    rows, lines = [], []
    tot = collections.OrderedDict()
    for tag, fl in jp.items():
        n = len(fl); m = sum((tag, f[1]) in result for f in fl)
        kinds = collections.Counter(result[(tag, f[1])][3].rstrip("0123456789.") for f in fl if (tag, f[1]) in result)
        bm = sum(f[2] for f in fl if (tag, f[1]) in result); bt = sum(f[2] for f in fl)
        tot[tag] = (n, m, bt, bm, kinds)
    for (tag, a), (pn, pm, pa, how) in sorted(result.items()):
        rows.append((tag, "%08X" % a, allf[(tag, a)][2], pn, pm, "%08X" % pa, how))
    if csvp:
        with open(csvp, "w", newline="") as o:
            csv.writer(o).writerows([("prog", "jp_addr", "size", "pal_name", "pal_mod", "pal_addr", "how")] + rows)
    print("%-10s %6s %6s %6s %9s %9s" % ("program", "funcs", "named", "pct", "bytes", "named_b"))
    for tag, (n, m, bt, bm, kinds) in tot.items():
        print("%-10s %6d %6d %5.1f%% %9d %9d  %s" % (tag, n, m, 100 * m / max(n, 1), bt, bm, dict(kinds)))
    un = [(tag, f[1], f[2], f[6]) for tag, fl in jp.items() for f in fl if (tag, f[1]) not in result]
    if md:
        with open(md, "w") as o:
            for (t, a, sz, nm) in un:
                w = weak.get((t, a))
                o.write("%s %08X %d %s %s\n" % (t, a, sz, nm, ("best=%s@%s:%.2f" % (w[0], w[1], w[3])) if w else "best=-"))
    return result, un, tot


if __name__ == "__main__":
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["extract", "prep", "match"])
    ap.add_argument("dir", nargs="?")
    ap.add_argument("--md"); ap.add_argument("--csv")
    a = ap.parse_args()
    if a.cmd == "extract": extract()
    elif a.cmd == "prep": prep(a.dir)
    else: match(a.dir, a.md, a.csv)
