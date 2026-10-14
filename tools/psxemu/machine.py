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
import sys

from . import cpu as _cpu
from . import gpu as _gpu

ROOT = getattr(sys, "_MEIPASS", None) or os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))   # PyInstaller: notes/*.csv are bundled
VBLANK_EVERY = 25000
PAD_BUF = 0x8009F7F0       # joypadInit: PadInitDirect(0x8009F7F0, 0x8009F812)
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
    "FUN_80069390", "FUN_800693c8", "FUN_80069410",     # libpad actuator calls through a pointer libpad's (HLE'd) init sets
}
KEEP = {"StartTask", "StoreImage", "SetDefDrawEnv", "SetDefDispEnv"}


class _Card:
    """A raw 128 KB memory card image behind the BIOS file calls on "bu00:" (open/lseek/read/write/close/firstfile/nextfile/erase).
    Writes only change the copy in memory: the image file is never written."""
    def __init__(self, path):
        self.img = bytearray(open(path, "rb").read())
        self.fds = {}
        self.next_fd = 3
        self.found = []
        self.pending = None      # event spec the next card TestEvent reports (4 = EvSpIOE, 0x2000 = EvSpNEW)
        self.seen = False

    def files(self):
        out = []
        for i in range(1, 16):
            e = self.img[i * 128:(i + 1) * 128]
            if e[0] != 0x51:
                continue
            name = bytes(e[10:30]).split(b"\0")[0].decode("ascii", "replace")
            blocks, j = [i], i
            while True:
                nx = struct.unpack_from("<H", self.img, j * 128 + 8)[0]
                if nx == 0xFFFF or len(blocks) > 15:
                    break
                j = nx + 1
                blocks.append(j)
            out.append([name, struct.unpack_from("<I", e, 4)[0], blocks])
        return out

    def _name(self, c, a):
        n = c.cstr(a)
        return n.split(":", 1)[1] if ":" in n else n

    def _dirent(self, c, buf):
        if not self.found:
            return 0
        name, size, _ = self.found.pop(0)
        c.write(buf, name.encode("ascii")[:20].ljust(20, b"\0") + struct.pack("<4I", 0x50, size, 0, 0) + bytes(4))
        return buf

    def call(self, c, num, a0, a1, a2):
        import fnmatch
        if num == 0x32:                                 # open(name, mode)
            name = self._name(c, a0)
            f = next((f for f in self.files() if f[0] == name), None)
            if f is None and not a1 & 0x200:
                return -1
            data = bytearray(b"".join(bytes(self.img[b * 8192:(b + 1) * 8192]) for b in f[2])[:f[1]]) if f else bytearray(8192 * (a1 >> 16 or 1))
            fd = self.next_fd
            self.next_fd += 1
            self.fds[fd] = [data, 0]
            return fd
        if a0 not in self.fds and num in (0x33, 0x34, 0x35, 0x36):
            return -1
        if num == 0x33:                                 # lseek(fd, off, whence)
            st = self.fds[a0]
            st[1] = a1 if a2 == 0 else st[1] + a1
            return st[1]
        if num == 0x34 or num == 0x35:                  # read / write(fd, buf, n)
            data, pos = self.fds[a0]
            if num == 0x34:
                chunk = bytes(data[pos:pos + a2])
                c.write(a1, chunk)
            else:
                chunk = c.read(a1, a2)
                data[pos:pos + a2] = chunk
            self.fds[a0][1] = pos + len(chunk)
            return len(chunk)
        if num == 0x36:                                 # close
            self.fds.pop(a0, None)
            return a0
        if num == 0x42:                                 # firstfile(pattern, dirent)
            pat = self._name(c, a0)
            self.found = [f for f in self.files() if fnmatch.fnmatchcase(f[0], pat.replace("?", "?"))]
            return self._dirent(c, a1)
        if num == 0x43:                                 # nextfile(dirent)
            return self._dirent(c, a0)
        return 1                                        # erase


class Machine:
    def __init__(self, exe=None, disc=None, names=None, card=None, boot=None):
        self.cpu = c = _cpu.CPU()
        self.card = _Card(card) if card else None   # memory card image (.mcd, 128 KB raw): read by the BIOS file calls on bu00:
        self.gpu = _gpu.GPU()
        self.names = names or load_names()
        self.addr = {}
        for n, al in self.names.items():
            for a in al:
                self.addr.setdefault(a, n)
        self.disc_path = disc or os.path.join(ROOT, "game", "disc", "Tombi.bin")
        self._disc = None
        self._layout = None
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
        self.pad_buf = PAD_BUF
        self.boot_path = None
        if boot is not None:            # boot the disc: SYSTEM.CNF -> SCES_013.31 menu entry `boot` -> MAINn.EXE through the ISO directory
            self.boot_path, _, _, exe = self.boot_menu_exe(boot)
        if exe is not False:
            exe = exe or os.path.join(ROOT, "game", "MAIN0.EXE")
            if names is None:
                self.remap_names(exe)
            self.load_exe(exe)
        self.install_hle()
        if boot is not None:
            c.w16(0x1F8003D8, 1)        # what SCES_013.31 leaves in the scratchpad before LoadExec (disc 01331, digital pad)
            c.w16(0x1F8003DA, 0)

    # ------------------------------------------------------------------ loading
    def load_exe(self, path):
        d = path if isinstance(path, (bytes, bytearray)) else open(path, "rb").read()
        pc, gp, taddr, tsize = struct.unpack_from("<4I", d, 0x10)
        spb, spo = struct.unpack_from("<2I", d, 0x30)
        self.cpu.write(taddr, d[0x800:0x800 + tsize])
        self.entry = pc
        self.cpu.R[28] = gp
        self.cpu.R[29] = (spb + spo) & 0xFFFFFFFF if spb else 0x801FFFF0
        self.cpu.pc = pc
        return pc

    def remap_names(self, exe):
        """The names/HLE addresses are those of MAIN0. The other MAINn (MAIN5 = Spanish: code from 0x8004DB4C linked 6 words
        earlier, data 0x28 bytes later) get every named function by matching its first instructions (immediates and jump targets
        masked) at the same address or 24 bytes earlier; PAD_BUF is read from joypadInit (lui/addiu a0)."""
        main0 = os.path.join(ROOT, "game", "MAIN0.EXE")
        b = exe if isinstance(exe, (bytes, bytearray)) else open(exe, "rb").read()
        if not os.path.exists(main0) or open(main0, "rb").read() == b:
            return
        a = open(main0, "rb").read()

        def words(d, o, n=12):
            out = []
            for (w,) in struct.iter_unpack("<I", d[o:o + 4 * n]):
                op = w >> 26
                out.append(w & 0xFC000000 if op in (2, 3) else w if op == 0 else w & 0xFFFF0000)
            return out
        names = {}
        for n, al in self.names.items():
            for ad in al:
                o = ad - 0x80010000 + 0x800
                if not 0x800 <= o < len(a) - 48:
                    names.setdefault(n, []).append(ad)
                    continue
                s = words(a, o)
                for d in (0, -24):
                    if words(b, o + d) == s:
                        names.setdefault(n, []).append(ad + d)
                        break
        self.names = names
        self.addr = {}
        for n, al in names.items():
            for x in al:
                self.addr.setdefault(x, n)
        j = names.get("joypadInit", [None])[0]
        if j:
            o = j - 0x80010000 + 0x800
            hi, lo = struct.unpack_from("<2I", b, o + 8)
            if hi >> 16 == 0x3C04 and lo >> 16 == 0x2484:
                self.pad_buf = (((hi & 0xFFFF) << 16) + ((lo & 0xFFFF) ^ 0x8000) - 0x8000) & 0xFFFFFFFF

    # ------------------------------------------------------------------ booting the disc the way the console does
    def iso_find(self, path):
        """(lba, size) of a file of the disc found through the ISO9660 directory (primary volume descriptor -> root -> subdirectories),
        the way the BIOS resolves "cdrom:\\EXE\\MAIN5.EXE;1" for LoadExec. None if it is not there."""
        pvd = self.read_sectors(16, 1)
        lba, size = struct.unpack_from("<I", pvd, 156 + 2)[0], struct.unpack_from("<I", pvd, 156 + 10)[0]
        parts = [p for p in path.replace("/", "\\").split(":", 1)[-1].split("\\") if p]
        for i, part in enumerate(parts):
            want = part.upper() if ";" in part or i < len(parts) - 1 else part.upper() + ";1"
            data = self.read_sectors(lba, (size + 2047) // 2048)
            o, hit = 0, None
            while o < len(data):
                n = data[o]
                if n == 0:
                    o = (o // 2048 + 1) * 2048
                    continue
                name = data[o + 33:o + 33 + data[o + 32]].decode("ascii", "replace").upper()
                if name == want or (i < len(parts) - 1 and name == part.upper()):
                    hit = struct.unpack_from("<I", data, o + 2)[0], struct.unpack_from("<I", data, o + 10)[0]
                    break
                o += n
            if hit is None:
                return None
            lba, size = hit
        return lba, size

    def boot_menu_exe(self, choice):
        """What SCES_013.31 does after its language menu, without running the menu: BOOT= of SYSTEM.CNF names the boot executable, its
        table of executables (pointers to "\\EXE\\MAINn.EXE;1", one per menu entry) gives the path of `choice` (0..4 = MAIN0, 5, 6, 7, 8),
        the path is resolved through the ISO directory like the BIOS LoadExec does, and the scratchpad words the boot executable leaves
        for the game are set (0x1F8003D8 = 1: disc SCES-01331, it picks the save names; 0x1F8003DA = pad type, 0 = digital pad).
        Returns (path, lba, size, bytes)."""
        cnf = self.iso_find("SYSTEM.CNF")
        text = self.read_sectors(cnf[0], 1)[:cnf[1]].decode("ascii", "replace")
        boot = next(l.split("=", 1)[1].strip() for l in text.splitlines() if l.strip().upper().startswith("BOOT"))
        blba, bsize = self.iso_find(boot)
        sces = self.read_sectors(blba, (bsize + 2047) // 2048)[:bsize]
        base = struct.unpack_from("<I", sces, 0x18)[0] - 0x800
        paths = {}
        for o in range(0x800, len(sces) - 4, 4):
            p = struct.unpack_from("<I", sces, o)[0]
            q = p - base
            if 0x800 <= q < len(sces) and sces[q:q + 5] == b"\\EXE\\":
                paths[o] = sces[q:sces.index(b"\0", q)].decode("ascii")
        # the menu table: 8-byte entries {path, "cdrom:"} in menu order
        runs = sorted(paths)
        table = None
        for o in runs:
            seq = [paths.get(o + 8 * k) for k in range(5)]
            if all(seq):
                table = seq
                break
        if not table:
            raise RuntimeError("no executable table in %s" % boot)
        path = "cdrom:" + table[choice]
        lba, size = self.iso_find(path)
        return path, lba, size, self.read_sectors(lba, (size + 2047) // 2048)[:size]

    def disc(self):
        if self._disc is None:
            self._disc = open(self.disc_path, "rb")
        return self._disc

    def read_sectors(self, lba, n):
        f = self.disc()
        if self._layout is None:        # raw 2352 byte sectors (Tombi.bin) or a cooked 2048 byte image (.iso)
            f.seek(16 * 2352 + 25)
            self._layout = (2352, 24) if f.read(5) == b"CD001" else (2048, 0)
        size, off = self._layout
        out = bytearray()
        for i in range(n):
            f.seek((lba + i) * size + off)
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
            if self.card and n == "MemCardInit":       # opens the card events TestEvent answers (its _bu_init etc. stay stubs)
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
        if self.card:
            self.cpu.write(0x150, bytes(8))     # no kernel device table (libapi's firstfile walks it looking for "bu" first)
            # libcard: the operation "completes" at once; the first _card_info after the card is inserted reports a new card (EvSpNEW),
            # the game then clears it (_card_clear) and asks again, as on hardware
            def card_op(name):
                def f(c):
                    k = self.card
                    k.pending = 0x2000 if name == "_card_info" and not k.seen else 4
                    k.seen = k.seen or name == "_card_info"
                    c.R[2] = 1
                return f
            # libapi's firstfile2 (just before the BIOS stubs: firstfile - 0x2A0) looks the "bu" device up in the kernel's device table
            # (there is no kernel) and patches it before calling firstfile: go to the card directly
            ff = self.names.get("firstfile", [0])[0] - 0x2A0
            if ff > 0 and self.cpu.r32(ff) == 0x27BDFFD8:
                h[ff] = lambda c: c.R.__setitem__(2, self.card.call(c, 0x42, c.R[4], c.R[5], 0) & 0xFFFFFFFF)
            for n in ("_card_info", "_card_load", "_card_clear", "_card_write", "_card_read", "_new_card"):
                for a in self.names.get(n, []):
                    h[a] = card_op(n)
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
        elif k == (0xB0, 0x0B):     # TestEvent: memory card I/O completes at once (EvSpIOE of SwCARD/HwCARD) when a card is inserted
            ev = self.events.get(a0)
            R[2] = 0
            if self.card and ev and ev[0] in (0xF4000001, 0xF0000011) and ev[1] == self.card.pending:
                R[2] = 1
                self.card.pending = None
        elif table == 0xB0 and num in (0x32, 0x33, 0x34, 0x35, 0x36, 0x42, 0x43, 0x45) and self.card:
            R[2] = self.card.call(c, num, a0, a1, a2) & 0xFFFFFFFF
            self.card.pending = 4       # asynchronous reads/writes (mode 0x8000) end with EvSpIOE: at once
            if self.verbose:
                print("card B0:%02X %08X %08X %08X -> %d" % (num, a0, a1, a2, R[2]))
        elif k == (0xA0, 0x3F) or k == (0xA0, 0x3E):
            R[2] = 0
        elif k == (0xB0, 0x56) or k == (0xB0, 0x57):
            R[2] = 0x80000E00
        else:
            if self.verbose:
                print("BIOS %02X:%02X a0=%08X" % (table, num, a0))
            R[2] = 0

    # ------------------------------------------------------------------ VBlank
    def set_pad(self, buttons):
        """Holds pad 1 buttons (PSX bit mask, 1 = pressed: SELECT 0x1, START 0x8, UP 0x10, RIGHT 0x20, DOWN 0x40, LEFT 0x80, L2 0x100,
        R2 0x200, L1 0x400, R1 0x800, TRIANGLE 0x1000, CIRCLE 0x2000, CROSS 0x4000, SQUARE 0x8000).  The game reads the PadInitDirect
        buffer at PAD_BUF (status 0, id 0x41 = digital pad, buttons active low)."""
        self.pad = buttons
        c = self.cpu
        c.w8(self.pad_buf, 0)
        c.w8(self.pad_buf + 1, 0x41)
        c.w16(self.pad_buf + 2, ~buttons & 0xFFFF)

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
        marks = self.walk_marks = []      # per primitive: address of the last empty OT entry before it (= its ordering table slot)
        mark = 0
        self.walk_head = a & 0xFFFFFF
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
                marks.append(mark)
            else:
                mark = a
            a = tag & 0xFFFFFF
        return out

    def draw_packets(self, pk):
        env = dict(self.drawenv) if self.drawenv else None
        self.frames.append((env, pk, self.dispenv, getattr(self, "walk_marks", None), getattr(self, "walk_head", 0)))
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
        self.ot_info = (ot & 0xFFFFFF, n, 0)
        for i in range(n - 1):
            c.w32(ot + 4 * i, (ot + 4 * (i + 1)) & 0xFFFFFF)
        c.w32(ot + 4 * (n - 1), 0xFFFFFF)
        c.R[2] = ot

    def h_clear_otagr(self, c):
        ot, n = c.R[4], c.R[5]
        self.ot_info = (ot & 0xFFFFFF, n, 1)      # (address, entries, reversed)
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

    # ------------------------------------------------------------------ save states
    STATE_KEYS = ("loc", "threads", "cur_th", "next_th", "events", "next_ev", "vblank_funcs", "vsync_cb", "vblanks", "frame_count",
                  "drawenv", "dispenv", "opened", "pad")

    def save_state(self, path):
        import pickle
        c = self.cpu
        st = {k: getattr(self, k, None) for k in self.STATE_KEYS}
        st.update(mem=bytes(c.mem), R=list(c.R), hi=c.hi, lo=c.lo, pc=c.pc, cop0=list(c.cop0), icount=c.icount,
                  gte=(list(c.gte.d), list(c.gte.c), c.gte.flag), vram=self.gpu.vram.tobytes(),
                  gpu=(self.gpu.tpage, self.gpu.tw, self.gpu.clip, self.gpu.ofs, self.gpu.set_mask),
                  card_seen=bool(self.card and self.card.seen))
        with open(path, "wb") as f:
            pickle.dump(st, f)

    def load_state(self, path):
        import pickle
        import numpy as np
        with open(path, "rb") as f:
            st = pickle.load(f)
        c = self.cpu
        c.mem[:] = st["mem"]
        c.R[:] = st["R"]
        c.hi, c.lo, c.pc, c.icount = st["hi"], st["lo"], st["pc"], st["icount"]
        c.cop0[:] = st["cop0"]
        c.gte.d[:], c.gte.c[:], c.gte.flag = st["gte"][0], st["gte"][1], st["gte"][2]
        self.gpu.vram[:] = np.frombuffer(st["vram"], dtype=np.uint16).reshape(self.gpu.vram.shape)
        g = self.gpu
        g.tpage, g.tw, g.clip, g.ofs, g.set_mask = st["gpu"]
        c.blocks.clear()
        for k in self.STATE_KEYS:
            setattr(self, k, st[k])
        if self.pad is not None:
            self.set_pad(self.pad)
        if self.card:
            c.write(0x150, bytes(8))
            self.card.seen = st.get("card_seen", True)

    pad = None

    # ------------------------------------------------------------------ output
    def png_area(self, x, y, w, h):
        from PIL import Image
        return Image.fromarray(self.gpu.rgb(x, y, w, h), "RGB")
