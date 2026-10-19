#!/usr/bin/env python3
"""What an object really looks like: spawns it in a running room, lets the game update it for some frames and captures the GPU
primitives its draw routine adds to the ordering table, rendered with the room's own VRAM.

tools/objinit.py only runs an object's initialiser and tools/animobj.py guesses the SPR table and frames from the code; objects drawn from
several parts, children, other tables or their own primitives come out unreadable that way.  Here nothing is guessed:

 1. the room is entered from the game's debug area select (tools/psxemu/capture.py; one cached machine state per room);
 2. the placement record is handed to MAIN0 spawnObjectFromRecord (8004cb00), so the object sits in the game's own pools;
 3. the game runs frame by frame.  The "is it on screen" tests answer 1 for our object and its children (objects allocated by code
    that holds one of ours in a saved register s0..s7); everything else runs the real code;
 4. after each frame, with the memory saved, the category draw routine (the loop that draws one draw list) is called with a list holding
    only our objects, a camera that puts the object at a fixed screen point, and an empty ordering table; the packets are read back,
    rasterized by tools/psxemu/gpu.py on a copy of VRAM, and the memory restored.

The method of step 4 (one-object draw list, identity camera translated to the object, OT walk) and the scratchpad addresses of the draw
lists are from TombaEdit (github.com/SiergiejW/TombaEdit, MIT, formats/level/draw.py); the code is our own.

  tools/objdraw.py AREA SECTION RAW_HEX OUT.png [--frames 64] [--every 4] [--state-dir DIR] [--base STATE]
  tools/objdraw.py AREA SECTION L/TT OUT.png ...    first placement of layer L type TT (hex) in the room's lists
  tools/objdraw.py AREA SECTION P/TT[:SUB[:B0C]] OUT.png  layer 3 (pool) type TT with that subtype / b0c, spawned next to the player
"""
import argparse, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)

SPAWN = 0x8004CB00                      # spawnObjectFromRecord(rec, z)
SPAWNED = 0x8009BD98                    # the object it made
SPAWN_L3 = 0x80020D20                   # spawnLayer3Object(type, x, y, z): pool objects are made by other objects, not placed
PLAYER = 0x800A6038
ALLOCS = (0x800182AC, 0x8001832C, 0x800183B8, 0x80018448, 0x800184D8, 0x80018568, 0x800185F8, 0x80018678)  # allocObjectLayer*
VISIBLE = (0x800202B4, 0x80020078, 0x800201AC, 0x800203DC, 0x80020490, 0x8002052C)   # as tools/objinit.py
# scratchpad (TombaEdit formats/level/draw.py): category -> (list length u16, list pointer)
LIST_READY = 0x1F8001C6
LISTS = {1: (0x1F80024E, 0x1F80025C), 2: (0x1F800250, 0x1F800260), 3: (0x1F800252, 0x1F800264),
         4: (0x1F800254, 0x1F800268), 5: (0x1F800256, 0x1F80026C), 8: (0x1F800258, 0x1F800274), 7: (0x1F80025A, 0x1F800270)}
PACKETS, ORDERING, MATRIX = 0x1F800164, 0x1F8001E0, 0x1F8000C0
CAMERA_A, CAMERA_B = 0x1F8000EE, 0x1F8000F6
ORIGIN, DEPTH = (160, 140), 512
OT_SLOTS = 0x1000


def draw_loops(main0):
    """{category: address} of the MAIN0 routines that use the category's two list words and LIST_READY (lui/addiu pairs)."""
    import csv
    funcs = []
    for r in csv.reader(open(os.path.join(HERE, "..", "notes", "functions_main0.csv"))):
        try:
            funcs.append((int(r[0], 16), int(r[2])))
        except (ValueError, IndexError):
            pass
    out = {}
    for a, n in funcs:
        if not 0x80010000 <= a < 0x80010000 + len(main0):
            continue
        hi, used = {}, set()
        for k in range(min(n, 2000) // 4 if n > 4 else 0):
            w = struct.unpack_from("<I", main0, a - 0x80010000 + 4 * k)[0]
            op, rs, rt, imm = w >> 26, w >> 21 & 31, w >> 16 & 31, w & 0xFFFF
            if op == 15:
                hi[rt] = imm << 16
            elif op in (9, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B) and rs in hi:
                used.add((hi[rs] + ((imm ^ 0x8000) - 0x8000)) & 0xFFFFFFFF)
        for cat, (cnt, items) in LISTS.items():
            if {cnt, items, LIST_READY} <= used:
                out.setdefault(cat, a)
    return out


class Session:
    """A machine in one room; `strip(raw)` spawns and films one placement record."""

    def __init__(self, area, section, base, state_dir=None, log=print):
        from psxemu import capture as C
        self.C, self.area, self.section, self.log = C, area, section, log
        self.m = C.base_state(base)
        if not state_dir:
            import tempfile
            state_dir = tempfile.mkdtemp(prefix="objdraw")
        self.path = os.path.join(state_dir, "room_%02d_%d.state" % (area, section))
        if os.path.exists(self.path):
            self.m.load_state(self.path)
        else:
            C.capture_room(self.m, base, area, section, log)
            self.m.save_state(self.path)
        self.m.render = False
        self.loops = draw_loops(bytes(self.m.cpu.mem[0x10000:0x200000]))

    def reset(self):
        """Back to the room as entered (memory, registers, VRAM, kernel state), also after a crash."""
        self.m.load_state(self.path)
        self.m.render = False

    # ------------------------------------------------------------------ hooks
    def _passthrough(self, addr, args):
        c = self.m.cpu
        h = c.hle.pop(addr)
        try:
            return c.call(addr, *args, max_instr=1_000_000)
        finally:
            c.hle[addr] = h

    def _hook(self):
        c = self.m.cpu
        saved = {a: c.hle.get(a) for a in VISIBLE + ALLOCS}

        def visible(addr):
            def h(cpu):
                o = cpu.R[4]
                if o in self.ours:
                    cpu.w8(o + 1, 1)
                    cpu.R[2] = 1
                else:
                    cpu.R[2] = self._passthrough(addr, cpu.R[4:8])
            return h

        def alloc(addr):
            def h(cpu):
                mine = self.ours and (0 in self.ours or any(r in self.ours for r in cpu.R[16:24]))   # called by code holding one of ours in s0..s7
                v = self._passthrough(addr, cpu.R[4:8])
                cpu.R[2] = v
                if v and mine:
                    self.ours.append(v)
            return h

        for a in VISIBLE:
            c.hle[a] = visible(a)
        for a in ALLOCS:
            c.hle[a] = alloc(a)
        return saved

    def _unhook(self, saved):
        c = self.m.cpu
        for a, h in saved.items():
            if h is None:
                c.hle.pop(a, None)
            else:
                c.hle[a] = h

    # ------------------------------------------------------------------ capture
    def spawn(self, raw):
        c = self.m.cpu
        if isinstance(raw, tuple):          # a layer 3 (pool) type (type, subtype, b0c), made next to the player
            x, y, z = (struct.unpack("<h", c.read(PLAYER + k, 2))[0] for k in (0x12, 0x16, 0x1A))
            self.ours.append(0)             # spawnLayer3Object keeps no object in s0..s7: accept its allocation
            c.call(SPAWN_L3, raw[0], x + 48, y - 16, z)
            self.ours.remove(0)
            o = self.ours.pop() if self.ours else 0
            if o:
                c.w8(o + 3, raw[1])
                c.w8(o + 0xC, raw[2])
            return o
        rec = 0x801FF000        # top of RAM below the stack area the game does not reach in a room
        saved = c.read(rec, 16)
        c.write(rec, raw)
        c.call(SPAWN, rec, 0xFFFFFFFF)
        c.write(rec, saved)
        return c.r32(SPAWNED)

    def capture(self, main):
        """Packets (lists of words, far first) drawn for our live objects, the camera centred on `main`."""
        import numpy as np
        c = self.m.cpu
        live = [o for o in dict.fromkeys(self.ours) if c.r8(o) and (o == main or self._near(o, main))]
        snap, gte = bytes(c.mem), (list(c.gte.d), list(c.gte.c), c.gte.flag)
        x, y, z = (struct.unpack("<h", c.read(main + k, 2))[0] for k in (0x12, 0x16, 0x1A))
        lst, pk, ot = 0x80180000, 0x80181000, 0x801A0000       # scratch RAM for the capture, restored afterwards
        out = []
        try:
            by = {}
            for o in live:
                by.setdefault(c.r8(o + 0x1C) & 0x7F, []).append(o)
            for cat, objs in sorted(by.items()):
                loop = self.loops.get(cat)
                if loop is None:
                    continue
                cnt, items = LISTS[cat]
                for i, o in enumerate(objs):
                    c.w32(lst + 4 * i, o)
                    c.w8(o + 1, 1)
                c.w16(LIST_READY, 1)
                c.w16(cnt, len(objs))
                c.w32(items, lst)
                c.w32(PACKETS, pk)
                c.w32(ORDERING, ot)
                tbl = np.arange(OT_SLOTS, dtype=np.uint32) * 4 + (ot & 0xFFFFFF) - 4
                tbl[0] = 0xFFFFFF
                c.write(ot, tbl.astype("<u4").tobytes())
                c.write(MATRIX, struct.pack("<9hh3i", 4096, 0, 0, 0, 4096, 0, 0, 0, 4096, 0, -x, -y, DEPTH - z))
                c.w16(CAMERA_A, x & 0xFFFF)
                c.w16(CAMERA_B, z & 0xFFFF)
                c.gte.write_ctrl(24, ORIGIN[0] << 16 & 0xFFFFFFFF)
                c.gte.write_ctrl(25, ORIGIN[1] << 16 & 0xFFFFFFFF)
                c.gte.write_ctrl(26, DEPTH)
                try:
                    c.call(loop, max_instr=2_000_000)
                except Exception as e:      # noqa: BLE001
                    self.log("draw loop %d: %s" % (cat, e))
                    continue
                a, n = (ot + (OT_SLOTS - 1) * 4) & 0xFFFFFF, 0
                while a != 0xFFFFFF and n < 20000:
                    tag = c.r32(0x80000000 | a)
                    if tag >> 24:
                        out.append([c.r32(0x80000000 | a + 4 + 4 * k) for k in range(tag >> 24)])
                    a, n = tag & 0xFFFFFF, n + 1
        finally:
            c.mem[:] = snap
            c.gte.d[:], c.gte.c[:], c.gte.flag = gte
        return out

    def _near(self, o, main, r=320):
        c = self.m.cpu
        return all(abs(struct.unpack("<h", c.read(o + k, 2))[0] - struct.unpack("<h", c.read(main + k, 2))[0]) < r for k in (0x12, 0x16))

    def render(self, packets):
        """RGBA image of the packets, cropped to what they cover (None when empty)."""
        import numpy as np
        from PIL import Image
        g = self.m.gpu
        vram = g.vram.copy()
        st = (g.tpage, g.tw, g.clip, g.ofs, g.set_mask)
        KEY = 0x7C1F | 0x8000
        X, Y = self.m.drawenv["clip"][:2] if self.m.drawenv else (0, 0)      # the game's draw buffer: no textures there
        try:
            g.vram[Y:Y + 256, X:X + 320] = KEY
            g.clip, g.ofs, g.set_mask = (X, Y, X + 319, Y + 255), (X, Y), 0
            g.run_packets(packets)
            raw = g.vram[Y:Y + 256, X:X + 320].copy()
            rgb = g.rgb(X, Y, 320, 256)
        finally:
            g.vram[:] = vram
            g.tpage, g.tw, g.clip, g.ofs, g.set_mask = st
        mask = raw != KEY
        if not mask.any():
            return None
        a = np.dstack([rgb, (mask * 255).astype(np.uint8)])
        return Image.fromarray(a, "RGBA")

    def strip(self, raw, frames=64, every=4, max_instr=6_000_000, seconds=240):
        """[(frame number, image)] of a placement record spawned in this room."""
        self.reset()
        self.ours = []
        saved = self._hook()
        shots = []
        try:
            main = self.spawn(raw)
            if not main:
                return shots
            self.ours.append(main)
            import time
            t0 = time.time()
            for f in range(frames):
                if time.time() - t0 > seconds:
                    self.log("time budget reached at frame %d" % f)
                    break
                self.m.run_frames(1, max_instr=max_instr)
                if not self.m.cpu.r8(main):
                    break
                if f % every == every - 1:
                    img = self.render(self.capture(main))
                    if img is not None:
                        shots.append((f, img))
        finally:
            self._unhook(saved)
        return shots


def sheet(shots, bg=(48, 48, 64, 255), cols=8, zoom=2):
    """One PNG of the frames: a grid of equal cells around the common origin, duplicates dropped."""
    from PIL import Image
    seen, imgs = set(), []
    for _, im in shots:
        k = im.tobytes()
        if k not in seen:
            seen.add(k)
            imgs.append(im)
    if not imgs:
        return None
    boxes = [im.getbbox() for im in imgs]
    x0, y0 = min(b[0] for b in boxes), min(b[1] for b in boxes)
    x1, y1 = max(b[2] for b in boxes), max(b[3] for b in boxes)
    w, h = x1 - x0 + 2, y1 - y0 + 2
    cols = min(cols, len(imgs))
    out = Image.new("RGBA", (w * cols, h * ((len(imgs) + cols - 1) // cols)), bg)
    for i, im in enumerate(imgs):
        out.alpha_composite(im.crop((x0 - 1, y0 - 1, x1 + 1, y1 + 1)), ((i % cols) * w, (i // cols) * h))
    return out.resize((out.width * zoom, out.height * zoom), Image.NEAREST) if zoom > 1 else out


def find_record(area, section, spec):
    """raw record: hex string, 'P/TT[:SUB[:B0C]]' = layer 3 (pool) type TT (hex) spawned by spawnLayer3Object, or 'L/TT' = first placement of layer L type TT (hex) owned by the room (else any of the area)."""
    if "/" not in spec:
        return bytes.fromhex(spec)
    if spec[:2].upper() == "P/":
        v = [int(t, 16) for t in spec[2:].split(":")] + [0, 0]
        return tuple(v[:3])
    import objects as O
    layer, typ = (int(v, 16) for v in spec.split("/"))
    rows = [r for r in O.collect(area) if (r["b1"] & 0x7F) == layer and r["type"] == typ]
    rows.sort(key=lambda r: (r["section"] != section, not r.get("own")))
    if not rows:
        raise SystemExit("no placement of %s in area %d" % (spec, area))
    return bytes.fromhex(rows[0]["raw"])


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("area", type=int)
    ap.add_argument("section", type=int)
    ap.add_argument("record")
    ap.add_argument("out")
    ap.add_argument("--frames", type=int, default=64)
    ap.add_argument("--every", type=int, default=4)
    ap.add_argument("--base", required=True, help="cached debug-select state (tools/psxemu/capture.py base_state)")
    ap.add_argument("--state-dir")
    a = ap.parse_args(argv)
    s = Session(a.area, a.section, a.base, a.state_dir)
    shots = s.strip(find_record(a.area, a.section, a.record), a.frames, a.every)
    img = sheet(shots)
    if img is None:
        print("nothing drawn")
        return 1
    img.save(a.out)
    print("%s: %d frames, %dx%d" % (a.out, len(shots), img.width, img.height))
    return 0


if __name__ == "__main__":
    sys.exit(main())
