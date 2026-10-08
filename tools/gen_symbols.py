#!/usr/bin/env python3
"""Generates splat symbol files (config/symbol_addrs_<prog>.txt) from:
  - src/*.c and src/x0nn/*.c headers (// FUNC addr size prog) + the C function name defined in each file (exact size)
  - notes/names_*.csv and notes/functions_*.csv (names and boundaries for the rest)
so that splat names each retail function exactly like our C function (objdiff pairs them by name)."""
import csv, os, re
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RANGES = {"MAIN0": (0x80010000, 0x80099000), "X000": (0x800E8028, 0x80140000)}
PROGS = {"MAIN0": ("functions_main0.csv", "names_main0.csv"), "X000": ("functions_x000.csv", "names_x000.csv")}
# area overlays (tools/areas.py bounds): same load address as X000, sources in src/x0nn/
for _n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19):
    _p = "X%03d" % _n
    RANGES[_p] = RANGES["X000"]
    PROGS[_p] = ("functions_%s.csv" % _p.lower(), "names_%s.csv" % _p.lower())
syms = {p: {} for p in PROGS}   # addr -> (name, size or None)
for p, (fcsv, ncsv) in PROGS.items():
    npath = os.path.join(R, "notes", ncsv)
    names = {r["address"]: r["name"] for r in csv.DictReader(open(npath))} if os.path.exists(npath) else {}
    for r in csv.DictReader(open(os.path.join(R, "notes", fcsv))):
        a = int(r["address"], 16)
        if not (RANGES[p][0] <= a < RANGES[p][1]): continue
        n = names.get(r["address"]) or ("func_%08X" % a if r["name"].startswith(("FUN_", "thunk_")) else r["name"])
        syms[p][a] = (n, None)
src = os.path.join(R, "src")
files = [os.path.join(src, f) for f in sorted(os.listdir(src))]
for d in sorted(os.listdir(src)):
    if re.match(r"x0\d\d$", d): files += [os.path.join(src, d, f) for f in sorted(os.listdir(os.path.join(src, d)))]
for path in files:
    f = os.path.basename(path)
    if not f.endswith(".c"): continue
    t = open(path, errors="replace").read()
    m = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)\s*(MAIN0|X0\d\d)?", t)
    if not m or "// MATCHING" not in t: continue
    a, size, p = int(m.group(1), 16), int(m.group(2)), m.group(3) or "MAIN0"
    stem = f[:-2]
    if not re.search(r"\b%s\s*\(" % re.escape(stem), t):
        print("WARN: %s does not define %s()" % (f, stem)); continue
    syms[p][a] = (stem, size)
for p in PROGS:
    seen = {}
    with open(os.path.join(R, "config", "symbol_addrs_%s.txt" % p.lower()), "w") as out:
        for a in sorted(syms[p]):
            n, size = syms[p][a]
            if n in seen: n = "%s_%08X" % (n, a)     # splat needs unique names
            seen[n] = a
            out.write("%s = 0x%08X; // type:func%s\n" % (n, a, (" size:0x%X" % size) if size else ""))
    print(p, len(syms[p]), "symbols")
