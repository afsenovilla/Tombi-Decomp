#!/usr/bin/env python3
"""Computes decompilation progress and writes docs/PROGRESS.md and docs/progress.json.

Reads notes/functions_*.csv (tools/export_functions.py), notes/names_*.csv and src/ (our own C).
Usage: python tools/progress.py
"""
import csv
import json
import os
import re
import datetime
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
N = lambda *p: os.path.join(ROOT, *p)
sys.path.insert(0, os.path.join(ROOT, "tools"))

# program -> (functions csv, names csv, code range [lo, hi])
# The keys are kept as-is because they are also the keys of "programs" in docs/progress.json;
# DISPLAY gives the English label used in PROGRESS.md and docs/progress.svg.
PROGRAMS = {
    "MAIN0.EXE (nucleo del juego)": ("functions_main0.csv", "names_main0.csv", (0x800163F4, 0x800778EF)),
    "X000.BIN (overlay AREA00)": ("functions_x000.csv", "names_x000.csv", (0x800E8028, 0x8013C0F8)),
}
DISPLAY = {
    "MAIN0.EXE (nucleo del juego)": "MAIN0.EXE (game core)",
    "X000.BIN (overlay AREA00)": "X000.BIN (AREA00 overlay)",
}


def read_csv(path):
    if not os.path.exists(path):
        return []
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


OVERLAYS = ["X%03d" % n for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__))))
import sysovl as _SO
SYSOVL = [p for p, (path, sha, code) in _SO.SYSOVL.items()
          if os.path.exists(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "game", path))]
OVL_AREAS = {"X001": "AREA01, AREA07", "X002": "AREA02, AREA19 (alt.)", "X004": "AREA04, AREA12"}
_RANGES = {}


def src_ranges():
    """{prog: [(addr, size)]} of the `// MATCHING addr size` markers in src/ (recursively); the program is the
    PROG field of the file's `// FUNC` header (overlay sources live in src/x0nn/ and share X000's addresses)."""
    if _RANGES: return _RANGES
    for dp, _, files in os.walk(N("src")):
        for fn in files:
            if not fn.endswith(".c"): continue
            t = open(os.path.join(dp, fn), errors="replace").read()
            h = re.search(r"//\s*FUNC\s+[0-9a-fA-F]+\s+\d+[ \t]*(MAIN0|X0\d\d|%s)?(?!\w)" % _SO.PROG_RE, t)
            prog = (h.group(1) if h else None) or "MAIN0"
            for m in re.finditer(r"//\s*MATCHING\s+([0-9a-fA-F]+)\s+(\d+)", t):
                _RANGES.setdefault(prog, []).append((int(m.group(1), 16), int(m.group(2))))
    return _RANGES


def matching_detail():
    """{prog: [bytes, n]} of the MATCHING markers."""
    out = {"MAIN0": [0, 0], "X000": [0, 0]}
    for prog, rs in src_ranges().items():
        out.setdefault(prog, [0, 0])
        for a, n in rs:
            out[prog][0] += n; out[prog][1] += 1
    return out


def count_matching_c():
    """Bytes of functions with 'matching' C declared in src/ with the marker // MATCHING <address> <bytes>."""
    total = 0
    src = N("src")
    if not os.path.isdir(src):
        return 0
    for dp, _, files in os.walk(src):
        for fn in files:
            if fn.endswith(".c"):
                for m in re.finditer(r"//\s*MATCHING\s+\w+\s+(\d+)", open(os.path.join(dp, fn), errors="replace").read()):
                    total += int(m.group(1))
    return total


def analyse(csv_f, names_f, rng):
    funcs = read_csv(N("notes", csv_f))
    ours = {}
    for r in read_csv(N("notes", names_f)):
        ours[int(r["address"], 16)] = r
    d = dict(lib=0, named=0, unnamed=0, typed=0, n_lib=0, n_named=0, n_unnamed=0, n_typed=0,
             excluded=0, n_excluded=0, unnamed_list=[], items=[], unnamed_typed=0, range=list(rng))
    for r in funcs:
        a, size = int(r["address"], 16), int(r["size"])
        if not (rng[0] <= a <= rng[1]):
            d["excluded"] += size
            d["n_excluded"] += 1
            continue
        mine = ours.get(a)
        is_fun = r["name"].startswith(("FUN_", "thunk_FUN_", "func_"))
        if mine is not None and "Psy-Q" in mine.get("comment", ""):
            d["lib"] += size; d["n_lib"] += 1
            d["items"].append((a, size, "lib"))
        elif mine is not None:
            d["named"] += size; d["n_named"] += 1
            if r["typed"] == "1": d["typed"] += size; d["n_typed"] += 1
            d["items"].append((a, size, "named"))
        elif not is_fun:
            d["lib"] += size; d["n_lib"] += 1          # name given by Ghidra (library signature)
            d["items"].append((a, size, "lib"))
        else:
            d["unnamed"] += size; d["n_unnamed"] += 1
            d["unnamed_list"].append((size, r["address"]))
            if r["typed"] == "1":
                d["typed"] += size; d["n_typed"] += 1; d["unnamed_typed"] += size
                d["items"].append((a, size, "typed"))
            else:
                d["items"].append((a, size, "unnamed"))
    d["game"] = d["named"] + d["unnamed"]
    d["n_game"] = d["n_named"] + d["n_unnamed"]
    d["unnamed_list"].sort(reverse=True)
    return d


def pct(a, b):
    return 100.0 * a / b if b else 0.0


def bar(p, w=24):
    k = int(round(p / 100 * w))
    return "[" + "#" * k + "." * (w - k) + "]"


COL = {"match": "#16a34a", "named": "#22c55e", "typed": "#38bdf8", "unnamed": "#f59e0b", "lib": "#94a3b8"}
FONT = "font-family=\"Segoe UI,Helvetica,Arial,sans-serif\""


def esc(t):
    return t.replace("&", "&amp;").replace("<", "&lt;")


def write_svgs(res, tot, matching, whole=(0, 1), areas=(0, 1)):
    hist_path = N("docs", "history.json")
    hist = json.load(open(hist_path)) if os.path.exists(hist_path) else []
    today = datetime.date.today().isoformat()
    point = {"date": today, "named": round(pct(tot["named"], tot["game"]), 2),
             "typed": round(pct(tot["typed"], tot["game"]), 2), "matching": round(pct(matching, tot["game"]), 2),
             "whole": round(pct(*whole), 2)}
    hist = [h for h in hist if h["date"] != today] + [point]
    json.dump(hist, open(hist_path, "w"), indent=1)

    W = 900
    rows = getattr(summary_lines, "rows", [])   # the same rows (and figures) as the Summary table
    H = 108 + 36 * (len(rows) + 1) + 30
    o = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" viewBox="0 0 %d %d" role="img" aria-label="Decompilation progress">' % (W, H, W, H),
         '<rect width="%d" height="%d" rx="14" fill="#ffffff" stroke="#e2e8f0"/>' % (W, H),
         '<text x="28" y="44" %s font-size="22" font-weight="700" fill="#0f172a">Tombi! (PAL Spanish) &#8212; decompilation progress</text>' % FONT,
         '<text x="28" y="66" %s font-size="12.5" fill="#64748b">Updated %s &#183; bytes of game code matching byte for byte (Psy-Q library excluded, each function once)</text>' % (FONT, today)]
    levels = [(name.split(":")[0] + ": matching C", m, g, "match", "%.1f %%") for name, g, n, m, nm in rows]
    levels.append(("Whole game: matching C", whole[0], whole[1], "match", "%.2f %%"))
    levels.append(("MAIN0 + X000: named by us", tot["named"], tot["game"], "named", "%.1f %%"))
    y = 108
    for lab, val, den, key, fmt in levels:
        p = pct(val, den)
        bold = ' font-weight="700"' if lab.startswith("Whole") else ""
        o.append('<text x="28" y="%d" %s font-size="14"%s fill="#1e293b">%s</text>' % (y + 4, FONT, bold, esc(lab)))
        o.append('<rect x="330" y="%d" width="450" height="18" rx="9" fill="#e2e8f0"/>' % (y - 10))
        wbar = max(0 if p == 0 else 4, 450 * p / 100.0)
        if wbar: o.append('<rect x="330" y="%d" width="%.1f" height="18" rx="9" fill="%s"/>' % (y - 10, wbar, COL[key]))
        o.append(('<text x="796" y="%d" %s font-size="15" font-weight="700" fill="#0f172a">' + fmt + '</text>') % (y + 5, FONT, p))
        y += 36
    o.append("</svg>")
    open(N("docs", "progress.svg"), "w").write("\n".join(o))


def covered(items, prog, lib=False):
    """Game-code bytes (non-library items; `lib=True`: library items only) covered by matched ranges of `prog`; a
    matched range can span several Ghidra/splat functions or include library code, so count the overlap, never the
    raw marker size. Library matches (Psy-Q sources ported from psx_tomba) are reported apart and never enter the
    game-code percentage."""
    ranges = src_ranges().get(prog, [])
    tot = 0
    for a, size, kind in items:
        if (kind == "lib") != lib: continue
        for s0, n in ranges:
            lo, hi = max(a, s0), min(a + size, s0 + n)
            if hi > lo: tot += hi - lo
    return tot


def overlays():
    """Per area overlay: game bytes, matched bytes, functions, and the unmatched functions with their status:
    piece  identical to a Ghidra piece of X000 that lies inside a larger matched range: needs its own C
    x000   identical to an X000 function that is not matched yet (comes for free when X000's is)
    dup    identical to a function of an earlier overlay (listed there)
    new    no twin anywhere: the real new work."""
    base_cov = {p: src_ranges().get(p, []) for p in ("MAIN0", "X000")}
    def is_matched(prog, a, size):
        return sum(max(0, min(a + size, s + n) - max(a, s)) for s, n in src_ranges().get(prog, [])) >= size
    out, todo = {}, []
    for p in OVERLAYS + SYSOVL:
        rows = read_csv(N("notes", "functions_%s.csv" % p.lower()))
        d = dict(game=0, n=len(rows), matched=0, n_matched=0, piece=0, x000=0, dup=0, new=0, n_new=0,
                 uniq=0, uniq_matched=0, n_uniq=0, n_uniq_matched=0)
        for r in rows:
            a, size = int(r["address"], 16), int(r["size"])
            d["game"] += size
            cov = sum(max(0, min(a + size, s + n) - max(a, s)) for s, n in src_ranges().get(p, []))
            d["matched"] += min(cov, size)
            if not r.get("twin"):  # first copy of this code anywhere in the game
                d["uniq"] += size; d["n_uniq"] += 1
                d["uniq_matched"] += min(cov, size); d["n_uniq_matched"] += cov >= size
            if cov >= size: d["n_matched"] += 1; continue
            tw = r.get("twin") or ""
            if tw[:5] in ("MAIN0", "X000:") and is_matched(tw.split(":")[0], int(tw.split(":")[1], 16), size): st = "piece"
            elif tw.startswith("X000:"): st = "x000"
            elif tw: st = "dup"
            else: st = "new"; d["n_new"] += 1
            d[st] += size - cov
            todo.append((size, p, r["address"], r["name"], st, tw))
        out[p] = d
    todo.sort(key=lambda x: (-x[0], x[1], x[2]))
    with open(N("notes", "todo_areas.csv"), "w") as f:
        f.write("prog,address,size,name,status,twin\n")
        for size, p, a, name, st, tw in todo:
            f.write("%s,%s,%d,%s,%s,%s\n" % (p, a, size, name, st, tw))
    return out


def summary_lines(res, cov, ovl, tot, matching):
    """The headline figures, shared by docs/PROGRESS.md, README.md and the objdiff report of decomp.dev: they come
    from the unit list of tools/units.py (every src file with a MATCHING marker, plus the csv rows no file covers),
    where every function is counted once: the area overlays repeat a lot of MAIN0/X000 code (and each other's), so
    only their first copy enters the total."""
    libm = getattr(summary_lines, "libm", 0)  # matched library bytes (set by main), reported apart
    import units as U
    T_ = U.totals()
    m0, x0 = T_["MAIN0"], T_["X000"]
    ou, om, onu, onm = (sum(T_[p][i] for p in U.OVERLAYS) for i in (0, 1, 2, 3))
    su, sm, snu, snm = (sum(T_[p][i] for p in U.SYSOVL if p in T_) for i in (0, 1, 2, 3))
    rows = [("MAIN0.EXE: engine, shared by every area", m0[0], m0[2], m0[1], m0[3]),
            ("X000.BIN: AREA00 + object code reused by all areas", x0[0], x0[2], x0[1], x0[3]),
            ("X001..X019.BIN: code specific to the other areas", ou, onu, om, onm)]
    if su:
        rows.append(("SYSTEM/*.BIN: options, sub-screens, game over, info screens", su, snu, sm, snm))
    T = sum(r[1] for r in rows); M = sum(r[3] for r in rows); F = sum(r[2] for r in rows); FM = sum(r[4] for r in rows)
    summary_lines.whole, summary_lines.areas, summary_lines.rows = (M, T), (om, ou), rows
    L = ["**Whole game: %.2f %% of the game's code matches byte for byte** (%d of %d bytes)." % (pct(M, T), M, T), "",
         "| Part | Code | Matching bytes | Matching |", "|---|---:|---:|---|"]
    for name, g, n, m, nm in rows:
        L.append("| %s | %d B | %d B | %.1f %% `%s` |" % (name, g, m, pct(m, g), bar(pct(m, g))))
    L.append("| **Whole game** | **%d B** | **%d B** | **%.2f %%** `%s` |" % (T, M, pct(M, T), bar(pct(M, T))))
    L += ["",
          "- Bytes of machine code in game functions, counted exactly like the objdiff report on decomp.dev"
          " (`tools/units.py` is the common list); Sony's Psy-Q library (%d B) is not counted: %d B of it (%.1f %%) also"
          " matches, from Psy-Q sources ported from psx_tomba, and is tracked apart." % (tot["lib"], libm, pct(libm, tot["lib"])),
          "- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each",
          "  other; those copies match automatically and are not added again (counting every copy separately gives %.1f %%)." % (
              pct(matching + sum(d["matched"] for d in ovl.values()), tot["game"] + sum(d["game"] for d in ovl.values()))),
          "- Also tracked for MAIN0 + X000: %.1f %% of the code named by us, %.1f %% typed with the TObj structure." % (
              pct(tot["named"], tot["game"]), pct(tot["typed"], tot["game"]))]
    return L


def update_readme(summary):
    path = N("README.md")
    a, b = "<!-- progress:start -->", "<!-- progress:end -->"
    t = open(path).read()
    if a not in t: return
    i, j = t.index(a) + len(a), t.index(b)
    open(path, "w").write(t[:i] + "\n" + "\n".join(summary) + "\n" + t[j:])


def data_name_counts():
    """Rows of notes/data_names_<prog>.csv (globals named by usage) and the generic function names left (notes/unnamed.json)."""
    import glob
    sysn = {}
    n = {}
    for p in sorted(glob.glob(N("notes", "data_names_*.csv"))):
        prog = os.path.basename(p)[len("data_names_"):-4]
        n[prog] = sum(1 for _ in csv.DictReader(open(p)))
    for prog in sorted(n):
        if prog not in ("main0", "x000"): sysn[prog.upper()] = n[prog]
    try: unnamed = json.load(open(N("notes", "unnamed.json")))["count"]
    except Exception: unnamed = 0
    return dict(main0=n.get("main0", 0), x000=n.get("x000", 0), sys=sum(sysn.values()),
                sys_detail=", ".join("%s %d" % kv for kv in sysn.items()), unnamed=unnamed)


def main():
    res = {k: analyse(*v) for k, v in PROGRAMS.items()}
    tot = {key: sum(r[key] for r in res.values()) for key in
           ("lib", "named", "unnamed", "typed", "game", "n_game", "n_named", "n_unnamed", "n_typed", "n_lib", "excluded", "unnamed_typed")}
    cov = {k.split(".")[0]: covered(r["items"], k.split(".")[0]) for k, r in res.items()}
    libcov = {k.split(".")[0]: covered(r["items"], k.split(".")[0], lib=True) for k, r in res.items()}
    summary_lines.libm = sum(libcov.values())
    ovl = overlays()
    matching = sum(cov.values())
    levels = [
        ("Game functions with our own names", tot["named"], "bytes"),
        ("Game functions with types (TObj)", tot["typed"], "bytes"),
        ("'Matching' C in src/", matching, "bytes"),
    ]
    out = []
    w = out.append
    w("# Tombi! decompilation progress (PAL Spanish, SCES_013.31)\n")
    w("_Generated with `python tools/progress.py` (%s). Do not edit by hand._\n" % datetime.date.today().isoformat())
    og, om, on = (sum(d[k] for d in ovl.values()) for k in ("game", "matched", "n_matched"))
    summary = summary_lines(res, cov, ovl, tot, matching)
    w("## Summary\n")
    out.extend(summary)
    w("")
    update_readme(summary)
    w("\"Game code\" = functions inside the code of the analyzed programs, **excluding** those from")
    w("Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra %d bytes (%d functions).\n" % (tot["lib"], tot["n_lib"]))
    w("## Breakdown by program\n")
    w("| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library | Psy-Q matching |")
    w("|---|---|---|---|---|---|---|")
    md = matching_detail()
    for k, r in res.items():
        w("| %s | %d B (%d f) | %.1f %% | %.1f %% | %.1f %% (%d f) | %d B | %d B (%.1f %%) |" % (
            DISPLAY.get(k, k), r["game"], r["n_game"], pct(r["named"], r["game"]), pct(r["typed"], r["game"]),
            pct(cov[k.split(".")[0]], r["game"]), md.get(k.split(".")[0], [0, 0])[1], r["lib"],
            libcov[k.split(".")[0]], pct(libcov[k.split(".")[0]], r["lib"])))
    w("")
    w("## Area overlays (X001..X019) and SYSTEM overlays\n")
    w("Raw code blobs loaded at `0x800E8028` like X000 ([notes/overlays.md](../notes/overlays.md)). Boundaries come from")
    w("`tools/areas.py bounds` (`notes/functions_x0nn.csv`); about 173 KB of object code is shared by all of them (and X000)")
    w("at the same addresses, so most of it is matched by copying the X000/MAIN0 twin (`tools/areas.py twins`, sources")
    w("in `src/x0nn/`). Unmatched functions are listed in [notes/todo_areas.csv](../notes/todo_areas.csv) by status:")
    w("*x000* = identical to an X000 function not matched yet, *dup* = identical to a function of an earlier overlay,")
    w("*piece* = identical to a Ghidra piece of a larger matched X000 range (needs its own C),")
    w("*new* = no twin anywhere (the real new work). The SYSTEM overlays (`tools/sysovl.py`: O0PTSUB0, D0SPSUB*, G0OVER,")
    w("I0NFO*, also at `0x800E8028`, sources in `src/<name>/`) are linked from source by `tools/build_full.py`.\n")
    w("| Overlay | Areas | Code | Functions | Matching | Pending: x000 | dup | piece | new |")
    w("|---|---|---|---|---|---|---|---|---|")
    for p, d in ovl.items():
        w("| %s.BIN | %s | %d B | %d | %.1f %% (%d f) | %d B | %d B | %d B | %d B (%d f) |" % (
            p, OVL_AREAS.get(p, "SYSTEM" if p in SYSOVL else "AREA" + p[2:]), d["game"], d["n"], pct(d["matched"], d["game"]), d["n_matched"],
            d["x000"], d["dup"], d["piece"], d["new"], d["n_new"]))
    w("| **Total** | | %d B | %d | %.1f %% (%d f) | %d B | %d B | %d B | %d B (%d f) |" % (
        og, sum(d["n"] for d in ovl.values()), pct(om, og), on, sum(d["x000"] for d in ovl.values()),
        sum(d["dup"] for d in ovl.values()), sum(d["piece"] for d in ovl.values()), sum(d["new"] for d in ovl.values()), sum(d["n_new"] for d in ovl.values())))
    w("")
    w("The *Matching* column counts every copy, including the shared code matched through twins; the whole-game")
    w("figure in the summary counts each function once.\n")
    w("## What is NOT counted (the real denominator is larger)\n")
    w("- `MAIN1..8.EXE`: they share ~98 % of the code (`.text`) with MAIN0; they are treated as variants and not added.")
    w("- `SCES_013.31` (loader, 651 KB): almost all Psy-Q library; not analyzed.")
    w("- AREA07 and AREA12 reuse X001/X004, AREA15 has no code; X1nn..X8nn are other languages, and so are the")
    w("  SYSTEM overlays of the variants 1..8 (D1SPSUB ... I8NFO3; only the variant 0 of MAIN0 is counted).")
    w("- Functions called only by an overlay that Ghidra does not recognize, and overlays not yet identified")
    w("  (172 MAIN0 targets at `0x800E8000+` do not fall in X000).")
    w("- Functions outside the code (`%d` bytes of Ghidra false positives in RAM with no contents)." % tot["excluded"])
    w("")
    w("## Where things stand (October 2026)\n")
    w("- Code: the whole-game figure of the summary above (2950 of 2955 functions). What does not match is five functions of the")
    w("  memory card / sub-screen overlays (D0SPSUB func_800E89F8, func_800ED0C4, func_800EFD6C; D0SPSUB2 func_800E999C;")
    w("  D0SPSUB3 func_800EC89C, plus the D0SPSUB3 twin of func_800E89F8). Each has functionally equivalent C in `src/wip/`")
    w("  whose header documents the remaining difference (register allocation and scheduler order only, the structure matches);")
    w("  [notes/remaining.md](../notes/remaining.md) lists them.")
    dn = data_name_counts()
    w("- Function names: essentially complete; %d functions keep a generic name (`notes/unnamed.json`: empty stubs, unreferenced" % dn["unnamed"])
    w("  code and event scenes nothing pins a behaviour to).")
    w("- Data names: done by usage for MAIN0 (%d globals in `notes/data_names_main0.csv`), X000 (%d) and the eight SYSTEM" % (dn["main0"], dn["x000"]))
    w("  overlays (%d: %s), applied to the sources with `tools/rename_data.py`. What is still spelled by" % (dn["sys"], dn["sys_detail"]))
    w("  address: animation scripts, elements of named pointer tables, field aliases and Psy-Q internals.")
    w("- Other versions: the Japanese release (50.5 %) and demo (39.6 %) are paused at their shared-C state.")
    w("")
    w("## Plan\n")
    w("1. Match every game function byte for byte (this report; five D0SPSUB* functions left, see above).")
    w("2. All area overlays (done: 16 overlays wired in, see above).")
    w("3. Remove the matching debt (`do{}while(0)`, `register asm`, volatile pins, `// FLAGS`, asm fences).")
    w("4. Readable names and types (names done by usage, see above; structs instead of raw offsets pending); then split")
    w("   the sources into segment files (one per original .c) and link with the splat build.")
    w("5. Visibility: objdiff report on decomp.dev (done).")
    w("6. Assets: a decompilation is complete only when the whole disc can be rebuilt. Inventory the files of the")
    w("   disc (`tools/isoext.py` extracts it), document every format (area `X0nn.BIN` data, graphics, sound banks,")
    w("   STR movies, texts, `SCES_013.31` and `MAIN1..8`), write unpack/repack tools and a `make` target that")
    w("   produces a bootable image from sources. Starts when the code passes ~95 %.")
    w("")
    w("## Largest unnamed functions (next targets)\n")
    allun = []
    for k, r in res.items():
        for size, addr in r["unnamed_list"]:
            allun.append((size, addr, k.split()[0]))
    allun.sort(reverse=True)
    w("| Size | Address | Program |")
    w("|---|---|---|")
    for size, addr, k in allun[:15]:
        w("| %d B | `%s` | %s |" % (size, addr, k))
    w("")
    w("## How it is measured\n")
    w("- Unit: bytes of code per function (the size reported by Ghidra).")
    w("- *Named*: it is in `notes/names_*.csv` and is not a library function. *Typed*: its first parameter is `TObj *`.")
    w("- *Matching*: functions in `src/*.c` marked with `// MATCHING <address> <bytes>`; verified byte for byte")
    w("  against the retail executable with `tools/ncheck.py` / `tools/matchcheck.py` (relocations masked).")
    w("- The typing figures come from the latest Ghidra export (`notes/functions_*.csv`) and may lag behind.")
    open(N("docs", "PROGRESS.md"), "w").write("\n".join(out) + "\n")
    json.dump({"date": datetime.date.today().isoformat(), "totals": {k: v for k, v in tot.items()},
               "matching_bytes": matching,
               "programs": {k: {x: y for x, y in r.items() if x not in ("unnamed_list", "items")} for k, r in res.items()},
               "overlays": ovl},
              open(N("docs", "progress.json"), "w"), indent=2)
    write_svgs(res, tot, matching, summary_lines.whole, summary_lines.areas)
    print("\n".join(out[:16]))


if __name__ == "__main__":
    main()
