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


def filter_src(text, dropped=None):
    """Make matching-oriented C compile natively without changing what it does.  Names of functions whose body is
    top-level MIPS asm are appended to `dropped` (they need a C version in port/shims/)."""
    out, pend, toplevel = [], None, None
    for line in text.split("\n"):
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
        line = re.sub(r"^static\s+(?=[\w\s\*]*\b(?:D|DAT|PTR|PTR_DAT)_[0-9A-Fa-f]{8}\s*(?:\[[^\]]*\]\s*)*=)", "", line)
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
        line = re.sub(r'\s*(?:__asm__|\basm)\s*\(\s*"\$?(?:[atvsk]\d|zero|at|gp|sp|fp|ra|\d+)"\s*\)(?=\s*[;=,])', "", line)
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


def sources(outdir, progs, ovls=()):
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
    refs = object_refs([(p[0].lower() + "__" + p[2]) if p[0] in ovl_names else p[2] for p in picked])
    allp = list(progs) + [p for p in ovls if p not in progs]
    prov = {p: provides(p) for p in allp}
    canon = {}                         # name -> address everybody agrees on (MAIN0 first)
    for p in allp:
        for n, a in prov[p].items(): canon.setdefault(n, a)
    ovlmap = []
    renamed = {}
    for prog, u, stem, name in picked:
        dst = os.path.join(outdir, "src", prog.lower(), stem + ".c")
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        dropped = []
        src = open(u.file, errors="replace").read()
        txt = filter_src(src, dropped)
        for old, new in FIXES.get(os.path.relpath(u.file, ROOT), []):
            if old not in txt: sys.exit("gen_port: port/fixes.txt: %r not found in %s" % (old, u.file))
            txt = txt.replace(old, new)
        if name in dropped:
            print("gen_port: %s is MIPS asm only: needs a port shim" % name)
        # names that mean another address in this file than in the rest of the game: rename them in this file
        ovl = prog in ovl_names
        ren = dict(refs.get((prog.lower() + "__" + stem) if ovl else stem, {}))
        for n, a in prov[prog].items():
            if canon.get(n) != a and re.search(r"\b%s\b" % re.escape(n), src): ren.setdefault(n, a)
        if ovl:
            for n in ovl_names[prog]: ren.pop(n, None)
        defs = "".join("#define %s %s__%08X\n" % (n, n, a) for n, a in sorted(ren.items()))
        for n, a in ren.items(): renamed["%s__%08X" % (n, a)] = a
        if ovl:
            defs += "".join("#define %s %s_%s\n" % (n, prog, n) for n in sorted(ovl_names[prog])
                            if re.search(r"\b%s\b" % re.escape(n), src))
        txt = '#include "port_prelude.h"\n%s#line 1 "%s"\n' % (defs, os.path.relpath(u.file, ROOT)) + txt
        if not os.path.exists(dst) or open(dst, errors="replace").read() != txt:
            open(dst, "w").write(txt)
        hm = re.search(r"^//\s*HEADERS\s+(\S+)", src[:600], re.M)
        if hm and hm.group(1) != "tomba24": sys.exit("gen_port: unknown header set " + hm.group(1))
        (files24 if hm else files).append(dst)
        if ovl:
            if name not in dropped: ovlmap.append((prog, u.addr, prog + "_" + name))
        elif name not in dropped: fmap.append((u.addr, name))
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
    # symbol -> address table for the link step
    with open(os.path.join(outdir, "symaddr.txt"), "w") as f:
        for n, a in sorted(canon.items()): f.write("%s %08X\n" % (n, a))
        for n, a in sorted(renamed.items()): f.write("%s %08X\n" % (n, a))
        for prog in ovls:
            for u in U[prog]: f.write("%s_%s %08X\n" % (prog, u.name, u.addr))
    with open(os.path.join(outdir, "asm_aliases.txt"), "w") as f:
        for n, e in sorted(ASM_ALIASES.items()): f.write("%s %s\n" % (n, e))
    with open(os.path.join(outdir, "unitnames.txt"), "w") as f:
        for prog in progs:
            for u in U[prog]: f.write("%08X %s\n" % (u.addr, u.name))
    with open(os.path.join(outdir, "ovl_funcnames.txt"), "w") as f:
        for prog in ovls:
            for u in U[prog]: f.write("%s\n" % u.name)
    code = []
    for prog in ("MAIN0", "X000"):
        for u in U[prog]: code.append((u.addr, u.addr + u.size))
    with open(os.path.join(outdir, "code_ranges.txt"), "w") as f:
        for a, e in sorted(code): f.write("%08X %08X\n" % (a, e))
    print("gen_port: %d sources (%s), %d mapped functions, %d per-file renames"
          % (len(files) + len(files24), " ".join(progs), len(fmap), len(renamed)))


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
            undefined.add(n); continue
        if t.isupper(): defined.add(n)
        if "/gen/src/" not in o:
            if t.isupper(): port_defined.add(n)
        elif t in "DBRCGS": data_defined.add(n)
    undefined -= defined
    ld, thunks = [], []
    funcnames = set(unitname.values()) | set(l.strip() for l in open(os.path.join(outdir, "ovl_funcnames.txt")))
    is_func_name = lambda n: n in funcnames or n.split("__")[0] in funcnames
    import sysovl
    ovl_re = re.compile(r"(?:%s)_\w" % sysovl.PROG_RE)
    asm_alias = dict(l.rstrip("\n").split(" ", 1) for l in open(os.path.join(outdir, "asm_aliases.txt")) if l.strip())

    def addr_of(n):
        if n in syms: return syms[n]
        m = re.match(r"(?:func|FUN|D|DAT|jtbl|LAB|PTR|PTR_DAT)_([0-9A-Fa-f]{8})$", n)
        return int(m.group(1), 16) if m else None
    for n in sorted(undefined):
        if n.startswith(("port_", "__", "_GLOBAL_OFFSET")): continue
        a = addr_of(n)
        if n in asm_alias:
            ld.append("%s = %s;" % (n, asm_alias[n])); continue
        if ovl_re.match(n):                # overlay-set function without native code: dispatch at call time
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
    for n in sorted(data_defined - port_defined):
        a = addr_of(n)
        if a is not None and not is_code(a) and n not in native_by_addr.values():
            ld.append("%s = 0x%08X;" % (n, a))
    with open(os.path.join(outdir, "syms.ld"), "w") as f:
        f.write("/* generated by port/tools/gen_port.py: retail addresses and aliases */\n")
        f.write("\n".join(ld) + "\n")
    # code the port does not have natively: a weak thunk that asks the runtime at call time (function of the area
    # overlay loaded now, or a logging stub).  port/shims/*.c override them with strong definitions.
    with open(os.path.join(outdir, "thunks.S"), "w") as f:
        f.write("/* generated by port/tools/gen_port.py */\n.text\n")
        for i, (n, a) in enumerate(thunks):
            f.write(".weak %s\n.type %s,@function\n%s:\n  pushl $.Lname%d\n  pushl $0x%08X\n"
                    "  call port_resolve_call\n  addl $8, %%esp\n  jmp *%%eax\n" % (n, n, n, i, a or 0))
        f.write(".section .rodata\n")
        for i, (n, a) in enumerate(thunks):
            f.write('.Lname%d: .asciz "%s"\n' % (i, n + (" [%s]" % unitname[a] if a in unitname and unitname[a] != n else "")))
    print("gen_port: %d retail addresses/aliases, %d call thunks" % (len(ld), len(thunks)))


if __name__ == "__main__":
    a = sys.argv[1:]
    if a and a[0] == "sources":
        a[1] = os.path.abspath(a[1]); os.makedirs(a[1], exist_ok=True)
        import sysovl
        ps = [p.upper() for p in a[2:]] or ["MAIN0", "X000"]
        sources(a[1], [p for p in ps if p not in sysovl.SYSOVL], [p for p in ps if p in sysovl.SYSOVL])
    elif a and a[0] == "link":
        link(a[1], a[2], a[3])
    else:
        sys.exit(__doc__)
