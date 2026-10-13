"""Geometry Transformation Engine (COP2) of the R3000A, after the public psx-spx description.

Registers: data 0..31 (VXY0..LZCR), control 32..63 (RT, TR, LLM, BK, LCM, FC, OFX, OFY, H, DQA, DQB, ZSF3, ZSF4, FLAG).  Commands:
RTPS RTPT NCLIP OP DPCS INTPL MVMVA NCDS CDP NCDT NCCS CC NCS NCT SQR DCPL DPCT AVSZ3 AVSZ4 GPF GPL NCCT.  The FLAG bits are set as on
the hardware for the saturations that matter to callers (IR, MAC, SZ, SXY, divide overflow); the error summary bit 31 is kept.
"""
M = 0xFFFFFFFF


def s16(v):
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def s32(v):
    v &= M
    return v - 0x100000000 if v & 0x80000000 else v


_UNR = [max(0, (0x40000 // (i + 0x100) + 1) // 2 - 0x101) for i in range(0x101)]


def _divide(h, sz3):
    if h < sz3 * 2:
        z = 0
        while z < 16 and not (sz3 << z) & 0x8000:
            z += 1
        n = h << z
        d = sz3 << z
        u = _UNR[(d - 0x7FC0) >> 7] + 0x101
        d = (0x2000080 - d * u) >> 8
        d = (0x0000080 + d * u) >> 8
        n = min(0x1FFFF, (n * d + 0x8000) >> 16)
        return n, False
    return 0x1FFFF, True


class GTE:
    def __init__(self):
        self.d = [0] * 32
        self.c = [0] * 32
        self.flag = 0

    # ---------------------------------------------------------------- register access
    def read_data(self, r):
        d = self.d
        if r in (1, 3, 5, 8, 9, 10, 11):
            return s16(d[r]) & M
        if r in (7, 16, 17, 18, 19):
            return d[r] & 0xFFFF
        if r == 15:
            return d[14]
        if r in (28, 29):
            def c(v):
                return max(0, min(0x1F, s16(v) >> 7))
            return c(d[9]) | c(d[10]) << 5 | c(d[11]) << 10
        return d[r] & M

    def write_data(self, r, v):
        v &= M
        d = self.d
        if r == 15:
            d[12], d[13], d[14] = d[13], d[14], v
            return
        if r == 28:
            d[28] = v & 0x7FFF
            d[9] = (v & 0x1F) << 7
            d[10] = ((v >> 5) & 0x1F) << 7
            d[11] = ((v >> 10) & 0x1F) << 7
            return
        if r == 29:
            return
        if r == 30:
            d[30] = v
            x = v if not v & 0x80000000 else (~v & M)
            n = 0
            while n < 32 and not x & (0x80000000 >> n):
                n += 1
            d[31] = n
            return
        if r == 31:
            return
        d[r] = v

    def read_ctrl(self, r):
        c = self.c
        if r in (26, 27, 29, 30):
            if r == 26:
                return c[26] & 0xFFFF       # H reads sign extended on hw, but unsigned use is common; keep 16 bits
            return s16(c[r]) & M
        if r == 31:
            f = self.flag & 0x7FFFF000
            if f & 0x7F87E000:
                f |= 0x80000000
            return f
        if r in (4, 12, 20):
            return s16(c[r]) & M
        return c[r] & M

    def write_ctrl(self, r, v):
        v &= M
        if r == 31:
            self.flag = v & 0x7FFFF000
            return
        self.c[r] = v

    # ---------------------------------------------------------------- helpers
    def _mat(self, base):
        c = self.c
        m = [s16(c[base]), s16(c[base] >> 16), s16(c[base + 1]), s16(c[base + 1] >> 16), s16(c[base + 2]), s16(c[base + 2] >> 16),
             s16(c[base + 3]), s16(c[base + 3] >> 16), s16(c[base + 4])]
        return [m[0:3], m[3:6], m[6:9]]

    def _vec(self, n):
        d = self.d
        if n == 3:
            return [s16(d[9]), s16(d[10]), s16(d[11])]
        return [s16(d[n * 2]), s16(d[n * 2] >> 16), s16(d[n * 2 + 1])]

    def _mac(self, i, v):
        if v > 0x7FFFFFFFFFF:
            self.flag |= 1 << (31 - i)
        elif v < -0x80000000000:
            self.flag |= 1 << (28 - i)
        return v

    def _ir(self, i, v, lm):
        lo = 0 if lm else -0x8000
        if i == 0:
            if v < 0:
                self.flag |= 1 << 12
                return 0
            if v > 0x1000:
                self.flag |= 1 << 12
                return 0x1000
            return v
        if v < lo:
            self.flag |= 1 << (25 - i)
            return lo
        if v > 0x7FFF:
            self.flag |= 1 << (25 - i)
            return 0x7FFF
        return v

    def _set_mac123(self, v, sf, lm):
        d = self.d
        for i in range(3):
            m = self._mac(i + 1, v[i]) >> sf
            d[25 + i] = m & M
            d[9 + i] = self._ir(i + 1, s32(m), lm) & 0xFFFF
        return v

    def _push_color(self):
        d = self.d
        cols = []
        for i in range(3):
            v = s32(d[25 + i]) >> 4
            if v < 0:
                self.flag |= 1 << (21 - i)
                v = 0
            elif v > 255:
                self.flag |= 1 << (21 - i)
                v = 255
            cols.append(v)
        code = d[6] >> 24
        d[20], d[21] = d[21], d[22]
        d[22] = cols[0] | cols[1] << 8 | cols[2] << 16 | code << 24

    def _push_z(self, v):
        d = self.d
        if v < 0:
            self.flag |= 1 << 18
            v = 0
        elif v > 0xFFFF:
            self.flag |= 1 << 18
            v = 0xFFFF
        d[16], d[17], d[18], d[19] = d[17], d[18], d[19], v

    def _rtp(self, n, sf, lm, last):
        d, c = self.d, self.c
        rt = self._mat(0)
        tr = [s32(c[5]), s32(c[6]), s32(c[7])]
        v = self._vec(n)
        mac = [(tr[i] << 12) + rt[i][0] * v[0] + rt[i][1] * v[1] + rt[i][2] * v[2] for i in range(3)]
        for i in range(3):
            self._mac(i + 1, mac[i])
        m = [x >> (12 * sf) for x in mac]
        for i in range(3):
            d[25 + i] = m[i] & M
        d[9] = self._ir(1, m[0], lm) & 0xFFFF
        d[10] = self._ir(2, m[1], lm) & 0xFFFF
        # IR3 saturation flag uses MAC3 >> 12 regardless of sf
        z = mac[2] >> 12
        ir3 = max(-0x8000 if not lm else 0, min(0x7FFF, m[2]))
        if z < -0x8000 or z > 0x7FFF:
            self.flag |= 1 << 22
        d[11] = ir3 & 0xFFFF
        self._push_z(z)
        h = c[26] & 0xFFFF
        q, ov = _divide(h, d[19])
        if ov:
            self.flag |= 1 << 17
        ofx, ofy = s32(c[24]), s32(c[25])
        sx = (q * s16(d[9]) + ofx) >> 16
        sy = (q * s16(d[10]) + ofy) >> 16
        if sx < -0x400 or sx > 0x3FF:
            self.flag |= 1 << 14
            sx = max(-0x400, min(0x3FF, sx))
        if sy < -0x400 or sy > 0x3FF:
            self.flag |= 1 << 13
            sy = max(-0x400, min(0x3FF, sy))
        d[12], d[13] = d[13], d[14]
        d[14] = (sx & 0xFFFF) | (sy & 0xFFFF) << 16
        if last:
            dqa, dqb = s16(c[27]), s32(c[28])
            p = dqb + dqa * q
            self._mac(0, p)
            d[24] = p & M
            d[8] = self._ir(0, p >> 12, True)

    def _ncd(self, n, sf, lm, mode):
        """mode: 'ncs' (light + bk), 'ncc' (* rgb), 'ncd' (* rgb + depth cue)."""
        d, c = self.d, self.c
        v = self._vec(n)
        llm = self._mat(8)
        lcm = self._mat(16)
        sh = 12 * sf
        self._set_mac123([llm[i][0] * v[0] + llm[i][1] * v[1] + llm[i][2] * v[2] for i in range(3)], sh, lm)
        ir = [s16(d[9]), s16(d[10]), s16(d[11])]
        bk = [s32(c[13]), s32(c[14]), s32(c[15])]
        self._set_mac123([(bk[i] << 12) + lcm[i][0] * ir[0] + lcm[i][1] * ir[1] + lcm[i][2] * ir[2] for i in range(3)], sh, lm)
        if mode != "ncs":
            self._color_mul(sf, lm, mode == "ncd")
        self._push_color()

    def _color_mul(self, sf, lm, depth):
        d, c = self.d, self.c
        rgb = [d[6] & 0xFF, (d[6] >> 8) & 0xFF, (d[6] >> 16) & 0xFF]
        ir = [s16(d[9]), s16(d[10]), s16(d[11])]
        prod = [(rgb[i] * ir[i]) << 4 for i in range(3)]
        if depth:
            fc = [s32(c[21]), s32(c[22]), s32(c[23])]
            tmp = []
            for i in range(3):
                t = self._mac(i + 1, (fc[i] << 12) - prod[i]) >> (12 * sf)
                tmp.append(self._ir(i + 1, t, False))
            ir0 = s16(d[8])
            prod = [prod[i] + ir0 * tmp[i] for i in range(3)]
        self._set_mac123(prod, 12 * sf, lm)

    # ---------------------------------------------------------------- commands
    def command(self, cmd):
        d, c = self.d, self.c
        self.flag = 0
        op = cmd & 0x3F
        sf = (cmd >> 19) & 1
        lm = bool((cmd >> 10) & 1)
        sh = 12 * sf
        if op == 0x01:        # RTPS
            self._rtp(0, sf, lm, True)
        elif op == 0x30:      # RTPT
            for i in range(3):
                self._rtp(i, sf, lm, i == 2)
        elif op == 0x06:      # NCLIP
            x0, y0 = s16(d[12]), s16(d[12] >> 16)
            x1, y1 = s16(d[13]), s16(d[13] >> 16)
            x2, y2 = s16(d[14]), s16(d[14] >> 16)
            v = x0 * y1 + x1 * y2 + x2 * y0 - x0 * y2 - x1 * y0 - x2 * y1
            self._mac(0, v)
            d[24] = v & M
        elif op == 0x2D:      # AVSZ3
            v = s16(c[29]) * (d[17] + d[18] + d[19])
            self._mac(0, v)
            d[24] = v & M
            self._otz(v >> 12)
        elif op == 0x2E:      # AVSZ4
            v = s16(c[30]) * (d[16] + d[17] + d[18] + d[19])
            self._mac(0, v)
            d[24] = v & M
            self._otz(v >> 12)
        elif op == 0x12:      # MVMVA
            mx = (cmd >> 17) & 3
            vx = (cmd >> 15) & 3
            tx = (cmd >> 13) & 3
            m = self._mat([0, 8, 16][mx]) if mx < 3 else [[-(d[6] & 0xFF) << 4, (d[6] & 0xFF) << 4, s16(d[8])],
                                                         [s16(c[1] >> 16)] * 3, [s16(c[2])] * 3]
            v = self._vec(vx)
            t = [[s32(c[5]), s32(c[6]), s32(c[7])], [s32(c[13]), s32(c[14]), s32(c[15])],
                 [s32(c[21]), s32(c[22]), s32(c[23])], [0, 0, 0]][tx]
            if tx == 2:       # FC bug: only the last two columns count, the first saturates
                res = []
                for i in range(3):
                    self._ir(i + 1, ((t[i] << 12) + m[i][0] * v[0]) >> sh, False)
                    res.append(m[i][1] * v[1] + m[i][2] * v[2])
                self._set_mac123(res, sh, lm)
            else:
                self._set_mac123([(t[i] << 12) + m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2] for i in range(3)], sh, lm)
        elif op == 0x0C:      # OP
            rt = self._mat(0)
            dd = [rt[0][0], rt[1][1], rt[2][2]]
            ir = [s16(d[9]), s16(d[10]), s16(d[11])]
            self._set_mac123([ir[2] * dd[1] - ir[1] * dd[2], ir[0] * dd[2] - ir[2] * dd[0], ir[1] * dd[0] - ir[0] * dd[1]], sh, lm)
        elif op == 0x28:      # SQR
            ir = [s16(d[9]), s16(d[10]), s16(d[11])]
            self._set_mac123([x * x for x in ir], sh, lm)
        elif op == 0x13:      # NCDS
            self._ncd(0, sf, lm, "ncd")
        elif op == 0x16:      # NCDT
            for i in range(3):
                self._ncd(i, sf, lm, "ncd")
        elif op == 0x1B:      # NCCS
            self._ncd(0, sf, lm, "ncc")
        elif op == 0x3F:      # NCCT
            for i in range(3):
                self._ncd(i, sf, lm, "ncc")
        elif op == 0x1E:      # NCS
            self._ncd(0, sf, lm, "ncs")
        elif op == 0x20:      # NCT
            for i in range(3):
                self._ncd(i, sf, lm, "ncs")
        elif op in (0x1C, 0x14):   # CC / CDP
            lcm = self._mat(16)
            ir = [s16(d[9]), s16(d[10]), s16(d[11])]
            bk = [s32(c[13]), s32(c[14]), s32(c[15])]
            self._set_mac123([(bk[i] << 12) + lcm[i][0] * ir[0] + lcm[i][1] * ir[1] + lcm[i][2] * ir[2] for i in range(3)], sh, lm)
            self._color_mul(sf, lm, op == 0x14)
            self._push_color()
        elif op in (0x10, 0x2A, 0x29, 0x11):   # DPCS, DPCT, DCPL, INTPL
            n = 3 if op == 0x2A else 1
            for k in range(n):
                if op == 0x10:
                    src = d[6]
                elif op == 0x2A:
                    src = d[20]
                else:
                    src = None
                if src is not None:
                    base = [(src & 0xFF) << 16, ((src >> 8) & 0xFF) << 16, ((src >> 16) & 0xFF) << 16]
                elif op == 0x29:
                    rgb = [d[6] & 0xFF, (d[6] >> 8) & 0xFF, (d[6] >> 16) & 0xFF]
                    base = [(rgb[i] * s16(d[9 + i])) << 4 for i in range(3)]
                else:
                    base = [s16(d[9 + i]) << 12 for i in range(3)]
                fc = [s32(c[21]), s32(c[22]), s32(c[23])]
                ir0 = s16(d[8])
                res = []
                for i in range(3):
                    t = self._ir(i + 1, self._mac(i + 1, (fc[i] << 12) - base[i]) >> sh, False)
                    res.append(base[i] + ir0 * t)
                self._set_mac123(res, sh, lm)
                self._push_color()
        elif op == 0x3D:      # GPF
            ir0 = s16(d[8])
            self._set_mac123([ir0 * s16(d[9 + i]) for i in range(3)], sh, lm)
            self._push_color()
        elif op == 0x3E:      # GPL
            ir0 = s16(d[8])
            self._set_mac123([(s32(d[25 + i]) << sh) + ir0 * s16(d[9 + i]) for i in range(3)], sh, lm)
            self._push_color()
        # unknown commands: ignored

    def _otz(self, v):
        if v < 0:
            self.flag |= 1 << 18
            v = 0
        elif v > 0xFFFF:
            self.flag |= 1 << 18
            v = 0xFFFF
        self.d[7] = v
