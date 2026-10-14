#!/usr/bin/env python3
"""Create a global-variable block struct from the extern declarations found in src/: include/<hdr> + entry in tools/blocks.json.
Usage: mkblock.py <name> <SYM> <hdr> <base-hex> <size-hex> [description]"""
import re, sys, os, json, glob, collections
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
name, sym, hdr, base, size = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4], 16), int(sys.argv[5], 16)
desc = sys.argv[6] if len(sys.argv) > 6 else "global variables"
CAN = {"unsigned char": "unsigned char", "u_char": "unsigned char", "u8": "unsigned char", "char": "unsigned char", "signed char": "unsigned char",
       "short": "short", "s16": "short", "unsigned short": "unsigned short", "u16": "unsigned short", "u_short": "unsigned short",
       "int": "int", "s32": "int", "unsigned int": "unsigned int", "u32": "unsigned int", "unsigned": "unsigned int", "long": "int", "unsigned long": "unsigned int"}
SZ = {"unsigned char": 1, "short": 2, "unsigned short": 2, "int": 4, "unsigned int": 4, "void *": 4}
votes = collections.defaultdict(collections.Counter)
arr = set()
for f in glob.glob(os.path.join(ROOT, "src", "**", "*.c"), recursive=True):
    for ln in open(f, errors="replace"):
        m = re.match(r"^\s*extern\s+([^;(]*);\s*$", ln)
        if not m or "D_" not in m.group(1): continue
        ps = [p.strip() for p in m.group(1).split(",")]
        fm = re.match(r"^(.*?)(\**)\s*(\w+)\s*(\[[^\]]*\])?$", ps[0], re.S)
        if not fm: continue
        bt = fm.group(1).strip()
        items = [(fm.group(2), fm.group(3), fm.group(4))] + [re.match(r"^(\**)\s*(\w+)\s*(\[[^\]]*\])?$", p).groups() for p in ps[1:] if re.match(r"^(\**)\s*(\w+)\s*(\[[^\]]*\])?$", p)]
        for st, nm, ar in items:
            mm = re.fullmatch(r"D_([0-9A-F]{8})", nm)
            if not mm: continue
            a = int(mm.group(1), 16)
            if not base <= a < base + size: continue
            if ar is not None and ar != "[]": arr.add(a)
            if st: votes[a]["void *"] += 1
            elif bt in CAN: votes[a][CAN[bt]] += 1
fields = []; end = 0
for a in sorted(votes):
    t = votes[a].most_common(1)[0][0]
    o = a - base
    if o < end or o + SZ[t] > size or o % SZ[t] or (base + o) % SZ[t]: continue
    fields.append([o, "%s%x" % ({1: "b", 2: "s", 4: "i"}[SZ[t]] if t != "void *" else "p", a & 0xFFFF), t]); end = o + SZ[t]
jp = os.path.join(ROOT, "tools", "blocks.json")
B = json.load(open(jp)) if os.path.exists(jp) else {}
B[name] = dict(sym=sym, hdr=hdr, base=base, size=size, fields=fields)
json.dump(B, open(jp, "w"), indent=1)
L = []; pos = 0
for o, n, t in fields:
    if o > pos: L.append("    unsigned char _pad%x[0x%x];   /* 0x%x */" % (pos, o - pos, pos))
    L.append("    %s %s;   /* 0x%x 0x%08X */" % (t, n, o, base + o)); pos = o + SZ[t]
if size > pos: L.append("    unsigned char _pad%x[0x%x];   /* 0x%x */" % (pos, size - pos, pos))
open(os.path.join(ROOT, "include", hdr), "w").write("""/* %s (0x%08X, size 0x%X): %s. Fields are named by their address (b=u8, s=s16/u16, i=int, p=pointer + low 16 bits of the
   address) until their meaning is known; types are the ones the sources declared. Plain C types, self-contained. */
#ifndef _TOMBA_%s_H
#define _TOMBA_%s_H
typedef struct %sBlock {
%s
} %sBlock;
extern %sBlock %s;
#endif
""" % (sym, base, size, desc, sym, sym, sym, "\n".join(L), sym, sym, sym))
print(name, len(fields), "fields")
