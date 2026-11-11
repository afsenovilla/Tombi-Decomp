#!/usr/bin/env python3
"""Full build: links MAIN0.EXE and X000.BIN from the matched C (src/*.c) plus splat assembly for
everything else, and checks the result against the retail files byte for byte.

How it works (all output under build/full/, ignored by git):
  1. tools/gen_symbols.py names the splat symbols after our C functions; splat 0.35.2 splits
     game/MAIN0.EXE and game/AREA00/X000.BIN (config/*.yaml, rewritten to build/full/*.yaml) into one
     .s per function plus the .rodata/.data files.
  2. Every src/*.c is compiled with the native pipeline of tools/ncheck.py (cpp -> cc1 -> maspsx -> as),
     with the old-gcc its `// CC gcc-X.Y.Z` header line names (default gcc-2.7.2; see ncheck.gcc_for).
     The `// FUNC addr size PROG` header is the truth: the C .text replaces every splat line inside
     [addr, addr+size), even when splat had cut that range into several functions.
  3. Data sections of a C object:
       - sections referenced through the section symbol (string literals, jump tables, static data) are
         placed at their original address, which is derived from the retail bytes of the relocated
         instructions (lui/addiu pairs); the splat lines of that range are dropped;
       - sections that only define globals: placed at their retail address when the code's uses of those
         globals all agree on one section address and the section has the retail bytes there (data moved
         into C); otherwise (data a ported header/file defines again) dropped and their symbols turned into
         undefined references, so the real data from splat is used.
     Every undefined symbol of a C object gets its address from the retail bytes too (checked for
     consistency, and against the splat label of the same name).
  4. All remaining splat lines go into one assembly file per program, cut into one section per
     contiguous range (`.org` keeps every line at its address); a linker script puts these sections and
     the C objects in address order.
     Data-only files, src/data/<prog>/*.c with a `// DATA addr size PROG` header instead of `// FUNC` (tables no
     code of ours references, e.g. animation scripts reached through other tables): the one data section the file
     defines is placed at addr, with the same checks (retail bytes, relocations resolved from the retail words); an
     extern it names must be that address: a D_/func_<addr> name, or a symbol of config/symbol_addrs_*.txt there.
  5. ld + objcopy produce build/full/MAIN0.EXE (with the PS-X EXE header) and build/full/X000.BIN,
     copied to build/MAIN0.EXE and build/X000.BIN, and compared with the retail files.

Also links the area overlays AREAnn/X0nn.BIN (src/x0nn/*.c + splat) and the SYSTEM code overlays of tools/sysovl.py
(src/<prog>/*.c + splat), each checked by its sha1 (the overlays are skipped when their retail file is missing).

Other versions (--version scus94236, tools/versions.py; output build/<ver>/full/): every program of config/<ver>/*.yaml,
each function at its version address from the src file tools/units.py version_sources gives it (PAL pairing, reuse.txt,
`// VERSION` lines; built with -DVERSION_<SERIAL>), plus the version's own data files (src/data/<ver>/<prog>/*.c). An
area overlay links only its area part; the X00 prefix it repeats comes from the X00 linked first. After each program a
name audit (build/<ver>/full/<prog>.names.txt, see name_audit) lists the relocation names whose meaning disagrees with
the address the retail bytes give them.

Usage: python3 tools/build_full.py [--version V] [--no-split] [-j N] [--asm-only] [--keep-going] [--only prog,prog]
  --no-split   reuse the previous splat output in build/full/asm
  --asm-only   ignore src/ (pure splat assembly: checks the asm path alone)
Requirements: the same as tools/ncheck.py (tools/setup_native.sh, including include/tomba/psyq: refused without it)
and splat 0.35.2 (tools/requirements.txt). The C files that did not compile (linked from asm) are listed in
<out>/compile_failures.txt; port/tools/gen_port.py refuses a link with any.
"""
import hashlib, os, re, shutil, struct, subprocess, sys, tempfile
from multiprocessing import Pool

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "build", "full")
ARGS = sys.argv[1:]
JOBS = int(ARGS[ARGS.index("-j") + 1]) if "-j" in ARGS else (os.cpu_count() or 4)
BIN = "mipsel-linux-gnu-"
AS = [BIN + "as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0"]
PROGS = {
    "main0": dict(name="MAIN0", game="game/MAIN0.EXE", vram=0x80010000, size=0x89000, header=0x800,
                  sha1="bec8dc5f4116f5e497f2337eef8c523ff6dee8fc", out="MAIN0.EXE"),
    "x000": dict(name="X000", game="game/AREA00/X000.BIN", vram=0x800E8028, size=0x53F50, header=0,
                 sha1="ce168475f667f2ca8842eabad2aec829e301c5c5", out="X000.BIN"),
}
# SYSTEM code overlays (tools/sysovl.py), linked from src/<prog lower>/*.c + splat, each checked by its sha1
sys.path.insert(0, os.path.join(ROOT, "tools"))
import sysovl as _SO
for _p, (_path, _sha, _code) in _SO.SYSOVL.items():
    _f = os.path.join(ROOT, "game", _path)
    PROGS[_p.lower()] = dict(name=_p, game="game/" + _path, vram=_SO.BASE, size=os.path.getsize(_f) if os.path.exists(_f) else 0,
                             header=0, sha1=_sha, out=_p + ".BIN", src="src/" + _p.lower())
# area overlays (AREAnn/X0nn.BIN), linked from src/x0nn/*.c + splat like the SYSTEM overlays; skipped when missing
for _n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19):
    _p = "x%03d" % _n
    _f = os.path.join(ROOT, "game", "AREA%02d" % _n, _p.upper() + ".BIN")
    _sha = re.search(r"^sha1: (\w+)", open(os.path.join(ROOT, "config", _p + ".yaml")).read(), re.M).group(1)
    PROGS[_p] = dict(name=_p.upper(), game="game/AREA%02d/%s.BIN" % (_n, _p.upper()), vram=0x800E8028,
                     size=os.path.getsize(_f) if os.path.exists(_f) else 0, header=0, sha1=_sha,
                     out=_p.upper() + ".BIN", src="src/" + _p)
if "--only" in ARGS:  # --only g0over,i0nfo: build just these programs (quick iteration)
    _keep = ARGS[ARGS.index("--only") + 1].lower().split(",")
    PROGS = {k: v for k, v in PROGS.items() if k in _keep}
# Another version (--version, tools/versions.py; set by setup_version from main): None = PAL (the reference).
VER = SERIAL = VEXE = None  # VEXE: the version's boot exe program (first in tools/versions.py)
PREFIX_PROG = "x00"  # the program whose first bytes every area overlay of a version repeats (its `[0x0, bin, xNN_shared]`)
ALLDEFS = {}         # version: {prog: {name: address}} of the globals the linked C defines (name audit)


def setup_version(ver):
    """--version <ver>: the programs of config/<ver>/<prog>.yaml (tools/versions.py order), output build/<ver>/full.
    Each program is linked from its code segment on (`type: code` start/vram); before it comes the PS-X EXE header
    (header segment, from splat) or, for an area overlay, the X00 prefix: the first bytes of the X00 linked here."""
    global OUT, PROGS, VER, SERIAL, VEXE
    from versions import VERSIONS
    import units as U
    VER, SERIAL, VEXE = ver, VERSIONS[ver]["serial"], U.version_progs(ver)[0]
    OUT = os.path.join(ROOT, "build", ver, "full")
    PROGS = {}
    for p in U.version_progs(ver):
        y = open(os.path.join(ROOT, "config", ver, p + ".yaml")).read()
        game = re.search(r"target_path:\s*(\S+)", y).group(1)
        m = re.search(r"type:\s*code\s*\n\s*start:\s*(0x[0-9A-Fa-f]+)\s*\n\s*vram:\s*(0x[0-9A-Fa-f]+)", y)
        start, vram = int(m.group(1), 16), int(m.group(2), 16)
        f = os.path.join(ROOT, game)
        PROGS[p] = dict(name=p.upper(), game=game, vram=vram, size=os.path.getsize(f) - start if os.path.exists(f) else 0,
                        header=start, head="asm" if re.search(r"type:\s*header", y) else ("prefix" if start else None),
                        sha1=re.search(r"^sha1:\s*(\w+)", y, re.M).group(1), out=os.path.basename(game))
    if "--only" in ARGS:
        keep = ARGS[ARGS.index("--only") + 1].lower().split(",")
        PROGS = {k: v for k, v in PROGS.items() if k in keep}


DATA_SECS = (".rodata", ".data", ".bss", ".sdata", ".sbss", ".rdata")


def run(cmd, **kw):
    r = subprocess.run(cmd, capture_output=True, text=True, **kw)
    if r.returncode:
        sys.exit("command failed: %s\n%s" % (" ".join(cmd[:6]), (r.stdout + r.stderr)[-3000:]))
    return r.stdout


# ---------------------------------------------------------------------------------------------- ELF
class Elf:
    """Minimal ELF32 little-endian relocatable reader/patcher (sections, symbols, REL relocations)."""

    def __init__(self, path):
        self.path = path
        self.d = bytearray(open(path, "rb").read())
        d = self.d
        shoff, = struct.unpack_from("<I", d, 0x20)
        shnum, shstrndx = struct.unpack_from("<HH", d, 0x30)
        self.sh = [list(struct.unpack_from("<10I", d, shoff + 40 * i)) for i in range(shnum)]
        self.shoff = shoff
        st = self.sh[shstrndx]
        self.names = [self._str(st[4], s[0]) for s in self.sh]
        self.idx = {n: i for i, n in enumerate(self.names)}
        self.symtab = next(i for i, s in enumerate(self.sh) if s[1] == 2)
        sy = self.sh[self.symtab]
        self.syms = []
        for k in range(sy[5] // 16):
            nm, val, size, info, other, shndx = struct.unpack_from("<IIIBBH", d, sy[4] + 16 * k)
            self.syms.append(dict(name=self._str(self.sh[sy[6]][4], nm), value=val, size=size, bind=info >> 4,
                                  type=info & 15, shndx=shndx, off=sy[4] + 16 * k))

    def _str(self, base, off):
        e = self.d.index(b"\0", base + off)
        return self.d[base + off:e].decode()

    def size(self, name):
        return self.sh[self.idx[name]][5] if name in self.idx else 0

    def data(self, name):
        s = self.sh[self.idx[name]]
        return bytes(self.d[s[4]:s[4] + s[5]]) if s[1] != 8 else bytes(s[5])

    def relocs(self, name):
        """[(offset, type, symbol dict)] of the REL section that applies to section `name`."""
        if name not in self.idx: return []
        t = self.idx[name]
        out = []
        for s in self.sh:
            if s[1] == 9 and s[7] == t:
                for k in range(s[5] // 8):
                    off, info = struct.unpack_from("<II", self.d, s[4] + 8 * k)
                    out.append((off, info & 0xFF, self.syms[info >> 8]))
        return out

    def undefine(self, sym):
        sym["shndx"] = 0
        struct.pack_into("<IIBBH", self.d, sym["off"] + 4, 0, 0, (sym["bind"] << 4), 0, 0)

    def set_align(self, align=1):
        for i, s in enumerate(self.sh):
            if s[2] & 2:  # SHF_ALLOC
                s[8] = align
                struct.pack_into("<10I", self.d, self.shoff + 40 * i, *s)

    def save(self, path):
        open(path, "wb").write(self.d)


def sext16(x):
    x &= 0xFFFF
    return x - 0x10000 if x & 0x8000 else x


def W(b, o):
    return struct.unpack_from("<I", b, o)[0]


# --------------------------------------------------------------------------------------------- splat
def split():
    if VER is None: run([sys.executable, os.path.join(ROOT, "tools", "gen_symbols.py")], cwd=ROOT)
    os.makedirs(OUT, exist_ok=True)
    cdir = os.path.join(ROOT, "config", *([VER] if VER else []))
    # a version's splat configs also read symbol_addrs_manual.txt (and any other file of its config dir)
    sfiles = (["symbol_addrs_%s.txt" % p for p in PROGS] if VER is None else
              sorted(f for f in os.listdir(cdir) if f.startswith("symbol_addrs_") and f.endswith(".txt")))
    for fn in sfiles:  # a C name like func_80045D0C at another address would hide splat's own func_80045D0C
        with open(os.path.join(OUT, fn), "w") as f:
            for l in open(os.path.join(cdir, fn)):
                m = re.match(r"((?:func|FUN|D)_([0-9A-Fa-f]{8})) = 0x([0-9A-Fa-f]+);", l)
                if m and int(m.group(2), 16) != int(m.group(3), 16):
                    l = l.replace(m.group(1), "%s__%s" % (m.group(1), m.group(3).upper()), 1)
                f.write(l)
    rel = os.path.relpath(OUT, ROOT)
    for p in PROGS:
        y = open(os.path.join(cdir, p + ".yaml")).read()
        y = re.sub(r"base_path: [./]+", "base_path: " + "/".join([".."] * len(rel.split("/"))), y)
        y = re.sub(r"(asm_path|src_path|build_path|ld_script_path|undefined_funcs_auto_path|undefined_syms_auto_path|asset_path): (asm|build)/%s"
                   % ((VER + "/?") if VER else "?"),
                   lambda m: "%s: %s/%s" % (m.group(1), rel, "asm/" if m.group(2) == "asm" else ""), y)
        y = y.replace("config/%ssymbol_addrs_" % (VER + "/" if VER else ""), rel + "/symbol_addrs_")
        if VER:  # header / trailing data blobs of the SYSTEM overlays as data lines (the link needs every byte as a
            # splat line); the X00 prefix of an area overlay stays a blob (it comes from the X00 linked here)
            y = re.sub(r"\[(0x[0-9A-Fa-f]+), bin, (\w+)\]",
                       lambda m: m.group(0) if m.group(2).endswith("_shared") else "[%s, data, %s]" % (m.group(1), m.group(2)), y)
        open(os.path.join(OUT, p + ".yaml"), "w").write(y)
        shutil.rmtree(os.path.join(OUT, "asm", p), ignore_errors=True)
        shutil.rmtree(os.path.join(OUT, "splat_src", p), ignore_errors=True)
        r = subprocess.run([sys.executable, "-m", "splat", "split", os.path.join(OUT, p + ".yaml"), "--disassemble-all"],
                           capture_output=True, text=True, cwd=ROOT)
        if r.returncode:
            sys.exit("splat failed for %s:\n%s" % (p, (r.stdout + r.stderr)[-3000:]))


ADDR_LINE = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8})\b")
LABEL = re.compile(r"^\s*(glabel|dlabel|jlabel|alabel|ehlabel)\s+(\S+)|^\s*([A-Za-z_.$][\w.$]*):\s*$")
DROP = re.compile(r"^\s*(nonmatching|endlabel|enddlabel|\.align|\.balign|\.section|\.include|\.set\s+no(at|reorder)|\.size|\.type)\b")


def load_chunks(p):
    """Every splat line with an address: [(addr, [labels], text)] sorted by address."""
    base = os.path.join(OUT, "asm", p)
    chunks = []
    for dp, _, fs in os.walk(base):
        for f in fs:
            if not f.endswith(".s") or f == "header.s": continue
            pend = []
            for line in open(os.path.join(dp, f)):
                line = line.rstrip("\n")
                m = ADDR_LINE.match(line)
                if m:
                    chunks.append([int(m.group(1), 16), pend, line]); pend = []
                    continue
                lm = LABEL.match(line)
                if lm:
                    pend.append(lm.group(2) or lm.group(3)); continue
                if not line.strip() or DROP.match(line) or line.strip().startswith("/*"):
                    continue
                sys.exit("unexpected splat line in %s: %r" % (f, line))
    chunks.sort(key=lambda c: c[0])
    for a, b in zip(chunks, chunks[1:]):
        if a[0] == b[0]: sys.exit("two splat lines at %08x" % a[0])
    return chunks


def data_unit_ranges(p):
    """[(start, end)] of the splat 'functions' of a version program that are data (config/<ver>/data_units.txt):
    splat disassembled them as code, and a table can read as a MIPS III instruction (dsll32, sd, ddiv: JP x14's
    placement tables) that the R3000 assembler refuses or widens; their lines are assembled as words"""
    if not VER: return []
    f = os.path.join(ROOT, "config", VER, "data_units.txt")
    out = []
    for l in (open(f) if os.path.exists(f) else ()):
        w = l.split()
        if len(w) >= 3 and not l.startswith("#") and w[0] == p:
            out.append((int(w[1], 16), int(w[1], 16) + int(w[2])))
    return out


WORD_LINE = re.compile(r"^(\s*/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/)\s*(.*)$")


def as_word(text):
    """a splat instruction line as the `.word` of its bytes (the comment holds them in file order)"""
    m = WORD_LINE.match(text)
    if not m or m.group(3).startswith(".word"): return text
    h = m.group(2)
    return "%s .word 0x%s%s%s%s" % (m.group(1), h[6:8], h[4:6], h[2:4], h[0:2])


def read_auto_syms(p):
    out = {}
    for k in ("funcs", "syms"):
        f = os.path.join(OUT, "undefined_%s_auto.%s.txt" % (k, p))
        if os.path.exists(f):
            for l in open(f):
                m = re.match(r"\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+)", l)
                if m: out[m.group(1)] = int(m.group(2), 16)
    return out


# ----------------------------------------------------------------------------------------- compile C
NC = {}


DATA_HDR = re.compile(r"^//\s*DATA\s+([0-9a-fA-F]+)\s+(\d+)[ \t]+(MAIN0|X0\d\d|%s)\s*$" % _SO.PROG_RE, re.M)


def data_header(path):
    """(addr, size, PROG) of a data-only file's `// DATA addr size PROG` header, or None."""
    m = DATA_HDR.search(open(path, errors="replace").read(400))
    return (int(m.group(1), 16), int(m.group(2)), m.group(3)) if m else None


def data_files(prog):
    """[(path, stem, addr, size)] of the data-only files of a program (src/data/<prog>/*.c)."""
    d = os.path.join(ROOT, "src", "data", prog.lower())
    out = []
    for f in sorted(os.listdir(d)) if os.path.isdir(d) else ():
        if not f.endswith(".c"): continue
        h = data_header(os.path.join(d, f))
        if h and h[2].lower() == prog.lower():
            out.append((os.path.join(d, f), f[:-2], h[0], h[1]))
    return out


_NAMES = {}


def symbol_names(prog):
    """{name: address} that an extern of a data-only file of `prog` may use: config/symbol_addrs of MAIN0 and of prog."""
    key = prog.lower()
    if key not in _NAMES:
        names = {}
        for p in (VEXE, "manual", key) if VER else ("main0", key):
            f = os.path.join(ROOT, "config", *([VER] if VER else []), "symbol_addrs_%s.txt" % p)
            if not os.path.exists(f): continue
            for l in open(f):
                m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", l)
                if m: names.setdefault(m.group(1), int(m.group(2), 16))
        _NAMES[key] = names
    return _NAMES[key]


def name_ok(prog, name, addr):
    """True when an extern `name` of a data-only file can stand for `addr`."""
    m = re.match(r"(?:D|func|FUN|DAT|LAB|jtbl)_([0-9A-Fa-f]{8})$", name)
    if m: return int(m.group(1), 16) == addr
    return symbol_names(prog).get(name) == addr


def _init_nc():
    src = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
    NC.update({"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"})
    exec(compile(src, "ncheck", "exec"), NC)


def _compile(job):
    path, flags, out, inc = job
    if not NC: _init_nc()
    d = tempfile.mkdtemp(dir=os.path.join(OUT, "tmp"))
    try:
        if SERIAL:  # another version: its #if branch (-DVERSION_<SERIAL>), INCLUDE_ASM branches compile to nothing
            import units as U
            NC["EXTRA_CPP"] = ["-DVERSION_" + SERIAL, "-DSKIP_ASM"]
            NC["EXTRA_AS"] = ""
            if flags is None:  # a function: `// FLAGS.<SERIAL>` etc. directives, scratchpad symbols as constants
                path = U.version_source(path, SERIAL, d)
                t = open(path, errors="replace").read()
                NC["EXTRA_AS"] = U.spad_set(t, SERIAL)
                fm = re.search(r"//\s*FLAGS\s+(.*)", t)
                flags = fm.group(1).strip() if fm else "-O2 -G0"
        NC["build"](path, flags, d, inc)
        shutil.copy(d + "/a.o", out)
        return None
    except subprocess.CalledProcessError as e:
        return ((e.stderr or b"").decode(errors="replace") if isinstance(e.stderr, bytes) else str(e.stderr or ""))[-300:]
    finally:
        shutil.rmtree(d, ignore_errors=True)


def compile_all():
    _init_nc()
    inc = os.path.join(OUT, "inc")
    shutil.rmtree(inc, ignore_errors=True); os.makedirs(inc)
    for f in os.listdir(os.path.join(ROOT, "include")):  # DOS is case-insensitive: copy both forms
        s = os.path.join(ROOT, "include", f)
        if os.path.isdir(s): shutil.copytree(s, os.path.join(inc, f)); continue
        for nm in {f, f.upper(), f.lower()}: shutil.copy(s, os.path.join(inc, nm))
    os.makedirs(os.path.join(OUT, "obj"), exist_ok=True); os.makedirs(os.path.join(OUT, "tmp"), exist_ok=True)
    units, jobs = [], []
    if VER: return compile_version(units, jobs, inc)
    files = [("src", f) for f in sorted(os.listdir(os.path.join(ROOT, "src")))]
    for P in PROGS.values():
        if P.get("src") and os.path.isdir(os.path.join(ROOT, P["src"])):
            files += [(P["src"], f) for f in sorted(os.listdir(os.path.join(ROOT, P["src"])))]
    for sd, f in files:
        if not f.endswith(".c"): continue
        path = os.path.join(ROOT, sd, f)
        h = NC["M"]["header"](path)
        if not h: continue
        addr, size, prog, flags = h
        if prog.lower() not in PROGS: continue
        stem = f[:-2] if sd == "src" else "%s__%s" % (prog.lower(), f[:-2])
        obj = os.path.join(OUT, "obj", stem + ".o")
        units.append(dict(name=stem, src=sd + "/" + f, addr=addr, size=size, prog=prog.lower(), obj=obj))
        jobs.append((path, flags, obj, inc))
    for p in PROGS:
        for path, stem, addr, size in data_files(p):
            obj = os.path.join(OUT, "obj", "data_%s_%s.o" % (p, stem))
            units.append(dict(name="data/%s/%s" % (p, stem), src=os.path.relpath(path, ROOT), addr=addr, size=size,
                              prog=p, obj=obj, data=True))
            jobs.append((path, "-O2 -G0", obj, inc))
    return compile_jobs(units, jobs)


def compile_jobs(units, jobs):
    with Pool(JOBS) as pool:
        res = pool.map(_compile, jobs)
    ok = []
    for u, r in zip(units, res):
        ok.append(u)
        if r is not None:
            u["failed"] = u["compile_error"] = True
            print("WARN %s does not compile, kept as asm: %s" % (u["src"], r.strip().splitlines()[-1] if r.strip() else "?"))
    return ok


def compile_version(units, jobs, inc):
    """The C units of a version: every function tools/units.py version_sources gives a src file (the shared file built
    with -DVERSION_<SERIAL>), at its version address; its size is what the object's .text gives (checked against the
    symbol / VERSION line size). Plus the version's own data files (src/data/<ver>/<prog>/*.c, tools/version_data.py)."""
    import units as U, version_data as VD
    vunits = U.version_units(VER)
    for p in PROGS:
        for vu in vunits.get(p, []):
            if not vu.file: continue
            obj = os.path.join(OUT, "obj", "%s__%08X.o" % (p, vu.addr))
            units.append(dict(name="%s__%08X" % (p, vu.addr), src="%s [%s %08X]" % (os.path.relpath(vu.file, ROOT), p, vu.addr),
                              addr=vu.addr, size=vu.size, prog=p, obj=obj, vname=vu.name))
            jobs.append((vu.file, None, obj, inc))
        for path, stem, addr, size in VD.version_files(VER, p):
            obj = os.path.join(OUT, "obj", "data_%s_%s.o" % (p, stem))
            units.append(dict(name="data/%s/%s" % (p, stem), src=os.path.relpath(path, ROOT), addr=addr, size=size,
                              prog=p, obj=obj, data=True))
            jobs.append((path, "-O2 -G0", obj, inc))
    ok = compile_jobs(units, jobs)
    vl = U.version_lines(SERIAL)
    for u in ok:
        if u.get("failed") or u.get("data"): continue
        n = Elf(u["obj"]).size(".text")
        want = vl.get((u["prog"], u["addr"]), (None, u["size"]))[1]
        if n == 0:
            u["failed"] = True
            print("WARN %s: no C for this version (INCLUDE_ASM branch), kept as asm" % u["src"])
        elif n != want:
            print("NOTE %s: .text is %d bytes, the symbol says %d" % (u["src"], n, want))
        u["size"] = n
    return ok


# ------------------------------------------------------------------------------------------ analysis
def analyse(u, game, vram, hdr):
    """Places the C object: .text at FUNC addr, referenced data sections at their retail address.
    Returns False (keep asm) when the object cannot be placed."""
    if u.get("failed"): return False
    for k in ("gplaced", "grefs", "bases", "drop"): u.pop(k, None)
    e = Elf(u["obj"])
    u["elf"] = e
    lim = vram + len(game) - hdr
    # retail bytes; past the end of the file (the bss) they are zeros
    gb = lambda a, n: (game[hdr + a - vram: hdr + a - vram + n] + bytes(max(0, a + n - max(a, lim))))[:n]
    if u.get("data"): return analyse_data(u, e, gb, vram, len(game) - hdr)
    if e.size(".text") != u["size"]:
        print("WARN %s: .text is %d bytes, FUNC says %d: kept as asm" % (u["src"], e.size(".text"), u["size"])); return False
    u["place"] = {".text": u["addr"]}
    u["syms"] = {}   # undefined symbol -> set of inferred addresses
    problems = []

    def infer(sec, base, retail):
        rel = e.relocs(sec)
        ours = e.data(sec)
        for i, (off, ty, sym) in enumerate(rel):
            if ty == 6: continue  # LO16: handled with its HI16
            if ty == 5:
                lo = next((r for r in rel[i + 1:] if r[1] == 6 and r[2] is sym), None)
                if lo is None: problems.append("HI16 without LO16 at %s+%x" % (sec, off)); continue
                val = ((W(retail, off) & 0xFFFF) << 16) + sext16(W(retail, lo[0]))
                add = ((W(ours, off) & 0xFFFF) << 16) + sext16(W(ours, lo[0]))
            elif ty == 4:
                if sym["type"] == 3 and e.names[sym["shndx"]] == ".text": continue  # j inside the function
                val = ((W(retail, off) & 0x3FFFFFF) << 2) | ((base + off) & 0xF0000000)
                add = (W(ours, off) & 0x3FFFFFF) << 2
            elif ty == 2:
                val, add = W(retail, off), W(ours, off)
            else:
                problems.append("relocation type %d at %s+%x" % (ty, sec, off)); continue
            val, add = val & 0xFFFFFFFF, add & 0xFFFFFFFF
            if 0 < sym["shndx"] < len(e.names) and sym["bind"] in (1, 2) and sym["type"] != 3 and e.names[sym["shndx"]] in DATA_SECS:
                u.setdefault("grefs", []).append((sym, (val - add) & 0xFFFFFFFF))  # global this file defines
            elif sym["shndx"] == 0:
                u["syms"].setdefault(sym["name"], set()).add((val - add) & 0xFFFFFFFF)
            else:
                tsec = e.names[sym["shndx"]] if sym["shndx"] < len(e.names) else "?"
                b = (val - add - (sym["value"] if sym["type"] != 3 else 0)) & 0xFFFFFFFF
                u.setdefault("bases", {}).setdefault(tsec, set()).add(b)

    def fits(sec, b):
        """The section is inside the program; a data section may run past the end of the file when its bytes there are
        zeros without relocations (an initialized table the retail exe ends inside of: the rest is in the bss)."""
        n = e.size(sec)
        if vram <= b and b + n <= lim: return True
        return (sec != ".text" and vram <= b < lim and e.sh[e.idx[sec]][1] != 8 and not any(e.data(sec)[lim - b:])
                and not any(off + 4 > lim - b for off, _, _ in e.relocs(sec)))

    def masked_equal(sec, b):
        ours, ret = bytearray(e.data(sec)), bytearray(gb(b, e.size(sec)))
        for off, ty, _ in e.relocs(sec):
            m = {2: 0xFFFFFFFF, 4: 0x3FFFFFF, 5: 0xFFFF, 6: 0xFFFF}.get(ty, 0)
            for buf in (ours, ret):
                struct.pack_into("<I", buf, off, W(buf, off) & ~m & 0xFFFFFFFF)
        return ours == ret

    def gref_place(sec):
        """A data section only reached through the globals it defines: placed when every use of them by .text gives
        the same section address, inside the program, and the section has the retail bytes there (else dropped)."""
        if e.sh[e.idx[sec]][1] == 8: return False
        bs = {(a - s["value"]) & 0xFFFFFFFF for s, a in u.get("grefs", []) if s["shndx"] == e.idx[sec]}
        if len(bs) != 1: return False
        b = next(iter(bs))
        if not fits(sec, b) or not masked_equal(sec, b): return False
        keep = (len(problems), {k: set(v) for k, v in u["syms"].items()}, list(u.get("grefs", [])),
                {k: set(v) for k, v in u.get("bases", {}).items()})
        infer(sec, b, gb(b, e.size(sec)))
        if (len(problems) != keep[0] or any(len(v) > 1 for v in u["syms"].values())
                or any(k not in u["place"] or v != {u["place"][k]} for k, v in u.get("bases", {}).items())):
            del problems[keep[0]:]; u["syms"], u["grefs"] = keep[1], keep[2]
            u.setdefault("bases", {}).clear(); u["bases"].update(keep[3])
            return False
        u["place"][sec] = b
        u.setdefault("gplaced", []).append(sec)
        return True

    infer(".text", u["addr"], gb(u["addr"], u["size"]))
    bases = u.setdefault("bases", {})
    if bases.get(".text", {u["addr"]}) != {u["addr"]}:
        problems.append(".text relocations point elsewhere")
    for sec in DATA_SECS:
        if not e.size(sec): continue
        if sec in bases:
            if len(bases[sec]) != 1:
                problems.append("%s has several bases %s" % (sec, sorted(map(hex, bases[sec])))); continue
            b = next(iter(bases[sec]))
            if not fits(sec, b):
                problems.append("%s would be at %08x, outside the program" % (sec, b)); continue
            u["place"][sec] = b
            if e.sh[e.idx[sec]][1] == 8:
                problems.append("%s is NOBITS" % sec); continue
            infer(sec, b, gb(b, e.size(sec)))
        elif sec not in u.get("nogref", ()) and gref_place(sec):
            pass  # globals this file defines, used by its code at their retail address with the retail bytes
        else:
            for s in e.syms:  # data this file defines again: use the real one (splat) instead
                if s["shndx"] == e.idx[sec] and s["bind"] in (1, 2):
                    e.undefine(s)
            u.setdefault("drop", []).append(sec)
    # resolve symbols defined in dropped sections that are referenced from .text: re-run inference
    if u.get("drop"):
        u["syms"] = {}; u["bases"] = {}; u["grefs"] = []
        infer(".text", u["addr"], gb(u["addr"], u["size"]))
        for sec, b in u["place"].items():
            if sec != ".text": infer(sec, b, gb(b, e.size(sec)))
    for sym, a in u.get("grefs", []):
        if sym["shndx"] != 0:
            sec = e.names[sym["shndx"]]
            if sec in u["place"] and u["place"][sec] + sym["value"] != a:
                problems.append("%s is defined at %08x, used at %08x" % (sym["name"], u["place"][sec] + sym["value"], a))
        else:
            u["syms"].setdefault(sym["name"], set()).add(a)
    for n, v in u["syms"].items():
        if len(v) > 1: problems.append("symbol %s resolves to %s" % (n, sorted(map(hex, v))))
    # placed bytes must equal the retail bytes (relocated words masked)
    for sec, b in u["place"].items():
        ours, ret = bytearray(e.data(sec)), bytearray(gb(b, e.size(sec)))
        for off, ty, _ in e.relocs(sec):
            m = {2: 0xFFFFFFFF, 4: 0x3FFFFFF, 5: 0xFFFF, 6: 0xFFFF}.get(ty, 0)
            for buf in (ours, ret):
                struct.pack_into("<I", buf, off, W(buf, off) & ~m & 0xFFFFFFFF)
        if ours != ret:
            k = next(i for i in range(len(ours)) if ours[i] != ret[i])
            problems.append("%s (at %08x, %d bytes) bytes differ from the game at %08x" % (sec, b, len(ours), b + k))
    if problems:
        print("WARN %s: kept as asm: %s" % (u["src"], "; ".join(problems[:4]))); return False
    return True


def analyse_data(u, e, gb, vram, n):
    """Places a data-only object (see data_files): its single data section at the DATA address."""
    secs = [s for s in e.idx if e.size(s) and e.sh[e.idx[s]][2] & 2 and s not in (".reginfo", ".pdr", ".MIPS.abiflags")]  # allocated sections with bytes
    problems = []
    if len(secs) != 1 or secs[0] not in DATA_SECS or e.sh[e.idx[secs[0]]][1] == 8:
        problems.append("needs exactly one initialized data section, has %s" % secs)
    elif e.size(secs[0]) != u["size"]:
        problems.append("%s is %d bytes, DATA says %d" % (secs[0], e.size(secs[0]), u["size"]))
    elif not (vram <= u["addr"] and u["addr"] + u["size"] <= vram + n):
        problems.append("outside the program")
    if not problems:
        sec, a = secs[0], u["addr"]
        u["place"], u["syms"] = {sec: a}, {}
        ours, ret = e.data(sec), gb(a, u["size"])
        rel = e.relocs(sec)
        for off, ty, sym in rel:
            if ty != 2: problems.append("relocation type %d at %s+%x" % (ty, sec, off)); continue
            t = (W(ret, off) - W(ours, off)) & 0xFFFFFFFF  # address the retail word gives the symbol (or section)
            if sym["shndx"] == 0:
                if not name_ok(u["prog"], sym["name"], t):
                    problems.append("%s at %s+%x would be %08x" % (sym["name"], sec, off, t))
                u["syms"].setdefault(sym["name"], set()).add(t)
            elif sym["shndx"] == e.idx[sec]:
                if (t - (sym["value"] if sym["type"] != 3 else 0)) & 0xFFFFFFFF != a:
                    problems.append("pointer at %s+%x does not point where the retail one does" % (sec, off))
            else:
                problems.append("relocation against another section at %s+%x" % (sec, off))
        mask = bytearray(ours), bytearray(ret)
        for off, ty, _ in rel:
            for buf in mask: struct.pack_into("<I", buf, off, 0)
        if mask[0] != mask[1]:
            k = next(i for i in range(len(ours)) if mask[0][i] != mask[1][i])
            problems.append("%s bytes differ from the game at %08x" % (sec, a + k))
        for nm, v in u["syms"].items():
            if len(v) > 1: problems.append("symbol %s resolves to %s" % (nm, sorted(map(hex, v))))
    if problems:
        print("WARN %s: kept as asm: %s" % (u["src"], "; ".join(problems[:4]))); return False
    return True


# ------------------------------------------------------------------------------------------- linking
def place_ranges(u, addrs, end, game, vram, hdr, bisect):
    """(start, end, section, zero padding) of each placed section of a unit."""
    ranges = []
    for s, a in u["place"].items():
        b = a + u["elf"].size(s)
        nxt = addrs[bisect.bisect_left(addrs, b)] if bisect.bisect_left(addrs, b) < len(addrs) else end
        # data ending inside a splat line (unaligned strings): zero padding up to the next line
        pad = nxt - b if (s != ".text" and 0 <= nxt - b < 8 and not any(game[hdr + b - vram:hdr + nxt - vram])) else 0
        ranges.append((a, b + pad, s, pad))
    return ranges


def place_bad(u, addrs, end, items, game, vram, hdr, bisect):
    """Why a unit's placed sections cannot be linked (None: they can)."""
    bad = None
    for a, b, s, _ in place_ranges(u, addrs, end, game, vram, hdr, bisect):
        if not (vram <= a and (b <= end or (s != ".text" and a < end))): bad = "%s outside the program" % s
        elif bisect.bisect_left(addrs, a) == len(addrs) or addrs[bisect.bisect_left(addrs, a)] != a:
            bad = "%s starts at %08x, inside a splat line" % (s, a)
        elif b < end and (bisect.bisect_left(addrs, b) == len(addrs) or addrs[bisect.bisect_left(addrs, b)] != b):
            bad = "%s ends at %08x, inside a splat line" % (s, b)
        for it in items:
            if a < it[1] and it[0] < b: bad = "%s overlaps %s" % (s, it[3][0]["src"])
    return bad


def build_prog(p, units):
    P = PROGS[p]
    vram, end = P["vram"], P["vram"] + P["size"]
    game = open(os.path.join(ROOT, P["game"]), "rb").read()
    chunks = load_chunks(p)
    addrs = [c[0] for c in chunks]
    import bisect
    # place C objects
    items = []   # (start, end, kind, payload)
    for u in sorted(units, key=lambda u: u["addr"]):
        if not analyse(u, game, vram, P["header"]): continue
        if u.get("gplaced") and place_bad(u, addrs, end, items, game, vram, P["header"], bisect):
            u["nogref"] = set(u["gplaced"])  # its globals' sections cannot go at their address: back to splat data
            if not analyse(u, game, vram, P["header"]): continue
        ranges = place_ranges(u, addrs, end, game, vram, P["header"], bisect)
        bad = place_bad(u, addrs, end, items, game, vram, P["header"], bisect)
        if bad:
            print("WARN %s: kept as asm: %s" % (u["src"], bad)); continue
        for a, b, s, pad in ranges:
            if b > a: items.append((a, b, "c", (u, s, pad)))
    items.sort()
    used = {it[3][0]["name"]: it[3][0] for it in items}
    # global symbols the C objects define
    cdefs = {}
    dupren = {}   # a version can build two functions from one shared file: the later definition is renamed name__ADDR
    for u in sorted(used.values(), key=lambda u: u["addr"]):
        e = u["elf"]
        for s in e.syms:
            if s["shndx"] not in (0, 0xFFF1, 0xFFF2) and s["bind"] in (1, 2) and s["shndx"] < len(e.names):
                sec = e.names[s["shndx"]]
                if sec in u["place"]:
                    a = u["place"][sec] + s["value"]
                    if s["name"] in cdefs and cdefs[s["name"]] != a:
                        nn = "%s__%08X" % (s["name"], a)
                        dupren.setdefault(u["name"], []).append("--redefine-sym=%s=%s" % (s["name"], nn))
                        cdefs[nn] = a
                    else:
                        cdefs[s["name"]] = a
    # splat labels
    known = {}
    for a, labels, _ in chunks:
        for l in labels: known.setdefault(l, a)
    auto = read_auto_syms(p)
    # assembly: everything not covered by C, one section per gap
    lines = ['.include "macro.inc"', ".set noat", ".set noreorder"]
    order = []   # (start, end, 'asm', section name) / c items
    pos, ci, nsec = vram, 0, 0
    dranges = data_unit_ranges(p)
    removed = {}
    citems = items + [(end, end, "end", None)]
    k = 0
    for it in citems:
        if it[0] > pos:  # asm gap [pos, it[0])
            sec = ".splat_%04d" % nsec; nsec += 1
            lines.append(".section %s, \"awx\"" % sec)
            while k < len(chunks) and chunks[k][0] < it[0]:
                a, labels, text = chunks[k]
                if a < pos: k += 1; continue
                lines.append(".org 0x%x" % (a - pos))
                for l in labels:
                    if not l.startswith(".L") and l not in cdefs: lines.append(".global %s" % l)
                    lines.append("%s:" % l)
                lines.append(as_word(text) if any(lo <= a < hi for lo, hi in dranges) else text)
                k += 1
            lines.append(".org 0x%x" % (it[0] - pos))
            order.append((pos, it[0], "asm", sec))
        if it[2] == "end": break
        while k < len(chunks) and chunks[k][0] < it[1]:  # lines replaced by C
            for l in chunks[k][1]: removed[l] = chunks[k][0]
            k += 1
        order.append(it)
        pos = it[1]
    # labels of the lines C replaced: still referenced by the remaining asm (calls into a C range, jump tables)
    provide = dict(auto)
    for l, a in sorted(removed.items()):
        if l in cdefs: continue
        if l.startswith(".L"): lines.insert(3, ".set %s, 0x%08X" % (l, a))
        else: provide.setdefault(l, a)
    # undefined symbols of the C objects: inferred addresses, renamed when the name means something else
    for u in used.values():
        ren = list(dupren.get(u["name"], []))
        for n, v in sorted(u["syms"].items()):
            a = next(iter(v))
            have = cdefs.get(n, known.get(n, None))
            if have is None and n in removed: have = removed[n]
            if have is not None and have != a:
                nn = "%s__%08X" % (n, a); ren.append("--redefine-sym=%s=%s" % (n, nn)); provide[nn] = a
            else:
                if n in provide and provide[n] != a:
                    nn = "%s__%08X" % (n, a); ren.append("--redefine-sym=%s=%s" % (n, nn)); provide[nn] = a
                else:
                    provide[n] = a
        e = u["elf"]; e.set_align(1)
        fixed = u["obj"][:-2] + ".link.o"
        e.save(fixed)
        if ren: run([BIN + "objcopy"] + ren + [fixed])
        u["link"] = fixed
    ALLDEFS[p] = (cdefs, used)
    asm = os.path.join(OUT, p + "_splat.s")
    open(asm, "w").write("\n".join(lines) + "\n")
    aobj = os.path.join(OUT, p + "_splat.o")
    run(AS + ["-I" + os.path.join(ROOT, "include", "tomba"), asm, "-o", aobj])
    e = Elf(aobj); e.set_align(1); e.save(aobj)
    for s in e.syms:  # splat names data inside its "code" (X000) by address without defining a label
        m = re.match(r"(?:D|func|jtbl|FUN|LAB|DAT)_([0-9A-Fa-f]{8})$", s["name"])
        if s["shndx"] == 0 and m and s["name"] not in provide:
            provide[s["name"]] = int(m.group(1), 16)
    # linker script
    ld = ["SECTIONS", "{", "    .%s 0x%08X : AT(0)" % (p, vram), "    {"]
    for it in order:
        if it[2] == "asm": ld.append("        %s(%s)" % (os.path.relpath(aobj, OUT), it[3]))
        else:
            ld.append("        %s(%s)" % (os.path.relpath(it[3][0]["link"], OUT), it[3][1]))
            if it[3][2]: ld.append("        . = . + %d;" % it[3][2])
    ld += ["    }", "    /DISCARD/ : { *(*) }", "}"]
    for n, a in sorted(provide.items()):
        ld.append("PROVIDE(%s = 0x%08X);" % (n, a))
    lds = os.path.join(OUT, p + ".full.ld")
    open(lds, "w").write("\n".join(ld) + "\n")
    elf = os.path.join(OUT, p + ".elf")
    run([BIN + "ld", "-EL", "-T", os.path.relpath(lds, OUT), "-Map", p + ".map", "-o", os.path.relpath(elf, OUT),
         "--no-check-sections"], cwd=OUT)
    raw = os.path.join(OUT, p + ".raw")
    run([BIN + "objcopy", "-O", "binary", "-j", "." + p, elf, raw])
    body = open(raw, "rb").read()
    if any(it[1] > end for it in order): body = body[:P["size"]]  # data run past the end of the file (zeros: bss)
    head = P.get("head", "asm" if P["header"] else None)
    if head == "asm":
        hs = os.path.join(OUT, "asm", p, "header.s")
        run(AS + [hs, "-o", os.path.join(OUT, "header.o")])
        run([BIN + "objcopy", "-O", "binary", "-j", ".data", os.path.join(OUT, "header.o"), os.path.join(OUT, "header.bin")])
        body = open(os.path.join(OUT, "header.bin"), "rb").read() + body
    elif head == "prefix":  # area overlay of a version: the X00 code linked here comes first
        pre = os.path.join(OUT, PREFIX_PROG.upper() + ".BIN")
        if not os.path.exists(pre): sys.exit("%s needs %s linked first (build %s too)" % (P["out"], pre, PREFIX_PROG))
        body = open(pre, "rb").read()[:P["header"]] + body
    out = os.path.join(OUT, P["out"])
    open(out, "wb").write(body)
    if VER is None: shutil.copy(out, os.path.join(ROOT, "build", P["out"]))
    ncfun = sum(1 for it in order if it[2] == "c" and it[3][1] == ".text")
    ndata = sum(1 for it in order if it[2] == "c" and it[3][0].get("data"))
    return out, game, order, (ncfun, ndata), sum(1 for u in units if not u.get("data"))


def report(p, out, game, order, ncfun, nunits):
    P = PROGS[p]
    data = open(out, "rb").read()
    sha = hashlib.sha1(data).hexdigest()
    ok = sha == P["sha1"]
    ncfun, ndata = ncfun
    print("%-9s %s  %s  (%d of %d src files linked, the rest kept as asm; %d data files)"
          % (P["out"], sha, "OK" if ok else "FAIL", ncfun, nunits, ndata))
    if ok: return True
    if len(data) != len(game): print("   size %d, retail %d" % (len(data), len(game)))
    diffs = [i for i in range(min(len(data), len(game))) if data[i] != game[i]]
    shown = set()
    for i in diffs:
        a = P["vram"] + i - P["header"]
        w = a & ~3
        if w in shown: continue
        shown.add(w)
        who = next((("C " + it[3][0]["src"] + " " + it[3][1]) if it[2] == "c" else "splat asm"
                    for it in order if it[0] <= a < it[1]), "header")
        print("   offset %06x  addr %08x  ours %s  game %s  %s" % (i, a, data[i & ~3:(i & ~3) + 4].hex(),
                                                                  game[i & ~3:(i & ~3) + 4].hex(), who))
        if len(shown) >= 20: print("   ... %d differing bytes" % len(diffs)); break
    return False


_VNAMES = {}


def version_names(prog):
    """{name: {addresses}} a C extern of a version program may stand for: config/<ver>/symbol_addrs of the boot exe
    (+ manual), of X00 for an area overlay (it runs with the X00 prefix), and of the program itself; plus the globals
    the C linked into those programs defines."""
    if prog in _VNAMES: return _VNAMES[prog]
    scope = [VEXE, "manual"] + ([PREFIX_PROG] if PROGS.get(prog, {}).get("head") == "prefix" else []) + [prog]
    names = {}
    for p in scope:
        f = os.path.join(ROOT, "config", VER, "symbol_addrs_%s.txt" % p)
        for l in open(f) if os.path.exists(f) else ():
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", l)
            if m: names.setdefault(m.group(1), set()).add(int(m.group(2), 16))
        for n, a in ALLDEFS.get(p, ({}, {}))[0].items():
            names.setdefault(n, set()).add(a)
    _VNAMES[prog] = names
    return names


def name_audit(p):
    """Version link: every relocation of the C linked into program p was resolved from the retail bytes (the link uses
    the retail address whatever the name says); this checks what the names say, in build/<ver>/full/<prog>.names.txt:
      - named: a name the version's symbols (or the C linked into the exe / X00 / this program) give another address;
      - raw (version C): a D_/func_<addr> name of a version-only file or branch that is not that address;
      - raw (shared C): the same in shared PAL code (PAL addresses: the version's address comes from the link);
      - unknown: names the version has no symbol for (PAL names: the link gives them their version address).
    The first two are naming bugs of the version C; tools/build_full.py PROVIDEs the real address under a renamed
    symbol (name__ADDR) whenever two uses of one name disagree, so the port never mixes them up."""
    names = version_names(p)
    out = {"named": [], "raw (version C)": [], "raw (shared C)": [], "unknown": []}
    ok = 0
    vfiles = {path for (prog, a), (path, _) in _vlines().items() if prog == p}
    for u in sorted(ALLDEFS[p][1].values(), key=lambda u: u["addr"]):
        if u.get("data"): continue
        path = os.path.join(ROOT, u["src"].split(" [")[0])
        own = path in vfiles or "_" + VER in os.path.basename(path)
        for n, v in sorted(u["syms"].items()):
            a = next(iter(v))
            m = re.match(r"(?:D|func|FUN|DAT|LAB|jtbl|data)_([0-9A-Fa-f]{8})(?:_%s)?$" % VER, n)
            if m:
                if int(m.group(1), 16) == a: ok += 1
                else: out["raw (version C)" if own else "raw (shared C)"].append("%s -> %08X  %s" % (n, a, u["src"]))
            elif n in names:
                if a in names[n]: ok += 1
                else: out["named"].append("%s -> %08X (symbols say %s)  %s"
                                          % (n, a, " ".join("%08X" % x for x in sorted(names[n])), u["src"]))
            else: out["unknown"].append("%s -> %08X  %s" % (n, a, u["src"]))
    with open(os.path.join(OUT, p + ".names.txt"), "w") as f:
        for k, ls in out.items():
            f.write("# %s (%d)\n" % (k, len(ls)) + "".join(l + "\n" for l in ls))
    print("   names: %d agree; %s (%s.names.txt)" % (ok, ", ".join("%d %s" % (len(v), k) for k, v in out.items()), p))
    for l in (out["named"] + out["raw (version C)"])[:8]: print("     " + l)
    return not out["named"] and not out["raw (version C)"]


def _vlines():
    import units as U
    if "v" not in _vlines.__dict__: _vlines.v = U.version_lines(SERIAL)
    return _vlines.v


def main():
    os.chdir(ROOT)
    if "--version" in ARGS:
        from versions import canon, REFERENCE
        v = canon(ARGS[ARGS.index("--version") + 1])
        if v != REFERENCE: setup_version(v)
    for p, P in list(PROGS.items()):
        if not os.path.exists(P["game"]):
            if p in ("main0", "x000") or VER: sys.exit("missing %s (put your own game files in game/)" % P["game"])
            print("skip %s: missing %s (extracted disc tree game/disc/fs)" % (P["out"], P["game"])); del PROGS[p]
    # the psx_tomba headers (`// HEADERS tomba24` / `tomba26` files include tomba/psyq/...) are not in the repo:
    # without them every such file fails to compile and stays asm, the link still matches by sha1, and a native port
    # generated from this link silently lacks those functions (USA: 611 files, 126 stubs, a crash at frame 0)
    if "--asm-only" not in ARGS and not os.path.isdir(os.path.join(ROOT, "include", "tomba", "psyq")):
        sys.exit("build_full: include/tomba/psyq is missing (the PSY-Q headers the psx_tomba files include): run "
                 "sudo tools/setup_native.sh first (in a worktree: copy include/tomba/psyq from the main checkout)")
    if "--no-split" not in ARGS: split()
    units = [] if "--asm-only" in ARGS else compile_all()
    # the C files that did not compile (kept as asm): port/tools/gen_port.py refuses a link that has any, since their
    # functions would be missing from the port
    failed = sorted(u["src"] for u in units if u.get("compile_error"))
    os.makedirs(OUT, exist_ok=True)
    with open(os.path.join(OUT, "compile_failures.txt"), "w") as f:
        f.write("".join(s + "\n" for s in failed))
    allok = True
    for p in PROGS:
        out, game, order, ncfun, n = build_prog(p, [u for u in units if u["prog"] == p])
        allok &= report(p, out, game, order, ncfun, n)
        if VER: name_audit(p)
    if failed:
        print("WARNING: %d C files did not compile and were linked from asm (%s/compile_failures.txt): the bytes "
              "match, but the native port refuses this link (it would lack their functions)"
              % (len(failed), os.path.relpath(OUT, ROOT)))
    print("OK" if allok else "FAIL")
    sys.exit(0 if allok else 1)


if __name__ == "__main__":
    main()
