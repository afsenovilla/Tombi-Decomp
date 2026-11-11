#!/usr/bin/env python3
"""Learns the characters of the USA disc's WFM glyphs (Tomba!, SCUS-94236) from the PAL English ones: writes
tools/wfm_charmap_usa.json (height:width:SHA-1[0..5] of the 4 bpp bitmap -> character; hashes only, no bitmaps).

The USA fonts are the PAL English fonts retouched (docs/USA.md "English text: USA vs PAL": 8 px all identical, 16 px
59 of 81, 24 px 21 of 58), so tools/wfm_charmap.json names only part of them.  The USA has no WFM files: its banks are
embedded in the area GAM files (dialogue banks with the 8 / 16 px glyphs, the 24 px event-title bank).  Each USA bank
has a PAL English twin (MFNTnnnB.WFM / CFNT999B.WFM) with the same number of dialogues in the same order, almost all
of them the same text: a USA glyph id that stands where the PAL dialogue has a known glyph, in dialogues of the same
length, gets that glyph's character (a vote over every such dialogue; ties and disagreements are reported, not kept).
port2/tools/gen_panel_charmap.py adds this table to the panel's (runtime/panel_charmap.h).

  python3 -I tools/wfm_charmap_usa.py PAL_DATA_DIR USA_FS_DIR [-o tools/wfm_charmap_usa.json]
"""
import argparse, collections, glob, hashlib, json, os, struct

HERE = os.path.dirname(os.path.abspath(__file__))


def gam_unpack(d):
    size, p, out, left, flags = struct.unpack_from("<I", d, 4)[0], 8, bytearray(), 0, 0
    while len(out) < size:
        if not left:
            flags, p, left = struct.unpack_from("<H", d, p)[0], p + 2, 16
        bit, flags, left = flags & 1, flags >> 1, left - 1
        if bit:
            off, ln = d[p], d[p + 1]
            p += 2
            for _ in range(ln):
                if len(out) >= size:
                    break
                out.append(out[-off])
        else:
            out.append(d[p])
            p += 1
    return bytes(out)


def read(path):
    d = open(path, "rb").read()
    return gam_unpack(d) if d[:4] == b"GAM\0" else d


NARGS = (1, 0, 0, 0, 2, 1, 2, 1, 2, 0, 0, 0)   # control codes 0xFFF2.. (tools/wfm_py.py)


class Bank:
    def __init__(self, d, o):
        self.d, self.o = d, o
        self.dpt = struct.unpack_from("<I", d, o + 8)[0]
        self.nd, self.ng = struct.unpack_from("<HH", d, o + 12)

    def glyph(self, i):
        d, o = self.d, self.o
        gp = struct.unpack_from("<H", d, o + 144 + 2 * i)[0]
        h, w = struct.unpack_from("<HH", d, o + gp + 2)
        img = d[o + gp + 8:o + gp + 8 + (w * h + 1) // 2]
        return "%d:%d:%s" % (h, w, hashlib.sha1(img).hexdigest()[:12])

    def dialogue(self, k):
        d, b = self.d, self.o + self.dpt
        p, out = b + struct.unpack_from("<H", d, b + 2 * k)[0], []
        while p + 2 <= len(d):
            v = struct.unpack_from("<H", d, p)[0]
            p += 2
            if v >= 0xFFFE:
                break
            if v >= 0xFFF2:
                out.append(("c", v))
                p += 2 * NARGS[v - 0xFFF2]
            elif v >= 0x8000:
                out.append(("g", v - 0x8000))
        return out


def banks(d):
    o, out = d.find(b"WFM3"), []
    while o >= 0:
        if o + 144 <= len(d):
            out.append(Bank(d, o))
        o = d.find(b"WFM3", o + 4)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("pal")
    ap.add_argument("usa")
    ap.add_argument("-o", "--out", default=os.path.join(HERE, "wfm_charmap_usa.json"))
    a = ap.parse_args()
    known = json.load(open(os.path.join(HERE, "wfm_charmap.json"), encoding="utf-8"))
    pal = collections.defaultdict(list)   # dialogue count -> PAL English banks
    for f in sorted(glob.glob(os.path.join(a.pal, "AREA*", "*B.WFM"))):
        for b in banks(read(f)):
            pal[b.nd].append(b)
    votes = collections.defaultdict(collections.Counter)
    for f in sorted(glob.glob(os.path.join(a.usa, "AREA*", "*.GAM")) + glob.glob(os.path.join(a.usa, "SYSTEM", "DSPSUB*.BIN"))):
        try:
            d = read(f)
        except Exception:
            continue
        for u in banks(d):
            for p in pal.get(u.nd, ()):
                for k in range(u.nd):
                    du, dp = u.dialogue(k), p.dialogue(k)
                    if len(du) != len(dp) or [t for t, _ in du] != [t for t, _ in dp]:
                        continue
                    for (tu, gu), (tp, gp) in zip(du, dp):
                        if tu != "g" or gu >= u.ng or gp >= p.ng:
                            continue
                        ch = known.get(p.glyph(gp))
                        if ch:
                            votes[u.glyph(gu)][ch] += 1
    out, bad = {}, 0
    for key, c in sorted(votes.items()):
        (ch, n), *rest = c.most_common()
        if key in known:
            continue                      # the PAL table has it already
        if rest and rest[0][1] * 4 > n:   # no clear winner
            print("unsure", key, c.most_common(3))
            bad += 1
            continue
        out[key] = ch
    with open(a.out, "w", encoding="utf-8") as f:
        f.write("{\n" + ",\n".join("%s: %s" % (json.dumps(k), json.dumps(v, ensure_ascii=False)) for k, v in sorted(out.items())) + "\n}\n")
    print("%d USA glyphs named (%d left out)" % (len(out), bad))


if __name__ == "__main__":
    main()
