#!/usr/bin/env python3
"""The game version the native port is generated for (docs/PORT_VERSIONS_PLAN.md stage U1).

`PORT_VERSION` (environment; port2/CMakeLists.txt passes its cache variable of the same name) picks the disc:
unset, `pal` or `sces01331` is the PAL reference (everything as before); `usa` / `scus94236` is the USA retail
(SCUS-94236); `jp` / `slps01144` the Japanese retail (SLPS-01144, the same layout as USA without GOVER).  port/tools/gen_port.py and port2/tools/gen2.py import this module first; for a version other than
PAL, install() makes the PAL-shaped tables of tools/units.py, tools/sysovl.py and tools/areas.py describe that
version's files, so the generator keeps the PAL program names (MAIN0, X000, X0nn, D0SPSUB, ...) that the runtime,
the fixes and the reports use:

  MAIN0 = the boot exe (SCUS_942.36), X000 = SYSTEM/X00.BIN, X0nn = SYSTEM/Xnn.BIN, the SYSTEM code overlays under
  their PAL names (D0SPSUB = SYSTEM/DSPSUB.BIN, ...);
  the units are those of tools/units.py units(<ver>) (one per version function, with the src file the version's
  full link builds it from), renamed to those programs;
  the symbol tables are build/<ver>/full (tools/build_full.py --version <ver>).
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import versions as V

VER = V.canon(os.environ.get("PORT_VERSION") or V.REFERENCE)
PAL = VER == V.REFERENCE
SUPPORTED = (V.REFERENCE, "scus94236", "slps01144")
if VER not in SUPPORTED:
    sys.exit("portver: PORT_VERSION=%s is not supported by the port yet (supported: %s)" % (VER, ", ".join(SUPPORTED)))
SERIAL = "SCES_01331" if PAL else V.VERSIONS[VER]["serial"]
MACRO = None if PAL else "VERSION_" + SERIAL          # -D for the game C (the version's #if branches)
FULL = os.path.join(ROOT, "build", "full") if PAL else os.path.join(ROOT, "build", VER, "full")

if PAL:
    GAME = os.path.join(ROOT, "game")
    MAIN_EXE = os.path.join(GAME, "MAIN0.EXE")
    X000_BIN = os.path.join(GAME, "AREA00", "X000.BIN")
    OVL_BASE = 0x800E8028
    AREA_OWN = 0x80115EA4             # first per-area function of X0nn (the code before it is X000's)
    BOOT = "EXE/MAIN0.EXE"
    BOOT_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc"
    HZ = 50
else:
    # per version: the boot exe, the overlay base and the X00 prefix every area overlay repeats, the SYSTEM code
    # overlays (tools/versions.py), the boot exe's sha1 (tools/extract_data.py VERSION_OF), where its Psy-Q part starts
    _VER = {"scus94236": ("SCUS_942.36", V.USA_OVL, V.USA_SHARED, V.USA_SYSOVL,
                          "81cbc79f0230aeb4252e058039f47ac95a777f5a", 0x8005B38C),
            "slps01144": ("SLPS_011.44", V.JP_OVL, V.JP_SHARED, V.JP_SYSOVL,
                          "07d577698ad31ec1a69b9d7b80a7c3f90b650888", 0x80059E4C)}[VER]
    GAME = os.path.join(ROOT, V.VERSIONS[VER]["game"])
    MAIN_EXE = os.path.join(GAME, _VER[0])
    X000_BIN = os.path.join(GAME, "SYSTEM", "X00.BIN")
    OVL_BASE = _VER[1]
    AREA_OWN = _VER[1] + _VER[2]
    SYSOVL = _VER[3]
    BOOT = _VER[0]
    BOOT_SHA1 = _VER[4]
    HZ = 60
OVL_END = 0x80140000                  # the load buffers above the overlay region (both versions)
MAIN_LOAD = 0x80010000
AREAS = (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)

# PAL program name (what the generator and the runtime call it) -> the version's program (tools/versions.py)
if PAL:
    PROG = {}
else:
    PROG = {"MAIN0": VER, "X000": "x00"}
    PROG.update(("X%03d" % n, "x%02d" % n) for n in AREAS)
    PROG.update((pal.upper(), usa) for usa, pal in SYSOVL.items())
VPROG = {v: k for k, v in PROG.items()}


def vprog(p):
    """the version's program name of a PAL program name (lower case, as build/<ver>/full names its files)"""
    return PROG.get(p.upper(), p.lower()) if not PAL else p.lower()


def rel_game(path):
    """path relative to ROOT/game (tools/sysovl.py SYSOVL paths are joined to game/)"""
    return os.path.relpath(path, os.path.join(ROOT, "game"))


def bins():
    """{PAL prog: (path relative to ROOT, load address)} of the version's files (tools/units.py BINS shape)"""
    if PAL: return None
    g = os.path.relpath(GAME, ROOT)
    out = {"MAIN0": (os.path.join(g, BOOT), MAIN_LOAD - 0x800),
           "X000": (os.path.join(g, "SYSTEM", "X00.BIN"), OVL_BASE)}
    for n in AREAS: out["X%03d" % n] = (os.path.join(g, "SYSTEM", "X%02d.BIN" % n), OVL_BASE)
    for usa, pal in SYSOVL.items(): out[pal.upper()] = (os.path.join(g, "SYSTEM", usa.upper() + ".BIN"), OVL_BASE)
    return out


_UNITS = None
# the Psy-Q libraries start here in the boot exe (FlushCache, the first BIOS call; USA 0x8005B38C, JP 0x80059E4C)
LIB_FROM = 0x8005B38C if PAL else _VER[5]


def version_units():
    """units(<ver>) in the PAL shape: keyed by PAL program name, Unit.prog the PAL name, Unit.lib from the PAL file
    the unit is built from (Psy-Q), or (a version-only file) by address inside the library part of the boot exe"""
    global _UNITS
    if _UNITS is not None: return _UNITS
    import units as UM
    pal = UM.units()
    plib, pname = {}, {}
    for p, us in pal.items():
        for u in us:
            if u.file: plib[u.file] = u.lib
            pname[(p.lower(), u.addr)] = u.name
    # the PAL function each version function pairs with (`PAL <module> 0x<addr>` of config/<ver>/symbol_addrs_*.txt)
    pair = {}
    for vp in VPROG:
        f = os.path.join(ROOT, "config", VER, "symbol_addrs_%s.txt" % vp)
        for l in (open(f) if os.path.exists(f) else ()):
            m = re.match(r"\s*\w+\s*=\s*0x([0-9A-Fa-f]+);.*\bPAL (\w+) 0x([0-9A-Fa-f]+)", l)
            if m: pair[(vp, int(m.group(1), 16))] = pname.get((m.group(2).lower(), int(m.group(3), 16)))
    out = {}
    # a `// VERSION <SERIAL> <prog> <addr> <size>` file for an address its symbol file does not list as a function
    # (USA 0x8002FA24, the message box of src/func_8002FA24_scus94236.c): the full link builds it all the same
    vl = UM.version_lines(SERIAL)
    for vp, us in UM.version_units(VER).items():
        p = VPROG.get(vp)
        if p is None: continue
        have = set(u.addr for u in us)
        us = sorted(us + [UM.Unit(vp, a, size, "func_%08X" % a, path, False, False, None)
                          for (q, a), (path, size) in vl.items() if q == vp and a not in have], key=lambda u: u.addr)
        lst = []
        for u in us:
            # Psy-Q: the PAL file is library code, or a version-only file in the library part of the boot exe
            lib = bool(plib.get(u.file)) or (p == "MAIN0" and u.addr >= LIB_FROM and u.file not in plib)
            nm = u.name
            # a library function keeps the PAL name the shims define (func_80073AF0_scus94236 = PAL's
            # SsSetAutoKeyOffMode)
            if p == "MAIN0" and u.addr >= LIB_FROM and pair.get((vp, u.addr)) and re.match(r"(?:func|FUN)_[0-9A-Fa-f]{8}", nm):
                nm = pair[(vp, u.addr)]
            lst.append(u._replace(prog=p, lib=lib, name=nm))
        out[p] = lst
    _UNITS = out
    return out


def install():
    """Point tools/units.py, tools/sysovl.py and tools/areas.py at the version's files (no-op for PAL)."""
    if PAL: return
    import units as UM, sysovl as SO
    version_units()          # (with the PAL tables still in place: the version's units pair with the PAL files)
    if not hasattr(UM, "_pal_units"):
        UM._pal_units = UM.units
        def vunits(ver=None):
            if ver is not None: return UM._pal_units(ver)
            return version_units()
        UM.units = vunits
    b = bins()
    UM.BINS.clear(); UM.BINS.update(b)
    UM._BLOBS.clear()
    for p in b:
        lo = OVL_BASE if p != "MAIN0" else 0x80010000
        UM.RANGES[p] = (lo, 0x80200000)
    SO.BASE = OVL_BASE
    # a PAL SYSTEM overlay the version does not have (JP: no GOVER) leaves the generator's tables
    for p in [p for p in SO.SYSOVL if p.lower() not in SYSOVL.values()]:
        del SO.SYSOVL[p]
    SO.PROGS[:] = list(SO.SYSOVL)
    SO.PROG_RE = "|".join(sorted(SO.PROGS, key=len, reverse=True))
    for usa, pal in SYSOVL.items():
        path = os.path.join(GAME, "SYSTEM", usa.upper() + ".BIN")
        us = version_units().get(pal.upper(), [])
        first = min((u.addr for u in us), default=OVL_BASE)
        SO.SYSOVL[pal.upper()] = (rel_game(path), "", first)
    try:
        import areas as AR
        AR.BASE = OVL_BASE
        for p, (path, base) in b.items():
            if p != "MAIN0": AR.OVERLAYS[p] = rel_game(os.path.join(ROOT, path))
        AR.FIRST.update((p, v[2]) for p, v in SO.SYSOVL.items())
        _orig = AR.prog_words
        def prog_words(prog, addr, size):
            if prog == "MAIN0":
                d = open(MAIN_EXE, "rb").read()
                o = 0x800 + addr - MAIN_LOAD
                return list(AR.words(d[o:o + size]))
            return _orig(prog, addr, size)
        AR.prog_words = prog_words
    except ImportError:
        pass


def header_h():
    """gen/port_version.h: the layout constants and retail addresses the runtime needs, per version"""
    syms = full_syms()
    def a(n, pal):
        if PAL: return pal
        if n not in syms:
            if 0x1F800000 <= pal < 0x1F800400: return pal      # the scratchpad: the same layout in every release
            sys.exit("portver: %s has no address in %s" % (n, FULL))
        return syms[n]
    # the retail data the runtime's debug features read or poke (runtime/debugmenu.c, the --poke NAME[+OFF] forms of
    # shims/psyq.c, the launchers' mods): GAME (its field offsets are the same in every release), GVARS before it, the
    # player object, the event flags, the debug-select flag (scratchpad)
    names = [("GAME", "PORT_A_GAME", 0x8009C960), ("GVARS", "PORT_A_GVARS", 0x8009C930),
             ("PLAYER", "PORT_A_PLAYER", 0x800A6038), ("SCRIPT_FLAGS", "PORT_A_SCRIPT_FLAGS", 0x8009CDA4),
             ("IS_DEBUG_MODE_ENABLED", "PORT_A_IS_DEBUG_MODE_ENABLED", 0x1F8001B4)]
    lines = ["/* generated by port/tools/portver.py: the game version this build runs (PORT_VERSION) */",
             "#ifndef PORT_VERSION_H", "#define PORT_VERSION_H",
             '#define PORT_VERSION_KEY "%s"' % VER, '#define PORT_VERSION_SERIAL "%s"' % SERIAL.replace("_", "-"),
             "#define PORT_VERSION_PAL %d" % PAL, "#define PORT_VERSION_USA %d" % (VER == "scus94236"),
             "#define PORT_VERSION_JP %d" % (VER == "slps01144"),
             '#define PORT_BOOT_FILE "%s"' % BOOT, '#define PORT_BOOT_SHA1 "%s"' % BOOT_SHA1,
             "#define PORT_DEFAULT_HZ %d" % HZ,
             "#define PORT_MAIN0_LOAD 0x%08Xu" % MAIN_LOAD, "#define PORT_OVL_BASE 0x%08Xu" % OVL_BASE,
             "#define PORT_OVL_END 0x%08Xu" % OVL_END, "#define PORT_AREA_OWN_CODE 0x%08Xu" % AREA_OWN]
    for n, macro, pal in names:
        lines.append("#define %s 0x%08Xu   /* %s */" % (macro, a(n, pal), n))
    lines.append("#endif")
    return "\n".join(lines) + "\n"


_FULL_SYMS = None


def full_syms():
    """{name: address} of every PROVIDE of the version's boot exe and X000 full link"""
    global _FULL_SYMS
    if _FULL_SYMS is None:
        _FULL_SYMS = {}
        for p in ("MAIN0", "X000"):
            f = os.path.join(FULL, vprog(p) + ".full.ld")
            if os.path.exists(f):
                for m in re.finditer(r"PROVIDE\((\w+) = 0x([0-9A-Fa-f]+)\);", open(f).read()):
                    _FULL_SYMS.setdefault(m.group(1), int(m.group(2), 16))
    return _FULL_SYMS


if __name__ == "__main__":
    print("version %s (%s), full link %s, overlay base 0x%08X" % (VER, SERIAL, FULL, OVL_BASE))
