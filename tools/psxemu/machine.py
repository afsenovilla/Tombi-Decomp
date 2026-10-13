"""The PlayStation around the CPU: MAIN0.EXE + disc + HLE of the BIOS and of the Psy-Q libraries the game links.

HLE is by function address (notes/names_main0.csv + notes/functions_main0.csv):
  * BIOS (A0/B0/C0): threads (OpenTh/ChangeTh/CloseTh, the game's task system runs on them), events (VBlank root counter event =
    the game's vblankHandler), the rest returns 0;
  * libgpu: ResetGraph/DrawSync/LoadImage/StoreImage/MoveImage/ClearImage/PutDrawEnv/PutDispEnv/DrawOTag/DrawOTagEnv/DrawPrim/
    ClearOTag(R)/VSync -> gpu.GPU;
  * libcd: CdInit/CdControl(F/B)/CdSync/CdRead/CdReadSync read sectors of the disc image (Setloc = BCD position);
  * libsnd/libspu/libpress/libcard/libpad/callbacks: stubs returning 0.
The game's own loader (cdLoadTask: CdRead + lzDecompress + LoadImage) runs natively on top of that.
"""
import csv
import os
import struct

from . import cpu as _cpu
from . import gpu as _gpu

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
VBLANK_EVERY = 25000
MOVIE_TASK = 0x8001D6A4     # moviePlayerTask (psx_tomba name)        # instructions between two simulated VBlank interrupts


def bcd(b):
    return (b >> 4) * 10 + (b & 15)


def load_names(paths=None):
    """{name: [addresses]} from the names / functions csv files."""
    out = {}
    paths = paths or [os.path.join(ROOT, "notes", "functions_main0.csv"), os.path.join(ROOT, "notes", "names_main0.csv")]
    for p in paths:
        if not os.path.exists(p):
            continue
        with open(p, newline="", encoding="utf-8") as f:
            for row in csv.DictReader(f):
                try:
                    a = int(row["address"], 16)
                except (KeyError, ValueError):
                    continue
                out.setdefault(row["name"], [])
                if a not in out[row["name"]]:
                    out[row["name"]].append(a)
    return out


STUB_PREFIXES = ("Ss", "Spu", "_Ss", "_spu", "_Spu", "SpuR", "DecDCT", "St", "Pad", "_card", "_bu", "Card", "Mem", "Sound", "Sfx")
STUB_NAMES = {
    "ResetCallback", "InterruptCallback", "DMACallback", "VSyncCallbacks", "StopCallback", "RestartCallback",
    "CheckCallback", "SetIntrMask", "startIntr", "stopIntr", "trapIntrVSync", "setIntrVSync", "_SsTrapIntrVSync",
    "InitCARD", "StartCARD", "StopCARD", "MemCardInit", "joypadInit", "SetVideoMode", "SetGraphDebug", "SetGraphQueue",
    "DrawSyncCallback", "CdSyncCallback", "CdReadyCallback", "CdDataCallback", "CdReadCallback", "CD_initintr",
    "cdSeekStream", "CdFlush", "CdSetDebug", "CdReadBreak", "SoundInit", "SoundShutdown", "SoundUpdate", "SoundStopAll",
    "SfxStopAll", "CdMix", "SetGraphReverse", "InitGeom_skip",
}
KEEP = {"StartTask", "StoreImage", "SetDefDrawEnv", "SetDefDispEnv"}


class Machine:
    def __init__(self, exe=None, disc=None, names=None):
        self.cpu = c = _cpu.CPU()
        self.gpu = _gpu.GPU()
        self.names = names or load_names()
        self.addr = {}
        for n, al in self.names.items():
            for a in al:
                self.addr.setdefault(a, n)
        self.disc_path = disc or os.path.join(ROOT, "game", "disc", "Tombi.bin")
        self._disc = None
        self.loc = 0
        self.threads = {}
        self.cur_th = 0xFF000000
        self.next_th = 0xFF000001
        self.events = {}
        self.next_ev = 0xF1000000
        self.vblank_funcs = []
        self.vsync_cb = None
        self.vblanks = 0
        self.render = True
        self.frames = []           # [(drawenv dict, packet list)] of the DrawOTag calls
        self.keep_frames = 4
        self.drawenv = None
        self.dispenv = None
        self.log = []
        self.verbose = False
        self.on_draw = None
        self.opened = []
        c.bios = self.bios
        if exe is not False:
            self.load_exe(exe or os.path.join(ROOT, "game", "MAIN0.EXE"))
        self.install_hle()

    # ------------------------------------------------------------------ loading
    def load_exe(self, path):
        d = open(path, "rb").read()
        pc, gp, taddr, tsize = struct.unpack_from("<4I", d, 0x10)
        spb, spo = struct.unpack_from("<2I", d, 0x30)
        self.cpu.write(taddr, d[0x800:0x800 + tsize])
        self.entry = pc
        self.cpu.R[28] = gp
        self.cpu.R[29] = (spb + spo) & 0xFFFFFFFF if spb else 0x801FFFF0
        self.cpu.pc = pc
        return pc

    def disc(self):
        if self._disc is None:
            self._disc = open(self.disc_path, "rb")
        return self._disc

    def read_sectors(self, lba, n):
        f = self.disc()
        out = bytearray()
        for i in range(n):
            f.seek((lba + i) * 2352 + 24)
            out += f.read(2048)
        return bytes(out)

    def fn(self, name):
        a = self.names.get(name)
        if not a:
            raise KeyError(name)
        return a[0]

    # ------------------------------------------------------------------ HLE table
    def install_hle(self):
        h = self.cpu.hle
        R = self.cpu.R

        def ret(v):
            def f(c):
                c.R[2] = v & 0xFFFFFFFF
            return f

        for n, al in self.names.items():
            if n in KEEP:
                continue
            if n in STUB_NAMES or (n.startswith(STUB_PREFIXES) and not n.startswith("Store")):
                for a in al:
                    h[a] = ret(0)
        table = {
            "ResetGraph": ret(0), "SetDispMask": ret(0), "DrawSync": ret(0),
            "LoadImage": self.h_load_image, "StoreImage": self.h_store_image, "MoveImage": self.h_move_image,
            "ClearImage": self.h_clear_image, "ClearImage2": self.h_clear_image,
            "PutDrawEnv": self.h_put_drawenv, "PutDispEnv": self.h_put_dispenv,
            "DrawOTag": self.h_draw_otag, "DrawOTagEnv": self.h_draw_otag_env, "DrawPrim": self.h_draw_prim,
            "ClearOTag": self.h_clear_otag, "ClearOTagR": self.h_clear_otagr,
            "VSync": self.h_vsync, "VSyncCallback": self.h_vsync_callback,
            "CdInit": ret(1), "CdControl": self.h_cd_control, "CdControlF": self.h_cd_control, "CdControlB": self.h_cd_control,
            "CdSync": ret(2), "CdReady": ret(2), "CdRead": self.h_cd_read, "CdReadSync": ret(0), "CdStatus": ret(2),
            "CdReset": ret(1), "CdMode": ret(0x80),
            "printf": self.h_printf,
        }
        for n, f in table.items():
            for a in self.names.get(n, []):
                h[a] = f
        # FMV player task (0x8001D6A4, MDEC + streaming): the movie "ends" at once: MOVIE_PLAY_STATE (0x1F8001CC) = idle, exitTask
        def movie(c):
            c.w8(0x1F8001CC, 0)
            c.pc_override = self.fn("exitTask")
        h[MOVIE_TASK] = movie

    # ------------------------------------------------------------------ BIOS
    def bios(self, c, table, num):
        R = c.R
        a0, a1, a2 = R[4], R[5], R[6]
        k = (table, num)
        if k == (0xB0, 0x0E):       # OpenThread(pc, sp, gp)
            tid = self.next_th
            self.next_th += 1
            regs = [0] * 32
            regs[29] = a1
            regs[30] = a1
            regs[28] = a2
            self.threads[tid] = (regs, 0, 0, a0)
            self.opened.append((tid, a0))
            if self.verbose:
                print("OpenTh %08X -> %s" % (a0, self.addr.get(a0, "?")))
            R[2] = tid
        elif k == (0xB0, 0x0F):     # CloseThread
            self.threads.pop(a0, None)
            R[2] = 1
        elif k == (0xB0, 0x10):     # ChangeThread
            tid = a0
            if tid == self.cur_th or tid not in self.threads and tid != 0xFF000000:
                R[2] = 1 if tid == self.cur_th else 0
                return
            saved = list(R)
            saved[2] = 1
            self.threads[self.cur_th] = (saved, c.hi, c.lo, R[31])
            regs, hi, lo, pc = self.threads[tid]
            R[:] = regs
            c.hi, c.lo = hi, lo
            self.cur_th = tid
            c.pc_override = pc
        elif k == (0xB0, 0x08):     # OpenEvent(class, spec, mode, func)
            ev = self.next_ev
            self.next_ev += 1
            self.events[ev] = (a0, a1, a2, R[7])
            if a0 == 0xF2000003 and R[7]:
                self.vblank_funcs.append(R[7])
            R[2] = ev
        elif k in ((0xB0, 0x0C), (0xB0, 0x0D), (0xB0, 0x09), (0xB0, 0x0A)):
            R[2] = 1
        elif k == (0xB0, 0x0B):     # TestEvent
            R[2] = 0
        elif k == (0xA0, 0x3F) or k == (0xA0, 0x3E):
            R[2] = 0
        elif k == (0xB0, 0x56) or k == (0xB0, 0x57):
            R[2] = 0x80000E00
        else:
            if self.verbose:
                print("BIOS %02X:%02X a0=%08X" % (table, num, a0))
            R[2] = 0

    # ------------------------------------------------------------------ VBlank
    def vblank(self):
        self.vblanks += 1
        c = self.cpu
        for f in self.vblank_funcs:
            c.call(f, max_instr=200000)
        if self.vsync_cb:
            c.call(self.vsync_cb, max_instr=200000)

    def run(self, instr):
        """Runs `instr` instructions, firing a VBlank every VBLANK_EVERY."""
        c = self.cpu
        done = 0
        while done < instr:
            n = c.run(max_instr=VBLANK_EVERY)
            done += n
            self.vblank()

    def run_frames(self, n, max_instr=None):
        """Runs until n more DrawOTag calls (frames) happened."""
        target = self.frame_count + n
        c = self.cpu
        start = c.icount
        while self.frame_count < target:
            c.run(max_instr=VBLANK_EVERY)
            self.vblank()
            if max_instr and c.icount - start > max_instr:
                break

    frame_count = 0

    # ------------------------------------------------------------------ libgpu
    def _rect(self, a):
        return struct.unpack("<4h", self.cpu.read(a, 8))

    def h_load_image(self, c):
        x, y, w, h = self._rect(c.R[4])
        if w > 0 and h > 0:
            self.gpu.load_image(x & 0x3FF, y & 0x1FF, w, h, c.read(c.R[5], w * h * 2))
        c.R[2] = 0

    def h_store_image(self, c):
        x, y, w, h = self._rect(c.R[4])
        if w > 0 and h > 0:
            c.write(c.R[5], self.gpu.store_image(x & 0x3FF, y & 0x1FF, w, h))
        c.R[2] = 0

    def h_move_image(self, c):
        x, y, w, h = self._rect(c.R[4])
        if w > 0 and h > 0:
            self.gpu.move(x, y, c.R[5] & 0x3FF, c.R[6] & 0x1FF, w, h)
        c.R[2] = 0

    def h_clear_image(self, c):
        x, y, w, h = self._rect(c.R[4])
        r, g, b = c.R[5] & 0xFF, c.R[6] & 0xFF, c.R[7] & 0xFF
        self.gpu.fill(x, y, w, h, _gpu.rgb15(r | g << 8 | b << 16))
        c.R[2] = 0

    def parse_drawenv(self, a):
        d = self.cpu.read(a, 28)
        cx, cy, cw, ch, ox, oy, twx, twy, tww, twh, tpage = struct.unpack_from("<11h", d, 0)
        dtd, dfe, isbg, r, g, b = d[22], d[23], d[24], d[25], d[26], d[27]
        return dict(clip=(cx, cy, cw, ch), ofs=(ox, oy), tw=(twx, twy, tww, twh), tpage=tpage & 0xFFFF, dtd=dtd, dfe=dfe,
                    isbg=isbg, rgb=(r, g, b))

    def apply_drawenv(self, e):
        g = self.gpu
        cx, cy, cw, ch = e["clip"]
        g.clip = (cx, cy, cx + cw - 1, cy + ch - 1)
        g.ofs = e["ofs"]
        g.tpage = e["tpage"]
        tx, ty, tw, th = e["tw"]
        g.tw = ((~(tw >> 3) & 31) if tw else 0, (~(th >> 3) & 31) if th else 0, (tx >> 3) & 31, (ty >> 3) & 31) if (tw or th) else (0, 0, 0, 0)
        if e["isbg"]:
            r, gg, b = e["rgb"]
            g.fill(cx, cy, cw, ch, _gpu.rgb15(r | gg << 8 | b << 16))

    def h_put_drawenv(self, c):
        e = self.parse_drawenv(c.R[4])
        self.drawenv = e
        self.apply_drawenv(e)
        c.R[2] = c.R[4]

    def h_put_dispenv(self, c):
        d = c.read(c.R[4], 20)
        x, y, w, h, sx, sy, sw, sh = struct.unpack_from("<8h", d, 0)
        self.dispenv = dict(disp=(x, y, w, h), screen=(sx, sy, sw, sh), isinter=d[16], isrgb24=d[17])
        c.R[2] = c.R[4]

    def walk_ot(self, a):
        """[(addr, [words])] of the primitives of an OT/primitive list starting at a."""
        out = []
        c = self.cpu
        seen = 0
        a &= 0xFFFFFF
        while a != 0xFFFFFF and seen < 200000:
            seen += 1
            o = _cpu.phys(a | 0x80000000)
            if o < 0:
                break
            tag = struct.unpack_from("<I", c.mem, o)[0]
            n = tag >> 24
            if n:
                out.append((a, list(struct.unpack_from("<%dI" % n, c.mem, o + 4))))
            a = tag & 0xFFFFFF
        return out

    def draw_packets(self, pk):
        env = dict(self.drawenv) if self.drawenv else None
        self.frames.append((env, pk, self.dispenv))
        del self.frames[:-self.keep_frames]
        self.frame_count += 1
        if self.render:
            self.gpu.run_packets([w for _, w in pk])
        if self.on_draw:
            self.on_draw(self, pk)

    def h_draw_otag(self, c):
        self.draw_packets(self.walk_ot(c.R[4]))

    def h_draw_otag_env(self, c):
        e = self.parse_drawenv(c.R[5])
        self.drawenv = e
        self.apply_drawenv(e)
        self.draw_packets(self.walk_ot(c.R[4]))

    def h_draw_prim(self, c):
        o = c.R[4]
        tag = c.r32(o)
        n = tag >> 24
        w = [c.r32(o + 4 + 4 * i) for i in range(n)]
        if self.render:
            self.gpu.run_packets([w])

    def h_clear_otag(self, c):
        ot, n = c.R[4], c.R[5]
        for i in range(n - 1):
            c.w32(ot + 4 * i, (ot + 4 * (i + 1)) & 0xFFFFFF)
        c.w32(ot + 4 * (n - 1), 0xFFFFFF)
        c.R[2] = ot

    def h_clear_otagr(self, c):
        ot, n = c.R[4], c.R[5]
        o = _cpu.phys(ot)
        if o >= 0 and n > 0:
            base = ot & 0xFFFFFF
            words = [(base + 4 * (i - 1)) & 0xFFFFFF for i in range(n)]
            words[0] = 0xFFFFFF
            c.mem[o:o + 4 * n] = struct.pack("<%dI" % n, *words)
        c.R[2] = ot

    def h_vsync(self, c):
        mode = _cpu.s32(c.R[4])
        if mode < 0:
            c.R[2] = self.vblanks
            return
        if mode == 0:
            self.vblank()
        elif mode > 1:
            for _ in range(mode):
                self.vblank()
        c.R[2] = self.vblanks

    def h_vsync_callback(self, c):
        self.vsync_cb = c.R[4] or None
        c.R[2] = 0

    # ------------------------------------------------------------------ libcd
    def h_cd_control(self, c):
        com, param = c.R[4] & 0xFF, c.R[5]
        if com in (0x02, 0x15, 0x16) and param:
            m, s, f = c.read(param, 3)
            self.loc = (bcd(m) * 60 + bcd(s)) * 75 + bcd(f) - 150
        c.R[2] = 1

    def h_cd_read(self, c):
        n, buf = c.R[4], c.R[5]
        data = self.read_sectors(self.loc, n)
        c.write(buf, data)
        if self.verbose:
            print("CdRead lba %d x%d -> %08X" % (self.loc, n, buf))
        self.loc += n
        c.R[2] = 1

    def h_printf(self, c):
        if self.verbose:
            print("printf:", c.cstr(c.R[4]).rstrip())
        c.R[2] = 0

    # ------------------------------------------------------------------ output
    def png_area(self, x, y, w, h):
        from PIL import Image
        return Image.fromarray(self.gpu.rgb(x, y, w, h), "RGB")
