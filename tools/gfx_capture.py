#!/usr/bin/env python3
"""Sprites exactly as the game draws them: a capture of the native port (port2 --dump-draws) for the graphics editor.

The port logs every distinct textured primitive it rasterises (port2/shims/drawdump.c): texture page, CLUT, the box of
its texture coordinates, the screen box and the texels + palette at that moment.  `run` drives the port headless
through the title and every room of the debug area select (the same pad script as port2/tools/sweep_areas.py) and
merges the logs into one deduplicated store; gfx_core lists the store as "Captured from the game" (exact palette,
page, size) and tools/gfx_measure.py compares the editor's own decoding against it.

  python3 tools/gfx_capture.py run [--exe build/port2/tombi2] [--data DIR | --disc IMG] [--out FILE] [--jobs N] [AREA[:SECTION] ...]
  python3 tools/gfx_capture.py info [FILE]
  python3 tools/gfx_capture.py export [FILE] OUTDIR          every captured sprite as PNG

Record ("DRW1", little endian): u32 magic, u16 area, section, u32 frame, u16 tx, ty, u8 depth (0 4-bit, 1 8-bit, 2 15-bit),
kind (0 tri, 1 quad, 2 rect), semi, pad, u16 clutX, clutY, u0, v0, w, h, tpage, clut, s16 sx, sy, sw, sh, u32 palette
count (16/256/0), the palette words, then w*h index bytes (15-bit: w*h words).  The store is the same records, gzip'd."""
import gzip, hashlib, json, os, struct, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
MAGIC = 0x31575244
HDR = struct.Struct("<12I")

# title -> debug select (GAME.selectedArea/Section poked) -> room, then walk / jump / attack / open the menu
PAD = ("1000:start,1300:circle:3,1500:right:300,1800:cross:3x8/40,2150:left:300,2450:square:3x8/40,"
       "2800:right+cross:3x30/60,3100:up:40,3160:down:40,3220:right:200,3450:cross:3x8/40,3600:start:3,3700:right:3x4/30")
FRAMES = 3900


class Draw:
    __slots__ = ("area", "section", "frame", "tx", "ty", "depth", "kind", "semi", "cx", "cy", "u", "v", "w", "h",
                 "tpage", "clut", "sx", "sy", "sw", "sh", "pal", "tex", "raw", "_key", "count")

    def __init__(self, raw, hdr, pal, tex):
        m, ar, fr, txy, dk, cxy, uv, wh, tc, sxy, swh, np_ = hdr
        self.raw = raw
        self.area, self.section = ar & 0xFFFF, ar >> 16
        self.frame = fr
        self.tx, self.ty = txy & 0xFFFF, txy >> 16
        self.depth, self.kind, self.semi = dk & 255, (dk >> 8) & 255, (dk >> 16) & 255
        self.cx, self.cy = cxy & 0xFFFF, cxy >> 16
        self.u, self.v = uv & 0xFFFF, uv >> 16
        self.w, self.h = wh & 0xFFFF, wh >> 16
        self.tpage, self.clut = tc & 0xFFFF, tc >> 16
        s16 = lambda x: x - 0x10000 if x & 0x8000 else x
        self.sx, self.sy, self.sw, self.sh = s16(sxy & 0xFFFF), s16(sxy >> 16), s16(swh & 0xFFFF), s16(swh >> 16)
        self.pal, self.tex = pal, tex          # tuple of words; bytes (indices) or tuple of words (15-bit)
        self._key = None
        self.count = 1

    @property
    def bpp(self):
        return (4, 8, 16)[self.depth]

    def key(self):
        """Identity without room / frame / screen position: page, CLUT position, UV box and content."""
        if self._key is None:
            h = hashlib.blake2b(digest_size=12)
            h.update(struct.pack("<9H", self.tx, self.ty, self.depth, self.cx, self.cy, self.u, self.v, self.w, self.h))
            h.update(self.raw[HDR.size:])
            self._key = h.digest()
        return self._key

    def words(self):
        """Colour words, row by row (palette applied)."""
        if self.depth == 2:
            return list(self.tex)
        p = self.pal
        return [p[i] for i in self.tex]

    def rgba(self):
        """RGBA bytes; colour word 0x0000 is transparent (the GPU skips it)."""
        import vram
        return b"".join(map(vram.RGBA.__getitem__, self.words()))

    def image(self):
        from PIL import Image
        return Image.frombytes("RGBA", (self.w, self.h), self.rgba())

    def page_xy(self):
        """VRAM word position of the texel box: (x, y, words wide)."""
        d = (4, 2, 1)[self.depth]
        return self.tx + self.u // d, self.ty + self.v, (self.u + self.w + d - 1) // d - self.u // d

    def label(self):
        return "%dx%d  %dbpp  page %d,%d  uv %d,%d  clut %d,%d" % (self.w, self.h, self.bpp, self.tx, self.ty, self.u, self.v, self.cx, self.cy)


def parse(data):
    """[Draw] of a raw (or gzip'd) record stream."""
    if data[:2] == b"\x1f\x8b":
        data = gzip.decompress(data)
    out, o, n = [], 0, len(data)
    while o + HDR.size <= n:
        hdr = HDR.unpack_from(data, o)
        if hdr[0] != MAGIC:
            break
        w, h, np_ = hdr[7] & 0xFFFF, hdr[7] >> 16, hdr[11]
        size = HDR.size + np_ * 2 + (w * h if np_ else w * h * 2)
        if o + size > n:
            break
        pal = struct.unpack_from("<%dH" % np_, data, o + HDR.size) if np_ else ()
        t0 = o + HDR.size + np_ * 2
        tex = data[t0:t0 + w * h] if np_ else struct.unpack_from("<%dH" % (w * h), data, t0)
        out.append(Draw(data[o:o + size], hdr, pal, tex))
        o += size
    return out


def load(path):
    with open(path, "rb") as f:
        return parse(f.read())


def merge(draws):
    """Deduplicated by key() (first occurrence kept, `count` = how many logs had it)."""
    seen, out = {}, []
    for d in draws:
        k = d.key()
        if k in seen:
            seen[k].count += 1
        else:
            seen[k] = d
            out.append(d)
    return out


def save(path, draws):
    tmp = path + ".tmp"
    with gzip.open(tmp, "wb", compresslevel=6) as f:
        for d in draws:
            f.write(d.raw)
    os.replace(tmp, path)


def default_store():
    """Where the editor looks for the capture: $TOMBI_GFX_CAPTURE, else <repo or exe folder>/gfx_capture.drw.gz."""
    p = os.environ.get("TOMBI_GFX_CAPTURE")
    if p:
        return p
    base = os.path.dirname(sys.executable) if getattr(sys, "frozen", False) else ROOT
    return os.path.join(base, "gfx_capture.drw.gz")


# ------------------------------------------------------------------ ground truth for the editor
def _vram_indices(d_words, x, y, w, h, tp_x, tp_y, depth, u, v, bw, bh):
    """Indices of the UV box (u, v, bw, bh) of a texture page at (tp_x, tp_y) read from a page image `d_words` placed at VRAM (x, y),
    w x h words; None when the box leaves the page."""
    out = bytearray()
    for j in range(bh):
        yy = tp_y + ((v + j) & 255) - y
        if not 0 <= yy < h:
            return None
        row = yy * w
        for i in range(bw):
            uu = (u + i) & 255
            if depth == 0:
                xx = tp_x + (uu >> 2) - x
                if not 0 <= xx < w:
                    return None
                out.append((d_words[row + xx] >> ((uu & 3) * 4)) & 15)
            else:
                xx = tp_x + (uu >> 1) - x
                if not 0 <= xx < w:
                    return None
                out.append((d_words[row + xx] >> ((uu & 1) * 8)) & 255)
    return bytes(out)


class Truth:
    """The capture as the editor uses it: which bit depth and CLUTs the game draws each page with, and the captured part images."""

    VERSION = 1

    def __init__(self, path):
        self.path = path
        self.draws = [d for d in load(path) if d.depth < 2]
        self.by_part = {}
        for d in self.draws:
            self.by_part.setdefault((d.area, d.u, d.v, d.w, d.h, d.cx, d.cy), []).append(d)
        self._pages = None

    def part(self, area, box, cx, cy, tx=None, ty=None):
        """The captured draw of a sprite part: UV box (u, v, w, h) of the part (the quad's box is one texel larger), CLUT position;
        prefers the same texture page.  None when the game was not seen drawing it."""
        u, v, w, h = box
        c = self.by_part.get((area, u, v, w + 1, h + 1, cx, cy)) or self.by_part.get((area, u, v, w, h, cx, cy))
        if not c:
            return None
        for d in c:
            if d.tx == tx and d.ty == ty:
                return d
        return c[0]

    def parts(self, area, box, cx, cy):
        u, v, w, h = box
        return (self.by_part.get((area, u, v, w + 1, h + 1, cx, cy)) or []) + (self.by_part.get((area, u, v, w, h, cx, cy)) or [])

    def pages(self, core):
        """{page name: [[depth, cx, cy, x, y, w, h, palette words as hex, weight]]} (x, y, w, h in page pixels at that depth): every captured draw whose texels
        are the page's own texels; cached next to the capture."""
        if self._pages is not None:
            return self._pages
        cache = self.path + ".pages.json"
        try:
            st = os.stat(self.path)
            j = json.load(open(cache))
            if j.get("version") == self.VERSION and j.get("size") == st.st_size and j.get("mtime") == int(st.st_mtime):
                self._pages = j["pages"]
                return self._pages
        except (OSError, ValueError):
            pass
        byxy = {}
        for d in self.draws:
            byxy.setdefault((d.tx, d.ty), []).append(d)
        out, seen = {}, set()
        for ld, lists in core.lists.items():
            for lst in lists:
                for i, r, bpp, cl in core.grpx(lst):
                    if bpp == 16 or r["name"] in seen:
                        continue
                    seen.add(r["name"])
                    x, y, w, h = r["arg"] & 0xFFFF, r["arg"] >> 16, r["w"], r["h"]
                    try:
                        data = core.original(r["name"])
                    except Exception:
                        continue
                    if len(data) < w * h * 2:
                        continue
                    words = struct.unpack_from("<%dH" % (w * h), data)
                    acc = {}
                    for (tx, ty), ds in byxy.items():
                        if not (x - 256 < tx < x + w and y - 256 < ty < y + h):
                            continue
                        for d in ds:
                            wx, wy, ww = d.page_xy()
                            if not (x <= wx and wx + ww <= x + w and y <= wy and wy + d.h <= y + h):
                                continue
                            if _vram_indices(words, x, y, w, h, tx, ty, d.depth, d.u, d.v, d.w, d.h) != bytes(d.tex):
                                continue
                            ppw = (4, 2)[d.depth]
                            q = 1 if d.kind != 2 and d.w > 1 and d.h > 1 else 0      # a polygon's UV box has one extra (undrawn) texel
                            k = (d.depth, d.cx, d.cy, (tx - x) * ppw + d.u, ty - y + d.v, d.w - q, d.h - q, "".join("%04x" % c for c in d.pal))
                            acc[k] = acc.get(k, 0) + d.w * d.h
                    if acc:
                        out[r["name"]] = [list(k) + [n] for k, n in sorted(acc.items())]
        self._pages = out
        try:
            st = os.stat(self.path)
            json.dump(dict(version=self.VERSION, size=st.st_size, mtime=int(st.st_mtime), pages=out), open(cache, "w"))
        except OSError:
            pass
        return out


_truth = {}


def truth(path=None):
    """Truth of the capture at `path` (default_store()), or None when there is no capture."""
    path = path or default_store()
    if path not in _truth:
        _truth[path] = Truth(path) if os.path.isfile(path) else None
    return _truth[path]


# ------------------------------------------------------------------ driving the port
def rooms():
    sys.path.insert(0, os.path.join(HERE, "psxemu"))
    from rooms import ROOM_COUNTS, DOOR_ROOMS
    return [(a, s) for a, n in ROOM_COUNTS for s in range(n) if (a, s) not in DOOR_ROOMS]


def _run(exe, args, data, disc, frames, log):
    fd, path = tempfile.mkstemp(suffix=".drw")
    os.close(fd)
    cmd = [exe, "--headless", "--skip-movies", "--frames", str(frames), "--dump-draws", path] + args
    if data:
        cmd += ["--data", data]
    if disc:
        cmd += ["--disc", disc]
    try:
        try:
            subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd=ROOT, timeout=900)
        except subprocess.TimeoutExpired:
            log("timeout: " + " ".join(args))
        return merge(load(path))
    finally:
        os.unlink(path)


def run(exe, out, data=None, disc=None, only=None, jobs=4, log=print):
    jobs_l = [("title", [], 1500)] if not only else []
    for a, s in rooms():
        if only and not any(o == a or o == (a, s) for o in only):
            continue
        jobs_l.append(("%02d:%d" % (a, s), ["--poke", "900:1F8001B4:1", "--poke", "1250:8009C960:%X:2" % a,
                                            "--poke", "1250:8009C962:%X:2" % s, "--pad-script", PAD], FRAMES))
    allv = load(out) if (only and os.path.exists(out)) else []
    with ThreadPoolExecutor(jobs) as ex:
        futs = [(nm, ex.submit(_run, exe, args, data, disc, fr, log)) for nm, args, fr in jobs_l]
        for nm, f in futs:
            r = f.result()
            allv = merge(allv + r)
            log("%-6s %5d draws, %6d total" % (nm, len(r), len(allv)))
    save(out, allv)
    return allv


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="c", required=True)
    s = sub.add_parser("run")
    s.add_argument("--exe", default=os.path.join(ROOT, "build", "port2", "tombi2"))
    s.add_argument("--data")
    s.add_argument("--disc")
    s.add_argument("--out", default=default_store())
    s.add_argument("--jobs", type=int, default=4)
    s.add_argument("rooms", nargs="*")
    s = sub.add_parser("info")
    s.add_argument("file", nargs="?", default=default_store())
    s = sub.add_parser("export")
    s.add_argument("file", nargs="?", default=default_store())
    s.add_argument("outdir")
    a = ap.parse_args(argv)
    if a.c == "run":
        only = [tuple(map(int, r.split(":"))) if ":" in r else int(r) for r in a.rooms]
        run(a.exe, a.out, a.data, a.disc, only, a.jobs)
        return 0
    ds = load(a.file)
    if a.c == "info":
        from collections import Counter
        print(len(ds), "draws")
        print("by depth:", dict(Counter(d.bpp for d in ds)))
        print("by area:", dict(sorted(Counter(d.area for d in ds).items())))
        return 0
    os.makedirs(a.outdir, exist_ok=True)
    for i, d in enumerate(ds):
        d.image().save(os.path.join(a.outdir, "%05d_a%02d_%dbpp_%d_%d_%dx%d.png" % (i, d.area, d.bpp, d.tx, d.ty, d.w, d.h)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
