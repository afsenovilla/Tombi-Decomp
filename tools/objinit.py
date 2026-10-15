#!/usr/bin/env python3
"""What an object's init code sets for its SPR drawing (texture page offset, CLUT, sprite bank), found by running the code.

An SPR object is drawn by `FUN_80052db8` with `tpage += o->w1e` and, when `o->b0d & 1`, `clut = o->w08`; both fields (and the SPR bank
pointer `o->w3c`, one of the scratchpad slots 0x1F8002C8 + 4 * n) are set by the handler of the object's type the first time it runs, often from
the placement record (variant byte, type) rather than from a constant, so reading the immediates of the code (tools/animobj.py) is only a
guess.  Here the handler is run on a zeroed object filled from the 16 byte placement record, in the interpreter of tools/psxemu/cpu.py.

The method (run one object's initialiser in isolated memory, answer the on-screen tests, GetClut and the draw-list calls with stubs, then
read the object back) and the PAL routine addresses below are from TombaEdit (github.com/SiergiejW/TombaEdit, MIT, formats/level/runtime.py
and game/builds.py); the code is our own.

  tools/objinit.py AREA HANDLER_HEX RAW_HEX   one record: prints w1e / w08 / b0d / SPR slot
"""
import os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)

# PAL (first language) routine addresses, as in TombaEdit game/builds.py EUROPE
GET_CLUT = 0x8005E420
VISIBLE = (0x800202B4, 0x80020078, 0x800201AC, 0x800203DC, 0x80020490, 0x8002052C)    # "is it on screen" tests: answer 1, set o->b01
QUEUES = (0x80018C2C, 0x80018C68, 0x80018CA4, 0x80018CF0, 0x80018D2C, 0x80018D68, 0x80018DA4)   # append an object to a draw list
SILENT = (0x8001E4F0, 0x80020A78, 0x80020B64)       # sound / flag queries: answer 0
SPR_SLOTS = 0x1F8002C8
OBJ = 0x80300000
SPR_MEM, BANK = 0x80400000, 0x40000
OVERLAY = 0x800E8028
STEPS, STEP_INSTR = 4, 4000

_exe = {}


def _cpu(area, reader):
    from psxemu import cpu as C
    c = C.CPU()
    if "exe" not in _exe:
        _exe["exe"] = reader("EXE/MAIN0.EXE") if reader else open(os.path.join(HERE, "..", "game", "MAIN0.EXE"), "rb").read()
    d = _exe["exe"]
    pc, gp, taddr, tsize = struct.unpack_from("<4I", d, 0x10)
    c.write(taddr, d[0x800:0x800 + tsize])
    c.R[28] = gp
    path = os.path.join(HERE, "..", "game", "AREA%02d" % area, "X0%02d.BIN" % area)
    c.write(OVERLAY, reader("AREA%02d/X0%02d.BIN" % (area, area)) if reader else open(path, "rb").read())
    for n in range(16):
        c.w32(SPR_SLOTS + 4 * n, SPR_MEM + n * BANK)

    def ret(f=None):
        def h(cpu):
            if f:
                f(cpu)
        return h

    def visible(cpu):
        o = cpu.R[4]
        if OBJ <= o < OBJ + 0x200:
            cpu.w8(o + 1, 1)
        cpu.R[2] = 1

    def clut(cpu):
        cpu.R[2] = (cpu.R[5] & 511) << 6 | (cpu.R[4] >> 4) & 63

    for a in VISIBLE:
        c.hle[a] = visible
    c.hle[GET_CLUT] = clut
    for a in QUEUES + SILENT:
        c.hle[a] = ret(lambda cpu: cpu.R.__setitem__(2, 0))
    return c


_cpus = {}


def run_init(area, handler, raw, reader=None):
    """Runs `handler` (an object update routine of the area overlay) on a fresh object made from the 16 byte placement record `raw`
    (the way the record spawner fills it, as TombaEdit's InitialPoseResolver.spawn does).  Returns dict(w1e, w08, b0d, b2e, slot) or None."""
    c = _cpus.get(area)
    if c is None:
        c = _cpus[area] = _cpu(area, reader)
        c.base_mem = bytes(c.mem)
    c.mem[:] = c.base_mem
    c.write(OBJ, bytes(0x200))
    c.w32(OBJ + 0x40, OBJ + 0x10)
    c.w32(OBJ + 0x44, OBJ + 0x18)
    c.write(OBJ, bytes((1, 0, raw[2], raw[4])))
    for off, v in ((0xC, raw[3]), (0xA, raw[12]), (0xF, raw[13]), (0x1C, raw[1]), (0x1D, raw[0]), (0x6B, ((raw[15] >> 2) - 1) & 0xFF)):
        c.w8(OBJ + off, v)
    c.write(OBJ + 0x2E, bytes((raw[5], 0)))
    for j, v in enumerate(struct.unpack_from("<3h", raw, 6)):
        c.w32(OBJ + 0x10 + 4 * j, (v << 16) & 0xFFFFFFFF)
    c.write(OBJ + 0x74, struct.pack("<3h", 4096, 4096, 4096))
    for _ in range(STEPS):
        c.R[29] = 0x803F0000
        c.R[4] = OBJ
        c.R[31] = 0x80001000
        c.pc = handler
        try:
            c.run(0x80001000, STEP_INSTR)
        except Exception:
            break
        if c.r32(OBJ + 0x3C) and c.r32(OBJ + 0x24):      # bank and animation chosen: initialised
            break
    sp = c.r32(OBJ + 0x3C)
    slot = (sp - SPR_MEM) // BANK if SPR_MEM <= sp < SPR_MEM + 16 * BANK and (sp - SPR_MEM) % BANK == 0 else None
    b0d = c.r8(OBJ + 0x0D)
    return dict(w1e=c.r16(OBJ + 0x1E), w08=c.r16(OBJ + 0x08), b0d=b0d, b2e=c.r8(OBJ + 0x2E), slot=slot)


def owner_inits(area, handler, raws, reader=None):
    """[(init dict, count)] over the placement records of one handler, most common first."""
    import collections
    cnt = collections.Counter()
    for r in raws:
        try:
            i = run_init(area, handler, bytes.fromhex(r) if isinstance(r, str) else r, reader)
        except Exception:
            continue
        if i and i["slot"] is not None:
            cnt[(i["w1e"], i["w08"] if i["b0d"] & 1 else None, i["slot"], i["b0d"] & 0x81)] += 1
    return [(dict(w1e=k[0], clut=k[1], slot=k[2], b0d=k[3]), n) for k, n in cnt.most_common()]


if __name__ == "__main__":
    print(run_init(int(sys.argv[1]), int(sys.argv[2], 16), bytes.fromhex(sys.argv[3])))
