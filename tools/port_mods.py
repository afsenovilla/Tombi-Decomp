#!/usr/bin/env python3
"""Mods of the native port (docs/PORT.md "Mods"): each one is a set of command-line options of build/port2/tombi2
(--poke / --poke-every / --hz / --keys ...), so the launcher (tools/port_launcher.py) only assembles a command
line.  Every mod has an id, an i18n label/description key and a function settings -> list of arguments.

Addresses: the --poke NAME[+OFF] forms (port2/shims/psyq.c port_addr_of_name), which the port resolves to the
release it runs (gen/version/port_version.h: PAL GAME 0x8009C960, USA 0x8009BCC8), so the same mods work in every build
(include/gamestate.h GAME: the field offsets are the same in every release):
  GAME+0 selectedArea (u16), GAME+2 selectedSection (u16), GAME+C playerAP (u32), GAME+10 playerHealth (u8, 4 = full),
  GAME+11 playerMaxHealth, GAME+20 playerLives (u8), IS_DEBUG_MODE_ENABLED the debug-select flag (scratchpad
  0x1F8001B4), SCRIPT_FLAGS + id the event flag bytes (0xFF = completed; notes/names.md).
"""
import os

import i18n
import save_import

# (area, sections) of the debug area select (tools/psxemu/rooms.py ROOM_COUNTS); the names from README
ROOM_COUNTS = [(0, 6), (1, 5), (2, 6), (3, 6), (4, 20), (5, 4), (6, 3), (8, 4), (9, 7), (10, 9), (11, 5), (13, 2),
               (14, 8), (16, 7), (17, 11), (18, 3), (19, 3)]
# section 0 of 11/16/17 is a cut town rebuilt playable in the port (--civ-town N, docs/CIVILIZATION.md "Playable towns")
CIV_TOWNS = {11: "Lumberjack Town", 16: "Stonetown", 17: "Iron Town"}
# the game's own map names (save_import.REGIONS_EN by section 0 of each area); the three Civilization areas share
# "Village of Civilization", so their cut town's name tells them apart
AREA_NAMES = {a: (save_import.place_name(a, 0, False) if a not in CIV_TOWNS else n + " (%s)" % CIV_TOWNS[a])
              for a, n in save_import.area_names().items()}


def area_label(a):
    """The area select entry: number and the map's name of the area."""
    return "%02d %s" % (a, AREA_NAMES.get(a, ""))
# rooms with an empty load list cannot be entered from the debug select (tools/psxemu/rooms.py DOOR_ROOMS)
DOOR_ROOMS = {(11, 0), (11, 3), (11, 4), (16, 0), (16, 1), (16, 2), (17, 0), (17, 6), (17, 7), (17, 8), (17, 9), (17, 10), (18, 0)}
EVENT_COUNT = 0x82     # event ids 0..0x81 (notes/names.md "Event links"); the flag bytes follow each other

BUTTONS = ["up", "down", "left", "right", "cross", "circle", "square", "triangle", "l1", "r1", "l2", "r2", "start", "select"]
DEFAULT_KEYS = {"up": "Up", "down": "Down", "left": "Left", "right": "Right", "cross": "X", "circle": "D", "square": "Z",
                "triangle": "S", "l1": "Q", "r1": "W", "l2": "A", "r2": "E", "start": "Return", "select": "Backspace"}

i18n.add({
    "es": {
        "mod.skipintro": "Saltar logos y vídeos de introducción",
        "mod.skipintro.d": "Arranca en la pantalla de título sin los logos ni el vídeo de apertura (los vídeos de la historia sí se reproducen).",
        "mod.hz60": "60 Hz (velocidad NTSC)",
        "mod.hz60.d": "El juego PAL corre a 50 Hz; con esta opción el port lo sincroniza a 60 vblanks por segundo (más rápido, música incluida).",
        "mod.areasel": "Selección de área (debug select)",
        "mod.areasel.d": "Abre el selector de áreas de depuración del juego y entra en el área/sección elegida en lugar de empezar la partida.",
        "mod.area": "Área", "mod.section": "Sección",
        "mod.titledebug": "Entrada DEBUG en el menú del título",
        "mod.titledebug.d": "Añade DEBUG a la derecha de OPCIONES en el título: abre el selector de áreas de depuración (y los pueblos reconstruidos si están generados) (--title-debug).",
        "cut.hint": "Los pueblos recortados reconstruidos (áreas 11, 16 y 17) se juegan desde la página «Contenido recortado».",
        "cut.c.text": "Textos y extras recortados",
        "cut.text.d": "Recupera diálogos quitados del juego y algunos detalles sin usar (bayas plateadas/doradas, bocadillo «?»).",
        "cut.c.towns": "Pueblos recortados (reconstruidos)",
        "cut.towns.d": "Tres pueblos que se quitaron del juego. Son reconstrucciones hechas para el port, no datos originales. La primera vez se generan a partir de tu disco (un minuto).",
        "cut.towns.i": "Los mapas y textos se montan con piezas del propio juego (tools/civ_rebuild.py, tools/civ_test_data.py) y se guardan en civ_cache/ junto a data/. Son una reconstrucción: el diseño final de estos pueblos no existe en el disco. Más detalles en docs/CIVILIZATION.md («Playable towns»).",
        "cut.play11": "Jugar Leñadores (AREA11)", "cut.d11": "El pueblo de los leñadores del País de la Civilización.",
        "cut.play16": "Jugar Ciudad de Piedra (AREA16)", "cut.d16": "La ciudad de piedra, junto a la torre del reloj.",
        "cut.play17": "Jugar Ciudad de hierro (AREA17)", "cut.d17": "La ciudad de los hombres de hierro.",
        "mod.civcoll": "Mostrar la capa de colisiones",
        "mod.civcoll.i": "En los pueblos reconstruidos (Lumberjack Town, Stonetown, Iron Town) dibuja el mapa de celdas encima del dibujo: rojo = sólido, naranja = diagonal, magenta = pared de un solo lado, cian = disparador. Sirve para comprobar el ajuste mientras caminas (--civ-collision).",
        "mod.civroute": "Entrar en los pueblos reconstruidos desde el centro en partida normal",
        "mod.civroute.i": "En partida normal, las salidas del centro de la Ciudad de la Civilización llevan a los pueblos recortados reconstruidos (sección 0 de las áreas 11, 16 y 17) en lugar de a las secciones de siempre; las salidas de los pueblos siguen al resto del juego (--civ-route). Usa los mismos mapas y textos generados que los botones de los pueblos.",
        "s.civgen": "Generando los pueblos reconstruidos en {0} (solo la primera vez, alrededor de un minuto)...",
        "s.civgenok": "Pueblos reconstruidos listos en {0}.",
        "s.civgenfail": "No se pudieron generar los pueblos reconstruidos: {0}",
        "s.civnodata": "Los pueblos reconstruidos se generan a partir de la carpeta data/ extraída del disco: elige primero la imagen del disco.",
        "mod.ap": "Empezar con AP", "mod.ap.d": "Puntos de aventura al empezar la partida (0..99999999).",
        "mod.lives": "Empezar con vidas", "mod.lives.d": "Vidas al empezar la partida (1..99).",
        "mod.inflives": "Vidas infinitas", "mod.inflives.d": "Mantiene las vidas en 99 cada segundo.",
        "mod.infhealth": "Vitalidad infinita", "mod.infhealth.d": "Mantiene la vitalidad al máximo (4) en cada vblank.",
        "mod.debugmenu": "Menú de depuración", "mod.debugmenu.d": "En el juego: Select+L1+R1 o F1 abre un menú de depuración (atravesar paredes, volar, trucos, eventos, saltar a cualquier sala, info en pantalla, velocidad).",
        "mod.events": "Desbloquear todos los eventos",
        "mod.events.d": "Marca los %d eventos como completados al empezar (los objetos ligados a eventos desaparecen; útil para explorar)." % EVENT_COUNT,
        "mod.keys": "Teclas personalizadas", "mod.keys.d": "Teclas del visor para cada botón de PS1 (nombres de tecla SDL: X, Return, Space, Left...).",
    },
    "en": {
        "mod.skipintro": "Skip intro logos and movies",
        "mod.skipintro.d": "Boots to the title screen without the logos and the opening movie (story movies still play).",
        "mod.hz60": "60 Hz (NTSC speed)",
        "mod.hz60.d": "The PAL game runs at 50 Hz; this paces the port at 60 vblanks per second (faster, music included).",
        "mod.areasel": "Area select (debug select)",
        "mod.areasel.d": "Opens the game's debug area select and enters the chosen area/section instead of starting the story.",
        "mod.area": "Area", "mod.section": "Section",
        "mod.titledebug": "DEBUG entry in the title menu",
        "mod.titledebug.d": "Adds DEBUG right of OPTIONS on the title screen: it opens the debug area select (and the rebuilt towns once generated) (--title-debug).",
        "cut.hint": "The rebuilt cut towns (areas 11, 16 and 17) are played from the \"Cut content\" page.",
        "cut.c.text": "Cut text and extras",
        "cut.text.d": "Restores dialogue removed from the game and a few unused details (silver/golden berries, '?' bubble).",
        "cut.c.towns": "Cut towns (rebuilt)",
        "cut.towns.d": "Three towns removed from the game. They are reconstructions made for the port, not original data. The first time they are generated from your disc (about a minute).",
        "cut.towns.i": "The maps and texts are assembled from the game's own pieces (tools/civ_rebuild.py, tools/civ_test_data.py) and kept in civ_cache/ next to data/. They are a reconstruction: the final design of these towns is not on the disc. More in docs/CIVILIZATION.md (\"Playable towns\").",
        "cut.play11": "Play Lumberjack Town (AREA11)", "cut.d11": "The lumberjacks' town of the Land of Civilization.",
        "cut.play16": "Play Stonetown (AREA16)", "cut.d16": "The stone town by the clock tower.",
        "cut.play17": "Play Iron Town (AREA17)", "cut.d17": "The town of the Iron Men.",
        "mod.civcoll": "Show collision overlay",
        "mod.civcoll.i": "In the rebuilt towns (Lumberjack Town, Stonetown, Iron Town) the cell map is drawn over the picture: red = solid, orange = diagonal, magenta = one-sided wall, cyan = trigger. For checking the fit while walking (--civ-collision).",
        "mod.civroute": "Enter the rebuilt towns from the hub in normal play",
        "mod.civroute.i": "In normal play the exits of the Civilization Town hub lead to the rebuilt cut towns (section 0 of areas 11, 16 and 17) instead of the usual sections; the towns' own exits lead on to the rest of the game (--civ-route). Uses the same generated maps and texts as the town buttons.",
        "s.civgen": "Generating the rebuilt towns in {0} (first time only, about a minute)...",
        "s.civgenok": "Rebuilt towns ready in {0}.",
        "s.civgenfail": "Could not generate the rebuilt towns: {0}",
        "s.civnodata": "The rebuilt towns are generated from the data/ folder extracted from the disc: choose the disc image first.",
        "mod.ap": "Start with AP", "mod.ap.d": "Adventure points at the start of the game (0..99999999).",
        "mod.lives": "Start with lives", "mod.lives.d": "Lives at the start of the game (1..99).",
        "mod.inflives": "Infinite lives", "mod.inflives.d": "Keeps the lives at 99 every second.",
        "mod.infhealth": "Infinite vitality", "mod.infhealth.d": "Keeps the vitality at the maximum (4) every vblank.",
        "mod.debugmenu": "Debug menu", "mod.debugmenu.d": "In game: Select+L1+R1 or F1 opens a debug menu (noclip, fly, cheats, events, warp to any room, info overlay, speed).",
        "mod.events": "Unlock all events",
        "mod.events.d": "Marks all %d events as completed at the start (objects tied to events are gone; for exploring)." % EVENT_COUNT,
        "mod.keys": "Custom keys", "mod.keys.d": "Viewer keys for each PS1 button (SDL key names: X, Return, Space, Left...).",
    },
})

# frames: the title appears before 1000 (docs/PORT.md: --pad-script 1000:start starts a new game); the pokes of a
# new game go after initGameConfig has run (frame 1100), the debug select after 1250 (the sweep script's values)
F_START, F_NEWGAME, F_SELECT, F_CIRCLE = 1000, 1100, 1250, 1300


def args_skipintro(s):
    return ["--skip-intro"] if s.get("skipintro", True) else []


def args_hz60(s):
    return ["--hz", "60"] if s.get("hz60") else ["--hz", "50"]


def civ_town(s):
    """The rebuilt town a "Cut content" page button asked for (transient "civ_play", never saved), else None."""
    t = s.get("civ_play")
    return int(t) if t in CIV_TOWNS else None


def civ_wanted(s):
    return bool(civ_town(s) or s.get("civ_route"))


def civ_args(s, maps, wfm):
    """--civ-town / --civ-route with the generated maps (tools/civ_rebuild.py) and texts (tools/civ_test_data.py),
    docs/CIVILIZATION.md "Playable towns"."""
    t = civ_town(s)
    if not (t or s.get("civ_route")):
        # the title DEBUG entry's area select reaches the rebuilt towns (sections 0 of 11/16/17) once they exist
        if s.get("titledebug") and os.path.isdir(maps) and os.path.isdir(wfm):
            return ["--civ-maps", maps, "--civ-wfm", wfm]
        return []
    out = ["--civ-town", str(t)] if t else ["--civ-route"]
    out += ["--civ-maps", maps, "--civ-wfm", wfm]
    return out + (["--civ-collision"] if s.get("civ_collision") else [])


def args_areasel(s):
    if not s.get("areasel") or civ_town(s):
        return []
    area, section = int(s.get("area", 0)), int(s.get("section", 0))
    return ["--poke", "900:IS_DEBUG_MODE_ENABLED:1", "--poke", "%d:GAME:%X:2" % (F_SELECT, area),
            "--poke", "%d:GAME+2:%X:2" % (F_SELECT, section), "--pad-script", "%d:start,%d:circle:3" % (F_START, F_CIRCLE)]


def args_titledebug(s):
    return ["--title-debug"] if s.get("titledebug") else []


def args_ap(s):
    v = int(s.get("ap", 0))
    return ["--poke", "g1:GAME+C:%X:4" % v] if s.get("ap_on") and v > 0 else []


def args_lives(s):
    v = int(s.get("lives", 3))
    return ["--poke", "g1:GAME+20:%X:1" % v] if s.get("lives_on") else []


def args_inflives(s):
    return ["--poke-every", "50:GAME+20:63:1"] if s.get("inflives") else []


def args_infhealth(s):
    return ["--poke-every", "1:GAME+10:4:1"] if s.get("infhealth") else []


def args_debugmenu(s):
    return ["--debug-menu"] if s.get("debugmenu") else []


def args_events(s):
    return ["--poke", "g1:SCRIPT_FLAGS:FF:1x%d" % EVENT_COUNT] if s.get("events") else []


PROMPT_SETS = ["playstation", "xbox", "keyboard"]


def args_prompts(s, out_dir=None):
    """--patch-tex FILE: the TBTX of the chosen glyph set, packed next to the launcher (needs Pillow and located rects)."""
    which = s.get("prompts", "playstation")
    if which == "playstation" or not out_dir:
        return []
    import os
    try:
        import prompt_glyphs as P
        out = os.path.join(out_dir, "prompts-%s.tbtx" % which)
        keys = s.get("keys") if s.get("keys_on") else None
        if P.pack(which, out, keys) == 0:
            return []           # no glyph rect located yet: nothing to patch
        return ["--patch-tex", out]
    except Exception:
        return []


def args_keys(s):
    if not s.get("keys_on"):
        return []
    keys = s.get("keys") or {}
    # only the keys that differ from runtime/sdl.c's defaults (a default kept keeps its second key, e.g. select's Right Shift)
    spec = ",".join("%s=%s" % (b, keys[b]) for b in BUTTONS if keys.get(b) and keys[b] != DEFAULT_KEYS[b])
    return ["--keys", spec] if spec else []


# id, label key, description key, kind ("check" | "check+area" | "check+int" | "check+keys"), settings -> args
MODS = [
    ("skipintro", "mod.skipintro", "mod.skipintro.d", "check", args_skipintro),
    ("hz60", "mod.hz60", "mod.hz60.d", "check", args_hz60),
    ("areasel", "mod.areasel", "mod.areasel.d", "check+area", args_areasel),
    ("titledebug", "mod.titledebug", "mod.titledebug.d", "check", args_titledebug),
    ("ap_on", "mod.ap", "mod.ap.d", "check+int:ap:0:99999999:1000", args_ap),
    ("lives_on", "mod.lives", "mod.lives.d", "check+int:lives:1:99:5", args_lives),
    ("inflives", "mod.inflives", "mod.inflives.d", "check", args_inflives),
    ("infhealth", "mod.infhealth", "mod.infhealth.d", "check", args_infhealth),
    ("events", "mod.events", "mod.events.d", "check", args_events),
    ("debugmenu", "mod.debugmenu", "mod.debugmenu.d", "check", args_debugmenu),
    ("keys_on", "mod.keys", "mod.keys.d", "check+keys", args_keys),
]


# the mods every release's build runs (a version without the "pokes" feature of tools/game_library.py, i.e. USA, gets
# only these): the --poke ones name the field (GAME+C...) and the port resolves it to its release; the debug menu reads
# the same names (port_version.h).  Not portable: --title-debug (its title-carousel hook, port/fixes.txt
# titleDrawMenuCarousel, is PAL-only: the USA carousel is its own file) and the 50 / 60 Hz switch (NTSC: always 60).
PORTABLE_MODS = frozenset(("skipintro", "keys_on", "areasel", "ap_on", "lives_on", "inflives", "infhealth", "events",
                           "debugmenu"))


def mod_args(settings, out_dir=None, allowed=None):
    """All the port arguments of the enabled mods (a new-game pad script is added when no mod gives one); allowed: the
    mod ids to consider (None: all)."""
    out = []
    for _id, _l, _d, _k, fn in MODS:
        if allowed is None or _id in allowed:
            out += fn(settings)
    if "--pad-script" not in out and settings.get("autostart", False):
        out += ["--pad-script", "%d:start" % F_START]
    return out


def default_settings():
    return {"skipintro": True, "hz60": True, "areasel": False, "titledebug": False, "area": 0, "section": 0, "ap_on": False, "ap": 10000,
            "lives_on": False, "lives": 5, "inflives": False, "infhealth": False, "events": False, "debugmenu": False, "keys_on": False,
            "keys": dict(DEFAULT_KEYS), "autostart": False, "prompts": "playstation", "civ_collision": False, "civ_route": False}
