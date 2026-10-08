#!/usr/bin/env python3
"""Verificador RAPIDO y nativo (sin Wine ni DOSBox): old-gcc `gcc-2.7.2-psx` cc1 + maspsx + GNU as.
Reproduce 607 de las 615 funciones que coinciden con la cadena original (CC1PSX 4.3 + ASPSX 2.86) en
segundos. Úsalo para iterar; la verificación final sigue siendo `tools/matchcheck.py`.

Uso: tools/ncheck.py [--score] [--asm] [--mark] src/X.c [...]
  --score  imprime `SCORE <fichero> <distancia>` (0 = idéntico)
  --asm    deja el .s generado en build/<nombre>.s
  --mark   añade `// MATCHING addr size` si coincide (mejor marca con matchcheck)
Requisitos: tools/setup_native.sh (old-gcc en /opt/oldgcc, maspsx en /opt/maspsx, binutils-mipsel-linux-gnu).
"""
import difflib, os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GCC = os.environ.get("OLDGCC", "/opt/oldgcc/gcc-2.7.2-psx")
MASPSX = os.environ.get("MASPSX", "/opt/maspsx/maspsx.py")
ASPSX_VER = os.environ.get("ASPSX_VER", "2.86")
_src = open(os.path.join(ROOT, "tools", "matchcheck.py")).read().replace("\nmain()\n", "\n")
M = {"__file__": os.path.join(ROOT, "tools", "matchcheck.py")}
exec(compile(_src, "matchcheck", "exec"), M)  # reutiliza header(), game_bytes(), mask()


def build(src, flags, d, inc):
    subprocess.run([GCC + "/cpp", "-undef", "-D__GNUC__=2", "-DMIPSEL", "-I" + inc, "-I" + inc + "/tomba", "-D_LANGUAGE_C", src, d + "/a.i"],
                   check=True, capture_output=True)
    subprocess.run([GCC + "/cc1", "-quiet", "-w", "-funsigned-char"] + flags.split() + [d + "/a.i", "-o", d + "/a.s"],
                   check=True, capture_output=True)
    g = re.search(r"-G(\d+)", flags)
    G = g.group(1) if g else "0"
    mas = subprocess.run([sys.executable, MASPSX, "--aspsx-version=" + ASPSX_VER] + (["-G" + G] if G != "0" else []),
                         input=open(d + "/a.s").read(), check=True, capture_output=True, text=True).stdout
    open(d + "/b.s", "w").write(mas)
    subprocess.run(["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O1", "-G0",
                    d + "/b.s", "-o", d + "/a.o"], check=True, capture_output=True)
    subprocess.run(["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text", d + "/a.o", d + "/t.bin"], check=True)
    rel = []
    for line in subprocess.run(["mipsel-linux-gnu-objdump", "-r", "-j", ".text", d + "/a.o"],
                               capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"([0-9a-f]{8})\s+(R_MIPS_\w+)", line)
        if m:
            rel.append((int(m.group(1), 16), 0x4A if m.group(2) == "R_MIPS_26" else 0x52))
    return open(d + "/t.bin", "rb").read(), rel


def main():
    opts = {a for a in sys.argv[1:] if a.startswith("--")}
    files = [a for a in sys.argv[1:] if not a.startswith("--")]
    inc = tempfile.mkdtemp()
    for f in os.listdir(os.path.join(ROOT, "include")):  # DOS no distingue mayúsculas: copia ambas formas
        src = os.path.join(ROOT, "include", f)
        if os.path.isdir(src):  # p.ej. include/tomba (cabeceras de psx_tomba)
            shutil.copytree(src, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}:
            shutil.copy(src, os.path.join(inc, nm))
    ok = bad = 0
    for f in files:
        name = os.path.basename(f)
        h = M["header"](f)
        if not h:
            print("SKIP (sin // FUNC):", f); continue
        addr, size, prog, flags = h
        d = tempfile.mkdtemp()
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
        if a == b and len(code) == size:
            ok += 1; print("MATCH   %s  %08x %d" % (name, addr, size))
            if "--mark" in opts and "// MATCHING" not in open(f).read():
                t = open(f).read().splitlines(); t.insert(1, "// MATCHING %08x %d" % (addr, size))
                open(f, "w").write("\n".join(t) + "\n")
        else:
            bad += 1; print("DIFF    %s  %08x  (ours %d B, game %d B)" % (name, addr, len(code), size))
            for k in range(0, max(len(a), len(b)), 4):
                if a[k:k + 4] != b[k:k + 4]:
                    print("   +%04x ours=%s game=%s" % (k, a[k:k + 4].hex(), b[k:k + 4].hex()))
    if "--score" not in opts:
        print("%d match, %d fallan" % (ok, bad))
    sys.exit(1 if bad else 0)


main()
