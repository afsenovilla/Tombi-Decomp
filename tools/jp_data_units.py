#!/usr/bin/env python3
"""Detects splat 'functions' of the Japanese retail build that are really data (jump tables, entry tables, area id
word, trailing tables): no jr/jal/sp adjustment in the whole body and at least one word that is a RAM pointer, or an
area ID word at 0x80115178, or a splat dlabel. Writes config/slps01144/data_units.txt (`<prog> <addr> <size> <code bytes after it, 0 = whole unit>`),
which tools/build_report.py --version slps01144 uses to leave those ranges out of the code units (they land in
<prog>/data). Needs asm/slps01144 (build_report). Usage: tools/jp_data_units.py [--write]"""
import os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = ROOT + "/asm/slps01144"
rows = []
c_addrs = set()
for dp, _, fs in os.walk(ROOT + "/src"):
    for f in fs:
        if f.endswith(".c"):
            m = re.search(r"//\s*VERSION\s+SLPS_01144\s+(\w+)\s+([0-9A-Fa-f]+)", open(os.path.join(dp, f), errors="replace").read(400))
            if m: c_addrs.add((m.group(1), int(m.group(2), 16)))
for prog in sorted(os.listdir(R)):
    d = "%s/%s/nonmatchings/%s" % (R, prog, prog)
    if not os.path.isdir(d): continue
    for fn in sorted(os.listdir(d)):
        t = open(d + "/" + fn).read()
        ws = re.findall(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/", t)
        if not ws: continue
        addr = int(ws[0][0], 16)
        w = [int.from_bytes(bytes.fromhex(x[1]), "little") for x in ws]
        isptr = lambda x: 0x80000000 <= x < 0x80200000
        ptr = sum(1 for x in w if isptr(x))
        pro = any((x >> 16) == 0x27BD and x & 0x8000 for x in w[:6])
        jrra = any(x == 0x03E00008 for x in w)
        isid = addr == 0x80115178 and prog != "slps01144" and len(w) == 1
        if (prog, addr) in c_addrs: continue
        if not jrra and not pro and (ptr or isid or t.startswith("dlabel")):
            if prog == "slps01144" and ptr * 2 < len(w): continue
            rows.append((prog, addr, len(w) * 4, 0))
        elif addr == 0x80115178 and prog != "slps01144":  # ID word + jump tables, then code: split at the first non-pointer word
            k = 1
            while k < len(w) and (isptr(w[k]) or w[k] == 0): k += 1
            if k >= 4 and k < len(w): rows.append((prog, addr, k * 4, len(w) * 4 - k * 4))
tot = sum(r[2] for r in rows)
print("%d units, %d bytes" % (len(rows), tot))
if "--write" in sys.argv:
    with open(ROOT + "/config/slps01144/data_units.txt", "w") as o:
        o.write("# splat 'functions' that are data (tools/jp_data_units.py): <prog> <addr> <size>; skipped as code units\n")
        for p, a, n, rest in rows: o.write("%s %08X %d %d\n" % (p, a, n, rest))
    for p, a, n, rest in rows:  # data + code in one splat function: give the code its own function symbol
        if not rest: continue
        sf = "%s/config/slps01144/symbol_addrs_%s.txt" % (ROOT, p)
        line = "func_%08X = 0x%08X; // type:func size:0x%X\n" % (a + n, a + n, rest)
        if line.split(" =")[0] + " =" not in open(sf).read(): open(sf, "a").write(line)
else:
    for p, a, n, rest in rows: print(p, "%08X" % a, n, rest)
