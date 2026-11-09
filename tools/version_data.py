#!/usr/bin/env python3
"""PAL data-only files (src/data/<prog>/*.c) reused by the report of another version (tools/build_report.py --version).

A PAL data file (`// DATA addr size PROG`, see tools/build_full.py data_files) is C: tables with pointers written as
symbols (functions, other data_XXXXXXXX tables, D_ globals). Most of those tables are in the other retail versions
too, at another address, with the pointers moved. This module finds where (no address table to maintain):

  1. each PAL data file of the program's PAL module (tools/versions.py pal_prog) is compiled once;
  2. its section is looked for in the version's retail file with the relocated words masked (the bytes C sets as
     constants must be equal); a file found exactly once in the free part of the program (not code, not data another
     unit claims, not the X00 prefix of an area overlay) is placed there; a file found several times or with too few
     constant bytes to search (a lone pointer) is tried at the address its placed neighbours give (same PAL->version
     shift as the nearest placed file before or after it);
  3. every relocation must then point where the version's word points, and that address must be the version's
     address of the symbol: a name the version's config/<ver>/symbol_addrs_*.txt gives, or the PAL address of the
     symbol carried over through the placed data files (a pointer into a table another data file holds) or the
     function pairing (`PAL <module> <addr>` comments and reuse.txt). A symbol neither of them knows is accepted only
     when all its uses in the version agree on one address inside the version's programs (counted as `inferred`).

The report then compares the compiled file, relocated to the version's addresses, with the retail bytes, exactly like
the PAL report does for its own data files (tools/report_data.py claim_data / patch_base).

The bytes nothing claims after that become the version's own data files (src/data/<ver>/<prog>/D_<addr>.c, header
`// DATA addr size <ver>/<prog>`): `gen` writes them from build/<ver>/data_rest.json (the ranges the last report left),
as words like the PAL `unclaimed range` files, without the WFM2/WFM3 banks and config/<ver>/data_assets.txt (assets).
See docs/JP.md "Data measure".

  python3 tools/version_data.py --version slps01144          what the PAL files would place, per program (code ranges
                                                             from the symbol files: an approximation of the report)
  python3 tools/version_data.py --version slps01144 gen [prog ...]
"""
import json, os, re, struct, subprocess, sys, tempfile, shutil

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import build_full as BF
from versions import VERSIONS, pal_prog

OVL_BASE_PAL = 0x800E8028  # first PAL overlay address: below it an address is MAIN0's
NAMED = re.compile(r"(?:D|func|FUN|DAT|LAB|jtbl|data)_([0-9A-Fa-f]{8})$")


def _compile(job):
    path, out, inc = job
    NC = _compile.__dict__.setdefault("nc", {})
    if not NC:
        s = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
        NC.update({"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"})
        exec(compile(s, "ncheck", "exec"), NC)
    d = tempfile.mkdtemp()
    try:
        NC["build"](path, "-O2 -G0", d, inc)
        os.makedirs(os.path.dirname(out), exist_ok=True)
        shutil.copy(d + "/a.o", out)
        return None
    except subprocess.CalledProcessError as e:
        return str(e.stderr)[-200:]
    finally:
        shutil.rmtree(d, ignore_errors=True)


def _compile_all(files, what):
    """Compiles {key: [(path, stem, addr, size, obj)]}; returns the same without the files that failed."""
    from multiprocessing import Pool
    inc = tempfile.mkdtemp()
    for f in os.listdir(os.path.join(ROOT, "include")):
        s = os.path.join(ROOT, "include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(inc, nm))
    jobs = [(f[0], f[4], inc) for fs in files.values() for f in fs]
    with Pool(os.cpu_count() or 4) as pool:
        res = pool.map(_compile, jobs)
    shutil.rmtree(inc, ignore_errors=True)
    bad = {j[1] for j, r in zip(jobs, res) if r}
    for j, r in zip(jobs, res):
        if r: print(what, "data compile failed:", os.path.relpath(j[0], ROOT), r)
    return {m: [f for f in fs if f[4] not in bad] for m, fs in files.items()}


def compile_pal(mods, out_dir):
    """{PAL module: [(path, stem, PAL addr, size, obj)]} of the PAL data files, compiled into out_dir/<module>/."""
    files = {}
    for m in mods:
        for path, stem, addr, size in BF.data_files(m):
            files.setdefault(m, []).append((path, stem, addr, size, os.path.join(out_dir, m, stem + ".o")))
    return _compile_all(files, "PAL")


VER_HDR = re.compile(r"^//\s*DATA\s+([0-9a-fA-F]{8})\s+(\d+)\s+(\w+)/(\w+)\s*$", re.M)


def version_files(ver, prog):
    """[(path, stem, addr, size)] of the version's own data files src/data/<ver>/<prog>/*.c."""
    d = os.path.join(ROOT, "src", "data", ver, prog)
    out = []
    for f in sorted(os.listdir(d)) if os.path.isdir(d) else ():
        if not f.endswith(".c"): continue
        m = VER_HDR.search(open(os.path.join(d, f), errors="replace").read(400))
        if m and m.group(3) == ver and m.group(4) == prog:
            out.append((os.path.join(d, f), f[:-2], int(m.group(1), 16), int(m.group(2))))
    return out


def compile_version(ver, progs, out_dir):
    """{prog: [(path, stem, addr, size, obj)]} of the version's own data files, compiled into out_dir/<prog>/."""
    files = {}
    for p in progs:
        for path, stem, addr, size in version_files(ver, p):
            files.setdefault(p, []).append((path, stem, addr, size, os.path.join(out_dir, p, stem + ".o")))
    return _compile_all(files, ver)


def gaps(start, end, taken):
    """[[a, b]] of the bytes of start..end outside every range of taken."""
    out, pos = [], start
    for a, b in sorted(taken):
        if a > pos: out.append([pos, a])
        pos = max(pos, b)
    if pos < end: out.append([pos, end])
    return out


class Maps:
    """What the version knows about addresses: names (config/<ver>/symbol_addrs_*.txt) and PAL -> version pairs."""

    def __init__(self, ver):
        self.ver = ver
        self.names = {}     # name -> version address (first one wins, the files of every program)
        self.pal = {}       # (PAL module, PAL addr) -> version address (functions)
        cdir = os.path.join(ROOT, "config", ver)
        for fn in sorted(os.listdir(cdir)):
            if not (fn.startswith("symbol_addrs_") and fn.endswith(".txt")): continue
            prog = fn[len("symbol_addrs_"):-4]
            for l in open(os.path.join(cdir, fn)):
                m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);(.*)", l)
                if not m: continue
                a = int(m.group(2), 16)
                self.names.setdefault(m.group(1), set()).add(a)
                p = re.search(r"PAL (\w+) 0x([0-9A-Fa-f]+)", m.group(3))
                if p: self.pal.setdefault((p.group(1), int(p.group(2), 16)), set()).add(a)
        rp = os.path.join(cdir, "reuse.txt")
        for l in open(rp) if os.path.exists(rp) else ():
            f = l.split("#")[0].split()
            if len(f) == 4: self.pal.setdefault((f[2].lower(), int(f[3], 16)), set()).add(int(f[1], 16))
        self.pal_names = {}  # PAL module -> {name: PAL addr}

    def pal_name(self, mod, name):
        if mod not in self.pal_names: self.pal_names[mod] = BF.symbol_names(mod)
        m = NAMED.match(name)
        return int(m.group(1), 16) if m else self.pal_names[mod].get(name)


def masked(e, sec):
    b = bytearray(e.data(sec))
    rel = e.relocs(sec)
    for off, ty, _ in rel:
        struct.pack_into("<I", b, off, 0)
    return bytes(b), {off for off, _, _ in rel}


def anchor(b, rel):
    """(offset, bytes) of the longest word-aligned run without relocations, or None when it has < 8 non-zero bytes."""
    best, cur = (0, 0), None
    for o in range(0, len(b) + 4, 4):
        if o < len(b) and o not in rel:
            if cur is None: cur = o
            continue
        if cur is not None and o - cur > best[1] - best[0]: best = (cur, min(o, len(b)))
        cur = None
    s = b[best[0]:best[1]]
    if sum(1 for x in s if x) < 8: return None
    return best[0], s


class Placer:
    """Places the PAL data files of one PAL module in one version program."""

    def __init__(self, maps, mod, prog, game, gbase, busy, placed_global):
        self.maps, self.mod, self.prog, self.game, self.gbase = maps, mod, prog, game, gbase
        self.end = gbase + len(game)
        self.busy = sorted(busy)            # [(a, b)] code / claimed / excluded ranges of this program
        self.placed = placed_global          # {(PAL module, PAL addr): (version addr, size)}, shared by programs
        self.inferred = placed_global.setdefault("_inferred", {})  # name -> version addr

    def free(self, a, n):
        if not (self.gbase <= a and a + n <= self.end): return False
        for c, d in self.busy:
            if c < a + n and a < d: return False
        return True

    def mod_of(self, pa):
        return "main0" if pa < OVL_BASE_PAL else self.mod

    def vaddr(self, pa):
        """Version address of a PAL address, or None: inside a placed data file, or a paired function."""
        m = self.mod_of(pa)
        for k, v in self.placed.items():
            if k != "_inferred" and k[0] == m and k[1] <= pa < k[1] + v[1]: return v[0] + pa - k[1]
        v = self.maps.pal.get((m, pa))
        return next(iter(v)) if v and len(v) == 1 else None

    def check(self, e, sec, a):
        """None (pointers good) or why not; plus the symbols inferred. Every relocation must give the version address
        of its symbol (see the module docstring)."""
        ours, ret = e.data(sec), self.game[a - self.gbase:a - self.gbase + e.size(sec)]
        syms, inf = {}, {}
        for off, ty, sym in e.relocs(sec):
            if ty != 2: return "relocation type %d" % ty, None, None
            t = (BF.W(ret, off) - BF.W(ours, off)) & 0xFFFFFFFF
            if sym["shndx"] == e.idx[sec]:
                if (t - (sym["value"] if sym["type"] != 3 else 0)) & 0xFFFFFFFF != a: return "self pointer", None, None
                continue
            if sym["shndx"] != 0: return "relocation against another section", None, None
            nm = sym["name"]
            if nm in syms:
                if syms[nm] != t: return "%s resolves twice" % nm, None, None
                continue
            syms[nm] = t
            vn = self.maps.names.get(nm)
            pa = self.maps.pal_name(self.mod, nm)
            exp = self.vaddr(pa) if pa is not None else None
            if vn and len(vn) == 1 and not NAMED.match(nm):
                if t not in vn: return "%s is %08x in the version, would be %08x" % (nm, next(iter(vn)), t), None, None
            elif exp is not None:
                if exp != t: return "%s (PAL %08x) is %08x in the version, would be %08x" % (nm, pa, exp, t), None, None
            elif nm in self.inferred:
                if self.inferred[nm] != t: return "%s inferred twice" % nm, None, None
            else:
                if not (0x80010000 <= t < 0x80200000): return "%s would be %08x" % (nm, t), None, None
                inf[nm] = t
        return None, syms, inf

    def run(self, files):
        """[(path, stem, PAL addr, size, obj, version addr, info)] of the files placed (info for report_data)."""
        out, todo = [], []
        cand = {}
        for f in files:
            path, stem, pa, size, obj = f
            e = BF.Elf(obj)
            secs = [s for s in BF.DATA_SECS if e.size(s)]
            if len(secs) != 1 or e.size(secs[0]) != size: continue
            sec = secs[0]
            b, rel = masked(e, sec)
            an = anchor(b, rel)
            hits = []
            if an:
                o, s = an
                i = self.game.find(s)
                while i >= 0 and len(hits) < 8:
                    a = self.gbase + i - o
                    if a % 4 == 0 and self.free(a, size):
                        r = self.game[a - self.gbase:a - self.gbase + size]
                        if len(r) == size and bytes(BF.mask_words(r, rel)) == b: hits.append(a)
                    i = self.game.find(s, i + 1)
            cand[stem] = (f, e, sec, hits)
        # pass 1: unique hits; then neighbours (several hits or no anchor), repeated while something gets placed
        for rnd in range(6):
            got = 0
            for stem, (f, e, sec, hits) in sorted(cand.items(), key=lambda kv: kv[1][0][2]):
                if stem in {o[1] for o in out}: continue
                path, _, pa, size, obj = f
                tries = hits if (rnd == 0 and len(hits) == 1) else []
                if rnd > 0:
                    near = self.neighbours(pa)
                    tries = [pa + d for d in near if (pa + d) in hits or (not hits and self.masked_ok(e, sec, pa + d))]
                for a in tries:
                    if not self.free(a, size) or any(c < a + size and a < d for c, d in self.taken(out)): continue
                    why, syms, inf = self.check(e, sec, a)
                    if why: continue
                    self.inferred.update(inf)
                    self.placed[(self.mod, pa)] = (a, size)
                    out.append((path, stem, pa, size, obj, a, dict(place={sec: a}, all_place={sec: a}, syms=syms),
                                len(inf)))
                    got += 1
                    break
            if rnd > 0 and not got: break
        return out

    def check_at(self, obj, a, size):
        """A version data file at its DATA address: (why not, None) or (None, info for report_data)."""
        e = BF.Elf(obj)
        secs = [s for s in e.idx if e.size(s) and e.sh[e.idx[s]][2] & 2 and s not in (".reginfo", ".pdr", ".MIPS.abiflags")]
        if len(secs) != 1 or secs[0] not in BF.DATA_SECS or e.sh[e.idx[secs[0]]][1] == 8:
            return "needs exactly one initialized data section, has %s" % secs, None
        sec = secs[0]
        if e.size(sec) != size: return "%s is %d bytes, DATA says %d" % (sec, e.size(sec), size), None
        if not self.free(a, size): return "overlaps code or data claimed by another unit", None
        if not self.masked_ok(e, sec, a): return "bytes differ from the game", None
        why, syms, inf = self.check(e, sec, a)
        if why: return why, None
        self.inferred.update(inf)
        return None, dict(place={sec: a}, all_place={sec: a}, syms=syms)

    def taken(self, out):
        return [(o[5], o[5] + o[3]) for o in out]

    def masked_ok(self, e, sec, a):
        n = e.size(sec)
        if not self.free(a, n): return False
        b, rel = masked(e, sec)
        r = self.game[a - self.gbase:a - self.gbase + n]
        return len(r) == n and bytes(BF.mask_words(r, rel)) == b

    def neighbours(self, pa):
        """PAL -> version shifts of the nearest placed files before and after PAL address pa (this module)."""
        mine = sorted((k[1], v[0] - k[1]) for k, v in self.placed.items() if k != "_inferred" and k[0] == self.mod)
        before = [d for a, d in mine if a < pa][-1:]
        after = [d for a, d in mine if a > pa][:1]
        return list(dict.fromkeys(before + after))


def _mask_words(buf, rel):
    b = bytearray(buf)
    for off in rel: struct.pack_into("<I", b, off, 0)
    return b


BF.mask_words = _mask_words


def prog_layout(ver, prog):
    """(game bytes, address of file offset 0, [(a, b)] excluded: PS-X EXE header, X00 prefix of the area overlays)."""
    y = open(os.path.join(ROOT, "config", ver, prog + ".yaml")).read()
    game = open(os.path.join(ROOT, re.search(r"target_path:\s*(\S+)", y).group(1)), "rb").read()
    m = re.search(r"start:\s*(0x[0-9A-Fa-f]+)\s*\n\s*vram:\s*(0x[0-9A-Fa-f]+)", y)
    gbase = int(m.group(2), 16) - int(m.group(1), 16)
    excl = []
    hdr = 0x800 if "type: header" in y else 0
    if hdr: excl.append((gbase, gbase + hdr))
    shared = VERSIONS[ver]["progs"].get(prog, (0, 0, 0, 0))[3]
    if shared: excl.append((gbase, gbase + shared))
    return game, gbase, excl


def wfm3_banks(game, gbase, a, b):
    """[(start, end)] of the WFM banks ("WFM3", the Japanese files "WFM2": glyph bitmaps + dialogue streams, assets)
    inside a..b of a program."""
    rx = re.compile(b"WFM[23]\0\0\0\0")
    out, lim = [], b - gbase
    m = rx.search(game, a - gbase, lim)
    while m:
        o = m.start()
        dpt, nd = struct.unpack_from("<IH", game, o + 8)
        end = o + dpt + 2 * nd
        if dpt and end <= lim:
            for k in range(nd):
                p = o + dpt + struct.unpack_from("<H", game, o + dpt + 2 * k)[0]
                while p + 2 <= lim and struct.unpack_from("<H", game, p)[0] not in (0xFFFE, 0xFFFF): p += 2
                end = max(end, min(p + 2, lim))
            end = min((end + 3) & ~3, lim)
            out.append((gbase + o, gbase + end))
            m = rx.search(game, end, lim)
        else:
            m = rx.search(game, o + 4, lim)
    return out


def assets(ver):
    """{prog: [(a, b)]} of config/<ver>/data_assets.txt (`<prog> <addr> <size>  # what`): graphics the generator
    never writes as C (ship scripts, not pixels), like the images PAL leaves out of src/data."""
    p = os.path.join(ROOT, "config", ver, "data_assets.txt")
    out = {}
    for l in open(p) if os.path.exists(p) else ():
        f = l.split("#")[0].split()
        if len(f) == 3: out.setdefault(f[0], []).append((int(f[1], 16), int(f[1], 16) + int(f[2], 0)))
    return out


def gen(ver, progs):
    """Writes src/data/<ver>/<prog>/D_<addr>.c for every range build/<ver>/data_rest.json leaves (the last report),
    the retail bytes as words like the PAL `unclaimed range` files (bytes where the range is not word aligned), minus
    the WFM3 banks and config/<ver>/data_assets.txt."""
    rest = json.load(open(os.path.join(ROOT, "build", ver, "data_rest.json")))
    ass = assets(ver)
    for prog in progs:
        if prog not in rest: continue
        game, gbase, excl = prog_layout(ver, prog)
        d = os.path.join(ROOT, "src", "data", ver, prog)
        n = nb = 0
        for a, b in rest[prog]:
            skip = [r for r in ass.get(prog, []) if r[0] < b and a < r[1]] + wfm3_banks(game, gbase, a, b)
            for c, e in gaps(a, b, skip):
                write_file(d, ver, prog, c, game[c - gbase:e - gbase])
                n += 1; nb += e - c
        print("%s %s: %d files, %d bytes" % (ver, prog, n, nb))


def write_file(d, ver, prog, a, data):
    os.makedirs(d, exist_ok=True)
    lines = ["// DATA %08x %d %s/%s" % (a, len(data), ver, prog),
             "// Data of %s %s that no C defines (yet): the bytes the report leaves (tools/version_data.py gen), as words"
             % (ver.upper(), prog.upper()),
             "// (bytes where the range is not word aligned). WFM3 banks and config/%s/data_assets.txt are left out." % ver, ""]
    if a % 4:
        lines += arr("unsigned char", "data_%08X" % a, ["0x%X" % x for x in data], 12)
    else:
        nw = len(data) // 4
        words = struct.unpack_from("<%dI" % nw, data)
        if nw: lines += arr("unsigned int", "data_%08X" % a, ["0x%X" % w for w in words], 4)
        if len(data) % 4:
            lines += arr("unsigned char", "data_%08X" % (a + 4 * nw), ["0x%X" % x for x in data[4 * nw:]], 12)
    open(os.path.join(d, "D_%08X.c" % a), "w").write("\n".join(lines))


def arr(ty, name, items, per):
    out = ["%s %s[%d] = {" % (ty, name, len(items))]
    for i in range(0, len(items), per):
        out.append("    " + ", ".join(items[i:i + per]) + ",")
    return out + ["};", ""]


def main():
    ver = sys.argv[sys.argv.index("--version") + 1]
    if "gen" in sys.argv:
        os.chdir(ROOT)
        progs = [p for p in VERSIONS[ver]["progs"] if os.path.exists("config/%s/%s.yaml" % (ver, p))]
        only = [a for a in sys.argv[sys.argv.index("gen") + 1:] if a in progs]
        return gen(ver, only or progs)
    os.chdir(ROOT)
    progs = [p for p in VERSIONS[ver]["progs"] if os.path.exists("config/%s/%s.yaml" % (ver, p))]
    mods = {p: pal_prog(ver, p) for p in progs}
    files = compile_pal(sorted(set(mods.values())), os.path.join(ROOT, "build", ver, "pal_data"))
    maps = Maps(ver)
    placed = {}
    # code ranges: the version's function symbols (an approximation of the report's units)
    tot = 0
    order = sorted(progs, key=lambda p: mods[p] != "main0")
    for p in order:
        game, gbase, excl = prog_layout(ver, p)
        code = []
        for l in open("config/%s/symbol_addrs_%s.txt" % (ver, p)):
            m = re.match(r"\w+ = 0x([0-9A-Fa-f]+); // type:func size:0x([0-9A-Fa-f]+)", l)
            if m: code.append((int(m.group(1), 16), int(m.group(1), 16) + int(m.group(2), 16)))
        pl = Placer(maps, mods[p], p, game, gbase, code + excl, placed)
        res = pl.run(files.get(mods[p], []))
        n = sum(r[3] for r in res)
        tot += n
        pal_n = sum(f[3] for f in files.get(mods[p], []))
        print("%-10s %-9s %3d/%3d files, %7d/%7d bytes, %d inferred symbols" % (
            p, mods[p], len(res), len(files.get(mods[p], [])), n, pal_n, sum(r[7] for r in res)))
    print("total", tot)


if __name__ == "__main__":
    main()
