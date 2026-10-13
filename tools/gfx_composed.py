"""Composed room views for the graphics editor ("Vista compuesta"): every area / room as the game draws it (the 3D room, objects and Tombi on top of
the sky / far background) and the sky / far background layer alone.  Captured by running MAIN0.EXE in the MIPS interpreter (tools/psxemu, see
notes/formats.md "MIPS capture"), generated on demand and cached in the user's cache folder:

  %LOCALAPPDATA%/TombiTools/cache/composed/<disc key>/room_<AA>_<n>_composed.png | _sky.png   (Linux/macOS: ~/.cache/TombiTools/...)

Needs the disc image (Tombi.bin / .iso, not just an extracted folder) and numpy + Pillow.  ~15 s per room, ~3 min for all 96 rooms with 4 processes.
Edits of the editor are not applied (the capture shows the original disc)."""
import hashlib, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import i18n
from i18n import tr

i18n.add({
    "es": {"t.composed": "Vista compuesta (sala y fondo)", "t.room": "Sala {0}", "t.roomsky": "Sala {0}: fondo / cielo", "b.composedall": "Generar vistas",
           "l.composing": "Capturando la vista compuesta… {0}/{1} (se ejecuta el juego en un intérprete MIPS, ~15 s por sala)",
           "l.composeddone": "Vistas compuestas listas ({0}) en {1}", "l.nosky": "Esta sala no tiene capa de cielo / fondo lejano.",
           "d.composedimg": "La vista compuesta necesita la imagen del disco (Tombi.bin / .iso), no solo una carpeta extraída.",
           "d.composednp": "La vista compuesta necesita numpy (pip install numpy): {0}", "d.composederr": "No se pudo capturar la sala: {0}",
           "d.composedall": "Se capturarán las {0} salas que faltan ejecutando el juego (unos minutos, varios procesos). Se guardan en:\n{1}\n\n¿Continuar?",
           "d.composedfail": "{0} sala(s) fallaron:\n{1}"},
    "en": {"t.composed": "Composed view (room and background)", "t.room": "Room {0}", "t.roomsky": "Room {0}: sky / background", "b.composedall": "Generate views",
           "l.composing": "Capturing the composed view… {0}/{1} (the game runs in a MIPS interpreter, ~15 s per room)",
           "l.composeddone": "Composed views ready ({0}) in {1}", "l.nosky": "This room has no sky / far background layer.",
           "d.composedimg": "The composed view needs the disc image (Tombi.bin / .iso), not just an extracted folder.",
           "d.composednp": "The composed view needs numpy (pip install numpy): {0}", "d.composederr": "The room could not be captured: {0}",
           "d.composedall": "The {0} missing rooms will be captured by running the game (a few minutes, several processes). They are saved in:\n{1}\n\nContinue?",
           "d.composedfail": "{0} room(s) failed:\n{1}"},
})


def cache_root():
    if os.name == "nt":
        base = os.environ.get("LOCALAPPDATA") or os.path.expanduser("~")
    else:
        base = os.environ.get("XDG_CACHE_HOME") or os.path.join(os.path.expanduser("~"), ".cache")
    return os.path.join(base, "TombiTools", "cache", "composed")


def cache_dir(image):
    """Folder of the captures of one disc image (key = path + size + mtime)."""
    st = os.stat(image)
    key = hashlib.sha1(("%s|%d|%d" % (os.path.abspath(image), st.st_size, int(st.st_mtime))).encode("utf-8", "replace")).hexdigest()[:12]
    return os.path.join(cache_root(), key)


def rooms():
    """[(area, sections)] of the disc."""
    from psxemu.rooms import ROOM_COUNTS
    return list(ROOM_COUNTS)


def unreachable(area, section):
    from psxemu.rooms import UNREACHABLE
    return (area, section) in UNREACHABLE


def file_name(area, section, kind):
    return "room_%02d_%d_%s.png" % (area, section, kind)


def cached(image, area, section, kind="composed"):
    """Path of the capture when it exists, else None."""
    p = os.path.join(cache_dir(image), file_name(area, section, kind))
    return p if os.path.isfile(p) else None


def missing(image):
    """[(area, section)] of the capturable rooms without a cached composed picture."""
    return [(a, s) for a, n in rooms() for s in range(n) if not unreachable(a, s) and not cached(image, a, s)]


def generate(image, exe, area=None, section=None, jobs=1, progress=None, log=lambda *a: None):
    """Captures one room (area + section) or all the missing ones into the cache.  Blocking: run it in a thread.  Returns [(area, section, status, info)].
    Raises ImportError when numpy is missing."""
    from psxemu import capture
    out = cache_dir(image)
    return capture.rooms(out, area, section, jobs=jobs, log=log, progress=progress, exe=exe, disc=image)
