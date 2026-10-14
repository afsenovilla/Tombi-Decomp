#!/usr/bin/env python3
"""Per-file object-code check: compile each given src file (default: files changed vs HEAD) from HEAD and from the working
tree and compare the objects (disassembly+relocs+data). Prints DIFF <file> for any that differ or fail. Usage: objcmp.py [files]"""
import os, subprocess, sys, shutil, tempfile
from multiprocessing import Pool
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
NC = {}
def init():
    src = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
    NC.update({"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"})
    exec(compile(src, "ncheck", "exec"), NC)
def mkinc(dst, rev):
    shutil.rmtree(dst, ignore_errors=True); os.makedirs(dst)
    for f in os.listdir(os.path.join(ROOT, "include")):
        s = os.path.join(ROOT, "include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(dst, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(dst, nm))
def comp(path, flags, inc, d):
    os.makedirs(d, exist_ok=True)
    NC["build"](path, flags, d, inc)
    import re
    nm = subprocess.run(["mipsel-linux-gnu-nm", d + "/a.o"], capture_output=True, text=True).stdout
    defs = []
    for l in nm.split("\n"):
        w = l.split()
        if len(w) == 2 and w[0] == "U":
            m = re.fullmatch(r"D_([0-9A-Fa-f]{8})", w[1])
            if m: defs.append("--defsym=%s=0x%s" % (w[1], m.group(1)))
            elif w[1] in ("PLAYER", "GVARS"): defs.append("--defsym=%s=0x%08X" % (w[1], {"PLAYER": 0x800A6038, "GVARS": 0x8009C930}[w[1]]))
            else:
                import zlib
                defs.append("--defsym=%s=0x%08x" % (w[1], 0x80300000 + (zlib.crc32(w[1].encode()) & 0xFFFFF) * 4))
    r = subprocess.run(["mipsel-linux-gnu-ld", "-EL", "-Ttext=0x80100000", "-Tdata=0x80200000", "-e0", "-o", d + "/a.elf", d + "/a.o"] + defs,
                       capture_output=True, text=True)
    if r.returncode: raise Exception(r.stderr)
    o = subprocess.run(["mipsel-linux-gnu-objdump", "-ds", d + "/a.elf"], capture_output=True, text=True).stdout
    return "\n".join(l for l in o.splitlines() if "file format" not in l)
def job(a):
    f, inc, tmp = a
    if not NC: init()
    path = os.path.join(ROOT, f)
    h = NC["M"]["header"](path)
    if not h: return None
    flags = h[3]
    try:
        new = comp(path, flags, inc, tempfile.mkdtemp(dir=tmp))
        old_src = subprocess.run(["git", "show", "HEAD:" + f], capture_output=True, cwd=ROOT).stdout
        # old file lives next to the original so relative includes work
        op = os.path.join(os.path.dirname(path), ".objcmp_old_" + os.path.basename(path))
        open(op, "wb").write(old_src)
        try: old = comp(op, flags, inc, tempfile.mkdtemp(dir=tmp))
        finally: os.remove(op)
    except Exception as e:
        return f + " (error: %s)" % str(getattr(e, "stderr", e))[-200:]
    if old != new and os.environ.get("OBJCMP_V"):
        import difflib
        print("\n".join(list(difflib.unified_diff(old.splitlines(), new.splitlines(), lineterm="", n=1))[:40]))
    return None if old == new else f
if __name__ == "__main__":
    files = sys.argv[1:] or subprocess.run(["git", "diff", "--name-only", "HEAD", "--", "src"], capture_output=True, text=True, cwd=ROOT).stdout.split()
    files = [f for f in files if f.endswith(".c")]
    tmp = tempfile.mkdtemp(); inc = os.path.join(tmp, "inc"); mkinc(inc, None)
    with Pool(8) as p: res = p.map(job, [(f, inc, tmp) for f in files])
    bad = [r for r in res if r]
    for b in bad: print("DIFF", b)
    print("checked %d, differ %d" % (len(files), len(bad)))
    shutil.rmtree(tmp, ignore_errors=True)
    sys.exit(1 if bad else 0)
