"""Palette inference for the GRPX pages that have no CLUT record of their own (4 bpp pages: the game loads them next to palette
rows that live elsewhere in VRAM).  Used by gfx_core (Surface palette + status text) and by the exporters.

Methods, in this order (the first that gives an answer wins):
  embedded  the page holds its own palettes in its last row (NOW LOADING: A00900.000, row 71)
  sprites   SPR parts (u,v,w,h,clut,tpage) of the area tables that lie inside the page vote for the CLUT word they use
  match     tiles of the page equal (as index patterns) to tiles of WPP sprites that decode with a known palette adopt that palette
  score     every known 16 colour row is scored by how smooth the page looks with it (adjacent-pixel colour distance, key colours
            0x7C1F and black/white noise penalised); the best one is chosen
Result: Guess(ref, method, label, votes).  The caller keeps manual overrides (Core.pal_choice) above everything."""
import struct
from collections import Counter

import area000 as A
import vram


class Guess:
    def __init__(self, ref, method, label, votes=0):
        self.ref, self.method, self.label, self.votes = ref, method, label, votes

    def __repr__(self):
        return "Guess(%s, %s, %s, %d)" % (self.ref and self.ref.key(), self.method, self.label, self.votes)


def _w2rgb(w):
    return ((w & 31) << 3, (w >> 5 & 31) << 3, (w >> 10 & 31) << 3)


# ------------------------------------------------------------------ sprites vote
_VOTES = {}


def _page_table(core):
    """[(page name, x words, y, w words, h)] of every 4 bpp unpaired page of every list, by list key."""
    out = {}
    for ld, lists in core.lists.items():
        for k, lst in enumerate(lists):
            out[(ld, k)] = [(r["name"], r["arg"] & 0xFFFF, r["arg"] >> 16, r["w"], r["h"]) for i, r, bpp, cl in core.grpx(lst) if bpp == 4]
    return out


def sprite_votes(core):
    """{page name: Counter((clut x, clut y, area))} from the SPR tables of all areas against the pages of the lists they are loaded with."""
    key = id(core.lists)
    if key in _VOTES:
        return _VOTES[key]
    pt = _page_table(core)
    votes = {}
    for ld, lists in core.lists.items():
        if ld == "SYS":
            continue
        for idx in range(len(lists)):
            pages = list(pt.get((ld, idx), [])) + list(pt.get((ld, 0), [])) + list(pt.get(("SYS", 0), []))
            for k in vram.sys_lists_for(ld, idx):
                pages += pt.get(("SYS", k), [])
            if not pages:
                continue
            for r, d in A.list_blocks(ld, idx):
                if (r["kind"] & 0xF0) != 0x30 or not d or len(d) < 8:
                    continue
                try:
                    frames = vram.parse_spr(d)
                except struct.error:
                    continue
                for parts in frames:
                    for p in parts:
                        try:
                            mode, x0, y0, x1, y1 = vram.rect_words(p, 0)
                        except Exception:
                            continue
                        if mode != 0:
                            continue
                        c = vram.wpp_clut(p[2])
                        cx, cy = (c & 0x3F) * 16, c >> 6 & 0x1FF
                        for nm, px, py, pw, ph in pages:
                            if px <= x0 and x1 < px + pw and py <= y0 and y1 < py + ph:
                                votes.setdefault(nm, Counter())[(cx, cy, ld)] += 1
    _VOTES[key] = votes
    return votes


def _ref_for(core, cx, cy, ld):
    order = []
    if ld and ld != "SYS" and ld in core.lists:
        order = [(ld, k) for k in range(len(core.lists[ld]))]
    order += [("SYS", 0), ("SYS", 8)]
    return core.clut_lookup(cx, cy, 16, order)


# ------------------------------------------------------------------ embedded
def embedded(core, name, d, w_words, h):
    """A palette block in the last row of the page: >= 16 words of valid colours behind a zero block, the rows above it empty."""
    if h < 9:
        return None
    last = d[(h - 1) * w_words * 2:h * w_words * 2]
    if any(d[(h - 3) * w_words * 2:(h - 1) * w_words * 2]) or not any(last):
        return None
    ws = struct.unpack("<%dH" % w_words, last)
    pals = [k for k in range(w_words // 16) if any(ws[k * 16:k * 16 + 16]) and ws[k * 16] == 0]
    if not pals:
        return None
    k = pals[0]
    from gfx_core import PalRef
    return Guess(PalRef(name, (h - 1) * w_words + k * 16, 16), "embedded", "%s row %d, palette %d" % (name.rsplit("/", 1)[-1], h - 1, k))


# ------------------------------------------------------------------ plane statistics
def _plane(core, name, ld, idx):
    p = core.page_info(name, ld, idx)
    d = core.get(name)
    from gfx_core import decode_plane
    return decode_plane(d, p["w"], p["h"], 4), p["w"] * 4, p["h"]


def pair_matrix(pl, W, H, step=2):
    """16 x 16 counts of horizontally adjacent index pairs (sampled every `step` rows) of the plane."""
    m = Counter()
    for y in range(0, H, step):
        row = pl[y * W:(y + 1) * W]
        for a, b in zip(row, row[1:]):
            if a != b or a:
                m[(a, b)] += 1
    return m


def known_rows(core):
    """[(label, PalRef)] every 16 colour row of the CLUT records (resident, SYS list 8/9 CLUTs and the first CLUT strip of one area)."""
    if getattr(core, "_known_rows", None) is not None:
        return core._known_rows
    from gfx_core import PalRef
    out, seen = [], set()
    lists = [("SYS", 0), ("SYS", 4), ("SYS", 8), ("SYS", 5), ("SYS", 7)]
    ar = sorted(l for l in core.lists if l != "SYS")
    if ar:
        lists.append((ar[0], 1))
    for ld, k in lists:
        if k >= len(core.lists[ld]):
            continue
        for i, r, bpp, cl in core.grpx(core.lists[ld][k]):
            if bpp != 16 or r["name"] in seen:
                continue
            seen.add(r["name"])
            w, h = r["w"], r["h"]
            d = core.get(r["name"])
            ws = struct.unpack_from("<%dH" % (len(d) // 2), d)
            for y in range(h):
                for s in range(w // 16):
                    ref = PalRef(r["name"], y * w + s * 16, 16)
                    if any(ws[y * w + s * 16 + 1:y * w + s * 16 + 16]):
                        out.append(("%s row %d.%d" % (r["name"].rsplit("/", 1)[-1], y, s), ref, ws[y * w + s * 16:y * w + s * 16 + 16]))
    core._known_rows = out
    return out


def score_palette(ws, m):
    """Lower = better.  Mean colour distance of the adjacent pixel pairs + penalties (key colours, repeated black, unused-colour noise)."""
    rgb = [_w2rgb(w) for w in ws]
    tot = sum(m.values()) or 1
    s = 0.0
    for (a, b), n in m.items():
        ca, cb = rgb[a], rgb[b]
        s += n * (abs(ca[0] - cb[0]) + abs(ca[1] - cb[1]) + abs(ca[2] - cb[2]))
    s /= tot
    used = Counter()
    for (a, b), n in m.items():
        used[a] += n
        used[b] += n
    pen = 0
    for i, n in used.items():
        w = ws[i]
        r, g, b = w & 31, w >> 5 & 31, w >> 10 & 31
        if i and r > 24 and b > 24 and g < 8:        # magenta key
            pen += 120 * n / tot
        if i and w in (0, 0x8000):
            pen += 60 * n / tot
    cols = {ws[i] & 0x7FFF for i in used if i}
    if len(cols) < len([i for i in used if i]):
        pen += 15
    return s + pen


# ------------------------------------------------------------------ match against known sprites
def _tiles(pl, W, H):
    out = {}
    for ty in range(0, H - 7, 8):
        for tx in range(0, W - 7, 8):
            t = bytes(pl[(ty + j) * W + tx:(ty + j) * W + tx + 8][i] for j in range(8) for i in range(8))
            if len(set(t)) >= 4:
                out.setdefault(t, 0)
    return out


def match_wpp(core, tiles):
    """Votes of the palettes of the WPP frames that contain the same 8x8 index tiles (flipless): {PalRef: n}.  Cached tile table."""
    tab = getattr(core, "_wpp_tiles", None)
    if tab is None:
        tab = {}
        for lst_ld, lists in core.lists.items():
            for k, lst in enumerate(lists):
                for r in lst:
                    nm = r["name"]
                    if (r["kind"] & 0xF0) != 0x60 or r["type"] == 0xFFFFFFFF or not nm.endswith(".000") or nm in tab.get("_done", ()):
                        continue
                    tab.setdefault("_done", set()).add(nm)
                    try:
                        man, files, frames = core.wpp(nm)
                    except Exception:
                        continue
                    for n in range(len(frames)):
                        try:
                            ref = core.wpp_palette(nm, n)
                            d = core.get("%s#%d" % (nm, n))
                            w, h = struct.unpack_from("<HH", d)
                            from gfx_core import decode_plane
                            pl = decode_plane(d[4:], w, h, 4)
                        except Exception:
                            continue
                        if not ref:
                            continue
                        for t in _tiles(pl, w * 4, h):
                            tab.setdefault(t, ref)
        core._wpp_tiles = tab
    c = Counter()
    for t in tiles:
        ref = tab.get(t)
        if ref is not None:
            c[ref] += 1
    return c


# ------------------------------------------------------------------ main entry
def infer(core, name, ld, idx, use_match=False):
    """Guess for the 4 bpp page `name` of list (ld, idx), or None."""
    cache = core.__dict__.setdefault("_pal_guess", {})
    key = (name, ld, idx, use_match)
    if key in cache:
        return cache[key]
    g = _infer(core, name, ld, idx, use_match)
    cache[key] = g
    return g


def _infer(core, name, ld, idx, use_match):
    p = core.page_info(name, ld, idx)
    d = core.get(name)
    g = embedded(core, name, d, p["w"], p["h"])
    if g:
        return g
    v = sprite_votes(core).get(name)
    if v:
        (cx, cy, ar), n = v.most_common(1)[0]
        ref = _ref_for(core, cx, cy, ar)
        if ref:
            return Guess(ref, "sprites", "%s, %d parts use CLUT (%d,%d)" % (ref.file.rsplit("/", 1)[-1], sum(v.values()), cx, cy), n)
    pl, W, H = _plane(core, name, ld, idx)
    if use_match:
        mv = match_wpp(core, _tiles(pl, W, H))
        if mv:
            ref, n = mv.most_common(1)[0]
            if n >= 3:
                return Guess(ref, "match", "%d tiles equal to sprites with this palette" % n, n)
    m = pair_matrix(pl, W, H)
    if not m:
        return None
    best = None
    for label, ref, ws in known_rows(core):
        s = score_palette(ws, m)
        if best is None or s < best[0]:
            best = (s, label, ref)
    if best:
        return Guess(best[2], "score", best[1], int(best[0]))
    return None
