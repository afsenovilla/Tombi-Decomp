#!/usr/bin/env python3
# jumpcheck.py OUT.txt: tools/psxemu reference run for the port's jump physics (see docs/PORT.md, "Jump arc").  New game, the opening dialogue (capture.TALK), then a standing
# jump and a running jump in AREA00 section 0; the player x,y (0x800A6038: +0x40 -> +2 = x, +0x16 = y) is logged
# per frame as "frame x y pad".
import sys, os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools"))
from psxemu import capture, machine
CROSS, RIGHT, START = 0x4000, 0x0020, 0x0008
out = open(sys.argv[1], "w")
m = capture.boot()
m.render = False
capture.title(m)
m.render = False
t0 = m.frame_count
print("title at", t0, file=out, flush=True)
def log(tag=""):
    c = m.cpu
    hp = c.r32(0x800A6038 + 0x40)
    px = c.r16(hp + 2) if 0x80000000 <= hp < 0x80200000 else -1
    py = c.r16(0x800A6038 + 0x16)
    px = px - 0x10000 if px >= 0x8000 else px
    py = py - 0x10000 if py >= 0x8000 else py
    print(m.frame_count - t0, px, py, "%04x" % (m.pad or 0), tag, file=out, flush=True)
def play(script):
    for buttons, n in script:
        m.set_pad(buttons)
        for _ in range(n):
            m.run_frames(1, max_instr=3_000_000)
            log()
    m.set_pad(0)
play([(START, 4), (0, 500)] + capture.TALK)
print("--- standing jump", file=out, flush=True)
play([(0, 30), (CROSS, 3), (0, 90)])
print("--- running jump", file=out, flush=True)
play([(RIGHT, 40), (RIGHT | CROSS, 3), (RIGHT, 90), (0, 30)])
print("--- long jump (cross held)", file=out, flush=True)
play([(RIGHT, 40), (RIGHT | CROSS, 30), (RIGHT, 60), (0, 30)])
print("done", file=out, flush=True)
