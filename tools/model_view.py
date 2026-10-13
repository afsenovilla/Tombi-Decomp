#!/usr/bin/env python3
"""3D model viewer core (no GUI): loads the WMD room models (kind 0x50) and the object models (kind 0x51) of the load lists as numpy arrays,
and a numpy software rasterizer (z-buffer, perspective-correct UV, nearest sampling, alpha cut-out) used when OpenGL is unavailable.
The tk widget is tools/model_viewer.py; the polygon format is parsed by tools/room.py (parse_wmd_ex), the glTF export is room.build_glb.

  tools/model_view.py room AR00:1 OUT.png [yaw pitch [W H]]       software render of a room
  tools/model_view.py obj  AR00:1:5 OUT.png [yaw pitch [W H]]     object model 5 of the table of load list AR00:1

Mesh = parts (dict pos (N,3) float32 [x, -y, -z like the .glb, scale 0.01], uv (N,2) texel coordinates in tex, col (N,3) 0..1 modulation,
idx (T,3) int32, tex (H,W,4) uint8 RGBA (alpha 0 = cut-out), kind 'model' | 'sky')."""
import math, os, struct, sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n

S = 0.01
_NOTEX = np.full((2, 2, 4), 255, np.uint8)
_RGBA = None


def _rgba_lut():
    global _RGBA
    if _RGBA is None:
        import vram as V
        _RGBA = np.frombuffer(b"".join(V.RGBA), np.uint8).reshape(65536, 4)
    return _RGBA


class Mesh:
    def __init__(self, name, parts, glb=None, center=None, radius=1.0):
        self.name, self.parts, self.glb_fn, self.center, self.radius = name, parts, glb, center, radius

    @property
    def tris(self):
        return sum(len(p["idx"]) for p in self.parts if p["kind"] == "model")

    def glb(self):
        return self.glb_fn() if self.glb_fn else None


def build_part(v, pk):
    """Atlas of the used (tpage, clut) pairs (256 x 256 tiles, 8 per row, as room.build_glb) + vertex arrays of the polygons pk (room.parse_wmd_ex)."""
    tiles = {}
    for cl, tp, cmd, vs, cols in pk:
        if tp >> 7 & 3 < 2:
            tiles.setdefault((tp & 0x1FF, cl), len(tiles))
    ncol = 8
    AW, AH = ncol * 256, max(1, (len(tiles) + ncol - 1) // ncol) * 256
    atlas = np.zeros((AH, AW, 4), np.uint8)
    nib = np.frombuffer(v.nib, np.uint8).reshape(512, 4096)
    d8 = np.frombuffer(v.d, np.uint8).reshape(512, 2048)
    for (tp, cl), k in tiles.items():
        mode, pgx, ty = tp >> 7 & 3, (tp & 15) * 64, (tp >> 4 & 1) * 256
        ws = v.palette(cl, 16 if mode == 0 else 256)[2]
        pal = _rgba_lut()[np.array(ws, np.int64)]
        src = nib[ty:ty + 256, pgx * 4:pgx * 4 + 256] if mode == 0 else d8[ty:ty + 256, pgx * 2:pgx * 2 + 256]
        oy, ox = k // ncol * 256, k % ncol * 256
        atlas[oy:oy + src.shape[0], ox:ox + src.shape[1]] = pal[src]
    pos, uv, col, idx = [], [], [], []
    for cl, tp, cmd, vs, cols in pk:
        mode = tp >> 7 & 3
        k = tiles.get((tp & 0x1FF, cl)) if mode < 2 else None
        b = len(pos)
        for q, c in zip(vs, cols):
            pos.append((q[0] * S, -q[1] * S, -q[2] * S))
            uv.append((0.5, 0.5) if k is None else (k % ncol * 256 + q[3] + .5, k // ncol * 256 + q[4] + .5))
            col.append((min(1.0, c[0] / 127.5), min(1.0, c[1] / 127.5), min(1.0, c[2] / 127.5)))
        idx.append((b, b + 1, b + 2))
        if len(vs) == 4:
            idx.append((b + 1, b + 3, b + 2))
    return dict(kind="model", pos=np.array(pos, np.float32).reshape(-1, 3), uv=np.array(uv, np.float32).reshape(-1, 2),
                col=np.array(col, np.float32).reshape(-1, 3), idx=np.array(idx, np.int32).reshape(-1, 3), tex=atlas)


def sky_part(sky, model):
    """Backdrop quad behind the model (same placement as room.build_glb); the picture is mirror-tiled into one wide texture."""
    pos = model["pos"]
    x0, y0, z0 = pos.min(0)
    x1, y1, _ = pos.max(0)
    h = max((y1 - y0) * 1.3, 256 * S)
    tw = h * sky.width / sky.height
    nt = max(1, int(((x1 - x0) * 1.4 + tw) / tw) + 1)
    a = np.array(sky.convert("RGBA"), np.uint8)  # a writable copy (asarray of a PIL image is read-only)
    a[..., 3] = 255
    wide = np.concatenate([a if i % 2 == 0 else a[:, ::-1] for i in range(nt)], 1)
    cx, cy = (x0 + x1) / 2, (y0 + y1) / 2
    z = z0 - 0.5 * h * 0.1 - 10 * S
    W, H = wide.shape[1], wide.shape[0]
    p = [(cx - nt * tw / 2, cy + h / 2, z), (cx + nt * tw / 2, cy + h / 2, z), (cx - nt * tw / 2, cy - h / 2, z), (cx + nt * tw / 2, cy - h / 2, z)]
    return dict(kind="sky", pos=np.array(p, np.float32), uv=np.array([(.5, .5), (W - .5, .5), (.5, H - .5), (W - .5, H - .5)], np.float32),
                col=np.ones((4, 3), np.float32), idx=np.array([(0, 1, 2), (1, 3, 2)], np.int32), tex=np.ascontiguousarray(wide))


def _finish(name, v, pk, sky, glb):
    part = build_part(v, pk)
    parts = [part]
    if sky is not None and len(part["pos"]):
        parts.insert(0, sky_part(sky, part))
    p = part["pos"] if len(part["pos"]) else np.zeros((1, 3), np.float32)
    c = (p.min(0) + p.max(0)) / 2
    return Mesh(name, parts, glb, c, float(np.linalg.norm(p.max(0) - p.min(0)) / 2) or 1.0)


def ld_name(ld):
    return "AREA" + ld[2:] if ld.startswith("AR") else ld


def rooms():
    """[(ld, idx, wmd name)] of every load list with a WMD room model."""
    import area000 as A
    out = []
    for ld, lists in A.load_lists().items():
        for i, lst in enumerate(lists):
            r = [x for x in lst if x["kind"] == 0x50 and x["name"].startswith("AREA")]
            if r:
                out.append((ld, i, r[0]["name"]))
    return out


def load_room(ld, idx, sky=True):
    import area000 as A, vram as V, room as R
    lst = A.load_lists()[ld][idx]
    rec = [r for r in lst if r["kind"] == 0x50 and r["name"].startswith("AREA")][0]
    d = V.resource(rec["name"])
    v = V.area_context(ld, idx)
    pk = R.parse_wmd_ex(d)
    sp = R.find_sky(ld, idx) if sky else None
    sk = R.load_sky(sp) if sp else None
    return _finish(rec["name"].split("/")[-1].split(".")[0], v, pk, sk, lambda: R.room_glb(ld, idx)[1])


def has_sky(ld, idx):
    import room as R
    try:
        return bool(R.find_sky(ld, idx))
    except Exception:
        return False


def _obj_blob(ld, idx):
    """Bytes of the kind 0x51 table of load list idx: an own file, or a piece (fileId -1) following the file of the previous records."""
    import area000 as A, vram as V
    data, off = None, 0
    for r in A.load_lists()[ld][idx]:
        if r["type"] != 0xFFFFFFFF:
            data, off = V.resource(r["name"]), (r["arg"] if r["kind"] == 0x50 else 0)
            if r["kind"] == 0x51:
                return data
        elif data is not None:
            if r["kind"] == 0x51:
                return data[off:off + r["arg"]]
            off += r["arg"]
    return None


def objects():
    """[(ld, idx, count)] one entry per distinct kind 0x51 table (the first load list that has it)."""
    import area000 as A
    out, seen = [], set()
    for ld, lists in A.load_lists().items():
        for i, lst in enumerate(lists):
            if any(r["kind"] == 0x51 for r in lst):
                try:
                    b = _obj_blob(ld, i)
                    n = struct.unpack_from("<I", b)[0]
                    if not (0 < n < 1000) or b in seen:
                        continue
                    seen.add(b)
                    out.append((ld, i, n))
                except Exception:
                    pass
    return out


def load_object(ld, idx, k):
    import vram as V, room as R
    b = _obj_blob(ld, idx)
    n = struct.unpack_from("<I", b)[0]
    o = list(struct.unpack_from("<%dI" % n, b, 4)) + [len(b)]
    sub = b[o[k]:o[k + 1]]
    # an object model is one sector (4 groups of packets): wrap it as a 1-sector WMD
    pk = R.parse_wmd_ex(struct.pack("<II", 1, 8) + sub, lenient=True)
    v = V.area_context(ld, idx)
    return _finish("%s:%d #%d" % (ld, idx, k), v, pk, None, lambda: R.build_glb(v, pk))


# ---------------------------------------------------------------- camera
class Camera:
    def __init__(self):
        self.fov = 55.0
        self.reset(np.zeros(3), 1.0)

    def reset(self, center=None, radius=None):
        if center is not None:
            self.c0, self.r0 = np.array(center, np.float64), float(radius)
        self.target = self.c0.copy()
        self.yaw, self.pitch = math.radians(25), math.radians(15)
        self.dist = self.r0 / math.tan(math.radians(self.fov) / 2) * 0.85

    def eye(self):
        cp = math.cos(self.pitch)
        return self.target + self.dist * np.array([math.sin(self.yaw) * cp, math.sin(self.pitch), math.cos(self.yaw) * cp])

    def axes(self):
        f = self.target - self.eye()
        f /= np.linalg.norm(f)
        r = np.cross(f, [0, 1, 0])
        r /= np.linalg.norm(r)
        return f, r, np.cross(r, f)

    def view(self):
        f, r, u = self.axes()
        e = self.eye()
        m = np.eye(4)
        m[0, :3], m[1, :3], m[2, :3] = r, u, -f
        m[:3, 3] = -m[:3, :3] @ e
        return m

    def clip(self):
        return self.r0 * 0.005, self.r0 * 60

    def proj(self, aspect):
        n, f = self.clip()
        t = 1 / math.tan(math.radians(self.fov) / 2)
        m = np.zeros((4, 4))
        m[0, 0], m[1, 1], m[2, 2], m[2, 3], m[3, 2] = t / aspect, t, (f + n) / (n - f), 2 * f * n / (n - f), -1
        return m

    def orbit(self, dx, dy):
        self.yaw -= dx * 0.008
        self.pitch = max(-1.5, min(1.5, self.pitch + dy * 0.008))

    def pan(self, dx, dy, h):
        _, r, u = self.axes()
        k = 2 * self.dist * math.tan(math.radians(self.fov) / 2) / max(1, h)
        self.target = self.target - r * dx * k + u * dy * k

    def zoom(self, steps):
        self.dist = max(self.r0 * 0.02, min(self.r0 * 40, self.dist * 0.88 ** steps))

    def fly(self, fwd, right, up):
        f, r, u = self.axes()
        self.target = self.target + (f * fwd + r * right + u * up) * self.dist * 0.05


# ---------------------------------------------------------------- software rasterizer
def _project(part, cam, W, H):
    pos = part["pos"].astype(np.float64)
    c = np.c_[pos, np.ones(len(pos))] @ (cam.proj(W / H) @ cam.view()).T
    w = c[:, 3]
    ok = w > cam.clip()[0]
    ws = np.where(ok, w, 1.0)
    sx = (c[:, 0] / ws * .5 + .5) * W
    sy = (1 - (c[:, 1] / ws * .5 + .5)) * H
    return sx, sy, ws, ok


def _frags(part, sx, sy, w, tri, W, H, opts):
    """Fragments (pixel index, view depth, rgb float 0..1) of the triangles `tri` (T,3) vertex indices."""
    x, y = sx[tri], sy[tri]
    iw = 1.0 / w[tri]
    den = (y[:, 1] - y[:, 2]) * (x[:, 0] - x[:, 2]) + (x[:, 2] - x[:, 1]) * (y[:, 0] - y[:, 2])
    keep = np.abs(den) > 1e-9
    x0 = np.maximum(np.floor(x.min(1)), 0)
    x1 = np.minimum(np.ceil(x.max(1)), W - 1)
    y0 = np.maximum(np.floor(y.min(1)), 0)
    y1 = np.minimum(np.ceil(y.max(1)), H - 1)
    keep &= (x1 >= x0) & (y1 >= y0)
    sel = np.nonzero(keep)[0]
    out = []
    if not len(sel):
        return out
    bw, bh = (x1 - x0 + 1)[sel], (y1 - y0 + 1)[sel]
    size = np.maximum(bw, bh)
    tex, uv, col = (part["tex"] if opts.get("tex", True) else None), part["uv"], part["col"]
    edges = [0, 2, 4, 8, 16, 32]
    groups = [(sel[(size > a) & (size <= b)], b, b) for a, b in zip(edges[:-1], edges[1:])]
    for t in sel[size > 32]:
        groups.append((np.array([t]), int(bw[sel == t][0]), int(bh[sel == t][0])))
    for ids, gw, gh in groups:
        if not len(ids):
            continue
        step = max(1, (1 << 20) // (gw * gh))
        for s in range(0, len(ids), step):
            i = ids[s:s + step]
            gx = x0[i, None, None] + np.arange(gw)[None, None, :]
            gy = y0[i, None, None] + np.arange(gh)[None, :, None]
            gx, gy = np.broadcast_arrays(gx, gy)
            px, py = gx + .5, gy + .5
            xa, ya = x[i, :, None, None], y[i, :, None, None]
            d = den[i, None, None]
            l0 = ((ya[:, 1] - ya[:, 2]) * (px - xa[:, 2]) + (xa[:, 2] - xa[:, 1]) * (py - ya[:, 2])) / d
            l1 = ((ya[:, 2] - ya[:, 0]) * (px - xa[:, 2]) + (xa[:, 0] - xa[:, 2]) * (py - ya[:, 2])) / d
            l2 = 1 - l0 - l1
            m = (l0 >= -1e-6) & (l1 >= -1e-6) & (l2 >= -1e-6) & (gx <= x1[i, None, None]) & (gy <= y1[i, None, None])
            ti, yy, xx = np.nonzero(m)
            if not len(ti):
                continue
            T = i[ti]
            a0, a1, a2 = l0[ti, yy, xx] * iw[T, 0], l1[ti, yy, xx] * iw[T, 1], l2[ti, yy, xx] * iw[T, 2]
            su = a0 + a1 + a2
            b0, b1, b2 = a0 / su, a1 / su, a2 / su
            v = tri[T]
            rgb = np.ones((len(T), 3))
            if opts.get("vcol", True):
                rgb = b0[:, None] * col[v[:, 0]] + b1[:, None] * col[v[:, 1]] + b2[:, None] * col[v[:, 2]]
            keepf = np.ones(len(T), bool)
            if tex is not None:
                u = b0 * uv[v[:, 0], 0] + b1 * uv[v[:, 1], 0] + b2 * uv[v[:, 2], 0]
                vv = b0 * uv[v[:, 0], 1] + b1 * uv[v[:, 1], 1] + b2 * uv[v[:, 2], 1]
                tx = np.clip(u.astype(np.int32), 0, tex.shape[1] - 1)
                ty = np.clip(vv.astype(np.int32), 0, tex.shape[0] - 1)
                tc = tex[ty, tx]
                keepf = tc[:, 3] >= 128
                rgb = rgb * tc[:, :3] / 255.0
            else:
                rgb = rgb * 0.8
            pix = (gy[ti, yy, xx] * W + gx[ti, yy, xx]).astype(np.int64)
            out.append((pix[keepf], (1.0 / su)[keepf], rgb[keepf]))
    return out


def render(mesh, cam, W, H, opts=None):
    """RGB uint8 (H, W, 3).  opts: tex, vcol, wire, sky (bool), bg (r, g, b)."""
    opts = opts or {}
    bg = np.array(opts.get("bg", (40, 40, 56)), np.float64) / 255
    img = np.empty((H * W, 3))
    img[:] = bg
    zbuf = np.full(H * W, np.inf)
    wire = opts.get("wire", False)
    for part in mesh.parts:
        if part["kind"] == "sky" and (not opts.get("sky", True) or wire):
            continue
        sx, sy, w, ok = _project(part, cam, W, H)
        idx = part["idx"]
        idx = idx[ok[idx].all(1)]
        if not len(idx):
            continue
        if wire:
            e = np.concatenate([idx[:, [0, 1]], idx[:, [1, 2]], idx[:, [2, 0]]])
            e = e[e[:, 0] < e[:, 1]] if False else e
            a, b = np.c_[sx[e[:, 0]], sy[e[:, 0]]], np.c_[sx[e[:, 1]], sy[e[:, 1]]]
            n = np.clip(np.ceil(np.abs(b - a).max(1)).astype(np.int64) + 1, 1, 1500)
            seg = np.repeat(np.arange(len(e)), n)
            tt = (np.arange(len(seg)) - np.repeat(np.cumsum(n) - n, n)) / np.maximum(np.repeat(n - 1, n), 1)
            p = a[seg] + (b[seg] - a[seg]) * tt[:, None]
            xi, yi = p[:, 0].astype(np.int64), p[:, 1].astype(np.int64)
            m = (xi >= 0) & (xi < W) & (yi >= 0) & (yi < H)
            img[yi[m] * W + xi[m]] = (.75, .9, 1.0)
            continue
        fr = _frags(part, sx, sy, w, idx, W, H, opts)
        if not fr:
            continue
        pix = np.concatenate([f[0] for f in fr])
        dep = np.concatenate([f[1] for f in fr])
        rgb = np.concatenate([f[2] for f in fr])
        o = np.lexsort((dep, pix))
        pix, dep, rgb = pix[o], dep[o], rgb[o]
        first = np.r_[True, pix[1:] != pix[:-1]]
        pix, dep, rgb = pix[first], dep[first], rgb[first]
        m = dep < zbuf[pix]
        zbuf[pix[m]] = dep[m]
        img[pix[m]] = rgb[m]
    return (np.clip(img, 0, 1) * 255 + .5).astype(np.uint8).reshape(H, W, 3)


i18n.add({
    "es": {"t.models": "Modelos 3D", "t.m.rooms": "Salas", "t.m.objs": "Objetos", "m.tex": "Texturas", "m.wire": "Malla", "m.vcol": "Color de vértice",
           "m.sky": "Cielo", "m.bg": "Fondo...", "m.glb": "Exportar .glb", "m.shot": "Captura PNG", "m.reset": "Reiniciar vista",
           "m.hint": "Arrastrar: orbitar | Botón der./central: desplazar | Rueda: zoom | Doble clic: reiniciar | WASD/QE: volar",
           "m.info": "{0}: {1} triángulos ({2})", "m.savedglb": "Exportado {0}", "m.savedshot": "Captura guardada: {0}", "m.noglb": "Sin datos para exportar",
           "m.gl": "OpenGL", "m.soft": "software", "m.obj": "Objeto {0}", "m.set": "Tabla {0} lista {1} ({2} modelos)"},
    "en": {"t.models": "3D models", "t.m.rooms": "Rooms", "t.m.objs": "Objects", "m.tex": "Textures", "m.wire": "Wireframe", "m.vcol": "Vertex colours",
           "m.sky": "Sky", "m.bg": "Background...", "m.glb": "Export .glb", "m.shot": "Screenshot PNG", "m.reset": "Reset view",
           "m.hint": "Drag: orbit | Right/middle drag: pan | Wheel: zoom | Double click: reset | WASD/QE: fly",
           "m.info": "{0}: {1} triangles ({2})", "m.savedglb": "Exported {0}", "m.savedshot": "Screenshot saved: {0}", "m.noglb": "Nothing to export",
           "m.gl": "OpenGL", "m.soft": "software", "m.obj": "Object {0}", "m.set": "Table {0} list {1} ({2} models)"},
})


def main(argv=None):
    a = list(argv if argv is not None else sys.argv[1:])
    if len(a) < 3 or a[0] not in ("room", "obj"):
        print(__doc__)
        return 1
    p = a[1].split(":")
    ld, idx = p[0], int(p[1])
    mesh = load_room(ld, idx) if a[0] == "room" else load_object(ld, idx, int(p[2]))
    cam = Camera()
    cam.reset(mesh.center, mesh.radius)
    if len(a) > 4:
        cam.yaw, cam.pitch = math.radians(float(a[3])), math.radians(float(a[4]))
    W, H = (int(a[5]), int(a[6])) if len(a) > 6 else (640, 480)
    import time
    t = time.time()
    im = render(mesh, cam, W, H)
    print("%d tris, %.2fs" % (mesh.tris, time.time() - t))
    from PIL import Image
    Image.fromarray(im).save(a[2])
    return 0


if __name__ == "__main__":
    sys.exit(main())
