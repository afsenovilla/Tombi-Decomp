"""PlayStation GPU model for the MIPS capture: VRAM (1024 x 512 15 bit words, numpy), the GP0 drawing commands of an ordering table and a
software rasterizer.

Coverage: POLY_F3/F4, POLY_FT3/FT4, POLY_G3/G4, POLY_GT3/GT4 (affine texture, 4/8/15 bpp, CLUT, raw / modulated), LINE_F/G (+ poly
lines), TILE (any size, 1/8/16), SPRT (any size, 8, 16), FILL (0x02), VRAM->VRAM copy (0x80), DR_TPAGE (E1), texture window (E2), DR_AREA
(E3/E4), DR_OFFSET (E5), mask (E6).  Semi transparency modes 0..3 (B/2+F/2, B+F, B-F, B+F/4) with texel bit 15 / per primitive flag.
Not modelled: dithering, the mask bit test, interlace.
"""
import struct

import numpy as np

VW, VH = 1024, 512


class GPU:
    def __init__(self):
        self.vram = np.zeros((VH, VW), dtype=np.uint16)
        self.reset_state()
        self.stats = {}

    def reset_state(self):
        self.tpage = 0
        self.tw = (0, 0, 0, 0)
        self.clip = (0, 0, VW - 1, VH - 1)
        self.ofs = (0, 0)
        self.set_mask = 0
        self.dtd = False

    # ---------------------------------------------------------------- transfers
    def load_image(self, x, y, w, h, data):
        a = np.frombuffer(bytes(data[:w * h * 2]), dtype="<u2").reshape(h, w)
        for r in range(h):
            yy = (y + r) % VH
            for xs in range(0, w, VW):
                pass
            if x + w <= VW:
                self.vram[yy, x:x + w] = a[r]
            else:
                for c in range(w):
                    self.vram[yy, (x + c) % VW] = a[r, c]

    def store_image(self, x, y, w, h):
        out = np.zeros((h, w), dtype=np.uint16)
        for r in range(h):
            yy = (y + r) % VH
            if x + w <= VW:
                out[r] = self.vram[yy, x:x + w]
            else:
                for c in range(w):
                    out[r, c] = self.vram[yy, (x + c) % VW]
        return out.astype("<u2").tobytes()

    def fill(self, x, y, w, h, word):
        x0, y0 = max(0, x), max(0, y)
        x1, y1 = min(VW, x + w), min(VH, y + h)
        if x1 > x0 and y1 > y0:
            self.vram[y0:y1, x0:x1] = word

    def move(self, sx, sy, dx, dy, w, h):
        blk = self.store_image(sx, sy, w, h)
        self.load_image(dx, dy, w, h, blk)

    # ---------------------------------------------------------------- GP0 stream
    def gp0_packet(self, words):
        """Executes one primitive (list of 32 bit words, first = command word)."""
        if not words:
            return
        cmd = words[0] >> 24
        self.stats[cmd] = self.stats.get(cmd, 0) + 1
        if cmd == 0x02:
            c = words[0]
            x, y = words[1] & 0x3F0, (words[1] >> 16) & 0x1FF
            w, h = ((words[2] & 0x3FF) + 15) & ~15, (words[2] >> 16) & 0x1FF
            self.fill(x, y, w, h, rgb15(c))
        elif 0x20 <= cmd < 0x40:
            self._poly(cmd, words)
        elif 0x40 <= cmd < 0x60:
            self._line(cmd, words)
        elif 0x60 <= cmd < 0x80:
            self._rect(cmd, words)
        elif 0x80 <= cmd < 0xA0:
            if len(words) >= 4:
                s, d, wh = words[1], words[2], words[3]
                self.move(s & 0x3FF, (s >> 16) & 0x1FF, d & 0x3FF, (d >> 16) & 0x1FF, (wh & 0xFFFF) or 0x400, (wh >> 16) or 0x200)
        elif 0xA0 <= cmd < 0xC0:
            if len(words) >= 3:
                xy, wh = words[1], words[2]
                w, h = (wh & 0xFFFF) or 0x400, (wh >> 16) or 0x200
                data = struct.pack("<%dI" % (len(words) - 3), *words[3:])
                self.load_image(xy & 0x3FF, (xy >> 16) & 0x1FF, w, h, data + bytes(w * h * 2))
        elif cmd == 0xE1:
            self.tpage = words[0] & 0xFFFF
            self.dtd = bool(words[0] & 0x200)
        elif cmd == 0xE2:
            v = words[0]
            self.tw = (v & 31, (v >> 5) & 31, (v >> 10) & 31, (v >> 15) & 31)
        elif cmd == 0xE3:
            v = words[0]
            self.clip = (v & 0x3FF, (v >> 10) & 0x3FF, self.clip[2], self.clip[3])
        elif cmd == 0xE4:
            v = words[0]
            self.clip = (self.clip[0], self.clip[1], v & 0x3FF, (v >> 10) & 0x3FF)
        elif cmd == 0xE5:
            v = words[0]
            ox, oy = v & 0x7FF, (v >> 11) & 0x7FF
            self.ofs = (ox - 0x800 if ox & 0x400 else ox, oy - 0x800 if oy & 0x400 else oy)
        elif cmd == 0xE6:
            self.set_mask = words[0] & 1

    def run_packets(self, packets):
        for p in packets:
            # a packet can hold several commands (DR_MODE = E1 + E2, DR_ENV ...): split by command length
            i = 0
            while i < len(p):
                n = cmd_len(p, i)
                self.gp0_packet(p[i:i + n])
                i += n

    # ---------------------------------------------------------------- texture sampling
    def _texels(self, u, v, tpage, clut):
        """u, v int arrays -> (15 bit words, opaque mask)."""
        tx = (tpage & 0xF) * 64
        ty = ((tpage >> 4) & 1) * 256
        bpp = (tpage >> 7) & 3
        mx, my, ox, oy = self.tw
        if mx or my:
            u = (u & ~(mx * 8)) | ((ox & mx) * 8)
            v = (v & ~(my * 8)) | ((oy & my) * 8)
        u = u & 0xFF
        v = v & 0xFF
        yy = (ty + v) % VH
        if bpp == 0:
            w = self.vram[yy, (tx + (u >> 2)) % VW]
            idx = (w >> ((u & 3) * 4)) & 0xF
        elif bpp == 1:
            w = self.vram[yy, (tx + (u >> 1)) % VW]
            idx = (w >> ((u & 1) * 8)) & 0xFF
        else:
            t = self.vram[yy, (tx + u) % VW]
            return t, t != 0
        cx, cy = (clut & 0x3F) * 16, (clut >> 6) & 0x1FF
        t = self.vram[cy, (cx + idx.astype(np.int64)) % VW]
        return t, t != 0

    # ---------------------------------------------------------------- pixel output
    def _plot(self, xs, ys, col, semi, tpage, trans_bits=None):
        """xs, ys: int arrays of screen coordinates (already offset); col: (n,3) int arrays of 8 bit colour or 15 bit words."""
        x0, y0, x1, y1 = self.clip
        m = (xs >= x0) & (xs <= x1) & (ys >= y0) & (ys <= y1) & (xs >= 0) & (xs < VW) & (ys >= 0) & (ys < VH)
        if not m.any():
            return
        xs, ys, col = xs[m], ys[m], col[m]
        if trans_bits is not None:
            trans_bits = trans_bits[m]
        if semi:
            mode = (tpage >> 5) & 3
            bw = self.vram[ys, xs]
            b = np.stack([(bw & 31), (bw >> 5) & 31, (bw >> 10) & 31], axis=1).astype(np.int32)
            f = np.stack([(col & 31), (col >> 5) & 31, (col >> 10) & 31], axis=1).astype(np.int32)
            if mode == 0:
                r = (b + f) >> 1
            elif mode == 1:
                r = b + f
            elif mode == 2:
                r = b - f
            else:
                r = b + (f >> 2)
            r = np.clip(r, 0, 31)
            outw = (r[:, 0] | (r[:, 1] << 5) | (r[:, 2] << 10)).astype(np.uint16) | (col & 0x8000)
            if trans_bits is not None:
                outw = np.where(trans_bits, outw, col)
            col = outw
        self.vram[ys, xs] = col.astype(np.uint16) | (0x8000 if self.set_mask else 0)

    # ---------------------------------------------------------------- primitives
    def _rect(self, cmd, w):
        textured = bool(cmd & 4)
        semi = bool(cmd & 2)
        raw = bool(cmd & 1)
        size = (cmd >> 3) & 3
        col = w[0] & 0xFFFFFF
        x, y = sxy(w[1])
        i = 2
        if textured:
            uvc = w[i]
            i += 1
            u0, v0, clut = uvc & 0xFF, (uvc >> 8) & 0xFF, uvc >> 16
        if size == 0:
            wh = w[i]
            ww, hh = wh & 0x3FF, (wh >> 16) & 0x1FF
        else:
            ww = hh = {1: 1, 2: 8, 3: 16}[size]
        if ww <= 0 or hh <= 0:
            return
        x += self.ofs[0]
        y += self.ofs[1]
        gy, gx = np.mgrid[0:hh, 0:ww]
        xs = (x + gx).ravel()
        ys = (y + gy).ravel()
        if not textured:
            c = np.full(xs.shape, rgb15(col), dtype=np.uint16)
            self._plot(xs, ys, c, semi, self.tpage)
            return
        tp = self.tpage
        # rectangles honour the texture flip bits of E1 (bits 12/13) only on some GPUs; ignored
        t, opaque = self._texels((u0 + gx).ravel(), (v0 + gy).ravel(), tp, clut)
        xs, ys, t = xs[opaque], ys[opaque], t[opaque]
        if not raw:
            t = modulate(t, col & 0xFF, (col >> 8) & 0xFF, (col >> 16) & 0xFF)
        self._plot(xs, ys, t, semi, tp, trans_bits=(t & 0x8000) != 0 if semi else None)

    def _poly(self, cmd, w):
        gouraud = bool(cmd & 0x10)
        quad = bool(cmd & 8)
        textured = bool(cmd & 4)
        semi = bool(cmd & 2)
        raw = bool(cmd & 1)
        n = 4 if quad else 3
        verts = []
        i = 0
        col = w[0] & 0xFFFFFF
        clut = tpage = None
        for k in range(n):
            if gouraud and k > 0:
                col = w[i] & 0xFFFFFF
                i += 1
            elif k == 0:
                i = 1
            if i >= len(w):
                return
            x, y = sxy(w[i])
            i += 1
            u = v = 0
            if textured:
                uv = w[i]
                i += 1
                u, v = uv & 0xFF, (uv >> 8) & 0xFF
                if k == 0:
                    clut = uv >> 16
                elif k == 1:
                    tpage = uv >> 16
            verts.append((x + self.ofs[0], y + self.ofs[1], col & 0xFF, (col >> 8) & 0xFF, (col >> 16) & 0xFF, u, v))
        tp = self.tpage
        if textured:
            tp = (self.tpage & ~0x9FF) | (tpage & 0x9FF)
            self.tpage = tp
        tris = [verts[0:3]] + ([verts[1:4]] if quad else [])
        for tri in tris:
            self._tri(tri, textured, semi, raw, gouraud, tp, clut)

    def _tri(self, tri, textured, semi, raw, gouraud, tp, clut):
        (x0, y0, *_), (x1, y1, *_), (x2, y2, *_) = tri
        area = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0)
        if area == 0:
            return
        if max(x0, x1, x2) - min(x0, x1, x2) > 1023 or max(y0, y1, y2) - min(y0, y1, y2) > 511:
            return
        cx0, cy0, cx1, cy1 = self.clip
        bx0, bx1 = max(min(x0, x1, x2), cx0), min(max(x0, x1, x2), cx1)
        by0, by1 = max(min(y0, y1, y2), cy0), min(max(y0, y1, y2), cy1)
        if bx1 < bx0 or by1 < by0:
            return
        gy, gx = np.mgrid[by0:by1 + 1, bx0:bx1 + 1]
        gx = gx.ravel().astype(np.int64)
        gy = gy.ravel().astype(np.int64)
        # edge functions (top-left fill rule approximation: include edges with w >= 0, exclude the right/bottom)
        def edge(ax, ay, bx, by):
            return (bx - ax) * (gy - ay) - (by - ay) * (gx - ax)
        w0 = edge(x1, y1, x2, y2)
        w1 = edge(x2, y2, x0, y0)
        w2 = edge(x0, y0, x1, y1)
        if area < 0:
            w0, w1, w2 = -w0, -w1, -w2
            area = -area
        inside = (w0 >= 0) & (w1 >= 0) & (w2 >= 0)
        # drop the right and bottom edges so neighbouring triangles do not overlap
        inside &= ~((gx == max(x0, x1, x2)) & (w0 * w1 * w2 == 0) & (bx1 != bx0))
        if not inside.any():
            return
        gx, gy, w0, w1, w2 = gx[inside], gy[inside], w0[inside], w1[inside], w2[inside]
        A = float(area)
        l0, l1, l2 = w0 / A, w1 / A, w2 / A
        def interp(k):
            return l0 * tri[0][k] + l1 * tri[1][k] + l2 * tri[2][k]
        if gouraud:
            r, g, b = interp(2).astype(np.int32), interp(3).astype(np.int32), interp(4).astype(np.int32)
        else:
            r = np.full(gx.shape, tri[0][2], np.int32)
            g = np.full(gx.shape, tri[0][3], np.int32)
            b = np.full(gx.shape, tri[0][4], np.int32)
        if textured:
            u = (interp(5) + 0.0001).astype(np.int64)
            v = (interp(6) + 0.0001).astype(np.int64)
            t, opaque = self._texels(u, v, tp, clut)
            if not raw:
                t = modulate_arr(t, r, g, b)
            gx, gy, t = gx[opaque], gy[opaque], t[opaque]
            self._plot(gx, gy, t, semi, tp, trans_bits=(t & 0x8000) != 0 if semi else None)
        else:
            c = ((np.clip(r, 0, 255) >> 3) | ((np.clip(g, 0, 255) >> 3) << 5) | ((np.clip(b, 0, 255) >> 3) << 10)).astype(np.uint16)
            self._plot(gx, gy, c, semi, self.tpage)

    def _line(self, cmd, w):
        gouraud = bool(cmd & 0x10)
        poly = bool(cmd & 8)
        semi = bool(cmd & 2)
        pts = []
        col = w[0] & 0xFFFFFF
        i = 1
        while i < len(w):
            if w[i] & 0xF000F000 == 0x50005000 and poly and len(pts) >= 2:
                break
            x, y = sxy(w[i])
            pts.append((x + self.ofs[0], y + self.ofs[1], col))
            i += 1
            if gouraud and i < len(w):
                if w[i] & 0xF000F000 == 0x50005000 and poly:
                    break
                col = w[i] & 0xFFFFFF
                i += 1
            if not poly and len(pts) == 2:
                break
        for (xa, ya, ca), (xb, yb, cb) in zip(pts, pts[1:]):
            n = max(abs(xb - xa), abs(yb - ya)) + 1
            if n > 2048:
                continue
            t = np.linspace(0.0, 1.0, n)
            xs = np.round(xa + (xb - xa) * t).astype(np.int64)
            ys = np.round(ya + (yb - ya) * t).astype(np.int64)
            if gouraud:
                ch = [((ca >> s) & 0xFF) * (1 - t) + ((cb >> s) & 0xFF) * t for s in (0, 8, 16)]
                c = ((ch[0].astype(int) >> 3) | ((ch[1].astype(int) >> 3) << 5) | ((ch[2].astype(int) >> 3) << 10)).astype(np.uint16)
            else:
                c = np.full(n, rgb15(ca), dtype=np.uint16)
            self._plot(xs, ys, c, semi, self.tpage)

    # ---------------------------------------------------------------- output
    def rgb(self, x, y, w, h):
        """(h, w, 3) uint8 of a VRAM rectangle (15 bit -> 24 bit)."""
        blk = self.vram[y:y + h, x:x + w].astype(np.uint32)
        r = (blk & 31) << 3
        g = ((blk >> 5) & 31) << 3
        b = ((blk >> 10) & 31) << 3
        out = np.stack([r | r >> 5, g | g >> 5, b | b >> 5], axis=2).astype(np.uint8)
        return out


def sxy(w):
    x = w & 0x7FF
    y = (w >> 16) & 0x7FF
    return (x - 0x800 if x & 0x400 else x), (y - 0x800 if y & 0x400 else y)


def rgb15(c):
    return ((c & 0xFF) >> 3) | (((c >> 8) & 0xFF) >> 3) << 5 | (((c >> 16) & 0xFF) >> 3) << 10


def modulate(t, r, g, b):
    return modulate_arr(t, np.int32(r), np.int32(g), np.int32(b))


def modulate_arr(t, r, g, b):
    t = t.astype(np.int32)
    tr, tg, tb = t & 31, (t >> 5) & 31, (t >> 10) & 31
    tr = np.clip((tr * r) >> 7, 0, 31)
    tg = np.clip((tg * g) >> 7, 0, 31)
    tb = np.clip((tb * b) >> 7, 0, 31)
    return (tr | (tg << 5) | (tb << 10) | (t & 0x8000)).astype(np.uint16)


def cmd_len(p, i):
    """Number of words of the GP0 command starting at p[i] (inside one OT packet)."""
    c = p[i] >> 24
    if 0x20 <= c < 0x40:
        n = 4 if c & 8 else 3
        per = 1 + (1 if c & 4 else 0) + (1 if c & 0x10 else 0)
        return 1 + n * per - (1 if c & 0x10 else 0)
    if 0x40 <= c < 0x60:
        if c & 8:
            return len(p) - i       # poly line: rest of packet
        return 3 + (1 if c & 0x10 else 0)
    if 0x60 <= c < 0x80:
        return 2 + (1 if c & 4 else 0) + (1 if (c >> 3) & 3 == 0 else 0)
    if c == 0x02:
        return 3
    if 0x80 <= c < 0xA0:
        return 4
    if 0xA0 <= c < 0xE0:
        return len(p) - i
    return 1
