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

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
N = lambda *p: os.path.join(ROOT, *p)

# program -> (functions csv, names csv, code range [lo, hi])
# The keys are kept as-is because they are also the keys of "programs" in docs/progress.json;
# DISPLAY gives the English label used in PROGRESS.md and the SVGs.
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
            h = re.search(r"//\s*FUNC\s+[0-9a-fA-F]+\s+\d+[ \t]*(MAIN0|X0\d\d)?", t)
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
LAB = {"match": "Matching C", "named": "Named by us", "typed": "Unnamed, with TObj",
       "unnamed": "Unnamed, untyped", "lib": "Sony library (Psy-Q)"}
FONT = "font-family=\"Segoe UI,Helvetica,Arial,sans-serif\""


def esc(t):
    return t.replace("&", "&amp;").replace("<", "&lt;")


def write_svgs(res, tot, matching):
    hist_path = N("docs", "history.json")
    hist = json.load(open(hist_path)) if os.path.exists(hist_path) else []
    today = datetime.date.today().isoformat()
    point = {"date": today, "named": round(pct(tot["named"], tot["game"]), 2),
             "typed": round(pct(tot["typed"], tot["game"]), 2), "matching": round(pct(matching, tot["game"]), 2)}
    hist = [h for h in hist if h["date"] != today] + [point]
    json.dump(hist, open(hist_path, "w"), indent=1)

    W = 900
    o = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="440" viewBox="0 0 %d 440" role="img" aria-label="Decompilation progress">' % (W, W),
         '<rect width="%d" height="440" rx="14" fill="#ffffff" stroke="#e2e8f0"/>' % W,
         '<text x="28" y="44" %s font-size="22" font-weight="700" fill="#0f172a">Tombi! (PAL Spanish) &#8212; decompilation progress</text>' % FONT,
         '<text x="28" y="66" %s font-size="12.5" fill="#64748b">Updated %s &#183; unit: bytes of game code (excluding the Psy-Q library)</text>' % (FONT, today)]
    # levels
    levels = [("Matching (C that recompiles identically)", matching, "match"), ("Functions with our own names", tot["named"], "named"),
              ("With the TObj structure applied (coverage)", tot["typed"], "typed")]
    y = 108
    for lab, val, key in levels:
        p = pct(val, tot["game"])
        o.append('<text x="28" y="%d" %s font-size="14" fill="#1e293b">%s</text>' % (y + 4, FONT, esc(lab)))
        o.append('<rect x="330" y="%d" width="450" height="18" rx="9" fill="#e2e8f0"/>' % (y - 10))
        wbar = max(0 if p == 0 else 4, 450 * p / 100.0)
        if wbar: o.append('<rect x="330" y="%d" width="%.1f" height="18" rx="9" fill="%s"/>' % (y - 10, wbar, COL[key]))
        o.append('<text x="796" y="%d" %s font-size="15" font-weight="700" fill="#0f172a">%.1f %%</text>' % (y + 5, FONT, p))
        y += 36
    # composition
    o.append('<text x="28" y="%d" %s font-size="15" font-weight="700" fill="#0f172a">Composition of the analyzed code</text>' % (y + 26, FONT))
    y += 52
    rows = [(k, r) for k, r in res.items()] + [("TOTAL", None)]
    for k, r in rows:
        if r is None:
            seg = {"lib": tot["lib"], "named": tot["named"], "typed": tot["unnamed_typed"] if "unnamed_typed" in tot else 0,
                   "unnamed": tot["unnamed"] - tot.get("unnamed_typed", 0), "match": matching}
        else:
            seg = {"lib": r["lib"], "named": r["named"], "typed": r["unnamed_typed"],
                   "unnamed": r["unnamed"] - r["unnamed_typed"], "match": 0}
        totb = sum(seg.values()) or 1
        o.append('<text x="28" y="%d" %s font-size="13" fill="#1e293b">%s</text>' % (y + 14, FONT, esc(DISPLAY.get(k, k))))
        x = 330.0
        for key in ("match", "named", "typed", "unnamed", "lib"):
            wseg = 520.0 * seg[key] / totb
            if wseg > 0:
                o.append('<rect x="%.1f" y="%d" width="%.1f" height="22" fill="%s"><title>%s: %d bytes</title></rect>' % (x, y, wseg, COL[key], LAB[key], seg[key]))
            x += wseg
        o.append('<text x="858" y="%d" %s font-size="12" fill="#64748b" text-anchor="start">%d KB</text>' % (y + 15, FONT, totb // 1024))
        y += 34
    # legend
    x = 28
    y += 8
    for key in ("match", "named", "typed", "unnamed", "lib"):
        o.append('<rect x="%d" y="%d" width="13" height="13" rx="3" fill="%s"/>' % (x, y, COL[key]))
        o.append('<text x="%d" y="%d" %s font-size="12" fill="#334155">%s</text>' % (x + 19, y + 11, FONT, esc(LAB[key])))
        x += 19 + 7 * len(LAB[key]) + 16
    o.append('<text x="28" y="%d" %s font-size="11.5" fill="#94a3b8">MAIN0 + X000 only: SCES_013.31 and MAIN1-8 (~98 %% shared with MAIN0) are not counted; the other area overlays are in PROGRESS.md.</text>' % (y + 38, FONT))
    o.append("</svg>")
    open(N("docs", "progress.svg"), "w").write("\n".join(o))

    # memory maps per program
    for key, (name, r) in zip(("main0", "x000"), res.items()):
        lo, hi = r["range"]
        name = DISPLAY.get(name, name)
        ROW = 0x4000                      # 16 KB per row
        rowsn = (hi - lo) // ROW + 1
        wpx = 880.0
        h = 62 + rowsn * 16 + 28
        m = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" viewBox="0 0 %d %d" role="img" aria-label="Map of %s">' % (W, h, W, h, esc(name)),
             '<rect width="%d" height="%d" rx="14" fill="#ffffff" stroke="#e2e8f0"/>' % (W, h),
             '<text x="28" y="36" %s font-size="18" font-weight="700" fill="#0f172a">Function map &#8212; %s</text>' % (FONT, esc(name)),
             '<text x="28" y="54" %s font-size="12" fill="#64748b">Each row = 16 KB from 0x%08X. Each block = one function, colored by its status. Light gray = data or gap.</text>' % (FONT, lo)]
        for i in range(rowsn):
            yy = 66 + i * 16
            m.append('<rect x="10" y="%d" width="%.1f" height="13" fill="#f1f5f9"/>' % (yy, wpx))
        for a, size, st in sorted(r["items"]):
            row, col = divmod(a - lo, ROW)
            while size > 0 and row < rowsn:
                part = min(size, ROW - col)
                x0 = 10 + wpx * col / ROW
                ww = max(1.0, wpx * part / ROW)
                m.append('<rect x="%.1f" y="%d" width="%.1f" height="13" fill="%s"><title>0x%08X %d B &#183; %s</title></rect>' % (x0, 66 + row * 16, ww, COL[st], a, size, LAB[st]))
                size -= part; row += 1; col = 0
        m.append("</svg>")
        open(N("docs", "map_%s.svg" % key), "w").write("\n".join(m))


def covered(items, prog):
    """Game-code bytes (non-library items) covered by matched ranges of `prog`; a matched range can span several
    Ghidra/splat functions or include library code, so count the overlap, never the raw marker size."""
    ranges = src_ranges().get(prog, [])
    tot = 0
    for a, size, kind in items:
        if kind == "lib": continue
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
    for p in OVERLAYS:
        rows = read_csv(N("notes", "functions_%s.csv" % p.lower()))
        d = dict(game=0, n=len(rows), matched=0, n_matched=0, piece=0, x000=0, dup=0, new=0, n_new=0)
        for r in rows:
            a, size = int(r["address"], 16), int(r["size"])
            d["game"] += size
            cov = sum(max(0, min(a + size, s + n) - max(a, s)) for s, n in src_ranges().get(p, []))
            d["matched"] += min(cov, size)
            if cov >= size: d["n_matched"] += 1; continue
            tw = r.get("twin") or ""
            if tw[:5] in ("MAIN0", "X000:") and is_matched(tw.split(":")[0], int(tw.split(":")[1], 16), size): st = "piece"
            elif tw.startswith("X000:"): st = "x000"
            elif tw.startswith("X0"): st = "dup"
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


def main():
    res = {k: analyse(*v) for k, v in PROGRAMS.items()}
    tot = {key: sum(r[key] for r in res.values()) for key in
           ("lib", "named", "unnamed", "typed", "game", "n_game", "n_named", "n_unnamed", "n_typed", "n_lib", "excluded", "unnamed_typed")}
    cov = {k.split(".")[0]: covered(r["items"], k.split(".")[0]) for k, r in res.items()}
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
    w("## Summary\n")
    w("| Level | Progress | Bytes | Functions |")
    w("|---|---|---|---|")
    w("| **C that matches byte for byte (matching)** | **%.1f %%** `%s` | %d / %d | %d |" % (pct(matching, tot["game"]), bar(pct(matching, tot["game"])), matching, tot["game"], sum(matching_detail()[k][1] for k in ("MAIN0", "X000"))))
    w("| Named by us | %.1f %% `%s` | %d / %d | %d / %d |" % (pct(tot["named"], tot["game"]), bar(pct(tot["named"], tot["game"])), tot["named"], tot["game"], tot["n_named"], tot["n_game"]))
    w("| With the TObj structure applied (coverage, not C progress) | %.1f %% `%s` | %d / %d | %d / %d |" % (pct(tot["typed"], tot["game"]), bar(pct(tot["typed"], tot["game"])), tot["typed"], tot["game"], tot["n_typed"], tot["n_game"]))
    og, om, on = sum(d["game"] for d in ovl.values()), sum(d["matched"] for d in ovl.values()), sum(d["n_matched"] for d in ovl.values())
    w("| Matching, all 18 programs (MAIN0 + X000 + the 16 other area overlays) | %.1f %% `%s` | %d / %d | %d |" % (
        pct(matching + om, tot["game"] + og), bar(pct(matching + om, tot["game"] + og)), matching + om, tot["game"] + og,
        sum(matching_detail()[k][1] for k in ("MAIN0", "X000")) + on))
    w("")
    w("\"Game code\" = functions inside the code of the analyzed programs, **excluding** those from")
    w("Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra %d bytes (%d functions).\n" % (tot["lib"], tot["n_lib"]))
    w("## Breakdown by program\n")
    w("| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library |")
    w("|---|---|---|---|---|---|")
    md = matching_detail()
    for k, r in res.items():
        w("| %s | %d B (%d f) | %.1f %% | %.1f %% | %.1f %% (%d f) | %d B |" % (
            DISPLAY.get(k, k), r["game"], r["n_game"], pct(r["named"], r["game"]), pct(r["typed"], r["game"]),
            pct(cov[k.split(".")[0]], r["game"]), md.get(k.split(".")[0], [0, 0])[1], r["lib"]))
    w("")
    w("## Area overlays (X001..X019)\n")
    w("Raw code blobs loaded at `0x800E8028` like X000 ([notes/overlays.md](../notes/overlays.md)). Boundaries come from")
    w("`tools/areas.py bounds` (`notes/functions_x0nn.csv`); about 173 KB of object code is shared by all of them (and X000)")
    w("at the same addresses, so most of it is matched by copying the X000/MAIN0 twin (`tools/areas.py twins`, sources")
    w("in `src/x0nn/`). Unmatched functions are listed in [notes/todo_areas.csv](../notes/todo_areas.csv) by status:")
    w("*x000* = identical to an X000 function not matched yet, *dup* = identical to a function of an earlier overlay,")
    w("*piece* = identical to a Ghidra piece of a larger matched X000 range (needs its own C),")
    w("*new* = no twin anywhere (the real new work).\n")
    w("| Overlay | Areas | Code | Functions | Matching | Pending: x000 | dup | piece | new |")
    w("|---|---|---|---|---|---|---|---|---|")
    for p, d in ovl.items():
        w("| %s.BIN | %s | %d B | %d | %.1f %% (%d f) | %d B | %d B | %d B | %d B (%d f) |" % (
            p, OVL_AREAS.get(p, "AREA" + p[2:]), d["game"], d["n"], pct(d["matched"], d["game"]), d["n_matched"],
            d["x000"], d["dup"], d["piece"], d["new"], d["n_new"]))
    w("| **Total** | | %d B | %d | %.1f %% (%d f) | %d B | %d B | %d B | %d B (%d f) |" % (
        og, sum(d["n"] for d in ovl.values()), pct(om, og), on, sum(d["x000"] for d in ovl.values()),
        sum(d["dup"] for d in ovl.values()), sum(d["piece"] for d in ovl.values()), sum(d["new"] for d in ovl.values()), sum(d["n_new"] for d in ovl.values())))
    w("")
    w("Grand total, all programs: **%d / %d B matching (%.1f %%)**; core (MAIN0 + X000) %.1f %%, area overlays %.1f %%.\n" % (
        matching + om, tot["game"] + og, pct(matching + om, tot["game"] + og), pct(matching, tot["game"]), pct(om, og)))
    w("## What is NOT counted (the real denominator is larger)\n")
    w("- `MAIN1..8.EXE`: they share ~98 % of the code (`.text`) with MAIN0; they are treated as variants and not added.")
    w("- `SCES_013.31` (loader, 651 KB): almost all Psy-Q library; not analyzed.")
    w("- The overlays are only counted in the \"area overlays\" section and the all-programs row: the summary above is MAIN0 + X000.")
    w("  AREA07 and AREA12 reuse X001/X004, AREA15 has no code; X1nn..X8nn are other languages.")
    w("- Functions called only by an overlay that Ghidra does not recognize, and overlays not yet identified")
    w("  (172 MAIN0 targets at `0x800E8000+` do not fall in X000).")
    w("- Functions outside the code (`%d` bytes of Ghidra false positives in RAM with no contents)." % tot["excluded"])
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
    write_svgs(res, tot, matching)
    print("\n".join(out[:16]))


if __name__ == "__main__":
    main()
