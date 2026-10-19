#!/usr/bin/env python3
"""Print the spawn / exit tables of the areas: D_80079A48[area] -> per-section tables of 8-byte entries
(x, y, type, next area, next section, next spawn), read by cameraEventScrollHandler when a section handler calls
tryStartItemInteraction(o, 2, exit).  Tables of AREA00/01 live in the area overlay, the others in MAIN0.

    python3 port/tools/exits.py AREA [X0nn.BIN]      e.g. python3 port/tools/exits.py 0 game/AREA00/X000.BIN"""
import struct
import sys

exe = open('game/MAIN0.EXE', 'rb').read()
base = struct.unpack('<I', exe[0x18:0x1c])[0]
ovl = open(sys.argv[2], 'rb').read() if len(sys.argv) > 2 else b''


def rd(a, n):
    if a >= 0x800E8028:
        off = a - 0x800E8028
        return ovl[off:off + n]
    off = a - base + 0x800
    return exe[off:off + n]


def u32(a): return struct.unpack('<I', rd(a, 4))[0]


area = int(sys.argv[1])
at = u32(0x80079A48 + area * 4)
print(f"area {area}: section tables at {at:#x}")
starts = []
for sec in range(24):
    st = u32(at + sec * 4)
    if st < 0x80010000 or st > 0x80140000 or (starts and st < starts[-1]):
        break
    starts.append(st)
for sec, st in enumerate(starts):
    n = ((starts[sec + 1] - st) // 8) if sec + 1 < len(starts) else 6
    print(f" section {sec} ({st:#x}):")
    for sp in range(n):
        e = rd(st + sp * 8, 8)
        if len(e) < 8:
            break
        x, y = struct.unpack('<hh', e[:4])
        print(f"   exit/spawn {sp}: x={x:#x} y={y:#x} type={e[4]} -> area {e[5]} section {e[6]} spawn {e[7]}")
