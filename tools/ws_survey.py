#!/usr/bin/env python3
"""Widescreen room survey: every room of the disc at 16:9, looking at the side strips the wide frame adds.

  python3 tools/ws_survey.py rooms  OUT [AREA[:SEC] ...]   spawn view of every debug-select room (frames + camera state)
  python3 tools/ws_survey.py sweep  OUT [AREA[:SEC] ...]   scrolling rooms: fly to the left and the right camera limit
  python3 tools/ws_survey.py sheet  OUT SHEET.png NAME[=LABEL] ...   a labelled contact sheet of OUT/NAME.png

`rooms` enters each room of tools/psxemu/rooms.py ROOM_COUNTS (minus DOOR_ROOMS: empty load lists, no model of their
own) through the debug area select, headless, `--widescreen 16:9`, and keeps frame 2000 as OUT/AA_S.png (frame 1700
as OUT/AA_S_a.png) plus OUT/rooms.json: the camera limits (CAMERA_SCROLL minX / maxX / minY / maxY), the player's
position and, per strip, how much of it is one flat colour (`flat`: the share of strip pixels equal to the strip's
most common colour; `inner`: the same measure over the 4:3 picture's outer 53 columns).  `sweep` then flies the
player (--sweep-x / --sweep-fly, no collision) past both ends of every room whose camera range is wider than a point
and keeps the frame with the camera resting at each limit (OUT/AA_S_L.png, _R.png) and frames on the way
(OUT/AA_S_L_pNN.png), into OUT/sweep.json.  Environment: TOMBI2 (the executable), TOMBI_DATA, JOBS, WS (16:9).
The frames are the user's game picture: never commit them."""
import json, os, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools", "psxemu"))
from rooms import ROOM_COUNTS, DOOR_ROOMS  # noqa: E402

EXE = os.environ.get("TOMBI2", os.path.join(ROOT, "build", "port2", "tombi2"))
DATA = os.environ.get("TOMBI_DATA", os.path.join(ROOT, "data"))
WS = os.environ.get("WS", "16:9")
PAD = {"16:9": 53, "21:9": 120, "32:9": 267}[WS]
CAMERA = 0x800A4550            # CAMERA_SCROLL (include/tomba/camera.h)
PLAYER = 0x800A6038
GAME = 0x8009C960
SELECT = ["--skip-movies", "--poke", "900:1F8001B4:1", "--pad-script", "1000:start,1300:circle:3"]
ENV = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
# rooms the select's default story state leaves at once (the Haunted Mansion pig throws Tomba out of the two thief's
# rooms and the trap room; the leaf slider is a ride into AREA09 s0): looked at this early instead of at frame 2000
EARLY = {(4, 5): 1600, (4, 12): 1600, (4, 17): 1600, (9, 6): 1600}


def room_list(args):
    want = []
    for a in args:
        p = a.split(":")
        want.append((int(p[0]), int(p[1]) if len(p) > 1 else None))
    out = []
    for area, n in ROOM_COUNTS:
        for s in range(n):
            if (area, s) in DOOR_ROOMS:
                continue
            if want and not any(area == wa and ws in (None, s) for wa, ws in want):
                continue
            out.append((area, s))
    return out


def run(area, sec, frames, extra, dump_dir=None, every=100, dump_from=0, ram=None):
    cmd = [EXE, "--data", DATA, "--headless"] + SELECT + ["--poke", "1250:%08X:%X:2" % (GAME, area),
                                                         "--poke", "1250:%08X:%X:2" % (GAME + 2, sec),
                                                         "--frames", str(frames), "--widescreen", WS]
    if dump_dir:
        cmd += ["--dump-frames", dump_dir, "--dump-every", str(every), "--dump-from", str(dump_from)]
    if ram:
        cmd += ["--dump-ram", "%d:%s" % ram]
    r = subprocess.run(cmd + extra, env=ENV, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=900)
    return r.returncode, r.stdout.decode("latin-1")


class Ram:
    def __init__(self, path):
        self.b = open(path, "rb").read()

    def s16(self, a):
        return struct.unpack_from("<h", self.b, a - 0x80000000)[0]

    def s32(self, a):
        return struct.unpack_from("<i", self.b, a - 0x80000000)[0]

    def state(self):
        return dict(cam=[self.s32(CAMERA + 8) >> 16, self.s32(CAMERA + 12) >> 16, self.s32(CAMERA + 16) >> 16],
                    lim=[self.s16(CAMERA + 0x2C), self.s16(CAMERA + 0x2E), self.s16(CAMERA + 0x30), self.s16(CAMERA + 0x32)],
                    player=[self.s32(PLAYER + 0x10) >> 16, self.s32(PLAYER + 0x14) >> 16, self.s32(PLAYER + 0x18) >> 16],
                    now=[self.s16(GAME + 0), self.s16(GAME + 2)])       # the room the game is in


def strip_stats(png):
    """Per side: share of the strip that is its most common colour, and the same for the 4:3 picture's outer columns."""
    from PIL import Image
    import numpy as np
    im = np.asarray(Image.open(png).convert("RGB")).astype(np.int32)
    h, w, _ = im.shape
    key = (im[..., 0] << 16) | (im[..., 1] << 8) | im[..., 2]
    body = key[16:h - 16]                     # the game's picture rows

    def flat(a):
        v, c = np.unique(a, return_counts=True)
        i = int(c.argmax())
        return round(float(c[i]) / a.size, 3), "%06x" % int(v[i])
    out = {}
    for name, strip, inner in (("L", body[:, :PAD], body[:, PAD:2 * PAD]), ("R", body[:, w - PAD:], body[:, w - 2 * PAD:w - PAD])):
        f, col = flat(strip)
        fi, _ = flat(inner)
        out[name] = dict(flat=f, colour=col, inner=fi)
    return out


def faults(log):
    return [ln[:160] for ln in log.splitlines() if "violation" in ln or "FATAL" in ln or "watchdog" in ln or "signal" in ln][:2]


def do_room(out, area, sec):
    tag = "%02d_%d" % (area, sec)
    d = os.path.join(out, "tmp_" + tag)
    ram = os.path.join(out, tag + ".ram")
    t0 = EARLY.get((area, sec), 2000)
    rc, log = run(area, sec, t0, [], d, 100, t0 - 100, (t0 - 5, ram))
    info = dict(area=area, sec=sec, rc=rc, t0=t0)
    fr = os.path.join(d, "frame_%06d.png" % t0)
    if os.path.exists(fr):
        os.replace(fr, os.path.join(out, tag + ".png"))
        os.replace(os.path.join(d, "frame_%06d.png" % (t0 - 100)), os.path.join(out, tag + "_a.png"))
        info["strips"] = strip_stats(os.path.join(out, tag + ".png"))
    if os.path.exists(ram):
        info.update(Ram(ram).state())
        os.remove(ram)
    subprocess.run(["rm", "-rf", d])
    info["fault"] = faults(log)
    return info


def sweep_end(out, room, side, margin):
    area, sec = room["area"], room["sec"]
    tag = "%02d_%d" % (area, sec)
    lo, hi = room["lim"][0], room["lim"][1]
    start = room.get("t0", 2000) - 100
    target = lo - margin if side == "L" else hi + margin
    steps = (hi - lo + 400) // 8 + 1             # wherever the room starts
    end = start + steps + 58
    wps = "%d," % target + ",".join(str(target + (i & 1)) for i in range(60))     # then hover there for 60 frames
    d = os.path.join(out, "tmp_%s_%s" % (tag, side))
    ram = os.path.join(out, "%s_%s.ram" % (tag, side))
    rc, log = run(area, sec, end, ["--sweep-x", "%d:8:%s" % (start, wps), "--sweep-fly", "-40"], d, 20, start, (end - 3, ram))
    fs = sorted(os.listdir(d)) if os.path.isdir(d) else []
    r = dict(rc=rc, target=target, fault=faults(log))
    if os.path.exists(ram):
        r.update(Ram(ram).state())
        os.remove(ram)
    r["left_room"] = r.get("now") != [area, sec] or not any(r.get("lim", [0]))
    if fs and not r["left_room"]:
        os.replace(os.path.join(d, fs[-1]), os.path.join(out, "%s_%s.png" % (tag, side)))      # hovering at the limit
        mid = fs[1:-2]
        for i, f in enumerate(mid[::max(1, len(mid) // 5)]):                                    # a few on the way
            os.replace(os.path.join(d, f), os.path.join(out, "%s_%s_p%02d.png" % (tag, side, i)))
        r["strips"] = strip_stats(os.path.join(out, "%s_%s.png" % (tag, side)))
    subprocess.run(["rm", "-rf", d])
    return r


def do_sweep(out, room):
    """Fly to each camera limit and hover: 40 units past it, or (when that fires the room's exit) 4 units short of it."""
    res = dict(area=room["area"], sec=room["sec"])
    for side in "LR":
        for margin in (40, -4, -40):
            r = sweep_end(out, room, side, margin)
            r["margin"] = margin
            if not r["left_room"]:
                break
        res[side] = r
    return res


def sheet(out, dst, names, cols=3):
    from PIL import Image, ImageDraw
    tiles = []
    for n in names:
        name, _, label = n.partition("=")
        p = os.path.join(out, name + ".png")
        if not os.path.exists(p):
            continue
        im = Image.open(p).convert("RGB")
        t = Image.new("RGB", (im.width, im.height + 22), (24, 24, 24))
        t.paste(im, (0, 22))
        dr = ImageDraw.Draw(t)
        dr.text((3, 5), label or name, fill=(255, 255, 255))
        dr.line([(PAD, 16), (PAD, 21)], fill=(255, 255, 0))                      # the 4:3 picture's edges
        dr.line([(im.width - PAD, 16), (im.width - PAD, 21)], fill=(255, 255, 0))
        tiles.append(t)
    if not tiles:
        return
    w, h = tiles[0].size
    rows = (len(tiles) + cols - 1) // cols
    sh = Image.new("RGB", (cols * (w + 4), rows * (h + 4)), (0, 0, 0))
    for i, t in enumerate(tiles):
        sh.paste(t, ((i % cols) * (w + 4), (i // cols) * (h + 4)))
    sh.save(dst)


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    mode, out = sys.argv[1], sys.argv[2]
    os.makedirs(out, exist_ok=True)
    jobs = int(os.environ.get("JOBS", "4"))
    if mode == "sheet":
        return sheet(out, sys.argv[3], sys.argv[4:], int(os.environ.get("COLS", "3")))
    rooms = room_list(sys.argv[3:])
    jpath = os.path.join(out, mode + ".json")
    old = json.load(open(jpath)) if os.path.exists(jpath) else {}
    if mode == "rooms":
        fn = lambda r: do_room(out, *r)                                   # noqa: E731
    elif mode == "sweep":
        base = json.load(open(os.path.join(out, "rooms.json")))
        rooms = [base["%d:%d" % r] for r in rooms if "lim" in base.get("%d:%d" % r, {})]
        rooms = [r for r in rooms if r["lim"][1] - r["lim"][0] > 16]
        fn = lambda r: do_sweep(out, r)                                   # noqa: E731
    else:
        sys.exit(__doc__)
    with ThreadPoolExecutor(jobs) as ex:
        for r in ex.map(fn, rooms):
            old["%d:%d" % (r["area"], r["sec"])] = r
            print(json.dumps(r), flush=True)
    json.dump(old, open(jpath, "w"), indent=1, sort_keys=True)


if __name__ == "__main__":
    main()
