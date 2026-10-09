#!/usr/bin/env python3
#!/usr/bin/env python3
"""Maps psx_tomba (NTSC-U) functions onto our PAL MAIN0.EXE by bytes: compiles each psx_tomba source file with the
native pipeline of tools/ncheck.py, masks the relocations of every function and searches the masked bytes in
game/MAIN0.EXE. One line per function: psx_tomba name, NTSC address, size, PAL hit(s) and a tag
(NEW = not matched here yet, HAVE:<file> = already matched, AMBIG = several identical copies, NOTFOUND = the PAL
code differs). Feed the output to `tools/tomba_port.py --batch`.
Usage: tools/tomba_map.py [--gcc /opt/oldgcc/gcc-2.7.2-psx] <psx_tomba .c files...>
Environment: PSX_TOMBA = checkout of https://github.com/hansbonini/psx_tomba (default <parent of this repo>/hansbonini/psx_tomba)."""
import os, re, struct, subprocess, sys, tempfile
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PT = os.environ.get("PSX_TOMBA", os.path.join(os.path.dirname(ROOT), "hansbonini", "psx_tomba"))
args = sys.argv[1:]
GCC = args[args.index("--gcc") + 1] if "--gcc" in args else os.environ.get("OLDGCC", "/opt/oldgcc/gcc-2.7.2-psx")
if "--gcc" in args: del args[args.index("--gcc"):args.index("--gcc") + 2]
files = args
MAIN0 = open(ROOT + "/game/MAIN0.EXE", "rb").read()
LOAD = struct.unpack("<I", MAIN0[0x18:0x1C])[0]
TEXT = MAIN0[0x800:]
# our matched funcs
have = {}
for f in os.listdir(ROOT + "/src"):
    if f.endswith(".c"):
        t = open(ROOT + "/src/" + f, errors="replace").read(400)
        m = re.search(r"//\s*MATCHING\s+([0-9a-fA-F]+)\s+(\d+)", t)
        if m: have[int(m.group(1), 16)] = f
ntsc = {}
for l in open(PT + "/symbols/scus_942.36/symbol_addrs.txt"):
    m = re.match(r"(\w+)\s*=\s*0x([0-9A-Fa-f]+)", l)
    if m: ntsc[m.group(1)] = int(m.group(2), 16)

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
    pat = bytearray(); mask = bytearray()
    for b in code: pat.append(b); mask.append(0xff)
    for off, ty, sym in rels:
        o = off - base
        if o < 0 or o + 4 > len(code): continue
        if ty == "R_MIPS_26":
            mask[o] = 0; mask[o+1] = 0; mask[o+2] = 0; mask[o+3] = 0xfc
        else:
            mask[o] = 0; mask[o+1] = 0
    rx = b""
    for p, m in zip(pat, mask):
        if m == 0xff: rx += re.escape(bytes([p]))
        elif m == 0: rx += b"."
        else:
            rx += b"[" + b"".join(re.escape(bytes([(p & m) | k])) for k in range(0, 256) if (k & m) == 0) + b"]"
    return re.compile(rx, re.S)

for src in files:
    d = tempfile.mkdtemp()
    try:
        code, rel, syms = build(src, "-O2 -G0", d)
    except subprocess.CalledProcessError as e:
        print("ERROR", src, (e.stderr or b"")[-300:].decode(errors="replace")); continue
    merged = {}
    for a, nm in syms:
        if a not in merged or merged[a] == "gcc2_compiled": merged[a] = nm
    syms = sorted(merged.items())
    rows = []
    for i, (a, nm) in enumerate(syms):
        end = syms[i+1][0] if i + 1 < len(syms) else len(code)
        fc = code[a:end]
        while len(fc) >= 8 and fc[-4:] == b"\0\0\0\0" and fc[-8:-4] == b"\0\0\0\0": fc = fc[:-4]  # trailing pad
        if len(fc) < 8: continue
        rx = pattern(fc, rel, a)
        hits = [LOAD + m.start() for m in rx.finditer(TEXT) if (m.start() % 4) == 0]
        hits = [h for h in hits]
        rows.append([nm, ntsc.get(nm, 0), len(fc), hits])
    deltas = [r[3][0] - r[1] for r in rows if len(r[3]) == 1 and r[1]]
    delta = sorted(deltas)[len(deltas)//2] if deltas else None
    for nm, na, sz, hits in rows:
        if len(hits) > 1 and na and delta is not None:
            best = min(hits, key=lambda h: abs(h - na - delta))
            if abs(best - na - delta) < 0x400: hits = [best]
        tag = "HAVE:" + have[hits[0]] if len(hits) == 1 and hits[0] in have else ("AMBIG" if len(hits) > 1 else ("NEW" if hits else "NOTFOUND"))
        print("%-28s ntsc=%08x size=%4d pal=%s %s %s" % (nm, na, sz, ",".join("%08x" % h for h in hits) or "-", tag, os.path.relpath(src, PT + "/src/scus_942.36")))
