#!/usr/bin/env python3
"""Dump the DATA part of the area overlays (AREAxx/X0nn.BIN, load address 0x800E8028). Layout notes: notes/overlays.md.

The overlay is a headerless image: [header table][entry stubs][functions + the data their compilation unit emitted
between them][trailing data]. Code ranges come from notes/functions_x0nn.csv; every other byte is data and is split into
items: ASCII strings, pointer tables (to overlay code, MAIN0, overlay data), zero runs, plain words. Names come from
config/symbol_addrs_x0nn.txt + config/symbol_addrs_main0.txt, else the csv, else D_<addr> / FUN_<addr>.

  tools/overlay_data.py summary [X0nn ...]        one line per overlay: sizes of header/stubs/code/data/tail
  tools/overlay_data.py dump X0nn [--min N]       every data gap with classified items (gaps shorter than N bytes hidden)
  tools/overlay_data.py header X0nn               the 29-entry dispatch table at the start and its stubs
  tools/overlay_data.py strings X0nn              ASCII strings found in data gaps

X0nn is a name (X000..X019) or a path to the .BIN. No repack: the files are byte-exact by construction.
"""
import argparse, glob, os, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BASE = 0x800E8028
SHARED_END = 0x80115EA4          # first 188,028 bytes are identical in all overlays
SHARED_DATA = 0x80114A48         # shared data block (SHARED_DATA..SHARED_END)
HDR_END = 0x800E9ECC             # first stub; the header table is the first 4*(1+29) bytes


def overlay_path(arg):
    if os.path.isfile(arg):
        return arg
    n = int(re.search(r"(\d+)", arg).group(1)) % 100
    hits = glob.glob(os.path.join(ROOT, "game", "AREA%02d" % n, "X0%02d.BIN" % n)) + \
        glob.glob(os.path.join(ROOT, "game", "disc", "fs", "AREA%02d" % n, "X0%02d.BIN" % n))
    if not hits:
        sys.exit("overlay %s not found under game/" % arg)
    return hits[0]


def prog_of(path):
    return re.search(r"X(0\d\d)\.BIN", path).group(1)


class Ctx:
    def __init__(self, path):
        self.path = path
        self.d = open(path, "rb").read()
        self.end = BASE + len(self.d)
        nn = prog_of(path)
        self.fn = []                       # (addr, size, name)
        csv = os.path.join(ROOT, "notes", "functions_x%s.csv" % nn)
        for l in open(csv).read().splitlines()[1:]:
            p = l.split(",")
            if p[1].isdigit():                       # address,size,name,twin (X001..)
                self.fn.append((int(p[0], 16), int(p[1]), p[2]))
            else:                                   # address,name,size,typed (X000)
                self.fn.append((int(p[0], 16), int(p[2]), p[1]))
        self.fn.sort()
        self.sym = {}
        have = {f[0] for f in self.fn}
        for f in ("symbol_addrs_x%s.txt" % nn, "symbol_addrs_main0.txt"):
            for l in open(os.path.join(ROOT, "config", f)):
                m = re.match(r"(\w+)\s*=\s*(0x[0-9A-Fa-f]+);(.*)", l)
                if m:
                    a = int(m.group(2), 16)
                    self.sym.setdefault(a, m.group(1))
                    z = re.search(r"type:func size:(0x[0-9A-Fa-f]+)", m.group(3))
                    if z and f.startswith("symbol_addrs_x") and self.in_ov(a) and a not in have:
                        self.fn.append((a, int(z.group(1), 16), m.group(1)))   # function symbols the csv lacks
                        have.add(a)
        self.fn.sort()
        self.starts = [f[0] for f in self.fn]
        for a, s, n in self.fn:
            self.sym.setdefault(a, n)

    def word(self, a):
        return struct.unpack_from("<I", self.d, a - BASE)[0]

    def in_ov(self, a):
        return BASE <= a < self.end

    def name(self, a):
        if a in self.sym:
            return self.sym[a]
        # inside a function: name+off
        import bisect
        i = bisect.bisect_right(self.starts, a) - 1
        if i >= 0 and self.fn[i][0] <= a < self.fn[i][0] + self.fn[i][1]:
            return "%s+0x%X" % (self.fn[i][2], a - self.fn[i][0])
        if self.in_ov(a):
            return "D_%08X" % a
        if 0x80010000 <= a < 0x8009A000:
            return "MAIN0:%08X" % a
        return "%08X" % a

    def gaps(self):
        """[(start, end)] of bytes not covered by a function (header+stubs counted as the first gap)."""
        out, cur = [], BASE
        for a, s, _ in self.fn:
            if a > cur:
                out.append((cur, a))
            cur = max(cur, a + s)
        if cur < self.end:
            out.append((cur, self.end))
        return out


def classify(c, lo, hi):
    """Split [lo,hi) into [(kind, start, end, text)]. Kinds: zero, str, ptr, word, code?"""
    items, a = [], lo
    d = c.d
    while a < hi:
        o = a - BASE
        # zero run
        n = 0
        while a + n < hi and d[o + n] == 0:
            n += 1
        if n >= 8:
            n -= n % 4 if (a + n < hi) else 0
            items.append(("zero", a, a + n, ""))
            a += n
            continue
        # ASCII string
        n = 0
        while a + n < hi and 32 <= d[o + n] < 127:
            n += 1
        if n >= 6 and (a + n < hi and d[o + n] == 0) and sum(chr(x).isalpha() for x in d[o:o + n]) * 10 >= n * 6:
            s = d[o:o + n].decode("ascii")
            e = a + n + 1
            while e % 4 and e < hi and d[e - BASE] == 0:
                e += 1
            items.append(("str", a, e, repr(s)))
            a = e
            continue
        # pointer table (aligned)
        if a % 4 == 0:
            k = 0
            while a + 4 * k + 4 <= hi:
                w = c.word(a + 4 * k)
                if not (0x80010000 <= w < 0x80200000):
                    break
                k += 1
            if k >= 2 or (k == 1 and c.in_ov(c.word(a))):
                tg = [c.name(c.word(a + 4 * i)) for i in range(k)]
                bases = {t.split("+")[0] for t in tg}
                kind = "jtbl" if len(bases) == 1 and "+" in tg[0] else "ptr"
                items.append((kind, a, a + 4 * k, ", ".join(c.name(c.word(a + 4 * i)) for i in range(min(k, 6))) + (" ..." if k > 6 else "")))
                a += 4 * k
                continue
        # sp adjust = probably code the csv does not know
        if a % 4 == 0 and a + 4 <= hi and (c.word(a) >> 16) == 0x27BD and (c.word(a) & 0x8000):
            items.append(("code?", a, hi, "addiu sp,sp,-N: probably an undetected function"))
            a = hi
            continue
        e = min(a + 4, hi)
        if items and items[-1][0] == "word" and items[-1][2] == a:
            k, s, _, t = items[-1]
            items[-1] = (k, s, e, t)
        else:
            items.append(("word", a, e, ""))
        a = e
    return items


def cmd_summary(args):
    names = args.names or ["X%03d" % n for n in (0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]
    print("%-5s %8s %8s %8s %8s %8s %8s %5s" % ("prog", "size", "hdr+stub", "code", "gap data", "tail", "strings", "funcs"))
    for nme in names:
        c = Ctx(overlay_path(nme))
        code = sum(s for _, s, _ in c.fn)
        first = c.fn[0][0]
        last = c.fn[-1][0] + c.fn[-1][1]
        mid = sum(e - s for s, e in c.gaps() if s != BASE and not (e == c.end and s >= last))
        tail = c.end - last
        strs = sum(1 for s, e in c.gaps() for it in classify(c, s, e) if it[0] == "str")
        print("%-5s %8d %8d %8d %8d %8d %8d %5d" % (prog_of(c.path), len(c.d), first - BASE, code, mid, tail, strs, len(c.fn)))
    return 0


def cmd_dump(args):
    c = Ctx(overlay_path(args.name))
    for s, e in c.gaps():
        if e - s < args.min:
            continue
        tag = "header+stubs" if s == BASE else "tail" if e == c.end and s >= c.fn[-1][0] + c.fn[-1][1] else "gap"
        sh = " [shared prefix]" if e <= SHARED_END else ""
        before = [f for f in c.fn if f[0] + f[1] == s]
        print("\n== %s 0x%08X..0x%08X (%d bytes, file 0x%X)%s  after %s" % (tag, s, e, e - s, s - BASE, sh, before[0][2] if before else "-"))
        if s == BASE:
            continue
        for k, a, b, t in classify(c, s, e):
            nm = c.sym.get(a, "")
            print("  %08X %5d %-5s %s%s" % (a, b - a, k, ("<%s> " % nm) if nm else "", t))
    return 0


def cmd_header(args):
    c = Ctx(overlay_path(args.name))
    n = c.word(BASE)
    print("count word = %d; table at 0x%08X (file 0x4)" % (n, BASE + 4))
    ents = [c.word(BASE + 4 + 4 * i) for i in range(n)]
    print("entries -> stubs at 0x%08X + 16*k, %d distinct" % (min(ents), len(set(ents))))
    for i, e in enumerate(ents):
        o = e - BASE
        w = struct.unpack_from("<4I", c.d, o)
        tgt = lambda x: ((x & 0x03FFFFFF) << 2) | 0x80000000
        if w[0] >> 26 == 3 and w[2] >> 26 == 2:
            print("  [%2d] %08X  stub: jal %s ; nop ; j %s ; nop" % (i, e, c.name(tgt(w[0])), c.name(tgt(w[2]))))
        elif w[0] == 0x8FBF0010:
            print("  [%2d] %08X  common epilogue (lw ra,0x10(sp); jr ra): empty handler" % (i, e))
        else:
            print("  [%2d] %08X  code inside %s" % (i, e, c.name(e)))
    first = c.fn[0][0]
    print("stubs end / first function: 0x%08X (file 0x%X)" % (first, first - BASE))
    return 0


def cmd_strings(args):
    c = Ctx(overlay_path(args.name))
    for s, e in c.gaps():
        for k, a, b, t in classify(c, s, e):
            if k == "str":
                print("%08X %s" % (a, t))
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("summary"); p.add_argument("names", nargs="*"); p.set_defaults(f=cmd_summary)
    p = sp.add_parser("dump"); p.add_argument("name"); p.add_argument("--min", type=int, default=0); p.set_defaults(f=cmd_dump)
    p = sp.add_parser("header"); p.add_argument("name"); p.set_defaults(f=cmd_header)
    p = sp.add_parser("strings"); p.add_argument("name"); p.set_defaults(f=cmd_strings)
    a = ap.parse_args()
    return a.f(a)


if __name__ == "__main__":
    sys.exit(main())
