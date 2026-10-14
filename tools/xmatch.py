#!/usr/bin/env python3
"""Find matched PAL functions inside another build's code (demo/JP executables and overlays). Reads only; prints.

Every PAL function with a size in config/symbol_addrs_<module>.txt (main0, x0nn) is turned into a masked word
sequence: j/jal targets, lui immediates and the 16-bit offset of loads/stores/addiu/ori whose base is not $sp are
zeroed, so relocated addresses (and moved globals) do not matter. Each target file is scanned for the same masked
sequence (longest functions first, no overlaps). A PS-X EXE is mapped from 0x800; other files from offset 0.

  python3 -I tools/xmatch.py TARGET [TARGET...] [--csv OUT.csv] [--repo .]

Output per target: matched function count, bytes covered, how many are byte-identical, and the PAL module mix.
--csv writes target, target offset, PAL module, PAL name, PAL address, size, identical. See docs/JPDEMO.md.
"""
import argparse, collections, csv, os, re, struct, sys

MEMOPS = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2a, 0x2b, 0x2e, 0x32, 0x3a}


def mask(w):
    op = w >> 26
    if op in (2, 3):
        return op << 26
    if op == 0x0F:
        return w & 0xFFFF0000
    if (op in MEMOPS or op in (8, 9, 0x0D)) and ((w >> 21) & 31) != 29:
        return w & 0xFFFF0000
    return w


def words(b):
    return struct.unpack("<%dI" % (len(b) // 4), b[: len(b) // 4 * 4])


def pal_funcs(repo):
    specs = [("main0", "game/MAIN0.EXE", 0x80010000, 0x800)]
    for f in sorted(os.listdir(os.path.join(repo, "config"))):
        m = re.fullmatch(r"(x0\d\d)\.yaml", f)
        if m:
            y = open(os.path.join(repo, "config", f)).read()
            tp = re.search(r"target_path:\s*(\S+)", y).group(1)
            vram = int(re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", y).group(1), 16)
            specs.append((m.group(1), tp, vram, 0))
    out = []
    for mod, path, vram, o0 in specs:
        sym = os.path.join(repo, "config", "symbol_addrs_%s.txt" % mod)
        if not os.path.exists(os.path.join(repo, path)) or not os.path.exists(sym):
            continue
        b = open(os.path.join(repo, path), "rb").read()
        for line in open(sym):
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func.*size:0x([0-9A-Fa-f]+)", line)
            if not m:
                continue
            a, s = int(m.group(2), 16), int(m.group(3), 16)
            off = a - vram + o0
            if s >= 20 and 0 <= off and off + s <= len(b):
                fb = b[off:off + s]
                out.append((mod, m.group(1), a, s, tuple(mask(w) for w in words(fb)), fb))
    out.sort(key=lambda f: -f[3])
    return out


def scan(path, funcs, K=4):
    b = open(path, "rb").read()
    if b[:8] == b"PS-X EXE":
        b = b[0x800:]
    tm = [mask(w) for w in words(b)]
    idx = collections.defaultdict(list)
    for i in range(len(tm) - K):
        idx[tuple(tm[i:i + K])].append(i)
    cov, hits = bytearray(len(tm)), []
    for mod, name, a, s, mw, fb in funcs:
        n = len(mw)
        for i in idx.get(mw[:K], ()):
            if tuple(tm[i:i + n]) == mw and not any(cov[i:i + n]):
                cov[i:i + n] = b"\1" * n
                hits.append((i * 4, mod, name, a, s, b[i * 4:i * 4 + s] == fb))
                break
    return len(b), sorted(hits)


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("targets", nargs="+")
    p.add_argument("--csv")
    p.add_argument("--repo", default=os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    a = p.parse_args()
    funcs = pal_funcs(a.repo)
    print("PAL functions with size:", len(funcs))
    rows = []
    for t in a.targets:
        size, hits = scan(t, funcs)
        nb = sum(h[4] for h in hits)
        mods = collections.Counter(h[1] for h in hits)
        print(f"{t}: {len(hits)} functions, {nb} bytes ({100 * nb / max(size, 1):.1f}%), "
              f"{sum(h[5] for h in hits)} identical; {dict(mods.most_common(6))}")
        rows += [(t, "0x%X" % h[0], h[1], h[2], "0x%08X" % h[3], h[4], int(h[5])) for h in hits]
    if a.csv:
        with open(a.csv, "w", newline="") as f:
            csv.writer(f).writerows([("target", "offset", "module", "name", "pal_addr", "size", "identical")] + rows)


if __name__ == "__main__":
    main()
