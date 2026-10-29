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


def _png(path, w, h, rows, pal=None):
    """PNG of rows of 15-bit colour words (pal None: RGBA, alpha classes of texpack.c: 0x0000 transparent, STP 128, else 255) or of
    CLUT indices (pal = the palette words: a palette PNG, PLTE + tRNS with the same classes; the port reads the indices)."""
    import zlib, struct
    ch = lambda t, d: struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    alpha = lambda c: 0 if c == 0 else 128 if c & 0x8000 else 255
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


def dump_static(out, data, log=print, progress=None):
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

    def emit(parts_dir, prefix, key, w, h, rows, meta):
        nonlocal n
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
                    bpp = 8 if i in pr else 4
                    depth = (4, 8).index(bpp)
                    up = _fnv(d[:w * h * 2])
                    words = struct.unpack_from("<%dH" % (w * h), d)
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
                        emit(top, "%s_p%d-%d_%dbpp" % (short, x1 - x1 % 64, y1 - y1 % 256, bpp), key, (x2 - x1) * tpw, y2 - y1,
                             rows, dict(_pal=pal, kind="page", resource=nm, list="%s:%d" % (ld, k), depth=bpp, vram=[x1, y1, x2 - x1, y2 - y1],
                                        upload=[x, y, w, h], palette=pal_src))
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
        if progress:
            progress(wi, len(work))
    index.close()
    with open(os.path.join(out, "LEEME.txt"), "w", encoding="utf-8") as f:
        f.write(LEEME)
    log("%d images in %s (%.0f s)" % (n, out, time.time() - t0))
    return n


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

Nombres
  <recurso>_p<X>-<Y>_<bits>__r<clave>.png  (pagina de VRAM X,Y de un fichero, 4/8 bits por pixel)
  <recurso>_f<NNNN>__r<clave>.png          (fotograma NNNN de un sprite)
  Solo importa lo que va tras el ultimo "__": el juego busca la clave en cualquier subcarpeta.
  Puedes mover las imagenes o cambiar la parte de delante, pero no toques "__r<clave>.png".

Como hacer un pack HD
  1. Copia a una carpeta nueva solo las imagenes que quieras cambiar (con subcarpetas o sin ellas).
  2. Reescalalas al MISMO tamano multiplicado por un entero (2x, 4x, 8x... hasta 16x).
  3. IMPORTANTE: las imagenes son PNG con paleta (indices de color). Si las guardas COMO PNG CON PALETA
     (sin cambiar la paleta), el juego aplica en cada momento la paleta real: la misma pagina sale bien
     con todas sus paletas (enemigos de otro color, destellos, animaciones de color). Los colores que ves
     al abrirlas son solo una paleta de ejemplo.
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
    a = ap.parse_args(argv)
    data = a.data or os.path.join(ROOT, "data")
    out = a.out or os.path.join(os.path.dirname(os.path.abspath(data)), "texture_dump")
    if not a.runtime:
        dump_static(out, data)
        return 0
    run(a.exe or default_exe(), out, data, None, a.jobs, a.quick, a.only, organize_data=None if a.raw else data)
    return 0


if __name__ == "__main__":
    sys.exit(main())
