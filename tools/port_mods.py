#!/usr/bin/env python3
"""Mods of the native port (docs/PORT.md "Mods"): each one is a set of command-line options of build/port2/tombi2
(--poke / --poke-every / --hz / --keys ...), so the launcher (tools/port_launcher.py) only assembles a command
line.  Every mod has an id, an i18n label/description key and a function settings -> list of arguments.

Addresses (PAL SCES-01331, include/tomba/game.h `gameConfig GAME` at 0x8009C960):
  GAME.selectedArea 0x8009C960 (u16), selectedSection 0x8009C962 (u16), playerAP 0x8009C96C (u32),
  playerHealth 0x8009C970 (u8, 4 = full), playerHealthDisplayed 0x8009C971, playerLives 0x8009C980 (u8),
  debug-select flag 0x1F8001B4 (scratchpad), event flag bytes 0x8009CDA4 + id (0xFF = completed; notes/names.md).
"""
import i18n

# (area, sections) of the debug area select (tools/psxemu/rooms.py ROOM_COUNTS); the names from README
ROOM_COUNTS = [(0, 6), (1, 5), (2, 6), (3, 6), (4, 20), (5, 4), (6, 3), (8, 4), (9, 7), (10, 9), (11, 5), (13, 2),
               (14, 8), (16, 7), (17, 11), (18, 3), (19, 3)]
AREA_NAMES = {0: "Village of All Beginnings", 1: "Forest / Watch Tower (AREA01)", 2: "Caves / tunnels (AREA02)",
              3: "Phoenix Mountain", 4: "Trick Village area (AREA04)", 5: "Baccus Village", 6: "Dirt Motocross",
              8: "Baccus Lake", 9: "Mushroom Village", 10: "Deep Jungle", 11: "Village of Civilization",
              13: "Pig Island", 14: "Evil Pigs", 16: "Civilization: Clock Tower", 17: "Civilization: Iron Tower",
              18: "Civilization: Y Crossing", 19: "Civilization (purified)"}
# rooms with an empty load list cannot be entered from the debug select (tools/psxemu/rooms.py DOOR_ROOMS)
DOOR_ROOMS = {(11, 0), (11, 3), (11, 4), (16, 0), (16, 1), (16, 2), (17, 0), (17, 6), (17, 7), (17, 8), (17, 9), (17, 10), (18, 0)}
EVENT_COUNT = 0x82     # event ids 0..0x81 (notes/names.md "Event links"); the flag bytes follow each other

BUTTONS = ["up", "down", "left", "right", "cross", "circle", "square", "triangle", "l1", "r1", "l2", "r2", "start", "select"]
DEFAULT_KEYS = {"up": "Up", "down": "Down", "left": "Left", "right": "Right", "cross": "X", "circle": "D", "square": "Z",
                "triangle": "S", "l1": "Q", "r1": "W", "l2": "A", "r2": "E", "start": "Return", "select": "Backspace"}

i18n.add({
    "es": {
        "mod.skipintro": "Saltar logos y vídeos de introducción",
        "mod.skipintro.d": "Arranca en la pantalla de título sin los logos ni el vídeo de apertura (--play-movies los reproduce).",
        "mod.hz60": "60 Hz (velocidad NTSC)",
        "mod.hz60.d": "El juego PAL corre a 50 Hz; con esta opción el port lo sincroniza a 60 vblanks por segundo (más rápido, música incluida).",
        "mod.areasel": "Selección de área (debug select)",
        "mod.areasel.d": "Abre el selector de áreas de depuración del juego y entra en el área/sección elegida en lugar de empezar la partida.",
        "mod.area": "Área", "mod.section": "Sección",
        "mod.ap": "Empezar con AP", "mod.ap.d": "Puntos de aventura al empezar la partida (0..99999999).",
        "mod.lives": "Empezar con vidas", "mod.lives.d": "Vidas al empezar la partida (1..99).",
        "mod.inflives": "Vidas infinitas", "mod.inflives.d": "Mantiene las vidas en 99 cada segundo.",
        "mod.infhealth": "Vitalidad infinita", "mod.infhealth.d": "Mantiene la vitalidad al máximo (4) en cada vblank.",
        "mod.events": "Desbloquear todos los eventos",
        "mod.events.d": "Marca los %d eventos como completados al empezar (los objetos ligados a eventos desaparecen; útil para explorar)." % EVENT_COUNT,
        "mod.keys": "Teclas personalizadas", "mod.keys.d": "Teclas del visor para cada botón de PS1 (nombres de tecla SDL: X, Return, Space, Left...).",
        "mod.prompts": "Iconos de botones", "mod.prompts.d": "Los iconos de botones de los tutoriales y menús: PlayStation (originales), Xbox (A/B/X/Y, LB/RB/LT/RT) o Teclado (las teclas configuradas). Dibujados por el port (tools/prompt_glyphs.py) y aplicados sobre la textura al cargarla; solo los iconos localizados en port/assets/prompts/manifest.json cambian.",
    },
    "en": {
        "mod.skipintro": "Skip intro logos and movies",
        "mod.skipintro.d": "Boots to the title screen without the logos and the opening movie (--play-movies plays them).",
        "mod.hz60": "60 Hz (NTSC speed)",
        "mod.hz60.d": "The PAL game runs at 50 Hz; this paces the port at 60 vblanks per second (faster, music included).",
        "mod.areasel": "Area select (debug select)",
        "mod.areasel.d": "Opens the game's debug area select and enters the chosen area/section instead of starting the story.",
        "mod.area": "Area", "mod.section": "Section",
        "mod.ap": "Start with AP", "mod.ap.d": "Adventure points at the start of the game (0..99999999).",
        "mod.lives": "Start with lives", "mod.lives.d": "Lives at the start of the game (1..99).",
        "mod.inflives": "Infinite lives", "mod.inflives.d": "Keeps the lives at 99 every second.",
        "mod.infhealth": "Infinite vitality", "mod.infhealth.d": "Keeps the vitality at the maximum (4) every vblank.",
        "mod.events": "Unlock all events",
        "mod.events.d": "Marks all %d events as completed at the start (objects tied to events are gone; for exploring)." % EVENT_COUNT,
        "mod.keys": "Custom keys", "mod.keys.d": "Viewer keys for each PS1 button (SDL key names: X, Return, Space, Left...).",
        "mod.prompts": "Button prompts", "mod.prompts.d": "The button icons of tutorials and menus: PlayStation (original), Xbox (A/B/X/Y, LB/RB/LT/RT) or Keyboard (the configured keys). Drawn by the port (tools/prompt_glyphs.py) and applied over the texture when the game loads it; only the icons located in port/assets/prompts/manifest.json change.",
    },
})

# frames: the title appears before 1000 (docs/PORT.md: --pad-script 1000:start starts a new game); the pokes of a
# new game go after initGameConfig has run (frame 1100), the debug select after 1250 (the sweep script's values)
F_START, F_NEWGAME, F_SELECT, F_CIRCLE = 1000, 1100, 1250, 1300


def args_skipintro(s):
    return [] if s.get("skipintro", True) else ["--play-movies"]


def args_hz60(s):
    return ["--hz", "60"] if s.get("hz60") else ["--hz", "50"]


def args_areasel(s):
    if not s.get("areasel"):
        return []
    area, section = int(s.get("area", 0)), int(s.get("section", 0))
    return ["--poke", "900:1F8001B4:1", "--poke", "%d:8009C960:%X:2" % (F_SELECT, area),
            "--poke", "%d:8009C962:%X:2" % (F_SELECT, section), "--pad-script", "%d:start,%d:circle:3" % (F_START, F_CIRCLE)]


def args_ap(s):
    v = int(s.get("ap", 0))
    return ["--poke", "%d:8009C96C:%X:4" % (F_NEWGAME, v)] if s.get("ap_on") and v > 0 else []


def args_lives(s):
    v = int(s.get("lives", 3))
    return ["--poke", "%d:8009C980:%X:1" % (F_NEWGAME, v)] if s.get("lives_on") else []


def args_inflives(s):
    return ["--poke-every", "50:8009C980:63:1"] if s.get("inflives") else []


def args_infhealth(s):
    return ["--poke-every", "1:8009C970:4:1"] if s.get("infhealth") else []


def args_events(s):
    return ["--poke", "%d:8009CDA4:FF:1x%d" % (F_NEWGAME, EVENT_COUNT)] if s.get("events") else []


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
    spec = ",".join("%s=%s" % (b, keys.get(b, DEFAULT_KEYS[b])) for b in BUTTONS if keys.get(b, DEFAULT_KEYS[b]))
    return ["--keys", spec]


# id, label key, description key, kind ("check" | "check+area" | "check+int" | "check+keys"), settings -> args
MODS = [
    ("skipintro", "mod.skipintro", "mod.skipintro.d", "check", args_skipintro),
    ("hz60", "mod.hz60", "mod.hz60.d", "check", args_hz60),
    ("areasel", "mod.areasel", "mod.areasel.d", "check+area", args_areasel),
    ("ap_on", "mod.ap", "mod.ap.d", "check+int:ap:0:99999999:1000", args_ap),
    ("lives_on", "mod.lives", "mod.lives.d", "check+int:lives:1:99:5", args_lives),
    ("inflives", "mod.inflives", "mod.inflives.d", "check", args_inflives),
    ("infhealth", "mod.infhealth", "mod.infhealth.d", "check", args_infhealth),
    ("events", "mod.events", "mod.events.d", "check", args_events),
    ("keys_on", "mod.keys", "mod.keys.d", "check+keys", args_keys),
]


def mod_args(settings, out_dir=None):
    """All the port arguments of the enabled mods (a new-game pad script is added when no mod gives one)."""
    out = []
    for _id, _l, _d, _k, fn in MODS:
        out += fn(settings)
    out += args_prompts(settings, out_dir)
    if "--pad-script" not in out and settings.get("autostart", False):
        out += ["--pad-script", "%d:start" % F_START]
    return out


def default_settings():
    return {"skipintro": True, "hz60": False, "areasel": False, "area": 0, "section": 0, "ap_on": False, "ap": 10000,
            "lives_on": False, "lives": 5, "inflives": False, "infhealth": False, "events": False, "keys_on": False,
            "keys": dict(DEFAULT_KEYS), "autostart": False, "prompts": "playstation"}
