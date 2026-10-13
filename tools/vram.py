#!/usr/bin/env python3
"""Tombi! (PAL) VRAM simulation: every graphic of the game as PNG.  Format notes: notes/formats.md (section "SPR / VRAM").

The game keeps all its graphics in the 1024 x 512 word PSX VRAM.  Every load list of SYS/LD0*.BIN (the area list, one list per room, and
the 95 lists of SYS/LD0SYS.BIN: title, loading screens, menus, NPC atlases, ...) names GRPX files (load kind 0x10/0x11) that are sent
there with LoadImage(x, y, w, h); the destination is in the record (`arg = y << 16 | x`, x in 16-bit words).  This module replays
those records into a 16-bit buffer and then cuts the SPR frame definitions (load kinds 0x30..0x3F, `FUN_80052db8`: 16-byte POLY_FT4
templates `u0 v0 clut | u1 v1 tpage | u2 v2 w h | u3 v3 dx dy`) out of it:

  tools/vram.py snapshot LIST OUT.png     full 1024 x 512 RGB555 view of one list (LIST = AREA00:1 | SYSTEM:2)
  tools/vram.py --all OUT [--fs DIR]      everything: OUT/spr (atlas cuts, composed frames), OUT/screens (pages, snapshots)
  tools/vram.py --stats                   counts only (lists, pages, tables, frames, parts), nothing is written

Also a library for tools/extract_sprites.py (`export_all`).  Pure Python, no dependencies.  The retail data is only read.
"""
import json, os, struct, sys, zlib
from array import array
from collections import Counter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import area000 as A

VW, VH = 1024, 512
TYPE_LZ = (1,)


# ------------------------------------------------------------------ PNG (pure python)
def _chunk(tag, data):
    c = struct.pack(">I", len(data)) + tag + data
    return c + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)


def png_rgba(w, h, rows):
    """rows: bytes of 4*w (RGBA)."""
    raw = b"".join(b"\0" + r for r in rows)
    return (b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0)) +
            _chunk(b"IDAT", zlib.compress(raw, 6)) + _chunk(b"IEND", b""))


def png_rgb(w, h, rows):
    raw = b"".join(b"\0" + r for r in rows)
    return (b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)) +
            _chunk(b"IDAT", zlib.compress(raw, 6)) + _chunk(b"IEND", b""))


def png_idx(w, h, rows, pal, alpha):
    """indexed PNG; pal = RGB bytes, alpha = bytes (one per palette entry, 0 = transparent; trailing 255 entries are dropped)."""
    raw = b"".join(b"\0" + r for r in rows)
    out = b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 3, 0, 0, 0)) + _chunk(b"PLTE", pal)
    a = bytes(alpha).rstrip(b"\xff")
    if a:
        out += _chunk(b"tRNS", a)
    return out + _chunk(b"IDAT", zlib.compress(raw, 6)) + _chunk(b"IEND", b"")


# RGB555 word -> bytes
RGB = [bytes(((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31)) for v in range(65536)]
RGBA = [RGB[v] + (b"\0" if v == 0 else b"\xff") for v in range(65536)]


def words_to_rgb(ws):
    return b"".join(map(RGB.__getitem__, ws))


def words_to_rgba(ws):
    return b"".join(map(RGBA.__getitem__, ws))


# ------------------------------------------------------------------ VRAM
_LO = bytes((i & 15) for i in range(256))
_HI = bytes((i >> 4) for i in range(256))


class Vram:
    """1024 x 512 words.  `d` = the words as bytes (8 bpp view: 2048 x 512), `nib` = 4 bpp view (4096 x 512, built on demand);
    `m_any` / `m4` / `m8` = per word flags: loaded by a GRPX record / ... and not a palette block / loaded and not paired with a 256 colour
    CLUT (a 4 bpp candidate)."""

    def __init__(self):
        self.d = bytearray(VW * VH * 2)
        self.m_any = bytearray(VW * VH)
        self.m4 = bytearray(VW * VH)
        self.recs = []          # [(name, x, y, w, h, kind)] in load order
        self.ld = None          # area ('AR05') whose CLUT files supply fallback palettes
        self._nib = None
        self._pal = {}

    def copy(self):
        o = Vram()
        o.d[:] = self.d
        o.m_any[:] = self.m_any
        o.m4[:] = self.m4
        o.recs = list(self.recs)
        o.ld = self.ld
        return o

    def load(self, x, y, w, h, data, name="", kind="page", four=True):
        """LoadImage of w x h words (clipped to the VRAM)."""
        self._nib = None
        self._pal = {}
        cw = min(w, VW - x)
        for r in range(h):
            if y + r >= VH or cw <= 0:
                break
            a = (y + r) * VW + x
            self.d[a * 2:(a + cw) * 2] = data[r * w * 2:(r * w + cw) * 2]
            self.m_any[a:a + cw] = b"\1" * cw
            self.m4[a:a + cw] = (b"\1" if four and kind == "page" else b"\0") * cw
        self.recs.append((name, x, y, w, h, kind))

    @property
    def nib(self):
        if self._nib is None:
            n = bytearray(len(self.d) * 2)
            n[0::2] = self.d.translate(_LO)
            n[1::2] = self.d.translate(_HI)
            self._nib = n
        return self._nib

    def words(self, x, y, n):
        a = y * VW + x
        return struct.unpack_from("<%dH" % n, self.d, a * 2)

    def palette(self, clut, n):
        """(rgb bytes, alpha bytes, words) of the n colours at the GPU CLUT word."""
        k = (clut, n)
        p = self._pal.get(k)
        if p is None:
            x, y = (clut & 0x3F) * 16, clut >> 6 & 0x1FF
            ws = self.words(x, y, min(n, VW - x))
            ws = ws + (0,) * (n - len(ws))
            p = (b"".join(RGB[w] for w in ws), bytes(0 if w == 0 else 255 for w in ws), ws)
            self._pal[k] = p
        return p

    def snapshot_png(self):
        ws = memoryview(self.d).cast("H")
        rows = []
        for y in range(VH):
            rows.append(words_to_rgb(ws[y * VW:(y + 1) * VW]))
        return png_rgb(VW, VH, rows)


# ------------------------------------------------------------------ load lists -> VRAM
_RES = {}


def resource(name):
    if name not in _RES:
        try:
            _RES[name] = A.read_resource(name)
        except (OSError, ValueError, IndexError):
            _RES[name] = None
    return _RES[name]


def is_grpx(r):
    return (r["kind"] & 0xF0) == 0x10 and r["type"] != 0xFFFFFFFF


def is_clut_rec(r):
    """palette blocks: 16 x n / 256 x n words with n <= 32 rows, DSPCLUT.TIM (48 x 31)."""
    return r["h"] <= 32 and r["w"] in (16, 48, 256)


def pair_cluts(lst):
    """{index of page record: index of the 256 colour CLUT record the list pairs it with}.  A 256 x n CLUT record (n <= 32 rows) belongs to
    the page records directly before it; a 16 x n sprite palette strip ends that run unpaired (so the
    resident HUD pages A00000/A10003 stay 4 bpp, A00006 + A00007 = start-up logo, B000 + CLUT01 = room, A00200 + A00201 = title screen)."""
    out, run = {}, []
    for i, r in enumerate(lst):
        if not is_grpx(r):
            continue
        if not is_clut_rec(r):
            run.append(i)
        elif r["w"] == 256:
            for p in run:
                out[p] = i
            run = []
        elif r["w"] == 16:
            run = []
    return out


def place(v, r, paired8=False):
    d = resource(r["name"])
    w, h = r["w"], r["h"]
    if d is None or w <= 0 or len(d) < w * h * 2:
        return False
    v.load(r["arg"] & 0xFFFF, r["arg"] >> 16, w, h, d[:w * h * 2], r["name"],
           "clut" if is_clut_rec(r) else "page", four=not paired8)
    return True


def load_list(v, lst, skip=()):
    """Replays the GRPX records of one load list into v.  Pages paired with a 256 colour CLUT are 8 bpp for the 4 bpp candidate mask."""
    pr = pair_cluts(lst)
    for i, r in enumerate(lst):
        if is_grpx(r) and r["name"] not in skip:
            place(v, r, i in pr)
    return v


SCREEN_SKIP = ("SYSTEM/A00006", "SYSTEM/A00007")


def area_lists(ld):
    return A.load_lists()[ld]


def resident(v):
    """SYS list 0 without its start-up logo (A00006/A00007 at 0,0): HUD pages, system sprite palettes (the resident VRAM state)."""
    load_list(v, A.load_lists()["SYS"][0], skip=("SYSTEM/A00006.GAM", "SYSTEM/A00007.000"))
    return v


def first_area_clut(ld):
    for lst in A.load_lists()[ld][1:]:
        for r in lst:
            if r["name"].rsplit("/", 1)[-1].startswith("CLUT"):
                return r
    return None


def area_context(ld, idx, extra=()):
    """VRAM of area `ld` ('AR05'), load list `idx`: resident SYS data + area list 0 + room list idx (+ extra SYS lists).  The area list has
    no CLUT of its own, so list 0 gets the first CLUTnn of the area (the sprite palette rows are identical in all of them)."""
    v = resident(Vram())
    v.ld = ld
    lists = A.load_lists()[ld]
    load_list(v, lists[0])
    if idx == 0:
        c = first_area_clut(ld)
        if c:
            place(v, c)
    else:
        load_list(v, lists[idx])
    for k in extra:
        load_list(v, A.load_lists()["SYS"][k])
    return v


# ------------------------------------------------------------------ SPR tables
def parse_spr(d):
    """[(parts)] per frame; a part = dict(u0 v0 clut u1 v1 tpage u2 v2 w h u3 v3 dx dy)."""
    n = struct.unpack_from("<H", d, 2)[0] // 4
    out = []
    for f in range(n):
        cnt, off = struct.unpack_from("<HH", d, 4 * f)
        ps = []
        for k in range(cnt):
            if off + 16 * k + 16 > len(d):
                break
            ps.append(struct.unpack_from("<BBHBBHBBBBBBbb", d, off + 16 * k))
        out.append(ps)
    return out


def part_size(p):
    return p[8], p[9]


def unpack_idx(src, width, base_off, x0, y0, w, h, fx, fy):
    """Rows of a w x h cut from an index plane (`src` rows of `width` bytes, origin at base_off), texture window wraps at 256."""
    rows = []
    for j in range(h):
        v = (y0 + (-j if fy else j)) & 255
        a = base_off + v * width
        if not fx and x0 + w <= 256:
            rows.append(bytes(src[a + x0:a + x0 + w]))
        else:
            rows.append(bytes(src[a + ((x0 + (-i if fx else i)) & 255)] for i in range(w)))
    return rows


def part_plane(v, tpage, mode_override=None):
    """(plane, width in plane units, origin offset of the page, mode, unit) of a texture page word."""
    mode = (tpage >> 7) & 3 if mode_override is None else mode_override
    p = tpage & 0x1F
    px, py = (p & 15) * 64, (p >> 4) * 256
    if mode == 0:
        return v.nib, VW * 4, py * VW * 4 + px * 4, 0
    if mode == 1:
        return v.d, VW * 2, py * VW * 2 + px * 2, 1
    return v.d, VW * 2, py * VW * 2 + px * 2, 2


def cut_part(v, p, base=0):
    """Pixels of one SPR part: (w, h, kind, rows, pal_rgb, alpha, words) with kind 'idx' (rows = index bytes) or 'rgb16' (rows = word lists)."""
    u0, v0, clut, u1, v1, tpage, u2, v2, w, h, u3, v3, dx, dy = p
    fx, fy = (u1 < u0), (v2 < v0)
    tp = tpage + base
    src, width, off, mode = part_plane(v, tp)
    if mode == 2:
        rows = []
        for j in range(h):
            vv = (v0 + (-j if fy else j)) & 255
            a = (off // 2) + vv * VW
            rows.append([struct.unpack_from("<H", v.d, (a + ((u0 + (-i if fx else i)) & 255)) * 2)[0] if a * 2 + 2 <= len(v.d) else 0 for i in range(w)])
        return w, h, "rgb16", rows, None, None, None
    rows = unpack_idx(src, width, off, u0, v0, w, h, fx, fy)
    n = 16 if mode == 0 else 256
    pal, alpha, ws = v.palette(clut, n)
    return w, h, "idx", rows, pal, alpha, ws


def rect_words(p, base):
    """(mode, x0, y0, x1, y1) in VRAM words (x1/y1 inclusive) of the texture rectangle of part p with tpage offset `base` (flips handled);
    mode = the colour depth of the resulting tpage (0 = 4 bpp, 1 = 8 bpp, 2 = 16 bpp)."""
    u0, v0, clut, u1, v1, tpage, u2, v2, w, h = p[:10]
    tp = tpage + base
    mode = min((tp >> 7) & 3, 2)
    q = tp & 0x1F
    px, py = (q & 15) * 64, (q >> 4) * 256
    ua, ub = (u0 - w + 1, u0) if u1 < u0 else (u0, u0 + w - 1)
    va, vb = (v0 - h + 1, v0) if v2 < v0 else (v0, v0 + h - 1)
    ua, ub, va, vb = max(ua, 0), min(ub, 255), max(va, 0), min(vb, 255)
    div = (4, 2, 1)[mode]
    return mode, px + ua // div, py + va, px + ub // div, py + vb


def page_blocks(parts, base):
    """VRAM blocks (x0, y0, x1, y1 in words, end exclusive) of the texture pages the parts reference with tpage offset `base`."""
    out = set()
    for p in parts:
        tp = p[5] + base
        mode = min((tp >> 7) & 3, 2)
        q = tp & 0x1F
        px, py = (q & 15) * 64, (q >> 4) * 256
        out.add((px, py, px + (64, 128, 256)[mode], py + 256))
    return list(out)


def fit_unused(v, blocks):
    """Share of the words of the GRPX pages touched by the referenced blocks that lie outside them: 0 = the table uses exactly the pages
    of the records it touches (an atlas made for the table), > 0 = the table reads part of a bigger atlas (or the wrong one)."""
    touched = outside = 0
    for name, x, y, w, h, kind in v.recs:
        if kind != "page":
            continue
        hit = [b for b in blocks if b[0] < x + w and x < b[2] and b[1] < y + h and y < b[3]]
        if not hit:
            continue
        a = w * h
        inside = 0
        for yy in range(y, y + h, 32):   # coarse: 32 row steps are enough, blocks are page aligned
            for xx in range(x, x + w, 32):
                if any(b[0] <= xx < b[2] and b[1] <= yy < b[3] for b in blocks):
                    inside += 1
        tot = ((h + 31) // 32) * ((w + 31) // 32)
        touched += tot
        outside += tot - inside
    return outside / touched if touched else 1.0


def score_candidate(v, ps, base):
    """Share of the sampled parts whose texture rectangle lies completely in loaded page data (4 bpp parts: pages that are not paired with
    a 256 colour CLUT, so not 8 bpp room backgrounds)."""
    ok = tot = 0
    for p in ps:
        mode, x0, y0, x1, y1 = rect_words(p, base)
        tot += 1
        if x1 >= VW or y1 >= VH or mode > 2:
            continue
        mask = v.m4 if mode == 0 else v.m_any
        if all(0 not in mask[y * VW + x0:y * VW + x1 + 1] for y in {y0, y1, (y0 + y1) // 2}):
            ok += 1
    return ok / max(tot, 1)


def sample_parts(parts, n):
    uniq = {}
    for p in parts:
        uniq[(p[0], p[1], p[3], p[4], p[5], p[6], p[7], p[8], p[9])] = p
    ps = list(uniq.values())
    if len(ps) > n:
        step = len(ps) / float(n)
        ps = [ps[int(i * step)] for i in range(n)]
    return ps


def _ring_cut(v, parts, base, cap=300):
    """Opaque pixels in the 1 px ring around the 4 bpp rectangles that belong to no rectangle of the table (sprite art cut through by a
    wrong rectangle) per rectangle, and the opaque share of the rectangles.  (None, 0) = nothing to measure."""
    ps = [p for p in sample_parts(parts, cap) if rect_words(p, base)[0] == 0]
    if not ps:
        return 0.0, 1.0
    U = {}
    rects = []
    for p in ps:
        mode, x0, y0, x1, y1 = rect_words(p, base)
        if x1 >= VW or y1 >= VH:
            return 99.0, 0.0
        rects.append((x0 * 4, y0, x1 * 4 + 3, y1))
        for y in range(y0, y1 + 1):
            U.setdefault(y, bytearray(VW * 4))[x0 * 4:x1 * 4 + 4] = b"\1" * (x1 * 4 + 4 - x0 * 4)
    nib = v.nib
    cut = nz = tl = 0
    empty = bytearray(VW * 4)
    for a, y0, b, y1 in rects:
        for y in range(y0, y1 + 1):
            seg = nib[y * VW * 4 + a:y * VW * 4 + b + 1]
            nz += len(seg) - seg.count(0)
            tl += len(seg)
        for y in (y0 - 1, y1 + 1):
            if 0 <= y < VH:
                u = U.get(y, empty)
                seg = nib[y * VW * 4 + max(a - 1, 0):y * VW * 4 + min(b + 2, VW * 4)]
                for k, c in enumerate(seg):
                    if c and not u[max(a - 1, 0) + k]:
                        cut += 1
        for y in range(y0, y1 + 1):
            u = U.get(y, empty)
            for x in (a - 1, b + 1):
                if 0 <= x < VW * 4 and nib[y * VW * 4 + x] and not u[x]:
                    cut += 1
    return cut / len(rects), nz / max(tl, 1)


def covered(v, p, base):
    """The texture rectangle of part p (offset `base`) lies completely in loaded page data (every row checked)."""
    mode, x0, y0, x1, y1 = rect_words(p, base)
    if x1 >= VW or y1 >= VH or mode > 2:
        return False
    mask = v.m4 if mode == 0 else v.m_any
    return all(0 not in mask[y * VW + x0:y * VW + x1 + 1] for y in range(y0, y1 + 1))


def infer_base(ctxs, parts, sample=150):
    """The game adds a per object tpage offset (TObj.w1e, set by the area overlay code) to the tpage of every part, so a table does not say
    where its texture is.  Among the candidate VRAM contexts (a list plus each SYS atlas list of the area) and offsets 0..31 take the one
    whose rectangles are all in loaded page data, that fits the loaded records best (the table's pages = exactly the pages of the atlas
    records it touches; example: the NPC table of AREA00 has 6 pages = SYSTEM/A01000 at x=64 -> offset 1) and, among equal fits, that cuts
    the least sprite art through the rectangle borders.  If nothing covers 97 % of the sampled rectangles the best partial cover (>= 25 %)
    is taken.  ctxs = [(key, Vram)].
    Returns (ctx index, base, info) or None; info["ties"] lists the equally good (ctx, base) pairs: more than one = ambiguous."""
    ps = sample_parts(parts, sample)
    maxp = max((p[5] & 0x1F for p in parts), default=0)
    allc = []
    for ci, (_, v) in enumerate(ctxs):
        for base in range(0, 32 - maxp):
            allc.append((score_candidate(v, ps, base), ci, base))
    cand = [(round(fit_unused(ctxs[ci][1], page_blocks(parts, base)), 2), ci, base, cov) for cov, ci, base in allc if cov >= 0.97]
    partial = False
    if not cand:
        mx = max((c[0] for c in allc), default=0)
        if mx < 0.25:
            return None
        partial = True
        cand = [(round(fit_unused(ctxs[ci][1], page_blocks(parts, base)), 2), ci, base, cov) for cov, ci, base in allc if cov >= mx - 0.02]
    cand.sort(key=lambda c: (c[0], c[1], c[2]))
    top = [c for c in cand if c[0] == cand[0][0]]
    scored = []
    for fit, ci, base, cov in top[:40]:
        cut, dens = _ring_cut(ctxs[ci][1], parts, base)
        scored.append((0 if dens >= 0.2 else 1, round(cut, 2), ci, base, cov))
    scored.sort()
    best = scored[0]
    ties = [[c[2], c[3]] for c in scored if c[0] == best[0] and c[1] <= best[1] * 1.25 + 0.05]
    alts = sorted({c[2] for c in allc if c[0] >= 0.25 and c[1] == best[2]} - {best[3]})
    return best[2], best[3], {"unused": cand[0][0], "ring_cut": best[1], "coverage": round(best[4], 2), "partial": partial,
                              "ties": ties[:8], "alt_offsets": alts, "candidates": len(cand)}


# ------------------------------------------------------------------ candidate contexts
_CTX = {}
_SYSK = {}


def sys_lists_for(ld, idx):
    """SYS list indices (>= 9) whose overlay (X0nn.BIN, I?NFO*.BIN) is also named by the area list or by list `idx`: the sprite/NPC atlases
    the area code loads with that room."""
    key = (ld, idx)
    if key in _SYSK:
        return _SYSK[key]
    L = A.load_lists()

    def ov(lst):
        return {r["name"].rsplit("/", 1)[-1] for r in lst if (r["kind"] & 0xF0) == 0xF0 and r["name"].endswith(".BIN")}
    mine = (ov(L[ld][0]) | ov(L[ld][idx])) - {"O0PTSUB0.BIN", "D0SPSUB.BIN", "D0SPSUB2.BIN", "D0SPSUB3.BIN", "G0OVER.BIN"}
    out = [k for k, lst in enumerate(L["SYS"]) if k >= 9 and (ov(lst) & mine) and any(is_grpx(r) for r in lst)]
    _SYSK[key] = out
    return out


def ctx_get(ld, idx, k=None):
    key = (ld, idx, k)
    if key not in _CTX:
        _CTX[key] = area_context(ld, idx, extra=(k,) if k is not None else ())
    return _CTX[key]


def resident_context(ld):
    """Resident SYS pages + the CLUT block of area `ld` only (for tables identical in every area: they can only use resident data)."""
    key = (ld, "res")
    if key not in _CTX:
        v = resident(Vram())
        v.ld = ld
        c = first_area_clut(ld)
        if c:
            place(v, c)
        _CTX[key] = v
    return _CTX[key]


_SHARED = None


def shared_tables():
    """Set of (kind, md5) of SPR tables that are byte-identical in the lists of >= 10 areas (Tombi, HUD, effects)."""
    global _SHARED
    if _SHARED is None:
        import hashlib
        areas = {}
        for ld in A.load_lists():
            if ld == "SYS":
                continue
            for i in range(len(A.load_lists()[ld])):
                for r, d in A.list_blocks(ld, i):
                    if (r["kind"] & 0xF0) == 0x30 and d:
                        areas.setdefault((r["kind"], hashlib.md5(d).hexdigest()), set()).add(ld)
        _SHARED = {k for k, v in areas.items() if len(v) >= 10}
    return _SHARED


def shared_candidates(ld):
    """Candidates for the tables used by every area (Tombi, HUD, effects): they can only use the resident VRAM."""
    return [((ld, "res", None), resident_context(ld))]


def candidates(ld, idx):
    """[(key, Vram)] where the textures of the SPR tables of list idx may be: the list itself, with each SYS list of the area's overlay."""
    out = [((ld, idx, None), ctx_get(ld, idx))]
    for k in sys_lists_for(ld, idx):
        out.append(((ld, idx, k), ctx_get(ld, idx, k)))
    return out


# ------------------------------------------------------------------ WPP-linked tables
def wpp_tables(lst):
    """[(file, table no, [decoded frame, ...])] of the WPP files (kind 0x6n) of a list: u32 table containers, the record `arg` is the size
    of the first one when the file holds two (A004.000: 358 + 83 frames)."""
    import wpp
    out = []
    for r in lst:
        if (r["kind"] & 0xF0) != 0x60 or r["type"] == 0xFFFFFFFF or not r["name"].endswith(".000"):
            continue
        d = resource(r["name"])
        if d is None:
            continue
        chunks = [d]
        if 0 < r["arg"] < len(d) and A.parse_table(d[:r["arg"]]):
            chunks = [d[:r["arg"]], d[r["arg"]:]]
        for ti, c in enumerate(chunks):
            t = A.parse_table(c)
            if not t:
                continue
            _, _, bounds, _ = t
            fr = [wpp.wpp_decode(c[bounds[i]:bounds[i + 1]]) for i in range(len(bounds) - 1)]
            out.append((r["name"], ti, fr))
    return out


def wpp_plane(o):
    """(rows of nibble bytes, W px, H) of a decoded WPP frame."""
    w, h = struct.unpack_from("<HH", o)
    raw = bytes(o[4:4 + 2 * w * h])
    n = bytearray(len(raw) * 2)
    n[0::2] = raw.translate(_LO)
    n[1::2] = raw.translate(_HI)
    return [bytes(n[y * w * 4:(y + 1) * w * 4]) for y in range(h)], w * 4, h


# ------------------------------------------------------------------ export of the SPR tables
def _rgba_row_idx(rowidx, prgba):
    return b"".join(map(prgba.__getitem__, rowidx))


def render_part(v, p, base, plane=None):
    """(w, h, png bytes, rgba rows, blank, note) of one SPR part from VRAM `v` (offset `base`) or from a WPP frame plane (rows, W, H)."""
    u0, v0, clut, u1, v1, tpage, u2, v2, w, h, u3, v3, dx, dy = p
    fx, fy = (u1 < u0), (v2 < v0)
    if plane is not None:
        rows_in, pw, ph = plane
        rows = []
        for j in range(h):
            vv = v0 + (-j if fy else j)
            src = rows_in[vv] if 0 <= vv < ph else b""
            if not fx:
                seg = src[u0:u0 + w] if u0 >= 0 else b""
                rows.append(seg + bytes(w - len(seg)))
            else:
                rows.append(bytes(src[u0 - i] if 0 <= u0 - i < len(src) else 0 for i in range(w)))
        pal, alpha, ws = v.palette(wpp_clut(clut), 16)
        kind, n = "idx", 16
    else:
        w, h, kind, rows, pal, alpha, ws = cut_part(v, p, base)
    note = None
    if kind == "idx" and not any(ws[1:]):
        pal, alpha, ws, note = fallback_palette(v, wpp_clut(clut) if plane is not None else clut, len(ws))
    if kind == "idx":
        prgba = [pal[i * 3:i * 3 + 3] + bytes((alpha[i],)) for i in range(len(alpha))]
        blank = not any(any(r) for r in rows) or not any(alpha[i] for r in rows for i in set(r))
        return w, h, png_idx(w, h, rows, pal, alpha), [_rgba_row_idx(r, prgba) for r in rows], blank, note
    rg = [words_to_rgba(r) for r in rows]
    blank = not any(any(r) for r in rows)
    return w, h, png_rgba(w, h, rg), rg, blank, None


def fallback_palette(v, clut, n):
    """Palette (rgb, alpha, words, note) for a CLUT that is all zero in the simulated VRAM: some palette slots are filled at run time or
    only by the CLUT block of another room of the area; look for the same slot in the other CLUTnn files of the area, else a grey ramp."""
    x, y = (clut & 0x3F) * 16, clut >> 6 & 0x1FF
    if v.ld and 128 <= x < 384 and 480 <= y < 512:
        adir = os.path.join(A.FS, area_name(v.ld))
        for fn in sorted(os.listdir(adir)) if os.path.isdir(adir) else []:
            if fn.startswith("CLUT") and fn.endswith(".GAM"):
                blob = resource(area_name(v.ld) + "/" + fn)
                if blob and len(blob) >= 256 * 32 * 2:
                    ws = struct.unpack_from("<%dH" % n, blob, ((y - 480) * 256 + (x - 128)) * 2)
                    if any(ws):
                        return (b"".join(RGB[w] for w in ws), bytes(0 if w == 0 else 255 for w in ws), ws, "palette slot empty in the list's CLUT, taken from " + fn)
    ws = tuple(0x421 * min(i * 2, 31) if i else 0 for i in range(n)) if n == 16 else tuple(0x421 * (i >> 3) if i else 0 for i in range(n))
    return (b"".join(RGB[w] for w in ws), bytes(0 if w == 0 else 255 for w in ws), ws, "palette slot is empty (set at run time): grey ramp")


def wpp_clut(c):
    """Tombi's parts carry the placeholder palette x=768 (rows 480/481): set at run time by GetClut(0x80, 0x1e0..), the CLUT of the area
    (VRAM 128, 480): same row, x = 128."""
    x, y = (c & 0x3F) * 16, c >> 6 & 0x1FF
    if x >= 512:
        x = 128 + (x & 0xFF)
    return (x // 16) | (y << 6)


def compose_rgba(items):
    """items [(dx, dy, w, h, rgba rows)] -> (ox, oy, W, H, rows).  Painted in part order; a part row over transparent canvas is copied whole."""
    if not items:
        return 0, 0, 0, 0, []
    x0 = min(i[0] for i in items)
    y0 = min(i[1] for i in items)
    W = max(i[0] + i[2] for i in items) - x0
    H = max(i[1] + i[3] for i in items) - y0
    cv = [bytearray(4 * W) for _ in range(H)]
    for dx, dy, w, h, rows in items:
        for j in range(h):
            dst = cv[dy - y0 + j]
            o = (dx - x0) * 4
            src = rows[j]
            if not any(dst[o + 3:o + 4 * w:4]):
                dst[o:o + 4 * w] = src
            else:
                for i in range(w):
                    if src[i * 4 + 3]:
                        dst[o + i * 4:o + i * 4 + 4] = src[i * 4:i * 4 + 4]
    return -x0, -y0, W, H, [bytes(r) for r in cv]


def write(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data)


def area_name(ld):
    return "SYSTEM" if ld == "SYS" else "AREA" + ld[2:]


class Stats(Counter):
    pass


def export_table(out_dir, ld, idx, kind, d, v, base, plane_frames, info, usage, st, alts=()):
    """Writes frame_NNNN_partK.png and frame_NNNN.png of one SPR table + manifest.json."""
    frames = parse_spr(d)
    man = {"load_list": "%s list %d" % (area_name(ld), idx), "kind": "0x%02X" % kind, "frames": len(frames), "tpage_offset": base,
           "source": info, "frame_list": []}
    for fi, parts in enumerate(frames):
        items, meta = [], []
        fbase = base
        if plane_frames is None and alts:
            live = [p for p in parts if p[8] and p[9]]
            if live and not all(covered(v, p, base) for p in live):
                # this frame belongs to another atlas of the same family (several objects share one table with different offsets)
                fbase = next((a for a in alts if all(covered(v, p, a) for p in live)), base)
        for k, p in enumerate(parts):
            if p[8] == 0 or p[9] == 0:
                continue
            pl = wpp_plane_of(plane_frames, fi)
            w, h, png, rgba, blank, note = render_part(v, p, fbase, pl)
            st["parts"] += 1
            if blank:
                st["blank_parts"] += 1
                meta.append({"part": k, "blank": True})
                continue
            write(os.path.join(out_dir, "frame_%04d_part%d.png" % (fi, k)), png)
            st["parts_written"] += 1
            items.append((p[12], p[13], w, h, rgba))
            meta.append({"part": k, "x": p[12], "y": p[13], "w": w, "h": h, "clut": "0x%04X" % p[2], "tpage": "0x%04X" % p[5],
                         "uv": [p[0], p[1]]})
            if note:
                meta[-1]["palette_note"] = note
                st["parts_with_fallback_palette"] += 1
            if plane_frames is None:
                note_usage(v, p, fbase, usage)
        st["frames"] += 1
        if not items:
            man["frame_list"].append({"frame": fi, "blank": True, "parts": meta})
            continue
        ox, oy, W, H, rows = compose_rgba(items)
        write(os.path.join(out_dir, "frame_%04d.png" % fi), png_rgba(W, H, rows))
        st["frames_written"] += 1
        fm = {"frame": fi, "origin": [ox, oy], "size": [W, H], "parts": meta}
        if fbase != base:
            fm["tpage_offset"] = fbase
        man["frame_list"].append(fm)
    write(os.path.join(out_dir, "manifest.json"), json.dumps(man, indent=1).encode())


def note_usage(v, p, base, usage):
    """Remember which palette cuts which page record (to colour the flat page PNGs): usage[record name][(bpp mode, clut)] += 1."""
    mode, x0, y0, x1, y1 = rect_words(p, base)
    cx, cy = (x0 + x1) // 2, (y0 + y1) // 2
    for name, x, y, w, h, kind in v.recs:
        if kind == "page" and x <= cx < x + w and y <= cy < y + h:
            pal = v.palette(p[2], 16 if mode == 0 else 256)[0]
            usage.setdefault(name, Counter())[(mode, p[2], pal)] += 1


def wpp_plane_of(plane_frames, fi):
    if plane_frames is None:
        return None
    return plane_frames[fi] if fi < len(plane_frames) else ([], 0, 0)


def table_signature(d, v, base, wpp=None):
    """Identity of a rendered table: table bytes, offset, WPP frames and the palette words the parts point at."""
    import hashlib
    h = hashlib.md5(d)
    h.update(b"%d" % base)
    if wpp is not None:
        h.update(wpp)
    for c in sorted({p[2] for f in parse_spr(d) for p in f}):
        h.update(bytes(str(v.palette(wpp_clut(c) if wpp is not None else c, 16)[2]), "ascii"))
    return h.hexdigest()


def export_spr(out, only=None, log=print):
    """OUT/spr/<AREA|SYSTEM>/listNN/sprKK/frame_NNNN_partK.png + frame_NNNN.png + manifest.json for every SPR table of every load list.
    Returns (stats, usage)."""
    st, usage, done = Stats(), {}, {}
    L = A.load_lists()
    for ld in sorted(L):
        if ld == "SYS":
            continue
        for idx, lst in enumerate(L[ld]):
            tabs = [(r, d) for r, d in A.list_blocks(ld, idx) if (r["kind"] & 0xF0) == 0x30 and d and len(d) >= 4]
            if not tabs or (only and only != "%s:%d" % (ld, idx)):
                continue
            wt = wpp_tables(lst)
            cands = None
            st["lists"] += 1
            for r, d in tabs:
                kind = r["kind"]
                nfr = struct.unpack_from("<H", d, 2)[0] // 4
                frames = parse_spr(d)
                dst = os.path.join(out, "spr", area_name(ld), "list%02d" % idx, "spr%02X" % kind)
                st["tables"] += 1
                st["frames_total"] += nfr
                st["parts_total"] += sum(len(f) for f in frames)
                link = next((t for t in wt if len(t[2]) == nfr), None)
                if link:
                    v = ctx_get(ld, idx)
                    info = {"wpp": "%s table %d" % (link[0], link[1]), "note": "parts cut from the decoded WPP frame of the same number"}
                    sig = table_signature(d, v, 0, b"".join(link[2]))
                    st["wpp_tables"] += 1
                    if sig in done:
                        write(os.path.join(dst, "manifest.json"), json.dumps({"same_as": done[sig], "source": info}, indent=1).encode())
                        st["tables_deduplicated"] += 1
                        continue
                    done[sig] = os.path.relpath(dst, os.path.join(out, "spr")).replace(os.sep, "/")
                    export_table(dst, ld, idx, kind, d, v, 0, [wpp_plane(o) for o in link[2]], info, usage, st)
                    continue
                import hashlib
                if (kind, hashlib.md5(d).hexdigest()) in shared_tables():
                    cs = shared_candidates(ld)
                else:
                    if cands is None:
                        cands = candidates(ld, idx)
                    cs = cands
                parts = [p for f in frames for p in f if p[8] and p[9]]
                res = infer_base(cs, parts) if parts else None
                if res is None:
                    st["unresolved_tables"] += 1
                    write(os.path.join(dst, "manifest.json"), json.dumps(
                        {"load_list": "%s list %d" % (area_name(ld), idx), "kind": "0x%02X" % kind, "frames": nfr,
                         "source": "no loaded page data covers the part rectangles under any tpage offset"}, indent=1).encode())
                    continue
                ci, base, info = res
                key = cs[ci][0]
                info = dict(info)
                info["sys_list"] = key[2]
                info["context"] = "resident" if key[1] == "res" else "list"
                info["ties"] = [[cs[c][0][2], b] for c, b in info["ties"]]
                if info["context"] == "resident":
                    sig = table_signature(d, cs[ci][1], base)
                    if sig in done:
                        write(os.path.join(dst, "manifest.json"), json.dumps({"same_as": done[sig], "source": info}, indent=1).encode())
                        st["tables_deduplicated"] += 1
                        continue
                    done[sig] = os.path.relpath(dst, os.path.join(out, "spr")).replace(os.sep, "/")
                export_table(dst, ld, idx, kind, d, cs[ci][1], base, None, info, usage, st, info.get("alt_offsets", ()))
            log("%s list %d: %d tables" % (area_name(ld), idx, len(tabs)))
    return st, usage


# ------------------------------------------------------------------ flat pages, swatches, snapshots
def page_rows_8(d, w, h):
    return [d[y * w * 2:(y + 1) * w * 2] for y in range(h)]


def page_rows_4(d, w, h):
    n = bytearray(w * h * 4)
    n[0::2] = bytes(d[:w * h * 2]).translate(_LO)
    n[1::2] = bytes(d[:w * h * 2]).translate(_HI)
    return [bytes(n[y * w * 4:(y + 1) * w * 4]) for y in range(h)]


def page_rows_16(d, w, h):
    ws = memoryview(bytes(d[:w * h * 2])).cast("H")
    return [words_to_rgb(ws[y * w:(y + 1) * w]) for y in range(h)]


def swatch(d, w, h, cell=4):
    """Palette block as a grid of colours (each colour cell x cell px)."""
    ws = memoryview(bytes(d[:w * h * 2])).cast("H")
    rows = []
    for y in range(h):
        line = words_to_rgb(ws[y * w:(y + 1) * w])
        big = b"".join(line[i * 3:i * 3 + 3] * cell for i in range(w))
        rows += [big] * cell
    return png_rgb(w * cell, h * cell, rows)


def clut_word(x, y):
    return (x // 16) | (y << 6)


def export_screens(out, usage, st, log=print, only=None):
    """OUT/screens/<AREA|SYSTEM>/listNN/: vram.png (the whole 1024 x 512 VRAM of the list, RGB555) and one PNG per GRPX record: pages with the
    palette the list pairs them with (8 bpp: the 256 colour CLUT record behind them; 4 bpp atlases: the palette most SPR parts use on them),
    palette blocks as swatches.  AREA lists are shown on top of the resident SYS pages, SYSTEM lists alone."""
    L = A.load_lists()
    for ld in sorted(L):
        if only and ld not in only:
            continue
        for idx, lst in enumerate(L[ld]):
            if not any(is_grpx(r) for r in lst):
                continue
            dst = os.path.join(out, "screens", area_name(ld), "list%02d" % idx)
            if ld == "SYS":
                v = Vram()
                load_list(v, lst)
            else:
                v = ctx_get(ld, idx)
            write(os.path.join(dst, "vram.png"), v.snapshot_png())
            st["snapshots"] += 1
            st["screen_lists"] += 1
            pr = pair_cluts(lst)
            for i, r in enumerate(lst):
                if not is_grpx(r):
                    continue
                d = resource(r["name"])
                w, h = r["w"], r["h"]
                if d is None or len(d) < w * h * 2:
                    st["missing_pages"] += 1
                    continue
                stem = os.path.basename(r["name"]).rsplit(".", 1)[0]
                if is_clut_rec(r):
                    write(os.path.join(dst, stem + "_palettes.png"), swatch(d, w, h))
                    st["palette_blocks"] += 1
                    continue
                st["pages"] += 1
                if i in pr:
                    c = lst[pr[i]]
                    ws = struct.unpack_from("<256H", resource(c["name"]), 0)
                    pal = b"".join(RGB[x] for x in ws)
                    u8 = [(n, k) for k, n in (usage.get(r["name"]) or {}).items() if k[0] == 1]
                    if u8:       # 8 bpp parts of an SPR table cut this page: use their palette
                        pal = max(u8)[1][2]
                    write(os.path.join(dst, stem + ".png"), png_idx(w * 2, h, page_rows_8(d, w, h), pal, b""))
                    st["pages_8bpp_paired"] += 1
                    continue
                u = usage.get(r["name"])
                if u:
                    (mode, clut, pal), _ = u.most_common(1)[0]
                    if mode == 0:
                        write(os.path.join(dst, stem + "_4bpp.png"), png_idx(w * 4, h, page_rows_4(d, w, h), pal, b""))
                    else:
                        write(os.path.join(dst, stem + "_8bpp.png"), png_idx(w * 2, h, page_rows_8(d, w, h), pal, b""))
                    st["pages_by_sprite_palette"] += 1
                    continue
                write(os.path.join(dst, stem + "_16bpp.png"), png_rgb(w, h, page_rows_16(d, w, h)))
                gray = bytes(i * 17 for i in range(16))
                write(os.path.join(dst, stem + "_4bpp_gray.png"), png_idx(w * 4, h, page_rows_4(d, w, h), b"".join(bytes((g, g, g)) for g in gray), b""))
                st["pages_unpaired"] += 1
        log("screens %s done" % area_name(ld))


# ------------------------------------------------------------------ everything
def export_all(out, log=print):
    """Walks every load list: OUT/spr (atlas cuts + composed frames) and OUT/screens (pages, palette blocks, VRAM snapshots).
    Returns the counters."""
    if not A.load_lists():
        raise SystemExit("vram: the load lists (SYS/LD0*.BIN) and the disc image are needed")
    st, usage = export_spr(out, log=log)
    export_screens(out, usage, st, log=log)
    st["lists_total"] = sum(len(v) for v in A.load_lists().values())
    return st


def stats_only():
    st = Counter()
    for ld, lists in A.load_lists().items():
        for idx, lst in enumerate(lists):
            st["lists"] += 1
            st["pages"] += sum(1 for r in lst if is_grpx(r) and not is_clut_rec(r))
            for r, d in A.list_blocks(ld, idx):
                if (r["kind"] & 0xF0) == 0x30 and d and len(d) >= 4:
                    fr = parse_spr(d)
                    st["spr_tables"] += 1
                    st["spr_frames"] += len(fr)
                    st["spr_parts"] += sum(len(f) for f in fr)
    return st


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--all", metavar="OUT", help="export every load list to OUT/spr and OUT/screens")
    ap.add_argument("--stats", action="store_true", help="count lists, pages, SPR tables/frames/parts")
    ap.add_argument("--fs", help="extracted disc tree (default game/disc/fs or $TOMBI_FS)")
    ap.add_argument("--img", help="disc image (default Tombi.bin next to the fs directory or $TOMBI_IMG)")
    ap.add_argument("cmd", nargs="*", help="snapshot LIST OUT.png   (LIST = AREA00:1 or SYSTEM:2)")
    a = ap.parse_args(argv)
    if a.fs:
        A.FS = os.path.abspath(a.fs)
    if a.img:
        A.IMG = os.path.abspath(a.img)
    if a.stats:
        print(dict(stats_only()))
        return 0
    if a.all:
        st = export_all(a.all)
        print(dict(st))
        return 0
    if len(a.cmd) == 3 and a.cmd[0] == "snapshot":
        nm, i = a.cmd[1].split(":")
        ld = "SYS" if nm.upper().startswith("SYS") else "AR" + nm[-2:]
        lst = A.load_lists()[ld][int(i)]
        if ld == "SYS":
            v = Vram()
            load_list(v, lst)
        else:
            v = ctx_get(ld, int(i))
        write(a.cmd[2], v.snapshot_png())
        print("%s list %s: %d records -> %s" % (nm, i, len(v.recs), a.cmd[2]))
        return 0
    ap.print_help()
    return 1


if __name__ == "__main__":
    sys.exit(main())
