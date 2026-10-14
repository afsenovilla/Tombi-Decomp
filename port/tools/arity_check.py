#!/usr/bin/env python3
"""List calls that pass fewer arguments than the callee's definition takes.

On the R3000 such a call still "works": the callee reads whatever the caller left in $a0-$a3 (often the caller's
own first parameter, untouched).  The matching decomp reproduces these calls byte for byte with `f()`, but on a
stack-based ABI the callee reads garbage.  The port fixes them with port/fixes.txt (gen_port.py applies it).

Usage: python3 port/tools/arity_check.py [GEN_DIR]     (default build/port/gen: the filtered sources)"""
import os, re, sys, collections

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GEN = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "build", "port", "gen")


def strip(s):
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.S)
    s = re.sub(r"//[^\n]*", " ", s)
    return re.sub(r'"(?:[^"\\]|\\.)*"', '""', s)


def nparams(p):
    p = p.strip()
    if p in ("", "void"): return 0
    if "..." in p: return -1
    depth, n = 0, 1
    for c in p:
        if c == "(": depth += 1
        elif c == ")": depth -= 1
        elif c == "," and depth == 0: n += 1
    return n


def args_at(s, i):
    """number of top-level arguments of the call whose '(' is at s[i]"""
    depth, n, empty = 0, 1, True
    for j in range(i, min(len(s), i + 4000)):
        c = s[j]
        if c == "(": depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0: return 0 if empty else n
        elif c == "," and depth == 1: n += 1
        elif not c.isspace() and depth >= 1 and j > i: empty = False
    return None


files = []
for dp, _, fs in os.walk(os.path.join(GEN, "src")):
    files += [os.path.join(dp, f) for f in fs if f.endswith(".c")]
defs = {}
texts = {}
for f in files:
    t = strip(open(f, errors="replace").read())
    texts[f] = t
    for m in re.finditer(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{)]*(?:\([^)]*\)[^;{)]*)*)\)\s*\{", t, re.M):
        if m.group(1) in ("if", "while", "for", "switch", "return"): continue
        defs.setdefault(m.group(1), (nparams(m.group(2)), f))
bad = collections.defaultdict(list)
for f, t in texts.items():
    for m in re.finditer(r"(?<![\w.>])(\w+)\s*\(", t):
        n = m.group(1)
        if n not in defs or defs[n][0] <= 0: continue
        # skip the definition / declarations themselves
        line_start = t.rfind("\n", 0, m.start()) + 1
        line = t[line_start:t.find("\n", m.start())]
        if re.match(r"\s*(extern\s+|static\s+)?[A-Za-z_][\w\s\*]*\b%s\s*\(" % n, line) and not re.match(r"\s*return\b", line) \
                and (line.rstrip().endswith(";") and "=" not in line.split(n)[0] or line.rstrip().endswith("{") or line.rstrip().endswith(")")):
            if re.match(r"\s*(extern\s+|static\s+)?(void|int|s32|u32|short|s16|u16|char|u8|s8|long|unsigned|signed|struct|TObj|\w+\s*\*)", line):
                continue
        k = args_at(t, m.end() - 1)
        if k is not None and k < defs[n][0]:
            bad[os.path.relpath(f, GEN)].append("%s: %d of %d args" % (n, k, defs[n][0]))
total = sum(len(v) for v in bad.values())
print("%d short calls in %d files" % (total, len(bad)))
for f in sorted(bad):
    print("%s: %s" % (f, "; ".join(sorted(set(bad[f])))))
