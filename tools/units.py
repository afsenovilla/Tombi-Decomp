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

Usage: python tools/units.py            prints the totals per program
       from units import units          -> {prog: [Unit]}  (prog = "MAIN0", "X000", "X001", ...)
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
_BLOBS = {}

# Per-area jump / pointer tables that Ghidra exported as "functions" (the csv rows func_80115EC4, func_80115F24,
# func_80116258, func_801162B8, func_8011658C, ... of the area overlays).  They are DATA: every word is a pointer into
# the overlay / MAIN0 (or a zero / flag constant), and the only jal that names them is a case of the area dispatchers
# (FUN_800e9f74 and the one before it) for an area id that has no overlay or is another overlay's, never reached
# while this overlay is loaded.  Verified per overlay; the same address is real code in other overlays (X005, X008,
# X019 ...), which are not listed.  A row that starts with such a table has the table cut off (the code follows).
# Never counted as code: the ranges are subtracted from the csv rows (so a regenerated csv stays correct) and
# tools/gen_symbols.py gives them no symbol (splat emits them as .word data).
DATA = [
    ("X001", 0x80115EC4, 96),
    ("X001", 0x80116258, 96),
    ("X001", 0x801162B8, 724),
    ("X002", 0x80115EC4, 96),
    ("X002", 0x80116258, 96),
    ("X002", 0x801162B8, 160),
    ("X003", 0x80115EC4, 96),
    ("X003", 0x80115F24, 820),
    ("X003", 0x80116258, 96),
    ("X003", 0x801162B8, 724),
    ("X003", 0x8011658C, 916),
    ("X003", 0x80116920, 156),
    ("X003", 0x801169BC, 916),
    ("X004", 0x80115EC4, 96),
    ("X004", 0x80115F24, 820),
    ("X004", 0x80116258, 96),
    ("X004", 0x801162B8, 724),
    ("X004", 0x8011658C, 472),
    ("X005", 0x80115EC4, 32),
    ("X006", 0x80115EC4, 96),
    ("X006", 0x80115F24, 488),
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
    ("X016", 0x80115EC4, 96),
    ("X016", 0x80115F24, 100),
    ("X017", 0x80115EC4, 96),
    ("X017", 0x80115F24, 504),
    ("X018", 0x80115EC4, 96),
    ("X018", 0x80115F24, 820),
    ("X018", 0x80116258, 96),
    ("X018", 0x801162B8, 32),
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
            h = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)[ \t]*(MAIN0|X0\d\d)?", t)
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
    names = re.findall(r"^[A-Za-z_][\w \t*]*?\b(\w+)\s*\([^;]*\)\s*(?:\n\s*)?\{", t, re.M)
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


def units():
    """{prog: [Unit]} sorted by address; every byte of every csv row (inside the code range) and every src file
    range belongs to exactly one unit."""
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


def counted(u):
    """A unit enters the headline figure when it is game code counted for the first time."""
    return not u.lib and not u.twin


def totals():
    """{prog: (game bytes, matched bytes, n units, n matched)} of the counted units."""
    t = {}
    for prog, us in units().items():
        g = m = n = nm = 0
        for u in us:
            if not counted(u): continue
            g += u.size; n += 1
            if u.file: m += u.size; nm += 1
        t[prog] = (g, m, n, nm)
    return t


if __name__ == "__main__":
    T = totals()
    G = M = 0
    for prog, (g, m, n, nm) in T.items():
        print("%-6s %8d B  matched %8d B  %5.1f %%  (%d/%d units)" % (prog, g, m, 100.0 * m / g if g else 0, nm, n))
        G += g; M += m
    print("TOTAL  %8d B  matched %8d B  %5.2f %%" % (G, M, 100.0 * M / G if G else 0))
