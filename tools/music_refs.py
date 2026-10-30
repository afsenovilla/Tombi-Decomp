#!/usr/bin/env python3
"""Which music (SEQ) and sample bank (WVD) every area section plays, read from the PAL MAIN0 tables.

  python3 tools/music_refs.py [MAIN0.EXE]        (default game/disc/fs/EXE/MAIN0.EXE)

Music is not in the SYS/LDnARxx lists. loadAreaSectionResources (0x8001D1BC) takes a sound-set number from
PTR_DAT_80077c4c[area + AREA_VARIANT][section] and loadSoundSet (0x8004FB68) runs SOUND_SET_LOAD_LISTS[set]
(0x8007D1B4, 20-byte load records inside MAIN0: kind 0x93/0x94 = WVD, 0xB0 = SEQ). openAreaBgm (0x8001EC80) then
plays whatever SEQ that set loaded. File names come from the MAIN0 file table comments in src/cdLoadTask.c.
Ends with the sound sets and SOUND files no table reaches. Trailing 00 entries of a row can be padding (AREA00/01/07/10/13). See docs/CIVILIZATION.md "Music".
"""
import os, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AREA_SETS, SETS, NSETS = 0x80077C4C, 0x8007D1B4, 0x34
EXTRA = (0x80077C9C, 0x80077CB0)   # DAT_80077c9c/ca4/cb0: extra effect banks for idx 1/7, 10, 12


def main():
    exe = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "game", "disc", "fs", "EXE", "MAIN0.EXE")
    d = open(exe, "rb").read()
    off = lambda va: va - 0x80010000 + 0x800
    word = lambda va: struct.unpack_from("<I", d, off(va))[0]
    nm = {int(m.group(1)): m.group(2).replace("SOUND/", "")
          for m in re.finditer(r"/\*\s*(\d+)\s+(\S+)\s*\*/", open(os.path.join(ROOT, "src", "cdLoadTask.c")).read())}
    sets = []
    for i in range(NSETS):
        o, files = off(word(SETS + 4 * i)), []
        while True:
            fid, slot, kind = struct.unpack_from("<hBB", d, o)
            o += 20
            if fid == -1:
                break
            files.append(nm.get(fid, "#%d" % fid))
        sets.append(files)
    ptrs = [word(AREA_SETS + 4 * a) for a in range(20)]
    ends = sorted(set(ptrs)) + [AREA_SETS]
    used = {0, 1, 2, 3}                       # boot / title / system sets (bootStartupTask, optsub ...)
    used |= set(d[off(EXTRA[0]):off(EXTRA[1]) + 20])
    for a, p in enumerate(ptrs):
        row = d[off(p):off(min(e for e in ends if e > p))]
        used |= set(row)
        if a == 15:
            continue                          # no AREA15: points at AREA00's row
        print("AREA%02d  %s" % (a, "  ".join("s%d:%02X %s" % (s, b, "+".join(sets[b])) for s, b in enumerate(row))))
    used |= {10, 12}                          # 9 -> 10 and 11 -> 12 by story flags in loadAreaSectionResources
    reached = {f for i in used if i < NSETS for f in sets[i]}
    snd = os.path.join(os.path.dirname(os.path.dirname(exe)), "SOUND")
    files = sorted(os.listdir(snd)) if os.path.isdir(snd) else []
    print("sets never reached:", [hex(i) for i in range(NSETS) if i not in used] or "none")
    print("SOUND files never reached:", [f for f in files if f not in reached] or "none")


if __name__ == "__main__":
    main()
