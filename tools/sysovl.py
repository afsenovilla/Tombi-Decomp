"""The SYSTEM code overlays (title/options, sub-screens, game over, info/inventory): the single list shared by
tools/matchcheck.py, tools/ncheck.py, tools/areas.py, tools/units.py, tools/build_full.py and tools/build_report.py.

Every file is a raw code blob loaded at 0x800E8028 (the same buffer as the area overlays X0nn) by a
`LOAD_KIND_INS` (0xF0) record of SYS/LD0SYS.BIN (addr 0 = the overlay buffer; G?OVER's record says 0x80000000,
its code is linked for 0x800E8028 too).  Checked by the jal targets: with base 0x800E8028 they land on function
starts in every file.  Layout: `u32 id` (0x31..0x38), a table of pointers into the overlay (entry points MAIN0
calls through), the code, then data.  D?SPSUB2 has no pointer table (a table of strings comes first).
The files of the variant `0` (MAIN0.EXE, PAL; ids of MAIN0's file table in brackets) are tracked; the other eight
variants (D1SPSUB ... I8NFO3) are the same code with other text offsets, as X1nn..X8nn are for the areas.
Read from the extracted disc tree (game/disc/fs, see tools/area000.py); nothing of it is committed.

prog: (path under game/, retail sha1, first code address = end of the header table)
"""
BASE = 0x800E8028
SYSOVL = {
    "O0PTSUB0": ("disc/fs/SYSTEM/O0PTSUB0.BIN", "4ff1dade2f430ce60d8e56c339b54ed8a82f3f68", 0x800E8078),  # [28] title options
    "D0SPSUB": ("disc/fs/SYSTEM/D0SPSUB.BIN", "9f7e3497e4cfe9474c61a8ff2b9f65cb77e8382b", 0x800E87BC),    # [39] sub-screen
    "D0SPSUB2": ("disc/fs/SYSTEM/D0SPSUB2.BIN", "6da6b0721a263baefa9f070f5138fc8f080a961c", 0x800E82A4),  # [45] memory card
    "D0SPSUB3": ("disc/fs/SYSTEM/D0SPSUB3.BIN", "1ada79a63ac93e7aa484994dce5b98f677cb6bbc", 0x800E8714),  # [50] sub-screen 3
    "G0OVER": ("disc/fs/SYSTEM/G0OVER.BIN", "5d3110197fb899bf9048b20657107c5c1bd3c0e5", 0x800E8044),      # [220] game over
    "I0NFO": ("disc/fs/SYSTEM/I0NFO.BIN", "7f239284db200169d93a9e1b06f7873d9615b627", 0x800E8214),        # [78] info
    "I0NFO2": ("disc/fs/SYSTEM/I0NFO2.BIN", "43b1ce474711d299c0a4bb7c5add80e745b67f77", 0x800E8B28),      # [113] info 2
    "I0NFO3": ("disc/fs/SYSTEM/I0NFO3.BIN", "3c42a02e42dc295eebdd86b8e658f26d39418e42", 0x800E847C),      # [84] info 3
}
PROGS = list(SYSOVL)
PROG_RE = "|".join(sorted(PROGS, key=len, reverse=True))  # for `// FUNC addr size PROG` (longest first)
