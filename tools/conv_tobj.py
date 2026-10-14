#!/usr/bin/env python3
"""Raw offset accesses on TObj* variables -> TObj fields: U8/S8/U16/S16/S32/PTR(v, off) and *(T*)((char*)v + off).
Per file: try all variables; else greedy per variable; keep only linked-object-identical results (tools/objcmp.py).
Usage: conv_tobj.py files..."""
import re, sys, os, subprocess, tempfile
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools")); os.chdir(ROOT)
import objcmp
fields = []
for l in open("include/tobj.h"):
    m = re.match(r"\s*(unsigned char|short|unsigned short|int|void \*|Fix16 \*|Fix16)\s+(\w+);\s*/\* \+0x([0-9a-f]+)", l)
    if m and not m.group(2).startswith("_pad"):
        fields.append((int(m.group(3), 16), m.group(2), m.group(1)))
fields.sort()
SIG = {"unsigned char": ("uc", 1), "short": ("s", 2), "unsigned short": ("us", 2), "int": ("i", 4), "void *": ("p", 4), "Fix16 *": ("p", 4)}
MAC = {"U8": "unsigned char", "S8": "signed char", "U16": "unsigned short", "S16": "short", "S32": "int", "PTR": "void *", "U8F": "unsigned char"}
CT = {"unsigned char": "uc", "signed char": "sc", "u8": "uc", "s8": "sc", "char": "uc", "short": "s", "s16": "s", "unsigned short": "us", "u16": "us", "int": "i", "s32": "i", "u32": "ui", "unsigned int": "ui", "void *": "p", "void*": "p"}

def access(v, off, ctype):
    """expression text for an lvalue of C type ctype at TObj offset off of pointer variable v, or None"""
    cands = [f for f in fields if f[0] <= off]
    if not cands or off >= 0xC0: return None
    fo, n, ft = cands[-1]
    if ft == "Fix16":
        if off == fo: base, ft2, d = "%s->%s.raw" % (v, n), "int", 0
        elif off == fo + 2: base, ft2, d = "%s->%s.p.whole" % (v, n), "short", 0
        else: return None
    else:
        base, ft2, d = "%s->%s" % (v, n), ft, off - fo
        if d and d >= {"unsigned char": 1, "short": 2, "unsigned short": 2, "int": 4}.get(ft, 4): return None
    k = CT.get(ctype.strip())
    if k is None: return None
    if d == 0 and ft2 in SIG and SIG[ft2][0] == k: return base
    if d == 0 and k == "p" and ft2 in ("void *", "Fix16 *"): return base
    if d: return "(*(%s*)((char*)&%s + %d))" % (ctype, base, d)
    return "(*(%s*)&%s)" % (ctype, base)

def convert(s, vars_, allow=None):
    for v in vars_:
        V = re.escape(v)
        def m1(m):
            if allow is not None and (v, int(m.group(2), 0)) not in allow: return m.group(0)
            r = access(v, int(m.group(2), 0), MAC[m.group(1)])
            return r if r else m.group(0)
        s = re.sub(r"\b(U8F|U8|S8|U16|S16|S32|PTR)\(%s, *(0x[0-9a-fA-F]+|\d+)\)" % V, m1, s)
        def m2(m):
            if allow is not None and (v, int(m.group(2), 0)) not in allow: return m.group(0)
            r = access(v, int(m.group(2), 0), m.group(1))
            return r if r else m.group(0)
        s = re.sub(r"\*\(((?:unsigned )?(?:char|short|int)|u8|s8|u16|s16|u32|s32|void ?\*) ?\*\)\(\((?:unsigned char|char|u8) ?\*\)%s \+ (0x[0-9a-fA-F]+|\d+)\)" % V, m2, s)
    return s

def tvars(s):
    vs = set()
    for m in re.finditer(r"\bTObj\s*\*([^;(){]*)[;,)=]", s):
        pass
    for m in re.finditer(r"\bTObj\s*(\*\s*\w+(?:\s*(?:=[^,;]*)?,\s*\*\s*\w+)*)", s):
        vs.update(re.findall(r"\*\s*(\w+)", m.group(1)))
    return sorted(vs)

def work(a):
    f, inc, tmp = a
    if not objcmp.NC: objcmp.init()
    orig = open(f, errors="surrogateescape").read()
    if not re.search(r"\b(U8F|U8|S8|U16|S16|S32|PTR)\(\w+, *0x|\)\s*\+ *0x", orig): return f, False
    vs = tvars(orig)
    def attempt(vlist):
        new = convert(orig, vlist)
        if new == orig: return None
        open(f, "w", errors="surrogateescape").write(new)
        if objcmp.job((f, inc, tmp)) is None: return new
        open(f, "w", errors="surrogateescape").write(orig); return None
    if not vs: return f, False
    r = attempt(vs)
    if r is None:
        keep = []
        for v in vs:
            new = convert(orig, keep + [v])
            if new == orig: continue
            open(f, "w", errors="surrogateescape").write(new)
            if objcmp.job((f, inc, tmp)) is None: keep.append(v)
        open(f, "w", errors="surrogateescape").write(orig)
        if keep:
            open(f, "w", errors="surrogateescape").write(convert(orig, keep))
            return f, True
        # stage 3: per (variable, offset)
        occ = set()
        for v in vs:
            for m in re.finditer(r"\b(?:U8F|U8|S8|U16|S16|S32|PTR)\(%s, *(0x[0-9a-fA-F]+|\d+)\)" % re.escape(v), orig): occ.add((v, int(m.group(1), 0)))
            for m in re.finditer(r"\)%s \+ (0x[0-9a-fA-F]+|\d+)\)" % re.escape(v), orig): occ.add((v, int(m.group(1), 0)))
        allow = set()
        for o_ in sorted(occ):
            new = convert(orig, vs, allow | {o_})
            if new == orig: continue
            open(f, "w", errors="surrogateescape").write(new)
            if objcmp.job((f, inc, tmp)) is None: allow.add(o_)
        open(f, "w", errors="surrogateescape").write(convert(orig, vs, allow) if allow else orig)
        return f, bool(allow)
    return f, True

if __name__ == "__main__":
    from multiprocessing import Pool
    tmp = tempfile.mkdtemp(); inc = os.path.join(tmp, "inc"); objcmp.mkinc(inc, None)
    files = [f for f in sys.argv[1:] if re.search(r'TOBJ\.H|tobj\.h', open(f, errors="surrogateescape").read())]
    with Pool(8) as p: res = p.map(work, [(f, inc, tmp) for f in files])
    print("converted %d of %d" % (sum(1 for f, ok in res if ok), len(files)))
