#!/usr/bin/env python3
"""Detects splat 'functions' of the Japanese retail build that are really data (jump tables, entry tables, area id
word, trailing tables): no jr/jal/sp adjustment in the whole body and at least one word that is a RAM pointer, or an
area ID word at 0x80115178, or a splat dlabel. Writes config/slps01144/data_units.txt (`<prog> <addr> <size> <code bytes after it, 0 = whole unit>`),
which tools/build_report.py --version slps01144 uses to leave those ranges out of the code units (they land in
<prog>/data). Needs asm/slps01144 (build_report). Usage: tools/jp_data_units.py [--version slps01144|scus94236] [--write]
(any version of tools/versions.py with the JP layout: the area ID word sits at the overlay base + the shared X00 prefix)."""
import os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from versions import VERSIONS, version_arg
VER = version_arg(sys.argv, "slps01144")
SER = VERSIONS[VER]["serial"]
_x01 = VERSIONS[VER]["progs"].get("x01")  # none in the beta (X00 only): no area ID word to look for
IDW = _x01[2] + _x01[3] if _x01 else -1  # area ID word (JP 0x80115178)
R = ROOT + "/asm/" + VER
rows = []
plain = []  # units with no return and no prologue that the rules above did not take (sandwich rule below)
c_addrs = set()
for dp, _, fs in os.walk(ROOT + "/src"):
    for f in fs:
        if f.endswith(".c"):
            m = re.search(r"//\s*VERSION\s+" + SER + r"\s+(\w+)\s+([0-9A-Fa-f]+)", open(os.path.join(dp, f), errors="replace").read(400))
            if m: c_addrs.add((m.group(1), int(m.group(2), 16)))
for prog in sorted(os.listdir(R)):
    d = "%s/%s/nonmatchings/%s" % (R, prog, prog)
    if not os.path.isdir(d): continue
    for fn in sorted(os.listdir(d)):
        t = open(d + "/" + fn).read()
        ws = re.findall(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/", t)
        if not ws: continue
        # unit start and byte size from every item line (.byte / .short lines carry no 8-digit word): build_report keys
        # the skip on the first item's address, so the row must start where the splat file starts
        items = re.findall(r"/\* [0-9A-F]+ ([0-9A-F]{8})(?: ([0-9A-F]+))? \*/", t)
        addr = int(items[0][0], 16)
        last = items[-1]
        ulen = int(last[0], 16) + (len(last[1]) // 2 if last[1] else 1) - addr
        w = [int.from_bytes(bytes.fromhex(x[1]), "little") for x in ws]
        isptr = lambda x: 0x80000000 <= x < 0x80200000
        ptr = sum(1 for x in w if isptr(x))
        pro = any((x >> 16) == 0x27BD and x & 0x8000 for x in w[:6])
        jrra = any(x == 0x03E00008 for x in w)
        isid = addr == IDW and prog != VER and len(w) == 1
        if (prog, addr) in c_addrs: continue
        if not jrra and not pro: plain.append((prog, addr, ulen))
        if not jrra and not pro and (ptr or isid or t.startswith("dlabel")):
            if prog == VER and ptr * 2 < len(w): continue
            rows.append((prog, addr, ulen, 0))
        elif addr == IDW and prog != VER:  # ID word + jump tables, then code: split at the first non-pointer word
            k = 1
            while k < len(w) and (isptr(w[k]) or w[k] == 0): k += 1
            if k >= 4 and k < len(w): rows.append((prog, addr, k * 4, len(w) * 4 - k * 4))
        elif t.startswith("dlabel") and (isptr(w[0]) or w[0] < 0x100) and ulen == len(w) * 4:
            # pointer / jump tables, then code (the X00 overlay head: entry table + jump tables, then the dispatch
            # stubs): split at the first prologue when nearly everything before it is pointers or small words
            k = next((i for i, x in enumerate(w) if (x >> 16) == 0x27BD and x & 0x8000), None)
            if k and k * 4 >= 64 and sum(1 for x in w[:k] if isptr(x) or x < 0x100) * 10 >= k * 9:
                rows.append((prog, addr, k * 4, len(w) * 4 - k * 4))
# a unit with no return and no prologue wedged between two data units is a table too (x14 801230FC: durations and
# packed coordinates, no pointer in it)
ends = {(p, a + n) for p, a, n, rest in rows if not rest}
starts = {(p, a) for p, a, n, rest in rows}
have = {(p, a) for p, a, n, rest in rows}
for prog, addr, ulen in plain:
    if (prog, addr) not in have and (prog, addr) in ends and (prog, addr + ulen) in starts:
        rows.append((prog, addr, ulen, 0))
rows.sort()
tot = sum(r[2] for r in rows)
print("%d units, %d bytes" % (len(rows), tot))
if "--write" in sys.argv:
    with open(ROOT + "/config/%s/data_units.txt" % VER, "w") as o:
        o.write("# splat 'functions' that are data (tools/jp_data_units.py): <prog> <addr> <size>; skipped as code units\n")
        for p, a, n, rest in rows: o.write("%s %08X %d %d\n" % (p, a, n, rest))
    for p, a, n, rest in rows:  # data + code in one splat function: give the code its own function symbol
        if not rest: continue
        sf = "%s/config/%s/symbol_addrs_%s.txt" % (ROOT, VER, p)
        line = "func_%08X = 0x%08X; // type:func size:0x%X\n" % (a + n, a + n, rest)
        if line.split(" =")[0] + " =" not in open(sf).read(): open(sf, "a").write(line)
else:
    for p, a, n, rest in rows: print(p, "%08X" % a, n, rest)
