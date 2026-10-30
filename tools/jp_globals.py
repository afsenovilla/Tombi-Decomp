#!/usr/bin/env python3
"""PAL -> JP (SLPS-01144) address table for globals and callees, learned from already-paired functions.

For every JP function with a PAL twin (`PAL <module> <addr>` comment of config/slps01144/symbol_addrs_<prog>.txt, or
config/slps01144/reuse.txt) both bodies are turned into a masked opcode skeleton (addresses/immediates of lui, loads, stores,
addiu, ori, j, jal dropped), aligned with difflib, and every aligned pair of instructions that reference an address (lui+lo
pairs tracked linearly; jal/j targets) casts one vote: PAL named symbol (base) -> JP address. A symbol is accepted when one
JP address gets >= 80 % of its votes. Global blocks (GAME, GVARS, ...) vote by base (jp_access - (pal_access - pal_base)).

  python3 tools/jp_globals.py            report only
  python3 tools/jp_globals.py --write    rewrite the `// BEGIN jp_globals` blocks of config/slps01144/symbol_addrs_<prog>.txt

The PAL name is then also the JP name, so shared PAL C compiles for JP (relocation names agree). Scratchpad (0x1F80xxxx)
is not mapped (same address in both). JP addresses that existing JP C refers to by their raw D_/func_ name are skipped."""
import collections, difflib, glob, os, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT + "/tools")
from xmatch import mask

MEMOPS = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2a, 0x2b, 0x2e, 0x32, 0x3a}
BEGIN, END = "// BEGIN jp_globals (tools/jp_globals.py)", "// END jp_globals"
BLOCKS = {"GCD": (0x8009CD00, 0x400), "GV2": (0x8009D2A0, 0x40), "GV6": (0x8009D600, 0xB0), "GVARS": (0x8009C930, 0x30),
          "PLAYER": (0x800A6038, 0x178)}


def rd(path):
    return open(os.path.join(ROOT, path), "rb").read()


class Image:
    """A code image: bytes at vram, 4-byte words."""
    def __init__(self, path, vram, off):
        self.b, self.vram, self.off = rd(path), vram, off

    def words(self, a, size):
        o = a - self.vram + self.off
        if o < 0 or o + size > len(self.b): return None
        return struct.unpack("<%dI" % (size // 4), self.b[o:o + size])


def yaml_img(ver_dir, prog):
    y = open("%s/config/%s/%s.yaml" % (ROOT, ver_dir, prog)).read()
    tp = re.search(r"target_path:\s*(\S+)", y).group(1)
    vram = int(re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", y).group(1), 16)
    return Image(tp, vram, 0x800 if prog == "slps01144" else 0)


def sx(v): return v - 0x10000 if v & 0x8000 else v


def refs(ws, base):
    """index -> ('d', addr) for lo-type instructions whose lui is known, ('j', target) for j/jal."""
    hi, out = {}, {}
    for i, w in enumerate(ws):
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op == 0x0F:
            hi[rt] = (w & 0xFFFF) << 16; continue
        if op in (2, 3):
            out[i] = ("j", ((w & 0x3FFFFFF) << 2) | 0x80000000); hi.clear() if False else None; continue
        if (op in MEMOPS or op in (9, 0x0D)) and rs in hi and rs != 29:
            lo = w & 0xFFFF
            out[i] = ("d", (hi[rs] + (lo if op == 0x0D else sx(lo))) & 0xFFFFFFFF)
        # register written by this instruction loses its lui value
        if op == 0 and w != 0:
            f = w & 63
            if f not in (8, 0x18, 0x19, 0x1A, 0x1B, 0x11, 0x13): hi.pop((w >> 11) & 31, None)
        elif op in (8, 9, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E) or (0x20 <= op <= 0x26):
            hi.pop(rt, None) if not (op in (9, 0x0D) and rs == rt and False) else None
        elif op in (3,): pass
    return out


def load_pal_names():
    """PAL named globals: address -> (name, size). From notes/data_names_*.csv of main0/x000 + the struct blocks."""
    syms = {}
    for f in ("main0", "x000"):
        for l in open("%s/notes/data_names_%s.csv" % (ROOT, f)).read().splitlines()[1:]:
            p = l.split(",", 3)
            if len(p) < 3: continue
            a = int(p[0], 16)
            if a >= 0x80000000:
                sz = int(p[2], 16) if p[2].startswith("0x") else 0
                syms[a] = (p[1], sz)
    for n, (a, s) in BLOCKS.items(): syms[a] = (n, s)
    return syms


def load_pal_funcs():
    """address -> name of PAL functions (config/symbol_addrs_main0.txt and symbol_addrs_x0nn; size)."""
    out, size = {}, {}
    for p in glob.glob(ROOT + "/config/symbol_addrs_*.txt"):
        mod = os.path.basename(p)[13:-4]
        for l in open(p):
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func(?:.*size:0x([0-9A-Fa-f]+))?", l)
            if m and not m.group(1).startswith("func_"):
                out[int(m.group(2), 16)] = m.group(1)
                if m.group(3): size[(mod, int(m.group(2), 16))] = int(m.group(3), 16)
    return out, size


def jp_symbol_files():
    return {os.path.basename(p)[13:-4]: p for p in sorted(glob.glob(ROOT + "/config/slps01144/symbol_addrs_*.txt"))
            if not p.endswith("manual.txt")}


def jp_pairs():
    """[(jp prog, jp addr, jp size, pal mod, pal addr)]"""
    pr, seen = [], set()
    palsize = load_pal_funcs()[1]
    for prog, p in jp_symbol_files().items():
        for l in open(p):
            m = re.match(r"\s*\w+\s*=\s*0x([0-9A-Fa-f]+);.*size:0x([0-9A-Fa-f]+)\s+PAL (\w+) 0x([0-9A-Fa-f]+)", l)
            if m: pr.append((prog, int(m.group(1), 16), int(m.group(2), 16), m.group(3), int(m.group(4), 16)))
    size = {(p, a): s for p, a, s, _, _ in pr}
    for l in open(ROOT + "/config/slps01144/reuse.txt"):
        m = re.match(r"(\w+) ([0-9A-Fa-f]{8}) (\w+) ([0-9A-Fa-f]{8})", l)
        if m:
            k = (m.group(1), int(m.group(2), 16))
            s = palsize.get((m.group(3), int(m.group(4), 16)))
            if k not in size and s: pr.append((k[0], k[1], s, m.group(3), int(m.group(4), 16))); size[k] = s
    return pr


def main():
    write = "--write" in sys.argv
    extra = []
    palsyms = load_pal_names()
    palfuncs, _ = load_pal_funcs()
    sorted_syms = sorted(palsyms)
    import bisect

    def container(a):
        """smallest named PAL symbol containing a (exact start first)."""
        if a in palsyms: return a
        best = None
        i = bisect.bisect_right(sorted_syms, a)
        for b in sorted_syms[max(0, i - 40):i]:
            s = palsyms[b][1]
            if s and b <= a < b + s and (best is None or s < palsyms[best][1]): best = b
        return best

    PALSIZE = {}
    if "--neartwin" in sys.argv:   # also learn from near-twins (similar skeleton, different size): tools/jp_neartwin.py candidates
        import jp_neartwin as NT
        for prog, a, s, name, us in NT.candidates(0.9):
            u = us[0]; mm = NT.MOD[u.file]; extra.append((prog, a, s, mm, u.addr)); PALSIZE[(mm, u.addr)] = u.size
    pal_img = {"main0": Image("game/MAIN0.EXE", 0x80010000, 0x800)}
    for p in glob.glob(ROOT + "/config/x0??.yaml"):
        m = os.path.basename(p)[:-5]
        try: pal_img[m] = yaml_img("", m) if False else None
        except Exception: pass
    for p in glob.glob(ROOT + "/config/x0??.yaml"):
        m = os.path.basename(p)[:-5]
        y = open(p).read()
        tp = re.search(r"target_path:\s*(\S+)", y).group(1)
        if os.path.exists(os.path.join(ROOT, tp)):
            pal_img[m] = Image(tp, int(re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", y).group(1), 16), 0)
    jp_img = {}
    for prog in jp_symbol_files():
        try: jp_img[prog] = yaml_img("slps01144", prog)
        except Exception: pass
    # votes
    dvote = collections.defaultdict(collections.Counter)   # pal base -> Counter(jp base)
    dprog = collections.defaultdict(set)                   # pal base -> progs that reference it
    fvote = collections.defaultdict(collections.Counter)   # pal func addr -> Counter(jp addr)
    fprog = collections.defaultdict(set)
    npairs = 0
    for prog, ja, js, mod, pa in jp_pairs() + extra:
        if prog not in jp_img or mod not in pal_img: continue
        pw, jw = pal_img[mod].words(pa, PALSIZE.get((mod, pa), js)), jp_img[prog].words(ja, js)
        if pw is None:
            # near twins can have a different size; take the PAL size when known
            continue
        if jw is None: continue
        npairs += 1
        pm, jm = [mask(w) for w in pw], [mask(w) for w in jw]
        pr_, jr_ = refs(pw, pa), refs(jw, ja)
        sm = difflib.SequenceMatcher(None, pm, jm, autojunk=False)
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag != "equal": continue
            for k in range(i2 - i1):
                a, b = pr_.get(i1 + k), jr_.get(j1 + k)
                if not a or not b or a[0] != b[0]: continue
                if a[0] == "j":
                    if a[1] in palfuncs: fvote[(prog, a[1])][b[1]] += 1
                elif a[1] < 0x80000000 or b[1] < 0x80000000: continue
                else:
                    c = container(a[1])
                    if c is not None:
                        dvote[c][b[1] - (a[1] - c)] += 1; dprog[c].add(prog)
    # accept
    gdata, gfunc = {}, {}
    for c, cnt in dvote.items():
        (jb, n), tot = cnt.most_common(1)[0], sum(cnt.values())
        if n >= 1 and n / tot >= 0.7: gdata[c] = (jb, n, tot)
    for (pg, c), cnt in fvote.items():
        (jb, n), tot = cnt.most_common(1)[0], sum(cnt.values())
        if n / tot >= 0.8: gfunc[(pg, c)] = (jb, n, tot)
    # names inside an accepted block follow the block (aliases / fields): jp = pal + delta; weak votes (>= 60 %) agree or are replaced
    for bn in ("GAME", "GVARS", "GCD", "GV2", "GV6", "PLAYER"):
        pa_ = [a for a, (n, s) in palsyms.items() if n == bn][0]
        if pa_ not in gdata: print("block %s not resolved: %s" % (bn, dvote.get(pa_))); continue
        delta, bsz = gdata[pa_][0] - pa_, palsyms[pa_][1]
        print("block %s: PAL 0x%08X -> JP 0x%08X (delta %+#x, votes %d/%d)" % (bn, pa_, gdata[pa_][0], delta, gdata[pa_][1], gdata[pa_][2]))
        for a, (n, s) in palsyms.items():
            if pa_ < a < pa_ + bsz and n not in BLOCKS:
                if a in gdata and gdata[a][0] != a + delta: print("  alias %s votes 0x%08X (own votes kept), block says 0x%08X" % (n, gdata[a][0], a + delta))
                if a not in gdata: gdata[a] = (a + delta, 0, 0)
    # JP existing names
    jpnamed, jpaddr = {}, collections.defaultdict(list)
    for prog, p in jp_symbol_files().items():
        for l in open(p):
            if l.startswith(BEGIN): break
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)", l)
            if m: jpnamed[m.group(1)] = int(m.group(2), 16); jpaddr[int(m.group(2), 16)].append(m.group(1))
    # raw names used by JP-only C (do not rename those addresses)
    raw = set()
    for f in glob.glob(ROOT + "/src/**/*.c", recursive=True):
        t = open(f, errors="replace").read()
        if f.endswith("_slps01144.c"):
            raw |= {int(x, 16) for x in re.findall(r"\bD_([0-9A-F]{8})\b", t)} | {int(x, 16) for x in re.findall(r"\bfunc_([0-9A-F]{8})\b", t)}
    # resolve JP-address clashes: one PAL name per JP address (most votes)
    owner = {}
    for c, (jb, n, tot) in sorted(gdata.items(), key=lambda kv: -kv[1][1]):
        owner.setdefault(jb, c)
    gdata = {c: v for c, v in gdata.items() if owner[v[0]] == c}
    auto = collections.defaultdict(set)   # functions splat already found (build/slps01144/undefined_funcs_auto.<prog>.txt)
    for f in glob.glob(ROOT + "/build/slps01144/undefined_funcs_auto.*.txt"):
        pg = os.path.basename(f).split(".")[1]
        for l in open(f):
            m = re.match(r"\w+ = 0x([0-9A-Fa-f]+)", l)
            if m: auto[pg].add(int(m.group(1), 16))
    outd, outf, skipped = {}, {}, collections.Counter()
    for c, (jb, n, tot) in gdata.items():
        name, size = palsyms[c]
        if jb in jpaddr: skipped["JP address already named"] += 1; continue
        if jb >= 0x800C0000: skipped["overlay-resident data (naming it re-splits the overlay)"] += 1; continue
        if jb in raw: skipped["exact base used raw by JP C (pinned)"] += 1; continue
        outd[name] = (jb, size, c)
    # raw D_ names that existing JP C uses and that now fall inside a named symbol stay as exact symbols (keeps those files matching)
    pins = set()
    for name, (jb, s, c) in outd.items():
        for r in raw:
            if jb <= r < jb + max(s, 1) and r not in jpaddr and r >= 0x80000000 and not (r == jb): pins.add(r)
    pins = {r for r in pins if re.search(r"\bD_%08X\b" % r, "".join(open(f, errors="replace").read() for f in glob.glob(ROOT + "/src/**/*_slps01144.c", recursive=True)))}
    print("pinned raw D_ names: %d" % len(pins))
    fown = {}
    for (pg, c), (jb, n, tot) in sorted(gfunc.items(), key=lambda kv: -kv[1][1]):
        fown.setdefault((pg, jb), c)
    for (pg, c), (jb, n, tot) in gfunc.items():
        if fown[(pg, jb)] != c: skipped["func addr clash"] += 1; continue
        name = palfuncs[c]
        if jb in raw: skipped["raw func_ used by JP C"] += 1; continue
        if jb in jpaddr: skipped["JP func already named"] += 1; continue
        if jb not in auto.get(pg.lstrip("_"), ()) and jb not in auto.get(pg, ()) and not all(jb not in s for s in auto.values()) is False and False: pass
        if "--funcs" not in sys.argv: skipped["callee names off (--funcs; they inflate the code total)"] += 1; continue
        if name in jpnamed or (pg, name) in outf: skipped["name taken"] += 1; continue
        outf[(pg, name)] = (jb, c)
    print("pairs used %d; data symbols voted %d accepted %d (of %d candidates); functions accepted %d of %d" % (
        npairs, len(gdata), len(outd), len(dvote), len(outf), len(fvote)))
    print("skipped:", dict(skipped))
    rej = [(palsyms[c][0], dict(cnt.most_common(3))) for c, cnt in dvote.items() if c not in gdata]
    print("ambiguous/rejected data: %d" % len(rej))
    for r in rej[:10]: print("  ", r)
    if not write:
        for n, (jb, s, c) in sorted(outd.items(), key=lambda kv: kv[1][0])[:40]: print("  %s 0x%08X (PAL 0x%08X) size 0x%X" % (n, jb, c, s))
        return
    # write: per program, every data symbol (exe gets all; overlays the ones they reference) and functions
    for prog, p in jp_symbol_files().items():
        lines = []
        for r in sorted(pins):
            lines.append("D_%08X = 0x%08X; // pin raw name used by JP C" % (r, r)) if prog == "slps01144" else None
        for n, (jb, s, c) in sorted(outd.items(), key=lambda kv: kv[1][0]):
            if prog == "slps01144" or prog in dprog[c]:
                lines.append("%s = 0x%08X; // %sPAL 0x%08X" % (n, jb, ("size:0x%X " % s) if s else "", c))
        for (pg, n), (jb, c) in sorted(outf.items(), key=lambda kv: kv[1][0]):
            if prog == pg: lines.append("%s = 0x%08X; // type:func PAL 0x%08X" % (n, jb, c))
        t = open(p).read()
        if BEGIN in t: t = t[:t.index(BEGIN)].rstrip("\n") + "\n"
        if lines: t += BEGIN + "\n" + "\n".join(lines) + "\n" + END + "\n"
        open(p, "w").write(t)
    print("written")


if __name__ == "__main__":
    main()
