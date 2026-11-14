"""PrePre Vol. 9 (PCPX-96093): Tombi examines the "2" graffiti rock (AREA01 section 1, x 4110), run in tools/psxemu.

  python3 tools/psxemu/pre9_rock.py frames OUT_DIR      PNG per game frame (320x224) of the scene, + scene.txt (frame ranges)
  python3 tools/psxemu/pre9_rock.py trace OUT.txt       run-time trace of the rock handler (call tree, stores, differential)
  python3 tools/psxemu/pre9_rock.py video OUT_DIR       OUT_DIR/pre9_graffiti_rock.mp4 (H.264, 2x, 60 fps, English title card and
                                                        captions) + .gif (1x, 30 fps, the examine only); needs ffmpeg and Pillow

Needs the demo extracted by `bash tools/disc_fetch.sh PREPRE9` (game_prepre9/).  What the script does, all deterministic:
  1. boot TOMBA.EXE (build profile "prepre9"), title, START, the opening dialogue in the Dwarf Village (AREA02 s1) clicked through;
  2. move to AREA01 section 1, spawn 1 (Tombi lands at x 4168, next to the rock) through the game's own area loader: GAME (0x800A2248)
     +0x334 / +0x336 / +0x340 = next area / section / spawn, game task (0x1F8001D4) mode 5 step 0 (what the exit triggers do,
     0x8002E078 + 0x8001E284).  This is the only RAM write; the rest is pad input;
  3. walk left to x <= 4112 and let go (Tombi slides to ~4100: in front of the plaque, contact result 4 at x >= ~4093), stand, hold UP, press CIRCLE: Pre9's idle step
     (X01 0x800F4F74) needs contact result 4 (0x800F49FC: standing, UP held = 0x1000 in the pad word 0x800A2FF8, cell attribute 4)
     and pad bit 0x20 (Circle) in SPAD+0x1FC; the rock handler (X01 0x800F1980) sees o->b69 == 1 and PLAYER+0xC0 = 1;
  4. the close-up runs (X01 0x800F1BF0) and waits for the button: CIRCLE again; the plaque flies back.
The game paces itself on the VBlank counter (0x1F8001E8 >= 0x1F8001EA = 1): one game frame per 60 Hz field, so the frames are a 60 fps
sequence.  Sound is not emulated.
"""
import collections
import os
import sys

if __package__ in (None, ""):
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from psxemu import machine as _machine
else:
    from . import machine as _machine

GAME = 0x800A2248            # u16 area, u16 section (Pre9 gameConfig)
GAME_TASK_PTR = 0x1F8001D4   # -> game task: +0x4C mode (1 side view, 5 area transition, ...), +0x4E step
PLAYER = 0x800AB668
ROCK_HANDLER = 0x800F1980    # X01 layer 4 entry 0x13
CLOSEUP_STEP = 0x800F1BF0
CROSS, CIRCLE, UP, LEFT, START = 0x4000, 0x2000, 0x10, 0x80, 0x8


def s32(v):
    return v - (1 << 32) if v & 0x80000000 else v


def player(m):
    c = m.cpu
    return dict(b04=c.r8(PLAYER + 4), step=c.r8(PLAYER + 5), x=s32(c.r32(PLAYER + 0x10)) >> 16, y=s32(c.r32(PLAYER + 0x14)) >> 16,
                area=c.r16(GAME), section=c.r16(GAME + 2))


class Run:
    def __init__(self, out=None):
        self.m = _machine.Machine(build="prepre9")
        self.out = out
        self.marks = []
        if out:
            os.makedirs(out, exist_ok=True)

    def frames(self, n, pad=0):
        m = self.m
        m.set_pad(pad)
        for _ in range(n):
            m.run_frames(1, max_instr=60_000_000)
            if self.out and m.frames:
                env = m.frames[-1][0]
                x, y, w, h = env["clip"]
                m.png_area(x, y, w, min(h, 512 - y)).save(os.path.join(self.out, "f%05d.png" % m.frame_count))

    def mark(self, what):
        self.marks.append((self.m.frame_count, what))

    def task(self):
        c = self.m.cpu
        t = c.r32(GAME_TASK_PTR)
        return c.r16(t + 0x4C), c.r16(t + 0x4E)

    def to_village(self):
        """Boot -> title -> START -> opening dialogue done (Tombi free in the Dwarf Village, AREA02 s1)."""
        out, self.out = self.out, None
        self.frames(400)
        self.frames(300)
        self.frames(4, START)
        self.frames(700)
        for _ in range(80):
            if player(self.m)["b04"] != 5 and self.task() == (2, 1):
                break
            self.frames(3, CIRCLE)
            self.frames(25)
        else:
            raise RuntimeError("opening dialogue did not end: %r" % player(self.m))
        self.out = out

    def to_rock(self):
        """Area transition to AREA01 s1 spawn 1 and walk to the plaque."""
        c = self.m.cpu
        c.w16(GAME + 0x334, 1)
        c.w16(GAME + 0x336, 1)
        c.w16(GAME + 0x340, 1)
        t = c.r32(GAME_TASK_PTR)
        c.w16(t + 0x4C, 5)
        c.w16(t + 0x4E, 0)
        self.mark("area transition to AREA01 s1 spawn 1 (RAM write)")
        for _ in range(600):
            self.frames(1)
            p = player(self.m)
            if p["area"] == 1 and p["section"] == 1 and self.task() == (1, 1) and p["b04"] == 1:
                break
        else:
            raise RuntimeError("AREA01 s1 did not start: %r %r" % (player(self.m), self.task()))
        self.frames(30)
        self.mark("walk left")
        for _ in range(120):
            if player(self.m)["x"] <= 4112:
                break
            self.frames(1, LEFT)
        self.frames(8)

    def examine(self, hold=210, tail=120):
        self.mark("hold UP")
        self.frames(6, UP)
        self.mark("UP + CIRCLE: examine")
        self.frames(2, UP | CIRCLE)
        self.frames(2, UP)
        if self.m.cpu.r8(PLAYER + 0xC0) != 1:
            raise RuntimeError("the examine did not start: %r" % player(self.m))
        self.frames(hold)
        self.mark("CIRCLE: close-up ends")
        self.frames(2, CIRCLE)
        self.frames(tail)
        self.mark("end")


def cmd_frames(out):
    r = Run()
    r.to_village()
    r.out = out
    os.makedirs(out, exist_ok=True)
    r.mark("start")
    r.to_rock()
    r.examine()
    with open(os.path.join(out, "scene.txt"), "w") as f:
        for fr, what in r.marks:
            f.write("%d %s\n" % (fr, what))
    print("frames in", out, r.marks)


# ------------------------------------------------------------------------------------------------------------------- video
TITLE = "PrePre Vol. 9 (1997 demo): examining the graffiti rock"
CARD = ["PlayStation Club PrePre Vol. 9 (PCPX-96093), Tombi demo",
        "AREA01 section 1, east end, next to the Dwarf Village: the \"2\" rock",
        "",
        "Rendered from the demo's own code by tools/psxemu (Python HLE).",
        "Tombi is sent to the rock through the game's own area loader",
        "(one RAM write); everything after that is pad input: Up + Circle.",
        "",
        "No sound: audio is not emulated."]


def _font(size):
    from PIL import ImageFont
    for p in ("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", "DejaVuSans.ttf"):
        try:
            return ImageFont.truetype(p, size)
        except OSError:
            pass
    return ImageFont.load_default()


def cmd_video(out):
    import shutil
    import subprocess
    import tempfile
    from PIL import Image, ImageDraw, ImageStat
    os.makedirs(out, exist_ok=True)
    tmp = tempfile.mkdtemp(prefix="pre9rock")
    raw, comp = os.path.join(tmp, "raw"), os.path.join(tmp, "comp")
    os.makedirs(comp)
    r = Run()
    r.to_village()
    r.out = raw
    os.makedirs(raw)
    r.mark("start")
    r.to_rock()
    r.examine()
    marks = dict((w, f) for f, w in r.marks)
    files = sorted(os.listdir(raw))
    num = [int(f[1:6]) for f in files]
    first = next(n for n, f in zip(num, files) if n > marks["start"] + 60 and ImageStat.Stat(Image.open(os.path.join(raw, f))).mean[0] > 0
                 and sum(ImageStat.Stat(Image.open(os.path.join(raw, f))).mean) > 5)      # the fade-in of AREA01 s1 after NOW LOADING
    caps = [(marks["start"], "AREA01 section 1, east end: the \"2\" plaque on the rock"),
            (marks["hold UP"], "Hold Up in front of the plaque..."),
            (marks["UP + CIRCLE: examine"], "...press Circle: the big \"?\" pops over Tombi"),
            (marks["UP + CIRCLE: examine"] + 14, "The plaque flies toward the camera, twice the size, and waits for a button"),
            (marks["CIRCLE: close-up ends"], "Circle: the plaque flies back (no text, no event, no flag)")]
    f_cap, f_title, f_card = _font(15), _font(20), _font(16)
    k = 0
    card = Image.new("RGB", (640, 480), (12, 12, 20))
    d = ImageDraw.Draw(card)
    tw = d.textlength(TITLE, font=f_title)
    while tw > 610:                              # keep the title on one line
        f_title = _font(f_title.size - 1)
        tw = d.textlength(TITLE, font=f_title)
    d.text((320, 130), TITLE, font=f_title, fill=(255, 235, 140), anchor="mm")
    for i, line in enumerate(CARD):
        d.text((320, 195 + 24 * i), line, font=f_card, fill=(225, 225, 225), anchor="mm")
    for _ in range(240):                         # 4 s
        card.save(os.path.join(comp, "c%05d.png" % k))
        k += 1
    sel = [(n, f) for n, f in zip(num, files) if n >= first]
    for n, f in sel:
        im = Image.open(os.path.join(raw, f)).convert("RGB").resize((640, 448), Image.NEAREST)
        fr = Image.new("RGB", (640, 480), (0, 0, 0))
        fr.paste(im, (0, 0))
        cap = [c for fn, c in caps if fn <= n][-1]
        ImageDraw.Draw(fr).text((320, 464), cap, font=f_cap, fill=(255, 255, 255), anchor="mm")
        fr.save(os.path.join(comp, "c%05d.png" % k))
        k += 1
    mp4 = os.path.join(out, "pre9_graffiti_rock.mp4")
    subprocess.run(["ffmpeg", "-y", "-loglevel", "error", "-framerate", "60", "-i", os.path.join(comp, "c%05d.png"), "-c:v", "libx264",
                    "-pix_fmt", "yuv420p", "-crf", "16", "-preset", "slow", "-movflags", "+faststart", mp4], check=True)
    # GIF: 1x, 30 fps, from "hold UP" to the end, with the captions (the 640x480 composition halved)
    g0 = 240 + sum(1 for n, _ in sel if n < marks["hold UP"] - 20)
    gif = os.path.join(out, "pre9_graffiti_rock.gif")
    subprocess.run(["ffmpeg", "-y", "-loglevel", "error", "-framerate", "60", "-start_number", str(g0), "-i", os.path.join(comp, "c%05d.png"),
                    "-vf", "fps=30,scale=320:240:flags=area,split[a][b];[a]palettegen=max_colors=128[p];[b][p]paletteuse=dither=none",
                    gif], check=True)
    with open(os.path.join(out, "scene.txt"), "w") as f:
        f.write("game frames %d..%d (60 fps), marks:\n" % (first, num[-1]))
        for fr_, w in r.marks:
            f.write("  %d %s\n" % (fr_, w))
    shutil.rmtree(tmp)
    print("wrote", mp4, gif)


# ------------------------------------------------------------------------------------------------------------------- trace
def _traced(state_run, examine):
    """Runs the examine sequence from a copy of the machine at the plaque with call-tree / store tracing.  examine=False is the control
    run: the same input without the CIRCLE presses."""
    r = Run()
    r.m.load_state(state_run["path"])
    m, c = r.m, r.m.cpu
    st = dict(stack=[], prev=None, tree=collections.Counter(), stores={}, gstores={}, hle=collections.Counter(), vb=False,
              states=collections.Counter())
    rock = state_run["rock"]

    def is_call(prev):
        b = c.blocks.get(prev)
        if not b:
            return False
        w = int.from_bytes(c.mem[b[2] - 8:b[2] - 4], "little")
        return (w >> 26) == 3 or ((w >> 26) == 0 and (w & 0x3F) == 9)

    def tr(cc, pc):
        stk = st["stack"]
        while stk and pc == stk[-1][1] and (cc.R[29] & 0xFFFFFFFF) >= stk[-1][2]:
            stk.pop()
        prev, st["prev"] = st["prev"], pc
        if prev is not None and is_call(prev) and (pc == ROCK_HANDLER or stk):
            stk.append((pc, cc.R[31], cc.R[29] & 0xFFFFFFFF))
            st["tree"][tuple(s[0] for s in stk)] += 1
            if pc == ROCK_HANDLER:
                st["states"][(cc.r8(rock + 5), cc.r8(rock + 6))] += 1

    def store(cc, pc, a, v, sz):
        a &= 0xFFFFFFFF
        if st["vb"]:
            return
        sp = cc.R[29] & 0xFFFFFFFF
        if st["stack"] and not (sp - 0x400 <= a < sp + 0x400):
            e = st["stores"].setdefault((pc, a, sz, st["stack"][-1][0]), [0, v, v, m.frame_count])
            e[0] += 1
            e[2] = v
        if (0x80010000 <= a < 0x800B8000 or 0x1F800100 <= a < 0x1F800400) and not 0x800A3000 <= a < 0x800A4B00:
            e = st["gstores"].setdefault((pc, a, sz), [0, set(), m.frame_count])
            e[0] += 1
            if len(e[1]) < 6:
                e[1].add(v & ((1 << (8 * sz)) - 1))

    c.trace = tr
    c.set_store_hook(store)
    vb = m.vblank

    def vblank():               # the VBlank "interrupt" runs between slices: not part of the call tree
        saved = st["stack"], st["prev"]
        st["stack"], st["prev"], st["vb"] = [], None, True
        vb()
        (st["stack"], st["prev"]), st["vb"] = saved, False
    m.vblank = vblank
    for a, f in list(c.hle.items()):
        def g(cc, f=f, a=a):
            if st["stack"]:
                st["hle"][(tuple(s[0] for s in st["stack"]), a)] += 1
            f(cc)
        c.hle[a] = g
    c.blocks.clear()
    for pad, n in ((UP, 6), (UP | CIRCLE, 2), (UP, 2), (0, 120), (CIRCLE, 2), (0, 70)):
        r.frames(n, pad if examine else pad & ~CIRCLE)
    return r, st


def cmd_trace(out):
    import tempfile
    r = Run()
    r.to_village()
    r.to_rock()
    c = r.m.cpu
    rock = None
    for o in range(0x0A0000, 0x1F0000, 4):          # the rock object: type 0x13, x 4110, y -399
        if c.mem[o + 2] == 0x13 and int.from_bytes(c.mem[o + 0x12:o + 0x14], "little") == 4110:
            rock = 0x80000000 + o
            break
    if rock is None:
        raise RuntimeError("rock object not found")
    tmp = tempfile.NamedTemporaryFile(suffix=".state", delete=False)
    tmp.close()
    r.m.save_state(tmp.name)
    base = dict(path=tmp.name, rock=rock)
    ra, A = _traced(base, True)
    rb, B = _traced(base, False)
    os.unlink(tmp.name)
    names = {a: n for n, al in ra.m.names.items() for a in al}

    def nm(a):
        return "%08X%s" % (a, " " + names[a] if a in names else "")

    def region(a):
        if rock <= a < rock + 0xC0:
            return "ROCK+%X" % (a - rock)
        if PLAYER <= a < PLAYER + 0x100:
            return "PLAYER+%X" % (a - PLAYER)
        return "other"
    L = ["PrePre 9 graffiti rock: run-time trace (tools/psxemu/pre9_rock.py trace).  Rock object %08X.  Script: UP x6, UP+CIRCLE x2, UP x2,"
         " idle x120, CIRCLE x2, idle x70 (control run: the same without CIRCLE)." % rock,
         "", "== call tree under X01 %08X (jal/jalr targets, by address; times called) ==" % ROCK_HANDLER]
    for p, n in sorted(A["tree"].items()):
        L.append("  " * len(p) + nm(p[-1]) + "  x%d" % n)
    L += ["", "== library (HLE) calls inside that tree =="]
    L += ["  %s from %s x%d" % (nm(a), " > ".join("%08X" % x for x in p), n) for (p, a), n in sorted(A["hle"].items())] or ["  none"]
    L += ["", "== handler entries by rock (step +5, substep +6) =="]
    L += ["  %r x%d" % (k, n) for k, n in sorted(A["states"].items())]
    L += ["", "== every store made inside that tree (stack frames excluded): pc [addr] region size function count first..last first-frame =="]
    for (pc, a, sz, fn), (n, v0, v1, f0) in sorted(A["stores"].items(), key=lambda x: (x[0][1], x[0][0])):
        L.append("  %08X [%08X] %-10s sz%d fn %08X n%d %X..%X f%d" % (pc, a, region(a), sz, fn, n, v0 & 0xFFFFFFFF, v1 & 0xFFFFFFFF, f0))
    L += ["", "== stores anywhere in RAM 0x80010000..0x800B8000 (ordering tables 0x800A3000..0x800A4B00 left out) and scratchpad "
          "0x1F800100.., by any code, that happen in the examine run and not in the control run: pc [addr] size count values first-frame =="]
    for k in sorted(set(A["gstores"]) - set(B["gstores"]), key=lambda k: (k[1], k[0])):
        n, vals, f0 = A["gstores"][k]
        L.append("  %08X [%08X] %-10s sz%d n%d %s f%d" % (k[0], k[1], region(k[1]), k[2], n, ",".join(sorted("%X" % v for v in vals)), f0))
    L += ["", "== RAM that differs at the end (examine vs control), 0x80010000..0x800B8000 and scratchpad 0x1F800100.. (ordering tables out) =="]
    ma, mb = ra.m.cpu.mem, rb.m.cpu.mem
    for lo, hi in ((0x10000, 0xA3000), (0xA4B00, 0xB8000), (0x200100, 0x200400)):
        i = lo
        while i < hi:
            if ma[i] != mb[i]:
                j = i
                while j < hi and ma[j] != mb[j]:
                    j += 1
                a = 0x80000000 + i if i < 0x200000 else 0x1F600000 + i
                L.append("  %08X %-10s len %d  examine %s  control %s" % (a, region(a), j - i, bytes(ma[i:j][:16]).hex(), bytes(mb[i:j][:16]).hex()))
                i = j
            i += 1
    with open(out, "w") as f:
        f.write("\n".join(L) + "\n")
    print("wrote", out, len(L), "lines")


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    if len(argv) == 2 and argv[0] == "frames":
        return cmd_frames(argv[1])
    if len(argv) == 2 and argv[0] == "video":
        return cmd_video(argv[1])
    if len(argv) == 2 and argv[0] == "trace":
        return cmd_trace(argv[1])
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main())
