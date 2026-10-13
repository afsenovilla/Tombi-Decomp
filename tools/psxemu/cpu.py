"""MIPS R3000A interpreter (pure Python) for running Tombi!'s own code.

Speed comes from a basic block compiler: every block (straight line code up to and including a branch and its delay slot) is turned into
Python source once and exec'd into a function `blk(R)` that returns the next PC.  The block remembers the bytes it was compiled from and
is recompiled when RAM under it changed (overlays, lzDecompress'ed code).

Memory map: 2 MB RAM (mirrored in the first 8 MB of KUSEG/KSEG0/KSEG1), 1 KB scratchpad at 0x1F800000, I/O 0x1F801000.. (`io_read` /
`io_write` hooks, reads 0 by default).  Not modelled: load delay slots (compiled code does not rely on them), exceptions, interrupts
(the harness fires "interrupts" itself with `call`), caches.

HLE: `hle[addr] = fn(cpu)` runs instead of the function at addr (fn returns, the CPU continues at RA unless fn sets cpu.pc_override);
calls to the BIOS vectors 0xA0/0xB0/0xC0 go to `bios(cpu, table, num)`.
"""
import struct

from . import gte as _gte

M = 0xFFFFFFFF
RAM = 0x200000
SP_OFF = 0x200000          # scratchpad lives after the RAM in the same bytearray
MEM_SIZE = RAM + 0x400
STOP = 0xFFFFFF00          # return address used by call()

_U32 = struct.Struct("<I")
_U16 = struct.Struct("<H")
_S16 = struct.Struct("<h")
_S8 = struct.Struct("<b")


class EmuError(RuntimeError):
    pass


def s32(v):
    return (v ^ 0x80000000) - 0x80000000


def phys(a):
    """offset into CPU.mem or -1."""
    p = a & 0x1FFFFFFF
    if p < 0x800000:
        return p & 0x1FFFFF
    if 0x1F800000 <= p < 0x1F800400:
        return p - 0x1F600000
    return -1


REGN = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]


class CPU:
    def __init__(self):
        self.mem = bytearray(MEM_SIZE)
        self.R = [0] * 32
        self.hi = 0
        self.lo = 0
        self.pc = 0
        self.cop0 = [0] * 32
        self.gte = _gte.GTE()
        self.hle = {}
        self.bios = None
        self.io_read = None
        self.io_write = None
        self.blocks = {}
        self.icount = 0
        self.pc_override = None
        self.trace = None           # fn(cpu, pc) called at every block start (debug)
        self.unknown_io = {}

    # ------------------------------------------------------------------ memory (slow path, used by HLE and by compiled code off RAM)
    def r8(self, a):
        o = phys(a)
        if o >= 0:
            return self.mem[o]
        return self._io_r(a, 1) & 0xFF

    def r16(self, a):
        o = phys(a)
        if o >= 0:
            return _U16.unpack_from(self.mem, o)[0]
        return self._io_r(a, 2) & 0xFFFF

    def r32(self, a):
        o = phys(a)
        if o >= 0:
            return _U32.unpack_from(self.mem, o)[0]
        return self._io_r(a, 4) & M

    def w8(self, a, v):
        o = phys(a)
        if o >= 0:
            self.mem[o] = v & 0xFF
        else:
            self._io_w(a, v & 0xFF, 1)

    def w16(self, a, v):
        o = phys(a)
        if o >= 0:
            _U16.pack_into(self.mem, o, v & 0xFFFF)
        else:
            self._io_w(a, v & 0xFFFF, 2)

    def w32(self, a, v):
        o = phys(a)
        if o >= 0:
            _U32.pack_into(self.mem, o, v & M)
        else:
            self._io_w(a, v & M, 4)

    def _io_r(self, a, n):
        a &= 0x1FFFFFFF
        if self.io_read:
            v = self.io_read(self, a, n)
            if v is not None:
                return v
        self.unknown_io[a] = self.unknown_io.get(a, 0) + 1
        return 0

    def _io_w(self, a, v, n):
        a &= 0x1FFFFFFF
        if self.io_write and self.io_write(self, a, v, n):
            return
        self.unknown_io[a] = self.unknown_io.get(a, 0) + 1

    def read(self, a, n):
        o = phys(a)
        if o >= 0 and o + n <= MEM_SIZE:
            return bytes(self.mem[o:o + n])
        return bytes(self.r8(a + i) for i in range(n))

    def write(self, a, data):
        o = phys(a)
        if o >= 0 and o + len(data) <= MEM_SIZE:
            self.mem[o:o + len(data)] = data
        else:
            for i, b in enumerate(data):
                self.w8(a + i, b)

    def cstr(self, a, lim=4096):
        out = bytearray()
        while len(out) < lim:
            c = self.r8(a + len(out))
            if c == 0:
                break
            out.append(c)
        return out.decode("latin-1")

    # unaligned (little endian)
    def lwl(self, a, old):
        w = self.r32(a & ~3)
        sh = 8 * (a & 3)
        return (old & (0x00FFFFFF >> sh)) | ((w << (24 - sh)) & M)

    def lwr(self, a, old):
        w = self.r32(a & ~3)
        sh = 8 * (a & 3)
        return (old & ((0xFFFFFF00 << (24 - sh)) & M)) | (w >> sh)

    def swl(self, a, v):
        al = a & ~3
        sh = 8 * (a & 3)
        self.w32(al, (self.r32(al) & ((0xFFFFFF00 << sh) & M)) | (v >> (24 - sh)))

    def swr(self, a, v):
        al = a & ~3
        sh = 8 * (a & 3)
        self.w32(al, (self.r32(al) & (0x00FFFFFF >> (24 - sh))) | ((v << sh) & M))

    # ------------------------------------------------------------------ coprocessors
    def mfc0(self, n):
        return self.cop0[n]

    def mtc0(self, n, v):
        self.cop0[n] = v

    def syscall(self, pc):
        """Psy-Q's EnterCriticalSection (a0=1) / ExitCriticalSection (a0=2): no interrupts here, just succeed."""
        self.R[2] = 1
        return pc + 4

    # ------------------------------------------------------------------ execution
    def run(self, until=STOP, max_instr=None):
        """Runs from self.pc until pc == until (or max_instr executed).  Returns the number of instructions executed."""
        blocks = self.blocks
        mem = self.mem
        hle = self.hle
        R = self.R
        pc = self.pc
        start = self.icount
        limit = None if max_instr is None else start + max_instr
        while pc != until:
            if pc in hle:
                self.pc = pc
                self.pc_override = None
                hle[pc](self)
                R[0] = 0
                pc = self.pc_override if self.pc_override is not None else R[31]
                self.pc_override = None
                self.icount += 10
                continue
            if pc in (0xA0, 0xB0, 0xC0, 0x800000A0, 0x800000B0, 0x800000C0):
                self.pc = pc
                self.pc_override = None
                self.bios(self, pc & 0xFF, R[9])
                R[0] = 0
                pc = self.pc_override if self.pc_override is not None else R[31]
                self.pc_override = None
                continue
            b = blocks.get(pc)
            if b is None or mem[b[1]:b[2]] != b[3]:
                b = self.compile(pc)
                blocks[pc] = b
            if self.trace:
                self.trace(self, pc)
            npc = b[0](R)
            self.icount += b[4]
            if npc is None:
                raise EmuError("block at %08X fell off" % pc)
            pc = npc
            if limit is not None and self.icount >= limit:
                break
            if self.pc_override is not None:      # set by a GTE/syscall/etc helper that wants a jump
                pc = self.pc_override
                self.pc_override = None
        self.pc = pc
        return self.icount - start

    def call(self, addr, *args, max_instr=None, sp=None):
        """Calls addr(args...) natively and returns v0; preserves the caller's registers (an 'interrupt')."""
        saved = (list(self.R), self.hi, self.lo, self.pc)
        R = self.R
        for i, a in enumerate(args[:4]):
            R[4 + i] = a & M
        if sp is not None:
            R[29] = sp
        R[29] = (R[29] - 0x40) & M
        for i, a in enumerate(args[4:]):
            self.w32(R[29] + 16 + 4 * i, a)
        R[31] = STOP
        self.pc = addr
        self.run(STOP, max_instr)
        if self.pc != STOP:
            raise EmuError("call %08X did not return (pc %08X)" % (addr, self.pc))
        v = R[2]
        self.R[:] = saved[0]
        self.hi, self.lo, self.pc = saved[1], saved[2], saved[3]
        return v

    # ------------------------------------------------------------------ block compiler
    def compile(self, pc):
        o = phys(pc)
        if o < 0:
            raise EmuError("execute outside RAM at %08X" % pc)
        lines = []
        n = 0
        a = pc
        while True:
            w = _U32.unpack_from(self.mem, phys(a))[0]
            kind = branch_kind(w)
            if kind:
                dw = _U32.unpack_from(self.mem, phys(a + 4))[0]
                lines += gen_branch(w, a, dw)
                n += 2
                a += 8
                break
            lines += gen(w, a)
            n += 1
            a += 4
            if n >= 64 or (a in self.hle):
                lines.append("return %d" % a)
                break
        src = "def blk(R):\n" + "".join("    " + l + "\n" for l in lines)
        g = dict(GLOBALS)
        g["cpu"] = self
        g["mem"] = self.mem
        g["gte"] = self.gte
        try:
            exec(compile(src, "<blk %08X>" % pc, "exec"), g)
        except SyntaxError:
            raise EmuError("bad block source at %08X:\n%s" % (pc, src))
        end = phys(pc) + 4 * n
        return (g["blk"], phys(pc), end, bytes(self.mem[phys(pc):end]), n, src)


def _rd32(cpu, a):
    return cpu.r32(a)


GLOBALS = {
    "U32": _U32.unpack_from, "U16": _U16.unpack_from, "S16": _S16.unpack_from, "S8": _S8.unpack_from,
    "P32": _U32.pack_into, "P16": _U16.pack_into,
}


def branch_kind(w):
    op = w >> 26
    if op == 0:
        f = w & 0x3F
        return f in (8, 9)
    if op in (1, 2, 3, 4, 5, 6, 7):
        return True
    return False


def _addr(rs, imm):
    imm = imm - 0x10000 if imm & 0x8000 else imm
    if rs == 0:
        return "a=%d" % (imm & M)
    return "a=(R[%d]+%d)&0xFFFFFFFF" % (rs, imm)


# load/store templates: off = offset into mem or slow path
def _load(rs, imm, rt, kind):
    out = [_addr(rs, imm), "p=a&0x1FFFFFFF"]
    fast = {"w": "U32(mem,o)[0]", "hu": "U16(mem,o)[0]", "h": "S16(mem,o)[0]&0xFFFFFFFF", "bu": "mem[o]",
            "b": "S8(mem,o)[0]&0xFFFFFFFF"}[kind]
    slow = {"w": "cpu.r32(a)", "hu": "cpu.r16(a)", "h": "((cpu.r16(a)^0x8000)-0x8000)&0xFFFFFFFF", "bu": "cpu.r8(a)",
            "b": "((cpu.r8(a)^0x80)-0x80)&0xFFFFFFFF"}[kind]
    dst = "R[%d]" % rt if rt else "_"
    out += ["if p<0x800000:", "    o=p&0x1FFFFF; %s=%s" % (dst, fast),
            "elif 0x1F800000<=p<0x1F800400:", "    o=p-0x1F600000; %s=%s" % (dst, fast),
            "else:", "    %s=%s" % (dst, slow)]
    return out


def _store(rs, imm, rt, kind):
    v = "R[%d]" % rt if rt else "0"
    fast = {"w": "P32(mem,o,%s)" % v, "h": "P16(mem,o,%s&0xFFFF)" % v, "b": "mem[o]=%s&0xFF" % v}[kind]
    slow = {"w": "cpu.w32(a,%s)" % v, "h": "cpu.w16(a,%s)" % v, "b": "cpu.w8(a,%s)" % v}[kind]
    return [_addr(rs, imm), "p=a&0x1FFFFFFF",
            "if p<0x800000:", "    o=p&0x1FFFFF; " + fast,
            "elif 0x1F800000<=p<0x1F800400:", "    o=p-0x1F600000; " + fast,
            "else:", "    " + slow]


def gen(w, pc):
    """Python lines for one non-branch instruction."""
    op = w >> 26
    rs = (w >> 21) & 31
    rt = (w >> 16) & 31
    rd = (w >> 11) & 31
    sa = (w >> 6) & 31
    f = w & 0x3F
    imm = w & 0xFFFF
    simm = imm - 0x10000 if imm & 0x8000 else imm
    S = lambda r: "R[%d]" % r
    SS = lambda r: "((R[%d]^0x80000000)-0x80000000)" % r
    if w == 0:
        return []
    if op == 0:
        if f in (0x0C,):          # syscall
            return ["cpu.pc_override=cpu.syscall(%d)" % pc, "return %d" % (pc + 4)]
        if f == 0x0D:
            return ["raise RuntimeError('break at %08X')" % pc]
        if f == 0x10:
            return ["R[%d]=cpu.hi" % rd] if rd else []
        if f == 0x12:
            return ["R[%d]=cpu.lo" % rd] if rd else []
        if f == 0x11:
            return ["cpu.hi=R[%d]" % rs]
        if f == 0x13:
            return ["cpu.lo=R[%d]" % rs]
        if f == 0x18:
            return ["t=%s*%s" % (SS(rs), SS(rt)), "cpu.lo=t&0xFFFFFFFF", "cpu.hi=(t>>32)&0xFFFFFFFF"]
        if f == 0x19:
            return ["t=R[%d]*R[%d]" % (rs, rt), "cpu.lo=t&0xFFFFFFFF", "cpu.hi=(t>>32)&0xFFFFFFFF"]
        if f == 0x1A:
            return ["n=%s; d=%s" % (SS(rs), SS(rt)),
                    "if d==0:", "    cpu.hi=n&0xFFFFFFFF; cpu.lo=1 if n<0 else 0xFFFFFFFF",
                    "elif n==-0x80000000 and d==-1:", "    cpu.lo=0x80000000; cpu.hi=0",
                    "else:", "    q=abs(n)//abs(d)", "    q=-q if (n<0)!=(d<0) else q", "    cpu.lo=q&0xFFFFFFFF; cpu.hi=(n-q*d)&0xFFFFFFFF"]
        if f == 0x1B:
            return ["n=R[%d]; d=R[%d]" % (rs, rt),
                    "if d==0:", "    cpu.hi=n; cpu.lo=0xFFFFFFFF",
                    "else:", "    cpu.lo=n//d; cpu.hi=n%d"]
        if rd == 0:
            return []
        D = "R[%d]" % rd
        if f == 0x00:
            return ["%s=(R[%d]<<%d)&0xFFFFFFFF" % (D, rt, sa)]
        if f == 0x02:
            return ["%s=R[%d]>>%d" % (D, rt, sa)]
        if f == 0x03:
            return ["%s=(%s>>%d)&0xFFFFFFFF" % (D, SS(rt), sa)]
        if f == 0x04:
            return ["%s=(R[%d]<<(R[%d]&31))&0xFFFFFFFF" % (D, rt, rs)]
        if f == 0x06:
            return ["%s=R[%d]>>(R[%d]&31)" % (D, rt, rs)]
        if f == 0x07:
            return ["%s=(%s>>(R[%d]&31))&0xFFFFFFFF" % (D, SS(rt), rs)]
        if f in (0x20, 0x21):
            return ["%s=(R[%d]+R[%d])&0xFFFFFFFF" % (D, rs, rt)]
        if f in (0x22, 0x23):
            return ["%s=(R[%d]-R[%d])&0xFFFFFFFF" % (D, rs, rt)]
        if f == 0x24:
            return ["%s=R[%d]&R[%d]" % (D, rs, rt)]
        if f == 0x25:
            return ["%s=R[%d]|R[%d]" % (D, rs, rt)]
        if f == 0x26:
            return ["%s=R[%d]^R[%d]" % (D, rs, rt)]
        if f == 0x27:
            return ["%s=(R[%d]|R[%d])^0xFFFFFFFF" % (D, rs, rt)]
        if f == 0x2A:
            return ["%s=1 if %s<%s else 0" % (D, SS(rs), SS(rt))]
        if f == 0x2B:
            return ["%s=1 if R[%d]<R[%d] else 0" % (D, rs, rt)]
        raise EmuError("unknown SPECIAL %08X at %08X" % (w, pc))
    if op == 0x0F:
        return ["R[%d]=%d" % (rt, (imm << 16) & M)] if rt else []
    if op in (0x08, 0x09):
        return ["R[%d]=(R[%d]+%d)&0xFFFFFFFF" % (rt, rs, simm)] if rt else []
    if op == 0x0A:
        return ["R[%d]=1 if %s<%d else 0" % (rt, SS(rs), simm)] if rt else []
    if op == 0x0B:
        return ["R[%d]=1 if R[%d]<%d else 0" % (rt, rs, simm & M)] if rt else []
    if op == 0x0C:
        return ["R[%d]=R[%d]&%d" % (rt, rs, imm)] if rt else []
    if op == 0x0D:
        return ["R[%d]=R[%d]|%d" % (rt, rs, imm)] if rt else []
    if op == 0x0E:
        return ["R[%d]=R[%d]^%d" % (rt, rs, imm)] if rt else []
    if op == 0x23:
        return _load(rs, imm, rt, "w")
    if op == 0x21:
        return _load(rs, imm, rt, "h")
    if op == 0x25:
        return _load(rs, imm, rt, "hu")
    if op == 0x20:
        return _load(rs, imm, rt, "b")
    if op == 0x24:
        return _load(rs, imm, rt, "bu")
    if op == 0x2B:
        return _store(rs, imm, rt, "w")
    if op == 0x29:
        return _store(rs, imm, rt, "h")
    if op == 0x28:
        return _store(rs, imm, rt, "b")
    if op == 0x22:
        return [_addr(rs, imm)] + (["R[%d]=cpu.lwl(a,R[%d])" % (rt, rt)] if rt else [])
    if op == 0x26:
        return [_addr(rs, imm)] + (["R[%d]=cpu.lwr(a,R[%d])" % (rt, rt)] if rt else [])
    if op == 0x2A:
        return [_addr(rs, imm), "cpu.swl(a,R[%d])" % rt]
    if op == 0x2E:
        return [_addr(rs, imm), "cpu.swr(a,R[%d])" % rt]
    if op == 0x10:          # COP0
        if rs == 0:
            return ["R[%d]=cpu.mfc0(%d)" % (rt, rd)] if rt else []
        if rs == 4:
            return ["cpu.mtc0(%d,R[%d])" % (rd, rt)]
        if rs == 0x10 and f == 0x10:
            return []         # rfe
        raise EmuError("unknown COP0 %08X at %08X" % (w, pc))
    if op == 0x12:          # COP2 (GTE)
        if rs & 0x10:
            return ["gte.command(%d)" % (w & 0x1FFFFFF)]
        if rs == 0:
            return ["R[%d]=gte.read_data(%d)" % (rt, rd)] if rt else ["gte.read_data(%d)" % rd]
        if rs == 2:
            return ["R[%d]=gte.read_ctrl(%d)" % (rt, rd)] if rt else []
        if rs == 4:
            return ["gte.write_data(%d,R[%d])" % (rd, rt)]
        if rs == 6:
            return ["gte.write_ctrl(%d,R[%d])" % (rd, rt)]
        raise EmuError("unknown COP2 %08X at %08X" % (w, pc))
    if op == 0x32:          # lwc2
        return [_addr(rs, imm), "gte.write_data(%d,cpu.r32(a))" % rt]
    if op == 0x3A:          # swc2
        return [_addr(rs, imm), "cpu.w32(a,gte.read_data(%d))" % rt]
    if op in (0x11, 0x13, 0x31, 0x33, 0x39, 0x3B):
        return []           # other coprocessors: absent
    raise EmuError("unknown opcode %08X at %08X" % (w, pc))


def gen_branch(w, pc, dw):
    op = w >> 26
    rs = (w >> 21) & 31
    rt = (w >> 16) & 31
    rd = (w >> 11) & 31
    imm = w & 0xFFFF
    simm = imm - 0x10000 if imm & 0x8000 else imm
    tgt = (pc + 4 + (simm << 2)) & M
    nxt = pc + 8
    SS = lambda r: "((R[%d]^0x80000000)-0x80000000)" % r
    if branch_kind(dw):
        raise EmuError("branch in delay slot at %08X" % pc)
    ds = gen(dw, pc + 4)
    if op == 0 and (w & 0x3F) == 8:              # jr
        return ["t=R[%d]" % rs] + ds + ["return t"]
    if op == 0 and (w & 0x3F) == 9:              # jalr
        out = ["t=R[%d]" % rs]
        if rd:
            out.append("R[%d]=%d" % (rd, nxt))
        return out + ds + ["return t"]
    if op == 2:
        return ds + ["return %d" % (((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2))]
    if op == 3:
        return ["R[31]=%d" % nxt] + ds + ["return %d" % (((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2))]
    if op == 4:
        c = "R[%d]==R[%d]" % (rs, rt)
    elif op == 5:
        c = "R[%d]!=R[%d]" % (rs, rt)
    elif op == 6:
        c = "%s<=0" % SS(rs)
    elif op == 7:
        c = "%s>0" % SS(rs)
    elif op == 1:
        if rt & 1:
            c = "%s>=0" % SS(rs)
        else:
            c = "%s<0" % SS(rs)
        out = ["c=%s" % c]
        if rt & 0x10:     # bltzal / bgezal
            out.append("R[31]=%d" % nxt)
        return out + ds + ["return %d if c else %d" % (tgt, nxt)]
    else:
        raise EmuError("bad branch %08X" % w)
    return ["c=%s" % c] + ds + ["return %d if c else %d" % (tgt, nxt)]
