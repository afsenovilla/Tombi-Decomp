#!/usr/bin/env python3
"""Complete texture dump without playing: drives the native port headless through every screen and room.

Every texture the game draws is written by the port's own `--dump-textures` (port2/shims/texpack.c), so the file
names / keys are exactly the ones `--texture-pack` looks up (no reimplementation of the keying that could drift).
This script only decides *where* the game goes:

  * the title screen (with the intro logos, and with them skipped),
  * every room of the debug area select (port_mods.ROOM_COUNTS minus DOOR_ROOMS), twice: once as a new game and
    once with every event flag set (other objects / NPCs / doors), each time walking, jumping, attacking and
    opening the pause screen (the pad script of tools/gfx_capture.py),
  * the in-game menu screens (`--menu`, D0SPSUB/2/3: items, status, save/load, ...), paging through them,
  * every item pop-up (`--test-item`, the item icons) and the game-over screen.

Each run dumps into its own temporary folder; the results are merged into OUT (default texture_dump/ next to data/)
with the same layout the port writes (areaXX/<key>.png, sub_<key>.png, textures.jsonl): a name already present
anywhere in OUT is skipped, so the tool can be re-run or combined with dumps made while playing.

  python3 tools/dump_all_textures.py [--exe build/port2/tombi2] [--data DIR | --disc IMG] [--out DIR] [--jobs N]
                                     [--quick] [--only rooms|title|menus|items]

Known gaps (cannot be reached from the debug hooks): textures only uploaded during scripted events/boss fights
that the debug select does not start, the ending, and palette-animation frames (a palette that changes every frame
gives one key per palette seen: only those that happen to be on screen in a run are dumped)."""
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


def _fnv(b):
    h = 0xCBF29CE484222325
    for x in b:
        h = ((h ^ x) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h


def resource_index(data, log=print):
    """{upload hash: (resource, load list folder, list index, wpp frame or None)} and {(x, y, w, h): [(resource, folder)]}: every
    GRPX record of the load lists (the words LoadImage receives) and every WPP sprite frame, hashed like texpack.c."""
    import cdfiles, extract_data as X
    man = X.read_manifest(data)
    if man:
        lay = {p: (l, sz) for l, _n, _sb, sz, p in man["files"] if not p.startswith("_disc")}
        cdfiles.layout_from_image = lambda _p: lay
    import area000 as A
    A.FS, A._LISTS = data, None
    import vram, wpp
    vram._RES.clear()
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

Carpetas
  Zonas/NN Nombre/Escenario           paginas de los escenarios (ficheros Bnnn del disco)
  Zonas/NN Nombre/Objetos y enemigos  paginas de objetos, enemigos y PNJ de la zona (Annn)
  Zonas/NN Nombre/Sprites/<fichero>   fotogramas de sprites animados de la zona
  Personajes/Tombi                    los fotogramas de Tombi
  Interfaz/...                        HUD, fuentes, menus, objetos del inventario, game over
  Pantallas/...                       logos, titulo, mapa del mundo, menus de guardar/cargar
  Comun/...                           paginas y sprites comunes a todo el juego
  Sin identificar/NN Nombre           texturas que no se pudieron asociar a un fichero del disco
                                      (cajas compuestas de varias subidas, datos generados por el juego)

Nombres
  <recurso>_<detalle>__<clave>.png. Solo importa la clave (los 16 hexadecimales, o sub_ + 16, tras el
  ultimo "__"): el juego busca la clave en cualquier subcarpeta. Puedes mover o renombrar la parte de
  delante como quieras, pero no toques "__<clave>.png".
  _p X-Y = pagina de textura en VRAM, _pal X-Y = paleta (CLUT) con la que se dibuja: la misma pagina
  con otra paleta es otra imagen. _fNNNN = fotograma de sprite. Un "~" delante = asociado por posicion.
  textures.jsonl: una linea por imagen (fichero, clave, recurso del disco, tamano, VRAM, paleta, zona).

Como hacer un pack HD
  1. Copia a una carpeta nueva solo los PNG que quieras cambiar (puedes conservar las subcarpetas).
  2. Redibujalos al MISMO tamano multiplicado por un entero (2x, 4x, 8x... hasta 16x), igual ancho y alto.
     PNG RGBA de 8 bits por canal, sin entrelazado.
  3. Transparencia: alfa < 64 = transparente, 64..191 = semitransparente (bit STP), >= 192 = opaco.
     Respeta las zonas transparentes del original.
  4. En el lanzador: Parches y mods > Pack de texturas > elige la carpeta. Necesita el renderizador OpenGL;
     usa "resolucion interna" 4 para un pack 4x. Linea de ordenes: tombi --renderer gl --texture-pack CARPETA
  Las texturas que no esten en el pack se dibujan como siempre. Si una paleta cambia en el juego
  (destellos, animaciones de color) se dibuja la original mientras dure.
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
    ap.add_argument("--exe", default=None)
    ap.add_argument("--data", default=None, help="extracted disc folder (default: data/ of the checkout)")
    ap.add_argument("--disc")
    ap.add_argument("--out", default=None, help="default: texture_dump/ next to the data folder")
    ap.add_argument("--jobs", type=int, default=None)
    ap.add_argument("--quick", action="store_true", help="rooms once (no event-flag pass), no item pop-ups")
    ap.add_argument("--only", choices=["rooms", "title", "menus", "items"])
    ap.add_argument("--list", action="store_true", help="print the runs and exit")
    ap.add_argument("--raw", action="store_true", help="keep the port's flat areaXX/<key>.png layout (no readable folders)")
    ap.add_argument("--organize-only", action="store_true", help="only sort an existing dump in OUT into the readable folders")
    a = ap.parse_args(argv)
    if a.list:
        for lb, args, fr in jobs(a.quick, a.only):
            print("%-22s %5d  %s" % (lb, fr, " ".join(args)))
        return 0
    data = a.data or (None if a.disc else os.path.join(ROOT, "data"))
    out = a.out or os.path.join(os.path.dirname(os.path.abspath(data or a.disc)), "texture_dump")
    names = None if a.raw or not data else data
    if a.organize_only:
        organize(out, names)
        return 0
    run(a.exe or default_exe(), out, data, a.disc, a.jobs, a.quick, a.only, organize_data=names)
    return 0


if __name__ == "__main__":
    sys.exit(main())
