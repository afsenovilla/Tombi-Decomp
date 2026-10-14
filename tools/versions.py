"""Non-PAL versions handled by tools/build_report.py --version, tools/jpdemo_syms.py and tools/jpdemo_reuse.py.

Per version: the config dir (config/<v>/), the game dir, the C dir (src/<v>/, files with a `// <TAG> <prog> <addr>
<size>` header), and per program: (decomp.dev category name, file name as passed to tools/xmatch.py, vram of
xmatch offset 0, first file offset that belongs to this program). The last field skips the X00 code that every
JP area overlay repeats at its start (it is counted once, in x00). See docs/JPDEMO.md and docs/JP.md.
"""

JP_OVL = 0x800E9AF8   # retail JP overlay base (jal targets on prologues, like the demo estimate)
JP_SHARED = 0x2B680   # every JP X<nn>.BIN starts with these bytes of X00.BIN; the area part follows

VERSIONS = {
    "jpdemo": {
        "game": "game_jpdemo", "tag": "JPDEMO",
        "progs": {"slpm80165": ("SLPM_801.65", "SLPM_801.65", 0x80010000, 0),
                  "x00": ("X00.BIN (SYSTEM)", "X00.BIN", 0x800E7698, 0)},
    },
    "jp": {
        "game": "game_jp", "tag": "JP",
        "progs": dict([("slps01144", ("SLPS_011.44", "SLPS_011.44", 0x80010000, 0)),
                       ("x00", ("X00.BIN (AREA00)", "X00.BIN", JP_OVL, 0))]
                      + [("x%02d" % n, ("X%02d.BIN (AREA%02d)" % (n, n), "X%02d.BIN" % n, JP_OVL, JP_SHARED))
                         for n in (1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 16, 17, 18, 19)]),
    },
}


def version_arg(argv, default="jpdemo"):
    """Value of --version in argv (default jpdemo, for the original demo-only command lines)."""
    return argv[argv.index("--version") + 1] if "--version" in argv else default
