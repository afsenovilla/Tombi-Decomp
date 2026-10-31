#!/usr/bin/env python3
"""Tomba! 2 (USA demo) vs Tomba! 1: measurements behind docs/TOMBA2_DEMO.md. Reads only; prints.

  python3 -I tools/tomba2_demo.py code    T2FS     T1 function matching (masked exact + fuzzy) on every T2 code file
  python3 -I tools/tomba2_demo.py blocks  T2FS DIR...   shared raw 64-byte blocks between T2 files and T1 files
  python3 -I tools/tomba2_demo.py strings T2FS DIR...   T2 ASCII strings (>=6) that also occur in T1 files
  python3 -I tools/tomba2_demo.py reverse T2FS     every non-lib T1 function -> best fuzzy match in T2 code
  python3 -I tools/tomba2_demo.py entropy T2FS     per-file size / entropy / first words

T2FS = the demo's extracted file system (tools/isoext.py). DIR = T1 trees to compare against (PAL fs, unpacked GAMs,
JP, beta, PrePre8...).

code: T1 functions = every `type:func size:` symbol of every config/*.yaml module (main0, x0nn, i0nfo*, d0spsub*,
g0over, o0ptsub0) with the xmatch.py mask (j/jal targets, lui and non-$sp memory/addiu/ori immediates zeroed).
  exact  = the masked T1 sequence occurs in the T2 file (longest first, no overlap); "identical" = raw bytes equal too.
  fuzzy  = T2 code not covered by an exact hit is cut at addiu sp,sp,-N prologues; each piece (>= MIN bytes) gets the
           best Jaccard of masked 3-word shingles against all T1 functions. Library classification: tools/units.py
           (MAIN0/X000 rows flagged Psy-Q); names from other modules are looked up in the MAIN0/X000 lib set by name.
"""
import collections, math, os, re, struct, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from xmatch import mask, words

CODE = ["TOMBA2/MAIN.EXE", "TOMBA2/BIN/A00.BIN", "TOMBA2/BIN/SOP.BIN", "TOMBA2/BIN/OPN.BIN", "TOMBA2/BIN/GAME.BIN",
        "TOMBA2/BIN/DEMO.BIN", "TOMBA2/BIN/START.BIN", "MAIN.EXE", "SCUS_944.64", "PSX.EXE"]


def t1_funcs():
    out = []
    for f in sorted(os.listdir(os.path.join(ROOT, "config"))):
        if not f.endswith(".yaml"): continue
        mod = f[:-5]; y = open(os.path.join(ROOT, "config", f)).read()
        tp = re.search(r"target_path:\s*(\S+)", y).group(1)
        sym = os.path.join(ROOT, "config", "symbol_addrs_%s.txt" % mod)
        if not os.path.exists(os.path.join(ROOT, tp)) or not os.path.exists(sym): continue
        b = open(os.path.join(ROOT, tp), "rb").read()
        if b[:8] == b"PS-X EXE": vram, o0 = struct.unpack("<I", b[0x18:0x1c])[0], 0x800
        else: vram, o0 = int(re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", y).group(1), 16), 0
        for line in open(sym):
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func.*size:0x([0-9A-Fa-f]+)", line)
            if not m: continue
            a, s = int(m.group(2), 16), int(m.group(3), 16); off = a - vram + o0
            if s >= 20 and 0 <= off and off + s <= len(b):
                fb = b[off:off + s]; out.append((mod, m.group(1), a, s, tuple(mask(w) for w in words(fb)), fb))
    out.sort(key=lambda f: -f[3])
    return out


# Psy-Q libpad/libapi functions that the PAL names csv does not flag as Psy-Q (named by hand in this project);
# the demo menu MAIN.EXE (another developer's program) contains the same set, which confirms they are library code.
LIBRX = re.compile(r"^(_?[pP]ad|_dir|sio|setRC2wait|chkRC2wait|intrMask|(Set|Get|Start|Reset)RCnt)")
LIBX = set()


def is_lib(name, lib): return name in lib or bool(LIBRX.match(name))


def lib_names():
    try:
        import units as U
        lib = set()
        for prog, us in U.units().items():
            for u in us:
                if u.lib: lib.add(u.name)
        return lib | LIBX
    except Exception as e:
        print("units.py unavailable (%s): no library split" % e); return set()


def load(path):
    b = open(path, "rb").read()
    return b[0x800:] if b[:8] == b"PS-X EXE" else b


def cmd_code(fs, MIN=48):
    funcs = t1_funcs(); lib = lib_names()
    print("T1 functions with size (all modules): %d" % len(funcs))
    seen = {}
    for f in funcs: seen.setdefault(f[4], f)
    funcs = list(seen.values())
    def sh(seq): return {tuple(seq[k:k + 3]) for k in range(len(seq) - 2)}
    idx, psh = collections.defaultdict(list), []
    for n, f in enumerate(funcs):
        s = sh(f[4]); psh.append(s)
        for t in s: idx[t].append(n)
    for rel in CODE:
        p = os.path.join(fs, rel)
        if not os.path.exists(p): continue
        b = load(p); w = words(b); tm = [mask(x) for x in w]
        k4 = collections.defaultdict(list)
        for i in range(len(tm) - 4): k4[tuple(tm[i:i + 4])].append(i)
        cov, hits = bytearray(len(tm)), []
        for f in funcs:
            n = len(f[4])
            for i in k4.get(f[4][:4], ()):
                if tuple(tm[i:i + n]) == f[4] and not any(cov[i:i + n]):
                    cov[i:i + n] = b"\1" * n; hits.append((i * 4, f, b[i * 4:i * 4 + f[3]] == f[5])); break
        hl = [h for h in hits if is_lib(h[1][1], lib)]; hg = [h for h in hits if not is_lib(h[1][1], lib)]
        print("\n== %s (%d bytes)" % (rel, len(b)))
        print("exact masked hits: %d (%d B), identical %d | lib %d (%d B) | non-lib %d (%d B)" % (
            len(hits), sum(h[1][3] for h in hits), sum(h[2] for h in hits), len(hl), sum(h[1][3] for h in hl),
            len(hg), sum(h[1][3] for h in hg)))
        for o, f, ident in sorted(hg, key=lambda h: -h[1][3]):
            print("   non-lib hit 0x%06X %-6s %-34s %4d B %s" % (o, f[0], f[1], f[3], "ident" if ident else ""))
        # fuzzy over the rest
        pieces, i = [], 0
        while i < len(w):
            if cov[i]: i += 1; continue
            j = i
            while j < len(w) and not cov[j]: j += 1
            st = [i] + [k for k in range(i + 1, j) if w[k] >> 16 == 0x27BD and (w[k] & 0x8000)] + [j]
            pieces += [(s, e) for s, e in zip(st, st[1:]) if (e - s) * 4 >= MIN and any(w[s:e])]
            i = j
        hist = collections.Counter(); top = []
        for s, e in pieces:
            ts = sh(tm[s:e])
            if not ts: continue
            cnt = collections.Counter()
            for t in ts:
                for n in idx.get(t, ()): cnt[n] += 1
            bs, bn = 0.0, None
            for n, c in cnt.most_common(10):
                jv = c / (len(ts) + len(psh[n]) - c)
                if jv > bs: bs, bn = jv, n
            band = "90-100" if bs >= .9 else "60-89" if bs >= .6 else "30-59" if bs >= .3 else "<30"
            hist[band] += 1
            if bs >= .3: top.append((int(bs * 100), s * 4, (e - s) * 4, funcs[bn][0], funcs[bn][1], is_lib(funcs[bn][1], lib)))
        print("fuzzy pieces >= %d B: %d; best-Jaccard bands %s" % (MIN, len(pieces), dict(hist)))
        print("fuzzy >= 30 by best-match class: lib %d, non-lib %d" % (sum(r[5] for r in top), sum(not r[5] for r in top)))
        for r in sorted(top, reverse=True):
            if r[5] and r[0] < 90: continue
            print("   sim %3d  0x%06X %5d B  ~ %-6s %s%s" % (r[0], r[1], r[2], r[3], r[4], " [lib]" if r[5] else ""))


def cmd_reverse(fs, MIN=32):
    """For every non-library T1 function (>= MIN bytes), the best Jaccard against any same-sized window of T2 code
    (MAIN.EXE + BIN overlays), windows cut at prologues as in `code`. Prints the band histogram and the best ones."""
    funcs = [f for f in t1_funcs() if f[3] >= MIN]; lib = lib_names()
    funcs = [f for f in funcs if not is_lib(f[1], lib)]
    def sh(seq): return {tuple(seq[k:k + 3]) for k in range(len(seq) - 2)}
    pieces = []
    for rel in CODE[:7]:
        p = os.path.join(fs, rel)
        if not os.path.exists(p): continue
        w = words(load(p)); tm = [mask(x) for x in w]
        st = [0] + [k for k in range(1, len(w)) if w[k] >> 16 == 0x27BD and (w[k] & 0x8000)] + [len(w)]
        for a, b in zip(st, st[1:]):
            if (b - a) * 4 >= 16: pieces.append((rel, a * 4, (b - a) * 4, sh(tm[a:b])))
    idx = collections.defaultdict(list)
    for n, pc in enumerate(pieces):
        for t in pc[3]: idx[t].append(n)
    hist = collections.Counter(); rows = []; seen = set()
    for f in funcs:
        if f[4] in seen: continue
        seen.add(f[4]); fs_ = sh(f[4]); cnt = collections.Counter()
        for t in fs_:
            for n in idx.get(t, ()): cnt[n] += 1
        bs, bn = 0.0, None
        for n, c in cnt.most_common(10):
            j = c / (len(fs_) + len(pieces[n][3]) - c)
            if j > bs: bs, bn = j, n
        hist["90-100" if bs >= .9 else "60-89" if bs >= .6 else "30-59" if bs >= .3 else "<30"] += 1
        if bs >= .5: rows.append((int(bs * 100), f[0], f[1], f[3], pieces[bn][0], pieces[bn][1], pieces[bn][2]))
    print("distinct non-lib T1 functions >= %d B: %d; best-Jaccard vs T2 code: %s" % (MIN, len(seen), dict(hist)))
    for r in sorted(rows, reverse=True): print("   sim %3d  %-6s %-36s %5d B  ~ %s+0x%06X (%d B)" % r)


def files_under(dirs, skip=()):
    for d in dirs:
        for r, _, fs in os.walk(d):
            for f in fs:
                p = os.path.join(r, f)
                if not any(s in p for s in skip) and os.path.getsize(p) > 0: yield p


def interesting(blk):
    return len(set(blk)) >= 12


def cmd_blocks(fs, dirs, B=64):
    idx = {}
    for p in files_under(dirs, skip=(".c", "/out/", ".cue")):
        if p.upper().endswith((".BIN",)) and os.path.getsize(p) > 50_000_000: continue  # disc images
        b = open(p, "rb").read()
        for o in range(0, len(b) - B + 1, B):
            k = b[o:o + B]
            if interesting(k): idx.setdefault(hash(k), (p, o))
    print("T1 indexed %d distinct %d-byte blocks" % (len(idx), B))
    for p in files_under([fs]):
        if os.path.getsize(p) > 20_000_000: continue
        b = open(p, "rb").read(); hits = collections.Counter(); n = 0; i = 0
        while i <= len(b) - B:
            k = b[i:i + B]
            if interesting(k) and hash(k) in idx:
                hits[os.path.relpath(idx[hash(k)][0])] += 1; n += 1; i += B
            else: i += 4
        print("%-32s %9d B  shared blocks %6d  %s" % (os.path.relpath(p, fs), len(b), n, hits.most_common(5)))


def strs(b, n=6):
    return set(m.group().decode() for m in re.finditer(rb"[\x20-\x7e]{%d,}" % n, b))


def cmd_strings(fs, dirs):
    t1 = set()
    for p in files_under(dirs, skip=(".c", "/out/")):
        if os.path.getsize(p) > 50_000_000: continue
        t1 |= strs(open(p, "rb").read())
    for p in files_under([fs]):
        if os.path.getsize(p) > 20_000_000: continue
        s = strs(open(p, "rb").read()); c = sorted(x for x in s & t1 if len(set(x)) >= 4)
        print("== %s: %d strings, %d also in T1" % (os.path.relpath(p, fs), len(s), len(c)))
        for x in c: print("   %r" % x)


def cmd_entropy(fs):
    for p in sorted(files_under([fs])):
        b = open(p, "rb").read(min(os.path.getsize(p), 4 << 20)); c = collections.Counter(b)
        e = -sum(v / len(b) * math.log2(v / len(b)) for v in c.values())
        print("%-32s %9d  H=%.2f  %s" % (os.path.relpath(p, fs), os.path.getsize(p), e, b[:16].hex()))


if __name__ == "__main__":
    c, fs, rest = sys.argv[1], sys.argv[2], sys.argv[3:]
    {"code": lambda: cmd_code(fs), "blocks": lambda: cmd_blocks(fs, rest), "strings": lambda: cmd_strings(fs, rest),
     "reverse": lambda: cmd_reverse(fs), "entropy": lambda: cmd_entropy(fs)}[c]()
