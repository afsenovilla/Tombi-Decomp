#!/usr/bin/env python3
"""Catalog every palette (CLUT) on every Tomba disc we have, and test them against the lost Civilization colours.
Findings: docs/CIVILIZATION.md, "Palette catalog". Reads only; the catalog and renders stay outside the repo.

  python3 -I tools/clut_catalog.py catalog OUT_DIR [DISC=FS ...]      catalog.csv / catalog.json / summary.txt
  python3 -I tools/clut_catalog.py test OUT_DIR --sprite PNG.. --page NAME=REF.png.. [--render DIR]

Default discs: pal=game/disc/fs, usa=/dev/shm/usa/fs, jp=/dev/shm/jp/fs, pp8=/dev/shm/pp8/TOMBA (tools/prepre8.py
extract), jpdemo=/dev/shm/jpdemo/fs (SLPM-80165), beta=/dev/shm/beta/fs (the 1998-02-03 English preview CD-R,
docs/BETA_980203.md); jpdemo and beta are used only when present.

Sources, per file (GAM unpacked):
  clut   AREAnn/CLUTnn (256 x 32 words) and PrePre 8 AREA00/CLUT00 (640 x 32 words at (384,480));
  sys    PrePre 8 SYSTEM/SYS001/002/004/006/007 (bare 16/256-colour palettes);
  small  every other .000 file of 32..1024 bytes (size multiple of 32): the load-list CLUT blocks such as PAL
         SYSTEM/A00007 (256 x 1) or A00805 (16 x 16), whatever their number on each disc; files named *CLUT*
         (SYSTEM/DSPCLUT.TIM is a bare CLUT, not a TIM) count as clut;
  tim    every TIM header (0x10, flags 8/9 with CLUT bit) at any 4-aligned offset: this covers the 0x8E/0x8F top-down
         pictures and TIMs inside load-list chunks and overlays;
  heur   conservative scan of the other non-page data: 32-byte aligned runs of 16 colours with entry 0 = 0x0000,
         entries 1..15 non-zero, bit 15 clear, >= 12 distinct, and at least two such runs in a row. Files whose
         size is a multiple of 32 KB (raw VRAM pages) are not scanned.
Normalisation: every source is cut into 16-colour rows (aligned to 16 inside its own row) and 256-colour windows
(16-entry steps over the source's colour stream); rows that are all one value are dropped; each row/window is
hashed (md5 of the 32/512 little-endian bytes) and the discs it occurs on are collected.
"""
import argparse, csv, hashlib, json, os, struct, sys
from collections import defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import area000 as A  # noqa: E402

DISCS = {"pal": os.path.join(ROOT, "game/disc/fs"), "usa": "/dev/shm/usa/fs", "jp": "/dev/shm/jp/fs",
         "pp8": "/dev/shm/pp8/TOMBA", "jpdemo": "/dev/shm/jpdemo/fs", "beta": "/dev/shm/beta/fs"}
SKIP_DIRS = {"MOVIE", "SOUND", "ZZZ"}
SKIP_EXT = (".VB", ".VH", ".WVD", ".STR", ".RAW", ".XA", ".CNF")
PP8_SYS = {"SYS001", "SYS002", "SYS004", "SYS006", "SYS007"}


def files(fs):
    for dp, dns, fns in os.walk(fs):
        dns[:] = sorted(d for d in dns if d not in SKIP_DIRS)
        for f in sorted(fns):
            if f.upper().endswith(SKIP_EXT):
                continue
            p = os.path.join(dp, f)
            d = open(p, "rb").read()
            if f.endswith(".GAM"):
                try:
                    d = A.gam_unpack(d)
                except (ValueError, IndexError):
                    continue
            yield os.path.relpath(p, fs), d


def tims(d):
    """(offset, clut_x, clut_y, w, h, words) of every TIM with a CLUT block."""
    out = []
    o = -1
    while True:
        o = d.find(b"\x10\0\0\0", o + 1)
        if o < 0 or o > len(d) - 20:
            break
        if o & 3:
            continue
        fl = struct.unpack_from("<I", d, o + 4)[0]
        if fl not in (8, 9):
            continue
        ln, x, y, w, h = struct.unpack_from("<IHHHH", d, o + 8)
        if not (0 < w <= 1024 and 0 < h <= 512 and ln == 12 + w * h * 2 and o + 8 + ln <= len(d)):
            continue
        out.append((o, x, y, w, h, list(struct.unpack_from("<%dH" % (w * h), d, o + 20))))
    return out


def heur(d, skip):
    out, run = [], []
    for o in range(0, len(d) - 31, 32):
        ok = False
        if not any(a <= o < b for a, b in skip):
            v = struct.unpack_from("<16H", d, o)
            ok = v[0] == 0 and all(0 < c < 0x8000 for c in v[1:]) and len(set(v)) >= 12
        if ok:
            run.append(o)
        else:
            if len(run) >= 2:
                out.append(run)
            run = []
    if len(run) >= 2:
        out.append(run)
    return out


def sources(disc, fs):
    """(file, kind, offset, width, words) per palette source of one disc."""
    for name, d in files(fs):
        base = os.path.basename(name).split(".")[0]
        skip = []
        if "CLUT" in base:
            w = 640 if disc == "pp8" else 256
            yield name, "clut", 0, w, list(struct.unpack_from("<%dH" % (len(d) // 2), d))
            continue
        if (disc == "pp8" and base in PP8_SYS) or (name.endswith(".000") and 32 <= len(d) <= 1024 and len(d) % 32 == 0):
            yield name, "sys" if disc == "pp8" else "small", 0, 256 if len(d) >= 512 else 16, list(struct.unpack_from("<%dH" % (len(d) // 2), d))
            continue
        for o, x, y, w, h, words in tims(d):
            skip.append((o, o + 20 + w * h * 2))
            yield name, "tim@%d,%d" % (x, y), o + 20, w, words
        if len(d) % 32768 == 0:
            continue
        for run in heur(d, skip):
            words = list(struct.unpack_from("<%dH" % (16 * len(run)), d, run[0]))
            yield name, "heur", run[0], 16, words


def h(words):
    return hashlib.md5(struct.pack("<%dH" % len(words), *words)).hexdigest()[:16]


def catalog(out, discs):
    rows = defaultdict(lambda: {"discs": set(), "where": [], "words": None})
    wins = defaultdict(lambda: {"discs": set(), "where": []})
    count = defaultdict(int)
    for disc, fs in discs.items():
        if not os.path.isdir(fs):
            print("%s: %s not available" % (disc, fs))
            continue
        for name, kind, off, w, words in sources(disc, fs):
            count[disc, kind.split("@")[0]] += 1
            for r in range(len(words) // w):
                for c in range(0, w - 15, 16):
                    seg = words[r * w + c:r * w + c + 16]
                    if len(set(seg)) < 2:
                        continue
                    e = rows[h(seg)]
                    e["discs"].add(disc)
                    e["words"] = seg
                    e["where"].append((disc, name, kind, off + (r * w + c) * 2, r, c))
            for s in range(0, len(words) - 255, 16):
                seg = words[s:s + 256]
                if len(set(seg)) < 16:
                    continue
                e = wins[h(seg)]
                e["discs"].add(disc)
                e["where"].append((disc, name, kind, off + s * 2))
    os.makedirs(out, exist_ok=True)
    with open(os.path.join(out, "catalog.csv"), "w", newline="") as f:
        wr = csv.writer(f)
        wr.writerow(["hash", "discs", "disc", "file", "kind", "offset", "row", "col", "colours"])
        for k, e in rows.items():
            for disc, name, kind, off, r, c in e["where"]:
                wr.writerow([k, "+".join(sorted(e["discs"])), disc, name, kind, "0x%X" % off, r, c,
                             " ".join("%04X" % v for v in e["words"])])
    json.dump({k: {"discs": sorted(e["discs"]), "words": e["words"], "where": e["where"]} for k, e in rows.items()},
              open(os.path.join(out, "catalog.json"), "w"))
    json.dump({k: {"discs": sorted(e["discs"]), "where": e["where"]} for k, e in wins.items()},
              open(os.path.join(out, "windows.json"), "w"))
    L = ["sources per disc/kind: " + ", ".join("%s %s %d" % (d, k, n) for (d, k), n in sorted(count.items()))]
    for d in discs:
        n = sum(d in e["discs"] for e in rows.values())
        u = sum(e["discs"] == {d} for e in rows.values())
        nw = sum(d in e["discs"] for e in wins.values())
        uw = sum(e["discs"] == {d} for e in wins.values())
        L.append("%-6s 16-colour rows: %6d distinct, %6d only here | 256-windows: %6d, %6d only here" % (d, n, u, nw, uw))
    combo = defaultdict(int)
    for e in rows.values():
        combo["+".join(sorted(e["discs"]))] += 1
    L.append("rows by disc set: " + ", ".join("%s %d" % kv for kv in sorted(combo.items(), key=lambda x: -x[1])))
    for d in discs:
        uniq = [(k, e) for k, e in rows.items() if e["discs"] == {d}]
        files_ = defaultdict(int)
        for k, e in uniq:
            for w in e["where"]:
                files_[w[1], w[2]] += 1
        L.append("\n== rows only on %s: %d, by file:" % (d, len(uniq)))
        for (fn, kind), n in sorted(files_.items(), key=lambda x: -x[1]):
            L.append("   %-28s %-14s %5d" % (fn, kind, n))
    L.append("\n== every row (hash discs: file kind offset row col)")
    for k, e in sorted(rows.items(), key=lambda x: (len(x[1]["discs"]), sorted(x[1]["discs"]))):
        w = e["where"]
        L.append("%s %-18s %s%s" % (k, "+".join(sorted(e["discs"])), "; ".join(
            "%s:%s %s 0x%X r%d c%d" % (x[0], x[1], x[2], x[3], x[4], x[5]) for x in w[:4]),
            " (+%d)" % (len(w) - 4) if len(w) > 4 else ""))
    open(os.path.join(out, "summary.txt"), "w").write("\n".join(L) + "\n")
    print("\n".join(L[:len(discs) + 3]))


# ------------------------------------------------------------------ tests
def rgb(v):
    return ((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31)


def sprite_colours(paths):
    from PIL import Image
    cnt = defaultdict(int)
    for p in paths:
        for px in Image.open(p).convert("RGBA").getdata():
            if px[3] < 128 or max(px[:3]) - min(px[:3]) < 24:   # transparent, black, white, grey
                continue
            cnt[px[:3]] += 1
    return cnt


def test_sprite(cat, paths, top=15):
    import numpy as np
    cnt = sprite_colours(paths)
    C = np.array(list(cnt), float)
    W = np.array(list(cnt.values()), float)
    res = []
    for k, e in cat.items():
        P = np.array([rgb(v) for v in e["words"][1:]], float)
        d = np.sqrt(((C[:, None] - P[None]) ** 2).sum(-1)).min(1)
        res.append(((d * W).sum() / W.sum(), k))
    res.sort()
    return res[:top], {d: min((s, k) for s, k in res if d in cat[k]["discs"]) for d in
                       {d for e in cat.values() for d in e["discs"]}}, res


def ref_palette(page, ref):
    """index -> median colour of the reference render (one pixel per page byte)."""
    import numpy as np
    from PIL import Image
    R = np.asarray(Image.open(ref).convert("RGB")).astype(float)
    I = np.frombuffer(page, np.uint8).reshape(256, -1)
    pal, n = np.zeros((256, 3)), np.zeros(256)
    for i in range(256):
        m = I == i
        if m.any():
            pal[i] = np.median(R[m], 0)
            n[i] = m.sum()
    return pal, n


def test_pages(cat_words, pages, top=10):
    """Mean error (pixel-weighted) of each 256-colour window (16-entry steps over each source stream) per page."""
    import numpy as np
    out = {}
    for name, (pal, n) in pages.items():
        best = []
        for key, (disc, src, kind, off, words) in cat_words.items():
            Wd = np.array([rgb(v) for v in words], float)
            for s in range(0, len(words) - 255, 16):
                P = Wd[s:s + 256]
                e = (np.sqrt(((P - pal) ** 2).sum(1)) * n).sum() / n.sum()
                best.append((e, disc, src, kind, off + s * 2))
        best.sort()
        out[name] = best[:top]
    return out


def render(page, words, path):
    from PIL import Image
    I = list(page)
    w = len(I) // 256
    pal = b"".join(bytes(rgb(v)) for v in words[:256])
    im = Image.frombytes("P", (w, 256), bytes(I))
    im.putpalette(pal)
    im.convert("RGB").save(path)


def cmd_test(a):
    cat = json.load(open(os.path.join(a.out, "catalog.json")))
    os.makedirs(a.render, exist_ok=True)
    L = []
    if a.sprite:
        top, perdisc, _ = test_sprite(cat, a.sprite)
        L.append("== Female Stonetown Resident: weighted mean nearest-colour distance per 16-colour row")
        for s, k in top:
            w = cat[k]["where"][0]
            L.append("  %6.1f %s %-14s %s" % (s, k, "+".join(cat[k]["discs"]), "%s:%s %s r%d c%d" % (w[0], w[1], w[2], w[4], w[5])))
        L.append("  best per disc (incl. shared rows): " + ", ".join("%s %.1f" % (d, s) for d, (s, k) in sorted(perdisc.items())))
        uniq = [(s, k) for s, k in _ if len(cat[k]["discs"]) == 1][:5]
        L.append("  best disc-unique rows: " + "; ".join("%.1f %s %s" % (s, cat[k]["discs"][0], cat[k]["where"][0][1]) for s, k in uniq))
    if a.page:
        streams = {}
        for disc, fs in DISCS.items():
            if os.path.isdir(fs):
                for name, kind, off, w, words in sources(disc, fs):
                    if len(words) >= 256:
                        streams[len(streams)] = (disc, name, kind, off, words)
        pages = {}
        for spec in a.page:
            nm, ref = spec.split("=")
            pages[nm] = (A.read_resource(nm, DISCS["pal"]),) + ref_palette(A.read_resource(nm, DISCS["pal"]), ref)
        res = test_pages(streams, {k: v[1:] for k, v in pages.items()})
        for nm, best in res.items():
            L.append("== %s: mean error per colour, best 256-colour windows" % nm)
            for e, disc, src, kind, off in best:
                L.append("  %6.1f %s:%s %s 0x%X" % (e, disc, src, kind, off))
            e, disc, src, kind, off = best[0]
            words = next(v[4] for v in streams.values() if v[0] == disc and v[1] == src and v[2] == kind)
            base = next(v[3] for v in streams.values() if v[0] == disc and v[1] == src and v[2] == kind)
            p = os.path.join(a.render, "%s_best_%s_%s.png" % (nm.replace("/", "_"), disc, src.replace("/", "_")))
            render(pages[nm][0], words[(off - base) // 2:], p)
            L.append("  render: " + p)
    open(os.path.join(a.out, "tests.txt"), "w").write("\n".join(L) + "\n")
    print("\n".join(L))


def main():
    ap = argparse.ArgumentParser()
    sp = ap.add_subparsers(dest="cmd", required=True)
    c = sp.add_parser("catalog")
    c.add_argument("out")
    c.add_argument("discs", nargs="*")
    t = sp.add_parser("test")
    t.add_argument("out")
    t.add_argument("--sprite", nargs="*", default=[])
    t.add_argument("--page", nargs="*", default=[])
    t.add_argument("--render", default="clut_renders")
    a = ap.parse_args()
    if a.cmd == "catalog":
        d = dict(DISCS)
        d.update(x.split("=", 1) for x in a.discs)
        catalog(a.out, d)
    else:
        cmd_test(a)


if __name__ == "__main__":
    main()
