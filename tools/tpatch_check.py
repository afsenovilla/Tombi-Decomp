#!/usr/bin/env python3
"""Checks that tombi2 --text-patch (port2/shims/tpatch.c) rebuilds byte for byte what the Python pipeline writes:
for every file of the .tpatch, the C output == tools/text_patch.py apply --files-out (the re-encoded WFM / BIN / EXE
files) and == tools/data_patch.py's patched data/ (what the PC launcher runs, file tables of MAINn.EXE included).
Needs the disc image (or a pristine data/) and a built tombi2.

  python3 tools/tpatch_check.py game/disc/Tombi.bin patches/es_mejorada.tombipatch.json [--data data] [--exe build/port2/tombi2]
"""
import argparse, os, subprocess, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("disc"); ap.add_argument("patch")
    ap.add_argument("--data", default=os.path.join(ROOT, "data"), help="pristine data/ (for the data_patch comparison)")
    ap.add_argument("--patched-data", help="a data/ the PC launcher already patched with this patch (skips rebuilding one)")
    ap.add_argument("--exe", default=os.path.join(ROOT, "build", "port2", "tombi2"))
    a = ap.parse_args()
    a.disc, a.patch, a.data, a.exe = map(os.path.abspath, (a.disc, a.patch, a.data, a.exe))
    tmp = tempfile.mkdtemp(prefix="tpcheck_")
    cdump = os.path.join(tmp, "c"); os.makedirs(cdump)
    env = dict(os.environ, TOMBI_TPATCH_DUMP=cdump, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    log = subprocess.run([a.exe, "--disc", a.disc, "--text-patch", a.patch, "--headless", "--frames", "1"],
                         env=env, capture_output=True, text=True, cwd=tmp).stdout
    print("".join(l + "\n" for l in log.splitlines() if "[tpatch]" in l), end="")
    got = {f.replace("_", "/", 1): open(os.path.join(cdump, f), "rb").read() for f in os.listdir(cdump)}
    if not got:
        sys.exit("FAIL: tombi2 rebuilt no file")
    fo = os.path.join(tmp, "files_out")
    subprocess.run([sys.executable, os.path.join(HERE, "text_patch.py"), "apply", a.disc, a.patch, "--files-out", fo],
                   check=True, capture_output=True)
    if a.patched_data:
        pd = a.patched_data
    else:
        import tpatch_build
        pd = tpatch_build.build(a.data, a.patch, os.path.join(tmp, "x.tpatch"), 1 << 30, work=os.path.join(tmp, "w"),
                                log=lambda *_: None)
    bad = 0
    for p, data in sorted(got.items()):
        ref_dp = open(os.path.join(pd, *p.split("/")), "rb").read()
        f = os.path.join(fo, *p.split("/"))
        ref_fo = open(f, "rb").read() if os.path.isfile(f) else None
        ok_dp = data == ref_dp
        ok_fo = ref_fo is None or data == ref_fo or p.upper().startswith("EXE/")
        note = "" if ref_fo is None else ("" if data == ref_fo else " (files-out differs: file tables updated by data_patch)")
        print("%-4s %-24s %7d bytes%s" % ("ok" if ok_dp and ok_fo else "FAIL", p, len(data), note))
        bad += not (ok_dp and ok_fo)
    missing = sorted(set(os.listdir(fo) and [os.path.relpath(os.path.join(d, x), fo).replace(os.sep, "/")
                                            for d, _, fs in os.walk(fo) for x in fs]) - set(got))
    for p in missing:
        print("FAIL %-24s in --files-out, not rebuilt by tombi2" % p); bad += 1
    print("%d files, %d mismatches" % (len(got), bad))
    import shutil
    shutil.rmtree(tmp, ignore_errors=True)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
