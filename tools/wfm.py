#!/usr/bin/env python3
"""Thin wrapper around the PAL fork of TombaTools (WFM text workflow). The Go source is NOT in this repo.

The fork (branch `pal` of hansbonini/tombatools plus lossless round trip and fonts/es) lives
outside the repo; point TOMBATOOLS at it (default /opt/psyq/tombatools-pal). The binary is
(re)built with `go build` when missing or older than the sources.

  tools/wfm.py decode  [PATH] [-o OUT]   PATH = one .WFM or a directory (default game/disc/fs)
                                         -> OUT/<relative path without .WFM>/{dialogues.yaml,glyphs/}
                                         (default OUT = build/wfm)
  tools/wfm.py encode  YAML  OUT.WFM     dialogues.yaml -> byte-exact WFM
  tools/wfm.py --check [PATH]            decode+encode every WFM, print how many are identical
                                         (PATH default game/disc/fs; exit 1 if any differs)

Options: --fonts SETS  reference font sets, comma separated: names under $TOMBATOOLS/fonts
(br, eu, us, es) or paths. Default: every set under fonts/ (includes `es`, the Spanish
glyphs). Also taken from $TOMBATOOLS_FONTS. Example: --fonts br,es
"""
import argparse, os, shutil, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TT = os.path.abspath(os.environ.get("TOMBATOOLS", "/opt/psyq/tombatools-pal"))
FS = os.path.join(ROOT, "game", "disc", "fs")


def binary():
    exe = os.path.join(TT, "tombatools")
    srcs = [os.path.join(d, f) for d, _, fs in os.walk(TT) for f in fs
            if f.endswith(".go") or f in ("go.mod", "go.sum")]
    if not os.path.exists(exe) or any(os.path.getmtime(s) > os.path.getmtime(exe) for s in srcs):
        go = shutil.which("go") or "/usr/local/go/bin/go"
        print("building %s" % exe, file=sys.stderr)
        subprocess.run([go, "build", "-o", exe, "."], cwd=TT, check=True)
    return exe


def font_dirs(spec):
    spec = spec or os.environ.get("TOMBATOOLS_FONTS") or ""
    if not spec:
        return os.path.join(TT, "fonts")
    return ",".join(p if os.sep in p else os.path.join(TT, "fonts", p) for p in spec.split(","))


def run(args, fonts):
    # cwd = fork dir so a relative fonts/ also works; fonts are absolute anyway
    env = dict(os.environ, TOMBATOOLS_FONTS=font_dirs(fonts))
    return subprocess.run([binary()] + args, cwd=TT, env=env, capture_output=True, text=True)


def wfm_files(path):
    path = os.path.abspath(path)
    if os.path.isfile(path):
        return [path]
    return sorted(os.path.join(d, f) for d, _, fs in os.walk(path) for f in fs if f.upper().endswith(".WFM"))


def rel(path):
    base = FS if os.path.abspath(path).startswith(FS + os.sep) else os.path.dirname(os.path.abspath(path))
    return os.path.splitext(os.path.relpath(os.path.abspath(path), base))[0]


def cmd_decode(a):
    files = wfm_files(a.path)
    out = os.path.abspath(a.out)
    def one(f):
        d = os.path.join(out, rel(f))
        os.makedirs(d, exist_ok=True)
        r = run(["wfm", "decode", f, d], a.fonts)
        return f, r.returncode
    with ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        bad = [f for f, rc in ex.map(one, files) if rc]
    print("decoded %d/%d into %s" % (len(files) - len(bad), len(files), out))
    return 1 if bad else 0


def cmd_encode(a):
    r = run(["wfm", "encode", os.path.abspath(a.yaml), os.path.abspath(a.wfm)], a.fonts)
    if r.returncode:
        sys.stderr.write(r.stderr)
    return r.returncode


def cmd_check(a):
    files = wfm_files(a.path)
    binary()
    def one(f):
        tmp = tempfile.mkdtemp(prefix="wfm_rt_")
        try:
            r = run(["wfm", "decode", f, tmp], a.fonts)
            if r.returncode:
                return f, "decode failed"
            out = os.path.join(tmp, "rt.WFM")
            r = run(["wfm", "encode", os.path.join(tmp, "dialogues.yaml"), out], a.fonts)
            if r.returncode or not os.path.exists(out):
                return f, "encode failed"
            return f, None if open(f, "rb").read() == open(out, "rb").read() else "differs"
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
    with ThreadPoolExecutor(os.cpu_count() or 2) as ex:
        res = list(ex.map(one, files))
    bad = [(f, why) for f, why in res if why]
    for f, why in bad:
        print("%s: %s" % (os.path.relpath(f, ROOT), why))
    print("identical: %d/%d" % (len(files) - len(bad), len(files)))
    return 1 if bad else 0


def main():
    argv = sys.argv[1:]
    if "--check" in argv:  # `wfm.py --check [PATH]` is the same as `wfm.py check [PATH]`
        argv.remove("--check")
        argv.insert(0, "check")
    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--fonts", help="font sets, comma separated (names under $TOMBATOOLS/fonts or paths)")
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd")
    d = sub.add_parser("decode", parents=[common])
    d.add_argument("path", nargs="?", default=FS)
    d.add_argument("-o", "--out", default=os.path.join(ROOT, "build", "wfm"))
    e = sub.add_parser("encode", parents=[common])
    e.add_argument("yaml")
    e.add_argument("wfm")
    c = sub.add_parser("check", parents=[common])
    c.add_argument("path", nargs="?", default=FS)
    a = p.parse_args(argv)
    if a.cmd is None:
        p.print_help()
        return 2
    if not os.path.isdir(TT):
        sys.exit("TOMBATOOLS=%s not found (set TOMBATOOLS to the tombatools-pal checkout)" % TT)
    return {"decode": cmd_decode, "encode": cmd_encode, "check": cmd_check}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main())
