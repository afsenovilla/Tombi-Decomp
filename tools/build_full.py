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
       - sections that only define globals (data a ported header/file defines again) are dropped and
         their symbols turned into undefined references, so the real data from splat is used.
     Every undefined symbol of a C object gets its address from the retail bytes too (checked for
     consistency, and against the splat label of the same name).
  4. All remaining splat lines go into one assembly file per program, cut into one section per
     contiguous range (`.org` keeps every line at its address); a linker script puts these sections and
     the C objects in address order.
  5. ld + objcopy produce build/full/MAIN0.EXE (with the PS-X EXE header) and build/full/X000.BIN,
     copied to build/MAIN0.EXE and build/X000.BIN, and compared with the retail files.

Usage: python3 tools/build_full.py [--no-split] [-j N] [--asm-only] [--keep-going]
  --no-split   reuse the previous splat output in build/full/asm
  --asm-only   ignore src/ (pure splat assembly: checks the asm path alone)
Requirements: the same as tools/ncheck.py (tools/setup_native.sh) and splat 0.35.2 (tools/requirements.txt).
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
    run([sys.executable, os.path.join(ROOT, "tools", "gen_symbols.py")], cwd=ROOT)
    os.makedirs(OUT, exist_ok=True)
    for p in PROGS:  # a C name like func_80045D0C at another address would hide splat's own func_80045D0C
        with open(os.path.join(OUT, "symbol_addrs_%s.txt" % p), "w") as f:
            for l in open(os.path.join(ROOT, "config", "symbol_addrs_%s.txt" % p)):
                m = re.match(r"((?:func|FUN|D)_([0-9A-Fa-f]{8})) = 0x([0-9A-Fa-f]+);", l)
                if m and int(m.group(2), 16) != int(m.group(3), 16):
                    l = l.replace(m.group(1), "%s__%s" % (m.group(1), m.group(3).upper()), 1)
                f.write(l)
    for p in PROGS:
        y = open(os.path.join(ROOT, "config", p + ".yaml")).read()
        y = y.replace("base_path: ..", "base_path: ../..")
        y = re.sub(r"(asm_path|src_path|build_path|ld_script_path|undefined_funcs_auto_path|undefined_syms_auto_path): (asm|build)/?",
                   lambda m: "%s: build/full/%s" % (m.group(1), "asm/" if m.group(2) == "asm" else ""), y)
        y = y.replace("config/symbol_addrs_", "build/full/symbol_addrs_")
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


def _init_nc():
    src = open(os.path.join(ROOT, "tools", "ncheck.py")).read().replace("\nmain()\n", "\n")
    NC.update({"__file__": os.path.join(ROOT, "tools", "ncheck.py"), "__name__": "ncheck_mod"})
    exec(compile(src, "ncheck", "exec"), NC)


def _compile(job):
    path, flags, out, inc = job
    if not NC: _init_nc()
    d = tempfile.mkdtemp(dir=os.path.join(OUT, "tmp"))
    try:
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
    for f in sorted(os.listdir(os.path.join(ROOT, "src"))):
        if not f.endswith(".c"): continue
        path = os.path.join(ROOT, "src", f)
        h = NC["M"]["header"](path)
        if not h: continue
        addr, size, prog, flags = h
        obj = os.path.join(OUT, "obj", f[:-2] + ".o")
        units.append(dict(name=f[:-2], src="src/" + f, addr=addr, size=size, prog=prog.lower(), obj=obj))
        jobs.append((path, flags, obj, inc))
    with Pool(JOBS) as pool:
        res = pool.map(_compile, jobs)
    ok = []
    for u, r in zip(units, res):
        ok.append(u)
        if r is not None:
            u["failed"] = True
            print("WARN %s does not compile, kept as asm: %s" % (u["src"], r.strip().splitlines()[-1] if r.strip() else "?"))
    return ok


# ------------------------------------------------------------------------------------------ analysis
def analyse(u, game, vram, hdr):
    """Places the C object: .text at FUNC addr, referenced data sections at their retail address.
    Returns False (keep asm) when the object cannot be placed."""
    if u.get("failed"): return False
    e = Elf(u["obj"])
    u["elf"] = e
    gb = lambda a, n: game[hdr + a - vram: hdr + a - vram + n]
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

    infer(".text", u["addr"], gb(u["addr"], u["size"]))
    bases = u.get("bases", {})
    if bases.get(".text", {u["addr"]}) != {u["addr"]}:
        problems.append(".text relocations point elsewhere")
    for sec in DATA_SECS:
        if not e.size(sec): continue
        if sec in bases:
            if len(bases[sec]) != 1:
                problems.append("%s has several bases %s" % (sec, sorted(map(hex, bases[sec])))); continue
            b = next(iter(bases[sec]))
            if not (vram <= b and b + e.size(sec) <= vram + len(game) - hdr):
                problems.append("%s would be at %08x, outside the program" % (sec, b)); continue
            u["place"][sec] = b
            if e.sh[e.idx[sec]][1] == 8:
                problems.append("%s is NOBITS" % sec); continue
            infer(sec, b, gb(b, e.size(sec)))
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
            problems.append("%s bytes differ from the game at %08x" % (sec, b + k))
    if problems:
        print("WARN %s: kept as asm: %s" % (u["src"], "; ".join(problems[:4]))); return False
    return True


# ------------------------------------------------------------------------------------------- linking
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
        ranges = []
        for s, a in u["place"].items():
            b = a + u["elf"].size(s)
            nxt = addrs[bisect.bisect_left(addrs, b)] if bisect.bisect_left(addrs, b) < len(addrs) else end
            # data ending inside a splat line (unaligned strings): zero padding up to the next line
            pad = nxt - b if (s != ".text" and nxt - b < 8 and not any(game[P["header"] + b - vram:P["header"] + nxt - vram])) else 0
            ranges.append((a, b + pad, s, pad))
        bad = None
        for a, b, s, _ in ranges:
            if not (vram <= a and b <= end): bad = "%s outside the program" % s
            elif bisect.bisect_left(addrs, a) == len(addrs) or addrs[bisect.bisect_left(addrs, a)] != a:
                bad = "%s starts at %08x, inside a splat line" % (s, a)
            elif b != end and (bisect.bisect_left(addrs, b) == len(addrs) or addrs[bisect.bisect_left(addrs, b)] != b):
                bad = "%s ends at %08x, inside a splat line" % (s, b)
            for it in items:
                if a < it[1] and it[0] < b: bad = "%s overlaps %s" % (s, it[3][0]["src"])
        if bad:
            print("WARN %s: kept as asm: %s" % (u["src"], bad)); continue
        for a, b, s, pad in ranges:
            if b > a: items.append((a, b, "c", (u, s, pad)))
    items.sort()
    used = {it[3][0]["name"]: it[3][0] for it in items}
    # global symbols the C objects define
    cdefs = {}
    for u in used.values():
        e = u["elf"]
        for s in e.syms:
            if s["shndx"] not in (0, 0xFFF1, 0xFFF2) and s["bind"] in (1, 2) and s["shndx"] < len(e.names):
                sec = e.names[s["shndx"]]
                if sec in u["place"]:
                    cdefs[s["name"]] = u["place"][sec] + s["value"]
    # splat labels
    known = {}
    for a, labels, _ in chunks:
        for l in labels: known.setdefault(l, a)
    auto = read_auto_syms(p)
    # assembly: everything not covered by C, one section per gap
    lines = ['.include "macro.inc"', ".set noat", ".set noreorder"]
    order = []   # (start, end, 'asm', section name) / c items
    pos, ci, nsec = vram, 0, 0
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
                lines.append(text)
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
        ren = []
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
    if P["header"]:
        hs = os.path.join(OUT, "asm", p, "header.s")
        run(AS + [hs, "-o", os.path.join(OUT, "header.o")])
        run([BIN + "objcopy", "-O", "binary", "-j", ".data", os.path.join(OUT, "header.o"), os.path.join(OUT, "header.bin")])
        body = open(os.path.join(OUT, "header.bin"), "rb").read() + body
    out = os.path.join(OUT, P["out"])
    open(out, "wb").write(body)
    shutil.copy(out, os.path.join(ROOT, "build", P["out"]))
    ncfun = sum(1 for it in order if it[2] == "c" and it[3][1] == ".text")
    return out, game, order, ncfun, len(units)


def report(p, out, game, order, ncfun, nunits):
    P = PROGS[p]
    data = open(out, "rb").read()
    sha = hashlib.sha1(data).hexdigest()
    ok = sha == P["sha1"]
    print("%-9s %s  %s  (%d of %d src files linked, the rest kept as asm)" % (P["out"], sha, "OK" if ok else "FAIL", ncfun, nunits))
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


def main():
    os.chdir(ROOT)
    for p, P in PROGS.items():
        if not os.path.exists(P["game"]): sys.exit("missing %s (put your own game files in game/)" % P["game"])
    if "--no-split" not in ARGS: split()
    units = [] if "--asm-only" in ARGS else compile_all()
    allok = True
    for p in PROGS:
        out, game, order, ncfun, n = build_prog(p, [u for u in units if u["prog"] == p])
        allok &= report(p, out, game, order, ncfun, n)
    print("OK" if allok else "FAIL")
    sys.exit(0 if allok else 1)


if __name__ == "__main__":
    main()
