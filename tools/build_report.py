#!/usr/bin/env python3
"""Builds the objdiff inputs and the progress report used by decomp.dev.

Steps (all output under build/ and asm/, both ignored by git):
  1. splat splits the retail binaries (config/main0.yaml, config/x000.yaml, config/x0nn.yaml for the area
     overlays) into one .s per function,
     named after our C functions (tools/gen_symbols.py writes the symbol files first).
  2. Each retail function is assembled into a *target* object  -> build/target/<prog>/<func>.o
  3. Each src/*.c and src/x0nn/*.c is compiled with the native toolchain (tools/ncheck.py) -> build/base/<prog>/<func>.o
     (honoring a `// CC gcc-X.Y.Z` header line: old-gcc of that version, see ncheck.gcc_for)
  4. objdiff.json is written with one unit per unit of tools/units.py (categories: game / sdk, main0 / x000 / x0nn):
     the same list docs/PROGRESS.md counts, so both figures agree by construction.
  5. objdiff-cli writes build/report.json.

Usage: python3 tools/build_report.py [--no-split] [--with-sdk] [--objdiff /path/to/objdiff-cli]
By default the Psy-Q library functions are left out, so the report (and the decomp.dev headline) measures game code only,
like docs/PROGRESS.md; --with-sdk adds them under the "sdk" category. Overlay functions that are twins of X000 (or of an
earlier overlay) are left out too, so shared code is counted once and the total equals the whole-game figure of PROGRESS.md.
Order: base objects are compiled first (gen_symbols reads them), then splat, then the target objects.
"""
import csv, json, os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
ARGS = sys.argv[1:]
OBJDIFF = ARGS[ARGS.index("--objdiff") + 1] if "--objdiff" in ARGS else os.environ.get("OBJDIFF", "objdiff-cli")
WITH_SDK = "--with-sdk" in ARGS  # also report the Psy-Q library units (category "sdk"); default: game code only
LINKED = ("main0", "x000")  # programs whose C objects tools/build_full.py links into the executable
PROGS = {"main0": "MAIN0", "x000": "X000"}
OVERLAYS = ["x%03d" % n for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]  # area overlays, src/x0nn/
# Overlays whose retail file is missing (legacy game zip with only MAIN0/X000) are left out of the report.
OVERLAYS = [p for p in OVERLAYS if os.path.exists(os.path.join(ROOT, "game", "AREA" + p[2:], p.upper() + ".BIN"))]
PROGS.update({p: p.upper() for p in OVERLAYS})
AS = ["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0",
      "-I" + os.path.join(ROOT, "include", "tomba")]


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw)


def main():
    os.chdir(ROOT)
    import units as U
    # base objects first: compile every src/*.c and src/x0nn/*.c with the native pipeline (tools/gen_symbols.py
    # reads them to give the inner functions of a block-matched file their own symbols)
    src_nc = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
    NC = {"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"}
    exec(compile(src_nc, "ncheck", "exec"), NC)
    inc = tempfile.mkdtemp()
    for f in os.listdir("include"):
        s = os.path.join("include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(inc, nm))
    base, nb = {}, 0
    shutil.rmtree("build/base", ignore_errors=True)
    for prog, us in U.units().items():
        for u in us:
            if not u.file: continue
            h = NC["M"]["header"](u.file)
            if not h: continue
            d = tempfile.mkdtemp()
            try:
                NC["build"](u.file, h[3], d, inc)
                out = "build/base/%s/%s.o" % (prog.lower(), u.name)
                os.makedirs(os.path.dirname(out), exist_ok=True)
                shutil.copy(d + "/a.o", out); base[(prog, u.addr)] = out; nb += 1
            except subprocess.CalledProcessError:
                print("base compile failed:", u.file)
            shutil.rmtree(d, ignore_errors=True)
    shutil.rmtree(inc, ignore_errors=True)
    run([sys.executable, "tools/gen_symbols.py"])
    if "--no-split" not in ARGS:
        for prog in PROGS:
            shutil.rmtree("asm/" + prog, ignore_errors=True)
            shutil.rmtree("build/splat_src/" + prog, ignore_errors=True)  # stale C stubs make splat skip functions
            r = subprocess.run([sys.executable, "-m", "splat", "split", "config/%s.yaml" % prog, "--disassemble-all"],
                               capture_output=True, text=True)
            if r.returncode:
                sys.exit("splat failed for %s:\n%s" % (prog, (r.stdout + r.stderr)[-3000:]))
    # target objects: one per unit, assembled from the splat function(s) inside the unit's range
    units, nt = [], 0
    shutil.rmtree("build/target", ignore_errors=True)
    for prog in PROGS:
        P = PROGS[prog]
        if P not in U.units(): continue
        fdir = "asm/%s/nonmatchings/%s" % (prog, prog)
        asm = {}  # address -> body
        for fn in sorted(os.listdir(fdir)):
            body = open(os.path.join(fdir, fn)).read()
            m = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", body)
            if m: asm[int(m.group(1), 16)] = body
        addrs = sorted(asm)
        for u in U.units()[P]:
            if not (U.counted(u) or (u.lib and not u.twin and WITH_SDK)): continue
            parts = [asm[a] for a in addrs if u.addr <= a < u.addr + u.size]
            if not parts: continue  # data (zero words) or a range splat merged elsewhere
            out = "build/target/%s/%s.o" % (prog, u.name)
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n' + "\n".join(parts))
            try:
                run(AS + [t.name, "-o", out]); nt += 1
            except subprocess.CalledProcessError as e:
                print("target asm failed:", prog, u.name, e.stderr[:120]); continue
            finally:
                os.unlink(t.name)
            unit = {"name": "%s/%s" % (prog, u.name), "target_path": out,
                    "metadata": {"progress_categories": [prog, "sdk" if u.lib else "game"]}}
            if (P, u.addr) in base: unit["base_path"] = base[(P, u.addr)]
            units.append(unit)
    cfg = {"$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
           "build_base": False, "build_target": False,
           "progress_categories": [{"id": "game", "name": "Game code"}, {"id": "sdk", "name": "Psy-Q SDK"},
                                   {"id": "main0", "name": "MAIN0.EXE"}, {"id": "x000", "name": "X000.BIN (AREA00)"}]
                                  + [{"id": p, "name": "%s.BIN (AREA%s)" % (p.upper(), p[2:])} for p in OVERLAYS],
           "options": {"functionRelocDiffs": "none"},
           "units": units}
    json.dump(cfg, open("objdiff.json", "w"), indent=1)
    print("units %d, target objects %d, base objects %d" % (len(units), nt, nb))
    gen = lambda: subprocess.run([OBJDIFF, "report", "generate", "-p", ".", "-o", "build/report.json"], check=True)
    gen()
    # "complete" (decomp.dev "fully linked"): a unit counts only when its object is really linked into the executable
    # by tools/build_full.py (src file of MAIN0 / X000; the area overlays are not linked from source yet) and matches
    # 100 % of its code. Its .data/.rodata still come from the retail binary (splat), so no data is claimed: the
    # units carry no data sections, hence complete_data stays 100 % of 0 bytes. Needs the first pass for the match.
    rep = {u["name"]: u for u in json.load(open("build/report.json"))["units"]}
    ncomplete = 0
    for unit in units:
        prog = unit["name"].split("/")[0]
        m = rep.get(unit["name"], {}).get("measures", {})
        if prog in LINKED and "base_path" in unit and m.get("matched_code_percent") == 100.0 and int(m.get("total_code", 0)):
            unit["metadata"]["complete"] = True; ncomplete += 1
    json.dump(cfg, open("objdiff.json", "w"), indent=1)
    gen()
    print("wrote build/report.json (%d complete units)" % ncomplete)


if __name__ == "__main__":
    main()
