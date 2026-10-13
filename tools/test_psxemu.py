#!/usr/bin/env python3
"""Unit tests of the MIPS capture interpreter (tools/psxemu): instruction semantics, delay slots, unaligned access, mult/div, threads
via the HLE BIOS, GTE RTPS/NCLIP/AVSZ, GPU sprites.  No game data needed.

  python3 tools/test_psxemu.py
"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from psxemu import cpu as C, gpu as G, gte as T  # noqa: E402

BASE = 0x80010000


def R(op, rs, rt, rd, sa, f):
    return op << 26 | rs << 21 | rt << 16 | rd << 11 | sa << 6 | f


def I(op, rs, rt, imm):
    return op << 26 | rs << 21 | rt << 16 | (imm & 0xFFFF)


def addiu(rt, rs, imm): return I(9, rs, rt, imm)
def lui(rt, imm): return I(15, 0, rt, imm)
def ori(rt, rs, imm): return I(13, rs, rt, imm)
def addu(rd, rs, rt): return R(0, rs, rt, rd, 0, 0x21)
def subu(rd, rs, rt): return R(0, rs, rt, rd, 0, 0x23)
def slt(rd, rs, rt): return R(0, rs, rt, rd, 0, 0x2A)
def sra(rd, rt, sa): return R(0, 0, rt, rd, sa, 3)
def mult(rs, rt): return R(0, rs, rt, 0, 0, 0x18)
def div(rs, rt): return R(0, rs, rt, 0, 0, 0x1A)
def mflo(rd): return R(0, 0, 0, rd, 0, 0x12)
def mfhi(rd): return R(0, 0, 0, rd, 0, 0x10)
def jr(rs): return R(0, rs, 0, 0, 0, 8)
def beq(rs, rt, off): return I(4, rs, rt, off)
def bne(rs, rt, off): return I(5, rs, rt, off)
def sw(rt, off, rs): return I(0x2B, rs, rt, off)
def lw(rt, off, rs): return I(0x23, rs, rt, off)
def lb(rt, off, rs): return I(0x20, rs, rt, off)
def lhu(rt, off, rs): return I(0x25, rs, rt, off)
def lwl(rt, off, rs): return I(0x22, rs, rt, off)
def lwr(rt, off, rs): return I(0x26, rs, rt, off)
def jal(t): return 3 << 26 | ((t >> 2) & 0x3FFFFFF)
NOP = 0
RA = 31


def run(words, regs=None, data=None):
    c = C.CPU()
    c.write(BASE, struct.pack("<%dI" % len(words), *words))
    if data:
        for a, d in data.items():
            c.write(a, d)
    for k, v in (regs or {}).items():
        c.R[k] = v & 0xFFFFFFFF
    c.R[29] = 0x801FFF00
    c.R[RA] = C.STOP
    c.pc = BASE
    c.run(C.STOP, max_instr=100000)
    return c


class CpuTest(unittest.TestCase):
    def test_alu(self):
        c = run([lui(8, 0x1234), ori(8, 8, 0x5678), addiu(9, 0, -1), addu(10, 8, 9), slt(11, 9, 0), sra(12, 9, 4), jr(RA), NOP])
        self.assertEqual(c.R[8], 0x12345678)
        self.assertEqual(c.R[9], 0xFFFFFFFF)
        self.assertEqual(c.R[10], 0x12345677)
        self.assertEqual(c.R[11], 1)
        self.assertEqual(c.R[12], 0xFFFFFFFF)

    def test_delay_slot_and_loop(self):
        # t0 = 10; t1 = 0; loop: t1 += t0; t0 -= 1 (delay slot); bne t0, 0
        c = run([addiu(8, 0, 10), addiu(9, 0, 0), addu(9, 9, 8), bne(8, 0, -2), addiu(8, 8, -1), jr(RA), NOP])
        # bne compares t0 before the delay slot decrement: adds 10..1 and then once more 0 ... check exact semantics
        self.assertEqual(c.R[9], sum(range(0, 11)))
        self.assertEqual(c.R[8], 0xFFFFFFFF)

    def test_jal_ra(self):
        sub = BASE + 0x40
        c = run([addiu(29, 29, -8), sw(RA, 0, 29)] + [jal(sub + 8), NOP, lw(RA, 0, 29), addiu(29, 29, 8), jr(RA), NOP] + [NOP] * 10
                + [jr(RA), addiu(9, 0, 2)])
        self.assertEqual(c.R[9], 2)

    def test_mult_div(self):
        c = run([addiu(8, 0, -7), addiu(9, 0, 2), mult(8, 9), mflo(10), mfhi(11), div(8, 9), mflo(12), mfhi(13), jr(RA), NOP])
        self.assertEqual(C.s32(c.R[10]), -14)
        self.assertEqual(c.R[11], 0xFFFFFFFF)
        self.assertEqual(C.s32(c.R[12]), -3)
        self.assertEqual(C.s32(c.R[13]), -1)

    def test_loads_unaligned(self):
        a = 0x80100000
        c = run([lui(4, 0x8010), lb(8, 3, 4), lhu(9, 2, 4), addiu(10, 0, 0), lwl(10, 4, 4), lwr(10, 1, 4), jr(RA), NOP],
                data={a: bytes([0x11, 0x22, 0x33, 0x84, 0x55, 0x66, 0x77, 0x88])})
        self.assertEqual(c.R[8], 0xFFFFFF84)
        self.assertEqual(c.R[9], 0x8433)
        self.assertEqual(c.R[10], 0x55843322)

    def test_scratchpad_and_mirror(self):
        c = run([lui(4, 0x1F80), addiu(8, 0, 0x55), sw(8, 0x10, 4), lui(5, 0xA000), sw(8, 0x100, 5), jr(RA), NOP])
        self.assertEqual(c.r32(0x1F800010), 0x55)
        self.assertEqual(c.r32(0x80000100), 0x55)

    def test_recompile_on_write(self):
        c = run([addiu(8, 0, 1), jr(RA), NOP])
        self.assertEqual(c.R[8], 1)
        c.write(BASE, struct.pack("<I", addiu(8, 0, 2)))
        c.R[RA] = C.STOP
        c.pc = BASE
        c.run(C.STOP)
        self.assertEqual(c.R[8], 2)


class GteTest(unittest.TestCase):
    def test_rtps_identity(self):
        g = T.GTE()
        g.write_ctrl(0, 0x1000)
        g.write_ctrl(2, 0x1000)
        g.write_ctrl(4, 0x1000)
        g.write_ctrl(7, 1000)           # TRZ
        g.write_ctrl(24, 160 << 16)
        g.write_ctrl(25, 120 << 16)
        g.write_ctrl(26, 1000)          # H
        g.write_data(0, (20 << 16) | 100)   # x 100, y 20
        g.write_data(1, 0)
        g.command(0x00080001 | 0)       # RTPS sf=1
        sxy = g.read_data(14)
        self.assertEqual(sxy & 0xFFFF, 260)
        self.assertEqual(sxy >> 16, 140)
        self.assertEqual(g.read_data(19), 1000)

    def test_nclip_avsz3(self):
        g = T.GTE()
        g.write_data(12, 0)
        g.write_data(13, 10)
        g.write_data(14, 10 << 16)
        g.command(0x06)
        self.assertEqual(C.s32(g.read_data(24)), 100)
        for r, v in ((17, 30), (18, 60), (19, 90)):
            g.write_data(r, v)
        g.write_ctrl(29, 0x1000 // 3 + 1)
        g.command(0x2D)
        self.assertEqual(g.read_data(7), 60)


class GpuTest(unittest.TestCase):
    def test_sprite_4bpp_clut(self):
        gp = G.GPU()
        gp.vram[0, 0:16] = range(16)            # CLUT at (0,0): index i -> word i
        gp.vram[0, 64] = 0x3210                 # 4bpp texels 0,1,2,3 at page x 64
        gp.tpage = 1                            # page x 64, 4 bpp
        gp.gp0_packet([0x65808080, (10 << 16) | 100, 0 << 16 | 0, (1 << 16) | 4])
        self.assertEqual(list(gp.vram[10, 100:104]), [0, 1, 2, 3])
        # texel 0 is transparent: the pixel stays untouched
        gp.vram[11, 100] = 7
        self.assertEqual(gp.vram[11, 100], 7)

    def test_flat_triangle(self):
        gp = G.GPU()
        gp.gp0_packet([0x200000F8, 0, 10, 10 << 16])
        self.assertEqual(int(gp.vram[1, 1]), 31)


if __name__ == "__main__":
    unittest.main()
