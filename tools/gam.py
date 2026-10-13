#!/usr/bin/env python3
"""Thin wrapper around the PAL fork of TombaTools (GAM = the game's LZ container). The Go source is NOT in this repo.

Same setup as tools/wfm.py: TOMBATOOLS points at the fork (default /opt/psyq/tombatools-pal, branch `pal`), the
binary is (re)built with `go build` when missing or older than the sources.

  tools/gam.py unpack [PATH] [-o OUT]    PATH = one .GAM or a directory (default game/disc/fs)
                                         -> OUT/<relative path>.UNGAM (default OUT = build/gam)
  tools/gam.py pack   IN.UNGAM OUT.GAM [--orig ORIG.GAM] [--no-pad]
  tools/gam.py pack   [DIR] [-o OUT] [--orig-dir FS] [--no-pad]
                                         every .UNGAM under DIR (default build/gam) -> OUT/<same path>.GAM
                                         (default OUT = build/gam_out). With --orig / --orig-dir the original
                                         compressed bytes are kept for every file whose data did not change.
  tools/gam.py --check [PATH]            unpack + pack every GAM (default game/disc/fs, 816 files) and print how
                                         many come back byte-identical:
                                           encoder only, default variant      (pure re-compression)
                                           encoder only, best of both variants (--no-pad tried when needed)
                                           with the original bytes kept for unchanged data (what a rebuild uses)
                                         exit 1 if the last number is not all files.

The re-compressor is the fork's reimplementation of the original encoder (see notes/assets_tombatools.md for
the rules found); it is not exact for every file, which is why `pack` can take the original GAM and keep its
bytes when the unpacked data is unchanged. Changed data is always re-encoded (valid stream, may differ from what
the original tool would have written).
"""
import argparse, os, shutil, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TT = os.path.abspath(os.environ.get("TOMBATOOLS", "/opt/psyq/tombatools-pal"))
FS = os.path.join(ROOT, "game", "disc", "fs")
UNPACKED = os.path.join(ROOT, "build", "gam")
PACKED = os.path.join(ROOT, "build", "gam_out")


def binary():
    exe = os.path.join(TT, "tombatools")
    srcs = [os.path.join(d, f) for d, _, fs in os.walk(TT) for f in fs
            if f.endswith(".go") or f in ("go.mod", "go.sum")]
    if not os.path.exists(exe) or any(os.path.getmtime(s) > os.path.getmtime(exe) for s in srcs):
        go = shutil.which("go") or "/usr/local/go/bin/go"
        print("building %s" % exe, file=sys.stderr)
        subprocess.run([go, "build", "-o", exe, "."], cwd=TT, check=True)
    return exe


def run(args):
    return subprocess.run([binary()] + args, cwd=TT, capture_output=True, text=True)


def gam_files(path):
    path = os.path.abspath(path)
    if os.path.isfile(path):
        return [path]
    return sorted(os.path.join(d, f) for d, _, fs in os.walk(path) for f in fs if f.upper().endswith(".GAM"))


def rel(path, ext):
    base = FS if os.path.abspath(path).startswith(FS + os.sep) else os.path.dirname(os.path.abspath(path))
    return os.path.splitext(os.path.relpath(os.path.abspath(path), base))[0] + ext


def unpack_one(src, dst):
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    r = run(["gam", "unpack", src, dst])
    return r.returncode == 0


def pack_one(src, dst, orig=None, no_pad=False):
    os.makedirs(os.path.dirname(os.path.abspath(dst)), exist_ok=True)
    args = ["gam", "pack"] + (["--no-pad"] if no_pad else []) + (["--orig", orig] if orig else []) + [src, dst]
    r = run(args)
    if r.returncode:
        sys.stderr.write(r.stdout + r.stderr)
    return r.returncode == 0


def cmd_unpack(a):
    files = gam_files(a.path)
    out = os.path.abspath(a.out)
    with ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        res = list(ex.map(lambda f: (f, unpack_one(f, os.path.join(out, rel(f, ".UNGAM")))), files))
    bad = [f for f, ok in res if not ok]
    for f in bad:
        print("failed: %s" % f, file=sys.stderr)
    print("unpacked %d/%d into %s" % (len(files) - len(bad), len(files), out))
    return 1 if bad else 0


def cmd_pack(a):
    first = a.paths[0] if a.paths else UNPACKED
    if os.path.isfile(first):
        if len(a.paths) != 2:
            sys.exit("usage: gam.py pack IN.UNGAM OUT.GAM [--orig ORIG.GAM]")
        return 0 if pack_one(os.path.abspath(first), os.path.abspath(a.paths[1]), a.orig and os.path.abspath(a.orig), a.no_pad) else 1
    root = os.path.abspath(first)
    out = os.path.abspath(a.out)
    files = sorted(os.path.join(d, f) for d, _, fs in os.walk(root) for f in fs if f.upper().endswith(".UNGAM"))
    orig_dir = os.path.abspath(a.orig_dir) if a.orig_dir else None

    def one(f):
        r = os.path.splitext(os.path.relpath(f, root))[0] + ".GAM"
        orig = os.path.join(orig_dir, r) if orig_dir and os.path.isfile(os.path.join(orig_dir, r)) else None
        return f, pack_one(f, os.path.join(out, r), orig, a.no_pad)
    with ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        res = list(ex.map(one, files))
    bad = [f for f, ok in res if not ok]
    print("packed %d/%d into %s" % (len(files) - len(bad), len(files), out))
    return 1 if bad else 0


def same(a, b):
    return os.path.exists(b) and open(a, "rb").read() == open(b, "rb").read()


def cmd_check(a):
    files = gam_files(a.path)
    binary()

    def one(f):
        tmp = tempfile.mkdtemp(prefix="gam_rt_")
        try:
            ung, out = os.path.join(tmp, "x.UNGAM"), os.path.join(tmp, "x.GAM")
            if not unpack_one(f, ung):
                return f, None
            pack_one(ung, out)
            plain = same(f, out)
            nopad = plain
            if not plain:
                pack_one(ung, out, no_pad=True)
                nopad = same(f, out)
            pack_one(ung, out, orig=f)
            return f, (plain, plain or nopad, same(f, out))
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
    with ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        res = list(ex.map(one, files))
    broken = [f for f, r in res if r is None]
    ok = [r for _, r in res if r]
    for f in broken:
        print("unpack failed: %s" % os.path.relpath(f, ROOT))
    n = len(files)
    print("files: %d" % n)
    print("identical, encoder only (default):             %d/%d" % (sum(r[0] for r in ok), n))
    print("identical, encoder only (best of both models): %d/%d" % (sum(r[1] for r in ok), n))
    print("identical, original bytes kept if unchanged:   %d/%d" % (sum(r[2] for r in ok), n))
    if a.verbose:
        for f, r in res:
            if r and not r[1]:
                print("re-encoded differently: %s" % os.path.relpath(f, ROOT))
    return 0 if len(ok) == n and all(r[2] for r in ok) else 1


def main():
    argv = sys.argv[1:]
    if "--check" in argv:  # `gam.py --check [PATH]` is the same as `gam.py check [PATH]`
        argv.remove("--check")
        argv.insert(0, "check")
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd")
    u = sub.add_parser("unpack")
    u.add_argument("path", nargs="?", default=FS)
    u.add_argument("-o", "--out", default=UNPACKED)
    k = sub.add_parser("pack")
    k.add_argument("paths", nargs="*")
    k.add_argument("-o", "--out", default=PACKED)
    k.add_argument("--orig", help="original GAM (single file mode)")
    k.add_argument("--orig-dir", help="directory with the original GAM files (tree mode), e.g. game/disc/fs")
    k.add_argument("--no-pad", action="store_true", help="other encoder variant (see the fork's pkg/gamlz.go)")
    c = sub.add_parser("check")
    c.add_argument("path", nargs="?", default=FS)
    c.add_argument("-v", "--verbose", action="store_true", help="list the files the encoder does not reproduce")
    a = p.parse_args(argv)
    if a.cmd is None:
        p.print_help()
        return 2
    if not os.path.isdir(TT):
        sys.exit("TOMBATOOLS=%s not found (set TOMBATOOLS to the tombatools-pal checkout)" % TT)
    return {"unpack": cmd_unpack, "pack": cmd_pack, "check": cmd_check}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main())
