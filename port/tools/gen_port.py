#!/usr/bin/env python3
"""Generator for the native port build (port/CMakeLists.txt drives it; see docs/PORT.md).

  gen_port.py sources OUTDIR [PROG...]
      Picks the src/*.c files of PROG (default MAIN0 X000): every game function, plus the Psy-Q files listed in
      LIB_KEEP (pure C helpers that never touch hardware).  Each file is copied through filter() into
      OUTDIR/src/<prog>/<stem>.c (src/ itself is never modified) and listed in OUTDIR/sources.cmake.
      Also writes OUTDIR/funcmap.c: retail address -> native function, used by the runtime to call through
      function pointers that live in retail data (object handler tables, task entry points, ...).

  gen_port.py link OUTDIR NM OBJLIST
      After compiling: reads the symbols of every object (NM = host nm, OBJLIST = file with one object per line)
      and resolves them against the retail symbol table (build/full/{main0,x000}.elf, made by
      tools/build_full.py):
        * an undefined name whose retail address starts a native function -> alias in OUTDIR/syms.ld;
        * an undefined name of code we do not have (Psy-Q, hardware) -> weak logging stub in OUTDIR/stubs.c
          (port/shims/*.c override them with real implementations);
        * every data name (undefined, or defined by a C file but living in the retail image) -> its retail
          address in OUTDIR/syms.ld, so native code and the retail data image agree on one copy.
"""
import os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))

# Psy-Q files compiled as they are: pure data-structure helpers (no GPU/GTE/SPU/CD register access).
LIB_KEEP = set("""
NextPrim IsEndPrim AddPrim AddPrims CatPrim TermPrim SetSemiTrans SetShadeTex SetPolyF3 SetPolyFT3 SetPolyG3
SetPolyGT3 SetPolyF4 SetPolyFT4 SetPolyG4 SetPolyGT4 SetSprt8 SetSprt16 SetSprt SetTile1 SetTile8 SetTile16 SetTile
SetLineF2 SetLineG2 SetLineF3 SetLineG3 SetLineF4 SetLineG4 SetDrawTPage SetDrawMove SetDrawLoad MargePrim
SetPriority SetDefDrawEnv SetDefDispEnv GetTPage GetClut
OpenTIM ReadTIM get_tim_addr
tolower setDrawSyncParams callObjectInteraction
""".split())
# functions defined `static` although a shared header declares them extern
STATIC_AFTER_EXTERN = ["getSfxVabOffset"]
# Game files that are really hardware drivers (pad/SIO bit-banging, memory card): replaced by shims.
GAME_SKIP_RE = re.compile(r"^(Pad[A-Z]\w*|_pad\w+|pad(Command|Get|Queue|Reset|Resolve|SetCmd)\w*|sioWaitRxReady|_version)$")


def translate_gte_define(d):
    """`#define gte_X(args) __asm__ volatile("...MIPS COP2..." : : "r"(a), ...)` -> the same macro in C on the
    software GTE of port/runtime/gte.c.  The macros are translated per file because several files define the same
    name with different store offsets."""
    m = re.match(r"\s*#\s*define\s+(gte_\w+)(\([^)]*\))?\s*(.*)$", d, re.S)
    name, params, body = m.group(1), m.group(2) or "", m.group(3)
    am = re.search(r"__asm__\s*(?:volatile)?\s*\((.*)\)\s*$", body, re.S)
    if not am:
        return "#define %s%s %s" % (name, params, body)
    inner = am.group(1)
    parts = inner.split(":")
    code = "".join(re.findall(r'"((?:[^"\\]|\\.)*)"', parts[0]))
    ops = []
    for sec in parts[1:3]:
        ops += [o.strip() for o in re.findall(r'"[^"]*"\s*\(([^)]*)\)', sec)]
    arg = lambda t: "(%s)" % ops[int(t.strip()[1:])]
    c, tmp, cnt = [], {}, [0]
    def newtmp(r):
        tmp[r] = "_t%d" % cnt[0]; cnt[0] += 1
        return tmp[r]
    for ins in re.split(r"[;\n]", code.replace("\\n", "\n")):
        ins = ins.strip()
        if not ins or ins == "nop": continue
        op, _, rest = ins.partition(" ") if " " in ins else ins.partition("\t")
        a = [x.strip() for x in rest.replace("\t", " ").split(",")]
        mm = lambda x: re.match(r"(-?\w+)\(\s*(%\d|\$\w+)\s*\)", x.replace(" ", ""))
        def addr(x):
            q = mm(x); base = arg(q.group(2)) if q.group(2).startswith("%") else tmp[q.group(2)]
            return "((unsigned char *)%s + %s)" % (base, q.group(1))
        reg = lambda x: int(x.strip().lstrip("$"))
        if op == ".word": c.append("port_gte_op(%s);" % a[0])
        elif op == "lwc2": c.append("port_gte_mtc2(%d, *(unsigned long *)%s);" % (reg(a[0]), addr(a[1])))
        elif op == "swc2": c.append("*(unsigned long *)%s = port_gte_mfc2(%d);" % (addr(a[1]), reg(a[0])))
        elif op == "mtc2": c.append("port_gte_mtc2(%d, (unsigned long)%s);" % (reg(a[1]), arg(a[0])))
        elif op == "ctc2":
            src = arg(a[0]) if a[0].startswith("%") else tmp[a[0]]
            c.append("port_gte_ctc2(%d, (unsigned long)%s);" % (reg(a[1]), src))
        elif op == "mfc2": c.append("long %s = port_gte_mfc2(%d);" % (newtmp(a[0]), reg(a[1])))
        elif op == "cfc2": c.append("long %s = port_gte_cfc2(%d);" % (newtmp(a[0]), reg(a[1])))
        elif op == "lw":
            ad = addr(a[1]); c.append("long %s = *(long *)%s;" % (newtmp(a[0]), ad))
        elif op == "sra": c.append("%s = %s >> %s;" % (tmp[a[0]], tmp[a[1]], a[2]))
        elif op == "sw": c.append("*(long *)%s = %s;" % (addr(a[1]), tmp[a[0]]))
        else:
            return "#define %s%s PORT_GTE_UNSUPPORTED(\"%s\")" % (name, params, ins)
    return "#define %s%s do { %s } while (0)" % (name, params, " ".join(c))


def tied_asm_moves(outs, ins):
    """`asm("" : "=r"(a), "=r"(b) : "0"(x), "1"(y), "r"(z))` -> `a = x; b = y;` (the old gcc's register "move";
    outputs that are not tied to an input have no value in C either and are left alone)."""
    o = re.findall(r'"=?&?r"\s*\(\s*([^()]*(?:\([^()]*\))?[^()]*)\)', outs)
    i = re.findall(r'"(\d+)"\s*\(\s*([^()]*(?:\([^()]*\))?[^()]*)\)', ins)
    stmts = ["%s = %s;" % (o[int(k)].strip(), v.strip()) for k, v in i if int(k) < len(o) and o[int(k)].strip() != v.strip()]
    return " ".join(stmts) if stmts else ";"


RETAIL_DEF = re.compile(r"^(?!\s|extern\b|static\b|typedef\b|#|//)(?P<decl>(?:(?:const|volatile|unsigned|signed|struct|union|enum)\s+)*\w+(?:\s+\*+|\s+)\**)\s*"
                        r"(?P<name>\w+)\s*(?P<dims>(?:\[[^\]]*\]\s*)*)(?P<end>=(?!=)|;)")
RETAIL_NAME = re.compile(r"(?:D|DAT|PTR|PTR_DAT|jtbl)_[0-9A-Fa-f]{8}")


def retail_definition(line, retail):
    """A file-scope definition of a name that lives in the retail image (an initialized copy of retail data, or a
    variable the asm data defines too) becomes an extern declaration of the retail name plus, when it has an
    initializer, a copy under another name.  syms.ld binds the retail name to the retail address, which the ELF
    link also does for the defining object (gas keeps relocations against global symbols); a COFF object resolves
    references to its own definitions section-relative at assembly time, so the mingw build read and wrote the
    native copy in the defining file and the retail copy everywhere else (cdLoadTask read SYS/LD0SYS.BIN into
    queueLoadList's own D_80098c44 while the load-list pointers of the rest of the game named 0x80098C44: the
    unloaded lists read as 0 and the Windows build queued garbage file ids)."""
    m = RETAIL_DEF.match(line)
    if not m or m.group("decl").split()[0] in ("return", "goto", "case", "else", "if", "while", "for", "switch", "do"):
        return line
    n = m.group("name")
    if n in ("main",) or n.startswith("port_"): return line
    if not (n in retail or RETAIL_NAME.match(n)): return line
    decl, dims = m.group("decl"), m.group("dims").strip()
    ext = "extern %s %s%s;" % (decl.strip(), n, dims)
    if m.group("end") == ";":
        return ext + " /* PORT: retail copy */" + line[m.end():]
    return "%s %s port_copy_%s%s =%s" % (ext, decl.strip(), n, dims, line[m.end():])


KUSEG_ASSIGN_RE = re.compile(r"^(\s*[^=;]+?=\s*)(.+?)\s*&\s*(0x(?:00)?[fF]{6})\s*;\s*$")
KUSEG_LINK_STORE_RE = re.compile(r"^(\s*\*[^=;]*?=\s*)\(\s*(unsigned long|u_long|u32)\s*\)\s*(\w+)\s*;\s*$")
KUSEG_CAST_RE = re.compile(r"\(\(\s*\(\s*(u_long|unsigned long|unsigned int|u32|int|s32)\s*\)\s*(\w+)\s*\)\s*&\s*(0x(?:00)?[fF]{6})\s*\)")
def port_kuseg_mask(line):
    """The game keeps some RAM pointers masked with 0xFFFFFF (KUSEG: the primitive buffer pointer NEXT_PRIM /
    D_1f800164 computed from LOAD_BUFFER_ALT, compactOrderingTable's `hole`) and dereferences them.  The port maps
    the KUSEG mirror only when the host leaves 0x10000-0x1FFFFF free (Windows often does not: a block in use by the
    loader meant the game drew its packets over the host's heap), so for the port the mask is followed by
    `| 0x80000000`: the same byte through the KSEG0 view, which is always mapped.  Only pointer values are
    rewritten: an assignment whose right-hand side names LOAD_BUFFER_ALT, and a masked pointer-to-integer cast
    `(((u_long)p) & 0xFFFFFF)`; a masked integer (`(*(int *)&p->b04 & 0xffffff) == 0x23001`, SPU voice masks)
    stays as it is."""
    m = KUSEG_LINK_STORE_RE.match(line)
    if m:   # `*(unsigned long *)otz = (unsigned long)p;`: an ordering-table link; a KSEG0 pointer stored whole
            # would put 0x80 in the tag's length byte (a KUSEG pointer had 0 there), so store the 24-bit link
        return "%s((%s)%s & 0xFFFFFF); /* PORT: 24-bit link */" % (m.group(1), m.group(2), m.group(3))
    m = KUSEG_ASSIGN_RE.match(line)
    if m and "LOAD_BUFFER_ALT" in m.group(2):
        return "%s((%s) & %s) | 0x80000000u; /* PORT: KSEG0 instead of KUSEG */" % (m.group(1), m.group(2), m.group(3))
    return KUSEG_CAST_RE.sub(r"((((\1)\2) & \3) | 0x80000000u /* PORT: KSEG0 */)", line)


def brace_depth(line, depth, in_comment):
    """Brace depth after `line` (braces inside comments, string and character literals do not count)."""
    i, n = 0, len(line)
    while i < n:
        c = line[i]
        if in_comment:
            if line.startswith("*/", i): in_comment = False; i += 2; continue
            i += 1; continue
        if line.startswith("//", i): break
        if line.startswith("/*", i): in_comment = True; i += 2; continue
        if c in "\"'":
            j = i + 1
            while j < n and line[j] != c:
                j += 2 if line[j] == "\\" else 1
            i = j + 1; continue
        if c == "{": depth += 1
        elif c == "}": depth -= 1
        i += 1
    return depth, in_comment


def check_retail_rewrites(text, path):
    """A retail_definition rewrite inside a function body turns a local into a store through the retail symbol
    of the same name (a lib alias such as `h` = SfxPlay2: native code on both hosts, so the Windows build wrote
    the player's position pointer over SfxPlay2 when the player held a pig).  Stop the generation, since the
    link cannot tell such a reference from a legitimate one."""
    depth, in_comment = 0, False
    for i, line in enumerate(text.split("\n"), 1):
        if (depth > 0 or in_comment) and ("/* PORT: retail copy */" in line or " port_copy_" in line):
            sys.exit("gen_port: %s:%d: retail-named definition rewritten inside a block: %s" % (path, i, line.strip()))
        depth, in_comment = brace_depth(line, depth, in_comment)


def filter_src(text, dropped=None, retail=()):
    """Make matching-oriented C compile natively without changing what it does.  Names of functions whose body is
    top-level MIPS asm are appended to `dropped` (they need a C version in port/shims/).  `retail` names data that
    the retail image holds (see retail_definition)."""
    out, pend, toplevel = [], None, None
    depth, in_comment = 0, False      # brace depth outside comments and strings: 0 is file scope
    for line in text.split("\n"):
        at_file_scope = depth == 0 and not in_comment
        depth, in_comment = brace_depth(line, depth, in_comment)
        if toplevel is not None:      # inside a top-level asm block
            toplevel += "\n" + line
            if line.rstrip().endswith(");"):
                if dropped is not None: dropped += re.findall(r"\.globl\s+(\w+)", toplevel)
                for a, e in re.findall(r"(?:^|[\"\s;])(\w+)\s*=\s*([\w\s+\-]+?)\s*(?:\"|;|$)", toplevel):
                    ASM_ALIASES[a] = e       # symbol assignment in top-level asm, e.g. FILE_LINK_SIZES = FILE_LINKS + 4
                toplevel = None
            continue
        if re.match(r'(__asm__|asm)\s*\(\s*"', line):
            if line.rstrip().endswith(");"):
                if dropped is not None: dropped += re.findall(r"\.globl\s+(\w+)", line)
            else:
                toplevel = line
            out.append(";/* PORT: top-level MIPS asm removed */")
            continue
        # old gcc accepts `static` after an extern declaration from a header; modern gcc does not
        line = re.sub(r"^static\s+(?=[\w\s\*]*\b(?:%s)\s*\()" % "|".join(STATIC_AFTER_EXTERN), "", line)
        # address-named static tables (initialized copies of retail data): make them global so the link step
        # binds the name to the retail address (the retail image is the one copy the game reads and writes)
        # (both at file scope only: a local declared on an unindented line, `int *h;` in ObjMotionStep, is a local
        # even when a retail symbol has its name: x000.full.ld PROVIDEs the lib aliases `f`, `g`, `h`, so the
        # Windows build wrote the player's x position pointer over SfxPlay2's code when the player held a pig)
        if at_file_scope:
            line = re.sub(r"^static\s+(?=[\w\s\*]*\b(?:D|DAT|PTR|PTR_DAT)_[0-9A-Fa-f]{8}\s*(?:\[[^\]]*\]\s*)*=)", "", line)
            line = retail_definition(line, retail)
        line = port_kuseg_mask(line)
        # old-gcc lvalue cast `(T *)x = v;`
        line = re.sub(r"^(\s*)\(\s*(\w+\s*\*+)\s*\)\s*(\w+)\s*=(?!=)", r"\1*(\2 *)&\3 =", line)
        if pend is not None:
            pend += "\n" + line.rstrip().rstrip("\\")
            if not line.rstrip().endswith("\\"):
                out.append(translate_gte_define(pend)); pend = None
            continue
        # local GTE macro definitions (inline MIPS COP2 asm) -> C on the software GTE
        if re.match(r"\s*#\s*define\s+gte_\w+", line):
            if line.rstrip().endswith("\\"): pend = line.rstrip().rstrip("\\")
            else: out.append(translate_gte_define(line))
            continue
        # register variables pinned to MIPS registers: drop the pin
        line = re.sub(r'\s*(?:__asm__|\basm)\s*\(\s*"\$?(?:[atvsk]\d|zero|at|gp|sp|fp|ra|\d+)"\s*\)(?=\s*[;=,]|\s*$)', "", line)
        # empty asm statements with outputs tied to inputs ("=r"(out) : "0"(in)): the old gcc's "move": out = in
        # (dropping them left `out` uninitialised when the two names differ, e.g. I0NFO3 func_800EAD48's player sprite)
        line = re.sub(r'\b(?:__asm__|asm)\s*(?:volatile\s*|__volatile__\s*)?\(\s*""\s*:((?:[^():]|\([^()]*\))*):((?:[^():]|\([^()]*\))*)(?::[^()]*)?\)\s*;',
                      lambda m: tied_asm_moves(m.group(1), m.group(2)), line)
        # empty asm statements with operands or clobbers inside a line (register allocation hints for the old gcc)
        line = re.sub(r'\b(?:__asm__|asm)\s*(?:volatile\s*|__volatile__\s*)?\(\s*""\s*:(?:[^()]|\([^()]*\))*\)\s*;', ";", line)
        # asm statements with MIPS instructions or register clobbers (empty barriers asm("") are harmless)
        if re.match(r'\s*(__asm__|asm)\s*(volatile\s*)?\(\s*"[^"]', line) or \
           re.match(r'\s*(__asm__|asm)\s*(volatile\s*)?\(\s*""\s*:', line):
            line = ";/* PORT: MIPS asm removed */"
        out.append(line)
    return "\n".join(out)


def make_includes(outdir):
    """OUTDIR/inc: include/ with upper-case aliases (the game was built on DOS); OUTDIR/inc24: the same with the
    `// HEADERS tomba24` set overlaid onto tomba/ (as tools/ncheck.py does).  Symlinks only."""
    def link(src, dst):
        if os.path.lexists(dst): os.remove(dst)
        os.symlink(src, dst)
    for name, overlay in (("inc", None), ("inc24", "tomba24")):
        d = os.path.join(outdir, name)
        os.makedirs(os.path.join(d, "tomba"), exist_ok=True)
        inc = os.path.join(ROOT, "include")
        for f in os.listdir(inc):
            s = os.path.join(inc, f)
            if f == "tomba": continue
            for nm in ({f} if os.path.isdir(s) else {f, f.upper(), f.lower()}): link(s, os.path.join(d, nm))
        for f in os.listdir(os.path.join(inc, "tomba")):
            link(os.path.join(inc, "tomba", f), os.path.join(d, "tomba", f))
        if overlay:
            ov = os.path.join(inc, overlay)
            for f in os.listdir(ov):
                link(os.path.join(ov, f), os.path.join(d, "tomba", f))


def provides(prog):
    """{name: address} of the PROVIDE lines tools/build_full.py wrote for one program."""
    p = os.path.join(ROOT, "build", "full", prog.lower() + ".full.ld")
    if not os.path.exists(p): sys.exit("gen_port: %s missing: run python3 tools/build_full.py first" % p)
    return {m.group(1): int(m.group(2), 16) for m in re.finditer(r"PROVIDE\((\w+) = 0x([0-9A-Fa-f]+)\);", open(p).read())}


def object_refs(stems):
    """{stem: {name: address}} for the undefined names that tools/build_full.py renamed to name__ADDR in a file's
    linked object (the same name means different addresses in different files)."""
    objs = [os.path.join(ROOT, "build", "full", "obj", s + ".link.o") for s in stems]
    objs = [o for o in objs if os.path.exists(o)]
    out = {}
    for i in range(0, len(objs), 400):
        r = subprocess.run(["mipsel-linux-gnu-nm", "-A"] + objs[i:i + 400], capture_output=True, text=True).stdout
        for l in r.split("\n"):
            parts = l.split()
            if len(parts) == 3 and parts[1] == "U":
                m = re.match(r"(\w+?)__([0-9A-F]{8})$", parts[2])
                if m:
                    stem = os.path.basename(parts[0].rstrip(":"))[:-len(".link.o")]
                    out.setdefault(stem, {})[m.group(1)] = int(m.group(2), 16)
    return out


def load_fixes():
    out = {}
    for l in open(os.path.join(ROOT, "port", "fixes.txt")):
        if not l.strip() or l.lstrip().startswith("#"): continue
        f, old, new = [x.strip() for x in l.rstrip("\n").split(" | ")]
        out.setdefault(f, []).append((old, new))
    return out


FIXES = {}
ASM_ALIASES = {}


OVL_BASE, OVL_END = 0x800E8028, 0x80140000


def area_syms(prog):
    """{name: address} of an area overlay X0nn, as tools/ncheck.py resolves the names of src/x0nn/*.c: MAIN0's
    symbol files plus the overlay's own (config/symbol_addrs_x0nn.txt, notes/names_x0nn.csv, the names of
    notes/functions_x0nn.csv and the functions src/x0nn/ defines)."""
    syms, names = {}, {}
    for f in ("main0", prog.lower()):
        p = os.path.join(ROOT, "config", "symbol_addrs_%s.txt" % f)
        for l in (open(p) if os.path.exists(p) else ()):
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9a-fA-F]+)", l)
            if m: syms[m.group(1)] = int(m.group(2), 16)
        for csvf in ("names_%s.csv" % f,) + (("functions_%s.csv" % f,) if f != "main0" else ()):
            p = os.path.join(ROOT, "notes", csvf)
            for l in (open(p) if os.path.exists(p) else ()):
                m = re.match(r"([0-9a-fA-F]{8}),(?:\d+,)?(\w+)", l)
                if m:
                    a = int(m.group(1), 16)
                    if names.setdefault(m.group(2), a) != a: names[m.group(2)] = None
    d = os.path.join(ROOT, "src", prog.lower())
    for f in (sorted(os.listdir(d)) if os.path.isdir(d) else ()):
        if not f.endswith(".c"): continue
        m = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)", open(os.path.join(d, f), errors="replace").read(300))
        if m: syms[f[:-2]] = int(m.group(1), 16)
    for k, a in names.items():
        if a is not None: syms.setdefault(k, a)
    return syms


def area_signatures(progs):
    """{prog: [(addr, word)] * 2}: two words of the area's own code that no other listed overlay has at that address,
    so the runtime can tell which area file is in RAM (all of them share X000's header word)."""
    import units as UN
    d = {p: open(os.path.join(ROOT, UN.BINS[p][0]), "rb").read() for p in progs}
    out = {}
    for p in progs:
        got = []
        for o in range(0x80115EA4 - OVL_BASE, len(d[p]) - 3, 4):
            w = d[p][o:o + 4]
            if w != b"\0\0\0\0" and all(d[q][o:o + 4] != w for q in progs if q != p):
                got.append((OVL_BASE + o, int.from_bytes(w, "little")))
                if len(got) == 2: break
        if len(got) < 2: sys.exit("gen_port: no signature words for " + p)
        out[p] = got
    return out


def area_plan(U, areas, ovls=()):
    """Which code of each area overlay the port compiles, and the dispatch table per area.
    A function of X000 is *shared* with area P when P's file has the same bytes over its whole range (same address,
    so the same jal targets and data addresses): P then runs X000's native code.  Everything else of P that has C
    is compiled with the prefix P_.  X000 functions that some area does not share are `varying`: their definition
    is renamed X000_<name> and every call by name goes through a dispatch thunk (port_resolve_call), which picks
    the native code of the area resident in RAM.
    Returns (vary {addr: X000 name}, compiled [(prog, unit)], areamap [(prog, addr, native name)])."""
    import units as UN
    blob = {}
    def data(p):
        if p not in blob:
            fn = os.path.join(ROOT, UN.BINS[p][0])
            blob[p] = open(fn, "rb").read() if os.path.exists(fn) else None
        return blob[p]
    def same(p, a, n):
        x, y = data("X000"), data(p)
        o = a - OVL_BASE
        return x is not None and y is not None and o + n <= len(y) and x[o:o + n] == y[o:o + n]
    x0 = {u.addr: u for u in U["X000"] if not u.lib}
    vary = {}
    for a, u in x0.items():
        if any(not same(p, a, u.size) for p in areas): vary[a] = u.name
    # a SYSTEM overlay function starting at the same address: MAIN0 calls it by the X000 name while that overlay is
    # resident (drawCategory7Objects -> 0x800EC710 is I0NFO2's own function in AREA05/AREA08 section 0), so those
    # calls must dispatch too
    ovl_starts = set(u.addr for p in ovls for u in U[p])
    for a, u in x0.items():
        if a in ovl_starts: vary[a] = u.name
    compiled, amap = [], []
    for a, u in x0.items():
        if a in vary: amap.append(("X000", a, None))
    for p in areas:
        if data(p) is None: sys.exit("gen_port: %s missing (game/%s)" % (p, UN.BINS[p][0]))
        for u in U[p]:
            if u.lib or not (OVL_BASE <= u.addr < OVL_END): continue
            xu = x0.get(u.addr)
            if xu and xu.size == u.size and same(p, u.addr, u.size):
                if u.addr in vary: amap.append((p, u.addr, None))      # X000's native code (renamed X000_)
                continue
            if u.file and u.file.endswith(".c"):
                # the name the file defines (func_801167A0.c defines area6ScriptOp): X0nn_<unit name> would be the
                # thunk itself, and the dispatch would loop forever
                nm = UN.defined_name(u.file, os.path.basename(u.file)[:-2])
                compiled.append((p, u)); amap.append((p, u.addr, p + "_" + nm))
    return vary, compiled, amap



def sources(outdir, progs, ovls=(), areas=()):
    FIXES.update(load_fixes())
    from units import units, defined_name
    U = units()
    files, files24, fmap = [], [], []
    make_includes(outdir)
    seen_names = {}
    picked = []
    for prog in progs:
        for u in U[prog]:
            if not u.file or not u.file.endswith(".c"): continue
            stem = os.path.basename(u.file)[:-2]
            if u.lib and stem not in LIB_KEEP and u.name not in LIB_KEEP: continue
            if GAME_SKIP_RE.match(stem): continue
            name = defined_name(u.file, stem)
            if name in seen_names:     # same function name in two programs (twin copies): keep the first
                continue
            seen_names[name] = u.addr
            picked.append((prog, u, stem, name))
    # overlay set: SYSTEM overlays (tools/sysovl.py) loaded at the same address as X000.  Every function such a
    # file defines (or that the overlay has as asm only) is renamed <PROG>_<name>, and the runtime dispatches
    # calls into the overlay region to that table while the overlay is resident (its header id at 0x800E8028).
    ovl_names = {}
    for prog in ovls:
        names = set()
        for u in U[prog]:
            names.add(u.name)
            if u.file and u.file.endswith(".c"):
                stem = os.path.basename(u.file)[:-2]
                names.add(defined_name(u.file, stem))
                picked.append((prog, u, stem, defined_name(u.file, stem)))
        ovl_names[prog] = names
    # area overlays X001..X019: their own code (not shared with X000) under the prefix X0nn_ (area_plan)
    vary, area_units, areamap = area_plan(U, areas, ovls) if areas else ({}, [], [])
    area_own = {}                      # prog -> {addr: native name}
    for prog, u in area_units:
        stem = os.path.basename(u.file)[:-2]
        area_own.setdefault(prog, {})[u.addr] = prog + "_" + defined_name(u.file, stem)
        picked.append((prog, u, stem, defined_name(u.file, stem)))
    asyms = {p: area_syms(p) for p in areas}
    x000_native = {}                   # varying X000 functions: addr -> X000_<name>
    refs = object_refs([(p[0].lower() + "__" + p[2]) if p[0] in ovl_names else p[2] for p in picked
                        if p[0] not in asyms])
    allp = list(progs) + [p for p in ovls if p not in progs]
    prov = {p: provides(p) for p in allp}
    canon = {}                         # name -> address everybody agrees on (MAIN0 first)
    for p in allp:
        for n, a in prov[p].items(): canon.setdefault(n, a)
    ovlmap = []
    out_texts, area_texts = {}, {}
    renamed = {}
    for prog, u, stem, name in picked:
        dst = os.path.join(outdir, "src", prog.lower(), stem + ".c")
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        dropped = []
        src = open(u.file, errors="replace").read()
        txt = filter_src(src, dropped, canon)
        check_retail_rewrites(txt, u.file)
        for old, new in FIXES.get(os.path.relpath(u.file, ROOT), []):
            if old not in txt: sys.exit("gen_port: port/fixes.txt: %r not found in %s" % (old, u.file))
            txt = txt.replace(old, new)
        if name in dropped:
            print("gen_port: %s is MIPS asm only: needs a port shim" % name)
        # names that mean another address in this file than in the rest of the game: rename them in this file
        ovl = prog in ovl_names
        area = prog in asyms
        defs = ""
        if area:
            ren = {}
            local = set(re.findall(r"^static\b[^;=(]*?\b(\w+)\s*\(", src, re.M))   # file-local helpers
            for n in set(re.findall(r"\b[A-Za-z_]\w*\b", src)):
                a = asyms[prog].get(n)
                if a is None or n in local: continue
                if a in area_own[prog]: defs += "#define %s %s\n" % (n, area_own[prog][a])
                elif canon.get(n) != a: ren[n] = a
        else:
            ren = dict(refs.get((prog.lower() + "__" + stem) if ovl else stem, {}))
            for n, a in prov[prog].items():
                if canon.get(n) != a and re.search(r"\b%s\b" % re.escape(n), src): ren.setdefault(n, a)
        if ovl:
            for n in ovl_names[prog]: ren.pop(n, None)
        defs += "".join("#define %s %s__%08X\n" % (n, n, a) for n, a in sorted(ren.items()))
        for n, a in ren.items(): renamed["%s__%08X" % (n, a)] = a
        if prog == "X000" and u.addr in vary:      # varying: callers go through the dispatch thunk of `name`
            x000_native[u.addr] = "X000_" + name
            defs += "#define %s X000_%s\n" % (name, name)
        if ovl:
            defs += "".join("#define %s %s_%s\n" % (n, prog, n) for n in sorted(ovl_names[prog])
                            if re.search(r"\b%s\b" % re.escape(n), src))
        txt = volatile_aliases(txt)
        txt = '#include "port_prelude.h"\n%s#line 1 "%s"\n' % (defs, os.path.relpath(u.file, ROOT)) + txt
        hm = re.search(r"^//\s*HEADERS\s+(\S+)", src[:600], re.M)
        if hm and hm.group(1) != "tomba24": sys.exit("gen_port: unknown header set " + hm.group(1))
        (files24 if hm else files).append(dst)
        if ovl:
            if name not in dropped: ovlmap.append((prog, u.addr, prog + "_" + name))
        elif area or u.addr in x000_native: pass
        elif name not in dropped: fmap.append((u.addr, name))
        if area: area_texts.setdefault(prog, {})[dst] = txt
        else: out_texts[dst] = txt
    # arity of the area functions by address: a call from shared code (MAIN0, X000) to an address whose code differs
    # per area forwards as many arguments as the widest definition there takes
    arity_addr = {}
    for prog, t in area_texts.items():
        for n, k in _global_defs(t).items():
            a = asyms[prog].get(n)
            if a is not None and k > 0: arity_addr[a] = max(arity_addr.get(a, 0), k)
    # ... and of every MAIN0/X000 definition by address, for calls through another name of the same function
    # (FUN_800efc04() -> ObjSetAnimFromTable(o))
    for prog in ovls:                               # SYSTEM overlays (their own names, by address)
        addr_of_name = {}
        for u in U[prog]:
            addr_of_name[u.name] = u.addr
            if u.file and u.file.endswith(".c"): addr_of_name[defined_name(u.file, os.path.basename(u.file)[:-2])] = u.addr
        texts = {f: t for f, t in out_texts.items() if "/src/%s/" % prog.lower() in f}
        for n, k in _global_defs(texts).items():
            a = addr_of_name.get(n)
            if a is not None and k > 0: arity_addr[a] = max(arity_addr.get(a, 0), k)
    base = {f: t for f, t in out_texts.items() if re.search(r"/src/(main0|x000)/", f)}
    for n, k in _global_defs(base).items():
        a = canon.get(n)
        if a is not None and k > 0: arity_addr[a] = max(arity_addr.get(a, 0), k)
    def fallback_for(table):
        def fb(n):
            a = table.get(n)
            if a is None:
                m = re.match(r"(?:func|FUN)_([0-9A-Fa-f]{8})$", n)
                a = int(m.group(1), 16) if m else None
            return arity_addr.get(a, 0)
        return fb
    nfwd, gdefs = forward_short_calls(out_texts, None, fallback_for(canon))
    for prog, t in area_texts.items():              # each area resolves names among its own definitions first
        nfwd += forward_short_calls(t, gdefs, fallback_for(asyms[prog]))[0]
        out_texts.update(t)
    for dst, txt in out_texts.items():
        if not os.path.exists(dst) or open(dst, errors="replace").read() != txt:
            open(dst, "w").write(txt)
    print("gen_port: %d short calls get the caller's incoming arguments (PORT_ARG)" % nfwd)
    with open(os.path.join(outdir, "sources.cmake"), "w") as f:
        f.write("set(PORT_GAME_SOURCES\n%s)\n" % "".join("  %s\n" % p for p in files))
        f.write("set(PORT_GAME_SOURCES_T24\n%s)\n" % "".join("  %s\n" % p for p in files24))
    with open(os.path.join(outdir, "funcmap.c"), "w") as f:
        f.write("/* generated by port/tools/gen_port.py */\n#include \"port_runtime.h\"\n")
        for a, n in fmap: f.write("extern void %s(void);\n" % n)
        f.write("const PortFunc port_funcmap[] = {\n")
        for a, n in sorted(fmap): f.write("  {0x%08X, (void *)%s, \"%s\"},\n" % (a, n, n))
        f.write("};\nconst int port_funcmap_count = %d;\n" % len(fmap))
        # overlay set: header id (first word of the file, at 0x800E8028 while resident) and the functions
        import sysovl
        ids = {}
        for prog in ovls:
            fn = os.path.join(ROOT, "game", sysovl.SYSOVL[prog][0])
            ids[prog] = int.from_bytes(open(fn, "rb").read(4), "little") if os.path.exists(fn) else 0
        hand = handwritten_ovl_funcs()
        for prog in ovls:                  # asm-only overlay functions translated by hand in port/runtime
            for u in U[prog]:
                n = prog + "_" + u.name
                if n in hand and not any(e[2] == n for e in ovlmap): ovlmap.append((prog, u.addr, n))
        for _, _, n in ovlmap: f.write("extern void %s(void);\n" % n)
        f.write("const PortOvlFunc port_ovlmap[] = {\n")
        for prog, a, n in sorted(ovlmap, key=lambda e: (ids[e[0]], e[1])):
            f.write("  {0x%02X, 0x%08X, (void *)%s, \"%s\"},\n" % (ids[prog], a, n, n))
        f.write("  {0, 0, 0, 0}\n};\nconst int port_ovlmap_count = %d;\n" % len(ovlmap))
        f.write("const PortOvl port_ovls[] = {\n%s  {0, 0}\n};\n" % "".join(
            "  {0x%02X, \"%s\"},\n" % (ids[p], p) for p in ovls))
        # area overlays: the area resident in RAM is recognised by signature words (area_signatures); per area
        # the native code of every varying address (port_lookup)
        alist = ["X000"] + list(areas) if areas else []
        ents = sorted(set((alist.index(p), a, n or x000_native.get(a)) for p, a, n in areamap
                          if (n or x000_native.get(a))))
        for n in sorted(set(e[2] for e in ents)): f.write("extern void %s(void);\n" % n)
        f.write("const PortAreaFunc port_areamap[] = {\n%s  {0, 0, 0}\n};\nconst int port_areamap_count = %d;\n" % (
            "".join("  {%d, 0x%08X, (void *)%s},\n" % e for e in ents), len(ents)))
        sigs = area_signatures(alist) if alist else {}
        f.write("const PortArea port_areas[] = {\n%s  {0, 0, {{0, 0}}}\n};\nconst int port_areas_count = %d;\n" % (
            "".join("  {%d, \"%s\", {%s}},\n" % (i, p, ", ".join("{0x%08X, 0x%08X}" % w for w in sigs[p]))
                    for i, p in enumerate(alist)), len(alist)))
        va = sorted(vary) + sorted(set(a for p, a, n in areamap))
        f.write("const unsigned int port_vary_addrs[] = {\n%s  0};\nconst int port_vary_count = %d;\n" % (
            "".join("  0x%08X,\n" % a for a in sorted(set(va))), len(set(va))))
    with open(os.path.join(outdir, "vary.txt"), "w") as f:
        for a in sorted(set(vary) | set(a for p, a, n in areamap)): f.write("%08X\n" % a)
    # symbol -> address table for the link step
    with open(os.path.join(outdir, "symaddr.txt"), "w") as f:
        for n, a in sorted(canon.items()): f.write("%s %08X\n" % (n, a))
        for n, a in sorted(renamed.items()): f.write("%s %08X\n" % (n, a))
        for prog in ovls:
            for u in U[prog]: f.write("%s_%s %08X\n" % (prog, u.name, u.addr))
        for prog in areas:
            for u in U[prog]: f.write("%s_%s %08X\n" % (prog, u.name, u.addr))
    with open(os.path.join(outdir, "asm_aliases.txt"), "w") as f:
        for n, e in sorted(ASM_ALIASES.items()): f.write("%s %s\n" % (n, e))
    with open(os.path.join(outdir, "unitnames.txt"), "w") as f:
        for prog in progs:
            for u in U[prog]: f.write("%08X %s\n" % (u.addr, u.name))
    with open(os.path.join(outdir, "libaddrs.txt"), "w") as f:      # Psy-Q code (hardware access): never interpreted
        for prog in progs:
            for u in U[prog]:
                if u.lib: f.write("%08X\n" % u.addr)
    with open(os.path.join(outdir, "ovl_funcnames.txt"), "w") as f:
        for prog in list(ovls) + list(areas):
            for u in U[prog]: f.write("%s\n" % u.name)
    code = []
    for prog in ("MAIN0", "X000"):
        for u in U[prog]: code.append((u.addr, u.addr + u.size))
    with open(os.path.join(outdir, "code_ranges.txt"), "w") as f:
        for a, e in sorted(code): f.write("%08X %08X\n" % (a, e))
    print("gen_port: %d sources (%s), %d mapped functions, %d overlay-set functions, %d per-file renames"
          % (len(files) + len(files24), " ".join(list(progs) + list(ovls) + list(areas)), len(fmap), len(ovlmap), len(renamed)))
    if areas:
        print("gen_port: areas: %d varying X000 functions, %d area functions compiled, %d area dispatch entries"
              % (len(vary), len(area_units), len(areamap)))


def volatile_aliases(txt):
    """One address under two data names in a file (D_1F80019E and its typed alias D_1F80019Es): gcc assumes distinct
    globals never alias, so a loop that decrements one and tests the other keeps the test in a register forever
    (playerTryTalkToNpc).  Every extern declaration of such names becomes volatile (both names, so the accesses keep
    their order)."""
    groups = {}
    for full, addr in re.findall(r"\b((?:D|DAT)_([0-9A-Fa-f]{8})\w*)\b", txt):
        if "__" in full: continue
        groups.setdefault(addr.upper(), set()).add(full)
    names = set(n for g in groups.values() if len(g) > 1 for n in g)
    if not names: return txt
    def fix(m):
        decl = m.group(0)
        hit = [n for n in names if re.search(r"\b%s\b" % re.escape(n), decl)]
        if not hit: return decl
        if "," in decl:     # several declarators: one declaration each (`extern T *a, b[];` -> `extern T *a; extern T b[];`)
            parts = decl.rstrip().rstrip(";").split(",")
            m0 = re.search(r"(\**\s*)\b\w+\s*(?:\[[^\]]*\]\s*)*$", parts[0])
            if not m0: return decl
            spec = parts[0][:m0.start()]
            return " ".join(fix1(spec.rstrip() + " " + d.strip() + ";", hit) for d in [parts[0][m0.start():]] + parts[1:])
        return fix1(decl, hit)
    def fix1(decl, hit):
        for n in hit:
            decl = re.sub(r"(?<![\w*])(\*?\s*)\b(%s)\b(?=\s*(?:\[[^\]]*\])*\s*[;,=])" % re.escape(n),
                          lambda k: k.group(1) + "volatile " + k.group(2), decl)
        return decl
    return re.sub(r"^[ \t]*extern\b[^;{(]*;", fix, txt, flags=re.M)


def _blank(s):
    """comments and string/char literals replaced by spaces (positions kept)"""
    sp = lambda m: re.sub(r"[^\n]", " ", m.group(0))
    s = re.sub(r"/\*.*?\*/", sp, s, flags=re.S)
    s = re.sub(r"//[^\n]*", sp, s)
    s = re.sub(r'"(?:[^"\\\n]|\\.)*"', lambda m: '"' + " " * (len(m.group(0)) - 2) + '"', s)
    return re.sub(r"'(?:[^'\\\n]|\\.)*'", lambda m: "'" + " " * (len(m.group(0)) - 2) + "'", s)


def _nparams(p):
    p = p.strip()
    if p in ("", "void"): return 0
    if "..." in p: return -1
    depth, n = 0, 1
    for c in p:
        if c == "(": depth += 1
        elif c == ")": depth -= 1
        elif c == "," and depth == 0: n += 1
    return n


_DECL_PRE = re.compile(r"\s*(?:extern\s+|static\s+)?(?:const\s+|volatile\s+)*[A-Za-z_]\w*(?:\s+\w+)*[\s\*]+$")
_KEYWORDS = ("return", "else", "case", "do", "goto")


def _global_defs(texts):
    out = {}
    for f, t in texts.items():
        for m in re.finditer(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{)]*(?:\([^)]*\)[^;{)]*)*)\)\s*\{", _blank(t), re.M):
            if m.group(1) in ("if", "while", "for", "switch", "return") or m.group(0).lstrip().startswith("static"): continue
            out.setdefault(m.group(1), _nparams(m.group(2)))
    return out


def forward_short_calls(texts, base_defs=None, fallback=None):
    """Register-carried arguments (see port/tools/arity_check.py): a call that passes fewer arguments than the
    callee's definition takes relies on $a0-$a3 still holding a value, most often the caller's own incoming
    argument.  Each missing argument i becomes PORT_ARG(i), the caller's incoming stack argument i (on i386 the
    slot exists even when the caller declares fewer parameters: it is what *its* caller pushed, so pass-through
    chains of dispatchers keep working).  The file's prototypes of the callee become unprototyped `f()`."""
    blank = {f: _blank(t) for f, t in texts.items()}
    gdefs, ldefs = {}, {}
    own = {}
    for f, t in blank.items():
        ldefs[f] = {}
        for m in re.finditer(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{)]*(?:\([^)]*\)[^;{)]*)*)\)\s*\{", t, re.M):
            if m.group(1) in ("if", "while", "for", "switch", "return"): continue
            ldefs[f].setdefault(m.group(1), _nparams(m.group(2)))
            if not m.group(0).lstrip().startswith("static"): own.setdefault(m.group(1), _nparams(m.group(2)))
    gdefs = dict(base_defs or {}); gdefs.update(own)
    total = 0
    for f, t in blank.items():
        defs = dict(gdefs); defs.update(ldefs[f])
        edits, protos = [], set()
        macros = set(re.findall(r"^\s*#\s*define\s+(\w+)", t[max(0, t.find("#line 1 ")):], re.M))
        # the port's renames: a per-file address rename or a SYSTEM overlay prefix hides the definition the call
        # means; an area prefix (X0nn_) does not (texts of one area are forwarded together)
        macros |= set(m.group(1) for m in re.finditer(r"^#define (\w+) (\w+)$", t[:max(0, t.find("#line 1 "))], re.M)
                      if not re.match(r"X0\d\d_", m.group(2)))
        # per-file address renames (`#define fa fa__8013839C`): the arity of what that address is
        by_addr = dict((m.group(1), int(m.group(2), 16)) for m in
                       re.finditer(r"^#define (\w+) \1__([0-9A-F]{8})$", t[:max(0, t.find("#line 1 "))], re.M))
        for m in re.finditer(r"(?<![\w.>])(\w+)\s*\(", t):
            n = m.group(1)
            if fallback and n not in ldefs[f] and n not in macros:
                fb = fallback(n)
                if fb > defs.get(n, 0): defs[n] = fb
            if fallback and n in by_addr and n not in ldefs[f]:
                fb = fallback("func_%08X" % by_addr[n])
                if fb > 0:
                    if defs.get(n, 0) <= 0 or n in macros: defs[n] = 0
                    defs[n] = max(defs[n], fb)
                    macros.discard(n)
            if defs.get(n, 0) <= 0 or n in macros: continue
            ls = t.rfind("\n", 0, m.start()) + 1
            pre = t[ls:m.start()]
            if (_DECL_PRE.match(pre) and pre.split()[0] not in _KEYWORDS) or re.match(r"\s*(extern|typedef)\b", pre):
                le = t.find("\n", m.start())
                if t[m.start():le if le >= 0 else len(t)].rstrip().endswith(";"): protos.add(n)
                continue
            i = m.end() - 1
            depth, k, empty, end = 0, 1, True, None
            for j in range(i, min(len(t), i + 4000)):
                c = t[j]
                if c == "(": depth += 1
                elif c == ")":
                    depth -= 1
                    if depth == 0: end = j; break
                elif c == "," and depth == 1: k += 1
                elif not c.isspace() and depth >= 1 and j > i: empty = False
            if end is None: continue
            k = 0 if empty else k
            if k >= defs[n]: continue
            edits.append((end, ("" if k == 0 else ", ") + ", ".join("PORT_ARG(%d)" % a for a in range(k, defs[n]))))
            protos.add(n)
        if not edits: continue
        total += len(edits)
        txt = texts[f]
        for pos, ins in sorted(edits, reverse=True):
            txt = txt[:pos] + ins + txt[pos:]
        for n in protos:
            # (not a statement: `else f(...);` / `return f(...);` would look like a declaration)
            txt = re.sub(r"(^[ \t]*(?!(?:else|return|case|do|goto)\b)(?:extern\s+)?[A-Za-z_][\w \t\*]*\b%s\s*)\([^;{()]*(?:\([^)]*\)[^;{()]*)*\)(\s*;)" % re.escape(n),
                         r"\1()\2", txt, flags=re.M)
            # declarations with several declarators: `extern void a(T *), n(T *);`
            txt = re.sub(r"^([ \t]*extern\b[^;={]*?[,\s\*]%s\s*)\([^;{()]*\)(?=[^;{]*;)" % re.escape(n), r"\1()", txt, flags=re.M)
        texts[f] = txt
    return total, gdefs


def handwritten_ovl_funcs():
    """Overlay functions the port defines by hand (port/runtime/ovl_*.c): <PROG>_<name>(...) definitions."""
    out = set()
    d = os.path.join(ROOT, "port", "runtime")
    for f in os.listdir(d):
        if f.startswith("ovl_") and f.endswith(".c"):
            out |= set(re.findall(r"^\w[\w \t\*]*?\b([A-Z0-9]+_\w+)\s*\([^;]*$", open(os.path.join(d, f)).read(), re.M))
    return out


def link(outdir, nm, objlist):
    import bisect
    syms = {}
    for l in open(os.path.join(outdir, "symaddr.txt")):
        n, a = l.split(); syms[n] = int(a, 16)
    code = [tuple(int(x, 16) for x in l.split()) for l in open(os.path.join(outdir, "code_ranges.txt"))]
    starts = [c[0] for c in code]

    def is_code(a):
        i = bisect.bisect_right(starts, a) - 1
        return i >= 0 and code[i][0] <= a < code[i][1]
    native_by_addr = {}
    for l in open(os.path.join(outdir, "funcmap.c")):
        m = re.match(r'\s*\{0x([0-9A-F]+), \(void \*\)(\w+)', l)
        if m: native_by_addr[int(m.group(1), 16)] = m.group(2)
    unitname = {}
    for l in open(os.path.join(outdir, "unitnames.txt")):
        a, n = l.split(); unitname.setdefault(int(a, 16), n)
    defined, undefined, data_defined, port_defined = set(), set(), set(), set()
    objs = [l.strip() for l in open(objlist) if l.strip()]
    r = ""
    for i in range(0, len(objs), 400):
        r += subprocess.run([nm, "-A"] + objs[i:i + 400], capture_output=True, text=True).stdout
    for l in r.split("\n"):
        parts = l.split()
        if len(parts) != 3: continue
        o, t, n = parts
        if t == "U":
            if "@" not in n and not n.startswith("__imp_"): undefined.add(n)   # Win32 imports (_Name@N) are the DLL's
            continue
        if t.isupper(): defined.add(n)
        if "/gen/src/" not in o:
            if t.isupper(): port_defined.add(n)
        elif t in "DBRCGS": data_defined.add(n)
    undefined -= defined
    ld, thunks = [], []
    funcnames = set(unitname.values()) | set(l.strip() for l in open(os.path.join(outdir, "ovl_funcnames.txt")))
    is_func_name = lambda n: n in funcnames or n.split("__")[0] in funcnames
    import sysovl
    ovl_re = re.compile(r"(?:%s|X0\d\d)_\w" % sysovl.PROG_RE)
    vp = os.path.join(outdir, "vary.txt")
    vary = set(int(l, 16) for l in open(vp) if l.strip()) if os.path.exists(vp) else set()
    data_name = lambda n: re.match(r"(?:D|DAT|PTR|PTR_DAT|jtbl)_", n)
    asm_alias = dict(l.rstrip("\n").split(" ", 1) for l in open(os.path.join(outdir, "asm_aliases.txt")) if l.strip())

    def addr_of(n):
        if n in syms: return syms[n]
        m = re.match(r"(?:func|FUN|D|DAT|jtbl|LAB|PTR|PTR_DAT)_([0-9A-Fa-f]{8})$", n)
        if not m:   # typed aliases of an address in the area C: D_1F8002DCx, DAT_1f800310p, D_8009C960A ...
            m = re.match(r"(?:D|DAT)_([0-9A-Fa-f]{8})[A-Za-z_]\w*$", n)
        return int(m.group(1), 16) if m else None
    for n in sorted(undefined):
        if n.startswith(("port_", "__", "_GLOBAL_OFFSET")): continue
        a = addr_of(n)
        if n in asm_alias:
            ld.append("%s = %s;" % (n, asm_alias[n])); continue
        if ovl_re.match(n):                # overlay-set function without native code: dispatch at call time
            thunks.append((n, a)); continue
        if a in vary and not data_name(n):  # a function that differs between the area overlays: dispatch
            thunks.append((n, a)); continue
        if a is not None and a in native_by_addr and native_by_addr[a] in defined:
            ld.append("%s = %s;" % (n, native_by_addr[a]))
        elif a is not None and unitname.get(a) in defined and unitname[a] != n:   # e.g. FUN_80063ffc -> SetGeomScreen shim
            ld.append("%s = %s;" % (n, unitname[a]))
        elif a is not None and (not is_code(a) or (0x800E8028 <= a < 0x80140000 and not is_func_name(n))) and \
                not (0x800E8028 <= a < 0x80140000 and n.startswith(("func_", "FUN_"))):
            # (inside the overlay region a name is data unless it names a function: the overlays share the region)
            ld.append("%s = 0x%08X;" % (n, a))
        else:
            thunks.append((n, a))
    # data a game C file defines but that lives in the retail image: use the retail copy
    twice = []
    for n in sorted(data_defined - port_defined):
        a = addr_of(n)
        # (inside the overlay region X000's code ranges say nothing about an area's data: D_80133B5C is a table
        # of X004 that X000 has code at)
        if a is not None and (not is_code(a) or (OVL_BASE <= a < OVL_END and data_name(n))) and \
                n not in native_by_addr.values():
            ld.append("%s = 0x%08X;" % (n, a)); twice.append(n)
    # Such a name is now defined twice: by the object and by syms.ld.  The ELF link binds every reference, the
    # defining object's own included, to the syms.ld address; a COFF object resolved its own references
    # section-relative at assembly time, so on Windows the defining file reads and writes its native copy while
    # the rest of the game uses the retail one (the queueLoadList bug of retail_definition).  retail_definition
    # rewrites the definitions it recognises; what is left here is a definition it did not (an asm("name")
    # label, a declaration over several lines, a header): list it so the Windows build can be checked.
    if twice:
        print("gen_port: %d retail-named data still defined by a game object (COFF binds the defining file's own "
              "references to its copy): %s" % (len(twice), " ".join(twice)))
    with open(os.path.join(outdir, "syms.ld"), "w") as f:
        f.write("/* generated by port/tools/gen_port.py: retail addresses and aliases */\n")
        f.write("\n".join(ld) + "\n")
    # code the port does not have natively: a weak thunk that asks the runtime at call time (function of the area
    # overlay loaded now, or a logging stub).  port/shims/*.c override them with strong definitions.
    with open(os.path.join(outdir, "thunks.S"), "w") as f:
        f.write("/* generated by port/tools/gen_port.py */\n.text\n")
        for i, (n, a) in enumerate(thunks):
            # .type is ELF only; a COFF weak symbol with a body in the same file is not a definition for the mingw
            # ld, so the Windows thunks are plain globals linked last (--allow-multiple-definition: shims win)
            f.write((".globl %s\n%s:\n" if "mingw" in nm else ".weak %s\n.type %s,@function\n%s:\n") % ((n, n) if "mingw" in nm else (n, n, n)))
            f.write("  pushl $.Lname%d\n  pushl $0x%08X\n  call port_resolve_call\n  addl $8, %%esp\n  jmp *%%eax\n" % (i, a or 0))
        # retail address -> native function for every unit the port defines by its own name (game C and shims):
        # the MIPS interpreter (runtime/mips_interp.c) calls these natively
        libmap = sorted((a, n) for a, n in unitname.items() if n in defined)
        f.write(".section .rodata\n.globl port_libmap\n.p2align 2\nport_libmap:\n")
        for i, (a, n) in enumerate(libmap): f.write("  .long 0x%08X, %s, .Llib%d\n" % (a, n, i))
        f.write(".globl port_libmap_count\nport_libmap_count: .long %d\n" % len(libmap))
        for i, (a, n) in enumerate(libmap): f.write('.Llib%d: .asciz "%s"\n' % (i, n))
        libs = sorted(int(l, 16) for l in open(os.path.join(outdir, "libaddrs.txt")) if l.strip())
        f.write(".globl port_libaddrs\nport_libaddrs:\n%s" % "".join("  .long 0x%08X\n" % a for a in libs))
        f.write(".globl port_libaddrs_count\nport_libaddrs_count: .long %d\n" % len(libs))
        for i, (n, a) in enumerate(thunks):
            f.write('.Lname%d: .asciz "%s"\n' % (i, n + (" [%s]" % unitname[a] if a in unitname and unitname[a] != n else "")))
    print("gen_port: %d retail addresses/aliases, %d call thunks" % (len(ld), len(thunks)))


if __name__ == "__main__":
    a = sys.argv[1:]
    if a and a[0] == "sources":
        a[1] = os.path.abspath(a[1]); os.makedirs(a[1], exist_ok=True)
        import sysovl
        ps = [p.upper() for p in a[2:]] or ["MAIN0", "X000"]
        isarea = lambda p: re.match(r"X0\d\d$", p) and p != "X000"
        sources(a[1], [p for p in ps if p not in sysovl.SYSOVL and not isarea(p)], [p for p in ps if p in sysovl.SYSOVL],
                [p for p in ps if isarea(p)])
    elif a and a[0] == "link":
        link(a[1], a[2], a[3])
    else:
        sys.exit(__doc__)
