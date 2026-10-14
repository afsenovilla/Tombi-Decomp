"""Composed screens captured by running the game: boots MAIN0.EXE in the interpreter from its entry point (the game's own loader reads
the disc image through the HLE CD library), lets the task system run, and rasterizes the ordering tables the game builds.

  python3 -m psxemu.capture title OUT.png        (from tools/)
  tools/psxemu/capture.py title OUT.png

Scenes:
  title    - boot -> SCE/Whoopee Camp logos -> title (the FMVs are skipped by the HLE movie task), captured once "PRESS START" blinks on.
  area00   - START on the title: first room of AREA00 (sky, village, Tombi, dialogue box).
  worldmap - + the dialogue clicked through (CROSS), SELECT menu -> Map.
  rooms    - every area / room (section) through the game's own debug area select, composed and sky only (see `rooms`).

  tools/psxemu/capture.py rooms OUT_DIR [--area 03] [--section 2] [--jobs 4] [--force]
"""
import os
import sys

if __package__ in (None, ""):
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from psxemu import machine as _machine
    from psxemu.rooms import ROOM_COUNTS, DOOR_ROOMS
else:
    from . import machine as _machine
    from .rooms import ROOM_COUNTS, DOOR_ROOMS

TITLE_TASK = "titleSequenceTask"


def boot(verbose=False, exe=None, disc=None):
    m = _machine.Machine(exe=exe, disc=disc)
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


# pad scripts: (buttons, frames held) - buttons as in Machine.set_pad, 0 = released
CROSS, SELECT, START, DOWN = 0x4000, 0x0001, 0x0008, 0x0040
# the old man's dialogue + "The 100 Year Old Wise Man" event banner (the emulation is deterministic: this exact timing was found by trial)
TALK = ([(CROSS, 3), (0, 30)] * 4 + [(0, 100), (SELECT, 3), (0, 120)] + [(CROSS, 3), (0, 40)] * 18 + [(0, 60)]
        + [(CROSS, 3), (0, 30)] * 4 + [(0, 60)])
TO_MAP = [(SELECT, 3), (0, 150)] + [(DOWN, 3), (0, 15)] * 3 + [(CROSS, 3), (0, 250)]   # menu: Item/Event/Status/Map


def play(m, script, max_instr=150_000_000):
    for buttons, n in script:
        m.set_pad(buttons)
        m.run_frames(n, max_instr=max_instr)
    m.set_pad(0)


def area00(m=None, log=print):
    """New game: the first room of AREA00 (Village of All Beginnings) with its sky/far background, Tombi and the first dialogue."""
    _, m = title(m, log)
    return area00_from(m, log)


def area00_from(m, log=print):
    """area00 starting from the machine state title() leaves."""
    play(m, [(START, 4), (0, 500)])   # rasterize all frames: the dialogue font is drawn into VRAM and used as a texture
    log("area00: frame %d, %d primitives" % (m.frame_count, len(m.frames[-1][1])))
    return frame_image(m), m


def worldmap(m=None, log=print):
    """Pause menu -> Map from the first room (after the opening dialogue)."""
    _, m = area00(m, log)
    return worldmap_from(m, log)


def worldmap_from(m, log=print):
    play(m, TALK + TO_MAP)
    log("worldmap: frame %d, %d primitives" % (m.frame_count, len(m.frames[-1][1])))
    return frame_image(m), m


# ------------------------------------------------------------------------------------------------------------- rooms
# The game has a debug area select (displayDebugScreen / debugSelectHandler, task mode 0 of gameplayStateDispatcher): with
# scratchpad.debug_mode_enabled (0x1F8001B4) set, NEW GAME shows "AREA SELECT / SECTION SELECT"; CIRCLE/START loads that area and
# section through the normal loader.  We set the flag before START, save the machine at that menu (rooms_base) and, per room, poke
# GAME.selectedArea / selectedSection and press CIRCLE.
DEBUG_FLAG = 0x1F8001B4
GAME_AREA, GAME_SECTION, GAME_SPAWN = 0x8009C960, 0x8009C962, 0x8009C982     # gameConfig.selectedArea / selectedSection / selectedSpawnPoint
SECTION_COUNT = 0x80078790       # u16 per area: sections of the area (debug select clamp; 0x8007878C = highest area)
GAME_TASK = 0x801FD800           # the game task (state 1 = gameplay): +0x4C = gameplay mode (0 debug select, 1/2/4/5/6 area modes, 7 area transition), +0x4E step
GAME_NEXT = 0x434                # gameConfig.nextArea / +2 nextSection / +0x10 nextSpawnPoint: what a door sets before the game enters mode 7 (not used)
LOADED_FLAG = 0x1F8001CF         # set once the room is loaded and running
CIRCLE_BTN = 0x2000
SETTLE = 60                      # frames after the room starts running (camera settled, Tombi on the ground)
RUN_MODES = (1, 2, 4, 5, 6)      # gameplay modes of the area (areaMode*Handler): step 1 = running
LOAD_TIMEOUT = 900
SKY_DEPTH = 100                  # ordering table slots 0..99 (far end): the sky / far background layers drawn by the area overlay
HUD_SLOTS = 3                    # the last ordering table slots: HUD, dialogue box and text


def base_state(path=None, log=print, exe=None, disc=None):
    """Machine paused at the debug area select; the state is cached in `path`."""
    m = boot(exe=exe, disc=disc)
    if path and os.path.exists(path):
        m.load_state(path)
        return m
    m.render = False
    title(m, log)
    m.cpu.w8(DEBUG_FLAG, 1)
    play(m, [(START, 4), (0, 200)])
    m.render = False
    if path:
        m.save_state(path)
    return m


def depth_of(m, idx=-1):
    """Per primitive of a recorded frame: ordering table slot, 0 = farthest (drawn first)."""
    _, pk, _, marks, head = m.frames[idx]
    return [(head - mk) >> 2 for mk in marks]


def render_frame(m, keep=None, idx=-1):
    """Re-rasterizes a recorded frame with only the primitives for which keep(depth, words) is true; VRAM is restored."""
    from PIL import Image
    env, pk, _, marks, head = m.frames[idx]
    g = m.gpu
    x, y, w, h = env["clip"]
    h = min(h, 512 - y)
    saved = g.vram[y:y + h, x:x + w].copy()
    st = (g.tpage, g.tw, g.clip, g.ofs, g.set_mask)
    m.apply_drawenv(env)
    g.run_packets([p[1] for p, mk in zip(pk, marks) if keep is None or keep((head - mk) >> 2, p[1])])
    img = Image.fromarray(g.rgb(x, y, w, h), "RGB")
    g.vram[y:y + h, x:x + w] = saved
    g.tpage, g.tw, g.clip, g.ofs, g.set_mask = st
    return img


def room_sections(m=None):
    """[(area, sections)] of the areas on the disc.  Read from the game (u16 table SECTION_COUNT) when a base machine is given, else the
    static ROOM_COUNTS (same values)."""
    if m is None:
        return list(ROOM_COUNTS)
    return [(a, m.cpu.r16(SECTION_COUNT + 2 * a)) for a, _ in ROOM_COUNTS]


def _running(m):
    c = m.cpu
    return c.r16(GAME_TASK + 0x4C) in RUN_MODES and c.r16(GAME_TASK + 0x4E) == 1 and c.r8(LOADED_FLAG)


def _wait_running(m, what):
    c = m.cpu
    run = None
    for i in range(LOAD_TIMEOUT):
        m.run_frames(1)
        if run is None and _running(m):
            run = i
        if run is not None and i - run >= SETTLE:
            return
    raise RuntimeError("%s did not start in %d frames" % (what, LOAD_TIMEOUT))


def capture_room(m, base, area, section, log=print):
    """Loads one room from the debug select and returns (composed, sky, info).  composed = everything but the HUD/dialogue slots;
    sky = only the far background slots (None when the room has none).  Raises RuntimeError when the room never starts (or EmuError
    when the game runs into code the emulator does not support: rooms with an EMPTY load list inherit the area overlay and the
    resources of the previous room, so entering them straight from the debug select crashes)."""
    m.load_state(base)
    m.render = False
    c = m.cpu
    c.w16(GAME_AREA, area)
    c.w16(GAME_SECTION, section)
    play(m, [(0, 2), (CIRCLE_BTN, 3), (0, 1)])
    _wait_running(m, "AREA%02d section %d" % (area, section))
    return _grab(m, area, section)


def _grab(m, area, section):
    c = m.cpu
    pk = m.frames[-1][1]
    dep = depth_of(m)
    n_ot = m.ot_info[1]
    composed = render_frame(m, lambda d, w: d < n_ot - HUD_SLOTS)
    sky = None
    if any(d < SKY_DEPTH for d in dep):
        sky = render_frame(m, lambda d, w: d < SKY_DEPTH)
    info = dict(frames=m.frame_count, prims=len(pk), sky_prims=sum(1 for d in dep if d < SKY_DEPTH),
                min3d=min([d for d in dep if d >= SKY_DEPTH] or [0]), area=c.r16(GAME_AREA), section=c.r16(GAME_SECTION))
    return composed, sky, info


# Rooms with an EMPTY load list (rooms.DOOR_ROOMS) are variants of a sibling room of the same area: they load nothing, the area keeps its overlay,
# model and textures, and the game enters them through the normal door path (mode 7).  We load a sibling through the debug select, then do what
# a door does (FUN_8002a4d0 state 1/2: nextArea / nextSection / nextSpawnPoint, fadeScreenControl 3 -> wait 1 -> task mode 7 step 0).
# Two things differ from a real load: (1) the empty list never queues a file, so LOAD_COMPLETE (0x1F8001CE) is never set again after the load-list file -> we set it when the
# transition waits (mode 7 step 4) for the lists after that file; (2) the object spawner (FUN_8004c570) reads the placement list APD_DATA[section & 7] (0x1F800358): a slot the
# sibling did not load is 0 / stale and would be read as an endless garbage list (garbage objects, jump to a bad pointer) -> an empty list is put there.
GAME_BASE = 0x8009C960
APD_DATA = 0x1F800358
SECTION_OBJECTS = 0x8007BEE4     # PTR_8007bee4[area][section] -> list of unlayered section objects (FUN_8004bfe4, mode 5)
LOAD_COMPLETE = 0x1F8001CE
SIBLINGS = {(11, 0): 1, (11, 3): 1, (11, 4): 2, (16, 0): 3, (16, 1): 3, (16, 2): 3, (17, 0): 1, (17, 6): 1, (17, 7): 2, (17, 8): 3,
            (17, 9): 1, (17, 10): 2, (18, 0): 1}


def enter_via_door(m, base, area, section, sibling, spawn=0, no_wide=False):
    m.load_state(base)
    m.render = False
    c = m.cpu
    c.w16(GAME_AREA, area)
    c.w16(GAME_SECTION, sibling)
    play(m, [(0, 2), (CIRCLE_BTN, 3), (0, 1)])
    _wait_running(m, "AREA%02d section %d (sibling)" % (area, sibling))
    p = c.r32(APD_DATA + 4 * (sibling & 7))           # the sibling's own list: its terminator record (type 0xFF) is an empty list
    while c.r8(p + 2) < 0xFF:
        p += 16
    if (section & 7) != (sibling & 7):       # a slot the sibling did not load holds a stale pointer (previous room / area) or 0
        c.w32(APD_DATA + 4 * (section & 7), p)
    if no_wide:                              # also no area-wide / section objects (their handlers may depend on the room the sibling set up)
        c.w32(APD_DATA + 4 * min(c.r16(SECTION_COUNT + 2 * area), 15), p)
        c.w32(c.r32(SECTION_OBJECTS + 4 * area) + 4 * section, p + 2)      # FUN_8004bfe4's list of the section (4 byte records, 0xFF = end)
    c.w16(GAME_BASE + 0x434, area)
    c.w16(GAME_BASE + 0x436, section)
    c.w16(GAME_BASE + 0x440, spawn)
    c.w8(GAME_BASE + 0x15, 3)                        # fadeScreenControl: fade out
    for _ in range(300):
        m.run_frames(1, max_instr=2_000_000)
        if c.r8(GAME_BASE + 0x15) == 1:
            break
    c.w16(GAME_TASK + 0x4C, 7)
    c.w16(GAME_TASK + 0x4E, 0)
    run = None
    seen = False                             # the area's load-list file (LD0ARnn) is read first: LOAD_COMPLETE goes 1, then the empty lists queue nothing
    for i in range(LOAD_TIMEOUT):
        m.run_frames(1, max_instr=2_000_000)
        if c.r16(GAME_TASK + 0x4C) == 7 and c.r16(GAME_TASK + 0x4E) == 4:
            if c.r8(LOAD_COMPLETE):
                seen = True
            elif seen:
                c.w8(LOAD_COMPLETE, 1)
        if run is None and _running(m) and c.r16(GAME_SECTION) == section:
            run = i
        if run is not None and i - run >= SETTLE:
            return
    raise RuntimeError("AREA%02d section %d did not start via door from %d" % (area, section, sibling))


def capture_room_door(m, base, area, section, sibling=None, log=print):
    """capture_room for a room with an empty load list (see above)."""
    sibling = sibling or SIBLINGS[(area, section)]
    try:
        enter_via_door(m, base, area, section, sibling)
    except Exception as e:      # noqa: BLE001  a stale object of the sibling crashes the emulator: retry without the area-wide and section objects
        log("AREA%02d:%d retry without area-wide objects (%s)" % (area, section, e))
        enter_via_door(m, base, area, section, sibling, no_wide=True)
    return _grab(m, area, section)


def room_name(area, section, kind):
    return "room_%02d_%d_%s.png" % (area, section, kind)


def _one(m, base, out, area, sec, nsec, force=False, log=print):
    """One room -> files."""
    pc, ps = (os.path.join(out, room_name(area, sec, k)) for k in ("composed", "sky"))
    if not force and os.path.exists(pc):
        return (area, sec, "skip", None)
    try:
        comp, sky, info = (capture_room_door if (area, sec) in DOOR_ROOMS else capture_room)(m, base, area, sec)
    except Exception as e:      # noqa: BLE001  one broken room must not stop the run
        log("AREA%02d:%d FAIL %s" % (area, sec, e))
        return (area, sec, "fail", str(e))
    comp.save(pc)
    if sky is not None:
        sky.save(ps)
    elif os.path.exists(ps):
        os.remove(ps)
    log("AREA%02d:%d ok %s" % (area, sec, info))
    return (area, sec, "ok", info)


_W = {}


def _worker(args):
    base, it, out, force, exe, disc = args
    if "m" not in _W:
        _W["m"] = boot(exe=exe, disc=disc)
    return _one(_W["m"], base, out, it[0], it[1], it[2], force, log=lambda *a: None)


def rooms(out, area=None, section=None, jobs=1, force=False, log=print, base_path=None, progress=None, exe=None, disc=None):
    """room_<AA>_<n>_composed.png / _sky.png for every room (or one area / section) into `out`.  Returns [(area, section, status, info)].
    progress(done, total) is called after every room.  jobs > 1 = worker processes (one machine each).  exe / disc: MAIN0.EXE and the disc
    image when they are not game/MAIN0.EXE and game/disc/Tombi.bin."""
    os.makedirs(out, exist_ok=True)
    base_path = base_path or os.path.join(out, "rooms_base.state")
    m = base_state(base_path, log, exe, disc)
    todo = [(a, s, n) for a, n in room_sections(m) for s in range(n) if (area is None or a == area) and (section is None or s == section)]
    results = []
    if jobs <= 1:
        for it in todo:
            results.append(_one(m, base_path, out, it[0], it[1], it[2], force, log))
            if progress:
                progress(len(results), len(todo))
    else:
        import multiprocessing as mp
        with mp.get_context("spawn").Pool(jobs) as pool:      # spawn: also safe from the editor's threads and in the frozen exe
            for r in pool.imap_unordered(_worker, [(base_path, it, out, force, exe, disc) for it in todo]):
                results.append(r)
                log("AREA%02d:%d %s" % r[:3] + ("" if r[2] == "skip" else "  %s" % (r[3],)))
                if progress:
                    progress(len(results), len(todo))
    return results


SCENES = {"title": title, "area00": area00, "worldmap": worldmap}


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    if argv and argv[0] == "rooms":
        import argparse
        ap = argparse.ArgumentParser(prog="capture.py rooms")
        ap.add_argument("out")
        ap.add_argument("--area", type=int)
        ap.add_argument("--section", type=int)
        ap.add_argument("--jobs", type=int, default=1)
        ap.add_argument("--force", action="store_true")
        a = ap.parse_args(argv[1:])
        r = rooms(a.out, a.area, a.section, a.jobs, a.force)
        bad = [x for x in r if x[2] == "fail"]
        print("%d rooms, %d failed" % (len(r), len(bad)))
        return 1 if bad else 0
    if len(argv) < 2 or argv[0] not in SCENES:
        print(__doc__)
        return 2
    img, m = SCENES[argv[0]]()
    img.save(argv[1])
    print("wrote", argv[1], img.size)
    return 0


if __name__ == "__main__":
    sys.exit(main())
