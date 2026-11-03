#!/usr/bin/env python3
"""Tombi! translation editor (Windows exe: tombi-text.exe).  Qt (PySide6), light / dark theme, Spanish and English interface.

Opens the player's own PAL disc (image or extracted folder) and lists EVERY text a translation patch can change, by
category: dialogues, signs, system messages, item names and descriptions, event titles / list names / hints, places
(save list, warp list), memory card screen, title and options menus, pause menus (tools/text_catalog.py).  For each
entry it shows the disc's original text of the chosen language, the improved translation (patches/es_mejorada, Spanish),
and your text, with search and filters; optionally the same entry in the other languages of the disc and in the Japanese
release (tools/jp_text.py, aligned with the PAL English text, which is shown as its translation).  While you type, the
validator of tools/check_text_patch.py checks the entry (glyphs, box size, pages, control codes) and the box is drawn as
the game draws it (tools/text_preview.py).

The result is a translation file (*.tombipatch.json, tools/text_patch.py).  "Install in the game" also compiles it to
translation.tpatch (tools/tpatch_build.py, from your disc) and copies both next to the game's executable: tombi2 loads
that file by itself and it wins over the bundled improved translation (docs/PORT.md "Your own translation file").
No game data is stored in this program: it reads your disc at run time.  No online service is used.

  python tools/text_editor.py [disc.bin | data_folder] [translation.tombipatch.json]
"""
import html, io, json, os, re, shutil, sys, tempfile, threading, traceback

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n
from i18n import tr
import text_strings  # noqa: F401  (the core's own messages)
import text_core as C
import text_patch as TP
import text_catalog as T
import wfm_py as W

from PySide6.QtCore import Qt, QThread, Signal, QTimer, QAbstractTableModel, QModelIndex, QSize
from PySide6.QtGui import (QAction, QColor, QFont, QIcon, QPalette, QPixmap, QSyntaxHighlighter, QTextCharFormat,
                           QGuiApplication, QKeySequence)
from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QGridLayout, QLabel,
                               QPushButton, QLineEdit, QComboBox, QCheckBox, QSplitter, QTreeWidget, QTreeWidgetItem,
                               QTableView, QHeaderView, QPlainTextEdit, QTextBrowser, QListWidget, QListWidgetItem,
                               QScrollArea, QFileDialog, QMessageBox, QDialog, QDialogButtonBox, QRadioButton,
                               QFormLayout, QProgressDialog, QStackedWidget, QToolButton, QMenu, QFrame, QSizePolicy,
                               QAbstractItemView, QGroupBox)

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
INSTALL_JSON = "translation.tombipatch.json"     # the fixed names tombi2 looks for next to its executable
INSTALL_TPATCH = "translation.tpatch"
LANG_NAMES = {"es": {"B": "Inglés", "F": "Francés", "G": "Alemán", "I": "Italiano", "H": "Neerlandés", "S": "Español",
                     "P": "Portugués", "W": "Sueco", "D": "Danés", "J": "Japonés"},
              "en": {"B": "English", "F": "French", "G": "German", "I": "Italian", "H": "Dutch", "S": "Spanish",
                     "P": "Portuguese", "W": "Swedish", "D": "Danish", "J": "Japanese"}}

_S = {
    "es": {
        "tt.title": "Tombi! – editor de traducciones", "tt.opendisc": "Abrir disco…", "tt.new": "Nueva traducción…",
        "tt.open": "Abrir traducción…", "tt.save": "Guardar", "tt.saveas": "Guardar como…", "tt.checkall": "Comprobar todo",
        "tt.install": "Instalar en el juego…", "tt.more": "Más", "tt.compile": "Compilar para el juego (.tpatch)…",
        "tt.iso": "Generar ISO parcheada…", "tt.exporttxt": "Exportar diálogos a texto…", "tt.importtxt": "Importar diálogos de texto…",
        "tt.jpdisc": "Elegir disco japonés…", "tt.help": "Ayuda", "tt.theme": "Tema oscuro", "tt.uilang": "Idioma / Language:",
        "tt.gamelang": "Idioma del juego:", "tt.search": "Buscar…", "tt.show": "Mostrar:", "tt.others": "Otros idiomas",
        "tt.japanese": "Japonés",
        "f.all": "Todo", "f.changed": "Solo cambiados", "f.untrans": "Solo sin traducir", "f.diffimp": "Distintos de la mejorada",
        "f.errors": "Con errores",
        "c.all": "Todos los textos", "c.dialogue": "Diálogos", "c.sign": "Carteles", "c.system": "Avisos del sistema",
        "c.item_name": "Objetos: nombres", "c.item_desc": "Objetos: descripciones", "c.event_title": "Eventos: títulos",
        "c.event_name": "Eventos: nombres en la lista", "c.event_hint": "Eventos: pistas", "c.place": "Lugares",
        "c.memcard": "Tarjeta de memoria", "c.title_menu": "Menú de título y opciones", "c.pause": "Menús de pausa",
        "c.jp_only": "Solo en la versión japonesa",
        "g.banner": "Cartel «evento superado»", "g.titles": "Títulos de evento", "g.verbs": "Verbos del aviso de objeto",
        "g.popups": "Mensajes emergentes", "g.dialogue_names": "Nombres en los diálogos", "g.list_names": "Nombres en el inventario",
        "g.event_list": "Lista de eventos", "g.event_hints": "Pistas de la lista de eventos", "g.save_list": "Lista de partidas guardadas",
        "g.warp_list": "Lista de viaje", "g.messages": "Mensajes", "g.title": "Pantalla de título", "g.options": "Opciones",
        "g.pause": "Pausa", "g.item_desc": "Descripciones («Ver»)", "g.area": "Zona {0}",
        "col.id": "Entrada", "col.orig": "Original del disco", "col.imp": "Mejorada", "col.cur": "Tu traducción", "col.state": "Estado",
        "st.same": "sin traducir", "st.changed": "cambiado", "st.err": "error", "st.jp": "solo JP",
        "d.orig": "Original del disco ({0})", "d.imp": "Traducción mejorada", "d.cur": "Tu traducción", "d.noimp": "(no hay traducción mejorada para este idioma)",
        "d.useorig": "Copiar original", "d.useimp": "Copiar mejorada", "d.fit": "Ajustar caja", "d.checks": "Comprobación",
        "d.preview": "Vista previa (como en el juego)", "d.nopreview": "Este texto no tiene vista previa de caja; se comprueba que cabe y que el juego tiene sus letras.",
        "d.others": "Otros idiomas del disco", "d.jp": "Japonés (versión japonesa, Ore! Tomba)",
        "d.jptrans": "Traducción: el texto inglés PAL emparejado con esta línea (no es una traducción automática).",
        "d.jpnone": "Esta entrada no tiene línea japonesa emparejada.", "d.jpnopal": "Línea japonesa sin equivalente en la versión PAL: no hay traducción.",
        "d.jpnoblock": "Bloque japonés que no se pudo emparejar con ningún archivo PAL (p. ej. títulos de evento): sin traducción.",
        "d.jpoff": "Marca «Japonés» para ver la línea de la versión japonesa.", "d.jpbuilding": "Leyendo y emparejando el disco japonés…",
        "d.targets": "Se escribe en {0} sitios", "d.readonly": "Solo lectura: esta línea no existe en el disco PAL.",
        "k.ok": "Sin problemas: el juego puede mostrar este texto.", "k.box": "Falta o sobra la etiqueta {BOX ancho alto}: déjala como en el original.",
        "k.glyph": "El carácter «{0}» no existe en las fuentes del juego: usa otro.", "k.wide": "Una línea es más ancha que la caja: {0}. Corta la línea con {{NL}} o acorta el texto.",
        "k.lines": "Demasiadas líneas en una página: añade {WAIT}{CLEAR} para pasar de página, o acorta.",
        "k.chars": "Más de 128 letras en una página: el juego no lo admite. Divide la página con {WAIT}{CLEAR}.",
        "k.codes": "Los códigos de control no coinciden con el original. Tu texto: {0} · original: {1}",
        "k.taller": "La caja es más alta que la original ({0}).", "k.wider": "La caja es más ancha de lo permitido ({0}).",
        "k.title": "Título de evento: {0}", "k.borrow": "Letras tomadas de otra fuente del disco (correcto): {0}",
        "k.console": "En consola solo se verían las primeras letras ({0}); el port muestra el título entero.",
        "k.memcard": "En la pantalla de la tarjeta de memoria el nombre no cambia (no cabe en su sitio); en el inventario sí.",
        "k.pool": "No hay sitio: {0}. Acorta este u otros textos de la misma pantalla.",
        "k.raw": "{0}", "k.whole": "Comprobación del parche completo",
        "m.nodisc": "Abre primero tu disco de Tombi! (PAL): la imagen .bin/.cue/.iso/.chd o la carpeta data/ extraída.",
        "m.welcome": "<h2>Editor de traducciones de Tombi!</h2><p>1. Pulsa <b>Abrir disco…</b> y elige tu imagen del juego (PAL, Tombi.bin) "
                     "o la carpeta <i>data</i> que extrajo el lanzador.</p><p>2. Empieza una <b>Nueva traducción</b> (desde el texto del disco o desde la "
                     "traducción mejorada) o <b>abre</b> una que ya tengas.</p><p>3. Busca los textos por categoría a la izquierda, edita a la derecha y "
                     "mira la vista previa y la comprobación.</p><p>4. <b>Guardar</b> crea tu archivo de traducción (.tombipatch.json). "
                     "<b>Instalar en el juego…</b> lo prepara y lo copia junto al ejecutable del juego: se carga solo y tiene prioridad sobre la "
                     "traducción mejorada incluida.</p><p>El programa no contiene datos del juego: lo lee todo de tu disco.</p>",
        "m.opening": "Leyendo {0}…", "m.opened": "Disco abierto: {0}", "m.cantopen": "No se pudo abrir", "m.unsaved": "Hay cambios sin guardar",
        "m.unsavedq": "¿Guardar los cambios de tu traducción?", "m.saved": "Traducción guardada: {0} ({1} textos cambiados)",
        "m.newtitle": "Nueva traducción", "m.newlang": "Idioma del juego que traduces:", "m.from": "Empezar desde:",
        "m.fromdisc": "El texto original del disco", "m.fromimp": "La traducción mejorada incluida (solo español)",
        "m.name": "Nombre de la traducción:", "m.author": "Autor:", "m.defname": "Mi traducción",
        "m.which": "Este archivo cambia textos de varios idiomas. ¿Cuál quieres editar?",
        "m.checking": "Comprobando todos los textos cambiados…", "m.checkdone": "Comprobación terminada: {0} textos con problemas.",
        "m.checkok": "Todo correcto: {0} textos cambiados, ningún problema.",
        "m.install": "Instalar en el juego", "m.installdir": "Elige la carpeta del juego (donde está tombi2 / tombi-port)",
        "m.compiling": "Preparando la traducción para el juego (lee tu disco, puede tardar unos minutos)…",
        "m.installed": "Listo. Copiados {0} y {1} en:\n{2}\n\nEl juego carga tu traducción automáticamente y tiene prioridad sobre la traducción mejorada incluida. "
                       "En el lanzador verás «Tu traducción» como parche de texto activo.",
        "m.compiled": "Archivo para el juego creado: {0}", "m.fixfirst": "Hay textos con errores; corrígelos antes de instalar:\n\n{0}",
        "m.needimage": "Para preparar el archivo del juego hace falta la imagen del disco (.bin/.cue/.iso/.chd) o una carpeta data/ sin parches.",
        "m.jpask": "Elige la imagen del disco japonés (Ore! Tomba, SLPS-01144)", "m.jpmissing": "No se encontró el disco japonés. Elígelo en Más → Elegir disco japonés…",
        "m.jpready": "Disco japonés emparejado: {0} líneas con equivalente PAL, {1} solo en japonés.",
        "m.isodone": "ISO generada: {0}", "m.working": "Trabajando…", "m.error": "Error", "m.done": "Hecho",
        "m.exported": "Exportado: {0}", "m.imported": "Importados {0} textos.", "m.nochanges": "No hay cambios que guardar.",
        "m.helptext": "Etiquetas que debes conservar: {BOX ancho alto} tamaño de la caja, {NL} salto de línea, {WAIT}{CLEAR} pasar página, "
                      "{VOICE n}, {COLOR n}, {PAUSE n}… códigos del juego. Los saltos de línea normales del cuadro son solo para leer mejor.\n\n"
                      "Filtros: «Solo sin traducir» = igual que el disco; «Solo cambiados» = distinto del disco.\n\n"
                      "Archivo del juego: «Instalar en el juego» crea translation.tombipatch.json y translation.tpatch junto al ejecutable. "
                      "En Android, importa translation.tpatch desde Opciones → Importar traducción.",
        "m.status": "{0} textos · {1} cambiados · idioma {2}",
        "s.title": "Título", "s.options": "Opción", "s.optionsAfter": "Extra del port", "s.messages": "Mensaje",
        "s.locations": "Lugar", "s.listNames": "Nombre", "s.eventNames": "Evento", "s.placeNames": "Lugar",
        "s.eventDescriptions": "Pista", "s.itemDescriptions": "Descripción", "s.pause": "Pausa",
    },
    "en": {
        "tt.title": "Tombi! – translation editor", "tt.opendisc": "Open disc…", "tt.new": "New translation…",
        "tt.open": "Open translation…", "tt.save": "Save", "tt.saveas": "Save as…", "tt.checkall": "Check all",
        "tt.install": "Install in the game…", "tt.more": "More", "tt.compile": "Compile for the game (.tpatch)…",
        "tt.iso": "Build patched ISO…", "tt.exporttxt": "Export dialogues as text…", "tt.importtxt": "Import dialogues from text…",
        "tt.jpdisc": "Choose Japanese disc…", "tt.help": "Help", "tt.theme": "Dark theme", "tt.uilang": "Idioma / Language:",
        "tt.gamelang": "Game language:", "tt.search": "Search…", "tt.show": "Show:", "tt.others": "Other languages",
        "tt.japanese": "Japanese",
        "f.all": "All", "f.changed": "Only changed", "f.untrans": "Only untranslated", "f.diffimp": "Different from improved",
        "f.errors": "With errors",
        "c.all": "All texts", "c.dialogue": "Dialogues", "c.sign": "Signs", "c.system": "System messages",
        "c.item_name": "Items: names", "c.item_desc": "Items: descriptions", "c.event_title": "Events: titles",
        "c.event_name": "Events: list names", "c.event_hint": "Events: hints", "c.place": "Places",
        "c.memcard": "Memory card", "c.title_menu": "Title and options menus", "c.pause": "Pause menus",
        "c.jp_only": "Only in the Japanese release",
        "g.banner": "\"Event cleared\" banner", "g.titles": "Event titles", "g.verbs": "Item popup verbs",
        "g.popups": "Popup messages", "g.dialogue_names": "Names in dialogues", "g.list_names": "Names in the inventory",
        "g.event_list": "Event list", "g.event_hints": "Event list hints", "g.save_list": "Save file list",
        "g.warp_list": "Travel list", "g.messages": "Messages", "g.title": "Title screen", "g.options": "Options",
        "g.pause": "Pause", "g.item_desc": "Descriptions (\"Look\")", "g.area": "Area {0}",
        "col.id": "Entry", "col.orig": "Disc original", "col.imp": "Improved", "col.cur": "Your translation", "col.state": "State",
        "st.same": "untranslated", "st.changed": "changed", "st.err": "error", "st.jp": "JP only",
        "d.orig": "Disc original ({0})", "d.imp": "Improved translation", "d.cur": "Your translation", "d.noimp": "(no improved translation for this language)",
        "d.useorig": "Copy original", "d.useimp": "Copy improved", "d.fit": "Fit box", "d.checks": "Check",
        "d.preview": "Preview (as in the game)", "d.nopreview": "This text has no box preview; it is checked to fit and to use letters the game has.",
        "d.others": "Other languages of the disc", "d.jp": "Japanese (Japanese release, Ore! Tomba)",
        "d.jptrans": "Translation: the PAL English text aligned with this line (not a machine translation).",
        "d.jpnone": "This entry has no aligned Japanese line.", "d.jpnopal": "Japanese line with no counterpart in the PAL release: no translation.",
        "d.jpnoblock": "Japanese block that matched no PAL file (e.g. event titles): no translation.",
        "d.jpoff": "Tick \"Japanese\" to see the line of the Japanese release.", "d.jpbuilding": "Reading and aligning the Japanese disc…",
        "d.targets": "Written to {0} places", "d.readonly": "Read only: this line does not exist on the PAL disc.",
        "k.ok": "No problems: the game can show this text.", "k.box": "The {BOX width height} tag is missing or extra: keep it as in the original.",
        "k.glyph": "The character \"{0}\" does not exist in the game's fonts: use another one.", "k.wide": "A line is wider than the box: {0}. Break it with {{NL}} or shorten the text.",
        "k.lines": "Too many lines on one page: add {WAIT}{CLEAR} to turn the page, or shorten it.",
        "k.chars": "More than 128 letters on one page: the game cannot do it. Split the page with {WAIT}{CLEAR}.",
        "k.codes": "The control codes differ from the original. Yours: {0} · original: {1}",
        "k.taller": "The box is taller than the original ({0}).", "k.wider": "The box is wider than allowed ({0}).",
        "k.title": "Event title: {0}", "k.borrow": "Letters taken from another font of the disc (fine): {0}",
        "k.console": "A console would show only the first letters ({0}); the port shows the whole title.",
        "k.memcard": "The memory card screen keeps the old name (no room there); the inventory gets the new one.",
        "k.pool": "No room: {0}. Shorten this or other texts of the same screen.",
        "k.raw": "{0}", "k.whole": "Check of the whole patch",
        "m.nodisc": "Open your Tombi! disc (PAL) first: the .bin/.cue/.iso/.chd image or the extracted data/ folder.",
        "m.welcome": "<h2>Tombi! translation editor</h2><p>1. Press <b>Open disc…</b> and pick your game image (PAL, Tombi.bin) "
                     "or the <i>data</i> folder the launcher extracted.</p><p>2. Start a <b>New translation</b> (from the disc text or from the "
                     "improved translation) or <b>open</b> one you have.</p><p>3. Find texts by category on the left, edit on the right and "
                     "watch the preview and the check.</p><p>4. <b>Save</b> writes your translation file (.tombipatch.json). "
                     "<b>Install in the game…</b> prepares it and copies it next to the game's executable: it loads by itself and wins over the "
                     "bundled improved translation.</p><p>The program holds no game data: it reads everything from your disc.</p>",
        "m.opening": "Reading {0}…", "m.opened": "Disc opened: {0}", "m.cantopen": "Could not open", "m.unsaved": "Unsaved changes",
        "m.unsavedq": "Save the changes of your translation?", "m.saved": "Translation saved: {0} ({1} texts changed)",
        "m.newtitle": "New translation", "m.newlang": "Game language you translate:", "m.from": "Start from:",
        "m.fromdisc": "The disc's original text", "m.fromimp": "The bundled improved translation (Spanish only)",
        "m.name": "Translation name:", "m.author": "Author:", "m.defname": "My translation",
        "m.which": "This file changes texts of several languages. Which one do you want to edit?",
        "m.checking": "Checking every changed text…", "m.checkdone": "Check finished: {0} texts with problems.",
        "m.checkok": "All fine: {0} texts changed, no problems.",
        "m.install": "Install in the game", "m.installdir": "Choose the game folder (where tombi2 / tombi-port is)",
        "m.compiling": "Preparing the translation for the game (reads your disc, can take a few minutes)…",
        "m.installed": "Done. Copied {0} and {1} to:\n{2}\n\nThe game loads your translation by itself and it wins over the bundled improved translation. "
                       "The launcher shows \"Your translation\" as the active text patch.",
        "m.compiled": "Game file written: {0}", "m.fixfirst": "Some texts have errors; fix them before installing:\n\n{0}",
        "m.needimage": "Preparing the game file needs the disc image (.bin/.cue/.iso/.chd) or an unpatched data/ folder.",
        "m.jpask": "Choose the Japanese disc image (Ore! Tomba, SLPS-01144)", "m.jpmissing": "Japanese disc not found. Choose it in More → Choose Japanese disc…",
        "m.jpready": "Japanese disc aligned: {0} lines with a PAL counterpart, {1} only in Japanese.",
        "m.isodone": "ISO built: {0}", "m.working": "Working…", "m.error": "Error", "m.done": "Done",
        "m.exported": "Exported: {0}", "m.imported": "Imported {0} texts.", "m.nochanges": "Nothing to save.",
        "m.helptext": "Tags to keep: {BOX width height} box size, {NL} line break, {WAIT}{CLEAR} next page, "
                      "{VOICE n}, {COLOR n}, {PAUSE n}… game codes. Plain line breaks in the box are only for reading.\n\n"
                      "Filters: \"Only untranslated\" = same as the disc; \"Only changed\" = different from the disc.\n\n"
                      "Game file: \"Install in the game\" writes translation.tombipatch.json and translation.tpatch next to the executable. "
                      "On Android, import translation.tpatch from Options → Import translation.",
        "m.status": "{0} texts · {1} changed · language {2}",
        "s.title": "Title", "s.options": "Option", "s.optionsAfter": "Port extra", "s.messages": "Message",
        "s.locations": "Place", "s.listNames": "Name", "s.eventNames": "Event", "s.placeNames": "Place",
        "s.eventDescriptions": "Hint", "s.itemDescriptions": "Description", "s.pause": "Pause",
    },
}
i18n.add(_S)


def lname(letter):
    return LANG_NAMES["es" if i18n.current() == "es" else "en"].get(letter, letter)


def bundled(name):
    for d in (os.path.join(getattr(sys, "_MEIPASS", ""), "patches"), os.path.join(ROOT, "patches")):
        p = os.path.join(d, name)
        if os.path.isfile(p):
            return p
    return None


def cache_dir():
    d = os.path.join(os.path.dirname(i18n.settings_path()), "cache")
    os.makedirs(d, exist_ok=True)
    return d


def setting(key, default=None):
    return i18n.load_settings().get("tt_" + key, default)


def set_setting(key, value):
    i18n.save_settings(**{"tt_" + key: value})


# ---- look (the launcher's two palettes, tools/port_launcher.py THEMES)
THEMES = {
    "light": dict(bg="#f3f4f7", card="#ffffff", line="#d9dce3", text="#1b1d22", muted="#5d6470", accent="#2160c4",
                  input="#ffffff", ok="#1e7e3e", err="#b3261e", warn="#a15c00", changed="#fff4dc", errrow="#fde7e5",
                  tag="#1a5fb4"),
    "dark": dict(bg="#15171b", card="#22252b", line="#3b4049", text="#e6e8ec", muted="#9ca3ae", accent="#4a8cff",
                 input="#1a1c21", ok="#5fd38a", err="#ff8a80", warn="#f0b45a", changed="#3a3220", errrow="#4a2424",
                 tag="#7fb0ff"),
}


def theme_name():
    t = setting("theme")
    if t in THEMES:
        return t
    forced = os.environ.get("TOMBI_THEME", "").lower()
    if forced in THEMES:
        return forced
    try:
        return "dark" if QGuiApplication.styleHints().colorScheme() == Qt.ColorScheme.Dark else "light"
    except AttributeError:
        return "light"


def apply_theme(app, name):
    t = THEMES[name]
    p = QPalette()
    for role, key in ((QPalette.Window, "bg"), (QPalette.WindowText, "text"), (QPalette.Base, "input"),
                      (QPalette.AlternateBase, "card"), (QPalette.Text, "text"), (QPalette.Button, "card"),
                      (QPalette.ButtonText, "text"), (QPalette.Highlight, "accent"), (QPalette.ToolTipBase, "card"),
                      (QPalette.ToolTipText, "text"), (QPalette.PlaceholderText, "muted"), (QPalette.Link, "accent")):
        p.setColor(role, QColor(t[key]))
    p.setColor(QPalette.HighlightedText, QColor("#ffffff"))
    app.setPalette(p)
    app.setStyleSheet("QGroupBox { font-weight: bold; border: 1px solid %(line)s; border-radius: 6px; margin-top: 8px; padding-top: 6px; }"
                      "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; }"
                      "QPlainTextEdit, QTextBrowser, QListWidget, QTreeWidget, QTableView { border: 1px solid %(line)s; border-radius: 4px; }"
                      "QLabel#hint { color: %(muted)s; }" % t)


class TagHighlighter(QSyntaxHighlighter):
    def __init__(self, doc, color):
        super().__init__(doc)
        self.f = QTextCharFormat()
        self.f.setForeground(QColor(color))

    def highlightBlock(self, text):
        for m in re.finditer(r"\{[^}]*\}", text):
            self.setFormat(m.start(), m.end() - m.start(), self.f)


class Worker(QThread):
    done = Signal(object)
    failed = Signal(str)
    progress = Signal(int, int)

    def __init__(self, fn, parent=None):
        super().__init__(parent)
        self.fn = fn

    def run(self):
        try:
            self.done.emit(self.fn(lambda a, b, *_: self.progress.emit(a, b)))
        except Exception as e:      # noqa: BLE001
            self.failed.emit(str(e) if isinstance(e, (C.CoreError, W.WfmError, SystemExit)) else traceback.format_exc())


def friendly(level, msg):
    """The validator's message (tools/check_text_patch.py, the menu modules) in plain words of the interface language."""
    m = msg
    if level == "info" and msg == "memcard":
        return tr("k.memcard")
    if msg.startswith("console: "):
        return tr("k.console", msg[9:])
    if level == "info":
        return tr("k.borrow", msg.split(": ")[-1])
    if m == "BOX added/removed":
        return tr("k.box")
    r = re.search(r"'(.)' \(U\+[0-9A-Fa-f]+\)|'(.)' is not in the pause menu glyphs|character '(.)' (?:has no glyph|not in)", m)
    if r:
        return tr("k.glyph", next(g for g in r.groups() if g))
    r = re.match(r"preview: (.*?)(?: \((.*)\))?$", m)
    if r:
        kinds, detail = r.group(1).split(", "), r.group(2) or ""
        if i18n.current() == "es":
            detail = re.sub(r"page (\d+) line (\d+) is (\d+) px wide, the box holds (\d+) \((\d+) px out: ",
                            r"página \1, línea \2: mide \3 px y la caja admite \4 (sobran \5 px: ", detail)
        out = []
        for k in kinds:
            out.append(tr("k.wide", detail) if k == "wide" else tr("k.lines") if k == "lines" else tr("k.chars") if k == "chars" else k)
        return " ".join(out)
    r = re.match(r"control codes differ: (.*) vs (.*)", m)
    if r:
        return tr("k.codes", r.group(1), r.group(2))
    r = re.match(r"BOX taller than original \((.*)\)", m)
    if r:
        return tr("k.taller", r.group(1))
    r = re.match(r"BOX wider than allowed \((.*)\)", m)
    if r:
        return tr("k.wider", r.group(1))
    if m.startswith("title: "):
        return tr("k.title", m[7:])
    if re.search(r"bytes, (?:the pool holds|its room is|of room|the room of|the pool has)|does not fit|needs \d+ bytes", m):
        return tr("k.pool", m)
    return tr("k.raw", m)


class EntryModel(QAbstractTableModel):
    COLS = ("col.id", "col.orig", "col.imp", "col.cur", "col.state")

    def __init__(self, win):
        super().__init__()
        self.win = win
        self.rows = []

    def set_rows(self, rows):
        self.beginResetModel()
        self.rows = rows
        self.endResetModel()

    def rowCount(self, parent=QModelIndex()):
        return 0 if parent.isValid() else len(self.rows)

    def columnCount(self, parent=QModelIndex()):
        return len(self.COLS)

    def headerData(self, s, o, role=Qt.DisplayRole):
        if role == Qt.DisplayRole and o == Qt.Horizontal:
            return tr(self.COLS[s])
        return None

    def data(self, idx, role=Qt.DisplayRole):
        if not idx.isValid():
            return None
        e = self.rows[idx.row()]
        cat = self.win.cat
        c = idx.column()
        if role == Qt.DisplayRole:
            if c == 0:
                return self.win.entry_label(e)
            if e.kind == "jp":
                return C.plain(e.jp[0], 70) if c == 1 else tr("st.jp") if c == 4 else ""
            if c == 1:
                return C.plain(cat.original(e) or "", 70)
            if c == 2:
                t = cat.improved_text(e)
                return "" if t is None else C.plain(t, 70)
            if c == 3:
                return C.plain(cat.text(e), 70)
            if c == 4:
                return tr("st.err") if e.eid in self.win.errors else tr("st.changed") if cat.changed(e) else tr("st.same")
        if role == Qt.BackgroundRole:
            t = THEMES[self.win.theme]
            if e.eid in self.win.errors:
                return QColor(t["errrow"])
            if e.kind != "jp" and cat.changed(e):
                return QColor(t["changed"])
        if role == Qt.ToolTipRole and c in (1, 2, 3) and e.kind != "jp":
            return {1: cat.original(e), 2: cat.improved_text(e), 3: cat.text(e)}[c]
        return None

    def refresh_row(self, e):
        try:
            r = self.rows.index(e)
        except ValueError:
            return
        self.dataChanged.emit(self.index(r, 0), self.index(r, len(self.COLS) - 1))


class NewDialog(QDialog):
    def __init__(self, parent, langs, has_imp):
        super().__init__(parent)
        self.setWindowTitle(tr("m.newtitle"))
        f = QFormLayout(self)
        self.lang = QComboBox()
        for l in langs:
            self.lang.addItem(lname(l), l)
        if "S" in langs:
            self.lang.setCurrentIndex(langs.index("S"))
        f.addRow(tr("m.newlang"), self.lang)
        self.r_disc = QRadioButton(tr("m.fromdisc"))
        self.r_imp = QRadioButton(tr("m.fromimp"))
        self.r_imp.setChecked(has_imp)
        self.r_disc.setChecked(not has_imp)
        self.r_imp.setEnabled(has_imp)
        box = QVBoxLayout()
        box.addWidget(self.r_disc)
        box.addWidget(self.r_imp)
        f.addRow(tr("m.from"), box)
        self.lang.currentIndexChanged.connect(self._lang)
        self.name = QLineEdit(tr("m.defname"))
        self.author = QLineEdit(setting("author", ""))
        f.addRow(tr("m.name"), self.name)
        f.addRow(tr("m.author"), self.author)
        bb = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        bb.accepted.connect(self.accept)
        bb.rejected.connect(self.reject)
        f.addRow(bb)
        self.has_imp = has_imp
        self._lang()

    def _lang(self):
        ok = self.has_imp and self.lang.currentData() == "S"
        self.r_imp.setEnabled(ok)
        if not ok:
            self.r_disc.setChecked(True)


class MainWindow(QMainWindow):
    def __init__(self, disc=None, trans=None, state=None):
        super().__init__()
        self.cat = None
        self.src = None
        self.path = None            # the translation file
        self.dirty = False
        self.errors = {}
        self.cur = None
        self.jp = None
        self._loading = False
        self._workers = []
        self.theme = theme_name()
        self.improved = None
        p = bundled("es_mejorada.tombipatch.json")
        if p:
            try:
                self.improved = TP.load(p)
            except C.CoreError:
                self.improved = None
        self.setWindowTitle(tr("tt.title"))
        self.resize(1440, 900)
        ic = os.path.join(getattr(sys, "_MEIPASS", ""), "icons", "text.png")
        if not os.path.isfile(ic):
            ic = os.path.join(ROOT, "tools", "icons", "text.png")
        if os.path.isfile(ic):
            self.setWindowIcon(QIcon(ic))
        self._build()
        self.timer = QTimer(self)
        self.timer.setSingleShot(True)
        self.timer.timeout.connect(self.refresh_checks)
        self.ftimer = QTimer(self)
        self.ftimer.setSingleShot(True)
        self.ftimer.timeout.connect(self.fill_table)
        if state:
            self._restore(state)
        elif disc:
            self.open_disc(disc, trans)

    # ---------------------------------------------------------------- window
    def _build(self):
        tb = self.addToolBar("main")
        tb.setMovable(False)
        tb.setToolButtonStyle(Qt.ToolButtonTextOnly)
        self.acts = {}
        for key, fn, sc in (("tt.opendisc", self.ask_disc, None), ("tt.new", self.new_translation, QKeySequence.New),
                            ("tt.open", self.ask_open, QKeySequence.Open), ("tt.save", self.save, QKeySequence.Save),
                            ("tt.saveas", self.save_as, None), ("tt.checkall", self.check_all, None),
                            ("tt.install", self.install, None)):
            a = QAction(tr(key), self)
            a.triggered.connect(fn)
            if sc:
                a.setShortcut(sc)
            tb.addAction(a)
            self.acts[key] = a
        more = QToolButton()
        more.setText(tr("tt.more"))
        more.setPopupMode(QToolButton.InstantPopup)
        m = QMenu(more)
        for key, fn in (("tt.compile", self.compile_only), ("tt.iso", self.build_iso), ("tt.exporttxt", self.export_txt),
                        ("tt.importtxt", self.import_txt), ("tt.jpdisc", self.ask_jp), ("tt.help", self.help)):
            a = m.addAction(tr(key))
            a.triggered.connect(fn)
        m.addSeparator()
        self.dark = m.addAction(tr("tt.theme"))
        self.dark.setCheckable(True)
        self.dark.setChecked(self.theme == "dark")
        self.dark.toggled.connect(self.toggle_theme)
        more.setMenu(m)
        tb.addWidget(more)
        sp = QWidget()
        sp.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)
        tb.addWidget(sp)
        tb.addWidget(QLabel(tr("tt.uilang") + " "))
        self.uilang = QComboBox()
        for k, v in i18n.LANGS.items():
            if k in ("es", "en"):
                self.uilang.addItem(v, k)
        self.uilang.setCurrentIndex(max(0, self.uilang.findData(i18n.current())))
        self.uilang.currentIndexChanged.connect(self.change_ui_lang)
        tb.addWidget(self.uilang)

        self.stack = QStackedWidget()
        self.setCentralWidget(self.stack)
        wel = QTextBrowser()
        wel.setHtml(tr("m.welcome"))
        wel.setOpenExternalLinks(False)
        wrap = QWidget()
        wl = QVBoxLayout(wrap)
        wl.setContentsMargins(60, 40, 60, 40)
        wl.addWidget(wel)
        bl = QHBoxLayout()
        for key, fn in (("tt.opendisc", self.ask_disc), ("tt.open", self.ask_open)):
            b = QPushButton(tr(key))
            b.setMinimumHeight(40)
            b.clicked.connect(fn)
            bl.addWidget(b)
        wl.addLayout(bl)
        self.stack.addWidget(wrap)

        main = QWidget()
        ml = QVBoxLayout(main)
        ml.setContentsMargins(6, 6, 6, 6)
        bar = QHBoxLayout()
        bar.addWidget(QLabel(tr("tt.gamelang")))
        self.gamelang = QComboBox()
        self.gamelang.currentIndexChanged.connect(self.change_game_lang)
        bar.addWidget(self.gamelang)
        self.search = QLineEdit()
        self.search.setPlaceholderText(tr("tt.search"))
        self.search.setClearButtonEnabled(True)
        self.search.textChanged.connect(lambda _: self.ftimer.start(250))
        bar.addWidget(self.search, 1)
        bar.addWidget(QLabel(tr("tt.show")))
        self.filter = QComboBox()
        for k in ("f.all", "f.changed", "f.untrans", "f.diffimp", "f.errors"):
            self.filter.addItem(tr(k), k)
        self.filter.currentIndexChanged.connect(self.fill_table)
        bar.addWidget(self.filter)
        self.show_others = QCheckBox(tr("tt.others"))
        self.show_others.setChecked(bool(setting("others", True)))
        self.show_others.toggled.connect(self.toggle_others)
        bar.addWidget(self.show_others)
        self.show_jp = QCheckBox(tr("tt.japanese"))
        self.show_jp.toggled.connect(self.toggle_jp)
        bar.addWidget(self.show_jp)
        ml.addLayout(bar)

        split = QSplitter(Qt.Horizontal)
        self.tree = QTreeWidget()
        self.tree.setHeaderHidden(True)
        self.tree.itemSelectionChanged.connect(self.fill_table)
        split.addWidget(self.tree)
        right = QSplitter(Qt.Vertical)
        self.model = EntryModel(self)
        self.table = QTableView()
        self.table.setModel(self.model)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.setSelectionMode(QAbstractItemView.SingleSelection)
        self.table.verticalHeader().hide()
        self.table.verticalHeader().setDefaultSectionSize(22)
        h = self.table.horizontalHeader()
        h.setSectionResizeMode(QHeaderView.Interactive)
        h.setStretchLastSection(True)
        for c, w in enumerate((150, 330, 330, 330, 90)):
            self.table.setColumnWidth(c, w)
        self.table.selectionModel().currentRowChanged.connect(self.on_row)
        right.addWidget(self.table)

        det = QWidget()
        dl = QVBoxLayout(det)
        dl.setContentsMargins(0, 4, 0, 0)
        self.head = QLabel("")
        f = self.head.font()
        f.setBold(True)
        f.setPointSize(f.pointSize() + 1)
        self.head.setFont(f)
        self.head.setWordWrap(True)
        dl.addWidget(self.head)
        mono = QFont("Consolas" if os.name == "nt" else "DejaVu Sans Mono")
        mono.setStyleHint(QFont.Monospace)
        mono.setPointSize(11)
        # the three texts side by side: disc original | improved | yours
        row = QSplitter(Qt.Horizontal)
        self.g_orig = QGroupBox(tr("d.orig", ""))
        gl = QVBoxLayout(self.g_orig)
        self.e_orig = QPlainTextEdit()
        self.e_orig.setReadOnly(True)
        self.e_orig.setFont(mono)
        gl.addWidget(self.e_orig)
        b = QPushButton(tr("d.useorig"))
        b.clicked.connect(self.use_orig)
        self.b_useorig = b
        gl.addWidget(b, 0, Qt.AlignLeft)
        row.addWidget(self.g_orig)
        self.g_imp = QGroupBox(tr("d.imp"))
        gl = QVBoxLayout(self.g_imp)
        self.e_imp = QPlainTextEdit()
        self.e_imp.setReadOnly(True)
        self.e_imp.setFont(mono)
        gl.addWidget(self.e_imp)
        b = QPushButton(tr("d.useimp"))
        b.clicked.connect(self.use_imp)
        self.b_useimp = b
        gl.addWidget(b, 0, Qt.AlignLeft)
        row.addWidget(self.g_imp)
        self.g_cur = QGroupBox(tr("d.cur"))
        gl = QVBoxLayout(self.g_cur)
        self.e_cur = QPlainTextEdit()
        self.e_cur.setFont(mono)
        self.e_cur.textChanged.connect(self.on_edit)
        gl.addWidget(self.e_cur)
        bl = QHBoxLayout()
        b = QPushButton(tr("d.fit"))
        b.clicked.connect(self.fit_box)
        self.b_fit = b
        bl.addWidget(b)
        bl.addStretch(1)
        self.targets = QLabel("")
        self.targets.setObjectName("hint")
        bl.addWidget(self.targets)
        gl.addLayout(bl)
        row.addWidget(self.g_cur)
        self.hl = [TagHighlighter(w.document(), THEMES[self.theme]["tag"]) for w in (self.e_orig, self.e_imp, self.e_cur)]
        # below: check + preview | other languages and Japanese
        low = QSplitter(Qt.Horizontal)
        left = QSplitter(Qt.Vertical)
        g = QGroupBox(tr("d.checks"))
        gl = QVBoxLayout(g)
        self.checks = QListWidget()
        self.checks.setWordWrap(True)
        gl.addWidget(self.checks)
        left.addWidget(g)
        g = QGroupBox(tr("d.preview"))
        gl = QVBoxLayout(g)
        self.pv = QLabel()
        self.pv.setAlignment(Qt.AlignLeft | Qt.AlignTop)
        self.pv.setWordWrap(True)
        sa = QScrollArea()
        sa.setWidget(self.pv)
        sa.setWidgetResizable(True)
        gl.addWidget(sa)
        left.addWidget(g)
        left.setSizes([90, 170])
        low.addWidget(left)
        self.g_others = QGroupBox(tr("d.others"))
        gl = QVBoxLayout(self.g_others)
        self.others = QTextBrowser()
        gl.addWidget(self.others)
        low.addWidget(self.g_others)
        low.setSizes([560, 560])
        vs = QSplitter(Qt.Vertical)
        vs.addWidget(row)
        vs.addWidget(low)
        vs.setSizes([190, 280])
        dl.addWidget(vs, 1)
        right.addWidget(det)
        right.setSizes([300, 520])
        split.addWidget(right)
        split.setSizes([260, 1180])
        split.setStretchFactor(1, 1)
        ml.addWidget(split, 1)
        self.stack.addWidget(main)
        self.status = self.statusBar()
        self.g_others.setVisible(self.show_others.isChecked() or self.show_jp.isChecked())
        self.set_editing(False)

    def set_editing(self, on):
        self.e_cur.setReadOnly(not on)
        for b in (self.b_useorig, self.b_useimp, self.b_fit):
            b.setEnabled(on)

    def closeEvent(self, ev):
        if not self.confirm_discard():
            ev.ignore()
            return
        ev.accept()

    def confirm_discard(self):
        if self.cat and self.dirty:
            r = QMessageBox.question(self, tr("m.unsaved"), tr("m.unsavedq"),
                                     QMessageBox.Save | QMessageBox.Discard | QMessageBox.Cancel)
            if r == QMessageBox.Cancel:
                return False
            if r == QMessageBox.Save:
                return self.save()
        return True

    def run(self, label, fn, done, modal=True):
        """fn(progress) in a thread; done(result) back on the GUI thread; errors in a message box."""
        dlg = None
        if modal:
            dlg = QProgressDialog(label, None, 0, 0, self)
            dlg.setWindowModality(Qt.WindowModal)
            dlg.setMinimumDuration(300)
            dlg.setCancelButton(None)
            dlg.show()
        self.status.showMessage(label)
        w = Worker(fn, self)
        self._workers.append(w)

        def fin(res):
            if dlg:
                dlg.close()
            self._workers.remove(w)
            done(res)

        def fail(msg):
            if dlg:
                dlg.close()
            self._workers.remove(w)
            self.status.showMessage(tr("m.error"))
            QMessageBox.critical(self, tr("m.error"), msg)

        def prog(a, b):
            if dlg and b:
                dlg.setMaximum(b)
                dlg.setValue(a)
        w.done.connect(fin)
        w.failed.connect(fail)
        w.progress.connect(prog)
        w.start()
        return w

    # ---------------------------------------------------------------- disc / translation
    def ask_disc(self):
        p, _ = QFileDialog.getOpenFileName(self, tr("tt.opendisc"), setting("dir", ""),
                                           "Disc (*.bin *.cue *.iso *.img *.chd);;All (*)")
        if not p:
            d = None
            if not p and QMessageBox.question(self, tr("tt.opendisc"), "data/ ?") == QMessageBox.Yes:
                d = QFileDialog.getExistingDirectory(self, tr("tt.opendisc"), setting("dir", ""))
            if not d:
                return
            p = d
        self.open_disc(p)

    def open_disc(self, path, trans=None, lang=None):
        if not self.confirm_discard():
            return
        try:
            self.status.showMessage(tr("m.opening", path))
            QApplication.processEvents()
            p = path
            if os.path.isfile(p) and p.lower().endswith(".cue"):
                import extract_data as X
                p = X.bin_of_cue(p)
            self.src = C.Source(p)
        except Exception as e:      # noqa: BLE001
            QMessageBox.critical(self, tr("m.cantopen"), str(e))
            return
        set_setting("disc", os.path.abspath(path))
        set_setting("dir", os.path.dirname(os.path.abspath(path)))
        self.disc_path = os.path.abspath(path)
        langs = C.Project(self.src).languages()
        self.gamelang.blockSignals(True)
        self.gamelang.clear()
        for l in langs:
            self.gamelang.addItem(lname(l), l)
        self.gamelang.blockSignals(False)
        if trans:
            self.open_translation(trans)
        else:
            last = setting("file")
            if last and os.path.isfile(last):
                self.open_translation(last)
            else:
                self.start(lang or ("S" if "S" in langs else langs[0]), self.improved if (lang or "S") == "S" else None,
                           None, tr("m.defname"))
        self.status.showMessage(tr("m.opened", os.path.basename(path)), 5000)

    def start(self, lang, base, path, title=None, author=None):
        """A Catalog of lang with the patch base (None: a new empty one)."""
        if base is None:
            base = {"format": TP.FORMAT, "version": TP.VERSION, "title": title or "", "author": author or "", "edits": {}}
        else:
            base = json.loads(json.dumps(base))
            if title is not None and path is None:
                base["title"] = title
                base["author"] = author if author is not None else base.get("author", "")
                base.pop("notes", None)
        QApplication.setOverrideCursor(Qt.WaitCursor)
        try:
            self.cat = T.Catalog(self.src, lang, base, self.improved if lang == "S" else None)
        finally:
            QApplication.restoreOverrideCursor()
        self.path = path
        self.dirty = path is None and bool(base.get("edits") or base.get("menus"))
        self.errors = {}
        self.cur = None
        if self.jp:
            self.cat.add_japanese(self.jp)
        self.gamelang.blockSignals(True)
        self.gamelang.setCurrentIndex(max(0, self.gamelang.findData(lang)))
        self.gamelang.blockSignals(False)
        self.stack.setCurrentIndex(1)
        self.fill_tree()
        self.update_title()

    def change_game_lang(self, _):
        if not self.cat:
            return
        lang = self.gamelang.currentData()
        if lang == self.cat.lang:
            return
        if not self.confirm_discard():
            self.gamelang.blockSignals(True)
            self.gamelang.setCurrentIndex(self.gamelang.findData(self.cat.lang))
            self.gamelang.blockSignals(False)
            return
        self.dirty = False
        self.start(lang, None, None, tr("m.defname"))

    def new_translation(self):
        if not self.src:
            QMessageBox.information(self, tr("tt.new"), tr("m.nodisc"))
            return self.ask_disc()
        if not self.confirm_discard():
            return
        d = NewDialog(self, C.Project(self.src).languages(), self.improved is not None)
        if d.exec() != QDialog.Accepted:
            return
        lang = d.lang.currentData()
        set_setting("author", d.author.text())
        self.dirty = False
        self.start(lang, self.improved if d.r_imp.isChecked() and lang == "S" else None, None, d.name.text(), d.author.text())
        self.dirty = True
        self.update_title()

    def ask_open(self):
        if not self.src:
            QMessageBox.information(self, tr("tt.open"), tr("m.nodisc"))
            return self.ask_disc()
        p, _ = QFileDialog.getOpenFileName(self, tr("tt.open"), setting("dir", ""), "Tombi translation (*.tombipatch.json *.json);;All (*)")
        if p and self.confirm_discard():
            self.dirty = False
            self.open_translation(p)

    def open_translation(self, p):
        try:
            patch = TP.load(p)
        except C.CoreError as e:
            QMessageBox.critical(self, tr("m.cantopen"), str(e))
            return
        langs = sorted({C.lang_of(x) for x in patch.get("edits", {})} |
                       {T.LANG_ORDER[int(m.group(1))] for f in (patch.get("menus") or {})
                        for m in [re.search(r"(\d)", os.path.basename(f))] if m and int(m.group(1)) < 9})
        lang = patch.get("language") if patch.get("language") in T.LANG_ORDER else None
        if not lang:
            if len(langs) <= 1:
                lang = langs[0] if langs else "S"
            else:
                from PySide6.QtWidgets import QInputDialog
                names = [lname(l) for l in langs]
                s, ok = QInputDialog.getItem(self, tr("tt.open"), tr("m.which"), names, names.index(lname("S")) if "S" in langs else 0, False)
                if not ok:
                    return
                lang = langs[names.index(s)]
        set_setting("file", os.path.abspath(p))
        self.start(lang, patch, os.path.abspath(p))

    def to_patch(self):
        p = self.cat.to_patch()
        p["language"] = self.cat.lang
        if self.src.image and not p.get("disc"):
            try:
                p["disc"] = TP.disc_id(self.src.image)
            except OSError:
                pass
        return p

    def save(self):
        if not self.cat:
            return False
        if not self.path:
            return self.save_as()
        TP.save(self.to_patch(), self.path)
        self.dirty = False
        set_setting("file", self.path)
        self.update_title()
        self.status.showMessage(tr("m.saved", self.path, self.cat.n_changed()), 8000)
        return True

    def save_as(self):
        if not self.cat:
            return False
        name = re.sub(r"[^\w-]+", "_", (self.cat.extra.get("title") or "traduccion")).strip("_") or "traduccion"
        p, _ = QFileDialog.getSaveFileName(self, tr("tt.saveas"), os.path.join(setting("dir", ""), name + ".tombipatch.json"),
                                           "Tombi translation (*.tombipatch.json)")
        if not p:
            return False
        if not p.endswith(".json"):
            p += ".tombipatch.json"
        self.path = p
        return self.save()

    def update_title(self):
        n = os.path.basename(self.path) if self.path else (self.cat.extra.get("title") if self.cat else "") or tr("m.defname")
        self.setWindowTitle("%s%s – %s" % ("*" if self.dirty else "", n, tr("tt.title")))
        if self.cat:
            self.status.showMessage(tr("m.status", len(self.cat.entries), self.cat.n_changed(), lname(self.cat.lang)))

    # ---------------------------------------------------------------- lists
    def entry_label(self, e):
        if e.kind == "wfm":
            p, i = e.targets[0]
            if e.cat in ("dialogue", "sign"):
                return "%s #%d" % (os.path.basename(p)[:-5], i)
            return "#%d" % i
        if e.kind == "jp":
            return "%s #%d" % e.key
        return "%s %d" % (tr("s." + e.key[0]), e.key[1])

    def group_label(self, g):
        if re.fullmatch(r"AREA\d\d", g or ""):
            return tr("g.area", g[4:])
        k = "g." + str(g)
        return tr(k) if k in i18n.CAT["es"] else str(g)

    def fill_tree(self):
        self.tree.blockSignals(True)
        self.tree.clear()
        counts = {}
        for e in self.cat.entries:
            counts.setdefault(e.cat, {}).setdefault(e.group, 0)
            counts[e.cat][e.group] += 1
        top = QTreeWidgetItem([tr("c.all") + "  (%d)" % len(self.cat.entries)])
        top.setData(0, Qt.UserRole, ("*", None))
        self.tree.addTopLevelItem(top)
        for c in T.CATEGORIES:
            if c not in counts:
                continue
            it = QTreeWidgetItem([tr("c." + c) + "  (%d)" % sum(counts[c].values())])
            it.setData(0, Qt.UserRole, (c, None))
            self.tree.addTopLevelItem(it)
            if len(counts[c]) > 1:
                for g in sorted(counts[c], key=lambda g: (not str(g).startswith("AREA"), str(g))):
                    ch = QTreeWidgetItem([self.group_label(g) + "  (%d)" % counts[c][g]])
                    ch.setData(0, Qt.UserRole, (c, g))
                    it.addChild(ch)
        self.tree.blockSignals(False)
        self.tree.setCurrentItem(self.tree.topLevelItem(1) or top)
        self.fill_table()

    def fill_table(self):
        if not self.cat:
            return
        it = self.tree.currentItem()
        c, g = it.data(0, Qt.UserRole) if it else ("*", None)
        q = C.fold(self.search.text().strip())
        f = self.filter.currentData()
        cat = self.cat
        rows = []
        for e in cat.entries:
            if c != "*" and e.cat != c:
                continue
            if g is not None and e.group != g:
                continue
            if f == "f.changed" and not cat.changed(e):
                continue
            if f == "f.untrans" and (e.kind == "jp" or cat.changed(e)):
                continue
            if f == "f.diffimp":
                imp = cat.improved_text(e)
                if imp is None or C.norm(imp) == C.norm(cat.text(e)):
                    continue
            if f == "f.errors" and e.eid not in self.errors:
                continue
            if q:
                hay = " ".join(x for x in ((e.jp or [""])[0], cat.original(e) or "", cat.text(e), cat.improved_text(e) or "",
                                           self.entry_label(e)) if x)
                if q not in C.fold(hay):
                    continue
            rows.append(e)
        cur = self.cur
        self.model.set_rows(rows)
        if cur in rows:
            self.table.selectRow(rows.index(cur))
        elif rows:
            self.table.selectRow(0)
        else:
            self.show_entry(None)

    def on_row(self, idx, _prev=None):
        if not idx.isValid() or idx.row() >= len(self.model.rows):
            return
        self.show_entry(self.model.rows[idx.row()])

    def select(self, eid):
        """Shows entry eid (used by the tests and the screenshots)."""
        e = self.cat.by_id[eid]
        self.tree.setCurrentItem(self.tree.topLevelItem(0))
        self.search.setText("")
        self.filter.setCurrentIndex(0)
        self.fill_table()
        self.table.selectRow(self.model.rows.index(e))
        self.table.scrollTo(self.model.index(self.model.rows.index(e), 0))

    def show_entry(self, e):
        self.cur = e
        self._loading = True
        cat = self.cat
        if e is None:
            for w in (self.e_orig, self.e_imp, self.e_cur):
                w.setPlainText("")
            self.head.setText("")
            self.checks.clear()
            self.pv.clear()
            self.others.clear()
            self.set_editing(False)
            self._loading = False
            return
        where = ""
        if e.kind == "wfm":
            where = e.targets[0][0]
        elif e.kind == "menu":
            tg = cat._menu_targets(e)[1]
            where = ", ".join(sorted({os.path.basename(t[0]) for t in tg}))
        self.head.setText("%s · %s · %s" % (tr("c." + e.cat), self.group_label(e.group), self.entry_label(e)) +
                          ("   <%s>" % where if where else ""))
        self.g_orig.setTitle(lname("J") if e.kind == "jp" else tr("d.orig", lname(cat.lang)))
        if e.kind == "jp":
            self.e_orig.setPlainText(e.jp[0])
            self.e_imp.setPlainText("")
            self.e_cur.setPlainText("")
            self.set_editing(False)
            self.targets.setText(tr("d.readonly"))
        else:
            self.e_orig.setPlainText(cat.original(e) or "")
            imp = cat.improved_text(e)
            self.e_imp.setPlainText(imp if imp is not None else tr("d.noimp"))
            self.g_imp.setEnabled(imp is not None)
            self.e_cur.setPlainText(cat.text(e))
            self.set_editing(True)
            self.b_useimp.setEnabled(imp is not None)
            self.b_fit.setEnabled(e.kind == "wfm" and e.cat not in ("event_title",))
            n = len(e.targets) if e.kind == "wfm" else len(cat._menu_targets(e)[1])
            self.targets.setText(tr("d.targets", n) if n > 1 else "")
        self._loading = False
        self.refresh_checks()
        self.show_others_panel()

    def on_edit(self):
        if self._loading or not self.cur or self.cur.kind == "jp":
            return
        self.cat.set_text(self.cur, self.e_cur.toPlainText())
        if not self.dirty:
            self.dirty = True
            self.update_title()
        self.model.refresh_row(self.cur)
        self.timer.start(300)

    def use_orig(self):
        if self.cur:
            self.e_cur.setPlainText(self.cat.original(self.cur) or "")

    def use_imp(self):
        if self.cur and self.cat.improved_text(self.cur) is not None:
            self.e_cur.setPlainText(self.cat.improved_text(self.cur))

    def fit_box(self):
        e = self.cur
        if not e or e.kind != "wfm":
            return
        import text_preview
        text = self.e_cur.toPlainText()
        try:
            words, wc = self.cat.proj.words(e.targets[0][0], e.targets[0][1], text)
        except W.WfmError as ex:
            QMessageBox.warning(self, tr("m.error"), str(ex))
            return
        w, h = text_preview.fit_box(wc, words)
        tag = "{BOX %d %d}" % (w, h)
        text = re.sub(r"\{BOX [^}]*\}", tag, text, count=1) if re.search(r"\{BOX [^}]*\}", text) else tag + "\n" + text
        self.e_cur.setPlainText(text)

    def refresh_checks(self):
        e = self.cur
        self.checks.clear()
        self.pv.clear()
        if not e or e.kind == "jp":
            return
        text = self.e_cur.toPlainText()
        t = THEMES[self.theme]
        try:
            res = self.cat.check(e, text)
        except Exception as ex:      # noqa: BLE001
            res = [("err", str(ex))]
        errs = [r for r in res if r[0] == "err"]
        if errs:
            self.errors[e.eid] = errs
        else:
            self.errors.pop(e.eid, None)
        if not errs:
            it = QListWidgetItem("✔ " + tr("k.ok"))
            it.setForeground(QColor(t["ok"]))
            self.checks.addItem(it)
        for lvl, m in res:
            it = QListWidgetItem(("✖ " if lvl == "err" else "ℹ ") + friendly(lvl, m))
            it.setForeground(QColor(t["err"] if lvl == "err" else t["muted"]))
            it.setToolTip(m)
            self.checks.addItem(it)
        try:
            im, warns = self.cat.preview(e, text, 2)
        except W.WfmError:
            im = None
        except Exception:      # noqa: BLE001
            im = None
        if im is None:
            self.pv.setText(tr("d.nopreview"))
            self.pv.setWordWrap(True)
        else:
            buf = io.BytesIO()
            im.save(buf, "PNG")
            pm = QPixmap()
            pm.loadFromData(buf.getvalue(), "PNG")
            self.pv.setPixmap(pm)
        self.model.refresh_row(e)

    # ---------------------------------------------------------------- other languages / Japanese
    def toggle_others(self, on):
        set_setting("others", bool(on))
        self.g_others.setVisible(on or self.show_jp.isChecked())
        self.show_others_panel()

    def toggle_jp(self, on):
        self.g_others.setVisible(on or self.show_others.isChecked())
        if on and self.jp is None and self.src:
            self.load_jp()
        self.show_others_panel()

    def jp_disc(self):
        p = setting("jp_disc")
        if p and os.path.isfile(p):
            return p
        cands = [os.path.join(ROOT, "game_jp", "disc", "tomba_jp.bin")]
        if self.src and self.src.image:
            d = os.path.dirname(self.src.image)
            cands += [os.path.join(d, f) for f in sorted(os.listdir(d)) if re.search(r"(jp|japan|ore|slps)", f, re.I)
                      and f.lower().endswith((".bin", ".iso", ".img"))]
        return next((c for c in cands if os.path.isfile(c)), None)

    def ask_jp(self):
        p, _ = QFileDialog.getOpenFileName(self, tr("m.jpask"), setting("dir", ""), "Disc (*.bin *.iso *.img);;All (*)")
        if p:
            set_setting("jp_disc", p)
            self.jp = None
            if self.show_jp.isChecked():
                self.load_jp()
            else:
                self.show_jp.setChecked(True)

    def load_jp(self):
        jp = self.jp_disc()
        pal = self.src.image if self.src else None
        if not jp or not pal:
            QMessageBox.information(self, tr("tt.japanese"), tr("m.jpmissing"))
            self.show_jp.setChecked(False)
            return
        key = "jp_%d_%d.json" % (os.path.getsize(jp), os.path.getsize(pal))
        cache = os.path.join(cache_dir(), key)

        def job(progress):
            try:
                return T.JpIndex.load(cache)
            except (OSError, ValueError, KeyError):
                pass
            idx = T.JpIndex.build(jp, pal)
            try:
                idx.save(cache)
            except OSError:
                pass
            return idx

        def done(idx):
            self.jp = idx
            if self.cat:
                self.cat.add_japanese(idx)
                self.fill_tree()
            self.status.showMessage(tr("m.jpready", len(idx.lines), len(idx.solo)), 8000)
        self.others.setHtml("<i>%s</i>" % html.escape(tr("d.jpbuilding")))
        self.run(tr("d.jpbuilding"), job, done)

    def show_others_panel(self):
        e = self.cur
        if not e or not self.g_others.isVisible():
            return
        cat = self.cat
        t = THEMES[self.theme]
        esc = lambda s: html.escape(s or "").replace("\n", "<br>")
        tagc = lambda s: re.sub(r"(\{[^}]*\})", r'<span style="color:%s">\1</span>' % t["tag"], esc(s))
        rows = []
        if self.show_jp.isChecked():
            if self.jp is None:
                rows.append("<tr><td colspan=2><i>%s</i></td></tr>" % esc(tr("d.jpbuilding")))
            elif e.jp:
                rows.append("<tr><td valign=top style='padding:3px 8px 3px 0'><b>%s</b></td><td style='padding:3px'>%s</td></tr>" % (
                    esc(lname("J")), "<hr>".join(tagc(j) for j in e.jp)))
                if e.kind == "jp":
                    note = tr("d.jpnopal") if e.note == "nopal" else tr("d.jpnoblock")
                    rows.append("<tr><td></td><td style='color:%s'><i>%s</i></td></tr>" % (t["warn"], esc(note)))
                else:
                    en = cat.other(e, "B")
                    rows.append("<tr><td valign=top style='padding:3px 8px 3px 0'><b>%s</b></td><td style='padding:3px'>%s<br>"
                                "<span style='color:%s'><i>%s</i></span></td></tr>" % (
                                    esc("→ " + ("Traducción" if i18n.current() == "es" else "Translation")), tagc(en), t["muted"], esc(tr("d.jptrans"))))
            else:
                rows.append("<tr><td colspan=2 style='color:%s'><i>%s</i></td></tr>" % (t["muted"], esc(tr("d.jpnone"))))
        if self.show_others.isChecked() and e.kind != "jp":
            for l in cat.proj.languages():
                if l == cat.lang:
                    continue
                o = cat.other(e, l)
                if o is None:
                    continue
                rows.append("<tr><td valign=top style='padding:3px 8px 3px 0'><b>%s</b></td><td style='padding:3px'>%s</td></tr>" % (esc(lname(l)), tagc(o)))
        self.others.setHtml("<table width=100%%>%s</table>" % "".join(rows))

    # ---------------------------------------------------------------- whole-file actions
    def check_all(self):
        if not self.cat:
            return

        def done(res):
            self.errors = {k: v for k, v in res.items() if k != "*"}
            self.model.layoutChanged.emit()
            n = len(self.errors) + (1 if "*" in res else 0)
            if n:
                msg = tr("m.checkdone", n)
                if "*" in res:
                    msg += "\n\n" + tr("k.whole") + ":\n" + "\n".join(m for _, m in res["*"][:10])
                QMessageBox.warning(self, tr("tt.checkall"), msg)
                self.filter.setCurrentIndex(self.filter.findData("f.errors"))
            else:
                QMessageBox.information(self, tr("tt.checkall"), tr("m.checkok", self.cat.n_changed()))
            self.status.showMessage(tr("m.checkdone", n), 8000)
        self.run(tr("m.checking"), lambda prog: self.cat.check_all(prog), done)

    def _compile(self, out_tpatch, then):
        """Saves the translation and compiles it to out_tpatch (tools/tpatch_build.py) from the disc; then(json_path)."""
        if not self.save():
            return
        files, errors = self.cat.proj.encode_edited()
        if errors:
            QMessageBox.warning(self, tr("m.install"), tr("m.fixfirst", "\n".join(errors[:12])))
            return
        import tpatch_build as TB
        src = self.src
        jp = self.path
        if src.image:
            fn = lambda prog: TB.build_from_disc(src.image, jp, out_tpatch, log=lambda *a: None, progress=prog)
        elif src.kind == "folder" and os.path.isfile(os.path.join(src.root, "manifest.txt")) and \
                not os.path.isdir(os.path.join(src.root, ".orig")):
            fn = lambda prog: TB.build(src.root, jp, out_tpatch, 16384, log=lambda *a: None)
        else:
            QMessageBox.warning(self, tr("m.install"), tr("m.needimage"))
            return
        self.run(tr("m.compiling"), fn, lambda _: then(jp))

    def compile_only(self):
        if not self.cat:
            return
        base = (self.path or "translation.tombipatch.json").replace(".tombipatch.json", "").replace(".json", "")
        p, _ = QFileDialog.getSaveFileName(self, tr("tt.compile"), base + ".tpatch", "tpatch (*.tpatch)")
        if p:
            self._compile(p, lambda _: QMessageBox.information(self, tr("m.done"), tr("m.compiled", p)))

    def install(self, folder=None):
        if not self.cat:
            QMessageBox.information(self, tr("m.install"), tr("m.nodisc"))
            return
        d = folder or QFileDialog.getExistingDirectory(self, tr("m.installdir"), setting("game_dir", ""))
        if not d:
            return
        set_setting("game_dir", d)
        tmp = tempfile.mkdtemp(prefix="tombi-tt-")
        out = os.path.join(tmp, INSTALL_TPATCH)

        def then(jp):
            try:
                # the .tpatch records the sha1 of the JSON it was compiled from: copy that very file
                shutil.copyfile(jp, os.path.join(d, INSTALL_JSON))
                shutil.copyfile(out, os.path.join(d, INSTALL_TPATCH))
            finally:
                shutil.rmtree(tmp, ignore_errors=True)
            msg = tr("m.installed", INSTALL_JSON, INSTALL_TPATCH, d)
            self.status.showMessage(msg.split("\n")[0], 10000)
            if not folder:
                QMessageBox.information(self, tr("m.install"), msg)
            self.installed = d
        self._compile(out, then)

    def build_iso(self):
        if not self.cat or not self.src.image:
            QMessageBox.information(self, tr("tt.iso"), tr("m.needimage"))
            return
        files, errors = self.cat.proj.encode_edited()
        if errors:
            QMessageBox.warning(self, tr("tt.iso"), tr("m.fixfirst", "\n".join(errors[:12])))
            return
        out, _ = QFileDialog.getSaveFileName(self, tr("tt.iso"), "Tombi-traducido.bin", "Disc (*.bin)")
        if not out:
            return
        work = os.path.join(os.path.dirname(os.path.abspath(out)), "tombi-text-trabajo")
        proj = self.cat.proj

        def job(prog):
            if not C.have_mkpsxiso():
                d = os.path.splitext(out)[0] + "_archivos"
                C.write_patched(proj, d)
                return d
            return C.build_report(proj, out, work)["out"]
        self.run(tr("m.working"), job, lambda r: QMessageBox.information(self, tr("m.done"), tr("m.isodone", r)))

    def export_txt(self):
        if not self.cat:
            return
        p, _ = QFileDialog.getSaveFileName(self, tr("tt.exporttxt"), "tombi-textos.txt", "Text (*.txt)")
        if p:
            self.cat.proj.langs = {self.cat.lang}
            try:
                open(p, "w", encoding="utf-8").write(self.cat.proj.export_txt())
            finally:
                self.cat.proj.langs = set(self.cat.proj.languages())
            self.status.showMessage(tr("m.exported", p), 8000)

    def import_txt(self):
        if not self.cat:
            return
        p, _ = QFileDialog.getOpenFileName(self, tr("tt.importtxt"), "", "Text (*.txt)")
        if p:
            n, probs = self.cat.proj.import_txt(open(p, encoding="utf-8").read())
            self.dirty = True
            self.fill_table()
            self.update_title()
            QMessageBox.information(self, tr("tt.importtxt"), tr("m.imported", n) + ("\n\n" + "\n".join(probs[:20]) if probs else ""))

    def help(self):
        QMessageBox.information(self, tr("tt.help"), tr("m.helptext"))

    # ---------------------------------------------------------------- interface language / theme
    def _state(self):
        return {"src": self.src, "disc": getattr(self, "disc_path", None), "cat": self.cat, "path": self.path, "dirty": self.dirty,
                "jp": self.jp, "errors": self.errors, "cur": self.cur.eid if self.cur else None,
                "show_jp": self.show_jp.isChecked(), "geom": self.saveGeometry()}

    def _restore(self, s):
        self.src, self.cat, self.path, self.jp, self.errors = s["src"], s["cat"], s["path"], s["jp"], s["errors"]
        self.disc_path = s.get("disc")
        self.restoreGeometry(s["geom"])
        if self.cat:
            for l in C.Project(self.src).languages():
                self.gamelang.addItem(lname(l), l)
            self.gamelang.blockSignals(True)
            self.gamelang.setCurrentIndex(max(0, self.gamelang.findData(self.cat.lang)))
            self.gamelang.blockSignals(False)
            self.stack.setCurrentIndex(1)
            self.show_jp.blockSignals(True)
            self.show_jp.setChecked(s["show_jp"])
            self.show_jp.blockSignals(False)
            self.g_others.setVisible(self.show_others.isChecked() or self.show_jp.isChecked())
            self.fill_tree()
            if s["cur"] in self.cat.by_id:
                self.select(s["cur"])
        self.dirty = s["dirty"]
        self.update_title()

    def _rebuild(self):
        st = self._state()
        w = MainWindow(state=st)
        self.dirty = False
        w.show()
        app = QApplication.instance()
        app._win = w
        self.close()

    def change_ui_lang(self, _):
        i18n.set_language(self.uilang.currentData())
        self._rebuild()

    def toggle_theme(self, on):
        set_setting("theme", "dark" if on else "light")
        apply_theme(QApplication.instance(), "dark" if on else "light")
        self._rebuild()


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    i18n.init()
    if i18n.current() not in ("es", "en"):
        i18n.set_language("en", save=False)
    app = QApplication.instance() or QApplication(sys.argv[:1])
    app.setStyle("Fusion")
    apply_theme(app, theme_name())
    disc = argv[0] if argv else setting("disc")
    if disc and not os.path.exists(disc):
        disc = None
    w = MainWindow(disc, argv[1] if len(argv) > 1 else None)
    app._win = w
    w.show()
    return app.exec()


if __name__ == "__main__":
    sys.exit(main())
