#!/usr/bin/env python3
"""Button prompt glyphs for the native port (docs/PORT.md "Mods": button prompts).  Draws our own replacement glyphs
(no third-party icon pack) for the PS1 button sprites the game shows in tutorials/menus, as clean 4-bpp art of the
same size as the originals, and packs them into the TBTX file that `tombi --patch-tex FILE` re-applies over the VRAM
after every LoadImage that overlaps the glyph rects (port/shims/gpu.c).

  python3 tools/prompt_glyphs.py draw                 # writes port/assets/prompts/<set>/<button>.png (committed art)
  python3 tools/prompt_glyphs.py pack SET OUT.tbtx [--keys up=Up,cross=X,...]
        SET: playstation | xbox | keyboard; "keyboard" renders the key names of the mapping (the viewer's defaults
        otherwise).  The VRAM rects and CLUT of each glyph come from port/assets/prompts/manifest.json; a glyph whose
        rect is null is skipped (see "Locating the glyphs" in docs/PORT.md).

Glyph pixels use palette indices 0 (transparent), 1 (dark outline), 2 (light fill) and 3 (mid tone) of the glyph's
CLUT, which the manifest maps to real CLUT entries ("idx": [i0, i1, i2, i3]) per rect.
"""
import json, os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ASSETS = os.path.join(getattr(sys, "_MEIPASS", ROOT), "port", "assets", "prompts")   # PyInstaller: bundled manifest
BUTTONS = ["cross", "circle", "square", "triangle", "l1", "r1", "l2", "r2", "start", "select", "up", "down", "left", "right"]
XBOX = {"cross": "A", "circle": "B", "square": "X", "triangle": "Y", "l1": "LB", "r1": "RB", "l2": "LT", "r2": "RT",
        "start": "≡", "select": "▣", "up": "↑", "down": "↓", "left": "←", "right": "→"}
DEFAULT_KEYS = {"up": "Up", "down": "Down", "left": "Left", "right": "Right", "cross": "X", "circle": "D", "square": "Z",
                "triangle": "S", "l1": "Q", "r1": "W", "l2": "A", "r2": "E", "start": "Return", "select": "Backspace"}
KEY_SHORT = {"Return": "Ent", "Backspace": "Bks", "Space": "Spc", "Up": "↑", "Down": "↓", "Left": "←", "Right": "→",
             "Left Shift": "Sft", "Right Shift": "Sft", "Left Ctrl": "Ctl", "Escape": "Esc", "Tab": "Tab"}
SIZE = 16          # glyph cell (pixels); the manifest rect may be smaller: the art is cropped to it, centred


def font(size):
    from PIL import ImageFont
    for name in ("DejaVuSans-Bold.ttf", "DejaVuSans.ttf", "arialbd.ttf", "arial.ttf"):
        try:
            return ImageFont.truetype(name, size)
        except OSError:
            continue
    return ImageFont.load_default()


def draw_glyph(kind, label):
    """A SIZE x SIZE 'P' image with indices 0..3: kind = circle (face buttons), pill (shoulders), key (keyboard), arrow."""
    from PIL import Image, ImageDraw
    im = Image.new("P", (SIZE, SIZE), 0)
    im.putpalette([0, 0, 0, 20, 20, 40, 240, 240, 240, 140, 140, 160] + [0] * (768 - 12))
    d = ImageDraw.Draw(im)
    if kind == "circle":
        d.ellipse([0, 0, SIZE - 1, SIZE - 1], fill=2, outline=1)
        d.ellipse([1, 1, SIZE - 2, SIZE - 2], outline=3)
    elif kind == "pill":
        d.rounded_rectangle([0, 2, SIZE - 1, SIZE - 3], radius=4, fill=2, outline=1)
    elif kind == "key":
        d.rounded_rectangle([0, 0, SIZE - 1, SIZE - 1], radius=2, fill=2, outline=1)
        d.line([2, SIZE - 2, SIZE - 3, SIZE - 2], fill=3)
    else:
        d.ellipse([0, 0, SIZE - 1, SIZE - 1], fill=2, outline=1)
    if label:
        f = font(9 if len(label) <= 1 else 7 if len(label) <= 2 else 6)
        bb = d.textbbox((0, 0), label, font=f)
        d.text(((SIZE - (bb[2] - bb[0])) // 2 - bb[0], (SIZE - (bb[3] - bb[1])) // 2 - bb[1]), label, font=f, fill=1)
    return im


def draw_ps_symbol(button):
    """PlayStation face symbols drawn as shapes (not text)."""
    from PIL import Image, ImageDraw
    im = draw_glyph("circle", "")
    d = ImageDraw.Draw(im)
    c = SIZE // 2
    if button == "cross":
        d.line([c - 4, c - 4, c + 4, c + 4], fill=1, width=2); d.line([c - 4, c + 4, c + 4, c - 4], fill=1, width=2)
    elif button == "circle":
        d.ellipse([c - 4, c - 4, c + 4, c + 4], outline=1, width=2)
    elif button == "square":
        d.rectangle([c - 4, c - 4, c + 4, c + 4], outline=1, width=2)
    elif button == "triangle":
        d.polygon([(c, c - 5), (c - 5, c + 4), (c + 5, c + 4)], outline=1)
        d.polygon([(c, c - 3), (c - 3, c + 3), (c + 3, c + 3)], outline=1)
    return im


def glyph(set_name, button, keys=None):
    if set_name == "playstation":
        if button in ("cross", "circle", "square", "triangle"):
            return draw_ps_symbol(button)
        if button in ("l1", "r1", "l2", "r2"):
            return draw_glyph("pill", button.upper())
        if button in ("start", "select"):
            return draw_glyph("pill", "St" if button == "start" else "Se")
        return draw_glyph("circle", XBOX[button])
    if set_name == "xbox":
        return draw_glyph("pill" if len(XBOX[button]) > 1 else "circle", XBOX[button])
    keys = keys or DEFAULT_KEYS
    k = keys.get(button, DEFAULT_KEYS[button])
    return draw_glyph("key", KEY_SHORT.get(k, k[:3]))


def cmd_draw():
    for set_name in ("playstation", "xbox", "keyboard"):
        d = os.path.join(ASSETS, set_name)
        os.makedirs(d, exist_ok=True)
        for b in BUTTONS:
            glyph(set_name, b).save(os.path.join(d, b + ".png"))
    print("glyphs written to", ASSETS)


def pack(set_name, out, keys=None, manifest=None):
    """TBTX file for --patch-tex from the manifest's rects; returns the number of glyphs packed."""
    man = manifest or json.load(open(os.path.join(ASSETS, "manifest.json"), encoding="utf-8"))
    entries = []
    for b in BUTTONS:
        m = man.get("glyphs", {}).get(b)
        if not m or not m.get("rect"):
            continue
        x, y, w, h = m["rect"]            # VRAM words (4 bpp: one word = 4 pixels) and rows
        idx = m.get("idx", [0, 1, 2, 3])
        im = glyph(set_name, b, keys)
        pw = w * 4
        ox, oy = max(0, (SIZE - pw) // 2), max(0, (SIZE - h) // 2)
        words = []
        for j in range(h):
            for i in range(w):
                v = 0
                for n in range(4):
                    px, py = ox + i * 4 + n, oy + j
                    p = im.getpixel((px, py)) if 0 <= px < SIZE and 0 <= py < SIZE else 0
                    v |= (idx[p] & 15) << (4 * n)
                words.append(v)
        entries.append((x, y, w, h, words))
    with open(out, "wb") as f:
        f.write(b"TBTX" + struct.pack("<I", len(entries)))
        for x, y, w, h, words in entries:
            f.write(struct.pack("<4H", x, y, w, h) + struct.pack("<%dH" % len(words), *words))
    return len(entries)


def main():
    a = sys.argv[1:]
    if a and a[0] == "draw":
        cmd_draw()
    elif len(a) >= 3 and a[0] == "pack":
        keys = None
        if "--keys" in a:
            keys = dict(kv.split("=", 1) for kv in a[a.index("--keys") + 1].split(",") if "=" in kv)
        print("%d glyphs -> %s" % (pack(a[1], a[2], keys), a[2]))
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
