#!/usr/bin/env python3
"""Compiles a text patch (*.tombipatch.json) into a .tpatch: the binary form tombi2 --text-patch applies natively
(port2/runtime/tpatch.c, docs/PORT.md "Standalone (no launcher) mode"), with no Python on the player's machine (Android).

The JSON holds only our text; turning it into game files needs the WFM encoder, the glyph pool of the disc's fonts and the
file-table updates of tools/data_patch.py.  This tool runs that exact pipeline once on a developer's extracted disc
(DATA_DIR = data/ of tools/extract_data.py, pristine) and stores the result as a COPY/LITERAL delta against the files of
the same disc: every run of 12+ bytes that exists somewhere in the source files becomes a COPY (source file, offset,
length), the rest are literals (our encoded text: dialogue word indices, headers, sizes, tables).  The player's own disc
supplies the copied bytes, so the .tpatch carries no game data; --max-literal guards that (the build fails when a file
needs more literal bytes than that, e.g. glyph bitmaps that are not copies).  The C side checks the sha1 of every source
file and of every rebuilt file, so a different disc never gets a half-applied patch.

  python3 tools/tpatch_build.py data/ patches/es_mejorada.tombipatch.json -o patches/es_mejorada.tpatch
  python3 tools/tpatch_build.py data/ patches/en_cut_content.tombipatch.json -o patches/en_cut_content.tpatch

Format (little endian): "TPCH" u32 version=1, u32 n_sources, u32 n_entries, char json_sha1[40], u16 title_len, title
  source: u16 path_len, path, u32 size, u8 sha1[20]
  entry:  u16 path_len, path, u32 size, u8 sha1[20], u8 n_ranges, n_ranges * (u32 lba, u32 sectors), u32 n_ops, ops
          (n_ranges 0: not a disc file, e.g. a --patch-tex TBTX; ranges: where the rebuilt file is read on the CD,
           the original place first, then the LBA past the end of the disc a grown file moved to)
  op:     u8 0 = COPY u16 source, u32 offset, u32 length | u8 1 = LITERAL u32 length, bytes
"""
import argparse, hashlib, json, os, shutil, struct, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import extract_data as X

K, STRIDE = 12, 4


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


def build(data_dir, patch_path, out, max_literal, work=None, log=print):
    import data_patch as DP
    patch = json.load(open(patch_path, encoding="utf-8"))
    tmp = work or tempfile.mkdtemp(prefix="tpatch_")
    pd = os.path.join(tmp, "data")
    if os.path.isdir(pd):
        shutil.rmtree(pd)
    log("copying %s -> %s" % (data_dir, pd))
    shutil.copytree(data_dir, pd, ignore=shutil.ignore_patterns(DP.ORIG))
    r = DP.apply(pd, patch_path, log=log)
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
    idx = Index(sources)
    entries, used, total_lit = [], set(), 0
    for p, new, ranges in targets:
        ops = idx.delta(new, sidx.get(p))
        nlit = sum(len(o[1]) for o in ops if o[0] == 1)
        total_lit += nlit
        if nlit > max_literal:
            raise SystemExit("%s: %d literal bytes (> --max-literal %d): not compiled" % (p, nlit, max_literal))
        used |= {o[1] for o in ops if o[0] == 0}
        # the original range first (manifest order is by LBA; the moved copy is past the end of the disc)
        entries.append((p, new, sorted((lba, n) for lba, n, _ in ranges), ops))
        log("  %-24s %6d bytes  %3d ops  %5d literal" % (p, len(new), len(ops), nlit))
    used = sorted(used)
    remap = {si: k for k, si in enumerate(used)}
    title = (patch.get("title") or "").encode("utf-8")
    b = bytearray(b"TPCH" + struct.pack("<III", 1, len(used), len(entries)))
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
            b += struct.pack("<BHII", 0, remap[o[1]], o[2], o[3]) if o[0] == 0 else struct.pack("<BI", 1, len(o[1])) + o[1]
    open(out, "wb").write(b)
    log("%s: %d files, %d sources, %d literal bytes, %d bytes" % (out, len(entries), len(used), total_lit, len(b)))
    if not work:
        shutil.rmtree(tmp, ignore_errors=True)
    return pd


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("data", help="pristine data/ folder (tools/extract_data.py) of the PAL disc SCES-01331")
    ap.add_argument("patch", help="*.tombipatch.json")
    ap.add_argument("-o", "--out", required=True)
    ap.add_argument("--max-literal", type=int, default=16384, help="largest literal total allowed per file")
    ap.add_argument("--work", help="keep the patched copy of data/ here (for tools/tpatch_check.py)")
    a = ap.parse_args()
    build(a.data, a.patch, a.out, a.max_literal, a.work)


if __name__ == "__main__":
    main()
