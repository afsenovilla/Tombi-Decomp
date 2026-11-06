#!/usr/bin/env python3
"""Set up a decomp-permuter job for one JP (SLPS-01144) function.

  python3 tools/jp_permute_setup.py FILE.c PROG TARGETNAME OUTDIR

Writes OUTDIR/base.c (the function's source preprocessed with VERSION_SLPS_01144, as tools/jpcheck.py compiles it),
OUTDIR/compile.sh (cc1 -> maspsx -> as with the file's `// FLAGS` and the scratchpad `.set` lines of build_report)
and OUTDIR/target.o (build/slps01144/target/PROG/TARGETNAME.o). Then run the permuter
(https://github.com/simonlindholm/decomp-permuter) on OUTDIR:  python3 permuter.py OUTDIR -j1 --stop-on-zero
"""
import os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT + "/tools")


def main():
    src, prog, name, out = sys.argv[1:5]
    import jpcheck, build_report as BR
    NC = jpcheck.load_ncheck()
    os.makedirs(out, exist_ok=True)
    t = open(src).read()
    fm = re.search(r"//\s*FLAGS\s+(.*)", t)
    flags = fm.group(1).strip() if fm else "-O2 -G0"
    inc = tempfile.mkdtemp()
    for f in os.listdir(ROOT + "/include"):
        p = ROOT + "/include/" + f
        if os.path.isdir(p): shutil.copytree(p, inc + "/" + f); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(p, inc + "/" + nm)
    GCC = NC["gcc_for"](src)
    inc2 = NC["headers_for"](src, out, inc)
    subprocess.run([GCC + "/cpp", "-undef", "-D__GNUC__=2", "-DMIPSEL", "-I" + inc2, "-I" + inc2 + "/tomba", "-D_LANGUAGE_C",
                    "-DVERSION_SLPS_01144", "-DSKIP_ASM", "-P", src, out + "/base.c"], check=True)
    open(out + "/extra_as.s", "w").write(BR._jpdemo_spad_set(t, "SLPS_01144"))
    g = re.search(r"-G(\d+)", flags)
    G = g.group(1) if g else "0"
    open(out + "/compile.sh", "w").write("""#!/bin/bash
# compile.sh IN.c -o OUT.o  (decomp-permuter)
set -e
D=$(mktemp -d); trap 'rm -rf $D' EXIT
H=%(here)s
%(gcc)s/cc1 -quiet -w -funsigned-char %(flags)s "$1" -o $D/a.s
python3 %(mas)s --aspsx-version=%(ver)s --expand-div %(G)s < $D/a.s > $D/m.s
cat $H/extra_as.s $D/m.s > $D/b.s
mipsel-linux-gnu-as -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0 $D/b.s -o "$3"
""" % {"here": os.path.abspath(out), "gcc": GCC, "flags": flags, "mas": NC["MASPSX"], "ver": NC["ASPSX_VER"],
       "G": ("-G" + G) if G != "0" else ""})
    os.chmod(out + "/compile.sh", 0o755)
    shutil.copy("%s/build/slps01144/target/%s/%s.o" % (ROOT, prog, name), out + "/target.o")
    open(out + "/settings.toml", "w").write('func_name = "%s"\ncompiler_type = "gcc"\n' % name)
    shutil.rmtree(inc, ignore_errors=True)
    print(out)


if __name__ == "__main__":
    main()
