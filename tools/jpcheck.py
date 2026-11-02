#!/usr/bin/env python3
"""Single-function checker for the Japanese retail build (also reached as `tools/ncheck.py --version slps01144 FILE`).
Compiles src/.../func_<addr>_slps01144.c (its `// VERSION SLPS_01144 <prog> <addr> <size>` line) with the VERSION_SLPS_01144
define and the native gcc-2.7.2 + maspsx chain, then diffs the disassembly against the splat target object
build/slps01144/target/<prog>/<func>.o (made by tools/build_report.py --version slps01144).
Usage: tools/jpcheck.py [--norm] FILE.c [prog]     prints IDENTICAL or a unified diff (ours vs retail).
  --norm  ignores HI/LO relocation lines and the scratchpad `(at)` offsets (for scratchpad/global accesses)."""
import difflib, os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def load_ncheck():
    s = open(ROOT + "/tools/ncheck.py").read().replace("\nmain()\n", "\n")
    NC = {"__file__": ROOT + "/tools/ncheck.py", "__name__": "ncheck_mod"}
    exec(compile(s, "ncheck", "exec"), NC)
    return NC


def dump(o, norm):
    out = subprocess.run(["mipsel-linux-gnu-objdump", "-dr", o], capture_output=True, text=True).stdout
    L = []
    for l in out.splitlines()[3:]:
        l = re.sub(r"^ *[0-9a-f]+:\t[0-9a-f ]+\t", "", l)
        l = re.sub(r"^\s*[0-9a-f]+: (R_MIPS_\w+)\t", r"   \1 ", l)
        if norm: l = re.sub(r"^(\s+R_MIPS_\w+) .*", r"\1", l)
        l = re.sub(r" <.*", "", l)
        l = re.sub(r"^(j|b\w*)\t(.*?)\b[0-9a-f]+$", r"\1\t\2", l)
        if norm:
            if re.match(r"\s+R_MIPS_PC16", l): continue  # branch to a jump-table label of the target asm
            l = re.sub(r"^(s[bhw]\t\w+,)\d+\(at\)", r"\1X(at)", l)
        if l.strip() and not l.startswith("Disassembly") and not l.startswith("0000"): L.append(l)
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


def check(src, prog=None, norm=False):
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
        name = os.path.basename(src)[:-2]
        tdir = "%s/build/slps01144/target/%s" % (ROOT, prog)
        tn = name if os.path.exists("%s/%s.o" % (tdir, name)) else name.replace("_slps01144", "")
        if not os.path.exists("%s/%s.o" % (tdir, tn)):
            return "NO TARGET %s/%s.o (run build_report --version slps01144)" % (tdir, tn)
        a, b = dump(d + "/a.o", norm), dump("%s/%s.o" % (tdir, tn), norm)
        return "\n".join(difflib.unified_diff(a, b, "ours", "retail", lineterm="", n=2)) or "IDENTICAL"
    finally:
        shutil.rmtree(inc, ignore_errors=True); shutil.rmtree(d, ignore_errors=True)


def main(argv):
    norm = "--norm" in argv
    args = [a for a in argv if not a.startswith("--")]
    if not args: sys.exit(__doc__)
    r = check(args[0], args[1] if len(args) > 1 else None, norm)
    print(r)
    sys.exit(0 if r == "IDENTICAL" else 1)


if __name__ == "__main__":
    main(sys.argv[1:])
