#!/usr/bin/env python3
"""Pure-Python WFM3 codec (dialogue text + glyph table) for the Tomba! PAL files; no external tools needed.

File layout (little endian):
  "WFM3", u32 padding, u32 dpt (offset of the dialogue pointer table), u16 n_dialogues, u16 n_glyphs, 128 reserved bytes
  u16 glyph_ptr[n_glyphs]            offsets from the start of the file
  glyphs: u16 clut, u16 height, u16 width, u16 handakuten, then (width*height+1)//2 bytes of 4bpp pixels (low nibble first)
  at dpt: u16 dialogue_ptr[n_dialogues]  offsets from dpt; each dialogue is a stream of u16 words ended by 0xFFFE / 0xFFFF
  (a few files end with an unterminated stream) and, rarely, trailing bytes.
Glyph header, as the game reads it: u16 w in VRAM words (4 px each; stored here as `clut`), u16 height, u16 advance (stored
as `width`; equals 4*w in all 36,046 glyphs), u16 handakuten. So the `font` of a dialogue, (height, clut), is really (height, w).
Words: 0x8000 + n = glyph n of the file (the engine uses the low 12 bits as id and the top nibble as flags: 8 normal, 0xC drawn
in the corner of the box without moving the cursor, 0xE no cursor advance; the 0xC0xx words, e.g. {C04D} {C052}, are therefore
corner icons such as the "next page" arrow); 0xFFF2..0xFFFD are control codes (below); anything else is kept as a raw word.
There are no character codes in the file: a glyph is a bitmap. tools/wfm_charmap.json maps bitmap hashes to characters
(made by tools/wfm_charmap_gen.py), so text can be shown and written as normal strings.

Text syntax (what words_to_text writes and text_to_words reads; real line feeds are cosmetic and ignored when encoding):
  plain characters            glyphs with a known character
  {G:n}                       glyph n without a character (or one the encoder could not pick back); keeps the file lossless
  {XXXX}                      any other raw word, four hex digits (for example {C052})
  {NL} {CLEAR} {WAIT} {HALT} {CHOICE} {F4}            0xFFFD 0xFFFB 0xFFFC 0xFFF3 0xFFF5 0xFFF4
  {BOX w h} {MOVE dx dy} {VOICE tone tail}             0xFFFA 0xFFF6 0xFFF8   (two arguments)
  {COLOR n} {PAUSE frames} {POSE n}                    0xFFF7 0xFFF9 0xFFF2   (one argument)
The terminator (0xFFFE close / 0xFFFF end / none) is a property of the dialogue, not part of the text.

  python3 tools/wfm_py.py --check [PATH]     decode+encode every WFM and count the byte-identical ones
"""
import hashlib, json, os, re, struct, sys

MAGIC = b"WFM3"
GLYPH_BASE = 0x8000
CTRL = {  # code: (name, number of arguments)
    0xFFF2: ("POSE", 1), 0xFFF3: ("HALT", 0), 0xFFF4: ("F4", 0), 0xFFF5: ("CHOICE", 0), 0xFFF6: ("MOVE", 2),
    0xFFF7: ("COLOR", 1), 0xFFF8: ("VOICE", 2), 0xFFF9: ("PAUSE", 1), 0xFFFA: ("BOX", 2), 0xFFFB: ("CLEAR", 0),
    0xFFFC: ("WAIT", 0), 0xFFFD: ("NL", 0),
}
NAME2CODE = {v[0]: k for k, v in CTRL.items()}
TERMINATORS = (0xFFFE, 0xFFFF)
NEWLINE_AFTER = ("NL", "CLEAR", "BOX")   # cosmetic line feed written after these tags

CHARMAP_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "wfm_charmap.json")


class WfmError(Exception):
    pass


class UnknownGlyph(WfmError):
    """A character has no glyph in the file and none could be borrowed from another file."""


def glyph_key(height, width, image):
    return "%d:%d:%s" % (height, width, hashlib.sha1(bytes(image)).hexdigest()[:12])


_charmap = None


def charmap():
    global _charmap
    if _charmap is None:
        cands = [CHARMAP_PATH, os.path.join(getattr(sys, "_MEIPASS", ""), "wfm_charmap.json")]
        for p in cands:
            if os.path.isfile(p):
                _charmap = json.load(open(p, encoding="utf-8"))
                break
        else:
            raise WfmError("wfm_charmap.json not found (tools/wfm_charmap_gen.py makes it)")
    return _charmap


class Glyph:
    __slots__ = ("clut", "height", "width", "hand", "image", "char")

    def __init__(self, clut, height, width, hand, image):
        self.clut, self.height, self.width, self.hand, self.image = clut, height, width, hand, bytes(image)
        self.char = charmap().get(glyph_key(height, width, self.image))

    def pixels(self):
        """Row-major list of palette indices (0..15)."""
        px = []
        for b in self.image:
            px.append(b & 15)
            px.append(b >> 4)
        return px[:self.width * self.height]

    def raw(self):
        return struct.pack("<4H", self.clut, self.height, self.width, self.hand) + self.image


class Dialogue:
    """words: the stream without its terminator; term: 0xFFFE, 0xFFFF or None (unterminated); font: (height, clut)."""

    def __init__(self, words, term, font):
        self.words, self.term, self.font = list(words), term, font


class Wfm:
    pass


def _image_size(w, h):
    return 0 if not w or not h else (w * h + 1) // 2


def decode(data):
    if data[:4] != MAGIC:
        raise WfmError("not a WFM3 file")
    pad, dpt, nd, ng = struct.unpack_from("<IIHH", data, 4)
    w = Wfm()
    w.padding, w.reserved = pad, bytes(data[16:144])
    base = 144 + 2 * ng
    if dpt < base or dpt + 2 * nd > len(data):
        raise WfmError("bad header")
    w.glyph_ptrs = list(struct.unpack_from("<%dH" % ng, data, 144))
    w.glyph_blob = bytes(data[base:dpt])
    w.glyphs = []
    for p in w.glyph_ptrs:
        clut, h, wd, hand = struct.unpack_from("<4H", data, p)
        n = _image_size(wd, h)
        w.glyphs.append(Glyph(clut, h, wd, hand, data[p + 8:p + 8 + n]))
    ptrs = struct.unpack_from("<%dH" % nd, data, dpt)
    w.dialogues = []
    end = dpt + 2 * nd
    for p in ptrs:
        pos = dpt + p
        words = []
        term = None
        while pos + 2 <= len(data):
            v = struct.unpack_from("<H", data, pos)[0]
            pos += 2
            if v in TERMINATORS:
                term = v
                break
            words.append(v)
        end = max(end, pos)
        w.dialogues.append(Dialogue(words, term, None))
    w.trailing = bytes(data[end:])
    w.charidx = _charindex(w.glyphs)
    for d in w.dialogues:
        d.font = detect_font(w, d.words)
    return w


def encode(w):
    ng, nd = len(w.glyphs), len(w.dialogues)
    base = 144 + 2 * ng
    ptrs = list(w.glyph_ptrs)
    blob = w.glyph_blob
    extra = len(w.glyphs) - len(ptrs)       # glyphs added by the editor
    if extra:
        ptrs = [p + 2 * extra for p in ptrs]
        for g in w.glyphs[len(ptrs):]:
            ptrs.append(base + len(blob))
            blob += g.raw()
    if any(p > 0xFFFF for p in ptrs) or base + len(blob) > 0xFFFF:
        raise WfmError("glyph table does not fit in 64 KB")
    dpt = base + len(blob)
    body, dptrs = b"", []
    for d in w.dialogues:
        dptrs.append(2 * nd + len(body))
        ws = d.words + ([d.term] if d.term is not None else [])
        body += struct.pack("<%dH" % len(ws), *ws)
    if 2 * nd + len(body) > 0xFFFF + 2:
        raise WfmError("dialogue data does not fit in 64 KB")
    return (MAGIC + struct.pack("<IIHH", w.padding, dpt, nd, ng) + w.reserved + struct.pack("<%dH" % ng, *ptrs)
            + blob + struct.pack("<%dH" % nd, *dptrs) + body + w.trailing)


# ---- characters <-> glyphs ----------------------------------------------------------------------------------------

def _charindex(glyphs):
    idx = {}
    for i, g in enumerate(glyphs):
        if g.char:
            idx.setdefault(g.char, []).append(i)
    return idx


def resolve(w, ch, font):
    """Glyph index that writes character ch for a dialogue whose font is (height, clut); None if the file has none."""
    c = w.charidx.get(ch)
    if not c:
        return None
    h, clut = font
    for i in c:
        if w.glyphs[i].height == h and w.glyphs[i].clut == clut:
            return i
    for i in c:
        if w.glyphs[i].height == h:
            return i
    return c[0]


def detect_font(w, words):
    """Most common (height, clut) among the glyphs of the stream; the default of the file when there are none."""
    seen = {}
    for v in words:
        if GLYPH_BASE <= v < GLYPH_BASE + len(w.glyphs):
            g = w.glyphs[v - GLYPH_BASE]
            if g.width and g.height:
                k = (g.height, g.clut)
                seen[k] = seen.get(k, 0) + 1
    if seen:
        return max(seen, key=lambda k: (seen[k], -list(seen).index(k)))
    ks = {}
    for g in w.glyphs:
        if g.height == 8:
            ks[g.clut] = ks.get(g.clut, 0) + 1
    return (8, max(ks, key=ks.get)) if ks else ((w.glyphs[0].height, w.glyphs[0].clut) if w.glyphs else (8, 0))


def words_to_text(w, words, font):
    out, i, n = [], 0, len(words)
    while i < n:
        v = words[i]
        i += 1
        if v in CTRL and i + CTRL[v][1] <= n:
            name, na = CTRL[v]
            args = words[i:i + na]
            i += na
            out.append("{%s%s}" % (name, "".join(" %d" % a for a in args)))
            if name in NEWLINE_AFTER:
                out.append("\n")
        elif GLYPH_BASE <= v < GLYPH_BASE + len(w.glyphs):
            gi = v - GLYPH_BASE
            ch = w.glyphs[gi].char
            if ch and ch not in "{}\n" and resolve(w, ch, font) == gi:
                out.append(ch)
            else:
                out.append("{G:%d}" % gi)
        else:
            out.append("{%04X}" % v)
    return "".join(out)


_TAG = re.compile(r"\{([A-Za-z0-9]+)((?::\d+)|(?: +-?\d+)*)\}")


def text_to_words(w, text, font, pool=None):
    """Inverse of words_to_text. pool (GlyphPool) lets characters missing from this file borrow a glyph from another file
    (it is appended to the glyph table). Raises UnknownGlyph / WfmError with a readable message."""
    words, pos = [], 0
    text = text.replace("\r", "").replace("\n", "")
    while pos < len(text):
        c = text[pos]
        if c == "{":
            m = _TAG.match(text, pos)
            if not m:
                raise WfmError("etiqueta mal formada cerca de %r" % text[pos:pos + 12])
            name, arg = m.group(1), m.group(2)
            pos = m.end()
            if name == "G" and arg.startswith(":"):
                gi = int(arg[1:])
                if gi >= len(w.glyphs):
                    raise WfmError("{G:%d}: el archivo solo tiene %d glifos" % (gi, len(w.glyphs)))
                words.append(GLYPH_BASE + gi)
            elif not arg and re.fullmatch(r"[0-9A-Fa-f]{4}", name):
                words.append(int(name, 16))
            elif name.upper() in NAME2CODE and not arg.startswith(":"):
                code = NAME2CODE[name.upper()]
                args = [int(a) for a in arg.split()]
                if len(args) != CTRL[code][1]:
                    raise WfmError("{%s} necesita %d argumento(s)" % (name.upper(), CTRL[code][1]))
                if any(not 0 <= a <= 0xFFFF for a in args):
                    raise WfmError("{%s}: argumento fuera de 0..65535" % name.upper())
                words.append(code)
                words.extend(args)
            else:
                raise WfmError("etiqueta desconocida {%s%s}" % (name, arg))
            continue
        gi = resolve(w, c, font)
        if gi is None:
            gi = borrow(w, c, font, pool)
        words.append(GLYPH_BASE + gi)
        pos += 1
    return words


def borrow(w, ch, font, pool):
    g = pool.find(ch, font) if pool else None
    if g is None:
        raise UnknownGlyph("el carácter %r (U+%04X) no tiene glifo en este archivo ni en ningún otro del disco" % (ch, ord(ch)))
    ng = Glyph(g.clut, g.height, g.width, g.hand, g.image)
    ng.char = ch
    w.glyphs.append(ng)
    w.charidx.setdefault(ch, []).append(len(w.glyphs) - 1)
    return len(w.glyphs) - 1


class GlyphPool:
    """Glyph bitmaps of every character seen in a set of decoded files, to add characters a file lacks."""

    def __init__(self):
        self.by = {}

    def add(self, w):
        for g in w.glyphs:
            if g.char and g.width and g.height:
                self.by.setdefault(g.char, {}).setdefault(g.height, g)

    def find(self, ch, font):
        c = self.by.get(ch)
        if not c:
            return None
        return c.get(font[0]) or None


# ---- self test ----------------------------------------------------------------------------------------------------

def check(path):
    files = [path] if os.path.isfile(path) else sorted(
        os.path.join(d, f) for d, _, fs in os.walk(path) for f in fs if f.upper().endswith(".WFM"))
    ok = textok = 0
    for f in files:
        data = open(f, "rb").read()
        try:
            w = decode(data)
            same = encode(w) == data
            t = all(text_to_words(w, words_to_text(w, d.words, d.font), d.font) == d.words for d in w.dialogues)
        except Exception as e:
            print("%s: %s" % (f, e))
            continue
        ok += same
        textok += t
        if not same or not t:
            print("%s: %s" % (f, "bytes differ" if not same else "text differs"))
    print("identical: %d/%d (text round trip %d/%d)" % (ok, len(files), textok, len(files)))
    return 0 if ok == len(files) == textok else 1


if __name__ == "__main__":
    a = [x for x in sys.argv[1:] if x != "--check"]
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    sys.exit(check(a[0] if a else os.path.join(root, "game", "disc", "fs")))
