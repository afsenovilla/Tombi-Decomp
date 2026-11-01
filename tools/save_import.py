#!/usr/bin/env python3
"""Import / export memory card saves for the native port's cards (tombi_mc0.mcd / tombi_mc1.mcd, raw 128 KB images,
the same layout on PC and Android; port2/shims/card.c).  Used by the PC launcher (tools/port_launcher.py); the Android
launcher has the same logic in SaveImport.java.

Accepted inputs:
  whole cards:  raw .mcd/.mcr/.mc/.srm/.ddf/.ps/.bin (131072 bytes, "MC"), DexDrive .gme (3904-byte header),
                .vgs/.mem (64-byte "VgsM" header)
  single saves: .mcs/.ps1/.mcb (128-byte directory frame + blocks), PS3 .psv ("\\0VSP", data at 0x84)
A whole card replaces the target (the old file kept as .bak); a single save goes into free blocks of the target card.

  python3 tools/save_import.py import FILE CARD.mcd
  python3 tools/save_import.py export-save CARD.mcd SLOT_BLOCK OUT.mcs
"""
import os, shutil, struct, sys, time

CARD_SIZE, BLOCK, FRAME = 0x20000, 0x2000, 0x80
TOMBI_PRODUCTS = ("BESCES-01330", "BESCES-01331", "BASCUS-94236", "BISLPS-01144")


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


def list_saves(card):
    out = []
    for i in range(1, 16):
        f = card[i * FRAME:(i + 1) * FRAME]
        if f[0] == 0x51:
            out.append((i, save_name(f), title(card[i * BLOCK:(i + 1) * BLOCK])))
    return out


def insert_save(card, frame, data):
    """put a single save into free blocks of card (bytearray); returns the first block. Raises if no room/duplicate."""
    n = len(data) // BLOCK
    name = save_name(frame)
    for _, nm, _ in list_saves(card):
        if nm == name:
            raise ImportError_("la tarjeta ya tiene una partida con el mismo nombre (%s)" % name)
    free = [i for i in range(1, 16) if card[i * FRAME] in (0xA0, 0xA1, 0xA2, 0xA3) or card[i * FRAME] == 0]
    if len(free) < n:
        raise ImportError_("no hay sitio en la tarjeta: hacen falta %d bloques libres y hay %d" % (n, len(free)))
    blocks = free[:n]
    for k, b in enumerate(blocks):
        f = bytearray(frame) if k == 0 else bytearray(FRAME)
        f[0] = 0x51 if k == 0 else (0x53 if k == n - 1 else 0x52)
        if k == 0:
            struct.pack_into("<I", f, 4, n * BLOCK)
        if n == 1 or k == n - 1:
            struct.pack_into("<H", f, 8, 0xFFFF)
        else:
            struct.pack_into("<H", f, 8, blocks[k + 1] - 1)
        f[0x7F] = xor(f[:0x7F])
        card[b * FRAME:(b + 1) * FRAME] = f
        card[b * BLOCK:(b + 1) * BLOCK] = data[k * BLOCK:(k + 1) * BLOCK]
    return blocks[0]


def backup(path):
    if not os.path.exists(path):
        return None
    bak = path + ".bak"
    if os.path.exists(bak):
        bak = "%s.%s.bak" % (path, time.strftime("%Y%m%d-%H%M%S"))
    shutil.copy2(path, bak)
    return bak


def import_file(src, dest):
    """import src into card file dest. Returns (summary_lines, warnings)."""
    raw = open(src, "rb").read()
    p = parse(raw, src)
    warn = []
    if p[0] == "card":
        bak = backup(dest)
        with open(dest, "wb") as f:
            f.write(p[1])
        lines = ["Tarjeta completa importada" + (" (copia de la anterior: %s)" % os.path.basename(bak) if bak else "")]
        saves = list_saves(p[1])
        lines += ["  %s  %s" % (n, t) for _, n, t in saves] or ["  (tarjeta vacía)"]
        if saves and not any(is_tombi(n) for _, n, _ in saves):
            warn.append("La tarjeta no contiene partidas de Tombi!/Tomba!")
        return lines, warn
    _, frame, data = p
    name = save_name(frame)
    if not is_tombi(name):
        warn.append("La partida (%s) no parece de Tombi!/Tomba!" % name)
    card = bytearray(open(dest, "rb").read()) if os.path.exists(dest) else blank_card()
    if len(card) != CARD_SIZE or card[:2] != b"MC":
        card = blank_card()
    b = insert_save(card, frame, data)
    backup(dest)
    with open(dest, "wb") as f:
        f.write(card)
    return ["Partida importada en el bloque %d: %s  %s" % (b, name, title(data))], warn


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
    data, b, seen = b"", block, 0
    while 1 <= b <= 15 and seen < 15:
        data += bytes(card[b * BLOCK:(b + 1) * BLOCK]); seen += 1
        nxt = struct.unpack_from("<H", card, b * FRAME + 8)[0]
        if nxt == 0xFFFF:
            break
        b = nxt + 1
    struct.pack_into("<H", f, 8, 0xFFFF)
    f[0x7F] = xor(f[:0x7F])
    return bytes(f) + data


if __name__ == "__main__":
    a = sys.argv[1:]
    if len(a) == 3 and a[0] == "import":
        l, w = import_file(a[1], a[2]); print("\n".join(l + w))
    elif len(a) == 4 and a[0] == "export-save":
        open(a[3], "wb").write(export_save(open(a[1], "rb").read(), int(a[2])))
    else:
        print(__doc__)
