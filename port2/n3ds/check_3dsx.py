#!/usr/bin/env python3
"""Checks a .3dsx as the loaders do (Azahar's src/core/loader/3dsx.cpp, the Homebrew Launcher's): header, segment
sizes, relocation tables.  Prints the first relocation a loader rejects (an absolute one whose word has any of the
top 4 bits set, a relative one with an unknown sub-type), with the nearest symbol of the ELF when one is given.

    python3 port2/n3ds/check_3dsx.py build/n3ds/tombi.3dsx [build/n3ds/tombi.elf [NM]]
"""
import bisect, struct, subprocess, sys

BASE = 0x00100000


def syms(elf, nm):
    out = subprocess.run([nm, "-n", elf], capture_output=True, text=True).stdout
    s = []
    for l in out.split("\n"):
        w = l.split()
        if len(w) == 3 and w[1] in "tTdDbBrR":
            s.append((int(w[0], 16), w[2]))
    return s


def near(s, a):
    if not s: return ""
    i = bisect.bisect_right([x for x, _ in s], a) - 1
    return "%s+0x%x" % (s[i][1], a - s[i][0]) if i >= 0 else ""


def main():
    f = open(sys.argv[1], "rb").read()
    s = syms(sys.argv[2], sys.argv[3] if len(sys.argv) > 3 else "arm-none-eabi-nm") if len(sys.argv) > 2 else []
    magic, hsize, rsize, ver, flags, code, ro, data, bss = struct.unpack_from("<4sHHIIIIII", f, 0)
    print("3dsx: header %d, reloc header %d, code %#x rodata %#x data %#x (bss %#x)" % (hsize, rsize, code, ro, data, bss))
    if magic != b"3DSX": sys.exit("not a 3dsx")
    seg = [(x + 0xFFF) & ~0xFFF for x in (code, ro, data)]
    offs = [seg[0], seg[0] + seg[1]]
    total = sum(seg)
    print("image %#x bytes (%.1f MB): %#x..%#x" % (total, total / 1048576, BASE, BASE + total))
    ntab = rsize // 4
    p = hsize
    counts = []
    for _ in range(3):
        counts.append(struct.unpack_from("<%dI" % ntab, f, p)); p += 4 * ntab
    img = bytearray(total)
    img[0:code] = f[p:p + code]; p += code
    img[seg[0]:seg[0] + ro] = f[p:p + ro]; p += ro
    img[offs[1]:offs[1] + data - bss] = f[p:p + data - bss]; p += data - bss
    bad = 0
    for sg in range(3):
        start = [0, offs[0], offs[1]][sg]
        for t in range(ntab):
            n = counts[sg][t]
            if t >= 2: p += 4 * n; continue
            pos = start
            for k in range(n):
                skip, patch = struct.unpack_from("<HH", f, p); p += 4
                pos += 4 * skip
                for _ in range(patch):
                    w = struct.unpack_from("<I", img, pos)[0]
                    sub = w >> 28
                    if (t == 0 and sub != 0) or (t == 1 and sub not in (0, 1)):
                        bad += 1
                        if bad <= 20:
                            a = BASE + pos
                            print("REJECTED: segment %d table %d at %#x (%s): word %#010x" % (sg, t, a, near(s, a), w))
                    pos += 4
    print("%d relocations a loader rejects" % bad)
    sys.exit(1 if bad else 0)


main()
