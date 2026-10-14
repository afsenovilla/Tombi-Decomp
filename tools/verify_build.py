#!/usr/bin/env python3
"""Run the full build and compare linked counts + 'kept as asm' list with build/baseline_*.
Usage: verify_build.py [--save]   (--save writes the baseline from this build)"""
import re, subprocess, sys, os
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
out = subprocess.run([sys.executable, os.path.join(ROOT, "tools/build_full.py"), "-j", "8"],
                     capture_output=True, text=True, cwd=ROOT)
log = out.stdout + out.stderr
asm = sorted(re.sub(r"tmp[a-z0-9_]{8}", "tmpX", l) for l in log.splitlines() if "kept as asm" in l and not l.startswith(("MAIN0", "X000")))
counts = sorted(l for l in log.splitlines() if l.startswith(("MAIN0.EXE", "X000.BIN")))
cur = "\n".join(counts + asm) + "\n"
p = os.path.join(ROOT, "build", "baseline_verify.txt")
if "--save" in sys.argv:
    open(p, "w").write(cur); print("saved"); sys.exit(0)
base = open(p).read()
if base == cur and all(" OK " in c for c in counts) and len(counts) == 2:
    print("VERIFY OK"); print("\n".join(counts)); sys.exit(0)
print("VERIFY FAILED"); print(cur[:3000]); sys.exit(1)
