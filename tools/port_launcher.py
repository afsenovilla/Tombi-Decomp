#!/usr/bin/env python3
"""Launcher of the native-pointer port (tombi-port.exe, docs/PORT.md "M3 phase 3" / "Mods"): a window where the
player picks their own disc image (.bin/.cue/.iso/.chd of Tombi! PAL SCES-01331), a save folder, the window options and
the mods, and a Play button that runs tombi-pc.exe --window with those options.  Same Tk toolkit and tools/i18n.py
catalogs as tombi-gfx / tombi-text.  No game data is bundled: the disc is checked (MAIN0.EXE extracted next to
tombi-port.json, the saved settings, and compared by sha1) before the first run, and the disc is then extracted
once into data/ next to the launcher (tools/extract_data.py): the game runs with --data and no longer needs the
image (the disc field is optional while data/ is valid for the same MAIN0.EXE).

  python3 tools/port_launcher.py             # from a checkout: uses build/port2-win/tombi2.exe (Windows) or build/port2/tombi2 (Linux)
  pyinstaller --onefile --windowed --name tombi-port ... tools/port_launcher.py   (.github/workflows/port2-exe.yml)
"""
import hashlib, re, json, os, struct, subprocess, sys, threading
import tkinter as tk
from tkinter import ttk, filedialog, messagebox

import i18n
from i18n import tr
# the port launcher speaks English unless the player picked a language in it (i18n.selector saves the choice)
if i18n.load_settings().get("language") not in i18n.LANGS:
    i18n.set_language("en", save=False)
import port_mods as M
import extract_data as X
import data_patch as DP

MAIN0_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc"     # EXE/MAIN0.EXE of SCES-01331 (.github/workflows/report.yml)
# the languages of the disc's boot menu: EXE/MAINn.EXE (tombi2 --lang N, docs/PORT.md "Languages")
GAME_LANGS = [(0, "English"), (5, "Español"), (6, "Português"), (7, "Svenska"), (8, "Dansk")]


def flag_image(n, w=24, h=16):
    """A small drawn flag for a game language (tk.PhotoImage; no files, no emoji: Tk on Windows draws neither)."""
    img = tk.PhotoImage(width=w, height=h)
    def fill(x0, y0, x1, y1, c):
        img.put(c, to=(max(0, x0), max(0, y0), min(w, x1), min(h, y1)))
    if n == 0:        # United Kingdom (simplified: blue field, white and red crosses)
        fill(0, 0, w, h, "#012169")
        for i in range(h):           # diagonals in white
            for x in (i * w // h, w - 1 - i * w // h):
                fill(x - 1, i, x + 2, i + 1, "white")
        fill(0, h // 2 - 3, w, h // 2 + 3, "white"); fill(w // 2 - 3, 0, w // 2 + 3, h, "white")
        fill(0, h // 2 - 2, w, h // 2 + 2, "#C8102E"); fill(w // 2 - 2, 0, w // 2 + 2, h, "#C8102E")
    elif n == 5:      # Spain
        fill(0, 0, w, h, "#AA151B"); fill(0, h // 4, w, h - h // 4, "#F1BF00")
    elif n == 6:      # Portugal
        fill(0, 0, w * 2 // 5, h, "#006600"); fill(w * 2 // 5, 0, w, h, "#FF0000")
        cx, cy, r = w * 2 // 5, h // 2, h // 4
        for y in range(h):
            for x in range(w):
                d = (x - cx) ** 2 + (y - cy) ** 2
                if r * r - 2 * r <= d <= r * r + 2 * r: img.put("#FFE000", to=(x, y, x + 1, y + 1))
    elif n == 7:      # Sweden
        fill(0, 0, w, h, "#006AA7"); fill(0, h // 2 - 2, w, h // 2 + 2, "#FECC02"); fill(w * 3 // 8 - 2, 0, w * 3 // 8 + 2, h, "#FECC02")
    elif n == 8:      # Denmark
        fill(0, 0, w, h, "#C8102E"); fill(0, h // 2 - 2, w, h // 2 + 2, "white"); fill(w * 3 // 8 - 2, 0, w * 3 // 8 + 2, h, "white")
    return img
UI_TO_GAME_LANG = {"en": 0, "es": 5, "pt": 6, "sv": 7, "da": 8}
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

i18n.add({
    "es": {
        "l.lang": "Idioma del juego:",
        "title": "Tombi! - port nativo", "tab.play": "Jugar", "tab.mods": "Mods", "tab.controls": "Controles",
        "l.disc": "Imagen del disco (.bin / .cue / .iso / .chd; se extrae una vez a la carpeta data/):", "b.browse": "Elegir...", "l.savedir": "Carpeta de partidas guardadas:",
        "l.scale": "Tamaño de la ventana:", "l.fullscreen": "Pantalla completa (F11 en el juego)", "l.hz": "Velocidad:",
        "l.ires": "Resolución interna (solo OpenGL):", "r.ires1": "1x (original)",
        "l.renderer": "Dibujado:", "r.gl": "OpenGL (tarjeta gráfica)", "r.soft": "Software (CPU, más lento)", "l.aspect": "Imagen:", "a.tv": "4:3 (como en la tele)", "a.sq": "Píxeles cuadrados", "a.w16": "16:9 panorámica (experimental)", "a.w21": "21:9 (experimental)", "a.w32": "32:9 (experimental)",
        "l.titlebg": "Fondo de título panorámico (PNG / JPG):", "l.titlebg.d": "Solo se usa con imagen panorámica (16:9, 21:9, 32:9): rellena los lados de la pantalla de título.", "ft.image": "Imágenes",
        "c.texpack": "Pack de texturas (HD) · experimental", "l.texpack": "Carpeta del pack (déjalo vacío para usar las texturas originales). Solo con OpenGL.",
        "b.dumpall": "Volcar todas las texturas", "b.openfolder": "Abrir carpeta",
        "l.texpack.d": "Pulsa «Volcar todas las texturas» para exportar las texturas del juego a la carpeta texture_dump. Edítalas o reescálalas (2x, 4x…) manteniendo el nombre del archivo y elige esa carpeta como pack.",
        "s.dumpall": "Volcando texturas… {}%", "s.dumpallok": "Listo: {} texturas en {}", "s.dumpallfail": "No se pudo volcar: {}",
        "l.pad": "Mando: cualquier mando compatible con SDL2 (Xbox, PS4/PS5, Switch Pro) se usa automáticamente; teclado: cursores, X = Cruz, Z = Cuadrado, S = Triángulo, D = Círculo, Q/W = L1/R1, A/E = L2/R2, Intro = Start, Retroceso = Select.",
        "b.play": "Jugar", "b.extracting": "Extrayendo... {0}%", "l.extracting": "Extrayendo los archivos del disco (solo la primera vez). Espera a que termine para jugar.", "b.stop": "Cerrar el juego", "b.copylog": "Copiar el registro", "s.copied": "Registro copiado al portapapeles ({0} líneas).", "l.autostart": "Empezar partida nueva automáticamente (pulsa Start en el título)",
        "s.nodisc": "Elige tu imagen del disco.", "s.checking": "Comprobando el disco...", "s.ok": "Disco compatible: {0}, MAIN0.EXE verificado (sha1).",
        "s.extracting": "Extrayendo el disco a {0}... {1}% ({2})", "s.dataok": "Datos del juego listos en {0} ({1} archivos): la imagen del disco ya no hace falta.",
        "s.extractfail": "No se pudo extraer el disco: {0}", "s.nodata": "Elige tu imagen del disco: se extrae una vez a la carpeta data/ y el juego ya no la necesita.",
        "s.badsha": "Disco NO compatible: {0} (sha1 de MAIN0.EXE {1}). El port solo funciona con Tombi! PAL SCES-01331 Rev 1 (si es SCES-01331, será la Rev 0); puede fallar.",
        "s.noexe": "No se encuentra EXE/MAIN0.EXE en la imagen: no es un disco de Tombi! PAL.", "s.notiso": "No es una imagen ISO9660: {0}",
        "s.running": "Jugando... (cerrar la ventana del juego termina)", "s.ended": "El juego terminó (código {0}). Registro: {1}",
        "s.nobin": "No se encuentra el binario del port ({0}). Compílalo (docs/PORT.md) o usa el exe distribuido.",
        "l.log": "Últimas líneas del registro del port:", "l.modshelp": "Cada mod es una opción del port (--poke, --hz...); se guardan en tombi-port.json.",
        "l.key": "Tecla", "b.defaults": "Valores por defecto", "l.cmd": "Comando:",
        "l.patch": "Parche de texto:", "b.nopatch": "Quitar",
        "l.patchhelp": "Un parche *.tombipatch.json (tombi-text, «Generar parche») se aplica a la carpeta data/ al pulsar Jugar; los originales se guardan en data/.orig/ y se restauran al quitarlo o cambiarlo. Incluido: {0}.",
        "l.patchnone": "ninguno", "s.patching": "Aplicando el parche de texto «{0}» a data/...",
        "s.patchok": "Parche de texto «{0}» aplicado ({1} archivos de data/; {2}).", "s.patchon": "Parche de texto activo: «{0}».",
        "s.patchoff": "Parche de texto quitado: data/ vuelve a tener los archivos originales ({0}).",
        "s.patchfail": "No se pudo aplicar el parche de texto: {0}", "s.patchmissing": "No se encuentra el parche de texto: {0}",
        "s.patchnodata": "El parche de texto se aplica cuando el disco está extraído en data/ (elige la imagen del disco).",
        "s.patchchk.disc": "sha1 del disco comprobado", "s.patchchk.main0": "comprobado MAIN0.EXE de data/; elige la imagen .bin para comprobar también el sha1 del disco",
        "s.patchmoved": "{0} archivos crecieron más que su sitio en el disco y se movieron al final (se leen con la tabla de archivos del idioma)",
    },
    "en": {
        "l.lang": "Game language:",
        "title": "Tombi! - native port", "tab.play": "Play", "tab.mods": "Mods", "tab.controls": "Controls",
        "l.disc": "Disc image (.bin / .cue / .iso / .chd; extracted once into the data/ folder):", "b.browse": "Browse...", "l.savedir": "Save folder (memory cards):",
        "l.scale": "Window size:", "l.fullscreen": "Fullscreen (F11 in game)", "l.hz": "Speed:",
        "l.ires": "Internal resolution (OpenGL only):", "r.ires1": "1x (original)",
        "l.renderer": "Renderer:", "r.gl": "OpenGL (graphics card)", "r.soft": "Software (CPU, slower)", "l.aspect": "Picture:", "a.tv": "4:3 (as on a TV)", "a.sq": "Square pixels", "a.w16": "16:9 widescreen (experimental)", "a.w21": "21:9 (experimental)", "a.w32": "32:9 (experimental)",
        "l.titlebg": "Wide title background (PNG / JPG):", "l.titlebg.d": "Only used in widescreen (16:9, 21:9, 32:9): fills the sides of the title screen.", "ft.image": "Images",
        "c.texpack": "Texture pack (HD) · experimental", "l.texpack": "Pack folder (leave empty for the original textures). OpenGL only.",
        "b.dumpall": "Dump all textures", "b.openfolder": "Open folder",
        "l.texpack.d": "Press “Dump all textures” to export the game's textures to the texture_dump folder. Edit or upscale them (2x, 4x…) keeping the file names, then choose that folder as the pack.",
        "s.dumpall": "Dumping textures… {}%", "s.dumpallok": "Done: {} textures in {}", "s.dumpallfail": "Could not dump: {}",
        "l.pad": "Controller: any SDL2-compatible pad (Xbox, PS4/PS5, Switch Pro) is used automatically; keyboard: arrows, X = Cross, Z = Square, S = Triangle, D = Circle, Q/W = L1/R1, A/E = L2/R2, Enter = Start, Backspace = Select.",
        "b.play": "Play", "b.extracting": "Extracting... {0}%", "l.extracting": "Extracting the disc files (first time only). Wait for it to finish to play.", "b.stop": "Close the game", "b.copylog": "Copy log", "s.copied": "Log copied to the clipboard ({0} lines).", "l.autostart": "Start a new game automatically (presses Start on the title)",
        "s.nodisc": "Pick your disc image.", "s.checking": "Checking the disc...", "s.ok": "Compatible disc: {0}, MAIN0.EXE verified (sha1).",
        "s.extracting": "Extracting the disc into {0}... {1}% ({2})", "s.dataok": "Game data ready in {0} ({1} files): the disc image is no longer needed.",
        "s.extractfail": "Could not extract the disc: {0}", "s.nodata": "Pick your disc image: it is extracted once into the data/ folder and the game no longer needs it.",
        "s.badsha": "NOT a compatible disc: {0} (MAIN0.EXE sha1 {1}). The port only runs Tombi! PAL SCES-01331 Rev 1 (an SCES-01331 here is Rev 0); it may fail.",
        "s.noexe": "EXE/MAIN0.EXE not found in the image: not a Tombi! PAL disc.", "s.notiso": "Not an ISO9660 image: {0}",
        "s.running": "Playing... (closing the game window ends it)", "s.ended": "The game ended (code {0}). Log: {1}",
        "s.nobin": "Port binary not found ({0}). Build it (docs/PORT.md) or use the distributed exe.",
        "l.log": "Last lines of the port's log:", "l.modshelp": "Each mod is a port option (--poke, --hz...); they are saved in tombi-port.json.",
        "l.key": "Key", "b.defaults": "Defaults", "l.cmd": "Command:",
        "l.patch": "Text patch:", "b.nopatch": "Clear",
        "l.patchhelp": "A *.tombipatch.json patch (tombi-text, \"Make patch\") is applied to the data/ folder when you press Play; the originals are kept in data/.orig/ and put back when it is cleared or changed. Bundled: {0}.",
        "l.patchnone": "none", "s.patching": "Applying the text patch \"{0}\" to data/...",
        "s.patchok": "Text patch \"{0}\" applied ({1} files of data/; {2}).", "s.patchon": "Text patch active: \"{0}\".",
        "s.patchoff": "Text patch removed: data/ has the original files again ({0}).",
        "s.patchfail": "Could not apply the text patch: {0}", "s.patchmissing": "Text patch not found: {0}",
        "s.patchnodata": "The text patch is applied once the disc is extracted into data/ (pick the disc image).",
        "s.patchchk.disc": "disc sha1 checked", "s.patchchk.main0": "MAIN0.EXE of data/ checked; pick the .bin image to check the disc sha1 too",
        "s.patchmoved": "{0} files grew beyond their place on the disc and were moved to its end (read through the language's own file table)",
    },
    "fr": {"l.lang": "Langue du jeu :", "title": "Tombi! - port natif", "tab.play": "Jouer", "tab.mods": "Mods", "tab.controls": "Commandes", "b.play": "Jouer",
           "l.disc": "Image du disque (.bin / .cue / .iso / .chd ; extraite une fois dans le dossier data/) :", "b.browse": "Parcourir...", "l.savedir": "Dossier des sauvegardes :",
           "s.extracting": "Extraction du disque vers {0}... {1} % ({2})", "s.dataok": "Données du jeu prêtes dans {0} ({1} fichiers) : l'image du disque n'est plus nécessaire.",
           "s.extractfail": "Impossible d'extraire le disque : {0}", "s.nodata": "Choisissez votre image du disque : elle est extraite une fois dans le dossier data/ et le jeu n'en a plus besoin."},
    "de": {"l.lang": "Spielsprache:", "title": "Tombi! - nativer Port", "tab.play": "Spielen", "tab.mods": "Mods", "tab.controls": "Steuerung", "b.play": "Spielen",
           "l.disc": "Disc-Image (.bin / .cue / .iso / .chd; wird einmal in den Ordner data/ entpackt):", "b.browse": "Durchsuchen...", "l.savedir": "Speicherordner:",
           "s.extracting": "Disc wird nach {0} entpackt... {1} % ({2})", "s.dataok": "Spieldaten bereit in {0} ({1} Dateien): das Disc-Image wird nicht mehr gebraucht.",
           "s.extractfail": "Die Disc konnte nicht entpackt werden: {0}", "s.nodata": "Wähle dein Disc-Image: es wird einmal in den Ordner data/ entpackt, danach braucht das Spiel es nicht mehr."},
    "it": {"l.lang": "Lingua del gioco:", "title": "Tombi! - port nativo", "tab.play": "Gioca", "tab.mods": "Mod", "tab.controls": "Controlli", "b.play": "Gioca",
           "l.disc": "Immagine del disco (.bin / .cue / .iso / .chd; estratta una volta nella cartella data/):", "b.browse": "Sfoglia...", "l.savedir": "Cartella dei salvataggi:",
           "s.extracting": "Estrazione del disco in {0}... {1}% ({2})", "s.dataok": "Dati del gioco pronti in {0} ({1} file): l'immagine del disco non serve più.",
           "s.extractfail": "Impossibile estrarre il disco: {0}", "s.nodata": "Scegli l'immagine del disco: viene estratta una volta nella cartella data/ e il gioco non ne ha più bisogno."},
    "pt": {"l.lang": "Idioma do jogo:", "title": "Tombi! - port nativo", "tab.play": "Jogar", "tab.mods": "Mods", "tab.controls": "Controlos", "b.play": "Jogar",
           "l.disc": "Imagem do disco (.bin / .cue / .iso / .chd; extraída uma vez para a pasta data/):", "b.browse": "Procurar...", "l.savedir": "Pasta dos jogos guardados:",
           "s.extracting": "A extrair o disco para {0}... {1}% ({2})", "s.dataok": "Dados do jogo prontos em {0} ({1} ficheiros): a imagem do disco já não é necessária.",
           "s.extractfail": "Não foi possível extrair o disco: {0}", "s.nodata": "Escolhe a imagem do disco: é extraída uma vez para a pasta data/ e o jogo deixa de precisar dela."},
})


# ---------------------------------------------------------------------------------------------- files
def app_dir():
    """Folder of the launcher: next to the exe when frozen (settings, MAIN0.EXE, log), else the repo's build/."""
    if getattr(sys, "frozen", False):
        return os.path.dirname(os.path.abspath(sys.executable))
    d = os.path.join(ROOT, "build", "launcher")
    os.makedirs(d, exist_ok=True)
    return d


def bin_dir():
    """Where tombi2(.exe) and SDL2.dll live: the PyInstaller bundle, else the build tree."""
    if getattr(sys, "_MEIPASS", None):
        return sys._MEIPASS
    if os.name == "nt":
        return os.path.join(ROOT, "build", "port2-win")
    return os.path.join(ROOT, "build", "port2")


def game_binary():
    if os.name == "nt":         # tombi-pc.exe: the game (the CMake target is still called tombi2); older bundles: tombi2.exe
        p = os.path.join(bin_dir(), "tombi-pc.exe")
        return p if os.path.isfile(p) else os.path.join(bin_dir(), "tombi2.exe")
    p = os.path.join(bin_dir(), "tombi-pc")     # the Linux bundle (dist/linux/tombi-port); the build tree: tombi2
    if not os.path.isfile(p):
        return os.path.join(bin_dir(), "tombi2")
    if not os.access(p, os.X_OK):               # (PyInstaller extraction may drop the exec bit)
        try:
            os.chmod(p, 0o755)
        except OSError:
            pass
    return p


def settings_file():
    return os.path.join(app_dir(), "tombi-port.json")


def data_dir():
    """Where the disc is extracted once (tools/extract_data.py): data/ next to the launcher, which tombi2 reads with --data."""
    return os.path.join(app_dir(), "data")


def data_ready():
    """sha1 of MAIN0.EXE of a complete extraction in data_dir(), else None."""
    return X.manifest_sha1(data_dir())


def bundled_patch():
    """patches/es_mejorada.tombipatch.json: bundled in the exe (copied next to it, so the saved path stays valid) or in the checkout."""
    name = "es_mejorada.tombipatch.json"
    src = os.path.join(getattr(sys, "_MEIPASS", None) or ROOT, "patches", name)
    if not os.path.isfile(src):
        return None
    if not getattr(sys, "frozen", False):
        return src
    dst = os.path.join(app_dir(), "patches", name)
    try:
        data = open(src, "rb").read()
        if not os.path.isfile(dst) or open(dst, "rb").read() != data:
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            open(dst, "wb").write(data)
        return dst
    except OSError:
        return src


def load_settings():
    s = M.default_settings()
    s.update({"disc": "", "savedir": "", "scale": 2, "fullscreen": False, "renderer": "gl", "patch_on": False, "patch": bundled_patch() or ""})
    try:
        s.update(json.load(open(settings_file(), encoding="utf-8")))
    except (OSError, ValueError):
        pass
    return s


def save_settings(s):
    try:
        json.dump(s, open(settings_file(), "w", encoding="utf-8"), indent=1)
    except OSError:
        pass


def bin_of_cue(path):
    """The data track file named by a .cue sheet (next to it)."""
    if not path.lower().endswith(".cue"):
        return path
    try:
        for line in open(path, encoding="utf-8", errors="replace"):
            line = line.strip()
            if line.upper().startswith("FILE "):
                name = line[5:].rsplit(" ", 1)[0].strip().strip('"')
                return os.path.join(os.path.dirname(path), name)
    except OSError:
        pass
    return path


def extract_main0(image, out):
    """EXE/MAIN0.EXE from the ISO9660 directory of a raw (2352) or cooked (2048) image (or a .chd) -> out; returns its sha1."""
    f = X.open_image(image)
    f.seek(16 * 2352 + 25)
    raw = f.read(5) == b"CD001"
    ss, uo = (2352, 24) if raw else (2048, 0)

    def sec(n, cnt=1):
        b = []
        for i in range(cnt):
            f.seek((n + i) * ss + uo)
            b.append(f.read(2048))
        return b"".join(b)
    pvd = sec(16)
    if pvd[1:6] != b"CD001":
        f.close()
        raise ValueError(tr("s.notiso", os.path.basename(image)))
    found = {}

    def walk(lba, size, pre):
        data = sec(lba, (size + 2047) // 2048)
        p = 0
        while p < size:
            n = data[p]
            if n == 0:
                p = (p // 2048 + 1) * 2048
                continue
            r = data[p:p + n]
            p += n
            elba, esz = struct.unpack("<I", r[2:6])[0], struct.unpack("<I", r[10:14])[0]
            nm = r[33:33 + r[32]]
            if nm in (b"\0", b"\1"):
                continue
            nm = nm.decode(errors="replace")
            if r[25] & 2:
                walk(elba, esz, pre + nm + "/")
            else:
                found[(pre + nm.split(";")[0]).upper()] = (elba, esz)
    root = pvd[156:190]
    walk(struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], "")
    if "EXE/MAIN0.EXE" not in found:
        f.close()
        raise ValueError(tr("s.noexe"))
    serial = None                   # the boot file named by SYSTEM.CNF: BOOT = cdrom:\\SCES_013.31;1 -> SCES-01331
    if "SYSTEM.CNF" in found:
        lba, size = found["SYSTEM.CNF"]
        m = re.search(r"([A-Z]{4})[_-](\d{3})\.?(\d{2})", sec(lba)[:size].decode("latin-1").upper())
        if m:
            serial = "%s-%s%s" % m.groups()
    lba, size = found["EXE/MAIN0.EXE"]
    data = sec(lba, (size + 2047) // 2048)[:size]
    f.close()
    with open(out, "wb") as o:
        o.write(data)
    return hashlib.sha1(data).hexdigest(), serial


# serials of the other Tomba! releases (the port runs only SCES-01331)
KNOWN_DISCS = {"SCES-01331": "Tombi! (PAL, Europa)",  # Rev 1 is the one the port is built from; Rev 0 has another MAIN0.EXE
               "SCES-01330": "Tombi! (PAL)", "SCUS-94236": "Tomba! (USA)",
               "SLPS-01144": "Ore no Tomba (Japón)", "SCUS-94454": "Tomba! 2 (USA)", "SCES-02005": "Tombi! 2 (PAL)"}


def build_id():
    """Short git sha of the build: build_id.txt bundled by port2-exe.yml, else `git rev-parse` of the checkout."""
    for d in (getattr(sys, "_MEIPASS", None), ROOT):
        if d and os.path.isfile(os.path.join(d, "build_id.txt")):
            return open(os.path.join(d, "build_id.txt"), encoding="utf-8", errors="replace").read().strip() or "unknown"
    try:
        return subprocess.check_output(["git", "-C", ROOT, "rev-parse", "--short", "HEAD"], text=True,
                                       stderr=subprocess.DEVNULL).strip() or "unknown"
    except Exception:
        return "unknown"


BUILD_ID = build_id()


def set_icon(root):
    for d in (os.path.join(getattr(sys, "_MEIPASS", ""), "icons"), os.path.join(ROOT, "tools", "icons")):
        for name in ("launcher", "port", "gfx"):
            ico, png = os.path.join(d, name + ".ico"), os.path.join(d, name + ".png")
            try:
                if os.name == "nt" and os.path.isfile(ico):
                    root.iconbitmap(ico)
                    return
                if os.path.isfile(png):
                    root._icon_img = tk.PhotoImage(file=png)
                    root.iconphoto(True, root._icon_img)
                    return
            except Exception:
                pass


# ---------------------------------------------------------------------------------------------- window
# --cut-content (port/fixes.txt): text cut from the game, recovered from the Japanese release
i18n.add({
    "es": {"l.cutcontent": "Activar contenido recortado",
           "i.cutcontent": "Textos que se quitaron del juego y que se han encontrado en la versión japonesa (Ore! Tomba). "
                           "Se activan dos:\n\n"
                           "• El barquero de la Ciudad de todo principio (AREA00): en el estanque al que llegas sobre la tortuga, "
                           "tras su «¡Hasta luego... o hasta nunca!» dice tres frases más (una puerta cerrada con llave, la cerradura "
                           "de los hombres de hierro y su barca en la playa del Bosque de las setas).\n"
                           "• Una carta de Ciudad Baco en la Montaña Fénix (AREA03): un segundo cartel a la izquierda del de guardar "
                           "partida, al principio de la zona; se lee igual que este (arriba y hablar).\n\n"
                           "Mencionan lugares y cosas que no existen en el juego final (el País de la Civilización, esa cerradura, "
                           "la barca).\n\n"
                           "Los textos están en español con el parche «Traducción mejorada»; en los demás idiomas, o sin el parche, "
                           "esos huecos del disco solo contienen un relleno («!!!!!!!!!!», «..........???»)."},
    "en": {"l.cutcontent": "Enable cut content",
           "i.cutcontent": "Text that was removed from the game and found in the Japanese release (Ore! Tomba). "
                           "Two pieces are enabled:\n\n"
                           "• The boatman in the Village of All Beginnings (AREA00): at the pond you reach on the turtle, after his "
                           "farewell he says three more lines (a locked door, a lock made by the Iron Men, his boat on the Mushroom "
                           "Forest beach).\n"
                           "• A letter from Baccus Village on Phoenix Mountain (AREA03): a second sign to the left of the save sign at "
                           "the start of the area, read the same way (Up, then talk).\n\n"
                           "They mention places and things that do not exist in the final game (the Land of Civilization, that lock, "
                           "the boat).\n\n"
                           "The texts are in Spanish with the 'Improved translation' patch; in the other languages, or without the "
                           "patch, those slots of the disc only hold filler ('!!!!!!!!!!', '..........???')."},
})

# the strings of the sidebar layout (the older keys above stay: status messages, options, mods)
i18n.add({
    "es": {"l.experimental": "Mostrar funciones experimentales", "l.fps60": "60 fps (experimental)", "nav.play": "Jugar", "nav.gfx": "Gráficos", "nav.controls": "Controles", "nav.mods": "Parches y mods", "nav.log": "Registro",
           "l.uilang": "Idioma del launcher", "l.starting": "Iniciando el juego...", "c.disc": "Disco", "c.saves": "Partidas", "c.game": "Idioma del juego",
           "c.patch": "Parche de texto", "c.speed": "Velocidad", "c.quick": "Opciones rápidas", "l.skipintro": "Saltar logos e intro",
           "l.autostart2": "Empezar partida automáticamente", "p.none": "Ninguno (texto original)", "l.patchhint": "Recomendado con el juego en español: Traducción mejorada (incluida).", "p.bundled": "Traducción mejorada (incluida)",
           "p.custom": "Otro archivo...", "sum.window": "{0}x ventana", "sum.full": "pantalla completa",
           "sum.ires": "{0}x interna", "c.renderer": "Dibujado", "c.window": "Ventana", "c.aspect": "Imagen", "c.ires": "Resolución interna",
           "l.iresoff": "Solo con OpenGL: el dibujado por software usa siempre la resolución original.",
           "c.keys": "Teclado", "c.pad": "Mando", "c.mods": "Mods", 
           "c.cmd": "Línea de comandos", "c.log": "Últimas líneas del registro", "c.status": "Estado"},
    "en": {"l.experimental": "Show experimental features", "l.fps60": "60 fps (experimental)", "nav.play": "Play", "nav.gfx": "Graphics", "nav.controls": "Controls", "nav.mods": "Patches and mods", "nav.log": "Log",
           "l.uilang": "Launcher language", "l.starting": "Starting the game...", "c.disc": "Disc", "c.saves": "Saves", "c.game": "Game language",
           "c.patch": "Text patch", "c.speed": "Speed", "c.quick": "Quick options", "l.skipintro": "Skip logos and intro",
           "l.autostart2": "Start a new game automatically", "p.none": "None (original text)", "l.patchhint": "Recommended with the game in Spanish: Improved translation (bundled).", "p.bundled": "Improved translation (bundled)",
           "p.custom": "Other file...", "sum.window": "{0}x window", "sum.full": "fullscreen",
           "sum.ires": "{0}x internal", "c.renderer": "Renderer", "c.window": "Window", "c.aspect": "Picture", "c.ires": "Internal resolution",
           "l.iresoff": "OpenGL only: the software renderer always draws at the original resolution.",
           "c.keys": "Keyboard", "c.pad": "Controller", "c.mods": "Mods", 
           "c.cmd": "Command line", "c.log": "Last lines of the log", "c.status": "Status"},
})

# design "A": Windows 11-like light window, grey sidebar, white cards
C_BG, C_SIDE, C_CARD, C_LINE = "#f3f3f5", "#ebecef", "#ffffff", "#e3e4e8"
C_TEXT, C_MUTED, C_ACCENT, C_ACCENT_HI = "#1b1b1f", "#5f636b", "#1f5fbf", "#2b6fd6"
DOT = {"ok": "#1e9e4a", "bad": "#d13438", "busy": "#e08a00", "info": "#8a8f98"}
PAGES = ["play", "gfx", "controls", "mods", "log"]


def ui_font():
    from tkinter import font
    fams = set(font.families())
    for f in ("Segoe UI", "Noto Sans", "DejaVu Sans", "Liberation Sans", "Helvetica"):
        if f in fams:
            return f
    return font.nametofont("TkDefaultFont").actual("family")


def setup_style(root):
    """The clam theme recoloured (clam lets every colour change, vista does not) + the named fonts."""
    from tkinter import font
    fam = ui_font()
    for name, size, weight in (("TkDefaultFont", 10, "normal"), ("TkTextFont", 10, "normal"), ("TkMenuFont", 10, "normal"),
                               ("TkHeadingFont", 10, "bold")):
        try:
            font.nametofont(name).configure(family=fam, size=size, weight=weight)
        except tk.TclError:
            pass
    root.configure(background=C_BG)
    st = ttk.Style(root)
    try:
        st.theme_use("clam")
    except tk.TclError:
        pass
    st.configure(".", background=C_CARD, foreground=C_TEXT, font=(fam, 10), bordercolor=C_LINE, lightcolor=C_CARD,
                 darkcolor=C_CARD, troughcolor=C_LINE, focuscolor=C_ACCENT, selectbackground=C_ACCENT, fieldbackground=C_CARD)
    st.configure("TFrame", background=C_CARD)
    st.configure("Bg.TFrame", background=C_BG)
    st.configure("Side.TFrame", background=C_SIDE)
    st.configure("TLabel", background=C_CARD)
    st.configure("Muted.TLabel", foreground=C_MUTED)
    st.configure("Head.TLabel", font=(fam, 10, "bold"))
    st.configure("Title.TLabel", background=C_BG, font=(fam, 18, "bold"))
    st.configure("Bg.TLabel", background=C_BG, foreground=C_MUTED)
    st.configure("Side.TLabel", background=C_SIDE, foreground=C_MUTED, font=(fam, 9))
    st.configure("App.TLabel", background=C_SIDE, font=(fam, 11, "bold"))
    for w in ("TCheckbutton", "TRadiobutton"):
        st.configure(w, background=C_CARD, indicatorbackground=C_CARD, indicatorforeground=C_ACCENT, indicatorsize=13, upperbordercolor=C_LINE,
                     lowerbordercolor=C_LINE)
        st.map(w, background=[("active", C_CARD)], indicatorbackground=[("disabled", C_BG), ("pressed", C_LINE)])
    st.configure("TButton", background="#fbfbfc", bordercolor="#d0d2d8", lightcolor="#fbfbfc", darkcolor="#fbfbfc", padding=(10, 4))
    st.map("TButton", background=[("pressed", "#e8e9ec"), ("active", "#f0f1f4")], lightcolor=[("active", "#f0f1f4")],
           darkcolor=[("active", "#f0f1f4")])
    st.configure("Accent.TButton", background=C_ACCENT, foreground="white", bordercolor=C_ACCENT, lightcolor=C_ACCENT,
                 darkcolor=C_ACCENT, font=(fam, 11, "bold"), padding=(26, 8))
    st.map("Accent.TButton", background=[("disabled", "#9db4d9"), ("pressed", "#174a96"), ("active", C_ACCENT_HI)],
           lightcolor=[("disabled", "#9db4d9"), ("active", C_ACCENT_HI)], darkcolor=[("disabled", "#9db4d9"), ("active", C_ACCENT_HI)],
           bordercolor=[("disabled", "#9db4d9")], foreground=[("disabled", "#eef2f8")])
    st.configure("Bar.TButton", padding=(14, 8))
    # sidebar entries: flat on grey, the current one a white pill (Nav.Sel)
    st.configure("Nav.TButton", background=C_SIDE, bordercolor=C_SIDE, lightcolor=C_SIDE, darkcolor=C_SIDE, anchor="w",
                 padding=(14, 7), font=(fam, 10))
    st.map("Nav.TButton", background=[("active", "#e1e2e6")], lightcolor=[("active", "#e1e2e6")], darkcolor=[("active", "#e1e2e6")],
           bordercolor=[("active", "#e1e2e6")])
    st.configure("NavSel.TButton", background=C_CARD, bordercolor=C_LINE, lightcolor=C_CARD, darkcolor=C_CARD, anchor="w",
                 padding=(14, 7), font=(fam, 10, "bold"), foreground=C_ACCENT)
    st.map("NavSel.TButton", background=[("active", C_CARD)])
    # segmented control: two toolbuttons, the chosen one in the accent colour
    st.configure("Seg.Toolbutton", background="#f1f2f4", bordercolor="#d0d2d8", lightcolor="#f1f2f4", darkcolor="#f1f2f4",
                 padding=(14, 4), anchor="center")
    st.map("Seg.Toolbutton", background=[("selected", C_ACCENT), ("active", "#e6e7ea")], foreground=[("selected", "white")],
           lightcolor=[("selected", C_ACCENT)], darkcolor=[("selected", C_ACCENT)], bordercolor=[("selected", C_ACCENT)])
    st.configure("TEntry", padding=(6, 4), insertcolor=C_TEXT)
    st.configure("TCombobox", padding=(6, 3), arrowsize=13, background="#fbfbfc")
    st.map("TCombobox", fieldbackground=[("readonly", C_CARD)], selectbackground=[("readonly", C_CARD)],
           selectforeground=[("readonly", C_TEXT)])
    st.configure("TSpinbox", padding=(6, 3), arrowsize=12)
    st.configure("Horizontal.TProgressbar", background=C_ACCENT, troughcolor="#e8e9ec", bordercolor=C_LINE, lightcolor=C_ACCENT,
                 darkcolor=C_ACCENT, thickness=6)
    root.option_add("*TCombobox*Listbox.font", (fam, 10))
    root.option_add("*TCombobox*Listbox.selectBackground", C_ACCENT)
    return fam


class App:
    def __init__(self, root):
        self.root = root
        self.s = load_settings()
        self.proc = None
        self.log_path = os.path.join(app_dir(), "tombi-port.log")   # the last run's log (Copy log works before a run too)
        self.vars = {}
        self.page = "play"
        self.starting = False
        self._level = "info"
        set_icon(root)
        self.fam = setup_style(root)
        root.geometry("960x660")
        root.minsize(820, 600)
        root.protocol("WM_DELETE_WINDOW", self.on_close)
        self._build()

    def rebuild(self):
        self.collect()
        msg, lvl = self.status.get(), self._level
        for w in self.root.winfo_children():
            w.destroy()
        self._build()
        if msg and lvl != "info":
            self.say(msg, lvl)

    def var(self, key, kind=tk.StringVar, default=""):
        v = self.vars.get(key)
        if v is None:
            v = self.vars[key] = kind(value=self.s.get(key, default))
        return v

    def say(self, msg, level="info"):
        """The status line (disc card, log page) and its coloured dot: ok green, bad red, busy orange, info grey."""
        self._level = level
        self.status.set(msg)
        try:
            self.dot.itemconfigure("d", fill=DOT.get(level, DOT["info"]))
        except tk.TclError:
            pass

    # ---- layout helpers
    def card(self, parent, title=None, pad=16):
        """A white card with a 1 px border; returns the inner (ttk) frame."""
        outer = tk.Frame(parent, background=C_CARD, highlightthickness=1, highlightbackground=C_LINE, highlightcolor=C_LINE, bd=0)
        inner = ttk.Frame(outer, padding=pad)
        inner.pack(fill="both", expand=True)
        if title:
            ttk.Label(inner, text=title, style="Head.TLabel").pack(anchor="w", pady=(0, 6))
        return outer, inner

    def page_frame(self, name, title, scroll=False):
        """A page: title and a body on the grey background (a scrolled one for the long ones)."""
        f = ttk.Frame(self.main, style="Bg.TFrame", padding=(24, 18, 24, 8))
        f.grid(row=0, column=0, sticky="nsew")
        ttk.Label(f, text=title, style="Title.TLabel").pack(anchor="w", pady=(0, 12))
        if not scroll:
            body = ttk.Frame(f, style="Bg.TFrame")
            body.pack(fill="both", expand=True)
            self.pages[name] = f
            return body
        holder = ttk.Frame(f, style="Bg.TFrame")
        holder.pack(fill="both", expand=True)
        cv = tk.Canvas(holder, background=C_BG, highlightthickness=0, bd=0)
        sb = ttk.Scrollbar(holder, orient="vertical", command=cv.yview)
        body = ttk.Frame(cv, style="Bg.TFrame")
        win = cv.create_window(0, 0, window=body, anchor="nw")
        body.bind("<Configure>", lambda e: cv.configure(scrollregion=cv.bbox("all")))
        cv.bind("<Configure>", lambda e: cv.itemconfigure(win, width=e.width))
        cv.configure(yscrollcommand=sb.set)
        cv.pack(side="left", fill="both", expand=True)
        sb.pack(side="right", fill="y")

        def wheel(e):
            if self.page == name:
                cv.yview_scroll(-1 if (e.num == 4 or e.delta > 0) else 1, "units")
        for seq in ("<MouseWheel>", "<Button-4>", "<Button-5>"):
            self.root.bind_all(seq, wheel, add="+")
        self.pages[name] = f
        return body

    def show(self, name):
        self.page = name
        self.pages[name].tkraise()
        for n, b in self.nav.items():
            b.configure(style="NavSel.TButton" if n == name else "Nav.TButton")

    # ---- window
    def _build(self):
        r = self.root
        r.title(tr("title") + "  [build " + BUILD_ID + "]")
        self.status = tk.StringVar(value=tr("s.nodata"))
        r.columnconfigure(1, weight=1)
        r.rowconfigure(0, weight=1)
        side = ttk.Frame(r, style="Side.TFrame", padding=(10, 14, 10, 14), width=208)
        side.grid(row=0, column=0, rowspan=2, sticky="ns")
        side.grid_propagate(False)
        side.pack_propagate(False)
        ttk.Label(side, text="Tombi!", style="App.TLabel").pack(anchor="w", padx=8, pady=(0, 14))
        self.nav = {}
        for n in PAGES:
            b = ttk.Button(side, text=tr("nav." + n), style="Nav.TButton", command=lambda n=n: self.show(n), takefocus=False)
            b.pack(fill="x", pady=1)
            self.nav[n] = b
        ttk.Label(side, text="build " + BUILD_ID, style="Side.TLabel").pack(side="bottom", anchor="w", padx=6, pady=(8, 0))
        lang = ttk.Frame(side, style="Side.TFrame")
        lang.pack(side="bottom", fill="x", padx=4)
        # experimental features (texture dump / packs, 60 fps): hidden unless asked for
        self.experimental = bool(self.s.get("experimental"))
        ev = tk.BooleanVar(value=self.experimental)

        def toggled():
            self.s["experimental"] = ev.get()
            self.root.after_idle(self.rebuild)
        ttk.Checkbutton(lang, text=tr("l.experimental"), variable=ev, command=toggled, takefocus=False).pack(anchor="w", pady=(0, 8))
        ttk.Label(lang, text=tr("l.uilang"), style="Side.TLabel").pack(anchor="w", pady=(0, 3))
        names = ["%s (%s)" % (v, k) for k, v in i18n.LANGS.items()]
        lv = tk.StringVar(value="%s (%s)" % (i18n.LANGS[i18n.current()], i18n.current()))
        cb = ttk.Combobox(lang, textvariable=lv, values=names, state="readonly")
        cb.pack(fill="x")

        def chosen(_=None):
            code = lv.get().rsplit("(", 1)[1].rstrip(")")
            if code != i18n.current():
                i18n.set_language(code)
                self.root.after_idle(self.rebuild)
        cb.bind("<<ComboboxSelected>>", chosen)

        self.main = ttk.Frame(r, style="Bg.TFrame")
        self.main.grid(row=0, column=1, sticky="nsew")
        self.main.columnconfigure(0, weight=1)
        self.main.rowconfigure(0, weight=1)
        self.pages = {}
        self.patch_ui = []
        self._build_play(self.page_frame("play", tr("nav.play")))
        self._build_gfx(self.page_frame("gfx", tr("nav.gfx")))
        self._build_controls(self.page_frame("controls", tr("nav.controls")))
        self._build_mods(self.page_frame("mods", tr("nav.mods"), scroll=True))
        self._build_log(self.page_frame("log", tr("nav.log")))
        self._build_bar(r)
        self.show(self.page)
        for k in ("renderer", "scale", "fullscreen", "internal_res", "aspect", "hz60"):
            self.vars[k].trace_add("write", lambda *_: self._refresh())
        self._refresh()
        if self.proc and self.proc.poll() is None:
            self.play_btn.state(["disabled"])
            self.stop_btn.pack(side="right", padx=(0, 8), after=self.play_btn)
            if self.starting:
                self._starting(True)
        if self.s.get("disc"):
            self.root.after(200, self.check_disc)
        elif data_ready():
            man = X.read_manifest(data_dir())
            self.say(tr("s.dataok", data_dir(), len(man["files"]) if man else 0), "ok")

    def _build_bar(self, r):
        """Bottom bar under every page: the options summary, Copy log, Close the game (while running) and Play."""
        tk.Frame(r, background=C_LINE, height=1).grid(row=1, column=1, sticky="new")
        bar = ttk.Frame(r, style="Bg.TFrame", padding=(24, 12, 24, 14))
        bar.grid(row=1, column=1, sticky="ew")
        self.summary = tk.StringVar()
        self.summary_lbl = ttk.Label(bar, textvariable=self.summary, style="Bg.TLabel")
        # shown from Play until the game prints "[port2] window ..." (port2/runtime/sdl.c) or exits
        self.start_box = ttk.Frame(bar, style="Bg.TFrame")
        ttk.Label(self.start_box, text=tr("l.starting"), style="Bg.TLabel", foreground=C_ACCENT).pack(side="left", padx=(0, 10))
        self.start_bar = ttk.Progressbar(self.start_box, mode="indeterminate", length=240)
        self.start_bar.pack(side="left")
        self.play_btn = ttk.Button(bar, text=tr("b.play"), command=self.play, style="Accent.TButton", width=14)
        self.play_btn.pack(side="right")
        self.stop_btn = ttk.Button(bar, text=tr("b.stop"), command=self.stop, style="Bar.TButton")
        ttk.Button(bar, text=tr("b.copylog"), command=self.copy_log, style="Bar.TButton").pack(side="right", padx=(0, 8))
        self.summary_lbl.pack(side="left")      # last: it gives up its width first when the bar is narrow

    def _refresh(self):
        """Summary line ("OpenGL · 2x window · 3x internal · 16:9") and the internal resolution enabled for OpenGL only."""
        try:
            gl = self.vars["renderer"].get() != "soft"
            parts = ["OpenGL" if gl else "Software",
                     tr("sum.full") if self.vars["fullscreen"].get() else tr("sum.window", self.vars["scale"].get())]
            if gl and self.vars["internal_res"].get() in (2, 3, 4):
                parts.append(tr("sum.ires", self.vars["internal_res"].get()))
            a = self.vars["aspect"].get()
            parts.append({"square": "1:1 px", "16:9": "16:9", "21:9": "21:9", "32:9": "32:9"}.get(a, "4:3"))
            parts.append("60 Hz" if self.vars["hz60"].get() else "50 Hz")
        except tk.TclError:
            return
        self.summary.set("  ·  ".join(parts))
        for b in getattr(self, "ires_btns", []):
            b.state(["!disabled"] if gl else ["disabled"])
        if hasattr(self, "ires_note"):
            if gl:
                self.ires_note.pack_forget()
            else:
                self.ires_note.pack(anchor="w", pady=(6, 0))

    def _browse_row(self, parent, key, command):
        row = ttk.Frame(parent)
        row.pack(fill="x")
        ttk.Entry(row, textvariable=self.var(key)).pack(side="left", fill="x", expand=True)
        ttk.Button(row, text=tr("b.browse"), command=command).pack(side="left", padx=(8, 0))
        return row

    def _build_play(self, f):
        o, c = self.card(f)
        o.pack(fill="x")
        ttk.Label(c, text=tr("c.disc"), style="Head.TLabel").pack(anchor="w")
        ttk.Label(c, text=tr("l.disc"), style="Muted.TLabel").pack(anchor="w", pady=(0, 4))
        self._browse_row(c, "disc", self.pick_disc)
        st = self.status_row = ttk.Frame(c)
        st.pack(fill="x", pady=(8, 0))
        self.dot = tk.Canvas(st, width=10, height=10, background=C_CARD, highlightthickness=0, bd=0)
        self.dot.create_oval(1, 1, 9, 9, fill=DOT["info"], outline="", tags="d")
        self.dot.pack(side="left", anchor="n", pady=(5, 0), padx=(0, 8))
        self.status_lbl = ttk.Label(st, textvariable=self.status, wraplength=640, justify="left")
        self.status_lbl.pack(side="left", fill="x", expand=True)
        st.bind("<Configure>", lambda e: self.status_lbl.configure(wraplength=max(200, e.width - 30)))
        # shown only while the disc is extracted into data/ (check_disc): a message and a bar under the status
        self.ext_box = ttk.Frame(c)
        ttk.Label(self.ext_box, text=tr("l.extracting"), foreground=DOT["busy"]).pack(anchor="w")
        self.ext_bar = ttk.Progressbar(self.ext_box, mode="determinate", maximum=100)
        self.ext_bar.pack(fill="x", pady=(4, 0))
        ttk.Label(c, text=tr("c.saves"), style="Head.TLabel").pack(anchor="w", pady=(16, 0))
        ttk.Label(c, text=tr("l.savedir"), style="Muted.TLabel").pack(anchor="w", pady=(0, 4))
        self._browse_row(c, "savedir", self.pick_savedir)

        o, c = self.card(f)
        o.pack(fill="x", pady=(12, 0))
        c.columnconfigure(0, weight=1, uniform="g")
        c.columnconfigure(1, weight=1, uniform="g")

        def cell(row, col, title):
            box = ttk.Frame(c)
            box.grid(row=row, column=col, sticky="new", padx=(0 if col == 0 else 12, 12 if col == 0 else 0), pady=(0 if row == 0 else 14, 0))
            ttk.Label(box, text=title, style="Head.TLabel").pack(anchor="w", pady=(0, 6))
            return box
        # game language (the boot menu's MAINn.EXE): a combobox
        box = cell(0, 0, tr("c.game"))
        default_lang = UI_TO_GAME_LANG.get(i18n.current(), 0)     # the launcher's language, English otherwise
        lv = self.var("lang", tk.IntVar, default_lang)
        names = [name for _n, name in GAME_LANGS]
        ids = [n for n, _name in GAME_LANGS]
        self.lang_cb = ttk.Combobox(box, values=names, state="readonly")
        self.lang_cb.current(ids.index(lv.get()) if lv.get() in ids else 0)
        self.lang_cb.bind("<<ComboboxSelected>>", lambda _e: (lv.set(ids[self.lang_cb.current()]), self._sync_patch_choice()))
        self.lang_cb.pack(fill="x")
        # text patch: none / the bundled one / another file (keys patch_on + patch, as before; no path shown unless "other")
        box = cell(0, 1, tr("c.patch"))
        self._patch_widget(box)
        # speed: segmented 50 / 60 Hz (the hz60 mod)
        box = cell(1, 0, tr("c.speed"))
        seg = ttk.Frame(box)
        seg.pack(anchor="w")
        hv = self.var("hz60", tk.BooleanVar, False)
        for text, val in (("50 Hz (PAL)", False), ("60 Hz", True)):
            ttk.Radiobutton(seg, text=text, variable=hv, value=val, style="Seg.Toolbutton", width=12,
                            command=self._refresh).pack(side="left")
        box = cell(1, 1, tr("c.quick"))
        ttk.Checkbutton(box, text=tr("l.skipintro"), variable=self.var("skipintro", tk.BooleanVar, True)).pack(anchor="w")
        ttk.Checkbutton(box, text=tr("l.autostart2"), variable=self.var("autostart", tk.BooleanVar, False)).pack(anchor="w", pady=(4, 0))

    def _patch_mode(self):
        """none / bundled / custom from the saved patch_on + patch (an old saved path to the bundled file -> bundled)."""
        bp = bundled_patch()
        cur = (self.vars["patch"].get() or "").strip()
        if not self.vars["patch_on"].get():
            return "none"
        if not cur or (bp and os.path.basename(cur) == os.path.basename(bp) and os.path.dirname(os.path.abspath(cur)).endswith("patches")):
            return "bundled" if bp else "none"
        return "custom"

    def _patch_widget(self, parent):
        """Combobox Ninguno / Traducción mejorada (incluida) / Otro archivo...; the path + Elegir only for "other"."""
        self.var("patch_on", tk.BooleanVar, False)
        self.var("patch")
        if not hasattr(self, "patch_ui"):
            self.patch_ui = []
        if self._patch_mode() == "bundled":        # migrate: the bundled file wherever this exe now lives
            self.vars["patch"].set(bundled_patch())
        modes = ["none"] + (["bundled"] if bundled_patch() else []) + ["custom"]
        names = [tr("p." + m) for m in modes]
        cb = ttk.Combobox(parent, values=names, state="readonly")
        cb.pack(fill="x")
        row = ttk.Frame(parent)
        ttk.Entry(row, textvariable=self.vars["patch"]).pack(side="left", fill="x", expand=True)
        ttk.Button(row, text=tr("b.browse"), command=self.pick_patch).pack(side="left", padx=(8, 0))
        hint = ttk.Label(parent, text=tr("l.patchhint"), style="Muted.TLabel", wraplength=300, justify="left")

        def chosen(_e=None):
            spanish = self.vars["lang"].get() == 5 if "lang" in self.vars else False
            m = [x for x in modes if x != "bundled" or spanish][cb.current()]
            if m == "none":
                self.vars["patch_on"].set(False)
            elif m == "bundled":
                self.vars["patch"].set(bundled_patch() or "")
                self.vars["patch_on"].set(True)
            else:
                if self._patch_mode() != "custom":
                    old = self.vars["patch"].get(), self.vars["patch_on"].get()
                    self.pick_patch()
                    if self._patch_mode() != "custom":      # cancelled: back to what it was
                        self.vars["patch"].set(old[0])
                        self.vars["patch_on"].set(old[1])
            self._sync_patch_choice()
        cb.bind("<<ComboboxSelected>>", chosen)
        self.patch_ui.append((cb, modes, row, hint))
        self._sync_patch_choice()

    def _sync_patch_choice(self):
        """Every patch combobox shown from the patch_on / patch settings."""
        m = self._patch_mode()
        spanish = self.vars.get("lang") is not None and self.vars["lang"].get() == 5
        for cb, modes, row, hint in getattr(self, "patch_ui", []):
            try:
                # the bundled Spanish translation only for the Spanish game (it does not touch the other languages)
                shown = [x for x in modes if x != "bundled" or spanish]
                cb.configure(values=[tr("p." + x) for x in shown])
                cb.current(shown.index(m) if m in shown else 0)
                row.pack(fill="x", pady=(6, 0)) if m == "custom" else row.pack_forget()
                hint.pack(anchor="w", pady=(4, 0)) if (m == "none" and spanish and "bundled" in modes) else hint.pack_forget()
            except tk.TclError:
                pass

    def clear_patch(self):
        self.vars["patch_on"].set(False)
        self._sync_patch_choice()

    def _radios(self, parent, key, kind, default, items):
        v = self.var(key, kind, default)
        out = []
        for text, val in items:
            b = ttk.Radiobutton(parent, text=text, variable=v, value=val)
            b.pack(anchor="w", pady=2)
            out.append(b)
        return out

    def _build_gfx(self, f):
        grid = ttk.Frame(f, style="Bg.TFrame")
        grid.pack(fill="x")
        grid.columnconfigure(0, weight=1, uniform="g")
        grid.columnconfigure(1, weight=1, uniform="g")
        o, c = self.card(grid, tr("c.renderer"))
        o.grid(row=0, column=0, sticky="nsew", padx=(0, 6))
        self._radios(c, "renderer", tk.StringVar, "gl", [(tr("r.gl"), "gl"), (tr("r.soft"), "soft")])
        o, c = self.card(grid, tr("c.window"))
        o.grid(row=0, column=1, sticky="nsew", padx=(6, 0))
        seg = ttk.Frame(c)
        seg.pack(anchor="w")
        sv = self.var("scale", tk.IntVar, 2)
        for n in (1, 2, 3):
            ttk.Radiobutton(seg, text="%dx" % n, variable=sv, value=n, style="Seg.Toolbutton", width=6).pack(side="left")
        ttk.Checkbutton(c, text=tr("l.fullscreen"), variable=self.var("fullscreen", tk.BooleanVar, False)).pack(anchor="w", pady=(10, 0))
        # --fps60: frames interpolated between the 50 Hz logic steps (OpenGL only)
        if self.experimental:
            ttk.Checkbutton(c, text=tr("l.fps60"), variable=self.var("fps60", tk.BooleanVar, False)).pack(anchor="w", pady=(4, 0))
        o, c = self.card(grid, tr("c.aspect"))
        o.grid(row=1, column=0, sticky="nsew", padx=(0, 6), pady=(12, 0))
        self._radios(c, "aspect", tk.StringVar, "tv", [(tr("a.tv"), "tv"), (tr("a.sq"), "square"), (tr("a.w16"), "16:9"), (tr("a.w21"), "21:9"), (tr("a.w32"), "32:9")])
        ttk.Label(c, text=tr("l.titlebg"), style="Muted.TLabel").pack(anchor="w", pady=(10, 4))   # --title-bg
        self._browse_row(c, "title_bg", self.pick_title_bg)
        ttk.Label(c, text=tr("l.titlebg.d"), style="Muted.TLabel", wraplength=300, justify="left").pack(anchor="w", pady=(4, 0))
        o, c = self.card(grid, tr("c.ires"))     # --internal-res: 3D drawn at N x (docs/PORT.md "HD"), OpenGL only
        o.grid(row=1, column=1, sticky="nsew", padx=(6, 0), pady=(12, 0))
        self.ires_btns = self._radios(c, "internal_res", tk.IntVar, 1, [(tr("r.ires1") if n == 1 else "%dx" % n, n) for n in (1, 2, 3, 4)])
        self.ires_note = ttk.Label(c, text=tr("l.iresoff"), style="Muted.TLabel", wraplength=300, justify="left")

    def _build_controls(self, f):
        o, c = self.card(f, tr("c.keys"))
        o.pack(fill="x")
        ttk.Checkbutton(c, text=tr("mod.keys"), variable=self.var("keys_on", tk.BooleanVar, False)).pack(anchor="w")
        ttk.Label(c, text=tr("mod.keys.d"), style="Muted.TLabel", wraplength=640, justify="left").pack(anchor="w", padx=(24, 0), pady=(0, 10))
        grid = ttk.Frame(c)
        grid.pack(anchor="w")
        keys = self.s.get("keys") or dict(M.DEFAULT_KEYS)
        self.key_vars = {}
        for i, b in enumerate(M.BUTTONS):
            ttk.Label(grid, text=b.capitalize(), width=10).grid(row=i % 7, column=(i // 7) * 2, sticky="w", pady=2)
            v = self.key_vars[b] = tk.StringVar(value=keys.get(b, M.DEFAULT_KEYS[b]))
            ttk.Entry(grid, textvariable=v, width=14).grid(row=i % 7, column=(i // 7) * 2 + 1, sticky="w", padx=(2, 24))
        ttk.Button(c, text=tr("b.defaults"), command=lambda: [v.set(M.DEFAULT_KEYS[b]) for b, v in self.key_vars.items()]).pack(anchor="w", pady=(10, 0))
        o, c = self.card(f, tr("c.pad"))
        o.pack(fill="x", pady=(12, 0))
        lbl = ttk.Label(c, text=tr("l.pad"), wraplength=640, justify="left")
        lbl.pack(anchor="w", fill="x")
        c.bind("<Configure>", lambda e: lbl.configure(wraplength=max(200, e.width - 40)))

    def _build_mods(self, f):
        o, c = self.card(f, tr("c.patch"))
        o.pack(fill="x", padx=(0, 4))
        self._patch_widget(c)
        bp = bundled_patch()
        ttk.Label(c, text=tr("l.patchhelp", os.path.basename(bp) if bp else tr("l.patchnone")), style="Muted.TLabel", wraplength=640,
                  justify="left").pack(anchor="w", pady=(8, 0))
        row = ttk.Frame(c)                                     # --cut-content (port/fixes.txt)
        row.pack(anchor="w", pady=(10, 0))
        ttk.Checkbutton(row, text=tr("l.cutcontent"), variable=self.var("cut_content", tk.BooleanVar, False)).pack(side="left")
        ttk.Button(row, text="(i)", width=3,
                   command=lambda: messagebox.showinfo(tr("l.cutcontent"), tr("i.cutcontent"))).pack(side="left", padx=(6, 0))
        if self.experimental:
            o, c = self.card(f, tr("c.texpack"))                 # --texture-pack / --dump-textures (port2/shims/texpack.c)
            o.pack(fill="x", pady=(12, 0), padx=(0, 4))
            ttk.Label(c, text=tr("l.texpack"), style="Muted.TLabel").pack(anchor="w", pady=(0, 4))
            self._browse_row(c, "texpack", self.pick_texpack)
            row = ttk.Frame(c)                                     # the full dump from the disc files (tools/dump_all_textures.py)
            row.pack(fill="x", pady=(8, 0))
            self.dumpall_btn = ttk.Button(row, text=tr("b.dumpall"), command=self.dump_all_textures)
            self.dumpall_btn.pack(side="left")
            self.dumpall_bar = ttk.Progressbar(row, mode="determinate", maximum=100, length=200)
            self.dumpall_bar.pack(side="left", padx=(10, 0))
            self.dumpall_open = ttk.Button(row, text=tr("b.openfolder"), command=self.open_texture_dump)
            self.dumpall_msg = tk.StringVar()
            ttk.Label(c, textvariable=self.dumpall_msg, wraplength=640, justify="left").pack(anchor="w", pady=(4, 0))
            ttk.Label(c, text=tr("l.texpack.d"), style="Muted.TLabel", wraplength=640, justify="left").pack(anchor="w", pady=(4, 0))
        o, c = self.card(f, tr("c.mods"))
        o.pack(fill="x", pady=(12, 0), padx=(0, 4))
        ttk.Label(c, text=tr("l.modshelp"), style="Muted.TLabel", wraplength=640, justify="left").pack(anchor="w", pady=(0, 8))
        for mid, lk, dk, kind, _fn in M.MODS:
            if mid in ("keys_on", "skipintro", "hz60"):
                continue      # on the Controls / Play pages
            box = ttk.Frame(c, padding=(0, 2))
            box.pack(fill="x")
            ttk.Checkbutton(box, text=tr(lk), variable=self.var(mid, tk.BooleanVar, False)).pack(side="left")
            if kind == "check+area":
                av, sv = self.var("area", tk.IntVar, 0), self.var("section", tk.IntVar, 0)
                ttk.Label(box, text=tr("mod.area")).pack(side="left", padx=(12, 4))
                names = ["%02d %s" % (a, M.AREA_NAMES.get(a, "")) for a, _ in M.ROOM_COUNTS]
                self.area_cb = ttk.Combobox(box, values=names, state="readonly", width=20)
                idx = [a for a, _ in M.ROOM_COUNTS]
                self.area_cb.current(idx.index(av.get()) if av.get() in idx else 0)
                self.area_cb.pack(side="left")
                ttk.Label(box, text=tr("mod.section")).pack(side="left", padx=(12, 4))
                self.sec_cb = ttk.Combobox(box, state="readonly", width=4)
                self.sec_cb.pack(side="left")

                def fill_sections(_=None, av=av, sv=sv):
                    a = M.ROOM_COUNTS[self.area_cb.current()][0]
                    av.set(a)
                    secs = [str(k) for k in range(M.ROOM_COUNTS[self.area_cb.current()][1]) if (a, k) not in M.DOOR_ROOMS]
                    self.sec_cb["values"] = secs
                    self.sec_cb.current(secs.index(str(sv.get())) if str(sv.get()) in secs else 0)
                    sv.set(int(self.sec_cb.get()))
                self.area_cb.bind("<<ComboboxSelected>>", fill_sections)
                self.sec_cb.bind("<<ComboboxSelected>>", lambda _e, sv=sv: sv.set(int(self.sec_cb.get())))
                fill_sections()
            elif kind.startswith("check+int:"):
                _, key, lo, hi, step = kind.split(":")
                iv = self.var(key, tk.IntVar, int(lo))
                ttk.Spinbox(box, from_=int(lo), to=int(hi), increment=int(step), textvariable=iv, width=10).pack(side="left", padx=12)
            ttk.Label(c, text=tr(dk), style="Muted.TLabel", wraplength=620, justify="left").pack(anchor="w", padx=(24, 0), pady=(0, 6))

    def _build_log(self, f):
        o, c = self.card(f, tr("c.status"))
        o.pack(fill="x")
        ttk.Label(c, textvariable=self.status, wraplength=640, justify="left").pack(anchor="w")
        o, c = self.card(f, tr("c.cmd"))
        o.pack(fill="x", pady=(12, 0))
        mono = ("Consolas", 9) if os.name == "nt" else ("DejaVu Sans Mono", 9)
        self.cmd_text = tk.Text(c, height=3, wrap="word", relief="flat", background="#f7f7f9", font=mono, highlightthickness=1,
                                highlightbackground=C_LINE, padx=6, pady=4)
        self.cmd_text.pack(fill="x")
        o, c = self.card(f, tr("c.log"))
        o.pack(fill="both", expand=True, pady=(12, 0))
        self.log_text = tk.Text(c, height=8, wrap="none", relief="flat", background="#f7f7f9", font=mono, highlightthickness=1,
                                highlightbackground=C_LINE, padx=6, pady=4)
        self.log_text.pack(fill="both", expand=True)
        try:            # the previous run's log, if any
            self.show_log_tail(open(self.log_path, encoding="utf-8", errors="replace").read().splitlines())
        except OSError:
            pass

    # ---- actions
    def collect(self):
        for k, v in self.vars.items():
            try:
                self.s[k] = v.get()
            except tk.TclError:
                pass
        if hasattr(self, "key_vars"):
            self.s["keys"] = {b: v.get().strip() for b, v in self.key_vars.items()}
        save_settings(self.s)

    def pick_disc(self):
        p = filedialog.askopenfilename(filetypes=[("Disc image", "*.bin *.cue *.iso *.img *.chd"), ("All", "*.*")])
        if p:
            self.vars["disc"].set(p)
            if not self.vars["savedir"].get():
                self.vars["savedir"].set(os.path.dirname(p))
            self.check_disc()

    def pick_patch(self):
        p = filedialog.askopenfilename(filetypes=[("Text patch", "*.tombipatch.json"), ("JSON", "*.json"), ("All", "*.*")])
        if p:
            self.vars["patch"].set(p)
            self.vars["patch_on"].set(True)
        self._sync_patch_choice()

    def disc_hash(self):
        """sha1 of the disc image (.bin / .iso; cached in the settings by path, size and date), None without one or for a .chd
        (hashing its whole track takes minutes: MAIN0.EXE of data/ is checked instead)."""
        image = bin_of_cue(self.s.get("disc") or "")
        if not image or not os.path.isfile(image) or X.chd.is_chd(image):
            return None
        st = os.stat(image)
        key = [os.path.abspath(image), st.st_size, int(st.st_mtime)]
        c = self.s.get("disc_sha1")
        if isinstance(c, list) and len(c) == 4 and c[:3] == key:
            return c[3]
        sha = DP.disc_sha1(image)
        self.s["disc_sha1"] = key + [sha]
        save_settings(self.s)
        return sha

    def sync_patch(self):
        """Puts data/ in the state the text patch settings ask for (applied / original). False when Play must not go on."""
        d, s = data_dir(), self.s
        st = DP.status(d)
        want = s.get("patch_on") and (s.get("patch") or "").strip()
        if want and str(s.get("lang")) != "5" and self._patch_mode() == "bundled":
            want = ""        # the bundled translation is for the Spanish game only: other languages play the original data
        try:
            if not want:
                if st:
                    n = DP.restore(d)
                    self.say(tr("s.patchoff", n), "info")
                return True
            p = want
            if not os.path.isfile(p):
                messagebox.showerror(tr("title"), tr("s.patchmissing", p))
                return False
            gfx_missing = False      # missing (older launcher, or skipped then) or built by another gfx_es version: rebuild
            try:
                if json.load(open(p, encoding="utf-8")).get("portGfx"):
                    import gfx_es
                    gfx_missing = not gfx_es.is_current(os.path.join(d, DP.GFX_TBTX))
            except (OSError, ValueError, ImportError):
                pass
            if st and st.get("sha1") == DP.file_sha1(p) and X.manifest_sha1(d) and not gfx_missing:
                self.say(tr("s.patchon", st.get("title") or os.path.basename(p)), "ok")
                return True
            self.say(tr("s.patching", os.path.basename(p)), "busy")
            self.root.update_idletasks()
            h = self.disc_hash()
            r = DP.apply(d, p, disc_hash=h, log=lambda m: self.say(m, "bad") if "skipped" in m else None)
            chk = tr("s.patchchk.disc") if h else tr("s.patchchk.main0")
            if r["moved"]:
                chk += "; " + tr("s.patchmoved", len(r["moved"]))
            self.say(tr("s.patchok", r["title"] or os.path.basename(p), len(r["files"]), chk), "ok")
            return True
        except (DP.PatchError, OSError, ValueError) as e:
            try:
                DP.restore(d)
            except OSError:
                pass
            self.say(tr("s.patchfail", e), "bad")
            messagebox.showerror(tr("title"), tr("s.patchfail", e))
            return False

    def pick_title_bg(self):
        p = filedialog.askopenfilename(filetypes=[(tr("ft.image"), "*.png *.jpg *.jpeg *.PNG *.JPG *.JPEG")])
        if p:
            self.vars["title_bg"].set(p)

    def texture_dump_dir(self):
        return os.path.join(os.path.dirname(os.path.abspath(data_dir())), "texture_dump")

    def open_texture_dump(self):
        p = self.texture_dump_dir()
        try:
            if os.name == "nt":
                os.startfile(p)
            else:
                subprocess.Popen(["open" if sys.platform == "darwin" else "xdg-open", p])
        except OSError:
            pass

    def dump_all_textures(self):
        """Every texture of the game read from the disc files in data/ (tools/dump_all_textures.py, no game run) into texture_dump/."""
        if not data_ready():
            messagebox.showerror(tr("title"), tr("s.nodata"))
            return
        import dump_all_textures as DT
        out = self.texture_dump_dir()
        self.dumpall_btn.state(["disabled"])
        self.dumpall_open.pack_forget()

        def progress(i, n):
            pct = i * 100 // max(1, n)
            self.root.after(0, self.dumpall_msg.set, tr("s.dumpall", pct))
            self.root.after(0, self.dumpall_bar.configure, {"value": pct})

        def work():
            try:
                total = DT.dump_static(out, data_dir(), log=lambda *a: None, progress=progress)
                self.root.after(0, self.dumpall_msg.set, tr("s.dumpallok", total, out))
                self.root.after(0, lambda: self.dumpall_open.pack(side="left", padx=(10, 0)))
            except Exception as e:
                self.root.after(0, self.dumpall_msg.set, tr("s.dumpallfail", e))
            self.root.after(0, self.dumpall_btn.state, ["!disabled"])
        threading.Thread(target=work, daemon=True).start()

    def pick_texpack(self):
        p = filedialog.askdirectory()
        if p:
            self.vars["texpack"].set(p)

    def pick_savedir(self):
        p = filedialog.askdirectory()
        if p:
            self.vars["savedir"].set(p)

    def check_disc(self):
        self.collect()
        image = bin_of_cue(self.s["disc"])
        self.say(tr("s.checking"), "busy")

        def work():
            try:
                sha, serial = extract_main0(image, os.path.join(app_dir(), "MAIN0.EXE"))
                name = "%s %s" % (serial or "?", KNOWN_DISCS.get(serial, ""))
                msg = tr("s.ok", name.strip()) if sha == MAIN0_SHA1 else tr("s.badsha", name.strip(), sha[:8])
            except (OSError, ValueError, struct.error) as e:
                self.root.after(0, self.say, str(e), "bad")
                return
            self.root.after(0, self.say, msg, "ok" if sha == MAIN0_SHA1 else "bad")
            # the one-time extraction into data/ (again when data/ was made from another disc: a different MAIN0)
            if data_ready() != sha:
                self.root.after(0, self._extracting, 0)
                try:
                    DP.restore(data_dir())       # a text patch in an old data/: originals back before (re)extracting
                except OSError:
                    pass
                last = [-1]

                def progress(done, total, path):
                    pct = done * 100 // max(1, total)
                    if pct != last[0]:
                        last[0] = pct
                        self.root.after(0, self.say, tr("s.extracting", data_dir(), pct, path), "busy")
                        self.root.after(0, self._extracting, pct)
                try:
                    man = X.extract(image, data_dir(), progress)
                    self.root.after(0, self.say, tr("s.dataok", data_dir(), len(man["files"])), "ok")
                except (OSError, ValueError, struct.error) as e:
                    self.root.after(0, self.say, tr("s.extractfail", e), "bad")
                self.root.after(0, self._extracting, None)
        threading.Thread(target=work, daemon=True).start()

    def _extracting(self, pct):
        """pct 0..100 while data/ is being extracted (Play disabled, bar and message shown); None when it ends."""
        if pct is None:
            self.ext_box.pack_forget()
            self.play_btn.configure(text=tr("b.play"))
            self.play_btn.state(["!disabled"])
            return
        if not self.ext_box.winfo_ismapped():
            self.ext_box.pack(fill="x", pady=(8, 0), after=self.status_row)
        self.play_btn.state(["disabled"])
        self.play_btn.configure(text=tr("b.extracting", pct))
        self.ext_bar["value"] = pct

    def command(self):
        self.collect()
        s = self.s
        exe = game_binary()
        source = ["--data", data_dir()] if data_ready() else ["--disc", bin_of_cue(s["disc"])]
        cmd = [exe] + source + ["--window", "--scale", str(s.get("scale", 2)),
               "--show-fps", "2",     # the measured vblank rate and its histogram in the log
               "--renderer", "soft" if s.get("renderer") == "soft" else "gl",
               "--aspect", s.get("aspect") if s.get("aspect") in ("square", "16:9", "21:9", "32:9") else "4:3"]
        if s.get("renderer") != "soft" and s.get("internal_res", 1) in (2, 3, 4):
            cmd += ["--internal-res", str(s["internal_res"])]
        if s.get("renderer") != "soft" and s.get("fps60") and s.get("experimental"):
            cmd += ["--fps60"]
        if s.get("lang"):
            cmd += ["--lang", str(s["lang"])]
        if s.get("cut_content"):
            cmd += ["--cut-content"]
        gfx = os.path.join(data_dir(), DP.GFX_TBTX)    # the Spanish redraws the text patch generated (tools/gfx_es)
        if str(s.get("lang")) != "5":
            self.gfx_note = "no (language is not Spanish)"
        elif cmd[1] != "--data":
            self.gfx_note = "no (running from the disc image, not data/)"
        elif not os.path.isfile(gfx):
            self.gfx_note = "no (%s missing: text patch off, or its redraws were skipped)" % gfx
        else:
            cmd += ["--patch-tex", gfx]
            self.gfx_note = "yes (%s)" % gfx
        if s.get("savedir"):
            cmd += ["--save-dir", s["savedir"]]
        if s.get("texpack") and os.path.isdir(s["texpack"]) and s.get("renderer") != "soft" and s.get("experimental"):
            cmd += ["--texture-pack", s["texpack"]]
        if s.get("title_bg") and s.get("aspect") in ("16:9", "21:9", "32:9"):
            cmd += ["--title-bg", s["title_bg"]]
        if s.get("fullscreen"):
            cmd += ["--fullscreen"]
        cmd += M.mod_args(s, app_dir())
        return cmd

    def play(self):
        cmd = self.command()
        self.cmd_text.delete("1.0", "end")
        self.cmd_text.insert("1.0", subprocess.list2cmdline(cmd))
        if not os.path.isfile(cmd[0]):
            messagebox.showerror(tr("title"), tr("s.nobin", cmd[0]))
            return
        if cmd[1] == "--disc" and (not os.path.isfile(cmd[2]) or not os.path.isfile(os.path.join(app_dir(), "MAIN0.EXE"))
                                   or X.chd.is_chd(cmd[2])):     # tombi2 reads .bin/.iso only: a .chd always goes through data/
            if os.path.isfile(cmd[2]):
                self.check_disc()      # extracts data/ as well; Play again when it says the data is ready
            messagebox.showerror(tr("title"), tr("s.nodata"))
            return
        if self.proc and self.proc.poll() is None:
            return
        if cmd[1] == "--data":
            if not self.sync_patch():
                return
            cmd = self.command()     # sync_patch may just have (re)built the redraws: --patch-tex must see them
            self.cmd_text.delete("1.0", "end")
            self.cmd_text.insert("1.0", subprocess.list2cmdline(cmd))
        elif self.s.get("patch_on"):
            self.say(tr("s.patchnodata"), "busy")
        self.log_path = os.path.join(app_dir(), "tombi-port.log")
        log = open(self.log_path, "w", encoding="utf-8", errors="replace")
        log.write("[launcher] build %s\n" % BUILD_ID)
        log.write("[launcher] Spanish graphics (--patch-tex): %s\n" % getattr(self, "gfx_note", "?"))      # the exe then prints its own id: [port2] build <sha> <date>
        log.flush()
        flags = subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0
        self.proc = subprocess.Popen(cmd, cwd=app_dir(), stdout=log, stderr=subprocess.STDOUT, creationflags=flags)
        self.say(tr("s.running"), "ok")
        self.play_btn.state(["disabled"])
        self._starting(True)
        self.stop_btn.pack(side="right", padx=(0, 8), after=self.play_btn)
        self.root.after(250, self.poll)

    def poll(self):
        if not self.proc:
            return
        try:
            lines = open(self.log_path, encoding="utf-8", errors="replace").read().splitlines()
            self.show_log_tail(lines)
            if self.starting and any(l.startswith("[port2] window ") for l in lines):
                self._starting(False)
        except OSError:
            pass
        rc = self.proc.poll()
        if rc is None:
            self.root.after(250 if self.starting else 1000, self.poll)
            return
        self._starting(False)
        self.say(tr("s.ended", rc, self.log_path), "info")
        self.play_btn.state(["!disabled"])
        self.stop_btn.pack_forget()
        self.proc = None

    def _starting(self, on):
        """The "Starting the game..." notice in the bottom bar (indeterminate bar) above the summary."""
        self.starting = on
        try:
            if on:
                self.start_box.pack(side="top", fill="x", pady=(0, 10), before=self.play_btn)
                self.start_bar.start(12)
            else:
                self.start_bar.stop()
                self.start_box.pack_forget()
        except tk.TclError:
            pass

    def show_log_tail(self, lines):
        """The last 8 lines, preceded by the warning / FATAL / access-violation lines from earlier in the log (a
        missing disc, a bad layout) so a problem reported at startup stays visible under the final block."""
        tail = lines[-8:]
        warn = [l for l in lines[:-8] if "warning" in l or "FATAL" in l or "access violation" in l][:6]
        self.log_text.delete("1.0", "end")
        self.log_text.insert("1.0", "\n".join(warn + (["..."] if warn else []) + tail))

    def copy_log(self):
        """Whole log file to the clipboard (what to paste in a bug report)."""
        try:
            text = open(self.log_path, encoding="utf-8", errors="replace").read() if self.log_path else self.log_text.get("1.0", "end")
        except OSError:
            text = self.log_text.get("1.0", "end")
        self.root.clipboard_clear()
        self.root.clipboard_append(text)
        self.say(tr("s.copied", len(text.splitlines())), "info")

    def stop(self):
        if self.proc and self.proc.poll() is None:
            self.proc.terminate()

    def on_close(self):
        self.collect()
        self.stop()
        self.root.destroy()


def main():
    root = tk.Tk()
    App(root)
    root.mainloop()


if __name__ == "__main__":
    main()
