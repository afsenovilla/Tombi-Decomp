#!/usr/bin/env python3
"""Texts of the in-game pause menus of EXE/MAINn.EXE (drawPauseMenu, src/drawPauseMenu.c): the Select menu
(Item / Event / Status / Map), the Start menu (PAUSE / QUIT / LOAD) and their Yes/No questions.

They are not WFM dialogues: each entry is {s16 x, s16 y, u32 string} in a -1-ended list, the string one glyph byte a cell
(8 px; 0 = blank cell, 0xFF ends it) over a strip of 8x16 glyphs at v 144 of the HUD page (SYSTEM/AnN003.GAM, tpage 0x14)
that holds exactly the letters the language needs.  The Spanish strip (MAIN5) is full: 31 glyphs and the blank make its 32
cells (256 px), and it has no 'O' nor 'b', so the Spanish disc wrote "Elmto" for Item.  The native port draws two more
glyphs for --lang 5 from the strip itself (port/fixes.txt, drawPauseMenu): 0x20 'O' (the 'U' closed by its own bottom
turned upside down) and 0x21 'b' (the 'd' mirrored).  They show garbage on a console: a patch that uses them is port only.

Edits (the "menus" section of a *.tombipatch.json, key "EXE/MAIN5.EXE"):
  "pause": {"OLD": "NEW"}     every entry whose string is OLD gets NEW, rewritten in place: NEW must fit the room of OLD
                              (its bytes up to the next 4-byte boundary, which hold only zero padding); the entry's x
                              moves by 4 px a glyph of difference so the text keeps its centre

  pause_text.py dump MAINn.EXE        prints the entry lists
"""
import struct, sys

# glyph codes of the strip of each executable (index = code; code 0 is the blank cell)
STRIPS = {5: " ACEGILMNPRSUadegiíjlmnoprstuv¿?"}
PORT_EXTRA = {5: {"O": 0x20, "b": 0x21}}     # drawn by the native port only (see above)


class PauseError(Exception):
    pass


def _base(exe):
    """load address of file offset 0 (PS-X EXE: t_addr at 0x18, the text starts at file offset 0x800)"""
    if exe[:8] != b"PS-X EXE":
        raise PauseError("not a PS-X EXE")
    return struct.unpack_from("<I", exe, 0x18)[0] - 0x800


def encode(text, lang=5):
    strip = STRIPS.get(lang)
    if strip is None:
        raise PauseError("no glyph strip known for MAIN%d" % lang)
    out = bytearray()
    for c in text:
        if c in PORT_EXTRA.get(lang, {}):
            out.append(PORT_EXTRA[lang][c])
        elif c in strip:
            out.append(strip.index(c))
        else:
            raise PauseError("%r is not in the pause menu glyphs of MAIN%d (%s)" % (c, lang, strip.strip() + "".join(PORT_EXTRA.get(lang, {}))))
    return bytes(out)


def decode(b, lang=5):
    strip = STRIPS[lang]
    rev = {v: k for k, v in PORT_EXTRA.get(lang, {}).items()}
    return "".join(strip[c] if c < len(strip) else rev.get(c, "{%02X}" % c) for c in b)


def entries(exe):
    """[(offset of the entry, x, y, offset of the string)] of every {x, y, pointer} entry that points at a 0xFF-ended
    string of glyph bytes and sits in a -1-ended list (x = -1)"""
    base = _base(exe)
    out = []
    for o in range(0x800, len(exe) - 8, 4):
        x, y, p = struct.unpack_from("<hhI", exe, o)
        s = p - base
        if not (0 <= x < 320 and 0 <= y < 256 and 0x800 <= s < len(exe)):
            continue
        e = exe.find(b"\xff", s, s + 41)
        if e <= s or any(c > 0x3F for c in exe[s:e]):
            continue
        k = o + 8                                   # the list goes on with entries until x = -1
        while k + 8 <= len(exe) and struct.unpack_from("<h", exe, k)[0] != -1:
            xx, yy, pp = struct.unpack_from("<hhI", exe, k)
            if not (0 <= xx < 320 and 0 <= yy < 256 and 0x800 <= pp - base < len(exe)):
                break
            k += 8
        if k + 2 <= len(exe) and struct.unpack_from("<h", exe, k)[0] == -1:
            out.append((o, x, y, s))
    return out


def apply(exe, spec, lang=5):
    d = bytearray(exe)
    ents = entries(exe)
    for old, new in (spec.get("pause") or {}).items():
        ob, nb = encode(old, lang), encode(new, lang)
        hit = [(o, x, s) for o, x, y, s in ents if exe[s:s + len(ob) + 1] == ob + b"\xff"]
        if not hit:
            raise PauseError("no pause menu entry reads %r" % old)
        for o, x, s in hit:
            room = (len(ob) + 1 + 3) & ~3
            if s & 3 or any(exe[s + len(ob) + 1:s + room]):
                raise PauseError("the string of %r is not followed by padding up to a 4-byte boundary" % old)
            if len(nb) + 1 > room:
                raise PauseError("%r needs %d bytes, the room of %r is %d" % (new, len(nb) + 1, old, room))
            d[s:s + room] = (nb + b"\xff").ljust(room, b"\0")
            struct.pack_into("<h", d, o, x + 4 * (len(ob) - len(nb)))
    return bytes(d)


if __name__ == "__main__":
    if len(sys.argv) != 3 or sys.argv[1] != "dump":
        sys.exit(__doc__)
    exe = open(sys.argv[2], "rb").read()
    for o, x, y, s in entries(exe):
        e = exe.index(b"\xff", s)
        print("@%05X x %3d y %3d @%05X %s  %r" % (o, x, y, s, exe[s:e].hex(" "), decode(exe[s:e])))
