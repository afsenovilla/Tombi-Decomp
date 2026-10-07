#!/usr/bin/env python3
"""Muestra ensamblador (capstone) y decompilado Ghidra de una función.
Uso: tools/fn.py <addr_hex> [MAIN0|X000]"""
import re, struct, sys, os
from capstone import Cs, CS_ARCH_MIPS, CS_MODE_MIPS32, CS_MODE_LITTLE_ENDIAN
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
addr = int(sys.argv[1], 16); prog = sys.argv[2] if len(sys.argv) > 2 else ("X000" if addr >= 0x800E8028 else "MAIN0")
csv = {"MAIN0": "notes/functions_main0.csv", "X000": "notes/functions_x000.csv"}[prog]
size = next(int(l.split(",")[2]) for l in open(os.path.join(R, csv)) if l.startswith("%08x," % addr))
if prog == "MAIN0":
    d = open(R + "/game/MAIN0.EXE", "rb").read(); off = 0x800 + addr - struct.unpack("<I", d[0x18:0x1c])[0]
else:
    d = open(R + "/game/AREA00/X000.BIN", "rb").read(); off = addr - 0x800E8028
md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS32 | CS_MODE_LITTLE_ENDIAN)
for i in md.disasm(d[off:off + size], addr): print("%08x  %s %s" % (i.address, i.mnemonic, i.op_str))
t = open(R + "/game/out/%s_decomp.c" % ("MAIN0_EXE" if prog == "MAIN0" else "X000_BIN"), errors="replace").read()
m = re.search(r"// ==== \S+ @ %08x size=\d+\n(.*?)(?=\n// ==== |\Z)" % addr, t, re.S)
print("\n" + (m.group(0) if m else "(sin decompilado)"))
