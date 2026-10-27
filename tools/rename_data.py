#!/usr/bin/env python3
"""Applies notes/data_names_main0.csv (address,name,size,source) to the C sources: renames the extern identifiers
whose address (derived from the retail bytes of the compiled object, or spelled in a D_/DAT_<addr> name) is exactly
a named global.  Only MAIN0 / scratchpad addresses (< 0x800E8028 or 0x1F80xxxx); other names are never touched.
Only identifiers declared (extern) in the file itself are renamed; headers are not touched (an identifier declared by a
shared header is only reported as `HDR`: rename it in the header, whose spelled address can be the NTSC one while the
bytes resolve to the PAL one, e.g. D_8009E748 -> CD_QUEUE). New declarations of named globals belong in
include/tomba/globals.h (one type for every source) or globals_game.h (game.h structs).
Needs the objects of a full build (build/full/obj) / report build (build/base/<prog>) to resolve short identifiers.
`--prog X000` applies notes/data_names_x000.csv to the overlay instead: only addresses in the overlay space
(0x800E8028 <= a < 0x80200000) resolved from the X000 objects of src/*.c files whose `// FUNC` header says X000, and the
same identifier renames in the src/x0NN twins of those files (by identifier: a twin's D_ names spell the X000 address even
when its own overlay puts the data elsewhere). src/data/<prog> files are never rewritten (their externs must spell the
address), nor the `*_slpm80165.c` demo sources (their addresses are the demo's) or the `// HEADERS tomba24` sources (US spellings).
`--prog <SYSTEM overlay>` (tools/sysovl.py: O0PTSUB0, D0SPSUB, D0SPSUB2, D0SPSUB3, G0OVER, I0NFO, I0NFO2, I0NFO3) applies
notes/data_names_<prog>.csv to src/<prog lower>/*.c (objects build/full/obj/<prog lower>__<stem>.o, retail bytes from
game/disc/fs/SYSTEM/<PROG>.BIN).  The overlays share the address space, so a csv names only its own overlay.  D0SPSUB3 is the
same code as D0SPSUB with its data at other addresses: in D0SPSUB mode every renamed file with a src/d0spsub3 twin (identical
text apart from the header and the overlay-range D_ spellings) gets the twin's identifiers renamed by position, and the twin's
addresses are appended to notes/data_names_d0spsub3.csv (source `twin of D0SPSUB <addr>`) so a later D0SPSUB3 run is consistent.
Usage: tools/rename_data.py [--prog MAIN0|X000|<SYSTEM overlay>] [--csv notes/data_names_<prog>.csv] [--dry]"""
import csv, os, re, struct, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
sys.argv, _a = [sys.argv[0]], sys.argv[1:]
import build_full as bf
PROGMODE = _a[_a.index("--prog") + 1].upper() if "--prog" in _a else "MAIN0"
CSV = _a[_a.index("--csv") + 1] if "--csv" in _a else os.path.join(ROOT, "notes", "data_names_%s.csv" % PROGMODE.lower())
DRY = "--dry" in _a
names = {}
for r in csv.DictReader(open(CSV)):
    names[int(r["address"], 16)] = r["name"]
if PROGMODE == "MAIN0":
    inrange = lambda a: 0x80010000 <= a < 0x800E8028 or 0x1F800000 <= a < 0x1F800400
else:
    inrange = lambda a: 0x800E8028 <= a < 0x80200000
OVERLAY_DIRS = ["x%03d" % n for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]
PROG = {"MAIN0": ("game/MAIN0.EXE", 0x80010000, 0x800), "X000": ("game/AREA00/X000.BIN", 0x800E8028, 0)}
for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19):
    PROG["X%03d" % n] = ("game/AREA%02d/X%03d.BIN" % (n, n), 0x800E8028, 0)
import sysovl
for _p, (_path, _sha, _code) in sysovl.SYSOVL.items():
    PROG[_p] = ("game/" + _path, sysovl.BASE, 0)
SYSMODE = PROGMODE in sysovl.SYSOVL
TWIN_CSV = os.path.join(ROOT, "notes", "data_names_d0spsub3.csv")
OVL_ID = re.compile(r"\b(?:D|DAT|PTR)_(80[01][0-9a-fA-F]{5})\b")
GAME = {}
def game(p):
    if p not in GAME:
        f = os.path.join(ROOT, PROG[p][0]); GAME[p] = open(f, "rb").read() if os.path.exists(f) else None
    return GAME[p]

def obj_syms(obj, addr, size, prog):
    g = game(prog)
    if g is None: return {}
    _, vram, hdr = PROG[prog]
    retail = g[hdr + addr - vram: hdr + addr - vram + size]
    e = bf.Elf(obj); ours = e.data(".text"); rel = e.relocs(".text"); out = {}
    if len(ours) != size or len(retail) != size: return {}
    for i, (off, ty, sym) in enumerate(rel):
        if ty != 5 or sym["shndx"] != 0: continue
        lo = next((r for r in rel[i + 1:] if r[1] == 6 and r[2] is sym), None)
        if lo is None: continue
        val = ((bf.W(retail, off) & 0xFFFF) << 16) + bf.sext16(bf.W(retail, lo[0]))
        add = ((bf.W(ours, off) & 0xFFFF) << 16) + bf.sext16(bf.W(ours, lo[0]))
        out.setdefault(sym["name"], set()).add((val - add) & 0xFFFFFFFF)
    return {k: next(iter(v)) for k, v in out.items() if len(v) == 1}

H = set()   # identifiers declared by the shared PAL headers (include/tomba, not the US copy in tomba24)
for h in os.listdir(os.path.join(ROOT, "include", "tomba")):
    if h.endswith(".h"):
        for mm in re.finditer(r"\bextern\b[^;{}()]*?\b(\w+)\s*(?:\[[^\]]*\])*\s*;", open(os.path.join(ROOT, "include", "tomba", h), errors="replace").read()):
            H.add(mm.group(1))
changed = skipped = 0
def walk():
    if SYSMODE:
        dp = os.path.join(ROOT, "src", PROGMODE.lower()); yield dp, os.listdir(dp); return
    for dp, dn, fn in os.walk(os.path.join(ROOT, "src")): yield dp, fn
twin_rows = []
def rename_sys_twin(stem, txt, used, idmap):
    """D0SPSUB -> D0SPSUB3: the twin spells its own data addresses, so the identifiers are paired by position: the two texts
    must be identical once the header lines and the overlay-range D_ identifiers are masked.  Renames the twin and collects
    csv rows (twin address, same name) for notes/data_names_d0spsub3.csv."""
    global changed
    tp = os.path.join(ROOT, "src", "d0spsub3", stem + ".c")
    if not os.path.exists(tp): return
    ttxt = open(tp, errors="surrogateescape").read()
    def mask(s):
        s = re.sub(r"^//\s*(FUNC|MATCHING)\b.*$", "", s, flags=re.M)
        return OVL_ID.sub("@", s)
    if mask(txt) != mask(ttxt): print("TWIN?", tp, "differs from the D0SPSUB source"); return
    pairs = {}
    for a, b in zip(OVL_ID.finditer(txt), OVL_ID.finditer(ttxt)):
        if pairs.setdefault(a.group(0), b.group(0)) != b.group(0): print("TWIN?", tp, "inconsistent pairing", a.group(0)); return
    tnew = ttxt
    for ident, nm in used.items():
        tid = pairs.get(ident)
        if tid is None: continue
        if re.search(r"\b%s\b" % re.escape(nm), ttxt): print("SKIP", tp, tid, nm); continue
        tnew = re.sub(r"\b%s\b" % re.escape(tid), nm, tnew)
        twin_rows.append((int(tid[-8:], 16), nm, "twin of D0SPSUB %08x" % idmap[ident]))
    if tnew != ttxt:
        changed += 1
        if not DRY: open(tp, "w", errors="surrogateescape").write(tnew)
def rename_twins(stem, used):
    """Applies the identifier renames of src/<stem>.c to its src/x0NN/<stem>.c twins (same source, other overlay)."""
    global changed
    for d in OVERLAY_DIRS:
        tp = os.path.join(ROOT, "src", d, stem + ".c")
        if not os.path.exists(tp): continue
        ttxt = open(tp, errors="surrogateescape").read(); tnew = ttxt
        for ident, nm in used.items():
            if not re.search(r"\bextern\b[^;{}]*\b%s\b" % re.escape(ident), ttxt): continue
            if re.search(r"\b%s\b" % re.escape(nm), ttxt): print("SKIP", tp, ident, nm); continue
            tnew = re.sub(r"\b%s\b" % re.escape(ident), nm, tnew)
        if tnew != ttxt:
            changed += 1
            if not DRY: open(tp, "w", errors="surrogateescape").write(tnew)

for dp, fn in walk():
    rel = os.path.relpath(dp, os.path.join(ROOT, "src"))
    if PROGMODE != "MAIN0" and not SYSMODE and rel != ".": continue   # overlay mode: src/*.c only, twins follow by identifier; never src/data
    for f in sorted(fn):
        if not f.endswith((".c", ".h")): continue
        if "slpm80165" in f: continue   # demo sources: their addresses are the demo's, never rewritten
        path = os.path.join(dp, f); txt = open(path, errors="surrogateescape").read()
        if "HEADERS tomba24" in txt[:600]: continue   # built with the US headers: they keep the US spellings
        m = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)\s+(\w+)", txt[:600])
        if PROGMODE != "MAIN0" and not (m and m.group(3).upper() == PROGMODE): continue
        idmap = {}   # identifier -> address
        for mm in re.finditer(r"\b(?:D|DAT)_([0-9a-fA-F]{8})\b", txt):
            idmap[mm.group(0)] = int(mm.group(1), 16)
        if m and f.endswith(".c"):
            addr, size, prog = int(m.group(1), 16), int(m.group(2)), m.group(3).upper()
            stem = f[:-2]
            cands = [os.path.join(ROOT, "build", "full", "obj", stem + ".o")] if rel == "." else []
            if SYSMODE: cands = [os.path.join(ROOT, "build", "full", "obj", "%s__%s.o" % (prog.lower(), stem))]
            cands.append(os.path.join(ROOT, "build", "base", prog.lower(), stem + ".o"))
            for o in cands:
                if os.path.exists(o) and prog in PROG:
                    try:
                        om = obj_syms(o, addr, size, prog)
                        if om: idmap = dict(om)   # real addresses win over addresses spelled in names
                    except Exception as ex: print("ERR", path, ex)
                    break
        used = {}
        new = txt
        for ident, a in sorted(idmap.items(), key=lambda x: -len(x[0])):
            nm = names.get(a)
            if not nm or not inrange(a) or nm == ident: continue
            local = re.search(r"\bextern\b[^;{}]*\b%s\b" % re.escape(ident), txt)
            if not local:
                if ident in H: print("HDR ", path, ident, nm)   # declared by a header: rename it there by hand
                continue
            if nm in used.values() or re.search(r"\b%s\b" % re.escape(nm), txt):
                print("SKIP", path, ident, nm); skipped += 1
                if f.endswith(".h"): H.discard(ident)
                continue
            used[ident] = nm
            new = re.sub(r"\b%s\b" % re.escape(ident), nm, new)
        if new != txt:
            changed += 1
            if not DRY: open(path, "w", errors="surrogateescape").write(new)
            if PROGMODE == "D0SPSUB" and f.endswith(".c"): rename_sys_twin(f[:-2], txt, used, idmap)
            elif PROGMODE != "MAIN0" and not SYSMODE and f.endswith(".c"): rename_twins(f[:-2], used)
if twin_rows:
    have = {int(r["address"], 16) for r in csv.DictReader(open(TWIN_CSV))} if os.path.exists(TWIN_CSV) else set()
    rows = sorted({r for r in twin_rows if r[0] not in have})
    for a, nm, src in rows: print("TWIN", "%08x" % a, nm)
    if rows and not DRY:
        new = not os.path.exists(TWIN_CSV)
        with open(TWIN_CSV, "a") as fo:
            if new: fo.write("address,name,size,source\n")
            for a, nm, src in rows: fo.write("%08x,%s,,%s\n" % (a, nm, src))
print("files changed:", changed, "skipped:", skipped)
