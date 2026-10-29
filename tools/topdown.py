#!/usr/bin/env python3
"""Top-down ("I0NFO2") sections of Tombi! (PAL): parse and render their data from the disc.  Notes: docs/CIVILIZATION.md.

The top-down engine (SYSTEM/I0NFO2.BIN, loaded at the overlay base 0x800E8028 instead of the area's X0nn.BIN) runs
AREA05 sections 0/2, AREA08 sections 0/2 and AREA19 section 2 on the retail disc, and was meant to run the cut section 0
of AREA11/16/17/18. Per section it needs, from the section load list (SYS/LD0ARnn.BIN list section+1):

  kind 0x8E  cell map   u16 w, u16 h (8x8 px cells), then 0x4000 bytes copied to TOPDOWN_MAP_TILES
                        (topdownLoadMapTiles 0x800ECA74): low 7 bits = collision shape 0..9, bit 7 = trigger
  kind 0x8F  picture    a TIM: 8 bpp, one 256 colour CLUT (+0x14), pixels (+0x220), ((w>>2)*32) x ((h>>2)*32) px
  kind 0x62  WPP        Tombi's top-down frames (0xCA3C bytes, the same block in every top-down section)
  kind 0x3A  SPR        Tombi's top-down sprite table (0x11DC bytes, same in every section)
  kind 0x3B..0x3E SPR   actor / scenery sprite tables (SPR_DATA[n], indexed by the *_ACTOR_DEFS spad field)
  kind 0x80+n APD       placement records (16 bytes, tools/objects.py)
  GRPX pages + CLUTnn   the texture pages the sprite tables point into
and the code overlay I0NFO2.BIN (kind 0xF0). The NPCs and props of the cut areas are not in APD but in I0NFO2's
NPC_SET_* tables (spawned by the unlayered variant object 0xC..0x11), which `npcs` dumps.

  tools/topdown.py lists                      the top-down section load lists of the disc (and the empty cut ones)
  tools/topdown.py info AREA:SEC              map / picture / sprite table sizes of one working section (e.g. 5:0)
  tools/topdown.py render AREA:SEC OUT.png [--grid] [--scale N]
                                              the section's picture with collision cells (--grid), APD records and,
                                              for AREA08/19, the NPC set records drawn over it
  tools/topdown.py grid AREA:SEC OUT.png      the cell map alone (one colour per shape, triggers outlined)
  tools/topdown.py npcs [SET]                 NPC_SET_0C..11 records of I0NFO2 (SET hex, e.g. 0D)
  tools/topdown.py defs                       *_ACTOR_DEFS / *_SCENERY_DEFS / anim tables of I0NFO2
  tools/topdown.py plan AREA OUT.png [--from AREA:SEC]
                                              the actors / props of a cut area (11/16/17/18) on a blank plan, or over
                                              the picture of a working section (layout check only)
  tools/topdown.py warps                      exit tables (MAIN0 0x80079A48) of the top-down areas

Reads game/disc/fs (TOMBI_FS) through tools/area000.py; writes only the PNG you name. Pure Python.
"""
import argparse, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import area000 as A  # noqa: E402

FS = A.FS
I0NFO2_BASE = 0x800E8028
MAIN0_BASE = 0x80010000 - 0x800  # file offset 0 of MAIN0.EXE (PS-X EXE header 0x800, text at 0x80010000)

# where the engine runs (area, section) -> NPC set variant (unlayered type 4, MAIN0 PTR_8007BEE4) and the
# per-area actor/scenery handler (updateL2Type30Object 0x8003BCD0 / dispatchAreaDialogInit 0x8002E414)
TOPDOWN = {
    (5, 0): 0x07, (5, 2): 0x07, (8, 0): 0x0C, (8, 2): 0x0C, (19, 2): 0x11,
    (11, 0): 0x0D, (16, 0): 0x0E, (17, 0): 0x0F, (18, 0): 0x10,
}
CUT = {11: 0x0D, 16: 0x0E, 17: 0x0F, 18: 0x10}
NPC_SETS = {  # variant: (SpawnA list pointer, SpawnB list pointer(s)); I0NFO2 data (notes/data_names_i0nfo2.csv)
    0x0C: (0x800FFE20, ()), 0x0D: (0x80100080, (0x800FFE24,)), 0x0E: (0x80100364, ("*0x801001D4", "*0x801001D8")),
    0x0F: (0x80100784, (0x80100368,)), 0x10: (0x801008D8, ()), 0x11: (0x80100B00, (0x801008DC,)),
}
SHAPE_RGB = {0: None, 1: (255, 64, 64), 2: (255, 128, 0), 3: (255, 255, 0), 4: (128, 255, 0), 5: (0, 160, 255),
             6: (0, 64, 255), 7: (160, 0, 255), 8: (255, 0, 255), 9: (255, 255, 255)}


# ---------------------------------------------------------------- disc access
def i0nfo2():
    return open(os.path.join(FS, "SYSTEM", "I0NFO2.BIN"), "rb").read()


def main0():
    return open(os.path.join(FS, "EXE", "MAIN0.EXE"), "rb").read() if os.path.exists(os.path.join(FS, "EXE", "MAIN0.EXE")) \
        else open(os.path.join(ROOT, "game", "MAIN0.EXE"), "rb").read()


def u32(d, base, a):
    return struct.unpack_from("<I", d, a - base)[0]


def section_blocks(area, sec):
    """[(record, bytes)] of the load list of AREAnn section `sec` (list index sec + 1)."""
    return A.list_blocks("AR%02d" % area, sec + 1)


def section_data(area, sec):
    """{'grid': bytes, 'tim': bytes, 'spr': {slot: bytes}, 'apd': {slot: bytes}} of a top-down section."""
    out = {"grid": None, "tim": None, "spr": {}, "apd": {}, "wpp": None, "code": None}
    for r, d in section_blocks(area, sec):
        k = r["kind"]
        if r["name"].endswith("I0NFO2.BIN") or r["name"].endswith("I0NFO.BIN"):
            out["code"] = r["name"]
        if d is None:
            continue
        if k == 0x8E:
            out["grid"] = d
        elif k == 0x8F:
            out["tim"] = d
        elif k & 0xF0 == 0x30:
            out["spr"][k & 15] = d
        elif k & 0xF0 == 0x80:
            out["apd"][k & 15] = d
        elif k == 0x62:
            out["wpp"] = d
    return out


def parse_grid(d):
    w, h = struct.unpack_from("<HH", d)
    return w, h, d[4:4 + 0x4000]


def parse_tim(d):
    """(width px, height px, 256 RGB tuples, pixel bytes): the kind 0x8F block is a plain 8 bpp TIM."""
    magic, flags = struct.unpack_from("<II", d)
    if magic != 0x10 or flags & 7 != 1 or not flags & 8:
        raise ValueError("not an 8 bpp TIM with CLUT (magic %x flags %x)" % (magic, flags))
    clen = struct.unpack_from("<I", d, 8)[0]
    clut = []
    for i in range(256):
        v = struct.unpack_from("<H", d, 0x14 + 2 * i)[0]
        clut.append(((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31))
    p = 8 + clen
    _, _, _, ww, hh = struct.unpack_from("<IHHHH", d, p)
    return ww * 2, hh, clut, d[p + 12:p + 12 + ww * 2 * hh]


def parse_apd(d):
    """16-byte placement records up to the first type >= 0xFF (src/FUN_8004cb00.c)."""
    out = []
    for o in range(0, len(d) - 15, 16):
        b = d[o:o + 16]
        if b[2] >= 0xFF:
            break
        x, y, z = struct.unpack_from("<hhh", b, 6)
        out.append({"b0": b[0], "layer": b[1] & 0x7F, "type": b[2], "b3": b[3], "sub": b[4], "x": x, "y": y, "z": z,
                    "bf": b[15]})
    return out


def npc_set(var, d=None):
    """(actors, props) of NPC set `var`: SpawnA (28 B: type, room, frame, anim, x0 y0 x1 y1 bounds, dialogue case,
    behaviour, x, y, subtype, b0c; spawned as L2 type 0x30) and SpawnB (16 B: type, room, -, anim, x, y, subtype,
    b0c; spawned as L3 type 0x1D), both ended by type 0xFF (spawnNpcSet0D.. 0x800FB054..)."""
    d = d or i0nfo2()
    pa, pbs = NPC_SETS[var]
    actors, props = [], []
    p = u32(d, I0NFO2_BASE, pa)
    while True:
        r = struct.unpack_from("<14h", d, p - I0NFO2_BASE)
        if r[0] == 0xFF:
            break
        actors.append(dict(zip(("type", "room", "frame", "anim", "bx0", "by0", "bx1", "by1", "dlg", "beh", "x", "y",
                                "sub", "bc"), r)))
        p += 28
    for pb in pbs:
        q = u32(d, I0NFO2_BASE, int(pb[1:], 16)) if isinstance(pb, str) else pb
        lst = []
        while True:
            r = struct.unpack_from("<8h", d, q - I0NFO2_BASE)
            if r[0] == 0xFF:
                break
            lst.append(dict(zip(("type", "room", "w4", "anim", "x", "y", "sub", "bc"), r)))
            q += 16
        props.append(lst)
    return actors, props


# ---------------------------------------------------------------- PNG / drawing
class Canvas:
    def __init__(self, w, h, bg=(0, 0, 0)):
        self.w, self.h = w, h
        self.px = bytearray(bytes(bg) * (w * h))

    def put(self, x, y, c, a=1.0):
        if 0 <= x < self.w and 0 <= y < self.h:
            i = (y * self.w + x) * 3
            if a >= 1:
                self.px[i:i + 3] = bytes(c)
            else:
                self.px[i:i + 3] = bytes(int(self.px[i + k] * (1 - a) + c[k] * a) for k in range(3))

    def rect(self, x0, y0, x1, y1, c, fill=False, a=1.0):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                if fill or y in (y0, y1) or x in (x0, x1):
                    self.put(x, y, c, a)

    def line(self, x0, y0, x1, y1, c):
        n = max(abs(x1 - x0), abs(y1 - y0), 1)
        for i in range(n + 1):
            self.put(x0 + (x1 - x0) * i // n, y0 + (y1 - y0) * i // n, c)

    def png(self, scale=1):
        rows = []
        for y in range(self.h):
            r = self.px[y * self.w * 3:(y + 1) * self.w * 3]
            if scale > 1:
                r = b"".join(r[i:i + 3] * scale for i in range(0, len(r), 3))
            rows += [bytes(r)] * scale
        return A.png_bytes(self.w * scale, self.h * scale, rows, "rgb")


def world_to_px(x, y):
    """Map pixel of a world position: the collision lookup is cell ((x + 0x80) >> 3, (y - 0x28) >> 3)
    (topdownCollideMapTile 0x800EE18C), the cell map and the picture share their origin."""
    return x + 0x80, y - 0x28


def draw_grid(cv, w, h, cells, alpha=0.45):
    for cy in range(h):
        for cx in range(w):
            v = cells[cy * w + cx] if cy * w + cx < len(cells) else 0
            c = SHAPE_RGB.get(v & 0x7F, (128, 128, 128))
            if c:
                cv.rect(cx * 8, cy * 8, cx * 8 + 7, cy * 8 + 7, c, fill=True, a=alpha)
            if v & 0x80:
                cv.rect(cx * 8, cy * 8, cx * 8 + 7, cy * 8 + 7, (0, 255, 255))


def marker(cv, x, y, c, r=4, cross=False):
    px, py = world_to_px(x, y)
    cv.rect(px - r, py - r, px + r, py + r, c)
    cv.rect(px - r + 1, py - r + 1, px + r - 1, py + r - 1, (0, 0, 0))
    if cross:
        cv.line(px - r, py - r, px + r, py + r, c)
        cv.line(px - r, py + r, px + r, py - r, c)


TRIGGER_RECT_LISTS = 0x800FEC9C  # short (*[5])[4]: AREA05, 11, 16, 17, 19 {x0 y0 x1 y1} house rects, 0xFF end
TRIGGER_AREAS = (5, 11, 16, 17, 19)


def trigger_rects(area, d=None):
    """House rectangles of topdownPlayerMainStep: standing on a trigger cell (bit 7) inside rect i sets the current
    room to i + 1 (D_800A60A3); objects whose room (wb6) differs are hidden, so roofs vanish and interiors show."""
    if area not in TRIGGER_AREAS:
        return []
    d = d or i0nfo2()
    p = u32(d, I0NFO2_BASE, TRIGGER_RECT_LISTS + 4 * TRIGGER_AREAS.index(area))
    out = []
    while True:
        r = struct.unpack_from("<4h", d, p - I0NFO2_BASE)
        if r[0] == 0xFF:
            return out
        out.append(r)
        p += 8


def draw_rooms(cv, area):
    for x0, y0, x1, y1 in trigger_rects(area):
        a, b = world_to_px(x0, y0), world_to_px(x1, y1)
        cv.rect(a[0], a[1], b[0], b[1], (80, 160, 255))


def draw_npcs(cv, var):
    actors, props = npc_set(var)
    for lst in props:
        for p in lst:
            marker(cv, p["x"], p["y"], (255, 200, 0), 3, cross=True)
    for a in actors:
        if a["beh"]:  # patrol box of PatrolV/H/Diag (box = absolute x0 y0 x1 y1 for 0D/0F, relative for 0E)
            x0, y0, x1, y1 = a["bx0"], a["by0"], a["bx1"], a["by1"]
            if var == 0x0E:
                x0, y0, x1, y1 = a["x"] + x0, a["y"] + y0, a["x"] + x1, a["y"] + y1
            p0, p1 = world_to_px(x0, y0), world_to_px(x1, y1)
            cv.rect(min(p0[0], p1[0]), min(p0[1], p1[1]), max(p0[0], p1[0]), max(p0[1], p1[1]), (0, 255, 0))
        marker(cv, a["x"], a["y"], (255, 0, 0) if a["room"] == 0 else (255, 0, 255), 5)


def render(area, sec, out, grid=False, scale=1):
    s = section_data(area, sec)
    if s["tim"] is None or s["grid"] is None:
        raise SystemExit("AREA%02d section %d has no map (empty load list or not a top-down section)" % (area, sec))
    w, h, clut, pix = parse_tim(s["tim"])
    gw, gh, cells = parse_grid(s["grid"])
    cv = Canvas(max(w, gw * 8), max(h, gh * 8), (40, 0, 40))
    for y in range(h):
        row = pix[y * w:(y + 1) * w]
        for x, v in enumerate(row):
            cv.put(x, y, clut[v])
    if grid:
        draw_grid(cv, gw, gh, cells)
    for slot, d in sorted(s["apd"].items()):
        for r in parse_apd(d):
            marker(cv, r["x"], r["y"], (0, 255, 255), 4)
    draw_rooms(cv, area)
    if (area, sec) in TOPDOWN and TOPDOWN[(area, sec)] in NPC_SETS:
        draw_npcs(cv, TOPDOWN[(area, sec)])
    open(out, "wb").write(cv.png(scale))
    print("%s: %dx%d picture, %dx%d cells, CLUT from the TIM" % (out, w, h, gw, gh))


def cmd_grid(area, sec, out, scale=2):
    s = section_data(area, sec)
    gw, gh, cells = parse_grid(s["grid"])
    cv = Canvas(gw * 8, gh * 8)
    draw_grid(cv, gw, gh, cells, alpha=1.0)
    open(out, "wb").write(cv.png(scale))
    hist = {}
    for v in cells[:gw * gh]:
        hist[v] = hist.get(v, 0) + 1
    print("%dx%d cells; values: %s" % (gw, gh, " ".join("%02x:%d" % kv for kv in sorted(hist.items()))))


def plan(area, out, base=None, scale=1):
    var = CUT.get(area) or TOPDOWN.get((area, 0))
    actors, props = npc_set(var)
    pts = [world_to_px(a["x"], a["y"]) for a in actors] + [world_to_px(p["x"], p["y"]) for l in props for p in l]
    if base:
        ba, bs = base
        s = section_data(ba, bs)
        w, h, clut, pix = parse_tim(s["tim"])
    else:
        w = (max(p[0] for p in pts) // 32 + 2) * 32
        h = (max(p[1] for p in pts) // 32 + 2) * 32
    cv = Canvas(w, h, (24, 24, 32))
    if base:
        for y in range(h):
            for x, v in enumerate(pix[y * w:(y + 1) * w]):
                cv.put(x, y, tuple(c // 2 for c in clut[v]))
    else:
        for x in range(0, w, 32):
            cv.line(x, 0, x, h - 1, (48, 48, 64))
        for y in range(0, h, 32):
            cv.line(0, y, w - 1, y, (48, 48, 64))
    draw_rooms(cv, area)
    draw_npcs(cv, var)
    open(out, "wb").write(cv.png(scale))
    print("%s: NPC set %02X of AREA%02d, %d actors, %d props, plan %dx%d px (world x+128, y-40)"
          % (out, var, area, len(actors), sum(map(len, props)), w, h))


# ---------------------------------------------------------------- text dumps
def cmd_lists():
    for area in (5, 8, 11, 16, 17, 18, 19):
        lists = A.load_lists().get("AR%02d" % area, [])
        for i, lst in enumerate(lists[1:]):
            names = [r["name"].split("/")[-1] for r in lst if r["type"] != 0xFFFFFFFF]
            kinds = " ".join("%02x" % r["kind"] for r in lst)
            tag = "TOPDOWN" if (area, i) in TOPDOWN else ""
            print("AREA%02d s%-2d %-7s %2d records  %s" % (area, i, tag, len(lst), kinds if lst else "(empty)"))
            if tag and lst:
                print("            files: %s" % " ".join(names))


def cmd_info(area, sec):
    s = section_data(area, sec)
    gw, gh, cells = parse_grid(s["grid"])
    w, h, clut, pix = parse_tim(s["tim"])
    print("AREA%02d section %d (code %s)" % (area, sec, s["code"]))
    print("  cell map  %d x %d cells (%d x %d px), buffer %d bytes" % (gw, gh, gw * 8, gh * 8, len(s["grid"])))
    print("  picture   %d x %d px 8 bpp (expected %d x %d from the cell map)" % (w, h, (gw >> 2) * 32, (gh >> 2) * 32))
    for slot, d in sorted(s["spr"].items()):
        n = struct.unpack_from("<H", d, 2)[0] // 4
        print("  SPR_DATA[%d] %5d bytes, %d frames" % (slot, len(d), n))
    for slot, d in sorted(s["apd"].items()):
        for r in parse_apd(d):
            print("  APD[%d] b0=%02x L%d type %02x b3 %d sub %d (%d, %d, %d) bf %02x" % (
                slot, r["b0"], r["layer"], r["type"], r["b3"], r["sub"], r["x"], r["y"], r["z"], r["bf"]))
    if s["wpp"]:
        print("  WPP 0x62  %d bytes" % len(s["wpp"]))


def cmd_npcs(which=None):
    d = i0nfo2()
    for var in sorted(NPC_SETS):
        if which is not None and var != which:
            continue
        actors, props = npc_set(var, d)
        where = [k for k, v in TOPDOWN.items() if v == var]
        print("NPC set %02X  (%s)" % (var, ", ".join("AREA%02d s%d" % k for k in where)))
        for i, a in enumerate(actors):
            print("  actor %2d sub %d dlg %2d beh %d room %3d frame %d anim %d b0c %2d at (%4d,%4d) box (%d,%d)-(%d,%d)" % (
                i, a["sub"], a["dlg"], a["beh"], a["room"], a["frame"], a["anim"], a["bc"], a["x"], a["y"],
                a["bx0"], a["by0"], a["bx1"], a["by1"]))
        for j, lst in enumerate(props):
            for i, p in enumerate(lst):
                print("  prop%s %2d sub %d anim %2d room %d type %d at (%4d,%4d)" % (
                    "" if j == 0 else "(alt)", i, p["sub"], p["anim"], p["room"], p["type"], p["x"], p["y"]))


DEFS = [  # name, address, kind ('obj' = {u8 box[4]; u16 w1e; s16 spad; void **anims}, 'obj3' = {u16 w1e; s16 _; s16 spad; s16 _; void **anims}), count
    ("TOPDOWN_ACTOR_DEFS (AREA05)", 0x800FED7C, "obj", 3),
    ("TOPDOWN_A08_ACTOR_DEFS", 0x800FEE58, "obj", 2),
    ("TOPDOWN_A11_ACTOR_DEFS", 0x800FEEB8, "obj", 5),
    ("TOPDOWN_A16_ACTOR_DEFS", 0x800FEEF4, "obj", 4),
    ("TOPDOWN_A17_ACTOR_DEFS", 0x800FEF64, "obj", 5),
    ("TOPDOWN_A18_ACTOR_DEFS", 0x800FEFA0, "obj", 7),
    ("TOPDOWN_A19_ACTOR_DEFS", 0x800FEFF4, "obj", 8),
    ("TOPDOWN_A05_SCENERY_DEFS", 0x800FEDC4, "obj3", 1),
    ("TOPDOWN_A11_SCENERY_DEFS", 0x800FF054, "obj3", 2),
    ("TOPDOWN_A16_SCENERY_DEFS", 0x800FEDD0, "obj3", 3),
    ("TOPDOWN_A17_SCENERY_DEFS", 0x800FEE04, "obj3", 3),
    ("TOPDOWN_A19_SCENERY_DEFS", 0x800FEE34, "obj3", 3),
]


def cmd_defs():
    d = i0nfo2()
    for name, addr, kind, n in DEFS:
        print(name, hex(addr))
        for i in range(n):
            o = addr - I0NFO2_BASE + 12 * i
            if kind == "obj":
                b0, b1, b2, b3, w1e, spad, anims = struct.unpack_from("<BBBBHhI", d, o)
                head = "box (%d,%d,%d,%d) w1e %04x SPR_DATA[%d]" % (b0, b1, b2, b3, w1e, spad)
            else:
                w1e, _, spad, _, anims = struct.unpack_from("<HhhhI", d, o)
                head = "w1e %04x SPR_DATA[%d]" % (w1e, spad)
            ptrs = []
            if I0NFO2_BASE <= anims < I0NFO2_BASE + len(d):
                for k in range(24):
                    v = u32(d, I0NFO2_BASE, anims + 4 * k)
                    if not I0NFO2_BASE <= v < I0NFO2_BASE + len(d):
                        break
                    ptrs.append(v)
            print("  [%d] %s anims %08x (%d scripts)" % (i, head, anims, len(ptrs)))


def cmd_warps():
    exe = main0()
    tab = [struct.unpack_from("<I", exe, 0x80079A48 - MAIN0_BASE + 4 * a)[0] for a in range(20)]
    for area in (5, 8, 11, 16, 17, 18, 19):
        t = tab[area]
        secs = []
        for s in range(12):
            p = struct.unpack_from("<I", exe, t - MAIN0_BASE + 4 * s)[0]
            if not 0x80070000 < p < 0x80090000 or (s and t + 4 * s in tab):
                break
            secs.append(p)
        starts = sorted(set(secs) | set(x for x in tab if 0x80070000 < x < 0x80090000))
        for s, p in enumerate(secs):
            nxt = [q for q in starts if q > p]
            n = min((nxt[0] - p) // 8 if nxt else 4, 8)
            ents = []
            for i in range(n):
                x, y, k, na, ns, nsp = struct.unpack_from("<hhBBBB", exe, p - MAIN0_BASE + 8 * i)
                ents.append("exit %d -> AREA%02d s%d spawn %d" % (i, na, ns, nsp))
            print("AREA%02d s%-2d %s" % (area, s, "; ".join(ents)))


def parse_as(s):
    a, b = s.split(":")
    return int(a), int(b)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    sp.add_parser("lists")
    p = sp.add_parser("info"); p.add_argument("where")
    p = sp.add_parser("render"); p.add_argument("where"); p.add_argument("out")
    p.add_argument("--grid", action="store_true"); p.add_argument("--scale", type=int, default=1)
    p = sp.add_parser("grid"); p.add_argument("where"); p.add_argument("out"); p.add_argument("--scale", type=int, default=2)
    p = sp.add_parser("npcs"); p.add_argument("set", nargs="?")
    sp.add_parser("defs")
    sp.add_parser("warps")
    p = sp.add_parser("plan"); p.add_argument("area", type=int); p.add_argument("out")
    p.add_argument("--from", dest="base"); p.add_argument("--scale", type=int, default=1)
    a = ap.parse_args()
    if a.cmd == "lists":
        cmd_lists()
    elif a.cmd == "info":
        cmd_info(*parse_as(a.where))
    elif a.cmd == "render":
        render(*parse_as(a.where), a.out, a.grid, a.scale)
    elif a.cmd == "grid":
        cmd_grid(*parse_as(a.where), a.out, a.scale)
    elif a.cmd == "npcs":
        cmd_npcs(int(a.set, 16) if a.set else None)
    elif a.cmd == "defs":
        cmd_defs()
    elif a.cmd == "warps":
        cmd_warps()
    elif a.cmd == "plan":
        plan(a.area, a.out, parse_as(a.base) if a.base else None, a.scale)


if __name__ == "__main__":
    main()
