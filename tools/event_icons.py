#!/usr/bin/env python3
"""Sub-screen icon pages of Tomba!/Tombi!: the event icons (the community "item icon" sheet) and the inventory item icons.
Reads only; nothing from the discs belongs in the repo.  See docs/CUT_CONTENT.md, "Event and item icons".

Event icons (src/d0spsub/subScreenDrawEventIcons.c, subScreenDrawSprite.c):
  icon of event k = SPR frame  *ICONS[k]  (ICONS = D_800FE974 in PAL D0SPSUB: u32 pointers to {u16 frame, 0, 0, 0x803C}
  records), frame k of the sub-screen sprite table (spad SPR_DATA[9], load kind 0x39: PAL AREAnn/A1nn.GAM, 7900 bytes,
  identical in every area; USA/JP/beta: inside AREAnn/A003.GAM; SLPM-80165: AREA00/A006.GAM; PrePre8: AREA00/DATA000.GAM).
  A frame is {u, v, clut, .., w, h, .., dx, dy} parts of 16 bytes; every event icon is one 24 x 16 part of the 4 bpp page
  at VRAM (256, 0) (PAL SYSTEM/A00300.000 words 128..191).  The CLUT word in the part names a 16 colour row of the
  48 x 31 block D0SPSUB uploads to (288, 480) (PAL file offset 0x98B8; other lists load SYSTEM/DSPCLUT.TIM there),
  except events 0x56/0x57/0x58/0x74/0x97/0x9D/0x9F/0xA4 whose CLUT is forced by the switch in subScreenDrawEventIcons.
  The list shows only events in SUBSCREEN_EVENT_ORDER (value = event + 1), and only once SCRIPT_FLAGS[event] == 0xFF.
Item icons (src/d0spsub/subScreenDrawItemIcon.c): item n = 16 x 16 cell (n % 15 * 16 + 8, n / 15 * 16 + 8) of the 4 bpp page
  at VRAM (192, 0) (PAL A00300.000 words 64..127), CLUT row ITEM_ICON_CLUTS[n] of the same block, column x = 304.

  python3 -I tools/event_icons.py list DISC              every event icon frame: events using it, shown/awarded, CLUT
  python3 -I tools/event_icons.py sheet DISC OUT.png     labelled sheet (events: frame / event ids; unused ones marked)
  python3 -I tools/event_icons.py compare OUT.png        icons of the demos / beta absent from the retail page, and
                                                         retail icons no event of any disc shows
DISC: pal | usa | jp | beta | jpdemo | pp8   (paths: game/disc/fs, /dev/shm/{usa,jp,beta,jpdemo}/fs, /dev/shm/pp8/TOMBA)
"""
import os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import area000  # noqa: E402

ROOT = os.path.dirname(HERE)
FS = {"pal": os.path.join(ROOT, "game/disc/fs"), "usa": "/dev/shm/usa/fs", "jp": "/dev/shm/jp/fs",
      "beta": "/dev/shm/beta/fs", "jpdemo": "/dev/shm/jpdemo/fs", "pp8": "/dev/shm/pp8/TOMBA"}
# CLUT overrides of subScreenDrawEventIcons, keyed by event (= ORDER value - 1)
CLUT_FORCE = {0x56: 0x7D12, 0x57: 0x7D52, 0x58: 0x7D92, 0x74: 0x7DD2, 0x97: 0x7A93, 0x9D: 0x7AD3, 0x9F: 0x7B13, 0xA4: 0x7B53}
# PAL anchors used to find the same tables on the other discs
PAL_DSP_BASE, PAL_CLUT_OFF, PAL_ORDER, PAL_ITEM_CLUTS = 0x800E8028, 0x98B8, 0x801086F0, 0x80109C88
REC_101 = bytes.fromhex("0808 5478 1f08 0000 0817 1810")   # PAL frame 101 part (the dummy icon), clut bytes masked below


def _u16(d, o):
    return struct.unpack_from("<H", d, o)[0]


def _u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


def _read(path):
    d = open(path, "rb").read()
    if path.upper().endswith(".GAM"):
        try:
            d = area000.gam_unpack(d)
        except Exception:
            pass
    return d


def _bank_start(d, i):
    """(start, nframes, frame) of the SPR table holding the part at byte i."""
    for s in range(i - 4, -1, -4):
        c0, o0 = struct.unpack_from("<hh", d, s)
        if not (0 < c0 < 20 and o0 > 0 and o0 % 4 == 0) or s + o0 > i:
            continue
        ok, prev, hit = True, o0, None
        for k in range(o0 // 4):
            c, o = struct.unpack_from("<hh", d, s + 4 * k)
            if o != prev or not 0 < c < 64:
                ok = False
                break
            if s + o == i:
                hit = k
            prev = o + 16 * c
        if ok and hit is not None:
            return s, o0 // 4, hit
    return None


def rgb(v):
    return ((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31, 0 if v == 0 else 255)


class Disc:
    def __init__(self, name):
        self.name, fs = name, FS[name]
        self.fs = fs
        pat = re.compile(re.escape(REC_101[:2]) + b"..\x1f\x08\x00\x00\x08\x17\x18\x10", re.S)
        # sprite table
        cands = []
        for dp, _, fn in os.walk(fs):
            for f in sorted(fn):
                if re.match(r"(A\d{3}|DATA000)\.GAM$", f, re.I) and ("AREA00" in dp or "AREA19" in dp):
                    cands.append(os.path.join(dp, f))
        self.bank = None
        for p in cands:
            d = _read(p)
            m = pat.search(d)
            if m:
                b = _bank_start(d, m.start())
                if b:
                    self.bank, self.bank_file, (self.bank_off, self.nframes, self.f101) = d, p, b
                    break
        # event page: find the rows of the dummy icon (frame f101) in the system pages
        u, v = 8, 8
        row = None
        pal = Disc.__dict__.get("_pal_rows")
        self.page = None
        if pal is None:
            pg = open(os.path.join(FS["pal"], "SYSTEM/A00300.000"), "rb").read()
            Disc._pal_rows = [pg[(v + y) * 384 + 256 + u // 2:(v + y) * 384 + 256 + u // 2 + 12] for y in range(16)]
            pal = Disc._pal_rows
        sysdir = os.path.join(fs, "SYSTEM")
        for f in sorted(os.listdir(sysdir)):
            if not f.upper().endswith(".000"):
                continue
            d = open(os.path.join(sysdir, f), "rb").read()
            i = d.find(pal[8])
            if i < 0:
                continue
            for st in (256, 384, 512, 768):
                if d[i + st:i + st + 12] == pal[9]:
                    self.page, self.page_file, self.stride = d, os.path.join(sysdir, f), st
                    self.org = i - (v + 8) * st - u // 2
                    break
            if self.page:
                break
        # sub-screen code: D0SPSUB / DSPSUB (retail, demo) or TOMBA.EXE (PrePre8)
        self.dsp = None
        for f in ("D0SPSUB.BIN", "DSPSUB.BIN"):
            if os.path.exists(os.path.join(sysdir, f)):
                self.dsp, self.dsp_file = open(os.path.join(sysdir, f), "rb").read(), os.path.join(sysdir, f)
        if self.dsp is None:
            self.dsp_file = os.path.join(fs, "TOMBA.EXE")
            self.dsp = open(self.dsp_file, "rb").read()
        self._find_tables()
        self._find_clut()

    def _find_tables(self):
        d, f0 = self.dsp, self.f101
        recs = [m.start() for m in re.finditer(re.escape(struct.pack("<H", f0)) + b"\x00\x00\x00\x00", d)
                if _u16(d, m.start() + 8) == f0 + 1 and _u16(d, m.start() + 16) == f0 + 2]
        best = None   # (length, table offset, base): the longest run of pointers to {frame >= f101} records
        for o in range(0, len(d) - 16, 4):
            v = _u32(d, o)
            if not 0x80010000 <= v < 0x80200000:
                continue
            for r in recs:
                b = v - r
                if d[:8] == b"PS-X EXE" and b != _u32(d, 0x18) - 0x800:
                    continue
                n = 0
                while o + 4 * n + 4 <= len(d):
                    q = _u32(d, o + 4 * n) - b
                    if not (0 <= q < len(d) - 8 and d[q + 2:q + 6] == b"\0\0\0\0" and f0 <= _u16(d, q) < self.nframes):
                        break
                    n += 1
                if best is None or n > best[0]:
                    best = (n, o, b)
        self.nev, self.icons_off, self.base = best
        n = self.nev
        self.icons = [_u16(d, _u32(d, self.icons_off + 4 * k) - self.base) for k in range(n)]
        pal = open(os.path.join(FS["pal"], "SYSTEM/D0SPSUB.BIN"), "rb").read()
        o = d.find(pal[PAL_ORDER - PAL_DSP_BASE:PAL_ORDER - PAL_DSP_BASE + 12])
        self.order = [x for x in d[o:o + 256] if x != 255] if o >= 0 else None
        if self.order is None:
            self.order = self._guess_order()
        o = d.find(pal[PAL_ITEM_CLUTS - PAL_DSP_BASE:PAL_ITEM_CLUTS - PAL_DSP_BASE + 24])
        self.item_cluts = d[o:o + 160] if o >= 0 else None

    def _guess_order(self):
        """Demo builds: a byte array of distinct event numbers 1..nev padded with 0xFF (the menu order)."""
        d, best = self.dsp, None
        for o in range(0, len(d) - 64):
            if d[o] == 0 or d[o] == 255:
                continue
            seen, k = set(), o
            while k < len(d) and d[k] != 255 and 0 < d[k] <= self.nev and d[k] not in seen:
                seen.add(d[k])
                k += 1
            if len(seen) >= 4 and k < len(d) and d[k] == 255 and (best is None or len(seen) > len(best)):
                if all(self.icons[x - 1] != self.f101 or x == 1 for x in seen):
                    best = sorted(seen, key=lambda x: d.index(bytes([x]), o))
        return best

    def _find_clut(self):
        pal = open(os.path.join(FS["pal"], "SYSTEM/D0SPSUB.BIN"), "rb").read()
        blk = pal[PAL_CLUT_OFF:PAL_CLUT_OFF + 48 * 31 * 2]
        self.clut, self.clut_src, self.clut_x, self.clut_w = None, None, 288, 48
        i = self.dsp.find(blk[96:192])
        tim = os.path.join(self.fs, "SYSTEM/DSPCLUT.TIM")
        if i < 96 and os.path.exists(tim):        # JP: the block differs from PAL; it starts like the disc's DSPCLUT.TIM
            i = self.dsp.find(open(tim, "rb").read()[96:192])
        if i >= 96:
            self.clut, self.clut_src = self.dsp[i - 96:i - 96 + 48 * 31 * 2], "%s+0x%X" % (os.path.basename(self.dsp_file), i - 96)
        elif os.path.exists(os.path.join(self.fs, "SYSTEM/DSPCLUT.TIM")):
            self.clut, self.clut_src = open(os.path.join(self.fs, "SYSTEM/DSPCLUT.TIM"), "rb").read(), "DSPCLUT.TIM"
        else:   # PrePre8: CLUT00 = 640 x 32 words at (384, 480)
            self.clut, self.clut_src = _read(os.path.join(self.fs, "AREA00/CLUT00.GAM")), "AREA00/CLUT00.GAM"
            self.clut_x, self.clut_w = 384, 640

    def palette(self, cw):
        x, y = (cw & 63) * 16 - self.clut_x, (cw >> 6) - 480
        if not (0 <= x <= self.clut_w - 16 and 0 <= (y * self.clut_w + x + 16) * 2 <= len(self.clut)):
            return [0x2108 * (i > 0) + 0x0842 * i for i in range(16)]       # outside the block: grey ramp
        return [_u16(self.clut, (y * self.clut_w + x + i) * 2) for i in range(16)]

    def frame_parts(self, f):
        c, o = struct.unpack_from("<hh", self.bank, self.bank_off + 4 * f)
        p0 = self.bank_off + o
        return [self.bank[p0 + 16 * k:p0 + 16 * k + 16] for k in range(c)]

    def pix(self, u, v):
        b = self.page[self.org + v * self.stride + u // 2]
        return b >> 4 * (u & 1) & 15

    def frame_image(self, f, cw=None):
        """(rgba rows 24 x 16, clut word) of event icon frame f (first part)."""
        p = self.frame_parts(f)[0]
        u, v, w, h = p[0], p[1], p[10], p[11]
        cw = cw or _u16(p, 2)
        P = self.palette(cw)
        return [[rgb(P[self.pix(u + x, v + y)]) for x in range(w)] for y in range(h)], cw, (u, v)

    def icon_index(self, f, cw=None):
        p = self.frame_parts(f)[0]
        u, v, w, h = p[0], p[1], p[10], p[11]
        return tuple(self.pix(u + x, v + y) for y in range(h) for x in range(w))

    def events_of(self):
        """{frame: [event, ...]} (event = 0-based id, the index of ICONS)."""
        out = {}
        for k, f in enumerate(self.icons):
            out.setdefault(f, []).append(k)
        return out


def unused_events():
    p = os.environ.get("UNUSED_EVENTS")
    if p and os.path.exists(p):
        return {int(l.split()[0]) for l in open(p, encoding="utf-8").read().splitlines()[8:] if l.strip()[:1].isdigit()}
    return set()


def event_names(disc):
    """PAL English titles (CFNT999B via the MAIN0 title table 0x80080410), or {} off PAL."""
    try:
        import text_core as C
        p = C.Project(C.Source(os.path.join(ROOT, "game/disc/Tombi.bin")))
        f = [x for x in p.src.files if x.upper().endswith("CFNT999B.WFM")][0]
        names = [p.original(f, i).replace("\n", " ").replace("{NL}", "") for i in range(p.count(f))]
        m = open(os.path.join(FS["pal"], "EXE/MAIN0.EXE"), "rb").read()
        tt = m[0x80080410 - _u32(m, 0x18) + 0x800:][:256]
        return {k: names[t] for k, t in enumerate(tt) if t != 255 and t < len(names)}
    except Exception:
        return {}


def classify(D, unused):
    ev, order = D.events_of(), set(D.order or [])
    rows = []
    for f in range(D.f101, D.nframes):
        es = ev.get(f, [])
        shown = [e for e in es if e + 1 in order]
        used = [e for e in shown if e not in unused]
        rows.append(dict(frame=f, events=es, shown=shown, used=used))
    return rows


def cmd_list(name):
    D, un = Disc(name), unused_events()
    nm = event_names(name) if name == "pal" else {}
    print("%s: page %s (+%d, stride %d)  sprites %s @%d (%d frames, dummy = %d)  icons @0x%X base 0x%08X (%d events)  "
          "order %d entries  CLUT %s" % (name, D.page_file, D.org, D.stride, D.bank_file, D.bank_off, D.nframes, D.f101,
                                        D.icons_off, D.base, D.nev, len(D.order or []), D.clut_src))
    for r in classify(D, un):
        p = D.frame_parts(r["frame"])[0]
        st = "used" if r["used"] else ("shown-but-never-awarded" if r["shown"] else ("not-in-menu" if r["events"] else "no-event"))
        print("frame %3d uv (%3d,%3d) clut (%d,%d) %-24s events %s %s" % (
            r["frame"], p[0], p[1], (_u16(p, 2) & 63) * 16, _u16(p, 2) >> 6, st, r["events"][:8],
            "; ".join(nm.get(e, "-") for e in r["events"][:2]) if r["frame"] != D.f101 else ""))


def _sheet(cells, out, cols=10, scale=3):
    from PIL import Image, ImageDraw
    cw, ch = 140, 64
    im = Image.new("RGBA", (cols * cw, ((len(cells) + cols - 1) // cols) * ch + 4), (36, 36, 48, 255))
    dr = ImageDraw.Draw(im)
    for j, (pix, lines, bad) in enumerate(cells):
        X, Y = (j % cols) * cw + 4, (j // cols) * ch + 4
        dr.rectangle((X - 1, Y - 1, X + 24 * 2, Y + 16 * 2), fill=(90, 90, 110, 255), outline=(255, 80, 80, 255) if bad else None)
        for y, rw in enumerate(pix):
            for x, c in enumerate(rw):
                if c[3]:
                    dr.rectangle((X + 2 * x, Y + 2 * y, X + 2 * x + 1, Y + 2 * y + 1), fill=c)
        for i, t in enumerate(lines):
            dr.text((X + 52 if i < 3 else X, Y + i * 10 if i < 3 else Y + 34 + (i - 3) * 10), t[:22 if i < 3 else 26],
                    fill=(255, 255, 160, 255) if i == 0 else (200, 220, 255, 255))
    im = im.resize((im.width * scale // 2, im.height * scale // 2), Image.NEAREST)
    im.save(out)


def cmd_sheet(name, out):
    D, un = Disc(name), unused_events()
    nm = event_names(name) if name == "pal" else {}
    cells = []
    for r in classify(D, un):
        f = r["frame"]
        if r["used"]:
            continue
        e0 = r["events"][0] if r["events"] else None
        pix, cw, (u, v) = D.frame_image(f, CLUT_FORCE.get(e0))
        st = "no event" if not r["events"] else ("never awarded" if r["shown"] else "not in menu")
        cells.append((pix, ["fr %d (%d,%d)" % (f, u + 256, v), "ev %s" % ",".join(map(str, r["events"][:4])) if r["events"] else "ev -",
                            st, nm.get(e0, "")[:26], "CLUT (%d,%d) %s" % ((cw & 63) * 16, cw >> 6, D.clut_src[:10])], True))
    _sheet(cells, out)


def cmd_compare(out):
    """Icons (pixel indices) of each disc's page that the PAL page does not have, and their events."""
    P = Disc("pal")
    have = {P.icon_index(f) for f in range(P.f101, P.nframes)}
    print("pal: %d icon frames" % (P.nframes - P.f101))
    cells = []
    for name in ("usa", "jp", "beta", "jpdemo", "pp8"):
        try:
            D = Disc(name)
        except Exception as e:
            print(name, "skipped:", e)
            continue
        ev = D.events_of()
        order = set(D.order or [])
        for f in range(D.f101, D.nframes):
            p = D.frame_parts(f)[0]
            if (p[10], p[11]) != (24, 16) or D.icon_index(f) in have:
                continue
            es = ev.get(f, [])
            pix, cw, (u, v) = D.frame_image(f)
            shown = [e for e in es if e + 1 in order]
            print("%-6s frame %3d uv (%3d,%3d) clut (%d,%d) events %s shown %s" % (name, f, u, v, (cw & 63) * 16, cw >> 6, es[:6], shown[:6]))
            cells.append((pix, ["%s fr %d" % (name, f), "(%d,%d)" % (u + 256, v), "ev %s" % ",".join(map(str, es[:3])),
                                "shown %s" % ",".join(map(str, shown[:3])), "CLUT (%d,%d) %s" % ((cw & 63) * 16, cw >> 6, D.clut_src[:12])],
                          not shown))
    if out:
        _sheet(cells, out)


def item_page(name="pal"):
    """[(n, clut row, rgba 16 x 16)] for the 160 item icons (PAL page A00300 words 64..127)."""
    D = Disc(name)
    pg = D.page
    out = []
    for n in range(165):
        row = D.item_cluts[n] if D.item_cluts and n < 160 else 0
        P = D.palette((row + 0x1E0) << 6 | 0x13)
        u0, v0 = n % 15 * 16 + 8, n // 15 * 16 + 8
        px = [[rgb(P[pg[D.org - 128 + (v0 + y) * D.stride + (u0 + x) // 2] >> 4 * ((u0 + x) & 1) & 15]) for x in range(16)]
              for y in range(16)]
        out.append((n, row, px))
    return out


if __name__ == "__main__":
    a = sys.argv[1:]
    if not a:
        sys.exit(__doc__)
    if a[0] == "list":
        cmd_list(a[1])
    elif a[0] == "sheet":
        cmd_sheet(a[1], a[2])
    elif a[0] == "compare":
        cmd_compare(a[1] if len(a) > 1 else None)
    else:
        sys.exit(__doc__)
