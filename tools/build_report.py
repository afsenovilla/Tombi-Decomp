#!/usr/bin/env python3
"""Builds the objdiff inputs and the progress report used by decomp.dev.

Steps (all output under build/ and asm/, both ignored by git):
  1. splat splits the retail binaries (config/main0.yaml, config/x000.yaml, config/x0nn.yaml for the area
     overlays) into one .s per function,
     named after our C functions (tools/gen_symbols.py writes the symbol files first).
  2. Each retail function is assembled into a *target* object  -> build/target/<prog>/<func>.o
  3. Each src/*.c and src/x0nn/*.c is compiled with the native toolchain (tools/ncheck.py) -> build/base/<prog>/<func>.o
     (honoring a `// CC gcc-X.Y.Z` header line: old-gcc of that version, see ncheck.gcc_for)
  4. objdiff.json is written with one unit per function (categories: game / sdk, main0 / x000).
  5. objdiff-cli writes build/report.json.

Usage: python3 tools/build_report.py [--no-split] [--objdiff /path/to/objdiff-cli]
"""
import csv, json, os, re, shutil, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
ARGS = sys.argv[1:]
OBJDIFF = ARGS[ARGS.index("--objdiff") + 1] if "--objdiff" in ARGS else os.environ.get("OBJDIFF", "objdiff-cli")
PROGS = {"main0": "MAIN0", "x000": "X000"}
OVERLAYS = ["x%03d" % n for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]  # area overlays, src/x0nn/
# Overlays whose retail file is missing (legacy game zip with only MAIN0/X000) are left out of the report.
OVERLAYS = [p for p in OVERLAYS if os.path.exists(os.path.join(ROOT, "game", "AREA" + p[1:], p.upper() + ".BIN"))]
PROGS.update({p: p.upper() for p in OVERLAYS})
AS = ["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0",
      "-I" + os.path.join(ROOT, "include", "tomba")]


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw)


def game_addresses():
    """Addresses of game (non-SDK) functions: the todo list plus everything already matched."""
    game = {r["address"] for r in csv.DictReader(open(os.path.join(ROOT, "notes", "todo_match.csv")))}
    for f in os.listdir(os.path.join(ROOT, "src")):
        if f.endswith(".c"):
            m = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)", open(os.path.join(ROOT, "src", f), errors="replace").read())
            if m: game.add(m.group(1).lower())
    return game


def main():
    os.chdir(ROOT)
    run([sys.executable, "tools/gen_symbols.py"])
    if "--no-split" not in ARGS:
        for prog in PROGS:
            shutil.rmtree("asm/" + prog, ignore_errors=True)
            shutil.rmtree("build/splat_src/" + prog, ignore_errors=True)  # stale C stubs make splat skip functions
            r = subprocess.run([sys.executable, "-m", "splat", "split", "config/%s.yaml" % prog, "--disassemble-all"],
                               capture_output=True, text=True)
            if r.returncode:
                sys.exit("splat failed for %s:\n%s" % (prog, (r.stdout + r.stderr)[-3000:]))
    game = game_addresses()
    units, nt, nb = [], 0, 0
    # base objects: compile every src/*.c with the native pipeline
    import importlib.util
    spec = importlib.util.spec_from_file_location("ncheck_mod", os.path.join(ROOT, "tools", "ncheck.py"))
    src_nc = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
    NC = {"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"}
    exec(compile(src_nc, "ncheck", "exec"), NC)
    inc = tempfile.mkdtemp()
    for f in os.listdir("include"):
        s = os.path.join("include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(inc, nm))
    base = {}
    srcs = [os.path.join("src", f) for f in sorted(os.listdir("src"))]
    for p in OVERLAYS:
        if os.path.isdir("src/" + p): srcs += [os.path.join("src", p, f) for f in sorted(os.listdir("src/" + p))]
    for path in srcs:
        f = os.path.basename(path)
        if not f.endswith(".c"): continue
        h = NC["M"]["header"](path)
        if not h: continue
        prog = h[2].lower()
        d = tempfile.mkdtemp()
        try:
            NC["build"](path, h[3], d, inc)
            out = "build/base/%s/%s.o" % (prog, f[:-2])
            os.makedirs(os.path.dirname(out), exist_ok=True)
            shutil.copy(d + "/a.o", out); base[(prog, f[:-2])] = out; nb += 1
        except subprocess.CalledProcessError:
            print("base compile failed:", f)
        shutil.rmtree(d, ignore_errors=True)
    # target objects: assemble each retail function
    for prog in PROGS:
        fdir = "asm/%s/nonmatchings/%s" % (prog, prog)
        for fn in sorted(os.listdir(fdir)):
            name = fn[:-2]
            body = open(os.path.join(fdir, fn)).read()
            m = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", body)
            addr = m.group(1).lower() if m else ""
            out = "build/target/%s/%s.o" % (prog, name)
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n' + body)
            try:
                run(AS + [t.name, "-o", out]); nt += 1
            except subprocess.CalledProcessError as e:
                print("target asm failed:", prog, name, e.stderr[:120]); continue
            finally:
                os.unlink(t.name)
            unit = {"name": "%s/%s" % (prog, name), "target_path": out,
                    "metadata": {"progress_categories": [prog, "game" if (addr in game or prog in OVERLAYS) else "sdk"]}}
            if (prog, name) in base: unit["base_path"] = base[(prog, name)]
            units.append(unit)
    cfg = {"$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
           "build_base": False, "build_target": False,
           "progress_categories": [{"id": "game", "name": "Game code"}, {"id": "sdk", "name": "Psy-Q SDK"},
                                   {"id": "main0", "name": "MAIN0.EXE"}, {"id": "x000", "name": "X000.BIN (AREA00)"}]
                                  + [{"id": p, "name": "%s.BIN (AREA%s)" % (p.upper(), p[2:])} for p in OVERLAYS],
           "options": {"functionRelocDiffs": "none"},
           "units": units}
    shutil.rmtree(inc, ignore_errors=True)
    json.dump(cfg, open("objdiff.json", "w"), indent=1)
    print("units %d, target objects %d, base objects %d" % (len(units), nt, nb))
    subprocess.run([OBJDIFF, "report", "generate", "-p", ".", "-o", "build/report.json"], check=True)
    print("wrote build/report.json")


main()
