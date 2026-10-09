#!/usr/bin/env python3
"""Ports one psx_tomba (NTSC-U) function to src/<func>.c at its PAL address: keeps only that function's definition
from the psx_tomba file (other definitions become prototypes, `inline` helpers stay as `static inline`, file-level
`__asm__("nop")` padding is dropped), renames the file-local `D_xxxxxxxx` externs to the PAL addresses read from the
retail lui/addiu pairs (game.h globals keep their NTSC names: build_full resolves them from the retail bytes) and
verifies with tools/ncheck.py. With --write a MATCH is saved as src/<func>.c with its `// MATCHING` line.
Usage: tools/tomba_port.py <psx_tomba .c file> <function> <pal_hex> [--write] [--keep]
       tools/tomba_port.py --batch <tomba_map output> ...   (ports every NEW/unique hit not matched yet, --write)
Environment: PSX_TOMBA (psx_tomba checkout), OLDGCC (compiler, default /opt/oldgcc/gcc-2.7.2-psx)."""
import os, re, struct, subprocess, sys, tempfile, shutil
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PT = os.environ.get("PSX_TOMBA", os.path.join(os.path.dirname(ROOT), "hansbonini", "psx_tomba"))
GCC = os.environ.get("OLDGCC", "/opt/oldgcc/gcc-2.7.2-psx")


def batch(mapfiles):
    """Port every function of the tomba_map listings that has one PAL hit (copies are disambiguated by the file's
    median NTSC->PAL shift) and is not matched in src/ yet."""
    base = os.path.join(PT, "src", "scus_942.36")
    files = {}
    for root, _, fs in os.walk(base):
        for f in fs: files.setdefault(f, []).append(os.path.join(root, f))
    rows = []
    for mf in mapfiles:
        for l in open(mf):
            m = re.match(r"(\S+)\s+ntsc=([0-9a-f]+) size=\s*(\d+) pal=(\S+) (\w+)", l)
            if not m: continue
            nm, na, pal = m.group(1), int(m.group(2), 16), m.group(4)
            src = l.strip().split()[-1]
            if nm in ("gcc2_compiled", "__gnu_compiled_c"): continue
            if not na:
                h = re.match(r"(?:func|FUN)_([0-9A-Fa-f]{8})$", nm)
                if h: na = int(h.group(1), 16)
            rows.append([nm, na, [int(x, 16) for x in pal.split(",")] if pal != "-" else [], m.group(5), src])
    by = {}
    for r in rows: by.setdefault(r[4], []).append(r)
    for src, rs in by.items():
        ds = sorted(r[2][0] - r[1] for r in rs if len(r[2]) == 1 and r[1])
        if not ds: continue
        delta = ds[len(ds) // 2]
        for r in rs:
            if len(r[2]) > 1 and r[1]:
                best = min(r[2], key=lambda h: abs(h - r[1] - delta))
                if abs(best - r[1] - delta) < 0x400: r[2] = [best]
    have = set()
    for f in os.listdir(ROOT + "/src"):
        if f.endswith(".c"):
            m = re.search(r"//\s*MATCHING\s+([0-9a-fA-F]+)", open(ROOT + "/src/" + f, errors="replace").read(300))
            if m: have.add(int(m.group(1), 16))
    for nm, na, hits, tag, src in rows:
        if len(hits) != 1:
            print("SKIP", nm, tag, src); continue
        if hits[0] in have: continue
        paths = [os.path.join(base, src)] if os.path.exists(os.path.join(base, src)) else files.get(os.path.basename(src), [])
        out = "??"
        for path in paths:
            r = subprocess.run([sys.executable, os.path.abspath(__file__), path, nm, "%08x" % hits[0], "--write"], capture_output=True, text=True)
            out = (r.stdout + r.stderr).strip()
            if "not found in" not in out: break
        print(out.splitlines()[0] if out else "??", "|", nm, src)


if "--batch" in sys.argv:
    batch([a for a in sys.argv[1:] if a != "--batch"]); sys.exit(0)
src, func, pal = sys.argv[1], sys.argv[2], int(sys.argv[3], 16)
write = "--write" in sys.argv
MAIN0 = open(ROOT + "/game/MAIN0.EXE", "rb").read()
LOAD = struct.unpack("<I", MAIN0[0x18:0x1C])[0]
def game(addr, n): return MAIN0[0x800 + addr - LOAD: 0x800 + addr - LOAD + n]
sizes = {}
for l in open(ROOT + "/notes/functions_main0.csv"):
    c = l.split(",")
    if re.match(r"[0-9a-f]{8}$", c[0]): sizes[int(c[0], 16)] = int(c[2])
text = open(src).read()
rel = os.path.relpath(src, PT + "/src/scus_942.36")

# ---- split top-level function definitions
def strip_comments(t):
    out = []; i = 0; n = len(t)
    while i < n:
        if t.startswith("/*", i):
            j = t.find("*/", i + 2); j = n if j < 0 else j + 2
            out.append(" " * (j - i)); i = j
        elif t.startswith("//", i):
            j = t.find("\n", i); j = n if j < 0 else j
            out.append(" " * (j - i)); i = j
        elif t[i] in "\"'":
            q = t[i]; j = i + 1
            while j < n and t[j] != q:
                if t[j] == "\\": j += 1
                j += 1
            out.append(t[i:j + 1]); i = j + 1
        else:
            out.append(t[i]); i += 1
    return "".join(out)
ct = strip_comments(text)
# blank preprocessor directives (with their continuation lines) so macro bodies never look like code
ct = re.sub(r"^[ \t]*#(?:[^\n\\]|\\\n|\\.)*", lambda m: re.sub(r"[^\n]", " ", m.group(0)), ct, flags=re.M)
KW = {"void", "int", "char", "short", "long", "unsigned", "signed", "return", "if", "while", "for", "switch", "sizeof", "static", "inline"}
def fname(hs):
    """Name of the function a definition head declares: the identifier followed by `(` that is not a type keyword
    and not a `(*ptr` declarator (handles `void (*CdDataCallback(void (*f)()))`)."""
    for m in re.finditer(r"(\(\*\s*)?(\w+)\s*\(", hs):
        if m.group(1) or m.group(2) in KW: continue
        return m.group(2)
    return None
defs = []  # (start, open_brace, end, name, is_static_inline)
depth = 0; last = 0; i = 0; n = len(ct); segs = [0]
while i < n:
    c = ct[i]
    if c == "{":
        if depth == 0:
            m = None
            for k in range(1, min(8, len(segs)) + 1):
                last = segs[-k]
                hs = ct[last:i].strip()
                m = re.search(r"(\w+)\s*\(([^;{}]*)\)\s*((?:[^;{}()]*;\s*)*)$", hs, re.S)
                if m and (k == 1 or m.group(3).strip()): break
                m = None
            if m and not re.match(r"(typedef|struct|union|enum)\b", hs) and "=" not in hs.split("(")[0]:
                # find end
                d = 0; j = i
                while j < n:
                    if ct[j] == "{": d += 1
                    elif ct[j] == "}":
                        d -= 1
                        if d == 0: break
                    j += 1
                start = last
                # skip blank, comment-only (blanked in ct) and preprocessor lines before the signature
                while start < i:
                    eol = ct.find("\n", start); eol = i if eol < 0 or eol > i else eol
                    line = ct[start:eol]
                    if line.strip() == "" or line.lstrip().startswith("#"): start = eol + 1
                    else: break
                start = min(start, i)
                defs.append((start, i, j + 1, fname(hs) or m.group(1), bool(m.group(3).strip()), bool(re.search(r"\binline\b|__inline__", hs))))
                i = j + 1; last = i; segs = [i]; continue
        depth += 1
    elif c == "}": depth -= 1
    elif c == ";" and depth == 0: last = i + 1; segs.append(last)
    i += 1
names = [d[3] for d in defs]
if func not in names: sys.exit("function %s not found in %s (have %s)" % (func, src, names))
out = []; pos = 0
for start, ob, end, name, knr, sinl in defs:
    out.append(text[pos:start])
    if name == func:
        out.append(text[start:end])
    elif sinl:
        out.append(re.sub(r"^(?!static)", "static ", text[start:end], count=1) if not re.match(r"\s*static\b", text[start:end]) else text[start:end])
    else:
        sig = re.sub(r"\s+", " ", text[start:ob].strip())
        if knr: sig = sig[:sig.index("(")] + "()"
        out.append(sig + ";")
    pos = end
out.append(text[pos:])
body = "".join(out)
# drop INCLUDE_ASM / INCLUDE_RODATA lines entirely (SKIP_ASM would also empty them)
body = re.sub(r"^[ \t]*//?[ \t]*INCLUDE_(ASM|RODATA)\([^\n]*\n", "", body, flags=re.M)
body = re.sub(r"^__asm__\s*\(\s*\"nop\"\s*\)\s*;[ \t]*\n", "", body, flags=re.M)  # file-level padding nops of other functions
body = re.sub(r"\n{3,}", "\n\n", body)

def build(txt, d):
    inc = d + "/inc"; os.makedirs(inc)
    for f in os.listdir(ROOT + "/include"):
        s = ROOT + "/include/" + f
        if os.path.isdir(s): shutil.copytree(s, inc + "/" + f)
        else:
            for nm in {f, f.upper(), f.lower()}: shutil.copy(s, inc + "/" + nm)
    open(d + "/a.c", "w").write(txt)
    subprocess.run([GCC + "/cpp", "-undef", "-D__GNUC__=2", "-DMIPSEL", "-I" + inc, "-I" + inc + "/tomba", "-I" + os.path.dirname(src), "-D_LANGUAGE_C", d + "/a.c", d + "/a.i"], check=True, capture_output=True)
    subprocess.run([GCC + "/cc1", "-quiet", "-w", "-funsigned-char", "-O2", "-G0", d + "/a.i", "-o", d + "/a.s"], check=True, capture_output=True)
    mas = subprocess.run([sys.executable, "/opt/maspsx/maspsx.py", "--aspsx-version=2.86", "--expand-div"], input=open(d + "/a.s").read(), check=True, capture_output=True, text=True).stdout
    open(d + "/b.s", "w").write(mas)
    subprocess.run(["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O1", "-G0", d + "/b.s", "-o", d + "/a.o"], check=True, capture_output=True)
    subprocess.run(["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text", d + "/a.o", d + "/t.bin"], check=True)
    rels = []
    for line in subprocess.run(["mipsel-linux-gnu-objdump", "-r", "-j", ".text", d + "/a.o"], capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"([0-9a-f]{8})\s+(R_MIPS_\w+)\s+(\S+)", line)
        if m: rels.append((int(m.group(1), 16), m.group(2), m.group(3)))
    return open(d + "/t.bin", "rb").read(), rels

d = tempfile.mkdtemp()
hdr = "// FUNC %08x %d MAIN0\n// Portado de psx_tomba (%s, %s); licencia MIT del proyecto original.\n#define SKIP_ASM\n"
try:
    code, rels = build(hdr % (pal, 0, rel, func) + body, d)
except subprocess.CalledProcessError as e:
    sys.exit("COMPILE ERROR %s: %s" % (func, (e.stderr or b"")[-600:].decode(errors="replace")))
size = len(code)
while size >= 8 and code[size - 8:size] == b"\0" * 8: size -= 4  # trailing .text padding (a function ends with jr ra; nop at most)
# PAL addresses of relocated symbols from the retail bytes
ref = game(pal, len(code))
W = lambda b, o: struct.unpack("<I", b[o:o + 4])[0]
paladdr = {}; hi = {}
for off, ty, sym in rels:
    if off + 4 > len(ref) or sym.startswith("."): continue
    g = W(ref, off); c = W(code, off)
    if ty == "R_MIPS_26": paladdr[sym] = ((g & 0x3FFFFFF) << 2) | 0x80000000
    elif ty == "R_MIPS_HI16": hi[sym] = (c, g)
    elif ty == "R_MIPS_LO16" and sym in hi:
        hc, hg = hi[sym]
        lo = lambda x: (x & 0xFFFF) - 0x10000 if x & 0x8000 else x & 0xFFFF
        paladdr[sym] = ((((hg & 0xFFFF) << 16) + lo(g)) - lo(c)) & 0xFFFFFFFF  # our lo may carry an offset: subtract ours
        # ours: addend = lo(c) relative to symbol (hi part handled by linker); keep simple: symbol address = game - ours_addend
# rename file-local hex symbols
ren = {}
for sym, a in paladdr.items():
    m = re.match(r"(D|func|FUN)_([0-9A-Fa-f]{8})$", sym)
    if not m or int(m.group(2), 16) == a: continue
    if re.search(r"\bextern\b[^;]*\b%s\b" % re.escape(sym), body):
        if m.group(1) == "D":
            ren[sym] = "D_%08X" % a
        else:
            ren[sym] = sym  # function names keep psx_tomba's name (config/symbol_addrs maps it)
body2 = body
for s, t in ren.items():
    if s != t: body2 = re.sub(r"\b%s\b" % re.escape(s), t, body2)
final = hdr % (pal, size, rel, func) + body2
# write & check with ncheck
stem = func
dst = ROOT + "/src/" + stem + ".c"
os.makedirs(ROOT + "/src/wip", exist_ok=True)
tmp = ROOT + "/src/wip/_port_" + stem + ".c"
if os.path.exists(dst):
    sys.exit("EXISTS %s" % dst)
open(tmp, "w").write(final)
r = subprocess.run([sys.executable, ROOT + "/tools/ncheck.py", tmp], capture_output=True, text=True)
res = r.stdout.strip().splitlines()
ok = any(l.startswith("MATCH") for l in res)
print("%s %s %08x size=%d renames=%s" % ("MATCH" if ok else "DIFF", func, pal, size, {k: v for k, v in ren.items() if k != v}))
if not ok: print("\n".join(res[:12]))
if ok and write:
    t = final.splitlines(); t.insert(1, "// MATCHING %08x %d" % (pal, size))
    open(dst, "w").write("\n".join(t) + "\n"); os.remove(tmp)
    print("WROTE", dst)
elif not write or not ok:
    (os.remove(tmp) if "--keep" not in sys.argv else print("KEPT", tmp))
shutil.rmtree(d, ignore_errors=True)
