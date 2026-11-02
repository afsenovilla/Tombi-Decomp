#!/usr/bin/env python3
"""Item names of the sub-screen lists (SYSTEM/DnSPSUB.BIN, DnSPSUB2.BIN, DnSPSUB3.BIN; n = language 0..8).

The inventory / pause lists do not read the WFM item names (MFNT #n): each sub-screen overlay keeps its own table of
glyph-id strings (s16; a character is latin-1 + 140 below 0x80 and + 74 from 0xC0; -3 then -1 end one; 0 pads to the
next string). So an item renamed in the dialogues keeps its old name in the lists unless it is renamed here too.

Edits (the "menus" section of a *.tombipatch.json, key "itemNames" under each of the three files):
  "itemNames": {"OLD": "NEW"}     every copy of OLD changes; in place when NEW fits the room of OLD, otherwise the pool
                                  of pointed strings around them is repacked (identical strings shared) and its pointers
                                  rewritten

  "placeNames": {"OLD": "NEW"}    DnSPSUB / DnSPSUB3 only: the place names of the warp list ("Usar" -> place popup,
                                  SUBSCREEN_POPUP_SUB_STRINGS), in the popup font (latin-1 - 32, 0xC0.. - 0x60, "\n" = the
                                  -3 line break, -1 ends one; tools/memcard_text.py encode). Every pointed copy of OLD
                                  changes; the pool of pointed place strings around them is repacked, pointers rewritten
  In "itemNames" a "\n" writes the -3 line break too.

  "eventNames": {"OLD": "NEW"}    DnSPSUB / DnSPSUB3: only the copies of OLD that the event list points to (its pointer table:
                                  the run of pointers that starts at the one to the first event name), applied before
                                  "itemNames", so an event name can differ from a place / item name with the same text
                                  ("Montaña de Fénix": the event "La Montaña del Fénix", the place "Montaña del Fénix")

  subscreen_text.py dump FILE.BIN      prints the strings
"""
import struct, sys


class SubscreenError(Exception):
    pass


def encode(text):
    out = []
    for c in text:
        o = ord(c)
        if c == "\n":
            out.append(-3)  # in-string line break
        elif 32 <= o < 0x80:
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


def _wfm_spans(d):
    """[start, end) of the WFM3 dialogue banks embedded in the overlay (DnSPSUB / DnSPSUB3 carry the item and event
    descriptions of "Ver" as one). Their text is a stream of 0x80nn glyph words, so two of them side by side read as a
    word 0x80nn80nn: an address inside the overlay. Such words are never pointers and must not be rewritten."""
    spans, w = [], d.find(b"WFM3")
    while 0 <= w and w + 148 <= len(d):
        dpt, nd, ng = struct.unpack_from("<IHH", d, w + 8)
        end = w + dpt + 2 * nd
        try:
            for g in range(ng):
                gp = struct.unpack_from("<H", d, w + 144 + 2 * g)[0]
                cw, h = struct.unpack_from("<2H", d, w + gp)
                end = max(end, w + gp + 8 + (cw * 4 * h + 1) // 2)
            for i in range(nd):
                o = w + dpt + struct.unpack_from("<H", d, w + dpt + 2 * i)[0]
                while o + 2 <= len(d):
                    c = struct.unpack_from("<H", d, o)[0]
                    o += 2
                    if c in (0xFFFE, 0xFFFF):
                        break
                end = max(end, o)
        except struct.error:
            end = len(d)
        end = min(end, len(d))
        spans.append((w, end))
        w = d.find(b"WFM3", end)
    return spans


def _words(d):
    """The overlay's 32-bit words, those inside an embedded WFM3 bank read as 0 (never a pointer)."""
    words = list(struct.unpack_from("<%dI" % (len(d) // 4), d))
    for a, b in _wfm_spans(bytes(d)):
        for i in range(a // 4, min((b + 3) // 4, len(words))):
            words[i] = 0
    return words


def _base(d, anchor):
    """Load address of the overlay: the shift that turns the most pointer words into string starts."""
    import collections
    words = set(_words(d))
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


def _slack(d, o, stop=None):
    """End of the room after offset o (just past a string): the -1 / 0 words the disc pads strings with ("-3, -1, -1, 0")."""
    while o + 2 <= len(d) and (stop is None or o < stop) and d[o:o + 2] in (b"\xff\xff", b"\0\0"):
        o += 2
    return o


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
    d = _repack(d, hits, lambda o, body: enc.get(body, body))
    # copies no pointer reaches (the room list's repeated slots) keep their place: rewritten where they fit
    return _in_place(bytes(d), names, lenient=True)


def _event_table(d, base):
    """Word offsets of the event list's pointer table: the run of in-file pointers that starts at the first pointer to the
    first event name (the string right after the "Habilidad" heading, dump index 3)."""
    strs = dump(d)
    first = next((o for i, (o, t) in enumerate(strs) if i and strs[i - 1][1] == "Habilidad"), None)
    if first is None:
        raise SubscreenError("event list not found")
    nw = len(d) // 4
    words = _words(d)
    try:
        i = words.index(base + first)
    except ValueError:
        raise SubscreenError("event list pointer table not found")
    j = i
    while j < nw and 0 <= words[j] - base < len(d):
        j += 1
    return range(i, j)


def apply_event_names(data, names):
    """eventNames: the copies of OLD that only the event list points to get NEW (pool repacked as in apply_names)."""
    d = bytearray(data)
    base = _base(bytes(d), 0)
    nw = len(d) // 4
    words = _words(d)
    table = _event_table(bytes(d), base)
    tset = set(table)
    enc = {encode(o) + struct.pack("<2h", -3, -1): encode(n) + struct.pack("<2h", -3, -1) for o, n in names.items()}
    hits = {}
    for k, body in enc.items():
        found = False
        for i in table:
            o = words[i] - base
            if bytes(d[o:o + len(k)]) == k:
                refs = [x for x in range(nw) if words[x] == words[i]]
                if any(x not in tset for x in refs):
                    raise SubscreenError("event name %r is shared with another list; split it first" % decode_body(k))
                hits[o] = body
                found = True
        if not found:
            raise SubscreenError("event name %r not found in the event list" % decode_body(k))
    return _repack(d, sorted(hits), lambda o, body: hits.get(o, body))


def decode_body(b):
    g = struct.unpack_from("<%dh" % (len(b) // 2), b)
    return "".join("\n" if x == -3 else chr(x - 140) if x < 268 else chr(x - 74) for x in g if x != -1)


def _repack(d, hits, new_body):
    """Repacks the pools of pointed strings around the offsets `hits` (new_body(offset, old body) -> body) and rewrites the
    pointers. A pool's room runs to the padding after its last string (the -1 / 0 words)."""
    base = _base(bytes(d), hits[0])
    nw = len(d) // 4
    words = _words(d)
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
        nxt = targets[hi + 1] if hi + 1 < len(targets) else len(d)
        start, stop = pool[0], _slack(d, _strend(d, pool[-1]), nxt) & ~3
        out, where = bytearray(), {}
        for o in pool:
            body = bytes(d[o:_strend(d, o)])
            body = new_body(o, body)
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


def _place_at(d, o):
    """Glyphs of the popup-font string at o (up to and with the -1), or None."""
    g = []
    while o + 2 * len(g) + 2 <= len(d) and len(g) < 80:
        c = struct.unpack_from("<H", d, o + 2 * len(g))[0]
        g.append(c)
        if c == 0xFFFF:
            return g if len(g) > 2 else None
        if not (c < 0xA8 or c == 0xFFFD):
            return None
    return None


def apply_places(data, names):
    import memcard_text as MC
    d = bytearray(data)
    enc = {}
    for o, n in names.items():
        try:
            enc[tuple(MC.encode(o))] = tuple(MC.encode(n))
        except MC.MemcardError as ex:
            raise SubscreenError(str(ex))
    base = _base(bytes(d), 0)
    nw = len(d) // 4
    words = _words(d)
    strs = {}
    for w in set(words):
        o = w - base
        if 0 <= o < len(d) and not o & 1:
            g = _place_at(d, o)
            if g:
                strs[o] = tuple(g)
    hits = [o for o, g in strs.items() if g in enc]
    missing = set(enc) - {strs[o] for o in hits}
    if missing:
        raise SubscreenError("place name %r not found" % MC.decode(list(sorted(missing)[0])))
    targets = sorted(strs)
    runs = set()
    for h in hits:
        lo = hi = targets.index(h)
        while lo > 0 and targets[lo - 1] + 2 * len(strs[targets[lo - 1]]) <= targets[lo] <= targets[lo - 1] + 2 * len(strs[targets[lo - 1]]) + 6:
            lo -= 1
        while hi + 1 < len(targets) and targets[hi] + 2 * len(strs[targets[hi]]) <= targets[hi + 1] <= targets[hi] + 2 * len(strs[targets[hi]]) + 6:
            hi += 1
        runs.add((lo, hi))
    moved = {}
    for lo, hi in sorted(runs):
        pool = targets[lo:hi + 1]
        start = pool[0]
        stop = pool[-1] + 2 * len(strs[pool[-1]])
        while stop + 2 <= len(d) and d[stop:stop + 2] == b"\xff\xff":
            stop += 2
        out, where = bytearray(), {}
        for o in pool:
            g = enc.get(strs[o], strs[o])
            if g not in where:
                while (start + len(out)) & 3:
                    out += b"\xff\xff"
                where[g] = start + len(out)
                out += struct.pack("<%dH" % len(g), *g)
            moved[o] = where[g]
        if len(out) > stop - start:
            raise SubscreenError("the place names need %d bytes, the pool has %d" % (len(out), stop - start))
        d[start:stop] = out + b"\xff" * (stop - start - len(out))
    for i, w in enumerate(words):
        if w - base in moved:
            struct.pack_into("<I", d, 4 * i, base + moved[w - base])
    return bytes(d)


def _in_place(data, names, lenient=False):
    """Every copy of each OLD rewritten where it is. lenient: copies that are gone (repacked) or too small are skipped."""
    d = bytearray(data)
    for old, new in names.items():
        pat = encode(old) + struct.pack("<2h", -3, -1)
        hits = [i for i in range(0, len(d) - len(pat) + 1, 2) if d[i:i + len(pat)] == pat]
        if not hits and not lenient:
            raise SubscreenError("item name %r not found" % old)
        for i in hits:
            room = len(encode(old))
            j = _slack(d, i + room + 4)                       # the -1 / 0 padding after the terminator is room too
            room += j - (i + room + 4)
            body = encode(new)
            if len(body) > room:
                if lenient:
                    continue
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
