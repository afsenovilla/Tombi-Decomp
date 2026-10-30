#!/usr/bin/env python3
"""Item names of the sub-screen lists (SYSTEM/DnSPSUB.BIN, DnSPSUB2.BIN, DnSPSUB3.BIN; n = language 0..8).

The inventory / pause lists do not read the WFM item names (MFNT #n): each sub-screen overlay keeps its own table of
glyph-id strings (s16; a character is latin-1 + 140 below 0x80 and + 74 from 0xC0; -3 then -1 end one; 0 pads to the
next string). So an item renamed in the dialogues keeps its old name in the lists unless it is renamed here too.

Edits (the "menus" section of a *.tombipatch.json, key "itemNames" under each of the three files):
  "itemNames": {"OLD": "NEW"}     rewritten in place; NEW must fit in the room of OLD (its length, padding included),
                                  every copy of OLD in the file changes

  subscreen_text.py dump FILE.BIN      prints the strings
"""
import struct, sys


class SubscreenError(Exception):
    pass


def encode(text):
    out = []
    for c in text:
        o = ord(c)
        if 32 <= o < 0x80:
            out.append(o + 140)
        elif 0xC0 <= o <= 0xFF:
            out.append(o + 74)
        else:
            raise SubscreenError("character %r not in the sub-screen font" % c)
    return b"".join(struct.pack("<h", g) for g in out)


def _end(data, o):
    """Offset of the -3, -1 terminator of the string starting at o."""
    while o + 4 <= len(data):
        if struct.unpack_from("<2h", data, o) == (-3, -1):
            return o
        o += 2
    raise SubscreenError("unterminated string")


def apply_names(data, names):
    d = bytearray(data)
    for old, new in names.items():
        pat = encode(old) + struct.pack("<2h", -3, -1)
        hits = [i for i in range(0, len(d) - len(pat) + 1, 2) if d[i:i + len(pat)] == pat]
        if not hits:
            raise SubscreenError("item name %r not found" % old)
        for i in hits:
            room = len(encode(old))
            j = i + room + 4                                  # zero padding after the terminator is room too
            while j + 2 <= len(d) and d[j:j + 2] == b"\0\0":
                room += 2; j += 2
            body = encode(new)
            if len(body) > room:
                raise SubscreenError("%r does not fit in the room of %r" % (new, old))
            d[i:i + room + 4] = body + struct.pack("<2h", -3, -1) + b"\0" * (room - len(body))
    return bytes(d)


def dump(data):
    s, o = [], 0
    while o + 2 <= len(data):
        g = struct.unpack_from("<h", data, o)[0]
        if 172 <= g < 267 or 266 <= g <= 329:
            j = o
            try:
                e = _end(data, o)
            except SubscreenError:
                break
            gl = struct.unpack_from("<%dh" % ((e - o) // 2), data, o)
            if e - o >= 6 and all(172 <= x <= 329 for x in gl):
                s.append((o, "".join(chr(x - 140) if x < 268 else chr(x - 74) for x in gl)))
                o = e + 4
                continue
        o += 2
    return s


if __name__ == "__main__":
    if len(sys.argv) == 3 and sys.argv[1] == "dump":
        for o, t in dump(open(sys.argv[2], "rb").read()):
            print("@%X %s" % (o, t))
    else:
        print(__doc__)
