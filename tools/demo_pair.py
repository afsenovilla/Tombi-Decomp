#!/usr/bin/env python3
"""Fuzzy pairing for the demo / prototype builds: for every unmatched function of a version, the closest functions that
already have matching C anywhere in the repo, ranked.

The pool is every function with C of every version whose game files are present: PAL (tools/units.py, the PAL body),
JP retail SLPS-01144, USA SCUS-94236 and the demo builds themselves (SLPM-80165, BETA_980203, PCPX_96084; for those only
the functions their last report scores 100 %), each with the body that builds it in that version (its
`#if defined(VERSION_<SERIAL>)` branch, or the PAL body it pairs with) and its own bytes from that version's disc.

Similarity, per pair:
  - `ops`: difflib ratio of the instruction skeletons (opcode / funct / cop op only: registers, immediates and
    relocated addresses ignored), so a function that moved, got other registers or other struct offsets still pairs;
  - `masked`: the same over tools/xmatch.py masked words (registers kept), a tie-break;
  - `calls`: overlap of the callees, each mapped to a canonical name through the symbol tables (a version callee
    paired with PAL `<module> <addr>` counts as that PAL function), Jaccard;
  - `graph`: call-graph neighbours: a version function paired with a PAL function (symbol table or reuse.txt) calls
    its unknown callees in the same order as the PAL function calls its own, so the k-th unknown callee is proposed
    as the twin of the k-th PAL callee (aligned call sequences, difflib), and those twins rank first.
Score = 0.8 ops + 0.2 calls (ops alone when neither side calls anything), x0.97 per 10 % of size difference.
A prefilter (hashed opcode 4-grams, numpy) keeps the best 60 of the pool per function before difflib.

  python3 tools/demo_pair.py [--version slpm80165[,beta980203,pcpx96084]] [--top 8] [--min 0.5] [--show N]
writes build/<ver>/pairs.json: {"<prog> <addr>": [{score, ops, masked, calls, graph, ver, prog, addr, size, name,
path}, ...]} for every unmatched function (tools/demo_sweep.py --pairs reads it), and prints a summary."""
import difflib, json, os, re, struct, sys
from multiprocessing import Pool

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
os.chdir(ROOT)
from versions import VERSIONS, REFERENCE, canon
from xmatch import mask
import units as U

DEMOS = ["slpm80165", "beta980203", "pcpx96084"]
POOL_VERS = [REFERENCE, "slps01144", "scus94236"] + DEMOS
NB = 4096       # hashed 4-gram buckets of the prefilter
PRE = 60        # prefilter survivors per function
MINSIZE = 12


def tok(w):
    """Register- and immediate-insensitive instruction token."""
    if w == 0: return 0
    op = w >> 26
    if op == 0: return 0x100 | (w & 63)
    if op == 1: return 0x200 | ((w >> 16) & 31)
    if op in (0x10, 0x11, 0x12, 0x13):
        rs = (w >> 21) & 31
        return 0x10000 | ((op & 3) << 12) | (rs << 6) | ((w & 63) if rs >= 16 else 0)
    return 0x300 | op


def jal_targets(ws, addr):
    out = []
    for i, w in enumerate(ws):
        if w >> 26 == 3: out.append(((addr + 4 * i) & 0xF0000000) | ((w & 0x3FFFFFF) << 2))
    return out


class Fn:
    __slots__ = ("ver", "prog", "addr", "size", "name", "path", "words", "toks", "masked", "calls", "callseq", "pal")

    def __init__(self, ver, prog, addr, size, name, path, words, pal=None):
        self.ver, self.prog, self.addr, self.size, self.name, self.path, self.words = ver, prog, addr, size, name, path, words
        self.toks = [tok(w) for w in words]
        self.masked = [mask(w) for w in words]
        self.pal = pal  # canonical (PAL) name of this function, or None
        self.calls, self.callseq = set(), []


def pal_image(mod):
    P = mod.upper()
    if P not in U.BINS or not os.path.exists(U.BINS[P][0]): return None
    return open(U.BINS[P][0], "rb").read(), U.BINS[P][1]


def words_at(img, a, size):
    if img is None or size < 4: return None
    o, size = a - img[1], size & ~3
    if o < 0 or o + size > len(img[0]): return None
    return list(struct.unpack("<%dI" % (size // 4), img[0][o:o + size]))


class Namer:
    """Canonical callee names: PAL function names, a version function by its PAL pair (or its own name when it is not
    a splat func_<addr>)."""
    def __init__(self):
        self.pal_units = {}
        for p, us in U.units().items():
            for u in us:
                self.pal_units[(p.lower(), u.addr)] = u
        self.ver_maps = {}

    def pal_name(self, mod, a):
        for m in (mod, "main0", "x000"):
            u = self.pal_units.get((m, a))
            if u: return u.name
        return None

    def ver_map(self, ver):
        if ver in self.ver_maps: return self.ver_maps[ver]
        import demo_sweep as DS
        out = {}
        for (prog, a), (n, s, mod, pa) in DS.sym_table(ver).items():
            c = None
            if mod: c = self.pal_name(mod, pa)
            if c is None and not re.match(r"func_[0-9A-F]{8}", n): c = re.sub(r"_(%s)$" % "|".join(VERSIONS), "", n)
            if c: out.setdefault(a, c)
        self.ver_maps[ver] = out
        return out

    def name(self, ver, prog, a):
        if ver == REFERENCE: return self.pal_name(prog, a)
        return self.ver_map(ver).get(a)


def report_matched(ver):
    p = "build/%s/report.json" % ver
    if not os.path.exists(p): return None
    out = set()
    for un in json.load(open(p))["units"]:
        prog = un["name"].split("/")[0]
        for f in un.get("functions", []):
            if float(f.get("fuzzy_match_percent") or 0) >= 100: out.add((prog, f["name"]))
    return out


def build_pool(namer, log=print):
    import demo_sweep as DS, build_report as BR
    pool = []
    imgs = {}
    for (mod, a), u in namer.pal_units.items():
        if not u.file: continue
        if mod not in imgs: imgs[mod] = pal_image(mod)
        w = words_at(imgs[mod], a, u.size)
        if not w or len(w) * 4 < MINSIZE: continue
        f = Fn(REFERENCE, mod, a, u.size, u.name, u.file, w, u.name)
        pool.append(f)
    log("pool: PAL %d functions with C" % len(pool))
    for ver in POOL_VERS[1:]:
        progs = DS.progs_of(ver)
        if not progs or not all(DS.image(ver, p) for p in progs):
            log("pool: %s skipped (no game files)" % ver); continue
        table = DS.sym_table(ver)
        srcs = BR.jpdemo_sources(progs, ver)
        ok = report_matched(ver) if ver in DEMOS else None
        n0 = len(pool)
        for key, (path, name) in srcs.items():
            if key not in table: continue
            n, s, mod, pa = table[key]
            if ok is not None and (key[0], n) not in ok: continue
            if key[0] not in imgs or (ver, key[0]) not in imgs: imgs[(ver, key[0])] = DS.image(ver, key[0])
            w = words_at(imgs[(ver, key[0])], key[1], s)
            if not w or s < MINSIZE: continue
            pool.append(Fn(ver, key[0], key[1], s, n, path, w, namer.pal_name(mod, pa) if mod else None))
        log("pool: %s %d functions with C" % (ver, len(pool) - n0))
    for f in pool:
        f.callseq = [namer.name(f.ver, f.prog, t) for t in jal_targets(f.words, f.addr)]
        f.calls = {c for c in f.callseq if c}
    return pool


def grams(toks):
    import numpy as np
    v = np.zeros(NB, dtype=np.float32)
    for i in range(len(toks) - 3):
        v[hash(tuple(toks[i:i + 4])) % NB] = 1.0
    if len(toks) < 4: v[hash(tuple(toks)) % NB] = 1.0
    return v


_P = {}


def _score(job):
    ti, toks, masked, calls, size, cands = job
    pool = _P["pool"]
    sm = difflib.SequenceMatcher(None, autojunk=False)
    sm.set_seq2(toks)
    out = []
    for ci in cands:
        f = pool[ci]
        sm.set_seq1(f.toks)
        if sm.real_quick_ratio() < 0.4 or sm.quick_ratio() < 0.4: continue
        r = sm.ratio()
        if r < 0.4: continue
        m = difflib.SequenceMatcher(None, f.masked, masked, autojunk=False).ratio()
        cj = len(calls & f.calls) / len(calls | f.calls) if (calls or f.calls) else None
        s = 0.8 * r + 0.2 * cj if cj is not None else r
        s *= 0.97 ** (abs(f.size - size) / max(size, f.size) * 10)
        out.append((s, r, m, cj, ci))
    out.sort(key=lambda x: -x[0])
    return ti, out


class Pairer:
    def __init__(self, log=print):
        self.log = log
        self.namer = Namer()
        self.pool = build_pool(self.namer, log)

    def graph_pairs(self, ver, table, img_of):
        """{(prog, addr): {PAL name: votes}}: callees of version functions paired with PAL, aligned with the PAL
        function's own callees."""
        out = {}
        imgs = {}
        for (prog, a), (n, s, mod, pa) in table.items():
            if not mod: continue
            u = self.namer.pal_units.get((mod, pa))
            if not u: continue
            if mod not in imgs: imgs[mod] = pal_image(mod)
            pw, vw = words_at(imgs[mod], pa, u.size), words_at(img_of(prog), a, s)
            if not pw or not vw: continue
            pseq = [self.namer.pal_name(mod, t) for t in jal_targets(pw, pa)]
            vtg = jal_targets(vw, a)
            vseq = [self.namer.name(ver, prog, t) for t in vtg]
            sm = difflib.SequenceMatcher(None, [x or "?" for x in vseq], [x or "?" for x in pseq], autojunk=False)
            for op, i1, i2, j1, j2 in sm.get_opcodes():
                if op != "replace" or i2 - i1 != j2 - j1: continue
                for k in range(i2 - i1):
                    if vseq[i1 + k] is None and pseq[j1 + k]:
                        d = out.setdefault(vtg[i1 + k], {})
                        d[pseq[j1 + k]] = d.get(pseq[j1 + k], 0) + 1
        return out

    def pair(self, ver, top=8, minr=0.5, jobs=None):
        import numpy as np
        import demo_sweep as DS
        table = DS.sym_table(ver)
        um = DS.unmatched(ver, table)
        imgs = {}

        def img_of(prog):
            if prog not in imgs: imgs[prog] = DS.image(ver, prog)
            return imgs[prog]

        targets = []
        for key in sorted(um):
            uname, usize = um[key]
            name, size, mod, pa = table.get(key, (uname, usize, None, None))
            w = words_at(img_of(key[0]), key[1], size)
            if not w or size < MINSIZE: continue
            f = Fn(ver, key[0], key[1], size, name, None, w)
            f.callseq = [self.namer.name(ver, key[0], t) for t in jal_targets(w, key[1])]
            f.calls = {c for c in f.callseq if c}
            targets.append(f)
        pool = [f for f in self.pool if not (f.ver == ver and (f.prog, f.addr) in um)]
        self.log("%s: %d unmatched functions to pair against %d pool functions" % (ver, len(targets), len(pool)))
        G = np.stack([grams(f.toks) for f in pool])
        gn = G.sum(1)
        sizes = np.array([f.size for f in pool], dtype=np.float32)
        by_name = {}
        for i, f in enumerate(pool):
            if f.pal: by_name.setdefault(f.pal, []).append(i)
        gp = self.graph_pairs(ver, table, img_of)
        jl = []
        for ti, f in enumerate(targets):
            t = grams(f.toks)
            inter = G @ t
            sc = inter / np.maximum(np.minimum(gn, t.sum()), 1) * np.minimum(sizes, f.size) / np.maximum(sizes, f.size)
            ok = (sizes >= f.size * 0.5) & (sizes <= f.size * 2.0)
            sc = np.where(ok, sc, -1)
            cand = list(np.argsort(-sc)[:PRE])
            # call-graph twins and every version's body of the PAL function the best rows name
            extra = set()
            for nm in gp.get(f.addr, {}):
                extra.update(by_name.get(nm, []))
            if mod_pal := (table.get((f.prog, f.addr), (0, 0, None, None))):
                if mod_pal[2]:
                    nm = self.namer.pal_name(mod_pal[2], mod_pal[3])
                    extra.update(by_name.get(nm, []))
            cand = [int(c) for c in cand if sc[c] > 0] + sorted(extra)
            jl.append((ti, f.toks, f.masked, f.calls, f.size, list(dict.fromkeys(cand))))
        _P["pool"] = pool
        res = {}
        with Pool(jobs or os.cpu_count() or 4, initializer=_P.update, initargs=({"pool": pool},)) as P:
            for ti, out in P.imap_unordered(_score, jl, chunksize=8):
                res[ti] = out
        result = {}
        for ti, f in enumerate(targets):
            graph = set(gp.get(f.addr, {}))
            rows, seen_groups = [], {}
            for s, r, m, cj, ci in res.get(ti, []):
                p = pool[ci]
                g = p.pal in graph if p.pal else False
                if s < minr and not g: continue
                rows.append({"score": round(s + (0.1 if g else 0), 4), "ops": round(r, 4), "masked": round(m, 4),
                             "calls": None if cj is None else round(cj, 3), "graph": g, "ver": p.ver, "prog": p.prog,
                             "addr": "%08X" % p.addr, "size": p.size, "name": p.name, "pal": p.pal,
                             "path": os.path.relpath(p.path, ROOT)})
            rows.sort(key=lambda x: -x["score"])
            # every version's body of the best PAL functions comes along (a demo often matches the JP branch)
            keep, groups = [], []
            for row in rows:
                gk = row["pal"] or (row["ver"], row["addr"])
                if gk not in groups:
                    if len(groups) >= top: continue
                    groups.append(gk)
                keep.append(row)
            result["%s %08X" % (f.prog, f.addr)] = {"name": f.name, "size": f.size, "cands": keep}
        return result


def main():
    argv = sys.argv[1:]
    vers = [canon(v) for v in argv[argv.index("--version") + 1].split(",")] if "--version" in argv else \
        [v for v in DEMOS if os.path.exists("build/%s/report.json" % v)]
    top = int(argv[argv.index("--top") + 1]) if "--top" in argv else 8
    minr = float(argv[argv.index("--min") + 1]) if "--min" in argv else 0.5
    show = int(argv[argv.index("--show") + 1]) if "--show" in argv else 0
    P = Pairer()
    for ver in vers:
        res = P.pair(ver, top, minr)
        out = "build/%s/pairs.json" % ver
        json.dump(res, open(out, "w"), indent=0)
        n = len(res)
        hist = {}
        for k, v in res.items():
            b = v["cands"][0]["score"] if v["cands"] else 0
            hist[">=0.9" if b >= 0.9 else ">=0.8" if b >= 0.8 else ">=0.7" if b >= 0.7 else ">=0.5" if b >= 0.5 else "none"] = \
                hist.get(">=0.9" if b >= 0.9 else ">=0.8" if b >= 0.8 else ">=0.7" if b >= 0.7 else ">=0.5" if b >= 0.5 else "none", 0) + 1
        print("%s: %d unmatched functions, best twin score %s -> %s" % (ver, n, ", ".join("%s: %d" % kv for kv in sorted(hist.items())), out))
        for k, v in list(res.items())[:show]:
            c = v["cands"][:3]
            print("  %s %-28s %5d  %s" % (k, v["name"], v["size"], "; ".join("%.3f %s %s %s %d" % (x["score"], x["ver"], x["name"], x["path"], x["size"]) for x in c)))


if __name__ == "__main__":
    main()
