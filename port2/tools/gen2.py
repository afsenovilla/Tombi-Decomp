#!/usr/bin/env python3
"""Generator for the native-pointer port (port2/CMakeLists.txt drives it; see docs/PORT.md "M3").

The game C is compiled as ordinary C with native pointers and ordinary (relocatable) global data: no fixed
addresses, no exception-handler tricks, no KUSEG mirror, no interpreter.  Everything that named a PS1 address in
the matching sources is turned into a symbol reference at generation time; nothing is resolved at run time.

  gen2.py sources OUTDIR [PROG...]
      1. port/tools/gen_port.py's source filter (GTE macros -> C, asm dropped, renames, PORT_ARG forwarding)
         writes OUTDIR/src.  Its retail-data rewrites are replaced: a retail-named C definition stays a plain
         extern (the generated data image holds the one copy), and the KUSEG/OT-link idiom becomes port_otaddr/
         port_otlink (port2/include/port_prelude.h).
      2. The address resolver replaces every `0x80xxxxxx` / `0x1F80xxxx` literal in OUTDIR/src and in the include
         trees with PORT_ADDR(symbol, offset): the function at that address, the data symbol at (or before) it,
         or the data image blob.  Unresolvable literals (no symbol covers the address) are listed in
         OUTDIR/literals.txt with file:line and left as they are.
      3. OUTDIR/port2_syms.h declares every symbol the resolver used (`extern unsigned char port_a_X[] asm("X")`,
         so the declaration never conflicts with the game's own typed externs of the same name).

  gen2.py link OUTDIR NM OBJLIST
      After compiling: every undefined name is resolved against the retail symbol table.
        * data name with a retail address -> `name = port_<region>_image + off;` in OUTDIR/syms.ld (an alias into
          the generated data image, which OUTDIR/data.c defines from the user's MAIN0.EXE / X000.BIN at BUILD time:
          game data, never committed.  Pointers inside the image are emitted as symbol references.);
        * a renamed/aliased function name -> `name = native;`;
        * code we have no C for (Psy-Q, BIOS, asm-only game functions) -> logging stub in OUTDIR/stubs.c.
      OUTDIR/report.txt sums it all up.
"""
import bisect, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))
sys.path.insert(0, os.path.join(ROOT, "port", "tools"))
import gen_port as GP

# ---------------------------------------------------------------------------------------------------------------
# memory regions of the native build: each becomes one relocatable array in OUTDIR/data.c
# (name, start, end, source file, file offset of `start`)
MAIN0_EXE = os.path.join(ROOT, "game", "MAIN0.EXE")
X000_BIN = os.path.join(ROOT, "game", "AREA00", "X000.BIN")
MAIN0_LOAD, X000_LOAD = 0x80010000, 0x800E8028


def regions():
    m0 = os.path.getsize(MAIN0_EXE) - 0x800 if os.path.exists(MAIN0_EXE) else 0x89000
    x0 = os.path.getsize(X000_BIN) if os.path.exists(X000_BIN) else 0x53FD0
    return [
        ("port_bios", 0x80000000, 0x80010000, None, 0),               # BIOS kernel area (TCB table): zero memory
        ("port_main0_image", MAIN0_LOAD, MAIN0_LOAD + m0, MAIN0_EXE, 0x800),
        ("port_main0_bss", MAIN0_LOAD + m0, X000_LOAD, None, 0),
        ("port_x000_image", X000_LOAD, X000_LOAD + x0, X000_BIN, 0),
        ("port_x000_bss", X000_LOAD + x0, 0x80140000, None, 0),
        ("port_heap", 0x80140000, 0x801FD800, None, 0),                # load buffers above the overlay
        ("port_hiram", 0x801FD800, 0x80200000, None, 0),               # top of RAM (the game's E-table)
        ("port_scratch", 0x1F800000, 0x1F800400, None, 0),             # scratchpad
        ("port_io", 0x1F801000, 0x1F803000, None, 0),                  # I/O registers: plain memory until shimmed
    ]


def region_of(a, regs):
    if 0xA0000000 <= a < 0xA0200000: a -= 0x20000000       # KSEG1 alias
    for n, s, e, f, o in regs:
        if s <= a < e: return n, a - s
    return None, 0


# ---------------------------------------------------------------------------------------------------------------
class AddrMap:
    """Retail address -> native symbol.  Code: the units of gen_port (unitnames.txt, code_ranges.txt, funcmap.c).
    Data: the PROVIDE names of build/full (symaddr.txt)."""
    def __init__(self, outdir):
        self.regs = regions()
        self.unit = {}                                    # function start -> unit name
        for l in open(os.path.join(outdir, "unitnames.txt")):
            a, n = l.split(); self.unit.setdefault(int(a, 16), n)
        self.native = {}                                  # function start -> native C name
        for l in open(os.path.join(outdir, "funcmap.c")):
            m = re.match(r'\s*\{0x([0-9A-F]+), \(void \*\)(\w+)', l)
            if m: self.native[int(m.group(1), 16)] = m.group(2)
        # MAIN0 functions translated by hand in port2/runtime (asm-only in the decomp, inside coarse units)
        rt = os.path.join(ROOT, "port2", "runtime")
        for fn in sorted(os.listdir(rt)):
            if not fn.endswith(".c"): continue
            for m in re.finditer(r"^\w[\w \t\*]*?\b((?:func|FUN)_([0-9A-Fa-f]{8}))\s*\([^;]*$", open(os.path.join(rt, fn)).read(), re.M):
                self.native.setdefault(int(m.group(2), 16), m.group(1))
        self.code = sorted(tuple(int(x, 16) for x in l.split()) for l in open(os.path.join(outdir, "code_ranges.txt")))
        self.code_starts = [c[0] for c in self.code]
        self.syms = {}                                    # name -> address
        self.by_addr = {}                                 # data address -> [names]
        for l in open(os.path.join(outdir, "symaddr.txt")):
            n, a = l.split(); a = int(a, 16); self.syms[n] = a
            if a not in self.unit and "__" not in n: self.by_addr.setdefault(a, []).append(n)
        self.data_addrs = sorted(self.by_addr)
        # every function start of the overlay region, over all overlays (X000, the areas, the SYSTEM set): each
        # gets a dispatch thunk (port_thunk_<addr>) that picks the native code of the resident overlay at run time
        from units import units
        self.U = units()
        self.ovl_starts = set()
        for p, us in self.U.items():
            for u in us:
                if GP.OVL_BASE <= u.addr < GP.OVL_END: self.ovl_starts.add(u.addr)
        vp = os.path.join(outdir, "vary.txt")
        self.vary = set(int(l, 16) for l in open(vp) if l.strip()) if os.path.exists(vp) else set()

    def thunk(self, a):
        """name of the dispatch thunk for retail address a, or None: an overlay-region function start whose code is
        not the one native X000 function every overlay shares"""
        if a in self.ovl_starts and (a in self.vary or a not in self.native): return "port_thunk_%08X" % a
        return None

    def is_code(self, a):
        i = bisect.bisect_right(self.code_starts, a) - 1
        return i >= 0 and self.code[i][0] <= a < self.code[i][1]

    def func_name(self, a):
        """native (or to-be-stubbed) name of the function starting at a, else None"""
        return self.native.get(a) or self.unit.get(a)

    def data_name(self, a):
        """the canonical data name at exactly a (prefers D_xxxxxxxx, then DAT_, then the rest)"""
        ns = self.by_addr.get(a)
        if not ns: return None
        key = lambda n: (0 if re.match(r"D_[0-9A-F]{8}$", n) else 1 if re.match(r"(DAT|PTR|PTR_DAT)_", n) else 2, n)
        return sorted(ns, key=key)[0]

    def resolve(self, a):
        """(symbol, offset, kind) for retail address a; kind in func/data/blob, or None."""
        f = self.func_name(a)
        if f: return f, 0, "func"
        reg, off = region_of(a, self.regs)
        if reg is None: return None
        if self.is_code(a):                           # inside a code unit: the image bytes (a label of a native
            return reg, off, "label" if self._unit_native(a) else "blob"    # function has no native equivalent)
        n = self.data_name(a)
        if n: return n, 0, "data"
        i = bisect.bisect_right(self.data_addrs, a) - 1   # the nearest data symbol before a, in the same region
        if i >= 0 and region_of(self.data_addrs[i], self.regs)[0] == reg and a - self.data_addrs[i] < 0x10000:
            return self.data_name(self.data_addrs[i]), a - self.data_addrs[i], "data"
        return reg, off, "blob"

    def _unit_native(self, a):
        i = bisect.bisect_right(self.code_starts, a) - 1
        return i >= 0 and self.code[i][0] in self.native

    def alias_expr(self, a):
        """ld-script expression for retail address a (data only), or None"""
        reg, off = region_of(a, self.regs)
        return "%s + 0x%X" % (reg, off) if reg else None


# ---------------------------------------------------------------------------------------------------------------
# step 1: gen_port's filter with the retail-data model and the KUSEG rewrite swapped out
def p2_retail_definition(line, retail):
    """A file-scope definition of a retail-named datum becomes an extern: the generated data image is the one copy
    (its initialiser, if any, is dropped: the image holds the same bytes)."""
    m = GP.RETAIL_DEF.match(line)
    if not m or m.group("decl").split()[0] in ("return", "goto", "case", "else", "if", "while", "for", "switch", "do"):
        return line
    n = m.group("name")
    if n in ("main",) or n.startswith("port_"): return line
    if not (n in retail or GP.RETAIL_NAME.match(n)): return line
    decl, dims = m.group("decl"), m.group("dims").strip()
    ext = "extern %s %s%s;" % (decl.strip(), n, dims)
    if m.group("end") == ";":
        return ext + " /* PORT: retail copy */" + line[m.end():]
    # keep the initialiser as an unused static copy so that a line-based initialiser stays balanced
    return "%s static %s port_copy_%s%s __attribute__((unused)) =%s" % (ext, decl.strip(), n, dims, line[m.end():])


def p2_kuseg_mask(line):
    """OT-link idiom: `*(u_long *)otz = (u_long)p;` -> port_otlink; `((u_long)p) & 0xFFFFFF` -> port_otaddr."""
    m = GP.KUSEG_LINK_STORE_RE.match(line)
    if m:
        return "%sport_otlink(%s); /* PORT: 24-bit link */" % (m.group(1), m.group(3))
    m = GP.KUSEG_ASSIGN_RE.match(line)
    if m and "LOAD_BUFFER_ALT" in m.group(2):
        return "%sport_otaddr(%s); /* PORT: 24-bit link */" % (m.group(1), m.group(2))
    return GP.KUSEG_CAST_RE.sub(r"port_otaddr((\1)\2)", line)


GP.retail_definition = p2_retail_definition
GP.port_kuseg_mask = p2_kuseg_mask
GP.ROOT = ROOT

# ---------------------------------------------------------------------------------------------------------------
# step 2: the address resolver
LIT_RE = re.compile(r"(?<![\w.])0[xX](8[0-9A-Fa-f]{7}|[aA]0[0-9A-Fa-f]{6}|1[fF]80[0-9A-Fa-f]{4})[uUlL]{0,3}(?![\w.])")


class Resolver:
    def __init__(self, amap):
        self.amap = amap
        self.used = {}            # symbol -> kind
        self.resolved = 0
        self.unresolved = []      # (file, line, literal, why)
        self.labels = []          # (file, line, literal, function): addresses inside natively compiled functions
        self.constants = 0        # 0x803Cxxxx and round 0x80NN0000 values: not addresses
        self.by_sym = {}

    def text(self, txt, path):
        blank = GP._blank(txt)
        copies = []               # ranges of the dropped `port_copy_` initialisers (the image holds those bytes)
        for cm in re.finditer(r"port_copy_\w+[^;{]*=\s*\{", blank):
            d, j = 1, cm.end()
            while j < len(blank) and d:
                d += (blank[j] == "{") - (blank[j] == "}"); j += 1
            copies.append((cm.start(), j))
        out, pos = [], 0
        for m in LIT_RE.finditer(blank):
            a = int(m.group(1), 16)
            if a < 0x80000000 and not (0x1F800000 <= a < 0x1F803000): continue
            if a == 0x80000000:                                        # the KSEG0 bit: `p | 0x80000000` (an OT
                before = blank[:m.start()].rstrip()                     # link back to a pointer) is the identity
                if before.endswith(("|", "+")):                          # on native pointers; as a mask it stays
                    out.append(txt[pos:m.start()]); out.append("0u /* PORT: KSEG0 */"); pos = m.end()
                continue
            if a >= 0x80200000 or (0x80000000 <= a < 0x80200000 and (a & 0xFFFF) == 0 and a != 0x80010000):
                self.constants += 1; continue                           # 0x803Cxxxx, 0x80020000: values
            line = blank.count("\n", 0, m.start()) + 1
            in_copy = any(s <= m.start() < e for s, e in copies)
            r = self.amap.resolve(a)
            if r is None:
                if not in_copy: self.unresolved.append((path, line, m.group(0), "no region"))
                continue
            sym, off, kind = r
            if kind == "label" and not in_copy:
                self.labels.append((path, line, m.group(0), self.amap.unit.get(self._func_at(a), "?")))
            if kind == "label": kind = "blob"
            # a code address used as an integer with an offset: keep it a code pointer
            self.used[sym] = kind
            self.by_sym[sym] = self.by_sym.get(sym, 0) + 1
            rep = "PORT_ADDR(port_a_%s, 0x%X)" % (sym, off) if kind != "blob" else "PORT_ADDR(%s, 0x%X)" % (sym, off)
            out.append(txt[pos:m.start()]); out.append(rep); pos = m.end()
            self.resolved += 1
        out.append(txt[pos:])
        return "".join(out)

    def _func_at(self, a):
        i = bisect.bisect_right(self.amap.code_starts, a) - 1
        return self.amap.code[i][0] if i >= 0 else 0


# files that define their own copy of a function another file defines (gen_port's STATIC_AFTER_EXTERN)
DUP_DEFS = {"startAreaBgmWithReverb": ["getSfxVabOffset"]}
# area files whose definition of a function duplicates the one of the file named after it: made weak
WEAK_DEFS = {"x005/stepBacoTownNpcTalk.c": ["X005_updateBacoTownNpc"], "x011/stepFactoryNpcTalk.c": ["X011_updateFactoryNpc"]}


def materialise(d):
    """Turn directory symlinks under d into real directories of file symlinks (so that a header copy can replace
    a file symlink without ever writing through to include/)."""
    for root, dirs, files in os.walk(d):
        for n in list(dirs):
            p = os.path.join(root, n)
            if os.path.islink(p):
                target = os.path.realpath(p)
                os.remove(p); os.makedirs(p)
                for f in os.listdir(target):
                    os.symlink(os.path.join(target, f), os.path.join(p, f))
        dirs[:] = [n for n in dirs]


def rewrite_tree(d, res, relroot):
    for root, dirs, files in os.walk(d):
        for f in files:
            p = os.path.join(root, f)
            if not f.lower().endswith((".h", ".inc", ".c")): continue
            try: txt = open(p, errors="replace").read()
            except OSError: continue
            rel = os.path.relpath(os.path.realpath(p), ROOT) if os.path.islink(p) else os.path.relpath(p, relroot)
            new = res.text(txt, rel)
            if new != txt:
                if os.path.islink(p): os.remove(p)
                open(p, "w").write(new)


BU_RE = re.compile(r"(?<![\w.>])(open|close|read|write|lseek|erase|format|firstfile|nextfile|undelete|rename)(\s*\()")
ASMNAME_RE = re.compile(r"(?<=[\w)\]])\s+(?:__asm__|asm)\s*\(\s*\"(?!(?:[atvsk]\d|zero|at|gp|sp|fp|ra)\")([A-Za-z_]\w*)\"\s*\)")


def sources(outdir, progs):
    import shutil, sysovl
    for name in ("inc", "inc24"):          # materialised header copies of the previous run
        shutil.rmtree(os.path.join(outdir, name), ignore_errors=True)
    isarea = lambda p: re.match(r"X0\d\d$", p) and p != "X000"
    base = [p for p in progs if p not in sysovl.SYSOVL and not isarea(p)]
    GP.sources(outdir, base, [p for p in progs if p in sysovl.SYSOVL], [p for p in progs if isarea(p)])
    # the BIOS memory-card file system (open/read/write/... on "bu00:") must not shadow the host libc's calls
    # of the same names (SDL, stdio): the game's calls are renamed port_bu_*, which port2/shims/card.c defines
    for root, dirs, files in os.walk(os.path.join(outdir, "src")):
        for f in files:
            pth = os.path.join(root, f); txt = open(pth, errors="replace").read()
            new = BU_RE.sub(r"port_bu_\1\2", txt)
            # a declaration aliased by name (`extern u8 V asm("DAT_8009c971");`, a handful of game files) names the
            # C symbol: on Win32 it needs the "_" prefix too (PORT2_ASM of port2_syms.h adds the target's)
            new = ASMNAME_RE.sub(r" PORT2_ASM(\1)", new)
            if new != txt: open(pth, "w").write(new)
    amap = AddrMap(outdir)
    res = Resolver(amap)
    for name in ("inc", "inc24"):
        materialise(os.path.join(outdir, name))
        rewrite_tree(os.path.join(outdir, name), res, outdir)
    rewrite_tree(os.path.join(outdir, "src"), res, outdir)
    # a file that defines a second (originally static) copy of a function another file defines: file-local name
    for stem, names in DUP_DEFS.items():
        for prog in progs:
            p = os.path.join(outdir, "src", prog.lower(), stem + ".c")
            if not os.path.exists(p): continue
            txt = open(p).read()
            defs = "".join("#define %s %s__%s\n" % (n, stem, n) for n in names)
            if defs not in txt: open(p, "w").write(txt.replace("\n", "\n" + defs, 1))
    for rel, names in WEAK_DEFS.items():
        p = os.path.join(outdir, "src", rel)
        if not os.path.exists(p): continue
        txt = open(p).read()
        pr = "".join("#pragma weak %s\n" % n for n in names)
        if pr not in txt: open(p, "w").write(txt.replace("\n", "\n" + pr, 1))
    with open(os.path.join(outdir, "port2_syms.h"), "w") as f:
        f.write("/* generated by port2/tools/gen2.py: every symbol the address resolver used */\n")
        # the asm name carries the target's C symbol prefix ("" on Linux, "_" on Win32/mingw), so port_a_X is the
        # same symbol as the game's own typed extern of X on both
        f.write("#define PORT2_STR_(x) #x\n#define PORT2_STR(x) PORT2_STR_(x)\n"
                "#define PORT2_ASM(x) __asm__(PORT2_STR(__USER_LABEL_PREFIX__) #x)\n")
        for s, k in sorted(res.used.items()):
            if k != "blob": f.write("extern unsigned char port_a_%s[] PORT2_ASM(%s);\n" % (s, s))
        for n, s, e, _, _ in amap.regs:
            f.write("extern unsigned char %s[];\n" % n)
    with open(os.path.join(outdir, "literals.txt"), "w") as f:
        f.write("# address literals the resolver could not map to a symbol (left as integers)\n")
        for p, l, lit, why in res.unresolved: f.write("%s:%d: %s (%s)\n" % (p, l, lit, why))
        f.write("# addresses inside natively compiled functions (resolved to the image bytes: a label, not callable)\n")
        for p, l, lit, fn in res.labels: f.write("%s:%d: %s (label in %s)\n" % (p, l, lit, fn))
        f.write("# constants skipped: %d\n" % res.constants)
    with open(os.path.join(outdir, "literals_resolved.txt"), "w") as f:
        for s, n in sorted(res.by_sym.items(), key=lambda e: -e[1]): f.write("%6d %s\n" % (n, s))
    print("gen2: %d address literals resolved (%d of them labels inside native functions), %d unresolved, "
          "%d constants skipped (%s/literals.txt), %d symbols declared"
          % (res.resolved, len(res.labels), len(res.unresolved), res.constants, outdir, len(res.used)))


# ---------------------------------------------------------------------------------------------------------------
# step 3: link-time resolution: data image, aliases, stubs
FORCE_STUB = set("open close read write lseek ioctl format firstfile nextfile erase undelete rename cd "
                 "start _96_remove _96_init".split())
LIBC_OK = set("memcpy memset memmove memcmp strlen strcpy strncpy strcmp strncmp strcat strchr strrchr sprintf "
              "printf puts putchar atoi abs labs toupper tolower isdigit isalpha isspace isupper islower".split())


def libc_symbols(nm):
    if "mingw" in nm:      # the mingw-w64 CRT: import libraries + libmingwex (names carry the Win32 "_" prefix)
        d = os.path.join(os.path.dirname(os.path.dirname(nm)), "i686-w64-mingw32", "lib")
        if not os.path.isdir(d): d = "/usr/i686-w64-mingw32/lib"
        out = set()
        for lib in ("libmsvcrt.a", "libmingwex.a", "libkernel32.a", "libmingw32.a"):
            if not os.path.exists(os.path.join(d, lib)): continue
            r = subprocess.run([nm, os.path.join(d, lib)], capture_output=True, text=True).stdout
            for l in r.split("\n"):
                w = l.split()
                if len(w) == 3 and w[1] in "TWiDIR" and w[2].startswith("_") and not w[2].startswith("__imp_"): out.add(w[2][1:])
        return out
    for p in ("/usr/lib32/libc.so.6", "/usr/lib/i386-linux-gnu/libc.so.6", "/lib32/libc.so.6"):
        if os.path.exists(p):
            r = subprocess.run([nm, "-D", p], capture_output=True, text=True).stdout
            return set(l.split()[2].split("@")[0] for l in r.split("\n") if len(l.split()) == 3 and l.split()[1] in "TWiD")
    return set()


def emit_data(outdir, amap, needed):
    """OUTDIR/data.c: one relocatable array per region, initialised from the retail image.  Aligned words that hold
    the address of a function or of a byte of any region become symbol references (relocations), exactly what the
    matched sources do with `.word D_xxxxxxxx`; everything else stays a number."""
    lines = ["/* generated by port2/tools/gen2.py from the user's MAIN0.EXE / X000.BIN: game data, never commit */",
             "#define PORT_ADDR(a, o) ((unsigned int)((unsigned char *)(a) + (o)))"]
    funcs, nptr, nfunc, ncodein, novl = set(), 0, 0, 0, 0
    thunks_used, nthunk = set(), 0
    # RAM is ONE array (port_ram, 2 MB, the retail layout) so that a CD read or a memset that crosses a region
    # boundary (LD0SYS.BIN lands across the end of MAIN0's .data into its .bss) stays contiguous; the region names
    # are aliases into it (syms.ld).  The scratchpad and the I/O page are their own arrays.
    blobs, ram_parts = [], []
    for name, s, e, path, foff in amap.regs:
        n = (e - s + 3) // 4
        in_ram = 0x80000000 <= s < 0x80200000
        if path is None or not os.path.exists(path):
            if not in_ram: blobs.append("unsigned int %s[%d] __attribute__((aligned(16))) = { 0 };   /* (.data, next to port_ram: below 16 MB) */" % (name, n))
            continue
        data = open(path, "rb").read()[foff:foff + (e - s)]
        data += b"\0" * (n * 4 - len(data))
        words = []
        for i in range(n):
            w = int.from_bytes(data[4 * i:4 * i + 4], "little")
            a = s + 4 * i
            if amap.is_code(a) or w < 0x80010000 and not (0x1F800000 <= w < 0x1F800400) or w >= 0x80200000 \
                    or (w & 3) and not (0x1F800000 <= w < 0x1F800400) or (w & 0xFFFF) == 0:   # (round values: flags, not pointers)
                words.append("0x%X" % w); continue
            th = amap.thunk(w)
            if th:
                thunks_used.add(th); nthunk += 1
                words.append("(unsigned int)(unsigned long)&%s" % th); continue
            f = amap.func_name(w)
            if f:
                funcs.add(f); nfunc += 1
                words.append("(unsigned int)(unsigned long)&%s" % f); continue
            reg, off = region_of(w, amap.regs)
            if reg is None:
                words.append("0x%X" % w); continue
            if amap.is_code(w):          # a pointer into a function body (handler entry without native code)
                if GP.OVL_BASE <= w < GP.OVL_END: novl += 1
                else: ncodein += 1
            nptr += 1
            words.append("PORT_ADDR(%s, 0x%X)" % (reg, off))
        body = ",\n".join(", ".join(words[j:j + 8]) for j in range(0, n, 8))
        if in_ram: ram_parts.append("[0x%X] = %s" % ((s - 0x80000000) // 4, body))
        else: blobs.append("unsigned int %s[%d] __attribute__((aligned(16))) = {\n%s\n};" % (name, n, body))
    blobs.append("unsigned int port_ram[0x80000] __attribute__((aligned(4096))) = {\n%s\n};" % ",\n".join(ram_parts))
    with open(os.path.join(outdir, "regions.ld"), "w") as f:
        f.write("/* generated by port2/tools/gen2.py: the RAM regions are offsets into port_ram */\n")
        for name, s, e, _, _ in amap.regs:
            if 0x80000000 <= s < 0x80200000: f.write("%s = port_ram + 0x%X;\n" % (name, s - 0x80000000))
    for n, s, e, _, _ in amap.regs: lines.append("extern unsigned int %s[];" % n)
    for n, s, e, _, _ in amap.regs: lines.append("const unsigned int %s_size = 0x%X;" % (n, e - s))
    for f in sorted(funcs | thunks_used): lines.append("extern void %s();" % f)
    lines += blobs
    open(os.path.join(outdir, "data.c"), "w").write("\n".join(lines) + "\n")
    return nptr, nfunc + nthunk, funcs, ncodein, novl


def link(outdir, nm, objlist):
    amap = AddrMap(outdir)
    defined, undefined, data_defined = set(), set(), set()
    objs = [l.strip() for l in open(objlist) if l.strip()]
    r = ""
    for i in range(0, len(objs), 400):
        r += subprocess.run([nm, "-A"] + objs[i:i + 400], capture_output=True, text=True).stdout
    # Win32 (mingw nm): every C symbol carries a "_" prefix; the names are handled unprefixed and the linker script
    # and the thunks get the prefix back when written (ldsym)
    pfx = "_" if "mingw" in nm else ""
    for l in r.split("\n"):
        parts = l.split()
        if len(parts) != 3: continue
        o, t, n = parts
        if pfx:
            # (_Name@N and __imp__Name: Win32 stdcall imports, the DLLs' own)
            if not n.startswith(pfx) or "@" in n or n.startswith("__imp_"): continue
            n = n[1:]
        if t == "U": undefined.add(n); continue
        if t.isupper(): defined.add(n)
        if "/gen/src/" in o and t in "DBRCGS": data_defined.add(n)
    undefined -= defined
    libc = libc_symbols(nm) - FORCE_STUB
    asm_alias = dict(l.rstrip("\n").split(" ", 1) for l in open(os.path.join(outdir, "asm_aliases.txt")) if l.strip())

    def addr_of(n):
        if n in amap.syms: return amap.syms[n]
        m = re.match(r"(?:func|FUN|D|DAT|jtbl|LAB|PTR|PTR_DAT|P)_([0-9A-Fa-f]{8})$", n)
        if not m: m = re.match(r"(?:D|DAT|P)_([0-9A-Fa-f]{8})[A-Za-z_]\w*$", n)
        return int(m.group(1), 16) if m else None

    ld, stubs, data_aliases, dummies = [], [], 0, []
    region_names = set(r[0] for r in amap.regs)
    import sysovl
    ovl_re = re.compile(r"(?:%s|X0\d\d)_\w" % sysovl.PROG_RE)
    thunk_aliases = 0
    for n in sorted(undefined):
        # (SDL_: the host library; port_: the runtime, or the tables this step generates)
        if n.startswith(("__", "_GLOBAL_OFFSET", "SDL_", "port_")) or n in libc or n in region_names: continue
        if n in asm_alias:
            ld.append("%s = %s;" % (n, asm_alias[n])); continue
        a = addr_of(n)
        f = amap.func_name(a) if a is not None else None
        is_data_name = re.match(r"(?:D|DAT|PTR|PTR_DAT|jtbl|P)_[0-9A-Fa-f]{8}", n)
        if a is not None and not is_data_name and not (f and f in defined and f != n and a not in amap.vary):
            th = amap.thunk(a)
            if th is None and ovl_re.match(n) and GP.OVL_BASE <= a < GP.OVL_END and a in amap.ovl_starts:
                th = "port_thunk_%08X" % a
            if th:        # a function of the overlay region: the resident overlay decides at call time
                ld.append("%s = %s;" % (n, th)); thunk_aliases += 1; continue
        if f and f in defined and f != n:
            ld.append("%s = %s;" % (n, f)); continue
        if a is not None and (not amap.is_code(a) or is_data_name):
            # (a data name inside an X000 code unit: another overlay's data, or a table a coarse unit covers)
            e = amap.alias_expr(a)
            if e:
                ld.append("%s = %s;" % (n, e)); data_aliases += 1; continue
            dummies.append((n, a)); continue
        if a is not None and amap.is_code(a) and not f:
            # inside an X000 unit, not a function start of any overlay: a label; not callable natively
            stubs.append((n, a, "overlay-target" if GP.OVL_BASE <= a < GP.OVL_END else "label")); continue
        unit = amap.unit.get(a) if a is not None else None
        kind = "port-runtime" if n.startswith("port_") else "psyq/bios" if (a is None or GP_islib(a)) else "game-asm"
        stubs.append((n, a, kind))
    # dispatch thunks (thunks.S): one per overlay-region function start; and the run-time tables (addrmap.c):
    # retail address -> native function of MAIN0/X000 (for the relocation of a loaded overlay), address -> thunk,
    # and the code ranges of every overlay (words inside them are instructions, never relocated)
    thunks = sorted(a for a in amap.ovl_starts if amap.thunk(a))
    with open(os.path.join(outdir, "thunks.S"), "w") as f:
        f.write("/* generated by port2/tools/gen2.py: call dispatch into the overlay region */\n.text\n")
        for a in thunks:
            f.write((".globl %sport_thunk_%08X\n" + ("" if pfx else ".type port_thunk_%08X,@function\n") + "%sport_thunk_%08X:\n"
                    "  pushl $0x%08X\n  call %sport_resolve_call\n  addl $4, %%esp\n  jmp *%%eax\n")
                    % ((pfx, a, a, pfx, a, a, pfx) if not pfx else (pfx, a, pfx, a, a, pfx)))
        if not pfx: f.write('.section .note.GNU-stack,"",@progbits\n')
    with open(os.path.join(outdir, "addrmap.c"), "w") as f:
        f.write("/* generated by port2/tools/gen2.py: run-time address tables */\n#include \"port_runtime.h\"\n")
        am = sorted((a, n) for a, n in amap.unit.items() if n in defined and amap.thunk(a) is None)
        for a, n in sorted(amap.native.items()):
            if n in defined and amap.thunk(a) is None and a not in dict(am): am.append((a, n))
        am = sorted(dict(am).items())
        for a, n in am: f.write("extern void %s();\n" % n)
        for a in thunks: f.write("extern void port_thunk_%08X();\n" % a)
        f.write("const PortAddr port_addrmap[] = {\n%s  {0, 0}\n};\nconst int port_addrmap_count = %d;\n"
                % ("".join("  {0x%08X, (void *)%s},\n" % e for e in am), len(am)))
        f.write("const PortAddr port_thunkmap[] = {\n%s  {0, 0}\n};\nconst int port_thunkmap_count = %d;\n"
                % ("".join("  {0x%08X, (void *)port_thunk_%08X},\n" % (a, a) for a in thunks), len(thunks)))
        # code ranges per overlay: areas by their index in port_areas (X000 = 0), SYSTEM overlays by header id
        alist = ["X000"] + sorted(p for p in amap.U if re.match(r"X0\d\d$", p) and p != "X000")
        ents = []
        for p in alist + list(sysovl.PROGS):
            rs = sorted((u.addr, u.addr + u.size) for u in amap.U[p] if GP.OVL_BASE <= u.addr < GP.OVL_END)
            merged = []
            for s, e in rs:
                if merged and s <= merged[-1][1]: merged[-1][1] = max(merged[-1][1], e)
                else: merged.append([s, e])
            if p in sysovl.SYSOVL:
                fn = os.path.join(ROOT, "game", sysovl.SYSOVL[p][0])
                kind, key = 1, (int.from_bytes(open(fn, "rb").read(4), "little") if os.path.exists(fn) else 0)
            else: kind, key = 0, alist.index(p)
            f.write("static const unsigned int ranges_%s[] = {%s};\n" % (p, ", ".join("0x%08X, 0x%08X" % tuple(r) for r in merged)))
            ents.append((kind, key, p, len(merged)))
        f.write("const PortOvlCode port_ovlcode[] = {\n%s  {0, 0, 0, 0, 0}\n};\nconst int port_ovlcode_count = %d;\n"
                % ("".join("  {%d, 0x%X, \"%s\", ranges_%s, %d},\n" % (k, key, p, p, n) for k, key, p, n in ents), len(ents)))
    # data a game C file still defines by itself (a definition the filter did not rewrite): it keeps its own copy,
    # which is wrong when other files reach the retail bytes through another name; listed in the report
    own_copies = sorted(n for n in data_defined if addr_of(n) is not None and n not in undefined)
    nptr, nfunc, dfuncs, ncodein, novl = emit_data(outdir, amap, undefined)
    # functions the data image points to but nobody defines: stubs as well
    for f in sorted(dfuncs - defined - libc - set(s[0] for s in stubs)):
        if f.startswith("port_thunk_"): continue
        a = next((a for a, n in amap.unit.items() if n == f), None)
        stubs.append((f, a, "psyq/bios" if a is None or GP_islib(a) else "game-asm"))
    ldsym = (lambda l: re.sub(r"(?<![\w$.])([A-Za-z_]\w*)", r"_\1", l)) if pfx else (lambda l: l)
    with open(os.path.join(outdir, "syms.ld"), "w") as f:
        f.write("/* generated by port2/tools/gen2.py: aliases into the data image and between function names */\n")
        for l in open(os.path.join(outdir, "regions.ld")):
            f.write(l if l.startswith("/*") else ldsym(l))
        f.write("\n".join(ldsym(l) for l in ld) + "\n")
        for n, a in dummies: f.write(ldsym("%s = port_dummy_%s;\n" % (n, n)))
    with open(os.path.join(outdir, "stubs.c"), "w") as f:
        f.write("/* generated by port2/tools/gen2.py: code the port has no native version of yet */\n")
        f.write("void port_stub_hit(const char *name);\n")
        for n, a in dummies: f.write("unsigned int port_dummy_%s[4];\n" % n)
        for n, a, k in stubs:
            f.write("int %s() { port_stub_hit(\"%s\"); return 0; } /* %s%s */\n"
                    % (n, n, k, " 0x%08X" % a if a is not None else ""))
    # hand-translated asm still needed: game units of MAIN0/X000 without C (gen_port kept them interpreted)
    from units import units
    U = units()
    asm_only = sorted((u.addr, u.name) for p in ("MAIN0", "X000") for u in U[p] if not u.lib and not u.file)
    lit = open(os.path.join(outdir, "literals.txt")).read()
    unres = [l for l in lit.split("\n") if l and not l.startswith("#") and "(label in" not in l]
    labels = [l for l in lit.split("\n") if "(label in" in l]
    const = re.search(r"# constants skipped: (\d+)", lit).group(1)
    nres = sum(int(l.split()[0]) for l in open(os.path.join(outdir, "literals_resolved.txt")))
    with open(os.path.join(outdir, "report.txt"), "w") as f:
        f.write("port2 build report (generated by port2/tools/gen2.py)\n\n")
        f.write("address literals: %d resolved to symbols, %d of them labels inside native functions (listed below), "
                "%d unresolved, %s non-address constants skipped\n" % (nres, len(labels), len(unres), const))
        f.write("data image: %d regions, %d pointer words to data, %d pointer words to functions, "
                "%d words pointing into the overlay region where X000 has no function start (other overlays' handlers), %d into MAIN0/X000 code interiors (switch jump tables and unrolled-loop entries of the matched code: unused natively)\n"
                % (len(amap.regs), nptr, nfunc, novl, ncodein))
        f.write("data aliases (retail name -> image offset): %d\n" % data_aliases)
        f.write("function aliases (renamed / address-named -> native): %d\n" % sum(1 for l in ld if not re.search(r"port_\w+_image|port_\w+_bss|port_scratch|port_io|port_hiram|port_bios", l)))
        f.write("names with no region (dummy storage): %d\n" % len(dummies))
        f.write("game objects that keep their own copy of retail-named data: %d\n" % len(own_copies))
        f.write("dispatch thunks (overlay-region function starts): %d; names aliased to a thunk: %d\n" % (len(thunks), thunk_aliases))
        f.write("stubs: %d (psyq/bios %d, game-asm %d, overlay-target %d, label %d, port-runtime %d)\n" % (
            len(stubs), *[sum(1 for s in stubs if s[2] == k) for k in ("psyq/bios", "game-asm", "overlay-target", "label", "port-runtime")]))
        f.write("hand-translated asm still needed: main (0x800163F4, port/runtime/main_port.c has it in C), the game-asm stubs above (pad driver: port/shims/pad.c), %d other MAIN0/X000 game units without C\n\n" % len(asm_only))
        f.write("## unresolved literals\n" + "".join(l + "\n" for l in unres))
        f.write("\n## literals naming a label inside a natively compiled function\n" + "".join(l + "\n" for l in labels))
        f.write("\n## game-asm stubs\n" + "".join("%s 0x%08X\n" % (n, a or 0) for n, a, k in stubs if k == "game-asm"))
        f.write("\n## overlay-target stubs (MAIN0 calls into the overlay region; dispatch by resident overlay next round)\n" + "".join("%s 0x%08X\n" % (n, a or 0) for n, a, k in stubs if k == "overlay-target"))
        f.write("\n## label stubs (addresses inside functions)\n" + "".join("%s 0x%08X\n" % (n, a or 0) for n, a, k in stubs if k == "label"))
        f.write("\n## psyq/bios stubs\n" + "".join("%s\n" % n for n, a, k in stubs if k == "psyq/bios"))
        f.write("\n## port-runtime stubs\n" + "".join("%s\n" % n for n, a, k in stubs if k == "port-runtime"))
        f.write("\n## dummies\n" + "".join("%s 0x%08X\n" % d for d in dummies))
        f.write("\n## own copies\n" + "".join("%s\n" % n for n in own_copies))
        f.write("\n## asm-only game functions\n" + "".join("%08X %s\n" % e for e in asm_only))
    print("gen2: %d aliases (%d to dispatch thunks), %d data aliases, %d thunks, %d stubs, %d dummies; report in %s/report.txt"
          % (len(ld), thunk_aliases, data_aliases, len(thunks), len(stubs), len(dummies), outdir))


_LIB = None
def GP_islib(a):
    global _LIB
    if _LIB is None:
        from units import units
        _LIB = set(u.addr for p in ("MAIN0", "X000") for u in units()[p] if u.lib)
    return a in _LIB


if __name__ == "__main__":
    a = sys.argv[1:]
    if a and a[0] == "sources":
        a[1] = os.path.abspath(a[1]); os.makedirs(a[1], exist_ok=True)
        sources(a[1], [p.upper() for p in a[2:]] or ["MAIN0", "X000"])
    elif a and a[0] == "link":
        link(a[1], a[2], a[3])
    else:
        sys.exit(__doc__)
