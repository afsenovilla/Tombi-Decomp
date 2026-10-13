#!/usr/bin/env python3
"""Offscreen renderer of a Tomba! dialogue: mock message boxes with the game's own glyphs (taken from the WFM file).

render(wfm, words, font, scale) -> (PIL image, [warnings]). Used by tools/text_editor.py for the live preview and by
`tools/text_core.py render` to write PNGs. Needs Pillow only.

Box rules measured on the 5,802 boxed dialogues of the Spanish files: BOX width = widest line in pixels + 16 (8 px
margin each side), BOX height = 16 + 13 * lines (any glyph height; cursor starts at (8, 6) and NL moves it to x 8, y + 13); a page ends at {CLEAR}; the engine keeps 128 glyphs
per message slot (MESSAGE_GLYPHS[][128]). Palette: an approximation for the 8/16 px fonts, TombaTools' EventClut for the 24 px one
(the real CLUTs are chosen at run time by the glyph clut field).
"""
import os, sys

from PIL import Image, ImageDraw

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wfm_py as W

MARGIN, LINE_STEP, GLYPH_LIMIT, TOP = 8, 13, 128, 6   # cursor starts at (8, 6) after BOX/CLEAR; NL = x 8, y + 13
# 8/16 px fonts: index 1 is the body of the strokes, 2..7 are the shaded edges (white text, grey shading in the game);
# the real CLUT is picked by the game at run time, so this is an approximation that keeps the strokes readable
DIALOGUE_RGB = ((0, 0, 0, 0), (250, 250, 250, 255), (150, 150, 176, 255), (214, 214, 230, 255), (186, 186, 208, 255),
                (122, 122, 156, 255), (98, 98, 132, 255), (74, 74, 108, 255)) + ((190, 190, 255, 255),) * 8
EVENT_CLUT = (0x01FF, 0x8400, 0x7FFF, 0x3DEF, 0x2529, 0x56B5, 0x00F0, 0x0198, 0x6739, 0x0134, 0x01FF, 0x7C00,
              0x7C00, 0x7C00, 0x7C00, 0x7C00)
BG, BOX_FILL, BOX_EDGE, RED = (34, 34, 40, 255), (20, 28, 92, 255), (214, 220, 255, 255), (255, 48, 48, 255)


def psx_rgba(c):
    return ((c & 31) << 3, ((c >> 5) & 31) << 3, ((c >> 10) & 31) << 3, 255)


def glyph_image(g):
    """RGBA image of one glyph (transparent where the pixel has the colour of index 0)."""
    event = g.height == 24
    pal = EVENT_CLUT if event else None
    im = Image.new("RGBA", (max(g.width, 1), max(g.height, 1)), (0, 0, 0, 0))
    put = im.load()
    for i, v in enumerate(g.pixels()):
        if v == 0 or (event and pal[v] == pal[0]):
            continue
        put[i % g.width, i // g.width] = psx_rgba(pal[v]) if event else DIALOGUE_RGB[v]
    return im


class Page:
    def __init__(self):
        self.lines = [[]]      # lists of glyph objects
        self.corner = []       # glyphs drawn in the box corner (flags 0xC: no advance)
        self.glyphs = 0

    def width(self, i):
        return sum(g.width for g in self.lines[i])


def paginate(wfm, words):
    pages, box, cur = [], None, Page()
    gcount = len(wfm.glyphs)
    i = 0
    while i < len(words):
        v = words[i]
        i += 1
        if v == 0xFFFA and i + 2 <= len(words):
            box = (words[i], words[i + 1])
            i += 2
        elif v in W.CTRL:
            name, na = W.CTRL[v]
            i += na
            if name == "NL":
                cur.lines.append([])
            elif name == "CLEAR":
                pages.append(cur)
                cur = Page()
        elif 0xC000 <= v < 0xF000 and (v & 0xFFF) < gcount:
            cur.corner.append(wfm.glyphs[v & 0xFFF])
        elif W.GLYPH_BASE <= v < W.GLYPH_BASE + gcount:
            cur.lines[-1].append(wfm.glyphs[v - W.GLYPH_BASE])
            cur.glyphs += 1
    if cur.glyphs or not pages or any(cur.lines):
        pages.append(cur)
    return pages, box


def fit_box(wfm, words):
    """(width, height) a BOX needs for these words: widest line + 16, 16 + 13 per line of the tallest page."""
    pages, _ = paginate(wfm, words)
    wmax = max((p.width(i) for p in pages for i in range(len(p.lines))), default=0)
    # a trailing empty line (text ends with {NL}) does not need space
    lines = max((len(p.lines) - (1 if not p.lines[-1] and len(p.lines) > 1 else 0) for p in pages), default=1)
    return wmax + 2 * MARGIN, 2 * MARGIN + LINE_STEP * max(lines, 1)


def render(wfm, words, font=None, scale=2):
    pages, box = paginate(wfm, words)
    warnings = []
    boxed = box is not None
    fit_w, fit_h = fit_box(wfm, words)
    if not boxed:
        warnings.append("Sin {BOX w h}: texto libre (menús, títulos); sin comprobación de caja.")
    bw, bh = box if boxed else (None, None)
    surfaces = []
    for pn, p in enumerate(pages, 1):
        lines = p.lines[:-1] if len(p.lines) > 1 and not p.lines[-1] else p.lines
        step = LINE_STEP
        gh = max((g.height for l in lines for g in l), default=8)
        if not boxed:
            step = max(LINE_STEP, gh + 2)
        w = bw if boxed else max([p.width(i) for i in range(len(lines))] + [8]) + 2 * MARGIN
        h = bh if boxed else 2 * MARGIN + step * max(len(lines), 1) + max(gh - step, 0)
        bad = False
        over_px = []
        for i, l in enumerate(lines):
            lw = p.width(i)
            if boxed and lw > bw - 2 * MARGIN:
                warnings.append("Página %d, línea %d: %d px de ancho; la caja admite %d (sobran %d)."
                                % (pn, i + 1, lw, bw - 2 * MARGIN, lw - (bw - 2 * MARGIN)))
                bad = True
                over_px.append(i)
        if boxed and len(lines) > (bh - 2 * MARGIN) // LINE_STEP:
            warnings.append("Página %d: %d líneas; la caja de %d px de alto admite %d."
                            % (pn, len(lines), bh, (bh - 2 * MARGIN) // LINE_STEP))
            bad = True
        if p.glyphs > GLYPH_LIMIT:
            warnings.append("Página %d: %d caracteres; el juego reserva %d por mensaje." % (pn, p.glyphs, GLYPH_LIMIT))
            bad = True
        # draw (extra room so overflowing text is visible)
        cw = max(w, max([p.width(i) + 2 * MARGIN for i in range(len(lines))] + [0]))
        ch = max(h, 2 * MARGIN + step * len(lines))
        im = Image.new("RGBA", (cw, ch), (0, 0, 0, 0))
        d = ImageDraw.Draw(im)
        d.rectangle([0, 0, w - 1, h - 1], fill=BOX_FILL, outline=RED if bad else BOX_EDGE)
        for (x, y) in ((0, 0), (w - 1, 0), (0, h - 1), (w - 1, h - 1)):
            im.putpixel((x, y), (0, 0, 0, 0))
        for i, l in enumerate(lines):
            x, y = MARGIN, TOP + i * step
            for g in l:
                if g.width and g.height:
                    gi = glyph_image(g)
                    im.alpha_composite(gi, (x, y))
                x += g.width
            if i in over_px:
                d.line([(MARGIN + bw - 2 * MARGIN if boxed else 0, y + 14), (x, y + 14)], fill=RED)
        for g in p.corner:   # corner icon (e.g. the next-page arrow); exact spot approximated
            im.alpha_composite(glyph_image(g), (max(w - g.width - 6, 0), max(h - g.height - 2, 0)))
        surfaces.append(im)
    gap = 8
    W_ = max(s.width for s in surfaces) + 2 * gap
    H_ = sum(s.height for s in surfaces) + gap * (len(surfaces) + 1)
    out = Image.new("RGBA", (W_, H_), BG)
    y = gap
    for s in surfaces:
        out.alpha_composite(s, (gap, y))
        y += s.height + gap
    if scale != 1:
        out = out.resize((out.width * scale, out.height * scale), Image.NEAREST)
    if boxed and (fit_w, fit_h) != (bw, bh):
        warnings.append("Caja ideal para este texto: {BOX %d %d} (ahora {BOX %d %d})." % (fit_w, fit_h, bw, bh))
        warnings[-1] = "i:" + warnings[-1]      # informational, not an error
    return out, warnings
