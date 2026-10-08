#!/usr/bin/env python3
"""FAST native checker (no Wine or DOSBox): old-gcc `gcc-2.7.2-psx` cc1 + maspsx + GNU as.
Reproduces 628 of the 636 functions that match with the original toolchain (CC1PSX 4.3 + ASPSX 2.86) in
seconds. Use it to iterate; the final check is still `tools/matchcheck.py`.

Usage: tools/ncheck.py [--score] [--asm] [--mark] src/X.c [...]
  --score  prints `SCORE <file> <distance>` (0 = identical)
  --asm    leaves the generated .s in build/<name>.s
  --mark   adds `// MATCHING addr size` if it matches (better to mark with matchcheck)
Requirements: tools/setup_native.sh (old-gcc in /opt/oldgcc, maspsx in /opt/maspsx, binutils-mipsel-linux-gnu).
"""
import difflib, os, re, shutil, struct, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GCC = os.environ.get("OLDGCC", "/opt/oldgcc/gcc-2.7.2-psx")
MASPSX = os.environ.get("MASPSX", "/opt/maspsx/maspsx.py")
ASPSX_VER = os.environ.get("ASPSX_VER", "2.86")
_src = open(os.path.join(ROOT, "tools", "matchcheck.py")).read().replace("\nmain()\n", "\n")
M = {"__file__": os.path.join(ROOT, "tools", "matchcheck.py")}
exec(compile(_src, "matchcheck", "exec"), M)  # reuses header(), game_bytes(), mask()


def build(src, flags, d, inc):
    subprocess.run([GCC + "/cpp", "-undef", "-D__GNUC__=2", "-DMIPSEL", "-I" + inc, "-I" + inc + "/tomba", "-D_LANGUAGE_C", src, d + "/a.i"],
                   check=True, capture_output=True)
    subprocess.run([GCC + "/cc1", "-quiet", "-w", "-funsigned-char"] + flags.split() + [d + "/a.i", "-o", d + "/a.s"],
                   check=True, capture_output=True)
    g = re.search(r"-G(\d+)", flags)
    G = g.group(1) if g else "0"
    mas = subprocess.run([sys.executable, MASPSX, "--aspsx-version=" + ASPSX_VER, "--expand-div"] + (["-G" + G] if G != "0" else []),
                         input=open(d + "/a.s").read(), check=True, capture_output=True, text=True).stdout
    open(d + "/b.s", "w").write(mas)
    subprocess.run(["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O1", "-G0",
                    d + "/b.s", "-o", d + "/a.o"], check=True, capture_output=True)
    subprocess.run(["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text", d + "/a.o", d + "/t.bin"], check=True)
    rel = []
    for line in subprocess.run(["mipsel-linux-gnu-objdump", "-r", "-j", ".text", d + "/a.o"],
                               capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"([0-9a-f]{8})\s+(R_MIPS_\w+)\s+(\S+)", line)
        if m:
            rel.append((int(m.group(1), 16), 0x4A if m.group(2) == "R_MIPS_26" else 0x52))
            if m.group(2) == "R_MIPS_26" and m.group(3) == ".text":
                LOCAL_J.append(int(m.group(1), 16))
            RELS.append((int(m.group(1), 16), m.group(2), m.group(3)))
    return open(d + "/t.bin", "rb").read(), rel


RELS = []     # (offset, type, symbol) of every .text relocation
_SYMS = {}
_NAMES = {}


def sym_addr(name):
    """Address of a symbol: config/symbol_addrs_*.txt and notes/names_*.csv, else the hex in its name
    (D_8009C984, DAT_8009c984A, PTR_DAT_8013a1d4_b, ...). None if unknown."""
    if not _SYMS:
        for f in ("main0", "x000"):
            for l in open(os.path.join(ROOT, "config", "symbol_addrs_%s.txt" % f)):
                m = re.match(r"\s*(\w+)\s*=\s*0x([0-9a-fA-F]+)", l)
                if m: _SYMS[m.group(1)] = int(m.group(2), 16)
            for l in open(os.path.join(ROOT, "notes", "names_%s.csv" % f)):
                m = re.match(r"([0-9a-fA-F]{8}),(\w+)", l)
                if m:
                    a = int(m.group(1), 16)
                    if _NAMES.setdefault(m.group(2), a) != a: _NAMES[m.group(2)] = None  # ambiguous
        for k, a in _NAMES.items(): _SYMS.setdefault(k, a)
    if name in _SYMS: return _SYMS[name]
    m = re.match(r"(?:PTR_)?(?:D|DAT|FUN|func|LAB|PTR)_([0-9a-fA-F]{8})", name)
    return int(m.group(1), 16) if m else None


def bad_symbols(code, ref, addr, text=""):
    """Relocated fields whose resolved address differs from the game's (masked by the byte compare)."""
    W = lambda b, o: struct.unpack("<I", b[o:o + 4])[0]
    out, hi = [], {}
    for off, ty, sym in RELS:
        if off + 4 > min(len(code), len(ref)) or sym.startswith("."): continue
        S = sym_addr(sym)
        if S is None: continue
        if sym not in _SYMS and not re.search(r"extern[^;(]*\b%s\b" % re.escape(sym), text):
            continue  # hex-named global declared in a shared header (psx_tomba uses NTSC addresses)
        c, g = W(code, off), W(ref, off)
        if ty == "R_MIPS_26":
            if S + ((c & 0x3FFFFFF) << 2) != (((g & 0x3FFFFFF) << 2) | (addr & 0xF0000000)): out.append((off, sym))
        elif ty == "R_MIPS_HI16":
            hi[sym] = (off, c, g)
        elif ty == "R_MIPS_LO16" and sym in hi:
            ho, hc, hg = hi[sym]
            lo = lambda x: (x & 0xFFFF) - 0x10000 if x & 0x8000 else x & 0xFFFF
            ours = (S + ((hc & 0xFFFF) << 16) + lo(c)) & 0xFFFFFFFF
            game = (((hg & 0xFFFF) << 16) + lo(g)) & 0xFFFFFFFF
            if ours != game: out.append((off, sym))
    return out


LOCAL_J = []  # offsets of `j` to labels inside the function: masked by the relocation, so checked separately


def bad_jumps(code, ref, addr):
    """Local `j` whose target (relative to the function start) differs from the game's."""
    out = []
    for off in LOCAL_J:
        if off + 4 > min(len(code), len(ref)): continue
        a = (struct.unpack("<I", code[off:off + 4])[0] & 0x3FFFFFF) << 2
        b = (((struct.unpack("<I", ref[off:off + 4])[0] & 0x3FFFFFF) << 2) | (addr & 0xF0000000)) - addr
        if a != b: out.append(off)
    return out


def main():
    opts = {a for a in sys.argv[1:] if a.startswith("--")}
    files = [a for a in sys.argv[1:] if not a.startswith("--")]
    inc = tempfile.mkdtemp()
    for f in os.listdir(os.path.join(ROOT, "include")):  # DOS is case-insensitive: copy both forms
        src = os.path.join(ROOT, "include", f)
        if os.path.isdir(src):  # e.g. include/tomba (psx_tomba headers)
            shutil.copytree(src, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}:
            shutil.copy(src, os.path.join(inc, nm))
    ok = bad = 0
    for f in files:
        name = os.path.basename(f)
        h = M["header"](f)
        if not h:
            print("SKIP (no // FUNC):", f); continue
        addr, size, prog, flags = h
        d = tempfile.mkdtemp()
        del LOCAL_J[:]; del RELS[:]
        try:
            code, rel = build(f, flags, d, inc)
            if "--asm" in opts:
                os.makedirs(os.path.join(ROOT, "build"), exist_ok=True)
                shutil.copy(d + "/a.s", os.path.join(ROOT, "build", name[:-2] + ".s"))
        except subprocess.CalledProcessError as e:
            err = (e.stderr or b"").decode(errors="replace").strip().splitlines()[:3]
            print("SCORE %s 9999" % name if "--score" in opts else "ERROR %s: %s" % (name, " | ".join(err)))
            bad += 1; shutil.rmtree(d, ignore_errors=True); continue
        shutil.rmtree(d, ignore_errors=True)
        ref = M["game_bytes"](prog, addr, size)
        a, b = M["mask"](code, rel), M["mask"](ref, rel)
        if "--score" in opts:
            A = [a[k:k + 4] for k in range(0, len(a), 4)]; B = [b[k:k + 4] for k in range(0, len(b), 4)]
            same = sum(x.size for x in difflib.SequenceMatcher(None, A, B, autojunk=False).get_matching_blocks())
            print("SCORE %s %d" % (name, (len(A) - same) + (len(B) - same)))
            continue
        bj = bad_jumps(code, ref, addr)
        bs = bad_symbols(code, ref, addr, open(f, errors="replace").read()) if a == b else []
        if a == b and len(code) == size and not bj and not bs:
            ok += 1; print("MATCH   %s  %08x %d" % (name, addr, size))
            if "--mark" in opts and "// MATCHING" not in open(f).read():
                t = open(f).read().splitlines(); t.insert(1, "// MATCHING %08x %d" % (addr, size))
                open(f, "w").write("\n".join(t) + "\n")
        else:
            bad += 1; print("DIFF    %s  %08x  (ours %d B, game %d B)" % (name, addr, len(code), size))
            for k in bj: print("   +%04x local j target differs" % k)
            for k, sy in bs: print("   +%04x address of %s differs" % (k, sy))
            for k in range(0, max(len(a), len(b)), 4):
                if a[k:k + 4] != b[k:k + 4]:
                    print("   +%04x ours=%s game=%s" % (k, a[k:k + 4].hex(), b[k:k + 4].hex()))
    shutil.rmtree(inc, ignore_errors=True)
    if "--score" not in opts:
        print("%d match, %d failed" % (ok, bad))
    sys.exit(1 if bad else 0)


main()
