#!/usr/bin/env python3
"""Tombi! (PAL) room backgrounds = WMD 3D models (load kind 0x50, `AREAnn/Dxxx.GAM`).  Notes: notes/formats.md ("WMD rooms").

  tools/room.py AREA00:1 OUT.png        render the model of load list 1 of area 00 (list index as in tools/vram.py snapshot)
  tools/room.py --all OUT_DIR           every room model of every area -> OUT_DIR/AREAnn_Dxxx.png
  tools/room.py --glb AREA00:1 OUT.glb  3D export (glTF 2.0 binary, one embedded texture atlas, vertex colours, Y up) for Blender / 3D Viewer
  tools/room.py --glb-all DIR           every room -> DIR/AREAnn_Dxxx.glb
  options: --sky-dir DIR   sky captures room_<AA>_<n>_sky.png (tools/psxemu/capture.py rooms DIR); default: the cache of tools/gfx_composed.py
           --no-sky        leave the sky out (by default it is added when a capture exists)
The sky (far background layer the area overlay draws behind the room) becomes an unlit, double sided backdrop quad behind the model in the .glb
(mirror-tiled horizontally to cover the model) and the picture behind the 2D room PNGs.

A WMD is a u32 table container ([N][off * N], no terminator; N sectors, vertical slices of the room).  Every sector = 4 groups `u32 count` + `count` GPU-like
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
import labels

SKY_DIR = None      # folder of the sky captures (None: the cache of gfx_composed for the disc image)
USE_SKY = True


def find_sky(ld, idx):
    """Path of the sky capture of load list idx of area ld ('AR04'), or None (also when sky is off / no capture / no PIL)."""
    if not USE_SKY:
        return None
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "psxemu"))
    try:
        from rooms import list_to_room
    finally:
        sys.path.pop(0)
    r = list_to_room(ld, idx)
    if not r:
        return None
    name = "room_%02d_%d_sky.png" % r
    d = SKY_DIR
    if d is None:
        try:
            import gfx_composed
            img = A.IMG or os.path.join(os.path.dirname(A.FS), "Tombi.bin")
            d = gfx_composed.cache_dir(img) if os.path.isfile(img) else None
        except Exception:
            d = None
    p = d and os.path.join(d, name)
    return p if p and os.path.isfile(p) else None


def load_sky(path):
    """PIL RGB image of a sky capture without its black letterbox bars (a fully black border of at most 8 px)."""
    from PIL import Image
    im = Image.open(path).convert("RGB")
    w, h = im.size
    px = im.load()
    def black_row(y): return all(px[x, y] == (0, 0, 0) for x in range(w))
    def black_col(x): return all(px[x, y] == (0, 0, 0) for y in range(h))
    t = 0
    while t < 8 and black_row(t): t += 1
    b = 0
    while b < 8 and black_row(h - 1 - b): b += 1
    l = 0
    while l < 8 and black_col(l): l += 1
    r = 0
    while r < 8 and black_col(w - 1 - r): r += 1
    return im.crop((l, t, w - r, h - b))


def sky_behind(png, sky):
    """PNG bytes of the room picture `png` over the sky image (scaled to the picture height, mirror-tiled horizontally)."""
    import io
    from PIL import Image
    room = Image.open(io.BytesIO(png)).convert("RGBA")
    W, H = room.size
    sw = max(1, round(sky.width * H / sky.height))
    tile = sky.resize((sw, H), Image.NEAREST)
    mir = tile.transpose(Image.FLIP_LEFT_RIGHT)
    bg = Image.new("RGB", (W, H))
    for k in range((W + sw - 1) // sw):
        bg.paste(tile if k % 2 == 0 else mir, (k * sw, 0))
    out = bg.convert("RGBA")
    out.alpha_composite(room)
    o = io.BytesIO()
    out.convert("RGB").save(o, "PNG")
    return o.getvalue()


def parse_wmd(d):
    """-> list of (clut, tpage, cmd, [(x, y, z, u, v)...]) for all sectors."""
    return [p[:4] for p in parse_wmd_ex(d)]


def parse_wmd_ex(d):
    """Like parse_wmd, plus the per vertex colours: (clut, tpage, cmd, verts, [(r, g, b)...])."""
    n = struct.unpack_from("<I", d, 0)[0]
    # N sector offsets (no terminator: with N = 1 the table is 8 bytes and sector 0 starts right after it); last sector runs to the end
    o = list(struct.unpack_from("<%dI" % n, d, 4)) + [len(d)]
    out = []
    for i in range(n):
        s = d[o[i]:o[i + 1]]
        p = 0
        for _ in range(4):
            c = struct.unpack_from("<I", s, p)[0]
            p += 4
            for _ in range(c):
                cl, tp, r0, g0, b0, cmd = struct.unpack_from("<HHBBBB", s, p)
                nv = 4 if cmd & 8 else 3
                ex = (nv - 1) * 4 if cmd & 0x10 else 0
                vs = [struct.unpack_from("<hhhBB", s, p + 8 + ex + 8 * k) for k in range(nv)]
                cols = [(r0, g0, b0)] * nv
                if ex:
                    cols = [cols[0]] + [struct.unpack_from("<BBB", s, p + 8 + 4 * k) for k in range(nv - 1)]
                out.append((cl, tp, cmd, vs, cols))
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


def build_glb(v, pk, scale=0.01, sky=None):
    """Binary glTF 2.0 (bytes) of the polygons `pk` (parse_wmd_ex): one mesh, one atlas texture of the used (tpage, clut) pairs.
    sky (PIL RGB, load_sky): backdrop quad.  Y is flipped (PS1 y is down) and z negated so the model is not mirrored; colours are the PS1 modulation values (0x80 = 1.0)."""
    import json
    tiles = {}
    for cl, tp, cmd, vs, cols in pk:
        if tp >> 7 & 3 < 2:
            tiles.setdefault((tp & 0x1FF, cl), len(tiles))
    ncol = 8
    nrow = max(1, (len(tiles) + ncol - 1) // ncol)
    AW, AH = ncol * 256, nrow * 256
    atlas = [bytearray(4 * AW) for _ in range(AH)]
    nib, dd = v.nib, v.d
    for (tp, cl), k in tiles.items():
        mode, pgx, ty = tp >> 7 & 3, (tp & 15) * 64, (tp >> 4 & 1) * 256
        ws = v.palette(cl, 16 if mode == 0 else 256)[2]
        pal = [V.RGB[w] + b"\xff" if w else b"\0\0\0\0" for w in ws]
        ox, oy = k % ncol * 256, k // ncol * 256
        for w in range(256):
            row = atlas[oy + w]
            if mode == 0:
                src = nib[(ty + w) * 4096 + pgx * 4:][:256]
            else:
                src = dd[(ty + w) * 2048 + pgx * 2:][:256]
            row[ox * 4:(ox + 256) * 4] = b"".join(pal[i] for i in src)
    pos, uvs, col, idx = [], [], [], []
    for cl, tp, cmd, vs, cols in pk:
        mode = tp >> 7 & 3
        k = tiles.get((tp & 0x1FF, cl)) if mode < 2 else None
        base = len(pos)
        for q, c in zip(vs, cols):
            pos.append((q[0] * scale, -q[1] * scale, -q[2] * scale))
            if k is None:  # 15 bpp: no palette, left on a transparent corner texel
                uvs.append((0.0, 0.0))
            else:
                uvs.append(((k % ncol * 256 + q[3] + .5) / AW, (k // ncol * 256 + q[4] + .5) / AH))
            col.append((min(255, c[0] * 2), min(255, c[1] * 2), min(255, c[2] * 2), 255))
        idx += [base, base + 1, base + 2]
        if len(vs) == 4:
            idx += [base + 1, base + 3, base + 2]
    n = len(pos)
    sky_png = b""
    if sky is not None:
        import io
        x0, x1 = min(p[0] for p in pos), max(p[0] for p in pos)
        y0, y1 = min(p[1] for p in pos), max(p[1] for p in pos)
        z0 = min(p[2] for p in pos)
        h = max((y1 - y0) * 1.3, 256 * scale)          # at least one PS1 screen high
        tw = h * sky.width / sky.height
        ntile = max(1, int(((x1 - x0) * 1.4 + tw) / tw) + 1)
        cx, cy = (x0 + x1) / 2, (y0 + y1) / 2
        z = z0 - 0.5 * h * 0.1 - 10 * scale            # just behind the farthest polygon
        sq = [(cx - ntile * tw / 2, cy + h / 2), (cx + ntile * tw / 2, cy + h / 2), (cx - ntile * tw / 2, cy - h / 2), (cx + ntile * tw / 2, cy - h / 2)]
        sky_pos = [(a, b, z) for a, b in sq]
        sky_uv = [(0.0, 0.0), (float(ntile), 0.0), (0.0, 1.0), (float(ntile), 1.0)]
        o = io.BytesIO()
        sky.save(o, "PNG")
        sky_png = o.getvalue()
    b_pos = struct.pack("<%df" % (3 * n), *[c for p in pos for c in p])
    b_uv = struct.pack("<%df" % (2 * n), *[c for p in uvs for c in p])
    b_col = bytes(c for p in col for c in p)
    b_idx = struct.pack("<%dI" % len(idx), *idx)
    png = V.png_rgba(AW, AH, atlas)
    bl = [b_pos, b_uv, b_col, b_idx, png]
    if sky is not None:
        bl += [struct.pack("<12f", *[c for p in sky_pos for c in p]), struct.pack("<8f", *[c for p in sky_uv for c in p]),
               struct.pack("<6I", 0, 1, 2, 1, 3, 2), sky_png]
    views, off, blob = [], 0, b""
    for b in bl:
        views.append({"buffer": 0, "byteOffset": off, "byteLength": len(b)})
        pad = -len(b) % 4
        blob += b + b"\0" * pad
        off += len(b) + pad
    for i in range(3):
        views[i]["target"] = 34962
    views[3]["target"] = 34963
    mn = [min(p[i] for p in pos) for i in range(3)]
    mx = [max(p[i] for p in pos) for i in range(3)]
    js = {
        "asset": {"version": "2.0", "generator": "Tombi-Decomp tools/room.py"},
        "scene": 0, "scenes": [{"nodes": [0]}], "nodes": [{"mesh": 0}],
        "meshes": [{"primitives": [{"attributes": {"POSITION": 0, "TEXCOORD_0": 1, "COLOR_0": 2}, "indices": 3, "material": 0}]}],
        "materials": [{"alphaMode": "MASK", "alphaCutoff": 0.5, "doubleSided": True,
                       "pbrMetallicRoughness": {"baseColorTexture": {"index": 0}, "metallicFactor": 0.0, "roughnessFactor": 1.0}}],
        "textures": [{"source": 0, "sampler": 0}],
        "samplers": [{"magFilter": 9728, "minFilter": 9728, "wrapS": 33071, "wrapT": 33071}],
        "images": [{"bufferView": 4, "mimeType": "image/png"}],
        "accessors": [
            {"bufferView": 0, "componentType": 5126, "count": n, "type": "VEC3", "min": mn, "max": mx},
            {"bufferView": 1, "componentType": 5126, "count": n, "type": "VEC2"},
            {"bufferView": 2, "componentType": 5121, "normalized": True, "count": n, "type": "VEC4"},
            {"bufferView": 3, "componentType": 5125, "count": len(idx), "type": "SCALAR"}],
        "bufferViews": views, "buffers": [{"byteLength": len(blob)}],
    }
    if sky is not None:
        views[5]["target"] = views[6]["target"] = 34962
        views[7]["target"] = 34963
        js["extensionsUsed"] = ["KHR_materials_unlit"]
        js["nodes"].append({"mesh": 1, "name": "sky"})
        js["scenes"][0]["nodes"].append(1)
        js["meshes"].append({"name": "sky", "primitives": [{"attributes": {"POSITION": 4, "TEXCOORD_0": 5}, "indices": 6, "material": 1}]})
        js["materials"].append({"name": "sky", "alphaMode": "OPAQUE", "doubleSided": True, "extensions": {"KHR_materials_unlit": {}},
                                "pbrMetallicRoughness": {"baseColorTexture": {"index": 1}, "metallicFactor": 0.0, "roughnessFactor": 1.0}})
        js["textures"].append({"source": 1, "sampler": 1})
        js["samplers"].append({"magFilter": 9729, "minFilter": 9729, "wrapS": 33648, "wrapT": 33071})
        js["images"].append({"bufferView": 8, "mimeType": "image/png"})
        mn2 = [min(p[i] for p in sky_pos) for i in range(3)]
        mx2 = [max(p[i] for p in sky_pos) for i in range(3)]
        js["accessors"] += [{"bufferView": 5, "componentType": 5126, "count": 4, "type": "VEC3", "min": mn2, "max": mx2},
                            {"bufferView": 6, "componentType": 5126, "count": 4, "type": "VEC2"},
                            {"bufferView": 7, "componentType": 5125, "count": 6, "type": "SCALAR"}]
    j = json.dumps(js, separators=(",", ":")).encode()
    j += b" " * (-len(j) % 4)
    return (struct.pack("<III", 0x46546C67, 2, 28 + len(j) + len(blob)) + struct.pack("<II", len(j), 0x4E4F534A) + j
            + struct.pack("<II", len(blob), 0x004E4942) + blob)


def room_glb(ld, idx):
    """(name, glb bytes) of the WMD of load list idx of area ld, or (None, None)."""
    lst = A.load_lists()[ld][idx]
    rec = [r for r in lst if r["kind"] == 0x50 and r["name"].startswith("AREA")]
    d = rec and V.resource(rec[0]["name"])
    if not d:
        return None, None
    sp = find_sky(ld, idx)
    return rec[0]["name"], build_glb(V.area_context(ld, idx), parse_wmd_ex(d), sky=load_sky(sp) if sp else None)


def export_glb(out, log=print):
    n = 0
    for ld, lists in A.load_lists().items():
        for i in range(len(lists)):
            try:
                nm, g = room_glb(ld, i)
            except Exception as e:
                log("room %s:%d skipped: %r" % (ld, i, e))
                continue
            if g:
                V.write(os.path.join(out, labels.fname("room:%s:%d" % (ld, i), nm.replace("/", "_").rsplit(".", 1)[0]) + ".glb"), g)
                n += 1
    return n


def room_image(ld, idx, scale=1):
    """PNG bytes of the WMD of load list idx of area ld ('AR00'), or None."""
    lst = A.load_lists()[ld][idx]
    rec = [r for r in lst if r["kind"] == 0x50 and r["name"].startswith("AREA")]
    if not rec:
        return None, None
    d = V.resource(rec[0]["name"])
    if not d:
        return None, None
    png = render(V.area_context(ld, idx), parse_wmd(d), scale)
    sp = find_sky(ld, idx)
    return rec[0]["name"], sky_behind(png, load_sky(sp)) if sp else png


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
                V.write(os.path.join(out, labels.fname("room:%s:%d" % (ld, i), nm.replace("/", "_").rsplit(".", 1)[0]) + ".png"), png)
                n += 1
    return n


def main(argv=None):
    global SKY_DIR, USE_SKY
    a = list(sys.argv[1:] if argv is None else argv)
    if "--no-sky" in a:
        USE_SKY = False
        a.remove("--no-sky")
    if "--sky-dir" in a:
        i = a.index("--sky-dir")
        SKY_DIR = a[i + 1]
        del a[i:i + 2]
    if len(a) == 2 and a[0] == "--glb-all":
        print(export_glb(a[1]), "rooms")
        return 0
    if len(a) == 3 and a[0] == "--glb":
        nm, i = a[1].split(":")
        _, g = room_glb("AR" + nm[-2:], int(i))
        if not g:
            sys.stderr.write("no WMD room in that list\n")
            return 1
        V.write(a[2], g)
        return 0
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
