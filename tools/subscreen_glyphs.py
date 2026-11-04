#!/usr/bin/env python3
"""The Select sub-screen label fonts (the wooden tabs and the leaf buttons), as a glyph table.

The sub-screen texture page (4 bpp, 256x256 texels, VRAM x 64 words, y 0) is a raw file per language on the PAL disc:
SYSTEM/A<n>0301.000, n = port --lang + 1 (A1 English, A6 Spanish, A7 Portuguese, A8 Swedish, A9 Danish; A2..A5 are
French, German, Italian, Dutch).  Its labels are drawn by hand into the picture: the tabs ("Item", "Event", "Status",
"Exit", "Map") in a dark red font with a shaded edge cut into the plank, the leaf buttons ("Use item", "View", "Sort")
in a white font with a grey edge.  The planks and the leaf are the same picture in every language, so the picture
without text is recovered texel by texel from the languages (all nine pages on the disc) that have no letter near that
texel (see clean()); a glyph is then its letter texels plus its edge: the texels near them that differ from that clean
picture.  The letters are found automatically (8-connected pieces, split by columns where letters touch; OVERRIDES
where that fails); the labels' split shows in --atlas.

  python3 tools/subscreen_glyphs.py --data data --json tools/subscreen_glyphs.json --header port2/runtime/subscreen_glyphs.h
  python3 tools/subscreen_glyphs.py --data data --atlas /tmp/x/atlas.png     # inspection only: never commit the PNG
  python3 tools/subscreen_glyphs.py --disc game/disc/Tombi.bin --atlas ...  # straight from the image

The JSON and the header hold coordinates only (and a checksum per source label, to recognise the right files); the
glyph pixels are cut from the user's own disc at run time (port2/runtime/subscreen_es.c) and by --atlas."""
import argparse, collections, json, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
PAGE_FILE = "SYSTEM/A%d0301.000"
PAGE_X = 64                               # VRAM x of the page, in 16-bit words (y 0)
LANG = {1: "en", 2: "fr", 3: "de", 4: "it", 5: "nl", 6: "es", 7: "pt", 8: "sv", 9: "da"}
TABLE_LANGS = (1, 2, 3, 4, 5, 6, 7, 8, 9)  # every page on the disc (1..4 are the boot menu's of SCES-01330; same files)
NEAR = 2                                  # texels within NEAR (Chebyshev) of a letter belong to it (its shaded edge)

# fonts: which palette indices are the letters, the rects ("slots") that share one background picture, the labels
FONTS = {
    "tab": {
        "text": (1,),
        "pair_gap": 0,                    # between letters from different words ("Objetos" fits the 64-texel plank)
        "slots": {                        # u, v, w, h; each tab its own plank picture
            "item":   (0x98, 0x38, 64, 24),
            "event":  (0x98, 0x50, 64, 24),
            "status": (0x78, 0x68, 64, 24),
            "exit":   (0xB8, 0x68, 48, 24),
            "map":    (0x78, 0x80, 64, 24),
        },
        "groups": [["item"], ["event"], ["status"], ["exit"], ["map"]],
        "labels": {                       # slot -> {file number: text}
            "item":   {1: "Item", 2: "Objet", 3: "Objekt", 4: "Oggetti", 5: "Vrwerp.", 6: "Elmto.", 7: "Item", 8: "Objekt", 9: "Objekt"},
            "event":  {1: "Event", 2: "Fait", 3: "Ereign.", 4: "Eventi", 5: "Event", 6: "Evento", 7: "Evento", 8: "Action", 9: "Handln."},
            "status": {1: "Status", 2: "Etat", 3: "Status", 4: "Stato", 5: "Status", 6: "Estado", 7: "Estado", 8: "Status", 9: "Status"},
            "exit":   {1: "Exit", 2: "Quitter", 3: "Beenden", 4: "Esci", 5: "Weg", 6: "Salir", 7: "Sair", 8: "Avbryt", 9: "Afslut"},
            "map":    {1: "Map", 2: "Carte", 3: "Karte", 4: "Mappa", 5: "Kaart", 6: "Mapa", 7: "Mapa", 8: "Karta", 9: "Kort"},
        },
    },
    "leaf": {
        "text": (1,),                     # the white; its grey rim (2) goes with the edge, like the tabs' shading
        "pair_gap": 0,
        "slots": {
            "use":  (0x28, 0x40, 56, 39),
            "view": (0x08, 0x68, 56, 39),
            "sort": (0x40, 0x68, 56, 39),
        },
        "groups": [["use", "view", "sort"]],   # the three buttons are one leaf picture
        "labels": {                       # two-line labels: "line1|line2"
            "use":  {1: "Use|item", 2: "Utiliser|Objet", 3: "Obj.|verw.", 4: "Usa", 5: "Gebruik", 6: "Usar|elmto", 7: "Usar|item", 8: "Anv|Obj", 9: "Brug|Objekt"},
            "view": {1: "View", 2: "Vue", 3: "Ansicht", 4: "Guarda", 5: "Bekijk", 6: "Ver", 7: "Ver", 8: "Visa", 9: "Vis"},
            "sort": {1: "Sort", 2: "Tri", 3: "Sort.", 4: "Ordina", 5: "Sorteer", 6: "ordenar", 7: "Escolhe", 8: "Sort", 9: "Sorter"},
        },
    },
}
# where one glyph of a character is preferred over the first one found: (font, char) -> (file number, slot, index in
# label), chosen for the Spanish "Objetos" / "objeto" (subscreen_es.c), so that as many neighbours as possible keep
# their original spacing: the tab's O b j e from Swedish "Objekt", t o from Spanish "Elmto.", s from "Estado"; the
# leaf's b j from Swedish "Obj", e o from Spanish "ordenar" (the "elmto" o touches its t), t from "elmto".
PREFER = {
    ("tab", "O"): (8, "item", 0), ("tab", "b"): (8, "item", 1), ("tab", "j"): (8, "item", 2),
    ("tab", "e"): (8, "item", 3), ("tab", "k"): (8, "item", 4), ("tab", "t"): (6, "item", 3),
    ("tab", "o"): (6, "item", 4), ("tab", "s"): (6, "status", 1),
    ("leaf", "O"): (8, "use", 3), ("leaf", "b"): (8, "use", 4), ("leaf", "j"): (8, "use", 5),
    ("leaf", "e"): (6, "sort", 3), ("leaf", "t"): (6, "use", 7), ("leaf", "o"): (6, "sort", 0),
    ("leaf", "s"): (6, "use", 1), ("leaf", "a"): (6, "use", 2),   # "sa" of "Usar" as the Spanish leaf writes it
    ("leaf", "k"): (5, "view", 2), ("leaf", "w"): (1, "view", 3),   # clean cuts: "Gebruik" / "verw." run their letters together
    ("leaf", "r"): (6, "view", 2), ("leaf", "u"): (4, "view", 1), ("leaf", "n"): (3, "view", 1),   # clean cuts (the Spanish "Usar" r drags a piece of
    # the a, the Dutch "Gebruik" letters touch); u from the Italian "Guarda", n from the German "Ansicht" (the user's picks)
}
# labels whose letters run into each other across the lines (Danish "Brug": the g's tail touches "Objekt"): the
# letters' rects, label-relative (x0, y0, x1, y1), in reading order
OVERRIDES = {
    # Swedish "Objekt": the j's dot sits over the b and the column split gave it to the b
    ("tab", 8, "item"): [[(9, 4, 19, 19)], [(20, 8, 28, 18)], [(28, 6, 36, 19)], [(36, 9, 44, 18)], [(44, 8, 50, 18)],
                         [(50, 8, 58, 18)]],                                                  # O b j e k t
    # Danish "Handln.": the H's right stem touches the a, so the column split gave it to the a
    ("tab", 9, "event"): [[(7, 5, 18, 19)], [(18, 7, 26, 18)], [(26, 9, 35, 18)], [(35, 7, 43, 18)], [(44, 7, 47, 18)],
                          [(47, 9, 55, 18)], [(55, 15, 58, 18)]],                          # H a n d l n .
    ("leaf", 9, "use"): [
        [(6, 7, 17, 21)], [(18, 10, 25, 20)], [(26, 11, 36, 20)], [(37, 10, 47, 23)],            # B r u g
        [(6, 22, 15, 32)], [(15, 22, 21, 33)], [(21, 21, 27, 35)], [(27, 23, 34, 33)],           # O b j e
        [(34, 23, 40, 33)], [(40, 23, 47, 33)]],                                                 # k t
}
WIDE, NARROW = set("mwMWHAK"), set("ijlI.,'!t")


# ---- reading the pages
def read_pages(data=None, disc=None):
    out = {}
    if disc:
        sys.path.insert(0, HERE)
        import extract_data
        img = extract_data.Image(disc)
        try:
            files = {p.upper(): (l, s) for p, l, s in img.files()}
            for n in LANG:
                e = files.get(PAGE_FILE % n)
                if e:
                    out[n] = img.user(e[0], (e[1] + 2047) // 2048)[:e[1]]
        finally:
            img.close()
    else:
        for n in LANG:
            p = os.path.join(data, *(PAGE_FILE % n).split("/"))
            if os.path.isfile(p):
                out[n] = open(p, "rb").read()
    return {n: d for n, d in out.items() if len(d) == 32768}


def texel(page, u, v):
    b = page[v * 128 + (u >> 1)]
    return b >> 4 if u & 1 else b & 15


def label_hash(page, r):
    """FNV-1a over the rect's texel indices (the same as subscreen_es.c's hash_rect)."""
    h = 2166136261
    u0, v0, w, hh = r
    for v in range(hh):
        for u in range(w):
            h = ((h ^ texel(page, u0 + u, v0 + v)) * 16777619) & 0xFFFFFFFF
    return h


# ---- the picture without text (mirrored by subscreen_es.c's clean_group)
def text_mask(page, r, text):
    u0, v0, w, h = r
    return [[texel(page, u0 + x, v0 + y) in text for x in range(w)] for y in range(h)]


def near(mask, x, y):
    h, w = len(mask), len(mask[0])
    for yy in range(max(0, y - NEAR), min(h, y + NEAR + 1)):
        for xx in range(max(0, x - NEAR), min(w, x + NEAR + 1)):
            if mask[yy][xx]:
                return True
    return False


def clean(pages, font, group):
    """The group's picture without letters: per texel, the most common value among the (language, slot) samples
    with no letter within NEAR (ties: the lower palette index).  Where every sample has a letter near, the nearest
    such texel on the same row (left first: the planks' grain runs along the rows); on a row with none, the most
    common value among the samples that are not letters themselves; failing that, 0."""
    f = FONTS[font]
    rects = [f["slots"][s] for s in group]
    w, h = rects[0][2], rects[0][3]
    samples = []
    for n in sorted(pages):
        for r in rects:
            samples.append((pages[n], r, text_mask(pages[n], r, f["text"])))

    def mode(x, y, strict):
        cnt = [0] * 16
        for pg, r, m in samples:
            if not (near(m, x, y) if strict else m[y][x]):
                cnt[texel(pg, r[0] + x, r[1] + y)] += 1
        return cnt.index(max(cnt)) if max(cnt) else None

    out = [[mode(x, y, True) for x in range(w)] for y in range(h)]
    for y in range(h):
        row = out[y][:]
        for x in range(w):
            if row[x] is not None:
                continue
            for d in range(1, w):
                if x - d >= 0 and row[x - d] is not None:
                    out[y][x] = row[x - d]
                    break
                if x + d < w and row[x + d] is not None:
                    out[y][x] = row[x + d]
                    break
            else:
                v = mode(x, y, False)
                out[y][x] = v if v is not None else 0
    return out


# ---- segmentation: a letter is a set of letter texels, given in the table as its rect (their bounding box) and one
# seed texel per 8-connected piece; letter_texels() rebuilds it from those (as subscreen_es.c's cut_glyph does)
def components(mask, pts):
    pts = set(pts)
    out = []
    while pts:
        p = min(pts, key=lambda q: (q[1], q[0]))
        stack, comp = [p], set()
        pts.discard(p)
        while stack:
            x, y = stack.pop()
            comp.add((x, y))
            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    q = (x + dx, y + dy)
                    if q in pts:
                        pts.discard(q)
                        stack.append(q)
        out.append(comp)
    return out


def letter_texels(mask, rect, seeds):
    """The letter texels 8-connected to the seeds inside rect (x0, y0, x1, y1): the runtime's reconstruction."""
    x0, y0, x1, y1 = rect
    pts = {(x, y) for y in range(y0, y1) for x in range(x0, x1) if mask[y][x]}
    out = set()
    for s in seeds:
        if tuple(s) in pts and tuple(s) not in out:
            for c in components(None, pts):
                if tuple(s) in c:
                    out |= c
    return out


def expect_w(c):
    return 10 if c in WIDE else 3 if c in NARROW else 7


def lines_of(mask, n):
    """Row ranges of n text lines: split at the emptiest row near the middle (two-line leaves)."""
    rows = [sum(r) for r in mask]
    ys = [y for y, c in enumerate(rows) if c]
    if n == 1:
        return [(ys[0], ys[-1] + 1)]
    lo, hi = ys[0], ys[-1]
    mid = (lo + hi) // 2
    cut = min(range(mid - 5, mid + 6), key=lambda y: (rows[y], abs(y - mid)))
    return [(lo, cut), (cut, hi + 1)]


def split_cols(texels, k, word):
    """Splits one piece holding k letters at column cuts: few texels on the cut, widths near the expected ones."""
    xs = sorted({x for x, _ in texels})
    a, b = xs[0], xs[-1] + 1
    col = [0] * (b + 1)
    for x, _ in texels:
        col[x] += 1
    INF = float("inf")
    best = {(0, a): (0, None)}
    for i in range(k):
        for x in range(a, b + 1):
            if (i, x) not in best:
                continue
            c0 = best[(i, x)][0]
            for e in range(x + 1, b + 1):
                if i == k - 1 and e != b:
                    continue
                cost = c0 + abs((e - x) - expect_w(word[i]))
                if e < b:
                    cost += 3 * min(col[e - 1], col[e])
                if cost < best.get((i + 1, e), (INF,))[0]:
                    best[(i + 1, e)] = (cost, x)
    cuts, e = [], b
    for i in range(k, 0, -1):
        x = best[(i, e)][1]
        cuts.append(x)
        e = x
    cuts = cuts[::-1][1:]
    parts = [set() for _ in range(k)]
    for p in texels:
        parts[sum(1 for c in cuts if p[0] >= c)].add(p)
    return parts


def segment(mask, lines, words, override=None):
    """The letters of every line: the 8-connected pieces, small ones above another piece (dots, accents) joined to
    it, then pieces holding several letters split by columns, as the word's expected widths say.  With an override,
    each letter is the letter texels in its rects (label-relative x0, y0, x1, y1) that no earlier letter took."""
    pts = [(x, y) for y in range(len(mask)) for x in range(len(mask[0])) if mask[y][x]]
    if override:
        taken, flat = set(), []
        for rects in override:
            t = {(x, y) for x, y in pts if any(a <= x < c and b <= y < d for a, b, c, d in rects)} - taken
            taken |= t
            flat.append(t)
        out, i = [], 0
        for word in words:
            out.append(flat[i:i + len(word)])
            i += len(word)
        return out
    comps = components(None, pts)
    out = []
    for (y0, y1), word in zip(lines, words):
        cs = [c for c in comps if y0 <= sum(y for _, y in c) / len(c) < y1]
        big = [c for c in cs if len(c) >= 6]
        groups = [set(c) for c in big]
        for c in cs:
            if len(c) >= 6:
                continue
            cx = {x for x, _ in c}
            cy = max(y for _, y in c)
            over = [g for g in groups if any(x in cx and y > cy for x, y in g)]
            if over:
                over[0] |= c
            else:
                groups.append(set(c))
        groups.sort(key=lambda g: min(x for x, _ in g))
        while len(groups) > len(word):       # stray specks: join the smallest to its nearest neighbour
            i = min(range(len(groups)), key=lambda j: len(groups[j]))
            g = groups.pop(i)
            j = min(range(len(groups)), key=lambda j: abs(min(x for x, _ in groups[j]) - min(x for x, _ in g)))
            groups[j] |= g
        # letters per piece: a monotone partition of the word whose expected widths fit the pieces' widths
        m, k = len(groups), len(word)
        wid = [max(x for x, _ in g) + 1 - min(x for x, _ in g) for g in groups]
        INF = float("inf")
        dp = {(0, 0): (0, None)}
        for j in range(m):
            for i in range(k):
                if (j, i) not in dp:
                    continue
                for n in range(1, k - i - (m - j - 1) + 1):
                    cost = dp[(j, i)][0] + abs(wid[j] - sum(expect_w(c) for c in word[i:i + n]) - (n - 1))
                    if cost < dp.get((j + 1, i + n), (INF,))[0]:
                        dp[(j + 1, i + n)] = (cost, i)
        counts, i = [], k
        for j in range(m, 0, -1):
            pi = dp[(j, i)][1]
            counts.append(i - pi)
            i = pi
        counts = counts[::-1]
        letters, i = [], 0
        for g, n in zip(groups, counts):
            parts = split_cols(g, n, word[i:i + n]) if n > 1 else [g]
            letters += parts
            i += n
        out.append(letters)
    return out


def describe(texels):
    """rect (x0, y0, x1, y1) and seeds: the topmost-leftmost texel of each 8-connected piece."""
    xs = [x for x, _ in texels]
    ys = [y for _, y in texels]
    seeds = [min(c, key=lambda q: (q[1], q[0])) for c in components(None, texels)]
    return (min(xs), min(ys), max(xs) + 1, max(ys) + 1), sorted(seeds, key=lambda q: (q[0], q[1]))


def build(pages):
    table = {"page": {"file": PAGE_FILE, "file_number": "port --lang + 1", "vram_x_words": PAGE_X, "vram_y": 0,
                      "bpp": 4, "near": NEAR, "langs": {str(n): LANG[n] for n in LANG}},
             "fonts": {}}
    cleans = {}
    for font, f in FONTS.items():
        for g in f["groups"]:
            c = clean(pages, font, g)
            for s in g:
                cleans[(font, s)] = c
        labels, glyphs, gaps = [], {}, []
        for slot, per in f["labels"].items():
            r = f["slots"][slot]
            for n, txt in sorted(per.items()):
                if n not in pages:
                    continue
                pg = pages[n]
                m = text_mask(pg, r, f["text"])
                words = txt.split("|")
                lab = {"file": n, "lang": LANG[n], "slot": slot, "rect": list(r), "text": " / ".join(words),
                       "hash": "0x%08x" % label_hash(pg, r), "lines": []}
                gi = 0
                lines = lines_of(m, len(words))
                for (y0, y1), word, letters in zip(lines, words, segment(m, lines, words, OVERRIDES.get((font, n, slot)))):
                    descs = [describe(t) for t in letters]
                    for t, (rc, sd) in zip(letters, descs):
                        assert letter_texels(m, rc, sd) == t, (n, slot, word)
                    bots = [rc[3] - 1 for (rc, _), ch in zip(descs, word) if ch not in "gjpqy.,'"]
                    base = max(set(bots), key=lambda b: (bots.count(b), b)) if bots else y1 - 1
                    line = {"text": word, "baseline": r[1] + base, "glyphs": []}
                    for i, ((rc, sd), ch) in enumerate(zip(descs, word)):
                        adv = descs[i + 1][0][0] - rc[0] if i + 1 < len(descs) else None
                        if adv is not None:
                            gaps.append(descs[i + 1][0][0] - rc[2])
                        gl = {"char": ch, "u": r[0] + rc[0], "v": r[1] + rc[1], "w": rc[2] - rc[0], "h": rc[3] - rc[1],
                              "baseline": r[1] + base, "advance": adv,
                              "seeds": [[r[0] + x, r[1] + y] for x, y in sd]}
                        line["glyphs"].append(gl)
                        pick = PREFER.get((font, ch))
                        src = {"file": n, "lang": LANG[n], "slot": slot, "index": gi}
                        if (pick is None and ch not in glyphs) or pick == (n, slot, gi):
                            glyphs[ch] = dict(src, **{k: gl[k] for k in ("u", "v", "w", "h", "baseline", "advance", "seeds")})
                        gi += 1
                    lab["lines"].append(line)
                labels.append(lab)
        gap = max(set(gaps), key=gaps.count) if gaps else 1
        for ch, g in glyphs.items():
            if g["advance"] is None:
                g["advance"] = g["w"] + gap
        table["fonts"][font] = {"text_indices": list(f["text"]), "gap": gap, "pair_gap": f["pair_gap"],
                                "slots": {k: list(v) for k, v in f["slots"].items()}, "groups": f["groups"],
                                "glyphs": dict(sorted(glyphs.items())), "labels": labels}
    sys.path.insert(0, HERE)
    import subscreen_extra_glyphs as E
    for fn, chars in E.REPLACE.items():      # letters ours replace (subscreen_extra_glyphs.py): out of the picks
        for ch in chars:
            table["fonts"][fn]["glyphs"].pop(ch, None)
    for fn, f in table["fonts"].items():     # the shaded edge / rim index our own letters get (subscreen_extra_glyphs.py)
        cnt = collections.Counter()
        for ch, g in f["glyphs"].items():
            for (x, y), t in glyph_texels(pages[g["file"]], fn, g["slot"], g, cleans[(fn, g["slot"])]).items():
                if t not in FONTS[fn]["text"]:
                    cnt[t] += 1
        f["edge"] = cnt.most_common(1)[0][0] if cnt else 0
    return table, cleans


def glyph_texels(pg, font, slot, g, cl):
    """{(u, v): index} of a glyph: its letter texels and the texels of its shaded edge (within NEAR of the letter,
    not nearer another letter texel, and unlike the clean picture).  Mirrors subscreen_es.c's cut_glyph."""
    f = FONTS[font]
    r = f["slots"][slot]
    m = text_mask(pg, r, f["text"])
    rc = (g["u"] - r[0], g["v"] - r[1], g["u"] - r[0] + g["w"], g["v"] - r[1] + g["h"])
    let = letter_texels(m, rc, [(x - r[0], y - r[1]) for x, y in g["seeds"]])
    out = {}
    for y in range(max(0, rc[1] - NEAR), min(r[3], rc[3] + NEAR)):
        for x in range(max(0, rc[0] - NEAR), min(r[2], rc[2] + NEAR)):
            t = texel(pg, r[0] + x, r[1] + y)
            if (x, y) in let:
                out[(r[0] + x, r[1] + y)] = t
                continue
            if m[y][x] or t == cl[y][x]:
                continue
            d_own = d_other = 99
            for yy in range(max(0, y - NEAR), min(r[3], y + NEAR + 1)):
                for xx in range(max(0, x - NEAR), min(r[2], x + NEAR + 1)):
                    if m[yy][xx]:
                        d = max(abs(xx - x), abs(yy - y))
                        if (xx, yy) in let:
                            d_own = min(d_own, d)
                        else:
                            d_other = min(d_other, d)
            if d_own <= NEAR and d_own <= d_other:
                out[(r[0] + x, r[1] + y)] = t
    return out


# ---- outputs
def write_header(table, path):
    """port2/runtime/subscreen_glyphs.h: the same table as C arrays (every glyph of every label, the picks)."""
    fonts = list(table["fonts"])
    L = ["/* Generated by tools/subscreen_glyphs.py from tools/subscreen_glyphs.json -- do not edit, rerun the tool.",
         " * The Select sub-screen label fonts as coordinates only: the glyphs are cut at run time from the disc's",
         " * " + table["page"]["file"].replace("%d", "<n>") + " (n = --lang + 1), see port2/runtime/subscreen_es.c. */",
         "#ifndef SUBSCREEN_GLYPHS_H", "#define SUBSCREEN_GLYPHS_H", "",
         "typedef struct { unsigned char font, group, u, v, w, h; } SubSlot;",
         "typedef struct { unsigned char baseline, nglyphs; unsigned short glyph; } SubLine;",
         "typedef struct { unsigned char font, file, slot, nlines; unsigned short line; unsigned hash; const char *text; } SubLabel;",
         "/* a letter: the letter texels 8-connected to its seeds inside its rect (u, v, w, h); advance: the step to the next",
         "   letter of its word (-1: the last) */",
         "typedef struct { char ch; unsigned char u, v, w, h, baseline; signed char advance; unsigned char nseeds;",
         "                 unsigned short seed, label; unsigned char index; } SubGlyph;",
         "typedef struct { unsigned short text_mask; unsigned char gap, pair_gap; } SubFont;", ""]
    L.append("enum { %s };" % ", ".join("SUB_FONT_%s = %d" % (fn.upper(), i) for i, fn in enumerate(fonts)))
    L.append("#define SUB_NEAR %d             /* texels this close to a letter are its edge */" % table["page"]["near"])
    L.append("#define SUB_PAGE_FILE \"%s\"" % table["page"]["file"])
    L.append("static const SubFont sub_fonts[%d] = {   /* text_mask: a bit per palette index of the letters */" % len(fonts))
    for fn in fonts:
        f = table["fonts"][fn]
        L.append("    { 0x%04x, %d, %d },   /* %s */" % (sum(1 << i for i in f["text_indices"]), f["gap"], f["pair_gap"], fn))
    L.append("};")
    slots, slot_ix, groups = [], {}, 0
    for fi, fn in enumerate(fonts):
        f = table["fonts"][fn]
        for g in f["groups"]:
            for s in g:
                slot_ix[(fn, s)] = len(slots)
                slots.append("    { %d, %d, 0x%02X, 0x%02X, %d, %d },   /* %s %s */" % ((fi, groups) + tuple(f["slots"][s]) + (fn, s)))
            groups += 1
    L.append("/* the label rects; the slots of one group show one picture behind their text */")
    L.append("#define SUB_NGROUPS %d" % groups)
    L.append("static const SubSlot sub_slots[%d] = {" % len(slots))
    L += slots + ["};"]
    labels, lines, glyphs, seeds, picks = [], [], [], [], []
    inst = {}
    for fi, fn in enumerate(fonts):
        f = table["fonts"][fn]
        for lab in f["labels"]:
            li = len(labels)
            labels.append("    { %d, %d, %d, %d, %d, %su, \"%s\" },   /* %s */" % (
                fi, lab["file"], slot_ix[(fn, lab["slot"])], len(lab["lines"]), len(lines), lab["hash"],
                lab["text"].replace('"', '\\"'), lab["lang"]))
            k = 0
            for ln in lab["lines"]:
                lines.append("    { 0x%02X, %d, %d }," % (ln["baseline"], len(ln["glyphs"]), len(glyphs)))
                for g in ln["glyphs"]:
                    inst[(fn, lab["file"], lab["slot"], k)] = len(glyphs)
                    glyphs.append("    { '%s', 0x%02X, 0x%02X, %d, %d, 0x%02X, %d, %d, %d, %d, %d },   /* %s %s */" % (
                        g["char"].replace("\\", "\\\\").replace("'", "\\'"), g["u"], g["v"], g["w"], g["h"], g["baseline"],
                        -1 if g["advance"] is None else g["advance"], len(g["seeds"]), len(seeds), li, k, lab["lang"], lab["text"]))
                    seeds += g["seeds"]
                    k += 1
        for ch, g in f["glyphs"].items():
            picks.append("    { %d, '%s', %d },   /* %s %s #%d */" % (fi, ch.replace("\\", "\\\\").replace("'", "\\'"),
                                                                   inst[(fn, g["file"], g["slot"], g["index"])],
                                                                   g["lang"], g["slot"], g["index"]))
    L.append("static const SubLabel sub_labels[%d] = {" % len(labels))
    L += labels + ["};"]
    L.append("static const SubLine sub_lines[%d] = {" % len(lines))
    L += lines + ["};"]
    L.append("static const SubGlyph sub_glyphs[%d] = {" % len(glyphs))
    L += glyphs + ["};"]
    L.append("static const unsigned char sub_seeds[%d][2] = {" % len(seeds))
    for i in range(0, len(seeds), 8):
        L.append("    " + " ".join("{ 0x%02X, 0x%02X }," % tuple(s) for s in seeds[i:i + 8]))
    L.append("};")
    L.append("/* the font: one glyph per character (font, character, index in sub_glyphs) */")
    L.append("typedef struct { unsigned char font; char ch; unsigned short glyph; } SubPick;")
    L.append("static const SubPick sub_picks[%d] = {" % len(picks))
    L += picks + ["};"]
    # our own letters (tools/subscreen_extra_glyphs.py): rows of ink bits (bit x = column x), the marks as U+0301 / U+0303
    sys.path.insert(0, HERE)
    import subscreen_extra_glyphs as E
    extras, rows = [], []
    for fi, fn in enumerate(fonts):
        items = list((E.TAB if fn == "tab" else E.LEAF).items()) + [("\u0301", (0, E.ACUTE)), ("\u0300", (0, E.GRAVE)),
                                                                     ("\u0308", (0, E.DIAERESIS)), ("\u0303", (0, E.TILDE)), ("\u0327", (0, E.CEDILLA))]
        for ch, (bl, bitmap) in items:
            w = len(bitmap[0])
            extras.append("    { %d, 0x%04X, %d, %d, %d, %d },   /* %s %s */" % (fi, ord(ch), w, len(bitmap), bl, len(rows), fn, ch if ord(ch) > 0x20 and ord(ch) < 0x300 else "mark"))
            rows += [sum(1 << x for x, c in enumerate(r) if c == "#") for r in bitmap]
    L.append("/* our own letters (not the disc's): ink bitmaps, the edge / rim added at run time; baseline = row index */")
    L.append("typedef struct { unsigned char font; unsigned short ch; unsigned char w, h, baseline; unsigned short rows; } SubExtra;")
    L.append("static const unsigned char sub_edge[%d] = { %s };   /* the shaded edge (tab) / grey rim (leaf) palette index */" % (
        len(fonts), ", ".join(str(table["fonts"][fn].get("edge", 0)) for fn in fonts)))
    L.append("static const SubExtra sub_extra[%d] = {" % len(extras))
    L += extras + ["};"]
    L.append("static const unsigned short sub_extra_rows[%d] = {" % len(rows))
    for i in range(0, len(rows), 12):
        L.append("    " + " ".join("0x%04X," % r for r in rows[i:i + 12]))
    L += ["};", "", "#endif", ""]
    open(path, "w").write("\n".join(L))


def shade(font, t):
    """A stand-in palette for the atlas (the real CLUTs are not in the page file): letters bright, the rest a ramp."""
    if font == "tab":
        return (220, 40, 40) if t == 1 else (150 - 9 * t, 105 - 7 * t, 60 - 4 * t) if t < 13 else (170, 120, 70)
    return (255, 255, 255) if t == 1 else (170, 200, 170) if t == 2 else (20 + 10 * t, 60 + 9 * t, 20 + 5 * t)


def write_atlas(table, pages, cleans, path, zoom=4):
    """Top: every glyph of the table cut out (green tick: the baseline).  Below: every label, with the letters the
    segmentation found in alternating tints, so a wrong cut shows."""
    from PIL import Image, ImageDraw
    tiles = []
    for fn, f in table["fonts"].items():
        for ch, g in f["glyphs"].items():
            tx = glyph_texels(pages[g["file"]], fn, g["slot"], g, cleans[(fn, g["slot"])])
            x0 = min(x for x, _ in tx)
            top = g["baseline"] - 20
            im = Image.new("RGB", (max(x for x, _ in tx) - x0 + 3, 28), (30, 30, 45))
            for (x, y), t in tx.items():
                if 0 <= y - top < 28:
                    im.putpixel((x - x0 + 2, y - top), shade(fn, t))
            im.putpixel((0, 20), (0, 220, 0))
            tiles.append(("%s %s" % (fn, ch), im))
    for fn, f in table["fonts"].items():
        for lab in f["labels"]:
            u0, v0, w, h = lab["rect"]
            pg = pages[lab["file"]]
            tm = set(f["text_indices"])
            im = Image.new("RGB", (w, h))
            for y in range(h):
                for x in range(w):
                    im.putpixel((x, y), tuple(c // 3 for c in shade(fn, texel(pg, u0 + x, v0 + y))))
            m = text_mask(pg, lab["rect"], tm)
            tints = [(255, 80, 80), (80, 200, 255), (255, 230, 60), (120, 255, 120)]
            k = 0
            for line in lab["lines"]:
                for g in line["glyphs"]:
                    rc = (g["u"] - u0, g["v"] - v0, g["u"] - u0 + g["w"], g["v"] - v0 + g["h"])
                    for x, y in letter_texels(m, rc, [(a - u0, b - v0) for a, b in g["seeds"]]):
                        im.putpixel((x, y), tints[k % 4])
                    k += 1
            tiles.append(("%s %s" % (lab["lang"], lab["text"]), im))
    cols, W, H = 8, 70 * zoom // 1, 40 * zoom + 14
    out = Image.new("RGB", (cols * W, (len(tiles) + cols - 1) // cols * H), (15, 15, 15))
    d = ImageDraw.Draw(out)
    for i, (name, im) in enumerate(tiles):
        x, y = i % cols * W, i // cols * H
        out.paste(im.resize((im.width * zoom, im.height * zoom), Image.NEAREST), (x, y + 12))
        d.text((x + 2, y), name, fill=(255, 255, 0))
    out.save(path)


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--data", default=os.path.join(os.path.dirname(HERE), "data"))
    ap.add_argument("--disc")
    ap.add_argument("--json", help="write the glyph table (coordinates only)")
    ap.add_argument("--header", help="write the C header generated from the table")
    ap.add_argument("--from-json", help="write --header from this JSON, without reading the disc")
    ap.add_argument("--atlas", help="a PNG of every glyph cut from the disc (scratch: never commit it)")
    a = ap.parse_args(argv)
    if a.from_json:
        write_header(json.load(open(a.from_json)), a.header)
        return 0
    pages = read_pages(a.data, a.disc)
    missing = [n for n in TABLE_LANGS if n not in pages]
    if missing:
        print("missing pages: %s" % ", ".join(PAGE_FILE % n for n in missing), file=sys.stderr)
        return 1
    table, cleans = build(pages)
    if a.json:
        with open(a.json, "w") as o:
            json.dump(table, o, indent=1, ensure_ascii=False)
            o.write("\n")
    if a.header:
        write_header(table, a.header)
    if a.atlas:
        write_atlas(table, pages, cleans, a.atlas)
    for fn, f in table["fonts"].items():
        print("%s: %d glyphs: %s" % (fn, len(f["glyphs"]), "".join(f["glyphs"])))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
