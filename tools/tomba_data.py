#!/usr/bin/env python3
"""Maps psx_tomba (NTSC-U) data symbols onto our PAL globals through code that is byte-identical in both builds.
Compiles every psx_tomba source like tools/tomba_map.py, finds each function in the PAL binary (masked bytes) and
pairs the relocated references (lui/addiu/lw/sw) of the US object (symbol + addend) with the address the retail PAL
bytes encode at the same instruction. A name is accepted only when every reference of it resolves to the same PAL
base address, no other name resolves to that address, and the declared size fits the observed accesses.
Output: CSV lines `address,name,size,source` on stdout (size from the US declaration, blank when unknown).
Usage: tools/tomba_data.py [--gcc DIR] [--bin game/MAIN0.EXE] <psx_tomba .c files...>   (env PSX_TOMBA = checkout)"""
import os, re, struct, subprocess, sys, tempfile, collections
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PT = os.environ.get("PSX_TOMBA", os.path.join(os.path.dirname(ROOT), "hansbonini", "psx_tomba"))
args = sys.argv[1:]
def opt(n, d):
    if n in args:
        i = args.index(n); v = args[i + 1]; del args[i:i + 2]; return v
    return d
CALLS = "--calls" in args
if CALLS: args.remove("--calls")
GCC = opt("--gcc", os.environ.get("OLDGCC", "/opt/oldgcc/gcc-2.7.2-psx"))
BIN = opt("--bin", "game/MAIN0.EXE")
files = args
if BIN.upper().endswith(".EXE"):
    RAW = open(os.path.join(ROOT, BIN), "rb").read()
    LOAD = struct.unpack("<I", RAW[0x18:0x1C])[0]; TEXT = RAW[0x800:]
else:
    TEXT = open(os.path.join(ROOT, BIN), "rb").read(); LOAD = 0x800E8028

def build(src, flags, d):
    subprocess.run([GCC + "/cpp", "-undef", "-D__GNUC__=2", "-DMIPSEL", "-I" + PT + "/include", "-I" + PT + "/include/psyq", "-I" + ROOT + "/include/tomba/psyq", "-D_LANGUAGE_C", "-DSKIP_ASM", src, d + "/a.i"], check=True, capture_output=True)
    subprocess.run([GCC + "/cc1", "-quiet", "-w", "-funsigned-char"] + flags.split() + [d + "/a.i", "-o", d + "/a.s"], check=True, capture_output=True)
    mas = subprocess.run([sys.executable, "/opt/maspsx/maspsx.py", "--aspsx-version=2.86", "--expand-div"], input=open(d + "/a.s").read(), check=True, capture_output=True, text=True).stdout
    open(d + "/b.s", "w").write(mas)
    subprocess.run(["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O1", "-G0", d + "/b.s", "-o", d + "/a.o"], check=True, capture_output=True)
    subprocess.run(["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text", d + "/a.o", d + "/t.bin"], check=True)
    rel = []
    for line in subprocess.run(["mipsel-linux-gnu-objdump", "-r", "-j", ".text", d + "/a.o"], capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"([0-9a-f]{8})\s+(R_MIPS_\w+)\s+(\S+)", line)
        if m: rel.append((int(m.group(1), 16), m.group(2), m.group(3)))
    syms = []
    for line in subprocess.run(["mipsel-linux-gnu-nm", "-n", d + "/a.o"], capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"([0-9a-f]{8}) [Tt] (\w+)", line)
        if m: syms.append((int(m.group(1), 16), m.group(2)))
    return open(d + "/t.bin", "rb").read(), rel, syms

def pattern(code, rels, base):
    mask = bytearray(b"\xff" * len(code))
    for off, ty, sym in rels:
        o = off - base
        if o < 0 or o + 4 > len(code): continue
        if ty == "R_MIPS_26": mask[o] = mask[o+1] = mask[o+2] = 0; mask[o+3] = 0xfc
        else: mask[o] = mask[o+1] = 0
    rx = b""
    for p, m in zip(code, mask):
        if m == 0xff: rx += re.escape(bytes([p]))
        elif m == 0: rx += b"."
        else: rx += b"[" + b"".join(re.escape(bytes([(p & m) | k])) for k in range(256) if (k & m) == 0) + b"]"
    return re.compile(rx, re.S)

W = lambda b, o: struct.unpack_from("<I", b, o)[0]
sx = lambda v: v - 0x10000 if v & 0x8000 else v
WID = {0x20: 1, 0x21: 2, 0x22: 4, 0x23: 4, 0x24: 1, 0x25: 2, 0x26: 4, 0x28: 1, 0x29: 2, 0x2a: 4, 0x2b: 4, 0x2e: 4, 0x09: 0}

# declared sizes from the US headers
SZ = {"char": 1, "u8": 1, "s8": 1, "u_char": 1, "byte": 1, "short": 2, "s16": 2, "u16": 2, "u_short": 2, "int": 4, "s32": 4, "u32": 4,
      "long": 4, "u_long": 4, "ulong": 4}
decl = {}
for l in open(PT + "/include/game/data.h"):
    m = re.match(r"extern\s+(?:const\s+)?([\w ]+?)\s*(\*?)\s*(\w+)(\[[^\]]*\])?\s*;", l)
    if not m: continue
    ty, ptr, nm, dim = m.groups()
    ty = ty.strip(); n = 4 if ptr else SZ.get(ty)
    if n is None: decl[nm] = None; continue
    if dim:
        d = dim[1:-1]
        decl[nm] = n * int(d, 0) if d else None
    else: decl[nm] = n

refs = collections.defaultdict(list)   # name -> [(base, width+addend, func, off)]
nfun = 0
for src in files:
    d = tempfile.mkdtemp()
    try: code, rel, syms = build(src, "-O2 -G0", d)
    except subprocess.CalledProcessError as e:
        print("ERROR", src, file=sys.stderr); continue
    merged = {}
    for a, nm in syms:
        if a not in merged or merged[a] in ("gcc2_compiled", "__gnu_compiled_c"): merged[a] = nm
    syms = sorted(merged.items())
    rows = []
    for i, (a, nm) in enumerate(syms):
        end = syms[i+1][0] if i + 1 < len(syms) else len(code)
        fc = code[a:end]
        while len(fc) >= 8 and fc[-4:] == b"\0\0\0\0" and fc[-8:-4] == b"\0\0\0\0": fc = fc[:-4]
        if len(fc) < 8: continue
        hits = [m.start() for m in pattern(fc, rel, a).finditer(TEXT) if m.start() % 4 == 0]
        rows.append((nm, a, fc, hits))
    for nm, a, fc, hits in rows:
        if len(hits) != 1: continue
        h = hits[0]; nfun += 1
        rr = [r for r in rel if a <= r[0] < a + len(fc)]
        for j, (off, ty, sym) in enumerate(rr):
            if CALLS and ty == "R_MIPS_26" and not sym.startswith("."):
                ro = off - a
                tgt = (((W(TEXT, h + ro) & 0x3FFFFFF) << 2) | 0x80000000) - ((W(fc, ro) & 0x3FFFFFF) << 2)
                refs[sym].append((tgt & 0xFFFFFFFF, 0, 0, nm, LOAD + h + ro)); continue
            if CALLS: continue
            if ty != "R_MIPS_LO16" or sym.startswith(".") or re.fullmatch(r"(D|func|FUN|DAT)_[0-9A-Fa-f]{8}", sym) or sym.startswith("_"): continue
            hi = next((r for r in reversed(rr[:j]) if r[1] == "R_MIPS_HI16" and r[2] == sym), None)
            if hi is None: continue
            ro, po = off - a, hi[0] - a
            pal = ((W(TEXT, h + po) & 0xFFFF) << 16) + sx(W(TEXT, h + ro) & 0xFFFF)
            us = ((W(fc, po) & 0xFFFF) << 16) + sx(W(fc, ro) & 0xFFFF)
            w = WID.get(W(TEXT, h + ro) >> 26)
            if w is None: continue
            refs[sym].append(((pal - us) & 0xFFFFFFFF, us & 0xFFFFFFFF, w, nm, LOAD + h + ro))
out, byaddr = {}, collections.defaultdict(list)
for nm, rs in refs.items():
    if CALLS and re.fullmatch(r"(D|func|FUN|DAT)_[0-9A-Fa-f]{8}", nm): continue
    bases = {r[0] for r in rs}
    if len(bases) != 1: print("# inconsistent", nm, sorted("%08x" % b for b in bases), file=sys.stderr); continue
    b = bases.pop(); sz = decl.get(nm)
    ext = max(r[1] + r[2] for r in rs)  # observed extent (addend + width)
    if sz is not None and ext > sz and sz in (1, 2, 4): print("# size", nm, sz, ext, file=sys.stderr); continue
    out[nm] = (b, sz, len(rs)); byaddr[b].append(nm)
for nm, (b, sz, n) in sorted(out.items(), key=lambda x: x[1][0]):
    if len(byaddr[b]) > 1: print("# alias", "%08x" % b, byaddr[b], file=sys.stderr); continue
    print("%08x,%s,%s,psx_tomba (US) %s=%d" % (b, nm, "" if sz is None else "0x%x" % sz, "calls" if CALLS else "refs", n))
print("# functions used: %d" % nfun, file=sys.stderr)
