"""Composed screens captured by running the game: boots MAIN0.EXE in the interpreter from its entry point (the game's own loader reads
the disc image through the HLE CD library), lets the task system run, and rasterizes the ordering tables the game builds.

  python3 -m psxemu.capture title OUT.png        (from tools/)
  tools/psxemu/capture.py title OUT.png

Scenes:
  title   - boot -> SCE/Whoopee Camp logos -> title (the FMVs are skipped by the HLE movie task), captured once "PRESS START" blinks on.
"""
import os
import sys

if __package__ in (None, ""):
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from psxemu import machine as _machine
else:
    from . import machine as _machine

TITLE_TASK = "titleSequenceTask"


def boot(verbose=False):
    m = _machine.Machine()
    m.verbose = verbose
    return m


def frame_image(m, idx=-1):
    """PIL image of the draw area of a recorded frame (the clip rectangle of its DRAWENV)."""
    env = m.frames[idx][0]
    x, y, w, h = env["clip"]
    return m.png_area(x, y, w, min(h, 512 - y))


def run_until(m, cond, max_instr=400_000_000, step=_machine.VBLANK_EVERY):
    c = m.cpu
    start = c.icount
    while not cond(m):
        c.run(max_instr=step)
        m.vblank()
        if c.icount - start > max_instr:
            raise RuntimeError("condition not reached after %d instructions" % max_instr)


def title(m=None, log=print, settle=150):
    """Image of the title screen (logo + landscape + PRESS START + menu + copyright) as the game composes it."""
    m = m or boot()
    task = m.fn(TITLE_TASK)
    state = {"title": False, "best": None, "n": 0}

    def on_draw(mm, pk):
        if any(f == task for _, f in mm.opened):
            state["title"] = True
        # the title frames: landscape page + logo sprites + text; PRESS START blinks (0x20 frame bit) -> most primitives
        if state["title"] and len(pk) >= 40:
            state["n"] += 1         # title frames so far: the logo drops in during the first ~2 seconds
        if state["n"] >= settle and len(pk) >= 60:
            state["best"] = mm.frame_count

    m.on_draw = on_draw
    run_until(m, lambda mm: state["best"] is not None)
    log("title: frame %d, %d primitives, %d instructions" % (m.frame_count, len(m.frames[-1][1]), m.cpu.icount))
    return frame_image(m), m


SCENES = {"title": title}


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    if len(argv) < 2 or argv[0] not in SCENES:
        print(__doc__)
        return 2
    img, m = SCENES[argv[0]]()
    img.save(argv[1])
    print("wrote", argv[1], img.size)
    return 0


if __name__ == "__main__":
    sys.exit(main())
