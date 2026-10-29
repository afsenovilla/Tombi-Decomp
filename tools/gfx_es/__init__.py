"""Spanish (--lang 5) graphic redraws for the native port, generated from the user's own extracted disc data.

No pixels are shipped: each part reads the original texture it redraws from data/ and returns VRAM rects, and build()
packs them into one TBTX file for `tombi2 --patch-tex FILE` (port2/shims/gpu.c). tools/data_patch.py runs it when a text
patch asks for it ("portGfx": "es") and the launcher passes the file with --lang 5. The port's source rules that show
the redraws (port/fixes.txt) only switch on when the rect is actually loaded (port_tex_patch_has), so without the file
the original English sprites stay.

Adding a redraw: a module with build(raw) -> [(x, y, w_words, h,
words)], an entry in PARTS, and a fixes.txt rule gated on port_tex_patch_has(x, y).
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


def build(data_dir, out, log=print):
    """Writes the TBTX of every part that builds; returns the number of rects (0: nothing written)."""
    import importlib
    rects = []
    for label, mod, src, read in PARTS:
        try:
            m = importlib.import_module("gfx_es." + mod)
            rects += m.build(read(data_dir, src))
        except Exception as e:     # Pillow missing, unexpected data...: that redraw is skipped, the port keeps the original
            log("gfx_es: %s skipped (%s)" % (label, e))
    if not rects:
        return 0
    b = bytearray(b"TBTX" + struct.pack("<I", len(rects)))
    for x, y, w, h, words in rects:
        b += struct.pack("<4H", x, y, w, h) + struct.pack("<%dH" % len(words), *words)
    with open(out + ".part", "wb") as f:
        f.write(b)
    os.replace(out + ".part", out)
    return len(rects)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit("usage: python3 -m gfx_es DATA_DIR OUT.tbtx   (from tools/)")
    print("%d rects" % build(sys.argv[1], sys.argv[2]))
