"""The game versions this repo builds from ONE shared src/ tree (sotn-decomp / psx_tomba multi-version scheme).

Each version is keyed by its lower-case disc serial. The reference version (PAL, sces01331) keeps the historical
layout (config/<prog>.yaml, asm/<prog>/, build/); every other version has config/<serial>/ (splat configs,
symbol_addrs_<prog>.txt, reuse.txt), asm/<serial>/<prog>/ and build/<serial>/. The C of every version lives in src/:
the build defines VERSION_<SERIAL> (tools/ncheck.py EXTRA_CPP) and a file can carry `#if defined(VERSION_SLPS_01144)`
branches (written by tools/version_share.py). See docs/JP.md, docs/JPDEMO.md and the README.

Per version: the game dir, the macro serial, the old names accepted by --version, and per program: (decomp.dev
category name, file name as passed to tools/xmatch.py, vram of xmatch offset 0, first file offset that belongs to
this program). The last field skips the X00 code that every JP area overlay repeats at its start (it is counted
once, in x00).
"""

JP_OVL = 0x800E9AF8   # retail JP overlay base (jal targets on prologues, like the demo estimate)
JP_SHARED = 0x2B680   # every JP X<nn>.BIN starts with these bytes of X00.BIN; the area part follows

REFERENCE = "sces01331"
SERIALS = {"sces01331": "SCES_01331", "slps01144": "SLPS_01144", "slpm80165": "SLPM_80165"}
ALIASES = {"pal": "sces01331", "jp": "slps01144", "jpdemo": "slpm80165"}

VERSIONS = {
    "slpm80165": {
        "game": "game_jpdemo", "serial": "SLPM_80165",
        "progs": {"slpm80165": ("SLPM_801.65", "SLPM_801.65", 0x80010000, 0),
                  "x00": ("X00.BIN (SYSTEM)", "X00.BIN", 0x800E7698, 0)},
    },
    "slps01144": {
        "game": "game_jp", "serial": "SLPS_01144",
        "progs": dict([("slps01144", ("SLPS_011.44", "SLPS_011.44", 0x80010000, 0)),
                       ("x00", ("X00.BIN (AREA00)", "X00.BIN", JP_OVL, 0))]
                      + [("x%02d" % n, ("X%02d.BIN (AREA%02d)" % (n, n), "X%02d.BIN" % n, JP_OVL, JP_SHARED))
                         for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]),
    },
}

# PAL program whose src directory holds the shared C of a version program (main exe and X00 -> src/, xNN -> src/x0NN/)
def pal_prog(ver, prog):
    if prog in ("slps01144", "slpm80165"): return "main0"
    return "x0" + prog[1:]


def pal_src_dir(ver, prog):
    p = pal_prog(ver, prog)
    return "src" if p in ("main0", "x000") else "src/" + p


def canon(v):
    """Canonical version key (serial) for a --version value (serial, SLPS_01144 style, or old alias jp/jpdemo)."""
    v = v.lower().replace("_", "")
    v = ALIASES.get(v, v)
    if v != REFERENCE and v not in VERSIONS:
        raise SystemExit("unknown version %r (known: %s; aliases %s)" % (v, ", ".join([REFERENCE] + list(VERSIONS)),
                                                                          ", ".join(ALIASES)))
    return v


def version_arg(argv, default="slpm80165"):
    """Canonical value of --version in argv (default: the demo, for the original demo-only command lines)."""
    return canon(argv[argv.index("--version") + 1]) if "--version" in argv else default
