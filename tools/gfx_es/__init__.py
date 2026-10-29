"""Spanish (--lang 5) graphic redraws for the native port, generated from the user's own extracted disc data.

No pixels are shipped: each part reads the original texture it redraws from data/ and returns VRAM rects, and build()
packs them into one TBTX file for `tombi2 --patch-tex FILE` (port2/shims/gpu.c). tools/data_patch.py runs it when a text
patch asks for it ("portGfx": "es") and the launcher passes the file with --lang 5. The port's source rules that show
the redraws (port/fixes.txt) only switch on when the rect is actually loaded (port_tex_patch_has), so without the file
the original English sprites stay.

Adding a redraw: a module with build(raw) -> [(x, y, w_words, h, words)] and key(raw) -> (kx, ky, kw, kh, original words):
a VRAM rect inside the original graphic, as the game uploads it.  The port writes a rect only right after the game uploads
exactly those texels (the owning page) and turns it off when anything else lands there, so the redraw must lie inside
that page's own upload (or space only that page's screen uses).  Then an entry in PARTS, and a fixes.txt rule gated on
port_tex_patch_has(x, y).
"""
import os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if os.path.dirname(HERE) not in sys.path:
    sys.path.insert(0, os.path.dirname(HERE))

TBTX_NAME = "port_gfx_es.tbtx"


def _gam(data_dir, name):
    import area000 as A
    return A.read_resource(name, data_dir)


def _raw(data_dir, name):
    return open(os.path.join(data_dir, *name.split("/")), "rb").read()


# (label, module, source file, reader)
PARTS = [
    ("VUELVE A INTENTARLO", "tryagain", "SYSTEM/A10003", _gam),
    ("CARGAR / GUARDAR", "cargar_guardar", "SYSTEM/A00400.000", _raw),
    ("CARGANDO", "cargando", "SYSTEM/A00900.000", _raw),
]


def fnv1a(data):
    h = 2166136261
    for c in data:
        h = ((h ^ c) * 16777619) & 0xFFFFFFFF
    return h


def page_key(page, stride, vx, vy, u, v, w, h):
    """key rect of a 4bpp page stored `stride` bytes a row and uploaded at VRAM word (vx, vy): pixels u..u+w-1, v..v+h-1
    (u, w multiples of 4) -> (kx, ky, kw, kh, words)"""
    words = [page[r*stride + u//2 + 2*i] | page[r*stride + u//2 + 2*i + 1] << 8 for r in range(v, v + h) for i in range(w // 4)]
    return (vx + u//4, vy + v, w//4, h, words)


def generator_id():
    """sha1 of this package's sources: a .tbtx built by another version of the generator is rebuilt (written next to it
    as <out>.ver)"""
    import hashlib
    d = os.path.dirname(os.path.abspath(__file__))
    h = hashlib.sha1()
    for f in sorted(os.listdir(d)):
        if f.endswith(".py"):
            h.update(f.encode() + b"\0" + open(os.path.join(d, f), "rb").read())
    return h.hexdigest()


def is_current(path):
    """True when path is a TBTK file this generator version wrote (else apply/launcher rebuild it)"""
    try:
        with open(path, "rb") as f:
            if f.read(4) != b"TBTK":
                return False
        return open(path + ".ver", encoding="utf-8").read().strip() == generator_id()
    except OSError:
        return False


def build(data_dir, out, log=print):
    """Writes the TBTX of every part that builds; returns the number of rects (0: nothing written)."""
    import importlib
    rects = []
    for label, mod, src, read in PARTS:
        try:
            m = importlib.import_module("gfx_es." + mod)
            raw = read(data_dir, src)
            k = m.key(raw)
            rects += [r + (k,) for r in m.build(raw)]
        except Exception as e:     # Pillow missing, unexpected data...: that redraw is skipped, the port keeps the original
            log("gfx_es: %s skipped (%s)" % (label, e))
    if not rects:
        return 0
    b = bytearray(b"TBTK" + struct.pack("<I", len(rects)))
    for x, y, w, h, words, (kx, ky, kw, kh, kwords) in rects:
        assert len(kwords) == kw * kh
        b += struct.pack("<8HI", x, y, w, h, kx, ky, kw, kh, fnv1a(struct.pack("<%dH" % len(kwords), *kwords)))
        b += struct.pack("<%dH" % len(words), *words)
    with open(out + ".part", "wb") as f:
        f.write(b)
    os.replace(out + ".part", out)
    with open(out + ".ver", "w", encoding="utf-8") as f:
        f.write(generator_id() + "\n")
    return len(rects)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit("usage: python3 -m gfx_es DATA_DIR OUT.tbtx   (from tools/)")
    print("%d rects" % build(sys.argv[1], sys.argv[2]))
