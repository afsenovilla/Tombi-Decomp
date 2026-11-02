#!/usr/bin/env python3
"""Single-function checker for the Japanese retail build (also reached as `tools/ncheck.py --version slps01144 FILE`).
Compiles src/.../func_<addr>_slps01144.c (its `// VERSION SLPS_01144 <prog> <addr> <size>` line) with the VERSION_SLPS_01144
define and the native gcc-2.7.2 + maspsx chain, then diffs the disassembly against the splat target object
build/slps01144/target/<prog>/<func>.o (made by tools/build_report.py --version slps01144).
Usage: tools/jpcheck.py [--norm] [--strict] FILE.c [prog] [target name]   prints IDENTICAL or a unified diff (ours vs retail).
  --norm    ignores relocation addends and HI/LO/PC16 relocation lines, like objdiff.
  --strict  keeps the branch targets (dropped by default): the check to run before committing a hand edit.
  The target object is build/slps01144/target/<prog>/<name>.o, <name> = the file name or the JP symbol given as 3rd argument."""
import difflib, os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def load_ncheck():
    s = open(ROOT + "/tools/ncheck.py").read().replace("\nmain()\n", "\n")
    NC = {"__file__": ROOT + "/tools/ncheck.py", "__name__": "ncheck_mod"}
    exec(compile(s, "ncheck", "exec"), NC)
    return NC


STRICT = False  # keep branch targets (a wrong branch destination otherwise passes as IDENTICAL)


def dump(o, norm):
    out = subprocess.run(["mipsel-linux-gnu-objdump", "-dr", o], capture_output=True, text=True).stdout
    L = []
    for l in out.splitlines()[3:]:
        l = re.sub(r"^ *[0-9a-f]+:\t[0-9a-f ]+\t", "", l)
        l = re.sub(r"^\s*[0-9a-f]+: (R_MIPS_\w+)\t", r"   \1 ", l)
        if norm: l = re.sub(r"^(\s+R_MIPS_\w+) .*", r"\1", l)
        l = re.sub(r" <.*", "", l)
        if not STRICT: l = re.sub(r"^(j|b\w*)\t(.*?)\b[0-9a-f]+$", r"\1\t\2", l)
        if norm:
            if re.match(r"\s+R_MIPS_PC16", l) or (re.match(r"\s+R_MIPS_26", l) and L and L[-1].startswith("j\t")):
                # branch/jump to a label of the target asm (relocated): its printed target means nothing
                if STRICT and L: L[-1] = re.sub(r"^(j|b\w*)\t(.*?)\b[0-9a-f]+$", r"\1\t\2RELOC", L[-1])
                if re.match(r"\s+R_MIPS_PC16", l): continue
            l = re.sub(r"^(s[bhw]\t\w+,)\d+\(at\)", r"\1X(at)", l)
        if l.strip() and not l.startswith("Disassembly") and not l.startswith("0000"): L.append(l)
    if STRICT and not norm:  # a branch to a jump-table label of the target asm carries a PC16 reloc: its printed target means nothing
        for i, l in enumerate(L):
            if i + 1 < len(L) and L[i + 1].startswith("   R_MIPS_PC16"): L[i] = re.sub(r"^(j|b\w*)\t(.*?)\b[0-9a-f]+$", r"\1\t\2RELOC", l)
    if norm:
        # objdiff ignores relocation addends: an immediate carried by a HI16/LO16 reloc does not count
        out = []
        for i, l in enumerate(L):
            if not l.startswith("   R_MIPS") and i + 1 < len(L) and re.match(r"\s+R_MIPS_(HI|LO)16", L[i + 1]) is not None:
                l = re.sub(r"(\w+\t\$?\w+,(?:\$?\w+,)?)-?\d+(\(\w+\))?$", lambda m: m.group(1) + "X" + (m.group(2) or ""), l)
                l = re.sub(r"^lui\t(\w+),0x[0-9a-f]+$", r"lui\t\1,X", l)
            out.append(l)
        L = [l for l in out if re.match(r"\s+R_MIPS_(HI|LO)16", l) is None]
    return L


def check(src, prog=None, norm=False, target=None):
    NC = load_ncheck()
    NC["EXTRA_CPP"] = ["-DVERSION_SLPS_01144", "-DSKIP_ASM"]
    t = open(src).read()
    if prog is None:
        m = re.search(r"//\s*VERSION\s+SLPS_01144\s+(\w+)", t)
        prog = m.group(1) if m else "slps01144"
    inc = tempfile.mkdtemp(); d = tempfile.mkdtemp()
    try:
        for f in os.listdir(ROOT + "/include"):
            p = ROOT + "/include/" + f
            if os.path.isdir(p): shutil.copytree(p, inc + "/" + f); continue
            for nm in {f, f.upper(), f.lower()}: shutil.copy(p, inc + "/" + nm)
        # PAL scratchpad names (config/slpm80165/spad.txt) and D_1F80xxxx as constants, like build_report does
        import build_report as BR
        NC["EXTRA_AS"] = BR._jpdemo_spad_set(t, "SLPS_01144")
        fm = re.search(r"//\s*FLAGS\s+(.*)", t)
        NC["build"](src, fm.group(1).strip() if fm else "-O2 -G0", d, inc)
        name = target or os.path.basename(src)[:-2]
        tdir = "%s/build/slps01144/target/%s" % (ROOT, prog)
        tn = name if os.path.exists("%s/%s.o" % (tdir, name)) else name.replace("_slps01144", "")
        if not os.path.exists("%s/%s.o" % (tdir, tn)):
            return "NO TARGET %s/%s.o (run build_report --version slps01144)" % (tdir, tn)
        a, b = dump(d + "/a.o", norm), dump("%s/%s.o" % (tdir, tn), norm)
        if STRICT:  # the other side of a relocated branch keeps its printed target: drop it too
            for i in range(min(len(a), len(b))):
                if a[i].endswith("RELOC") or b[i].endswith("RELOC"):
                    a[i] = re.sub(r"^(j|b\w*)\t(.*?)\b(?:[0-9a-f]+|RELOC)$", r"\1\t\2", a[i])
                    b[i] = re.sub(r"^(j|b\w*)\t(.*?)\b(?:[0-9a-f]+|RELOC)$", r"\1\t\2", b[i])
        return "\n".join(difflib.unified_diff(a, b, "ours", "retail", lineterm="", n=2)) or "IDENTICAL"
    finally:
        shutil.rmtree(inc, ignore_errors=True); shutil.rmtree(d, ignore_errors=True)


def main(argv):
    global STRICT
    norm = "--norm" in argv
    STRICT = "--strict" in argv
    args = [a for a in argv if not a.startswith("--")]
    if not args: sys.exit(__doc__)
    r = check(args[0], args[1] if len(args) > 1 else None, norm, args[2] if len(args) > 2 else None)
    print(r)
    sys.exit(0 if r == "IDENTICAL" else 1)


if __name__ == "__main__":
    main(sys.argv[1:])
