#!/usr/bin/env python3
"""Renames MAIN0 functions: for every `address,name` row of the given csv, git-mv the src file whose // FUNC header has that
address (src/<old>.c -> src/<new>.c, also its twins in src/x0nn), replaces the old name (FUN_/func_<addr> or the old
stem) in src/ and include/, and writes the row to notes/names_main0.csv ("name from psx_tomba (US)").
Usage: tools/apply_func_names.py rows.csv"""
import csv, os, re, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
rows = [(int(l.split(",")[0], 16), l.split(",")[1].strip()) for l in open(sys.argv[1]) if l.strip() and not l.startswith("address")]
files = {}   # addr -> [paths] of MAIN0 files
stems = {}
for dp, dn, fn in os.walk(os.path.join(ROOT, "src")):
    for f in fn:
        if not f.endswith(".c"): continue
        stems.setdefault(f[:-2], []).append(os.path.join(dp, f))
        m = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+\d+\s+MAIN0", open(os.path.join(dp, f), errors="replace").read(300))
        if m: files.setdefault(int(m.group(1), 16), []).append(os.path.join(dp, f))
subs = []   # (regex, new)
for a, new in rows:
    if new in stems and not any(p in files.get(a, []) for p in stems[new]):
        print("SKIP name taken", hex(a), new); continue
    pats = [r"(?:FUN|func)_(?i:%08x)" % a]
    for p in files.get(a, []):
        stem = os.path.basename(p)[:-2]
        if stem != new:
            subprocess.run(["git", "mv", p, os.path.join(os.path.dirname(p), new + ".c")], check=True, cwd=ROOT)
            if not re.fullmatch(r"(?i:(?:FUN|func)_%08x)" % a, stem): pats.append(re.escape(stem))
    subs.append((re.compile(r"\b(?:%s)\b" % "|".join(pats)), new, a))
n = 0
for base in ("src", "include"):
    for dp, dn, fn in os.walk(os.path.join(ROOT, base)):
        for f in fn:
            if not f.endswith((".c", ".h")): continue
            p = os.path.join(dp, f); t = open(p, errors="surrogateescape").read(); o = t
            for rx, new, a in subs: t = rx.sub(new, t)
            if t != o: open(p, "w", errors="surrogateescape").write(t); n += 1
print("files edited:", n)
# names csv
p = os.path.join(ROOT, "notes", "names_main0.csv")
lines = open(p).read().splitlines(); hdr, body = lines[0], lines[1:]
d = {}
order = []
for l in body:
    a = int(l.split(",")[0], 16)
    if a not in d: order.append(a)
    d[a] = l
for a, new in rows:
    if a not in d: order.append(a)
    d[a] = "%08x,%s,alta,name from psx_tomba (US)" % (a, new)
open(p, "w").write("\n".join([hdr] + [d[a] for a in sorted(order)]) + "\n")
