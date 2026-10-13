#!/usr/bin/env python3
"""Tombi! (PAL) character animations: decode the animation scripts, name them, export GIF + PNG strips.

  tools/anim.py list [--json]            catalogue of Tombi's animations (id, name, frames, ticks, loop)
  tools/anim.py export OUT [--zoom N] [--tick MS]
                                         OUT/<id>_<name>.gif (animated) + OUT/<id>_<name>.png (strip) + OUT/catalog.json

Needs the extracted disc tree (game/disc/fs, see tools/area000.py) and Pillow.  Format notes: notes/formats.md ("Animations").

Data (decoded from AnimAdvance / AnimJump / AnimLoadDuration in MAIN0 and ObjSetAnimFromTable in every area overlay):
 - the overlay (all 17 X0nn.BIN identical) holds a table of 90 u32 pointers at 0x800E8404 (file offset 0x3DC); the player's animation
   id (TObj+0x2C, set by PlayerSetAnimIfChanged) indexes it; the pointers go to scripts in MAIN0's data (0x80010748..0x80011800);
 - a script entry = 8 bytes `u16 frame | u16 box | u16 arg | u16 dur_flags`: frame = WPP frame of Tombi's 358 frame table
   (AREA00/A004.000 first table, drawn with the SPR 0x30 parts for the offsets), dur_flags & 0x3FFF = ticks the frame is shown,
   bits 14/15 = what happens when the time is up: 00 next entry, 0x4000 the next 4 bytes are a pointer, jump there (loop; the 4 bytes are
   not an entry), 0x8000 end (the last frame is held and the caller is told "finished" every dur ticks), 0xC000 jump + "finished";
 - several ids point into the middle of a longer script (the same movement starting at a later frame), or share a script.
`box` is the hit box index when the entry is advanced by AnimAdvanceWithBox (enemy table), `arg` is unknown (0 for most, signed
values like -14 or 200..128 in the 4/13/14 scripts).
"""
import argparse, io, json, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import area000 as A

TICK_MS = 40            # assumed: one game tick = 2 VSyncs at 50 Hz (PAL); only used for the preview speed
TABLE_ADDR = 0x800E8404  # pointer table in the overlay (ObjSetAnimFromTable)
OVERLAY_BASE = 0x800E8028
N_ANIMS = 90
TOMBI_WPP = "AREA00/A004.000"

# name per animation id: from the state code that selects it (X000_BIN_decomp.c, src/x008) cross-checked with the rendered strips.
# Ids missing here keep the numbered name `anim_NN`.  The confidence of each is in NAMES_NOTE.
NAMES = {
    0: "idle", 1: "walk", 2: "jog", 3: "run", 4: "fall", 5: "crouch", 6: "climb_back", 7: "climb_back_still", 9: "hurt_knockback",
    10: "climb_side_idle", 11: "climb_side_up", 12: "climb_side_down", 13: "jump_start", 14: "jump", 15: "jump_alt", 16: "flung_air",
    30: "edge_balance", 33: "crouch_walk", 35: "climb_back_grab", 36: "turn_around", 45: "climb_side_up_b", 47: "knocked_down",
    48: "get_up", 49: "roll_land", 53: "dive_horizontal", 55: "get_up_slow", 57: "knockdown_big", 61: "recover_flip",
    67: "swim_stroke", 68: "swim_float", 69: "swim_turn", 71: "swim_stroke_slow", 78: "victory_pose", 80: "shocked_skeleton",
    81: "shiver_cold", 83: "burnt_fall",
}
# how each name was found: H = state code (pad bits / speed thresholds in FUN_800efa80, FUN_801099b8...), M = state code + strip,
# L = strip only (guess from the pose).  Everything not listed keeps its number.
NAMES_NOTE = {i: "H" for i in (0, 1, 2, 3, 10, 11, 12)}
NAMES_NOTE.update({i: "M" for i in (4, 5, 6, 9, 13, 14, 15, 33, 36, 48, 49, 67, 68, 80)})
NAMES_NOTE.update({i: "L" for i in NAMES if i not in NAMES_NOTE})


def name_of(i):
    return NAMES.get(i, "anim_%02d" % i)


# ------------------------------------------------------------------ script decoding
def _fs_reader(fs=None):
    fs = fs or A.FS
    return lambda name: open(os.path.join(fs, name), "rb").read()


class Exe:
    """MAIN0.EXE (script data) + AREA00/X000.BIN (the pointer table); `reader(name)` returns the bytes of a disc file."""

    def __init__(self, reader=None):
        reader = reader or _fs_reader()
        d = reader("EXE/MAIN0.EXE")
        self.base = struct.unpack_from("<I", d, 0x18)[0]
        self.d = d
        self.ov = reader("AREA00/X000.BIN")

    def u16(self, a):
        o = a - self.base + 0x800
        return struct.unpack_from("<H", self.d, o)[0]

    def u32(self, a):
        o = a - self.base + 0x800
        return struct.unpack_from("<I", self.d, o)[0]

    def table(self):
        o = TABLE_ADDR - OVERLAY_BASE
        return list(struct.unpack_from("<%dI" % N_ANIMS, self.ov, o))


def decode_script(exe, start, starts=()):
    """Walk the script from `start` like AnimAdvance: entries [(addr, frame, box, arg, dur, flags)], loop info.
    returns dict(entries, loop (index the script jumps back to or None), end ('loop'|'hold'|'jump'), finished (the end signals))."""
    ents, seen, a = [], {}, start
    loop, end, signal = None, "hold", False
    while True:
        if a in seen:             # reached an entry again without a jump record: cannot happen in the data, stop
            loop, end = seen[a], "loop"
            break
        fr, box, arg, v = (exe.u16(a + 2 * k) for k in range(4))
        seen[a] = len(ents)
        ents.append(dict(addr=a, frame=fr, box=box, arg=arg - 0x10000 if arg >= 0x8000 else arg, dur=v & 0x3FFF, flags=v & 0xC000))
        f = v & 0xC000
        if f == 0:
            a += 8
        elif f == 0x8000:
            end, signal = "hold", True
            break
        else:
            tgt = exe.u32(a + 8)
            signal = f == 0xC000
            if tgt in seen:
                loop, end = seen[tgt], "loop"
                break
            a = tgt   # jump into code not walked yet (a later part of another script): keep walking
            end = "loop"
            ents[-1]["jump"] = tgt
    return dict(entries=ents, loop=loop, end=end, signal=signal)


def catalog(reader=None):
    exe = Exe(reader)
    tab = exe.table()
    firsts = {}
    out = []
    for i, p in enumerate(tab):
        s = decode_script(exe, p)
        ticks = sum(e["dur"] for e in s["entries"])
        first = firsts.setdefault(p, i)
        out.append(dict(id=i, name=name_of(first), start="%08X" % p, alias_of=None if first == i else first,
                        entries=s["entries"], loop=s["loop"], end=s["end"], signal=s["signal"], ticks=ticks,
                        frames=[e["frame"] for e in s["entries"]], confidence=NAMES_NOTE.get(i, "")))
    return out


# ------------------------------------------------------------------ rendering
class Sprites:
    """Tombi's 358 frames (WPP 0x60 of AREA00/A004) as PIL RGBA images with their draw offsets (SPR 0x30 parts)."""

    def __init__(self, row=0):
        import vram, wpp
        from PIL import Image
        self.Image = Image
        lst = A.find_list(TOMBI_WPP)[2]
        tabs = [t for t in vram.wpp_tables(lst) if t[0] == TOMBI_WPP]
        frames = next(f for _, ti, f in tabs if len(f) == 358)
        spr = next(d for k, n, d in wpp.spr_candidates(TOMBI_WPP) if k == 0x30 and n == 358)
        parts = vram.parse_spr(spr)
        blob = A.read_resource(A.find_clut(TOMBI_WPP)["name"])
        pal = [(0, 0, 0, 0)] + [tuple(A.rgb555(v)) + (255,) for v in struct.unpack_from("<15H", blob, (row * 256 + 1) * 2)]
        self.n = len(frames)
        self.img, self.off = [], []
        for i, o in enumerate(frames):
            rows, w, h = vram.wpp_plane(o)
            im = Image.new("RGBA", (w, h))
            im.putdata([pal[b] for r in rows for b in r])
            ps = parts[i]
            ox = min(p[12] for p in ps) if ps else 0
            oy = min(p[13] for p in ps) if ps else 0
            self.img.append(im)
            self.off.append((ox, oy))

    def frame(self, i):
        return self.img[i], self.off[i]


def bounds(spr, frames):
    x0 = y0 = 1 << 20
    x1 = y1 = -(1 << 20)
    for f in set(frames):
        im, (ox, oy) = spr.frame(f)
        x0, y0 = min(x0, ox), min(y0, oy)
        x1, y1 = max(x1, ox + im.width), max(y1, oy + im.height)
    return x0, y0, x1, y1


def compose(spr, f, box, zoom=1, bg=(0, 0, 0, 0)):
    """One animation frame on a canvas `box` (x0, y0, x1, y1) in game coordinates; the origin (object position) is marked by none."""
    x0, y0, x1, y1 = box
    c = spr.Image.new("RGBA", (x1 - x0, y1 - y0), bg)
    im, (ox, oy) = spr.frame(f)
    c.alpha_composite(im, (ox - x0, oy - y0))
    if zoom != 1:
        c = c.resize((c.width * zoom, c.height * zoom), spr.Image.NEAREST)
    return c


def timeline(an):
    """[(entry index, frame, ticks)] of one pass of the script (to the end or one loop)."""
    return [(k, e["frame"], max(e["dur"], 1)) for k, e in enumerate(an["entries"])]


def gif_frames(spr, an, zoom=2, tick=TICK_MS, bg=(40, 40, 56, 255)):
    box = bounds(spr, an["frames"])
    ims, durs = [], []
    for k, fr, t in timeline(an):
        ims.append(compose(spr, fr, box, zoom, bg))
        durs.append(t * tick)
    return ims, durs


def save_gif(spr, an, path, zoom=2, tick=TICK_MS):
    ims, durs = gif_frames(spr, an, zoom, tick)
    ims[0].save(path, save_all=True, append_images=ims[1:], duration=durs, loop=0, disposal=2)


def strip(spr, an, zoom=2, bg=(40, 40, 56, 255)):
    box = bounds(spr, an["frames"])
    cells = [compose(spr, e["frame"], box, zoom, bg) for e in an["entries"]]
    w, h = cells[0].size
    s = spr.Image.new("RGBA", ((w + 2) * len(cells), h), (0, 0, 0, 255))
    for k, c in enumerate(cells):
        s.paste(c, (k * (w + 2), 0))
    return s


def fname(an):
    return "%02d_%s" % (an["id"], an["name"])


def export(out, zoom=2, tick=TICK_MS, reader=None, log=print):
    os.makedirs(out, exist_ok=True)
    cat = catalog(reader)
    spr = Sprites()
    n = 0
    for an in cat:
        if an["alias_of"] is not None or not an["entries"]:
            continue
        save_gif(spr, an, os.path.join(out, fname(an) + ".gif"), zoom, tick)
        strip(spr, an, zoom).save(os.path.join(out, fname(an) + ".png"))
        n += 1
    with open(os.path.join(out, "catalog.json"), "w") as f:
        json.dump(dict(owner="Tombi", wpp=TOMBI_WPP, tick_ms=tick, animations=cat), f, indent=1)
    log("%d animations -> %s" % (n, out))
    return n


def cmd_list(a):
    cat = catalog()
    if a.json:
        print(json.dumps(cat, indent=1))
        return 0
    for an in cat:
        al = " (= %02d)" % an["alias_of"] if an["alias_of"] is not None else ""
        lp = "loop->%d" % an["loop"] if an["loop"] is not None else an["end"]
        print("%02d %-22s %s %2d frames %4d ticks %-8s%s%s" % (an["id"], an["name"], an["start"], len(an["entries"]), an["ticks"], lp,
                                                              " finished-signal" if an["signal"] else "", al))
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("list")
    p.add_argument("--json", action="store_true")
    p = sp.add_parser("export")
    p.add_argument("out")
    p.add_argument("--zoom", type=int, default=2)
    p.add_argument("--tick", type=int, default=TICK_MS, help="ms per game tick in the GIF (default %d)" % TICK_MS)
    a = ap.parse_args(argv)
    if a.cmd == "list":
        return cmd_list(a)
    export(a.out, a.zoom, a.tick)
    return 0


if __name__ == "__main__":
    sys.exit(main())
