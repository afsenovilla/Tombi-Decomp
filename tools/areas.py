#!/usr/bin/env python3
"""Area overlays (game/AREAnn/X0nn.BIN): function boundaries and "twins" of already matched functions.

All 17 unique area overlays of the PAL Spanish release are raw code blobs loaded at 0x800E8028
(see notes/overlays.md). X000 was analysed with Ghidra; the others are analysed here:

  python3 tools/areas.py bounds            writes notes/functions_x0nn.csv (address,size,name,twin)
  python3 tools/areas.py twins [X001 ...]  copies matched src/*.c whose retail bytes are identical (relocation
                                           fields masked) to src/x0nn/, renaming overlay-internal symbols,
                                           then verifies them with tools/ncheck.py (adds // MATCHING)
  python3 tools/areas.py check            heuristic boundaries of X000 vs Ghidra's notes/functions_x000.csv

Boundaries: seeds = jal targets inside the overlay + the twins found by masked byte search; every seed is
swept linearly until a `jr ra` that no forward branch skips; code that follows a function end directly
continues as a new function when it starts with `addiu sp,sp,-N`, is a jal target or is referenced by a data word;
remaining gaps are scanned for valid code ending in `jr ra`. Details in notes/overlays.md.
`twins` never overwrites an existing src/x0nn/ file that is already marked // MATCHING.
"""
import csv, os, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BASE = 0x800E8028
OVERLAYS = {"X000": "AREA00/X000.BIN"}
for _n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19):
    OVERLAYS["X%03d" % _n] = "AREA%02d/X%03d.BIN" % (_n, _n)
NEW = [p for p in OVERLAYS if p != "X000"]
ENTRY_STUBS = (0x800E9ECC, 0x800E9F74)  # header pointer targets: jal/j stubs + shared epilogue (not a C function)
JR_RA = 0x03E00008


def path(prog):
    return os.path.join(ROOT, "game", OVERLAYS[prog])


def load(prog):
    return open(path(prog), "rb").read()


def words(d):
    return struct.unpack("<%dI" % (len(d) // 4), d[:len(d) // 4 * 4])


def branch_target(w, pc):
    """Target of a conditional branch / local j, else None."""
    op = w >> 26
    if op in (4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17) or (op == 1 and ((w >> 16) & 0x1F) in (0, 1, 2, 3, 0x10, 0x11)):
        off = w & 0xFFFF
        if off & 0x8000: off -= 0x10000
        return pc + 4 + off * 4
    if op == 2:
        return ((w & 0x3FFFFFF) << 2) | (pc & 0xF0000000)
    return None


def valid(w):
    """Rough R3000 opcode validity (data rejection)."""
    op = w >> 26
    if op == 0:
        fn = w & 0x3F
        return fn in (0, 2, 3, 4, 6, 7, 8, 9, 12, 13, 16, 17, 18, 19, 24, 25, 26, 27, 32, 33, 34, 35, 36, 37, 38, 39, 42, 43)
    if op == 1: return ((w >> 16) & 0x1F) in (0, 1, 0x10, 0x11)
    if op == 0x10 or op == 0x12: return True  # cop0 / gte
    return op in (2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
                  0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A)


def is_prologue(w):
    return (w & 0xFFFF8000) == 0x27BD8000  # addiu sp,sp,-N


def sweep(W, i, stop, base):
    """Index after the function starting at word i (stop = index of the next known seed), or None if data."""
    maxf = i
    k = i
    while k < stop:
        w = W[k]
        if not valid(w): return None
        t = branch_target(w, base + 4 * k)
        if t is not None:
            ti = (t - base) // 4
            if ti > maxf: maxf = ti
        if w == JR_RA and k + 2 > maxf:
            return k + 2
        k += 1
    return stop


def functions(prog, extra=()):
    """[(addr, size)] for one overlay."""
    d = load(prog)
    W = words(d)
    n = len(W)
    lo, hi = BASE, BASE + 4 * n
    calls = set()
    for k, w in enumerate(W):
        if w >> 26 == 3:
            t = ((w & 0x3FFFFFF) << 2) | 0x80000000
            if lo <= t < hi and t % 4 == 0: calls.add((t - lo) // 4)
    datarefs = {(w - lo) // 4 for w in W if lo <= w < hi and w % 4 == 0}
    seeds = sorted({s for s in calls | {(a - lo) // 4 for a in extra} if s >= (ENTRY_STUBS[1] - lo) // 4})
    out = {}
    pend = list(seeds)
    seedset = set(seeds)
    done = set()
    while pend:
        pend.sort()
        s = pend.pop(0)
        if s in done: continue
        done.add(s)
        if any(a <= s < e for a, e in out.items()): continue
        nxt = next((x for x in sorted(seedset | set(out)) if x > s), n)
        e = sweep(W, s, nxt, lo)
        if e is None or e <= s: continue
        out[s] = e
        # code that follows directly
        c = e
        while c < n and W[c] == 0 and c + 1 < n and W[c + 1] == 0: c += 1  # nop padding (rare)
        if c < n and c not in seedset and c not in out and (is_prologue(W[c]) or c in datarefs):
            seedset.add(c); pend.append(c)
    return [(lo + 4 * a, 4 * (e - a)) for a, e in sorted(out.items())]


# ------------------------------------------------------------------------------------- masked search
MEMI = {9, 13, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x32, 0x3A}


def masks(W):
    """Per-word AND mask hiding the relocation fields: jal/j targets, lui immediates and the %lo
    immediates of instructions based on a lui'd register (followed through `addu at,at,reg`)."""
    out, marked = [], set()
    for w in W:
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        mk = 0xFFFFFFFF
        if op in (2, 3): mk = 0xFC000000
        elif op == 15: mk = 0xFFFF0000
        elif op in MEMI and rs in marked: mk = 0xFFFF0000
        out.append(mk)
        if op == 15:
            marked.add(rt); continue
        if op == 0:
            if (w & 0x3F) == 33 and (rs in marked or rt in marked) and rd:
                marked.add(rd); continue
            dst = rd
        elif 8 <= op <= 14 or 0x20 <= op <= 0x26: dst = rt
        elif op == 3: dst = 31
        else: dst = None
        marked.discard(dst)
    return out


class Image:
    def __init__(self, prog):
        self.prog = prog
        self.d = load(prog)
        self.W = words(self.d)
        self.n = len(self.W)
        self.idx = {}
        W = self.W
        for k in range(self.n - 2):
            self.idx.setdefault((W[k], W[k + 1], W[k + 2]), []).append(k)
        lo, hi = BASE, BASE + 4 * self.n
        self.calls = set()
        for w in W:
            if w >> 26 == 3:
                t = ((w & 0x3FFFFFF) << 2) | 0x80000000
                if lo <= t < hi: self.calls.add((t - lo) // 4)
        self.datarefs = {(w - lo) // 4 for w in W if lo <= w < hi and w % 4 == 0}

    def plausible(self, c, nwords):
        W = self.W
        if c in self.calls or c in self.datarefs: return True
        if nwords <= 4: return False
        return (c >= 2 and W[c - 2] == JR_RA) or is_prologue(W[c]) or \
            (c >= 1 and (BASE <= W[c - 1] < BASE + 4 * self.n or not valid(W[c - 1])))  # after data

    def find(self, P):
        """Word indexes where the masked pattern P (list of words) occurs at a plausible function start."""
        M = masks(P)
        L = len(P)
        if L > self.n: return []
        anchor = next((j for j in range(L - 2) if M[j] == M[j + 1] == M[j + 2] == 0xFFFFFFFF
                       and (P[j], P[j + 1], P[j + 2]) != (0, 0, 0)), None)
        if anchor is None:
            cands = sorted(self.calls)
        else:
            cands = [k - anchor for k in self.idx.get((P[anchor], P[anchor + 1], P[anchor + 2]), ()) if k >= anchor]
        W = self.W
        out = []
        for c in cands:
            if c + L > self.n: continue
            if all((W[c + i] & M[i]) == (P[i] & M[i]) for i in range(L)) and self.plausible(c, L):
                out.append(c)
        return out


HEXNAME = re.compile(r"^((?:PTR_)?(?:D|DAT|FUN|func|LAB|PTR|jtbl))_([0-9a-fA-F]{8})(.*)$")


def rehex(name, addr):
    """FUN_800e9f74 -> FUN_<addr> keeping the prefix, suffix and hex case."""
    m = HEXNAME.match(name)
    h = m.group(2)
    up = any(c in "ABCDEF" for c in h) or (not any(c in "abcdef" for c in h) and m.group(1) in ("func", "D", "jtbl"))
    return m.group(1) + "_" + (("%08X" if up else "%08x") % addr) + m.group(3)


def src_functions():
    """Matched src/*.c (MAIN0 and X000): [(stem, prog, addr, size, path)]."""
    out = []
    for f in sorted(os.listdir(os.path.join(ROOT, "src"))):
        if not f.endswith(".c"): continue
        p = os.path.join(ROOT, "src", f)
        t = open(p, errors="replace").read()
        m = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)\s*(\w+)?", t)
        if not m or "// MATCHING" not in t: continue
        out.append((f[:-2], m.group(3) or "MAIN0", int(m.group(1), 16), int(m.group(2)), p))
    return out


_MAIN0 = None


def prog_words(prog, addr, size):
    global _MAIN0
    if prog == "MAIN0":
        if _MAIN0 is None: _MAIN0 = open(os.path.join(ROOT, "game", "MAIN0.EXE"), "rb").read()
        load_ = struct.unpack("<I", _MAIN0[0x18:0x1C])[0]
        o = 0x800 + addr - load_
        return list(words(_MAIN0[o:o + size]))
    d = load(prog)
    o = addr - BASE
    return list(words(d[o:o + size]))


def names_x000():
    return {int(r["address"], 16): r["name"] for r in csv.DictReader(open(os.path.join(ROOT, "notes", "names_x000.csv")))}


def analyse(progs):
    """For each overlay: functions [(addr, size, name, kind, twin)] with kind src|x000|new.
    twin = (prog, addr, stem-or-None) of the identical MAIN0/X000 function."""
    srcf = src_functions()
    srcf.sort(key=lambda r: -r[3])
    nx = names_x000()
    gh = [(a, s) for a, s in ghidra("X000")]
    gh.sort(key=lambda r: -r[1])
    pats_src = [(r, prog_words(r[1], r[2], r[3])) for r in srcf]
    pats_gh = [((None, "X000", a, s, None), prog_words("X000", a, s)) for a, s in gh]
    res = {}
    for prog in progs:
        im = Image(prog)
        taken = {}   # start index -> (end index, info)
        occ = [False] * im.n

        def claim(c, L, info):
            if any(occ[c:c + L]): return False
            for i in range(c, c + L): occ[i] = True
            taken[c] = (c + L, info)
            return True
        for kind, pats in (("src", pats_src), ("x000", pats_gh)):
            for (stem, tp, ta, ts, path), P in pats:
                if ts < 8 or len(P) * 4 != ts: continue
                if kind == "x000" and not (len(P) >= 2 and P[-2] == JR_RA): continue  # Ghidra fragment, not a function
                for c in im.find(P):
                    if c < (ENTRY_STUBS[1] - BASE) // 4: continue
                    claim(c, len(P), (kind, tp, ta, stem, path))
        fixed = {BASE + 4 * c: 4 * (e - c) for c, (e, _) in taken.items()}
        fl = functions(prog, fixed)
        out = []
        used = {}
        for a, s in fl:
            c = (a - BASE) // 4
            if c in taken and BASE + 4 * taken[c][0] == a + s:
                kind, tp, ta, stem, path = taken[c][1]
                if kind == "src":
                    name = rehex(stem, a) if HEXNAME.match(stem) else stem
                else:
                    nm = nx.get(ta)
                    name = nm if nm and not HEXNAME.match(nm) else "func_%08X" % a
                    stem = None
                twin = (tp, ta, stem, path)
            else:
                kind, name, twin = "new", "func_%08X" % a, None
            if name in used: name = "%s_%08X" % (name, a)
            used[name] = a
            out.append((a, s, name, kind, twin))
        res[prog] = out
    return res


LOADSTORE = set(range(0x20, 0x27)) | set(range(0x28, 0x2C)) | {0x32, 0x3A}


def strict(w):
    """Valid instruction that is not a typical data word (pointer = `lb rt,imm(zero)`)."""
    return valid(w) and not ((w >> 26) in LOADSTORE and ((w >> 21) & 31) == 0)


def strict_sweep(W, i, stop, base):
    """(end, None) for code from i to a final `jr ra`, else (None, index of the offending word)."""
    maxf = i
    for k in range(i, stop):
        w = W[k]
        if not strict(w): return None, k
        t = branch_target(w, base + 4 * k)
        if t is not None:
            ti = (t - base) // 4
            if ti < i or ti >= stop: return None, k
            if ti > maxf: maxf = ti
        if w == JR_RA and k + 2 > maxf and k + 2 <= stop:
            return k + 2, None
    return None, stop - 1


def functions(prog, fixed=None):
    """[(addr, size)] for one overlay; `fixed` = {addr: size} known boundaries (twins)."""
    fixed = fixed or {}
    d = load(prog)
    W = words(d)
    n = len(W)
    lo = BASE
    im_calls = set()
    for w in W:
        if w >> 26 == 3:
            t = ((w & 0x3FFFFFF) << 2) | 0x80000000
            if lo <= t < lo + 4 * n: im_calls.add((t - lo) // 4)
    datarefs = {(w - lo) // 4 for w in W if lo <= w < lo + 4 * n and w % 4 == 0}
    fx = {(a - lo) // 4: (a - lo + s) // 4 for a, s in fixed.items()}
    first = (ENTRY_STUBS[1] - lo) // 4
    seedset = {s for s in im_calls if s >= first} | set(fx)
    out = {}
    pend = sorted(seedset)
    done = set()
    import bisect
    while pend:
        pend.sort()
        s = pend.pop(0)
        if s in done: continue
        done.add(s)
        if s in fx:
            e = fx[s]
        else:
            if any(a <= s < e and a in fx for a, e in out.items()): continue
            starts = sorted(seedset | set(out))
            nxt = starts[bisect.bisect_right(starts, s)] if bisect.bisect_right(starts, s) < len(starts) else n
            e = sweep(W, s, nxt, lo)
            if e is None or e <= s: continue
        # a seed inside a function found earlier splits it (jal into the middle, as Ghidra does)
        for a in list(out):
            if a < s < out[a] and a not in fx: out[a] = s
        out[s] = e
        c = e
        if c < n and c not in seedset and c not in out and (is_prologue(W[c]) or c in datarefs):
            seedset.add(c); pend.append(c)
    # fill the gaps: leaf functions reached only through pointers built with lui/addiu, code after data
    cov = [False] * n
    for a, e in out.items():
        for i in range(a, e): cov[i] = True
    i = first
    while i < n:
        if cov[i]: i += 1; continue
        j = i
        while j < n and not cov[j]: j += 1
        c = i
        while c < j:
            while c < j and (W[c] == 0 or not strict(W[c])): c += 1
            if c >= j: break
            e, bad = strict_sweep(W, c, j, lo)
            if e is None:
                c = bad + 1; continue
            out[c] = e
            c = e
        i = j
    # drop non-fixed functions overlapping fixed ones
    res = []
    for a in sorted(out):
        e = out[a]
        if a not in fx:
            e2 = min([x for x in fx if a < x < e] or [e])
            if any(x <= a < fx[x] for x in fx): continue
            e = e2
        res.append((lo + 4 * a, 4 * (e - a)))
    return res


def ghidra(prog):
    return [(int(r["address"], 16), int(r["size"])) for r in csv.DictReader(open(os.path.join(ROOT, "notes", "functions_%s.csv" % prog.lower())))]


def cmd_check():
    mine = functions("X000")
    gh = ghidra("X000")
    ms, gs = dict(mine), dict(gh)
    same = sum(1 for a in gs if ms.get(a) == gs[a])
    print("ours %d (%d B), ghidra %d (%d B), identical %d" % (len(ms), sum(ms.values()), len(gs), sum(gs.values()), same))
    onlyg = [(hex(a), s) for a, s in gh if a not in ms]
    onlym = [(hex(a), s) for a, s in mine if a not in gs]
    diff = [(hex(a), ms[a], gs[a]) for a in gs if a in ms and ms[a] != gs[a]]
    print("only ghidra", len(onlyg), onlyg[:30])
    print("only ours", len(onlym), onlym[:30])
    print("size differs", len(diff), diff[:30])


def cmd_bounds(progs):
    """notes/functions_x0nn.csv: address,size,name,twin. twin = PROG:address of an identical function (masked
    relocations): a MAIN0/X000 function, or the first occurrence in an earlier overlay (X001 < X002 < ...)."""
    res = analyse(progs)
    seen = {}
    for prog in progs:
        im = load(prog)
        rows = []
        for a, s, name, kind, twin in res[prog]:
            tw = "%s:%08x" % (twin[0], twin[1]) if twin else ""
            if not twin:
                W = list(words(im[a - BASE:a - BASE + s]))
                sig = tuple(w & m for w, m in zip(W, masks(W)))
                if sig in seen and s > 8: tw = seen[sig]
                else: seen.setdefault(sig, "%s:%08x" % (prog, a))
            rows.append((a, s, name, tw))
        with open(os.path.join(ROOT, "notes", "functions_%s.csv" % prog.lower()), "w") as f:
            f.write("address,size,name,twin\n")
            for r in rows: f.write("%08x,%d,%s,%s\n" % r)
        k = {}
        for a, s, name, kind, twin in res[prog]:
            k.setdefault(kind, [0, 0]); k[kind][0] += 1; k[kind][1] += s
        dup = sum(r[1] for r in rows if r[3][:2] == "X0" and not r[3].startswith("X000"))
        print(prog, len(rows), "functions", sum(r[1] for r in rows), "B", k, "same as an earlier overlay: %d B" % dup)
    return res


# ----------------------------------------------------------------------------------------------- twins
def _nc():
    src = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
    NC = {"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"}
    exec(compile(src, "ncheck", "exec"), NC)
    return NC


def _inc():
    import shutil, tempfile
    inc = tempfile.mkdtemp()
    for f in os.listdir(os.path.join(ROOT, "include")):
        s = os.path.join(ROOT, "include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(inc, nm))
    return inc


def compile_relocs(NC, path, flags, inc):
    """(.text bytes, .text relocations [(off, type, sym)], other-section relocations [(sec, sym)]) or None."""
    import shutil, subprocess, tempfile
    d = tempfile.mkdtemp()
    del NC["RELS"][:]; del NC["LOCAL_J"][:]
    try:
        code, _ = NC["build"](path, flags, d, inc)
    except subprocess.CalledProcessError:
        shutil.rmtree(d, ignore_errors=True); return None
    rels = list(NC["RELS"])
    other, sec = [], None
    for line in subprocess.run(["mipsel-linux-gnu-objdump", "-r", d + "/a.o"], capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"RELOCATION RECORDS FOR \[(\S+)\]", line)
        if m: sec = m.group(1); continue
        m = re.match(r"[0-9a-f]{8}\s+R_MIPS_\w+\s+(\S+)", line)
        if m and sec.startswith((".data", ".rodata", ".sdata", ".rdata")) and not m.group(1).startswith("."):
            other.append((sec, m.group(1)))
    shutil.rmtree(d, ignore_errors=True)
    return code, rels, other


def sym_map(code, rels, tb, ob):
    """{sym: (address in the twin program, address in the overlay)}: retail relocated field minus the
    addend of our object (REL: the addend sits in our instruction). None if inconsistent."""
    W = lambda b, o: struct.unpack_from("<I", b, o)[0]
    lo16 = lambda x: (x & 0xFFFF) - 0x10000 if x & 0x8000 else x & 0xFFFF
    res, hi = {}, {}
    for off, ty, sym in rels:
        if sym.startswith("."): continue
        if off + 4 > min(len(code), len(tb)): return None
        if ty == "R_MIPS_26":
            add = (W(code, off) & 0x3FFFFFF) << 2
            t = ((W(tb, off) & 0x3FFFFFF) << 2) | 0x80000000
            o = ((W(ob, off) & 0x3FFFFFF) << 2) | 0x80000000
        elif ty == "R_MIPS_HI16":
            hi[sym] = off; continue
        elif ty == "R_MIPS_LO16" and sym in hi:
            h = hi[sym]
            add = ((W(code, h) & 0xFFFF) << 16) + lo16(W(code, off))
            t = ((W(tb, h) & 0xFFFF) << 16) + lo16(W(tb, off))
            o = ((W(ob, h) & 0xFFFF) << 16) + lo16(W(ob, off))
        else:
            return None
        res.setdefault(sym, set()).add(((t - add) & 0xFFFFFFFF, (o - add) & 0xFFFFFFFF))
    out = {}
    for sym, v in res.items():
        if len(v) != 1: return None
        out[sym] = v.pop()
    return out


def cmd_twins(progs):
    import shutil, subprocess
    res = analyse(NEW)
    NC = _nc(); inc = _inc()
    cache = {}
    report = {}
    for prog in progs:
        outdir = os.path.join(ROOT, "src", prog.lower())
        os.makedirs(outdir, exist_ok=True)
        name_at = {a: n for a, s, n, k, tw in res[prog]}
        im = load(prog)
        written, skipped = [], []
        for a, s, name, kind, twin in res[prog]:
            if kind != "src": continue
            tp, ta, stem, path = twin
            dst = os.path.join(outdir, name + ".c")
            if os.path.exists(dst) and "// MATCHING" in open(dst, errors="replace").read():
                continue  # already there (maybe edited by hand): keep it
            text = open(path, errors="replace").read()
            h = NC["M"]["header"](path)
            if path not in cache: cache[path] = compile_relocs(NC, path, h[3], inc)
            cr = cache[path]
            if cr is None: skipped.append((name, "twin does not compile")); continue
            code, rels, other = cr
            tb = bytes(struct.pack("<%dI" % (s // 4), *prog_words(tp, ta, s)))
            ob = im[a - BASE:a - BASE + s]
            sm = sym_map(code, rels, tb, ob)
            if sm is None: skipped.append((name, "inconsistent relocations")); continue
            new = {}
            for sym, (taddr, oaddr) in sm.items():
                if taddr == oaddr: continue
                if HEXNAME.match(sym): new[sym] = rehex(sym, oaddr)
                elif any(r[1] == "R_MIPS_26" and r[2] == sym for r in rels):
                    new[sym] = name_at.get(oaddr, "func_%08X" % oaddr)
                else: new[sym] = "D_%08X" % oaddr
            ovl = lambda n: HEXNAME.match(n) and BASE <= int(HEXNAME.match(n).group(2), 16) < 0x80140000
            if any(sym in new or ovl(sym) for sec, sym in other):
                skipped.append((name, "data relocations to overlay symbols")); continue
            new[stem] = name
            new = {k: v for k, v in new.items() if k != v}
            body = re.sub(r"//\s*MATCHING[^\n]*\n", "", text)
            body = re.sub(r"//\s*FUNC\s+[0-9a-fA-F]+\s+\d+[ \t]*\w*", "// FUNC %08x %d %s" % (a, s, prog), body, count=1)
            if new:
                body = re.sub(r"\b(%s)\b" % "|".join(sorted(map(re.escape, new), key=len, reverse=True)),
                              lambda m: new[m.group(1)], body)
            open(dst, "w").write(body)
            written.append(name)
        report[prog] = (written, skipped)
    shutil.rmtree(inc, ignore_errors=True)
    # verify with ncheck: keep only exact matches, marked
    for prog in progs:
        written, skipped = report[prog]
        files = [os.path.join(ROOT, "src", prog.lower(), n + ".c") for n in written]
        ok = set()
        for k in range(0, len(files), 200):
            r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", "ncheck.py"), "--mark"] + files[k:k + 200],
                               capture_output=True, text=True)
            for line in r.stdout.splitlines():
                m = re.match(r"MATCH\s+(\S+)\.c", line)
                if m: ok.add(m.group(1))
                elif not line.startswith(("MATCH", "   ")) and "match," not in line: print(prog, line)
        for n in written:
            if n not in ok:
                os.unlink(os.path.join(ROOT, "src", prog.lower(), n + ".c")); skipped.append((n, "ncheck DIFF"))
        print("%s: %d twins match, %d skipped %s" % (prog, len(ok), len(skipped), skipped[:8]))


if __name__ == "__main__":
    a = sys.argv[1:]
    if a and a[0] == "check": cmd_check()
    elif a and a[0] == "bounds": cmd_bounds(a[1:] or NEW)
    elif a and a[0] == "twins": cmd_twins(a[1:] or NEW)
