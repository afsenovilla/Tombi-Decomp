#!/usr/bin/env python3
"""Renames area overlay functions (X001..X019), one overlay at a time.
Input csv rows: `ovl,address,name` (ovl = 1..19).  For each row: git-mv src/x0nn/<old>.c -> <new>.c when the file is named
after the function, replaces FUN_/func_<addr> by the new name in src/x0nn/*.c only (overlay-scoped: the same address in another
overlay is another function), sets the name in notes/functions_x0nn.csv and renames the tools/labels/labels.json
`animowner:aNN_<old>` keys of the areas that use the overlay.
Refuses a name already used by another function of the same overlay.
Usage: tools/apply_ovl_names.py rows.csv"""
import csv, glob, os, re, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AREAS = {1: [1, 7], 4: [4, 12]}

rows = []
for l in open(sys.argv[1]):
    l = l.split("#")[0].strip()
    if not l or l.startswith("ovl"): continue
    o, a, n = [x.strip() for x in l.split(",")[:3]]
    rows.append((int(o), int(a, 16), n))

labels_p = os.path.join(ROOT, "tools", "labels", "labels.json")
labels = open(labels_p).read()
labels_changed = False
by_ovl = {}
for o, a, n in rows: by_ovl.setdefault(o, []).append((a, n))
for o, lst in sorted(by_ovl.items()):
    d = os.path.join(ROOT, "src", "x%03d" % o)
    csvp = os.path.join(ROOT, "notes", "functions_x%03d.csv" % o)
    lines = open(csvp).read().splitlines()
    names = {l.split(",")[2]: int(l.split(",")[0], 16) for l in lines[1:]}
    stems = {}
    hdr = {}
    for f in glob.glob(os.path.join(d, "*.c")):
        stems[os.path.basename(f)[:-2]] = f
        m = re.match(r"//\s*FUNC\s+([0-9a-fA-F]+)", open(f, errors="replace").read(64))
        if m: hdr[int(m.group(1), 16)] = f
    subs = []
    for a, n in lst:
        if (n in names and names[n] != a) or (n in stems and hdr.get(a) != stems[n]):
            print("SKIP name taken x%03d %08x %s" % (o, a, n)); continue
        old = None
        for i, l in enumerate(lines):
            f = l.split(",")
            if i and int(f[0], 16) == a:
                old = f[2]; f[2] = n; lines[i] = ",".join(f)
        if old is None:
            print("SKIP no csv row x%03d %08x" % (o, a)); continue
        p = hdr.get(a)
        if p and re.fullmatch(r"(?i:(?:FUN|func)_%08x)" % a, os.path.basename(p)[:-2]):
            subprocess.run(["git", "mv", p, os.path.join(d, n + ".c")], check=True, cwd=ROOT)
        subs.append((re.compile(r"\b(?:FUN|func)_(?i:%08x)\b" % a), n))
        for ar in AREAS.get(o, [o]):
            ko, kn = "animowner:a%02d_%s" % (ar, old.lower()), "animowner:a%02d_%s" % (ar, n.lower())
            if '"%s"' % ko in labels:
                labels = labels.replace('"%s"' % ko, '"%s"' % kn); labels_changed = True
    open(csvp, "w").write("\n".join(lines) + "\n")
    for f in glob.glob(os.path.join(d, "*.c")):
        t = open(f, errors="surrogateescape").read(); u = t
        for rx, n in subs: u = rx.sub(n, u)
        if u != t: open(f, "w", errors="surrogateescape").write(u)
if labels_changed:
    open(labels_p, "w").write(labels)
print("done", len(rows))
