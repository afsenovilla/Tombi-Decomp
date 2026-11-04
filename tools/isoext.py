#!/usr/bin/env python3
"""Extracts the ISO9660 file system of a PS1 disc image (raw 2352-byte sectors, MODE2).
Usage: python3 tools/isoext.py disc.bin out_dir"""
import sys, os, struct
img, out = sys.argv[1], sys.argv[2]
f = open(img, "rb")
f.seek(16 * 2352 + 25)
SS, UO = (2352, 24) if f.read(5) == b"CD001" else (2048, 0)   # raw MODE2/2352 or a 2048-byte ISO
def sec(n, cnt=1):
    b = b""
    for i in range(cnt):
        f.seek((n + i) * SS + UO); b += f.read(2048)
    return b
def walk(lba, size, path):
    d = sec(lba, (size + 2047) // 2048); i = 0
    while i < size:
        l = d[i]
        if l == 0: i = (i // 2048 + 1) * 2048; continue
        elba, esz = struct.unpack("<I", d[i+2:i+6])[0], struct.unpack("<I", d[i+10:i+14])[0]
        flags, nl = d[i+25], d[i+32]; name = d[i+33:i+33+nl]
        if name not in (b"\x00", b"\x01"):
            nm = name.decode("ascii", "replace").split(";")[0]
            p = os.path.join(path, nm)
            if flags & 2: os.makedirs(p, exist_ok=True); walk(elba, esz, p)
            else:
                with open(p, "wb") as o:
                    left = esz; n = elba
                    while left > 0:
                        # XA form2 (2324-byte) sectors: STR/XA streams; read raw user data per submode
                        if SS == 2352:
                            f.seek(n * 2352 + 16); sh = f.read(8)
                            if len(sh) < 8: break          # the entry runs past the image (a truncated dump): keep what there is
                            form2 = sh[2] & 0x20
                        else:
                            f.seek(n * 2048); form2 = 0
                        chunk = f.read(2324 if form2 else 2048)
                        if not chunk: break
                        o.write(chunk[:min(left, len(chunk))] if not form2 else chunk); left -= 2048; n += 1
        i += l
pvd = sec(16)
root = pvd[156:190]
walk(struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], out)
