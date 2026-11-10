#!/usr/bin/env python3
"""Launcher of the native-pointer port (tombi-port.exe, docs/PORT.md "M3 phase 3" / "Mods" / "Game library"): a window
with a library of the player's own disc images (.bin/.cue/.iso/.chd of Tombi! PAL SCES-01331 / SCES-01330 or Tomba!
USA SCUS-94236; tools/game_library.py identifies them and runs each with the port binary of its version), a save
folder, the window options and
the mods, and a Play button that runs tombi-pc.exe --window with those options.  Qt (PySide6) like the save editor
(tools/tombi_save_editor.py, opened from it with Edit save...), the tools/i18n.py catalogs of tombi-gfx / tombi-text.  No game data is bundled: the disc is checked (MAIN0.EXE extracted next to
tombi-port.json, the saved settings, and compared by sha1) before the first run, and the disc is then extracted
once into data/ next to the launcher (tools/extract_data.py): the game runs with --data and no longer needs the
image (the disc field is optional while data/ is valid for the same MAIN0.EXE).

  python3 tools/port_launcher.py             # from a checkout: uses build/port2-win/tombi2.exe (Windows) or build/port2/tombi2 (Linux)
  pyinstaller --onefile --windowed --name tombi-port ... tools/port_launcher.py   (.github/workflows/port2-exe.yml)
"""
import hashlib, re, json, os, shutil, struct, subprocess, sys, threading

try:
    from PySide6.QtCore import Qt, QTimer, QObject, Signal, QEvent, QSize, QRectF, QPointF
    from PySide6.QtGui import QIcon, QPixmap, QPainter, QColor, QPen, QFont, QFontMetricsF, QFontDatabase, QPainterPath, \
        QPalette, QGuiApplication, QImage, QLinearGradient
    from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QFrame, QLabel, QPushButton, QToolButton, QAbstractButton, \
        QHBoxLayout, QVBoxLayout, QGridLayout, QLineEdit, QComboBox, QCheckBox, QRadioButton, QButtonGroup, QSpinBox, \
        QAbstractSpinBox, QProgressBar, QPlainTextEdit, QScrollArea, QStackedWidget, QFileDialog, QMessageBox, \
        QInputDialog, QDialog, QColorDialog, QSizePolicy, QStyleFactory, QLayout
except ImportError as _e:       # running from source without Qt
    sys.exit("tombi-port launcher: PySide6 is needed (pip install PySide6-Essentials): %s" % _e)

import i18n
from i18n import tr
# the port launcher speaks English unless the player picked a language in it (i18n.selector saves the choice)
if i18n.load_settings().get("language") not in i18n.LANGS:
    i18n.set_language("en", save=False)
import port_mods as M
import prompt_custom as PC
import extract_data as X
import data_patch as DP
import game_library as GL
import disc_archive as DA

MAIN0_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc"     # EXE/MAIN0.EXE of SCES-01331 / SCES-01330 Rev 1 (August 1998; .github/workflows/report.yml)
REV0_SHA1 = "118459cef9b3747f57b13a498c475ffb8ab512ec"      # EXE/MAIN0.EXE of SCES-01331 / SCES-01330 Rev 0 (July 1998 build: other overlays, not supported)
# the languages of the disc's boot menu: EXE/MAINn.EXE (tombi2 --lang N, docs/PORT.md "Languages")
GAME_LANGS = [(0, "English"), (5, "Español"), (1, "Français"), (2, "Deutsch"), (3, "Italiano"), (4, "Nederlands"),
              (6, "Português"), (7, "Svenska"), (8, "Dansk")]   # the nine MAINn.EXE of the PAL disc (1..4 are not in its boot menu)


UI_TO_GAME_LANG = {"en": 0, "fr": 1, "de": 2, "it": 3, "nl": 4, "es": 5, "pt": 6, "sv": 7, "da": 8}
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

i18n.add({
    "es": {
        "l.lang": "Idioma del juego:",
        "title": "Tombi! - port nativo", "tab.play": "Jugar", "tab.mods": "Mods", "tab.controls": "Controles",
        "l.disc": "Imagen del disco (.bin / .cue / .iso / .chd / .zip / .7z; se extrae una vez a la carpeta data/):", "b.browse": "Elegir...", "l.savedir": "Carpeta de partidas guardadas:", "b.saveimp": "Importar partida…", "b.saveexp": "Exportar partida…", "s.saverun": "Cierra el juego antes de importar o exportar partidas.", "s.saveimpok": "Partida importada", "s.saveexpok": "Tarjeta exportada a {}", "s.saveerr": "No se pudo importar/exportar: {}", "s.saveask": "¿Importar este archivo en la tarjeta del port ({})?\nUna tarjeta completa sustituye a la actual (se guarda una copia en backup/).\nSi ya hay una partida con el mismo nombre, se pregunta si reemplazarla.", "s.saveexpq": "¿Exportar solo una partida como .mcs? (No = tarjeta completa .mcd)",
        "l.scale": "Tamaño de la ventana:", "l.fullscreen": "Pantalla completa (F11 en el juego)", "l.hz": "Velocidad:",
        "l.ires": "Resolución interna (solo OpenGL):", "r.ires1": "1x (original)",
        "l.renderer": "Dibujado:", "r.gl": "OpenGL (tarjeta gráfica)", "r.soft": "Software (CPU, más lento)", "l.aspect": "Imagen:", "a.tv": "4:3 (como en la tele)", "a.sq": "Píxeles cuadrados", "a.w16": "16:9 panorámica (experimental)", "a.w21": "21:9 (experimental)", "a.w32": "32:9 (experimental)", "a.auto": "Auto (llena la pantalla, experimental)",
        "ft.image": "Imágenes",
        "c.texpack": "Pack de texturas (HD) · experimental", "l.texpack": "Carpeta del pack (déjalo vacío para usar las texturas originales). Solo con OpenGL.",
        "b.dumpall": "Volcar todas las texturas", "b.openfolder": "Abrir carpeta",
        "l.texpack.d": "Pulsa «Volcar todas las texturas» para exportar las texturas del juego a la carpeta texture_dump. Edítalas o reescálalas (2x, 4x…) manteniendo el nombre del archivo y elige esa carpeta como pack.",
        "s.dumpall": "Volcando texturas… {}%", "s.dumpallok": "Listo: {} texturas en {}", "s.dumpallfail": "No se pudo volcar: {}",
        "l.pad": "Mando: cualquier mando compatible con SDL2 (Xbox, PS4/PS5, Switch Pro) se usa automáticamente; teclado: cursores, X = Cruz, Z = Cuadrado, S = Triángulo, D = Círculo, Q/W = L1/R1, A/E = L2/R2, Intro = Start, Retroceso = Select.",
        "b.play": "Jugar", "b.extracting": "Extrayendo... {0}%", "l.extracting": "Extrayendo los archivos del disco (solo la primera vez). Espera a que termine para jugar.", "b.stop": "Cerrar el juego", "b.copylog": "Copiar el registro", "s.copied": "Registro copiado al portapapeles ({0} líneas).", "l.autostart": "Empezar partida nueva automáticamente (pulsa Start en el título)",
        "s.nodisc": "Elige tu imagen del disco.", "s.checking": "Comprobando el disco...", "s.ok": "Disco compatible: {0}, MAIN0.EXE verificado (sha1).",
        "s.extracting": "Extrayendo el disco a {0}... {1}% ({2})", "s.dataok": "Datos del juego listos en {0} ({1} archivos): la imagen del disco ya no hace falta.",
        "s.extractfail": "No se pudo extraer el disco: {0}", "s.nodata": "Elige tu imagen del disco: se extrae una vez a la carpeta data/ y el juego ya no la necesita.",
        "s.badsha": "Disco NO compatible: {0} (sha1 de MAIN0.EXE {1}). El port solo funciona con Tombi! PAL Rev 1 (SCES-01331 o SCES-01330, agosto de 1998); puede fallar.",
        "s.rev0": "Disco Tombi! PAL Rev 0: {0} (build de julio de 1998). El port necesita la Rev 1 de agosto de 1998 (SCES-01331 o SCES-01330): la Rev 0 tiene otros overlays y no arranca.",
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
        "l.disc": "Disc image (.bin / .cue / .iso / .chd / .zip / .7z; extracted once into the data/ folder):", "b.browse": "Browse...", "l.savedir": "Save folder (memory cards):", "b.saveimp": "Import save…", "b.saveexp": "Export save…", "s.saverun": "Close the game before importing or exporting saves.", "s.saveimpok": "Save imported", "s.saveexpok": "Card exported to {}", "s.saveerr": "Import/export failed: {}", "s.saveask": "Import this file into the port's memory card ({})?\nA whole card replaces the current one (a copy is kept in backup/).\nIf a save with the same name is there, you are asked whether to replace it.", "s.saveexpq": "Export a single save as .mcs? (No = whole card .mcd)",
        "l.scale": "Window size:", "l.fullscreen": "Fullscreen (F11 in game)", "l.hz": "Speed:",
        "l.ires": "Internal resolution (OpenGL only):", "r.ires1": "1x (original)",
        "l.renderer": "Renderer:", "r.gl": "OpenGL (graphics card)", "r.soft": "Software (CPU, slower)", "l.aspect": "Picture:", "a.tv": "4:3 (as on a TV)", "a.sq": "Square pixels", "a.w16": "16:9 widescreen (experimental)", "a.w21": "21:9 (experimental)", "a.w32": "32:9 (experimental)", "a.auto": "Auto (fill the display, experimental)",
        "ft.image": "Images",
        "c.texpack": "Texture pack (HD) · experimental", "l.texpack": "Pack folder (leave empty for the original textures). OpenGL only.",
        "b.dumpall": "Dump all textures", "b.openfolder": "Open folder",
        "l.texpack.d": "Press “Dump all textures” to export the game's textures to the texture_dump folder. Edit or upscale them (2x, 4x…) keeping the file names, then choose that folder as the pack.",
        "s.dumpall": "Dumping textures… {}%", "s.dumpallok": "Done: {} textures in {}", "s.dumpallfail": "Could not dump: {}",
        "l.pad": "Controller: any SDL2-compatible pad (Xbox, PS4/PS5, Switch Pro) is used automatically; keyboard: arrows, X = Cross, Z = Square, S = Triangle, D = Circle, Q/W = L1/R1, A/E = L2/R2, Enter = Start, Backspace = Select.",
        "b.play": "Play", "b.extracting": "Extracting... {0}%", "l.extracting": "Extracting the disc files (first time only). Wait for it to finish to play.", "b.stop": "Close the game", "b.copylog": "Copy log", "s.copied": "Log copied to the clipboard ({0} lines).", "l.autostart": "Start a new game automatically (presses Start on the title)",
        "s.nodisc": "Pick your disc image.", "s.checking": "Checking the disc...", "s.ok": "Compatible disc: {0}, MAIN0.EXE verified (sha1).",
        "s.extracting": "Extracting the disc into {0}... {1}% ({2})", "s.dataok": "Game data ready in {0} ({1} files): the disc image is no longer needed.",
        "s.extractfail": "Could not extract the disc: {0}", "s.nodata": "Pick your disc image: it is extracted once into the data/ folder and the game no longer needs it.",
        "s.badsha": "NOT a compatible disc: {0} (MAIN0.EXE sha1 {1}). The port only runs Tombi! PAL Rev 1 (SCES-01331 or SCES-01330, August 1998); it may fail.",
        "s.rev0": "Tombi! PAL Rev 0 disc: {0} (July 1998 build). The port needs the August 1998 Rev 1 (SCES-01331 or SCES-01330): Rev 0 has other overlays and does not boot.",
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
})


# ---------------------------------------------------------------------------------------------- files
i18n.add({   # Edit save... (the save editor in a window of the launcher); the other languages: port_launcher_lang.py
    "es": {"b.saveedit": "Editar partida…", "s.editnocard": "Todavía no hay tarjeta de memoria del port:\n{}\nJuega y guarda una vez (o importa una partida) antes de editarla.",
           "s.editnoqt": "El editor de partidas necesita PySide6 (pip install PySide6-Essentials): {}", "s.editrun": "cierra el juego antes de guardar",
           "s.editopen": "Cierra el editor de partidas antes de jugar."},
    "en": {"b.saveedit": "Edit save…", "s.editnocard": "There is no port memory card yet:\n{}\nPlay and save once (or import a save) before editing it.",
           "s.editnoqt": "The save editor needs PySide6 (pip install PySide6-Essentials): {}", "s.editrun": "close the game before saving",
           "s.editopen": "Close the save editor before playing."},
})
i18n.add({   # Import save...: a save with the same name already on the card (save_import.Conflict)
    "es": {"t.savedup": "Ya existe la partida", "b.savereplace": "Reemplazar", "b.saveslot": "Guardar en otra ranura",
           "b.savecancel": "Cancelar"},
    "en": {"t.savedup": "Save already there", "b.savereplace": "Replace", "b.saveslot": "Save to another slot",
           "b.savecancel": "Cancel"},
})
i18n.add({   # a disc inside a .zip / .7z (tools/disc_archive.py), or a file that is no disc image
    "es": {"s.unpacking": "Descomprimiendo {0}… {1} %",
           "s.a.notimage": "«{0}» no es una imagen de disco de PlayStation. Elige la imagen de tu disco: .bin (con su .cue), .iso, .chd, o un .zip / .7z que la contenga.",
           "s.a.rar": "«{0}» es un archivo .rar, y el lanzador no puede abrirlo. Descomprímelo antes (o usa un .zip / .7z) y elige la imagen del disco.",
           "s.a.noimage": "«{0}» no contiene ninguna imagen de disco (.bin, .iso, .img o .chd).",
           "s.a.nested": "«{0}» contiene otro archivo comprimido, no una imagen de disco. Descomprímelo antes y elige la imagen.",
           "s.a.files": "«{0}» contiene los archivos sueltos del disco ({1}), no su imagen. El juego necesita la imagen completa: .bin con su .cue, .iso o .chd (también dentro de un .zip / .7z).",
           "s.a.cue": "«{0}» contiene el .cue, pero no la pista de datos que nombra ({1}).",
           "s.a.bad": "No se puede leer «{0}»: {1}. Descomprímelo y elige la imagen del disco.",
           "s.a.space": "No hay espacio libre suficiente para descomprimir «{0}»: hacen falta {1} MB, hay {2} MB."},
    "en": {"s.unpacking": "Unpacking {0}… {1} %",
           "s.a.notimage": "“{0}” is not a PlayStation disc image. Pick the image of your disc: .bin (with its .cue), .iso, .chd, or a .zip / .7z holding one.",
           "s.a.rar": "“{0}” is a .rar archive, which the launcher cannot open. Extract it first (or use a .zip / .7z) and pick the disc image.",
           "s.a.noimage": "“{0}” holds no disc image (.bin, .iso, .img or .chd).",
           "s.a.nested": "“{0}” holds another archive, not a disc image. Extract it first and pick the image.",
           "s.a.files": "“{0}” holds the disc’s loose files ({1}), not its image. The game needs the whole image: .bin with its .cue, .iso or .chd (inside a .zip / .7z too).",
           "s.a.cue": "“{0}” holds the .cue sheet but not the data track it names ({1}).",
           "s.a.bad": "“{0}” cannot be read: {1}. Extract it and pick the disc image.",
           "s.a.space": "Not enough free space to unpack “{0}”: {1} MB needed, {2} MB free."},
    "fr": {"s.unpacking": "Décompression de {0}… {1} %",
           "s.a.notimage": "« {0} » n'est pas une image de disque PlayStation. Choisissez l'image de votre disque : .bin (avec son .cue), .iso, .chd, ou un .zip / .7z qui la contient.",
           "s.a.rar": "« {0} » est une archive .rar, que le lanceur ne sait pas ouvrir. Décompressez-la d'abord (ou utilisez un .zip / .7z) et choisissez l'image du disque.",
           "s.a.noimage": "« {0} » ne contient aucune image de disque (.bin, .iso, .img ou .chd).",
           "s.a.nested": "« {0} » contient une autre archive, pas une image de disque. Décompressez-la d'abord et choisissez l'image.",
           "s.a.files": "« {0} » contient les fichiers du disque ({1}), pas son image. Le jeu a besoin de l'image complète : .bin avec son .cue, .iso ou .chd (aussi dans un .zip / .7z).",
           "s.a.cue": "« {0} » contient le .cue, mais pas la piste de données qu'il désigne ({1}).",
           "s.a.bad": "Impossible de lire « {0} » : {1}. Décompressez-le et choisissez l'image du disque.",
           "s.a.space": "Pas assez d'espace libre pour décompresser « {0} » : {1} Mo nécessaires, {2} Mo libres."},
    "de": {"s.unpacking": "{0} wird entpackt… {1} %",
           "s.a.notimage": "„{0}“ ist kein PlayStation-Disc-Image. Wähle das Image deiner Disc: .bin (mit ihrer .cue), .iso, .chd oder ein .zip / .7z, das es enthält.",
           "s.a.rar": "„{0}“ ist ein .rar-Archiv, das der Launcher nicht öffnen kann. Entpacke es zuerst (oder nimm ein .zip / .7z) und wähle das Disc-Image.",
           "s.a.noimage": "„{0}“ enthält kein Disc-Image (.bin, .iso, .img oder .chd).",
           "s.a.nested": "„{0}“ enthält ein weiteres Archiv, kein Disc-Image. Entpacke es zuerst und wähle das Image.",
           "s.a.files": "„{0}“ enthält die einzelnen Dateien der Disc ({1}), nicht ihr Image. Das Spiel braucht das ganze Image: .bin mit ihrer .cue, .iso oder .chd (auch in einem .zip / .7z).",
           "s.a.cue": "„{0}“ enthält die .cue, aber nicht den Datentrack, den sie nennt ({1}).",
           "s.a.bad": "„{0}“ kann nicht gelesen werden: {1}. Entpacke es und wähle das Disc-Image.",
           "s.a.space": "Nicht genug freier Speicher zum Entpacken von „{0}“: {1} MB nötig, {2} MB frei."},
    "it": {"s.unpacking": "Decompressione di {0}… {1} %",
           "s.a.notimage": "«{0}» non è un'immagine di disco PlayStation. Scegli l'immagine del tuo disco: .bin (con il suo .cue), .iso, .chd, oppure un .zip / .7z che la contiene.",
           "s.a.rar": "«{0}» è un archivio .rar, che il launcher non sa aprire. Estrailo prima (o usa un .zip / .7z) e scegli l'immagine del disco.",
           "s.a.noimage": "«{0}» non contiene nessuna immagine di disco (.bin, .iso, .img o .chd).",
           "s.a.nested": "«{0}» contiene un altro archivio, non un'immagine di disco. Estrailo prima e scegli l'immagine.",
           "s.a.files": "«{0}» contiene i file sciolti del disco ({1}), non la sua immagine. Il gioco ha bisogno dell'immagine completa: .bin con il suo .cue, .iso o .chd (anche dentro un .zip / .7z).",
           "s.a.cue": "«{0}» contiene il .cue, ma non la traccia dati che indica ({1}).",
           "s.a.bad": "Impossibile leggere «{0}»: {1}. Estrailo e scegli l'immagine del disco.",
           "s.a.space": "Spazio libero insufficiente per decomprimere «{0}»: servono {1} MB, liberi {2} MB."},
    "nl": {"s.unpacking": "{0} uitpakken… {1} %",
           "s.a.notimage": "‘{0}’ is geen PlayStation-disc-image. Kies de image van je disc: .bin (met de .cue), .iso, .chd, of een .zip / .7z waarin die zit.",
           "s.a.rar": "‘{0}’ is een .rar-archief, dat de launcher niet kan openen. Pak het eerst uit (of gebruik een .zip / .7z) en kies de disc-image.",
           "s.a.noimage": "‘{0}’ bevat geen disc-image (.bin, .iso, .img of .chd).",
           "s.a.nested": "‘{0}’ bevat nog een archief, geen disc-image. Pak het eerst uit en kies de image.",
           "s.a.files": "‘{0}’ bevat de losse bestanden van de disc ({1}), niet de image. De game heeft de hele image nodig: .bin met de .cue, .iso of .chd (ook in een .zip / .7z).",
           "s.a.cue": "‘{0}’ bevat de .cue, maar niet het datatrack dat die noemt ({1}).",
           "s.a.bad": "‘{0}’ kan niet worden gelezen: {1}. Pak het uit en kies de disc-image.",
           "s.a.space": "Niet genoeg vrije ruimte om ‘{0}’ uit te pakken: {1} MB nodig, {2} MB vrij."},
    "pt": {"s.unpacking": "A descomprimir {0}… {1} %",
           "s.a.notimage": "«{0}» não é uma imagem de disco de PlayStation. Escolhe a imagem do teu disco: .bin (com o seu .cue), .iso, .chd, ou um .zip / .7z que a contenha.",
           "s.a.rar": "«{0}» é um arquivo .rar, que o lançador não consegue abrir. Descomprime-o primeiro (ou usa um .zip / .7z) e escolhe a imagem do disco.",
           "s.a.noimage": "«{0}» não contém nenhuma imagem de disco (.bin, .iso, .img ou .chd).",
           "s.a.nested": "«{0}» contém outro arquivo comprimido, não uma imagem de disco. Descomprime-o primeiro e escolhe a imagem.",
           "s.a.files": "«{0}» contém os ficheiros soltos do disco ({1}), não a sua imagem. O jogo precisa da imagem completa: .bin com o seu .cue, .iso ou .chd (também dentro de um .zip / .7z).",
           "s.a.cue": "«{0}» contém o .cue, mas não a pista de dados que ele indica ({1}).",
           "s.a.bad": "Não é possível ler «{0}»: {1}. Descomprime-o e escolhe a imagem do disco.",
           "s.a.space": "Não há espaço livre suficiente para descomprimir «{0}»: são precisos {1} MB, há {2} MB."},
    "sv": {"s.unpacking": "Packar upp {0}… {1} %",
           "s.a.notimage": "”{0}” är ingen PlayStation-skivavbild. Välj avbilden av din skiva: .bin (med sin .cue), .iso, .chd eller en .zip / .7z som innehåller den.",
           "s.a.rar": "”{0}” är ett .rar-arkiv, som startprogrammet inte kan öppna. Packa upp det först (eller använd en .zip / .7z) och välj skivavbilden.",
           "s.a.noimage": "”{0}” innehåller ingen skivavbild (.bin, .iso, .img eller .chd).",
           "s.a.nested": "”{0}” innehåller ett annat arkiv, ingen skivavbild. Packa upp det först och välj avbilden.",
           "s.a.files": "”{0}” innehåller skivans lösa filer ({1}), inte dess avbild. Spelet behöver hela avbilden: .bin med sin .cue, .iso eller .chd (även i en .zip / .7z).",
           "s.a.cue": "”{0}” innehåller .cue-filen men inte dataspåret den anger ({1}).",
           "s.a.bad": "”{0}” kan inte läsas: {1}. Packa upp den och välj skivavbilden.",
           "s.a.space": "Inte tillräckligt med ledigt utrymme för att packa upp ”{0}”: {1} MB behövs, {2} MB ledigt."},
    "da": {"s.unpacking": "Pakker {0} ud… {1} %",
           "s.a.notimage": "“{0}” er ikke et PlayStation-diskaftryk. Vælg aftrykket af din disk: .bin (med dens .cue), .iso, .chd eller en .zip / .7z, der indeholder det.",
           "s.a.rar": "“{0}” er et .rar-arkiv, som starteren ikke kan åbne. Pak det ud først (eller brug en .zip / .7z) og vælg diskaftrykket.",
           "s.a.noimage": "“{0}” indeholder intet diskaftryk (.bin, .iso, .img eller .chd).",
           "s.a.nested": "“{0}” indeholder et andet arkiv, ikke et diskaftryk. Pak det ud først og vælg aftrykket.",
           "s.a.files": "“{0}” indeholder diskens løse filer ({1}), ikke dens aftryk. Spillet skal bruge hele aftrykket: .bin med dens .cue, .iso eller .chd (også i en .zip / .7z).",
           "s.a.cue": "“{0}” indeholder .cue-filen, men ikke det datanummer, den nævner ({1}).",
           "s.a.bad": "“{0}” kan ikke læses: {1}. Pak det ud og vælg diskaftrykket.",
           "s.a.space": "Ikke nok ledig plads til at pakke “{0}” ud: {1} MB kræves, {2} MB ledigt."},
})
i18n.add({   # the game library (tools/game_library.py); the other languages: port_launcher_lang.py
    "es": {"c.library": "Juegos", "b.addgame": "Añadir juego…",
           "l.library": "Añade la imagen de cada disco que tengas (.bin / .cue / .iso / .chd / .zip / .7z). Se reconoce por su ejecutable de "
                        "arranque y se extrae una vez a su propia carpeta; el logotipo sale de tu propio disco.",
           "l.addgame": "Tombi! PAL o Tomba! USA",
           "c.options": "Opciones del juego", "l.onlylang": "{0} (el único idioma de este disco)",
           "l.unsupported": "{0}: disco reconocido, pero el port todavía no lo ejecuta.",
           "l.unsupported2": "Puedes dejarlo en la biblioteca: se podrá jugar cuando el port lo admita. Las versiones que "
                             "funcionan ahora son Tombi! PAL (SCES-01331 / SCES-01330, revisión de agosto de 1998) y Tomba! USA (SCUS-94236).",
           "l.nopatches": "Todavía no hay traducciones ni parches de texto para la versión {0}.",
           "l.ntscopts": "Los idiomas, la traducción mejorada, los 50 Hz, el contenido cortado y los trucos son de la versión PAL.",
           "lib.unchecked": "Sin comprobar", "lib.unsupported": "Aún no compatible", "lib.ready": "Listo para jugar",
           "lib.toextract": "Falta extraerlo", "lib.noimage": "No se encuentra la imagen", "lib.unknown": "no es un disco de Tombi!",
           "rg.PAL": "PAL", "rg.USA": "USA", "rg.Japan": "Japón",
           "s.busyextract": "Espera a que termine la extracción del disco.",
           "s.notgame": "{0} ({1}, {2}): el port no lo reconoce. Añade un disco de Tombi! PAL o de Tomba! USA.",
           "q.removegame": "¿Quitar {0} de la biblioteca?", "b.removegame": "Quitar de la biblioteca…",
           "q.removedata": "¿Borrar también los archivos extraídos?\n{0}\n(Se vuelven a extraer si añades el disco otra vez.)",
           "s.ok2": "Disco compatible: {0}, {1} verificado (sha1).", "s.pickgame": "Elige un juego de la biblioteca.",
           "s.noimage": "No se encuentra la imagen del disco: {0}", "l.speedntsc": "60 Hz: la velocidad de este juego (NTSC).",
           "l.nomods": "La entrada DEBUG del título solo existe en la versión PAL por ahora."},
    "en": {"c.library": "Games", "b.addgame": "Add game…",
           "l.library": "Add the image of each disc you own (.bin / .cue / .iso / .chd / .zip / .7z). It is recognised by its boot "
                        "executable and extracted once into a folder of its own; the logo comes from your own disc.",
           "l.addgame": "Tombi! PAL or Tomba! USA",
           "c.options": "Game options", "l.onlylang": "{0} (the only language of this disc)",
           "l.unsupported": "{0}: disc recognised, but the port does not run it yet.",
           "l.unsupported2": "You can keep it in the library: it becomes playable once the port supports it. The versions that "
                             "run now are Tombi! PAL (SCES-01331 / SCES-01330, August 1998 revision) and Tomba! USA (SCUS-94236).",
           "l.nopatches": "No translations or text patches for the {0} version yet.",
           "l.ntscopts": "The languages, the improved translation, 50 Hz, the cut content and the cheats are for the PAL version.",
           "lib.unchecked": "Not checked yet", "lib.unsupported": "Not supported yet", "lib.ready": "Ready to play",
           "lib.toextract": "Needs extracting", "lib.noimage": "Disc image not found", "lib.unknown": "not a Tombi! disc",
           "rg.PAL": "PAL", "rg.USA": "USA", "rg.Japan": "Japan",
           "s.busyextract": "Wait for the disc extraction to finish.",
           "s.notgame": "{0} ({1}, {2}): the port does not know this disc. Add a Tombi! PAL or Tomba! USA disc.",
           "q.removegame": "Remove {0} from the library?", "b.removegame": "Remove from the library…",
           "q.removedata": "Delete the extracted files too?\n{0}\n(They are extracted again if you add the disc again.)",
           "s.ok2": "Compatible disc: {0}, {1} verified (sha1).", "s.pickgame": "Pick a game of the library.",
           "s.noimage": "Disc image not found: {0}", "l.speedntsc": "60 Hz: this game's own speed (NTSC).",
           "l.nomods": "The title DEBUG entry is PAL-only for now."},
})


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


def game_binary(sha1=None):
    """The port binary of the selected game's version (game_library.VERSIONS "binary": tombi-pc for PAL, tombi-pc-usa for
    USA; + .exe on Windows) in the bundle, else the checkout's CMake tree of that version (build/port2[-win] for PAL,
    build/port2-scus94236[-win] for USA: tombi2)."""
    v = GL.version_of(sha1 or current_sha1()) or GL.VERSIONS[GL.PAL_SHA1]
    pal = v is GL.VERSIONS[GL.PAL_SHA1]
    name = v["binary"] or "tombi-pc"
    if not pal and not getattr(sys, "_MEIPASS", None) \
            and not os.path.isfile(os.path.join(bin_dir(), name + (".exe" if os.name == "nt" else ""))):
        tree = os.path.join(ROOT, "build", v["build_dir"] + ("-win" if os.name == "nt" else ""))
        return os.path.join(tree, "tombi2.exe" if os.name == "nt" else "tombi2")
    if os.name == "nt":         # tombi-pc.exe: the game (the CMake target is still called tombi2); older bundles: tombi2.exe
        p = os.path.join(bin_dir(), name + ".exe")
        return p if os.path.isfile(p) or not pal else os.path.join(bin_dir(), "tombi2.exe")
    p = os.path.join(bin_dir(), name)     # the Linux bundle (release asset tombi-port); the build tree: tombi2
    if not os.path.isfile(p):
        return p if not pal else os.path.join(bin_dir(), "tombi2")
    if not os.access(p, os.X_OK):               # (PyInstaller extraction may drop the exec bit)
        try:
            os.chmod(p, 0o755)
        except OSError:
            pass
    return p


def settings_file():
    return os.path.join(app_dir(), "tombi-port.json")


# The selected game of the library (App.select_game): its data folder (next to the launcher) and the sha1 of its game
# executable (game_library.VERSIONS). Before any game is known: data/ and the PAL version (an older launcher's setup).
CURRENT = {"data": "data", "sha1": ""}


def current_sha1():
    return CURRENT.get("sha1") or GL.PAL_SHA1


def has(feature):
    """The selected game's version offers this launcher option (game_library FEATURES: lang, patch, hz, cut, ...)."""
    return GL.has(current_sha1(), feature)


def data_dir():
    """Where the selected game's disc is extracted once (tools/extract_data.py): data/ next to the launcher for the
    first PAL disc (as always), data-<serial>/ for the others; tombi2 reads it with --data."""
    return os.path.join(app_dir(), CURRENT.get("data") or "data")


def version_save_dir(savedir):
    """The folder of the selected game's memory card: the save folder as set (empty = the port's default, next to
    data/) for PAL; the version's own subfolder of it for another release (game_library VERSIONS "save_sub": USA
    saves_usa/, as files/saves_usa on Android), so each game keeps its own tombi_mc0.mcd.  "" = the port's default."""
    sub = (GL.version_of(current_sha1()) or {}).get("save_sub") or ""
    if not sub:
        return savedir
    return os.path.join(savedir or app_dir(), sub)


def art_cache(serial):
    """The card art rebuilt from the player's own disc (game_library.make_art): library/<serial>/ next to the launcher."""
    return os.path.join(app_dir(), "library", serial or "unknown")


def civ_dir():
    """Cache of the rebuilt cut towns (--civ-town / --civ-route): maps/ (tools/civ_rebuild.py) and wfm/
    (tools/civ_test_data.py), generated from data/ by the launcher; next to data/, never in the repo."""
    return os.path.join(app_dir(), "civ_cache")


CIV_SOURCES = ("civ_rebuild.py", "civ_rebuild_layout.json", "civ_test_data.py", "area000.py", "topdown.py")


def civ_stamp():
    """What the cache was built from: the generator sources (bundled layout + build id when frozen) and data/'s MAIN0."""
    h = hashlib.sha1(b"civ-cache 3")   # 2: one WFM bank per language (civ_MFNTnn0<letter>.WFM); 3: AREA16/17 scene lines
    here = getattr(sys, "_MEIPASS", None) or os.path.dirname(os.path.abspath(__file__))
    for n in CIV_SOURCES:
        p = os.path.join(here, n)
        if os.path.isfile(p):
            h.update(open(p, "rb").read())
    if getattr(sys, "frozen", False):
        h.update(BUILD_ID.encode())
    return "%s %s" % (h.hexdigest(), X.manifest_sha1(data_dir()) or "")


def civ_ready():
    d = civ_dir()
    try:
        ok = open(os.path.join(d, "stamp.txt"), encoding="utf-8").read().strip() == civ_stamp()
    except OSError:
        return False
    need = [os.path.join(d, "maps", "AREA%d" % a, f) for a in (11, 16, 17) for f in ("map8F.tim", "map8E.bin", "map8F_in.tim")]
    need += [os.path.join(d, "wfm", "civ_MFNT%d0%s.WFM" % (a, L)) for a in (11, 16, 17) for L in "BFGIHSPWD"]
    return ok and all(os.path.isfile(p) for p in need)


def civ_generate(log):
    """Builds civ_dir() from data/ in this process (the frozen launcher has no python to run the scripts with)."""
    import contextlib
    d, data = civ_dir(), data_dir()
    os.environ["TOMBI_FS"] = data              # tools/area000.py reads the extracted disc from here (no image needed:
    os.environ["TOMBI_IMG"] = ""               # the ISO layout comes from data/manifest.txt, the movie from data/MOVIE)
    import area000, topdown, civ_rebuild, civ_test_data
    area000.FS = topdown.FS = data
    area000.IMG = ""
    area000._REC = area000._LISTS = None

    class W:              # print() writes its pieces separately: one log line per newline
        buf = ""

        def write(self, t):
            self.buf += t
            *lines, self.buf = self.buf.split("\n")
            for line in lines:
                if line.strip():
                    log(line)

        def flush(self):
            pass
    with contextlib.redirect_stdout(W()):
        maps, wfm = os.path.join(d, "maps"), os.path.join(d, "wfm")
        os.makedirs(wfm, exist_ok=True)
        try:
            os.remove(os.path.join(d, "stamp.txt"))
        except OSError:
            pass
        civ_test_data.build_all_wfm(data, wfm)     # one bank per language letter (Spanish S, English elsewhere)
        layout = json.load(open(civ_rebuild.LAYOUT, encoding="utf-8"))
        for a in (17, 11, 16):
            civ_rebuild.build(a, layout, {}, maps)
    open(os.path.join(d, "stamp.txt"), "w", encoding="utf-8").write(civ_stamp() + "\n")


def data_ready():
    """sha1 of the game executable (MAIN0.EXE for PAL, the boot exe of the others) of a complete extraction in
    data_dir(), else None."""
    return GL.data_complete(data_dir())


def bundled_patch(name="es_mejorada.tombipatch.json"):
    """patches/<name> (es_mejorada.tombipatch.json by default): bundled in the exe (copied next to it, so the saved path stays
    valid) or in the checkout."""
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


# The player's own translation (tombi-text "Install in the game" / "Import translation..." below): fixed names next to the
# launcher, the folder tombi2 runs in (cwd); tombi2 loads it by itself and it wins over the bundled improved translation
# (docs/PORT.md "Your own translation file").  With data/ the launcher applies the JSON like any text patch.
TRANSLATION_JSON = "translation.tombipatch.json"
TRANSLATION_TPATCH = "translation.tpatch"


def translation_files(d=None):
    """(json, tpatch) of the installed translation in d (default app_dir()); None for a missing one."""
    d = d or app_dir()
    j, t = os.path.join(d, TRANSLATION_JSON), os.path.join(d, TRANSLATION_TPATCH)
    return (j if os.path.isfile(j) else None), (t if os.path.isfile(t) else None)


def translation_title(d=None):
    """The title of the installed translation (from the JSON, else the .tpatch header), or None."""
    j, t = translation_files(d)
    try:
        if j:
            return json.load(open(j, encoding="utf-8")).get("title") or TRANSLATION_JSON
        if t:
            b = open(t, "rb").read(4096)
            if b[:4] == b"TPCH":
                n = struct.unpack_from("<H", b, 56)[0]
                return b[58:58 + n].decode("utf-8", "replace") or TRANSLATION_TPATCH
    except (OSError, ValueError, struct.error):
        pass
    return None


def import_translation(src, d=None):
    """Copies a translation file (*.tombipatch.json, with its compiled .tpatch twin when it sits beside it, or a .tpatch)
    to the fixed names in d; returns the path the launcher selects. Raises ValueError for a file that is neither."""
    d = d or app_dir()
    j, t = os.path.join(d, TRANSLATION_JSON), os.path.join(d, TRANSLATION_TPATCH)
    if src.lower().endswith(".tpatch"):
        if open(src, "rb").read(4) != b"TPCH":
            raise ValueError("not a .tpatch file (tools/tpatch_build.py)")
        if os.path.abspath(src) != os.path.abspath(t):
            shutil.copyfile(src, t)
        if os.path.isfile(j):
            os.remove(j)            # an older JSON would win over the new .tpatch
        return t
    try:
        p = json.load(open(src, encoding="utf-8"))
    except (OSError, ValueError) as e:
        raise ValueError(str(e))
    if not isinstance(p, dict) or p.get("format") != "tombi-text-patch":
        raise ValueError("not a Tombi translation file (*.tombipatch.json)")
    if os.path.abspath(src) != os.path.abspath(j):
        shutil.copyfile(src, j)
    twin = src[:-len(".tombipatch.json")] + ".tpatch" if src.endswith(".tombipatch.json") else os.path.splitext(src)[0] + ".tpatch"
    if os.path.isfile(twin):
        if os.path.abspath(twin) != os.path.abspath(t):
            shutil.copyfile(twin, t)
    elif os.path.isfile(t):
        os.remove(t)                # compiled from another JSON: tombi2 would refuse it anyway
    return j


def remove_translation(d=None):
    for f in translation_files(d):
        if f:
            os.remove(f)


def translation_native(s):
    """True when tombi2 should load the installed translation by itself: chosen, and not applied to data/ by the launcher
    (no data/, or only the compiled .tpatch was imported)."""
    j, t = translation_files()
    if not (s.get("patch_on") and (s.get("patch") or "") in (j or "", t or "") and (j or t)):
        return False
    return bool(t) and not (j and data_ready())


def load_settings():
    s = M.default_settings()
    s.update({"disc": "", "savedir": "", "scale": 2, "fullscreen": False, "renderer": "gl", "patch_on": False, "patch": bundled_patch() or ""})
    try:
        s.update(json.load(open(settings_file(), encoding="utf-8")))
    except (OSError, ValueError):
        pass
    if not s.get("hz60_default"):     # 60 Hz became the default: settings saved before (always with hz60) move to it once
        s["hz60"], s["hz60_default"] = True, True
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


# (the releases the launcher knows, their names and options: tools/game_library.py VERSIONS / OTHER_DISCS)


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


def app_version():
    """Release number (VERSION at the repo root, bundled next to build_id.txt by port2-exe.yml)."""
    for d in (getattr(sys, "_MEIPASS", None), ROOT):
        if d and os.path.isfile(os.path.join(d, "VERSION")):
            v = open(os.path.join(d, "VERSION"), encoding="utf-8", errors="replace").read().strip()
            if v:
                return v
    return ""


APP_VERSION = app_version()
VERSION_TAG = ("v" + APP_VERSION + " · " if APP_VERSION else "") + "build " + BUILD_ID


def icon_file(name):
    """A file of tools/icons: bundled under icons/ by port2-exe.yml (--add-data ...;icons), else the checkout's."""
    for d in ([os.path.join(sys._MEIPASS, "icons")] if getattr(sys, "_MEIPASS", None) else []) + [os.path.join(ROOT, "tools", "icons")]:
        p = os.path.join(d, name)
        if os.path.isfile(p):
            return p
    return None


# ---------------------------------------------------------------------------------------------- window
# --cut-content (port/fixes.txt): text cut from the game, recovered from the Japanese release
i18n.add({
    "es": {"l.cutcontent": "Activar contenido recortado",
           "i.cutcontent": "Textos que se quitaron del juego y que se han encontrado en la versión japonesa (Ore! Tomba). "
                           "Se activan cuatro:\n\n"
                           "• El barquero de la Ciudad de todo principio (AREA00): en el estanque al que llegas sobre la tortuga, "
                           "tras su «¡Hasta luego... o hasta nunca!» dice tres frases más (una puerta cerrada con llave, la cerradura "
                           "de los hombres de hierro y su barca en la playa del Bosque de las setas).\n"
                           "• Una carta de Ciudad Baco en la Montaña del Fénix (AREA03): un segundo cartel a la izquierda del de guardar "
                           "partida, al principio de la zona; se lee igual que este (arriba y hablar).\n"
                           "• El tablón de anuncios de Ciudad Baco (AREA05): estaba fuera del alcance de Tombi; ahora se lee desde el suelo, "
                           "bajo el tablón (ratón desaparecido, luego «¡Enhorabuena!» y «¡Se necesita aventurero!»).\n"
                           "• Un punto de guardado «La roca de la sirena» en la playa de la meta de la carrera (AREA06), junto al tablón de récords.\n\n"
                           "Mencionan lugares y cosas que no existen en el juego final (el País de la Civilización, esa cerradura, "
                           "la barca).\n\n"
                           "Además, dos cosas que el juego tiene pero nunca usa:\n"
                           "• Bayas plateadas y doradas: la fruta que sueltan enemigos y cofres sale a veces plateada (1 de cada 8) "
                           "o dorada (1 de cada 32); dan 2 de vida en vez de 1 (y 100 AP, como la normal).\n"
                           "• El bocadillo grande «?»: aparece sobre Tombi cuando vuelves a hablar con alguien que no tiene nada "
                           "nuevo que decir (personajes que repiten su frase), en las escenas vistas desde arriba.\n"
                           "La frecuencia de las bayas y el momento del «?» los hemos elegido nosotros: el juego tiene el "
                           "código y los gráficos, pero no dice cuándo debían salir.\n\n"
                           "Los textos están en español con el parche «Traducción mejorada»; en los demás idiomas, o sin el parche, "
                           "esos huecos del disco solo contienen un relleno («!!!!!!!!!!», «..........???»). En inglés (sin otro parche de texto) se aplica solo el texto inglés incluido (en_cut_content).\n\n"
                           "Opcional, pantalla de título temprana: si indicas la carpeta con los ficheros TOMBA del disco «PrePre Vol. 8» "
                           "(PCPX-96084, una demo japonesa de junio de 1997; extraídos de tu propio disco con tools/prepre8.py extract), "
                           "el paisaje del título es el de esa versión (roca-torre y río) en vez del final. Eso es arte real del juego "
                           "de entonces; el logo, el menú y los textos siguen siendo los finales (el logo «オレっ！トンバ» de esa versión "
                           "no se usa: no sabemos cómo se montaba). No se incluye nada de ese disco.",
           "l.earlytitle": "Título temprano (opcional): carpeta TOMBA del disco PrePre Vol. 8 (tools/prepre8.py extract):"},
    "en": {"l.cutcontent": "Enable cut content",
           "i.cutcontent": "Text that was removed from the game and found in the Japanese release (Ore! Tomba). "
                           "Four pieces are enabled:\n\n"
                           "• The boatman in the Village of All Beginnings (AREA00): at the pond you reach on the turtle, after his "
                           "farewell he says three more lines (a locked door, a lock made by the Iron Men, his boat on the Mushroom "
                           "Forest beach).\n"
                           "• A letter from Baccus Village on Phoenix Mountain (AREA03): a second sign to the left of the save sign at "
                           "the start of the area, read the same way (Up, then talk).\n"
                           "• The Baccus Village notice board (AREA05): it was out of Tombi's reach; now it is read from the ground below it "
                           "(missing mouse, then 'Congratulations!' and 'Adventurer wanted!').\n"
                           "• A 'Mermaid's Singing Rock' save point on the race finish beach (AREA06), next to the record board.\n\n"
                           "They mention places and things that do not exist in the final game (the Land of Civilization, that lock, "
                           "the boat).\n\n"
                           "Also two things the game has but never uses:\n"
                           "• Silver and golden berries: fruit dropped by enemies and chests is sometimes silver (1 in 8) or golden "
                           "(1 in 32); they restore 2 vitality instead of 1 (and give 100 AP like the normal one).\n"
                           "• The big '?' bubble: it shows over Tombi when you talk again to someone who has nothing new to say "
                           "(characters that repeat their line), in the top-down scenes.\n"
                           "How often the berries show up and when the '?' appears are our choice: the game has the code and the "
                           "graphics but never says when they were meant to appear.\n\n"
                           "The texts are in Spanish with the 'Improved translation' patch; in the other languages, or without the "
                           "patch, those slots of the disc only hold filler ('!!!!!!!!!!', '..........???'). With the game in English (and no other text patch) the bundled English text (en_cut_content) is applied.\n\n"
                           "Optional early title screen: if you point to the folder with the TOMBA files of the 'PrePre Vol. 8' disc "
                           "(PCPX-96084, a Japanese demo from June 1997; extracted from your own disc with tools/prepre8.py extract), the "
                           "title landscape is that build's (rock spire and river) instead of the final one. That is real game art of the "
                           "time; the logo, menu and text stay the final ones (that build's 'オレっ！トンバ' logo is not used: how it was "
                           "laid out is unknown). Nothing from that disc is included.",
           "l.earlytitle": "Early title (optional): TOMBA folder of the PrePre Vol. 8 disc (tools/prepre8.py extract):"},
})

# the strings of the sidebar layout (the older keys above stay: status messages, options, mods)
i18n.add({
    "es": {"l.experimental": "Funciones experimentales", "l.fps60": "60 fps (experimental)", "nav.play": "Jugar", "nav.gfx": "Gráficos y rendimiento", "nav.controls": "Controles", "nav.mods": "Parches y mods", "nav.cut": "Contenido recortado", "nav.log": "Registro",
           "l.uilang": "Idioma del launcher", "l.starting": "Iniciando el juego...", "c.disc": "Disco", "c.saves": "Partidas", "c.game": "Idioma del juego",
           "c.patch": "Parche de texto", "c.speed": "Velocidad", "c.quick": "Opciones rápidas", "l.skipintro": "Saltar logos e intro",
           "l.autostart2": "Empezar partida automáticamente", "l.sublabels": "Rótulos del menú Select (juego en español)",
           "l.sublabelshint": "item=Objetos,event=Eventos,status=Estado,exit=Salir,map=Mapa,use=Usar|objeto,view=Ver,sort=Ordenar: las pestañas y los botones de hoja, con las letras del propio disco (| parte la línea del botón; acentos y ñ permitidos). Vacío: Objetos / Usar objeto con la traducción mejorada.", "p.none": "Ninguno (texto original)", "l.patchhint": "Recomendado con el juego en español: Traducción mejorada (incluida).", "p.bundled": "Traducción mejorada (incluida)",
           "p.custom": "Otro archivo...", "p.translation": "Tu traducción (translation.tombipatch.json)",
           "b.importtr": "Importar traducción…", "b.removetr": "Quitar traducción",
           "q.removetr": "¿Borrar la traducción importada (translation.tombipatch.json / translation.tpatch) de la carpeta del juego?",
           "l.tractive": "Traducción activa: «{0}» (en {1}). Tiene prioridad sobre la traducción mejorada incluida.",
           "l.trinstalled": "Hay una traducción importada («{0}»): elígela arriba como «Tu traducción» para usarla.",
           "l.trnone": "«Importar traducción…» copia un archivo de traducción (.tombipatch.json o .tpatch, hecho con tombi-text) junto al juego y lo activa.",
           "s.trimported": "Traducción importada: «{0}» (copiada en {1}).", "s.trbad": "No es un archivo de traducción válido: {0}",
           "sum.window": "{0}x ventana", "sum.full": "pantalla completa",
           "sum.ires": "{0}x interna", "c.renderer": "Dibujado", "c.window": "Ventana", "c.aspect": "Imagen", "c.ires": "Resolución interna",
           "l.iresoff": "Solo con OpenGL: el dibujado por software usa siempre la resolución original.",
           "c.keys": "Teclado", "c.pad": "Mando", "c.mods": "Mods", 
           "c.cmd": "Línea de comandos", "c.log": "Últimas líneas del registro", "c.status": "Estado"},
    "en": {"l.experimental": "Experimental features", "l.fps60": "60 fps (experimental)", "nav.play": "Play", "nav.gfx": "Graphics & performance", "nav.controls": "Controls", "nav.mods": "Patches and mods", "nav.cut": "Cut content", "nav.log": "Log",
           "l.uilang": "Launcher language", "l.starting": "Starting the game...", "c.disc": "Disc", "c.saves": "Saves", "c.game": "Game language",
           "c.patch": "Text patch", "c.speed": "Speed", "c.quick": "Quick options", "l.skipintro": "Skip logos and intro",
           "l.autostart2": "Start a new game automatically", "l.sublabels": "Select menu labels (game in Spanish)",
           "l.sublabelshint": "item=Objetos,event=Eventos,status=Estado,exit=Salir,map=Mapa,use=Usar|objeto,view=Ver,sort=Ordenar: the tabs and the leaf buttons, in the disc's own lettering (| breaks the button's line; accents and ñ allowed). Empty: Objetos / Usar objeto with the improved translation.", "p.none": "None (original text)", "l.patchhint": "Recommended with the game in Spanish: Improved translation (bundled).", "p.bundled": "Improved translation (bundled)",
           "p.custom": "Other file...", "p.translation": "Your translation (translation.tombipatch.json)",
           "b.importtr": "Import translation…", "b.removetr": "Remove translation",
           "q.removetr": "Delete the imported translation (translation.tombipatch.json / translation.tpatch) from the game folder?",
           "l.tractive": "Active translation: \"{0}\" (in {1}). It wins over the bundled improved translation.",
           "l.trinstalled": "An imported translation is there (\"{0}\"): pick \"Your translation\" above to use it.",
           "l.trnone": "\"Import translation…\" copies a translation file (.tombipatch.json or .tpatch, made with tombi-text) next to the game and selects it.",
           "s.trimported": "Translation imported: \"{0}\" (copied to {1}).", "s.trbad": "Not a valid translation file: {0}",
           "sum.window": "{0}x window", "sum.full": "fullscreen",
           "sum.ires": "{0}x internal", "c.renderer": "Renderer", "c.window": "Window", "c.aspect": "Picture", "c.ires": "Internal resolution",
           "l.iresoff": "OpenGL only: the software renderer always draws at the original resolution.",
           "c.keys": "Keyboard", "c.pad": "Controller", "c.mods": "Mods", 
           "c.cmd": "Command line", "c.log": "Last lines of the log", "c.status": "Status"},
})

# the Controls page: the drawn pad (App._pad_draw)
i18n.add({
    "es": {"c.controls": "Teclado y mando", "pad.hint": "Haz clic en un botón o en su tecla y pulsa la tecla nueva (Esc cancela). Cambiar una tecla activa las teclas personalizadas.",
           "pad.press": "Pulsa una tecla", "pad.nokey": "(ninguna)",
           "pad.note": "Mando: cualquier mando compatible con SDL2 (Xbox, PS4/PS5, Switch Pro) se usa automáticamente, con los botones en la misma posición del dibujo.",
           "btn.up": "Arriba", "btn.down": "Abajo", "btn.left": "Izquierda", "btn.right": "Derecha", "btn.cross": "Cruz", "btn.circle": "Círculo",
           "btn.square": "Cuadrado", "btn.triangle": "Triángulo", "btn.l1": "L1", "btn.r1": "R1", "btn.l2": "L2", "btn.r2": "R2",
           "btn.start": "Start", "btn.select": "Select"},
    "en": {"c.controls": "Keyboard and controller", "pad.hint": "Click a button or its key, then press the new key (Esc cancels). Changing a key turns custom keys on.",
           "pad.press": "Press a key", "pad.nokey": "(none)",
           "pad.note": "Controller: any SDL2-compatible pad (Xbox, PS4/PS5, Switch Pro) is used automatically, with the buttons where the drawing has them.",
           "btn.up": "Up", "btn.down": "Down", "btn.left": "Left", "btn.right": "Right", "btn.cross": "Cross", "btn.circle": "Circle",
           "btn.square": "Square", "btn.triangle": "Triangle", "btn.l1": "L1", "btn.r1": "R1", "btn.l2": "L2", "btn.r2": "R2",
           "btn.start": "Start", "btn.select": "Select"},
})

# the Controls page: the controller card (--dualshock, --prompts; docs/PORT.md "DualShock" / "Button prompts")
i18n.add({
    "es": {"l.rumble": "Intensidad de vibración:", "rumble.off": "Apagada",
           "c.pad": "Mando", "l.dualshock": "Mando DualShock (analógico): stick izquierdo con velocidad y vibración",
           "l.prompts": "Iconos de botones en los textos:", "p.auto": "Auto (según el mando)", "p.ps": "PlayStation",
           "p.xbox": "Xbox", "p.nin": "Nintendo", "pi.custom": "Personalizado", "b.custom": "Personalizar botones…",
           "pc.title": "Personalizar botones", "pc.preset": "Partir de:", "pc.sym": "Símbolos:", "pc.disc": "Disco", "pc.letter": "Letra",
           "pc.sym.xbox": "Letras Xbox", "pc.sym.nintendo": "Letras Nintendo", "pc.sym.ps": "Formas PlayStation",
           "pc.pos": "Abajo (A Xbox)|Derecha (B Xbox)|Izquierda (X Xbox)|Arriba (Y Xbox)", "pc.ok": "Usar", "pc.cancel": "Cancelar", "p.gen": "Genérico"},
    "en": {"l.rumble": "Vibration intensity:", "rumble.off": "Off",
           "c.pad": "Controller", "l.dualshock": "DualShock controller (analog): left stick with speed, and rumble",
           "l.prompts": "Button icons in the texts:", "p.auto": "Auto (from the controller)", "p.ps": "PlayStation",
           "p.xbox": "Xbox", "p.nin": "Nintendo", "pi.custom": "Custom", "b.custom": "Customise buttons…",
           "pc.title": "Customise buttons", "pc.preset": "Start from:", "pc.sym": "Symbols:", "pc.disc": "Disc", "pc.letter": "Letter",
           "pc.sym.xbox": "Xbox letters", "pc.sym.nintendo": "Nintendo letters", "pc.sym.ps": "PlayStation shapes",
           "pc.pos": "Bottom (Xbox A)|Right (Xbox B)|Left (Xbox X)|Top (Xbox Y)", "pc.ok": "Use", "pc.cancel": "Cancel", "p.gen": "Generic"},
})

# --fps (docs/PORT.md "High frame rates"): the Graphics page's frame-rate card
i18n.add({
    "es": {"c.perf": "Velocidad e imágenes por segundo", "c.fps": "Imágenes por segundo", "fps.off": "Igual que el juego", "fps.auto": "Auto (la frecuencia de la pantalla)", "fps.unl": "Sin límite (sin vsync)",
           "l.speedhint": "La velocidad del juego. 50 Hz es la original PAL; 60 Hz juega como las versiones americana y japonesa (la misma lógica, 60 veces por segundo).",
           "l.fpshint": "Por encima de la velocidad del juego, las imágenes intermedias se interpolan entre los pasos del juego, que mantiene su velocidad. Con el dibujado por software cada imagen intermedia cuesta lo que dibujar una."},
    "en": {"c.perf": "Speed and frame rate", "c.fps": "Frame rate", "fps.off": "Same as the game", "fps.auto": "Auto (the display's refresh rate)", "fps.unl": "Unlimited (no vsync)",
           "l.speedhint": "How fast the game runs. 50 Hz is the original PAL speed; 60 Hz plays like the US and Japanese versions (the same game logic, 60 times a second).",
           "l.fpshint": "Above the game's speed, the in-between frames are interpolated between the game's steps; the game keeps its speed. With the software renderer each in-between frame costs as much as drawing one."},
})

# the Controls page redesign: icons combobox, keyboard table, controller table (docs/PORT.md "Launcher")
i18n.add({
    "es": {"l.showfps": "Mostrar FPS", "c.icons": "Iconos de botones", "c.keyboard": "Teclado", "b.restore": "Restaurar predeterminados",
           "kb.hint": "Haz clic en la tecla de un botón y pulsa la tecla nueva (Esc cancela).",
           "kb.clash": "Aviso: la tecla {0} está asignada a varios botones ({1}).",
           "gp.hint": "Cualquier mando compatible con SDL2 (Xbox, PS4/PS5, Switch Pro). Haz clic en un botón de la foto o elige en la lista qué botón del mando hace de cada botón de PS1.",
           "gp.clash": "Aviso: {0} está asignado a varios botones ({1}).", "p.gen": "Genérico"},
    "en": {"l.showfps": "Show FPS", "c.icons": "Button icons", "c.keyboard": "Keyboard", "b.restore": "Restore defaults",
           "kb.hint": "Click a button's key, then press the new key (Esc cancels).",
           "kb.clash": "Warning: the key {0} is bound to several buttons ({1}).",
           "gp.hint": "Any SDL2-compatible controller (Xbox, PS4/PS5, Switch Pro). Click a button on the photo or pick in the list which controller button acts as each PS1 button.",
           "gp.clash": "Warning: {0} is bound to several buttons ({1}).", "p.gen": "Generic"},
})

# --prompts on PC (the AYN Thor styles are only offered by the Android launcher; a saved thor style reads as auto)
PROMPT_STYLES = [("auto", "p.auto"), ("playstation", "p.ps"), ("xbox", "p.xbox"), ("nintendo", "p.nin"), ("generic", "p.gen"),
                 ("custom", "pi.custom")]
# --pad-map: the defaults of runtime/sdl.c cbind_defaults(), and the SDL GameController sources offered
PAD_DEFAULTS = {"up": "dpup", "down": "dpdown", "left": "dpleft", "right": "dpright", "cross": "a", "circle": "b", "square": "x",
                "triangle": "y", "l1": "leftshoulder", "r1": "rightshoulder", "l2": "lefttrigger", "r2": "righttrigger",
                "start": "start", "select": "back"}
PAD_SOURCES = [   # SDL name, English label, Spanish label
    ("dpup", "D-pad up", "Cruceta arriba"), ("dpdown", "D-pad down", "Cruceta abajo"), ("dpleft", "D-pad left", "Cruceta izquierda"),
    ("dpright", "D-pad right", "Cruceta derecha"), ("a", "A (bottom)", "A (abajo)"), ("b", "B (right)", "B (derecha)"),
    ("x", "X (left)", "X (izquierda)"), ("y", "Y (top)", "Y (arriba)"), ("leftshoulder", "LB / L1", "LB / L1"),
    ("rightshoulder", "RB / R1", "RB / R1"), ("lefttrigger", "LT / L2", "LT / L2"), ("righttrigger", "RT / R2", "RT / R2"),
    ("start", "Start / Menu", "Start / Menú"), ("back", "Back / Select / View", "Back / Select / Vista"),
    ("guide", "Guide / Home / PS", "Guía / Home / PS"), ("leftstick", "L3 (left stick press)", "L3 (pulsar stick izq.)"),
    ("rightstick", "R3 (right stick press)", "R3 (pulsar stick der.)"), ("misc1", "Share / Capture", "Compartir / Captura"),
    ("touchpad", "Touchpad", "Panel táctil"), ("paddle1", "Paddle 1", "Paleta 1"), ("paddle2", "Paddle 2", "Paleta 2"),
    ("paddle3", "Paddle 3", "Paleta 3"), ("paddle4", "Paddle 4", "Paleta 4"),
    ("leftx-", "Left stick left", "Stick izq. izquierda"), ("leftx+", "Left stick right", "Stick izq. derecha"),
    ("lefty-", "Left stick up", "Stick izq. arriba"), ("lefty+", "Left stick down", "Stick izq. abajo"),
    ("rightx-", "Right stick left", "Stick der. izquierda"), ("rightx+", "Right stick right", "Stick der. derecha"),
    ("righty-", "Right stick up", "Stick der. arriba"), ("righty+", "Right stick down", "Stick der. abajo"),
    ("", "(none)", "(ninguno)")]
PAD_SRC_KEYS = [k for k, _, _ in PAD_SOURCES]
i18n.add({"en": {"src." + k: en for k, en, _es in PAD_SOURCES}, "es": {"src." + k: es for k, _en, es in PAD_SOURCES}})
# French, German, Italian, Dutch, Portuguese, Swedish and Danish: every key above (and port_mods.py's) in one file
import port_launcher_lang  # noqa: E402,F401
PAD_SHORT = {"dpup": "D-pad \u2191", "dpdown": "D-pad \u2193", "dpleft": "D-pad \u2190", "dpright": "D-pad \u2192", "a": "A", "b": "B",
             "x": "X", "y": "Y", "leftshoulder": "LB", "rightshoulder": "RB", "lefttrigger": "LT", "righttrigger": "RT",
             "start": "Start", "back": "Back", "guide": "Guide", "leftstick": "L3", "rightstick": "R3", "misc1": "Share",
             "touchpad": "Touchpad", "leftx-": "LS \u2190", "leftx+": "LS \u2192", "lefty-": "LS \u2191", "lefty+": "LS \u2193",
             "rightx-": "RS \u2190", "rightx+": "RS \u2192", "righty-": "RS \u2191", "righty+": "RS \u2193"}


def pad_map_arg(pm):
    """--pad-map value: only the PS1 buttons that differ from runtime/sdl.c's defaults ("" = unbound), or None."""
    pm = pm or {}
    diff = [(b, pm[b]) for b in M.BUTTONS if b in pm and pm[b] != PAD_DEFAULTS[b]]
    return ",".join("%s=%s" % d for d in diff) or None


RUMBLE_STEPS = [0, 25, 50, 75, 100, 125, 150, 200]   # --rumble choices, percent

# Tk keysym -> SDL key name, kept for reference (the Qt launcher maps key events with QT_TO_SDL below; what --keys takes, SDL_GetScancodeFromName); letters, digits and F1..F24 map to themselves
# (letters upper-cased).  "," is left out: it separates the --keys entries.
TK_TO_SDL = {"Return": "Return", "BackSpace": "Backspace", "space": "Space", "Tab": "Tab", "Up": "Up", "Down": "Down",
             "Left": "Left", "Right": "Right", "Shift_L": "Left Shift", "Shift_R": "Right Shift", "Control_L": "Left Ctrl",
             "Control_R": "Right Ctrl", "Alt_L": "Left Alt", "Alt_R": "Right Alt", "Super_L": "Left GUI", "Super_R": "Right GUI",
             "Prior": "PageUp", "Next": "PageDown", "Home": "Home", "End": "End", "Insert": "Insert", "Delete": "Delete",
             "Caps_Lock": "CapsLock", "minus": "-", "equal": "=", "bracketleft": "[", "bracketright": "]", "backslash": "\\",
             "semicolon": ";", "apostrophe": "'", "grave": "`", "period": ".", "slash": "/", "KP_Enter": "Keypad Enter",
             "KP_Add": "Keypad +", "KP_Subtract": "Keypad -", "KP_Multiply": "Keypad *", "KP_Divide": "Keypad /",
             "KP_Decimal": "Keypad .", "KP_Delete": "Keypad .", "KP_Insert": "Keypad 0", "KP_End": "Keypad 1", "KP_Down": "Keypad 2",
             "KP_Next": "Keypad 3", "KP_Left": "Keypad 4", "KP_Begin": "Keypad 5", "KP_Right": "Keypad 6", "KP_Home": "Keypad 7",
             "KP_Up": "Keypad 8", "KP_Prior": "Keypad 9"}


def sdl_key_name(keysym):
    if keysym in TK_TO_SDL:
        return TK_TO_SDL[keysym]
    if len(keysym) == 1 and keysym.isascii() and keysym.isalnum():
        return keysym.upper()
    if re.fullmatch(r"F([1-9]|1[0-9]|2[0-4])", keysym):
        return keysym
    if re.fullmatch(r"KP_[0-9]", keysym):
        return "Keypad " + keysym[3]
    return None

# Qt key -> SDL key name (what --keys takes); the same names as TK_TO_SDL above.  The keypad keys come with
# Qt.KeypadModifier; left / right modifiers are told apart by the native key code (X11 keysym, Windows scan code).
def _qk(name):
    return Qt.Key[name].value


QT_TO_SDL = {_qk(k): v for k, v in (
    ("Key_Return", "Return"), ("Key_Enter", "Keypad Enter"), ("Key_Backspace", "Backspace"), ("Key_Space", "Space"),
    ("Key_Tab", "Tab"), ("Key_Up", "Up"), ("Key_Down", "Down"), ("Key_Left", "Left"), ("Key_Right", "Right"),
    ("Key_PageUp", "PageUp"), ("Key_PageDown", "PageDown"), ("Key_Home", "Home"), ("Key_End", "End"),
    ("Key_Insert", "Insert"), ("Key_Delete", "Delete"), ("Key_CapsLock", "CapsLock"), ("Key_Minus", "-"),
    ("Key_Equal", "="), ("Key_BracketLeft", "["), ("Key_BracketRight", "]"), ("Key_Backslash", "\\"),
    ("Key_Semicolon", ";"), ("Key_Apostrophe", "'"), ("Key_QuoteLeft", "`"), ("Key_Period", "."), ("Key_Slash", "/"))}
QT_KEYPAD = {_qk(k): v for k, v in (
    ("Key_Enter", "Keypad Enter"), ("Key_Return", "Keypad Enter"), ("Key_Plus", "Keypad +"), ("Key_Minus", "Keypad -"),
    ("Key_Asterisk", "Keypad *"), ("Key_Slash", "Keypad /"), ("Key_Period", "Keypad ."), ("Key_Comma", "Keypad ."),
    ("Key_Delete", "Keypad ."), ("Key_Insert", "Keypad 0"), ("Key_End", "Keypad 1"), ("Key_Down", "Keypad 2"),
    ("Key_PageDown", "Keypad 3"), ("Key_Left", "Keypad 4"), ("Key_Clear", "Keypad 5"), ("Key_Right", "Keypad 6"),
    ("Key_Home", "Keypad 7"), ("Key_Up", "Keypad 8"), ("Key_PageUp", "Keypad 9"))}
QT_KEYPAD.update({_qk("Key_%d" % n): "Keypad %d" % n for n in range(10)})
QT_MODS = {_qk("Key_Shift"): "%s Shift", _qk("Key_Control"): "%s Ctrl", _qk("Key_Alt"): "%s Alt", _qk("Key_AltGr"): "%s Alt",
           _qk("Key_Meta"): "%s GUI", _qk("Key_Super_L"): "Left GUI", _qk("Key_Super_R"): "Right GUI"}
RIGHT_NATIVE = {0xffe2, 0xffe4, 0xffea, 0xffec, 0xffe8, 0xfe03,       # X11 keysyms Shift_R, Control_R, Alt_R, Super_R, Meta_R, AltGr
                0xA1, 0xA3, 0xA5, 0x5C}                                # Windows VK_RSHIFT, VK_RCONTROL, VK_RMENU, VK_RWIN
RIGHT_SCAN = {0x36, 0x11D, 0x138, 0x15C}                               # Windows scan codes (extended bit 0x100)


def sdl_key_from_qt(e):
    """The SDL name of a QKeyEvent's key, or None for one --keys cannot name ("," separates the --keys entries)."""
    k = e.key()
    if e.modifiers() & Qt.KeyboardModifier.KeypadModifier and k in QT_KEYPAD:
        return QT_KEYPAD[k]
    if k in QT_MODS:
        fmt = QT_MODS[k]
        if "%s" not in fmt:
            return fmt
        right = e.nativeVirtualKey() in RIGHT_NATIVE or (os.name == "nt" and e.nativeScanCode() in RIGHT_SCAN) \
            or k == _qk("Key_AltGr")
        return fmt % ("Right" if right else "Left")
    if k in QT_TO_SDL:
        return QT_TO_SDL[k]
    if _qk("Key_A") <= k <= _qk("Key_Z") or _qk("Key_0") <= k <= _qk("Key_9"):
        return chr(k)
    if _qk("Key_F1") <= k <= _qk("Key_F24"):
        return "F%d" % (k - _qk("Key_F1") + 1)
    return None


# ---------------------------------------------------------------------------------------------- look
# One light and one dark palette (the system's colour scheme picks; TOMBI_THEME=light|dark forces one), drawn with
# the Fusion style and a style sheet on the launcher window only (the save editor keeps its own).
THEMES = {
    "light": dict(bg="#f3f4f7", side="#e9ebf0", card="#ffffff", line="#e1e4ea", text="#1b1d22", muted="#5d6470",
                  faint="#a0a6b0", accent="#2160c4", accent_hi="#2b6fd6", accent_press="#1a4fa3",
                  accent_soft="#e7effb", accent_t="#ffffff", input="#ffffff", input_line="#cbd0d9", hover="#f1f3f6",
                  press="#e3e6ec", warn="#b3261e", code="#f6f7f9", nav_hover="#dee1e7", track="#e3e6ec",
                  pad_line="#6c717a", chip="#ffffff", chip_line="#c9ccd3"),
    "dark": dict(bg="#15171b", side="#1c1f24", card="#22252b", line="#30343c", text="#e6e8ec", muted="#9ca3ae",
                 faint="#646a74", accent="#4a8cff", accent_hi="#629bff", accent_press="#3a74d6", accent_soft="#223553",
                 accent_t="#ffffff", input="#1a1c21", input_line="#3b4049", hover="#2a2e35", press="#343841",
                 warn="#ff8a80", code="#1a1c21", nav_hover="#272a31", track="#343841", pad_line="#a3a9b3",
                 chip="#2a2e35", chip_line="#4a505a"),
}
DOT = {"ok": "#1e9e4a", "bad": "#d13438", "busy": "#e08a00", "info": "#8a8f98"}
PAGES = ["play", "gfx", "controls", "mods", "cut", "log"]
NAV_GLYPHS = {"play": "▶", "gfx": "▣", "controls": "✛", "mods": "✦", "cut": "✂", "log": "☰"}


def theme_name():
    forced = os.environ.get("TOMBI_THEME", "").lower()
    if forced in THEMES:
        return forced
    try:
        return "dark" if QGuiApplication.styleHints().colorScheme() == Qt.ColorScheme.Dark else "light"
    except AttributeError:          # Qt < 6.5
        return "light"


_ART_DIR = None


def art_dir(t, name):
    """Check marks, radio dots and the combobox arrow as PNGs (drawn 3x: sharp at any DPI) in a temporary folder
    for the style sheet; one set per theme."""
    global _ART_DIR
    if _ART_DIR is None:
        import tempfile, atexit, shutil
        _ART_DIR = tempfile.mkdtemp(prefix="tombi-launcher-")
        atexit.register(shutil.rmtree, _ART_DIR, True)
    d = os.path.join(_ART_DIR, name)
    if os.path.isdir(d):
        return d.replace("\\", "/")
    os.makedirs(d)
    S = 54

    def img(fn, draw):
        im = QImage(S, S, QImage.Format_ARGB32_Premultiplied)
        im.fill(Qt.transparent)
        p = QPainter(im)
        p.setRenderHint(QPainter.Antialiasing)
        draw(p)
        p.end()
        im.save(os.path.join(d, fn))

    def box(fill, edge, tick):
        def draw(p):
            p.setPen(QPen(QColor(edge), 3.2))
            p.setBrush(QColor(fill))
            p.drawRoundedRect(QRectF(2.5, 2.5, S - 5, S - 5), 10, 10)
            if tick:
                pen = QPen(QColor(tick), 6.5)
                pen.setCapStyle(Qt.RoundCap)
                pen.setJoinStyle(Qt.RoundJoin)
                p.setPen(pen)
                path = QPainterPath(QPointF(14, 28))
                path.lineTo(23, 37)
                path.lineTo(40, 18)
                p.setBrush(Qt.NoBrush)
                p.drawPath(path)
        return draw

    def radio(fill, edge, dot):
        def draw(p):
            p.setPen(QPen(QColor(edge), 3.2))
            p.setBrush(QColor(fill))
            p.drawEllipse(QRectF(2.5, 2.5, S - 5, S - 5))
            if dot:
                p.setPen(Qt.NoPen)
                p.setBrush(QColor(dot))
                p.drawEllipse(QRectF(16, 16, S - 32, S - 32))
        return draw

    def arrow(col):
        def draw(p):
            pen = QPen(QColor(col), 5)
            pen.setCapStyle(Qt.RoundCap)
            pen.setJoinStyle(Qt.RoundJoin)
            p.setPen(pen)
            path = QPainterPath(QPointF(15, 21))
            path.lineTo(27, 33)
            path.lineTo(39, 21)
            p.drawPath(path)
        return draw
    img("check_off.png", box(t["input"], t["input_line"], None))
    img("check_off_hover.png", box(t["input"], t["accent"], None))
    img("check_on.png", box(t["accent"], t["accent"], "#ffffff"))
    img("check_off_dis.png", box(t["side"], t["line"], None))
    img("check_on_dis.png", box(t["track"], t["track"], t["faint"]))
    img("radio_off.png", radio(t["input"], t["input_line"], None))
    img("radio_off_hover.png", radio(t["input"], t["accent"], None))
    img("radio_on.png", radio(t["input"], t["accent"], t["accent"]))
    img("radio_off_dis.png", radio(t["side"], t["line"], None))
    img("radio_on_dis.png", radio(t["side"], t["line"], t["faint"]))
    img("arrow.png", arrow(t["muted"]))
    img("arrow_up.png", lambda p: (p.translate(0, S), p.scale(1, -1), arrow(t["muted"])(p)))
    img("arrow_dis.png", arrow(t["faint"]))
    return d.replace("\\", "/")


def launcher_qss(t, name):
    a = art_dir(t, name)
    v = dict(t, art=a, dot_busy=DOT["busy"])
    return """
#root, #page, QScrollArea, #scrollBody { background: %(bg)s; }
QWidget { color: %(text)s; }
QToolTip { background: %(card)s; color: %(text)s; border: 1px solid %(line)s; padding: 4px 8px; }
#sidebar { background: %(side)s; border-right: 1px solid %(line)s; }
#appName { font-size: 16pt; font-weight: 700; color: %(text)s; }
#appSub { color: %(muted)s; font-size: 9pt; }
#sideCaption { color: %(muted)s; font-size: 9pt; }
#build { color: %(faint)s; font-size: 8.5pt; }
QPushButton#nav { text-align: left; padding: 8px 12px; border: 1px solid transparent; border-radius: 8px;
    background: transparent; font-size: 10.5pt; }
QPushButton#nav:hover { background: %(nav_hover)s; }
QPushButton#nav:checked { background: %(card)s; border-color: %(line)s; color: %(accent)s; font-weight: 600; }
#pageTitle { font-size: 19pt; font-weight: 700; }
QScrollArea { border: none; }
QFrame#card { background: %(card)s; border: 1px solid %(line)s; border-radius: 12px; }
QFrame#card QLabel, QFrame#card QCheckBox, QFrame#card QRadioButton { background: transparent; }
#cardTitle { font-size: 11pt; font-weight: 600; }
#cellTitle { font-weight: 600; }
#muted { color: %(muted)s; }
#warn { color: %(warn)s; }
#busyText { color: %(dot_busy)s; }
#accentText { color: %(accent)s; }
#nameSel { color: %(accent)s; font-weight: 600; }
#nameWarn { color: %(warn)s; }
QFrame#sep { background: %(line)s; border: none; min-height: 1px; max-height: 1px; }
#bar { background: %(bg)s; border-top: 1px solid %(line)s; }
#summary { color: %(muted)s; }
QPushButton { background: %(input)s; border: 1px solid %(input_line)s; border-radius: 7px; padding: 6px 14px; min-height: 18px; }
QPushButton:hover { background: %(hover)s; }
QPushButton:pressed { background: %(press)s; }
QPushButton:disabled { color: %(faint)s; background: %(side)s; border-color: %(line)s; }
QPushButton:focus { border-color: %(accent)s; }
QPushButton#primary { background: %(accent)s; color: %(accent_t)s; border: 1px solid %(accent)s; font-size: 11pt;
    font-weight: 600; padding: 9px 34px; border-radius: 8px; }
QPushButton#primary:hover { background: %(accent_hi)s; border-color: %(accent_hi)s; }
QPushButton#primary:pressed { background: %(accent_press)s; }
QPushButton#primary:disabled { background: %(accent_soft)s; border-color: %(accent_soft)s; color: %(faint)s; }
QPushButton#seg { border-radius: 0px; margin-left: -1px; padding: 6px 14px; }
QPushButton#segL { border-top-right-radius: 0px; border-bottom-right-radius: 0px; padding: 6px 14px; }
QPushButton#segR { border-top-left-radius: 0px; border-bottom-left-radius: 0px; margin-left: -1px; padding: 6px 14px; }
QPushButton#seg:checked, QPushButton#segL:checked, QPushButton#segR:checked {
    background: %(accent)s; border-color: %(accent)s; color: %(accent_t)s; font-weight: 600; }
QPushButton#keycap { font-weight: 600; text-align: center; }
QPushButton#keycap:checked { background: %(accent)s; border-color: %(accent)s; color: %(accent_t)s; }
QPushButton#swatch { min-width: 64px; }
QToolButton#info { border: none; background: transparent; color: %(accent)s; font-size: 13pt; font-weight: 700; padding: 0px 2px; }
QToolButton#info:hover { color: %(accent_hi)s; }
QLineEdit, QComboBox, QSpinBox { background: %(input)s; border: 1px solid %(input_line)s; border-radius: 7px;
    padding: 5px 8px; min-height: 18px; selection-background-color: %(accent)s; selection-color: #ffffff; }
QLineEdit:focus, QComboBox:focus, QSpinBox:focus { border-color: %(accent)s; }
QComboBox:hover, QLineEdit:hover, QSpinBox:hover { border-color: %(faint)s; }
QComboBox:disabled, QLineEdit:disabled { color: %(faint)s; background: %(side)s; }
QComboBox::drop-down { border: none; width: 26px; subcontrol-origin: padding; subcontrol-position: center right; }
QComboBox::down-arrow { image: url(%(art)s/arrow.png); width: 14px; height: 14px; }
QComboBox::down-arrow:disabled { image: url(%(art)s/arrow_dis.png); }
QComboBox QAbstractItemView { background: %(card)s; border: 1px solid %(line)s; border-radius: 6px; padding: 4px;
    outline: none; selection-background-color: %(accent_soft)s; selection-color: %(text)s; }
QSpinBox::up-button, QSpinBox::down-button { width: 18px; border: none; background: transparent; }
QSpinBox::up-arrow { image: url(%(art)s/arrow_up.png); width: 10px; height: 10px; }
QSpinBox::down-arrow { image: url(%(art)s/arrow.png); width: 10px; height: 10px; }
QCheckBox, QRadioButton { spacing: 9px; padding: 2px 0px; }
QCheckBox::indicator, QRadioButton::indicator { width: 18px; height: 18px; }
QCheckBox::indicator:unchecked { image: url(%(art)s/check_off.png); }
QCheckBox::indicator:unchecked:hover { image: url(%(art)s/check_off_hover.png); }
QCheckBox::indicator:checked { image: url(%(art)s/check_on.png); }
QCheckBox::indicator:unchecked:disabled { image: url(%(art)s/check_off_dis.png); }
QCheckBox::indicator:checked:disabled { image: url(%(art)s/check_on_dis.png); }
QRadioButton::indicator:unchecked { image: url(%(art)s/radio_off.png); }
QRadioButton::indicator:unchecked:hover { image: url(%(art)s/radio_off_hover.png); }
QRadioButton::indicator:checked { image: url(%(art)s/radio_on.png); }
QRadioButton::indicator:unchecked:disabled { image: url(%(art)s/radio_off_dis.png); }
QRadioButton::indicator:checked:disabled { image: url(%(art)s/radio_on_dis.png); }
QCheckBox:disabled, QRadioButton:disabled { color: %(faint)s; }
QProgressBar { background: %(track)s; border: none; border-radius: 3px; min-height: 6px; max-height: 6px; }
QProgressBar::chunk { background: %(accent)s; border-radius: 3px; }
QPlainTextEdit#code { background: %(code)s; border: 1px solid %(line)s; border-radius: 8px; padding: 4px;
    selection-background-color: %(accent)s; selection-color: #ffffff; }
QScrollBar:vertical { background: transparent; width: 12px; margin: 2px; }
QScrollBar::handle:vertical { background: %(input_line)s; border-radius: 4px; min-height: 32px; }
QScrollBar::handle:vertical:hover { background: %(faint)s; }
QScrollBar:horizontal { background: transparent; height: 12px; margin: 2px; }
QScrollBar::handle:horizontal { background: %(input_line)s; border-radius: 4px; min-width: 32px; }
QScrollBar::add-line, QScrollBar::sub-line { width: 0px; height: 0px; }
QScrollBar::add-page, QScrollBar::sub-page { background: none; }
QDialog, QMessageBox { background: %(bg)s; }
""" % v


def launcher_palette(t):
    p = QPalette()
    for role, key in ((QPalette.Window, "bg"), (QPalette.WindowText, "text"), (QPalette.Base, "input"),
                      (QPalette.AlternateBase, "card"), (QPalette.Text, "text"), (QPalette.Button, "input"),
                      (QPalette.ButtonText, "text"), (QPalette.Highlight, "accent"), (QPalette.ToolTipBase, "card"),
                      (QPalette.ToolTipText, "text"), (QPalette.PlaceholderText, "faint"), (QPalette.Mid, "line"),
                      (QPalette.Link, "accent")):
        p.setColor(role, QColor(t[key]))
    p.setColor(QPalette.HighlightedText, QColor("#ffffff"))
    for role, key in ((QPalette.Text, "faint"), (QPalette.WindowText, "faint"), (QPalette.ButtonText, "faint")):
        p.setColor(QPalette.Disabled, role, QColor(t[key]))
    return p


def flag_pixmap(n, w=24, h=16):
    """A small drawn flag for a game language (no files, no emoji), drawn at 2x for HiDPI."""
    k = 2
    pm = QPixmap(w * k, h * k)
    pm.fill(Qt.transparent)
    p = QPainter(pm)
    p.setRenderHint(QPainter.Antialiasing)
    p.scale(k, k)

    def fill(x0, y0, x1, y1, c):
        p.fillRect(QRectF(x0, y0, x1 - x0, y1 - y0), QColor(c))
    if n == 0:        # United Kingdom (simplified)
        fill(0, 0, w, h, "#012169")
        p.setPen(QPen(QColor("white"), 3))
        p.drawLine(QPointF(0, 0), QPointF(w, h))
        p.drawLine(QPointF(w, 0), QPointF(0, h))
        p.setPen(QPen(QColor("#C8102E"), 1))
        p.drawLine(QPointF(0, 0), QPointF(w, h))
        p.drawLine(QPointF(w, 0), QPointF(0, h))
        fill(0, h / 2 - 3, w, h / 2 + 3, "white")
        fill(w / 2 - 3, 0, w / 2 + 3, h, "white")
        fill(0, h / 2 - 2, w, h / 2 + 2, "#C8102E")
        fill(w / 2 - 2, 0, w / 2 + 2, h, "#C8102E")
    elif n == 1:      # France
        fill(0, 0, w / 3, h, "#0055A4")
        fill(w / 3, 0, w * 2 / 3, h, "white")
        fill(w * 2 / 3, 0, w, h, "#EF4135")
    elif n == 2:      # Germany
        fill(0, 0, w, h / 3, "black")
        fill(0, h / 3, w, h * 2 / 3, "#DD0000")
        fill(0, h * 2 / 3, w, h, "#FFCE00")
    elif n == 3:      # Italy
        fill(0, 0, w / 3, h, "#009246")
        fill(w / 3, 0, w * 2 / 3, h, "white")
        fill(w * 2 / 3, 0, w, h, "#CE2B37")
    elif n == 4:      # Netherlands
        fill(0, 0, w, h / 3, "#AE1C28")
        fill(0, h / 3, w, h * 2 / 3, "white")
        fill(0, h * 2 / 3, w, h, "#21468B")
    elif n == 5:      # Spain
        fill(0, 0, w, h, "#AA151B")
        fill(0, h / 4, w, h - h / 4, "#F1BF00")
    elif n == 6:      # Portugal
        fill(0, 0, w * 2 / 5, h, "#006600")
        fill(w * 2 / 5, 0, w, h, "#FF0000")
        p.setPen(QPen(QColor("#FFE000"), 1.6))
        p.setBrush(Qt.NoBrush)
        p.drawEllipse(QPointF(w * 2 / 5, h / 2), h / 4, h / 4)
    elif n == 7:      # Sweden
        fill(0, 0, w, h, "#006AA7")
        fill(0, h / 2 - 2, w, h / 2 + 2, "#FECC02")
        fill(w * 3 / 8 - 2, 0, w * 3 / 8 + 2, h, "#FECC02")
    elif n == 8:      # Denmark
        fill(0, 0, w, h, "#C8102E")
        fill(0, h / 2 - 2, w, h / 2 + 2, "white")
        fill(w * 3 / 8 - 2, 0, w * 3 / 8 + 2, h, "white")
    elif n == 9:      # United States (simplified: the English of the USA disc)
        for i in range(7):
            fill(0, h * 2 * i / 13, w, h * (2 * i + 1) / 13, "#B22234")
            if i < 6:
                fill(0, h * (2 * i + 1) / 13, w, h * (2 * i + 2) / 13, "white")
        fill(0, 0, w * 0.42, h * 7 / 13, "#3C3B6E")
    elif n == 10:     # Japan (the Japanese disc)
        fill(0, 0, w, h, "white")
        p.setPen(Qt.NoPen)
        p.setBrush(QColor("#BC002D"))
        p.drawEllipse(QPointF(w / 2, h / 2), h * 0.3, h * 0.3)
    p.setPen(QPen(QColor(0, 0, 0, 60), 0.6))
    p.setBrush(Qt.NoBrush)
    p.drawRect(QRectF(0.3, 0.3, w - 0.6, h - 0.6))
    p.end()
    pm.setDevicePixelRatio(k)
    return pm


def window_icon():
    for d in (os.path.join(getattr(sys, "_MEIPASS", ""), "icons"), os.path.join(ROOT, "tools", "icons")):
        for name in ("launcher", "port", "gfx"):
            for ext in ((".ico", ".png") if os.name == "nt" else (".png", ".ico")):
                p = os.path.join(d, name + ext)
                if os.path.isfile(p):
                    ic = QIcon(p)
                    if not ic.isNull():
                        return ic
    return QIcon()


def build_command(s, civ_play=None):
    """The tombi-pc command line for the settings s (and a rebuilt town button's civ_play, never saved); returns
    (cmd, gfx_note), gfx_note telling the log why the Spanish redraws (--patch-tex) are or are not passed."""
    s = dict(s, civ_play=civ_play)
    exe = game_binary()
    source = ["--data", data_dir()] if data_ready() else ["--disc", bin_of_cue(s["disc"])]
    cmd = [exe] + source + ["--window", "--scale", str(s.get("scale", 2)),
           "--log-fps", "2",      # the measured vblank rate and its histogram in the log (no counter on screen)
           "--renderer", "soft" if s.get("renderer") == "soft" else "gl",
           "--aspect", s.get("aspect") if s.get("aspect") in ("square", "16:9", "21:9", "32:9", "auto") else "4:3"]
    if s.get("renderer") != "soft" and s.get("internal_res", 1) in (2, 3, 4):
        cmd += ["--internal-res", str(s["internal_res"])]
    fps = s.get("fps") or ("auto" if s.get("fps60") and s.get("experimental") else "off")
    if fps != "off":                       # --fps: interpolated presents (both renderers; the old OpenGL-only 60 fps switch = auto)
        cmd += ["--fps", str(fps)]
    lang = has("lang")         # (the USA build is English only: no --lang, no Spanish redraws, no labels)
    if lang and s.get("lang"):
        cmd += ["--lang", str(s["lang"])]
    if lang and str(s.get("lang")) == "5" and (s.get("subscreen_labels") or "").strip():
        cmd += ["--subscreen-labels", s["subscreen_labels"].strip()]   # the Select tabs / leaf buttons, in the disc's lettering
    if has("cut") and s.get("cut_content"):
        cmd += ["--cut-content"]
        if s.get("early_title"):
            cmd += ["--early-title", s["early_title"]]
    gfx = os.path.join(data_dir(), DP.GFX_TBTX)    # the Spanish redraws the text patch generated (tools/gfx_es)
    if not lang:
        note = "no (%s: English only)" % (GL.version_of(current_sha1()) or {}).get("region", "?")
    elif str(s.get("lang")) != "5":
        note = "no (language is not Spanish)"
    elif cmd[1] != "--data":
        note = "no (running from the disc image, not data/)"
    elif not os.path.isfile(gfx):
        note = "no (%s missing: text patch off, or its redraws were skipped)" % gfx
    else:
        cmd += ["--patch-tex", gfx]
        note = "yes (%s)" % gfx
    if not translation_native(s):
        cmd += ["--no-translation-file"]      # tombi2 would load translation.tpatch from its cwd: only when it is the choice
    sd = version_save_dir(s.get("savedir") or "")
    if sd:
        if sd != s.get("savedir"):         # (a version's own subfolder: the port does not create folders)
            os.makedirs(sd, exist_ok=True)
        cmd += ["--save-dir", sd]
    if s.get("texpack") and os.path.isdir(s["texpack"]) and s.get("renderer") != "soft" and s.get("experimental"):
        cmd += ["--texture-pack", s["texpack"]]
    if s.get("fullscreen"):
        cmd += ["--fullscreen"]
    if s.get("show_fps"):
        cmd += ["--show-fps"]
    if s.get("dualshock"):
        cmd += ["--dualshock"]
    try:
        rumble = max(0, min(200, int(s.get("rumble", 100))))
    except (TypeError, ValueError):
        rumble = 100
    if rumble != 100:
        cmd += ["--rumble", str(rumble)]
    pm = pad_map_arg(s.get("pad_map"))
    if pm:
        cmd += ["--pad-map", pm]
    if has("prompts") and s.get("prompts_style", "auto") in ("playstation", "xbox", "nintendo", "generic", "custom"):
        cmd += ["--prompts", s["prompts_style"]]
    if has("prompts") and s.get("prompts_style") == "custom":
        cmd += ["--prompt-colors", s.get("prompt_colors") or PC.DEFAULT]
    # no --launcher-exit on PC: the Quit Game entries are for handhelds; here the window is simply closed
    # a version without the PAL-address mods gets the portable ones only (no --hz: an NTSC build runs at 60 by itself)
    cmd += M.mod_args(s, app_dir(), allowed=None if has("pokes") else M.PORTABLE_MODS)
    if has("cut"):
        cmd += M.civ_args(s, os.path.join(civ_dir(), "maps"), os.path.join(civ_dir(), "wfm"))
    return cmd, note


# ---------------------------------------------------------------------------------------------- settings
_TCL_TRUE, _TCL_FALSE = ("1", "true", "yes", "on"), ("0", "false", "no", "off")


def coerce(v, kind):
    """A saved value read as the launcher's widgets read it (bool / int / str), or ValueError when it cannot be
    (then the saved value is kept as it is, and the widget shows its default)."""
    if kind is bool:
        if isinstance(v, (bool, int, float)):
            return bool(v)
        t = str(v).strip().lower()
        if t in _TCL_TRUE or t in _TCL_FALSE:
            return t in _TCL_TRUE
        raise ValueError(v)
    if kind is int:
        if isinstance(v, bool):
            return int(v)
        if isinstance(v, int):
            return v
        try:
            return int(str(v).strip())
        except ValueError:
            return int(float(str(v).strip()))
    if isinstance(v, bool):
        return "1" if v else "0"
    return v if isinstance(v, str) else str(v)


class Bridge(QObject):
    """Runs callables on the GUI thread (the disc check / extraction / texture dump / town generation threads)."""
    call = Signal(object)

    def __init__(self):
        super().__init__()
        self.call.connect(lambda fn: fn())


class WheelGuard(QObject):
    """Comboboxes and spin boxes inside the scrolled pages change only when focused: a wheel over them scrolls the page."""

    def eventFilter(self, obj, e):
        if e.type() == QEvent.Wheel and isinstance(obj, (QComboBox, QAbstractSpinBox)) and not obj.hasFocus() \
                and obj.window() is self.parent():         # (the save editor's widgets keep their own behaviour)
            w = obj.parentWidget()
            while w is not None and not isinstance(w, QScrollArea):
                w = w.parentWidget()
            if w is not None:
                QApplication.sendEvent(w.verticalScrollBar(), e)
            return True
        return False


class KeyButton(QPushButton):
    """A keyboard-table key: click, then the next key pressed is the new binding (Esc cancels; focus loss ends)."""

    def __init__(self, app, b):
        super().__init__()
        self.app, self.b = app, b
        self.setObjectName("keycap")
        self.setCheckable(True)
        self.clicked.connect(lambda: self.app._key_begin(self.b))

    def event(self, e):
        if e.type() == QEvent.KeyPress and self.app.key_capture == self.b:
            self.app._key_press(self.b, e)
            return True
        return super().event(e)

    def focusOutEvent(self, e):
        super().focusOutEvent(e)
        self.app._key_end(self.b)


class Swatch(QPushButton):
    def set_color(self, rgb):
        c = QColor("#%06x" % rgb)
        fg = "#000000" if c.lightness() > 140 else "#ffffff"
        self.setStyleSheet("QPushButton { background: %s; color: %s; border: 1px solid rgba(0,0,0,60); }" % (c.name(), fg))


class GlyphPreview(QWidget):
    """The four button prompts drawn as the game draws them (tools/prompt_custom.py = port2/shims/prompts.c)."""

    def __init__(self, st):
        super().__init__()
        self.st = st
        self.setFixedSize(12 * 7 * 3 + 40, 16 * 7 * 3 + 20)

    def paintEvent(self, _e):
        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing)
        path = QPainterPath()
        path.addRoundedRect(QRectF(self.rect()), 10, 10)
        p.fillPath(path, QColor("#5a8a5a"))
        k = 6
        for i, (gx, gy) in enumerate([(1, 2), (2, 1), (0, 1), (1, 0)]):    # south, east, west, north on a 3 x 3 grid
            g = PC.glyph(i, self.st["sym"], *self.st["cols"][i])
            ox, oy = 10 + gx * 12 * k * 1.15, 10 + gy * 16 * k * 0.85
            for y, row in enumerate(g):
                for x, c in enumerate(row):
                    if c:
                        p.fillRect(QRectF(ox + x * k, oy + y * k, k, k), QColor(*c))
        p.end()


class PadView(QWidget):
    """The PS1 pad photo (tools/icons/ps1_pad.png) with a leader line from each button to a chip showing the
    controller button bound to it; hovering highlights a button, clicking selects its row in the table below.
    Without the photo the same layout is drawn over a plain vector pad."""
    PAD_W, PAD_H = 660, 326
    # the photo (463x315, background removed) is drawn PHOTO_W base units wide with its top-left at PHOTO_XY; the
    # hotspots / line ends are in the photo's own pixels.  The photo looks at the pad from the front, so of each
    # shoulder only the top shows: L1/R1 are the light shoulder blocks, L2/R2 the dark ridge above them.
    PHOTO_XY, PHOTO_W, PHOTO_SIZE = (154, 34), 352, (463, 315)
    PHOTO_ROWS = {"l2": 26, "l1": 58, "up": 92, "left": 121, "down": 150, "right": 180,
                  "r2": 26, "r1": 58, "triangle": 92, "circle": 121, "cross": 150, "square": 180, "select": 306, "start": 306}
    PHOTO_SPOTS = {   # kind, box (photo px).  The pad is symmetric about x 231
        "l2": ("rect", 60, -4, 132, 10), "l1": ("rect", 50, 10, 140, 40),
        "r2": ("rect", 330, -4, 402, 10), "r1": ("rect", 322, 10, 412, 40),
        "up": ("rect", 77, 70, 109, 102), "down": ("rect", 77, 125, 109, 158),
        "left": ("rect", 48, 99, 79, 129), "right": ("rect", 105, 99, 137, 129),
        "triangle": ("oval", 349, 58, 387, 96), "circle": ("oval", 392, 97, 430, 135),
        "cross": ("oval", 349, 134, 387, 172), "square": ("oval", 306, 96, 344, 134),
        "select": ("rect", 186, 134, 215, 155), "start": ("rect", 245, 134, 276, 155),
    }
    # where each leader line ends (photo px); "v" routes go across to the x of their end, then straight up/down to it
    PHOTO_LINES = {"l2": (80, 5), "l1": (68, 26), "up": (82, 84), "left": (54, 114), "down": (82, 147), "right": ("v", 121, 125),
                   "r2": (382, 5), "r1": (394, 26), "triangle": (384, 72), "circle": (426, 116), "cross": (384, 158),
                   "square": ("v", 325, 131), "select": ("v", 201, 152), "start": ("v", 260, 152)}
    FACE = {"triangle": "#1fa67a", "circle": "#e0413c", "cross": "#4f7fd9", "square": "#d96bb0"}

    def __init__(self, app):
        super().__init__()
        self.app = app
        self.setMouseTracking(True)
        self.setMinimumHeight(300)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        p = icon_file("ps1_pad.png")
        self.photo = QPixmap(p) if p else QPixmap()
        self.spots = {}
        self.hot = None
        self._cache = None

    def sizeHint(self):
        return QSize(660, 300)

    def _geom(self):
        cw, ch = self.width(), self.height()
        s = min(cw / self.PAD_W, ch / self.PAD_H, 1.3)
        return s, (cw - self.PAD_W * s) / 2, (ch - self.PAD_H * s) / 2

    def paintEvent(self, _e):
        t = self.app.t
        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing)
        p.setRenderHint(QPainter.SmoothPixmapTransform)
        s, ox, oy = self._geom()
        if s <= 0.05:
            return
        pw, ph = self.PHOTO_SIZE
        k = self.PHOTO_W / pw                      # photo px -> base units
        bx, by = self.PHOTO_XY
        X0, Y0 = ox + bx * s, oy + by * s
        f = k * s                                  # photo px -> widget px
        self.spots = {b: (kind, X0 + u0 * f, Y0 + v0 * f, X0 + u1 * f, Y0 + v1 * f)
                      for b, (kind, u0, v0, u1, v1) in self.PHOTO_SPOTS.items()}
        if not self.photo.isNull():
            dpr = self.devicePixelRatioF()
            w, h = max(1, round(pw * f * dpr)), max(1, round(ph * f * dpr))
            if self._cache is None or self._cache.size() != QSize(w, h):
                self._cache = self.photo.scaled(w, h, Qt.IgnoreAspectRatio, Qt.SmoothTransformation)
                self._cache.setDevicePixelRatio(dpr)
            p.drawPixmap(QPointF(X0, Y0), self._cache)
        else:
            self._draw_vector(p, X0, Y0, f)
        hot_set = {self.hot, self.app.pad_capture}
        acc = QColor(t["accent"])
        for b in M.BUTTONS:                        # the translucent ring of the hovered / selected button
            if b not in hot_set:
                continue
            kind, x0, y0, x1, y1 = self.spots[b]
            r = QRectF(x0, y0, x1 - x0, y1 - y0)
            fill = QColor(acc)
            fill.setAlpha(60)
            ring = QColor(acc)
            ring.setAlpha(235)
            p.setPen(QPen(ring, max(2.0, 2 * s)))
            p.setBrush(fill)
            if kind == "oval":
                p.drawEllipse(r)
            else:
                p.drawRoundedRect(r, 5 * s, 5 * s)
        pt = lambda u, v: (bx + u * k, by + v * k)
        R = self.PHOTO_ROWS
        lines = {}
        for b, end in self.PHOTO_LINES.items():
            if end[0] == "v":
                ex, ey = pt(*end[1:])
                lines[b] = [(ex, R[b]), (ex, ey)]
            else:
                lines[b] = [pt(*end)]
        nf, kf = QFont(self.font()), QFont(self.font())
        nf.setPixelSize(max(9, round(12.5 * s)))
        kf.setPixelSize(max(9, round(12.5 * s)))
        kf.setBold(True)
        nfm, kfm = QFontMetricsF(nf), QFontMetricsF(kf)
        P = lambda x, y: QPointF(ox + x * s, oy + y * s)
        cx = self.PAD_W / 2
        lx_in, rx_in = 150, self.PAD_W - 150
        chip_h, pad_x = 24, 9
        self.chips = {}
        for b, route in lines.items():
            y = R[b]
            left = b in ("l2", "l1", "up", "left", "down", "right", "select")
            edge_x = (cx - 38 if left else cx + 38) if b in ("select", "start") else (lx_in if left else rx_in)
            name = tr("btn." + b)
            key = self.app._pad_key_text(b)
            room = (edge_x if left else self.PAD_W - edge_x) - 6 - (nfm.horizontalAdvance(name) / s + 8 if name else 0)
            maxw = max(40, room) * s - 2 * pad_x * s
            if kfm.horizontalAdvance(key) > maxw:
                while len(key) > 1 and kfm.horizontalAdvance(key + "…") > maxw:
                    key = key[:-1]
                key += "…"
            kw = kfm.horizontalAdvance(key) / s + 2 * pad_x
            x0, x1 = (edge_x - kw, edge_x) if left else (edge_x, edge_x + kw)
            hot = b in hot_set
            capt = b == self.app.pad_capture
            col = acc if hot else QColor(t["pad_line"])
            knee = edge_x + (12 if left else -12)
            pts = [(edge_x, y)] + ([(knee, y)] if b not in ("select", "start", "right", "square") else []) + route
            pen = QPen(col, max(1.0, (2 if hot else 1.2) * s))
            p.setPen(pen)
            p.setBrush(Qt.NoBrush)
            for (ax, ay), (bx2, by2) in zip(pts, pts[1:]):
                p.drawLine(P(ax, ay), P(bx2, by2))
            ex, ey = route[-1]
            p.setPen(QPen(QColor(t["card"]), max(1.0, s)))
            p.setBrush(col)
            p.drawEllipse(P(ex, ey), 2.6 * s + 0.5, 2.6 * s + 0.5)
            chip = QRectF(P(x0, y - chip_h / 2), P(x1, y + chip_h / 2))
            self.chips[b] = chip
            p.setPen(QPen(acc if (hot or capt) else QColor(t["chip_line"]), max(1.0, s)))
            p.setBrush(acc if capt else (QColor(t["accent_soft"]) if hot else QColor(t["chip"])))
            p.drawRoundedRect(chip, 6 * s, 6 * s)
            p.setFont(kf)
            p.setPen(QColor("#ffffff") if capt else QColor(t["text"]))
            p.drawText(chip, Qt.AlignCenter, key)
            p.setFont(nf)
            p.setPen(acc if hot else QColor(t["muted"]))
            if left:
                p.drawText(QRectF(P(0, y - 12), P(x0 - 7, y + 12)), Qt.AlignRight | Qt.AlignVCenter, name)
            else:
                p.drawText(QRectF(P(x1 + 7, y - 12), P(self.PAD_W, y + 12)), Qt.AlignLeft | Qt.AlignVCenter, name)
        p.end()

    def _draw_vector(self, p, X0, Y0, f):
        """A plain pad under the hotspots when the photo is missing."""
        t = self.app.t
        pw, ph = self.PHOTO_SIZE
        body = QPainterPath()
        body.addRoundedRect(QRectF(X0 + 20 * f, Y0 + 40 * f, (pw - 40) * f, 150 * f), 60 * f, 60 * f)
        for gx in (93, 369):
            body.addEllipse(QPointF(X0 + gx * f, Y0 + 200 * f), 75 * f, 100 * f)
        p.setPen(QPen(QColor(t["faint"]), 1.2))
        p.setBrush(QColor("#d7d8dc"))
        p.drawPath(body.simplified())
        for b, (kind, x0, y0, x1, y1) in self.spots.items():
            r = QRectF(x0, y0, x1 - x0, y1 - y0)
            p.setPen(QPen(QColor("#2c2e33"), 1))
            if kind == "oval":
                p.setBrush(QColor("#3f4148"))
                p.drawEllipse(r)
                p.setPen(QPen(QColor(self.FACE[b]), max(1.5, 2.2 * f)))
                p.setBrush(Qt.NoBrush)
                p.drawEllipse(r.adjusted(r.width() * .3, r.height() * .3, -r.width() * .3, -r.height() * .3))
            else:
                p.setBrush(QColor("#55585f") if b in ("up", "down", "left", "right") else QColor("#b9bbc1"))
                p.drawRoundedRect(r, 4, 4)

    def _button_at(self, pos):
        x, y = pos.x(), pos.y()
        for b, (kind, x0, y0, x1, y1) in self.spots.items():
            if kind == "oval":
                rx, ry = (x1 - x0) / 2, (y1 - y0) / 2
                if rx > 0 and ry > 0 and ((x - x0 - rx) / rx) ** 2 + ((y - y0 - ry) / ry) ** 2 <= 1:
                    return b
            elif x0 <= x <= x1 and y0 <= y <= y1:
                return b
        for b, r in getattr(self, "chips", {}).items():
            if r.contains(QPointF(x, y)):
                return b
        return None

    def mouseMoveEvent(self, e):
        b = self._button_at(e.position())
        if b != self.hot:
            self.hot = b
            self.setCursor(Qt.PointingHandCursor if b else Qt.ArrowCursor)
            self.update()

    def leaveEvent(self, _e):
        if self.hot:
            self.hot = None
            self.setCursor(Qt.ArrowCursor)
            self.update()

    def mousePressEvent(self, e):
        b = self._button_at(e.position())
        if b:
            self.app._pad_select(b)
        elif self.app.pad_capture:
            self.app.pad_capture = None
            self.app._padmap_sync()


def wrap_label(text, name=None, sel=False):
    lb = QLabel(text)
    lb.setWordWrap(True)
    if name:
        lb.setObjectName(name)
    if sel:
        lb.setTextInteractionFlags(Qt.TextSelectableByMouse)
    return lb


def hbox(*widgets, spacing=8, margins=(0, 0, 0, 0), stretch_last=False):
    w = QWidget()
    lay = QHBoxLayout(w)
    lay.setContentsMargins(*margins)
    lay.setSpacing(spacing)
    for x in widgets:
        if x is None:
            lay.addStretch(1)
        elif isinstance(x, int):
            lay.addSpacing(x)
        else:
            lay.addWidget(x)
    return w, lay


LIBRARY_VIEW = "list"       # the library's default look (setting "library_view": grid, list or banner)


class TileGrid(QWidget):
    """The grid view's cards in as many columns as the width takes (laid out again when the window is resized)."""
    GAP = 12

    def __init__(self, tiles):
        super().__init__()
        self.tiles, self.cols = tiles, 0
        self.g = QGridLayout(self)
        self.g.setContentsMargins(0, 4, 0, 4)
        self.g.setHorizontalSpacing(self.GAP)
        self.g.setVerticalSpacing(self.GAP)
        self.g.setSizeConstraint(QLayout.SetNoConstraint)
        self._place(4)

    def _place(self, cols):
        cols = max(1, min(cols, len(self.tiles)))
        if cols == self.cols:
            return
        self.cols = cols
        for t in self.tiles:
            self.g.removeWidget(t)
        for i, t in enumerate(self.tiles):
            self.g.addWidget(t, i // cols, i % cols, Qt.AlignLeft | Qt.AlignTop)
        for c in range(len(self.tiles) + 1):
            self.g.setColumnStretch(c, 1 if c == cols else 0)

    def minimumSizeHint(self):        # one column is enough: the width never pushes the page wider
        m = self.g.minimumSize()
        return QSize(self.tiles[0].width() if self.tiles else 0, m.height())

    def resizeEvent(self, e):
        w = self.tiles[0].width() if self.tiles else 1
        cols = self.cols
        self._place((e.size().width() + self.GAP) // (w + self.GAP))
        super().resizeEvent(e)
        if cols != self.cols:
            self.updateGeometry()


class GameTile(QAbstractButton):
    """A game of the library drawn as a card: its title art (the title screen rebuilt from the player's disc,
    game_library.make_art) or its logo, the name, serial and languages, and its state (a coloured dot). Modes: "grid"
    (cover cards), "list" (rows with the logo), "banner" (the selected game, wide) and "chip" (the small logo buttons
    under the banner). d["add"]: the Add game tile."""
    SIZES = {"grid": (152, 206), "list": (0, 84), "banner": (0, 232), "chip": (148, 78)}

    def __init__(self, app, d, mode):
        super().__init__()
        self.a, self.d, self.mode = app, d, mode
        self.setCursor(Qt.PointingHandCursor)
        self.setAttribute(Qt.WA_Hover)
        self.setFocusPolicy(Qt.StrongFocus)
        w, h = self.SIZES[mode]
        if mode == "list" and d.get("add"):
            h = 52
        if w:
            self.setFixedSize(w, h)
        else:
            self.setFixedHeight(h)
            self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        if mode == "banner":
            self.setCursor(Qt.ArrowCursor)
            self.setFocusPolicy(Qt.NoFocus)

    def contextMenuEvent(self, e):
        if self.d.get("add") or self.mode == "banner":
            return
        from PySide6.QtWidgets import QMenu
        m = QMenu(self.a)
        act = m.addAction(tr("b.removegame"))
        if m.exec(e.globalPos()) is act:      # after the menu: removing rebuilds the window (this card too)
            QTimer.singleShot(0, lambda s=self.d.get("serial", ""): self.a.remove_game(s))

    def _font(self, pt, weight=QFont.Normal):
        f = QFont(self.font())
        f.setPointSizeF(pt)
        f.setWeight(weight)
        return f

    def _cover(self, p, pm, r, radius, top_only=False, middle=False):
        """pm scaled to fill r (cropped, centred horizontally, from the top or, middle=True, centred) inside a rounded clip."""
        path = QPainterPath()
        path.addRoundedRect(r, radius, radius)
        if top_only:                          # square bottom corners: the text area continues below
            path.addRect(QRectF(r.x(), r.y() + r.height() / 2, r.width(), r.height() / 2))
        p.save()
        path.setFillRule(Qt.WindingFill)
        p.setClipPath(path.simplified())
        s = max(r.width() / pm.width(), r.height() / pm.height())
        w, h = pm.width() * s, pm.height() * s
        p.setRenderHint(QPainter.SmoothPixmapTransform, True)
        y = r.y() + ((r.height() - h) / 2 if middle else 0)
        p.drawPixmap(QRectF(r.x() + (r.width() - w) / 2, y, w, h), pm, QRectF(pm.rect()))
        p.restore()

    def _logo(self, p, r, color):
        """The logo fitted in r, or the game's name in large letters when there is no art."""
        pm = self.d.get("art", {}).get("logo")
        if pm is not None:
            s = min(r.width() / pm.width(), r.height() / pm.height())
            w, h = pm.width() * s, pm.height() * s
            p.setRenderHint(QPainter.SmoothPixmapTransform, True)
            p.drawPixmap(QRectF(r.x() + (r.width() - w) / 2, r.y() + (r.height() - h) / 2, w, h), pm, QRectF(pm.rect()))
            return
        p.setPen(QColor(color))
        text = self.d.get("short") or self.d.get("title") or "?"
        size = max(10.0, r.height() / 3.2)
        f = self._font(size, QFont.Black)
        width = QFontMetricsF(f).horizontalAdvance(text)
        if width > r.width() * 0.92:          # one line, like the art: shrink a long name (オレっ！トンバ) to fit
            f = self._font(max(6.0, size * r.width() * 0.92 / width), QFont.Black)
        p.setFont(f)
        p.drawText(r, Qt.AlignCenter, text)

    _flag_cache = {}

    def _flags(self, p, x, y, h=11):
        """the disc's languages as small drawn flags in a row from x, centred on y; returns the right end"""
        w = round(h * 1.5)
        for n in self.d.get("flags") or ():
            pm = GameTile._flag_cache.get((n, w, h))
            if pm is None:
                pm = GameTile._flag_cache[(n, w, h)] = flag_pixmap(n, w, h)
            p.drawPixmap(QPointF(x, y - h / 2), pm)
            x += w + 4
        return x

    def _status(self, p, x, y, t, color_text):
        dot = DOT.get(self.d.get("level"), DOT["info"])
        p.setPen(Qt.NoPen)
        p.setBrush(QColor(dot))
        p.drawEllipse(QRectF(x, y - 4.5, 9, 9))
        p.setPen(QColor(color_text))
        p.setFont(self._font(9))
        p.drawText(QPointF(x + 15, y + 4), self.d.get("status", ""))

    def paintEvent(self, _e):
        t = self.a.t
        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing)
        r = QRectF(self.rect()).adjusted(1.5, 1.5, -1.5, -1.5)
        sel, hover = self.d.get("selected"), self.underMouse() and self.mode != "banner"
        radius = 12 if self.mode in ("grid", "banner") else 10
        if self.d.get("add"):
            pen = QPen(QColor(t["accent"] if hover else t["input_line"]), 1.6, Qt.DashLine)
            p.setPen(pen)
            p.setBrush(QColor(t["hover"] if hover else t["card"]))
            p.drawRoundedRect(r, radius, radius)
            p.setPen(QColor(t["accent"]))
            if self.mode == "grid":
                p.setFont(self._font(30, QFont.Light))
                p.drawText(QRectF(r.x(), r.y() + 48, r.width(), 50), Qt.AlignCenter, "+")
                p.setFont(self._font(10.5, QFont.DemiBold))
                p.drawText(QRectF(r.x(), r.y() + 102, r.width(), 24), Qt.AlignCenter, self.d["title"])
                p.setPen(QColor(t["muted"]))
                p.setFont(self._font(8.5))
                p.drawText(QRectF(r.x() + 12, r.y() + 128, r.width() - 24, 60), Qt.AlignHCenter | Qt.AlignTop | Qt.TextWordWrap,
                           self.d.get("sub", ""))
            elif self.mode == "chip":
                p.setFont(self._font(20, QFont.Light))
                p.drawText(QRectF(r.x(), r.y() + 4, r.width(), 40), Qt.AlignCenter, "+")
                p.setFont(self._font(9, QFont.DemiBold))
                p.drawText(QRectF(r.x(), r.y() + 42, r.width(), 24), Qt.AlignCenter, self.d["title"])
            else:
                p.setFont(self._font(10.5, QFont.DemiBold))
                p.drawText(r, Qt.AlignCenter, "+   " + self.d["title"])
            p.end()
            return
        edge = QColor(t["accent"]) if sel else QColor(t["faint"] if hover else t["line"])
        art = self.d.get("art", {})
        if self.mode == "grid":
            p.setPen(Qt.NoPen)
            p.setBrush(QColor(t["card"]))
            p.drawRoundedRect(r, radius, radius)
            ar = QRectF(r.x(), r.y(), r.width(), r.width() * 0.75)
            if art.get("title") is not None:
                self._cover(p, art["title"], ar, radius, top_only=True)
            else:
                self._fill_soft(p, ar, radius, t)
                self._logo(p, ar.adjusted(18, 18, -18, -18), t["accent"])
            y = ar.bottom() + 22
            p.setPen(QColor(t["text"]))
            p.setFont(self._fit(self.d["title"], 10.5, r.width() - 24, QFont.DemiBold))
            p.drawText(QPointF(r.x() + 12, y), self.d["title"])
            p.setPen(QColor(t["muted"]))
            line = self.d.get("sub", "")                                           # the serial, then the disc's languages (flags)
            p.setFont(self._fit(line, 8.5, r.width() - 24))
            p.drawText(QPointF(r.x() + 12, y + 18), line)
            self._flags(p, r.x() + 12, y + 29, 10)
            self._status(p, r.x() + 12, y + 52, t, t["muted"])
            p.setBrush(Qt.NoBrush)
            p.setPen(QPen(edge, 2.6 if sel else 1.2))
            p.drawRoundedRect(r, radius, radius)
            if sel:
                self._check(p, QPointF(r.right() - 18, r.y() + 18), t)
        elif self.mode == "list":
            p.setPen(QPen(edge, 2.2 if sel else 1.1))
            p.setBrush(QColor(t["accent_soft"] if sel else t["hover"] if hover else t["card"]))
            p.drawRoundedRect(r, radius, radius)
            lr = QRectF(r.x() + 12, r.y() + 10, 150, r.height() - 20)
            self._logo(p, lr, t["accent"])
            x = lr.right() + 18
            p.setPen(QColor(t["text"]))
            p.setFont(self._font(11, QFont.DemiBold))
            p.drawText(QPointF(x, r.y() + 28), self.d["title"])
            p.setPen(QColor(t["muted"]))
            p.setFont(self._font(9))
            sub = self.d.get("sub", "")
            p.drawText(QPointF(x, r.y() + 48), sub)
            self._flags(p, x + QFontMetricsF(self._font(9)).horizontalAdvance(sub) + 10, r.y() + 44, 11)
            self._status(p, x, r.y() + 66, t, t["muted"])
            if sel:
                self._check(p, QPointF(r.right() - 22, r.center().y()), t)
        elif self.mode == "chip":
            p.setPen(QPen(edge, 2.4 if sel else 1.1))
            p.setBrush(QColor(t["accent_soft"] if sel else t["hover"] if hover else t["card"]))
            p.drawRoundedRect(r, radius, radius)
            self._logo(p, r.adjusted(12, 8, -12, -24), t["accent"])
            p.setPen(QColor(t["muted"]))
            p.setFont(self._font(8))
            dot = DOT.get(self.d.get("level"), DOT["info"])
            label = self.d.get("sub", "")
            fm = QFontMetricsF(self._font(8))
            w = fm.horizontalAdvance(label) + 13
            x0 = r.center().x() - w / 2
            p.setPen(Qt.NoPen)
            p.setBrush(QColor(dot))
            p.drawEllipse(QRectF(x0, r.bottom() - 15, 7, 7))
            p.setPen(QColor(t["muted"]))
            p.drawText(QPointF(x0 + 13, r.bottom() - 8), label)
        else:                                  # banner: the selected game, wide
            if art.get("scene") is not None:     # the landscape behind, the logo crisp on it (not scaled up with it)
                self._cover(p, art["scene"], r, radius, middle=True)
            else:
                self._fill_soft(p, r, radius, t)
            self._logo(p, QRectF(r.x() + 22, r.y() + 16, r.width() * 0.5, r.height() - 106), t["accent"])
            g = QLinearGradient(r.x(), r.bottom() - 96, r.x(), r.bottom())
            g.setColorAt(0, QColor(0, 0, 0, 0))
            g.setColorAt(1, QColor(0, 0, 0, 170))
            path = QPainterPath()
            path.addRoundedRect(r, radius, radius)
            p.save()
            p.setClipPath(path)
            p.fillRect(QRectF(r.x(), r.bottom() - 96, r.width(), 96), g)
            p.restore()
            p.setPen(QColor("#ffffff"))
            p.setFont(self._font(15, QFont.Bold))
            p.drawText(QPointF(r.x() + 20, r.bottom() - 40), self.d["title"])
            p.setFont(self._font(9.5))
            p.setPen(QColor(255, 255, 255, 220))
            sub = self.d.get("sub", "")
            p.drawText(QPointF(r.x() + 20, r.bottom() - 18), sub)
            self._flags(p, r.x() + 20 + QFontMetricsF(self._font(9.5)).horizontalAdvance(sub) + 10, r.bottom() - 22, 12)
            fm = QFontMetricsF(self._font(9))
            sw = fm.horizontalAdvance(self.d.get("status", "")) + 34
            pill = QRectF(r.right() - 16 - sw, r.bottom() - 40, sw, 26)
            p.setPen(Qt.NoPen)
            p.setBrush(QColor(0, 0, 0, 120))
            p.drawRoundedRect(pill, 13, 13)
            self._status(p, pill.x() + 11, pill.center().y(), t, "#ffffff")
        if self.hasFocus() and self.mode != "banner":
            p.setBrush(Qt.NoBrush)
            p.setPen(QPen(QColor(t["accent"]), 1, Qt.DotLine))
            p.drawRoundedRect(r.adjusted(3, 3, -3, -3), radius - 2, radius - 2)
        p.end()

    def _fill_soft(self, p, r, radius, t):
        g = QLinearGradient(r.topLeft(), r.bottomRight())
        g.setColorAt(0, QColor(t["accent_soft"]))
        g.setColorAt(1, QColor(t["card"]))
        path = QPainterPath()
        path.addRoundedRect(r, radius, radius)
        p.fillPath(path, g)

    def _check(self, p, c, t):
        p.setPen(QPen(QColor("#ffffff"), 2))
        p.setBrush(QColor(t["accent"]))
        p.drawEllipse(c, 11, 11)
        pen = QPen(QColor("#ffffff"), 2.4)
        pen.setCapStyle(Qt.RoundCap)
        pen.setJoinStyle(Qt.RoundJoin)
        p.setPen(pen)
        path = QPainterPath(QPointF(c.x() - 5, c.y()))
        path.lineTo(c.x() - 1.5, c.y() + 3.5)
        path.lineTo(c.x() + 5, c.y() - 3.5)
        p.setBrush(Qt.NoBrush)
        p.drawPath(path)

    def _fit(self, text, pt, width, weight=QFont.Normal):
        """The font at pt, smaller (down to 7 pt) when text is wider than width: never cut with an ellipsis."""
        f = self._font(pt, weight)
        while pt > 7 and QFontMetricsF(f).horizontalAdvance(text) > width:
            pt -= 0.5
            f = self._font(pt, weight)
        return f
class App(QMainWindow):
    def __init__(self):
        super().__init__()
        self.s = load_settings()
        if GL.migrate(self.s, app_dir()):          # an older launcher's single disc / data/ becomes the first game
            save_settings(self.s)
        self.extracting, self.ext_pct = False, None
        self._art_tried = set()
        self._apply_entry()
        self.proc = None
        self.editor = None           # the save editor window (edit_save), while open
        self.log_path = os.path.join(app_dir(), "tombi-port.log")   # the last run's log (Copy log works before a run too)
        self.page = "play"
        self.starting = False
        self._level = "info"
        self.status_msg = ""
        self.civ_play = None
        self.bridge = Bridge()
        self.wheel_guard = WheelGuard(self)
        QApplication.instance().installEventFilter(self.wheel_guard)
        self.poll_timer = QTimer(self)
        self.poll_timer.setSingleShot(True)
        self.poll_timer.timeout.connect(self.poll)
        self.setWindowIcon(window_icon())
        scr = QGuiApplication.primaryScreen().availableGeometry() if QGuiApplication.primaryScreen() else None
        w, h = 1010, 720
        if scr is not None:
            w, h = min(w, scr.width()), min(h, scr.height() - 40)
        self.resize(w, h)
        self.setMinimumSize(min(w, 860), min(h, 600))
        self._apply_theme()
        try:
            QGuiApplication.styleHints().colorSchemeChanged.connect(lambda *_: (self._apply_theme(), self.rebuild()))
        except AttributeError:
            pass
        self._build()

    # ---- helpers
    def after(self, ms, fn, *args):
        QTimer.singleShot(ms, lambda: fn(*args))

    def later(self, fn, *args):
        """fn(*args) on the GUI thread (from a worker thread)."""
        self.bridge.call.emit(lambda: fn(*args))

    def _apply_theme(self):
        self.theme = theme_name()
        self.t = THEMES[self.theme]
        self.setPalette(launcher_palette(self.t))
        self.setStyleSheet(launcher_qss(self.t, self.theme))

    def v(self, key, kind=str, default=""):
        """The setting key read as a widget of kind bool / int / str reads it, written back normalised (what the
        Tk launcher's variables did); a value that does not convert is kept and the widget shows default."""
        raw = self.s.get(key, default)
        try:
            val = coerce(raw, kind)
        except (ValueError, TypeError):
            return coerce(default, kind)
        self.s[key] = val
        return val

    def setv(self, key, val):
        self.s[key] = val
        if key in ("renderer", "scale", "fullscreen", "internal_res", "aspect", "hz60"):
            self._refresh()

    def say(self, msg, level="info"):
        """The status line (disc card, log page) and its coloured dot: ok green, bad red, busy orange, info grey."""
        self._level = level
        self.status_msg = msg
        for lb in (getattr(self, "status_lbl", None), getattr(self, "log_status", None)):
            try:
                if lb is not None:
                    lb.setText(msg)
            except RuntimeError:
                pass
        try:
            self.dot.setStyleSheet("background: %s; border-radius: 5px;" % DOT.get(level, DOT["info"]))
        except (RuntimeError, AttributeError):
            pass

    def info(self, text):
        QMessageBox.information(self, tr("title"), text)

    def error(self, text):
        QMessageBox.critical(self, tr("title"), text)

    def warning(self, text):
        QMessageBox.warning(self, tr("title"), text)

    def ask(self, text):
        return QMessageBox.question(self, tr("title"), text, QMessageBox.Yes | QMessageBox.No) == QMessageBox.Yes

    # ---- layout helpers
    def card(self, parent_lay, title=None, spacing=8):
        """A rounded card added to parent_lay; returns its inner vertical layout."""
        fr = QFrame()
        fr.setObjectName("card")
        lay = QVBoxLayout(fr)
        lay.setContentsMargins(20, 16, 20, 18)
        lay.setSpacing(spacing)
        if title:
            t = QLabel(title)
            t.setObjectName("cardTitle")
            lay.addWidget(t)
        if parent_lay is not None:
            parent_lay.addWidget(fr)
        lay.frame = fr
        return lay

    def page_frame(self, name, title):
        """A page: its title and a scrolled body (cards stacked from the top)."""
        page = QWidget()
        page.setObjectName("page")
        outer = QVBoxLayout(page)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.setSpacing(0)
        area = QScrollArea()
        area.setWidgetResizable(True)
        area.setFrameShape(QFrame.NoFrame)
        area.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        body = QWidget()
        body.setObjectName("scrollBody")
        lay = QVBoxLayout(body)
        lay.setContentsMargins(28, 22, 28, 22)
        lay.setSpacing(14)
        t = QLabel(title)
        t.setObjectName("pageTitle")
        lay.addWidget(t)
        lay.addSpacing(2)
        area.setWidget(body)
        outer.addWidget(area)
        self.stack.addWidget(page)
        self.pages[name] = page
        self.page_areas[name] = area
        return lay

    def show_page(self, name):
        self.page = name
        self.stack.setCurrentWidget(self.pages[name])
        for n, b in self.nav.items():
            b.setChecked(n == name)

    def check(self, key, text, default=False):
        cb = QCheckBox(text)
        cb.setChecked(self.v(key, bool, default))
        cb.toggled.connect(lambda on, key=key: self.setv(key, on))
        return cb

    def radios(self, lay, key, kind, default, items):
        cur = self.v(key, kind, default)
        grp = QButtonGroup(lay.parentWidget() or self)
        out = []
        for text, val in items:
            rb = QRadioButton(text)
            rb.setChecked(cur == val)
            rb.toggled.connect(lambda on, key=key, val=val: on and self.setv(key, val))
            grp.addButton(rb)
            lay.addWidget(rb)
            out.append(rb)
        grp.setExclusive(True)           # (an unknown saved value: none checked, the value kept until one is clicked)
        out[0]._grp = grp
        return out

    def segmented(self, key, kind, default, items, min_w=0):
        """A row of toggle buttons (one chosen, accent coloured) for key."""
        cur = self.v(key, kind, default)
        w, lay = hbox(spacing=0)
        grp = QButtonGroup(w)
        grp.setExclusive(False)
        for i, (text, val) in enumerate(items):
            b = QPushButton(text)
            b.setCheckable(True)
            b.setObjectName("segL" if i == 0 else "segR" if i == len(items) - 1 else "seg")
            b.setChecked(cur == val)
            b.setCursor(Qt.PointingHandCursor)
            if min_w:
                b.setMinimumWidth(min_w)

            def clicked(_=False, b=b, val=val):
                for x in grp.buttons():
                    x.setChecked(x is b)
                self.setv(key, val)
            b.clicked.connect(clicked)
            grp.addButton(b)
            lay.addWidget(b)
        lay.addStretch(1)
        w._grp = grp
        return w

    def browse_row(self, key, command):
        e = QLineEdit(self.v(key, str, ""))
        e.textChanged.connect(lambda t, key=key: self.setv(key, t))
        b = QPushButton(tr("b.browse"))
        b.clicked.connect(command)
        w, lay = hbox(e, b)
        lay.setStretch(0, 1)
        self.edits[key] = e
        return w

    def set_edit(self, key, text):
        self.s[key] = text
        e = self.edits.get(key)
        if e is not None:
            e.setText(text)

    def info_button(self, title, text):
        b = QToolButton()
        b.setObjectName("info")
        b.setText("ⓘ")
        b.setCursor(Qt.PointingHandCursor)
        b.setToolTip(title)
        b.clicked.connect(lambda: QMessageBox.information(self, title, text))
        return b

    # ---- window
    def rebuild(self, check=True):
        """The window built again (launcher language, theme, another game: its pages and options); check=False: the
        selected game's disc is not checked again (it just was)."""
        self.collect()
        msg, lvl = self.status_msg, self._level
        old = self.centralWidget()
        self._build(check)
        if old is not None:
            old.deleteLater()
        if msg and lvl != "info":
            self.say(msg, lvl)
        if self.extracting:
            self._extracting(self.ext_pct)

    def _build(self, check=True):
        self.setWindowTitle(tr("title") + "  [" + VERSION_TAG + "]")
        self.edits = {}
        root = QWidget()
        root.setObjectName("root")
        grid = QGridLayout(root)
        grid.setContentsMargins(0, 0, 0, 0)
        grid.setSpacing(0)
        side = QFrame()
        side.setObjectName("sidebar")
        side.setFixedWidth(236)
        sl = QVBoxLayout(side)
        sl.setContentsMargins(12, 18, 12, 14)
        sl.setSpacing(2)
        head, hl = hbox(spacing=10, margins=(6, 0, 0, 0))
        logo = QLabel()
        ic = window_icon()
        if not ic.isNull():
            logo.setPixmap(ic.pixmap(QSize(34, 34)))
        name = QLabel("Tombi!")
        name.setObjectName("appName")
        hl.addWidget(logo)
        hl.addWidget(name)
        hl.addStretch(1)
        sl.addWidget(head)
        sl.addSpacing(16)
        self.nav = {}
        for n in PAGES:
            b = QPushButton("%s   %s" % (NAV_GLYPHS[n], tr("nav." + n).replace("&", "&&")))   # & is a mnemonic marker in a QPushButton
            b.setObjectName("nav")
            b.setCheckable(True)
            b.setCursor(Qt.PointingHandCursor)
            b.setFocusPolicy(Qt.NoFocus)
            b.clicked.connect(lambda _=False, n=n: self.show_page(n))
            sl.addWidget(b)
            self.nav[n] = b
        self.nav["cut"].setVisible(has("cut"))      # the cut content is PAL-only (port2: --cut-content, the rebuilt towns)
        if self.page == "cut" and not has("cut"):
            self.page = "play"
        # wide enough for the longest page name in bold (the checked style), in any launcher language
        bf = QFont(QApplication.font())            # the style sheet's nav font: 10.5 pt, 600 when checked
        bf.setPointSizeF(10.5)
        bf.setWeight(QFont.DemiBold)
        fm = QFontMetricsF(bf)
        need = max(fm.horizontalAdvance(b.text().replace("&&", "&")) for b in self.nav.values()) + 2 * 12 + 2 + 2 * 12 + 10
        side.setFixedWidth(max(236, int(need)))
        sl.addStretch(1)
        # experimental features (texture dump / packs): hidden unless asked for
        self.experimental = bool(self.s.get("experimental"))
        ex = QCheckBox(tr("l.experimental"))
        ex.setChecked(self.experimental)
        ex.setFocusPolicy(Qt.NoFocus)

        def toggled(on):
            self.s["experimental"] = on
            QTimer.singleShot(0, self.rebuild)
        ex.toggled.connect(toggled)
        sl.addWidget(ex)
        sl.addSpacing(10)
        cap = QLabel(tr("l.uilang"))
        cap.setObjectName("sideCaption")
        sl.addWidget(cap)
        cb = QComboBox()
        codes = list(i18n.LANGS)
        cb.addItems(["%s (%s)" % (i18n.LANGS[k], k) for k in codes])
        cb.setCurrentIndex(codes.index(i18n.current()) if i18n.current() in codes else 0)

        def chosen(i):
            code = codes[i]
            if code != i18n.current():
                i18n.set_language(code)
                QTimer.singleShot(0, self.rebuild)
        cb.activated.connect(chosen)
        sl.addWidget(cb)
        sl.addSpacing(10)
        bl = QLabel(VERSION_TAG)
        bl.setObjectName("build")
        sl.addWidget(bl)
        grid.addWidget(side, 0, 0, 2, 1)

        self.stack = QStackedWidget()
        grid.addWidget(self.stack, 0, 1)
        grid.setColumnStretch(1, 1)
        grid.setRowStretch(0, 1)
        self.pages, self.page_areas = {}, {}
        self.patch_ui = []
        self._build_play(self.page_frame("play", tr("nav.play")))
        self._build_gfx(self.page_frame("gfx", tr("nav.gfx")))
        self._build_controls(self.page_frame("controls", tr("nav.controls")))
        self._build_mods(self.page_frame("mods", tr("nav.mods")))
        self._build_cut(self.page_frame("cut", tr("nav.cut")))
        self._build_log(self.page_frame("log", tr("nav.log")))
        grid.addWidget(self._build_bar(), 1, 1)
        self.setCentralWidget(root)
        self.show_page(self.page)
        self.play_btn.setFocus()
        self.say(tr("s.nodata") if not self.entries() else tr("s.pickgame"), "info")
        self._refresh()
        if self.proc and self.proc.poll() is None:
            self.play_btn.setEnabled(False)
            self.stop_btn.show()
            if self.starting:
                self._starting(True)
        e = self.cur_entry()
        v = GL.version_of(current_sha1()) if e is not None and e.get("sha1") else None
        if e is not None and e.get("sha1") and not (v and v["supported"]):
            self.say(tr("l.unsupported", self.game_name(e)), "bad")
            self.play_btn.setEnabled(False)
        elif data_ready() and (not check or not os.path.isfile(bin_of_cue(self.s.get("disc") or ""))):
            man = X.read_manifest(data_dir())
            self.say(tr("s.dataok", data_dir(), len(man["files"]) if man else 0), "ok")
        elif check and self.s.get("disc"):
            if os.path.isfile(bin_of_cue(self.s["disc"])):
                self.after(200, self.check_disc)
            else:
                self.say(tr("s.noimage", self.s["disc"]), "bad")

    def _build_bar(self):
        """Bottom bar under every page: the options summary, Copy log, Close the game (while running) and Play."""
        bar = QFrame()
        bar.setObjectName("bar")
        v = QVBoxLayout(bar)
        v.setContentsMargins(28, 12, 28, 14)
        v.setSpacing(10)
        # shown from Play until the game prints "[port2] window ..." (port2/runtime/sdl.c) or exits
        lb = QLabel(tr("l.starting"))
        lb.setObjectName("accentText")
        self.start_bar = QProgressBar()
        self.start_bar.setTextVisible(False)
        self.start_bar.setFixedWidth(240)
        self.start_box, _ = hbox(lb, self.start_bar, None, spacing=10)
        self.start_box.hide()
        v.addWidget(self.start_box)
        self.summary = QLabel()
        self.summary.setObjectName("summary")
        self.summary.setMinimumWidth(60)
        self.summary.setSizePolicy(QSizePolicy.Ignored, QSizePolicy.Preferred)
        copy = QPushButton(tr("b.copylog"))
        copy.clicked.connect(self.copy_log)
        self.stop_btn = QPushButton(tr("b.stop"))
        self.stop_btn.clicked.connect(self.stop)
        self.stop_btn.hide()
        self.play_btn = QPushButton(tr("b.play"))
        self.play_btn.setObjectName("primary")
        self.play_btn.setMinimumWidth(150)
        self.play_btn.setCursor(Qt.PointingHandCursor)
        self.play_btn.clicked.connect(self.play_normal)
        self.play_btn.setDefault(True)
        row, rl = hbox(self.summary, copy, self.stop_btn, self.play_btn, spacing=8)
        rl.setStretch(0, 1)
        v.addWidget(row)
        return bar

    def _refresh(self):
        """Summary line ("OpenGL · 2x window · 3x internal · 16:9 · 50 Hz") and the internal resolution enabled for OpenGL only."""
        s = self.s
        gl = s.get("renderer") != "soft"
        parts = ["OpenGL" if gl else "Software",
                 tr("sum.full") if s.get("fullscreen") else tr("sum.window", s.get("scale", 2))]
        if gl and s.get("internal_res") in (2, 3, 4):
            parts.append(tr("sum.ires", s.get("internal_res")))
        a = s.get("aspect")
        parts.append({"square": "1:1 px", "16:9": "16:9", "21:9": "21:9", "32:9": "32:9", "auto": "auto"}.get(a, "4:3"))
        parts.append("60 Hz" if s.get("hz60") or not has("hz") else "50 Hz")
        try:
            self.summary.setText("  ·  ".join(parts))
            for b in getattr(self, "ires_btns", []):
                b.setEnabled(gl)
            if hasattr(self, "ires_note"):
                self.ires_note.setVisible(not gl)
        except (RuntimeError, AttributeError):
            pass

    def _build_play(self, f):
        c = self.card(f, spacing=8)
        t = QLabel(tr("c.library"))          # (Add game... is the last card of the library)
        t.setObjectName("cardTitle")
        c.addWidget(t)
        c.addWidget(wrap_label(tr("l.library"), "muted"))
        c.addWidget(self._library_view())
        self.dot = QLabel()
        self.dot.setFixedSize(10, 10)
        self.status_lbl = wrap_label("", sel=True)
        st, sl = hbox(spacing=10, margins=(0, 6, 0, 0))
        sl.addWidget(self.dot, 0, Qt.AlignTop)
        sl.addSpacing(0)
        sl.addWidget(self.status_lbl, 1)
        self.dot.setContentsMargins(0, 0, 0, 0)
        sl.setAlignment(self.dot, Qt.AlignTop)
        self.dot_wrap = st
        c.addWidget(st)
        # shown only while the disc is extracted into data/ (check_disc): a message and a bar under the status
        self.ext_box = QWidget()
        el = QVBoxLayout(self.ext_box)
        el.setContentsMargins(0, 6, 0, 0)
        el.setSpacing(6)
        el.addWidget(wrap_label(tr("l.extracting"), "busyText"))
        self.ext_bar = QProgressBar()
        self.ext_bar.setRange(0, 100)
        self.ext_bar.setTextVisible(False)
        el.addWidget(self.ext_bar)
        self.ext_box.hide()
        c.addWidget(self.ext_box)

        e = self.cur_entry()
        v = GL.version_of(current_sha1()) if e is None or e.get("sha1") else None
        if e is not None and e.get("sha1") and not (v and v["supported"]):
            # recognised, but the port does not run it (JP retail, PAL Rev 0): what it is, no options
            c = self.card(f, tr("c.options"))
            c.addWidget(wrap_label(tr("l.unsupported", self.game_name(e)), "warn"))
            c.addWidget(wrap_label(tr("l.unsupported2"), "muted"))
            self._build_saves(f)
            f.addStretch(1)
            return

        c = self.card(f, tr("c.options") if not has("lang") else None)
        g = QGridLayout()
        g.setHorizontalSpacing(28)
        g.setVerticalSpacing(18)
        g.setColumnStretch(0, 1)
        g.setColumnStretch(1, 1)
        c.addLayout(g)

        def cell(row, col, title):
            box = QWidget()
            lay = QVBoxLayout(box)
            lay.setContentsMargins(0, 0, 0, 0)
            lay.setSpacing(6)
            t = QLabel(title)
            t.setObjectName("cellTitle")
            lay.addWidget(t)
            g.addWidget(box, row, col, Qt.AlignTop)
            return lay
        lay = cell(0, 0, tr("c.game"))
        if not has("lang"):
            # one language on the disc (USA: English): nothing to choose
            lb = QLabel()
            lb.setPixmap(flag_pixmap(9 if (v or {}).get("key") == "usa" else 0))
            row, rl = hbox(lb, QLabel(tr("l.onlylang", "English")), None, spacing=8)
            lay.addWidget(row)
        else:
            # game language (the boot menu's MAINn.EXE): a combobox with the flags
            default_lang = UI_TO_GAME_LANG.get(i18n.current(), 0)     # the launcher's language, English otherwise
            cur = self.v("lang", int, default_lang)
            ids = [n for n, _name in GAME_LANGS]
            self.lang_cb = QComboBox()
            self.lang_cb.setIconSize(QSize(24, 16))
            for n, name in GAME_LANGS:
                self.lang_cb.addItem(QIcon(flag_pixmap(n)), name)
            self.lang_cb.setCurrentIndex(ids.index(cur) if cur in ids else 0)
            self.lang_cb.activated.connect(lambda i: (self.setv("lang", ids[i]), self._sync_patch_choice()))
            lay.addWidget(self.lang_cb)
        # quick options under the language, in the same column (the text patch card on the right is taller)
        lay.addSpacing(14)
        t = QLabel(tr("c.quick"))
        t.setObjectName("cellTitle")
        lay.addWidget(t)
        lay.addWidget(self.check("skipintro", tr("l.skipintro"), True))
        lay.addWidget(self.check("autostart", tr("l.autostart2"), False))
        if has("lang"):
            # the Select sub-screen's labels (--subscreen-labels, Spanish only): item=..,event=..,status=..,exit=..,map=..,use=..|..,view=..,sort=..
            t = QLabel(tr("l.sublabels"))
            t.setToolTip(tr("l.sublabelshint"))
            lay.addWidget(t)
            e = QLineEdit(self.v("subscreen_labels", str, ""))
            e.setPlaceholderText("item=Objetos,use=Usar|objeto")
            e.setToolTip(tr("l.sublabelshint"))
            e.textChanged.connect(lambda t: self.setv("subscreen_labels", t.strip()))
            self.edits["subscreen_labels"] = e
            lay.addWidget(e)
        if has("patch"):
            # text patch: none / the bundled one / another file (keys patch_on + patch; no path shown unless "other")
            self._patch_widget(cell(0, 1, tr("c.patch")))
        else:
            # the hook for per-version retranslations (game_library.VERSIONS "text_patches"): none for this version yet
            lay = cell(0, 1, tr("c.patch"))
            lay.addWidget(wrap_label(tr("l.nopatches", (v or {}).get("region", "")), "muted"))
            lay.addWidget(wrap_label(tr("l.ntscopts"), "muted"))
        self._build_saves(f)
        # (the speed, 50 / 60 Hz, is on the Graphics and performance page, next to the frame rate)
        f.addStretch(1)


    def _build_saves(self, f):
        c = self.card(f, tr("c.saves"), spacing=6)
        c.addWidget(wrap_label(tr("l.savedir"), "muted"))
        c.addWidget(self.browse_row("savedir", self.pick_savedir))
        bi, be, bed = QPushButton(tr("b.saveimp")), QPushButton(tr("b.saveexp")), QPushButton(tr("b.saveedit"))
        bi.clicked.connect(self.import_save)
        be.clicked.connect(self.export_save)
        bed.clicked.connect(self.edit_save)
        row, _ = hbox(bi, be, bed, None, margins=(0, 4, 0, 0))
        c.addWidget(row)

    # ---- the game library (tools/game_library.py)
    def entries(self):
        g = self.s.get("games")
        if not isinstance(g, list):
            g = self.s["games"] = []
        return g

    def cur_entry(self):
        es = self.entries()
        for e in es:
            if e.get("serial") == self.s.get("game"):
                return e
        return es[0] if es else None

    def _apply_entry(self):
        """CURRENT (data folder, version) and the "disc" setting from the selected entry."""
        e = self.cur_entry()
        if e is None:                         # an empty library (Add game... to start)
            CURRENT.update(data="data", sha1="")
            self.s["disc"] = ""
            return
        self.s["game"] = e.get("serial", "")
        CURRENT.update(data=e.get("data") or "data", sha1=e.get("sha1") or "")
        self.s["disc"] = e.get("image") or ""

    def game_name(self, e):
        """"Tombi! (PAL)", "Tomba! (USA)", "Ore! Tomba (Japan)": the card's title."""
        v = GL.version_of(e.get("sha1"))
        if not v:
            return os.path.basename(bin_of_cue(e.get("image") or "")) or "?"
        return "%s (%s)" % (v["title"], tr("rg." + v["region"]))

    def game_sub(self, e):
        """"SCES-01331": the serial (the disc's languages are drawn as flags beside it: game_flags)."""
        return e.get("serial") or tr("lib.unchecked")

    def game_flags(self, e):
        """the flag_pixmap indices of the disc's languages (game_library.DISC_FLAGS)"""
        return GL.DISC_FLAGS.get(e.get("serial"), [])

    def game_status(self, e):
        """(text, level) of a library entry: ready / to extract / image missing / not supported yet."""
        v = GL.version_of(e.get("sha1"))
        img = bin_of_cue(e.get("image") or "")
        if not e.get("sha1"):
            return tr("lib.unchecked"), "busy"
        if not (v and v["supported"]):
            return tr("lib.unsupported"), "bad"
        if GL.data_complete(os.path.join(app_dir(), e.get("data") or "data")) == e["sha1"]:
            return tr("lib.ready"), "ok"
        if img and os.path.isfile(img):
            return tr("lib.toextract"), "info"
        return tr("lib.noimage"), "bad"

    def game_art(self, e):
        """{"logo": QPixmap, "title": QPixmap} rebuilt from the player's disc the first time (library/<serial>/), {}
        when there is nothing to build it from: the card then shows the name."""
        serial, sha = e.get("serial"), e.get("sha1")
        if not serial or not GL.version_of(sha):
            return {}
        d = art_cache(serial)
        files = {k: os.path.join(d, k + ".png") for k in GL.ART}
        if not all(os.path.isfile(p) for p in files.values()) and (serial, sha) not in self._art_tried:
            self._art_tried.add((serial, sha))
            data = os.path.join(app_dir(), e.get("data") or "data")
            img = bin_of_cue(e.get("image") or "")
            src = None
            try:
                if GL.data_complete(data) == sha:
                    src = GL.DirSource(data)
                elif img and os.path.isfile(img) and not DA.is_archive(img):
                    src = GL.ImageSource(img)
                if src is not None:
                    GL.make_art(src, sha, d)
            except (OSError, ValueError, KeyError, struct.error):
                pass
            finally:
                if isinstance(src, GL.ImageSource):
                    src.close()
        out = {}
        for k, p in files.items():
            if os.path.isfile(p):
                pm = QPixmap(p)
                if not pm.isNull():
                    out[k] = pm
        return out

    def _tile_data(self, e):
        text, level = self.game_status(e)
        return dict(serial=e.get("serial", ""), title=self.game_name(e), short=(GL.version_of(e.get("sha1")) or {}).get("title") or self.game_name(e),
                    sub=self.game_sub(e), flags=self.game_flags(e), status=text, level=level, art=self.game_art(e),
                    selected=e is self.cur_entry())

    def _library_view(self):
        """The game cards in the chosen look (setting "library_view": grid, list or banner; TOMBI_LIBRARY_VIEW forces one)."""
        mode = os.environ.get("TOMBI_LIBRARY_VIEW") or self.s.get("library_view") or LIBRARY_VIEW
        if mode not in ("grid", "list", "banner"):
            mode = LIBRARY_VIEW
        es = sorted(self.entries(), key=GL.sort_key)
        self.tiles = []
        box = QWidget()
        if mode == "list":
            lay = QVBoxLayout(box)
            lay.setContentsMargins(0, 4, 0, 4)
            lay.setSpacing(8)
            for e in es:
                lay.addWidget(self._tile(e, "list"))
            lay.addWidget(self._add_tile("list"))
            return box
        if mode == "banner":
            lay = QVBoxLayout(box)
            lay.setContentsMargins(0, 4, 0, 4)
            lay.setSpacing(10)
            cur = self.cur_entry()
            if cur is not None:
                b = GameTile(self, self._tile_data(cur), "banner")
                lay.addWidget(b)
            row, rl = hbox(spacing=10)
            for e in es:
                rl.addWidget(self._tile(e, "chip"))
            rl.addWidget(self._add_tile("chip"))
            rl.addStretch(1)
            lay.addWidget(row)
            return box
        return TileGrid([self._tile(e, "grid") for e in es] + [self._add_tile("grid")])

    def _tile(self, e, mode):
        t = GameTile(self, self._tile_data(e), mode)
        t.clicked.connect(lambda _=False, s=e.get("serial", ""): self.select_game(s))
        t.setToolTip("%s\n%s\n%s" % (self.game_name(e), e.get("image") or "", os.path.join(app_dir(), e.get("data") or "data")))
        self.tiles.append(t)
        return t

    def _add_tile(self, mode):
        t = GameTile(self, dict(add=True, title=tr("b.addgame"), sub=tr("l.addgame")), mode)
        t.clicked.connect(self.add_game)
        return t

    def select_game(self, serial):
        if serial == self.s.get("game") and self.cur_entry() is not None and self.cur_entry().get("serial") == serial:
            return
        if self.extracting:
            self.warning(tr("s.busyextract"))
            return
        self.collect()
        self.s["game"] = serial
        self._apply_entry()
        save_settings(self.s)
        self.rebuild()

    def add_game(self):
        p = self._open_file(tr("b.addgame"), "Disc image (*.bin *.cue *.iso *.img *.chd *.zip *.7z);;All (*)")
        if p:
            self.add_image(p)

    def add_image(self, p, wait=False):
        """Identifies the image (a thread; wait=True: here, for the tests), adds it to the library, selects it and
        extracts it (check_disc)."""
        if self.extracting:
            self.warning(tr("s.busyextract"))
            return
        self.say(tr("s.checking"), "busy")

        def work():
            try:
                img = self.disc_image(p)
                info = GL.identify_image(img)
            except DA.ArchiveError as e:
                self.drop_unpacked(p)
                self.later(self.archive_error, p, e)
                return
            except (OSError, ValueError, struct.error) as e:
                self.drop_unpacked(p)
                self.later(self.say, tr("s.notiso", "%s (%s)" % (os.path.basename(p), e)), "bad")
                return
            if not (info["version"] and info["version"]["supported"]):
                self.drop_unpacked(p)        # (nothing to extract: the temporary image goes now)
            self.later(self._added, p, info)
        if wait:
            work()
            self.app_events()
        else:
            threading.Thread(target=work, daemon=True).start()

    def app_events(self):
        QApplication.processEvents()

    # ---- a disc inside a .zip / .7z (tools/disc_archive.py): unpacked once next to the launcher (.unpack/), used by
    # the identification and the extraction that follows, then deleted
    def disc_image(self, p, progress=True):
        """The image to read for the picked file p: p itself (a .cue: its .bin), or for an archive the image inside it,
        unpacked into .unpack/ (once: kept until drop_unpacked). Raises DA.ArchiveError (a .rar, a file that is no disc
        image, an archive without one ...)."""
        k = DA.kind(p)
        if k == "notimage":
            raise DA.ArchiveError("notimage", "%s is not a disc image" % os.path.basename(p))
        if k not in ("zip", "7z", "rar"):
            return bin_of_cue(p)
        if not hasattr(self, "_unpacked"):
            self._unpacked = {}
        t = self._unpacked.get(p)
        if t and os.path.isfile(t):
            return t
        last = [-1]

        def prog(done, total):
            pct = done * 100 // max(1, total)
            if progress and pct != last[0]:
                last[0] = pct
                self.later(self.say, tr("s.unpacking", os.path.basename(p), pct), "busy")
        t = DA.extract_image(p, os.path.join(app_dir(), ".unpack"), prog)
        self._unpacked[p] = t
        return t

    def drop_unpacked(self, p):
        """deletes the image disc_image unpacked from the archive p (and .unpack/ when empty)"""
        t = getattr(self, "_unpacked", {}).pop(p, None)
        if t:
            try:
                os.remove(t)
            except OSError:
                pass
        try:
            os.rmdir(os.path.join(app_dir(), ".unpack"))
        except OSError:
            pass

    def archive_error(self, p, e):
        """the message of a DA.ArchiveError, in the launcher's language"""
        nm = os.path.basename(p)
        if e.code == "files":
            lo = getattr(e, "loose", None) or {}
            v = GL.version_of(lo.get("sha1"))
            what = ("%s (%s)" % (v["title"], tr("rg." + v["region"]))) if v else (lo.get("serial") or "?")
            msg = tr("s.a.files", nm, what)
        elif e.code == "cue":
            msg = tr("s.a.cue", nm, getattr(e, "track", "?"))
        elif e.code == "space":
            msg = tr("s.a.space", nm, getattr(e, "need", 0) >> 20, getattr(e, "free", 0) >> 20)
        elif e.code == "bad":
            msg = tr("s.a.bad", nm, str(e))
        elif e.code in ("notimage", "rar", "noimage", "nested"):
            msg = tr("s.a." + e.code, nm)
        else:
            msg = str(e)
        self.say(msg, "bad")
        if e.code != "cancelled":
            self.error(msg)

    def _added(self, p, info):
        if not info["version"]:
            msg = tr("s.notgame", os.path.basename(p), info["serial"] or "?", info["name"] or tr("lib.unknown"))
            self.say(msg, "bad")
            self.error(msg)
            return
        self.collect()
        for e in list(self.entries()):          # an older launcher's disc that was never identified: this one replaces it
            if not e.get("serial") and bin_of_cue(e.get("image") or "") == bin_of_cue(p):
                self.entries().remove(e)
        e = GL.add_entry(self.entries(), info, p)
        self.s["games"] = sorted(self.entries(), key=GL.sort_key)
        if not self.s.get("savedir"):
            self.s["savedir"] = os.path.dirname(os.path.abspath(p))
        self.s["game"] = e["serial"]
        self._apply_entry()
        save_settings(self.s)
        self.rebuild()               # checks the disc and extracts it (_build -> check_disc)

    def remove_game(self, serial=None):
        """Remove from the library... (a card's menu; serial None: the selected game), and its data folder if asked."""
        e = self.cur_entry() if serial is None else next((o for o in self.entries() if o.get("serial") == serial), None)
        if e is None:
            return
        if self.extracting or (self.proc and self.proc.poll() is None):
            self.warning(tr("s.busyextract") if self.extracting else tr("s.running"))
            return
        was_current = e is self.cur_entry()
        d = os.path.join(app_dir(), e.get("data") or "data")
        if not self.ask(tr("q.removegame", self.game_name(e))):
            return
        self.collect()
        if os.path.isdir(d) and self.ask(tr("q.removedata", d)):
            shutil.rmtree(d, ignore_errors=True)
        shutil.rmtree(art_cache(e.get("serial")), ignore_errors=True)
        self.entries().remove(e)
        if was_current:
            self.s["game"] = self.entries()[0].get("serial", "") if self.entries() else ""
        self._apply_entry()
        save_settings(self.s)
        self.rebuild()

    def _identified(self, e, info):
        """check_disc found what the selected entry's image is: the entry follows (serial, version, data folder)."""
        if e not in self.entries() or not info["version"]:
            return
        es = self.entries()
        for o in es:
            if o is not e and o.get("serial") == info["serial"]:
                es.remove(e)                   # the same disc twice: keep the older entry
                o["image"] = e.get("image") or o.get("image", "")
                e = o
                break
        changed = (e.get("serial"), e.get("sha1")) != (info["serial"], info["game_sha1"])
        e["serial"], e["sha1"] = info["serial"], info["game_sha1"]
        want = GL.data_name(info["serial"], [o for o in es if o is not e])
        if e.get("data") != want and not GL.data_complete(os.path.join(app_dir(), e.get("data") or "data")):
            e["data"] = want
            changed = True
        if changed:
            self.s["game"] = e["serial"]
            self._apply_entry()
            save_settings(self.s)

    def _patch_mode(self):
        """none / bundled / translation / custom from the saved patch_on + patch (an old saved path to the bundled file ->
        bundled; the installed translation.tombipatch.json / .tpatch -> translation)."""
        bp = bundled_patch()
        cur = (self.s.get("patch") or "").strip()
        if not self.s.get("patch_on"):
            return "none"
        if cur and cur in [f for f in translation_files() if f]:
            return "translation"
        if not cur or (bp and os.path.basename(cur) == os.path.basename(bp) and os.path.dirname(os.path.abspath(cur)).endswith("patches")):
            return "bundled" if bp else "none"
        return "custom"

    def _translation_auto(self):
        """A translation file that appeared (or changed) next to the launcher is selected: it wins over the bundled
        improved translation until the player picks something else (that choice holds until the file changes again)."""
        j, t = translation_files()
        f = j or t
        if not f:
            if self._patch_mode() == "translation" or (self.s.get("patch") or "").endswith((TRANSLATION_JSON, TRANSLATION_TPATCH)):
                self.s["patch_on"] = bool(bundled_patch()) and self.s.get("lang") == 5
                self.s["patch"] = bundled_patch() or ""
            return
        st = os.stat(f)
        stamp = [os.path.basename(f), st.st_size, int(st.st_mtime)]
        if self.s.get("translation_seen") != stamp:
            self.s["translation_seen"] = stamp
            self.s["patch"], self.s["patch_on"] = f, True
        elif self._patch_mode() == "translation" and self.s.get("patch") != f:
            self.s["patch"] = f

    def import_translation(self):
        """Import translation...: the file is copied next to the launcher under the fixed name tombi2 looks for, and chosen."""
        p = self._open_file(tr("b.importtr"), "Tombi translation (*.tombipatch.json *.tpatch);;All (*)")
        if not p:
            return
        try:
            f = import_translation(p)
        except (OSError, ValueError) as e:
            self.error(tr("s.trbad", e))
            return
        st = os.stat(f)
        self.s["translation_seen"] = [os.path.basename(f), st.st_size, int(st.st_mtime)]
        self.s["patch"], self.s["patch_on"] = f, True
        save_settings(self.s)
        self._sync_patch_choice()
        self.say(tr("s.trimported", translation_title() or os.path.basename(f), app_dir()), "ok")

    def remove_translation(self):
        if QMessageBox.question(self, tr("b.removetr"), tr("q.removetr")) != QMessageBox.Yes:
            return
        try:
            remove_translation()
        except OSError as e:
            self.error(str(e))
            return
        self._translation_auto()
        save_settings(self.s)
        self._sync_patch_choice()

    def _patch_widget(self, lay):
        """Combobox None / Improved translation (bundled) / Your translation / Other file...; the path + Browse only for
        "other"; Import translation... / Remove and the line saying which translation is active."""
        self.v("patch_on", bool, False)
        self.v("patch", str, "")
        if self._patch_mode() == "bundled":        # migrate: the bundled file wherever this exe now lives
            self.s["patch"] = bundled_patch()
        self._translation_auto()
        modes = ["none"] + (["bundled"] if bundled_patch() else []) + ["translation", "custom"]
        cb = QComboBox()
        lay.addWidget(cb)
        e = QLineEdit(self.s.get("patch") or "")
        e.textChanged.connect(lambda t: self.s.__setitem__("patch", t))
        b = QPushButton(tr("b.browse"))
        b.clicked.connect(self.pick_patch)
        row, rl = hbox(e, b, margins=(0, 4, 0, 0))
        rl.setStretch(0, 1)
        lay.addWidget(row)
        hint = wrap_label(tr("l.patchhint"), "muted")
        lay.addWidget(hint)
        bi, br = QPushButton(tr("b.importtr")), QPushButton(tr("b.removetr"))
        bi.clicked.connect(self.import_translation)
        br.clicked.connect(self.remove_translation)
        trow, tl = hbox(bi, br, margins=(0, 4, 0, 0))
        tl.addStretch(1)
        lay.addWidget(trow)
        active = wrap_label("", "muted")
        lay.addWidget(active)

        def shown_modes():
            spanish = self.s.get("lang") == 5
            have = any(translation_files())
            return [x for x in modes if (x != "bundled" or spanish) and (x != "translation" or have)]

        def chosen(i):
            m = shown_modes()[i]
            if m == "none":
                self.s["patch_on"] = False
            elif m == "bundled":
                self.s["patch"] = bundled_patch() or ""
                self.s["patch_on"] = True
            elif m == "translation":
                j, t = translation_files()
                self.s["patch"], self.s["patch_on"] = j or t, True
            elif self._patch_mode() != "custom":
                old = self.s.get("patch"), self.s.get("patch_on")
                self.pick_patch()
                if self._patch_mode() != "custom":      # cancelled: back to what it was
                    self.s["patch"], self.s["patch_on"] = old
            self._sync_patch_choice()
        cb.activated.connect(chosen)
        self.patch_ui.append((cb, modes, row, hint, e, br, active))
        self._sync_patch_choice()

    def _sync_patch_choice(self):
        """Every patch combobox shown from the patch_on / patch settings."""
        m = self._patch_mode()
        spanish = self.s.get("lang") == 5
        have = any(translation_files())
        title = translation_title()
        for cb, modes, row, hint, e, br, active in getattr(self, "patch_ui", []):
            try:
                # the bundled Spanish translation only for the Spanish game (it does not touch the other languages)
                shown = [x for x in modes if (x != "bundled" or spanish) and (x != "translation" or have)]
                cb.blockSignals(True)
                cb.clear()
                cb.addItems([tr("p." + x) for x in shown])
                cb.setCurrentIndex(shown.index(m) if m in shown else 0)
                cb.blockSignals(False)
                if e.text() != (self.s.get("patch") or ""):
                    e.blockSignals(True)
                    e.setText(self.s.get("patch") or "")
                    e.blockSignals(False)
                row.setVisible(m == "custom")
                hint.setVisible(m == "none" and spanish and "bundled" in modes and not have)
                br.setVisible(have)
                if m == "translation":
                    active.setText(tr("l.tractive", title or "?", app_dir()))
                elif have:
                    active.setText(tr("l.trinstalled", title or "?"))
                else:
                    active.setText(tr("l.trnone"))
            except RuntimeError:
                pass

    def clear_patch(self):
        self.s["patch_on"] = False
        self._sync_patch_choice()

    def custom_buttons(self):
        """The "Customise buttons" dialog: per position a disc and a letter colour, the symbols, presets, a preview
        drawn as the game draws it (tools/prompt_custom.py = port2/shims/prompts.c)."""
        sym, cols = PC.from_arg(self.v("prompt_colors", str, PC.DEFAULT))
        st = {"sym": sym, "cols": list(cols)}
        d = QDialog(self)
        d.setWindowTitle(tr("pc.title"))
        es = tr("pc.ok") == "Usar"
        top = QGridLayout(d)
        top.setContentsMargins(20, 18, 20, 18)
        top.setHorizontalSpacing(10)
        top.setVerticalSpacing(8)
        pcb = QComboBox()
        pcb.addItems([p[1] if es else p[2] for p in PC.PRESETS])
        pcb.setCurrentIndex(-1)
        scb = QComboBox()
        scb.addItems([tr("pc.sym." + k) for k in PC.SYMS])
        top.addWidget(QLabel(tr("pc.preset")), 0, 0)
        top.addWidget(pcb, 0, 1, 1, 2)
        top.addWidget(QLabel(tr("pc.sym")), 1, 0)
        top.addWidget(scb, 1, 1, 1, 2)
        preview = GlyphPreview(st)
        top.addWidget(preview, 0, 3, 6, 1, Qt.AlignCenter)
        top.setColumnMinimumWidth(3, preview.width() + 16)
        swatches = []

        def redraw():
            for i, (bd, bl) in enumerate(swatches):
                bd.set_color(st["cols"][i][0])
                bl.set_color(st["cols"][i][1])
            scb.blockSignals(True)
            scb.setCurrentIndex(PC.SYMS.index(st["sym"]))
            scb.blockSignals(False)
            preview.update()

        def pick(i, j):
            c = QColorDialog.getColor(QColor("#%06x" % st["cols"][i][j]), d)
            if c.isValid():
                cur = list(st["cols"][i])
                cur[j] = int(c.name()[1:], 16)
                st["cols"][i] = tuple(cur)
                redraw()
        names = tr("pc.pos").split("|")
        for i in range(4):
            top.addWidget(QLabel(names[i]), 2 + i, 0)
            bd, bl = Swatch(tr("pc.disc")), Swatch(tr("pc.letter"))
            bd.setObjectName("swatch")
            bl.setObjectName("swatch")
            bd.clicked.connect(lambda _=False, i=i: pick(i, 0))
            bl.clicked.connect(lambda _=False, i=i: pick(i, 1))
            top.addWidget(bd, 2 + i, 1)
            top.addWidget(bl, 2 + i, 2)
            swatches.append((bd, bl))

        def on_preset(i):
            p = PC.PRESETS[i]
            st["sym"], st["cols"] = p[3], list(p[4])
            redraw()

        def on_sym(i):
            st["sym"] = PC.SYMS[i]
            redraw()
        pcb.activated.connect(on_preset)
        scb.activated.connect(on_sym)

        def ok():
            self.s["prompt_colors"] = PC.to_arg(st["sym"], st["cols"])
            self.v("prompts_style", str, "auto")
            self.s["prompts_style"] = "custom"
            self._prompts_sync()
            d.accept()
        okb, cancel = QPushButton(tr("pc.ok")), QPushButton(tr("pc.cancel"))
        okb.setObjectName("primary")
        okb.clicked.connect(ok)
        cancel.clicked.connect(d.reject)
        row, _ = hbox(None, okb, cancel, margins=(0, 10, 0, 0))
        top.addWidget(row, 6, 0, 1, 4)
        redraw()
        d.exec()

    def _build_gfx(self, f):
        g = QGridLayout()
        g.setHorizontalSpacing(14)
        g.setVerticalSpacing(14)
        g.setColumnStretch(0, 1)
        g.setColumnStretch(1, 1)
        f.addLayout(g)
        c = self.card(None, tr("c.renderer"))
        g.addWidget(c.frame, 0, 0)
        self.radios(c, "renderer", str, "gl", [(tr("r.gl"), "gl"), (tr("r.soft"), "soft")])
        c.addStretch(1)
        c = self.card(None, tr("c.window"))
        g.addWidget(c.frame, 0, 1)
        c.addWidget(self.segmented("scale", int, 2, [("%dx" % n, n) for n in (1, 2, 3)], min_w=56))
        c.addSpacing(4)
        c.addWidget(self.check("fullscreen", tr("l.fullscreen"), False))
        # --show-fps: the on-screen counter (--log-fps 2 always logs the rate)
        c.addWidget(self.check("show_fps", tr("l.showfps"), False))
        c.addStretch(1)
        c = self.card(None, tr("c.aspect"))
        g.addWidget(c.frame, 1, 0)
        self.radios(c, "aspect", str, "tv", [(tr("a.tv"), "tv"), (tr("a.sq"), "square"), (tr("a.w16"), "16:9"),
                                            (tr("a.w21"), "21:9"), (tr("a.w32"), "32:9"), (tr("a.auto"), "auto")])
        c.addStretch(1)
        c = self.card(None, tr("c.ires"))     # --internal-res: 3D drawn at N x (docs/PORT.md "HD"), OpenGL only
        g.addWidget(c.frame, 1, 1)
        self.ires_btns = self.radios(c, "internal_res", int, 1, [(tr("r.ires1") if n == 1 else "%dx" % n, n) for n in (1, 2, 3, 4)])
        self.ires_note = wrap_label(tr("l.iresoff"), "muted")
        c.addWidget(self.ires_note)
        c.addStretch(1)
        # performance: the game's speed (--hz 60, the hz60 mod) and the frame rate (--fps: presents per second,
        # interpolated, both renderers) in one card, so it is clear which is which
        c = self.card(None, tr("c.perf"))
        g.addWidget(c.frame, 2, 0, 1, 2)
        t = QLabel(tr("c.speed")); t.setObjectName("cellTitle"); c.addWidget(t)
        if has("hz"):
            c.addWidget(self.segmented("hz60", bool, True, [("50 Hz (PAL)", False), ("60 Hz", True)], min_w=110))
            c.addWidget(wrap_label(tr("l.speedhint"), "muted"))
        else:                                   # an NTSC game: always 60 Hz
            c.addWidget(wrap_label(tr("l.speedntsc"), "muted"))
        c.addSpacing(10)
        t = QLabel(tr("c.fps")); t.setObjectName("cellTitle"); c.addWidget(t)
        default = "auto" if self.s.get("fps60") and self.experimental else "off"
        c.addWidget(self.segmented("fps", str, default, [(tr("fps.off"), "off"), ("Auto", "auto"), ("60", "60"), ("90", "90"),
                                                         ("120", "120"), ("144", "144"), (tr("fps.unl").split(" (")[0], "unlimited")]))
        c.addWidget(wrap_label(tr("fps.auto") + " · " + tr("fps.unl") + ". " + tr("l.fpshint"), "muted"))
        f.addStretch(1)

    def _build_controls(self, f):
        """Three cards: the button icons of the texts (--prompts, a combobox), the keyboard bindings (--keys, a table:
        click a key and press the new one) and the controller bindings (--pad-map: the PS1 pad photo with a leader line
        from each button to its controller button, and a table of comboboxes; clicking a button on the photo selects its
        row), with the DualShock switch and the vibration intensity."""
        # ---- button icons
        c = self.card(f, tr("c.icons"))
        c.addWidget(QLabel(tr("l.prompts")))
        cur = self.v("prompts_style", str, "auto")
        if cur not in [k for k, _ in PROMPT_STYLES]:
            self.s["prompts_style"] = "auto"            # the AYN Thor styles are Android only; an unknown value too
        self.prompt_cb = QComboBox()
        self.prompt_cb.addItems([tr(t) for _, t in PROMPT_STYLES])
        self.prompt_cb.setMinimumWidth(220)
        self.cust_btn = QPushButton(tr("b.custom"))
        self.cust_btn.clicked.connect(self.custom_buttons)
        self.prompt_cb.activated.connect(lambda i: (self.s.__setitem__("prompts_style", PROMPT_STYLES[i][0]), self._prompts_sync()))
        row, _ = hbox(self.prompt_cb, self.cust_btn, None)
        c.addWidget(row)
        self._prompts_sync()
        c.frame.setVisible(has("prompts"))      # the icons are redrawn in the PAL fonts (port2/shims/prompts.c)

        # ---- keyboard
        c = self.card(f, tr("c.keyboard"))
        c.addWidget(wrap_label(tr("kb.hint"), "muted"))
        keys = self.s.get("keys") if self.s.get("keys_on") else None
        keys = dict(M.DEFAULT_KEYS, **(keys or {}))
        self.key_vals, self.key_btns, self.key_names = {}, {}, {}
        self.key_capture = None
        g = QGridLayout()
        g.setHorizontalSpacing(10)
        g.setVerticalSpacing(6)
        g.setColumnStretch(1, 1)
        g.setColumnStretch(4, 1)
        g.setColumnMinimumWidth(2, 24)
        for i, b in enumerate(M.BUTTONS):
            r, col = i % 7, (i // 7) * 3
            self.key_vals[b] = keys.get(b) or ""
            nm = QLabel(tr("btn." + b))
            kb = KeyButton(self, b)
            kb.setMinimumWidth(150)
            g.addWidget(nm, r, col)
            g.addWidget(kb, r, col + 1)
            self.key_btns[b], self.key_names[b] = kb, nm
        c.addLayout(g)
        self.key_warn = wrap_label("", "warn")
        c.addWidget(self.key_warn)
        rb = QPushButton(tr("b.restore"))
        rb.clicked.connect(self._keys_restore)
        row, _ = hbox(None, rb)
        c.addWidget(row)
        self._keys_sync()

        # ---- controller
        c = self.card(f, tr("c.pad"))
        c.addWidget(wrap_label(tr("gp.hint"), "muted"))
        self.pad_capture = None              # the selected PS1 button (its row in the table below)
        self.pad_view = PadView(self)
        c.addWidget(self.pad_view)
        pm = dict(PAD_DEFAULTS, **(self.s.get("pad_map") or {}))
        labels = [self._src_label(k) for k, _, _ in PAD_SOURCES]
        self.pad_vals, self.pad_cbs, self.pad_names = {}, {}, {}
        g = QGridLayout()
        g.setHorizontalSpacing(10)
        g.setVerticalSpacing(6)
        g.setColumnStretch(1, 1)
        g.setColumnStretch(4, 1)
        g.setColumnMinimumWidth(2, 24)
        for i, b in enumerate(M.BUTTONS):
            r, col = i % 7, (i // 7) * 3
            val = self.pad_vals[b] = pm.get(b, "")
            nm = QLabel(tr("btn." + b))
            cb = QComboBox()
            cb.addItems(labels if val in PAD_SRC_KEYS else labels + [val])     # a hand-written source stays choosable
            cb.setMaxVisibleItems(16)
            cb.activated.connect(lambda _i, b=b, cb=cb: self._padmap_chosen(b, cb))
            cb.installEventFilter(self)
            cb._pad_button = b
            g.addWidget(nm, r, col)
            g.addWidget(cb, r, col + 1)
            self.pad_cbs[b], self.pad_names[b] = cb, nm
        c.addLayout(g)
        self.pad_warn = wrap_label("", "warn")
        c.addWidget(self.pad_warn)
        rb = QPushButton(tr("b.restore"))
        rb.clicked.connect(self._padmap_restore)
        row, _ = hbox(None, rb)
        c.addWidget(row)
        sep = QFrame()
        sep.setObjectName("sep")
        c.addWidget(sep)
        c.addWidget(self.check("dualshock", tr("l.dualshock"), False))
        # --rumble: vibration intensity (runtime/sdl.c), stored as the percent; shown as "Off" / "N %"
        rv = self.v("rumble", int, 100)
        rcb = QComboBox()
        rcb.addItems([tr("rumble.off") if n == 0 else "%d %%" % n for n in RUMBLE_STEPS])
        rcb.setCurrentIndex(RUMBLE_STEPS.index(rv) if rv in RUMBLE_STEPS else RUMBLE_STEPS.index(100))
        rcb.activated.connect(lambda i: self.s.__setitem__("rumble", RUMBLE_STEPS[i]))
        row, _ = hbox(QLabel(tr("l.rumble")), rcb, None)
        c.addWidget(row)
        self._padmap_sync()
        f.addStretch(1)

    def eventFilter(self, obj, e):
        if e.type() == QEvent.FocusIn and getattr(obj, "_pad_button", None):
            if self.pad_capture != obj._pad_button:
                self._pad_select(obj._pad_button, focus=False)
        return False

    def _prompts_sync(self):
        k = self.s.get("prompts_style")
        keys = [x for x, _ in PROMPT_STYLES]
        try:
            if k in keys:
                self.prompt_cb.setCurrentIndex(keys.index(k))
            self.cust_btn.setEnabled(k == "custom")
        except (RuntimeError, AttributeError):
            pass

    # ---- keyboard table
    def _key_text(self, b):
        k = self.key_vals[b].strip()
        if not k:
            return tr("pad.nokey")
        if b == "select" and k == M.DEFAULT_KEYS["select"]:
            return k + " / Right Shift"     # runtime/sdl.c keymap: the default select has a second key
        return k

    def _keys_sync(self):
        used = {}
        for b in M.BUTTONS:
            k = self.key_vals[b].strip().lower()
            if k:
                used.setdefault(k, []).append(b)
            btn = self.key_btns[b]
            btn.setText(tr("pad.press") + "…" if self.key_capture == b else self._key_text(b))
            btn.setChecked(self.key_capture == b)
        clash = [bs for bs in used.values() if len(bs) > 1]
        for b in M.BUTTONS:
            lb = self.key_names[b]
            lb.setObjectName("nameWarn" if any(b in bs for bs in clash) else "")
            lb.style().unpolish(lb)
            lb.style().polish(lb)
        self.key_warn.setText("\n".join(tr("kb.clash", self.key_vals[bs[0]].strip(), ", ".join(tr("btn." + x) for x in bs))
                                        for bs in clash))
        self.key_warn.setVisible(bool(clash))

    def _key_begin(self, b):
        if self.key_capture and self.key_capture != b:
            self._key_end(self.key_capture)
        self.key_capture = b
        self.key_btns[b].setFocus()
        self._keys_sync()

    def _key_end(self, b):
        if self.key_capture == b:
            self.key_capture = None
            self._keys_sync()

    def _key_press(self, b, e):
        if e.key() == Qt.Key_Escape:
            self._key_end(b)
            return
        name = sdl_key_from_qt(e)
        if not name:
            return                           # a key --keys cannot name: keep waiting
        self.key_capture = None
        self.key_vals[b] = name
        self._keys_sync()

    def _keys_restore(self):
        self.key_capture = None
        for b in self.key_vals:
            self.key_vals[b] = M.DEFAULT_KEYS[b]
        self._keys_sync()

    # ---- controller table
    @staticmethod
    def _src_label(k):
        return tr("src." + k) if k in PAD_SRC_KEYS else k

    def _padmap_chosen(self, b, cb):
        lab = cb.currentText()
        self.pad_vals[b] = next((k for k in PAD_SRC_KEYS if lab == tr("src." + k)), lab)
        self._padmap_sync()

    def _padmap_sync(self):
        used = {}
        for b in M.BUTTONS:
            k = self.pad_vals[b]
            cb = self.pad_cbs[b]
            i = cb.findText(self._src_label(k))
            if i < 0:
                cb.addItem(self._src_label(k))
                i = cb.count() - 1
            cb.setCurrentIndex(i)
            if k:
                used.setdefault(k, []).append(b)
        clash = [(k, bs) for k, bs in used.items() if len(bs) > 1]
        for b in M.BUTTONS:
            lb = self.pad_names[b]
            lb.setObjectName("nameSel" if b == self.pad_capture else ("nameWarn" if any(b in bs for _, bs in clash) else ""))
            lb.style().unpolish(lb)
            lb.style().polish(lb)
        self.pad_warn.setText("\n".join(tr("gp.clash", self._src_label(k), ", ".join(tr("btn." + x) for x in bs)) for k, bs in clash))
        self.pad_warn.setVisible(bool(clash))
        self.pad_view.update()

    def _padmap_restore(self):
        for b in self.pad_vals:
            self.pad_vals[b] = PAD_DEFAULTS[b]
        self._padmap_sync()

    def _pad_select(self, b, focus=True):
        self.pad_capture = b
        if focus:
            self.pad_cbs[b].setFocus()
        self._padmap_sync()

    def _pad_key_text(self, b):
        k = self.pad_vals[b]
        return PAD_SHORT.get(k, k) or tr("pad.nokey")

    def _build_mods(self, f):
        if has("patch"):
            c = self.card(f, tr("c.patch"))
            self._patch_widget(c)
            bp = bundled_patch()
            c.addWidget(wrap_label(tr("l.patchhelp", os.path.basename(bp) if bp else tr("l.patchnone")), "muted"))
        if self.experimental:
            c = self.card(f, tr("c.texpack"))                 # --texture-pack / --dump-textures (port2/shims/texpack.c)
            c.addWidget(wrap_label(tr("l.texpack"), "muted"))
            c.addWidget(self.browse_row("texpack", self.pick_texpack))
            self.dumpall_btn = QPushButton(tr("b.dumpall"))   # the full dump from the disc files (tools/dump_all_textures.py)
            self.dumpall_btn.clicked.connect(self.dump_all_textures)
            self.dumpall_bar = QProgressBar()
            self.dumpall_bar.setRange(0, 100)
            self.dumpall_bar.setTextVisible(False)
            self.dumpall_bar.setFixedWidth(200)
            self.dumpall_open = QPushButton(tr("b.openfolder"))
            self.dumpall_open.clicked.connect(self.open_texture_dump)
            self.dumpall_open.hide()
            row, _ = hbox(self.dumpall_btn, self.dumpall_bar, self.dumpall_open, None, spacing=10, margins=(0, 4, 0, 0))
            c.addWidget(row)
            self.dumpall_msg = wrap_label("", sel=True)
            self.dumpall_msg.hide()                           # shown with the first progress / result message
            c.addWidget(self.dumpall_msg)
            c.addWidget(wrap_label(tr("l.texpack.d"), "muted"))
            row.setVisible(has("texdump"))                    # (tools/dump_all_textures.py reads the PAL disc's layout)
        c = self.card(f, tr("c.mods"))
        c.addWidget(wrap_label(tr("l.modshelp"), "muted"))
        c.addSpacing(4)
        if not has("pokes"):                                  # only port_mods.PORTABLE_MODS in this version
            c.addWidget(wrap_label(tr("l.nomods"), "muted"))
        for mid, lk, dk, kind, _fn in M.MODS:
            if mid in ("keys_on", "skipintro", "hz60") or not (has("pokes") or mid in M.PORTABLE_MODS):
                continue      # on the Controls / Play pages
            box = QWidget()
            bl = QVBoxLayout(box)
            bl.setContentsMargins(0, 2, 0, 6)
            bl.setSpacing(4)
            chk = self.check(mid, tr(lk), False)
            row, rl = hbox(chk, spacing=12)
            bl.addWidget(row)
            if kind == "check+area":
                self.v("area", int, 0)
                self.v("section", int, 0)
                names = [M.area_label(a) for a, _ in M.ROOM_COUNTS]
                idx = [a for a, _ in M.ROOM_COUNTS]
                self.area_cb = QComboBox()
                self.area_cb.addItems(names)
                self.area_cb.setCurrentIndex(idx.index(self.s["area"]) if self.s["area"] in idx else 0)
                self.area_cb.view().setMinimumWidth(self.area_cb.minimumSizeHint().width())
                self.sec_cb = QComboBox()
                self.sec_cb.setMinimumWidth(70)
                g = QGridLayout()
                g.setContentsMargins(28, 2, 0, 2)
                g.setHorizontalSpacing(10)
                g.setVerticalSpacing(6)
                g.addWidget(QLabel(tr("mod.area")), 0, 0)
                g.addWidget(self.area_cb, 0, 1)
                g.addWidget(QLabel(tr("mod.section")), 1, 0)
                g.addWidget(self.sec_cb, 1, 1, Qt.AlignLeft)
                g.setColumnStretch(2, 1)
                bl.addLayout(g)

                def fill_sections(_=None):
                    i = self.area_cb.currentIndex()
                    a = M.ROOM_COUNTS[i][0]
                    self.s["area"] = a
                    self.sec_nums = [k for k in range(M.ROOM_COUNTS[i][1]) if (a, k) not in M.DOOR_ROOMS]
                    self.sec_cb.clear()
                    self.sec_cb.addItems([str(k) for k in self.sec_nums])
                    sv = self.s.get("section")
                    self.sec_cb.setCurrentIndex(self.sec_nums.index(sv) if sv in self.sec_nums else 0)
                    self.s["section"] = self.sec_nums[self.sec_cb.currentIndex()]
                self.area_cb.activated.connect(fill_sections)
                self.sec_cb.activated.connect(lambda i: self.s.__setitem__("section", self.sec_nums[i]))
                fill_sections()
            elif kind.startswith("check+int:"):
                _, key, lo, hi, step = kind.split(":")
                val = self.v(key, int, int(lo))
                sp = QSpinBox()
                sp.setRange(int(lo), int(hi))
                sp.setSingleStep(int(step))
                sp.setValue(val if isinstance(val, int) else int(lo))
                sp.setMinimumWidth(120)
                sp.setFocusPolicy(Qt.StrongFocus)
                sp.valueChanged.connect(lambda n, key=key: self.s.__setitem__(key, n))
                rl.addWidget(sp)
            rl.addStretch(1)
            d = wrap_label(tr(dk), "muted")
            d.setContentsMargins(28, 0, 0, 0)
            bl.addWidget(d)
            if kind == "check+area":                  # the rebuilt cut towns live on their own page now
                d = wrap_label(tr("cut.hint"), "muted")
                d.setContentsMargins(28, 0, 0, 0)
                bl.addWidget(d)
            c.addWidget(box)
        f.addStretch(1)

    def _build_cut(self, f):
        """The "Cut content" page: the --cut-content switch, the early title and the rebuilt towns (one button each)."""
        c = self.card(f, tr("cut.c.text"))
        row, _ = hbox(self.check("cut_content", tr("l.cutcontent"), False),          # --cut-content (port/fixes.txt)
                      self.info_button(tr("l.cutcontent"), tr("i.cutcontent")), None, spacing=4)
        c.addWidget(row)
        d = wrap_label(tr("cut.text.d"), "muted")
        d.setContentsMargins(28, 0, 0, 0)
        c.addWidget(d)
        c.addSpacing(8)
        c.addWidget(wrap_label(tr("l.earlytitle"), "muted"))
        c.addWidget(self.browse_row("early_title", self.pick_early_title))           # --early-title (port2/runtime/titlebg.c)
        c = self.card(f, tr("cut.c.towns"))                  # docs/CIVILIZATION.md "Playable towns"
        d = wrap_label(tr("cut.towns.d"), "muted")
        row, rl = hbox(d, self.info_button(tr("cut.c.towns"), tr("cut.towns.i")), spacing=4)
        rl.setStretch(0, 1)
        rl.setAlignment(rl.itemAt(1).widget(), Qt.AlignTop)
        c.addWidget(row)
        g = QGridLayout()
        g.setContentsMargins(0, 6, 0, 6)
        g.setHorizontalSpacing(14)
        g.setVerticalSpacing(10)
        g.setColumnStretch(1, 1)
        for i, a in enumerate((11, 16, 17)):
            b = QPushButton(tr("cut.play%d" % a))
            b.setMinimumWidth(260)
            b.clicked.connect(lambda _=False, a=a: self.play_town(a))
            g.addWidget(b, i, 0)
            g.addWidget(wrap_label(tr("cut.d%d" % a)), i, 1)
        c.addLayout(g)
        for key in ("civ_route", "civ_collision"):
            lk_ = {"civ_route": "mod.civroute", "civ_collision": "mod.civcoll"}[key]
            row, _ = hbox(self.check(key, tr(lk_), False), self.info_button(tr(lk_), tr(lk_ + ".i")), None, spacing=4)
            c.addWidget(row)
        f.addStretch(1)

    def play_town(self, a):
        """A town button: --civ-town a (the maps and texts are generated the first time, then the game starts)."""
        self.civ_play = a
        self.play()

    def play_normal(self):
        self.civ_play = None
        self.play()

    def _build_log(self, f):
        c = self.card(f, tr("c.status"))
        self.log_status = wrap_label(self.status_msg, sel=True)
        c.addWidget(self.log_status)
        mono = QFontDatabase.systemFont(QFontDatabase.FixedFont)
        mono.setPointSizeF(max(8.5, self.font().pointSizeF() - 0.5))
        c = self.card(f, tr("c.cmd"))
        self.cmd_text = QPlainTextEdit()
        self.cmd_text.setObjectName("code")
        self.cmd_text.setFont(mono)
        self.cmd_text.setReadOnly(True)
        self.cmd_text.setFixedHeight(QFontMetricsF(mono).lineSpacing() * 3 + 22)
        c.addWidget(self.cmd_text)
        c = self.card(f, tr("c.log"))
        self.log_text = QPlainTextEdit()
        self.log_text.setObjectName("code")
        self.log_text.setFont(mono)
        self.log_text.setReadOnly(True)
        self.log_text.setLineWrapMode(QPlainTextEdit.NoWrap)
        self.log_text.setMinimumHeight(QFontMetricsF(mono).lineSpacing() * 10 + 22)
        c.addWidget(self.log_text, 1)
        c.frame.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Expanding)
        f.setStretchFactor(c.frame, 1)
        try:            # the previous run's log, if any
            self.show_log_tail(open(self.log_path, encoding="utf-8", errors="replace").read().splitlines())
        except OSError:
            pass

    # ---- actions
    def collect(self):
        if hasattr(self, "key_vals"):
            self.s["keys"] = {b: v.strip() for b, v in self.key_vals.items()}
            self.s["keys_on"] = self.s["keys"] != M.DEFAULT_KEYS     # --keys only when a key differs
        if hasattr(self, "pad_vals"):
            self.s["pad_map"] = dict(self.pad_vals)
        save_settings(self.s)

    def _open_file(self, title, filt):
        p, _ = QFileDialog.getOpenFileName(self, title, "", filt)
        return p

    def _pick_dir(self):
        return QFileDialog.getExistingDirectory(self)

    def pick_disc(self):
        """(the single disc field of older launchers is the library's Add game... now)"""
        self.add_game()

    def pick_patch(self):
        p = self._open_file(tr("c.patch"), "Text patch (*.tombipatch.json);;JSON (*.json);;All (*)")
        if p:
            self.s["patch"] = p
            self.s["patch_on"] = True
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
        if want and self._patch_mode() == "translation" and not want.endswith(".json"):
            want = ""        # only the compiled translation.tpatch: data/ stays original and tombi2 applies it in memory
        if want and str(s.get("lang")) != "5" and self._patch_mode() == "bundled":
            want = ""        # the bundled translation is for the Spanish game only: other languages play the original data
        if not want and s.get("cut_content") and str(s.get("lang") or 0) == "0":
            # --cut-content in English: the English text of the cut lines (data/ holds one patch at a time; with another
            # text patch chosen that one wins and the cut lines show the disc's filler)
            want = bundled_patch("en_cut_content.tombipatch.json") or ""
        try:
            if not want:
                if st:
                    n = DP.restore(d)
                    self.say(tr("s.patchoff", n), "info")
                return True
            p = want
            if not os.path.isfile(p):
                self.error(tr("s.patchmissing", p))
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
            QApplication.processEvents()
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
            self.error(tr("s.patchfail", e))
            return False

    def pick_early_title(self):
        p = self._pick_dir()
        if p:
            self.set_edit("early_title", p)

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
            self.error(tr("s.nodata"))
            return
        import dump_all_textures as DT
        out = self.texture_dump_dir()
        self.dumpall_btn.setEnabled(False)
        self.dumpall_open.hide()

        def msg(text):
            self.dumpall_msg.setText(text)
            self.dumpall_msg.show()

        def progress(i, n):
            pct = i * 100 // max(1, n)
            self.later(lambda: (msg(tr("s.dumpall", pct)), self.dumpall_bar.setValue(pct)))

        def work():
            try:
                total = DT.dump_static(out, data_dir(), log=lambda *a: None, progress=progress)
                self.later(lambda: (msg(tr("s.dumpallok", total, out)), self.dumpall_open.show()))
            except Exception as e:
                self.later(msg, tr("s.dumpallfail", e))
            self.later(self.dumpall_btn.setEnabled, True)
        threading.Thread(target=work, daemon=True).start()

    def pick_texpack(self):
        p = self._pick_dir()
        if p:
            self.set_edit("texpack", p)

    def pick_savedir(self):
        p = self._pick_dir()
        if p:
            self.set_edit("savedir", p)

    def card_file(self):
        """the card the port uses (port2/shims/card.c: --save-dir, else next to the disc image / the data/ folder)"""
        self.collect()
        d = version_save_dir(self.s.get("savedir") or "") or (
            os.path.dirname(data_dir()) if data_ready()
            else os.path.dirname(bin_of_cue(self.s["disc"])) if self.s.get("disc") else app_dir())
        return os.path.join(d, "tombi_mc0.mcd")

    def _save_busy(self):
        if self.proc and self.proc.poll() is None:
            self.warning(tr("s.saverun"))
            return True
        return False

    def import_save(self):
        import save_import as SI
        if self._save_busy():
            return
        p = self._open_file(tr("b.saveimp"), "PS1 saves (*.mcd *.mcr *.mc *.srm *.ddf *.ps *.bin *.gme *.vgs *.mem *.mcs *.ps1 *.mcb *.psv);;All (*)")
        if not p:
            return
        dest = self.card_file()
        if not self.ask(tr("s.saveask", dest)):
            return
        try:
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            try:
                lines, warn = SI.import_file(p, dest, es=i18n.current() == "es")
            except SI.Conflict as c:
                choice = self.ask_save_conflict(c)
                if choice is None:
                    return
                if choice == "slot":
                    lines, warn = SI.import_file(p, dest, new_slot=True, es=i18n.current() == "es")
                else:
                    lines, warn = SI.import_file(p, dest, overwrite=True, es=i18n.current() == "es")
        except (SI.ImportError_, OSError) as e:
            self.error(tr("s.saveerr", e))
            return
        (self.warning if warn else self.info)("\n".join(lines + warn))
        self.say(tr("s.saveimpok"), "ok")

    def ask_save_conflict(self, c):
        """the card already holds a save with that name: Replace (the card is copied into backup/ first) / Save to
        another slot of the card (only when it has room and a free slot number: c.new_slot, named in the question) /
        Cancel -> "replace", "slot" or None"""
        box = QMessageBox(self)
        box.setWindowTitle(tr("t.savedup"))
        box.setIcon(QMessageBox.Question)
        box.setText(c.question(i18n.current() == "es"))
        rep = box.addButton(tr("b.savereplace"), QMessageBox.AcceptRole)
        oth = box.addButton(tr("b.saveslot"), QMessageBox.ActionRole) if c.new_slot else None
        can = box.addButton(tr("b.savecancel"), QMessageBox.RejectRole)
        box.setDefaultButton(can)
        box.setEscapeButton(can)
        box.exec()
        b = box.clickedButton()
        return "replace" if b is rep else "slot" if oth is not None and b is oth else None

    def export_save(self):
        import save_import as SI
        if self._save_busy():
            return
        src = self.card_file()
        try:
            card = open(src, "rb").read() if os.path.exists(src) else bytes(SI.blank_card())
            saves = SI.list_saves(card)
            if saves and self.ask(tr("s.saveexpq")):
                blk = saves[0][0]
                if len(saves) > 1:
                    txt = "\n".join("%d: %s  %s" % s for s in saves)
                    blk, ok = QInputDialog.getInt(self, tr("title"), txt, blk, 1, 15)
                    if not ok:
                        return
                out, _ = QFileDialog.getSaveFileName(self, tr("b.saveexp"), "tombi.mcs", "MCS (*.mcs)")
                if out:
                    open(out, "wb").write(SI.export_save(card, blk))
            else:
                out, _ = QFileDialog.getSaveFileName(self, tr("b.saveexp"), "tombi.mcd", "Memory card (*.mcd *.mcr);;All (*)")
                if out:
                    SI.export_card(src, out)
            if out:
                self.say(tr("s.saveexpok", out), "ok")
        except (SI.ImportError_, OSError) as e:
            self.error(tr("s.saveerr", e))

    def editor_open(self):
        try:
            return self.editor is not None and self.editor.isVisible()
        except RuntimeError:            # the Qt object is gone
            self.editor = None
            return False

    def edit_save(self):
        """Edit save...: the save editor (tools/tombi_save_editor.py, docs/SAVE_EDITOR.md) in its own window of this
        process, on the port's card (card_file).  Not while the game runs (nor saves while it runs); saving overwrites
        the card and keeps a timestamped .bak next to it."""
        if self.editor_open():
            self.editor.raise_()
            self.editor.activateWindow()
            return
        if self._save_busy():
            return
        card = self.card_file()
        if not os.path.isfile(card):
            self.info(tr("s.editnocard", card))
            return
        try:
            import tombi_save_editor as SE
        except ImportError as e:        # running from source without QtSvg
            self.error(tr("s.editnoqt", e))
            return

        def guard():
            return tr("s.editrun") if self.proc and self.proc.poll() is None else None

        def closed():
            self.editor = None
        try:
            # the active text patch: the editor shows its item names (Spanish improved translation: Mangual...)
            tp = (self.s.get("patch") or "") if self.s.get("patch_on") else ""
            self.editor = SE.host_in_qt(self, card, lang=i18n.current(), write_guard=guard, on_closed=closed,
                                        text_patch=tp)
        except Exception as e:
            self.editor = None
            self.error(tr("s.saveerr", e))

    def check_disc(self, wait=False):
        """The selected game's disc image: what it is (game_library.identify_image: SYSTEM.CNF boot file + sha1), then
        the one-time extraction into its data folder when that is not complete. A thread (wait=True: here, the tests)."""
        self.collect()
        e = self.cur_entry()
        picked = (e or {}).get("image") or self.s.get("disc") or ""
        image = bin_of_cue(picked)
        if not image or self.extracting:
            return
        archive = DA.is_archive(picked)
        if archive and GL.data_complete(data_dir()) and GL.data_complete(data_dir()) == (e or {}).get("sha1"):
            return                               # extracted already: the archive is not unpacked again
        self.say(tr("s.checking"), "busy")

        def work():
            img = image
            try:
                if archive:
                    img = self.disc_image(picked)
                info = GL.identify_image(img)
                if info["game_sha1"] in (MAIN0_SHA1, REV0_SHA1):     # tombi2 --disc (PAL) checks MAIN0.EXE next to the launcher
                    extract_main0(img, os.path.join(app_dir(), "MAIN0.EXE"))
            except DA.ArchiveError as ex:
                self.drop_unpacked(picked)
                self.later(self.archive_error, picked, ex)
                return
            except (OSError, ValueError, struct.error) as ex:
                self.drop_unpacked(picked)
                self.later(self.say, str(ex), "bad")
                return
            self.later(self._checked, e, img, info, picked if archive else None)
        if wait:
            work()
            QApplication.processEvents()
        else:
            threading.Thread(target=work, daemon=True).start()

    def _checked(self, e, image, info, archive=None):
        """check_disc's answer, on the GUI thread: the message, the entry updated, the extraction started (archive: the
        .zip / .7z the image was unpacked from; that copy is deleted when the extraction ends or is not needed)."""
        sha, v = info["game_sha1"], info["version"]
        name = ("%s %s" % (info["serial"] or "?", info["name"])).strip()
        if sha == MAIN0_SHA1:
            msg, lvl = tr("s.ok", name), "ok"
        elif sha == REV0_SHA1:
            msg, lvl = tr("s.rev0", name), "bad"
        elif v and v["supported"]:
            msg, lvl = tr("s.ok2", name, info["boot"]), "ok"
        elif v:
            msg, lvl = tr("l.unsupported", name), "bad"
        else:
            msg, lvl = tr("s.notgame", os.path.basename(archive or image), info["serial"] or "?", info["name"] or tr("lib.unknown")), "bad"
        if e is not None and e in self.entries() and v:
            before = (e.get("serial"), e.get("sha1"), e.get("data"))
            self._identified(e, info)
            if (e.get("serial"), e.get("sha1"), e.get("data")) != before and e in self.entries():
                self.rebuild(check=False)
        self.say(msg, lvl)
        if not (v and v["supported"]) or self.extracting:
            if archive:
                self.drop_unpacked(archive)
            return
        ddir = data_dir()
        if GL.data_complete(ddir) == sha:
            if archive:
                self.drop_unpacked(archive)
            return
        # the one-time extraction into the data folder (again when it was made from another disc)
        self.extracting = True
        self._extracting(0)

        def work():
            try:
                DP.restore(ddir)       # a text patch in an old data/: originals back before (re)extracting
            except OSError:
                pass
            last = [-1]

            def progress(done, total, path):
                pct = done * 100 // max(1, total)
                if pct != last[0]:
                    last[0] = pct
                    self.later(self.say, tr("s.extracting", ddir, pct, path), "busy")
                    self.later(self._extracting, pct)
            try:
                man = X.extract(image, ddir, progress)
                self.later(self.say, tr("s.dataok", ddir, len(man["files"])), "ok")
            except (OSError, ValueError, struct.error) as ex:
                self.later(self.say, tr("s.extractfail", ex), "bad")
            finally:
                if archive:
                    self.drop_unpacked(archive)
            self.later(self._extracting, None)
        threading.Thread(target=work, daemon=True).start()

    def _extracting(self, pct):
        """pct 0..100 while data/ is being extracted (Play disabled, bar and message shown); None when it ends (the
        cards are drawn again: the game is ready)."""
        self.ext_pct = pct
        try:
            if pct is None:
                self.extracting = False
                self.ext_box.hide()
                self.play_btn.setText(tr("b.play"))
                self.play_btn.setEnabled(True)
                msg, lvl = self.status_msg, self._level
                self._art_tried.clear()
                self.rebuild(check=False)
                self.say(msg, lvl)
                return
            self.ext_box.show()
            self.play_btn.setEnabled(False)
            self.play_btn.setText(tr("b.extracting", pct))
            self.ext_bar.setValue(pct)
        except RuntimeError:
            pass

    def command(self):
        self.collect()
        self.cmd_s = dict(self.s, civ_play=self.civ_play)     # civ_play: transient, never saved
        cmd, self.gfx_note = build_command(self.s, self.civ_play)
        return cmd

    def _show_cmd(self, cmd):
        self.cmd_text.setPlainText(subprocess.list2cmdline(cmd))

    def play(self):
        if self.editor_open():          # the game would read (and write) the card the editor holds
            self.warning(tr("s.editopen"))
            self.editor.raise_()
            self.editor.activateWindow()
            return
        e = self.cur_entry()
        v = GL.version_of(current_sha1())
        if e is not None and e.get("sha1") and not (v and v["supported"]):
            self.error(tr("l.unsupported", self.game_name(e)))
            return
        if self.extracting:
            self.warning(tr("s.busyextract"))
            return
        cmd = self.command()
        self._show_cmd(cmd)
        if not os.path.isfile(cmd[0]):
            self.error(tr("s.nobin", cmd[0]))
            return
        pal = current_sha1() == MAIN0_SHA1
        if cmd[1] == "--disc" and (not pal or not os.path.isfile(cmd[2]) or not os.path.isfile(os.path.join(app_dir(), "MAIN0.EXE"))
                                   or X.chd.is_chd(cmd[2])):     # tombi2 reads .bin/.iso only: a .chd always goes through data/
            # (the other versions always run from their data folder)
            if os.path.isfile(cmd[2]):
                self.check_disc()      # extracts data/ as well; Play again when it says the data is ready
            self.error(tr("s.nodata"))
            return
        if self.proc and self.proc.poll() is None:
            return
        if cmd[1] == "--data" and has("patch"):
            if not self.sync_patch():
                return
            cmd = self.command()     # sync_patch may just have (re)built the redraws: --patch-tex must see them
            self._show_cmd(cmd)
        elif self.s.get("patch_on") and has("patch"):
            self.say(tr("s.patchnodata"), "busy")
        if has("cut") and M.civ_wanted(self.cmd_s):
            if cmd[1] != "--data":
                self.error(tr("s.civnodata"))
                return
            if not civ_ready():
                self.generate_civ()       # Play goes on by itself once the towns are built
                return
        self.log_path = os.path.join(app_dir(), "tombi-port.log")
        log = open(self.log_path, "w", encoding="utf-8", errors="replace")
        log.write("[launcher] build %s\n" % BUILD_ID)
        log.write("[launcher] game: %s %s (%s)\n" % ((e or {}).get("serial") or "?", (v or {}).get("region", "PAL"), data_dir()))
        log.write("[launcher] Spanish graphics (--patch-tex): %s\n" % getattr(self, "gfx_note", "?"))
        log.write("[launcher] command: %s\n" % " ".join(cmd))      # the exe then prints its own id: [port2] build <sha> <date>
        log.flush()
        flags = subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0
        self.civ_play = None
        self.proc = subprocess.Popen(cmd, cwd=app_dir(), stdout=log, stderr=subprocess.STDOUT, creationflags=flags)
        self.say(tr("s.running"), "ok")
        self.play_btn.setEnabled(False)
        self._starting(True)
        self.stop_btn.show()
        self.poll_timer.start(250)

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
            self.poll_timer.start(250 if self.starting else 1000)
            return
        self._starting(False)
        self.say(tr("s.ended", rc, self.log_path), "info")
        try:                           # the game quit (its QUIT entry, or the window closed): this window comes back
            if self.isMinimized():
                self.showNormal()
            self.raise_()
            self.activateWindow()
        except Exception:
            pass
        self.play_btn.setEnabled(True)
        self.stop_btn.hide()
        self.proc = None

    def generate_civ(self):
        """First use of --civ-town / --civ-route: the maps and texts, built in a thread (about a minute), the output on
        the Log page; Play continues when they are ready."""
        self.play_btn.setEnabled(False)
        self.show_page("log")
        self.log_text.clear()
        self.say(tr("s.civgen", civ_dir()), "busy")

        def log(line):
            self.later(self.log_text.appendPlainText, line)

        def done(err):
            self.play_btn.setEnabled(True)
            if err:
                self.say(tr("s.civgenfail", err), "bad")
                return
            self.say(tr("s.civgenok", civ_dir()), "ok")
            self.play()

        def work():
            err = None
            try:
                civ_generate(log)
            except BaseException as e:          # SystemExit from the scripts too
                import traceback
                for l in traceback.format_exc().splitlines():
                    log(l)
                err = str(e) or type(e).__name__
            self.later(done, err)
        threading.Thread(target=work, daemon=True).start()

    def _starting(self, on):
        """The "Starting the game..." notice in the bottom bar (an indeterminate bar) above the summary."""
        self.starting = on
        try:
            self.start_bar.setRange(0, 0 if on else 1)
            self.start_box.setVisible(on)
        except RuntimeError:
            pass

    def show_log_tail(self, lines):
        """The last 8 lines, preceded by the warning / FATAL / access-violation lines from earlier in the log (a
        missing disc, a bad layout) so a problem reported at startup stays visible under the final block."""
        tail = lines[-8:]
        warn = [l for l in lines[:-8] if "warning" in l or "FATAL" in l or "access violation" in l][:6]
        self.log_text.setPlainText("\n".join(warn + (["..."] if warn else []) + tail))

    def copy_log(self):
        """Whole log file to the clipboard (what to paste in a bug report)."""
        try:
            text = open(self.log_path, encoding="utf-8", errors="replace").read() if self.log_path else self.log_text.toPlainText()
        except OSError:
            text = self.log_text.toPlainText()
        QApplication.clipboard().setText(text)
        self.say(tr("s.copied", len(text.splitlines())), "info")

    def stop(self):
        if self.proc and self.proc.poll() is None:
            self.proc.terminate()

    def closeEvent(self, e):
        if self.editor_open():
            self.editor.close()         # asks about unsaved edits; No keeps both windows open
            if self.editor_open():
                e.ignore()
                return
        self.collect()
        self.stop()
        e.accept()


def make_app(argv=None):
    QApplication.setHighDpiScaleFactorRoundingPolicy(Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)
    app = QApplication.instance() or QApplication(argv if argv is not None else sys.argv)
    app.setStyle(QStyleFactory.create("Fusion"))
    f = QFont(app.font())
    if os.name == "nt":
        fams = set(QFontDatabase.families())
        for fam in ("Segoe UI Variable Text", "Segoe UI"):
            if fam in fams:
                f.setFamily(fam)
                break
    f.setPointSizeF(10)
    app.setFont(f)
    app.setApplicationName("Tombi! launcher")
    return app


def main():
    app = make_app()
    w = App()
    w.show()
    return app.exec()


if __name__ == "__main__":
    sys.exit(main())
