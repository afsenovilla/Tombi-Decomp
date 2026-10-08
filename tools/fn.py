#!/usr/bin/env python3
"""Shows the assembly (capstone) and Ghidra decompilation of a function.
Usage: tools/fn.py <addr_hex> [MAIN0|X000|X001..X019]"""
import re, struct, sys, os
from capstone import Cs, CS_ARCH_MIPS, CS_MODE_MIPS32, CS_MODE_LITTLE_ENDIAN
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
addr = int(sys.argv[1], 16); prog = sys.argv[2] if len(sys.argv) > 2 else ("X000" if addr >= 0x800E8028 else "MAIN0")
csv = "notes/functions_%s.csv" % prog.lower()
def _size():
    for l in open(os.path.join(R, csv)):
        if l.startswith("%08x," % addr):  # MAIN0/X000: address,name,size,typed; overlays: address,size,name,twin
            c = l.split(","); return int(c[2] if prog in ("MAIN0", "X000") else c[1])
    # not in Ghidra's list: use splat's boundaries (asm/<prog>/nonmatchings/<prog>/*.s)
    d = os.path.join(R, "asm", prog.lower(), "nonmatchings", prog.lower())
    for f in (os.listdir(d) if os.path.isdir(d) else []):
        t = open(os.path.join(d, f)).read(2000)
        if "%08X " % addr in t.split("\n", 3)[2] if t.count("\n") > 2 else False:
            m = re.search(r"nonmatching \S+, 0x([0-9A-Fa-f]+)", t)
            if m: return int(m.group(1), 16)
    return 0x200
size = _size()
if prog == "MAIN0":
    d = open(R + "/game/MAIN0.EXE", "rb").read(); off = 0x800 + addr - struct.unpack("<I", d[0x18:0x1c])[0]
else:  # area overlays: game/AREAnn/X0nn.BIN, all loaded at 0x800E8028
    d = open(R + "/game/AREA%s/%s.BIN" % (prog[2:], prog), "rb").read(); off = addr - 0x800E8028
md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS32 | CS_MODE_LITTLE_ENDIAN)
for i in md.disasm(d[off:off + size], addr): print("%08x  %s %s" % (i.address, i.mnemonic, i.op_str))
if prog not in ("MAIN0", "X000"): sys.exit(0)  # no Ghidra export for the other overlays
t = open(R + "/game/out/%s_decomp.c" % ("MAIN0_EXE" if prog == "MAIN0" else "X000_BIN"), errors="replace").read()
m = re.search(r"// ==== \S+ @ %08x size=\d+\n(.*?)(?=\n// ==== |\Z)" % addr, t, re.S)
print("\n" + (m.group(0) if m else "(no decompilation)"))
