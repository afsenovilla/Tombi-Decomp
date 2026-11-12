"""Build profiles of tools/psxemu: which executable and disc image to run and where the addresses the HLE needs are.

  Machine(build="prepre9")          python3 tools/psxemu/capture.py --build prepre9 boot OUT.png

Retail PAL MAIN0 ("pal", the default) keeps its hand-named symbol tables (notes/functions_main0.csv + names_main0.csv) and its
fixed addresses.  Every other build gets its Psy-Q library entry points (and the few game functions the HLE stubs) from a table of
addresses found by byte signature of the matched PAL functions (tools/psxemu/syms/<build>.csv, made by libsig.py), so the BIOS /
libgpu / libcd / libsnd / libpad HLE runs the same way on top of them.

Paths are relative to the repo root (the extraction folders of tools/disc_fetch.sh: game_<name>/fs + game_<name>/disc.bin) and can
be overridden with --exe / --disc.  Fields:
  exe, disc         executable and raw (2352) or cooked (2048) disc image
  syms              None = the PAL notes csv; else tools/psxemu/syms/<build>.csv
  pad_buf           PadInitDirect buffer, when it is a constant the HLE writes the pad to; None = taken from the game's own
                    PadInitDirect call (libpad) or BIOS PAD_init / InitPAD call (old libetc PadRead, PrePre 9)
  movie_task        address of the FMV player task the HLE ends at once (None = none / not located)
  movie_state       its "playing" byte, cleared when the task is skipped
  cd_irq            emulate CdReadyCallback reads (CdlReadN + CdGetSector from the data-ready callback; the demos' loader), off
                    for PAL, whose loader uses CdRead
  boot_sel          PAL only: 0x1F8003D8 / 0x1F8003DA words SCES_013.31 leaves (Machine(boot=...))
  notes             what is known to work
"""
import os
import sys

ROOT = getattr(sys, "_MEIPASS", None) or os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SYMS_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "syms")

# game functions (not library) the HLE / capture.py look up by name; libsig.py keeps these too
GAME_NAMES = {"exitTask", "titleSequenceTask", "moviePlayerTask", "joypadInit", "StartTask", "MemCardInit", "updateMainObjects"}


class Profile(dict):
    __getattr__ = dict.get

    def path(self, key):
        p = self.get(key)
        if p is None:
            return None
        return p if os.path.isabs(p) else os.path.join(ROOT, p)


PROFILES = {
    "pal": Profile(name="pal", title="Tombi! PAL (SCES-01331) MAIN0.EXE", exe="game/MAIN0.EXE", disc="game/disc/Tombi.bin", syms=None,
                   pad_buf=0x8009F7F0, movie_task=0x8001D6A4, movie_state=0x1F8001CC),
    "usa": Profile(name="usa", title="Tomba! USA (SCUS-94236)", exe="game_usa/fs/SCUS_942.36", disc="game_usa/disc.bin",
                   syms="usa", cd_irq=True, movie_state=0x1F8001CC),
    "jp": Profile(name="jp", title="Ore! Tomba JP (SLPS-01144)", exe="game_jp/SLPS_011.44", disc="game_jp/disc/tomba_jp.bin",
                  syms="jp", cd_irq=True, movie_state=0x1F8001CC),
    "beta": Profile(name="beta", title="Ore! Tomba beta 1998-02-03", exe="game_beta980203/fs/SLPS_011.44",
                    disc="game_beta980203/disc.bin", syms="beta", cd_irq=True, movie_state=0x1F8001CC),
    "jpdemo": Profile(name="jpdemo", title="Ore! Tomba JP demo (SLPM-80165)", exe="game_jp_demo/fs/SLPM_801.65",
                      disc="game_jp_demo/disc.bin", syms="jpdemo", cd_irq=True),
    "prepre9": Profile(name="prepre9", title="PlayStation Club PrePre Vol. 9 (PCPX-96093) TOMBA/TOMBA.EXE",
                       exe="game_prepre9/fs/TOMBA/TOMBA.EXE", disc="game_prepre9/disc.bin", syms="prepre9", cd_irq=True,
                       movie_task=0x8002060C, movie_state=0x1F8001CC),     # StartTask(1, 0x8002060C) after 0x1F8001CC = 1 (0x8001C800)
    "prepre8": Profile(name="prepre8", title="PlayStation Club PrePre Vol. 8 (PCPX-96084) TOMBA/TOMBA.EXE",
                       exe="game_prepre8/fs/TOMBA/TOMBA.EXE", disc="game_prepre8/disc.bin", syms="prepre8", cd_irq=True),
    "dengeki": Profile(name="dengeki", title="Dengeki PlayStation D5 (SLPM-80186) Tomba demo", exe="game_dengeki_d5/fs/TOMBA/TOMBA.EXE",
                       disc="game_dengeki_d5/disc.bin", syms="dengeki", cd_irq=True),
    "hyper": Profile(name="hyper", title="Hyper PlayStation Re-mix No.12 (SLPM-80167) Tomba demo", exe="game_hyper12/fs/TOMBA/TOMBA.EXE",
                     disc="game_hyper12/disc.bin", syms="hyper", cd_irq=True),
}


def get(build):
    if isinstance(build, Profile):
        return build
    try:
        return PROFILES[build or "pal"]
    except KeyError:
        raise SystemExit("unknown build %r (one of %s)" % (build, ", ".join(PROFILES)))


def load_syms(prof):
    """{name: [addresses]} of a build's signature table."""
    import csv
    p = os.path.join(SYMS_DIR, "%s.csv" % prof.syms)
    if not os.path.exists(p):
        raise SystemExit("no symbol table %s: run python3 tools/psxemu/libsig.py %s" % (p, prof.name))
    out = {}
    with open(p, newline="") as f:
        for r in csv.DictReader(l for l in f if not l.startswith("#")):
            out.setdefault(r["name"], []).append(int(r["address"], 16))
    out.update({k: list(v) for k, v in (prof.extra_syms or {}).items()})
    return out
