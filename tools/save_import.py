#!/usr/bin/env python3
"""Import / export memory card saves for the native port's memory card (tombi_mc0.mcd, a raw 128 KB image, the same
layout on PC and Android; port2/shims/card.c: the port emulates only memory card 1).  Used by the PC launcher (tools/port_launcher.py); the Android
launcher has the same logic in SaveImport.java.

Accepted inputs:
  whole cards:  raw .mcd/.mcr/.mc/.srm/.ddf/.ps/.bin (131072 bytes, "MC"), DexDrive .gme (3904-byte header),
                .vgs/.mem (64-byte "VgsM" header)
  single saves: .mcs/.ps1/.mcb (128-byte directory frame + blocks), PS3 .psv ("\\0VSP", data at 0x84)
A whole card replaces the target; a single save goes into free blocks of the target card.  A single save whose name
is already on the card is refused (Conflict: the launchers ask Replace / Save to another slot / Cancel) unless
--overwrite: the old save is removed and the new one takes its blocks (any block count); or --new-slot: the save goes
in under the first free slot name of the card (BESCES-01331TOMBA-00 -> -01 ..., the slot digits of its title too).  Before a card is written a copy goes to
backup/ next to it (backup/tombi_mc0-YYYYMMDD-HHMMSS.mcd, the last 5 of each card kept).

  python3 tools/save_import.py import FILE CARD.mcd [--overwrite | --new-slot]
  python3 tools/save_import.py export-save CARD.mcd SLOT_BLOCK OUT.mcs
"""
import os, shutil, struct, sys, time

CARD_SIZE, BLOCK, FRAME = 0x20000, 0x2000, 0x80
TOMBI_PRODUCTS = ("BESCES-01330", "BESCES-01331", "BASCUS-94236", "BISLPS-01144")
# per release (tools/tombi_save.py LAYOUTS, docs/SAVE_FORMAT.md): play-time vblanks per second and the title offset of
# the slot number (None: not known, JP); the same table in SaveImport.java
PRODUCT_HZ = {"BESCES-01330": 50, "BESCES-01331": 50, "BASCUS-94236": 60, "BISLPS-01144": 60, "BISCPS-99999": 60}
PRODUCT_SLOT_OFS = {"BESCES-01330": 0x16, "BESCES-01331": 0x16, "BASCUS-94236": 0x14}
BACKUP_KEEP = 5

# The place of a save as the game's Map screen and the second screen name it (port2/runtime/panel.c room_names: the
# map area names of D0SPSUB, through the map link of the room GAME.selectedArea / GAME.section; area 14 = the Evil Pig
# rooms, their own table).  REGIONS_EN[i] / REGIONS_ES[i] = map area name i of the English disc / the improved Spanish
# translation (patches/es_mejorada.tombipatch.json) (12 = area 14's first room); AREA_REGIONS[area][section] = the
# region of each section (hex digit); a section past the string, or with no map area, takes section 0's.  Same tables
# in SaveImport.java.
REGIONS_EN = ("Village of All Beginnings", "Dwarf Forest", "Mushroom Forest", "Phoenix's Mountain", "Baccus Village",
              "Haunted Mansion", "Deep Jungle", "Village of Civilization", "Trick Village",
              "Village of All Beginnings", "Dirt Motocross", "Phoenix's Mountain", "Evil Pig Area One")
REGIONS_ES = ("Ciudad de todo principio", "Bosque de enanos", "Bosque de setas", "Montaña del Fénix", "Ciudad Baco",
              "Mansión encantada", "Jungla profunda", "Ciudad de civilización", "Ciudad de trucos",
              "Ciudad de todo principio", "Motocross sucio", "Montaña del Fénix", "Área Cerdiablo uno")
AREA_REGIONS = {0: "000000", 1: "11111", 2: "111111", 3: "333333", 4: "55555555555555555555", 5: "4444", 6: "a",
                8: "5522", 9: "2222222", 10: "666866688", 11: "77777", 13: "10", 14: "c077732000",
                16: "7777777", 17: "77777777770", 18: "770", 19: "11b"}


# The map's room names (MAP_ROOM_NAMES by the room's map link, MAP_AREA14_ROOM_NAMES for area 14; tools/map_tables.py,
# docs/STATUS.md 2026-11-04): ROOM_NAMES_*[AREA_ROOMS[area][section]], English disc / improved Spanish (the patch's
# placeNames applied).  Every section of an area has an entry; the game shows the same name for sections that share
# a map link (the mansion's interior rooms all read "East Side of Mansion").
ROOM_NAMES_EN = ('Village of All Beginnings', 'Forest of All Beginnings', "100 Year Old Man's Hut", "Ol' Pond",
                 'Forest of 100 Flowers', 'Wobbly Wharf', 'Watch Tower', 'Charity Square', 'Dwarf Village',
                 'Stormy Mountain', 'Lava Caves', "Phoenix's Nest", 'North Side of Mansion', 'West Side of Mansion',
                 'South Side of Mansion', 'East Side of Mansion', 'Baccus Village', 'Dirt Motocross',
                 "The Mermaid's Singing Rock", 'Mushroom Forest', 'Mansion', 'Masakari Jungle', 'Masakari River',
                 'Old Tree Hill', 'Trick Village', 'Lumberjack Factory', 'Evil Pig Area One', 'Evil Pig Area Two',
                 'Evil Pig Area Three', 'Evil Pig Area Four', 'Evil Pig Area Five', 'Evil Pig Area Six',
                 'Evil Pig Area Seven', 'Evil Pig Area Eight', 'Clock Tower', 'Iron Castle', 'The Y Crossing',
                 'Hidden Village')
ROOM_NAMES_ES = ('Ciudad de todo principio', 'Bosque de todo principio', 'Cabaña Anciano centenario',
                 'Viejo estanque', 'Bosque de 100 flores', 'Muelle inestable', 'Torre de vigilancia',
                 'Plaza de la caridad', 'Ciudad de enanos', 'Montaña tormentosa', 'Cuevas de lava', 'Nido de Fénix',
                 'Norte de la mansión', 'Oeste de la mansión', 'Sur de la mansión', 'Este de la mansión',
                 'Ciudad Baco', 'Motocross sucio', 'La roca de la sirena', 'Bosque de setas', 'Mansión',
                 'Jungla Masakari', 'Río Masakari', 'Colina del árbol viejo', 'Ciudad de trucos', 'Serrería',
                 'Área Cerdiablo uno', 'Área Cerdiablo dos', 'Área Cerdiablo tres', 'Área Cerdiablo cuatro',
                 'Área Cerdiablo cinco', 'Área Cerdiablo seis', 'Área Cerdiablo siete', 'Área Cerdiablo ocho',
                 'Torre reloj', 'Castillo de Hierro', 'Cruce en Y', 'Ciudad oculta')
AREA_ROOMS = {0: [0, 1, 1, 2, 0, 3], 1: [4, 4, 5, 6, 7], 2: [8, 8, 8, 8, 8, 8], 3: [9, 9, 10, 11, 9, 9], 4: [12, 13, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15], 5: [16, 16, 16, 16], 6: [17, 18, 18], 8: [12, 12, 19, 20], 9: [19, 20, 20, 20, 20, 20, 20], 10: [21, 22, 23, 24, 21, 22, 23, 24, 24], 11: [25, 25, 25, 25, 25], 13: [7, 2], 14: [26, 27, 28, 29, 30, 31, 32, 33], 16: [34, 34, 34, 34, 34, 34, 34], 17: [35, 35, 35, 35, 35, 35, 35, 35, 36, 36, 0], 18: [36, 36, 0], 19: [8, 8, 37]}


def room_name(area, section=0, es=True):
    """the map's room name of a save's place, in the improved Spanish or the English disc; None for an unknown room"""
    r = AREA_ROOMS.get(area)
    if r is None or not (0 <= section < len(r)):
        return None
    return (ROOM_NAMES_ES if es else ROOM_NAMES_EN)[r[section]]


def place_name(area, section=0, es=True):
    """"area · room" of a save's place (the room only when it adds something); None for an unknown area"""
    a, r = area_name(area, section, es), room_name(area, section, es)
    return None if a is None else a if r is None or r == a else "%s · %s" % (a, r)


def area_names(es=False):
    """{disc area: map name of its section 0} for the tools' area lists (the game's own names, not hand-made ones)"""
    return {a: area_name(a, 0, es) for a in sorted(AREA_REGIONS)}


def area_name(area, section=0, es=True):
    """the place of a save (GAME.selectedArea, GAME.section): the map name of the improved Spanish translation, or of
    the English disc; None for an unknown area"""
    r = AREA_REGIONS.get(area)
    if r is None:
        return None
    return (REGIONS_ES if es else REGIONS_EN)[int(r[section] if 0 <= section < len(r) else r[0], 16)]


class ImportError_(Exception):
    pass


def xor(b):
    x = 0
    for c in b:
        x ^= c
    return x


def blank_card():
    c = bytearray(CARD_SIZE)
    c[0:2] = b"MC"; c[0x7F] = xor(c[:0x7F])
    for i in range(1, 16):
        o = i * FRAME
        c[o] = 0xA0; c[o + 8] = c[o + 9] = 0xFF; c[o + 0x7F] = xor(c[o:o + 0x7F])
    for i in range(16, 36):
        o = i * FRAME
        c[o:o + 4] = b"\xff" * 4; c[o + 8] = c[o + 9] = 0xFF; c[o + 0x7F] = xor(c[o:o + 0x7F])
    o = 63 * FRAME
    c[o:o + 2] = b"MC"; c[o + 0x7F] = xor(c[o:o + 0x7F])
    return c


def parse(raw, name=""):
    """-> ("card", bytes128K) or ("save", frame128, data) ; raises ImportError_ for unknown files."""
    ext = os.path.splitext(name)[1].lower()
    if len(raw) == CARD_SIZE and raw[:2] == b"MC":
        return ("card", bytes(raw))
    if raw[:11] == b"123-456-STD" and len(raw) >= 0xF40 + CARD_SIZE and raw[0xF40:0xF42] == b"MC":
        return ("card", bytes(raw[0xF40:0xF40 + CARD_SIZE]))
    if raw[:4] == b"VgsM" and len(raw) >= 64 + CARD_SIZE and raw[64:66] == b"MC":
        return ("card", bytes(raw[64:64 + CARD_SIZE]))
    if raw[:4] == b"\0VSP" and len(raw) > 0x84:
        data = raw[0x84:]
        if data[:2] != b"SC" or len(data) % BLOCK:
            raise ImportError_("archivo .psv no reconocido (sin datos de partida de PS1)")
        fname = raw[0x64:0x84].split(b"\0")[0]
        f = bytearray(FRAME); f[0] = 0x51
        struct.pack_into("<I", f, 4, len(data)); f[10:10 + len(fname[:20])] = fname[:20]
        return ("save", bytes(f), bytes(data))
    if len(raw) > FRAME and raw[0] == 0x51 and (len(raw) - FRAME) % BLOCK == 0 and raw[FRAME:FRAME + 2] == b"SC":
        return ("save", bytes(raw[:FRAME]), bytes(raw[FRAME:]))
    if len(raw) == CARD_SIZE and ext in (".mcd", ".mcr", ".mc", ".srm", ".ddf", ".ps", ".bin"):
        raise ImportError_("la imagen de tarjeta no empieza por \"MC\" (no formateada o dañada)")
    raise ImportError_("formato no reconocido: se aceptan .mcd/.mcr/.mc/.srm/.ddf/.ps/.bin, .gme, .vgs/.mem, "
                       ".mcs/.ps1 y .psv")


def save_name(frame):
    return bytes(frame[10:30]).split(b"\0")[0].decode("ascii", "replace")


def is_tombi(name):
    return any(name.startswith(p) for p in TOMBI_PRODUCTS) or "TOMBA" in name


def title(data):
    """the save's title (Shift-JIS, frame 'SC' + 4 bytes; 64 bytes) as text"""
    t = bytes(data[4:68]).split(b"\0")[0]
    try:
        import unicodedata
        return unicodedata.normalize("NFKC", t.decode("shift_jis", "replace")).strip()
    except Exception:
        return ""


def save_info(name, data, es=True):
    """one line naming a save for the user: its slot and, when the game data parses (a Tombi! save whose GAME
    checksum is right, docs/SAVE_FORMAT.md), the area Continue loads and the play time; else the save's title.
    PS1 saves store no date."""
    slot = name[-2:] if name[-2:].isdigit() and is_tombi(name) else ""
    head = ("Ranura " if es else "Slot ") + slot if slot else name
    g = bytes(data[0x200:0xC00])
    if slot and bytes(data[:2]) == b"SC" and len(g) == 0xA00 and xor(g[:-1]) == g[-1]:
        area, sec, t = struct.unpack_from("<HH", g, 0) + struct.unpack_from("<I", g, 8)
        a = area_name(area, sec, es)
        hz = PRODUCT_HZ.get(name[:12], 50)
        parts = [head] + ([a] if a else []) + ["%d:%02d h" % (t // (3600 * hz), t // (60 * hz) % 60)]
        return " · ".join(parts)
    t = title(data)
    return head + ("  " + t if t else "")


def list_saves(card):
    out = []
    for i in range(1, 16):
        f = card[i * FRAME:(i + 1) * FRAME]
        if f[0] == 0x51:
            out.append((i, save_name(f), title(card[i * BLOCK:(i + 1) * BLOCK])))
    return out


def chain(card, block):
    """the blocks of the save that starts at block (following the directory frames' link field)"""
    out, b = [], block
    while 1 <= b <= 15 and b not in out:
        out.append(b)
        nxt = struct.unpack_from("<H", card, b * FRAME + 8)[0]
        if nxt == 0xFFFF:
            break
        b = nxt + 1
    return out


def find_save(card, name):
    """first block of the save called name, or None"""
    for b, nm, _ in list_saves(card):
        if nm == name:
            return b
    return None


def free_blocks(card):
    return [i for i in range(1, 16) if card[i * FRAME] in (0, 0xA0, 0xA1, 0xA2, 0xA3)]


def delete_save(card, block):
    """frees the save's blocks (directory frames back to the formatted 'free' state, data zeroed); returns them"""
    blocks = chain(card, block)
    for b in blocks:
        f = bytearray(FRAME)
        f[0] = 0xA0; f[8] = f[9] = 0xFF; f[0x7F] = xor(f[:0x7F])
        card[b * FRAME:(b + 1) * FRAME] = f
        card[b * BLOCK:(b + 1) * BLOCK] = bytes(BLOCK)
    return blocks


def insert_save(card, frame, data, prefer=()):
    """put a single save into free blocks of card (bytearray), the blocks in prefer first (a replaced save goes where
    the old one was); returns the first block.  Raises if no room / duplicate."""
    n = len(data) // BLOCK
    name = save_name(frame)
    if find_save(card, name) is not None:
        raise ImportError_("la tarjeta ya tiene una partida con el mismo nombre (%s)" % name)
    free = free_blocks(card)
    free = [b for b in prefer if b in free] + [b for b in free if b not in prefer]
    if len(free) < n:
        raise ImportError_("no hay sitio en la tarjeta: hacen falta %d bloques libres y hay %d" % (n, len(free)))
    blocks = free[:n]
    for k, b in enumerate(blocks):
        f = bytearray(frame) if k == 0 else bytearray(FRAME)
        f[0] = 0x51 if k == 0 else (0x53 if k == n - 1 else 0x52)
        struct.pack_into("<I", f, 4, n * BLOCK if k == 0 else 0)
        struct.pack_into("<H", f, 8, 0xFFFF if k == n - 1 else blocks[k + 1] - 1)
        f[0x7F] = xor(f[:0x7F])
        card[b * FRAME:(b + 1) * FRAME] = f
        card[b * BLOCK:(b + 1) * BLOCK] = data[k * BLOCK:(k + 1) * BLOCK]
    return blocks[0]


def backup(path, keep=BACKUP_KEEP):
    """a copy of the card in backup/ next to it (backup/tombi_mc0-YYYYMMDD-HHMMSS.mcd); only the newest `keep`
    copies of that card are kept.  Returns the copy's path (None when there is no card yet)."""
    if not os.path.exists(path):
        return None
    d = os.path.join(os.path.dirname(os.path.abspath(path)), "backup")
    os.makedirs(d, exist_ok=True)
    stem, ext = os.path.splitext(os.path.basename(path))
    stamp = time.strftime("%Y%m%d-%H%M%S")
    bak, k = os.path.join(d, "%s-%s%s" % (stem, stamp, ext)), 2
    while os.path.exists(bak):
        bak, k = os.path.join(d, "%s-%s-%d%s" % (stem, stamp, k, ext)), k + 1
    shutil.copyfile(path, bak)
    mine = sorted((f for f in os.listdir(d) if f.startswith(stem + "-") and f.endswith(ext)),
                  key=lambda f: (os.path.getmtime(os.path.join(d, f)), f))
    for f in mine[:max(0, len(mine) - keep)]:
        os.remove(os.path.join(d, f))
    return bak


SLOTS = 15     # the game's slots 00..14 (the file name's last two digits; docs/SAVE_FORMAT.md)


def free_slot(card, name, need=1):
    """the first slot number (00..14) whose Tombi file name (name with its last two digits replaced) is not on the
    card, as "NN", when the card also has `need` free blocks; else None"""
    if not (is_tombi(name) and name[-2:].isdigit()) or len(free_blocks(card)) < need:
        return None
    for k in range(SLOTS):
        if find_save(card, name[:-2] + "%02d" % k) is None:
            return "%02d" % k
    return None


def renumber(frame, data, nn):
    """the save as slot nn ("NN"): the file name's last two digits and, when they are fullwidth digits (Shift-JIS
    0x824F + n), the slot number in the title (save +0x16 PAL, +0x14 USA: PRODUCT_SLOT_OFS; 2 characters; not covered
    by the checksum)"""
    f, d = bytearray(frame), bytearray(data)
    name = save_name(f)
    nb = (name[:-2] + nn).encode("ascii")
    f[10:30] = nb + bytes(20 - len(nb))
    f[0x7F] = xor(f[:0x7F])
    o = PRODUCT_SLOT_OFS.get(name[:12])
    if o and all(d[o + 2 * i] == 0x82 and 0x4F <= d[o + 1 + 2 * i] <= 0x58 for i in range(2)):
        for i in range(2):
            d[o + 1 + 2 * i] = 0x4F + int(nn[i])
    return bytes(f), bytes(d)


def read_card(path):
    card = bytearray(open(path, "rb").read()) if os.path.exists(path) else blank_card()
    if len(card) != CARD_SIZE or card[:2] != b"MC":
        card = blank_card()
    return card


class Conflict(ImportError_):
    """the card already holds a save with the imported save's name: the caller asks (Replace / Save to another slot
    when new_slot / Cancel) and calls import_file again with overwrite=True or new_slot=True.  new_slot: the slot the
    save would get ("NN", free_slot), None when the card has no room or no free slot"""
    def __init__(self, dest, name, existing, incoming, new_slot):
        self.dest, self.name, self.existing, self.incoming, self.new_slot = dest, name, existing, incoming, new_slot
        ImportError_.__init__(self, "ya hay una partida «%s» en la tarjeta" % existing)

    def question(self, es=True):
        if es:
            return ("Ya hay una partida «%s» en la tarjeta. ¿Reemplazarla?\n\nPartida importada: «%s»" % (self.existing, self.incoming)
                    + ("\n\nO guárdala en otra ranura: Ranura %s." % self.new_slot if self.new_slot else ""))
        return ("The memory card already holds the save “%s”. Replace it?\n\nImported save: “%s”" % (self.existing, self.incoming)
                + ("\n\nOr save it to another slot: Slot %s." % self.new_slot if self.new_slot else ""))


def import_file(src, dest, overwrite=False, new_slot=False, es=True):
    """import src into card file dest.  Returns (summary_lines, warnings).  A single save whose name is already on
    the card raises Conflict unless overwrite (then the old save is removed and its blocks reused first) or new_slot
    (then it goes in as the first free slot, renumber).  The card is copied into backup/ before it is written."""
    raw = open(src, "rb").read()
    p = parse(raw, src)
    warn = []
    if p[0] == "card":
        bak = backup(dest)
        with open(dest, "wb") as f:
            f.write(p[1])
        lines = ["Tarjeta completa importada" + (" (copia de la anterior: backup/%s)" % os.path.basename(bak) if bak else "")]
        saves = list_saves(p[1])
        lines += ["  %s  %s" % (n, t) for _, n, t in saves] or ["  (tarjeta vacía)"]
        if saves and not any(is_tombi(n) for _, n, _ in saves):
            warn.append("La tarjeta no contiene partidas de Tombi!/Tomba!")
        return lines, warn
    _, frame, data = p
    name = save_name(frame)
    if not is_tombi(name):
        warn.append("La partida (%s) no parece de Tombi!/Tomba!" % name)
    card = read_card(dest)
    old = find_save(card, name)
    prefer, replaced = (), None
    if old is not None and new_slot:
        nn = free_slot(card, name, len(data) // BLOCK)
        if nn is None:
            raise ImportError_("no hay ninguna ranura libre en la tarjeta (o no hay sitio)")
        frame, data = renumber(frame, data, nn)
        name = save_name(frame)
    elif old is not None:
        replaced = save_info(name, card[old * BLOCK:(old + 1) * BLOCK], es)
        if not overwrite:
            raise Conflict(dest, name, replaced, save_info(name, data, es), free_slot(card, name, len(data) // BLOCK))
        prefer = delete_save(card, old)      # on the in-memory copy: the file is written only if all of it fits
    b = insert_save(card, frame, data, prefer)
    bak = backup(dest)
    with open(dest, "wb") as f:
        f.write(card)
    lines = ["Partida importada en la tarjeta, bloque %d: %s" % (b, save_info(name, data, es))]
    if replaced:
        lines.append("Sustituye a «%s»%s" % (replaced, " (copia de la tarjeta: backup/%s)" % os.path.basename(bak)
                                                       if bak else ""))
    return lines, warn


def export_card(src, out):
    raw = open(src, "rb").read() if os.path.exists(src) else bytes(blank_card())
    if len(raw) != CARD_SIZE or raw[:2] != b"MC":
        raise ImportError_("la tarjeta no es una imagen válida de 128 KB")
    with open(out, "wb") as f:
        f.write(raw)


def export_save(card, block):
    """-> .mcs bytes (directory frame + blocks following the link chain)"""
    f = bytearray(card[block * FRAME:(block + 1) * FRAME])
    if f[0] != 0x51:
        raise ImportError_("no hay partida en el bloque %d" % block)
    data = b"".join(bytes(card[b * BLOCK:(b + 1) * BLOCK]) for b in chain(card, block))
    struct.pack_into("<H", f, 8, 0xFFFF)
    f[0x7F] = xor(f[:0x7F])
    return bytes(f) + data


if __name__ == "__main__":
    a = sys.argv[1:]
    ow, ns = "--overwrite" in a, "--new-slot" in a
    a = [x for x in a if x not in ("--overwrite", "--new-slot")]
    if len(a) == 3 and a[0] == "import":
        try:
            l, w = import_file(a[1], a[2], overwrite=ow, new_slot=ns)
        except Conflict as e:
            sys.exit(e.question() + "\n(--overwrite la sustituye" + (", --new-slot la guarda en la ranura %s" % e.new_slot
                                                                     if e.new_slot else "")
                     + "; antes se copia la tarjeta en backup/)")
        except (ImportError_, OSError) as e:
            sys.exit("error: %s" % e)
        print("\n".join(l + w))
    elif len(a) == 4 and a[0] == "export-save":
        open(a[3], "wb").write(export_save(open(a[1], "rb").read(), int(a[2])))
    else:
        print(__doc__)
