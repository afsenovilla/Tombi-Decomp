#!/usr/bin/env python3
"""Headless check of the side-view room exits at several widths (docs/PORT.md "Widescreen exits").

For every edge exit of tools/colview_exits.json (one whose H range is open on one side: a left or right exit), the
port is started on the debug area select, Tomba is put 70 units inside the exit (PLAYER x, 0x800A604A, poked a few
times), then the pad walks him towards it.  The PORT_CVLOG lines of the collision view (port2/runtime/colview.c) give
his last position in the room and the room he ends up in.  Run it at 4:3 and at the wide sizes and compare: a moved
exit must lead to the same room, on the floor (Y close to the 4:3 one), with H moved out by the pad.

  python3 tools/ws_exit_sweep.py                       # the exits marked "ws" (moved), at 4:3 16:9 21:9 32:9
  python3 tools/ws_exit_sweep.py --all --ws 4:3        # every edge exit, 4:3 only
  python3 tools/ws_exit_sweep.py --rooms 2:5,5:1       # only these area:section
  python3 tools/ws_exit_sweep.py --json out.json       # also write the rows

Limits (seen on 2026-11-16): after the debug select some rooms hold Tomba in a dialogue or an event (AREA03 s0,
AREA06 s2's mermaid, ...), reset his position or drop him into a pit; those rows read "stay".  Sections whose path
is the other axis (AREA00 s4, AREA01 s2/s3, AREA04 s1: their exits are depth exits, not screen edges) are not
meaningful here.  An exit with an extra condition (a flag) needs that flag poked (--poke-every FRAME:ADDR:VAL:SIZE,
passed through with --extra).
"""
import argparse, concurrent.futures as cf, json, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def jobs(all_exits, rooms):
    out = {}
    for e in json.load(open(os.path.join(ROOT, "tools", "colview_exits.json"))):
        h = e["h"]
        if not h or (h[0] > -32768 and h[1] < 32767):
            continue                                   # not an edge exit (a door box, a depth exit)
        if not all_exits and "ws" not in e:
            continue
        side = ("R" if e["ws"][0] else "L") if "ws" in e else ("R" if h[1] >= 32767 else "L")
        thr = h[0] if side == "R" else h[1]
        for s in e["sections"]:
            if rooms and (e["area"], s) not in rooms:
                continue
            out.setdefault((e["area"], s, side), (thr, e["exit"]))
    return sorted(out.items())


def run(exe, data, ws, job, t0, extra):
    (a, s, side), (thr, ex) = job
    start = thr - 70 if side == "R" else thr + 70
    args = [exe, "--data", data, "--headless", "--show-collision", "--skip-movies",
            "--poke", "900:1F8001B4:1", "--poke", "1250:8009C960:%X:2" % a, "--poke", "1250:8009C962:%X:2" % s]
    for f in range(t0, t0 + 60, 6):
        args += ["--poke", "%d:800A604A:%X:2" % (f, start & 0xFFFF)]
    args += ["--pad-script", "1000:start,1300:circle:3,%d:%s:500" % (t0 + 70, "right" if side == "R" else "left"),
             "--frames", str(t0 + 650)] + extra
    if ws != "4:3":
        args[4:4] = ["--widescreen", ws]
    env = dict(os.environ, PORT_CVLOG="1", SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    err = subprocess.run(args, env=env, capture_output=True, text=True, timeout=600).stderr
    rows = []
    for l in err.splitlines():
        if l.startswith("CVLOG"):
            p = l.split()
            rows.append((int(p[1]), int(p[2]), int(p[3][2:]), int(p[4][2:])))
    idx = [i for i, r in enumerate(rows) if (r[0], r[1]) == (a, s)]
    if not idx:
        return dict(area=a, section=s, side=side, thr=thr, exit=ex, ws=ws, result="noroom")
    last = rows[idx[-1]]
    after = rows[idx[-1] + 1:]
    ys = [rows[i][3] for i in idx[-120:]]
    return dict(area=a, section=s, side=side, thr=thr, exit=ex, ws=ws,
                result=("exit to %d:%d" % after[0][:2]) if after else "stay", h=last[2], ymin=min(ys), ymax=max(ys))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe", default=os.path.join(ROOT, "build", "port2", "tombi2"))
    ap.add_argument("--data", default=os.path.join(ROOT, "data"))
    ap.add_argument("--ws", default="4:3,16:9,21:9,32:9")
    ap.add_argument("--all", action="store_true", help="every edge exit, not only the moved ones")
    ap.add_argument("--rooms", default="", help="area:section,... (decimal)")
    ap.add_argument("--t0", type=int, default=1500, help="frame of the first position poke")
    ap.add_argument("--extra", default="", help="more port arguments, space separated")
    ap.add_argument("-j", type=int, default=8)
    ap.add_argument("--json")
    a = ap.parse_args()
    rooms = {tuple(int(v) for v in r.split(":")) for r in a.rooms.split(",") if r}
    js = jobs(a.all, rooms)
    widths = a.ws.split(",")
    with cf.ThreadPoolExecutor(a.j) as p:
        res = list(p.map(lambda x: run(a.exe, a.data, x[1], x[0], a.t0, a.extra.split()),
                         [(j, w) for j in js for w in widths]))
    by = {}
    for r in res:
        by.setdefault((r["area"], r["section"], r["side"]), {})[r["ws"]] = r
    for k in sorted(by):
        cells = []
        for w in widths:
            r = by[k][w]
            cells.append("%s %s" % (w, r["result"]) + ("" if r["result"] == "noroom" else
                         " H=%d Y=%d..%d" % (r["h"], r["ymin"], r["ymax"])))
        print("AREA%02d s%d %s | %s" % (k[0], k[1], "right" if k[2] == "R" else "left", " | ".join(cells)))
    if a.json:
        json.dump(res, open(a.json, "w"), indent=1)
    return 0


if __name__ == "__main__":
    sys.exit(main())
