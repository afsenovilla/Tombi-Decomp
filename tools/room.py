#!/usr/bin/env python3
"""Tombi! (PAL) room backgrounds = WMD 3D models (load kind 0x50, `AREAnn/Dxxx.GAM`).  Notes: notes/formats.md ("WMD rooms").

  tools/room.py AREA00:1 OUT.png        render the model of load list 1 of area 00 (list index as in tools/vram.py snapshot)
  tools/room.py --all OUT_DIR           every room model of every area -> OUT_DIR/AREAnn_Dxxx.png

A WMD is a u32 table container (N sectors, vertical slices of the room).  Every sector = 4 groups `u32 count` + `count` GPU-like
packets (flat tri, flat quad, gouraud tri, gouraud quad), then 16 B zero padding.  Packet: `u16 clut, u16 tpage, r g b cmd`
(cmd bit 3 = quad, bit 4 = gouraud: 2 (tri) / 3 (quad) more `r g b cmd` words follow), then 3/4 vertices `s16 x, y, z, u8 u, v`.
clut / tpage are real GPU words (4 bpp 16-colour palettes `x = (c & 63) * 16`, `y = c >> 6`, mode bits 7-8 of tpage), so the per polygon
palette is in the data.  The pages are the room pages of the load list (VRAM replay, tools/vram.py).  The renderer projects
orthographically (x right, y down, z only orders the polygons far to near); the in-game camera is not reproduced.
"""
import os, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import area000 as A
import vram as V


def parse_wmd(d):
    """-> list of (clut, tpage, cmd, [(x, y, z, u, v)...]) for all sectors."""
    n = struct.unpack_from("<I", d, 0)[0]
    o = list(struct.unpack_from("<%dI" % (n + 1), d, 4))
    o[-1] = o[-1] or len(d)
    out = []
    for i in range(n):
        s = d[o[i]:o[i + 1]]
        p = 0
        for _ in range(4):
            c = struct.unpack_from("<I", s, p)[0]
            p += 4
            for _ in range(c):
                cl, tp, _r, _g, _b, cmd = struct.unpack_from("<HHBBBB", s, p)
                nv = 4 if cmd & 8 else 3
                ex = (nv - 1) * 4 if cmd & 0x10 else 0
                vs = [struct.unpack_from("<hhhBB", s, p + 8 + ex + 8 * k) for k in range(nv)]
                out.append((cl, tp, cmd, vs))
                p += 8 + ex + 8 * nv
    return out


def render(v, pk, scale=1):
    """RGBA PNG bytes of the model; texels read from VRAM `v`, word 0 = transparent."""
    xs = [q[0] for p in pk for q in p[3]]
    ys = [q[1] for p in pk for q in p[3]]
    x0, y0 = min(xs), min(ys)
    W, H = (max(xs) - x0) // scale + 1, (max(ys) - y0) // scale + 1
    buf = [bytearray(4 * W) for _ in range(H)]
    nib, dd = v.nib, v.d
    for cl, tp, cmd, vs in sorted(pk, key=lambda p: -sum(q[2] for q in p[3])):
        mode, pgx, ty = tp >> 7 & 3, (tp & 15) * 64, (tp >> 4 & 1) * 256
        if mode == 3:
            continue
        rgb = [V.RGB[w] if w else None for w in v.palette(cl, 16 if mode == 0 else 256)[2]]
        tris = [vs[:3]] if len(vs) == 3 else [vs[:3], vs[1:]]
        for t in tris:
            px = [(q[0] - x0) / scale for q in t]
            py = [(q[1] - y0) / scale for q in t]
            den = (py[1] - py[2]) * (px[0] - px[2]) + (px[2] - px[1]) * (py[0] - py[2])
            if den == 0:
                continue
            for yy in range(max(int(min(py)), 0), min(int(max(py)), H - 1) + 1):
                row = buf[yy]
                for xx in range(max(int(min(px)), 0), min(int(max(px)), W - 1) + 1):
                    a = ((py[1] - py[2]) * (xx - px[2]) + (px[2] - px[1]) * (yy - py[2])) / den
                    b = ((py[2] - py[0]) * (xx - px[2]) + (px[0] - px[2]) * (yy - py[2])) / den
                    c = 1 - a - b
                    if a < -.01 or b < -.01 or c < -.01:
                        continue
                    u = int(a * t[0][3] + b * t[1][3] + c * t[2][3]) & 255
                    w = int(a * t[0][4] + b * t[1][4] + c * t[2][4]) & 255
                    if mode == 0:
                        i = nib[(ty + w) * 4096 + pgx * 4 + u]
                    elif mode == 1:
                        i = dd[(ty + w) * 2048 + pgx * 2 + u]
                    else:
                        continue
                    col = rgb[i]
                    if col:
                        row[xx * 4:xx * 4 + 4] = col + b"\xff"
    return V.png_rgba(W, H, buf)


def room_image(ld, idx, scale=1):
    """PNG bytes of the WMD of load list idx of area ld ('AR00'), or None."""
    lst = A.load_lists()[ld][idx]
    rec = [r for r in lst if r["kind"] == 0x50 and r["name"].startswith("AREA")]
    if not rec:
        return None, None
    d = V.resource(rec[0]["name"])
    if not d:
        return None, None
    return rec[0]["name"], render(V.area_context(ld, idx), parse_wmd(d), scale)


def export_rooms(out, log=print, scale=2):
    n = 0
    for ld, lists in A.load_lists().items():
        for i in range(len(lists)):
            try:
                nm, png = room_image(ld, i, scale)
            except Exception as e:  # unknown variants (e.g. AREA06 D004.000) are skipped
                log("room %s:%d skipped: %r" % (ld, i, e))
                continue
            if png:
                V.write(os.path.join(out, nm.replace("/", "_").rsplit(".", 1)[0] + ".png"), png)
                n += 1
    return n


def main(argv=None):
    a = sys.argv[1:] if argv is None else argv
    if len(a) == 2 and a[0] == "--all":
        print(export_rooms(a[1]), "rooms")
        return 0
    if len(a) >= 2 and ":" in a[0]:
        nm, i = a[0].split(":")
        scale = int(a[2]) if len(a) > 2 else 1
        _, png = room_image("AR" + nm[-2:], int(i), scale)
        if not png:
            sys.stderr.write("no WMD room in that list\n")
            return 1
        V.write(a[1], png)
        return 0
    sys.stderr.write(__doc__)
    return 1


if __name__ == "__main__":
    sys.exit(main())
