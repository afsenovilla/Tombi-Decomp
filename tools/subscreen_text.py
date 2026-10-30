#!/usr/bin/env python3
"""Item names of the sub-screen lists (SYSTEM/DnSPSUB.BIN, DnSPSUB2.BIN, DnSPSUB3.BIN; n = language 0..8).

The inventory / pause lists do not read the WFM item names (MFNT #n): each sub-screen overlay keeps its own table of
glyph-id strings (s16; a character is latin-1 + 140 below 0x80 and + 74 from 0xC0; -3 then -1 end one; 0 pads to the
next string). So an item renamed in the dialogues keeps its old name in the lists unless it is renamed here too.

Edits (the "menus" section of a *.tombipatch.json, key "itemNames" under each of the three files):
  "itemNames": {"OLD": "NEW"}     every copy of OLD changes; in place when NEW fits the room of OLD, otherwise the pool
                                  of pointed strings around them is repacked (identical strings shared) and its pointers
                                  rewritten

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


def _base(d, anchor):
    """Load address of the overlay: the shift that turns the most pointer words into string starts."""
    import collections
    words = set(struct.unpack_from("<%dI" % (len(d) // 4), d))
    offs = [o for o, _ in dump(d)]
    c = collections.Counter(w - o for o in offs[::5] for w in words if 0x80000000 <= w < 0x80200000 and (w - o) % 4 == 0)
    best = max(((sum(1 for o in offs if cand + o in words), cand) for cand, _ in c.most_common(10)), default=(0, 0))
    if best[0] < len(offs) // 2:
        raise SubscreenError("overlay load address not found (the names are not reached through pointers here)")
    return best[1]


def _is_str(d, o):
    if o & 1 or o + 2 > len(d):
        return False
    n = 0
    while o + 2 <= len(d) and n < 64:
        g = struct.unpack_from("<h", d, o)[0]
        if g == -1:
            return n > 1
        if not (g == -3 or 160 <= g < 340):
            return False
        o += 2; n += 1
    return False


def _strend(d, o):
    while struct.unpack_from("<h", d, o)[0] != -1:
        o += 2
    return o + 2


def apply_names(data, names):
    """In place when every new name fits its room; otherwise the whole pool of pointed strings around the item names
    is repacked (identical strings shared, starts 4-byte aligned) and every pointer to it rewritten."""
    try:
        return _in_place(data, names)
    except SubscreenError as ex:
        if "does not fit" not in str(ex):
            raise
    d = bytearray(data)
    enc = {encode(o) + struct.pack("<2h", -3, -1): encode(n) + struct.pack("<2h", -3, -1) for o, n in names.items()}
    hits = [i for i in range(0, len(d), 2) for k in enc if d[i:i + len(k)] == k]
    if not hits:
        raise SubscreenError("no item name found")
    base = _base(bytes(d), hits[0])
    nw = len(d) // 4
    words = list(struct.unpack_from("<%dI" % nw, d))
    targets = sorted({w - base for w in words if 0 <= w - base < len(d) and _is_str(d, w - base)})
    runs = set()
    for h in hits:
        if h not in targets:
            continue
        lo = hi = targets.index(h)
        # the pool: the run of pointed strings around it, each starting at most 4 bytes after the previous one ends
        while lo > 0 and targets[lo] - _strend(d, targets[lo - 1]) <= 4:
            lo -= 1
        while hi + 1 < len(targets) and targets[hi + 1] - _strend(d, targets[hi]) <= 4:
            hi += 1
        runs.add((lo, hi))
    moved = {}
    for lo, hi in sorted(runs):
        pool = targets[lo:hi + 1]
        start, stop = pool[0], (_strend(d, pool[-1]) + 3) & ~3
        out, where = bytearray(), {}
        for o in pool:
            body = bytes(d[o:_strend(d, o)])
            body = enc.get(body, body)
            if body not in where:
                where[body] = start + len(out)
                out += body + b"\0" * (-len(body) % 4)
            moved[o] = where[body]
        if len(out) > stop - start:
            raise SubscreenError("the item names need %d bytes, the pool has %d" % (len(out), stop - start))
        d[start:stop] = out + b"\0" * (stop - start - len(out))
    for i, w in enumerate(words):
        if w - base in moved:
            struct.pack_into("<I", d, 4 * i, base + moved[w - base])
    return bytes(d)


def _in_place(data, names):
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
