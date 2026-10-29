#!/usr/bin/env python3
"""Complete texture dump straight from the disc files (no game run): every page and sprite frame, ready for a texture pack.

Default (static, seconds): every GRPX page of every load list (SYS/LD0*.BIN, read through tools/area000 from data/) cut at
its texture pages, and every WPP sprite frame, written as palette PNGs (CLUT indices + a representative palette) named
"<descripcion>__r<key>.png" in readable folders (Zonas/NN Nombre/{Escenario,Objetos y enemigos,Sprites/..}, Personajes/Tombi,
Interfaz/.., Pantallas/.., Comun/..) with textures.jsonl and LEEME.txt.  key = FNV-1a(FNV-1a of the words LoadImage receives,
depth, page part) - the CLUT-less key of port2/shims/texpack.c, which matches it under any palette and applies the live
palette to an indexed replacement.

  python3 tools/dump_all_textures.py [--data DIR] [--out DIR]
  python3 tools/dump_all_textures.py --runtime [--exe build/port2/tombi2] [--jobs N] [--quick]   (secondary: headless sweep
        of every room/menu/item with the port's --dump-textures, exact per-palette keys, for uploads the game builds itself)
"""
import json, os, shutil, subprocess, sys, tempfile, time
from concurrent.futures import ThreadPoolExecutor, as_completed

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

SELECT = ["--poke", "900:1F8001B4:1"]                      # debug area select (docs/PORT.md "Mods")
START = "1000:start,1300:circle:3"
WALK = ("1500:right:300,1800:cross:3x8/40,2150:left:300,2450:square:3x8/40,2800:right+cross:3x30/60,3100:up:40,"
        "3160:down:40,3220:right:200,3450:cross:3x8/40,3600:start:3,3700:right:3x4/30")
EVENTS = ["--poke", "1100:8009CDA4:FF:1x130"]               # every event flag completed
LIVE = ["--poke-every", "1:8009C970:4:1", "--poke-every", "50:8009C980:9:1"]   # no death interrupting a room run
MENUS = range(0, 10)
ITEMS = range(0, 0xA0)


def rooms():
    import port_mods as M
    return [(a, s) for a, n in M.ROOM_COUNTS for s in range(n) if (a, s) not in M.DOOR_ROOMS]


def room_args(a, s):
    return SELECT + ["--poke", "1250:8009C960:%X:2" % a, "--poke", "1250:8009C962:%X:2" % s]


def jobs(quick=False, only=None):
    """[(label, args, frames)]"""
    out = []
    if only in (None, "title"):
        out.append(("title+logos", ["--play-movies", "--pad-script", "2600:down:3,2700:down:3,2800:up:3"], 3000))
        out.append(("title", ["--skip-movies", "--pad-script", "1100:down:3,1200:down:3,1300:cross:3,1500:circle:3"], 1800))
    if only in (None, "rooms"):
        for a, s in rooms():
            out.append(("room %02d:%d" % (a, s), ["--skip-movies"] + room_args(a, s) + LIVE + ["--pad-script", START + "," + WALK], 3900))
            if not quick:
                out.append(("room %02d:%d events" % (a, s), ["--skip-movies"] + room_args(a, s) + EVENTS + LIVE +
                            ["--pad-script", START + "," + WALK], 3900))
    if only in (None, "menus"):
        nav = ",".join("%d:%s:3" % (1900 + 60 * i, b) for i, b in enumerate(
            ["right", "right", "right", "down", "down", "cross", "circle", "left", "down", "cross", "circle", "up", "right", "cross"]))
        for n in MENUS:
            out.append(("menu %d" % n, ["--skip-movies"] + room_args(0, 1) + ["--menu", "1700:%d" % n, "--pad-script", START + "," + nav], 2900))
        out.append(("game over", ["--skip-movies"] + room_args(0, 1) + ["--poke", "1700:8009C980:0:1", "--poke-every", "1:8009C970:0:1",
                                                                        "--pad-script", START], 3200))
    if only in (None, "items") and not quick:
        for i in ITEMS:
            out.append(("item %02X" % i, ["--skip-movies"] + room_args(0, 1) + ["--test-item", "1650:%d" % i, "--pad-script", START], 1850))
    return out


def key_of(fn):
    """The pack key of a file name: "<16 hex>.png", "sub_<16 hex>.png", or either after a "...__" prefix (texpack.c parse_name)."""
    if not fn.lower().endswith(".png"):
        return None
    b = fn[:-4]
    if "__" in b:
        b = b[b.rindex("__") + 2:] if len(b) - b.rindex("__") - 2 in (16, 20) else b
    h = b[4:] if b.lower().startswith("sub_") else b
    if len(h) != 16 or any(c not in "0123456789abcdefABCDEF" for c in h):
        return None
    return ("sub_" + h.lower()) if len(b) == 20 else h.lower()


class Merger:
    """Moves the PNGs of one run's dump folder into OUT unless OUT already has that name; appends the index lines."""

    def __init__(self, out):
        self.out = out
        os.makedirs(out, exist_ok=True)
        self.names = set()                                  # keys ("sub_" + hex for sub boxes) already in OUT
        for dp, _dn, fn in os.walk(out):
            self.names.update(k for k in map(key_of, fn) if k)
        self.index = open(os.path.join(out, "textures.jsonl"), "a", encoding="utf-8")

    def merge(self, d, label):
        new = 0
        try:
            lines = open(os.path.join(d, "textures.jsonl"), encoding="utf-8").read().splitlines()
        except OSError:
            return 0
        for ln in lines:
            try:
                j = json.loads(ln)
            except ValueError:
                continue
            rel = j["file"]
            name = key_of(os.path.basename(rel))
            src = os.path.join(d, rel)
            if name in self.names or not os.path.isfile(src):
                continue
            dst = os.path.join(self.out, rel)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            shutil.move(src, dst)
            j["run"] = label
            self.index.write(json.dumps(j) + "\n")
            self.names.add(name)
            new += 1
        self.index.flush()
        return new

    def close(self):
        self.index.close()


def _run(exe, label, args, frames, data, disc, tmp):
    d = tempfile.mkdtemp(prefix="run", dir=tmp)
    cmd = [exe, "--headless", "--frames", str(frames), "--dump-textures", d] + args
    if data:
        cmd += ["--data", data]
    if disc:
        cmd += ["--disc", disc]
    ok = True
    try:
        subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=600,
                       creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
    except subprocess.TimeoutExpired:
        ok = False
    return label, d, ok


def run(exe, out, data=None, disc=None, njobs=None, quick=False, only=None, log=print, progress=None, organize_data=None):
    """Returns (unique textures written by this call, total in OUT, runs)."""
    js = jobs(quick, only)
    njobs = njobs or max(1, min(8, (os.cpu_count() or 2) - 1))
    m = Merger(out)
    before = len(m.names)
    tmp = tempfile.mkdtemp(prefix=".dumptmp", dir=out)     # same volume as OUT: moves are renames
    t0 = time.time()
    try:
        with ThreadPoolExecutor(njobs) as ex:
            futs = [ex.submit(_run, exe, lb, a, fr, data, disc, tmp) for lb, a, fr in js]
            for i, f in enumerate(as_completed(futs), 1):
                label, d, ok = f.result()
                n = m.merge(d, label)
                shutil.rmtree(d, ignore_errors=True)
                log("[%3d/%d] %-22s +%4d  (%d total)%s" % (i, len(js), label, n, len(m.names), "" if ok else "  timeout"))
                if progress:
                    progress(i, len(js))
    finally:
        m.close()
        shutil.rmtree(tmp, ignore_errors=True)
    log("done in %.0f s: %d new textures, %d in %s" % (time.time() - t0, len(m.names) - before, len(m.names), out))
    if organize_data:
        organize(out, organize_data, log)
    return len(m.names) - before, len(m.names), len(js)


# ------------------------------------------------------------------ readable folders
ZONAS = {0: "Aldea del Principio", 1: "Bosque y Torre Vigia", 2: "Cuevas y tuneles", 3: "Montana del Fenix", 4: "Aldea de los Trucos",
         5: "Aldea Baccus", 6: "Motocross", 7: "Area 07", 8: "Lago Baccus", 9: "Aldea de las Setas", 10: "Jungla Profunda",
         11: "Aldea de la Civilizacion", 12: "Area 12", 13: "Isla de los Cerdos", 14: "Cerdos Malvados", 15: "Area 15",
         16: "Civilizacion - Torre del Reloj", 17: "Civilizacion - Torre de Hierro", 18: "Civilizacion - Cruce en Y",
         19: "Civilizacion purificada"}
SCREENS = {"SYSTEM/A09400": "Logo Sony", "SYSTEM/A00100": "Logo Whoopee Camp", "SYSTEM/A00200": "Titulo", "SYSTEM/A00202": "Titulo",
           "SYSTEM/A00701": "Mapa del mundo", "SYSTEM/A00400": "Menu Guardar-Cargar", "SYSTEM/A00401": "Menu Guardar-Cargar",
           "SYSTEM/A00500": "Menu Info", "SYSTEM/A00501": "Menu Info", "SYSTEM/A00900": "Now Loading",
           "SYSTEM/A00000": "HUD y fuentes", "SYSTEM/A10003": "HUD y fuentes"}
PLAYER_WPP = ()          # filled from the dump (_player_files)


def zona(a):
    return "%02d %s" % (a, ZONAS.get(a, "Area %02d" % a))


def _fnv(b, h=0xCBF29CE484222325):
    for x in b:
        h = ((h ^ x) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h


def resource_index(data, log=print):
    """{upload hash: (resource, load list folder, list index, wpp frame or None)} and {(x, y, w, h): [(resource, folder)]}: every
    GRPX record of the load lists (the words LoadImage receives) and every WPP sprite frame, hashed like texpack.c."""
    A = disc_env(data)
    import vram, wpp
    byhash, byrect = {}, {}
    lists = A.load_lists(data)
    for ld, ll in lists.items():
        for k, lst in enumerate(ll):
            for r in lst:
                nm = r["name"]
                try:
                    if vram.is_grpx(r) and not vram.is_clut_rec(r):
                        d, n = vram.resource(nm), r["w"] * r["h"] * 2
                        if d and len(d) >= n:
                            byhash.setdefault(_fnv(d[:n]), (nm, ld, k, None))
                        byrect.setdefault((r["arg"] & 0xFFFF, r["arg"] >> 16, r["w"], r["h"]), []).append((nm, ld))
                    elif (r["kind"] & 0xF0) == 0x60 and r["type"] != 0xFFFFFFFF and nm.endswith(".000") and wpp.is_wpp(nm):
                        _m, _f, fr = wpp.unpack_wpp(vram.resource(nm), nm, wpp.is_wpp(nm))
                        for fk, o in fr.items():
                            byhash.setdefault(_fnv(o[4:]), (nm, ld, k, int(fk.split("frame_")[1][:4])))
                except Exception as e:          # one unreadable resource only loses its names
                    log("  (%s: %s)" % (nm, e))
    return byhash, byrect


def disc_env(data):
    """Points tools/area000 (load lists, resources) at an extracted data/ folder: the file table comes from its manifest.txt
    (tools/extract_data.py), so no disc image is needed."""
    import cdfiles, extract_data as X
    man = X.read_manifest(data)
    if man:
        lay = {p: (l, sz) for l, _n, _sb, sz, p in man["files"] if not p.startswith("_disc")}
        cdfiles.layout_from_image = lambda _p: lay
    import area000 as A
    if not hasattr(A, "_dt_orig_read"):
        A._dt_orig_read = A.read_resource
    A.read_resource = lambda name, fs=None: A._dt_orig_read(name, data)
    A.FS, A._LISTS = data, None
    import vram
    vram._RES.clear()
    A.load_lists(data)
    return A


def page_parts(x, y, w, h, depth):
    """[(x1, y1, x2, y2)] VRAM word boxes of the upload (x, y, w, h) cut at the texture pages a primitive of this depth (0 4-bit,
    1 8-bit, 2 15-bit) uses: page columns start at multiples of 64 words (8-bit pages tiled from the upload's own column),
    rows at 0 / 256; texpack.c keys the part of the upload inside the page."""
    pw = (64, 128, 256)[depth]
    out = []
    tx0 = x - x % 64
    for ty in range(y - y % 256, y + h, 256):
        tx = tx0
        while tx < x + w:
            x1, y1, x2, y2 = max(x, tx), max(y, ty), min(x + w, tx + pw), min(y + h, ty + 256)
            if x2 > x1 and y2 > y1:
                out.append((x1, y1, x2, y2))
            tx += pw
    return out


def any_key(uphash, depth, x, y, part):
    """The "r" key of texpack.c: FNV-1a(upload hash, depth, offset / size of the page part), no CLUT."""
    import struct
    x1, y1, x2, y2 = part
    k = _fnv(struct.pack("<Q", uphash))
    return _fnv(struct.pack("<5I", depth, x1 - x, y1 - y, x2 - x1, y2 - y1), k)


def _png(path, w, h, rows, pal=None, opaque=False):
    """PNG of rows of 15-bit colour words (pal None: RGBA, alpha classes of texpack.c: 0x0000 transparent, STP 128, else 255) or of
    CLUT indices (pal = the palette words: a palette PNG, PLTE + tRNS with the same classes; the port reads the indices)."""
    import zlib, struct
    ch = lambda t, d: struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    alpha = lambda c: 0 if c == 0 else 255 if opaque else 128 if c & 0x8000 else 255     # opaque: previews (STP shown solid)
    rgb = lambda c: bytes((((c & 31) << 3) | ((c & 31) >> 2), (((c >> 5) & 31) << 3) | (((c >> 5) & 31) >> 2),
                           (((c >> 10) & 31) << 3) | (((c >> 10) & 31) >> 2)))
    raw = bytearray()
    if pal is None:
        for row in rows:
            raw.append(0)
            for c in row:
                raw += rgb(c) + bytes((alpha(c),))
        hdr, extra = struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0), b""
    else:
        for row in rows:
            raw.append(0)
            raw += bytes(row)
        hdr = struct.pack(">IIBBBBB", w, h, 8, 3, 0, 0, 0)
        extra = ch(b"PLTE", b"".join(rgb(c) for c in pal)) + ch(b"tRNS", bytes(alpha(c) for c in pal))
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + ch(b"IHDR", hdr) + extra + ch(b"IDAT", zlib.compress(bytes(raw), 6)) + ch(b"IEND", b""))


def _decode(words, ww, part_w, part_h, ox, oy, depth, pal=None):
    """Rows of the part (ox, oy words into a ww-word-wide plane): CLUT indices (4/8-bit) or colour words (15-bit)."""
    rows = []
    for yy in range(part_h):
        base = (oy + yy) * ww + ox
        seg = words[base:base + part_w]
        if depth == 2:
            rows.append(list(seg))
        elif depth == 1:
            rows.append([(v >> s) & 255 for v in seg for s in (0, 8)])
        else:
            rows.append([(v >> s) & 15 for v in seg for s in (0, 4, 8, 12)])
    return rows


def _clut_words(A, name, first, n):
    import struct
    d = A.read_resource(name)
    if not d or len(d) < (first + n) * 2:
        return None
    return list(struct.unpack_from("<%dH" % n, d, first * 2))


def _grey(n):
    return [0] + [((i * 31 // (n - 1)) * 0x421) | 0x8000 * 0 for i in range(1, n)]


def _part_box(p):
    """(u, v, w, h) texel box of an SPR part (u0 v0 clut u1 v1 tpage u2 v2 w h u3 v3 dx dy; flipped parts run backwards)."""
    us, vs = (p[0], p[3], p[6], p[10]), (p[1], p[4], p[7], p[11])
    return min(us), min(vs), p[8], p[9]


def nibble_jump(d, w, h):
    """Mean |low - high| nibble of the non-empty bytes of a page (sampled).  A real 4 bpp page draws neighbour texels with near indices
    (about 0.5..1.6); 8 bpp data read as nibbles jumps (about 4..5): the room pictures stored next to the room atlases (AREA00 B301,
    AREA02 B001...) are 8 bpp although the load list pairs them with no 256 colour CLUT of their own (vram.pair_cluts takes them for 4 bpp)."""
    s = n = 0
    for y in range(0, h, 3):
        row = d[y * w * 2:(y + 1) * w * 2:2]
        for v in row:
            lo, hi = v & 15, v >> 4
            if lo and hi:
                s += abs(lo - hi)
                n += 1
    return s / n if n > 50 else 0.0


def smooth_score(d, w, h, pal):
    """Mean RGB distance of horizontal neighbour texels of an 8 bpp page under the 256 colour palette `pal` (words; lower = more natural)."""
    rgb = [((c & 31), (c >> 5) & 31, (c >> 10) & 31) for c in pal]
    s = n = miss = 0
    wp = w * 2
    for y in range(0, h, 3):
        b = y * wp
        for x in range(0, wp - 1, 2):
            i, j = d[b + x], d[b + x + 1]
            if i and j:
                if not pal[i] or not pal[j]:        # a texel the row leaves empty: this is not the picture's palette
                    miss += 1
                    continue
                a, c = rgb[i], rgb[j]
                s += abs(a[0] - c[0]) + abs(a[1] - c[1]) + abs(a[2] - c[2])
                n += 1
    return s / n + 200.0 * miss / (n + miss) if n > 50 else 999.0


def _blob_palettes(rows, w, h, owner, cands, first):
    """Texels of a 4 bpp page no known draw covers: cut into blobs (8 x 8 cells joined when they touch) and give each the 16 colour
    palette among `cands` under which it looks most natural (neighbour colour distance; used indices that are empty or share a colour
    cost a lot).  Marks them in `owner` with first + index of the palette; returns the palettes used ([(palette)])."""
    C = 8
    cw, chh = (w + C - 1) // C, (h + C - 1) // C
    occ = [0] * (cw * chh)
    for y in range(h):
        r = rows[y]
        for x in range(w):
            if r[x] and not owner[y * w + x]:
                occ[(y // C) * cw + x // C] = 1
    lab, blobs = [0] * (cw * chh), []
    for c0 in range(cw * chh):
        if occ[c0] and not lab[c0]:
            st, cells = [c0], []
            lab[c0] = len(blobs) + 1
            while st:
                c = st.pop()
                cells.append(c)
                cx, cy = c % cw, c // cw
                for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
                    if 0 <= nx < cw and 0 <= ny < chh and occ[ny * cw + nx] and not lab[ny * cw + nx]:
                        lab[ny * cw + nx] = len(blobs) + 1
                        st.append(ny * cw + nx)
            blobs.append(cells)
    crgb = [[((c & 31), (c >> 5) & 31, (c >> 10) & 31) for c in p] for p in cands]
    used, out = {}, []
    for bi, cells in enumerate(blobs):
        pairs, idx = {}, set()
        for c in cells:
            cx, cy = c % cw, c // cw
            for y in range(cy * C, min(cy * C + C, h)):
                r = rows[y]
                for x in range(cx * C, min(cx * C + C, w)):
                    a = r[x]
                    if a and not owner[y * w + x]:
                        idx.add(a)
                        if x + 1 < w and r[x + 1]:
                            pairs[(a, r[x + 1])] = pairs.get((a, r[x + 1]), 0) + 1
                        if y + 1 < h and rows[y + 1][x]:
                            pairs[(a, rows[y + 1][x])] = pairs.get((a, rows[y + 1][x]), 0) + 1
        tot = sum(pairs.values()) or 1
        best = None
        for pi, (p, rgb) in enumerate(zip(cands, crgb)):
            pen = sum(1 for i in idx if not p[i]) * 60.0 + (len(idx) - len({p[i] for i in idx})) * 15.0
            if best is not None and pen >= best[0]:
                continue
            dist = lambda a, b: abs(rgb[a][0] - rgb[b][0]) + abs(rgb[a][1] - rgb[b][1]) + abs(rgb[a][2] - rgb[b][2])
            near = sum(n * dist(a, b) for (a, b), n in pairs.items()) / tot
            spread = sum(dist(a, b) for a in idx for b in idx) / max(1, len(idx) * (len(idx) - 1))
            s = pen + 40.0 * near / (spread + 4.0)     # neighbours close for the colours the blob has: flat ramps do not win
            if best is None or s < best[0]:
                best = (s, pi)
        if best is None:
            continue
        if best[1] not in used:
            used[best[1]] = first + len(out)
            out.append(cands[best[1]])
        val = used[best[1]]
        for c in cells:
            cx, cy = c % cw, c // cw
            for y in range(cy * C, min(cy * C + C, h)):
                r = rows[y]
                for x in range(cx * C, min(cx * C + C, w)):
                    if r[x] and not owner[y * w + x]:
                        owner[y * w + x] = val
    return out


def locate(v, tp, u0, v0, u1, v1, uphash):
    """(key, part, px, py, tpw, depth) of the dumped page part a primitive with tpage `tp` and texel box u0..u1 x v0..v1 (inclusive)
    samples in VRAM context `v`, or None."""
    depth = (tp >> 7) & 3
    if depth > 1 or u0 < 0 or v0 < 0 or u1 > 255 or v1 > 255:
        return None
    tpw = (4, 2)[depth]
    px, py = (tp & 15) * 64, ((tp >> 4) & 1) * 256
    wx0, wx1, wy0, wy1 = px + u0 // tpw, px + u1 // tpw + 1, py + v0, py + v1 + 1
    rec = next((rc for rc in reversed(v.recs) if rc[5] == "page" and rc[1] <= wx0 and wx1 <= rc[1] + rc[3]
                and rc[2] <= wy0 and wy1 <= rc[2] + rc[4]), None)
    if not rec:
        return None
    nm, x, y, rw, rh, _k = rec
    hsh = uphash(nm, rw, rh)
    if hsh is None:
        return None
    part = next((pp for pp in page_parts(x, y, rw, rh, depth) if pp[0] <= wx0 and wx1 <= pp[2] and pp[1] <= wy0 and wy1 <= pp[3]), None)
    if not part:
        return None
    return any_key(hsh, depth, x, y, part), part, px, py, tpw, depth


def _model_blobs(lst):
    """Bytes of the WMD room model (kind 0x50) and of the object model table (kind 0x51) of a load list (own files or pieces)."""
    import vram
    data, off = None, 0
    for r in lst:
        if r["type"] != 0xFFFFFFFF:
            data, off = vram.resource(r["name"]), (r["arg"] if r["kind"] == 0x50 else 0)
            if r["kind"] in (0x50, 0x51) and data:
                yield r["kind"], data
        elif data is not None:
            if r["kind"] in (0x50, 0x51):
                yield r["kind"], data[off:off + r["arg"]]
            off += r["arg"]


def model_uses(ld, idx, lst, uphash):
    """[(key, depth, palette words, [(x, y)] polygon in page part texels)]: every textured polygon of the room / object models of list
    idx (tools/room.py WMD packets carry the real GPU tpage + CLUT words, so each polygon names its own palette)."""
    import struct
    import vram, room as R
    out, v = [], None
    for kind, b in _model_blobs(lst):
        pks = []
        try:
            if kind == 0x50:
                pks = R.parse_wmd_ex(b, lenient=True)
            else:
                n = struct.unpack_from("<I", b)[0]
                if not 0 < n < 1000 or len(b) < 4 + 4 * n:
                    continue
                o = list(struct.unpack_from("<%dI" % n, b, 4)) + [len(b)]
                for i in range(n):
                    if o[i] < o[i + 1] <= len(b):
                        pks += R.parse_wmd_ex(struct.pack("<II", 1, 8) + b[o[i]:o[i + 1]], lenient=True)
        except Exception:
            continue
        if not pks:
            continue
        v = v or vram.ctx_get(ld, idx)
        seen = set()
        for cl, tp, _cmd, vs, _c in pks:
            uv = tuple((q[3], q[4]) for q in vs)
            if (cl, tp, uv) in seen:
                continue
            seen.add((cl, tp, uv))
            us, ws = [q[0] for q in uv], [q[1] for q in uv]
            loc = locate(v, tp, min(us), min(ws), max(us), max(ws), uphash)
            if not loc:
                continue
            key, part, px, py, tpw, depth = loc
            pal = tuple(v.palette(cl, (16, 256)[depth])[2])
            pts = [(u + (px - part[0]) * tpw, w + py - part[1]) for u, w in uv]
            for tri in ([pts[:3]] if len(pts) == 3 else [pts[:3], [pts[1], pts[3], pts[2]]]):
                out.append((key, depth, pal, tri))
    return out


def _fill_tri(owner, W, H, tri, val):
    """Marks with `val` the texels of `owner` (W x H list) inside the triangle (edges included)."""
    (x0, y0), (x1, y1), (x2, y2) = tri
    den = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2)
    for y in range(max(min(y0, y1, y2), 0), min(max(y0, y1, y2), H - 1) + 1):
        for x in range(max(min(x0, x1, x2), 0), min(max(x0, x1, x2), W - 1) + 1):
            if den == 0:
                owner[y * W + x] = val       # degenerate (a line of texels): its box
                continue
            a = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / den
            b = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / den
            if a >= -.01 and b >= -.01 and a + b <= 1.01:
                owner[y * W + x] = val


def sprite_crops(ld, idx, lst, uphash, log=print):
    """[(key, u, v, w, h, depth, palette words, index rows, label)]: every SPR part of the tables of list idx as a rectangle of the
    page part (or WPP frame) it is cut from, with the palette the part is drawn with (vram.export_spr's resolution of each table:
    the WPP frame of the same number, else the context and tpage offset of infer_base; Tombi's placeholder CLUT -> vram.wpp_clut)."""
    import struct, hashlib
    import area000 as A, vram
    out = []
    tabs = [(r, d) for r, d in A.list_blocks(ld, idx) if (r["kind"] & 0xF0) == 0x30 and d and len(d) >= 4]
    if not tabs:
        return out
    wt = vram.wpp_tables(lst)
    cands = None
    for r, d in tabs:
        nfr = struct.unpack_from("<H", d, 2)[0] // 4
        frames = vram.parse_spr(d)
        link = next((t for t in wt if len(t[2]) == nfr), None)
        if link:
            v = vram.ctx_get(ld, idx)
            for fi, o in enumerate(link[2]):
                fw, fh = struct.unpack_from("<HH", o)
                if not fw or not fh or fi >= len(frames):
                    continue
                key = any_key(_fnv(o[4:4 + fw * fh * 2]), 0, 0, 0, (0, 0, fw, fh))
                rows, W, H = vram.wpp_plane(o)
                for pi, p in enumerate(frames[fi]):
                    u, vv, w, h = _part_box(p)
                    if w <= 0 or h <= 0 or u + w > W or vv + h > H:
                        continue
                    pal = list(v.palette(vram.wpp_clut(p[2]), 16)[2])
                    out.append((key, u, vv, w, h, 0, pal, [rows[vv + j][u:u + w] for j in range(h)],
                                "%s_f%04d_parte%d" % (os.path.basename(_base(link[0])), fi, pi)))
            continue
        if (r["kind"], hashlib.md5(d).hexdigest()) in vram.shared_tables():
            cs = vram.shared_candidates(ld)
        else:
            cands = cands if cands is not None else vram.candidates(ld, idx)
            cs = cands
        parts = [p for f in frames for p in f if p[8] and p[9]]
        res = vram.infer_base(cs, parts) if parts else None
        if res is None:
            continue
        ci, base, _info = res
        v = cs[ci][1]
        for fi, f in enumerate(frames):
            for pi, p in enumerate(f):
                u, vv, w, h = _part_box(p)
                tp = p[5] + base
                depth = (tp >> 7) & 3
                if depth > 1 or w <= 0 or h <= 0 or u + w > 256 or vv + h > 256:
                    continue
                tpw = (4, 2)[depth]
                px, py = (tp & 15) * 64, ((tp >> 4) & 1) * 256
                wx0, wx1, wy0, wy1 = px + u // tpw, px + (u + w - 1) // tpw + 1, py + vv, py + vv + h
                rec = next((rc for rc in reversed(v.recs) if rc[5] == "page" and rc[1] <= wx0 and wx1 <= rc[1] + rc[3]
                            and rc[2] <= wy0 and wy1 <= rc[2] + rc[4]), None)
                if not rec:
                    continue
                nm, x, y, rw, rh, _k = rec
                hsh = uphash(nm, rw, rh)
                if hsh is None:
                    continue
                part = next((pp for pp in page_parts(x, y, rw, rh, depth) if pp[0] <= wx0 and wx1 <= pp[2] and pp[1] <= wy0 and wy1 <= pp[3]), None)
                if not part:
                    continue
                key = any_key(hsh, depth, x, y, part)
                cu, cv = u + (px - part[0]) * tpw, vv + py - part[1]
                if cu < 0 or cv < 0:
                    continue
                n = (16, 256)[depth]
                pal = list(v.palette(p[2], n)[2])
                src = v.nib if depth == 0 else v.d
                VWp = 1024 * tpw
                rows = [bytes(src[(py + vv + j) * VWp + px * tpw + u:(py + vv + j) * VWp + px * tpw + u + w]) for j in range(h)]
                out.append((key, cu, cv, w, h, depth, pal, rows, "%s_spr%02X_f%04d_parte%d" % (os.path.basename(_base(nm)), r["kind"], fi, pi)))
    return out


def capture_uses(path, pending, log=print):
    """{page key: [(palette, ("rect", u, v, w, h))]} from a capture of the port (tools/gfx_capture.py, port2 --dump-draws): every draw
    the game was seen making whose texels are those of a dumped page part, with the palette it was drawn with (exact)."""
    import gfx_capture as GC
    try:
        draws = [d for d in GC.load(path) if d.depth < 2]
    except Exception as e:
        log("  (capture %s: %s)" % (path, e))
        return {}
    parts = [(m["vram"], dep, key, rows) for key, (_t, _p, _w, _h, rows, m, dep, _r) in pending.items()]
    out, seen = {}, set()
    for d in draws:
        wx, wy, ww = d.page_xy()
        q = 1 if d.kind != 2 and d.w > 1 and d.h > 1 else 0          # a polygon's UV box has one extra (undrawn) texel
        for (x1, y1, pw, ph), dep, key, rows in parts:
            if dep != d.depth or not (x1 <= wx and wx + ww <= x1 + pw and y1 <= wy and wy + d.h <= y1 + ph):
                continue
            tpw = (4, 2)[dep]
            cu, cv = (d.tx - x1) * tpw + d.u, d.ty - y1 + d.v
            if cu < 0 or cv < 0 or any(bytes(rows[cv + j][cu:cu + d.w]) != bytes(d.tex[j * d.w:(j + 1) * d.w]) for j in range(d.h)):
                continue
            k = (key, cu, cv, d.w - q, d.h - q, tuple(d.pal))
            if k not in seen:
                seen.add(k)
                out.setdefault(key, []).append((tuple(d.pal), ("rect", cu, cv, d.w - q, d.h - q)))
            break
    return out


def dump_static(out, data, log=print, progress=None, capture=None, make_capture=False):
    """Every image of every resource, read from the disc files (no game run): each GRPX page of every load list cut at its texture
    pages, and every WPP sprite frame, named "<descripcion>__r<key>.png" in the readable tree.  Returns the number of images."""
    import struct
    A = disc_env(data)
    import vram, wpp
    lists = A.load_lists(data)
    if not lists:
        raise RuntimeError("no load lists in %s (is it an extracted disc? tools/extract_data.py)" % data)
    os.makedirs(out, exist_ok=True)
    index = open(os.path.join(out, "textures.jsonl"), "w", encoding="utf-8")
    done, n, t0 = set(), 0, time.time()
    work = [(ld, k, lst) for ld, ll in sorted(lists.items(), key=lambda kv: (kv[0] != "SYS", kv[0])) for k, lst in enumerate(ll)]
    global PLAYER_WPP
    PLAYER_WPP = ("AREA00/A004",)            # Tombi's sprite file (the WPP of the area list of AREA00, drawn in every area)
    safe = lambda t: "".join(c if c.isalnum() or c in " -_~." else "_" for c in t).strip()

    def first_pal(ld, k, bpp, paired):
        """A representative palette (the preview colours only: the "r" key matches the page with any palette)."""
        if bpp == 8 and paired is not None:
            return _clut_words(A, paired["name"], 0, 256), paired["name"] + " row 0"
        order = [(ld, k)] + ([(ld, 0)] if ld != "SYS" and k else []) + ([("SYS", 0)] if (ld, k) != ("SYS", 0) else [])
        recs = [r for l, kk in order for r in lists[l][kk] if vram.is_grpx(r) and vram.is_clut_rec(r)]
        recs.sort(key=lambda r: r["w"] == 256)
        for r in recs:
            if bpp == 8 and r["w"] == 256:
                return _clut_words(A, r["name"], 0, 256), r["name"] + " row 0"
            if bpp == 4:
                for y in range(r["h"]):
                    for sx in range(r["w"] // 16):
                        p = _clut_words(A, r["name"], y * r["w"] + sx * 16, 16)
                        if p and any(p):
                            return p, "%s row %d +%d" % (r["name"], y, sx * 16)
        return _grey(256 if bpp == 8 else 16), "grey"

    def best8(ld, k, d, w, h):
        """The 256 colour CLUT row that makes an unpaired 8 bpp page look most natural (room pictures: a row of the area CLUTnn)."""
        order = [(ld, k)] + ([(ld, 0)] if ld != "SYS" and k else [])
        best = None
        for l, kk in order:
            for r in lists[l][kk]:
                if vram.is_grpx(r) and vram.is_clut_rec(r) and r["w"] == 256:
                    for row in range(r["h"]):
                        p = _clut_words(A, r["name"], row * 256, 256)
                        if p and sum(1 for c in p if c) > 16:
                            sc = smooth_score(d, w, h, p)
                            if best is None or sc < best[0]:
                                best = (sc, p, "%s row %d" % (r["name"], row))
        return (best[1], best[2]) if best else (None, None)

    s16 = {}

    def sub16(lk):
        """Every non-empty 16 colour palette of the CLUT records of list lk ("AR00:4"), its area list 0 and SYSTEM list 0."""
        if lk not in s16:
            ld, k = lk.split(":")
            k = int(k)
            order = [(ld, k)] + ([(ld, 0)] if ld != "SYS" and k else []) + ([("SYS", 0)] if (ld, k) != ("SYS", 0) else [])
            out, seen = [], set()
            for l, kk in order:
                for r in lists[l][kk]:
                    if vram.is_grpx(r) and vram.is_clut_rec(r) and r["name"] not in seen:
                        seen.add(r["name"])
                        for y in range(r["h"]):
                            for sx in range(r["w"] // 16):
                                p = _clut_words(A, r["name"], y * r["w"] + sx * 16, 16)
                                if p and sum(1 for c in p if c) > 3 and tuple(p) not in seen:
                                    seen.add(tuple(p))
                                    out.append(tuple(p))
            s16[lk] = out
        return s16[lk]

    page_dir, crops_done, n_crops, hcache = {}, set(), [0], {}
    pending, uses = {}, {}       # page key -> (folder, prefix, w, h, rows, meta, depth, palette is real); page key -> [(palette, shape)]

    def uphash(nm, w, h):
        if (nm, w, h) not in hcache:
            d = vram.resource(nm)
            hcache[(nm, w, h)] = _fnv(d[:w * h * 2]) if d and len(d) >= w * h * 2 else None
        return hcache[(nm, w, h)]

    def emit(parts_dir, prefix, key, w, h, rows, meta):
        nonlocal n
        page_dir[key] = (list(parts_dir), prefix)
        rel = "/".join(safe(x) for x in parts_dir) + "/" + safe(prefix).replace("__", "_") + "__r%016x.png" % key
        os.makedirs(os.path.join(out, os.path.dirname(rel)), exist_ok=True)
        _png(os.path.join(out, rel), w, h, rows, meta.pop("_pal", None))
        meta.update(file=rel, key="r%016x" % key, size=[w, h])
        index.write(json.dumps(meta, ensure_ascii=False) + "\n")
        n += 1

    for wi, (ld, k, lst) in enumerate(work, 1):
        pr = vram.pair_cluts(lst)
        for i, r in enumerate(lst):
            nm = r["name"]
            try:
                if vram.is_grpx(r) and not vram.is_clut_rec(r):
                    x, y, w, h = r["arg"] & 0xFFFF, r["arg"] >> 16, r["w"], r["h"]
                    d = vram.resource(nm)
                    if not d or len(d) < w * h * 2 or w <= 0 or h <= 0:
                        continue
                    pal = None
                    if i in pr:
                        bpp = 8
                    elif nibble_jump(d, w, h) > 2.5:           # a room picture (8 bpp) with no CLUT of its own: the most natural row
                        bpp = 8
                        pal, pal_src = best8(ld, k, d, w, h)
                    else:
                        bpp = 4
                    depth = (4, 8).index(bpp)
                    up = _fnv(d[:w * h * 2])
                    words = struct.unpack_from("<%dH" % (w * h), d)
                    real = bpp == 8
                    if pal is None:
                        pal, pal_src = first_pal(ld, k, bpp, lst[pr[i]] if i in pr else None)
                    short = os.path.basename(_base(nm))
                    if _base(nm) in SCREENS:
                        top = ["Interfaz", SCREENS[_base(nm)]] if "HUD" in SCREENS[_base(nm)] else ["Pantallas", SCREENS[_base(nm)]]
                    elif ld == "SYS":
                        top = ["Comun", "Paginas"]
                    else:
                        top = ["Zonas", zona(int(ld[2:])), "Escenario" if short.startswith("B") else "Objetos y enemigos"]
                    for part in page_parts(x, y, w, h, depth):
                        key = any_key(up, depth, x, y, part)
                        if key in done:
                            continue
                        done.add(key)
                        x1, y1, x2, y2 = part
                        tpw = (4, 2)[depth]
                        rows = _decode(words, w, x2 - x1, y2 - y1, x1 - x, y1 - y, depth, pal)
                        prefix = "%s_p%d-%d_%dbpp" % (short, x1 - x1 % 64, y1 - y1 % 256, bpp)
                        page_dir[key] = (list(top), prefix)
                        pending[key] = (top, prefix, (x2 - x1) * tpw, y2 - y1, rows,
                                        dict(_pal=pal, kind="page", resource=nm, list="%s:%d" % (ld, k), depth=bpp, vram=[x1, y1, x2 - x1, y2 - y1],
                                             upload=[x, y, w, h], palette=pal_src), depth, real)
                elif (r["kind"] & 0xF0) == 0x60 and r["type"] != 0xFFFFFFFF and nm.endswith(".000") and wpp.is_wpp(nm):
                    _m, files, frames = wpp.unpack_wpp(vram.resource(nm), nm, wpp.is_wpp(nm))
                    try:
                        fps = wpp.frame_palettes(nm, frames)
                    except Exception:
                        fps = {}
                    short = os.path.basename(_base(nm))
                    player = _base(nm) in PLAYER_WPP
                    who = ["Personajes", "Tombi"] if player else (["Comun", "Sprites", short] if ld == "SYS" else
                                                                ["Zonas", zona(int(ld[2:])), "Sprites", short])
                    for fk in sorted(frames):
                        o = frames[fk]
                        fw, fh = struct.unpack_from("<HH", o)
                        if not fw or not fh:
                            continue
                        key = any_key(_fnv(o[4:4 + fw * fh * 2]), 0, 0, 0, (0, 0, fw, fh))
                        if key in done:
                            continue
                        done.add(key)
                        info = fps.get(fk)
                        pal = _clut_words(A, info["clut"], info["row"] * 256 + info["x0"], 16) if info else None
                        if not pal:
                            pal, src = first_pal(ld, k, 4, None)
                        else:
                            src = "%s row %d +%d" % (info["clut"], info["row"], info["x0"])
                        words = struct.unpack_from("<%dH" % (fw * fh), o, 4)
                        fno = int(fk.split("frame_")[1][:4])
                        emit(who, "%s_f%04d" % ("Tombi" if player else short, fno), key, fw * 4, fh,
                             _decode(words, fw, fw, fh, 0, 0, 0, pal),
                             dict(_pal=pal, kind="sprite", resource=nm, frame=fno, list="%s:%d" % (ld, k), depth=4, palette=src))
            except Exception as e:
                log("  (%s: %s)" % (nm, e))
        if ld != "SYS":
            try:
                for key, depth, pal, tri in model_uses(ld, k, lst, uphash):
                    uses.setdefault(key, []).append((pal, ("tri", tri)))
            except Exception as e:
                log("  (models %s:%d: %s)" % (ld, k, e))
            try:
                for key, cu, cv, w, h, depth, pal, rows, label in sprite_crops(ld, k, lst, uphash, log):
                    ck = (key, cu, cv, w, h)
                    if ck in crops_done:
                        continue
                    crops_done.add(ck)
                    uses.setdefault(key, []).append((tuple(pal), ("rect", cu, cv, w, h)))
                    page = page_dir.get(key)
                    if not page:
                        folder = ["Zonas", zona(int(ld[2:])), "Sprites sueltos"]
                    elif "_spr" in label:                      # a rectangle of an atlas page: next to the page, one folder per page
                        folder = page[0] + [page[1] + " - sprites"]
                    else:                                       # a part of a WPP frame: one folder per sprite file
                        folder = page[0] + ["Partes con su paleta"]
                    rel = "/".join(safe(x) for x in folder) + "/" + safe(label).replace("__", "_") + "__r%016x_%d_%d_%d_%d.png" % (key, cu, cv, w, h)
                    os.makedirs(os.path.join(out, os.path.dirname(rel)), exist_ok=True)
                    _png(os.path.join(out, rel), w, h, [list(r) for r in rows], pal)
                    index.write(json.dumps(dict(kind="sprite part", file=rel, key="r%016x" % key, rect=[cu, cv, w, h], depth=(4, 8)[depth],
                                                list="%s:%d" % (ld, k)), ensure_ascii=False) + "\n")
                    n_crops[0] += 1
            except Exception as e:
                log("  (sprites %s:%d: %s)" % (ld, k, e))
        if progress:
            progress(wi, len(work))
    import gfx_capture as GC
    if capture is None:
        capture = GC.default_store()
    if make_capture and not os.path.isfile(capture) and os.path.isfile(default_exe()):
        log("capturing the game's draws (port headless, every room) into %s ..." % capture)
        try:
            GC.run(default_exe(), capture, data=data, jobs=max(2, (os.cpu_count() or 4) // 2), log=log)
        except Exception as e:
            log("  (capture failed: %s)" % e)
    cap = capture_uses(capture, pending, log) if capture and os.path.isfile(capture) else {}
    if cap:
        log("capture %s: %d pages with draws seen in the game" % (capture, len(cap)))
    for key, lst_ in cap.items():                # last = on top: what the game was seen drawing wins
        uses.setdefault(key, []).extend(lst_)
    n_prev = 0
    for key, (top, prefix, w, h, rows, meta, depth, real) in pending.items():
        # the page as the game shows it ("Vista previa", RGBA, never read by the port: no "__<key>" in its name): every texel a model
        # polygon or a sprite samples, with that polygon's / sprite's own palette; texels nothing known draws stay grey (4 bpp) or keep
        # the page palette (8 bpp pictures, whose palette is the real one)
        owner, pals, cover = [0] * (w * h), [None], {}
        for pal, shp in uses.get(key, ()):
            if len(pal) != (16, 256)[depth] or not any(pal):
                continue
            if pal not in cover:
                cover[pal] = len(pals)
                pals.append(pal)
            pi = cover[pal]
            if shp[0] == "tri":
                _fill_tri(owner, w, h, shp[1], pi)
            else:
                _c, cu, cv, cw, ch = shp
                for yy in range(max(cv, 0), min(cv + ch, h)):
                    owner[yy * w + max(cu, 0):yy * w + min(cu + cw, w)] = [pi] * (min(cu + cw, w) - max(cu, 0))
        cnt = {}
        for yy in range(h):
            r = rows[yy]
            for xx in range(w):
                o = owner[yy * w + xx]
                if o and r[xx]:
                    cnt[o] = cnt.get(o, 0) + 1
        known = sum(cnt.values())
        if depth == 0 and not real:            # texels no known draw covers: a palette per blob, deduced (neighbour colours)
            pals += _blob_palettes(rows, w, h, owner, sub16(meta["list"]), len(pals))
        if cnt and not real:                    # the editable page: the palette most of it is drawn with instead of an example one
            meta["_pal"] = list(pals[max(cnt, key=cnt.get)])
            meta["palette"] = "most used by its sprites / models"
        base = meta["_pal"] if real else None
        grey = [0] + [((i * 2 if depth == 0 else i // 8) | 1) * 0x421 for i in range(1, 256)]
        prow = []
        for yy in range(h):
            r, line = rows[yy], []
            for xx in range(w):
                ix, o = r[xx], owner[yy * w + xx]
                c = pals[o][ix] if o else base[ix] if base else grey[ix]
                line.append(0 if not ix and not (o or base) else c if c else (0x8000 if ix and real else 0))
            prow.append(line)
        meta["preview"] = "%.0f%% exact, rest deduced" % (100.0 * known / max(1, sum(1 for r in rows for v in r if v)))
        emit(top, prefix, key, w, h, rows, meta)
        pdir = os.path.join(out, *[safe(x) for x in top], "Vista previa (solo para ver)")
        os.makedirs(pdir, exist_ok=True)
        _png(os.path.join(pdir, safe(prefix).replace("__", "_") + ".png"), w, h, prow, None, opaque=True)
        n_prev += 1
    index.close()
    with open(os.path.join(out, "LEEME.txt"), "w", encoding="utf-8") as f:
        f.write(LEEME)
    log("%d page previews" % n_prev)
    log("%d images + %d sprite rectangles in %s (%.0f s)" % (n, n_crops[0], out, time.time() - t0))
    return n + n_crops[0]


def _base(nm):
    return nm.rsplit(".", 1)[0]


def place_of(j, byhash, byrect):
    """(folder parts, name prefix, resource or None) of one index entry."""
    area, run = j.get("area", 0), j.get("run", "")
    res = byhash.get(int(j["upload"], 16)) if j.get("upload") else None
    approx = ""
    if not res and j.get("uprect"):
        c = byrect.get(tuple(j["uprect"]), [])
        own = [x for x in c if x[1] == "AR%02d" % area] or [x for x in c if x[1] == "SYS"]
        if own:
            res, approx = (own[0][0], own[0][1], None, None), "~"
    size = "%dx%d" % tuple(j["size"])
    pal = "_pal%d-%d" % tuple(j["clut"]) if j.get("clut", [-1])[0] >= 0 else ""
    if res:
        nm, ld, _k, frame = res
        short = os.path.basename(_base(nm))
        if frame is not None:
            who = ["Personajes", "Tombi"] if _base(nm) in PLAYER_WPP else (["Comun", "Sprites", short] if ld == "SYS" else
                                                                          ["Zonas", zona(int(ld[2:])), "Sprites", short])
            return who, "%s_f%04d_%s%s" % ("Tombi" if who[0] == "Personajes" else short, frame, size, pal), nm
        if _base(nm) in SCREENS:
            top = ["Interfaz", SCREENS[_base(nm)]] if "HUD" in SCREENS[_base(nm)] else ["Pantallas", SCREENS[_base(nm)]]
        elif ld == "SYS":
            top = ["Comun", "Paginas"]
        else:
            z = zona(int(ld[2:]))
            top = ["Zonas", z, "Escenario" if short.startswith("B") else "Objetos y enemigos"]
        pg = "_p%d-%d" % tuple(j["page"])
        return top, "%s%s%s_%s%s" % (approx, short, pg, size, pal), nm
    if run.startswith("title"):
        top = ["Pantallas", "Titulo"]
    elif run.startswith(("menu", "item", "game over")):
        top = ["Interfaz", "Menus y objetos" if not run.startswith("game") else "Game over"]
    else:
        top = ["Sin identificar", zona(area)]
    kind = "caja" if j.get("kind") == "sub" else "vram%d-%d" % tuple(j["vram"][:2])
    return top, "%s_%s%s" % (kind, size, pal), None


def _player_files(entries, byhash):
    """Tombi: the WPP sprite file whose frames are drawn in the most areas (AREA00/A004.000: he is on screen everywhere)."""
    areas = {}
    for j in entries:
        r = byhash.get(int(j["upload"], 16)) if j.get("upload") else None
        if r and r[3] is not None:
            areas.setdefault(r[0], set()).add(j.get("area"))
    best = max(areas.items(), key=lambda kv: len(kv[1]), default=(None, ()))
    return (_base(best[0]),) if len(best[1]) >= 3 else ()


def organize(out, data, log=print):
    """Moves every PNG of OUT into the readable tree (Zonas/.., Personajes/.., Interfaz/.., Pantallas/.., Comun/.., Sin identificar/..),
    named "<recurso>_<detalle>__<clave>.png", and rewrites textures.jsonl (file, key, resource).  Idempotent."""
    global PLAYER_WPP
    ip = os.path.join(out, "textures.jsonl")
    entries, seen = [], set()
    for ln in open(ip, encoding="utf-8"):
        try:
            j = json.loads(ln)
        except ValueError:
            continue
        k = key_of(os.path.basename(j["file"]))
        if k and k not in seen and os.path.isfile(os.path.join(out, j["file"])):
            seen.add(k)
            j["key"] = k
            entries.append(j)
    byhash, byrect = resource_index(data, log)
    PLAYER_WPP = _player_files(entries, byhash)
    named = 0
    for j in entries:
        parts, prefix, res = place_of(j, byhash, byrect)
        safe = lambda t: "".join(c if c.isalnum() or c in " -_~." else "_" for c in t).strip()
        rel = "/".join(safe(p) for p in parts) + "/" + safe(prefix).replace("__", "_") + "__" + j["key"] + ".png"
        if rel != j["file"]:
            dst = os.path.join(out, rel)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            os.replace(os.path.join(out, j["file"]), dst)
            j["file"] = rel
        j["resource"] = res
        named += res is not None
    with open(ip + ".tmp", "w", encoding="utf-8") as f:
        for j in sorted(entries, key=lambda j: j["file"]):
            f.write(json.dumps(j, ensure_ascii=False) + "\n")
    os.replace(ip + ".tmp", ip)
    for dp, dn, fn in sorted(os.walk(out, topdown=False)):     # the port's areaXX folders, now empty
        if dp != out and not dn and not fn:
            try:
                os.rmdir(dp)
            except OSError:
                pass
    with open(os.path.join(out, "LEEME.txt"), "w", encoding="utf-8") as f:
        f.write(LEEME)
    log("organized: %d textures, %d named after their disc resource" % (len(entries), named))
    return len(entries), named


LEEME = """Volcado completo de texturas de Tombi! (tools/dump_all_textures.py)
=====================================================================

Sale directamente de los ficheros del disco (carpeta data/), sin jugar.

Carpetas
  Zonas/NN Nombre/Escenario           paginas de los escenarios (ficheros Bnnn del disco)
  Zonas/NN Nombre/Objetos y enemigos  paginas de objetos, enemigos y personajes de la zona (Annn)
  Zonas/NN Nombre/Sprites/<fichero>   fotogramas de sprites animados de la zona
  Personajes/Tombi                    los fotogramas de Tombi
  Interfaz/...  Pantallas/...         HUD y fuentes; logos, titulo, menus, mapa, NOW LOADING
  Comun/...                           paginas y sprites del sistema comunes a todo el juego
  ".../<pagina> - sprites/"           cada sprite de esa pagina recortado y CON SU PALETA REAL
  ".../Partes con su paleta/"         cada parte de un fotograma (p.ej. cabeza y piernas de Tombi) con su paleta
  ".../Vista previa (solo para ver)/"  LAS PAGINAS COMO SE VEN EN EL JUEGO (RGBA): cada zona con la paleta con
                                      la que el juego la dibuja. Solo para mirar: el juego no las carga
                                      (no llevan "__r<clave>"); edita las de la carpeta de arriba.

Nombres
  <recurso>_p<X>-<Y>_<bits>__r<clave>.png  (pagina de VRAM X,Y de un fichero, 4/8 bits por pixel)
  <recurso>_f<NNNN>__r<clave>.png          (fotograma NNNN de un sprite)
  <sprite>__r<clave>_U_V_W_H.png          (un rectangulo U,V de W x H de esa pagina o fotograma)
  Solo importa lo que va tras el ultimo "__": el juego busca la clave en cualquier subcarpeta.
  Las paginas completas editables son PNG con paleta (indices): una pagina mezcla sprites con paletas
  distintas, asi que un solo PNG con paleta no puede mostrarlos todos bien; llevan la paleta que mas usa
  esa pagina. Para ver los colores reales abre "Vista previa (solo para ver)". Los fondos de habitacion
  (p.ej. B301 de la Aldea) son de 8 bits por pixel y salen con su paleta real.
  Los recortes de sprites llevan su paleta real. Edita el recorte (se pega encima de la pagina al cargar el pack) o la pagina entera.
  Puedes mover las imagenes o cambiar la parte de delante, pero no toques "__r<clave>.png".

Como hacer un pack HD
  1. Copia a una carpeta nueva solo las imagenes que quieras cambiar (con subcarpetas o sin ellas).
  2. Reescalalas al MISMO tamano multiplicado por un entero (2x, 4x, 8x... hasta 16x).
  3. IMPORTANTE: las imagenes son PNG con paleta (indices de color). Si las guardas COMO PNG CON PALETA
     (sin cambiar la paleta), el juego aplica en cada momento la paleta real: la misma pagina sale bien
     con todas sus paletas (enemigos de otro color, destellos, animaciones de color).
     Si las guardas en RGB/RGBA (color libre), se dibujan siempre con esos colores, sea cual sea la paleta.
  4. Transparencia (RGBA): alfa < 64 transparente, 64..191 semitransparente, >= 192 opaco.
  5. Lanzador: Mods > Pack de texturas (HD) > elige la carpeta. Solo con OpenGL; para un pack 4x usa
     resolucion interna 4. Linea de ordenes: --renderer gl --texture-pack CARPETA

Limites: algunos fondos que el juego compone o modifica antes de subirlos a la VRAM no salen del
disco tal cual y no se pueden reemplazar con este volcado (usa --dump-textures jugando para esos).
"""


def default_exe():
    try:
        import port_launcher
        p = port_launcher.game_binary()
        if p and os.path.isfile(p):
            return p
    except Exception:
        pass
    for p in ("build/port2/tombi2", "build/port2-win/tombi2.exe", "build/port2-win/tombi-pc.exe"):
        p = os.path.join(ROOT, p)
        if os.path.isfile(p):
            return p
    return os.path.join(ROOT, "build", "port2", "tombi2")


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--data", default=None, help="extracted disc folder (default: data/ of the checkout)")
    ap.add_argument("--out", default=None, help="default: texture_dump/ next to the data folder")
    ap.add_argument("--runtime", action="store_true", help="secondary: run the game headless through every room (--dump-textures keys)")
    ap.add_argument("--exe", default=None, help="--runtime: the port binary")
    ap.add_argument("--jobs", type=int, default=None)
    ap.add_argument("--quick", action="store_true", help="--runtime: rooms once, no item pop-ups")
    ap.add_argument("--only", choices=["rooms", "title", "menus", "items"])
    ap.add_argument("--raw", action="store_true", help="--runtime: keep the port's flat areaXX/<key>.png layout")
    ap.add_argument("--capture", default=None, help="draw capture of the port (tools/gfx_capture.py; default gfx_capture.drw.gz): exact palettes for the previews")
    ap.add_argument("--make-capture", action="store_true", help="run the port headless through every room first when there is no capture")
    a = ap.parse_args(argv)
    data = a.data or os.path.join(ROOT, "data")
    out = a.out or os.path.join(os.path.dirname(os.path.abspath(data)), "texture_dump")
    if not a.runtime:
        dump_static(out, data, capture=a.capture, make_capture=a.make_capture)
        return 0
    run(a.exe or default_exe(), out, data, None, a.jobs, a.quick, a.only, organize_data=None if a.raw else data)
    return 0


if __name__ == "__main__":
    sys.exit(main())
