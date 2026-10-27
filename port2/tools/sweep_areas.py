#!/usr/bin/env python3
"""Dynamic sweep of every room the debug select can open (docs/PORT.md "M3": audit / sweep).

For each (area, section) of tools/psxemu/rooms.py ROOM_COUNTS minus DOOR_ROOMS (rooms with an empty load list,
which crash in the real game too), run the port headless from the title into that room with the debug area
select (`--skip-movies --poke 900:1F8001B4:1`, `GAME.selectedArea/Section` poked at 1250, Circle at 1300) and a generic pad
script (walk right and left, jump, attack, run+jump, up/down), then grep the log for what a retail code pointer
would leave behind: an access violation, a stub call, a call into the overlay region without native code,
a dispatch to port_missing, FATAL, a watchdog line, a non-zero exit.

Usage: python3 port2/tools/sweep_areas.py [--exe build/port2/tombi2] [--frames N] [--jobs J] [--data DIR | --disc IMG]
                                        [--out DIR] [--pad SCRIPT] [AREA[:SECTION] ...]
  --out DIR keeps every room's log (sweep_AA_SS.log); the table goes to stdout (and DIR/sweep.txt).
  AREA or AREA:SECTION restricts the sweep.  Exit status 1 when any room has a finding."""
import os, re, subprocess, sys, time
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools", "psxemu"))
from rooms import ROOM_COUNTS, DOOR_ROOMS

LOG_CAP = 4 << 20
PAD = ("1000:start,1300:circle:3,1500:right:300,1800:cross:3x8/40,2150:left:300,2450:square:3x8/40,"
       "2800:right+cross:3x30/60,3100:up:40,3160:down:40,3220:right:200,3450:cross:3x8/40")
PATTERNS = [("fault", r"access violation|signal 1[01]|signal [4-8]|Segmentation fault|SIGSEGV|SIGILL|SIGBUS|FATAL"),
            ("nocode", r"no native code|without native code: [1-9]|calls into the overlay region without native code: [1-9]"),
            ("missing", r"dispatch .*port_missing"),
            ("stub", r"\[port2\] stub: (?!FUN_800635e8\b)"),       # SetFogNear is the known Psy-Q stub (open point 3)
            ("watchdog", r"watchdog:"),
            ("unknown-ovl", r"unknown overlay"),
            ("bad-cd", r"CdRead failed|load record with that file id is garbage")]


def run_room(exe, area, section, frames, data, disc, outdir, pad):
    # --skip-movies: with the opening movie playing, the title and the debug select come later and the
    # scripted Circle at 1300 misses the menu (the pokes of GAME.selectedArea/Section land at 1250 either way)
    cmd = [exe, "--headless", "--skip-movies", "--frames", str(frames), "--poke", "900:1F8001B4:1",
           "--poke", "1250:8009C960:%X:2" % area, "--poke", "1250:8009C962:%X:2" % section, "--pad-script", pad]
    if data: cmd += ["--data", data]
    if disc: cmd += ["--disc", disc]
    t0 = time.time()
    # the log goes to a file and is cut at LOG_CAP: a fault report that loops (AREA00 section 1 of build 0765922
    # wrote 1 GB of stack lines) must not fill the disk
    path = os.path.join(outdir or ROOT, "sweep_%02d_%02d.log" % (area, section))
    # the output is read through a pipe and written up to LOG_CAP; past the cap the process is killed, since
    # nothing it still prints is read (the cap must act while it runs, not after: the disk filled otherwise)
    size, chunks, code = 0, [" ".join(cmd) + "\n"], None
    pr = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=ROOT)
    while True:
        try: b = pr.stdout.read1(65536)
        except Exception: b = b""
        if not b: break
        size += len(b)
        if size <= LOG_CAP: chunks.append(b.decode(errors="replace"))
        elif pr.poll() is None: pr.kill(); code = "runaway"; break
        if time.time() - t0 > 900: pr.kill(); code = "timeout"; break
    pr.wait()
    if code is None: code = pr.returncode
    log = "".join(chunks)
    if code == "runaway": log += "\n[sweep] log cut at %d bytes, process killed\n" % LOG_CAP
    if outdir: open(os.path.join(outdir, "sweep_%02d_%02d.log" % (area, section)), "w").write(log)
    findings = []
    if code == "runaway": findings.append("runaway log")
    for name, pat in PATTERNS:
        hits = [l for l in log.split("\n") if re.search(pat, l)]
        if hits: findings.append("%s x%d: %s" % (name, len(hits), hits[0].strip()[:150]))
    m = re.findall(r"frame (\d+):.*overlay (\S+)", log)
    last = m[-1] if m else ("?", "?")
    reached = re.findall(r"reached (\d+) frames", log)
    if code not in (0, None) and not any(f.startswith("fault") for f in findings):
        findings.append("exit %s" % code)
    if not reached and not findings: findings.append("stopped before %d frames" % frames)
    return (area, section, last[1], last[0], time.time() - t0, findings)


def main():
    a = sys.argv[1:]
    exe = os.path.join(ROOT, "build", "port2", "tombi2"); frames = 3600; jobs = 4; data = disc = outdir = None; pad = PAD
    rooms = []
    while a:
        x = a.pop(0)
        if x == "--exe": exe = a.pop(0)
        elif x == "--frames": frames = int(a.pop(0))
        elif x == "--jobs": jobs = int(a.pop(0))
        elif x == "--data": data = a.pop(0)
        elif x == "--disc": disc = a.pop(0)
        elif x == "--out": outdir = a.pop(0)
        elif x == "--pad": pad = a.pop(0)
        else: rooms.append(x)
    if not data and not disc:
        for d in (os.path.join(ROOT, "data"), "data"):
            if os.path.isdir(d): data = d; break
    sel = []
    for area, n in ROOM_COUNTS:
        for s in range(n):
            if (area, s) in DOOR_ROOMS: continue
            if rooms and not any(r == str(area) or r == "%d:%d" % (area, s) for r in rooms): continue
            sel.append((area, s))
    if outdir: os.makedirs(outdir, exist_ok=True)
    with ThreadPoolExecutor(jobs) as ex:
        res = list(ex.map(lambda r: run_room(exe, r[0], r[1], frames, data, disc, outdir, pad), sel))
    lines = ["area section overlay  last-frame  seconds  result"]
    bad = 0
    for area, s, ovl, last, secs, f in res:
        lines.append("%4d %7d %-8s %10s %7.0f  %s" % (area, s, ovl, last, secs, "ok" if not f else "; ".join(f)))
        bad += bool(f)
    lines.append("%d rooms, %d with findings (frames %d, pad %s)" % (len(res), bad, frames, pad))
    txt = "\n".join(lines) + "\n"
    sys.stdout.write(txt)
    if outdir: open(os.path.join(outdir, "sweep.txt"), "w").write(txt)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
