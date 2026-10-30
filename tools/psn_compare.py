#!/usr/bin/env python3
"""Compares two extracted disc file trees (e.g. PSN PS1 Classic vs retail) file by file.
Usage: python3 tools/psn_compare.py A_dir B_dir [--struct]
Prints identical / changed / added (only in A) / removed (only in B) with size and sha1.
--struct: only compare names and sizes (different regions). Changed PS-X EXEs get a byte-range diff summary."""
import sys, os, hashlib
def tree(root):
    r = {}
    for dp, _, fs in os.walk(root):
        for f in fs:
            p = os.path.join(dp, f); r[os.path.relpath(p, root).upper()] = p
    return r
def sha1(p): return hashlib.sha1(open(p, "rb").read()).hexdigest()
def ranges(a, b, gap=16):
    out = []; n = min(len(a), len(b)); i = 0
    while i < n:
        if a[i] != b[i]:
            s = i; e = i
            while i < n and (a[i] != b[i] or i - e < gap):
                if a[i] != b[i]: e = i
                i += 1
            out.append((s, e + 1))
        else: i += 1
    return out
a, b = tree(sys.argv[1]), tree(sys.argv[2]); st = "--struct" in sys.argv
same = chg = 0
for k in sorted(set(a) | set(b)):
    if k not in b: print(f"ADDED    {k} {os.path.getsize(a[k])} {sha1(a[k])}"); continue
    if k not in a: print(f"REMOVED  {k} {os.path.getsize(b[k])} {sha1(b[k])}"); continue
    sa, sb = os.path.getsize(a[k]), os.path.getsize(b[k])
    if st:
        print(f"{'SAMESIZE' if sa == sb else 'SIZEDIFF'} {k} {sa} {sb}"); continue
    ha, hb = sha1(a[k]), sha1(b[k])
    if ha == hb: same += 1; print(f"SAME     {k} {sa} {ha}"); continue
    chg += 1; print(f"CHANGED  {k} {sa} {ha} <- {sb} {hb}")
    da, db = open(a[k], "rb").read(), open(b[k], "rb").read()
    if da[:8] == b"PS-X EXE" or sa < 4 << 20:
        rs = ranges(da, db); tot = sum(e - s for s, e in rs)
        print(f"    {len(rs)} ranges, {tot} bytes differ" + (f", size {sa - sb:+d}" if sa != sb else ""))
        org = int.from_bytes(da[0x18:0x1c], "little") - 0x800 if da[:8] == b"PS-X EXE" else 0
        for s, e in rs[:40]:
            ram = f" ram {org + s:08x}" if org else ""
            print(f"    {s:08x}-{e:08x} ({e - s}){ram} new={da[s:min(e, s + 16)].hex()} old={db[s:min(e, s + 16)].hex()}")
print(f"# same={same} changed={chg}")
