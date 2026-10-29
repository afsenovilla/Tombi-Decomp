#!/usr/bin/env python3
"""Render the room data of the Japanese demo (SLPM-80165) with the PAL tools (tools/room.py, tools/vram.py).

The demo has no SYS/LD0*.BIN load lists, so the VRAM of a room is rebuilt from the PAL AREA00 lists, whose layout the
demo files follow (same sizes, same names): room group n (n = 0..5) = B n00 page(s) at (64, 0) + CLUT0(n+1) at
(128, 480) + D n01 (WHM collision, 0x11 tag) + D n02.000 + D n03 (WMD room model, groups 0-2 only).  Area list 0 is
A000 (64x256 at 192,256), A001 (448x256 at 576,0), A002 (192x256 at 0,256).  The early AREA08 set (B000, B001,
CLUT01, D003) is placed the same way.  Pages that are bigger in the demo than in PAL are loaded with the demo size
(width = len / 512 words).

  python3 tools/jpdemo_rooms.py DEMO_FS OUT_DIR [--pal]     DEMO_FS = extracted demo disc (tools/isoext.py)
Writes OUT_DIR/<AREA>_<room>.png (orthographic WMD render, as tools/room.py), OUT_DIR/<AREA>_<group>_vram.png (16-bit
VRAM view of the room pages and palettes) and, with --pal, PAL_AREA00_D?01.png for comparison.
"""
import os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gam_py, room
import vram as V

AREA_LIST = [("A000.GAM", 192, 256), ("A001.GAM", 576, 0), ("A002.GAM", 0, 256)]


def parse_wmd_demo(d):
    """Demo WMD -> [(clut, tpage, cmd, [(x, y, z, u, v)...])].  Same container and 4 groups per sector as PAL (tools/room.py), but
    the packets keep the GPU order: `r g b cmd`, `u0 v0 clut`, `u1 v1 tpage`, `u2 v2 -`, (`u3 v3 -`), then 3/4 vertices
    `s16 x, y, z, pad`; gouraud packets put a colour word before every uv word after the first.  Some sectors carry a
    fifth group of untextured flat quads (cmd 0x28) after the four textured ones; it is skipped."""
    n = struct.unpack_from("<I", d, 0)[0]
    if not 1 <= n <= 1024 or 4 * n + 4 > len(d):
        return []
    o = list(struct.unpack_from("<%dI" % n, d, 4)) + [len(d)]
    out = []
    for i in range(n):
        s, p = d[o[i]:o[i + 1]], 0
        try:
            for _ in range(4):
                c = struct.unpack_from("<I", s, p)[0]
                p += 4
                for _ in range(c):
                    cmd = s[p + 3]
                    nv = 4 if cmd & 8 else 3
                    g = 4 if cmd & 0x10 else 0
                    uv = [struct.unpack_from("<BBH", s, p + 4)]
                    q = p + 8
                    for k in range(nv - 1):
                        uv.append(struct.unpack_from("<BBH", s, q + g))
                        q += 4 + g
                    vs = []
                    for k in range(nv):
                        x, y, z = struct.unpack_from("<hhh", s, q)
                        vs.append((x, y, z, uv[k][0], uv[k][1]))
                        q += 8
                    out.append((uv[0][2], uv[1][2], cmd, vs))
                    p = q
        except struct.error:
            pass
    return out


def rd(path):
    d = open(path, "rb").read()
    return gam_py.unpack(d) if d[:4] == b"GAM\0" else d


def put(v, path, x, y):
    d = rd(path)
    w = len(d) // 512
    v.load(x, y, w, 256, d, os.path.basename(path))


def context(area_dir, n, area="AREA00"):
    v = V.Vram()
    for f, x, y in AREA_LIST:
        p = os.path.join(area_dir, f)
        if os.path.exists(p) and area == "AREA00":
            put(v, p, x, y)
    pages = sorted(f for f in os.listdir(area_dir) if f.startswith("B%d" % n) and f.endswith(".GAM"))
    x = 64
    for f in pages:
        d = rd(os.path.join(area_dir, f))
        w = len(d) // 512
        v.load(x, 0, w, 256, d, f)
        x += w
    c = os.path.join(area_dir, "CLUT%02d.GAM" % (n + 1))
    if os.path.exists(c):
        v.load(128, 480, 256, 32, rd(c), "CLUT", "clut")
    return v, pages


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    fs, out = sys.argv[1], sys.argv[2]
    os.makedirs(out, exist_ok=True)
    for area in ("AREA00", "AREA08"):
        ad = os.path.join(fs, area)
        for n in range(6):
            if not any(f.startswith("B%d" % n) for f in os.listdir(ad)):
                continue
            v, pages = context(ad, n, area)
            open(os.path.join(out, "%s_B%d_vram.png" % (area, n)), "wb").write(v.snapshot_png())
            for f in sorted(os.listdir(ad)):
                if not (f.startswith("D%d" % n) and f.endswith(".GAM")):
                    continue
                d = rd(os.path.join(ad, f))
                if d[0] == 0x11:
                    continue
                pk = parse_wmd_demo(d)
                xs = [q[0] for p in pk for q in p[3]]
                if not pk or max(xs) - min(xs) > 16384:
                    print(area, f, "not a WMD")
                    continue
                open(os.path.join(out, "%s_%s.png" % (area, f[:4])), "wb").write(room.render(v, pk, 2))
                print(area, f, len(pk), "polygons, pages", pages)
    if "--pal" in sys.argv:
        for nm in ("D001", "D101", "D201"):
            print("PAL", nm, room.room_image("AR00", int(nm[1]) + 1, 2)[0])
            open(os.path.join(out, "PAL_AREA00_%s.png" % nm), "wb").write(room.room_image("AR00", int(nm[1]) + 1, 2)[1])


if __name__ == "__main__":
    main()
