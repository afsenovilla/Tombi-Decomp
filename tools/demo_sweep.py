#!/usr/bin/env python3
"""Automatic sweep for the demo / prototype builds (tools/versions.py: slpm80165, beta980203, pcpx96084): C that
already matches the same function in PAL or in the Japanese retail build (SLPS-01144), compiled for the version and
installed when it matches the version's bytes.

For every function the last report (build/<ver>/report.json, from tools/build_report.py --version <ver>) leaves
unmatched and that has no hand-written C for this version yet, the candidate sources are, in this order:
  - the shared PAL file of its PAL counterpart (`PAL <module> <addr>` of config/<ver>/symbol_addrs_<prog>.txt and
    config/<ver>/reuse.txt): its PAL body and the branches it carries for the other versions (SLPS_01144 first);
  - the JP-only twins of the same PAL function (src/[x0nn/]<name>_slps01144.c, found through config/slps01144/
    symbol_addrs_*.txt and the `// VERSION SLPS_01144` lines) and, by name, the PAL and JP functions called like it;
  - for a function without a PAL pair (splat's func_<addr>, most of the unmatched code) or whose pair has no C, the
    PAL C functions and the JP retail functions with version C of the same size whose masked words (tools/xmatch.py:
    relocations and non-stack immediates ignored) are at least --min (0.9) similar, best three.
Each only when its size equals the version function's size. Every candidate body is compiled (tools/ncheck.py chain
with -DVERSION_<SERIAL>, the PAL scratchpad names as constants, like build_report) with its file's own flags and the
common variants (`-O2 -G0`, `-O2 -G0 -fno-strength-reduce`) and compared strictly against the splat target object
(tools/jpcheck.py dump: relocation addends ignored like objdiff, branch targets kept). The first match is installed
the way the repo shares C between versions (README "How the versions share code", tools/version_share.py):
  - the PAL body with the PAL file's own flags: only a pairing: a symbol line `<name> = 0x<addr>; // type:func
    size:0x<size>  PAL <module> 0x<addr>` in config/<ver>/symbol_addrs_<prog>.txt (PAL name when free in that
    program, else func_<addr>_<ver>) or, for a function the table already names, a config/<ver>/reuse.txt line;
  - the PAL body with other flags: a `// VERSION <SERIAL> <prog> <addr> <size>` line and a `// FLAGS.<SERIAL>` line
    in the PAL file (no branch), when no other function of the version is built from that file;
  - a body from elsewhere: a `#if defined(VERSION_<SERIAL>)` branch of the PAL file (replacing its INCLUDE_ASM_V
    branch, keeping the PAL function name), when no other function of the version is built from that file and the
    file has no branch for the version yet;
  - otherwise src/[x0nn/]<name>_<ver>.c, whole body guarded, function named after the version symbol.
Idempotent: an installed function is skipped on the next run (it has version C or a pairing), whether or not the
report was rebuilt; a rebuilt report (--report) is what confirms the figures.

  python3 tools/demo_sweep.py [--version slpm80165[,beta980203,...]] [--dry-run] [--report] [--only ADDR[,ADDR]]
                              [--limit N] [--jobs N] [--min 0.9] [--fix [--fix-min 0.8]] [--show]
  --version   versions to sweep (default: every demo version with a report; one without game files or report is skipped)
  --dry-run   print the matches, install nothing
  --report    rebuild build/<ver>/report.json after installing (tools/build_report.py --version <ver>)
  --fix       slow second pass: the best near candidate of each function still unmatched (another size allowed, within
              15 %, masked ratio >= --fix-min) goes through the tools/jp_autofix.py edits (literals shifted by the
              immediates of the diff, pad bits, a statement moved or deleted), kept when the diff shrinks, installed
              on a strict match; minutes per function, so not for CI
  --show      print the diff line count of the best failed candidate per function
Prints `MATCH <ver> <prog> <addr> <size> <name> <- <source> [<flags>]` per match, the install, and a summary per
version. Exit status 0 always (CI use); the summary says what was installed."""
import difflib, hashlib, json, os, re, shutil, struct, subprocess, sys, tempfile
from multiprocessing import Pool

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
os.chdir(ROOT)
from versions import VERSIONS, canon, pal_src_dir
import version_share as VS
import build_report as BR
import units as U
import jpcheck
from xmatch import mask

DEMO_VERSIONS = ["slpm80165", "beta980203", "pcpx96084"]
LABEL = {"slpm80165": "Demo", "beta980203": "Beta", "pcpx96084": "PrePre 8"}
FLAG_VARIANTS = ["-O2 -G0", "-O2 -G0 -fno-strength-reduce"]
JP = "slps01144"
SYM_RX = re.compile(r"(\w+) = 0x([0-9A-Fa-f]+); // type:func size:0x([0-9A-Fa-f]+)(?:\s+PAL (\w+) 0x([0-9A-Fa-f]+))?")
TOP = 3  # masked-similarity candidates per function
FIX_SIZE = 0.15  # --fix: size tolerance of the near candidates (fraction)


def progs_of(ver):
    return [p for p in VERSIONS[ver]["progs"] if os.path.exists("config/%s/%s.yaml" % (ver, p))]


def sym_table(ver):
    """{(prog, addr): (name, size, PAL module or None, PAL addr or None)} of config/<ver>/symbol_addrs_<prog>.txt,
    with config/<ver>/reuse.txt pairs applied (like build_report.jpdemo_sources)."""
    out = {}
    for prog in progs_of(ver):
        p = "config/%s/symbol_addrs_%s.txt" % (ver, prog)
        if not os.path.exists(p): continue
        for l in open(p):
            m = SYM_RX.match(l)
            if m:
                out[(prog, int(m.group(2), 16))] = (m.group(1), int(m.group(3), 16), m.group(4) and m.group(4).lower(),
                                                   m.group(5) and int(m.group(5), 16))
    rp = "config/%s/reuse.txt" % ver
    if os.path.exists(rp):
        for l in open(rp):
            f = l.split("#")[0].split()
            if not f: continue
            k = (f[0], int(f[1], 16))
            old = out.get(k)
            if len(f) == 4: out[k] = ((old or ("func_%s_%s" % (f[1], ver), 0))[0], (old or (0, 0))[1], f[2].lower(), int(f[3], 16))
            elif f[2] == "-" and old: out[k] = (old[0], old[1], None, None)
    return out


def unmatched(ver, table):
    """{(prog, addr): (name, size)} of the functions the report leaves below 100 %."""
    rep = json.load(open("build/%s/report.json" % ver))
    by_name = {(p, n): (p, a) for (p, a), (n, _, _, _) in table.items()}
    out = {}
    for un in rep["units"]:
        prog = un["name"].split("/")[0]
        for f in un.get("functions", []):
            if float(f.get("fuzzy_match_percent") or 0) >= 100: continue
            k = by_name.get((prog, f["name"]))
            if k is None:
                m = re.match(r"func_([0-9A-F]{8})$", f["name"])
                if m: k = (prog, int(m.group(1), 16))
            if k: out[k] = (f["name"], int(f.get("size") or 0))
    return out


def image(ver, prog):
    """(bytes, address of file offset 0) of a version program, from config/<ver>/<prog>.yaml; None without the file."""
    p = "config/%s/%s.yaml" % (ver, prog)
    if not os.path.exists(p): return None
    y = open(p).read()
    tp = re.search(r"target_path:\s*(\S+)", y).group(1)
    m = re.search(r"start:\s*(0x[0-9A-Fa-f]+)\s*\n\s*vram:\s*(0x[0-9A-Fa-f]+)", y)
    if not m or not os.path.exists(tp): return None
    return open(tp, "rb").read(), int(m.group(2), 16) - int(m.group(1), 16)


def mwords(img, a, size):
    """Masked words of a function (tools/xmatch.py mask), or None outside the image."""
    if img is None or size < 4: return None
    o, size = a - img[1], size & ~3
    if o < 0 or o + size > len(img[0]): return None
    return tuple(mask(w) for w in struct.unpack("<%dI" % (size // 4), img[0][o:o + size]))


def file_dirs(hdr, serial=None):
    """FLAGS / CC / HEADERS / AS of a header, with the `// D.<serial>` lines of one version applied."""
    d = VS.directives(hdr)
    if serial:
        for k in VS.DIRECTIVES:
            for l in hdr:
                m = re.match(r"//\s*%s\.%s\s+(.*)$" % (k, serial), l)
                if m:
                    v = m.group(1).strip()
                    if v == "default": d.pop(k, None)
                    else: d[k] = v
    return d


def unwrap(t, ser):
    """A branch body wrapped once more in `#if defined(VERSION_<ser>)` ... `#endif` (older folds), without the wrap."""
    L = t.strip("\n").split("\n")
    if len(L) > 2 and L[0].strip() == "#if defined(VERSION_%s)" % ser and L[-1].strip() == "#endif":
        return "\n".join(L[1:-1])
    return t


def version_sizes(hdr):
    """{serial: size} of the `// VERSION <serial> <prog> <addr> <size>` lines of a header."""
    out = {}
    for l in hdr:
        m = re.match(r"//\s*VERSION\s+(\w+)\s+\w+\s+[0-9A-Fa-f]{8}\s+(\d+)", l)
        if m: out[m.group(1)] = int(m.group(2))
    return out


class Sweep:
    def __init__(self, ver, minr=0.9):
        self.ver, self.serial, self.minr = ver, VERSIONS[ver]["serial"], minr
        self.progs = progs_of(ver)
        self.table = sym_table(ver)
        self.sources = BR.jpdemo_sources(self.progs, ver)  # (prog, addr) -> (path, name)
        self.vlines = BR.version_lines(self.serial)       # (prog, addr) -> (path, size)
        self.pal = {}       # (PAL module, addr) -> Unit with C
        self.pal_name = {}  # name -> [Unit]
        for p, us in U.units().items():
            for u in us:
                if not u.file: continue
                self.pal[(p.lower(), u.addr)] = u
                self.pal_name.setdefault(u.name, []).append(u)
        # JP retail functions: by PAL pair and by name, with the file that builds them (a `// VERSION SLPS_01144` line)
        self.jp = sym_table(JP) if os.path.isdir("config/" + JP) else {}
        self.jp_lines = BR.version_lines(VERSIONS[JP]["serial"]) if self.jp else {}
        self.jp_by_pair, self.jp_by_name, self.jp_by_jname = {}, {}, {}
        for k, (n, s, mod, a) in self.jp.items():
            self.jp_by_jname[n] = (k, mod, a)
            src = self.jp_lines.get(k)
            if not src: continue
            row = (src[0], n, s, mod, a)
            if mod: self.jp_by_pair.setdefault((mod, a), []).append(row)
            self.jp_by_name.setdefault(n, []).append(row)
        # the version's own functions by PAL pair (to map a JP-only callee to its twin here)
        self.by_pair = {(mod, a): (k, n) for k, (n, s, mod, a) in self.table.items() if mod}
        self.cache = {}
        self.pools = None
        self.images = {}

    def read(self, path):
        if path not in self.cache:
            hdr, rest = VS.split_header(open(path, errors="replace").read())
            branches, pal = VS.parse_branches(rest)
            self.cache[path] = (hdr, rest, branches, pal)
        return self.cache[path]

    # ---- masked-word pools: PAL C functions and JP retail functions with version C, by size ----
    def load_pools(self):
        if self.pools is not None: return
        self.pools = {}
        imgs = {}
        for (mod, a), u in self.pal.items():
            if mod not in imgs:
                P = mod.upper()
                imgs[mod] = (open(U.BINS[P][0], "rb").read(), U.BINS[P][1]) if P in U.BINS and os.path.exists(U.BINS[P][0]) else None
            w = mwords(imgs[mod], a, u.size)
            if w: self.pools.setdefault(u.size, []).append(("pal", u, w))
        for k, (n, s, mod, a) in self.jp.items():
            if k not in self.jp_lines: continue
            if k[0] not in imgs: imgs[k[0]] = image(JP, k[0])
            w = mwords(imgs[k[0]], k[1], s)
            if w: self.pools.setdefault(s, []).append(("jp", (self.jp_lines[k][0], n, s, mod, a), w))

    def similar(self, prog, addr, size, near=False, minr=None, top=TOP):
        """[(ratio, kind, row)] of the pool functions of the same size (near: within +-FIX_SIZE), best `top` above
        minr (default self.minr)."""
        self.load_pools()
        minr = self.minr if minr is None else minr
        if prog not in self.images: self.images[prog] = image(self.ver, prog)
        w = mwords(self.images[prog], addr, size)
        if not w: return []
        sm = difflib.SequenceMatcher(None, autojunk=False)
        sm.set_seq2(list(w))
        sizes = range(int(size * (1 - FIX_SIZE)) & ~3, int(size * (1 + FIX_SIZE)) + 4, 4) if near else [size]
        out = []
        for s2 in sizes:
            for kind, row, pw in self.pools.get(s2, []):
                sm.set_seq1(list(pw))
                if sm.real_quick_ratio() < minr or sm.quick_ratio() < minr: continue
                r = sm.ratio()
                if r >= minr: out.append((r, kind, row))
        out.sort(key=lambda x: -x[0])
        return out[:top]

    def map_jp_names(self, body):
        """JP-only names of a JP body mapped to this version's twins; None when one has no twin here."""
        for n in sorted(set(re.findall(r"\b\w+_%s\b" % JP, body))):
            row = self.jp_by_jname.get(n)
            if not row or not row[1] or (row[1], row[2]) not in self.by_pair: return None
            body = re.sub(r"\b%s\b" % re.escape(n), self.by_pair[(row[1], row[2])][1], body)
        if re.search(r"\b\w+_%s\b" % JP, body): return None
        return body

    def candidates(self, key, name, size, mod, addr, rows=None, exact=True):
        """[{body, dirs, flags, origin, palfile, palname, palpair, kind}] for one function, deduplicated by body.
        rows: only these (ratio, kind, row) similarity rows; exact=False keeps bodies of another size (--fix)."""
        out, seen, ctx = [], set(), {"ratio": None}  # ratio: masked similarity of the row (None: symbol-table pair)

        def add(body, dirs, origin, palfile, palname, kind, palpair=None):
            body = body.strip("\n")
            if not body or "INCLUDE_ASM" in body or "#if defined(VERSION_" in body: return
            if "_%s" % JP in body:
                body = self.map_jp_names(body)
                if body is None: return
            h = hashlib.sha1(body.encode()).hexdigest()
            if h in seen: return
            seen.add(h)
            own = dirs.get("FLAGS", "-O2 -G0")
            flags = [own] + [f for f in FLAG_VARIANTS if f != own]
            out.append({"body": body, "dirs": dirs, "flags": flags, "origin": origin, "palfile": palfile, "palname": palname,
                        "kind": kind, "palpair": palpair, "own": own, "ratio": ctx["ratio"]})

        def pal_file(u, how=""):
            hdr, rest, branches, pal = self.read(u.file)
            rel, pair = os.path.relpath(u.file, ROOT), (u.prog.lower(), u.addr)
            if u.size == size or not exact:
                if pal is not None: add(pal, file_dirs(hdr), "PAL C of %s%s" % (rel, how), u.file, u.name, "pal", pair)
                elif not branches: add(rest, file_dirs(hdr), "PAL C of %s%s" % (rel, how), u.file, u.name, "pal", pair)
            vs = version_sizes(hdr)
            for ser, t in sorted(branches, key=lambda b: b[0] != VERSIONS[JP]["serial"]):
                if ser == self.serial or (exact and vs.get(ser, size) != size): continue
                add(unwrap(t, ser), file_dirs(hdr, ser), "%s branch of %s%s" % (ser, rel, how), u.file, u.name, "branch", pair)

        def jp_rows(rows, how=""):
            for path, jname, jsize, jmod, jaddr in rows:
                if exact and jsize != size: continue
                u2 = self.pal.get((jmod, jaddr)) if jmod else None
                palname = u2.name if u2 else None
                hdr, rest, branches, pal = self.read(path)
                for ser, t in branches:
                    if ser != VERSIONS[JP]["serial"]: continue
                    t = unwrap(t, ser)
                    if jname not in (palname, name): t = re.sub(r"\b%s\b" % re.escape(jname), palname or name, t)
                    add(t, file_dirs(hdr, ser), "JP C of %s (%s)%s" % (os.path.relpath(path, ROOT), jname, how),
                        u2.file if u2 else None, palname, "jp", (jmod, jaddr) if u2 else None)

        if rows is None:
            u = self.pal.get((mod, addr)) if mod else None
            if u: pal_file(u)
            if mod: jp_rows(self.jp_by_pair.get((mod, addr), []))
            if not re.match(r"func_[0-9A-F]{8}", name):
                for u2 in self.pal_name.get(name, []):
                    if u2 is not u: pal_file(u2)
                jp_rows(self.jp_by_name.get(name, []))
            if out and u and u.size == size: return out
            rows = self.similar(key[0], key[1], size)  # no usable pair: the functions that look like it
        for r, kind, row in rows:
            how, ctx["ratio"] = " (%.3f masked)" % r, r
            if kind == "pal": pal_file(row, how)
            else: jp_rows([row], how)
        return out

    # ---- install ----
    def users(self, path, key):
        return [k for k, (p, _) in self.sources.items() if os.path.abspath(p) == os.path.abspath(path) and k != key]

    def has_version_c(self, key):
        """True when a `// VERSION` line already builds this function here (hand C, or a plain PAL body with its own
        flags); an INCLUDE_ASM branch does not count."""
        if key not in self.vlines: return False
        hdr, rest, branches, pal = self.read(self.vlines[key][0])
        mine = [t for s, t in branches if s == self.serial]
        return not mine or "INCLUDE_ASM" not in mine[0]

    def install(self, key, name, size, cand, flags):
        """Returns (path, kind), kind in pairing / header-only / branch / version-only."""
        prog, addr = key
        label = LABEL.get(self.ver, self.serial)
        if key not in self.table:  # splat's func_<addr>: name it and pair it in the symbol table
            name = self.add_symbol(key, size, cand["palpair"], cand["palname"], cand["ratio"])
        if cand["palpair"]:
            note = "// %s version of PAL %s %08X (%s): %s [%s] (tools/demo_sweep.py)" % (
                label, cand["palpair"][0], cand["palpair"][1], os.path.basename(cand["palfile"]), cand["origin"], flags)
        else:
            note = "// %s version: %s [%s] (tools/demo_sweep.py)" % (label, cand["origin"], flags)
        dirs = dict(cand["dirs"]); dirs["FLAGS"] = flags
        palfile = cand["palfile"]
        if palfile and cand["kind"] == "pal" and flags == cand["own"]:
            return self.pair(key, name, size, cand["palpair"]), "pairing"
        if palfile and not self.users(palfile, key):
            hdr, rest, branches, pal = self.read(palfile)
            mine = [t for s, t in branches if s == self.serial]
            free = not mine or "INCLUDE_ASM" in mine[0]
            if free and cand["kind"] == "pal":
                self._header_only(palfile, key, size, dirs, hdr, rest, branches, pal)
                return palfile, "header-only"
            if free and (pal is not None or not branches):
                self._branch(palfile, key, size, dirs, hdr, rest, branches, pal, note + "\n" + cand["body"])
                return palfile, "branch"
        return self._version_only(key, name, size, dirs, cand["body"], cand["palname"], note), "version-only"

    def sym_path(self, prog):
        return "config/%s/symbol_addrs_%s.txt" % (self.ver, prog)

    def add_symbol(self, key, size, palpair, palname, ratio=None):
        """Names a splat func_<addr> in the version's symbol table with its PAL pair; returns the name. The PAL name
        when that program does not use it yet and the pair is sure (symbol-table pair, or identical masked words of
        at least 16 instructions: a tiny getter matches any other), else func_<addr>_<ver>."""
        prog, addr = key
        used = {n for (p, a), (n, _, _, _) in self.table.items() if p == prog}
        sure = ratio is None or (ratio >= 1.0 and size >= 64)
        name = palname if palname and palname not in used and sure else "func_%08X_%s" % (addr, self.ver)
        line = "%s = 0x%08X; // type:func size:0x%X%s\n" % (name, addr, size, "  PAL %s 0x%08X" % (palpair[0], palpair[1]) if palpair else "")
        p = self.sym_path(prog)
        lines = open(p).readlines() if os.path.exists(p) else []
        at = len(lines)
        for i, l in enumerate(lines):
            m = re.match(r"\w+ = 0x([0-9A-Fa-f]+);", l)
            if m and int(m.group(1), 16) > addr: at = i; break
        lines.insert(at, line)
        open(p, "w").write("".join(lines))
        self.table[key] = (name, size, palpair[0] if palpair else None, palpair[1] if palpair else None)
        if palpair: self.by_pair[palpair] = (key, name)
        return name

    def pair(self, key, name, size, palpair):
        """The PAL file as is builds this function: the symbol table pair (just added for a func_<addr>) or, for a
        function the table already names with another pair, a config/<ver>/reuse.txt line."""
        prog, addr = key
        cur = self.table[key]
        p = self.sym_path(prog)
        if (cur[2], cur[3]) != palpair:
            rp = "config/%s/reuse.txt" % self.ver
            with open(rp, "a") as f:
                f.write("%s %08X %s %08X  # %s (tools/demo_sweep.py)\n" % (prog, addr, palpair[0], palpair[1], name))
            self.table[key] = (name, size, palpair[0], palpair[1])
            p = rp
        self.sources[key] = (self.pal[palpair].file, name)
        return p

    def _vline(self, key, size, hdr):
        vline = "// VERSION %s %s %08X %d" % (self.serial, key[0], key[1], size)
        if vline not in hdr:
            hdr.insert(max([i + 1 for i, l in enumerate(hdr) if re.match(r"//\s*(FUNC|MATCHING|VERSION)\s", l)] or [len(hdr)]), vline)

    def _dir_lines(self, hdr, dirs, pd):
        """`// D.<serial> value` lines for the directives that differ from the file's PAL ones."""
        hdr = [l for l in hdr if not re.match(r"//\s*(%s)\.%s\s" % ("|".join(VS.DIRECTIVES), self.serial), l)]
        eff = dict(pd); eff.setdefault("FLAGS", "-O2 -G0")
        want = dict(dirs); want.setdefault("FLAGS", "-O2 -G0")
        for d in VS.DIRECTIVES:
            if want.get(d) != eff.get(d): hdr.append("// %s.%s %s" % (d, self.serial, want.get(d, "default")))
        return hdr

    def _header_only(self, path, key, size, dirs, hdr, rest, branches, pal):
        hdr = self._dir_lines(hdr, dirs, VS.directives(hdr))
        self._vline(key, size, hdr)
        branches = [(s, t) for s, t in branches if s != self.serial]
        body = VS.join_branches(branches, pal) if branches else (pal if pal is not None else rest).strip("\n") + "\n"
        open(path, "w").write("\n".join(hdr) + "\n" + body)
        self.cache.pop(path, None); self.sources[key] = (path, None); self.vlines[key] = (path, size)

    def _branch(self, path, key, size, dirs, hdr, rest, branches, pal, body):
        if pal is None: pal = rest  # a plain PAL file: its whole body becomes the #else branch
        hdr = self._dir_lines(hdr, dirs, VS.directives(hdr))
        self._vline(key, size, hdr)
        branches = [(s, t) for s, t in branches if s != self.serial] + [(self.serial, body)]
        open(path, "w").write("\n".join(hdr) + "\n" + VS.join_branches(branches, pal))
        self.cache.pop(path, None); self.sources[key] = (path, None); self.vlines[key] = (path, size)

    def _version_only(self, key, name, size, dirs, body, palname, note):
        prog, addr = key
        if palname and palname != name: body = re.sub(r"\b%s\b" % re.escape(palname), name, body)
        body = note + "\n" + VS.suffix_names(body, self.ver)
        fname = name if name.endswith("_" + self.ver) else name + "_" + self.ver
        d = pal_src_dir(self.ver, prog)
        path = os.path.join(d, fname + ".c")
        if os.path.exists(path): path = os.path.join(d, "%s_%08X.c" % (fname, addr))
        hdr = ["// VERSION %s %s %08X %d" % (self.serial, prog, addr, size)]
        want = dict(dirs)
        if want.get("FLAGS") == "-O2 -G0": want.pop("FLAGS")
        hdr += ["// %s.%s %s" % (k, self.serial, v) for k, v in sorted(want.items())]
        open(path, "w").write("\n".join(hdr) + "\n" + VS.join_branches([(self.serial, body)], None))
        self.sources[key] = (path, None); self.vlines[key] = (path, size)
        return path


# ---- compile + compare (worker processes) ----
_W = {}


def _init(inc):
    s = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
    NC = {"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"}
    exec(compile(s, "ncheck", "exec"), NC)
    _W["nc"], _W["inc"] = NC, inc


def _dump(o):
    jpcheck.STRICT = True
    try: return jpcheck.dump(o, True)
    finally: jpcheck.STRICT = False


def _same(a, b):
    if len(a) != len(b): return False
    for x, y in zip(a, b):
        if x.endswith("RELOC") or y.endswith("RELOC"):
            x = re.sub(r"^(j|b\w*)\t(.*?)\b(?:[0-9a-f]+|RELOC)$", r"\1\t\2", x)
            y = re.sub(r"^(j|b\w*)\t(.*?)\b(?:[0-9a-f]+|RELOC)$", r"\1\t\2", y)
        if x != y: return False
    return True


def _job(job):
    """(index, ok, detail): compiles one candidate body with one flag set and compares it with the target object."""
    i, serial, body, dirs, flags, target = job
    NC = _W["nc"]
    NC["EXTRA_CPP"] = ["-DVERSION_" + serial, "-DSKIP_ASM"]
    d = tempfile.mkdtemp()
    try:
        text = "// FLAGS %s\n" % flags + "".join("// %s %s\n" % (k, v) for k, v in dirs.items() if k != "FLAGS") + body + "\n"
        src = os.path.join(d, "cand.c")
        open(src, "w").write(text)
        NC["EXTRA_AS"] = BR._jpdemo_spad_set(text, serial)
        NC["build"](src, flags, d, _W["inc"])
        a, b = _dump(d + "/a.o"), _dump(target)
        if _same(a, b): return i, True, 0
        return i, False, sum(1 for x, y in zip(a, b) if x != y) + abs(len(a) - len(b))
    except subprocess.CalledProcessError as e:
        return i, False, "ERROR " + (e.stderr.decode() if isinstance(e.stderr, bytes) else str(e.stderr))[-120:].replace("\n", " ")
    except Exception as e:
        return i, False, "ERROR %s" % e
    finally:
        shutil.rmtree(d, ignore_errors=True)


def _check_file(path, serial, target, strict=False):
    """jpcheck-style result for a version source file: IDENTICAL, a unified diff (ours vs target), or ERROR."""
    NC = _W["nc"]
    NC["EXTRA_CPP"] = ["-DVERSION_" + serial, "-DSKIP_ASM"]
    t = open(path, errors="replace").read()
    fm = re.search(r"//\s*FLAGS\s+(.*)", t)
    d = tempfile.mkdtemp()
    try:
        NC["EXTRA_AS"] = BR._jpdemo_spad_set(t, serial)
        NC["build"](path, fm.group(1).strip() if fm else "-O2 -G0", d, _W["inc"])
        jpcheck.STRICT = strict
        a, b = jpcheck.dump(d + "/a.o", True), jpcheck.dump(target, True)
        if strict:
            for i in range(min(len(a), len(b))):
                if a[i].endswith("RELOC") or b[i].endswith("RELOC"):
                    a[i] = re.sub(r"^(j|b\w*)\t(.*?)\b(?:[0-9a-f]+|RELOC)$", r"\1\t\2", a[i])
                    b[i] = re.sub(r"^(j|b\w*)\t(.*?)\b(?:[0-9a-f]+|RELOC)$", r"\1\t\2", b[i])
        return "\n".join(difflib.unified_diff(a, b, "ours", "retail", lineterm="", n=2)) or "IDENTICAL"
    except subprocess.CalledProcessError as e:
        return "ERROR " + (e.stderr.decode() if isinstance(e.stderr, bytes) else str(e.stderr))[-120:].replace("\n", " ")
    except Exception as e:
        return "ERROR %s" % e
    finally:
        jpcheck.STRICT = False
        shutil.rmtree(d, ignore_errors=True)


def _fix_job(job):
    """(index, ok, text, diff lines): tools/jp_autofix.py edits (literals shifted, pad bits, statements moved or
    deleted) on a near candidate until it matches the target, with the checker of this version."""
    i, serial, text, prog, target = job
    import jp_autofix as JA
    JA.check = lambda path, prog, strict=False: _check_file(path, serial, target, strict)
    d = tempfile.mkdtemp()
    try:
        path = os.path.join(d, "fix.c")
        text, r = JA.try_fix(path, prog, text)
        if r == "IDENTICAL":
            open(path, "w").write(text)
            r = _check_file(path, serial, target, True)
        return i, r == "IDENTICAL", text, 0 if r == "IDENTICAL" else (len(JA.diff_lines(r)) if not r.startswith("ERROR") else r[:100])
    except Exception as e:
        return i, False, text, "ERROR %s" % e
    finally:
        shutil.rmtree(d, ignore_errors=True)


def fix_text(serial, key, size, cand):
    """The standalone version source tools/jp_autofix.py edits: VERSION header, directives, guarded body."""
    hdr = ["// VERSION %s %s %08X %d" % (serial, key[0], key[1], size), "// FLAGS " + cand["own"]]
    hdr += ["// %s %s" % (k, v) for k, v in sorted(cand["dirs"].items()) if k != "FLAGS"]
    return "\n".join(hdr + ["#if defined(VERSION_%s)" % serial, cand["body"], "#endif", ""])


def include_dir():
    inc = tempfile.mkdtemp()
    for f in os.listdir("include"):
        s = os.path.join("include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(inc, nm))
    return inc


def sweep(ver, install=True, only=None, limit=None, jobs=None, show=False, minr=0.9, fix=False, fixmin=0.8):
    if not os.path.exists("build/%s/report.json" % ver):
        print("%s: no build/%s/report.json (python3 tools/build_report.py --version %s), skipped" % (ver, ver, ver))
        return None
    S = Sweep(ver, minr)
    um = unmatched(ver, S.table)
    work, skipped_c, no_cand, no_target = [], 0, 0, 0
    for key in sorted(um):
        if only and key[1] not in only: continue
        uname, usize = um[key]
        name, size, mod, addr = S.table.get(key, (uname, usize, None, None))
        target = "build/%s/target/%s/%s.o" % (ver, key[0], uname)
        if not os.path.exists(target) or not size: no_target += 1; continue
        if S.has_version_c(key): skipped_c += 1; continue
        cands = S.candidates(key, name, size, mod, addr)
        if not cands: no_cand += 1; continue
        work.append((key, name, size, cands, target))
        if limit and len(work) >= limit: break
    jl = []
    for wi, (key, name, size, cands, target) in enumerate(work):
        for ci, c in enumerate(cands):
            for fi, f in enumerate(c["flags"]):
                jl.append(((wi, ci, fi), S.serial, c["body"], c["dirs"], f, target))
    print("%s: %d unmatched functions, %d with candidates (%d compile jobs), %d with hand C, %d without a candidate, %d without target"
          % (ver, len(um), len(work), len(jl), skipped_c, no_cand, no_target), flush=True)
    inc = include_dir()
    res = {}
    try:
        with Pool(jobs or os.cpu_count() or 4, initializer=_init, initargs=(inc,)) as pool:
            for i, ok, det in pool.imap_unordered(_job, jl, chunksize=4):
                res[i] = (ok, det)
    finally:
        shutil.rmtree(inc, ignore_errors=True)
    matched, installed, kinds = [], [], {}
    for wi, (key, name, size, cands, target) in enumerate(work):
        hit = None
        for ci, c in enumerate(cands):
            for fi, f in enumerate(c["flags"]):
                if res.get((wi, ci, fi), (False, None))[0]: hit = (c, f); break
            if hit: break
        if not hit:
            if show:
                best = min((r[1] for k, r in res.items() if k[0] == wi and not isinstance(r[1], str)), default=None)
                errs = [r[1] for k, r in res.items() if k[0] == wi and isinstance(r[1], str)]
                print("DIFF  %s %s %08X %5d %-40s best %s%s" % (ver, key[0], key[1], size, name, best, (" " + errs[0]) if best is None and errs else ""))
            continue
        c, f = hit
        matched.append(key)
        print("MATCH %s %s %08X %5d %-40s <- %s [%s]" % (ver, key[0], key[1], size, name, c["origin"], f), flush=True)
        if install:
            p, kind = S.install(key, name, size, c, f)
            kinds[kind] = kinds.get(kind, 0) + 1
            installed.append((key, name, p, kind))
            print("      installed (%s): %s" % (kind, os.path.relpath(p, ROOT)), flush=True)
    print("%s: matched %d of %d tried; installed %d%s" % (ver, len(matched), len(work), len(installed),
          (" (" + ", ".join("%d %s" % (n, k) for k, n in sorted(kinds.items())) + ")") if kinds else ""), flush=True)
    nfix = 0
    if fix:
        # near candidates (another size, masked ratio >= fixmin) edited by tools/jp_autofix.py until they match
        fw = []
        for key in sorted(um):
            if only and key[1] not in only: continue
            if key in matched or S.has_version_c(key): continue
            uname, usize = um[key]
            name, size, mod, addr = S.table.get(key, (uname, usize, None, None))
            target = "build/%s/target/%s/%s.o" % (ver, key[0], uname)
            if not os.path.exists(target) or size < 24: continue
            rows = S.similar(key[0], key[1], size, near=True, minr=fixmin, top=1)
            if not rows: continue
            cands = S.candidates(key, name, size, mod, addr, rows=rows, exact=False)
            if not cands: continue
            fw.append((key, name, size, cands[0], target))
            if limit and len(fw) >= limit: break
        jl = [(wi, S.serial, fix_text(S.serial, key, size, c), key[0], target) for wi, (key, name, size, c, target) in enumerate(fw)]
        print("%s: --fix: %d near candidates (masked ratio >= %.2f, size within %d %%)" % (ver, len(jl), fixmin, FIX_SIZE * 100), flush=True)
        inc = include_dir()
        try:
            with Pool(jobs or os.cpu_count() or 4, initializer=_init, initargs=(inc,)) as pool:
                for wi, ok, text, det in pool.imap_unordered(_fix_job, jl):
                    key, name, size, c, target = fw[wi]
                    if not ok:
                        if show: print("DIFF  %s %s %08X %5d %-40s fix of %s: %s" % (ver, key[0], key[1], size, name, c["origin"], det), flush=True)
                        continue
                    body = text.split("#if defined(VERSION_%s)\n" % S.serial, 1)[1].rsplit("#endif", 1)[0]
                    c = dict(c, body=body.strip("\n"), kind="fix", origin=c["origin"] + ", edited (tools/jp_autofix.py edits)")
                    matched.append(key); nfix += 1
                    print("MATCH %s %s %08X %5d %-40s <- %s [%s]" % (ver, key[0], key[1], size, name, c["origin"], c["own"]), flush=True)
                    if install:
                        p, kind = S.install(key, name, size, c, c["own"])
                        kinds[kind] = kinds.get(kind, 0) + 1
                        installed.append((key, name, p, kind))
                        print("      installed (%s): %s" % (kind, os.path.relpath(p, ROOT)), flush=True)
        finally:
            shutil.rmtree(inc, ignore_errors=True)
        print("%s: --fix matched %d of %d; installed %d in all%s" % (ver, nfix, len(jl), len(installed),
              (" (" + ", ".join("%d %s" % (n, k) for k, n in sorted(kinds.items())) + ")") if kinds else ""), flush=True)
    return {"version": ver, "unmatched": len(um), "tried": len(work), "matched": len(matched), "installed": installed, "fixed": nfix}


def report(ver):
    r = subprocess.run([sys.executable, "tools/build_report.py", "--version", ver], capture_output=True, text=True)
    if r.returncode:
        print("%s: build_report failed:\n%s" % (ver, (r.stdout + r.stderr)[-2000:])); return None
    m = json.load(open("build/%s/report.json" % ver))["measures"]
    print("%s: report %.2f %% of code (%s of %s bytes), %d of %d functions" % (
        ver, float(m["matched_code_percent"]), m["matched_code"], m["total_code"], int(m["matched_functions"]), int(m["total_functions"])))
    return m


def main():
    argv = sys.argv[1:]
    vers = [canon(v) for v in argv[argv.index("--version") + 1].split(",")] if "--version" in argv else \
        [v for v in DEMO_VERSIONS if os.path.exists("build/%s/report.json" % v)]
    only = set(int(x, 16) for x in argv[argv.index("--only") + 1].split(",")) if "--only" in argv else None
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    jobs = int(argv[argv.index("--jobs") + 1]) if "--jobs" in argv else None
    minr = float(argv[argv.index("--min") + 1]) if "--min" in argv else 0.9
    fixmin = float(argv[argv.index("--fix-min") + 1]) if "--fix-min" in argv else 0.8
    if not vers:
        print("no demo version has a report (python3 tools/build_report.py --version slpm80165 first)"); return
    out = []
    for ver in vers:
        if ver not in VERSIONS or ver == JP:
            print("%s is not a demo version (%s)" % (ver, ", ".join(DEMO_VERSIONS))); continue
        r = sweep(ver, install="--dry-run" not in argv, only=only, limit=limit, jobs=jobs, show="--show" in argv, minr=minr,
                  fix="--fix" in argv, fixmin=fixmin)
        if r and "--report" in argv and "--dry-run" not in argv and r["installed"]:
            r["measures"] = report(ver)
        if r: out.append(r)
    print("\nSummary")
    for r in out:
        print("  %s: %d installed of %d unmatched (%d tried)%s" % (r["version"], len(r["installed"]), r["unmatched"], r["tried"],
              "; %.2f %% of code, %d of %d functions" % (float(r["measures"]["matched_code_percent"]), int(r["measures"]["matched_functions"]),
                                                          int(r["measures"]["total_functions"])) if r.get("measures") else ""))


if __name__ == "__main__":
    main()
