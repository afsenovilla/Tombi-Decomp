#!/usr/bin/env python3
"""PrePre Vol. 8 (PCPX-96084), Sony's Japanese demo compilation, Tomba part: the oldest known Tomba build.
See docs/PREPRE8.md. Reads only; nothing extracted from the disc belongs in the repo.

  python3 -I tools/prepre8.py extract DISC.bin OUT      /TOMBA only (raw MODE2/2352 or 2048 image); *.STR are written
                                                       as raw 2352-byte sectors (OUT/.../X.STR.raw, playable by ffmpeg -f psxstr)
  python3 -I tools/prepre8.py unpack TOMBA_DIR OUT      every *.GAM unpacked (OUT/<name>.bin)
  python3 -I tools/prepre8.py loadlist TOMBA.EXE        file table + per-room load lists of the exe
  python3 -I tools/prepre8.py pages UNPACKED OUT        4bpp/16bpp grey views of the TIM/SYS pages, colour title screen
  python3 -I tools/prepre8.py player PLAYER.WPP OUT     Tomba frame sheet (313 frames, same WPP format as PAL A004.000)
  python3 -I tools/prepre8.py rooms UNPACKED OUT        the three WMD room models rendered with their VRAM (tools/room.py)

Formats (all as in PAL/SLPM-80165): GAM = LZ container; TIMnnn/SYSnnn = raw VRAM pages (width = len/512 words, 256
rows); CLUT00 = 640x32 words at (384,480); DATA001/3/4 = WHM collision + WMD model (old SLPM-80165 packet layout,
tools/jpdemo_rooms.py) + small tables; DATA002 = WHM only (2D room over the pre-rendered TIM007); DATA000 = sprite
and animation tables (kinds 0x30-0x53, 0xB0); PLAYER.WPP = WPP frame table; *.VH/*.VB = plain VAB banks.
"""
import os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

FILE_TABLE = 0x9FA3A      # exe file offset of the 39 x 0x40-byte entries: u16 kind, path, runtime CdlFILE
LOAD_LISTS = 0xA05FC      # 20-byte records: u16 file, u16 kind<<8|flag, u32 dest, u16 x, y, w, h, u16 n, u16 -
EXE_VRAM, EXE_HDR = 0x80010000, 0x800


def extract(img, out):
    f = open(img, "rb")
    f.seek(16 * 2352 + 25)
    ss, uo = (2352, 24) if f.read(5) == b"CD001" else (2048, 0)

    def sec(n, c=1):
        b = b""
        for i in range(c):
            f.seek((n + i) * ss + uo)
            b += f.read(2048)
        return b

    def walk(lba, size, path):
        d, i = sec(lba, (size + 2047) // 2048), 0
        while i < size:
            ln = d[i]
            if ln == 0:
                i = (i // 2048 + 1) * 2048
                continue
            elba, esz = struct.unpack_from("<I", d, i + 2)[0], struct.unpack_from("<I", d, i + 10)[0]
            fl, nl = d[i + 25], d[i + 32]
            nm = d[i + 33:i + 33 + nl].decode("latin1").split(";")[0]
            p = path + "/" + nm
            if nm not in ("\0", "\1") and (p + "/").upper().startswith("/TOMBA/"):
                if fl & 2:
                    os.makedirs(out + p, exist_ok=True)
                    walk(elba, esz, p)
                elif nm.upper().endswith(".STR") and ss == 2352:
                    with open(out + p + ".raw", "wb") as o:
                        for k in range(esz // 2048):
                            f.seek((elba + k) * 2352)
                            o.write(f.read(2352))
                else:
                    open(out + p, "wb").write(sec(elba, (esz + 2047) // 2048)[:esz])
            i += ln

    pvd = sec(16)
    r = pvd[156:190]
    os.makedirs(out, exist_ok=True)
    walk(struct.unpack_from("<I", r, 2)[0], struct.unpack_from("<I", r, 10)[0], "")


def unpack(src, out):
    import gam_py
    os.makedirs(out, exist_ok=True)
    for dp, _, fs in os.walk(src):
        for n in sorted(fs):
            if n.upper().endswith(".GAM"):
                open(os.path.join(out, n[:-4] + ".bin"), "wb").write(gam_py.unpack(open(os.path.join(dp, n), "rb").read()))


def names(d):
    out, o = [], FILE_TABLE
    while d[o:o + 6] == b"\\TOMBA":
        out.append(d[o:o + 0x30].split(b"\0")[0].decode().rsplit("\\", 1)[1].split(";")[0])
        o += 0x40
    return out


def loadlist(exe):
    d = open(exe, "rb").read()
    nm = names(d)
    for i, n in enumerate(nm):
        print("%2d %s" % (i, n))
    o = LOAD_LISTS
    while True:
        i, k, dst, x, y, w, h, c, _ = struct.unpack_from("<HHIHHHHHH", d, o)
        if i == 0xFFFF:
            print("---")
        elif i == 0 and k == 0 and dst == 0 and x == 0:
            break
        elif i >= len(nm):
            break
        else:
            kind = k >> 8
            if kind in (0x10,):
                print("  %-12s page at (%d,%d) %dx%d" % (nm[i], x, y, w, h))
            elif c == 0xFFFF:
                print("    sub-chunk kind 0x%02X size %d" % (kind, x | y << 16))
            else:
                print("  %-12s kind 0x%02X dest 0x%08X size %d" % (nm[i], kind, dst, x | y << 16))
        o += 20


def _grey(a):
    import numpy as np
    g = np.empty((a.shape[0], a.shape[1] * 2), np.uint8)
    g[:, 0::2] = (a & 15) * 17
    g[:, 1::2] = (a >> 4) * 17
    return g


def _rgb(h):
    import numpy as np
    h = h.astype(np.uint32)
    return np.stack([(h & 31) << 3, ((h >> 5) & 31) << 3, ((h >> 10) & 31) << 3], -1).astype(np.uint8)


def pages(un, out, sysdir=None):
    import numpy as np
    from PIL import Image
    os.makedirs(out, exist_ok=True)
    for f in sorted(os.listdir(un)):
        if not f.endswith(".bin") or f.startswith(("DATA", "CLUT")):
            continue
        d = np.frombuffer(open(os.path.join(un, f), "rb").read(), np.uint8)
        a = d.reshape(256, -1)
        Image.fromarray(_grey(a)).save(os.path.join(out, f[:-4] + "_4bpp.png"))
        Image.fromarray(a).save(os.path.join(out, f[:-4] + "_8bpp.png"))
    sysdir = sysdir or os.path.join(os.path.dirname(un.rstrip("/")), "fs", "TOMBA", "SYSTEM")
    s5 = os.path.join(un, "SYS005.bin")
    if os.path.exists(s5) and os.path.exists(os.path.join(sysdir, "SYS006.000")):
        a = np.frombuffer(open(s5, "rb").read(), np.uint8).reshape(256, -1)
        bg = _rgb(np.frombuffer(open(os.path.join(sysdir, "SYS006.000"), "rb").read(), "<u2"))[a]
        lg = _rgb(np.frombuffer(open(os.path.join(sysdir, "SYS007.000"), "rb").read(), "<u2"))[a]
        bg[:, 320:] = lg[:, 320:]       # background (x < 320) uses SYS006, the logo SYS007
        Image.fromarray(bg).save(os.path.join(out, "title_SYS005.png"))
    t7 = os.path.join(un, "TIM007.bin")
    if os.path.exists(t7):
        c = np.frombuffer(open(os.path.join(un, "CLUT00.bin"), "rb").read(), "<u2").reshape(32, 640)
        a = np.frombuffer(open(t7, "rb").read(), np.uint8).reshape(256, -1)
        Image.fromarray(_rgb(c[20, :256])[a]).save(os.path.join(out, "TIM007_interior.png"))   # CLUT00 row 20


def player(wpp, out):
    import numpy as np
    import wpp as W
    from PIL import Image
    d = open(wpp, "rb").read()
    n = struct.unpack_from("<I", d, 0)[0]
    offs = list(struct.unpack_from("<%dI" % (n + 1), d, 4))
    cols, cell = 20, 64
    sheet = np.zeros((cell * ((n + cols - 1) // cols), cell * cols), np.uint8)
    for i in range(n):
        b = bytes(W.wpp_decode(d[offs[i]:offs[i + 1] if i + 1 < len(offs) else len(d)]))
        w, h = struct.unpack_from("<HH", b, 0)
        g = _grey(np.frombuffer(b[4:4 + 2 * w * h], np.uint8).reshape(h, 2 * w))
        r, c = divmod(i, cols)
        sheet[r * cell:r * cell + min(h, cell), c * cell:c * cell + min(4 * w, cell)] = g[:cell, :cell]
    os.makedirs(out, exist_ok=True)
    Image.fromarray(sheet).save(os.path.join(out, "player_sheet.png"))
    print(n, "frames")


ROOMS = {"village": ([("TIM003", 320, 0, 64), ("TIM004", 448, 0, 64)], "DATA001", 0, 181348),
         "street": ([], "DATA003", 1892, 1892 + 71936),
         "house": ([("TIM010", 448, 0, 64)], "DATA004", 2628, 2628 + 55012)}


def rooms(un, out):
    import room, vram as V, jpdemo_rooms as J
    os.makedirs(out, exist_ok=True)
    rd = lambda n: open(os.path.join(un, n + ".bin"), "rb").read()
    for name, (extra, f, a, b) in ROOMS.items():
        v = V.Vram()
        v.load(384, 480, 640, 32, rd("CLUT00"), "CLUT00", "clut")
        for n, x, y, w in [("TIM000", 0, 0, 512), ("TIM001", 512, 0, 512), ("TIM002", 0, 256, 320)] + extra:
            v.load(x, y, w, 256, rd(n), n)
        pk = J.parse_wmd_demo(rd(f)[a:b])
        open(os.path.join(out, "room_%s_%s.png" % (name, f)), "wb").write(room.render(v, pk, 2))
        print(name, f, len(pk), "polygons")


def main():
    a = sys.argv[1:]
    if len(a) < 2:
        sys.exit(__doc__)
    cmd = {"extract": extract, "unpack": unpack, "loadlist": loadlist, "pages": pages, "player": player,
           "rooms": rooms}.get(a[0])
    if not cmd:
        sys.exit(__doc__)
    cmd(*a[1:])


if __name__ == "__main__":
    main()
