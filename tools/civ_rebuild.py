#!/usr/bin/env python3
"""Best-effort reconstruction of the lost background pictures (kind 0x8F) and cell maps (kind 0x8E) of the cut
Civilization top-down towns: AREA11 s0 (Lumberjack Town), AREA16 s0 (Stonetown), AREA17 s0 (Iron Town).

RECONSTRUCTION, NOT ORIGINAL. See docs/CIVILIZATION.md, "Reconstruction (not original)". In short:

  real   the 8 bpp scenery cut-outs of the SYS reload pages (SYSTEM/A06100, A07600/01, A08000), with their real
         indices; the house rectangles, residents, patrol boxes and exit rectangles, read from I0NFO2 / the code.
  ours   where every cut-out goes, the ground fill, the collision, and the colours. The CLUTs are FMV-derived:
         community renders of the pages (if you have them, --ref) and MOVIE/BUNMEI.STR, filled by harmonic
         interpolation over the page's index adjacency. They are not the lost original palettes.
  FMV    by default the median mosaic of the BUNMEI.STR flyover of each town is laid under the cut-outs where the
         flyover shows the town (re-indexed to the town CLUT). --no-fmv gives the cut-outs-only picture.

Nothing from the disc is stored in the repository: the layout file tools/civ_rebuild_layout.json holds only
coordinates, rectangles and the reasons for each choice. Every pixel comes from your own disc at run time.

  python3 tools/civ_rebuild.py                       # all three towns -> build/civ_rebuild/AREAnn/
  python3 tools/civ_rebuild.py --area 17 [--no-fmv | --both] [--refdir DIR | --ref A08000=125.png ...]
  python3 tools/civ_rebuild.py --area 17 --preview DIR    # also copy the previews to DIR

Output per town (build/civ_rebuild/AREAnn/):
  map8F.tim     the picture as the engine reads kind 0x8F: 8 bpp TIM, CLUT at +0x14, pixels at +0x220,
                width W px = 8 * cell-map width, height H px (docs "Map picture (kind 0x8F)")
  map8E.bin     the cell map as kind 0x8E: u16 w, u16 h (cells of 8 x 8 px), then 0x4000 bytes, row-major
  picture.png   the same picture as an indexed PNG          cells.png   the cell map tinted over the picture
  preview.png   picture + houses, residents, patrol boxes, props and exits (world (x, y) = pixel (x+128, y-40))
  source.png    which layer each pixel came from (grey ground, blue FMV, green cut-out, yellow stamp)
  manifest.json sizes, CLUT provenance, the warnings of the cell-map checks
Needs numpy and Pillow, the extracted disc (game/disc/fs or TOMBI_FS) and the image (game/disc/Tombi.bin or TOMBI_IMG).
"""
import argparse, importlib, json, os, struct, sys

import numpy as np
from PIL import Image, ImageDraw

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import area000 as A  # noqa: E402
import topdown as T  # noqa: E402

LAYOUT = os.path.join(HERE, "civ_rebuild_layout.json")
OUT = os.path.join(ROOT, "build", "civ_rebuild")
NPC_VAR = {11: 0x0D, 16: 0x0E, 17: 0x0F}
# community clutgen pairs (CLUT*.png = page pieces drawn with GGDdefault.act at 2x, PHOT*.png = the matching
# FMV frame at 2x): which page each pair shows, identified by us from the pictures
CLUTGEN = {"CLUT.png": ("PHOT.png", "A06100"), "CLUT-2.png": ("PHOT-2.png", "A08000"), "CLUT-4.png": ("PHOT-4.png", "A08000"),
           "CLUT-3.png": ("PHOT-3.png", "A07600"), "CLUT-X.png": ("PHOT-X.png", "A08600")}
REFDIR = {"125.png": "A08000", "127.png": "A08600", "128.png": "A06100"}


def img_path():
    return A.IMG or os.path.join(os.path.dirname(A.FS), "Tombi.bin")


# ---------------------------------------------------------------- pages
def page(name):
    """SYS reload page as a (256, width) index array (one byte per pixel, 8 bpp view)."""
    d = A.read_resource("SYSTEM/" + name)
    return np.frombuffer(d, np.uint8).reshape(256, -1)


def page8(name, L):
    """The 8 bpp part of a page: the first `skip_x` bytes of some pages hold 4 bpp sprites."""
    p = page(name).copy()
    p[:, :L["pages"].get(name, {}).get("skip_x", 0)] = 0
    return p


# ---------------------------------------------------------------- FMV mosaics
def _shift(a, b):
    """Integer translation between two frames by phase correlation."""
    R = np.fft.fft2(a - a.mean()) * np.conj(np.fft.fft2(b - b.mean()))
    r = np.real(np.fft.ifft2(R / (np.abs(R) + 1e-9)))
    y, x = np.unravel_index(np.argmax(r), r.shape)
    H, W = a.shape
    return (y - H if y > H // 2 else y), (x - W if x > W // 2 else x)


def mosaic(movie, f0, f1, cache):
    """Median mosaic of frames f0..f1 (a camera pan at the page scale): sparkles and walkers drop out."""
    fn = os.path.join(cache, "%s_%d_%d.png" % (movie, f0, f1))
    if os.path.exists(fn):
        return np.array(Image.open(fn).convert("RGB"))
    S = importlib.import_module("str")
    img = img_path()
    lba, size = S.str_files(img)["MOVIE/%s.STR" % movie]
    fr = []
    for num, w, h, bs in S.frames(img, lba, size):
        if num > f1:
            break
        if num >= f0:
            fr.append(np.frombuffer(S.decode_frame(bs, w, h), np.uint8).reshape(h, w, 3).astype(np.float32))
    pos = [(0, 0)]
    for a, b in zip(fr, fr[1:]):
        dy, dx = _shift(a.mean(2), b.mean(2))
        pos.append((pos[-1][0] + dy, pos[-1][1] + dx))
    y0 = min(p[0] for p in pos); x0 = min(p[1] for p in pos)
    h, w = fr[0].shape[:2]
    H = max(p[0] for p in pos) - y0 + h; W = max(p[1] for p in pos) - x0 + w
    st = np.full((len(fr), H, W, 3), np.nan, np.float32)
    for k, (F, (y, x)) in enumerate(zip(fr, pos)):
        st[k, y - y0:y - y0 + h, x - x0:x - x0 + w] = F
    with np.errstate(all="ignore"):
        import warnings
        warnings.simplefilter("ignore")
        m = np.nan_to_num(np.nanmedian(st, 0)).astype(np.uint8)
    os.makedirs(cache, exist_ok=True)
    Image.fromarray(m).save(fn)
    return m


# ---------------------------------------------------------------- CLUT (FMV-derived, not original)
def cooc(D):
    """How often index a touches index b on the page (4-neighbourhood, index 0 = transparent excluded)."""
    W = np.zeros((256, 256))
    for a, b in ((D[:, :-1], D[:, 1:]), (D[:-1, :], D[1:, :])):
        a = a.ravel().astype(int); b = b.ravel().astype(int); k = (a > 0) & (b > 0) & (a != b)
        np.add.at(W, (a[k], b[k]), 1)
    return W + W.T


def harmonic(W, anchors):
    """Colours of the indices without an anchor: the solution of the Dirichlet problem on the adjacency graph
    (each colour is the weighted mean of its neighbours'). Tested on A08000/A06100/A08600 with 30 % of the community
    colours as anchors: mean error 18-26 per channel on the rest."""
    P = np.zeros((256, 3)); known = np.zeros(256, bool)
    for i, c in anchors.items():
        P[i] = c; known[i] = True
    U = [i for i in range(1, 256) if not known[i] and W[i].sum() > 0]
    if U and known.any():
        L = np.diag(W.sum(1)) - W
        P[U] = np.linalg.lstsq(L[np.ix_(U, U)], -L[np.ix_(U, np.nonzero(known)[0])] @ P[known], rcond=None)[0]
    return np.clip(P, 0, 255)


def clutgen_samples(cdir):
    """Per page group: {index: mean colour} by the community clutgen method: each 2x2 block of PHOT whose CLUT pixel
    is opaque gives one sample (the block mean) to the index whose GGDdefault.act colour that CLUT pixel has."""
    g = np.frombuffer(open(os.path.join(cdir, "GGDdefault.act"), "rb").read()[:768], np.uint8).reshape(256, 3).astype(int)
    key = {tuple(c): i for i, c in enumerate(g)}
    out = {}
    for cf, (pf, pg_) in CLUTGEN.items():
        if not os.path.exists(os.path.join(cdir, cf)):
            continue
        C = np.array(Image.open(os.path.join(cdir, cf)).convert("RGBA")).astype(int)
        Ph = np.array(Image.open(os.path.join(cdir, pf)).convert("RGBA")).astype(float)
        h, w = (min(C.shape[0], Ph.shape[0]) // 2) * 2, (min(C.shape[1], Ph.shape[1]) // 2) * 2
        blk = Ph[:h, :w].reshape(h // 2, 2, w // 2, 2, 4)
        ok = (blk[..., 3] > 0).all((1, 3)) & (C[:h:2, :w:2, 3] > 0)
        mean = blk[..., :3].mean((1, 3))
        acc = out.setdefault(pg_, {})
        for (y, x) in zip(*np.nonzero(ok)):
            i = key.get(tuple(C[2 * y, 2 * x, :3]))
            if i:
                acc.setdefault(i, []).append(mean[y, x])
    return {pg_: {i: np.mean(v, 0) for i, v in d.items() if len(v) >= 2} for pg_, d in out.items()}


COMMUNITY = {}


def derive_clut(name, L, refs, cache):
    """256 colours for page `name`. Sources, in order of trust:
    'ref'  the community render of the page (index -> colour, exact pixel correspondence), if given with --ref;
    'fmv'  the BUNMEI.STR mosaic sampled where a patch of the page was registered (layout 'fmv_anchors');
    'fill' harmonic interpolation from the two above over the page's index adjacency.
    Index 0 stays transparent black."""
    group = L["pages"].get(name, {}).get("clut_group", [name])  # pages that share one CLUT (one VRAM page split)
    Ds = {n: page8(n, L) for n in group}
    used = np.zeros(256, bool)
    for D in Ds.values():
        used |= np.bincount(D.ravel(), minlength=256) > 0
    used[0] = False
    ref = {}
    for n, D in Ds.items():
        if n not in refs:
            continue
        R = np.array(Image.open(refs[n]).convert("RGB")).astype(float)
        if R.shape[:2] != D.shape:
            sys.exit("--ref %s: %s is %dx%d, the page is %dx%d" % (n, refs[n], R.shape[1], R.shape[0], D.shape[1], D.shape[0]))
        for i in np.unique(D[D > 0]):
            ref.setdefault(int(i), np.median(R[D == i], 0))
    samp = [[] for _ in range(256)]
    for n, D in Ds.items():
        for x, y, k, mos, dx, dy in L["fmv_anchors"].get(n, []):
            m = mosaics(L, cache)[mos]
            P = D[y:y + k, x:x + k]
            v, u = np.nonzero(P)
            Y, X = v + dy, u + dx
            ok = (Y >= 0) & (Y < m.shape[0]) & (X >= 0) & (X < m.shape[1])
            for i, c in zip(P[v[ok], u[ok]], m[Y[ok], X[ok]]):
                samp[i].append(c)
    fmv = {i: np.median(np.array(s), 0) for i, s in enumerate(samp) if len(s) >= 3 and i}
    comm = {}
    for n in group:
        comm.update(COMMUNITY.get(n, {}))
    anchors = dict(fmv); anchors.update(ref); anchors.update(comm)
    P = harmonic(sum(cooc(D) for D in Ds.values()), anchors) if anchors else np.zeros((256, 3))
    src = ["-"] * 256
    for i in range(1, 256):
        if i in comm:
            P[i] = comm[i]; src[i] = "clutgen"
        elif i in ref:
            P[i] = ref[i]; src[i] = "ref"
        elif i in fmv:
            P[i] = fmv[i]; src[i] = "fmv"
        elif used[i]:
            src[i] = "fill"
    both = [i for i in ref if i in fmv]
    cr = [i for i in comm if i in ref and used[i]]
    stats = dict(pages=group, used=int(used.sum()), clutgen=len([i for i in comm if used[i]]),
                 clutgen_vs_ref_mean_abs=round(float(np.mean([np.abs(comm[i] - ref[i]).mean() for i in cr])), 1) if cr else None,
                 ref=len(ref), fmv=len(fmv), fill=src.count("fill"),
                 ref_vs_fmv_mean_abs=round(float(np.mean([np.abs(ref[i] - fmv[i]).mean() for i in both])), 1) if both else None)
    raw = np.clip(np.round(P), 0, 255)
    Q = quantize15(raw)
    d = np.abs(Q.astype(int) - raw)[used]
    words = [rgb555(Q[i]) if i else 0 for i in range(256)]
    uw = [words[i] for i in range(1, 256) if used[i]]
    stats.update(quantized="PS1 15-bit, round(c*31/255)", quant_max_change=int(d.max()) if d.size else 0,
                 quant_mean_change=round(float(d.mean()), 2) if d.size else 0.0,
                 quant_collapsed=len(uw) - len(set(uw)), clut15=["%04X" % w for w in words])
    return Q, src, stats


_MOS = {}


def mosaics(L, cache):
    if not _MOS:
        for k, (movie, f0, f1) in L["mosaics"].items():
            _MOS[k] = mosaic(movie, f0, f1, cache)
    return _MOS


# ---------------------------------------------------------------- composition
def nearest(rgb, pal, allowed):
    """Index (from `allowed`) of the closest palette colour for each RGB pixel."""
    cand = pal[allowed].astype(np.int32)
    flat = rgb.reshape(-1, 3).astype(np.int32)
    out = np.empty(len(flat), np.uint8)
    for s in range(0, len(flat), 65536):
        d = ((flat[s:s + 65536, None, :] - cand[None]) ** 2).sum(2)
        out[s:s + 65536] = np.asarray(allowed)[d.argmin(1)]
    return out.reshape(rgb.shape[:2])


def compose(area, L, T_, pal, cache, use_fmv=True):
    W, H = T_["size"]
    can = np.zeros((H, W), np.uint8)
    src = np.zeros((H, W), np.uint8)  # 1 ground, 2 FMV, 3 cut-out, 4 FMV stamp
    allowed = [i for i in range(1, 256) if pal[i].any() or i in T_.get("black_ok", [])] or [1]
    pages = {}

    def pg(n):
        if n not in pages:
            pages[n] = page8(n, L)
        return pages[n]

    def remap(n):
        """Indices of page n in the town CLUT: identity for the town's own page, nearest colour otherwise."""
        if n in L["pages"].get(T_["page"], {}).get("clut_group", [T_["page"]]):
            return np.arange(256, dtype=np.uint8)
        P2, _, _ = derive_clut(n, L, {}, cache)
        r = nearest(P2[None], pal, allowed)[0]; r[0] = 0
        return r

    rng = np.random.default_rng(T_.get("seed", 1))
    fmv_tiles = {}

    def tile_fill(tset, x0, y0, w, h):
        tiles = L["tilesets"][tset]
        if "mosaic" in tiles:  # tiles cut from the FMV mosaic (re-indexed); without the FMV, the fallback tileset
            if not use_fmv:
                return tile_fill(tiles["nofmv"], x0, y0, w, h)
            if tset not in fmv_tiles:
                fmv_tiles[tset] = nearest(mosaics(L, cache)[tiles["mosaic"]], pal, allowed)
            P = fmv_tiles[tset]
        else:
            P = pg(tiles["page"])
        tw, th = tiles["size"]
        for ty in range(y0, y0 + h, th):
            for tx in range(x0, x0 + w, tw):
                sx, sy = tiles["at"][rng.integers(len(tiles["at"]))]
                t = P[sy:sy + th, sx:sx + tw][:max(0, min(th, y0 + h - ty)), :max(0, min(tw, x0 + w - tx))]
                yy, xx = max(0, ty), max(0, tx)
                t = t[yy - ty:, xx - tx:][:H - yy, :W - xx]
                can[yy:yy + t.shape[0], xx:xx + t.shape[1]] = t
                src[yy:yy + t.shape[0], xx:xx + t.shape[1]] = 1

    for g in T_["ground"]:
        tile_fill(g["tiles"], *g["rect"])
    if use_fmv:
        for f in T_.get("fmv", []):
            m = mosaics(L, cache)[f["mosaic"]]
            sx, sy, w, h = f["crop"]
            ax, ay = f["at"]
            part = m[sy:sy + h, sx:sx + w]
            keep = part.sum(2) > 0
            for ex in f.get("erase", []):  # mosaic rects left to the ground (baked-in people, frame edges)
                keep[max(0, ex[1] - sy):ex[1] - sy + ex[3], max(0, ex[0] - sx):ex[0] - sx + ex[2]] = False
            idx = nearest(part, pal, allowed)
            y0, x0 = max(0, ay), max(0, ax)
            y1, x1 = min(H, ay + h), min(W, ax + w)
            k = keep[y0 - ay:y1 - ay, x0 - ax:x1 - ax]
            can[y0:y1, x0:x1][k] = idx[y0 - ay:y1 - ay, x0 - ax:x1 - ax][k]
            src[y0:y1, x0:x1][k] = 2
    place = list(T_["place"])
    # houses: roof tiles over the trigger rectangle, a real wall cut-out tiled along its front, a door (our design)
    for i, r in enumerate(T.trigger_rects(area)):
        dw = T_["houses"][i].get("draw")
        if not dw:
            continue
        x0, y0, x1, y1 = world_rect_px(r)
        wh = dw["wall_h"]
        tile_fill(dw["roof"], x0, y0, x1 - x0 + 1, y1 - y0 + 1 - wh)
        pn, sx, sy, pw, ph = L["pieces"][dw["wall"]]
        for wx in range(x0, x1 + 1, pw):
            place.insert(0, ["@%s:%d:%d:%d:%d:%d" % (pn, sx, sy, min(pw, x1 + 1 - wx), ph, 0), wx, y1 + 1 - ph])
        door = dw.get("door") if use_fmv else dw.get("door_nofmv", dw.get("door"))
        if door and not (door.startswith("fmv:") and not use_fmv):
            dx0, dwid = T_["houses"][i]["door"]
            w_, h_ = (L["fmv_stamps"][door[4:]] if door.startswith("fmv:") else L["pieces"][door])[3:5]
            place.append([door, dx0 + dwid // 2 - w_ // 2, y1 + 1 - h_ + dw.get("door_dy", 0)])
    for p in place:
        name, ax, ay = p[0], p[1], p[2]
        note = p[3] if len(p) > 3 and isinstance(p[3], str) else ""
        if (note.startswith("[nofmv]") and use_fmv) or (note.startswith("[fmv]") and not use_fmv):
            continue
        if name.startswith("@"):  # internal: a cropped piece "@page:x:y:w:h:flip"
            pn, sx, sy, w, h, _ = name[1:].split(":")
            sx, sy, w, h = int(sx), int(sy), int(w), int(h)
            spr, kind = remap(pn)[pg(pn)[sy:sy + h, sx:sx + w]], 3
        elif name.startswith("fmv:"):
            if not use_fmv:
                continue
            mk, sx, sy, w, h = L["fmv_stamps"][name[4:]]
            idx = nearest(mosaics(L, cache)[mk][sy:sy + h, sx:sx + w], pal, allowed)
            spr, kind = idx, 4
        else:
            pn, sx, sy, w, h = L["pieces"][name]
            spr, kind = remap(pn)[pg(pn)[sy:sy + h, sx:sx + w]], 3
        if "flip" in note.split(":")[0]:
            spr = spr[:, ::-1]
        h, w = spr.shape
        y0, x0 = max(0, ay), max(0, ax)
        y1, x1 = min(H, ay + h), min(W, ax + w)
        if y1 <= y0 or x1 <= x0:
            continue
        s = spr[y0 - ay:y1 - ay, x0 - ax:x1 - ax]
        k = s > 0
        can[y0:y1, x0:x1][k] = s[k]
        src[y0:y1, x0:x1][k] = kind
    hole = can == 0
    if hole.any():  # nothing may stay transparent: give holes the most common ground index
        vals = can[~hole]
        can[hole] = np.bincount(vals, minlength=256).argmax() if vals.size else 1
    return can, src


# ---------------------------------------------------------------- cell map
def world_rect_px(r):
    (x0, y0), (x1, y1) = T.world_to_px(r[0], r[1]), T.world_to_px(r[2], r[3])
    return min(x0, x1), min(y0, y1), max(x0, x1), max(y0, y1)


def patrol_box(var, a):
    x0, y0, x1, y1 = a["bx0"], a["by0"], a["bx1"], a["by1"]
    if var == 0x0E:  # set 0E stores the box relative to the start point (docs, topdown.draw_npcs)
        x0, y0, x1, y1 = a["x"] + x0, a["y"] + y0, a["x"] + x1, a["y"] + y1
    return min(x0, x1), min(y0, y1), max(x0, x1), max(y0, y1)


def cellmap(area, T_):
    """Our collision: 9 = solid block, 0 = floor, 0x80 = trigger floor (house interiors and doors).
    The working sections also use the diagonal and one-sided walls (1-8); we use only 9."""
    W, H = T_["size"]
    cw, ch = W // 8, H // 8
    c = np.zeros((ch, cw), np.uint8)
    warn = []

    def cells(x0, y0, x1, y1):  # px rect (inclusive) -> cell slice, clipped
        return slice(max(0, y0 // 8), max(0, min(ch, y1 // 8 + 1))), slice(max(0, x0 // 8), max(0, min(cw, x1 // 8 + 1)))

    c[0, :] = c[-1, :] = 9
    c[:, 0] = c[:, -1] = 9
    for r in T_.get("solid", []):
        c[cells(r[0], r[1], r[0] + r[2] - 1, r[1] + r[3] - 1)] = 9
    rects = T.trigger_rects(area)
    for i, r in enumerate(rects):
        x0, y0, x1, y1 = world_rect_px(r)
        hs = T_["houses"][i]
        c[cells(x0, y0, x1, y1)] = 9
        ix0, iy0, ix1, iy1 = hs.get("inside", [x0 + 8, y0 + 8, x1 - 8, y1 - 8])
        c[cells(ix0, iy0, ix1, iy1)] = 0x80
        dx0, dw = hs["door"]
        side = hs.get("side", "S")
        if side == "S":
            c[cells(dx0, iy1, dx0 + dw - 1, y1 + 8)] = 0x80
        elif side == "N":
            c[cells(dx0, y0 - 8, dx0 + dw - 1, iy0)] = 0x80
        elif side == "E":
            c[cells(ix1, dx0, x1 + 8, dx0 + dw - 1)] = 0x80
        else:
            c[cells(x0 - 8, dx0, ix0, dx0 + dw - 1)] = 0x80
    for e in T_["exits"]:
        x0, y0, x1, y1 = world_rect_px(e["world"])
        c[cells(max(0, x0), max(0, y0), min(W - 1, x1), min(H - 1, y1))] = 0
    for r in T_.get("free", []):
        c[cells(r[0], r[1], r[0] + r[2] - 1, r[1] + r[3] - 1)] &= 0x80
    # the residents must stand and walk on floor: carve, and report, every solid cell under them
    var = NPC_VAR[area]
    actors, _ = T.npc_set(var)
    for n, a in enumerate(actors, 1):
        boxes = [(a["x"], a["y"], a["x"], a["y"])]
        if a["beh"]:
            boxes.append(patrol_box(var, a))
        for bx0, by0, bx1, by1 in boxes:
            (px0, py0), (px1, py1) = T.world_to_px(bx0, by0), T.world_to_px(bx1, by1)
            sl = cells(px0 - 4, py0 - 4, px1 + 4, py1 + 4)
            bad = c[sl] & 0x7F
            if bad.any():
                warn.append("resident %d (%d,%d) room %d: %d solid cells carved" % (n, a["x"], a["y"], a["room"], int((bad > 0).sum())))
                c[sl] = np.where(bad > 0, 0x80 if a["room"] else 0, c[sl])
            if a["room"] and not (c[sl] & 0x80).all():
                warn.append("resident %d (%d,%d) is indoors (room %d) but stands on non-trigger floor" % (n, a["x"], a["y"], a["room"]))
    return c, warn


# ---------------------------------------------------------------- output
def q5(c):
    """Nearest PS1 5-bit level of an 8-bit channel (same rule as tools/clut_from_sprite.py)."""
    return int(round(float(c) * 31 / 255))


def quantize15(P):
    """Each entry to the nearest 15-bit colour, shown back as round(v*255/31)."""
    return np.array([[round(q5(c) * 255 / 31) for c in e] for e in P], np.uint8)


def rgb555(c):
    v = q5(c[0]) | q5(c[1]) << 5 | q5(c[2]) << 10
    return v or 0x8000  # 0x0000 is transparent on the PS1: an opaque black needs the STP bit


def tim8(pix, pal):
    H, W = pix.shape
    clut = struct.pack("<256H", 0, *[rgb555(c) for c in pal[1:]])
    return (struct.pack("<II", 0x10, 9) + struct.pack("<IHHHH", 12 + 512, 0, 0, 256, 1) + clut +
            struct.pack("<IHHHH", 12 + W * H, 0, 0, W // 2, H) + pix.tobytes())


def cells8e(c):
    ch, cw = c.shape
    b = c.tobytes()
    return struct.pack("<HH", cw, ch) + b + bytes(0x4000 - len(b))


def font(n):
    for f in ("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", "DejaVuSans-Bold.ttf"):
        try:
            from PIL import ImageFont
            return ImageFont.truetype(f, n)
        except OSError:
            pass
    from PIL import ImageFont
    return ImageFont.load_default()


def overlay(area, T_, rgb):
    im = Image.fromarray(rgb).convert("RGB")
    dr = ImageDraw.Draw(im)
    f = font(11)
    for i, r in enumerate(T.trigger_rects(area)):
        x0, y0, x1, y1 = world_rect_px(r)
        dr.rectangle([x0, y0, x1, y1], outline=(80, 160, 255), width=2)
        dr.text((max(2, x0 + 3), y0 + 2), "H%d" % (i + 1), fill=(80, 160, 255), font=f)
    for k, e in enumerate(T_["exits"]):
        x0, y0, x1, y1 = world_rect_px(e["world"])
        x0, y0 = max(0, x0), max(0, y0)
        x1, y1 = min(im.width - 1, x1), min(im.height - 1, y1)
        dr.rectangle([x0, y0, x1, y1], outline=(255, 140, 0), width=2)
        dr.text((x0 + 3, y0 + 2), "exit %d: %s" % (k, e["to"]), fill=(255, 140, 0), font=f)
    var = NPC_VAR[area]
    actors, props = T.npc_set(var)
    for lst in props:
        for p in lst:
            x, y = T.world_to_px(p["x"], p["y"])
            dr.rectangle([x - 3, y - 3, x + 3, y + 3], outline=(255, 220, 0))
    for n, a in enumerate(actors, 1):
        if a["beh"]:
            x0, y0, x1, y1 = patrol_box(var, a)
            (px0, py0), (px1, py1) = T.world_to_px(x0, y0), T.world_to_px(x1, y1)
            dr.rectangle([px0, py0, px1, py1], outline=(0, 255, 0))
        x, y = T.world_to_px(a["x"], a["y"])
        col = (255, 40, 40) if a["room"] == 0 else (255, 0, 255)
        dr.ellipse([x - 5, y - 5, x + 5, y + 5], fill=col, outline=(0, 0, 0))
        dr.text((x + 7, y - 7), str(n), fill=(255, 255, 255), font=f, stroke_width=2, stroke_fill=(0, 0, 0))
    return im


def render_interiors(area, L, refs, T_, rgb, out, cache):
    """Interior view (each house's roof gone and its matched room drawn centred in the trigger rectangle, clipped to
    it, as when Tomba is inside; the residents with room n are marked) and a contact sheet (exterior | interior)."""
    os.makedirs(out, exist_ok=True)
    view = Image.fromarray(rgb).convert("RGB")
    sheet_rows = []
    rects = T.trigger_rects(area)
    f = font(12)
    for it in L.get("interiors", {}).get(str(area), []):
        pn, sx, sy, w, h = it["piece"]
        pal, _, _ = derive_clut(pn, L, refs, cache)
        P = page(pn)[sy:sy + h, sx:sx + w]
        room = Image.fromarray(pal[P]).convert("RGBA")
        room.putalpha(Image.fromarray(((P > 0) * 255).astype(np.uint8)))
        x0, y0, x1, y1 = world_rect_px(rects[it["house"] - 1])
        ext = view.crop((x0 - 16, y0 - 16, x1 + 16, y1 + 16))
        box = Image.new("RGBA", (x1 - x0 + 1, y1 - y0 + 1), (0, 0, 0, 255))
        box.alpha_composite(room, ((box.width - w) // 2, (box.height - h) // 2)) if w <= box.width and h <= box.height else \
            box.alpha_composite(room.crop(((w - box.width) // 2 if w > box.width else 0, (h - box.height) // 2 if h > box.height else 0,
                                           (w - box.width) // 2 + box.width if w > box.width else w,
                                           (h - box.height) // 2 + box.height if h > box.height else h)),
                                (max(0, (box.width - w) // 2), max(0, (box.height - h) // 2)))
        sheet_rows.append((it, ext, room))
        view.paste(box.convert("RGB"), (x0, y0))
    dr = ImageDraw.Draw(view)
    for i, r in enumerate(rects):
        x0, y0, x1, y1 = world_rect_px(r)
        dr.rectangle([x0, y0, x1, y1], outline=(80, 160, 255), width=2)
        dr.text((max(2, x0 + 3), y0 + 2), "H%d" % (i + 1), fill=(80, 160, 255), font=f, stroke_width=2, stroke_fill=(0, 0, 0))
    actors, _ = T.npc_set(NPC_VAR[area])
    for n, a in enumerate(actors, 1):
        if a["room"]:
            x, y = T.world_to_px(a["x"], a["y"])
            dr.ellipse([x - 5, y - 5, x + 5, y + 5], fill=(255, 0, 255), outline=(0, 0, 0))
            dr.text((x + 7, y - 7), "%d" % n, fill=(255, 255, 255), font=f, stroke_width=2, stroke_fill=(0, 0, 0))
    view.save(os.path.join(out, "a%02d_interior_view.png" % area))
    if sheet_rows:
        W = max(e.width + r.width for _, e, r in sheet_rows) + 60
        H = sum(max(e.height, r.height) + 70 for _, e, r in sheet_rows)
        S = Image.new("RGB", (max(W, 700), H), (30, 30, 30)); d = ImageDraw.Draw(S); y = 0
        for it, e, r in sheet_rows:
            d.text((8, y + 4), "%s H%d -> %s %s  [%s]  (reconstruction, our guess)" % (T_["name"], it["house"], it["piece"][0],
                   tuple(it["piece"][1:]), it["conf"]), fill=(255, 255, 255), font=f)
            d.text((8, y + 22), it["why"][:150], fill=(190, 190, 190), font=font(10))
            S.paste(e, (8, y + 44)); S.paste(r, (e.width + 40, y + 44), r)
            y += max(e.height, r.height) + 70
        S.save(os.path.join(out, "a%02d_interiors_sheet.png" % area))


def build(area, L, refs, out, use_fmv=True, preview=None, interiors=None):
    T_ = L["towns"][str(area)]
    cache = os.path.join(out, "fmv")
    pal, psrc, stats = derive_clut(T_["page"], L, refs, cache)
    pix, src = compose(area, L, T_, pal, cache, use_fmv)
    cells, warn = cellmap(area, T_)
    d = os.path.join(out, "AREA%02d" % area + ("" if use_fmv else "_nofmv"))
    os.makedirs(d, exist_ok=True)
    open(os.path.join(d, "map8F.tim"), "wb").write(tim8(pix, pal))
    open(os.path.join(d, "map8E.bin"), "wb").write(cells8e(cells))
    pim = Image.fromarray(pix, "P"); pim.putpalette(pal.astype(np.uint8).ravel().tolist()); pim.save(os.path.join(d, "picture.png"))
    rgb = pal[pix]
    tint = rgb.copy().astype(float)
    big = np.kron(cells, np.ones((8, 8), np.uint8))
    for v, col in ((9, (255, 255, 255)), (0x80, (0, 255, 255))):
        m = big == v
        tint[m] = tint[m] * 0.7 + np.array(col) * 0.3
    Image.fromarray(tint.astype(np.uint8)).save(os.path.join(d, "cells.png"))
    srcpal = np.array([(0, 0, 0), (110, 110, 110), (60, 110, 255), (60, 220, 60), (255, 220, 0)], np.uint8)
    Image.fromarray(srcpal[src]).save(os.path.join(d, "source.png"))
    pv = overlay(area, T_, tint.astype(np.uint8))
    pv.save(os.path.join(d, "preview.png"))
    man = dict(area=area, name=T_["name"], width=int(pix.shape[1]), height=int(pix.shape[0]),
               cells=[int(cells.shape[1]), int(cells.shape[0])], clut_page=T_["page"], clut=stats, fmv_underlay=use_fmv,
               clut_note="FMV-derived, not original, quantized to PS1 15-bit (clut.clut15 = the TIM words; sources per index: clutgen = community FMV-derived CLUT (clutgen), ref = community render, fmv = our BUNMEI.STR anchors, fill = harmonic interpolation)", pixel_sources={k: int((src == v).sum()) for k, v in
                                                                   (("ground", 1), ("fmv", 2), ("cutout", 3), ("fmv_stamp", 4))},
               warnings=warn, files=["map8F.tim", "map8E.bin", "picture.png", "cells.png", "preview.png", "source.png"])
    json.dump(man, open(os.path.join(d, "manifest.json"), "w"), indent=1)
    if interiors and use_fmv:
        render_interiors(area, L, refs, T_, rgb, interiors, cache)
    if preview:
        os.makedirs(preview, exist_ok=True)
        tag = "a%02d%s" % (area, "" if use_fmv else "_nofmv")
        pv.save(os.path.join(preview, "%s_preview.png" % tag))
        Image.fromarray(rgb).save(os.path.join(preview, "%s_picture.png" % tag))
        Image.fromarray(srcpal[src]).save(os.path.join(preview, "%s_source.png" % tag))
    print("AREA%02d s0 %s: %dx%d px, %dx%d cells, CLUT %s %s -> %s" % (area, T_["name"], pix.shape[1], pix.shape[0],
          cells.shape[1], cells.shape[0], T_["page"], {k: v for k, v in stats.items() if k != "clut15"}, d))
    for w in warn:
        print("  warning:", w)
    return d


PAGE_SETS = [["A06100"], ["A07600", "A07601"], ["A08000"], ["A08600", "A08601"]]


def render_pages(L, refs, out, cache):
    """Each scenery page (pairs side by side) in its quantized custom CLUT, with the 256 entries below as swatches
    labelled with their 15-bit hex words. The 4 bpp sprite columns (skip_x) are shown as stored, in the same CLUT."""
    os.makedirs(out, exist_ok=True)
    f = font(10)
    for grp in PAGE_SETS:
        pal, _, st = derive_clut(grp[0], L, refs, cache)
        ims = [page(n) for n in grp]
        W = sum(p.shape[1] for p in ims)
        sw, rows = W // 16, 16
        im = Image.new("RGB", (W, 256 + 20 + rows * 30), (24, 24, 24))
        x = 0
        for p in ims:
            im.paste(Image.fromarray(pal[p]), (x, 0)); x += p.shape[1]
        dr = ImageDraw.Draw(im)
        dr.text((4, 258), "%s  FMV-derived custom CLUT, not original, quantized to PS1 15-bit  (collapsed %d, max change %d)"
                % (" + ".join(grp), st["quant_collapsed"], st["quant_max_change"]), fill=(230, 230, 230), font=f)
        for i in range(256):
            cx, cy = (i % 16) * sw, 276 + (i // 16) * 30
            dr.rectangle([cx + 1, cy + 1, cx + sw - 2, cy + 17], fill=tuple(int(v) for v in pal[i]))
            dr.text((cx + 2, cy + 18), "%02X:%s" % (i, st["clut15"][i]), fill=(200, 200, 200), font=f)
        fn = os.path.join(out, "scenery_%s.png" % grp[0]); im.save(fn)
        print(fn, {k: v for k, v in st.items() if k != "clut15"})


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--area", type=int, action="append", choices=(11, 16, 17))
    ap.add_argument("--out", default=OUT)
    ap.add_argument("--layout", default=LAYOUT)
    ap.add_argument("--ref", action="append", default=[], metavar="PAGE=PNG",
                    help="community colour render of a page (same size as the page), e.g. A08000=125.png")
    ap.add_argument("--refdir", help="folder with the community renders numbered as shared in the project "
                    "(125.png = A08000, 127.png = A08600, 128.png = A06100); same as three --ref")
    ap.add_argument("--community-cluts", metavar="DIR", help="the community clutgen folder (GGDdefault.act, CLUT*.png, "
                    "PHOT*.png): its index colours come first ('community FMV-derived CLUT (clutgen)')")
    ap.add_argument("--interiors", metavar="OUT", help="also render the interior view and the house/interior sheet to OUT")
    ap.add_argument("--pages", metavar="OUT", help="only render the scenery pages with their CLUTs to OUT")
    ap.add_argument("--no-fmv", action="store_true", help="cut-outs and ground only, no FMV mosaic underlay")
    ap.add_argument("--both", action="store_true", help="build with and without the FMV underlay")
    ap.add_argument("--preview", help="also write the previews to this directory")
    a = ap.parse_args()
    L = json.load(open(a.layout, encoding="utf-8"))
    refs = dict(r.split("=", 1) for r in a.ref)
    if a.refdir:
        for n, pg_ in REFDIR.items():
            f = os.path.join(a.refdir, n)
            if os.path.exists(f):
                refs.setdefault(pg_, f)
    if a.community_cluts:
        COMMUNITY.update(clutgen_samples(a.community_cluts))
    if a.pages:
        return render_pages(L, refs, a.pages, os.path.join(a.out, "fmv"))
    for area in a.area or (17, 11, 16):
        for fmv in ((True, False) if a.both else (not a.no_fmv,)):
            build(area, L, refs, a.out, fmv, a.preview, a.interiors)


if __name__ == "__main__":
    main()
