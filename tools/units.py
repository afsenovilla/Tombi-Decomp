#!/usr/bin/env python3
"""The single list of code units shared by tools/progress.py, tools/gen_symbols.py and tools/build_report.py, so
that docs/PROGRESS.md and the objdiff report (decomp.dev) count exactly the same bytes.

A unit is a contiguous range of machine code of one program:
  - every src file with a `// MATCHING addr size` marker is one unit (its exact range, whatever the csv says);
  - the parts of notes/functions_*.csv rows that no such file covers are units too (whole rows, or the
    uncovered remainder of a row that a file cuts), except the ranges listed in DATA (jump tables, not code);
Each unit knows its program, address, size, name, the src file (if any), whether it is Psy-Q library code
(same rule as progress.py: names csv comment "Psy-Q", or a real name given by Ghidra) and, for the area
overlays, whether it is a twin (a copy of code counted in MAIN0, X000 or an earlier overlay).  The headline
figure of the project is: game bytes = sum of non-library, non-twin units; matched = those with a file.

Other versions (tools/versions.py): units(ver) gives one unit per function of config/<ver>/symbol_addrs_<prog>.txt
(`type:func size:`; the splat 'functions' of config/<ver>/data_units.txt are data, left out), with the src file that
builds it (version_sources: PAL pairing comments, config/<ver>/reuse.txt, `// VERSION` lines); prog = the version
program ("scus94236", "x00", "x01", "dspsub", ...). tools/build_report.py --version and tools/build_full.py --version
use the same resolution.

Usage: python tools/units.py [--version V]   prints the totals per program
       from units import units          -> {prog: [Unit]}  (prog = "MAIN0", "X000", "X001", ...)
       units("scus94236")               -> {prog: [Unit]}  (prog = "scus94236", "x00", ...)
"""
import bisect
import csv
import os
import re
import sys
from collections import namedtuple

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
N = lambda *p: os.path.join(ROOT, *p)

Unit = namedtuple("Unit", "prog addr size name file lib twin csv")
# csv: the csv row this unit comes from (address) or None for a src file unit

RANGES = {"MAIN0": (0x800163F4, 0x800778EF), "X000": (0x800E8028, 0x8013C0F8)}
OVERLAYS = ["X%03d" % n for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]
for _p in OVERLAYS:
    RANGES[_p] = (0x800E8028, 0x80140000)
PROGS = ["MAIN0", "X000"] + OVERLAYS
BINS = {"MAIN0": ("game/MAIN0.EXE", 0x80010000 - 0x800), "X000": ("game/AREA00/X000.BIN", 0x800E8028)}
for _p in OVERLAYS:
    BINS[_p] = ("game/AREA" + _p[2:] + "/" + _p + ".BIN", 0x800E8028)
# SYSTEM code overlays (tools/sysovl.py): code range = header table end .. end of the file
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sysovl as _SO
SYSOVL = _SO.PROGS
for _p, (_path, _sha, _code) in _SO.SYSOVL.items():
    _f = N("game", _path)
    RANGES[_p] = (_code, _SO.BASE + (os.path.getsize(_f) if os.path.exists(_f) else 0x30000))
    BINS[_p] = ("game/" + _path, _SO.BASE)
PROGS = PROGS + SYSOVL
_BLOBS = {}

# Per-area jump / pointer tables that Ghidra exported as "functions" (the csv rows func_80115EC4, func_80115F24,
# func_80116258, func_801162B8, func_8011658C, ... of the area overlays).  They are DATA: every word is a pointer into
# the overlay / MAIN0 (or a zero / flag constant), and the only jal that names them is a case of the area dispatchers
# (FUN_800e9f74 and the one before it) for an area id that has no overlay or is another overlay's, never reached
# while this overlay is loaded.  Verified per overlay; the same address is real code in other overlays (X005, X008,
# X019 ...), which are not listed.  Second batch (X000 80116258/801162B8, X001 8011658C, X003 80116D50, X005/X013
# 801188FC, X005/X008 80117CAC): same checks; their jals are other areas' dispatcher cases, the MAIN0 pointers are slots of
# other areas' tables.  Third batch: X016 8011B170 and X018 8011C758 (pointer tables jal'd by the shared player-step code,
# step 0x47 / step 0x48 with object kind 0xE): proved unreachable while that overlay is loaded (0x47 is written only by
# X014 code and PLAYER is cleared on every area entry; no object of type 0xE exists in AREA18).  Fourth batch: X002
# 8011E688 (anim offset data jal'd by step 0x25): AREA02 has climbable walls (b9e = 6, step 13), but step 0x25 needs
# b9e == 6 with a terrain probe result != 1, which only steps 9 / 0x43 at state != 0 could see; both clear b9e at state 0.  A row that starts with such a table has the table cut off (the code follows).
# Never counted as code: the ranges are subtracted from the csv rows (so a regenerated csv stays correct) and
# tools/gen_symbols.py gives them no symbol (splat emits them as .word data).
DATA = [
    ("X000", 0x80116258, 96),
    ("X000", 0x801162B8, 116),
    ("X001", 0x80115EC4, 96),
    ("X001", 0x80116258, 96),
    ("X001", 0x801162B8, 724),
    ("X001", 0x8011658C, 52),
    ("X002", 0x80115EC4, 96),
    ("X002", 0x80116258, 96),
    ("X002", 0x801162B8, 160),
    ("X002", 0x8011E688, 40),
    ("X003", 0x80115EC4, 96),
    ("X003", 0x80115F24, 820),
    ("X003", 0x80116258, 96),
    ("X003", 0x801162B8, 724),
    ("X003", 0x8011658C, 916),
    ("X003", 0x80116920, 156),
    ("X003", 0x801169BC, 916),
    ("X003", 0x80116D50, 548),
    ("X004", 0x80115EC4, 96),
    ("X004", 0x80115F24, 820),
    ("X004", 0x80116258, 96),
    ("X004", 0x801162B8, 724),
    ("X004", 0x8011658C, 472),
    ("X005", 0x80115EC4, 32),
    ("X005", 0x80117CAC, 16),
    ("X005", 0x801188FC, 20),
    ("X006", 0x80115EC4, 96),
    ("X006", 0x80115F24, 488),
    ("X008", 0x80117CAC, 16),
    ("X008", 0x8011832C, 276),
    ("X009", 0x80115EC4, 96),
    ("X009", 0x80115F24, 820),
    ("X009", 0x80116258, 32),
    ("X010", 0x80115EC4, 96),
    ("X010", 0x80116258, 96),
    ("X010", 0x801162B8, 724),
    ("X010", 0x8011658C, 84),
    ("X011", 0x80115EC4, 96),
    ("X011", 0x80115F24, 156),
    ("X013", 0x80115EC4, 96),
    ("X013", 0x80115F24, 144),
    ("X013", 0x801188FC, 20),
    ("X016", 0x80115EC4, 96),
    ("X016", 0x80115F24, 100),
    ("X016", 0x8011B170, 448),
    ("X017", 0x80115EC4, 96),
    ("X017", 0x80115F24, 504),
    ("X018", 0x80115EC4, 96),
    ("X018", 0x80115F24, 820),
    ("X018", 0x80116258, 96),
    ("X018", 0x801162B8, 32),
    ("X018", 0x8011C758, 680),
]


def word(prog, addr):
    """The 32-bit word of the retail binary at addr (None when the file is not there)."""
    if prog not in _BLOBS:
        path, base = BINS[prog]
        _BLOBS[prog] = (open(N(path), "rb").read() if os.path.exists(N(path)) else b"", base)
    blob, base = _BLOBS[prog]
    off = addr - base
    return int.from_bytes(blob[off:off + 4], "little") if blob and 0 <= off and off + 4 <= len(blob) else None


def read_csv(path):
    if not os.path.exists(path):
        return []
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


def src_files():
    """{prog: [(addr, size, stem, path)]} of the src files with a MATCHING marker (recursively, src/wip excluded)."""
    out = {}
    for dp, dirs, files in os.walk(N("src")):
        dirs[:] = [d for d in dirs if d != "wip"]
        for fn in sorted(files):
            if not fn.endswith(".c"): continue
            path = os.path.join(dp, fn)
            t = open(path, errors="replace").read()
            h = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)[ \t]*(MAIN0|X0\d\d|%s)?(?!\w)" % _SO.PROG_RE, t)
            m = re.search(r"//\s*MATCHING\s+([0-9a-fA-F]+)\s+(\d+)", t)
            if not h or not m: continue
            prog = (h.group(3) or "MAIN0").upper()
            out.setdefault(prog, []).append((int(m.group(1), 16), int(m.group(2)), fn[:-2], path))
    for p in out: out[p].sort()
    return out


def defined_name(path, stem):
    """The name of the function the file defines for the retail one (the stem, or whatever C/asm name is used)."""
    t = open(path, errors="replace").read()
    if re.search(r"\b%s\s*\(" % re.escape(stem), t): return stem
    m = re.search(r"\.globl\s+(\w+)", t)                       # top-level asm body
    if m: return m.group(1)
    names = re.findall(r"^([A-Za-z_][\w \t*]*?\b(\w+)\s*\([^;]*\)\s*(?:\n\s*)?\{)", t, re.M)
    names = [n for head, n in names if not head.startswith("static")] or [n for head, n in names]  # skip static helpers
    return names[0] if names else stem


def lib_rows(prog):
    """{address: True} for the library rows of MAIN0/X000 (same classification as progress.py)."""
    ours = {int(r["address"], 16): r for r in read_csv(N("notes", "names_%s.csv" % prog.lower()))}
    lib = {}
    for r in read_csv(N("notes", "functions_%s.csv" % prog.lower())):
        a = int(r["address"], 16)
        mine = ours.get(a)
        is_fun = r["name"].startswith(("FUN_", "thunk_FUN_", "func_"))
        if mine is not None: lib[a] = "Psy-Q" in mine.get("comment", "")
        else: lib[a] = not is_fun
    return lib


_CACHE = {}


def units(ver=None):
    """{prog: [Unit]} sorted by address; every byte of every csv row (inside the code range) and every src file
    range belongs to exactly one unit. ver: another version (tools/versions.py), see version_units."""
    if ver is not None:
        from versions import canon, REFERENCE
        if canon(ver) != REFERENCE: return version_units(canon(ver))
    if _CACHE: return _CACHE
    files = src_files()
    for prog in PROGS:
        lo, hi = RANGES[prog]
        rows = []
        for r in read_csv(N("notes", "functions_%s.csv" % prog.lower())):
            a, size = int(r["address"], 16), int(r["size"] or 0)
            if not (lo <= a <= hi) or size <= 0: continue
            rows.append((a, size, r["name"], r.get("twin") or ""))
        rows.sort()
        lib = lib_rows(prog) if prog in ("MAIN0", "X000") else {}
        names = {int(r["address"], 16): r.get("comment", "") for r in read_csv(N("notes", "names_%s.csv" % prog.lower()))}
        starts = [r[0] for r in rows]
        def row_of(addr):   # the csv row containing addr (library/twin flags of a file come from it)
            i = bisect.bisect_right(starts, addr) - 1
            if i >= 0 and rows[i][0] <= addr < rows[i][0] + rows[i][1]: return rows[i]
            return None
        out = []
        fl = [f for f in files.get(prog, []) if lo <= f[0] <= hi]
        # src file units; overlapping files are an error of the sources, keep the first and warn
        end = 0
        for a, size, stem, path in fl:
            if a < end:
                print("WARN: %s overlaps the previous unit" % path, file=sys.stderr); continue
            row = row_of(a)
            is_lib = lib.get(row[0], False) if row else ("Psy-Q" in names.get(a, ""))  # no csv row: names csv decides
            out.append(Unit(prog, a, size, stem, path, is_lib, bool(row and row[3]), None))
            end = a + size
        # csv rows: the parts not covered by a file
        for a, size, name, tw in rows:
            pieces = [(a, a + size)]
            for dp, da, ds in DATA:
                if dp != prog: continue
                nxt = []
                for s, e in pieces:
                    if da + ds <= s or da >= e: nxt.append((s, e)); continue
                    if s < da: nxt.append((s, da))
                    if da + ds < e: nxt.append((da + ds, e))
                pieces = nxt
            for u in out:
                if u.file is None: continue
                fa, fb = u.addr, u.addr + u.size
                nxt = []
                for s, e in pieces:
                    if fb <= s or fa >= e: nxt.append((s, e)); continue
                    if s < fa: nxt.append((s, fa))
                    if fb < e: nxt.append((fb, e))
                pieces = nxt
            for s, e in pieces:
                nm = name if s == a else "%s_%08X" % (name, s)
                out.append(Unit(prog, s, e - s, nm, None, lib.get(a, False), bool(tw), a))
        out.sort(key=lambda u: u.addr)
        # Ghidra fragments: a csv "function" shorter than 8 bytes (a tail cut off a library function) or one that
        # starts with a zero word (padding/data) is not a function; with a symbol there splat emits the rest of
        # the program as .word.  Merged into the previous csv unit when adjacent, dropped otherwise.
        merged = []
        for u in out:
            if u.file is None and (u.size < 8 or word(prog, u.addr) == 0):
                if merged and merged[-1].file is None and merged[-1].addr + merged[-1].size == u.addr:
                    v = merged[-1]; merged[-1] = v._replace(size=v.size + u.size)
                continue
            merged.append(u)
        _CACHE[prog] = merged
    return _CACHE


# ------------------------------------------------------------------------------------------------- other versions
def version_lines(serial):
    """{(prog, addr): (src path, size)} of the `// VERSION <SERIAL> <prog> <addr> <size>` lines of the shared src tree:
    the file whose `#if defined(VERSION_<SERIAL>)` branch (or whole guarded body) builds that version function."""
    out = {}
    rx = re.compile(r"^//\s*VERSION\s+" + serial + r"\s+(\w+)\s+([0-9A-Fa-f]{8})\s+(\d+)", re.M)
    for dp, dirs, fs in os.walk(N("src")):
        dirs[:] = [d for d in dirs if d != "wip"]
        for fn in sorted(fs):
            if not fn.endswith(".c"): continue
            path = os.path.join(dp, fn)
            t = open(path, errors="replace").read()
            if "VERSION" not in t: continue
            for m in rx.finditer(t):
                out[(m.group(1), int(m.group(2), 16))] = (path, int(m.group(3)))
    return out


def version_sources(progs, ver="slpm80165"):
    """{(prog, version addr): (src path, version name)}: the C that builds each function of a version.

    By default a version function reuses the PAL file that matches the PAL function tools/xmatch.py paired it with: the
    `PAL <module> <address>` comment of config/<ver>/symbol_addrs_<prog>.txt is looked up by address among the src
    files (units()), so PAL renames do not break it; the file must cover exactly the version function's size.
    config/<ver>/reuse.txt overrides or adds pairs, one per line: `<prog> <addr> <PAL module> <PAL addr>` or
    `<prog> <addr> -` (no C). A function that differs from PAL is built from the shared file that carries a
    `// VERSION <SERIAL> <prog> <addr> <size>` line (its `#if defined(VERSION_<SERIAL>)` branch: version C, or
    INCLUDE_ASM, which compiles to nothing with -DSKIP_ASM; see tools/version_share.py).
    """
    from versions import VERSIONS
    pal = {}
    for p, us in units().items():
        for u in us:
            if u.file: pal[(p.lower(), u.addr)] = u
    out, names = {}, {}
    for prog in progs:
        for l in open(N("config", ver, "symbol_addrs_%s.txt" % prog)):
            m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+); // type:func size:0x([0-9A-Fa-f]+)\s+PAL (\w+) 0x([0-9A-Fa-f]+)", l)
            if not m: continue
            a = int(m.group(2), 16)
            names[(prog, a)] = (m.group(1), int(m.group(3), 16))
            u = pal.get((m.group(4), int(m.group(5), 16)))
            if u and u.size == int(m.group(3), 16): out[(prog, a)] = (u.file, m.group(1))
    rp = N("config", ver, "reuse.txt")
    if os.path.exists(rp):
        for l in open(rp):
            f = l.split("#")[0].split()
            if not f: continue
            prog, a = f[0], int(f[1], 16)
            nm = names.get((prog, a), ("func_%08X" % a, 0))[0]
            if len(f) == 4:
                u = pal.get((f[2].lower(), int(f[3], 16)))
                if u: out[(prog, a)] = (u.file, nm)
                else: print("reuse.txt: no PAL src at", f[2], f[3])
            elif f[2] == "-": out.pop((prog, a), None)
    for (prog, a), (path, _) in version_lines(VERSIONS[ver]["serial"]).items():
        if prog in progs: out[(prog, a)] = (path, names.get((prog, a), ("func_%08X" % a, 0))[0])
    return out


def version_source(src, serial, tmpdir):
    """Copy of a shared src file for building version <serial>: a `// D.<SERIAL> value` directive (D = FLAGS, CC,
    HEADERS, AS) replaces the file's `// D` line (value `default`: no such line), so a version branch can use other
    compiler options than the PAL code of the same file."""
    t = open(src, errors="replace").read()
    for d in ("FLAGS", "CC", "HEADERS", "AS", "ASPSX"):
        m = re.search(r"^//\s*%s\.%s\s+(.*)$" % (d, serial), t, re.M)
        if not m: continue
        t = re.sub(r"(?m)^//\s*%s\s.*\n?" % d, "", t)
        val = m.group(1).strip()
        t = re.sub(r"(?m)^//\s*%s\.%s\s.*$" % (d, serial), "" if val == "default" else "// %s %s" % (d, val), t)
    out = os.path.join(tmpdir, os.path.basename(src))
    open(out, "w").write(t)
    return out


def spad_set(text, serial=None):
    """`.set` lines that make PAL scratchpad symbols (config/slpm80165/spad.txt) constants, since the version targets
    keep every scratchpad address literal. Version C may also name any scratchpad address D_1F80xxxx."""
    from versions import VERSIONS
    S = spad_set.__dict__
    if "tab" not in S:
        p = N("config", "slpm80165", "spad.txt")
        S["tab"] = [l.split()[:2] for l in open(p) if l.strip() and not l.startswith("#")] if os.path.exists(p) else []
    tab = dict(S["tab"])
    # config/<ver>/spad_remap.txt (`<PAL addr> <version addr>` per line): that version keeps the variable elsewhere in
    # the scratchpad, so every PAL symbol at that address is assembled at the version's address instead.
    ver = next((v for v, c in VERSIONS.items() if c.get("serial") == serial), None) if serial else None
    rp = N("config", ver or "-", "spad_remap.txt")
    if os.path.exists(rp):
        rm = dict(l.split()[:2] for l in open(rp) if l.strip() and not l.startswith("#"))
        tab = {n: rm.get(a.upper(), a) for n, a in tab.items()}
    tab.update((n, n[2:]) for n in re.findall(r"\bD_1F80[0-9A-F]{4}\b", text))
    return "".join("\t.set\t%s,0x%s\n" % (n, a) for n, a in sorted(tab.items()))


def version_progs(ver):
    """The programs of a version that have a splat config (config/<ver>/<prog>.yaml), in tools/versions.py order."""
    from versions import VERSIONS
    return [p for p in VERSIONS[ver]["progs"] if os.path.exists(N("config", ver, p + ".yaml"))]


_VCACHE = {}


def version_units(ver):
    """{prog: [Unit]} of a version: one unit per `type:func size:` line of config/<ver>/symbol_addrs_<prog>.txt that is
    not a data unit (config/<ver>/data_units.txt), with its src file from version_sources (None: no C)."""
    if ver in _VCACHE: return _VCACHE[ver]
    progs = version_progs(ver)
    srcs = version_sources(progs, ver)
    dfile = N("config", ver, "data_units.txt")
    dunits = {(l.split()[0], int(l.split()[1], 16)) for l in (open(dfile) if os.path.exists(dfile) else ())
              if l.strip() and not l.startswith("#")}
    out = {}
    for prog in progs:
        us, seen = [], set()
        for l in open(N("config", ver, "symbol_addrs_%s.txt" % prog)):
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func.*size:0x([0-9A-Fa-f]+)", l)
            if not m: continue
            a = int(m.group(2), 16)
            if (prog, a) in dunits or a in seen: continue
            seen.add(a)
            us.append(Unit(prog, a, int(m.group(3), 16), m.group(1), srcs.get((prog, a), (None,))[0], False, False, None))
        out[prog] = sorted(us, key=lambda u: u.addr)
    _VCACHE[ver] = out
    return out


def counted(u):
    """A unit enters the headline figure when it is game code counted for the first time."""
    return not u.lib and not u.twin


def totals(ver=None):
    """{prog: (game bytes, matched bytes, n units, n matched)} of the counted units."""
    t = {}
    for prog, us in units(ver).items():
        g = m = n = nm = 0
        for u in us:
            if not counted(u): continue
            g += u.size; n += 1
            if u.file: m += u.size; nm += 1
        t[prog] = (g, m, n, nm)
    return t


if __name__ == "__main__":
    T = totals(sys.argv[sys.argv.index("--version") + 1] if "--version" in sys.argv else None)
    G = M = 0
    for prog, (g, m, n, nm) in T.items():
        print("%-6s %8d B  matched %8d B  %5.1f %%  (%d/%d units)" % (prog, g, m, 100.0 * m / g if g else 0, nm, n))
        G += g; M += m
    print("TOTAL  %8d B  matched %8d B  %5.2f %%" % (G, M, 100.0 * M / G if G else 0))
