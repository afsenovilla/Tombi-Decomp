#!/usr/bin/env python3
"""Texts of the memory card screen (LOAD / SAVE, SYSTEM/DnSPSUB2.BIN, n = language 0..8; decomp: src/d0spsub2/).

Not WFM dialogues: glyph-id strings (u16, 0xFFFD = line break, 0xFFFF ends one) in the overlay's data, drawn by
memCardDrawString with the 5-byte {u v w h flag} glyph table MEMCARD_FONT.  A character is latin-1 minus 32 below 0x80
and 0xC0..0xFF -> 0x60..0x9F ('á' = 0x81, 'ñ' = 0x91); '¿' is 0xA7; "{N}" writes glyph N (hex or decimal as in
Python: {0xAB} = the (x) button, {0xA8} = the (o) button); a line feed writes the 0xFFFD break.

Two pointer tables (absolute addresses; the overlay's load address differs per language and is found from the data):
  MESSAGES[22]     memCardDrawMessage n (0 "load failed" .. 17 slot prompt, 18 = MEMCARD_FOOTER_STRING, the two help
                   lines at the bottom: in the footer (mode 2) glyph 0xA8 also starts the second line, 20 / 21 "save /
                   load complete")
  LOCATIONS[136]   memCardDrawFileLocation: the place names of the save list (mode 0: 0xFFFD is skipped, so a name
                   broken in two lines needs its own space before the break)

Edits (the "menus" section of a *.tombipatch.json, keyed by file, like tools/menu_text.py for OnPTSUB0):
  "messages":  {"<i>": "TEXT"}             repacked in their own pool (index order, 4-byte aligned starts as the native port's
                                           --lang relocation wants) and the table rewritten: may grow while the pool holds them
  "locations": {"<i>": "TEXT"}             rewritten in place (the room up to the next string); every entry of the table
                                           pointing at the same string changes with it
  "font":      {"<glyph>": [u, v, w, h, flag]}   MEMCARD_FONT records (0xA0.. = 12x12 button icons: 0xA8 (o), 0xAB (x);
                                           w = 0 draws nothing and does not advance)
  "iconParts": {"<i>": [x, y, u, v, w, h, cx, cy]}  MEMCARD_FILE_ICON_PARTS (0..2 = the "LV." of a save entry, 8x16 cells
                                           of the big font: row v 16 = '@'..'_', row v 32 starts with 'a' at u 16)

  memcard_text.py dump FILE.BIN          prints the tables
"""
import re, struct, sys

N_MESSAGES = 22
FONT_SIG = bytes([64, 0, 8, 8, 0, 72, 0, 8, 8, 0])       # MEMCARD_FONT glyphs 8 and 9 (the same in every language)
ICON_SIG = struct.pack("<2h", 32, 2)                       # first record of MEMCARD_FILE_ICON_PARTS: x 32, y 2 ...
ICON_SIG2 = struct.pack("<4h", 8, 16, 304, 496)            # ... w 8, h 16, clut (304, 496)


class MemcardError(Exception):
    pass


def _h(d, o):
    return struct.unpack_from("<H", d, o)[0]


class Layout:
    def __init__(self, data):
        d = self.d = bytes(data)
        starts = set(o for o in range(4, len(d) - 2, 2) if _h(d, o - 2) == 0xFFFF and _h(d, o) != 0xFFFF)
        self.base = self.msg = None
        for i in range(0, len(d) - 4 * N_MESSAGES, 4):
            t = struct.unpack_from("<%dI" % N_MESSAGES, d, i)
            if not all(0x800E0000 < x < 0x80100000 for x in t) or any(t[k] >= t[k + 1] for k in range(N_MESSAGES - 1)):
                continue
            for base in sorted({t[0] - s for s in starts}):
                offs = [x - base for x in t]
                if all(4 <= o < len(d) - 2 and (o in starts or (_h(d, o - 2) == 0 and _h(d, o - 4) == 0xFFFF)) for o in offs):
                    self.base, self.msg = base, i
                    break
            if self.msg is not None:
                break
        if self.msg is None:
            raise MemcardError("message table not found")
        self.messages = list(struct.unpack_from("<%dI" % N_MESSAGES, d, self.msg))
        # the place names: every string an aligned word of the data points at, below the message pool (the 2-level table
        # MEMCARD_LOCATION_STRINGS[area][room] of memCardDrawFileLocation; one string can be shared by several rooms)
        lo = min(self.messages) - self.base
        refs = {}
        for j in range(self.msg + 4 * N_MESSAGES, len(d) - 3, 4):     # the tables follow the message table
            o = struct.unpack_from("<I", d, j)[0] - self.base
            if 4 <= o < lo and (o in starts or (_h(d, o - 2) == 0 and _h(d, o - 4) == 0xFFFF)):
                refs.setdefault(o + self.base, []).append(j)
        self.locations = sorted(refs)
        self.location_refs = refs
        self.starts = sorted(set(self.messages + self.locations))
        f = d.find(FONT_SIG)
        if f < 0:
            raise MemcardError("glyph table not found")
        self.font = f - 8 * 5
        k = 0
        self.icon = None
        while True:
            k = d.find(ICON_SIG, k)
            if k < 0:
                break
            if k % 2 == 0 and d[k + 8:k + 16] == ICON_SIG2:
                self.icon = k
                break
            k += 2
        if self.icon is None:
            raise MemcardError("icon part list not found")

    def off(self, a):
        return a - self.base

    def string(self, a):
        o, g = self.off(a), []
        while True:
            c = _h(self.d, o + 2 * len(g))
            g.append(c)
            if c == 0xFFFF:
                return g

    def room(self, a):
        """Bytes from string a up to the next string (or the data that follows its terminator)."""
        o = self.off(a)
        nxt = [self.off(x) for x in self.starts if self.off(x) > o]
        end = o + 2 * len(self.string(a))
        return max(end, min(nxt)) - o if nxt and min(nxt) - o <= end - o + 4 else end - o


def encode(text):
    out, i = [], 0
    while i < len(text):
        m = re.match(r"\{(0x[0-9A-Fa-f]+|\d+)\}", text[i:])
        if m:
            out.append(int(m.group(1), 0))
            i += m.end()
            continue
        ch = text[i]
        i += 1
        o = ord(ch)
        if ch == "\n":
            c = 0xFFFD
        elif 0x20 <= o < 0x7F:
            c = o - 32
        elif ch == "¿":
            c = 0xA7
        elif 0xC0 <= o <= 0xFF:
            c = o - 0x60
        else:
            raise MemcardError("character %r has no glyph in the memory card font" % ch)
        out.append(c)
    return out + [0xFFFF]


def decode(glyphs):
    s = []
    for c in glyphs:
        if c == 0xFFFF:
            break
        if c == 0xFFFD:
            s.append("\n")
        elif c < 0x5F:
            s.append(chr(c + 32))
        elif c == 0xA7:
            s.append("¿")
        elif 0x60 <= c < 0xA0:
            s.append(chr(c + 0x60))
        else:
            s.append("{0x%X}" % c)
    return "".join(s)


def _pack(g):
    return struct.pack("<%dH" % len(g), *g)


def apply(data, spec):
    """Returns data with the edits of spec (see the module doc) applied. Raises MemcardError."""
    L = Layout(data)
    d = bytearray(data)
    base = L.base
    msg = spec.get("messages") or {}
    if msg:
        old = [L.string(a) for a in L.messages]
        new = list(old)
        for k, t in msg.items():
            i = int(k, 0)
            if not 0 <= i < N_MESSAGES:
                raise MemcardError("message %s does not exist (0..%d)" % (k, N_MESSAGES - 1))
            new[i] = encode(t)
        lo = min(L.messages) - base
        hi = (max(a - base + 2 * len(g) for a, g in zip(L.messages, old)) + 3) & ~3
        if any(lo <= L.off(a) < hi for a in L.locations):
            raise MemcardError("place names inside the message pool")
        pool, ptrs = bytearray(), []
        for g in new:
            b = _pack(g)
            ptrs.append(base + lo + len(pool))
            pool += b + b"\x00\x00" * ((-len(b) // 2) % 2)
        if len(pool) > hi - lo:
            raise MemcardError("messages: %d bytes, the pool holds %d" % (len(pool), hi - lo))
        d[lo:hi] = pool + b"\xff" * (hi - lo - len(pool))
        for i, p in enumerate(ptrs):
            struct.pack_into("<I", d, L.msg + 4 * i, p)
    for k, t in (spec.get("locations") or {}).items():
        i = int(k, 0)
        if not 0 <= i < len(L.locations):
            raise MemcardError("place name %s does not exist (0..%d)" % (k, len(L.locations) - 1))
        a = L.locations[i]
        g = encode(t)
        room = L.room(a)
        if 2 * len(g) > room:
            raise MemcardError("place name %d: %d bytes, %d of room" % (i, 2 * len(g), room))
        o = L.off(a)
        d[o:o + room] = _pack(g) + b"\xff" * (room - 2 * len(g))
    for k, r in (spec.get("font") or {}).items():
        i = int(k, 0)
        if len(r) != 5 or not 0 <= i < 0x100:
            raise MemcardError("font record %s: [u, v, w, h, flag]" % k)
        d[L.font + 5 * i:L.font + 5 * i + 5] = bytes(r)
    for k, r in (spec.get("iconParts") or {}).items():
        i = int(k, 0)
        if len(r) != 8 or not 0 <= i < 6:
            raise MemcardError("icon part %s: [x, y, u, v, w, h, cx, cy]" % k)
        struct.pack_into("<8h", d, L.icon + 16 * i, *r)
    if len(d) != len(data):
        raise MemcardError("size changed")
    return bytes(d)


def dump(data):
    L = Layout(data)
    print("base %08X  messages @%X  place names %d  font @%X  icon parts @%X" % (
        L.base, L.msg, len(L.locations), L.font, L.icon))
    for i, a in enumerate(L.messages):
        print("M%-3d %r" % (i, decode(L.string(a))))
    for i, a in enumerate(L.locations):
        print("L%-3d @%X %r" % (i, L.off(a), decode(L.string(a))))
    for i in range(6):
        print("P%d %s" % (i, struct.unpack_from("<8h", L.d, L.icon + 16 * i)))


def main(argv):
    if len(argv) == 3 and argv[1] == "dump":
        dump(open(argv[2], "rb").read())
        return 0
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
