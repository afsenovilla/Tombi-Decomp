#!/usr/bin/env python3
"""Tombi! (PAL) character animations: decode the animation scripts, name them, export GIF + PNG strips.

  tools/anim.py list [--owner all|tombi|<handler>] [--json]
                                         catalogue (owner, id, name, script, frames, ticks, loop); default owner tombi
  tools/anim.py export OUT [--owner ...] [--zoom N] [--tick MS]
                                         OUT/<owner>/<id>.gif (animated) + <id>.png (strip) + OUT/catalog.json
                                         (owners: tombi, then every enemy/NPC/object handler per area, see tools/animobj.py)

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
    """Display name: the user/default label (tools/labels.py, id "anim:tombi:N") over the built-in name over anim_NN."""
    import labels
    return labels.name("anim:tombi:%d" % i) or NAMES.get(i, "anim_%02d" % i)


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
        def pal_of(r):
            return [(0, 0, 0, 0)] + [tuple(A.rgb555(v)) + (255,) for v in struct.unpack_from("<15H", blob, (r * 256 + 1) * 2)]
        pals = {}
        self.n = len(frames)
        self.img, self.off = [], []
        for i, o in enumerate(frames):
            rows, w, h = vram.wpp_plane(o)
            im = Image.new("RGBA", (w, h))
            # every part has its own CLUT (the game draws the upper body with row 480, the lower body with row 481 of the area
            # CLUT, captured from the port): colour each part's box of the frame with its palette
            px = [pals.setdefault(row, pal_of(row))[b] for r in rows for b in r]
            for p in parts[i]:
                if not p[8] or not p[9]:
                    continue
                pr = row + (((p[2] >> 6) & 0x1FF) - 480)
                if pr == row or not 0 <= pr < 32:
                    continue
                pl = pals.setdefault(pr, pal_of(pr))
                u0, v0 = min(p[0], p[3], p[6], p[10]), min(p[1], p[4], p[7], p[11])
                for y in range(max(v0, 0), min(v0 + p[9], h)):
                    rr = rows[y]
                    for x in range(max(u0, 0), min(u0 + p[8], w)):
                        px[y * w + x] = pl[rr[x]]
            im.putdata(px)
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
        fr = spr.frame(f)
        if fr is None:
            continue
        im, (ox, oy) = fr
        x0, y0 = min(x0, ox), min(y0, oy)
        x1, y1 = max(x1, ox + im.width), max(y1, oy + im.height)
    if x0 > x1:
        return 0, 0, 16, 16
    return x0, y0, x1, y1


def compose(spr, f, box, zoom=1, bg=(0, 0, 0, 0)):
    """One animation frame on a canvas `box` (x0, y0, x1, y1) in game coordinates; the origin (object position) is marked by none."""
    x0, y0, x1, y1 = box
    c = spr.Image.new("RGBA", (x1 - x0, y1 - y0), bg)
    fr = spr.frame(f)
    if fr is not None:
        im, (ox, oy) = fr
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


def fname(an, owner="tombi"):
    """File stem of one animation: Tombi `NN_name`, other owners `NN` (+ `_name` when it has a label)."""
    import labels
    if owner == "tombi":
        return "%02d_%s" % (an["id"], labels.slug(an["name"]) or "anim_%02d" % an["id"])
    nm = labels.name("anim:%s:%d" % (owner, an["id"]))
    return "%02d" % an["id"] + ("_" + labels.slug(nm) if nm else "")


# ------------------------------------------------------------------ all owners: Tombi + enemies / NPCs / objects
def tombi_owner(reader=None):
    cat = catalog(reader)
    return dict(owner="tombi", name="Tombi", handler=None, area=None, spr=dict(wpp=TOMBI_WPP), animations=cat)


_objs = {}


def object_owners(reader=None, area=None):
    """Owners of the enemy / NPC / object animation scripts (tools/animobj.py): names from labels ("animowner:<id>") else the handler name."""
    import animobj, labels
    k = (id(reader), area)
    if k not in _objs:
        ows = animobj.resolve_sources(animobj.build_owners(area, reader))
        for ow in ows:
            ow["name"] = labels.name("animowner:" + ow["owner"]) or ow["handler"]
            n = None
            if ow["spr"]:
                n = animobj.table_frames("AR%02d" % ow["area"], ow["spr"]["list"], int(ow["spr"]["kind"], 16))
            for a in ow["animations"]:
                a["name"] = labels.name("anim:%s:%d" % (ow["owner"], a["id"])) or a["name"]
                a["renderable"] = n is not None and max(a["frames"]) < n
        _objs[k] = ows
    return _objs[k]


def catalog_all(reader=None, owner="all"):
    """[owner dict]: Tombi first, then the object owners.  `owner` = all | tombi | owner id | handler name (case-insensitive)."""
    out = []
    o = (owner or "all").lower()
    if o in ("all", "tombi"):
        out.append(tombi_owner(reader))
    if o != "tombi":
        for ow in object_owners(reader):
            if o == "all" or o in (ow["owner"].lower(), ow["handler"].lower(), ow["name"].lower()):
                out.append(ow)
    return out


def owner_source(ow):
    """Frame source of an owner: Sprites (Tombi) or animobj.SprFrames; None when nothing draws it."""
    if ow["owner"] == "tombi":
        return Sprites()
    if not ow.get("spr"):
        return None
    import animobj
    return animobj.spr_frames("AR%02d" % ow["area"], ow["spr"]["list"], int(ow["spr"]["kind"], 16), ow["spr"].get("w1e", ()), ow["spr"].get("clut"))


def anim_json(ow, an):
    return dict(id=an["id"], name=an["name"], script=an["start"], alias_of=an.get("alias_of"), table=an.get("table"), index=an.get("index"),
                frames=an["frames"], durations=[max(e["dur"], 1) for e in an["entries"]], loop=an["loop"], end=an["end"], signal=an["signal"],
                ticks=an["ticks"], renderable=an.get("renderable", True))


def export(out, zoom=2, tick=TICK_MS, reader=None, log=print, owner="all"):
    os.makedirs(out, exist_ok=True)
    n = 0
    meta = []
    tspr = None
    for ow in catalog_all(reader, owner):
        spr = owner_source(ow)
        d = os.path.join(out, ow["owner"])
        done = 0
        for an in ow["animations"]:
            if an.get("alias_of") is not None or not an["entries"] or spr is None or not an.get("renderable", True):
                continue
            os.makedirs(d, exist_ok=True)
            stem = fname(an, ow["owner"])
            save_gif(spr, an, os.path.join(d, stem + ".gif"), zoom, tick)
            strip(spr, an, zoom).save(os.path.join(d, stem + ".png"))
            an["file"] = ow["owner"] + "/" + stem + ".gif"
            done += 1
        n += done
        m = {k: v for k, v in ow.items() if k not in ("animations", "sprs")}
        m["animations"] = [dict(anim_json(ow, a), file=a.get("file")) for a in ow["animations"]]
        meta.append(m)
        if ow["owner"] != "tombi":
            log("%s (%s, area %s): %d/%d animations" % (ow["owner"], ow["handler"], ow["area"], done, len(ow["animations"])))
    with open(os.path.join(out, "catalog.json"), "w") as f:
        json.dump(dict(tick_ms=tick, owners=meta), f, indent=1)
    log("%d animations -> %s" % (n, out))
    return n


def cmd_list(a):
    cats = catalog_all(owner=a.owner)
    if not cats:
        print("no owner %r (try: list --owner all)" % a.owner, file=sys.stderr)
        return 1
    if a.json:
        print(json.dumps([dict({k: v for k, v in ow.items() if k not in ("animations", "sprs")},
                               animations=[anim_json(ow, x) for x in ow["animations"]]) for ow in cats], indent=1))
        return 0
    for ow in cats:
        if ow["owner"] != "tombi":
            sp = ow["spr"]
            print("# %s  handler %s @%s  area %s  types %s  frames from %s" % (
                ow["owner"], ow["handler"], ow["handler_addr"], ow["area"], ",".join(ow["types"]),
                ("list %d SPR %s" % (sp["list"], sp["kind"])) if sp else "(not resolved)"))
        for an in ow["animations"]:
            al = " (= %02d)" % an["alias_of"] if an.get("alias_of") is not None else ""
            lp = "loop->%d" % an["loop"] if an["loop"] is not None else an["end"]
            print("%s %02d %-22s %s %2d frames %4d ticks %-8s%s%s%s" % (
                ow["owner"], an["id"], an["name"], an["start"], len(an["entries"]), an["ticks"], lp, " finished-signal" if an["signal"] else "",
                al, "" if an.get("renderable", True) else " [frames outside the SPR table]"))
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("list")
    p.add_argument("--json", action="store_true")
    p.add_argument("--owner", default="tombi", help="all | tombi | owner id | handler name (default tombi)")
    p = sp.add_parser("export")
    p.add_argument("out")
    p.add_argument("--owner", default="all")
    p.add_argument("--zoom", type=int, default=2)
    p.add_argument("--tick", type=int, default=TICK_MS, help="ms per game tick in the GIF (default %d)" % TICK_MS)
    a = ap.parse_args(argv)
    if a.cmd == "list":
        return cmd_list(a)
    export(a.out, a.zoom, a.tick, owner=a.owner)
    return 0


if __name__ == "__main__":
    sys.exit(main())
