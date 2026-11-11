#!/usr/bin/env python3
"""Builds the objdiff inputs and the progress report used by decomp.dev.

Steps (all output under build/ and asm/, both ignored by git):
  1. splat splits the retail binaries (config/main0.yaml, config/x000.yaml, config/x0nn.yaml for the area
     overlays, config/<sysovl>.yaml for the SYSTEM overlays of tools/sysovl.py) into one .s per function,
     named after our C functions (tools/gen_symbols.py writes the symbol files first).
  2. Each retail function is assembled into a *target* object  -> build/target/<prog>/<func>.o
  3. Each src/*.c and src/x0nn/*.c is compiled with the native toolchain (tools/ncheck.py) -> build/base/<prog>/<func>.o
     (honoring a `// CC gcc-X.Y.Z` header line: old-gcc of that version, see ncheck.gcc_for)
  4. objdiff.json is written with one unit per unit of tools/units.py (categories: game / sdk, main0 / x000 / x0nn):
     the same list docs/PROGRESS.md counts, so both figures agree by construction.
     Data (tools/report_data.py): each unit also carries the data sections its C object would get placed at their
     retail address (bytes compared after relocation; a unit left out of the code figures, a twin or a Psy-Q
     function without --with-sdk, still reports that data as a data-only unit <prog>/<name>.data, since those bytes
     are this program's own), and one target-only unit <prog>/data holds every other
     non-code byte of the program, so matched_data / total_data is the share of the retail data C really reproduces.
  5. objdiff-cli writes build/report.json.

Usage: python3 tools/build_report.py [--no-split] [--with-sdk] [--objdiff /path/to/objdiff-cli]
       python3 tools/build_report.py --version slps01144|slpm80165 [--objdiff ...]   (Japanese retail / demo disc,
       see jpdemo_main; the old names jp / jpdemo are accepted too; sces01331 / pal is the default PAL report)
By default the Psy-Q library functions are left out, so the report (and the decomp.dev headline) measures game code only,
like docs/PROGRESS.md; --with-sdk adds them under the "sdk" category. Overlay functions that are twins of X000 (or of an
earlier overlay) are left out too, so shared code is counted once and the total equals the whole-game figure of PROGRESS.md.
Order: base objects are compiled first (gen_symbols reads them), then splat, then the target objects.
"""
import csv, json, os, re, shutil, struct, subprocess, sys, tempfile

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
PROGS_VRAM = {"MAIN0": 0x80010000}  # first byte after the file header (U.BINS gives the address of file offset 0)
# SYSTEM code overlays (tools/sysovl.py): src/<prog lower>/, linked by tools/build_full.py like MAIN0 / X000
import sysovl as _SO
SYSOVL = [p.lower() for p, (path, sha, code) in _SO.SYSOVL.items() if os.path.exists(os.path.join(ROOT, "game", path))]
PROGS.update({p: p.upper() for p in SYSOVL})
LINKED = LINKED + tuple(SYSOVL) + tuple(OVERLAYS)  # area overlays: linked from src/x0nn/ by build_full too
AS = ["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0",
      "-I" + os.path.join(ROOT, "include", "tomba")]


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw)


# Non-PAL versions (tools/versions.py): slpm80165 = Japanese demo disc (SLPM-80165, decomp.dev version SLPM_801.65),
# slps01144 = Japanese retail (SLPS-01144, version SLPS_011.44). config/<v>/<prog>.yaml splits the version's game dir;
# one unit per function, built from the shared src/ tree compiled with -DVERSION_<SERIAL>. Output: build/<v>/report.json.
from versions import VERSIONS, REFERENCE, canon
# the source resolution of a version function lives in tools/units.py (shared with tools/build_full.py --version)
from units import version_lines, version_source, version_sources as jpdemo_sources, spad_set as _jpdemo_spad_set


def _jpdemo_compile(job):
    src, out, name, inc, serial = job[:5]
    tsize = job[5] if len(job) > 5 else 0
    NC = _jpdemo_compile.__dict__.setdefault("nc", {})
    if not NC:
        s = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
        NC.update({"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"})
        exec(compile(s, "ncheck", "exec"), NC)
    # the shared src file, compiled for this version: VERSION_<SERIAL> picks its branch; SKIP_ASM makes an INCLUDE_ASM
    # branch compile to nothing (that function is not matched by C in this version, so the unit gets no base object)
    NC["EXTRA_CPP"] = ["-DVERSION_" + serial, "-DSKIP_ASM"]
    d = tempfile.mkdtemp()
    src = version_source(src, serial, d)
    t = open(src, errors="replace").read()
    NC["EXTRA_AS"] = _jpdemo_spad_set(t, serial)
    fm = re.search(r"//\s*FLAGS\s+(.*)", t)
    flags = fm.group(1).strip() if fm else "-O2 -G0"
    try:
        NC["build"](src, flags, d, inc)
        # the function at .text offset 0 takes the demo name, so objdiff pairs it with the target symbol
        from build_full import Elf
        e = Elf(d + "/a.o")
        text = e.idx.get(".text")
        funcs = [y for y in e.syms if y["type"] == 2 and y["shndx"] == text]
        first = sorted((y["size"] == 0, y["bind"] != 1, y["name"]) for y in funcs if y["value"] == 0)
        if not first: return "INCLUDE_ASM"  # no C for this function in this version
        first = first[0][2]
        if first != name:
            subprocess.run(["mipsel-linux-gnu-objcopy", "--redefine-sym", "%s=%s" % (name, name + "_pal"),
                            "--redefine-sym", "%s=%s" % (first, name), d + "/a.o"], check=True)
        whole = len(funcs) > 1 and tsize and tsize == e.size(".text")
        if len(funcs) == 1 or whole:
            # the only function: give it the whole .text (trailing alignment nops), like splat does; or several PAL
            # functions that the version target splits as one function of the same total size (e.g. x00 8011C1E4)
            e = Elf(d + "/a.o")
            y = next(y for y in e.syms if y["type"] == 2 and y["shndx"] == text and y["name"] == name)
            struct.pack_into("<I", e.d, y["off"] + 8, e.size(".text"))
            e.save(d + "/a.o")
        os.makedirs(os.path.dirname(out), exist_ok=True)
        shutil.copy(d + "/a.o", out)
        return None
    except subprocess.CalledProcessError as e:
        return str(e.stderr)[-200:]
    finally:
        shutil.rmtree(d, ignore_errors=True)


def jpdemo_base(progs, units, out_dir, ver="slpm80165"):
    """Compiles the C of every version unit that has one (jpdemo_sources) into build/<ver>/base/; sets base_path."""
    from multiprocessing import Pool
    srcs = jpdemo_sources(progs, ver)
    inc = tempfile.mkdtemp()
    for f in os.listdir("include"):
        s = os.path.join("include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(inc, nm))
    shutil.rmtree(out_dir + "/base", ignore_errors=True)
    jobs, want = [], []
    for u in units:
        prog, name = u["name"].split("/", 1)
        a = u.get("addr")
        if a is None or (prog, a) not in srcs: continue
        src, _ = srcs[(prog, a)]
        out = "%s/base/%s/%s.o" % (out_dir, prog, name)
        jobs.append((src, out, name, inc, VERSIONS[ver]["serial"], u.get("size", 0))); want.append((u, out))
    with Pool(os.cpu_count() or 4) as pool:
        res = pool.map(_jpdemo_compile, jobs)
    n = nasm = 0
    for (u, out), r, j in zip(want, res, jobs):
        if r is None: u["base_path"] = out; n += 1
        elif r == "INCLUDE_ASM": nasm += 1
        else: print(ver, "base compile failed:", u["name"], os.path.relpath(j[0], ROOT), r)
    print("%s: %d units left to INCLUDE_ASM by their shared src file" % (ver, nasm))
    shutil.rmtree(inc, ignore_errors=True)
    for u in units: u.pop("addr", None)
    return n


def jpdemo_data(progs, units, addrs, out_dir, ver="slpm80165"):
    """Data sections of the version report, as for PAL (tools/report_data.py): claimed sections on the units with C;
    then the PAL data files the version has too (tools/version_data.py: same bytes, pointers moved), then the version's
    own data files (src/data/<ver>/<prog>/*.c, `// DATA addr size <ver>/<prog>`, written by tools/version_data.py gen);
    the other non-code bytes of each program go in a target-only <prog>/data unit. The X00 code and data that every
    area overlay repeats at its start (tools/versions.py, last field of a program) are X00's: counted once, in x00, like
    the code. build/<ver>/data_rest.json lists the ranges left per program (input of tools/version_data.py gen)."""
    import report_data as RD, version_data as VD
    from versions import pal_prog
    dtmp = tempfile.mkdtemp()
    shutil.rmtree(out_dir + "/base_data", ignore_errors=True)
    mods = {p: pal_prog(ver, p) for p in progs}
    pal_files = VD.compile_pal(sorted(set(mods.values())), out_dir + "/pal_data")
    own_files = VD.compile_version(ver, progs, out_dir + "/ver_data")
    maps, placed, rest_json = VD.Maps(ver), {}, {}
    data_units = []

    def data_unit(name, obj, info, game, gbase, prog):
        """Target: the retail bytes of the placed section; base: the object relocated to the version's addresses."""
        out = "%s/target/%s/data/%s.o" % (out_dir, prog, name)
        dbase = "%s/base_data/%s/data/%s.o" % (out_dir, prog, name)
        for f in (out, dbase): os.makedirs(os.path.dirname(f), exist_ok=True)
        dclaims = RD.patch_base(obj, dbase, info)
        with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
            t.write('.include "macro.inc"\n.set noat\n.set noreorder\n' + RD.target_asm(dclaims, game, gbase, 0, dtmp))
        try:
            run(AS + [t.name, "-o", out])
        except subprocess.CalledProcessError as er:
            print("target data asm failed:", prog, name, er.stderr[:120]); return None
        finally:
            os.unlink(t.name)
        data_units.append({"name": "%s/data/%s" % (prog, name), "target_path": out, "base_path": dbase,
                           "metadata": {"progress_categories": [prog]}})
        return [(b, b + m) for b, m in dclaims.values()]

    for prog in sorted(progs, key=lambda p: mods[p] != "main0"):  # main exe first: overlay data points into it
        game, gbase, excl = VD.prog_layout(ver, prog)
        code = [(a, a + n) for nm, (a, n) in addrs.items() if nm.startswith(prog + "/") and a is not None] + excl
        claimed = []
        for u in units:
            if not u["name"].startswith(prog + "/"): continue
            body = u.pop("asm", "")
            a, n = addrs[u["name"]]
            if "base_path" not in u or a is None: continue
            info = RD.claim(u["name"], u["base_path"], a, n, game, gbase, 0)
            if not info: continue
            e = RD.BF.Elf(u["base_path"])
            rng = [(b, b + e.size(sec)) for sec, b in info["place"].items()]
            if any(b < d and c < f for b, f in rng for c, d in code + claimed): continue
            dbase = u["base_path"].replace("/base/", "/base_data/", 1)
            os.makedirs(os.path.dirname(dbase), exist_ok=True)
            dclaims = RD.patch_base(u["base_path"], dbase, info)
            with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
                # same scratchpad `.set` constants as the plain target build below (the code is reassembled here)
                spad = "".join("\t.set\t%s,0x%s\n" % (n, n[2:]) for n in sorted(set(re.findall(r"\bD_1F80[0-9A-F]{4}\b", body))))
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n' + spad + '.section .text\n' + body
                        + RD.target_asm(dclaims, game, gbase, 0, dtmp))
            try:
                run(AS + [t.name, "-o", u["target_path"]])
            except subprocess.CalledProcessError as er:
                print("target data asm failed:", u["name"], er.stderr[:120]); continue
            finally:
                os.unlink(t.name)
            u["base_path"] = dbase
            claimed += [(b, b + m) for b, m in dclaims.values()]
        nunit = sum(b - a for a, b in claimed)
        # PAL data files found in this program
        pl = VD.Placer(maps, mods[prog], prog, game, gbase, code + claimed, placed)
        npal = 0
        for path, stem, pa, size, obj, va, info, ninf in pl.run(pal_files.get(mods[prog], [])):
            r = data_unit("D_%08X" % va, obj, info, game, gbase, prog)
            if r: claimed += r; npal += size
        # the version's own data files
        nown = 0
        for path, stem, va, size, obj in own_files.get(prog, []):
            pl.busy = sorted(code + claimed)
            why, info = pl.check_at(obj, va, size)
            if why:
                print("%s: %s not reported: %s" % (ver, os.path.relpath(path, ROOT), why)); continue
            r = data_unit(stem, obj, info, game, gbase, prog)
            if r: claimed += r; nown += size
        rest = "%s/target/%s/data.o" % (out_dir, prog)
        n = RD.rest_object(rest, game, gbase, 0, code, claimed, AS)
        rest_json[prog] = VD.gaps(gbase, gbase + len(game), code + claimed)
        print("data %s: %d bytes claimed by C units, %d by PAL data files, %d by version data files, %d bytes left"
              % (prog, nunit, npal, nown, n))
        if n: data_units.append({"name": "%s/data" % prog, "target_path": rest, "metadata": {"progress_categories": [prog]}})
    units += data_units
    json.dump(rest_json, open(out_dir + "/data_rest.json", "w"))
    for u in units: u.pop("size", None); u.pop("asm", None)
    shutil.rmtree(dtmp, ignore_errors=True)


def jpdemo_main(ver="slpm80165"):
    os.chdir(ROOT)
    names = {p: v[0] for p, v in VERSIONS[ver]["progs"].items()}
    progs = [p for p in names if os.path.exists("config/%s/%s.yaml" % (ver, p))]
    if not progs:
        sys.exit("config/%s/ not set up" % ver)
    units, out_dir = [], "build/" + ver
    shutil.rmtree(out_dir + "/target", ignore_errors=True)
    for prog in progs:
        if "--no-split" not in ARGS:
            shutil.rmtree("asm/%s/%s" % (ver, prog), ignore_errors=True)
            shutil.rmtree("%s/splat_src/%s" % (out_dir, prog), ignore_errors=True)
            r = subprocess.run([sys.executable, "-m", "splat", "split", "config/%s/%s.yaml" % (ver, prog),
                                "--disassemble-all"], capture_output=True, text=True)
            if r.returncode:
                sys.exit("splat failed for %s %s:\n%s" % (ver, prog, (r.stdout + r.stderr)[-3000:]))
        fdir = "asm/%s/%s/nonmatchings/%s" % (ver, prog, prog)
        dfile = "config/%s/data_units.txt" % ver  # splat 'functions' that are data (tools/jp_data_units.py): not code units
        dunits = {(l.split()[0], int(l.split()[1], 16)) for l in (open(dfile) if os.path.exists(dfile) else ())
                  if l.strip() and not l.startswith("#")}
        for fn in sorted(os.listdir(fdir)):
            if not fn.endswith(".s"): continue
            if (prog, int((re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", open(os.path.join(fdir, fn)).read()) or [0, "0"])[1], 16)) in dunits: continue
            name = fn[:-2]
            out = "%s/target/%s/%s.o" % (out_dir, prog, name)
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
                src_s = open(os.path.join(fdir, fn)).read()
                # scratchpad addresses are fixed: assemble them as constants, like _jpdemo_spad_set does on the C side
                # (splat names some of them D_1F80xxxx in the Japanese retail target)
                spad = "".join("\t.set\t%s,0x%s\n" % (n, n[2:]) for n in sorted(set(re.findall(r"\bD_1F80[0-9A-F]{4}\b", src_s))))
                # handwritten/data-as-code functions can hold R4000 opcodes (dsll32, sd...): allow them, the bytes are identical
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n' + (".set mips3\n" if re.search(r"\b(dsll32|dsrl32|dsra32|dsll|dsrl|dsra|dadd[u]?|ddiv[u]?|dmult[u]?|sd|ld)\b", src_s) else "") + spad + '.section .text\n' + src_s)
            try:
                run(AS + [t.name, "-o", out])
            except subprocess.CalledProcessError as e:
                print("target asm failed:", prog, name, e.stderr[:120]); continue
            finally:
                os.unlink(t.name)
            body = open(os.path.join(fdir, fn)).read()
            am = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", body)
            sm = re.search(r"nonmatching\s+\S+,\s*0x([0-9A-Fa-f]+)", body)
            units.append({"name": "%s/%s" % (prog, name), "target_path": out, "addr": int(am.group(1), 16) if am else None,
                          "size": int(sm.group(1), 16) if sm else 0, "asm": body,
                          "metadata": {"progress_categories": [prog]}})
    addrs = {u["name"]: (u["addr"], u["size"]) for u in units}
    nbase = jpdemo_base(progs, units, out_dir, ver)
    jpdemo_data(progs, units, addrs, out_dir, ver)
    cfg = {"$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
           "build_base": False, "build_target": False,
           "progress_categories": [{"id": p, "name": names[p]} for p in progs],
           "options": {"functionRelocDiffs": "none"},
           "units": units}
    # objdiff reads objdiff.json from the project dir: keep each version's project in build/<ver>/ apart from the PAL one
    for u in units:
        for k in ("target_path", "base_path"):
            if k in u: u[k] = os.path.relpath(u[k], out_dir)
    json.dump(cfg, open(out_dir + "/objdiff.json", "w"), indent=1)
    print("%s units %d, base objects %d" % (ver, len(units), nbase))
    subprocess.run([OBJDIFF, "report", "generate", "-p", out_dir, "-o", out_dir + "/report.json"], check=True)
    print("wrote %s/report.json" % out_dir)


def main():
    if "--version" in ARGS:
        v = canon(ARGS[ARGS.index("--version") + 1])
        if v != REFERENCE: return jpdemo_main(v)
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
    base, srcof, nb = {}, {}, 0
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
                shutil.copy(d + "/a.o", out); base[(prog, u.addr)] = out; srcof[(prog, u.addr)] = u.file; nb += 1
            except subprocess.CalledProcessError:
                print("base compile failed:", u.file)
            shutil.rmtree(d, ignore_errors=True)
    # data-only files (src/data/<prog>/*.c, see tools/build_full.py data_files)
    import build_full as BFM
    dfiles = {}
    for prog in PROGS:
        for path, stem, addr, size in BFM.data_files(prog):
            d = tempfile.mkdtemp()
            try:
                NC["build"](path, "-O2 -G0", d, inc)
                out = "build/base/%s/data/%s.o" % (prog, stem)
                os.makedirs(os.path.dirname(out), exist_ok=True)
                shutil.copy(d + "/a.o", out); nb += 1
                dfiles.setdefault(prog, []).append((path, stem, addr, size, out))
            except subprocess.CalledProcessError:
                print("base compile failed:", path)
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
    import report_data as RD
    units, nt, ndata = [], 0, {}
    shutil.rmtree("build/target", ignore_errors=True)
    shutil.rmtree("build/base_data", ignore_errors=True)
    dtmp = tempfile.mkdtemp()
    for prog in PROGS:
        P = PROGS[prog]
        if P not in U.units(): continue
        gpath, gbase = U.BINS[P]
        game = open(gpath, "rb").read()
        code = [(u.addr, u.addr + u.size) for u in U.units()[P]]
        if gbase != PROGS_VRAM.get(P, gbase): code.append((gbase, PROGS_VRAM[P]))  # PS-X EXE header
        claimed = []
        fdir = "asm/%s/nonmatchings/%s" % (prog, prog)
        asm = {}  # address -> body
        for fn in sorted(os.listdir(fdir)):
            body = open(os.path.join(fdir, fn)).read()
            m = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", body)
            if m: asm[int(m.group(1), 16)] = body
        addrs = sorted(asm)
        for u in U.units()[P]:
            # a unit left out of the code figures (twin of an earlier program's function, or Psy-Q without --with-sdk)
            # still owns the data its C places in *this* program: reported as a data-only unit (no .text in the target)
            data_only = not (U.counted(u) or (u.lib and not u.twin and WITH_SDK))
            if data_only and (P, u.addr) not in base: continue
            parts = [asm[a] for a in addrs if u.addr <= a < u.addr + u.size]
            if not parts: continue  # data (zero words) or a range splat merged elsewhere
            out = "build/target/%s/%s.o" % (prog, u.name)
            os.makedirs(os.path.dirname(out), exist_ok=True)
            # data sections: the ones build_full would place at their retail address (see tools/report_data.py)
            dclaims, dbase, extra = {}, None, ""
            if (P, u.addr) in base:
                info = RD.claim(srcof[(P, u.addr)], base[(P, u.addr)], u.addr, u.size, game, gbase, 0)
                if info:
                    rng = [(a, a + RD.BF.Elf(base[(P, u.addr)]).size(sec)) for sec, a in info["place"].items()]
                    if not any(a < d and c < b for a, b in rng for c, d in code + claimed):
                        dbase = "build/base_data/%s/%s.o" % (prog, u.name)
                        os.makedirs(os.path.dirname(dbase), exist_ok=True)
                        dclaims = RD.patch_base(base[(P, u.addr)], dbase, info)
                        extra = RD.target_asm(dclaims, game, gbase, 0, dtmp)
            if data_only:
                if not dclaims: continue
                parts = []
            # splat pairs a plain `addiu rX, rY, 0x20` after a non-relocated `lui 0x1f80` into a lone %lo(D_1F800020):
            # with no %hi of that symbol in the function it is just a constant offset (our C emits the number)
            def _lone_lo(body):
                his = set(re.findall(r"%hi\((D_1F80[0-9A-F]{4})\)", body))
                return re.sub(r"%lo\((D_1F80([0-9A-F]{4}))\)",
                              lambda m: m.group(0) if m.group(1) in his else "0x" + m.group(2), body)
            parts = [_lone_lo(p) for p in parts]
            with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n' + "\n".join(parts) + extra)
            try:
                run(AS + [t.name, "-o", out]); nt += 1
            except subprocess.CalledProcessError as e:
                print("target asm failed:", prog, u.name, e.stderr[:120]); continue
            finally:
                os.unlink(t.name)
            claimed += [(a, a + n) for a, n in dclaims.values()]
            unit = {"name": "%s/%s%s" % (prog, u.name, ".data" if data_only else ""), "target_path": out,
                    "metadata": {"progress_categories": [prog, "sdk" if u.lib else "game"]}}
            if (P, u.addr) in base: unit["base_path"] = dbase or base[(P, u.addr)]
            units.append(unit)
        for path, stem, addr, size, obj in dfiles.get(prog, []):
            info = RD.claim_data(path, obj, addr, size, prog, game, gbase, 0)
            if not info: continue
            if any(addr < d and c < addr + size for c, d in code + claimed):
                print("data file %s overlaps code or data claimed by another unit: not reported" % path); continue
            out = "build/target/%s/data/%s.o" % (prog, stem)
            dbase = "build/base_data/%s/data/%s.o" % (prog, stem)
            for f in (out, dbase): os.makedirs(os.path.dirname(f), exist_ok=True)
            dclaims = RD.patch_base(obj, dbase, info)
            with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n' + RD.target_asm(dclaims, game, gbase, 0, dtmp))
            try:
                run(AS + [t.name, "-o", out]); nt += 1
            except subprocess.CalledProcessError as e:
                print("target asm failed:", path, e.stderr[:120]); continue
            finally:
                os.unlink(t.name)
            claimed += [(a, a + n) for a, n in dclaims.values()]
            units.append({"name": "%s/data/%s" % (prog, stem), "target_path": out, "base_path": dbase,
                          "metadata": {"progress_categories": [prog, "game"]}})
        # every other non-code byte of the program: one target-only unit (unmatched until C claims it)
        rest = "build/target/%s/data.o" % prog
        n = RD.rest_object(rest, game, gbase, 0, code, claimed, AS)
        ndata[prog] = (sum(b - a for a, b in claimed), n)
        if n:
            units.append({"name": "%s/data" % prog, "target_path": rest,
                          "metadata": {"progress_categories": [prog, "game"]}})
    shutil.rmtree(dtmp, ignore_errors=True)
    for prog, (c, n) in ndata.items():
        print("data %s: %d bytes claimed by C units, %d bytes left" % (prog, c, n))
    cfg = {"$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
           "build_base": False, "build_target": False,
           "progress_categories": [{"id": "game", "name": "Game code"}, {"id": "sdk", "name": "Psy-Q SDK"},
                                   {"id": "main0", "name": "MAIN0.EXE"}, {"id": "x000", "name": "X000.BIN (AREA00)"}]
                                  + [{"id": p, "name": "%s.BIN (AREA%s)" % (p.upper(), p[2:])} for p in OVERLAYS]
                                  + [{"id": p, "name": "%s.BIN (SYSTEM)" % p.upper()} for p in SYSOVL],
           "options": {"functionRelocDiffs": "none"},
           "units": units}
    json.dump(cfg, open("objdiff.json", "w"), indent=1)
    print("units %d, target objects %d, base objects %d" % (len(units), nt, nb))
    gen = lambda: subprocess.run([OBJDIFF, "report", "generate", "-p", ".", "-o", "build/report.json"], check=True)
    gen()
    # "complete" (decomp.dev "fully linked"): a unit counts only when its object is really linked into the executable
    # by tools/build_full.py (src file of MAIN0 / X000 / the area overlays / the SYSTEM overlays) and matches
    # 100 % of its code and of the data sections it claims (tools/report_data.py: the ones build_full places at their
    # retail address; data the C does not define still comes from splat and stays in <prog>/data). Needs the first pass.
    rep = {u["name"]: u for u in json.load(open("build/report.json"))["units"]}
    ncomplete = 0
    for unit in units:
        prog = unit["name"].split("/")[0]
        m = rep.get(unit["name"], {}).get("measures", {})
        if (prog in LINKED and "base_path" in unit and m.get("matched_code_percent") == 100.0 and int(m.get("total_code", 0))
                and int(m.get("matched_data", 0)) == int(m.get("total_data", 0))):
            unit["metadata"]["complete"] = True; ncomplete += 1
    json.dump(cfg, open("objdiff.json", "w"), indent=1)
    gen()
    print("wrote build/report.json (%d complete units)" % ncomplete)


if __name__ == "__main__":
    main()
