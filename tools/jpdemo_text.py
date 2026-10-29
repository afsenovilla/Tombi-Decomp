#!/usr/bin/env python3
"""Dump the text of the Japanese demo disc (SLPM-80165) and compare it with the retail JP release (SLPS-01144).

The demo stores text in WFM1 blocks: SYSTEM/A00013.000 (= A00016.000), SYSTEM/A00014.000, and inside the unpacked
GAMs AREA00/A006.GAM and AREA08/A003.GAM (the unreferenced early AREA08 set).  WFM1 is WFM2 (tools/jp_text.py)
without the 128-byte group table:
  "WFM1", u32 0, u32 dpt, u16 n_dialogues, u16 n_glyphs, u16 glyph_ptr[n_glyphs] at 0x10, glyphs (u16 w, h, adv, 0 +
  4bpp image of (4*w) x h), u16 dialogue_ptr[n] at dpt (offsets from dpt), word streams as in WFM2/WFM3.
Glyphs are decoded with tools/jp_charmap.json (bitmap hash -> character); unknown glyphs print as {Gn} and are drawn
in OUT/glifos_sin_identificar.png.  Each demo dialogue is matched against every retail JP dialogue (flattened text,
difflib ratio); those below --thr (except the
'アイテム１２３' / 'イベント０４５' placeholder slots) go to OUT/SOLO_DEMO.txt as early/cut text candidates.

  python3 tools/jpdemo_text.py DEMO_BIN OUT_DIR [--jp DISC_JP] [--thr 0.6]
"""
import argparse, difflib, os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gam_py, jp_text


def parse_wfm1(b):
    if len(b) < 0x10 or b[:4] != b"WFM1":
        return None
    dpt, nd, ng = struct.unpack_from("<IHH", b, 8)
    if dpt > len(b) or nd == 0 or dpt + 2 * nd > len(b):
        return None
    glyphs = []
    for p in struct.unpack_from("<%dH" % ng, b, 0x10):
        w, h, adv, z = struct.unpack_from("<4H", b, p)
        glyphs.append((w, h, adv, bytes(b[p + 8:p + 8 + (4 * w * h + 1) // 2])))
    dl = []
    for p in struct.unpack_from("<%dH" % nd, b, dpt):
        pos, ws = dpt + p, []
        while pos + 2 <= len(b):
            v = struct.unpack_from("<H", b, pos)[0]
            pos += 2
            if v in (0xFFFE, 0xFFFF):
                break
            ws.append(v)
        dl.append(ws)
    return glyphs, dl


def containers(files):
    out = []
    for name in sorted(files):
        d = files[name]
        if d[:4] == b"GAM\0":
            try:
                d = gam_py.unpack(d)
            except Exception:
                continue
        i, k = d.find(b"WFM1"), 0
        while i >= 0:
            r = parse_wfm1(d[i:])
            if r:
                out.append((name, k, r[0], r[1]))
                k += 1
            i = d.find(b"WFM1", i + 4)
    return out


PLACEHOLDER = re.compile("^(アイテム|イベント)[０-９]+$")   # unfilled slots of the item/event tables


def key(t):
    return re.sub(r"\{[^}]*\}|\s", "", jp_text.flat(t))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("demo")
    ap.add_argument("out")
    ap.add_argument("--jp")
    ap.add_argument("--thr", type=float, default=0.6)
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    dec = jp_text.Decoder()
    retail = []
    if a.jp:
        for name, k, g, dl in jp_text.containers(jp_text.read_disc(a.jp)):
            for i, ws in enumerate(dl):
                t = key(dec.text(g, ws))
                if len(t) >= 2:
                    retail.append(("%s#%d/%d" % (name, k, i), t))
    seen, solo = {}, []
    for name, k, glyphs, dl in containers(jp_text.read_disc(a.demo)):
        texts = [dec.text(glyphs, ws) for ws in dl]
        tag = "%s_%d" % (name.replace("/", "_").rsplit(".", 1)[0], k)
        sig = hash(tuple(texts))
        if sig in seen:
            print(tag, "= copia de", seen[sig])
            continue
        seen[sig] = tag
        with open(os.path.join(a.out, tag + ".txt"), "w", encoding="utf-8") as f:
            f.write("# %s, bloque WFM1 %d: %d diálogos, %d glifos\n" % (name, k, len(dl), len(glyphs)))
            for i, t in enumerate(texts):
                f.write("\n## %d\n%s\n" % (i, t))
                kt = key(t)
                if not retail or not jp_text.real(t):
                    continue
                best, bn = 0, None
                for rn, rt in retail:
                    sm = difflib.SequenceMatcher(None, kt, rt, autojunk=False)
                    if sm.real_quick_ratio() <= best or sm.quick_ratio() <= best:
                        continue
                    r = sm.ratio()
                    if r > best:
                        best, bn = r, rn
                f.write("  [retail %.2f %s]\n" % (best, bn))
                if best < a.thr and not PLACEHOLDER.match(kt):
                    solo.append((tag, i, best, bn, t))
    jp_text.atlas(dec, os.path.join(a.out, "glifos_sin_identificar.png"))
    with open(os.path.join(a.out, "SOLO_DEMO.txt"), "w", encoding="utf-8") as f:
        f.write("# Diálogos de la demo sin equivalente cercano en el JP final (ratio < %.2f)\n" % a.thr)
        for tag, i, r, bn, t in solo:
            f.write("\n== %s #%d (mejor %.2f: %s)\n%s\n" % (tag, i, r, bn, t))
    print("glifos sin identificar:", len(dec.unknown), "solo demo:", len(solo))


if __name__ == "__main__":
    main()
