#!/usr/bin/env python3
"""Dump all the text of the Japanese release (Ore! Tomba, SLPS-01144) from its disc image.

The JP disc has no *.WFM files: the dialogue fonts are WFM2 blocks embedded in the (GAM-compressed) area files
(AREAnn/A00x.GAM, AREA00/D*.GAM, AREA19/D*.GAM ...) and in SYSTEM/DSPSUB*.BIN.  WFM2 is WFM3 (tools/wfm_py.py) with:
  "WFM2", u32 pad, u32 dpt, u16 n_dialogues, u16 n_glyphs, 128 bytes (u16 table: first glyph of each group), glyph ptrs, glyphs,
  dialogue table at dpt.  Glyph header u16 w (VRAM words, 4 px), u16 height, u16 advance, u16 0; image is (4*w) x height 4bpp
  (in WFM2 advance != 4*w: kana are 10 px tall/10 advance in a 12 px cell, kanji 12x12, event titles 20x20).
  Words: 0x8000|n glyph n; 0xD000|n a kana drawn without advancing followed by 0xE000|n the dakuten/handakuten drawn on top
  (written here as the composed character, e.g. ガ); 0xC000|n corner icon; 0xFFF2..0xFFFD control codes as in wfm_py.
  SYSTEM/DSPSUB2.BIN also holds plain Shift-JIS strings (map names), dumped as well.
Glyphs are bitmaps: tools/jp_charmap.json maps "height:w:sha1[:12]" of the image to a character (hand identified).
Unidentified glyphs are written as {Gnnn} and drawn in glifos_sin_identificar.png.

  python3 tools/jp_text.py DISC_JP OUT_DIR [--pal DISC_PAL] [--lang S]
With --pal, each JP dialogue is aligned with the PAL WFM of the same area (control-code skeleton, difflib) and the PAL
text of that language is printed under it; JP dialogues with no PAL counterpart go to OUT_DIR/SOLO_JAPON.txt.
"""
import argparse, collections, difflib, hashlib, json, os, re, struct, sys, unicodedata
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cdfiles, gam_py
from wfm_py import CTRL

CHARMAP = os.path.join(os.path.dirname(os.path.abspath(__file__)), "jp_charmap.json")
MARKS = {"゛": "゙", "。": "゚", "゜": "゚"}


def read_disc(path):
    """{name: bytes} of every file of a raw/cooked image (movies and audio skipped)."""
    lay = cdfiles.layout_from_image(path)
    f = open(path, "rb")
    f.seek(16 * 2352 + 25)
    ss, uo = (2352, 24) if f.read(5) == b"CD001" else (2048, 0)
    out = {}
    for name, (lba, size) in lay.items():
        if name.endswith((".STR", ".WVD", ".SEQ", ".WSS")):
            continue
        b = bytearray()
        for i in range((size + 2047) // 2048):
            f.seek((lba + i) * ss + uo)
            b += f.read(2048)
        out[name] = bytes(b[:size])
    f.close()
    return out


def parse_wfm2(b):
    if len(b) < 0x90:
        return None
    dpt, nd, ng = struct.unpack_from("<IHH", b, 8)
    if dpt > len(b) or dpt < 0x90 + 2 * ng or nd == 0 or nd > 5000 or dpt + 2 * nd > len(b):
        return None
    glyphs = []
    for p in struct.unpack_from("<%dH" % ng, b, 0x90):
        w, h, adv, z = struct.unpack_from("<4H", b, p)
        n = (4 * w * h + 1) // 2
        glyphs.append((w, h, adv, bytes(b[p + 8:p + 8 + n])))
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
    """[(file, ordinal, glyphs, dialogues)] for every WFM2 block of the disc (GAM files unpacked first)."""
    out = []
    for name in sorted(files):
        d = files[name]
        if d[:4] == b"GAM\0":
            try:
                d = gam_py.unpack(d)
            except Exception:
                continue
        i, k = d.find(b"WFM2"), 0
        while i >= 0:
            r = parse_wfm2(d[i:])
            if r:
                out.append((name, k, r[0], r[1]))
                k += 1
            i = d.find(b"WFM2", i + 4)
    return out


def compose(t):
    """kana + separate dakuten/handakuten glyph (the font has no voiced kana) -> composed kana."""
    def f(m):
        c = unicodedata.normalize("NFC", m.group(1) + MARKS[m.group(2)])
        return c if len(c) == 1 else m.group(0)
    return re.sub("([\u3041-\u30fe])([゛゜])", f, t)


def gkey(g):
    return "%d:%d:%s" % (g[1], g[0], hashlib.sha1(g[3]).hexdigest()[:12])


class Decoder:
    def __init__(self):
        self.cm = json.load(open(CHARMAP, encoding="utf-8"))
        self.unknown = {}      # key -> (id, glyph)

    def ch(self, g):
        k = gkey(g)
        c = self.cm.get(k)
        if c is None:
            if k not in self.unknown:
                self.unknown[k] = (len(self.unknown), g)
            return "{G%d}" % self.unknown[k][0]
        return c

    def text(self, glyphs, ws):
        out, i = [], 0
        while i < len(ws):
            v = ws[i]
            i += 1
            if v in CTRL:
                nm, na = CTRL[v]
                a = ws[i:i + na]
                i += na
                out.append("{%s%s}" % (nm, "".join(" %d" % x for x in a)))
                if nm in ("NL", "CLEAR", "BOX"):
                    out.append("\n")
            elif v >= 0x8000 and (v & 0xFFF) < len(glyphs):
                fl, c = v >> 12, self.ch(glyphs[v & 0xFFF])
                if fl == 0xD and i < len(ws) and ws[i] >> 12 == 0xE and (ws[i] & 0xFFF) < len(glyphs):
                    m = self.ch(glyphs[ws[i] & 0xFFF])
                    i += 1
                    out.append(unicodedata.normalize("NFC", c + MARKS[m]) if m in MARKS and len(c) == 1 else c + m)
                elif fl == 8:
                    out.append(c)
                else:
                    out.append("{%X:%s}" % (fl, c))
            else:
                out.append("{%04X}" % v)
        return compose("".join(out)).rstrip("\n")


def skeleton(ws):
    """Language-independent shape of a dialogue: order of VOICE/CHOICE/POSE/HALT codes."""
    s, i = [], 0
    while i < len(ws):
        v = ws[i]
        i += 1
        if v in CTRL:
            nm, na = CTRL[v]
            i += na
            if nm in ("VOICE", "CHOICE", "POSE", "HALT") and (not s or s[-1] != nm):
                s.append(nm)
    return tuple(s)


def nglyphs(ws):
    if 0xFFF4 in ws:            # "<item>{F4}をつかった": PAL keeps only the name, the verb lives elsewhere
        ws = ws[:ws.index(0xFFF4)]
    return sum(1 for v in ws if 0x8000 <= v < 0xFFF0 and v >> 12 in (8, 0xD))


def align(jp, pal_dl):
    """Needleman-Wunsch on (skeleton, text length): {jp index: [pal index]} ([] = JP dialogue left without a partner)."""
    import math
    a = [(skeleton(ws), nglyphs(ws)) for ws in jp]
    b = [(skeleton(d.words), nglyphs(d.words)) for d in pal_dl]
    ratios = sorted((y[1] + 1) / (x[1] + 1) for x, y in zip(a, b))
    k = ratios[len(ratios) // 2] if ratios else 2.0
    gap = -1.5

    def sc(x, y):
        s = 2.0 if x[0] == y[0] else -2.0
        if x[1] == 0 and y[1] == 0:
            return s + 1.5
        return s + 1.5 - min(3.0, abs(math.log((y[1] + 1) / ((x[1] + 1) * k))) * 2)
    n, m = len(a), len(b)
    D = [[0.0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        D[i][0] = i * gap
    for j in range(1, m + 1):
        D[0][j] = j * gap
    for i in range(1, n + 1):
        Di, Dp, ai = D[i], D[i - 1], a[i - 1]
        for j in range(1, m + 1):
            Di[j] = max(Dp[j - 1] + sc(ai, b[j - 1]), Dp[j] + gap, Di[j - 1] + gap)
    out, i, j = {}, n, m
    while i > 0 and j > 0:
        if D[i][j] == D[i - 1][j - 1] + sc(a[i - 1], b[j - 1]):
            out[i - 1] = [j - 1]
            i, j = i - 1, j - 1
        elif D[i][j] == D[i - 1][j] + gap:
            out[i - 1] = []
            i -= 1
        else:
            j -= 1
    while i > 0:
        out[i - 1] = []
        i -= 1
    return out


def sjis_strings(files):
    out = []
    for name in sorted(files):
        if name != "SYSTEM/DSPSUB2.BIN":
            continue
        for m in re.finditer(rb"(?:[\x81-\x84\x88-\x9f\xe0-\xea][\x40-\x7e\x80-\xfc]){2,}", files[name]):
            try:
                s = m.group().decode("cp932").rstrip("　")
            except UnicodeDecodeError:
                continue
            if sum("ぁ" <= c <= "ヾ" or "一" <= c <= "鿿" for c in s) >= 2:
                out.append((name, m.start(), s))
    return out


def atlas(dec, path):
    from PIL import Image, ImageDraw
    items = sorted(dec.unknown.values())
    if not items:
        return
    S, cols, cell = 4, 12, 96
    A = Image.new("L", (cols * cell, ((len(items) + cols - 1) // cols) * cell), 255)
    d = ImageDraw.Draw(A)
    for n, (gid, g) in enumerate(items):
        w, h = 4 * g[0], g[1]
        px = []
        for b in g[3]:
            px += [b & 15, b >> 4]
        thr = 3 if h >= 20 else None
        im = Image.new("L", (w, h))
        im.putdata([0 if ((v >= 3) if thr else v not in (0, 2, 14, 15)) else 255 for v in px[:w * h]])
        x, y = (n % cols) * cell, (n // cols) * cell
        A.paste(im.resize((w * S, h * S), Image.NEAREST), (x + 4, y + 4))
        d.text((x + 4, y + h * S + 6), "G%d" % gid, fill=0)
    A.save(path)


PAL_SPSUB = "BFGIHSPWD"     # SYSTEM/DnSPSUB*.BIN: n = index of the language letter (B=0 ... S=5 ... D=8)


def pal_wfms(path, lang):
    """{name: (Wfm, [text])}: the PAL *<lang>.WFM files plus the WFM3 blocks of SYSTEM/D<n>SPSUB*.BIN (as SYSTEM/DSPSUB*.BIN#k)."""
    import wfm_py as W
    lay = cdfiles.layout_from_image(path)
    f = open(path, "rb")

    def rd(n):
        lba, size = lay[n]
        b = bytearray()
        for i in range((size + 2047) // 2048):
            f.seek((lba + i) * 2352 + 24)
            b += f.read(2048)
        return bytes(b[:size])
    blobs = {n: rd(n) for n in lay if n.endswith(lang + ".WFM")}
    sp = "SYSTEM/D%dSPSUB" % PAL_SPSUB.index(lang)
    for n in lay:
        if n.startswith(sp):
            d, k, i = rd(n), 0, 0
            i = d.find(b"WFM3")
            while i >= 0:
                blobs["SYSTEM/DSPSUB%s#%d" % (n[len(sp):].split(".")[0], k)] = d[i:]
                k += 1
                i = d.find(b"WFM3", i + 4)
    f.close()
    out = {}
    for n, b in blobs.items():
        try:
            w = W.decode(b)
        except Exception:
            continue
        out[n] = (w, [W.words_to_text(w, d.words, d.font) for d in w.dialogues])
    return out


def real(t):
    """True when a JP dialogue holds actual words (two kana/kanji at least)."""
    return sum("\u3041" <= c <= "\u3096" or "\u30a1" <= c <= "\u30fa" or "\u4e00" <= c <= "\u9fff" for c in t) >= 2


def flat(t):
    t = re.sub(r"\{(NL|CLEAR|WAIT|BOX[^}]*|VOICE[^}]*|POSE[^}]*|PAUSE[^}]*|COLOR[^}]*|MOVE[^}]*|F4|HALT|C0..)\}", " ", t)
    return re.sub(r"\s+", " ", t).strip()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("disc")
    ap.add_argument("out")
    ap.add_argument("--pal")
    ap.add_argument("--lang", default="S")
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    if os.path.exists(os.path.join(a.out, "_copias.txt")):
        os.remove(os.path.join(a.out, "_copias.txt"))
    files = read_disc(a.disc)
    conts = containers(files)
    dec = Decoder()
    pal = pal_wfms(a.pal, a.lang) if a.pal else {}
    seen, solo, stats = {}, [], collections.Counter()
    for name, k, glyphs, dl in conts:
        texts = [dec.text(glyphs, ws) for ws in dl]
        sig = hashlib.sha1(json.dumps(texts).encode()).hexdigest()
        tag = "%s_%d" % (name.replace("/", "_").rsplit(".", 1)[0], k)
        stats["containers"] += 1
        stats["dialogues"] += len(dl)
        if sig in seen:
            stats["duplicate containers"] += 1
            with open(os.path.join(a.out, "_copias.txt"), "a", encoding="utf-8") as f:
                f.write("%s = copia de %s\n" % (tag, seen[sig]))
            continue
        seen[sig] = tag
        # PAL counterpart: the WFM of the same area whose skeleton aligns best
        match, pmap = None, {}
        area = name.split("/")[0]
        best = 0
        a_sk = [skeleton(ws) for ws in dl]
        titles = sum(g[1] >= 20 for g in glyphs) * 2 > len(glyphs)   # event titles: PAL keeps one global list
        for pn, (pw, ptexts) in pal.items():
            if titles or not pn.startswith(area + "/"):
                continue
            sm = difflib.SequenceMatcher(None, a_sk, [skeleton(d.words) for d in pw.dialogues], autojunk=False)
            m = sum(x.size for x in sm.get_matching_blocks())
            if "#" in pn:       # SYSTEM/DSPSUB: same file and block number first
                m += len(dl) * (pn.split("#")[0] == name.rsplit(".", 1)[0]) + len(dl) * (pn.endswith("#%d" % k))
            if m > best:
                best, match, ops = m, pn, sm.get_opcodes()
        if match and best >= 0.5 * len(dl):
            ptexts = pal[match][1]
            pmap = align(dl, pal[match][0].dialogues)
            for i, t in enumerate(texts):
                if not real(t):
                    continue
                if pmap.get(i) == []:
                    solo.append((tag, i, match, t, "sin equivalente en PAL", ""))
                elif pmap.get(i) and not any(re.search(r"[A-Za-z0-9\u00c0-\u00ff]", flat(ptexts[j])) for j in pmap[i]):
                    solo.append((tag, i, match, t, "el diálogo PAL correspondiente está vacío o es un relleno",
                                 " / ".join(flat(ptexts[j]) for j in pmap[i])))
        else:
            match = None
        stats["matched containers" if match else "containers without PAL match"] += 1
        with open(os.path.join(a.out, tag + ".txt"), "w", encoding="utf-8") as f:
            f.write("# %s, bloque WFM2 %d: %d diálogos, %d glifos%s\n" % (
                name, k, len(dl), len(glyphs), ", PAL: %s" % match if match else ""))
            for i, t in enumerate(texts):
                f.write("\n## %d\n%s\n" % (i, t))
                if match and pmap.get(i):
                    for j in pmap[i]:
                        f.write("  [PAL %d] %s\n" % (j, flat(pal[match][1][j])))
    with open(os.path.join(a.out, "SYSTEM_DSPSUB_sjis.txt"), "w", encoding="utf-8") as f:
        for n, o, s in sjis_strings(files):
            f.write("%s 0x%05X %s\n" % (n, o, s))
    atlas(dec, os.path.join(a.out, "glifos_sin_identificar.png"))
    total = sum(1 for _ in dec.cm) + len(dec.unknown)
    with open(os.path.join(a.out, "SOLO_JAPON.txt"), "w", encoding="utf-8") as f:
        f.write("# Diálogos JP sin equivalente PAL (alineación por estructura de códigos; revisar a mano)\n")
        for tag, i, pn, t, why, pt in solo:
            f.write("\n== %s #%d (PAL %s): %s%s\n%s\n" % (tag, i, pn, why, " [PAL: %s]" % pt if pt else "", t))
    print(dict(stats), "glifos sin identificar:", len(dec.unknown), "JP-only candidatos:", len(solo))


if __name__ == "__main__":
    main()
