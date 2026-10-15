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
LINKED = LINKED + tuple(SYSOVL)
AS = ["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0",
      "-I" + os.path.join(ROOT, "include", "tomba")]


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw)


# Non-PAL versions (tools/versions.py): slpm80165 = Japanese demo disc (SLPM-80165, decomp.dev version SLPM_801.65),
# slps01144 = Japanese retail (SLPS-01144, version SLPS_011.44). config/<v>/<prog>.yaml splits the version's game dir;
# one unit per function, built from the shared src/ tree compiled with -DVERSION_<SERIAL>. Output: build/<v>/report.json.
from versions import VERSIONS, REFERENCE, canon


def version_lines(serial):
    """{(prog, addr): (src path, size)} of the `// VERSION <SERIAL> <prog> <addr> <size>` lines of the shared src tree:
    the file whose `#if defined(VERSION_<SERIAL>)` branch (or whole guarded body) builds that version function."""
    out = {}
    rx = re.compile(r"^//\s*VERSION\s+" + serial + r"\s+(\w+)\s+([0-9A-Fa-f]{8})\s+(\d+)", re.M)
    for dp, dirs, fs in os.walk(os.path.join(ROOT, "src")):
        dirs[:] = [d for d in dirs if d != "wip"]
        for fn in sorted(fs):
            if not fn.endswith(".c"): continue
            path = os.path.join(dp, fn)
            t = open(path, errors="replace").read()
            if "VERSION" not in t: continue
            for m in rx.finditer(t):
                out[(m.group(1), int(m.group(2), 16))] = (path, int(m.group(3)))
    return out


def jpdemo_sources(progs, ver="slpm80165"):
    """{(prog, demo addr): (src path, demo name)}: the C that builds each demo function.

    By default a demo function reuses the PAL file that matches the PAL function tools/xmatch.py paired it with: the
    `PAL <module> <address>` comment of config/<ver>/symbol_addrs_<prog>.txt is looked up by address among the src
    files (tools/units.py), so PAL renames do not break it; the file must cover exactly the demo function's size.
    config/<ver>/reuse.txt overrides or adds pairs, one per line: `<prog> <demo addr> <PAL module> <PAL addr>` or
    `<prog> <demo addr> -` (no C). A function that differs from PAL is built from the shared file that carries a
    `// VERSION <SERIAL> <prog> <addr> <size>` line (its `#if defined(VERSION_<SERIAL>)` branch: version C, or
    INCLUDE_ASM, which compiles to nothing here, so the unit gets no base object; see tools/version_share.py).
    """
    import units as U
    pal = {}
    for p, us in U.units().items():
        for u in us:
            if u.file: pal[(p.lower(), u.addr)] = u
    out, names = {}, {}
    for prog in progs:
        for l in open("config/%s/symbol_addrs_%s.txt" % (ver, prog)):
            m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+); // type:func size:0x([0-9A-Fa-f]+)\s+PAL (\w+) 0x([0-9A-Fa-f]+)", l)
            if not m: continue
            a = int(m.group(2), 16)
            names[(prog, a)] = (m.group(1), int(m.group(3), 16))
            u = pal.get((m.group(4), int(m.group(5), 16)))
            if u and u.size == int(m.group(3), 16): out[(prog, a)] = (u.file, m.group(1))
    if os.path.exists("config/%s/reuse.txt" % ver):
        for l in open("config/%s/reuse.txt" % ver):
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
    for d in ("FLAGS", "CC", "HEADERS", "AS"):
        m = re.search(r"^//\s*%s\.%s\s+(.*)$" % (d, serial), t, re.M)
        if not m: continue
        t = re.sub(r"(?m)^//\s*%s\s.*\n?" % d, "", t)
        val = m.group(1).strip()
        t = re.sub(r"(?m)^//\s*%s\.%s\s.*$" % (d, serial), "" if val == "default" else "// %s %s" % (d, val), t)
    out = os.path.join(tmpdir, os.path.basename(src))
    open(out, "w").write(t)
    return out


def _jpdemo_spad_set(text):
    """`.set` lines that make PAL scratchpad symbols (config/slpm80165/spad.txt) constants, since the demo target keeps
    every scratchpad address literal (config/slpm80165/symbol_addrs_manual.txt). Demo C may also name any scratchpad
    address D_1F80xxxx."""
    S = _jpdemo_spad_set.__dict__
    if "tab" not in S:
        p = os.path.join(ROOT, "config", "slpm80165", "spad.txt")
        S["tab"] = [l.split()[:2] for l in open(p) if l.strip() and not l.startswith("#")] if os.path.exists(p) else []
    tab = dict(S["tab"])
    tab.update((n, n[2:]) for n in re.findall(r"\bD_1F80[0-9A-F]{4}\b", text))
    return "".join("\t.set\t%s,0x%s\n" % (n, a) for n, a in sorted(tab.items()))


def _jpdemo_compile(job):
    src, out, name, inc, serial = job
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
    NC["EXTRA_AS"] = _jpdemo_spad_set(t)
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
        if len(funcs) == 1:  # the only function: give it the whole .text (trailing alignment nops), like splat does
            e = Elf(d + "/a.o")
            y = next(y for y in e.syms if y["type"] == 2 and y["shndx"] == text)
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
        jobs.append((src, out, name, inc, VERSIONS[ver]["serial"])); want.append((u, out))
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
    """Data sections of the demo report, as for PAL (tools/report_data.py): claimed sections on the units with C,
    the other non-code bytes of each program in a target-only <prog>/data unit."""
    import report_data as RD
    dtmp = tempfile.mkdtemp()
    shutil.rmtree(out_dir + "/base_data", ignore_errors=True)
    for prog in progs:
        y = open("config/%s/%s.yaml" % (ver, prog)).read()
        game = open(re.search(r"target_path:\s*(\S+)", y).group(1), "rb").read()
        vram = int(re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", y).group(1), 16)
        hdr = 0x800 if "type: header" in y else 0
        gbase = vram - hdr
        code = [(a, a + n) for nm, (a, n) in addrs.items() if nm.startswith(prog + "/") and a is not None]
        if hdr: code.append((gbase, vram))
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
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n' + body
                        + RD.target_asm(dclaims, game, gbase, 0, dtmp))
            try:
                run(AS + [t.name, "-o", u["target_path"]])
            except subprocess.CalledProcessError as er:
                print("target data asm failed:", u["name"], er.stderr[:120]); continue
            finally:
                os.unlink(t.name)
            u["base_path"] = dbase
            claimed += [(b, b + m) for b, m in dclaims.values()]
        rest = "%s/target/%s/data.o" % (out_dir, prog)
        n = RD.rest_object(rest, game, gbase, 0, code, claimed, AS)
        print("data %s: %d bytes claimed by C units, %d bytes left" % (prog, sum(b - a for a, b in claimed), n))
        if n: units.append({"name": "%s/data" % prog, "target_path": rest, "metadata": {"progress_categories": [prog]}})
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
        for fn in sorted(os.listdir(fdir)):
            if not fn.endswith(".s"): continue
            name = fn[:-2]
            out = "%s/target/%s/%s.o" % (out_dir, prog, name)
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as t:
                t.write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n'
                        + open(os.path.join(fdir, fn)).read())
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
    # by tools/build_full.py (src file of MAIN0 / X000 / the SYSTEM overlays; the area overlays are not linked from source yet) and matches
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
