#!/usr/bin/env python3
"""Survey of "Tomba! (Beta prototype) (1998-02-03)", CD-R "TOMBI NTSC Preview Version". Findings: docs/BETA_980203.md.
Reads only; nothing from the disc is written into the repo.

  python3 -I tools/beta980203.py lists IMAGE FS [AREA ...]   load lists per area/section (kinds, files); flags top-down kinds
  python3 -I tools/beta980203.py text FS OUT [--with FS2 ...] every WFM3 block as text, one OUT/<file>@<offset>.txt each
  python3 -I tools/beta980203.py tim8 FS                      every 8 bpp TIM (the top-down map pictures are 8 bpp)

Layout (boot SLPS_011.44, sha1 1e69d1414dcca09fe360fe69337fc8e62968fda7):
  file table   file offset 0x66DF4, 1055 entries of 8 bytes (BCD CdlLOC + size, as PAL MAINn / tools/cdfiles.py); SYSTEM and
               MOVIE first, two empty entries, then AREA00.. (entry 225 = AREA00/A000.GAM). Load-list file ids index it.
  load lists   20-byte records (s16 file, u8 slot, u8 kind, u32 dest, u32 arg, s16 w, s16 h, u32 type), file -1 ends a list.
               SYS lists start at 0x80095BD8 (vram); area lists are reached through the pointer table at 0x800A1478:
               20 area lists (AREA00..19), then the section lists of every area in order; the u16 section counts per
               area are at 0x800786F0 (AREA15 counts 1, its 9 lists are the WSS script lists).
  text         WFM3 blocks (the PAL format, tools/wfm_py.py) embedded in the GAM area files, like the JP disc; the glyphs
               are new bitmaps, so `text` maps them by pixel equality with the PAL English font and, where that fails, by
               aligning each dialogue with the PAL English (MFNTnn0B) dialogue of the same number when both have the same
               glyph count (majority vote over the whole disc).
"""
import argparse, collections, glob, hashlib, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import area000 as A  # noqa: E402
import cdfiles as C  # noqa: E402

EXE = "SLPS_011.44"
TABLE, TABLE_N = 0x66DF4, 1055
PTRS, NAREA, COUNTS = 0x800A1478, 20, 0x800786F0
TOPDOWN = {0x8E, 0x8F, 0x3B, 0x3C, 0x3D, 0x3E}
PAL_FS = os.path.join(ROOT, "game/disc/fs")


def off(v):
    return v - 0x80010000 + 0x800


def names(img, exe):
    lay = C.layout_from_image(img)
    byk = {(l, C.tsize(n, s)): n for n, (l, s) in lay.items()}
    out = {}
    for i in range(TABLE_N):
        o = TABLE + 8 * i
        lba = C.dec_loc(exe[o:o + 4])
        out[i] = byk.get((lba, struct.unpack_from("<I", exe, o + 4)[0])) if lba else None
    return out


def parse_list(exe, v, nm):
    o, recs = off(v), []
    while True:
        fid, slot, kind, dest, arg, w, h, typ = struct.unpack_from("<hBBIIhhI", exe, o)
        if fid == -1:
            return recs
        recs.append(dict(name=nm.get(fid) if typ != 0xFFFFFFFF else None, kind=kind, type=typ, arg=arg))
        o += 20


def area_lists(img, fs):
    """{area: [area list, section 0 list, section 1 list, ...]}"""
    exe = open(os.path.join(fs, EXE), "rb").read()
    nm = names(img, exe)
    ptr = lambda i: struct.unpack_from("<I", exe, off(PTRS) + 4 * i)[0]
    cnt = struct.unpack_from("<%dH" % NAREA, exe, off(COUNTS))
    out, k = {}, NAREA
    for a in range(NAREA):
        n = 9 if a == 15 else cnt[a]   # AREA15: 9 WSS lists, counted as 1
        out[a] = [parse_list(exe, ptr(a), nm)] + [parse_list(exe, ptr(k + s), nm) for s in range(n)]
        k += n
    return out


def cmd_lists(a):
    L = area_lists(a.image, a.fs)
    want = [int(x) for x in a.areas] or list(L)
    for ar in want:
        print("AREA%02d sections: %s records" % (ar, [len(l) for l in L[ar][1:]]))
        for i, lst in enumerate(L[ar]):
            td = sorted({"%02X" % r["kind"] for r in lst if r["kind"] in TOPDOWN})
            print("  %-10s %2d  %s%s" % ("area" if i == 0 else "s%d" % (i - 1), len(lst),
                                         " ".join("%02X" % r["kind"] for r in lst), "   top-down: " + ",".join(td) if td else ""))


def wfm_blocks(fs):
    import wfm_py
    for dp, _, fl in sorted(os.walk(fs)):
        for n in sorted(fl):
            if n.endswith((".STR", ".DAT", ".WVD", ".VB", ".VH", ".SEQ")):
                continue
            p = os.path.join(dp, n)
            d = open(p, "rb").read()
            if d[:4] == b"GAM\0":
                try:
                    d = A.gam_unpack(d)
                except Exception:
                    continue
            i = d.find(b"WFM3")
            while i >= 0:
                try:
                    yield os.path.relpath(p, fs), i, wfm_py.decode(d[i:])
                except Exception:
                    pass
                i = d.find(b"WFM3", i + 4)


def glyph_key(g):
    return (g.height, tuple(g.pixels()))


def cmd_text(a):
    import wfm_py
    pal = [wfm_py.decode(open(p, "rb").read()) for p in sorted(glob.glob(os.path.join(PAL_FS, "*", "*B.WFM")))]
    M = {}
    for w in pal:
        for g in w.glyphs:
            if g.char:
                M.setdefault(glyph_key(g), g.char)
    seq = lambda w, d: [w.glyphs[x & 0xFFF] for x in d.words if x & 0xF000 == 0x8000 and (x & 0xFFF) < len(w.glyphs)]
    votes = collections.defaultdict(collections.Counter)
    blocks = {fs: list(wfm_blocks(fs)) for fs in [a.fs] + a.with_}
    for fs, bl in blocks.items():
        for _, _, w in bl:
            for pw in (p for p in pal if len(p.dialogues) == len(w.dialogues)):
                for d, pd in zip(w.dialogues, pw.dialogues):
                    s, t = seq(w, d), seq(pw, pd)
                    if s and len(s) == len(t):
                        for x, y in zip(s, t):
                            if y.char:
                                votes[glyph_key(x)][y.char] += 1
    for k, c in votes.items():
        ch, n = c.most_common(1)[0]
        if k not in M and n >= 2 and n >= 0.6 * sum(c.values()):
            M[k] = ch
    os.makedirs(a.out, exist_ok=True)
    tot = unk = 0
    for rel, i, w in blocks[a.fs]:
        for g in w.glyphs:
            g.char = g.char or M.get(glyph_key(g))
            tot += 1
            unk += not g.char
        w.charidx = wfm_py._charindex(w.glyphs)
        with open(os.path.join(a.out, "%s@%05X.txt" % (rel.replace("/", "_"), i)), "w", encoding="utf-8") as o:
            for k, d in enumerate(w.dialogues):
                o.write("#%d %s\n" % (k, wfm_py.words_to_text(w, d.words, d.font).replace("\n", " ")))
    print("%d WFM3 blocks, %d glyphs, %d without a character" % (len(blocks[a.fs]), tot, unk))


def cmd_tim8(a):
    for dp, _, fl in sorted(os.walk(a.fs)):
        for n in sorted(fl):
            if n.endswith((".STR", ".WVD", ".DAT", ".VB", ".VH", ".SEQ")):
                continue
            p = os.path.join(dp, n)
            d = open(p, "rb").read()
            if d[:4] == b"GAM\0":
                try:
                    d = A.gam_unpack(d)
                except Exception:
                    continue
            o = d.find(b"\x10\0\0\0\x09\0\0\0")
            while o >= 0:
                if o % 4 == 0 and o + 20 <= len(d):
                    cl = struct.unpack_from("<I", d, o + 8)[0]
                    cw, ch = struct.unpack_from("<2H", d, o + 16)
                    if cw == 256 and 0 < ch <= 16 and o + 8 + cl + 12 <= len(d):
                        iw, ih = struct.unpack_from("<2H", d, o + 8 + cl + 8)
                        img = d[o + 8 + cl + 12:o + 8 + cl + 12 + iw * ih * 2]
                        print("%-22s 0x%05X  %dx%d  md5 %s" % (os.path.relpath(p, a.fs), o, iw * 2, ih,
                                                              hashlib.md5(img).hexdigest()))
                o = d.find(b"\x10\0\0\0\x09\0\0\0", o + 4)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("lists")
    p.add_argument("image")
    p.add_argument("fs")
    p.add_argument("areas", nargs="*")
    p = sp.add_parser("text")
    p.add_argument("fs")
    p.add_argument("out")
    p.add_argument("--with", dest="with_", nargs="*", default=[], help="more discs (e.g. the USA fs) to vote glyphs")
    p = sp.add_parser("tim8")
    p.add_argument("fs")
    a = ap.parse_args()
    {"lists": cmd_lists, "text": cmd_text, "tim8": cmd_tim8}[a.cmd](a)


if __name__ == "__main__":
    main()
