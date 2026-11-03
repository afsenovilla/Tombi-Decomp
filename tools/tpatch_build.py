#!/usr/bin/env python3
"""Compiles a text patch (*.tombipatch.json) into a .tpatch: the binary form tombi2 --text-patch applies natively
(port2/runtime/tpatch.c, docs/PORT.md "Standalone (no launcher) mode"), with no Python on the player's machine (Android).

The JSON holds only our text; turning it into game files needs the WFM encoder, the glyph pool of the disc's fonts and the
file-table updates of tools/data_patch.py.  This tool runs that exact pipeline once on a developer's extracted disc
(DATA_DIR = data/ of tools/extract_data.py, pristine) and stores the result as a COPY/LITERAL delta against the files of
the same disc, so the literals hold ONLY our own data (our encoded text: dialogue word indices, headers, sizes, the table
values computed from our text) and everything that comes from the disc is an operation the C applies to the player's own
files at load time:
  - COPY: every run of 12+ bytes found in the source files (and, after the check below, every shorter run of 8+);
  - GLYPH: a glyph bitmap of a WFM that is not a byte copy of a disc glyph (glyphShift: the 16 px ',' and 'u'-acute moved
    sideways, the 8 px accented letters moved up; composed accented capitals): built row by row from rows of the disc's
    glyphs (source glyph, row, horizontal shift), so no pixel is stored in the .tpatch.
The build fails when a literal run still holds 8+ bytes that occur anywhere in the source files (game data), or a file
needs more than --max-literal literal bytes; it prints what the remaining literal bytes are.  The C side checks the
sha1 of every source file and of every rebuilt file, so a different disc never gets a half-applied patch.

  python3 tools/tpatch_build.py data/ patches/es_mejorada.tombipatch.json -o patches/es_mejorada.tpatch
  python3 tools/tpatch_build.py data/ patches/en_cut_content.tombipatch.json -o patches/en_cut_content.tpatch
  python3 tools/tpatch_build.py Tombi.bin my.tombipatch.json -o my.tpatch      (from the disc image: slim_extract first)

Format (little endian): "TPCH" u32 version=2, u32 n_sources, u32 n_entries, char json_sha1[40], u16 title_len, title
  source: u16 path_len, path, u32 size, u8 sha1[20]
  entry:  u16 path_len, path, u32 size, u8 sha1[20], u8 n_ranges, n_ranges * (u32 lba, u32 sectors), u32 n_ops, ops
          (n_ranges 0: not a disc file, e.g. a --patch-tex TBTX; ranges: where the rebuilt file is read on the CD,
           the original place first, then the LBA past the end of the disc a grown file moved to)
  op:     u8 0 = COPY u16 source, u32 offset, u32 length | u8 1 = LITERAL u32 length, bytes
        | u8 2 = GLYPH u16 width, u16 height, height * (u16 source, u32 offset, u8 row, i8 dx): writes (width*height+1)/2
          bytes of 4bpp pixels (low nibble first); output row r = row `row` of the width x height 4bpp image at `offset`
          of source `source`, moved dx pixels right (pixels moved out are dropped, the rest are 0); row 0xFF = empty
"""
import argparse, hashlib, json, os, shutil, struct, sys, tempfile
import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import extract_data as X

K, STRIDE = 12, 4
CHECK = 8          # no literal run may hold this many bytes that occur in the source files


def manifest_files(d):
    """{path: (lba, sectors, bytes)} of the 2048-stored files plus the extra ranges of grown files"""
    m = X.read_manifest(d)
    out = {}
    for lba, n, stored, nbytes, p in m["files"]:
        if stored == 2048 and not p.startswith(X.BLOB_DIR + "/"):
            out.setdefault(p, []).append((lba, n, nbytes))
    return out


class Index:
    def __init__(self, files):
        self.files = files            # [(path, bytes)]
        self.h = {}
        for si, (_, b) in enumerate(files):
            for o in range(0, len(b) - K + 1, STRIDE):
                self.h.setdefault(b[o:o + K], []).append((si, o))

    def delta(self, new, prefer=None):
        ops, lit, i = [], bytearray(), 0
        while i < len(new):
            best = None
            if i + K <= len(new):
                # an indexed block can start up to STRIDE-1 bytes after i: try each alignment, extend both ways
                for a in range(STRIDE):
                    if i + a + K > len(new):
                        break
                    for si, o in self.h.get(new[i + a:i + a + K], ())[:64]:
                        src = self.files[si][1]
                        s, t = o, i + a
                        while s > 0 and t > i and src[s - 1] == new[t - 1]:
                            s -= 1; t -= 1
                        if t != i:
                            continue
                        e = o + K
                        while e < len(src) and t + (e - s) < len(new) and src[e] == new[t + (e - s)]:
                            e += 1
                        n = e - s
                        if not best or n > best[2] or (n == best[2] and si == prefer):
                            best = (si, s, n)
                    if best:
                        break
            if best and best[2] >= K:
                if lit:
                    ops.append((1, bytes(lit))); lit = bytearray()
                ops.append((0, best[0], best[1], best[2]))
                i += best[2]
            else:
                lit.append(new[i]); i += 1
        if lit:
            ops.append((1, bytes(lit)))
        return ops


def wfm_glyphs(b):
    """[(image offset, width, height)] of the glyph bitmaps of a WFM3 file, one per distinct offset"""
    if b[:4] != b"WFM3" or len(b) < 144:
        return []
    ng, out = struct.unpack_from("<H", b, 14)[0], {}
    for i in range(ng):
        p = struct.unpack_from("<H", b, 144 + 2 * i)[0]
        if p + 8 > len(b):
            continue
        _, h, w, _ = struct.unpack_from("<4H", b, p)
        n = (w * h + 1) // 2
        if n and p + 8 + n <= len(b):
            out[p + 8] = (w, h)
    return sorted((o, w, h) for o, (w, h) in out.items())


def glyph_rows(b, o, w, h):
    px = []
    for x in b[o:o + (w * h + 1) // 2]:
        px += (x & 15, x >> 4)
    return [tuple(px[r * w:(r + 1) * w]) for r in range(h)]


def render_glyph(sources, op):
    """the bytes a GLYPH op writes (same as port2/shims/tpatch.c)"""
    _, w, h, spec = op
    px = [0] * (w * h)
    for r, sp in enumerate(spec):
        if sp is None:
            continue
        si, o, sr, dx = sp
        b = sources[si][1]
        for c in range(w):
            sc = c - dx
            if 0 <= sc < w:
                k = sr * w + sc
                x = b[o + k // 2]
                px[r * w + c] = x >> 4 if k & 1 else x & 15
    if len(px) % 2:
        px.append(0)
    return bytes(px[i] | px[i + 1] << 4 for i in range(0, len(px), 2))


class Glyphs:
    """the glyph bitmaps of the source WFMs: exact copies, and rows (with a horizontal shift) to build the others from"""

    def __init__(self, sources):
        self.sources, self.img, self.byfile, self.rowidx = sources, {}, {}, {}
        for si, (p, b) in enumerate(sources):
            if p.upper().endswith(".WFM"):
                for o, w, h in wfm_glyphs(b):
                    self.img.setdefault((w, h, b[o:o + (w * h + 1) // 2]), (si, o))
                    self.byfile.setdefault((si, w, h), []).append((si, o))
                    self.byfile.setdefault((None, w, h), []).append((si, o))

    def rows(self, key):
        """{ink of a row without its leading/trailing empty columns: (source, offset, row, first ink column)}"""
        if key not in self.rowidx:
            d = {}
            for si, o in self.byfile.get(key, ()):
                for r, R in enumerate(glyph_rows(self.sources[si][1], o, key[1], key[2])):
                    nz = [c for c, x in enumerate(R) if x]
                    if nz:
                        d.setdefault(R[nz[0]:nz[-1] + 1], (si, o, r, nz[0]))
            self.rowidx[key] = d
        return self.rowidx[key]

    def explain(self, new, o, w, h, prefer):
        """a GLYPH op that rebuilds the bitmap new[o:...] from rows of the disc's glyphs, or None"""
        spec = []
        for R in glyph_rows(new, o, w, h):
            nz = [c for c, x in enumerate(R) if x]
            if not nz:
                spec.append(None)
                continue
            ink = R[nz[0]:nz[-1] + 1]
            hit = self.rows((prefer, w, h)).get(ink) if prefer is not None else None
            hit = hit or self.rows((None, w, h)).get(ink)
            if not hit:
                return None
            spec.append((hit[0], hit[1], hit[2], nz[0] - hit[3]))
        op = (2, w, h, spec)
        return op if render_glyph(self.sources, op) == new[o:o + (w * h + 1) // 2] else None


def disc_grams(entries, sources):
    """{8-byte window of a literal run that occurs in a source file: (source, offset)}"""
    grams = set()
    for e in entries:
        for op in e[3]:
            if op[0] == 1:
                L = op[1]
                grams.update(int.from_bytes(L[i:i + CHECK], "little") for i in range(len(L) - CHECK + 1))
    found = {}
    if not grams:
        return found
    keys = np.array(sorted(grams), dtype=np.uint64)
    for si, (_, b) in enumerate(sources):
        n = len(b) - CHECK + 1
        if n <= 0:
            continue
        a = np.frombuffer(b, dtype=np.uint8)
        v = np.zeros(n, dtype=np.uint64)
        for k in range(CHECK):
            v |= a[k:k + n].astype(np.uint64) << np.uint64(8 * k)
        for i in np.nonzero(np.isin(v, keys, assume_unique=False))[0]:
            found.setdefault(int(v[i]), (si, int(i)))
    return found


def copy_disc_runs(entries, sources, found):
    """turns every literal stretch of 8+ bytes that exists in a source file into a COPY"""
    for e in entries:
        ops = []
        for op in e[3]:
            if op[0] != 1:
                ops.append(op)
                continue
            L, i, lit = op[1], 0, bytearray()
            while i < len(L):
                hit = found.get(int.from_bytes(L[i:i + CHECK], "little")) if i + CHECK <= len(L) else None
                if hit:
                    si, o = hit
                    src, n = sources[si][1], CHECK
                    while i + n < len(L) and o + n < len(src) and src[o + n] == L[i + n]:
                        n += 1
                    if lit:
                        ops.append((1, bytes(lit))); lit = bytearray()
                    ops.append((0, si, o, n))
                    i += n
                else:
                    lit.append(L[i]); i += 1
            if lit:
                ops.append((1, bytes(lit)))
        e[3] = ops


def op_len(op):
    return op[3] if op[0] == 0 else len(op[1]) if op[0] == 1 else (op[1] * op[2] + 1) // 2


def literal_kind(path, new, o):
    """what a literal byte at offset o of a rebuilt file is (for the report)"""
    if path.upper().endswith(".WFM") and new[:4] == b"WFM3":
        dpt, nd, ng = struct.unpack_from("<IHH", new, 8)
        if o < 144:
            return "WFM header (sizes, counts)"
        if o < 144 + 2 * ng:
            return "WFM glyph pointer table"
        if dpt <= o < dpt + 2 * nd:
            return "WFM dialogue pointer table"
        if o >= dpt + 2 * nd:
            return "WFM dialogue words (our text: glyph indices, codes)"
        for p in struct.unpack_from("<%dH" % ng, new, 144):
            if p <= o < p + 8:
                return "WFM glyph headers (width, height, advance)"
        return "WFM other"
    return "%s (file tables / our text)" % (path.split("/")[0] if path.upper().startswith("EXE/") else path.rsplit(".", 1)[-1].upper())


SLIM_SKIP = ("MOVIE/", "SOUND/", "ZZZ/", X.BLOB_DIR + "/")     # never read by a text patch


def slim_extract(image, out_dir, progress=None):
    """A data/ folder (tools/extract_data.py layout, full manifest.txt) with only the Form 1 files a text patch reads (no
    movies, sound, padding or raw blobs: ~190 MB instead of ~450): what build(..., partial=True) needs, from the player's
    own disc image (.bin / .cue / .iso / .chd)."""
    img = X.Image(X.bin_of_cue(image))
    try:
        entries, main0 = [], None
        for path, lba, size in img.files():
            n = (size + 2047) // 2048
            raw = path.upper().endswith((".STR", ".XA")) or X.has_form2(img, lba, n)
            entries.append((lba, n, 2352 if raw else 2048, n * 2352 if raw else size, path))
            if path.upper() == "EXE/MAIN0.EXE":
                main0 = (lba, size)
        entries.sort()
        prev, blobs = 0, []
        for lba, n, _, _, _ in entries:
            if lba > prev:
                blobs.append((prev, lba - prev))
            prev = max(prev, lba + n)
        if prev < img.sectors:
            blobs.append((prev, img.sectors - prev))
        entries = sorted(entries + [(lba, n, 2352, n * 2352, "%s/%06d.raw" % (X.BLOB_DIR, lba)) for lba, n in blobs])
        todo = [e for e in entries if e[2] == 2048 and not e[4].upper().startswith(SLIM_SKIP)]
        for k, (lba, n, _, nbytes, path) in enumerate(todo):
            dst = os.path.join(out_dir, *path.split("/"))
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with open(dst, "wb") as o:
                o.write(img.user(lba, n)[:nbytes])
            if progress:
                progress(k + 1, len(todo), path)
        sha = hashlib.sha1(img.user(main0[0], (main0[1] + 2047) // 2048)[:main0[1]]).hexdigest() if main0 else ""
        X.write_manifest(out_dir, {"sectors": img.sectors, "main0_sha1": sha, "source": os.path.basename(image), "files": entries})
    finally:
        img.close()


def _link_or_copy(src, dst):
    """copytree helper: a hard link (data_patch writes every file it changes as a new file, the source stays untouched)."""
    try:
        os.link(src, dst)
    except OSError:
        shutil.copy2(src, dst)


def build_from_disc(image, patch_path, out, max_literal=16384, log=print, progress=None):
    """build() from a disc image: a slim extraction into a temporary folder first (deleted afterwards)."""
    tmp = tempfile.mkdtemp(prefix="tpatch_disc_")
    try:
        d = os.path.join(tmp, "src")
        log("reading the disc files a text patch needs from %s" % image)
        slim_extract(image, d, progress)
        build(d, patch_path, out, max_literal, os.path.join(tmp, "work"), log, partial=True)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def build(data_dir, patch_path, out, max_literal, work=None, log=print, partial=False):
    import data_patch as DP
    patch = json.load(open(patch_path, encoding="utf-8"))
    tmp = work or tempfile.mkdtemp(prefix="tpatch_")
    pd = os.path.join(tmp, "data")
    if os.path.isdir(pd):
        shutil.rmtree(pd)
    log("copying %s -> %s" % (data_dir, pd))
    shutil.copytree(data_dir, pd, ignore=shutil.ignore_patterns(DP.ORIG), copy_function=_link_or_copy)
    r = DP.apply(pd, patch_path, log=log, partial=partial, gfx=bool(os.environ.get("TPATCH_WITH_GFX")))
    before, after = manifest_files(data_dir), manifest_files(pd)
    targets = [(p, open(os.path.join(pd, *p.split("/")), "rb").read(), after[p]) for p in r["files"]]
    gfx = os.path.join(pd, DP.GFX_TBTX)
    if os.path.isfile(gfx) and not os.environ.get("TPATCH_WITH_GFX"):
        log("note: %s not compiled (its redraw pixels would be literals: game-derived art); the port builds no Spanish "
            "redraws natively yet" % DP.GFX_TBTX)
    # sources: the original of every target, every WFM (the glyph pool) and every file of the same folders
    dirs = {p.split("/")[0] for p, _, _ in targets}
    src_paths = sorted(p for p in before if p.split("/")[0] in dirs or p.upper().endswith(".WFM"))
    sources = [(p, open(os.path.join(data_dir, *p.split("/")), "rb").read()) for p in src_paths]
    sidx = {p: i for i, (p, _) in enumerate(sources)}
    log("indexing %d source files (%d KB)" % (len(sources), sum(len(b) for _, b in sources) >> 10))
    idx, gp = Index(sources), Glyphs(sources)
    entries = []
    for p, new, ranges in targets:
        segs = []      # the glyph bitmaps of a WFM: a COPY of the same disc glyph, else a GLYPH op (never literal pixels)
        if p.upper().endswith(".WFM"):
            for o, w, h in wfm_glyphs(new):
                n = (w * h + 1) // 2
                hit = gp.img.get((w, h, new[o:o + n]))
                op = (0, hit[0], hit[1], n) if hit else gp.explain(new, o, w, h, sidx.get(p))
                if not op:
                    raise SystemExit("%s: glyph bitmap at %#x (%dx%d) is not made of rows of the disc's glyphs" % (p, o, w, h))
                segs.append((o, o + n, op))
        ops, pos = [], 0
        for a, e, op in segs:
            if a < pos:
                raise SystemExit("%s: overlapping glyph bitmaps at %#x" % (p, a))
            if a > pos:
                ops += idx.delta(new[pos:a], sidx.get(p))
            ops.append(op)
            pos = e
        if pos < len(new):
            ops += idx.delta(new[pos:], sidx.get(p))
        # the original range first (manifest order is by LBA; the moved copy is past the end of the disc)
        entries.append([p, new, sorted((lba, n) for lba, n, _ in ranges), ops])
    log("checking the literals against the source files")
    copy_disc_runs(entries, sources, disc_grams(entries, sources))
    left = disc_grams(entries, sources)
    if left:
        raise SystemExit("FAIL: %d literal %d-byte runs still occur in the source files (game data)" % (len(left), CHECK))
    kinds, used, total_lit, nglyph = {}, set(), 0, 0
    for p, new, _, ops in entries:
        o, nlit = 0, 0
        for op in ops:
            if op[0] == 1:
                nlit += len(op[1])
                for j in range(len(op[1])):
                    k = literal_kind(p, new, o + j)
                    kinds[k] = kinds.get(k, 0) + 1
            elif op[0] == 0:
                used.add(op[1])
            else:
                nglyph += 1
                used |= {sp[0] for sp in op[3] if sp}
            o += op_len(op)
        assert o == len(new), p
        total_lit += nlit
        if nlit > max_literal:
            raise SystemExit("%s: %d literal bytes (> --max-literal %d): not compiled" % (p, nlit, max_literal))
        log("  %-24s %6d bytes  %4d ops  %5d literal" % (p, len(new), len(ops), nlit))
    log("literal bytes by kind (none of them occurs as %d+ bytes in the disc files; %d glyph bitmaps built by GLYPH ops):"
        % (CHECK, nglyph))
    for k, v in sorted(kinds.items(), key=lambda x: -x[1]):
        log("  %7d  %s" % (v, k))
    used = sorted(used)
    remap = {si: k for k, si in enumerate(used)}
    title = (patch.get("title") or "").encode("utf-8")
    b = bytearray(b"TPCH" + struct.pack("<III", 2, len(used), len(entries)))
    b += DP.file_sha1(patch_path).encode() + struct.pack("<H", len(title)) + title
    for si in used:
        p, data = sources[si]
        pe = p.encode()
        b += struct.pack("<H", len(pe)) + pe + struct.pack("<I", len(data)) + hashlib.sha1(data).digest()
    for p, new, ranges, ops in entries:
        pe = p.encode()
        b += struct.pack("<H", len(pe)) + pe + struct.pack("<I", len(new)) + hashlib.sha1(new).digest()
        b += struct.pack("<B", len(ranges)) + b"".join(struct.pack("<II", l, n) for l, n in ranges)
        b += struct.pack("<I", len(ops))
        for o in ops:
            if o[0] == 0:
                b += struct.pack("<BHII", 0, remap[o[1]], o[2], o[3])
            elif o[0] == 1:
                b += struct.pack("<BI", 1, len(o[1])) + o[1]
            else:
                b += struct.pack("<BHH", 2, o[1], o[2])
                for sp in o[3]:
                    b += struct.pack("<HIBb", remap[sp[0]], sp[1], sp[2], sp[3]) if sp else struct.pack("<HIBb", 0, 0, 0xFF, 0)
    open(out, "wb").write(b)
    log("%s: %d files, %d sources, %d literal bytes, %d bytes" % (out, len(entries), len(used), total_lit, len(b)))
    if not work:
        shutil.rmtree(tmp, ignore_errors=True)
    return pd


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("data", help="pristine data/ folder (tools/extract_data.py) of the PAL disc SCES-01331, or the disc image itself")
    ap.add_argument("patch", help="*.tombipatch.json")
    ap.add_argument("-o", "--out", required=True)
    ap.add_argument("--max-literal", type=int, default=16384, help="largest literal total allowed per file")
    ap.add_argument("--work", help="keep the patched copy of data/ here (for tools/tpatch_check.py)")
    a = ap.parse_args()
    if os.path.isfile(a.data):
        build_from_disc(a.data, a.patch, a.out, a.max_literal)
    else:
        build(a.data, a.patch, a.out, a.max_literal, a.work)


if __name__ == "__main__":
    main()
