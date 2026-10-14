#!/usr/bin/env python3
"""Greedy per-token conversion: for each file, try converting the tokens of a conv module one at a time (as cast form,
else direct-only form) and keep each only if the linked object stays identical.
Usage: greedy_conv.py <module> files...   (module in tools/: conv_player ...)"""
import sys, os, importlib, subprocess, tempfile
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools")); os.chdir(ROOT)
import objcmp
from multiprocessing import Pool
mod = importlib.import_module(sys.argv[1])

def work(a):
    f, inc, tmp = a
    if not objcmp.NC: objcmp.init()
    orig = subprocess.run(["git", "show", "HEAD:" + f], capture_output=True, cwd=ROOT).stdout
    keep = {}
    for t in mod.tokens(f):
        for direct in (False, True):
            open(f, "wb").write(orig)
            mod.ONLY = dict(keep); mod.ONLY[t] = direct
            if not mod.conv(f): continue
            r = objcmp.job((f, inc, tmp))
            if r is None:
                keep[t] = direct; break
    open(f, "wb").write(orig)
    if keep:
        mod.ONLY = keep; mod.conv(f)
        if objcmp.job((f, inc, tmp)) is not None:
            open(f, "wb").write(orig); keep = {}
    return f, keep

if __name__ == "__main__":
    files = sys.argv[2:]
    tmp = tempfile.mkdtemp(); inc = os.path.join(tmp, "inc"); objcmp.mkinc(inc, None)
    with Pool(8) as p: res = p.map(work, [(f, inc, tmp) for f in files])
    n = sum(1 for f, k in res if k)
    print("files converted: %d of %d" % (n, len(files)))
