"""Finds the functions the HLE needs (Psy-Q libraries + a few game functions) in another build's executable by byte signature.

The HLE of machine.py is keyed by function address.  For retail PAL MAIN0 the names come from notes/functions_main0.csv +
notes/names_main0.csv; for the other builds (USA, JP, beta, demos) every named PAL MAIN0 function with a size is turned into a
masked word sequence (j/jal targets, lui immediates and the 16-bit offsets of loads/stores/addiu/ori off a base other than $sp are
zeroed, as in tools/xmatch.py) and searched in the target executable, longest functions first, without overlaps.  The hits are
written as a small table of addresses (tools/psxemu/syms/<build>.csv: name,address; no code or data of the game) that
builds.py loads.

  python3 tools/psxemu/libsig.py prepre9            (the exe of the build profile, see builds.py)
  python3 tools/psxemu/libsig.py prepre9 --exe PATH  [--all]

--all keeps every matched PAL name (default: only the names the HLE or capture.py use, plus the Psy-Q library names).
"""
import argparse
import collections
import csv
import os
import struct
import sys

if __package__ in (None, ""):
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from psxemu import builds as _builds
    from psxemu import machine as _machine
else:
    from . import builds as _builds
    from . import machine as _machine

ROOT = _builds.ROOT
MEMOPS = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A}
K = 4


def mask(w):
    op = w >> 26
    if op in (2, 3):
        return op << 26
    if op == 0x0F:
        return w & 0xFFFF0000
    if (op in MEMOPS or op in (8, 9, 0x0D)) and ((w >> 21) & 31) != 29:
        return w & 0xFFFF0000
    return w


def _words(b):
    return struct.unpack("<%dI" % (len(b) // 4), b[: len(b) // 4 * 4])


def pal_functions(main0=None):
    """[(name, pal_address, size, masked words)] of the named PAL MAIN0 functions, longest first."""
    main0 = main0 or os.path.join(ROOT, "game", "MAIN0.EXE")
    b = open(main0, "rb").read()
    names = {}
    for p in ("names_main0.csv", "functions_main0.csv"):         # names_main0 wins (hand-named), functions_main0 adds the rest
        with open(os.path.join(ROOT, "notes", p), newline="", encoding="utf-8") as f:
            for r in csv.DictReader(f):
                try:
                    nl = names.setdefault(int(r["address"], 16), [])
                    if r["name"] not in nl:
                        nl.append(r["name"])
                except (KeyError, ValueError):
                    pass
    sizes = {}
    with open(os.path.join(ROOT, "notes", "functions_main0.csv"), newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            try:
                sizes[int(r["address"], 16)] = int(r["size"])
            except (KeyError, ValueError):
                pass
    pm = [mask(w) for w in _words(b[0x800:])]
    out = []
    for a, s in sizes.items():
        i = (a - 0x80010000) // 4
        if s >= 20 and 0 <= i and i + s // 4 <= len(pm):
            out.append((names[a], a, s, i))
    out.sort(key=lambda f: (-f[2], f[1]))
    return pm, out


def _index(tm):
    idx = collections.defaultdict(list)
    for i in range(len(tm) - K):
        idx[tuple(tm[i:i + K])].append(i)
    return idx


def _hits(tm, idx, sig):
    n = len(sig)
    return [i for i in idx.get(sig[:K], ()) if tuple(tm[i:i + n]) == sig]


def scan(exe_bytes, pal, load=None, max_ext=64):
    """{name: [address]} of the PAL functions found in a PS-X EXE (or a raw blob loaded at `load`).  A function's signature is its
    masked words; when that sequence is not unique in PAL MAIN0 (small wrappers: LoadImage / StoreImage, a jal + return) or hits
    more than once in the target, it is extended over the words that follow it in PAL (the next function of the same library
    object) until both are unique; no hit or no unique hit within max_ext words = not found."""
    pm, funcs = pal
    b = exe_bytes
    if b[:8] == b"PS-X EXE":
        load = struct.unpack_from("<I", b, 0x18)[0]
        b = b[0x800:]
    tm = [mask(w) for w in _words(b)]
    pidx, tidx = _index(pm), _index(tm)
    found = {}
    for nl, a, s, i0 in funcs:
        n = s // 4
        while n <= s // 4 + max_ext and i0 + n <= len(pm):
            sig = tuple(pm[i0:i0 + n])
            th = _hits(tm, tidx, sig)
            if not th:
                break
            if len(th) == 1 and len(_hits(pm, pidx, sig)) == 1:
                for name in nl:
                    found.setdefault(name, []).append(load + 4 * th[0])
                break
            n += 1
    return found


LIB_PREFIXES = ("Ss", "_Ss", "Spu", "_Spu", "_spu", "Cd", "CD_", "St", "DecDCT", "Pad", "_card", "_bu", "Card", "Mem", "Sound", "Sfx")


def wanted(name):
    m = _machine
    return (name in m.STUB_NAMES or name in m.KEEP or name in m.HLE_NAMES or name.startswith(LIB_PREFIXES)
            or name in _builds.GAME_NAMES)


def build_table(build, exe=None, keep_all=False, log=print):
    prof = _builds.get(build)
    exe = exe or prof.path("exe")
    found = scan(open(exe, "rb").read(), pal_functions())
    rows = sorted((a, n) for n, al in found.items() for a in al if keep_all or wanted(n))
    out = os.path.join(_builds.SYMS_DIR, "%s.csv" % build)
    os.makedirs(_builds.SYMS_DIR, exist_ok=True)
    with open(out, "w", newline="") as f:
        f.write("# %s: PAL MAIN0 functions found in %s by masked byte signature (tools/psxemu/libsig.py); addresses only\n"
                % (build, os.path.basename(exe)))
        w = csv.writer(f, lineterminator="\n")
        w.writerow(("address", "name"))
        for a, n in rows:
            w.writerow(("%08x" % a, n))
    log("%s: %d names (%d PAL functions matched) -> %s" % (build, len(rows), len(found), out))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("build")
    ap.add_argument("--exe")
    ap.add_argument("--all", action="store_true")
    a = ap.parse_args(argv)
    build_table(a.build, a.exe, a.all)
    return 0


if __name__ == "__main__":
    sys.exit(main())
