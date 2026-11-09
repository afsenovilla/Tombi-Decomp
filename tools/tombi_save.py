#!/usr/bin/env python3
"""Tombi! / Tomba! memory card saves: open a card image, list the game's saves, decode / edit the fields, rewrite the
checksum and write the card back (docs/SAVE_FORMAT.md has the layout, derived from D0SPSUB2: func_800E999C builds the
save, memCardFileListStep writes / loads it, memCardStoreChecksum / memCardVerifyChecksum).

Containers: raw 128 KB cards (.mcd .mcr .mc .srm .ps, the native port's tombi_mc0.mcd), DexDrive .gme
(3904-byte header), .vgs/.mem (64-byte header), and single-save .mcs / .mcb (one directory frame + the blocks).
Writing never touches the source unless asked: save() writes to a new path, or keeps PATH.bak when overwriting.

  python3 tools/tombi_save.py list CARD
  python3 tools/tombi_save.py show CARD [SLOT]
  python3 tools/tombi_save.py set CARD SLOT FIELD=VALUE ... -o OUT     (fields: see FIELDS; event.N=0..255, item.N=QTY)
  python3 tools/tombi_save.py roundtrip CARD                           (load -> write -> compare, byte-identical)

Item / event names (optional) come from the user's own disc: names_from_disc(PATH_TO_BIN_OR_EXTRACTED_FOLDER, lang).
Nothing of the game is stored here.  Item names are the sub-screen list's own (SYSTEM/DnSPSUB.BIN ITEM_NAME_STRINGS[
ITEM_NAME_INDEX[id]], what the Items list shows for item id), not the AREA00 MFNT "got item" bank, which is not in id
order (it skips Bucket, Telescope, Tear Jar...).
"""
import os, shutil, struct, sys

CARD_SIZE = 0x20000
BLOCK = 0x2000
FRAME = 0x80
SAVE_LEN = 0xC00          # bytes the game writes (memCardWriteFile(..., 0xC00, 1)); one block on the card
GAME_OFS = 0x200          # the GAME block (RAM 0x8009C960) follows the 0x200-byte title/icon header
GAME_LEN = 0xA00          # checksummed area: 0x988 bytes of GAME + zero pad, the XOR of the first 0x9FF in the last byte
GAME_COPY = 0x988         # bytes of GAME the game copies (the rest of the 0xA00 stays 0)

# product codes of the save file names ("BESCES-01331TOMBA-00": region + code + "TOMBA-" + slot 00..14)
PRODUCTS = {"BESCES-01331": "PAL (SCES-01331) Tombi!", "BASCUS-94236": "USA (SCUS-94236) Tomba!",
            "BISLPS-01144": "JP (SLPS-01144) Ore no Tomba",
            # development product code: the JP retail lists and loads these too (never writes them), docs/SAVE_FORMAT.md
            "BISCPS-99999": "JP development code (SCPS-99999, loaded by SLPS-01144)"}

# What differs between the releases' saves (docs/SAVE_FORMAT.md "USA saves"): the GAME block (0x200..0xBFF), its
# checksum and the list fields are the same in every release; the play time counts vblanks at the release's own rate,
# and the title's digits sit at other offsets (each save builder writes them: PAL D0SPSUB2 func_800E999C, USA
# DSPSUB2 func_800E954C).
#   hz      vblanks per second of GAME.totalTimePlayed (PAL 50; USA divides by 216000 / 3600: 60; the JP play-time
#           draw func_800ED850_slps01144 too)
#   digits  title offsets of the slot number (2 digits), the play time hhh:mm (5), lives (2) and level (2);
#           None = not known (the JP save builder is not decompiled): the title is left as it is
#   icon    byte 2 of the header (0x11: a 1-frame icon, PAL; 0x13: the 3-frame animated icon, USA)
LAYOUTS = {
    "pal": dict(hz=50, icon=0x11, digits=dict(slot=(0x16, 0x18), time=(0x1E, 0x20, 0x22, 0x26, 0x28),
                                              lives=(0x30, 0x32), level=(0x40, 0x42))),
    "usa": dict(hz=60, icon=0x13, digits=dict(slot=(0x14, 0x16), time=(0x1C, 0x1E, 0x20, 0x24, 0x26),
                                              lives=(0x2E, 0x30), level=(0x3E, 0x40))),
    "jp": dict(hz=60, icon=None, digits=None),
}
PRODUCT_LAYOUT = {"BESCES-01331": "pal", "BESCES-01330": "pal", "BASCUS-94236": "usa", "BISLPS-01144": "jp",
                  "BISCPS-99999": "jp"}


def layout(product):
    """the LAYOUTS entry of a save file name (its first 12 characters, the product code); PAL for an unknown one"""
    return LAYOUTS[PRODUCT_LAYOUT.get((product or "")[:12], "pal")]


def time_max(hz):
    """the largest play time the save title shows (999:59), in vblanks at hz per second"""
    return 999 * 3600 * hz + 59 * 60 * hz

# ---- GAME fields (include/gamestate.h: offsets into GAME = save + 0x200) ------------------------------------------
# name: (offset, struct fmt, min, max, description)
FIELDS = {
    "area":            (0x000, "<H", 0, 19, "GAME.selectedArea (area the Continue loads)"),
    "section":         (0x002, "<H", 0, 31, "GAME.selectedSection"),
    "time":            (0x008, "<I", 0, time_max(50), "GAME.totalTimePlayed (vblanks: 50 per second PAL, 60 USA / JP)"),
    "ap":              (0x00C, "<I", 0, 9999999, "GAME.playerAP (adventure points)"),
    "health":          (0x010, "B", 0, 16, "GAME.playerHealth (set to max health on load)"),
    "max_health":      (0x011, "B", 1, 16, "GAME.playerMaxHealth (vitality: 8, 16 with the golden bowl bonus)"),
    "level":           (0x013, "B", 0, 99, "GAME.unk13: the 'Level' of the save title"),
    "lives":           (0x020, "B", 0, 99, "GAME.playerLives ('P=' of the save title)"),
    "red_level":       (0x438, "B", 0, 255, "GAME.redExpLevel"),
    "green_level":     (0x439, "B", 0, 255, "GAME.greenExpLevel"),
    "blue_level":      (0x43A, "B", 0, 255, "GAME.blueExpLevel"),
    "red_bar":         (0x43B, "B", 0, 255, "GAME.redExpBar"),
    "green_bar":       (0x43C, "B", 0, 255, "GAME.greenExpBar"),
    "blue_bar":        (0x43D, "B", 0, 255, "GAME.blueExpBar"),
    "golden_bowl":     (0x71F, "B", 0, 255, "GAME.goldenBowlState (non-zero: max health may reach 16)"),
    "bonus_health":    (0x720, "B", 0, 255, "GAME.bonusHealth (hearts picked up at max health 8)"),
    "cur_area":        (0x948, "<H", 0, 19, "GAME.currentArea"),
    "cur_section":     (0x94A, "<H", 0, 31, "GAME.currentSection"),
    "cur_spawn":       (0x94C, "<H", 0, 255, "GAME.currentSpawnPoint"),
    "weapon":          (0x952, "B", 0, 255, "GAME.playerEquips.weapon (raw id)"),
    "pants":           (0x953, "B", 0, 255, "GAME.playerEquips.pants (raw id)"),
    "purified_areas":  (0x963, "B", 0, 255, "GAME.purifiedAreas"),
}
EVENTS_OFS, EVENT_COUNT = 0x444, 256      # SCRIPT_FLAGS (0x8009CDA4): 0 not started, 1..0xFE started, 0xFF completed
EVENT_USED = 0x82                         # ids 0..0x81 are the game's events (tools/port_mods.py EVENT_COUNT)
ITEMS_OFS, ITEM_COUNT = 0x744, 256        # GAME.item[id] = quantity
INV_OFS, INV_COUNTER, INV_SORT = 0x844, 0x944, 0x946   # inventory.slots[256] (ids, newest first), counter, sortMode
PICKED_OFS = 0x34                         # areaNN_pickedItems[32] x 20 (picked-up item flags per area)
POS_OFS = 0x954                           # player position copied at save time (3 x int)
# ids marked cut in docs/CUT_CONTENT.md (no code gives / starts them)
ITEM_USED = 160                           # ids 0..159: the sub-screen's ITEM_ICON_CLUTS / name tables (160+ = "Chick")
UNUSED_ITEMS = {52, 57, 58, 59, 61, 90, 91, 97, 108, 109, 110, 111, 112, 113, 114, 119, 122, 123, 137, 140, 147}
UNUSED_EVENTS = {6, 18, 45, 52, 59, 60, 61, 74, 76, 77, 83, 89}


class SaveError(Exception):
    pass


def xor(b):
    x = 0
    for c in b:
        x ^= c
    return x


# ---- containers ---------------------------------------------------------------------------------------------------
class Card:
    """A 128 KB card image plus the bytes around it in its container."""

    def __init__(self, path):
        self.path = path
        raw = open(path, "rb").read()
        self.orig = raw
        ext = os.path.splitext(path)[1].lower()
        self.kind, self.pre, self.post = "raw", b"", b""
        if raw[:2] == b"MC" and len(raw) >= CARD_SIZE:
            self.data = bytearray(raw[:CARD_SIZE]); self.post = raw[CARD_SIZE:]
        elif raw[:11] == b"123-456-STD" and len(raw) >= 0xF40 + CARD_SIZE:          # DexDrive .gme
            self.kind, self.pre = "gme", raw[:0xF40]
            self.data = bytearray(raw[0xF40:0xF40 + CARD_SIZE]); self.post = raw[0xF40 + CARD_SIZE:]
        elif raw[:4] == b"VgsM" and len(raw) >= 64 + CARD_SIZE:                     # .vgs / .mem
            self.kind, self.pre = "vgs", raw[:64]
            self.data = bytearray(raw[64:64 + CARD_SIZE]); self.post = raw[64 + CARD_SIZE:]
        elif raw[:1] == b"Q" and ext in (".mcs", ".mcb", ".ps1") and len(raw) >= FRAME + BLOCK:   # single save
            self.kind = "mcs"
            n = (len(raw) - FRAME) // BLOCK
            self.data = bytearray(_blank_card())
            for i in range(n):                         # place the save in blocks 1..n, linked
                f = bytearray(raw[:FRAME]) if i == 0 else bytearray(FRAME)
                f[0] = 0x51 if i == 0 else (0x53 if i == n - 1 else 0x52)
                struct.pack_into("<H", f, 8, i + 1 if i < n - 1 else 0xFFFF)
                f[0x7F] = xor(f[:0x7F])
                self.data[(1 + i) * FRAME:(2 + i) * FRAME] = f
                self.data[(1 + i) * BLOCK:(2 + i) * BLOCK] = raw[FRAME + i * BLOCK:FRAME + (i + 1) * BLOCK]
            self.mcs_blocks = n
        elif len(raw) == CARD_SIZE:
            self.data = bytearray(raw)
        else:
            raise SaveError("not a memory card image (raw .mcd/.mcr, .gme, .vgs/.mem or .mcs): %s" % path)

    def directory(self):
        """[(block, name, size)] of the first blocks of every file."""
        out = []
        for i in range(1, 16):
            f = self.data[i * FRAME:(i + 1) * FRAME]
            if f[0] == 0x51:
                name = bytes(f[10:30]).split(b"\0")[0].decode("ascii", "replace")
                out.append((i, name, struct.unpack_from("<I", f, 4)[0]))
        return out

    def saves(self):
        return [Save(self, b, n) for b, n, _ in self.directory() if any(n.startswith(p) for p in PRODUCTS)]

    def to_bytes(self):
        d = bytes(self.data)
        if self.kind == "mcs":
            b = self.saves()[0].block if self.saves() else 1
            return bytes(self.orig[:FRAME]) + d[b * BLOCK:(b + self.mcs_blocks) * BLOCK]
        return self.pre + d + self.post

    def save(self, out=None, backup=True):
        out = out or self.path
        if os.path.abspath(out) == os.path.abspath(self.path) and backup and os.path.exists(out):
            shutil.copy2(out, out + ".bak")
        with open(out, "wb") as f:
            f.write(self.to_bytes())
        return out


def _blank_card():
    c = bytearray(CARD_SIZE)
    c[0:2] = b"MC"; c[0x7F] = xor(c[:0x7F])
    for i in range(1, 16):
        f = c[i * FRAME:(i + 1) * FRAME]
        f[0] = 0xA0; f[8] = f[9] = 0xFF; f[0x7F] = xor(f[:0x7F])
        c[i * FRAME:(i + 1) * FRAME] = f
    for i in range(16, 36):
        f = bytearray(FRAME); f[0:4] = b"\xff" * 4; f[8] = f[9] = 0xFF; f[0x7F] = xor(f[:0x7F])
        c[i * FRAME:(i + 1) * FRAME] = f
    c[63 * FRAME:63 * FRAME + 2] = b"MC"; c[63 * FRAME + 0x7F] = xor(c[63 * FRAME:63 * FRAME + 0x7F])
    return c


# ---- one save --------------------------------------------------------------------------------------------------
FW_DIGIT0 = 0x824F        # Shift-JIS fullwidth '0'


class Save:
    def __init__(self, card, block, name):
        self.card, self.block, self.name = card, block, name
        self.base = block * BLOCK
        self.product = name[:12]
        self.region = PRODUCTS.get(self.product, "?")
        self.layout = layout(self.product)
        self.hz = self.layout["hz"]           # play time vblanks per second (50 PAL, 60 USA / JP)
        try:
            self.slot = int(name[-2:])
        except ValueError:
            self.slot = -1
        h = self.header
        if h[:2] != b"SC":
            raise SaveError("block %d (%s): no 'SC' title frame" % (block, name))

    @property
    def raw(self):
        return self.card.data[self.base:self.base + SAVE_LEN]

    @property
    def header(self):
        return bytes(self.card.data[self.base:self.base + 0x200])

    def title(self):
        return bytes(self.header[4:68]).split(b"\0")[0].decode("shift_jis", "replace")

    def game_ofs(self, o):
        return self.base + GAME_OFS + o

    def checksum_ok(self):
        g = self.card.data[self.game_ofs(0):self.game_ofs(GAME_LEN)]
        return xor(g[:-1]) == g[-1]

    def fix_checksum(self):
        g = self.card.data[self.game_ofs(0):self.game_ofs(GAME_LEN)]
        self.card.data[self.game_ofs(GAME_LEN - 1)] = xor(g[:-1])

    # fields
    def get(self, name):
        o, fmt = FIELDS[name][:2]
        return struct.unpack_from(fmt, self.card.data, self.game_ofs(o))[0]

    def set(self, name, value, check=True):
        o, fmt, lo, hi, _ = FIELDS[name]
        if name == "time":
            hi = time_max(self.hz)
        value = int(value)
        if check and not lo <= value <= hi:
            raise SaveError("%s = %d out of range %d..%d" % (name, value, lo, hi))
        if name == "max_health" and check and value > 8 and not self.get("golden_bowl"):
            raise SaveError("max_health > 8 needs the golden bowl bonus (golden_bowl != 0)")
        if self.get(name) == value:
            return
        struct.pack_into(fmt, self.card.data, self.game_ofs(o), value)
        if name in ("time", "lives", "level"):
            self.update_title()
        self.fix_checksum()

    def event(self, i):
        return self.card.data[self.game_ofs(EVENTS_OFS + i)]

    def set_event(self, i, v):
        if self.event(i) != v:
            self.card.data[self.game_ofs(EVENTS_OFS + i)] = v & 0xFF
            self.fix_checksum()

    def item(self, i):
        return self.card.data[self.game_ofs(ITEMS_OFS + i)]

    def inventory(self):
        n = struct.unpack_from("<H", self.card.data, self.game_ofs(INV_COUNTER))[0]
        return list(self.card.data[self.game_ofs(INV_OFS):self.game_ofs(INV_OFS + min(n, 256))])

    def set_item(self, i, qty):
        """qty 0 removes the item from the inventory list; otherwise it is added (in front, like addItemToInventory)."""
        qty = int(qty)
        if not 0 <= qty <= 255:
            raise SaveError("item %d: quantity %d out of range 0..255" % (i, qty))
        inv = self.inventory()
        if qty == self.item(i) and ((qty > 0) == (i in inv)):
            return
        d = self.card.data
        if qty and i not in inv:
            inv.insert(0, i)
            struct.pack_into("<H", d, self.game_ofs(INV_SORT),
                             struct.unpack_from("<H", d, self.game_ofs(INV_SORT))[0] | 0x8000)
        elif not qty and i in inv:
            inv.remove(i)
        d[self.game_ofs(ITEMS_OFS + i)] = qty
        d[self.game_ofs(INV_OFS):self.game_ofs(INV_OFS + 256)] = bytes(inv) + bytes(256 - len(inv))
        struct.pack_into("<H", d, self.game_ofs(INV_COUNTER), len(inv))
        self.fix_checksum()

    def hms(self):
        """the play time as (hours, minutes, seconds) at the release's rate"""
        s = self.get("time") // self.hz
        return s // 3600, s // 60 % 60, s % 60

    def update_title(self):
        """Rewrites the digits of the Shift-JIS title (play time hhh:mm, P=lives, Level) like the release's save builder
        (PAL func_800E999C, USA func_800E954C: LAYOUTS digits); positions that are not fullwidth digits (another title
        layout) are left alone, and a release whose layout is not known (JP) keeps its title."""
        d, b = self.card.data, self.base
        pos = self.layout["digits"]
        if not pos:
            return
        t = self.get("time")
        hh = t // (3600 * self.hz)
        m = t // (60 * self.hz) - hh * 60
        vals = dict(time=(hh // 100 % 10, hh // 10 % 10, hh % 10, m // 10 % 10, m % 10),
                    lives=(self.get("lives") // 10 % 10, self.get("lives") % 10),
                    level=(self.get("level") // 10 % 10, self.get("level") % 10))
        digits = {}
        for k, vs in vals.items():
            digits.update(zip(pos[k], vs))
        for o, v in digits.items():
            cur = struct.unpack_from(">H", d, b + o)[0]
            if FW_DIGIT0 <= cur <= FW_DIGIT0 + 9:
                struct.pack_into(">H", d, b + o, FW_DIGIT0 + v)

    def summary(self):
        h, m, _ = self.hms()
        return "%s slot %02d  %s  %d:%02d  AP %d  HP %d/%d  lives %d  area %d/%d  checksum %s" % (
            self.region, self.slot, self.title(), h, m, self.get("ap"), self.get("health"),
            self.get("max_health"), self.get("lives"), self.get("area"), self.get("section"),
            "ok" if self.checksum_ok() else "BAD")


# ---- names from the user's disc -------------------------------------------------------------------------------
LANGS = {"B": "English", "S": "Español", "F": "Français", "G": "Deutsch", "I": "Italiano", "H": "Nederlands",
         "P": "Português", "D": "Dansk", "W": "Svenska"}
EVENT_TITLE_TABLE = 0x80080410     # PAL MAIN0: event id -> CFNT999 title index (0xFF = none)


def _clean(s):
    import re
    s = re.sub(r"\{[^}]*\}", "", s)
    return " ".join(s.split())


# DnSPSUB.BIN of each MFNT language letter (n = the MAINn.EXE language: 0 English .. 8 Dansk)
SPSUB_LANG = {"B": 0, "F": 1, "G": 2, "I": 3, "H": 4, "S": 5, "P": 6, "W": 7, "D": 8}
ITEM_NAME_INDEX_LEN = 200                 # ITEM_NAME_INDEX (u8[200], identity on the PAL disc), then ITEM_NAME_STRINGS


def item_names(spsub):
    """{id: name} of items 0..ITEM_USED-1 from a DnSPSUB.BIN / DnSPSUB3.BIN: ITEM_NAME_STRINGS[ITEM_NAME_INDEX[id]]
    (PAL D0SPSUB 0x801096D4 / 0x8010979C, src/d0spsub/subScreenDrawItemNames.c), list-font glyph strings
    (tools/subscreen_text.py)."""
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import subscreen_text as ST
    d = bytes(spsub)
    i = d.find(bytes(range(ITEM_USED)))
    if i < 0:
        raise SaveError("item name table not found")
    base = ST._base(d, None)
    out = {}
    for k in range(ITEM_USED):
        p = struct.unpack_from("<I", d, i + ITEM_NAME_INDEX_LEN + 4 * d[i + k])[0] - base
        e = ST._strend(d, p)
        g = struct.unpack_from("<%dh" % ((e - p) // 2 - 1), d, p)
        out[k] = " ".join("".join(ST._glyph_ch(x) for x in g).split())
    return out


def patch_item_names(patch, lang):
    """{old: new} list-name edits of a *.tombipatch.json for language lang (its "menus" / SYSTEM/DnSPSUB.BIN /
    "itemNames"; e.g. patches/es_mejorada.tombipatch.json for "S": Bola -> Mangual).  {} when there are none."""
    import json
    try:
        j = json.load(open(patch, encoding="utf-8"))
        m = j.get("menus", {}).get("SYSTEM/D%dSPSUB.BIN" % SPSUB_LANG.get(lang, 0), {}).get("itemNames", {})
    except (OSError, ValueError, AttributeError):
        return {}
    return {" ".join(o.split()): " ".join(n.split()) for o, n in m.items()}


def names_from_disc(path, lang="B", patch=None):
    """{'items': {id: name}, 'events': {id: title}, 'untitled': set(ids)} read from the user's PAL disc image (.bin/.cue/
    .iso) or extracted folder.  patch: a *.tombipatch.json whose list-name edits for lang replace the item names (the
    names the game shows with that text patch).  Raises on failure; nothing is cached on disk."""
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import text_core as C
    if path.lower().endswith(".cue"):
        import re
        txt = open(path, encoding="latin-1").read()
        m = re.search(r'FILE\s+"([^"]+)"', txt)
        if m:
            path = os.path.join(os.path.dirname(path), m.group(1))
    src = C.Source(path)
    proj = C.Project(src)
    files = src.names()
    out = {"items": {}, "events": {}, "untitled": set()}
    out["items"] = item_names(src.read_any("SYSTEM/D%dSPSUB.BIN" % SPSUB_LANG.get(lang, 0)))
    if patch:
        ren = patch_item_names(patch, lang)
        out["items"] = {k: ren.get(v, v) for k, v in out["items"].items()}
    cf = [f for f in files if f.upper().endswith("CFNT999%s.WFM" % lang)]
    try:
        exe = src.read_any("EXE/MAIN0.EXE")
    except Exception:
        exe = None
    if cf and exe and exe[:8] == b"PS-X EXE":
        titles = [_clean(proj.original(cf[0], k)) for k in range(proj.count(cf[0]))]
        t0 = struct.unpack_from("<I", exe, 0x18)[0]
        tbl = exe[EVENT_TITLE_TABLE - t0 + 0x800:][:256]
        for e in range(EVENT_COUNT):
            t = tbl[e] if e < 200 else 0xFF
            if t != 0xFF and t < len(titles):
                out["events"][e] = titles[t]
            else:
                out["untitled"].add(e)
    return out


# ---- CLI ------------------------------------------------------------------------------------------------------
def _pick(card, slot):
    ss = card.saves()
    if not ss:
        raise SaveError("no Tombi save on this card")
    if slot is None:
        return ss[0]
    for s in ss:
        if s.slot == int(slot) or s.block == int(slot):
            return s
    raise SaveError("no save in slot %s" % slot)


def main(argv):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["list", "show", "set", "roundtrip"])
    ap.add_argument("card")
    ap.add_argument("rest", nargs="*")
    ap.add_argument("-o", "--out")
    a = ap.parse_args(argv)
    c = Card(a.card)
    if a.cmd == "list":
        for b, n, sz in c.directory():
            print("block %2d  %-20s %6d bytes" % (b, n, sz))
        for s in c.saves():
            print("  " + s.summary())
    elif a.cmd == "show":
        s = _pick(c, a.rest[0] if a.rest else None)
        print(s.summary())
        for k, v in FIELDS.items():
            print("  %-15s %10d   %s" % (k, s.get(k), v[4]))
        print("  events  " + " ".join("%d:%s" % (i, "done" if s.event(i) == 255 else s.event(i))
                                      for i in range(EVENT_COUNT) if s.event(i)))
        print("  items   " + " ".join("%d x%d" % (i, s.item(i)) for i in s.inventory()))
    elif a.cmd == "set":
        if not a.out:
            raise SaveError("set needs -o OUT (the source card is never overwritten from the command line)")
        s = _pick(c, a.rest[0])
        for kv in a.rest[1:]:
            k, v = kv.split("=")
            v = int(v, 0)
            if k.startswith("event."):
                s.set_event(int(k[6:], 0), v)
            elif k.startswith("item."):
                s.set_item(int(k[5:], 0), v)
            else:
                s.set(k, v)
        c.save(a.out)
        print(Card(a.out).saves()[0].summary() if Card(a.out).saves() else "written")
    elif a.cmd == "roundtrip":
        out = c.to_bytes()
        print("identical" if out == c.orig else "DIFFERENT")
        return 0 if out == c.orig else 1
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main(sys.argv[1:]))
    except SaveError as e:
        print("error:", e, file=sys.stderr)
        sys.exit(1)
