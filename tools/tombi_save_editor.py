#!/usr/bin/env python3
"""Tombi Save Editor: a PKHeX-style editor for Tombi! / Tomba! memory card saves (docs/SAVE_EDITOR.md).

Qt (PySide6) front end of tools/tombi_save.py.  Open a card image (.mcd/.mcr/.gme/.vgs/.mcs, the native port's
tombi_mc0.mcd) or drop it on the window, pick a save on the left, edit, Save (keeps CARD.bak) or Save as.  Item / event
names and item icons are read at run time from the user's own disc image or extracted folder ("Game data"): nothing of
the game ships with the editor.  Without it the editor shows ids.  Fonts: Nunito and Lilita One (SIL OFL 1.1, bundled
in save_editor_assets/fonts with their licences).  Icons are drawn inline (own SVG paths).

  python3 tools/tombi_save_editor.py [CARD]
"""
import json, os, struct, sys, time, unicodedata

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import tombi_save as T  # noqa: E402

from PySide6.QtCore import Qt, QSize, QRectF, QByteArray, QAbstractTableModel, QModelIndex, QSortFilterProxyModel, \
    QTimer, Signal  # noqa: E402
from PySide6.QtGui import QFont, QFontDatabase, QIcon, QPixmap, QPainter, QImage, QColor, QPen, QAction, \
    QKeySequence, QPainterPath, QFontMetrics  # noqa: E402
from PySide6.QtSvg import QSvgRenderer  # noqa: E402
from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QFrame, QLabel, QPushButton, QToolButton, \
    QHBoxLayout, QVBoxLayout, QGridLayout, QSpinBox, QSlider, QCheckBox, QComboBox, QLineEdit, QScrollArea, \
    QStackedWidget, QTableView, QHeaderView, QStyledItemDelegate, QAbstractItemView, QFileDialog, QMessageBox, \
    QButtonGroup, QSizePolicy, QMenu, QStyle, QPlainTextEdit  # noqa: E402

APP = "Tombi! Save Editor"
CFG = os.path.join(os.path.expanduser("~"), ".tombi_save_editor.json")
ASSETS = os.path.join(getattr(sys, "_MEIPASS", HERE), "save_editor_assets")
CARD_FILTER = "Memory card (*.mcd *.mcr *.mc *.srm *.gme *.vgs *.mem *.mcs *.mcb *.ps *.ps1);;All files (*)"
CARD_EXTS = (".mcd", ".mcr", ".mc", ".srm", ".gme", ".vgs", ".mem", ".mcs", ".mcb", ".ps", ".ps1")

AREAS = {0: "Village of All Beginnings", 1: "Forest / Watch Tower", 2: "Caves / tunnels", 3: "Phoenix Mountain",
         4: "Trick Village area", 5: "Baccus Village", 6: "Dirt Motocross", 7: "(7)", 8: "Baccus Lake",
         9: "Mushroom Village", 10: "Deep Jungle", 11: "Lumberjack Town / Factory", 12: "(12)", 13: "Pig Island",
         14: "Evil Pigs", 15: "(15)", 16: "Stonetown / Clock Tower", 17: "Iron Town / Iron Castle",
         18: "Civilization Town", 19: "Dwarf Village"}

# ---- strings ---------------------------------------------------------------------------------------------------
UI = {
    "en": {
        "open": "Open card", "data": "Game data", "save": "Save", "saveas": "Save as", "undo": "Undo",
        "unsaved": "Unsaved changes", "nocard": "No card open", "card": "Memory card", "saves": "{0} saves",
        "save1": "1 save", "slot": "Slot", "overview": "Overview", "player": "Player", "items": "Inventory",
        "events": "Events", "misc": "Misc", "drop": "Drop a memory card here", "drop2": "or use Open card.  Raw .mcd / "
        ".mcr, DexDrive .gme, .vgs / .mem, single-save .mcs and the native port's tombi_mc0.mcd are supported.",
        "nodata": "No game data", "nodata2": "Item and event names show as ids. Load your own Tombi! disc image or "
        "extracted folder to see names and icons.", "datafrom": "{1} items · {2} events", "playtime": "Play time",
        "ap": "Adventure points", "level": "Level", "lives": "Lives", "hp": "Vitality", "evdone": "Events completed",
        "itemsown": "Items owned", "location": "Location", "checksum": "Checksum", "ok": "valid", "bad": "BAD",
        "region": "Region", "file": "File", "block": "Block", "vitals": "Vitals", "progress": "Progress",
        "exp": "Experience", "equip": "Equipment", "continue": "Continue point",
        "warnloc": "Changing area / section moves where Continue starts. Pick a room you have already reached.",
        "search_items": "Search items by name or id", "search_events": "Search events by title or id",
        "showcut": "Show cut / unused", "owned": "Owned only", "owned1": "Owned", "qty": "Qty", "cut": "CUT",
        "all": "All", "ns": "Not started", "st": "Started", "done": "Completed", "untitled": "(no title)",
        "sel": "Selection", "complete_all": "Complete all shown", "reset_all": "Reset all shown", "raw": "Raw",
        "set": "Set", "id": "#", "title": "Title", "state": "State", "value": "Value", "shown": "{0} shown",
        "time": "Play time", "h": "h", "m": "m", "s": "s", "purified": "Purified areas", "cur_area": "Current area",
        "cur_section": "Current section", "names_lang": "Game text language", "diag": "Diagnostics",
        "saved": "Saved {0}", "savedbak": "Saved {0} · previous version kept as {1}", "undone": "Undid: {0}",
        "nothing": "Nothing to undo", "opened": "Opened {0}", "nosave": "No Tombi! save on this card: {0}",
        "loaded": "Game data loaded: {0}", "dataerr": "Could not read game data: {0}", "openerr": "Could not open: {0}",
        "saveerr": "Not saved: {0}", "hpmax": "HP cannot exceed max HP ({0})", "quit": "Discard unsaved changes?",
        "quit2": "The card has edits that are not saved.", "theme": "Toggle light / dark", "pickdata":
        "Tombi! disc image (.bin / .cue / .iso) — cancel to pick an extracted folder", "pickdir":
        "Extracted game folder (with AREA00, EXE)", "range": "{0} – {1}", "events_n": "{0} / {1}",
        "datahint": "Item icons and names come from your disc at run time; nothing is stored.",
        "lv": "Lv", "sec": "Section", "spawn": "Spawn point", "area": "Area", "edit_ev": "event states",
        "edit_item": "item {0}", "tip_cycle": "Double-click a row to cycle its state",
    },
    "es": {
        "open": "Abrir tarjeta", "data": "Datos del juego", "save": "Guardar", "saveas": "Guardar como",
        "undo": "Deshacer", "unsaved": "Cambios sin guardar", "nocard": "Ninguna tarjeta", "card": "Tarjeta de memoria",
        "saves": "{0} partidas", "save1": "1 partida", "slot": "Partida", "overview": "Resumen", "player": "Jugador",
        "items": "Inventario", "events": "Eventos", "misc": "Varios", "drop": "Suelta aquí una tarjeta de memoria",
        "drop2": "o usa Abrir tarjeta.  Admite .mcd / .mcr, DexDrive .gme, .vgs / .mem, .mcs de una partida y el "
        "tombi_mc0.mcd del port nativo.", "nodata": "Sin datos del juego", "nodata2": "Los objetos y eventos se "
        "muestran como ids. Carga tu propia imagen del disco de Tombi! o una carpeta extraída para ver nombres e iconos.",
        "datafrom": "{1} objetos · {2} eventos", "playtime": "Tiempo de juego", "ap": "Puntos de aventura",
        "level": "Nivel", "lives": "Vidas", "hp": "Vitalidad", "evdone": "Eventos completados",
        "itemsown": "Objetos en posesión", "location": "Lugar", "checksum": "Suma de control", "ok": "válida",
        "bad": "ERRÓNEA", "region": "Región", "file": "Archivo", "block": "Bloque", "vitals": "Vitalidad",
        "progress": "Progreso", "exp": "Experiencia", "equip": "Equipo", "continue": "Punto de Continuar",
        "warnloc": "Cambiar área / sección mueve dónde empieza Continuar. Elige una sala ya visitada.",
        "search_items": "Buscar objetos por nombre o id", "search_events": "Buscar eventos por título o id",
        "showcut": "Ver recortados / sin uso", "owned": "Solo en posesión", "owned1": "Tiene", "qty": "Cant.",
        "cut": "RECORTE", "all": "Todos", "ns": "Sin empezar", "st": "Empezado", "done": "Completado",
        "untitled": "(sin título)", "sel": "Selección", "complete_all": "Completar los visibles",
        "reset_all": "Reiniciar los visibles", "raw": "Valor", "set": "Poner", "id": "#", "title": "Título",
        "state": "Estado", "value": "Valor", "shown": "{0} visibles", "time": "Tiempo de juego", "h": "h", "m": "min",
        "s": "s", "purified": "Áreas purificadas", "cur_area": "Área actual", "cur_section": "Sección actual",
        "names_lang": "Idioma de los textos del juego", "diag": "Diagnóstico", "saved": "Guardado {0}",
        "savedbak": "Guardado {0} · versión anterior en {1}", "undone": "Deshecho: {0}", "nothing": "Nada que deshacer",
        "opened": "Abierto {0}", "nosave": "No hay partidas de Tombi! en esta tarjeta: {0}",
        "loaded": "Datos del juego cargados: {0}", "dataerr": "No se pudieron leer los datos del juego: {0}",
        "openerr": "No se pudo abrir: {0}", "saveerr": "No guardado: {0}", "hpmax": "La vida no puede superar la "
        "vida máxima ({0})", "quit": "¿Descartar los cambios sin guardar?", "quit2": "La tarjeta tiene cambios sin "
        "guardar.", "theme": "Tema claro / oscuro", "pickdata": "Imagen del disco de Tombi! (.bin / .cue / .iso) — "
        "cancela para elegir una carpeta extraída", "pickdir": "Carpeta del juego extraída (con AREA00, EXE)",
        "range": "{0} – {1}", "events_n": "{0} / {1}", "datahint": "Los iconos y nombres se leen de tu disco al "
        "ejecutar; no se guarda nada.", "lv": "Nv", "sec": "Sección", "spawn": "Punto de aparición", "area": "Área",
        "edit_ev": "estados de eventos", "edit_item": "objeto {0}", "tip_cycle": "Doble clic en una fila cambia su estado",
    },
}
FIELD_LABELS = {
    "en": {"ap": "AP", "max_health": "Max HP", "health": "HP", "lives": "Lives", "level": "Level (title)",
           "red_level": "Red level", "green_level": "Green level", "blue_level": "Blue level", "red_bar": "Red bar",
           "green_bar": "Green bar", "blue_bar": "Blue bar", "golden_bowl": "Golden bowl", "bonus_health":
           "Bonus hearts", "weapon": "Weapon (raw id)", "pants": "Pants (raw id)", "section": "Section",
           "cur_spawn": "Spawn point", "purified_areas": "Purified areas", "cur_area": "Current area",
           "cur_section": "Current section", "area": "Area", "time": "Play time"},
    "es": {"ap": "AP", "max_health": "Vida máxima", "health": "Vida", "lives": "Vidas", "level": "Nivel (título)",
           "red_level": "Nivel rojo", "green_level": "Nivel verde", "blue_level": "Nivel azul", "red_bar": "Barra roja",
           "green_bar": "Barra verde", "blue_bar": "Barra azul", "golden_bowl": "Cuenco dorado", "bonus_health":
           "Corazones extra", "weapon": "Arma (id)", "pants": "Pantalón (id)", "section": "Sección",
           "cur_spawn": "Punto de aparición", "purified_areas": "Áreas purificadas", "cur_area": "Área actual",
           "cur_section": "Sección actual", "area": "Área", "time": "Tiempo de juego"},
}
FIELD_HINTS = {
    "en": {"max_health": "8 normally, up to 16 with the golden bowl", "golden_bowl": "non-zero lets max HP reach 16",
           "level": "the Level shown in the save title", "health": "refilled to max on load"},
    "es": {"max_health": "8 normalmente, hasta 16 con el cuenco dorado", "golden_bowl": "distinto de 0: vida "
           "máxima hasta 16", "level": "el Nivel del título de la partida", "health": "se rellena al cargar"},
}

# ---- theme -----------------------------------------------------------------------------------------------------
THEMES = {
    "dark": dict(bg="#121614", side="#171c19", card="#1f2621", card2="#272f29", hover="#2b342d", border="#323c34",
                 text="#eef1ea", muted="#a9b3ab", faint="#6f7a72", input="#151a17", primary="#3a7c45",
                 primary_h="#45904f", primary_t="#ffffff", accent="#ff6aa8", accent_soft="#3b2230",
                 accent_t="#ff8fbe", brown="#7a2e00", brown_t="#fff8e6", cream="#fff8e6", ok="#79d389",
                 ok_soft="#1e3524", warn="#f2b45a", warn_soft="#3a2e18", err="#ff8a80", err_soft="#3d1f1d",
                 sel="#2f6b3a", glyph="light", title="#fff8e6", red="#ff6b6b", green="#5cc46d", blue="#5aa7ff"),
    "light": dict(bg="#f3f0e7", side="#ebe7db", card="#ffffff", card2="#f6f3ea", hover="#efeadc", border="#ddd6c4",
                  text="#1c2620", muted="#56625a", faint="#9aa39b", input="#ffffff", primary="#2f6b3a",
                  primary_h="#275a31", primary_t="#ffffff", accent="#ff6aa8", accent_soft="#ffe3ef",
                  accent_t="#a4235c", brown="#7a2e00", brown_t="#fff8e6", cream="#fff8e6", ok="#2a6b36",
                  ok_soft="#e1f0e2", warn="#8a5300", warn_soft="#fbefd9", err="#b3261e", err_soft="#fbe4e2",
                  sel="#d9ead9", glyph="dark", title="#275a31", red="#d64545", green="#3a8f48", blue="#2f74d0"),
}


def qss(t):
    a = ASSETS.replace("\\", "/")
    g = t["glyph"]
    return """
* { font-family: "Nunito"; font-size: 10pt; color: %(text)s; }
QMainWindow, #root { background: %(bg)s; }
QToolTip { background: %(card2)s; color: %(text)s; border: 1px solid %(border)s; padding: 4px 8px; border-radius: 6px; }
#topbar { background: %(side)s; border-bottom: 1px solid %(border)s; }
#appTitle { font-family: "Lilita One"; font-size: 17pt; color: %(title)s; letter-spacing: 0.5px; }
#fileChip { background: %(card)s; border: 1px solid %(border)s; border-radius: 13px; padding: 3px 12px; color: %(muted)s; }
#dirtyChip { background: %(accent_soft)s; border-radius: 13px; padding: 3px 12px; color: %(accent_t)s; font-weight: 700; }
#sidebar { background: %(side)s; border-right: 1px solid %(border)s; }
#caption { color: %(muted)s; font-size: 8pt; font-weight: 800; letter-spacing: 1.2px; }
#cardName { font-size: 12pt; font-weight: 800; }
#muted, .muted { color: %(muted)s; }
#small { color: %(muted)s; font-size: 9pt; }
#slotCard { background: %(card)s; border: 1px solid %(border)s; border-radius: 12px; }
#slotCard:hover { background: %(hover)s; }
#slotCard[selected="true"] { border: 2px solid %(primary)s; background: %(card2)s; }
#slotTitle { font-weight: 800; font-size: 10.5pt; }
#slotMeta { color: %(muted)s; font-size: 9pt; }
#badge { background: %(card2)s; border: 1px solid %(border)s; border-radius: 8px; padding: 0px 7px; font-size: 8pt;
         font-weight: 800; color: %(muted)s; }
#badgeOk { background: %(ok_soft)s; border-radius: 8px; padding: 1px 8px; font-size: 8.5pt; font-weight: 800; color: %(ok)s; }
#badgeErr { background: %(err_soft)s; border-radius: 8px; padding: 1px 8px; font-size: 8.5pt; font-weight: 800; color: %(err)s; }
#badgeCut { background: %(brown)s; border-radius: 7px; padding: 0px 6px; font-size: 7.5pt; font-weight: 800; color: %(brown_t)s; }
#dataCard { background: %(card)s; border: 1px solid %(border)s; border-radius: 12px; }
QPushButton, QToolButton { background: %(card)s; border: 1px solid %(border)s; border-radius: 9px; padding: 6px 14px;
         font-weight: 700; }
QPushButton:hover, QToolButton:hover { background: %(hover)s; }
QPushButton:pressed, QToolButton:pressed { background: %(card2)s; }
QPushButton:disabled, QToolButton:disabled { color: %(faint)s; background: %(side)s; }
QPushButton#primary { background: %(primary)s; border: 1px solid %(primary)s; color: %(primary_t)s; }
QPushButton#primary:hover { background: %(primary_h)s; }
QPushButton#primary:disabled { background: %(card2)s; border-color: %(border)s; color: %(faint)s; }
QPushButton#ghost, QToolButton#ghost { background: transparent; border: 1px solid transparent; }
QPushButton#ghost:hover, QToolButton#ghost:hover { background: %(hover)s; }
QPushButton#seg { border-radius: 0px; padding: 5px 11px; background: %(card)s; }
QPushButton#seg:checked { background: %(primary)s; color: %(primary_t)s; border-color: %(primary)s; }
QPushButton#segL { border-top-left-radius: 9px; border-bottom-left-radius: 9px; border-top-right-radius: 0; border-bottom-right-radius: 0; padding: 5px 11px; }
QPushButton#segR { border-top-right-radius: 9px; border-bottom-right-radius: 9px; border-top-left-radius: 0; border-bottom-left-radius: 0; padding: 5px 11px; margin-left: -1px; }
QPushButton#segL:checked, QPushButton#segR:checked { background: %(primary)s; color: %(primary_t)s; border-color: %(primary)s; }
QPushButton#chip { border-radius: 14px; padding: 4px 12px; background: %(card)s; }
QPushButton#chip:checked { background: %(sel)s; border-color: %(primary)s; color: %(text)s; }
#tabbar { background: transparent; }
QPushButton#tab { background: transparent; border: none; border-radius: 0; padding: 10px 4px 9px 4px; margin-right: 18px;
         font-size: 10.5pt; font-weight: 700; color: %(muted)s; border-bottom: 3px solid transparent; }
QPushButton#tab:hover { color: %(text)s; }
QPushButton#tab:checked { color: %(text)s; border-bottom: 3px solid %(accent)s; }
#tabline { background: %(border)s; max-height: 1px; min-height: 1px; }
#panel { background: %(card)s; border: 1px solid %(border)s; border-radius: 14px; }
#panelTitle { font-size: 11pt; font-weight: 800; }
#heroTitle { font-family: "Nunito"; font-size: 15pt; font-weight: 800; }
#statValue { font-size: 18pt; font-weight: 800; }
#statLabel { color: %(muted)s; font-size: 9pt; font-weight: 700; }
#fieldLabel { font-weight: 700; }
#hint { color: %(muted)s; font-size: 8.5pt; }
#err { color: %(err)s; font-size: 8.5pt; font-weight: 700; }
#warn { background: %(warn_soft)s; color: %(warn)s; border-radius: 9px; padding: 8px 12px; font-weight: 600; }
QSpinBox, QLineEdit, QComboBox { background: %(input)s; border: 1px solid %(border)s; border-radius: 8px; padding: 4px 8px;
         min-height: 20px; selection-background-color: %(primary)s; selection-color: #ffffff; }
QSpinBox:focus, QLineEdit:focus, QComboBox:focus { border: 1px solid %(accent)s; }
QSpinBox[invalid="true"] { border: 1px solid %(err)s; }
QSpinBox::up-button, QSpinBox::down-button { subcontrol-origin: border; width: 18px; border: none; background: transparent; }
QSpinBox::up-button { subcontrol-position: top right; }
QSpinBox::down-button { subcontrol-position: bottom right; }
QSpinBox::up-arrow { image: url(%(a)s/icons/chev-up-%(g)s.svg); width: 9px; height: 9px; }
QSpinBox::down-arrow { image: url(%(a)s/icons/chev-down-%(g)s.svg); width: 9px; height: 9px; }
QSpinBox::up-button:hover, QSpinBox::down-button:hover { background: %(hover)s; border-radius: 4px; }
QComboBox::drop-down { border: none; width: 22px; }
QComboBox::down-arrow { image: url(%(a)s/icons/chev-down-%(g)s.svg); width: 10px; height: 10px; }
QComboBox QAbstractItemView { background: %(card)s; border: 1px solid %(border)s; selection-background-color: %(sel)s;
         selection-color: %(text)s; outline: none; padding: 4px; }
QCheckBox { spacing: 8px; }
QCheckBox::indicator { width: 16px; height: 16px; border-radius: 5px; border: 1.5px solid %(faint)s; background: %(input)s; }
QCheckBox::indicator:hover { border-color: %(primary)s; }
QCheckBox::indicator:checked { background: %(primary)s; border-color: %(primary)s; image: url(%(a)s/icons/check.svg); }
QSlider::groove:horizontal { height: 4px; background: %(card2)s; border-radius: 2px; }
QSlider::sub-page:horizontal { background: %(primary)s; border-radius: 2px; }
QSlider::handle:horizontal { background: %(cream)s; border: 2px solid %(primary)s; width: 12px; height: 12px;
         margin: -6px 0; border-radius: 8px; }
QSlider[tone="red"]::sub-page:horizontal { background: %(red)s; }
QSlider[tone="green"]::sub-page:horizontal { background: %(green)s; }
QSlider[tone="blue"]::sub-page:horizontal { background: %(blue)s; }
QScrollArea, QScrollArea > QWidget > QWidget { background: transparent; border: none; }
QScrollBar:vertical { background: transparent; width: 10px; margin: 2px; }
QScrollBar::handle:vertical { background: %(border)s; border-radius: 4px; min-height: 30px; }
QScrollBar::handle:vertical:hover { background: %(faint)s; }
QScrollBar::add-line, QScrollBar::sub-line { height: 0; width: 0; }
QScrollBar::add-page, QScrollBar::sub-page { background: transparent; }
QScrollBar:horizontal { height: 0px; }
#itemCard { background: %(card)s; border: 1px solid %(border)s; border-radius: 12px; }
#itemCard:hover { background: %(hover)s; }
#itemCard[owned="true"] { border: 1px solid %(primary)s; background: %(card2)s; }
#iconWell { background: %(card2)s; border-radius: 9px; }
#itemName { font-weight: 700; }
#itemId { color: %(muted)s; font-size: 8.5pt; }
QTableView { background: %(card)s; border: 1px solid %(border)s; border-radius: 12px; gridline-color: transparent;
         selection-background-color: %(sel)s; selection-color: %(text)s; alternate-background-color: %(card2)s; outline: none; }
QTableView::item { padding: 0 8px; border: none; }
QHeaderView { background: transparent; }
QHeaderView::section { background: %(card)s; color: %(muted)s; border: none; border-bottom: 1px solid %(border)s;
         padding: 8px; font-weight: 800; font-size: 8.5pt; }
QTableCornerButton::section { background: %(card)s; border: none; }
#statusbar { background: %(side)s; border-top: 1px solid %(border)s; }
#status[sev="info"] { color: %(muted)s; }
#status[sev="ok"] { color: %(ok)s; font-weight: 700; }
#status[sev="err"] { color: %(err)s; font-weight: 700; }
#dropZone { border: 2px dashed %(border)s; border-radius: 22px; background: %(card)s; }
#dropTitle { font-size: 16pt; font-weight: 800; }
QPlainTextEdit { background: %(input)s; border: 1px solid %(border)s; border-radius: 10px; padding: 8px;
         font-family: "Consolas", "DejaVu Sans Mono", monospace; font-size: 9pt; }
QProgressBar { background: %(card2)s; border: none; border-radius: 4px; max-height: 8px; }
QProgressBar::chunk { background: %(primary)s; border-radius: 4px; }
QMenu { background: %(card)s; border: 1px solid %(border)s; padding: 4px; border-radius: 8px; }
QMenu::item { padding: 6px 18px; border-radius: 6px; }
QMenu::item:selected { background: %(sel)s; }
QMessageBox { background: %(card)s; }
""" % dict(t, a=a, g=g)


# ---- inline icons (own simple paths, 24x24 stroke style) -------------------------------------------------------
ICONS = {
    "open": '<path d="M3 7a2 2 0 0 1 2-2h4l2 2h8a2 2 0 0 1 2 2v8a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2z"/>',
    "disc": '<circle cx="12" cy="12" r="9"/><circle cx="12" cy="12" r="2.5"/><path d="M12 3a9 9 0 0 1 9 9"/>',
    "save": '<path d="M5 3h11l4 4v12a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V5a1 1 0 0 1 1-2z"/><path d="M8 3v5h7V3"/>'
            '<rect x="7" y="13" width="10" height="6" rx="1"/>',
    "saveas": '<path d="M5 3h11l4 4v5"/><path d="M4 5v14a2 2 0 0 0 2 2h6"/><path d="M8 3v5h7V3"/>'
              '<path d="M17 15v6M14 18h6"/>',
    "undo": '<path d="M9 14 4 9l5-5"/><path d="M4 9h11a5 5 0 0 1 0 10h-3"/>',
    "sun": '<circle cx="12" cy="12" r="4"/><path d="M12 2v2M12 20v2M4.9 4.9l1.4 1.4M17.7 17.7l1.4 1.4M2 12h2M20 12h2'
           'M4.9 19.1l1.4-1.4M17.7 6.3l1.4-1.4"/>',
    "moon": '<path d="M20 14.5A8 8 0 1 1 9.5 4a6.5 6.5 0 0 0 10.5 10.5z"/>',
    "card": '<path d="M6 2h9l4 4v14a2 2 0 0 1-2 2H6a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2z"/><path d="M8 6v3M11 6v3M14 6v3"/>',
    "clock": '<circle cx="12" cy="12" r="9"/><path d="M12 7v5l3 2"/>',
    "star": '<path d="m12 3 2.7 5.6 6.1.9-4.4 4.3 1 6.1L12 17l-5.4 2.9 1-6.1-4.4-4.3 6.1-.9z"/>',
    "heart": '<path d="M12 20s-7-4.4-7-10a4 4 0 0 1 7-2.6A4 4 0 0 1 19 10c0 5.6-7 10-7 10z"/>',
    "pin": '<path d="M12 21s-6-5.3-6-11a6 6 0 0 1 12 0c0 5.7-6 11-6 11z"/><circle cx="12" cy="10" r="2.2"/>',
    "flag": '<path d="M5 21V4M5 4h11l-2 4 2 4H5"/>',
    "bag": '<path d="M5 8h14l-1 12H6z"/><path d="M9 8V6a3 3 0 0 1 6 0v2"/>',
    "shield": '<path d="M12 3 5 6v6c0 4.5 3 7.5 7 9 4-1.5 7-4.5 7-9V6z"/><path d="m9 12 2 2 4-4"/>',
    "search": '<circle cx="11" cy="11" r="6.5"/><path d="m20 20-4.2-4.2"/>',
    "check": '<path d="m5 12.5 4.5 4.5L19 7.5"/>',
    "reset": '<path d="M4 12a8 8 0 1 0 2.4-5.7"/><path d="M4 4v4h4"/>',
    "life": '<circle cx="12" cy="8" r="4"/><path d="M5 21a7 7 0 0 1 14 0"/>',
    "drop": '<path d="M12 3v12M7 10l5 5 5-5"/><path d="M4 17v2a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2v-2"/>',
    "info": '<circle cx="12" cy="12" r="9"/><path d="M12 11v6M12 7.5v.5"/>',
}


def svg_pixmap(name, color, size=18, ratio=1.0):
    svg = ('<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="%s" stroke-width="2" '
           'stroke-linecap="round" stroke-linejoin="round">%s</svg>' % (color, ICONS[name]))
    r = QSvgRenderer(QByteArray(svg.encode()))
    px = QPixmap(int(size * ratio), int(size * ratio))
    px.fill(Qt.transparent)
    p = QPainter(px)
    r.render(p)
    p.end()
    px.setDevicePixelRatio(ratio)
    return px


def svg_icon(name, color, size=18):
    return QIcon(svg_pixmap(name, color, size, 2.0))


# ---- images from the card / the user's disc ----------------------------------------------------------------------
def bgr555(v):
    if v == 0:
        return 0
    r, g, b = (v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31
    return 0xFF000000 | r << 16 | g << 8 | b


def save_icon(save):
    """First frame of the save's own 16 x 16 4 bpp memory card icon (header 0x60 CLUT, 0x80 pixels)."""
    h = save.header
    clut = [bgr555(struct.unpack_from("<H", h, 0x60 + 2 * i)[0]) for i in range(16)]
    img = QImage(16, 16, QImage.Format_ARGB32)
    for y in range(16):
        for x in range(16):
            b = h[0x80 + y * 8 + x // 2]
            img.setPixel(x, y, clut[b >> 4 * (x & 1) & 15])
    return img


def item_icons(path):
    """{item id: QImage 16 x 16} from the user's PAL disc (docs/CUT_CONTENT.md "Event and item icons",
    tools/event_icons.py item_page): page SYSTEM/A00300.000, CLUT block and ITEM_ICON_CLUTS of SYSTEM/D0SPSUB.BIN."""
    import text_core as C
    if path.lower().endswith(".cue"):
        import re
        m = re.search(r'FILE\s+"([^"]+)"', open(path, encoding="latin-1").read())
        if m:
            path = os.path.join(os.path.dirname(path), m.group(1))
    src = C.Source(path)
    page = src.read_any("SYSTEM/A00300.000")
    dsp = src.read_any("SYSTEM/D0SPSUB.BIN")
    clut = dsp[0x98B8:0x98B8 + 48 * 31 * 2]
    cluts = dsp[0x80109C88 - 0x800E8028:][:160]
    out = {}
    for n in range(165):
        row = cluts[n] if n < 160 else 0
        pal = [bgr555(struct.unpack_from("<H", clut, (row * 48 + 16 + i) * 2)[0]) for i in range(16)]
        u0, v0 = n % 15 * 16 + 8, n // 15 * 16 + 8
        img = QImage(16, 16, QImage.Format_ARGB32)
        for y in range(16):
            for x in range(16):
                b = page[128 + (v0 + y) * 384 + (u0 + x) // 2]
                img.setPixel(x, y, pal[b >> 4 * ((u0 + x) & 1) & 15])
        out[n] = img
    return out


def scaled(img, size):
    return QPixmap.fromImage(img.scaled(size, size, Qt.KeepAspectRatio, Qt.FastTransformation))


def fmt_time(vb):
    t = vb // 50
    return "%d:%02d:%02d" % (t // 3600, t // 60 % 60, t % 60)


# ---- small widgets ---------------------------------------------------------------------------------------------
def label(text, name=None, wrap=False):
    w = QLabel(text)
    if name:
        w.setObjectName(name)
    if wrap:
        w.setWordWrap(True)
    return w


def hbox(*ws, spacing=8, margins=(0, 0, 0, 0)):
    l = QHBoxLayout()
    l.setSpacing(spacing)
    l.setContentsMargins(*margins)
    for w in ws:
        if w is None:
            l.addStretch(1)
        elif isinstance(w, int):
            l.addSpacing(w)
        elif isinstance(w, QWidget):
            l.addWidget(w)
        else:
            l.addLayout(w)
    return l


def repolish(w):
    w.style().unpolish(w)
    w.style().polish(w)


class Clickable(QFrame):
    clicked = Signal()

    def mousePressEvent(self, e):
        if e.button() == Qt.LeftButton:
            self.clicked.emit()
        super().mousePressEvent(e)


class Panel(QFrame):
    def __init__(self, title=None, icon=None, theme=None):
        super().__init__()
        self.setObjectName("panel")
        self.lay = QVBoxLayout(self)
        self.lay.setContentsMargins(18, 16, 18, 18)
        self.lay.setSpacing(12)
        if title:
            row = QHBoxLayout()
            row.setSpacing(8)
            if icon and theme:
                ic = QLabel()
                ic.setPixmap(svg_pixmap(icon, theme["primary"] if theme is THEMES["light"] else theme["ok"], 18, 2))
                row.addWidget(ic)
            row.addWidget(label(title, "panelTitle"))
            row.addStretch(1)
            self.lay.addLayout(row)


class FieldRow(QWidget):
    """Label + spin box (+ slider for small ranges) + range hint + inline error."""
    def __init__(self, key, title, hint, lo, hi, on_commit, slider=True, tone=None, step=1):
        super().__init__()
        self.key, self.on_commit = key, on_commit
        g = QGridLayout(self)
        g.setContentsMargins(0, 0, 0, 0)
        g.setHorizontalSpacing(12)
        g.setVerticalSpacing(2)
        lab = label(title, "fieldLabel")
        lab.setFixedWidth(118)
        g.addWidget(lab, 0, 0)
        self.spin = QSpinBox()
        self.spin.setRange(lo, hi)
        self.spin.setSingleStep(step)
        self.spin.setGroupSeparatorShown(hi > 9999)
        self.spin.setFixedWidth(130 if hi > 9999 else 92)
        self.spin.setKeyboardTracking(False)
        self.spin.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.slider = None
        if slider and hi - lo <= 255:
            self.slider = QSlider(Qt.Horizontal)
            self.slider.setRange(lo, hi)
            self.slider.setMinimumWidth(120)
            if tone:
                self.slider.setProperty("tone", tone)
            self.slider.valueChanged.connect(self.spin.setValue)
            g.addWidget(self.slider, 0, 1)
        else:
            g.addItem(QHBoxLayout(), 0, 1)
        g.addWidget(self.spin, 0, 2)
        sub = hint + ("  ·  " if hint else "") + "%s – %s" % (format(lo, ","), format(hi, ","))
        self.hint = label(sub, "hint")
        g.addWidget(self.hint, 1, 0, 1, 3)
        self.err = label("", "err")
        self.err.hide()
        g.addWidget(self.err, 2, 0, 1, 3)
        g.setColumnStretch(1, 1)
        self.spin.valueChanged.connect(self._changed)
        self._quiet = False

    def set(self, v):
        self._quiet = True
        self.spin.setValue(v)
        if self.slider:
            self.slider.blockSignals(True)
            self.slider.setValue(v)
            self.slider.blockSignals(False)
        self._quiet = False
        self.show_error(None)

    def _changed(self, v):
        if self.slider:
            self.slider.blockSignals(True)
            self.slider.setValue(v)
            self.slider.blockSignals(False)
        if not self._quiet:
            self.on_commit(self, v)

    def show_error(self, msg):
        self.err.setText(msg or "")
        self.err.setVisible(bool(msg))
        self.spin.setProperty("invalid", bool(msg))
        repolish(self.spin)


# ---- events table ----------------------------------------------------------------------------------------------
class EventModel(QAbstractTableModel):
    def __init__(self, win):
        super().__init__()
        self.win = win

    def rowCount(self, parent=QModelIndex()):
        return T.EVENT_USED if self.win.save else 0

    def columnCount(self, parent=QModelIndex()):
        return 4

    def headerData(self, s, o, role=Qt.DisplayRole):
        if o == Qt.Horizontal and role == Qt.DisplayRole:
            L = self.win.L
            return [L["id"], L["title"], L["state"], L["value"]][s].upper()
        if o == Qt.Horizontal and role == Qt.TextAlignmentRole:
            return int(Qt.AlignLeft | Qt.AlignVCenter) if s in (1, 2) else int(Qt.AlignRight | Qt.AlignVCenter)

    def data(self, ix, role=Qt.DisplayRole):
        i, c = ix.row(), ix.column()
        if role == Qt.DisplayRole:
            if c == 0:
                return str(i)
            if c == 1:
                return self.win.event_title(i)
            if c == 2:
                return self.win.state_name(self.win.save.event(i))
            if c == 3:
                return str(self.win.save.event(i))
        if role == Qt.UserRole:
            return self.win.save.event(i)
        if role == Qt.UserRole + 1:
            return i in T.UNUSED_EVENTS
        if role == Qt.TextAlignmentRole:
            return int(Qt.AlignRight | Qt.AlignVCenter) if c in (0, 3) else int(Qt.AlignLeft | Qt.AlignVCenter)
        if role == Qt.ToolTipRole:
            return self.win.L["tip_cycle"]

    def refresh(self):
        self.beginResetModel()
        self.endResetModel()

    def changed(self):
        if self.rowCount():
            self.dataChanged.emit(self.index(0, 0), self.index(self.rowCount() - 1, 3))


class EventFilter(QSortFilterProxyModel):
    def __init__(self):
        super().__init__()
        self.text, self.state, self.cut = "", -1, True

    def filterAcceptsRow(self, r, parent):
        m = self.sourceModel()
        v = m.data(m.index(r, 0), Qt.UserRole)
        if not self.cut and r in T.UNUSED_EVENTS:
            return False
        if self.state == 0 and v != 0 or self.state == 1 and v in (0, 255) or self.state == 2 and v != 255:
            return False
        if self.text:
            t = self.text.lower()
            return t == str(r) or t in m.data(m.index(r, 1)).lower()
        return True


class EventDelegate(QStyledItemDelegate):
    def __init__(self, win):
        super().__init__()
        self.win = win

    def paint(self, p, opt, ix):
        c = ix.column()
        if c not in (1, 2):
            return super().paint(p, opt, ix)
        t = self.win.theme
        self.initStyleOption(opt, ix)
        text = opt.text
        opt.text = ""
        QApplication.style().drawControl(QStyle.CE_ItemViewItem, opt, p)
        p.save()
        p.setRenderHint(QPainter.Antialiasing)
        r = opt.rect.adjusted(10, 0, -10, 0)
        f = QFont(opt.font)
        if c == 1:
            cut = ix.data(Qt.UserRole + 1)
            bw = 0
            if cut:
                f2 = QFont(f)
                f2.setPointSizeF(7.5)
                f2.setBold(True)
                fm2 = QFontMetrics(f2)
                tag = self.win.L["cut"]
                bw = fm2.horizontalAdvance(tag) + 12
                br = QRectF(r.right() - bw, r.center().y() - 9, bw, 18)
                p.setPen(Qt.NoPen)
                p.setBrush(QColor(t["brown"]))
                p.drawRoundedRect(br, 7, 7)
                p.setPen(QColor(t["brown_t"]))
                p.setFont(f2)
                p.drawText(br, Qt.AlignCenter, tag)
            p.setFont(f)
            untitled = text.startswith("(")
            p.setPen(QColor(t["muted"] if untitled else t["text"]))
            fm = QFontMetrics(f)
            p.drawText(r.adjusted(0, 0, -bw - 8, 0), Qt.AlignVCenter | Qt.AlignLeft,
                       fm.elidedText(text, Qt.ElideRight, r.width() - bw - 8))
        else:
            v = ix.data(Qt.UserRole)
            fg, bg = (t["ok"], t["ok_soft"]) if v == 255 else (t["warn"], t["warn_soft"]) if v else (t["muted"], t["card2"])
            f.setBold(True)
            f.setPointSizeF(8.5)
            fm = QFontMetrics(f)
            w = fm.horizontalAdvance(text) + 26
            br = QRectF(r.left(), r.center().y() - 10, w, 20)
            p.setPen(Qt.NoPen)
            p.setBrush(QColor(bg))
            p.drawRoundedRect(br, 10, 10)
            p.setBrush(QColor(fg))
            p.drawEllipse(QRectF(br.left() + 9, br.center().y() - 3, 6, 6))
            p.setPen(QColor(fg))
            p.setFont(f)
            p.drawText(br.adjusted(19, 0, 0, 0), Qt.AlignVCenter | Qt.AlignLeft, text)
        p.restore()


# ---- main window -----------------------------------------------------------------------------------------------
def load_cfg():
    try:
        return json.load(open(CFG, encoding="utf-8"))
    except Exception:
        return {}


class Editor(QMainWindow):
    def __init__(self, card=None, cfg=None):
        super().__init__()
        self.cfg = load_cfg() if cfg is None else cfg
        self.lang = self.cfg.get("ui", "es") if self.cfg.get("ui") in UI else "es"
        self.theme_name = self.cfg.get("theme", "dark") if self.cfg.get("theme") in THEMES else "dark"
        self.card = self.save = None
        self.saves = []
        self.names = {"items": {}, "events": {}, "untitled": set()}
        self.icons = {}
        self.data_path = None
        self.history = []          # [label, key, card bytes before, time]
        self.clean = None
        self.loading = False
        self.status_msg = ("", "info")
        self.setAcceptDrops(True)
        self.resize(1320, 840)
        self.setMinimumSize(1060, 680)
        self.apply_theme()
        self.build()
        if self.cfg.get("data"):
            self.load_data(self.cfg["data"], quiet=True)
        if card:
            self.open_card(card)
        self.refresh_all()

    # ---- helpers
    @property
    def L(self):
        return UI[self.lang]

    @property
    def theme(self):
        return THEMES[self.theme_name]

    def names_lang(self):
        n = self.cfg.get("names_lang")
        return n if n in T.LANGS else ("S" if self.lang == "es" else "B")

    def save_cfg(self):
        try:
            json.dump(self.cfg, open(CFG, "w", encoding="utf-8"), indent=1)
        except Exception:
            pass

    def apply_theme(self):
        QApplication.instance().setStyleSheet(qss(self.theme))

    def icon(self, name, size=18, color=None):
        return svg_icon(name, color or self.theme["text"], size)

    def item_name(self, i):
        return self.names["items"].get(i) or ("Item #%d" % i if self.lang == "en" else "Objeto #%d" % i)

    def event_title(self, i):
        t = self.names["events"].get(i)
        if t:
            return t
        if self.names["events"]:
            return self.L["untitled"]
        return ("Event #%d" if self.lang == "en" else "Evento #%d") % i

    def state_name(self, v):
        L = self.L
        return L["done"] if v == 255 else ("%s · %d" % (L["st"], v)) if v else L["ns"]

    def area_name(self, a):
        return AREAS.get(a, "(%d)" % a)

    # ---- layout
    def build(self):
        root = QWidget()
        root.setObjectName("root")
        v = QVBoxLayout(root)
        v.setContentsMargins(0, 0, 0, 0)
        v.setSpacing(0)
        v.addWidget(self.build_topbar())
        mid = QHBoxLayout()
        mid.setSpacing(0)
        mid.addWidget(self.build_sidebar())
        self.main_stack = QStackedWidget()
        self.main_stack.addWidget(self.build_empty())
        self.main_stack.addWidget(self.build_main())
        mid.addWidget(self.main_stack, 1)
        v.addLayout(mid, 1)
        sb = QFrame()
        sb.setObjectName("statusbar")
        sl = QHBoxLayout(sb)
        sl.setContentsMargins(18, 6, 18, 6)
        self.status_icon = QLabel()
        self.status = label("", "status")
        self.status.setProperty("sev", "info")
        self.status_right = label("", "small")
        sl.addWidget(self.status_icon)
        sl.addWidget(self.status, 1)
        sl.addWidget(self.status_right)
        v.addWidget(sb)
        self.setCentralWidget(root)
        for seq, fn in ((QKeySequence.Undo, self.undo), (QKeySequence.Save, self.do_save),
                        (QKeySequence("Ctrl+Shift+S"), self.do_save_as), (QKeySequence.Open, self.ask_open)):
            a = QAction(self)
            a.setShortcut(seq)
            a.triggered.connect(fn)
            self.addAction(a)

    def build_topbar(self):
        L, t = self.L, self.theme
        bar = QFrame()
        bar.setObjectName("topbar")
        h = QHBoxLayout(bar)
        h.setContentsMargins(18, 10, 18, 10)
        h.setSpacing(8)
        logo = QLabel()
        logo.setPixmap(self.logo_pixmap(30))
        h.addWidget(logo)
        h.addWidget(label("Tombi! Save Editor", "appTitle"))
        h.addSpacing(10)
        self.file_chip = label("", "fileChip")
        self.dirty_chip = label("●  " + L["unsaved"], "dirtyChip")
        h.addWidget(self.file_chip)
        h.addWidget(self.dirty_chip)
        h.addStretch(1)

        def btn(text, icon, fn, name=None, tip=None):
            b = QPushButton(text)
            if name:
                b.setObjectName(name)
            b.setIcon(self.icon(icon, 16, t["primary_t"] if name == "primary" else None))
            b.setIconSize(QSize(16, 16))
            b.setCursor(Qt.PointingHandCursor)
            b.clicked.connect(fn)
            if tip:
                b.setToolTip(tip)
            return b
        h.addWidget(btn(L["open"], "open", self.ask_open, tip="Ctrl+O"))
        h.addWidget(btn(L["data"], "disc", self.ask_data, tip=L["datahint"]))
        self.undo_btn = btn(L["undo"], "undo", self.undo, tip="Ctrl+Z")
        h.addWidget(self.undo_btn)
        h.addSpacing(6)
        grp = QButtonGroup(bar)
        for code, name in (("en", "segL"), ("es", "segR")):
            b = QPushButton(code.upper())
            b.setObjectName(name)
            b.setCheckable(True)
            b.setChecked(code == self.lang)
            b.setCursor(Qt.PointingHandCursor)
            b.clicked.connect(lambda _=False, c=code: self.set_lang(c))
            grp.addButton(b)
            h.addWidget(b)
        th = QToolButton()
        th.setObjectName("ghost")
        th.setIcon(self.icon("sun" if self.theme_name == "dark" else "moon", 18))
        th.setIconSize(QSize(18, 18))
        th.setToolTip(L["theme"])
        th.setCursor(Qt.PointingHandCursor)
        th.clicked.connect(self.toggle_theme)
        h.addWidget(th)
        h.addSpacing(6)
        self.saveas_btn = btn(L["saveas"], "saveas", self.do_save_as, tip="Ctrl+Shift+S")
        self.save_btn = btn(L["save"], "save", self.do_save, "primary", "Ctrl+S")
        h.addWidget(self.saveas_btn)
        h.addWidget(self.save_btn)
        return bar

    def logo_pixmap(self, size):
        """A pink-haired round badge in the launcher's palette (own drawing, no game art)."""
        px = QPixmap(size * 2, size * 2)
        px.fill(Qt.transparent)
        p = QPainter(px)
        p.setRenderHint(QPainter.Antialiasing)
        s = size * 2
        p.setPen(Qt.NoPen)
        p.setBrush(QColor("#2f6b3a"))
        p.drawRoundedRect(QRectF(0, 0, s, s), s * 0.28, s * 0.28)
        p.setBrush(QColor("#ff6aa8"))
        path = QPainterPath()
        path.moveTo(s * 0.18, s * 0.52)
        path.cubicTo(s * 0.15, s * 0.12, s * 0.85, s * 0.12, s * 0.82, s * 0.52)
        path.lineTo(s * 0.70, s * 0.40)
        path.lineTo(s * 0.62, s * 0.50)
        path.lineTo(s * 0.50, s * 0.38)
        path.lineTo(s * 0.38, s * 0.50)
        path.lineTo(s * 0.30, s * 0.40)
        path.closeSubpath()
        p.drawPath(path)
        p.setBrush(QColor("#fff8e6"))
        p.drawEllipse(QRectF(s * 0.27, s * 0.46, s * 0.46, s * 0.40))
        p.setBrush(QColor("#7a2e00"))
        p.drawEllipse(QRectF(s * 0.38, s * 0.58, s * 0.07, s * 0.10))
        p.drawEllipse(QRectF(s * 0.55, s * 0.58, s * 0.07, s * 0.10))
        p.end()
        px.setDevicePixelRatio(2)
        return px

    def build_sidebar(self):
        L = self.L
        side = QFrame()
        side.setObjectName("sidebar")
        side.setFixedWidth(300)
        v = QVBoxLayout(side)
        v.setContentsMargins(16, 18, 16, 16)
        v.setSpacing(10)
        v.addWidget(label(L["card"].upper(), "caption"))
        self.card_name = label(L["nocard"], "cardName")
        self.card_meta = label("", "small")
        v.addWidget(self.card_name)
        v.addWidget(self.card_meta)
        v.addSpacing(4)
        self.slot_area = QScrollArea()
        self.slot_area.setWidgetResizable(True)
        self.slot_host = QWidget()
        self.slot_lay = QVBoxLayout(self.slot_host)
        self.slot_lay.setContentsMargins(0, 0, 4, 0)
        self.slot_lay.setSpacing(8)
        self.slot_lay.addStretch(1)
        self.slot_area.setWidget(self.slot_host)
        v.addWidget(self.slot_area, 1)
        dc = QFrame()
        dc.setObjectName("dataCard")
        dl = QVBoxLayout(dc)
        dl.setContentsMargins(14, 12, 14, 12)
        dl.setSpacing(4)
        r = QHBoxLayout()
        self.data_icon = QLabel()
        r.addWidget(self.data_icon)
        self.data_title = label("", "fieldLabel")
        r.addWidget(self.data_title, 1)
        dl.addLayout(r)
        self.data_text = label("", "small", wrap=True)
        dl.addWidget(self.data_text)
        v.addWidget(dc)
        return side

    def build_empty(self):
        L = self.L
        w = QWidget()
        v = QVBoxLayout(w)
        v.setContentsMargins(40, 40, 40, 40)
        z = Clickable()
        z.setObjectName("dropZone")
        z.setCursor(Qt.PointingHandCursor)
        z.clicked.connect(self.ask_open)
        zl = QVBoxLayout(z)
        zl.setAlignment(Qt.AlignCenter)
        zl.setSpacing(10)
        ic = QLabel()
        ic.setPixmap(svg_pixmap("drop", self.theme["accent"], 56, 2))
        ic.setAlignment(Qt.AlignCenter)
        zl.addWidget(ic)
        t1 = label(L["drop"], "dropTitle")
        t1.setAlignment(Qt.AlignCenter)
        zl.addWidget(t1)
        t2 = label(L["drop2"], "muted", wrap=True)
        t2.setAlignment(Qt.AlignCenter)
        t2.setFixedWidth(480)
        zl.addWidget(t2, 0, Qt.AlignCenter)
        b = QPushButton(L["open"])
        b.setObjectName("primary")
        b.setIcon(self.icon("open", 16, "#ffffff"))
        b.clicked.connect(self.ask_open)
        b.setCursor(Qt.PointingHandCursor)
        zl.addSpacing(6)
        zl.addWidget(b, 0, Qt.AlignCenter)
        v.addWidget(z)
        return w

    def build_main(self):
        L = self.L
        w = QWidget()
        v = QVBoxLayout(w)
        v.setContentsMargins(28, 8, 28, 0)
        v.setSpacing(0)
        tabs = QHBoxLayout()
        tabs.setSpacing(0)
        self.tab_group = QButtonGroup(w)
        self.pages = QStackedWidget()
        for k, (key, builder) in enumerate((("overview", self.build_overview), ("player", self.build_player),
                                            ("items", self.build_items), ("events", self.build_events),
                                            ("misc", self.build_misc))):
            b = QPushButton(L[key])
            b.setObjectName("tab")
            b.setCheckable(True)
            b.setCursor(Qt.PointingHandCursor)
            b.clicked.connect(lambda _=False, k=k: self.show_page(k))
            self.tab_group.addButton(b, k)
            tabs.addWidget(b)
            self.pages.addWidget(builder())
        self.tab_group.button(self.cfg.get("tab", 0) if 0 <= self.cfg.get("tab", 0) < 5 else 0).setChecked(True)
        self.pages.setCurrentIndex(self.tab_group.checkedId())
        tabs.addStretch(1)
        v.addLayout(tabs)
        line = QFrame()
        line.setObjectName("tabline")
        v.addWidget(line)
        v.addSpacing(16)
        v.addWidget(self.pages, 1)
        return w

    def show_page(self, k):
        self.pages.setCurrentIndex(k)
        if k == 2:
            QTimer.singleShot(0, self.layout_items)

    def scroll(self, inner):
        s = QScrollArea()
        s.setWidgetResizable(True)
        s.setWidget(inner)
        return s

    # overview
    def build_overview(self):
        L, t = self.L, self.theme
        inner = QWidget()
        v = QVBoxLayout(inner)
        v.setContentsMargins(0, 0, 8, 20)
        v.setSpacing(16)
        hero = Panel()
        hl = QHBoxLayout()
        hl.setSpacing(20)
        self.hero_icon = QLabel()
        self.hero_icon.setFixedSize(88, 88)
        self.hero_icon.setObjectName("iconWell")
        self.hero_icon.setAlignment(Qt.AlignCenter)
        hl.addWidget(self.hero_icon)
        col = QVBoxLayout()
        col.setSpacing(6)
        self.hero_title = label("", "heroTitle")
        self.hero_sub = label("", "muted")
        self.hero_badges = QHBoxLayout()
        self.hero_badges.setSpacing(6)
        col.addStretch(1)
        col.addWidget(self.hero_title)
        col.addWidget(self.hero_sub)
        col.addLayout(self.hero_badges)
        col.addStretch(1)
        hl.addLayout(col, 1)
        hero.lay.addLayout(hl)
        v.addWidget(hero)
        grid = QGridLayout()
        grid.setSpacing(16)
        self.stats = {}
        specs = [("playtime", "clock", t["accent"]), ("ap", "star", t["warn"]), ("level", "flag", t["ok"]),
                 ("lives", "life", t["blue"]), ("hp", "heart", t["red"]), ("location", "pin", t["ok"])]
        for k, (key, ic, col_) in enumerate(specs):
            p = Panel()
            p.lay.setSpacing(4)
            r = QHBoxLayout()
            i = QLabel()
            i.setPixmap(svg_pixmap(ic, col_, 18, 2))
            r.addWidget(i)
            r.addWidget(label(L[key], "statLabel"), 1)
            p.lay.addLayout(r)
            val = label("", "statValue")
            p.lay.addWidget(val)
            sub = label("", "small")
            p.lay.addWidget(sub)
            self.stats[key] = (val, sub)
            grid.addWidget(p, k // 3, k % 3)
        v.addLayout(grid)
        prog = QGridLayout()
        prog.setSpacing(16)
        from PySide6.QtWidgets import QProgressBar
        self.progress = {}
        for k, (key, ic) in enumerate((("evdone", "check"), ("itemsown", "bag"))):
            p = Panel(L[key], ic, t)
            val = label("", "statValue")
            bar = QProgressBar()
            bar.setTextVisible(False)
            p.lay.addWidget(val)
            p.lay.addWidget(bar)
            self.progress[key] = (val, bar)
            prog.addWidget(p, 0, k)
        v.addLayout(prog)
        v.addStretch(1)
        return self.scroll(inner)

    # player
    def field(self, key, slider=True, tone=None):
        FL, FH = FIELD_LABELS[self.lang], FIELD_HINTS[self.lang]
        lo, hi = T.FIELDS[key][2:4]
        f = FieldRow(key, FL.get(key, key), FH.get(key, ""), lo, hi, self.commit_field, slider, tone)
        self.fields[key] = f
        return f

    def build_player(self):
        L, t = self.L, self.theme
        self.fields = {}
        inner = QWidget()
        g = QGridLayout(inner)
        g.setContentsMargins(0, 0, 8, 20)
        g.setSpacing(16)
        p1 = Panel(L["vitals"], "heart", t)
        for k in ("health", "max_health", "lives", "golden_bowl", "bonus_health"):
            p1.lay.addWidget(self.field(k))
        p2 = Panel(L["progress"], "star", t)
        p2.lay.addWidget(self.field("ap", False))
        p2.lay.addWidget(self.field("level"))
        p3 = Panel(L["exp"], "flag", t)
        for c in ("red", "green", "blue"):
            p3.lay.addWidget(self.field(c + "_level", tone=c))
        for c in ("red", "green", "blue"):
            p3.lay.addWidget(self.field(c + "_bar", tone=c))
        p4 = Panel(L["equip"], "shield", t)
        p4.lay.addWidget(self.field("weapon", False))
        p4.lay.addWidget(self.field("pants", False))
        p5 = Panel(L["continue"], "pin", t)
        r = QGridLayout()
        r.setHorizontalSpacing(12)
        al = label(FIELD_LABELS[self.lang]["area"], "fieldLabel")
        al.setFixedWidth(118)
        r.addWidget(al, 0, 0)
        self.area_cb = QComboBox()
        for a, n in AREAS.items():
            self.area_cb.addItem("%02d  %s" % (a, n), a)
        self.area_cb.currentIndexChanged.connect(self.commit_area)
        r.addWidget(self.area_cb, 0, 1)
        r.setColumnStretch(1, 1)
        p5.lay.addLayout(r)
        p5.lay.addWidget(self.field("section"))
        p5.lay.addWidget(self.field("cur_spawn", False))
        p5.lay.addWidget(label(L["warnloc"], "warn", wrap=True))
        left = QVBoxLayout()
        left.setSpacing(16)
        left.addWidget(p1)
        left.addWidget(p2)
        left.addWidget(p4)
        left.addStretch(1)
        right = QVBoxLayout()
        right.setSpacing(16)
        right.addWidget(p3)
        right.addWidget(p5)
        right.addStretch(1)
        g.addLayout(left, 0, 0)
        g.addLayout(right, 0, 1)
        g.setColumnStretch(0, 1)
        g.setColumnStretch(1, 1)
        return self.scroll(inner)

    # inventory
    def build_items(self):
        L, t = self.L, self.theme
        w = QWidget()
        v = QVBoxLayout(w)
        v.setContentsMargins(0, 0, 0, 0)
        v.setSpacing(12)
        self.item_search = QLineEdit()
        self.item_search.setPlaceholderText(L["search_items"])
        self.item_search.addAction(self.icon("search", 16, t["muted"]), QLineEdit.LeadingPosition)
        self.item_search.setClearButtonEnabled(True)
        self.item_search.setMinimumWidth(320)
        self.item_search.textChanged.connect(self.filter_items)
        self.item_owned = QPushButton(L["owned"])
        self.item_owned.setObjectName("chip")
        self.item_owned.setCheckable(True)
        self.item_owned.toggled.connect(self.filter_items)
        self.item_cut = QPushButton(L["showcut"])
        self.item_cut.setObjectName("chip")
        self.item_cut.setCheckable(True)
        self.item_cut.setChecked(True)
        self.item_cut.toggled.connect(self.filter_items)
        self.item_count = label("", "small")
        v.addLayout(hbox(self.item_search, self.item_owned, self.item_cut, None, self.item_count))
        self.item_host = QWidget()
        self.item_grid = QGridLayout(self.item_host)
        self.item_grid.setContentsMargins(0, 0, 8, 16)
        self.item_grid.setSpacing(10)
        self.item_scroll = self.scroll(self.item_host)
        v.addWidget(self.item_scroll, 1)
        self.item_cards = {}
        self.item_cols = 0
        self.item_scroll.viewport().installEventFilter(self)
        return w

    def make_item_cards(self):
        for c in self.item_cards.values():
            c.setParent(None)
            c.deleteLater()
        self.item_cards = {}
        n = max(len(self.names["items"]), 0xAB)
        for i in range(n):
            self.item_cards[i] = self.item_card(i)
        self.item_cols = 0
        self.layout_items()

    def item_card(self, i):
        L, t = self.L, self.theme
        c = QFrame()
        c.setObjectName("itemCard")
        c.setFixedHeight(78)
        c.setMinimumWidth(200)
        g = QGridLayout(c)
        g.setContentsMargins(10, 10, 12, 10)
        g.setHorizontalSpacing(10)
        g.setVerticalSpacing(4)
        well = QLabel()
        well.setObjectName("iconWell")
        well.setFixedSize(44, 44)
        well.setAlignment(Qt.AlignCenter)
        if i in self.icons:
            well.setPixmap(scaled(self.icons[i], 32))
        else:
            well.setPixmap(svg_pixmap("bag", t["faint"], 20, 2))
        g.addWidget(well, 0, 0, 2, 1, Qt.AlignTop)
        nm = label(self.item_name(i), "itemName")
        nm.setToolTip(self.item_name(i))
        nm.setMinimumWidth(40)
        nm.setSizePolicy(QSizePolicy.Ignored, QSizePolicy.Preferred)
        g.addWidget(nm, 0, 1, 1, 3)
        row = QHBoxLayout()
        row.setSpacing(6)
        row.addWidget(label("#%d" % i, "itemId"))
        if i in T.UNUSED_ITEMS:
            row.addWidget(label(L["cut"], "badgeCut"))
        row.addStretch(1)
        chk = QCheckBox()
        chk.setToolTip(L["owned1"])
        chk.setCursor(Qt.PointingHandCursor)
        row.addWidget(chk)
        q = QSpinBox()
        q.setRange(0, 255)
        q.setFixedWidth(68)
        q.setKeyboardTracking(False)
        q.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        q.setToolTip(L["qty"])
        row.addWidget(q)
        g.addLayout(row, 1, 1, 1, 3)
        g.setColumnStretch(1, 1)
        c.chk, c.qty, c.name = chk, q, self.item_name(i).lower()
        chk.toggled.connect(lambda on, i=i: self.commit_item(i, have=on))
        q.valueChanged.connect(lambda val, i=i: self.commit_item(i, qty=val))
        return c

    def eventFilter(self, obj, e):
        try:
            if e.type() == e.Type.Resize and obj is self.item_scroll.viewport():
                QTimer.singleShot(0, self.layout_items)
        except (RuntimeError, AttributeError):
            return False
        return super().eventFilter(obj, e)

    def visible_items(self):
        flt = self.item_search.text().strip().lower()
        out = []
        for i, c in self.item_cards.items():
            if flt and flt not in c.name and flt != str(i) and flt != "#%d" % i:
                continue
            if not self.item_cut.isChecked() and i in T.UNUSED_ITEMS:
                continue
            if self.item_owned.isChecked() and not c.chk.isChecked():
                continue
            out.append(i)
        return out

    def layout_items(self, force=False):
        if not self.item_cards:
            return
        width = max(self.item_scroll.viewport().width(), self.pages.width() - 14) - 8
        cols = max(1, min(6, (width + 10) // 236))
        vis = self.visible_items()
        key = (cols, tuple(vis))
        if key == getattr(self, "_item_key", None) and not force:
            return
        self._item_key = key
        while self.item_grid.count():
            self.item_grid.takeAt(0)
        for c in self.item_cards.values():
            c.hide()
        for k, i in enumerate(vis):
            c = self.item_cards[i]
            self.item_grid.addWidget(c, k // cols, k % cols)
            c.show()
        for col in range(6):
            self.item_grid.setColumnStretch(col, 1 if col < cols else 0)
        self.item_grid.setRowStretch(len(vis) // cols + 1, 1)
        self.item_grid.activate()
        self.item_host.adjustSize()
        self.item_count.setText(self.L["shown"].format(len(vis)))

    def filter_items(self, *a):
        self.layout_items()

    # events
    def build_events(self):
        L, t = self.L, self.theme
        w = QWidget()
        v = QVBoxLayout(w)
        v.setContentsMargins(0, 0, 0, 16)
        v.setSpacing(12)
        self.ev_search = QLineEdit()
        self.ev_search.setPlaceholderText(L["search_events"])
        self.ev_search.addAction(self.icon("search", 16, t["muted"]), QLineEdit.LeadingPosition)
        self.ev_search.setClearButtonEnabled(True)
        self.ev_search.setMinimumWidth(300)
        grp = QButtonGroup(w)
        chips = []
        for k, key in enumerate(("all", "ns", "st", "done")):
            b = QPushButton(L[key])
            b.setObjectName("chip")
            b.setCheckable(True)
            b.setChecked(k == 0)
            b.setCursor(Qt.PointingHandCursor)
            b.clicked.connect(lambda _=False, k=k: self.set_ev_state_filter(k - 1))
            grp.addButton(b)
            chips.append(b)
        self.ev_cut = QPushButton(L["showcut"])
        self.ev_cut.setObjectName("chip")
        self.ev_cut.setCheckable(True)
        self.ev_cut.setChecked(True)
        self.ev_count = label("", "small")
        v.addLayout(hbox(self.ev_search, 8, *chips, 8, self.ev_cut, None, self.ev_count))
        self.ev_model = EventModel(self)
        self.ev_proxy = EventFilter()
        self.ev_proxy.setSourceModel(self.ev_model)
        tv = QTableView()
        self.ev_view = tv
        tv.setModel(self.ev_proxy)
        tv.setItemDelegate(EventDelegate(self))
        tv.setSelectionBehavior(QAbstractItemView.SelectRows)
        tv.setSelectionMode(QAbstractItemView.ExtendedSelection)
        tv.setEditTriggers(QAbstractItemView.NoEditTriggers)
        tv.setShowGrid(False)
        tv.setAlternatingRowColors(True)
        tv.verticalHeader().hide()
        tv.verticalHeader().setDefaultSectionSize(36)
        hh = tv.horizontalHeader()
        hh.setSectionResizeMode(1, QHeaderView.Stretch)
        hh.setHighlightSections(False)
        tv.setColumnWidth(0, 56)
        tv.setColumnWidth(2, 190)
        tv.setColumnWidth(3, 80)
        tv.doubleClicked.connect(self.cycle_events)
        tv.setContextMenuPolicy(Qt.CustomContextMenu)
        tv.customContextMenuRequested.connect(self.event_menu)
        v.addWidget(tv, 1)
        self.ev_search.textChanged.connect(self.filter_events)
        self.ev_cut.toggled.connect(self.filter_events)

        def b(text, icon, fn, name=None):
            x = QPushButton(text)
            if name:
                x.setObjectName(name)
            x.setIcon(self.icon(icon, 15, t["primary_t"] if name == "primary" else None))
            x.setCursor(Qt.PointingHandCursor)
            x.clicked.connect(fn)
            return x
        self.ev_raw = QSpinBox()
        self.ev_raw.setRange(0, 255)
        self.ev_raw.setFixedWidth(72)
        sel = label(L["sel"] + ":", "fieldLabel")
        v.addLayout(hbox(sel, b(L["ns"], "reset", lambda: self.set_events(0)),
                         b(L["st"], "flag", lambda: self.set_events(1)),
                         b(L["done"], "check", lambda: self.set_events(255)), 10,
                         label(L["raw"], "muted"), self.ev_raw, b(L["set"], "check", lambda: self.set_events(
                             self.ev_raw.value())), None,
                         b(L["reset_all"], "reset", lambda: self.set_events(0, True)),
                         b(L["complete_all"], "check", lambda: self.set_events(255, True), "primary")))
        return w

    def set_ev_state_filter(self, k):
        self.ev_proxy.state = k
        self.filter_events()

    def filter_events(self, *a):
        self.ev_proxy.text = self.ev_search.text().strip()
        self.ev_proxy.cut = self.ev_cut.isChecked()
        self.ev_proxy.invalidateFilter()
        self.ev_count.setText(self.L["shown"].format(self.ev_proxy.rowCount()))

    def event_menu(self, pos):
        L = self.L
        m = QMenu(self)
        for txt, v in ((L["ns"], 0), (L["st"], 1), (L["done"], 255)):
            m.addAction(txt, lambda v=v: self.set_events(v))
        m.exec(self.ev_view.viewport().mapToGlobal(pos))

    def selected_events(self, shown=False):
        if shown:
            return [self.ev_proxy.mapToSource(self.ev_proxy.index(r, 0)).row() for r in range(self.ev_proxy.rowCount())]
        return sorted({self.ev_proxy.mapToSource(ix).row() for ix in self.ev_view.selectionModel().selectedRows()})

    # misc
    def build_misc(self):
        L, t = self.L, self.theme
        inner = QWidget()
        g = QGridLayout(inner)
        g.setContentsMargins(0, 0, 8, 20)
        g.setSpacing(16)
        p = Panel(L["time"], "clock", t)
        r = QHBoxLayout()
        r.setSpacing(6)
        self.time_spins = []
        for lo, hi, unit in ((0, 999, L["h"]), (0, 59, L["m"]), (0, 59, L["s"])):
            s = QSpinBox()
            s.setRange(lo, hi)
            s.setKeyboardTracking(False)
            s.setFixedWidth(84)
            s.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
            s.valueChanged.connect(self.commit_time)
            self.time_spins.append(s)
            r.addWidget(s)
            r.addWidget(label(unit, "muted"))
            r.addSpacing(8)
        r.addStretch(1)
        p.lay.addLayout(r)
        p.lay.addWidget(label("max 999:59:00", "hint"))
        self.time_err = label("", "err")
        self.time_err.hide()
        p.lay.addWidget(self.time_err)
        p2 = Panel(L["progress"], "flag", t)
        for k in ("purified_areas", "cur_area", "cur_section"):
            p2.lay.addWidget(self.field(k, False))
        p3 = Panel(L["names_lang"], "disc", t)
        self.names_cb = QComboBox()
        for code, n in T.LANGS.items():
            self.names_cb.addItem(n, code)
        self.names_cb.setCurrentIndex(list(T.LANGS).index(self.names_lang()))
        self.names_cb.currentIndexChanged.connect(self.set_names_lang)
        p3.lay.addWidget(self.names_cb)
        p3.lay.addWidget(label(L["datahint"], "hint", wrap=True))
        p4 = Panel(L["diag"], "info", t)
        self.diag = QPlainTextEdit()
        self.diag.setReadOnly(True)
        self.diag.setMinimumHeight(200)
        p4.lay.addWidget(self.diag)
        left = QVBoxLayout()
        left.setSpacing(16)
        left.addWidget(p)
        left.addWidget(p3)
        left.addStretch(1)
        right = QVBoxLayout()
        right.setSpacing(16)
        right.addWidget(p2)
        right.addStretch(1)
        g.addLayout(left, 0, 0)
        g.addLayout(right, 0, 1)
        g.addWidget(p4, 1, 0, 1, 2)
        g.setColumnStretch(0, 1)
        g.setColumnStretch(1, 1)
        g.setRowStretch(2, 1)
        return self.scroll(inner)

    # ---- edits (live, one undo step each; a burst on the same control is one step)
    def edit(self, label_, key, fn):
        if not self.save or self.loading:
            return False
        before = bytes(self.card.data)
        try:
            fn()
        except T.SaveError as e:
            self.card.data[:] = before
            self.set_status(str(e), "err")
            return str(e)
        if bytes(self.card.data) != before:
            now = time.monotonic()
            h = self.history
            if not (h and key and h[-1][1] == key and now - h[-1][3] < 1.5):
                h.append([label_, key, before, now])
                del h[:-500]
            else:
                h[-1][3] = now
        self.after_edit()
        return None

    def commit_field(self, row, v):
        k, s = row.key, self.save
        if s is None or self.loading:
            return

        def fn():
            if k == "health" and v > s.get("max_health"):
                raise T.SaveError(self.L["hpmax"].format(s.get("max_health")))
            if k == "max_health" and s.get("health") > v:
                s.set("health", v)
            s.set(k, v)
        err = self.edit(FIELD_LABELS[self.lang].get(k, k), "f:" + k, fn)
        row.show_error(err)
        if err:
            return
        if k == "max_health" and "health" in self.fields:
            self.fields["health"].set(s.get("health"))
        if k == "golden_bowl" and "max_health" in self.fields:
            self.fields["max_health"].show_error(None)

    def commit_area(self, ix):
        if self.save and not self.loading:
            self.edit(FIELD_LABELS[self.lang]["area"], "f:area", lambda: self.save.set("area", self.area_cb.itemData(ix)))

    def commit_time(self, *a):
        if not self.save or self.loading:
            return
        h, m, s = (x.value() for x in self.time_spins)
        sec = h * 3600 + m * 60 + s
        cur = self.save.get("time")
        if cur // 50 == sec:
            return
        err = self.edit(self.L["time"], "f:time", lambda: self.save.set("time", sec * 50))
        self.time_err.setText(err or "")
        self.time_err.setVisible(bool(err))

    def commit_item(self, i, have=None, qty=None):
        if not self.save or self.loading:
            return
        s, c = self.save, self.item_cards[i]
        if have is not None:
            q = max(1, c.qty.value()) if have else 0
        else:
            q = qty
        err = self.edit(self.L["edit_item"].format(i), "i:%d" % i, lambda: s.set_item(i, q))
        if not err:
            self.sync_item(i)

    def sync_item(self, i):
        s, c = self.save, self.item_cards[i]
        self.loading = True
        own = i in s.inventory()
        c.chk.setChecked(own)
        c.qty.setValue(s.item(i))
        c.setProperty("owned", own)
        repolish(c)
        self.loading = False

    def set_events(self, v, shown=False):
        ids = self.selected_events(shown)
        if not ids:
            return
        self.edit(self.L["edit_ev"], None, lambda: [self.save.set_event(i, v) for i in ids])
        self.ev_model.changed()
        self.filter_events()

    def cycle_events(self, *a):
        def fn():
            for i in self.selected_events():
                e = self.save.event(i)
                self.save.set_event(i, 1 if e == 0 else 255 if e != 255 else 0)
        self.edit(self.L["edit_ev"], None, fn)
        self.ev_model.changed()
        self.filter_events()

    def after_edit(self):
        self.refresh_chrome()
        self.refresh_overview()
        self.refresh_slots(light=True)

    def undo(self):
        if not self.history or not self.card:
            self.set_status(self.L["nothing"], "info")
            return
        lab, _, before, _ = self.history.pop()
        self.card.data[:] = before
        self.refresh_all()
        self.set_status(self.L["undone"].format(lab), "info")

    def dirty(self):
        return bool(self.card) and bytes(self.card.data) != self.clean

    # ---- refresh
    def refresh_all(self):
        self.refresh_chrome()
        self.refresh_slots()
        self.refresh_data_card()
        has = self.save is not None
        self.main_stack.setCurrentIndex(1 if has else 0)
        if has:
            self.load_fields()
        self.ev_model.refresh()
        self.filter_events()

    def refresh_chrome(self):
        L = self.L
        d = self.dirty()
        self.dirty_chip.setVisible(d)
        self.file_chip.setText(os.path.basename(self.card.path) if self.card else L["nocard"])
        self.save_btn.setEnabled(bool(self.save) and d)
        self.saveas_btn.setEnabled(bool(self.save))
        self.undo_btn.setEnabled(bool(self.history))
        self.setWindowTitle(("● " if d else "") + (os.path.basename(self.card.path) + " — " if self.card else "") + APP)
        if self.save:
            ok = all(s.checksum_ok() for s in self.saves)
            self.status_right.setText("%s: %s" % (L["checksum"], L["ok"] if ok else L["bad"]))

    def refresh_data_card(self):
        L, t = self.L, self.theme
        if self.data_path:
            self.data_icon.setPixmap(svg_pixmap("disc", t["ok"], 18, 2))
            self.data_title.setText(os.path.basename(self.data_path.rstrip("/\\")) or self.data_path)
            self.data_text.setText(L["datafrom"].format(self.data_path, len(self.names["items"]),
                                                        len(self.names["events"])) + "\n" + L["datahint"])
        else:
            self.data_icon.setPixmap(svg_pixmap("info", t["warn"], 18, 2))
            self.data_title.setText(L["nodata"])
            self.data_text.setText(L["nodata2"])

    def refresh_slots(self, light=False):
        L, t = self.L, self.theme
        if self.card:
            self.card_name.setText(os.path.basename(self.card.path))
            n = len(self.saves)
            fmt = {"raw": "raw .mcd", "gme": "DexDrive .gme", "vgs": ".vgs", "mcs": ".mcs"}.get(self.card.kind, "")
            self.card_meta.setText("%s  ·  %s" % (L["save1"] if n == 1 else L["saves"].format(n), fmt))
        else:
            self.card_name.setText(L["nocard"])
            self.card_meta.setText(L["drop"])
        if light and getattr(self, "slot_cards", None):
            for s, c in zip(self.saves, self.slot_cards):
                self.fill_slot_card(c, s)
            return
        while self.slot_lay.count() > 1:
            it = self.slot_lay.takeAt(0)
            if it.widget():
                it.widget().deleteLater()
        self.slot_cards = []
        for k, s in enumerate(self.saves):
            c = Clickable()
            c.setObjectName("slotCard")
            c.setCursor(Qt.PointingHandCursor)
            c.setProperty("selected", s is self.save)
            g = QGridLayout(c)
            g.setContentsMargins(12, 12, 12, 12)
            g.setHorizontalSpacing(12)
            g.setVerticalSpacing(3)
            ic = QLabel()
            ic.setObjectName("iconWell")
            ic.setFixedSize(48, 48)
            ic.setAlignment(Qt.AlignCenter)
            ic.setPixmap(scaled(save_icon(s), 32))
            g.addWidget(ic, 0, 0, 3, 1, Qt.AlignTop)
            c.title, c.reg = label("", "slotTitle"), label(s.region.split(" ")[0], "badge")
            g.addLayout(hbox(c.title, None, c.reg, spacing=6), 0, 1)
            c.loc = label("", "slotMeta")
            g.addWidget(c.loc, 1, 1)
            c.meta = label("", "slotMeta")
            g.addWidget(c.meta, 2, 1)
            g.setColumnStretch(1, 1)
            c.clicked.connect(lambda k=k: self.pick(k))
            self.fill_slot_card(c, s)
            self.slot_lay.insertWidget(k, c)
            self.slot_cards.append(c)

    def fill_slot_card(self, c, s):
        L = self.L
        c.title.setText("%s %02d" % (L["slot"], s.slot))
        c.loc.setText(self.area_name(s.get("area")))
        c.meta.setText("%s   ·   %s %d   ·   %s AP" % (fmt_time(s.get("time")), L["lv"], s.get("level"),
                                                     format(s.get("ap"), ",")))

    def refresh_overview(self):
        if not self.save:
            return
        s, L, t = self.save, self.L, self.theme
        self.hero_icon.setPixmap(scaled(save_icon(s), 64))
        self.hero_title.setText("%s %02d  ·  %s" % (L["slot"], s.slot, self.area_name(s.get("area"))))
        self.hero_sub.setText(unicodedata.normalize("NFKC", s.title()).replace("\u2212", "-"))
        while self.hero_badges.count():
            it = self.hero_badges.takeAt(0)
            if it.widget():
                it.widget().deleteLater()
        ok = s.checksum_ok()
        for txt, name in ((s.region, "badge"), ("%s %d" % (L["block"], s.block), "badge"),
                          ("%s %s" % (L["checksum"], L["ok"] if ok else L["bad"]), "badgeOk" if ok else "badgeErr")):
            self.hero_badges.addWidget(label(txt, name))
        self.hero_badges.addStretch(1)
        st = self.stats
        st["playtime"][0].setText(fmt_time(s.get("time")))
        st["playtime"][1].setText("h:mm:ss")
        st["ap"][0].setText(format(s.get("ap"), ","))
        st["ap"][1].setText("max 9,999,999")
        st["level"][0].setText(str(s.get("level")))
        st["level"][1].setText("R %d · G %d · B %d" % (s.get("red_level"), s.get("green_level"), s.get("blue_level")))
        st["lives"][0].setText(str(s.get("lives")))
        st["lives"][1].setText("P = %02d" % s.get("lives"))
        st["hp"][0].setText("%d / %d" % (s.get("health"), s.get("max_health")))
        st["hp"][1].setText(FIELD_LABELS[self.lang]["golden_bowl"] + (" ✓" if s.get("golden_bowl") else " –"))
        st["location"][0].setText("%02d · %d" % (s.get("area"), s.get("section")))
        st["location"][1].setText(self.area_name(s.get("area")))
        done = sum(1 for i in range(T.EVENT_USED) if s.event(i) == 255 and i not in T.UNUSED_EVENTS)
        tot = T.EVENT_USED - len(T.UNUSED_EVENTS)
        self.progress["evdone"][0].setText(L["events_n"].format(done, tot))
        self.progress["evdone"][1].setRange(0, tot)
        self.progress["evdone"][1].setValue(done)
        n_inv = len(s.inventory())
        tot_i = max(len(self.names["items"]), 0xAB) - len(T.UNUSED_ITEMS)
        self.progress["itemsown"][0].setText(L["events_n"].format(n_inv, tot_i))
        self.progress["itemsown"][1].setRange(0, tot_i)
        self.progress["itemsown"][1].setValue(min(n_inv, tot_i))

    def load_fields(self):
        s = self.save
        self.loading = True
        try:
            for k, f in self.fields.items():
                f.set(s.get(k))
            a = s.get("area")
            ix = self.area_cb.findData(a)
            self.area_cb.setCurrentIndex(max(ix, 0))
            t = s.get("time") // 50
            for sp, v in zip(self.time_spins, (t // 3600, t // 60 % 60, t % 60)):
                sp.setValue(v)
            self.time_err.hide()
        finally:
            self.loading = False
        if not self.item_cards:
            self.make_item_cards()
        for i in self.item_cards:
            self.sync_item(i)
        self.layout_items(force=True)
        self.refresh_overview()
        self.diag.setPlainText(self.diagnostics())

    def diagnostics(self):
        s = self.save
        lines = ["%s  %s" % (self.card.path, self.card.kind)]
        for b, n, sz in self.card.directory():
            lines.append("  block %2d  %-20s %6d bytes" % (b, n, sz))
        lines.append("")
        for x in self.saves:
            lines.append(unicodedata.normalize("NFKC", x.summary()))
        lines.append("")
        lines.append("inventory (%d): %s" % (len(s.inventory()), ", ".join(self.item_name(i) for i in s.inventory())))
        return "\n".join(lines)

    # ---- status
    def set_status(self, msg, sev="info"):
        t = self.theme
        self.status.setText(msg)
        self.status.setProperty("sev", sev)
        repolish(self.status)
        ic = {"ok": ("check", t["ok"]), "err": ("info", t["err"])}.get(sev, ("info", t["muted"]))
        self.status_icon.setPixmap(svg_pixmap(ic[0], ic[1], 16, 2) if msg else QPixmap())
        self.status_msg = (msg, sev)

    # ---- actions
    def pick(self, k):
        self.save = self.saves[k]
        for c, s in zip(self.slot_cards, self.saves):
            c.setProperty("selected", s is self.save)
            repolish(c)
        self.load_fields()
        self.ev_model.refresh()
        self.filter_events()
        self.refresh_chrome()

    def ask_open(self):
        p, _ = QFileDialog.getOpenFileName(self, self.L["open"], self.cfg.get("last_dir", ""), CARD_FILTER)
        if p:
            self.open_card(p)

    def confirm_discard(self):
        if not self.dirty():
            return True
        r = QMessageBox.question(self, APP, self.L["quit"] + "\n" + self.L["quit2"],
                                 QMessageBox.Discard | QMessageBox.Cancel, QMessageBox.Cancel)
        return r == QMessageBox.Discard

    def open_card(self, path, confirm=True):
        if confirm and not self.confirm_discard():
            return False
        try:
            card = T.Card(path)
            saves = card.saves()
        except Exception as e:
            self.set_status(self.L["openerr"].format(e), "err")
            return False
        if not saves:
            self.set_status(self.L["nosave"].format(os.path.basename(path)), "err")
            return False
        self.card, self.saves, self.save = card, saves, saves[0]
        self.clean = bytes(card.data)
        self.history = []
        self.cfg["last_dir"] = os.path.dirname(os.path.abspath(path))
        self.save_cfg()
        self.refresh_all()
        self.set_status(self.L["opened"].format(path), "ok")
        return True

    def ask_data(self):
        L = self.L
        p, _ = QFileDialog.getOpenFileName(self, L["pickdata"], self.cfg.get("data", ""),
                                           "Disc image (*.bin *.cue *.iso *.img);;All files (*)")
        if not p:
            p = QFileDialog.getExistingDirectory(self, L["pickdir"], self.cfg.get("data", ""))
        if p:
            self.load_data(p)

    def load_data(self, path, quiet=False):
        QApplication.setOverrideCursor(Qt.WaitCursor)
        try:
            names = T.names_from_disc(path, self.names_lang())
            try:
                icons = item_icons(path)
            except Exception:
                icons = {}
            self.names, self.icons, self.data_path = names, icons, path
            self.cfg["data"] = path
            self.save_cfg()
            ok = True
        except Exception as e:
            ok = False
            if not quiet:
                self.set_status(self.L["dataerr"].format(e), "err")
        finally:
            QApplication.restoreOverrideCursor()
        if ok:
            self.set_status(self.L["loaded"].format(path), "ok")
            if self.item_cards:
                self.make_item_cards()
            self.refresh_all()
        return ok

    def set_names_lang(self, ix):
        self.cfg["names_lang"] = self.names_cb.itemData(ix)
        self.save_cfg()
        if self.data_path:
            self.load_data(self.data_path)

    def rebuild(self):
        tab = self.tab_group.checkedId() if hasattr(self, "tab_group") else 0
        self.cfg["tab"] = tab
        self.item_cards = {}
        msg = self.status_msg
        old = self.centralWidget()
        self.build()
        if old:
            old.deleteLater()
        self._item_key = None
        self.refresh_all()
        self.set_status(*msg)

    def set_lang(self, code):
        if code == self.lang:
            return
        self.lang = self.cfg["ui"] = code
        self.save_cfg()
        self.rebuild()

    def toggle_theme(self):
        self.theme_name = self.cfg["theme"] = "light" if self.theme_name == "dark" else "dark"
        self.save_cfg()
        self.apply_theme()
        self.rebuild()

    def validate(self):
        for s in self.saves:
            if s.get("health") > s.get("max_health"):
                raise T.SaveError("%s %02d: %s" % (self.L["slot"], s.slot, self.L["hpmax"].format(s.get("max_health"))))
            if s.get("max_health") > 8 and not s.get("golden_bowl"):
                raise T.SaveError("%s %02d: max_health > 8 needs the golden bowl" % (self.L["slot"], s.slot))
            if not s.checksum_ok():
                s.fix_checksum()

    def write(self, out):
        try:
            self.validate()
            same = os.path.abspath(out) == os.path.abspath(self.card.path)
            self.card.save(out)
            if not same:
                self.card.path = out
                self.card.orig = open(out, "rb").read()
        except Exception as e:
            self.set_status(self.L["saveerr"].format(e), "err")
            return False
        self.clean = bytes(self.card.data)
        self.refresh_chrome()
        self.refresh_slots(light=True)
        self.card_name.setText(os.path.basename(out))
        bak = out + ".bak"
        if same and os.path.exists(bak):
            self.set_status(self.L["savedbak"].format(out, os.path.basename(bak)), "ok")
        else:
            self.set_status(self.L["saved"].format(out), "ok")
        return True

    def do_save(self):
        if self.save and self.dirty():
            self.write(self.card.path)

    def do_save_as(self):
        if not self.save:
            return
        base, ext = os.path.splitext(self.card.path)
        p, _ = QFileDialog.getSaveFileName(self, self.L["saveas"], base + "_edit" + ext, CARD_FILTER)
        if p:
            self.write(p)

    # ---- window events
    def dragEnterEvent(self, e):
        if e.mimeData().hasUrls() and any(u.toLocalFile().lower().endswith(CARD_EXTS) or os.path.isfile(u.toLocalFile())
                                          for u in e.mimeData().urls()):
            e.acceptProposedAction()

    def dropEvent(self, e):
        for u in e.mimeData().urls():
            p = u.toLocalFile()
            if os.path.isfile(p):
                self.open_card(p)
                break

    def closeEvent(self, e):
        if self.confirm_discard():
            self.cfg["tab"] = self.tab_group.checkedId()
            self.save_cfg()
            e.accept()
        else:
            e.ignore()


def make_app(argv=None):
    app = QApplication.instance() or QApplication(argv or sys.argv)
    for f in ("Nunito-Regular.ttf", "Nunito-Bold.ttf", "Nunito-ExtraBold.ttf", "LilitaOne-Regular.ttf"):
        QFontDatabase.addApplicationFont(os.path.join(ASSETS, "fonts", f))
    app.setFont(QFont("Nunito", 10))
    app.setApplicationName(APP)
    return app


def main():
    app = make_app()
    w = Editor(sys.argv[1] if len(sys.argv) > 1 else None)
    app.setWindowIcon(QIcon(w.logo_pixmap(64)))
    w.show()
    return app.exec()


if __name__ == "__main__":
    sys.exit(main())
