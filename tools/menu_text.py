#!/usr/bin/env python3
"""Texts of the title screen and the options menus (SYSTEM/OnPTSUB0.BIN, n = language 0..8; decomp: src/o0ptsub0/).

These are not WFM dialogues: they are glyph-id strings (s16, -3 then -1 ends one) in the overlay's data, reached through two
pointer tables (absolute addresses; the overlay's load address differs per language and is found from the data itself):

  TITLE_STRINGS[5]    titleDrawText (0 = "PRESS (START) or (x) BUTTON", 1 = copyright, 2 = TITLE_MENU_GLYPHS of the
                      carousel, 3/4 = arrows), followed by 0, 0 and TITLE_STRING_PALETTES (one palette byte per drawn glyph,
                      glyph 0 = space draws nothing and takes no byte)
  OPTIONS_STRINGS[]   optionsDrawLabel* (0..5 options rows, 6.. values, 0x14.. bottom help lines, 0x17.. controls screen)

Fonts (5-byte {u v w h big} records, TITLE_FONT / OPTIONS_FONT): a character is latin-1 minus 32 below 0x80, and
0xC0..0xFF -> 96..159 (so 'É' = 105, 'Ó' = 115, 'ó' = 147); the options font has a second, small copy of all of it at +168
(the mixed-case help lines).  A space is glyph 0 (big) / 168 (small); "{N}" writes glyph N (title: 176 = (x) button).

Edits (the "menus" section of a *.tombipatch.json, keyed by file):
  "options": {"<index>": "TEXT"}            the options strings are repacked in their own pool (index order, 4-byte aligned; a string equal
                                            to the end of one already placed points into it)
                                            and the table is rewritten: they may grow as long as the pool still holds them all
  "title":   {"<index>": {"text": T, "pal": [p, ...]}}   rewritten in place (no room to move); pal: one byte per drawn glyph,
                                            it may run past its array only into bytes that already hold the same values
  "cursorRects":    {"<i>": [x, y, w, h]}   OPTIONS_CURSOR_RECTS (green box of the options rows; 4.. also _NO_ACTUATOR)
  "optionsSprites": {"<i>": [x, y]}         OPTIONS_MENU_SPRITES (0/1 = the (x) / (o) icons of the help lines)
  "buttonSprites":  {"<i>": [x, y]}         BUTTON_CONFIG_SPRITES (same first two entries on the controls screen)
  "adjustSprites":  {"<i>": [x, y]}         SCREEN_ADJUST_SPRITES (the two icons of the screen adjust help lines)
  "optionsAfter":   {"<i>": "TEXT"}         an extra string stored right behind options string i, in no
                                            table: --lang 5 of the native port draws it as the controls screen title
Positions given in the code (x of each label) are not data: the native port moves those through port/fixes.txt.

  menu_text.py dump FILE.BIN          prints the tables
"""
import re, struct, sys

TS_TO_RECTS = -0x6C        # OPTIONS_CURSOR_RECTS relative to TITLE_STRINGS (the same in every language)
TS_TO_OPTIONS = 0x30
TS_TO_OPT_SPRITES = 0x19C
TS_TO_BTN_SPRITES = 0x1CC
TS_TO_ADJ_SPRITES = 0x37C
SMALL = 168


class MenuError(Exception):
    pass


def _u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


def _string(d, o):
    g = []
    while True:
        c = struct.unpack_from("<h", d, o + 2 * len(g))[0]
        g.append(c)
        if c == -1:
            return g


class Layout:
    def __init__(self, data):
        d = self.d = bytes(data)
        cop = d.find(struct.pack("<4h", 32, 0, 160, 161))        # "(c) 1997 ..." = TITLE_STRINGS[1]
        if cop < 0:
            raise MenuError("not a title/options overlay (OnPTSUB0.BIN)")
        for t in range(0, len(d) - 40, 4):
            v = struct.unpack_from("<10I", d, t)
            if v[5] == 0 and v[6] == 0 and v[1] - cop == v[1] - cop and all(0x80000000 < x < 0x80200000 for x in v[:5] + v[7:]):
                base = v[1] - cop
                if all(0 <= x - base < len(d) for x in v[:5] + v[7:]):
                    break
        else:
            raise MenuError("TITLE_STRINGS not found")
        self.base, self.ts = base, t
        self.title = list(v[:5])
        self.title_pal = list(v[7:10])
        o, self.options = t + TS_TO_OPTIONS, []
        while 0 <= _u32(d, o) - base < len(d):
            self.options.append(_u32(d, o))
            o += 4
        # options font: right after the title strings, up to the first options string
        self.font_t = None
        self.font_o = (self.title[4] - base + 2 * len(_string(d, self.title[4] - base)) + 3) & ~3
        self.font_o_n = (min(self.options) - base - self.font_o) // 5

    def off(self, a):
        return a - self.base

    def string(self, a):
        return _string(self.d, self.off(a))

    def width_o(self, glyphs):
        w = 0
        for c in glyphs:
            if c >= 0:
                f = self.font_o + 5 * c
                if self.d[f + 4] == 0:
                    w += self.d[f + 2]
        return w


def encode(text, small=False):
    """Glyph ids of text (see the module doc). small: the options font's small copy (+168)."""
    out, i = [], 0
    while i < len(text):
        m = re.match(r"\{(-?\d+)\}", text[i:])
        if m:
            out.append(int(m.group(1)))
            i += m.end()
            continue
        ch = text[i]
        i += 1
        o = ord(ch)
        if ch == " ":
            c = 0
        elif 0x20 < o < 0x7F:
            c = o - 32
        elif 0xC0 <= o <= 0xFF:
            c = o - 0xC0 + 96
            if ch in "ÈÉ":
                c ^= 1          # the font draws 104 with the acute accent and 105 with the grave one (É = 104)
        else:
            raise MenuError("character %r has no glyph in the menu fonts" % ch)
        out.append(c + SMALL if small else c)
    return out + [-3, -1]


def decode(glyphs):
    s = []
    for c in glyphs:
        if c in (-3, -1):
            continue
        k = c - SMALL if c >= SMALL and c < SMALL + 160 else c
        if k == 0:
            s.append(" ")
        elif 0 < k < 95:
            s.append(chr(k + 32))
        elif 96 <= k < 160:
            s.append(chr((k ^ 1 if k in (104, 105) else k) - 96 + 0xC0))
        else:
            s.append("{%d}" % c)
    return "".join(s)


def _short(glyphs):
    """An options string without the -3 (line break) before its -1 when that saves a word of the 4-byte aligned pool:
    titleDrawString stops at the -1, the break only moved the pen after the last glyph."""
    return glyphs[:-2] + [-1] if len(glyphs) % 2 else glyphs


def _pack(glyphs):
    return struct.pack("<%dh" % len(glyphs), *glyphs)


def apply(data, spec):
    """Returns data with the edits of spec (see the module doc) applied. Raises MenuError."""
    L = Layout(data)
    d = bytearray(data)
    base = L.base
    # options strings: repack the whole pool
    opt = spec.get("options") or {}
    if opt:
        old = [L.string(a) for a in L.options]
        new = list(old)
        for k, t in opt.items():
            i = int(k, 0)
            if not 0 <= i < len(new):
                raise MenuError("options string %s does not exist (0..%d)" % (k, len(new) - 1))
            new[i] = _short(encode(t, small=any(c >= SMALL for c in old[i] if c >= 0)))
        lo = min(L.options) - base
        hi = (max(a - base + 2 * len(g) for a, g in zip(L.options, old)) + 3) & ~3
        after = {int(k, 0): v for k, v in (spec.get("optionsAfter") or {}).items()}
        pool, ptrs = bytearray(), []
        for i, g in enumerate(new):
            b = _pack(g)
            # a string that ends another one already placed points into it; every string starts 4-byte aligned like the
            # original ones (the native port's --lang only takes aligned words for pointers, port2/runtime/lang.c)
            at = next((k for k in range(0, len(pool) - len(b) + 1, 4) if pool[k:k + len(b)] == b), -1) if i not in after else -1
            if at < 0:
                at = len(pool)
                pool += b + b"\xff\xff" * ((-len(b) // 2) % 2)
            ptrs.append(base + lo + at)
            if i in after:      # an extra string right behind string i (not in the table: the native port finds it there)
                b = _pack(_short(encode(after[i], small=any(c >= SMALL for c in g if c >= 0))))
                pool += b + b"\xff\xff" * ((-len(b) // 2) % 2)
        if len(pool) > hi - lo:
            raise MenuError("options strings: %d bytes, the pool holds %d" % (len(pool), hi - lo))
        d[lo:hi] = pool + b"\xff" * (hi - lo - len(pool))
        for i, p in enumerate(ptrs):
            struct.pack_into("<I", d, L.ts + TS_TO_OPTIONS + 4 * i, p)
    # title strings: in place
    for k, e in (spec.get("title") or {}).items():
        i = int(k, 0)
        a = L.off(L.title[i])
        room = min([L.off(x) for x in L.title if L.off(x) > a] + [len(d)]) - a
        g = encode(e["text"])
        if 2 * len(g) > room:
            raise MenuError("title string %d: %d bytes, %d of room" % (i, 2 * len(g), room))
        d[a:a + room] = _pack(g) + b"\xff" * (room - 2 * len(g))
        if "pal" in e:
            pal = bytes(e["pal"])
            if len(pal) != sum(1 for c in g if c > 0):
                raise MenuError("title string %d: %d palettes for %d glyphs" % (i, len(pal), sum(1 for c in g if c > 0)))
            p = L.off(L.title_pal[i])
            end = min([L.off(x) for x in L.title_pal if L.off(x) > p] + [p + len(pal)])
            for j, v in enumerate(pal):
                if p + j < end:
                    d[p + j] = v
                elif d[p + j] != v:
                    raise MenuError("title string %d: palette %d runs past its array into a different value" % (i, j))
    for key, at, n in (("cursorRects", L.ts + TS_TO_RECTS, 4), ("optionsSprites", L.ts + TS_TO_OPT_SPRITES, 2),
                       ("buttonSprites", L.ts + TS_TO_BTN_SPRITES, 2), ("adjustSprites", L.ts + TS_TO_ADJ_SPRITES, 2)):
        size = 8 if n == 4 else 16
        for k, v in (spec.get(key) or {}).items():
            if len(v) != n:
                raise MenuError("%s %s: %d values expected" % (key, k, n))
            struct.pack_into("<%dh" % n, d, at + size * int(k, 0), *v)
    return bytes(d)


def dump(data):
    L = Layout(data)
    print("load address %08x, TITLE_STRINGS at file %#x, options font at %#x" % (L.base, L.ts, L.font_o))
    for i, a in enumerate(L.title):
        print("title %d: %r" % (i, decode(L.string(a))))
    for i, a in enumerate(L.options):
        g = L.string(a)
        print("options %#x: %r (%d px)" % (i, decode(g), L.width_o(g)))
    print("cursorRects", [struct.unpack_from("<4h", L.d, L.ts + TS_TO_RECTS + 8 * i) for i in range(8)])


if __name__ == "__main__":
    if len(sys.argv) == 3 and sys.argv[1] == "dump":
        dump(open(sys.argv[2], "rb").read())
    else:
        print(__doc__)
