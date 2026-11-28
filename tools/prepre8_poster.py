#!/usr/bin/env python3
"""Draw the PrePre 8 ("Pure 8") events poster (docs/wiki/img/prepre8_events_poster.png) from docs/PREPRE8_EVENTS.md.

All 169 event titles in five columns, grouped by the audited area, each with its evidence tag, its fate in Tomba! 1
(and Tomba! 2) and its best description with the source tag. The doc is the only input (parsed by
tools/prepre8_wiki.py), and the image holds only text the script draws, so it can be rebuilt anywhere:

  python3 tools/prepre8_poster.py            draw the poster (the wiki build, tools/prepre8_wiki.py, also does)
  python3 tools/prepre8_poster.py -o X.png   draw it somewhere else

Needs Pillow. Fonts: Inter if installed (fonts-inter), else DejaVu Sans, else Pillow's built-in font. The PNG is
quantised to a small palette and carries a hash of its inputs that `prepre8_wiki.py --check` compares.
"""
import argparse, hashlib, os, re, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import prepre8_wiki as W  # noqa: E402

from PIL import Image, ImageDraw, ImageFont, PngImagePlugin  # noqa: E402

OUT = os.path.join(W.WIKI, "img", "prepre8_events_poster.png")
HASH_KEY = "prepre8-source-sha256"

# --- fonts -------------------------------------------------------------------------------------------------------
FONT_DIRS = ["/usr/share/fonts/opentype/inter", "/usr/share/fonts/truetype/inter", "/usr/share/fonts/inter",
             "/usr/local/share/fonts", os.path.expanduser("~/.local/share/fonts"), os.path.expanduser("~/.fonts")]
DEJAVU_DIRS = ["/usr/share/fonts/truetype/dejavu", "/usr/share/fonts/dejavu", "/usr/share/fonts/TTF"]
FALLBACK = {"Bold": "DejaVuSans-Bold.ttf", "SemiBold": "DejaVuSans-Bold.ttf", "Medium": "DejaVuSans.ttf",
            "Regular": "DejaVuSans.ttf"}


def font(weight, size):
    for d in FONT_DIRS:
        for ext in ("otf", "ttf"):
            p = os.path.join(d, "Inter-%s.%s" % (weight, ext))
            if os.path.exists(p):
                return ImageFont.truetype(p, size)
    for d in DEJAVU_DIRS:
        p = os.path.join(d, FALLBACK[weight])
        if os.path.exists(p):
            return ImageFont.truetype(p, size)
    try:
        return ImageFont.truetype(FALLBACK[weight], size)   # on the font path of the system
    except OSError:
        return ImageFont.load_default(size)


f_title = font("Bold", 64)
f_sub = font("Regular", 28)
f_head = font("Bold", 30)
f_hsub = font("Regular", 21)
f_name = font("SemiBold", 24)
f_num = font("Medium", 21)
f_fate = font("Regular", 21)
f_tag = font("Bold", 15)
f_desc = font("Regular", 19)
f_dtag = font("Bold", 15)
f_leg = font("Regular", 24)
f_legb = font("Bold", 24)

# --- colours and layout ------------------------------------------------------------------------------------------
BG = (17, 19, 24)
PANEL = (27, 30, 38)
TEXT = (232, 234, 240)
MUTED = (150, 156, 170)
FATE = {"same": (110, 214, 140), "cut": (235, 110, 110), "t2n": (205, 140, 255), "t2t": (240, 170, 80)}
SRC = {"Pure 8 dialogue": (255, 214, 102), "JP demo": (120, 200, 255), "Yoiko 2": (120, 200, 255),
       "Dengeki D5": (120, 200, 255), "retail EN": (190, 196, 210), "JP hint (translated)": (255, 160, 200),
       "none": (120, 124, 136)}
SHORT = {"JP hint (translated)": "JP HINT, TRANSLATED", "retail EN": "RETAIL EN", "none": "NONE",
         "Pure 8 dialogue": "PURE 8 DIALOGUE", "Dengeki D5": "DENGEKI D5", "JP demo": "JP DEMO", "Yoiko 2": "YOIKO 2"}
DTXT = (196, 200, 212)
LVC = {"E": ((60, 140, 255), "EVIDENCE"), "S": ((48, 170, 190), "STRONG"), "W": ((150, 130, 60), "WEAK"),
       "U": ((85, 88, 98), "UNKNOWN")}
LEVEL = {"evidence": "E", "strong": "S", "weak": "W", "unknown": "U"}

COLS = 5
COLW = 900
GAP = 40
MARGIN = 60
W_ = MARGIN * 2 + COLS * COLW + (COLS - 1) * GAP
TAGW = 118
INDENT = TAGW + 14 + 56     # tag, gap, number

# groups with no retail area get a fixed sub line instead of the counts (heading prefix -> text)
SUMMARY = [
    ("Pig Town", "Two titles name a Pig Town; where it was is not known."),
    ("Title cluster: the island", 'Consecutive titles that all name "the island". No data: a cluster, not an area.'),
    ("Title cluster: light, sun", "Consecutive titles about light, sun, cold, melting, snow. No data: a cluster, "
     "not an area."),
    ("Title cluster: sky", "Two consecutive sky titles. No data: a cluster, not an area."),
    ("Title cluster: caves", "Two consecutive cave titles. No data: a cluster, not an area."),
    ("Title cluster: light (", "Two consecutive light titles. No data: a cluster, not an area."),
    ("Title cluster: grand finale", "Three ending placeholders. No data: a cluster, not an area."),
    ("Data for more than one area", "Data places these in two or more areas."),
    ("Area unknown", "Theme only, or nothing: no area claimed."),
]
# small groups whose events never start a new column (they stay with their heading)
SMALL = ("Pig Town", "Title cluster: sky", "Title cluster: caves", "Title cluster: light (",
         "Title cluster: grand finale")


def wrap(text, fnt, width):
    words, lines, cur = text.split(" "), [], ""
    for w_ in words:
        t = (cur + " " + w_).strip()
        if fnt.getlength(t) <= width:
            cur = t
        else:
            lines.append(cur)
            cur = w_
    lines.append(cur)
    return lines


# --- what each event shows, from the doc's row -------------------------------------------------------------------
def in_retail(ev):
    return ev["fate"].startswith("retail")


def fate_of(ev):
    """(colour key, second line) from the Fate and Tomba! 2 columns."""
    f, en, t2 = ev["fate"], ev["en"], ev["t2"]
    m = re.match(r"^retail(?: \((?:same|renamed)\)|, other id): (.*)$", f)
    if m:
        txt = m.group(1)
        if f.startswith("retail, other id"):
            s = "In Tomba! 1 as " + txt
        elif txt.rstrip("?!.").lower() == en.rstrip("?!.").lower():
            s = "In Tomba! 1, same event"
        else:
            s = 'In Tomba! 1 as "%s"' % txt
        if t2.startswith("same name:"):
            s += "; also in Tomba! 2"
        return "same", s
    s = "Cut"
    m = re.match(r"^cut, id reused for (.*?)(?:; Tomba! 2 .*)?$", f)
    if m:
        s += " (its id was reused for %s)" % m.group(1)
    if t2.startswith("same name: "):
        return "t2n", s + ". Tomba! 2, same name: " + t2[len("same name: "):]
    if t2.startswith("theme only: "):
        return "t2t", s + ". Tomba! 2, theme only: " + t2[len("theme only: "):]
    return "cut", s


def desc_entries(ev):
    """[(tag, text, note)] of the Description column, in the doc's order."""
    out = []
    for d in ev["desc"]:
        m = re.match(r"^\*\*(.+?)\*\*: (.*)$", d)
        if not m:
            continue
        text, note = m.group(2), ""
        n = re.search(r"\s*\*\((.*)\)\*\s*$", text)
        if n:
            text, note = text[:n.start()], n.group(1)
        out.append((m.group(1), text.strip(), note))
    return out or [("none", "No description in any build.", "")]


def desc_lines(ev):
    """The first description (the beta's draft English never first) and, unless that is the retail line, the retail
    EN or translated JP hint; [(tag, label, label width, wrapped lines)]."""
    d = [x for x in desc_entries(ev) if x[0] not in ("beta 980203", "retail EN, other event")]
    d = d or [("none", "No description in any build.", "")]
    pick = [d[0]]
    if d[0][0] != "retail EN":
        pick += [x for x in d[1:] if x[0] in ("retail EN", "JP hint (translated)")][:1]
    out = []
    for tag, text, note in pick:
        if "differs" in note:
            text += " (differs from retail)"
        if "later event" in note:
            text += " (written for the later event)"
        lab = SHORT.get(tag, tag.upper()) + "  "
        lw = f_dtag.getlength(lab)
        out.append((tag, lab, lw, wrap(text, f_desc, COLW - INDENT - 10 - lw)))
    return out


def group_sub(g, events):
    ids = g["ids"]
    ret = sum(1 for i in ids if in_retail(events[i]))
    fixed = next((s for pre, s in SUMMARY if g["title"].startswith(pre)), None)
    if len(ids) == 1:
        return "1 title, in Tomba! 1." if ret else "1 title, cut."
    if fixed:
        return "%d titles. %s" % (len(ids), fixed)
    return "%d titles: %d in Tomba! 1, %d cut." % (len(ids), ret, len(ids) - ret)


# --- layout and drawing ------------------------------------------------------------------------------------------
def layout(groups, events):
    blocks = []     # (kind, height, payload)
    for g in groups:
        hl = wrap(g["title"], f_head, COLW - 30)
        sl = wrap(group_sub(g, events), f_hsub, COLW - 30)
        blocks.append(("head", 22 + 38 * len(hl) + 28 * len(sl) + 14, (hl, sl), g))
        for i in sorted(g["ids"]):
            ev = events[i]
            kind, ft = fate_of(ev)
            nl = wrap(ev["en"], f_name, COLW - INDENT - 10)
            fl = wrap(ft, f_fate, COLW - INDENT - 10)
            dl = desc_lines(ev)
            dh = sum(24 * len(x[3]) for x in dl) + 4
            blocks.append(("ev", 31 * len(nl) + 27 * len(fl) + dh + 14, (ev, nl, fl, kind, dl), g))
    target = sum(b[1] for b in blocks) / COLS
    cols, cur, h = [], [], 0
    for k, b in enumerate(blocks):
        nxt_ev = blocks[k + 1][1] if b[0] == "head" and k + 1 < len(blocks) else 0
        small = b[0] == "ev" and b[3]["title"].startswith(SMALL)
        if cur and not small and h + b[1] + nxt_ev > target + 40 and len(cols) < COLS - 1:
            cols.append(cur)
            cur, h = [], 0
        cur.append(b)
        h += b[1]
    cols.append(cur)
    # a group that continues in the next column gets a short "(cont.)" header
    for c in cols:
        if c[0][0] == "ev":
            hl = wrap(c[0][3]["title"] + " (cont.)", f_head, COLW - 30)
            c.insert(0, ("head", 22 + 38 * len(hl) + 14, (hl, []), c[0][3]))
    return cols


def draw(groups, events):
    cols = layout(groups, events)
    LEG_TOP = 60
    title_h = 80 + 40 + 40
    legend_h = 250
    top = LEG_TOP + title_h + legend_h + 30
    foot = wrap(FOOT, f_hsub, W_ - 2 * MARGIN)
    H = top + max(sum(b[1] for b in c) + 60 for c in cols) + MARGIN + 28 * (len(foot) - 1)

    img = Image.new("RGB", (W_, H), BG)
    d = ImageDraw.Draw(img)
    d.text((MARGIN, LEG_TOP), 'PrePre 8 ("Pure 8", June 1997): all %d event titles, with descriptions' % len(events),
           font=f_title, fill=TEXT)
    d.text((MARGIN, LEG_TOP + 88), "English translation of each Japanese title, grouped by the area the data supports. "
           "Number = index in the PrePre 8 list (retail event id = number - 1).", font=f_sub, fill=MUTED)
    d.text((MARGIN, LEG_TOP + 126), "Areas in story order, then the places and title clusters with no retail area, "
           'then titles with data for several areas, then "Area unknown".', font=f_sub, fill=MUTED)

    # legend
    ly = LEG_TOP + title_h + 10
    d.rounded_rectangle((MARGIN, ly, W_ - MARGIN, ly + legend_h - 20), radius=14, fill=PANEL)
    x0 = MARGIN + 30
    d.text((x0, ly + 20), "Fate (colour of the second line)", font=f_legb, fill=TEXT)
    fl = [("same", "In Tomba! 1: the same event (retail English name shown when it differs)"),
          ("cut", "Cut: not in Tomba! 1 (where retail gave the id to another event, it is named)"),
          ("t2n", "Cut, and Tomba! 2 has an event of the same name"),
          ("t2t", "Cut, and Tomba! 2 has a similar idea: theme only, not proof the content moved")]
    for k, (key, txt) in enumerate(fl):
        yy = ly + 62 + k * 40
        d.rounded_rectangle((x0, yy + 4, x0 + 26, yy + 26), radius=5, fill=FATE[key])
        d.text((x0 + 40, yy), txt, font=f_leg, fill=FATE[key])
    x1 = MARGIN + 30 + 2 * (COLW + GAP) - 40
    d.text((x1, ly + 20), "Evidence for the area (tag before each title)", font=f_legb, fill=TEXT)
    ll = [("E", "data places it: PrePre 8 code or text, a demo or JP retail area list, the same retail event"),
          ("S", "same retail id used in that area for an event of the same theme"),
          ("W", "list position only: between two titles of one area (right about 3 times in 4)"),
          ("U", "theme only, or nothing")]
    for k, (key, txt) in enumerate(ll):
        yy = ly + 62 + k * 40
        col, name = LVC[key]
        d.rounded_rectangle((x1, yy + 2, x1 + TAGW, yy + 30), radius=6, fill=col)
        tw = f_tag.getlength(name)
        d.text((x1 + (TAGW - tw) / 2, yy + 7), name, font=f_tag, fill=(255, 255, 255))
        d.text((x1 + TAGW + 16, yy), txt, font=f_leg, fill=TEXT)
    x2 = MARGIN + 30 + 4 * (COLW + GAP) - 300
    d.text((x2, ly + 20), "Description source (third line)", font=f_legb, fill=TEXT)
    sl = [("Pure 8 dialogue", "PrePre 8's own dialogue about it"),
          ("Dengeki D5", "a 1997 demo hint (also JP DEMO), when it differs"),
          ("retail EN", "PAL English hint of the retail id"),
          ("JP hint (translated)", "JP retail hint, when the PAL one is ???"),
          ("none", "no description in any build")]
    for k, (key, txt) in enumerate(sl):
        yy = ly + 56 + k * 33
        d.text((x2, yy + 4), SHORT[key], font=f_dtag, fill=SRC[key])
        d.text((x2 + 200, yy), txt, font=f_desc, fill=TEXT)

    for c, col in enumerate(cols):
        x = MARGIN + c * (COLW + GAP)
        y = top
        for kind, hgt, p, _g in col:
            if kind == "head":
                hl, sl = p
                d.rounded_rectangle((x, y + 8, x + COLW, y + hgt - 4), radius=10, fill=PANEL)
                yy = y + 16
                for line in hl:
                    d.text((x + 16, yy), line, font=f_head, fill=TEXT)
                    yy += 38
                for line in sl:
                    d.text((x + 16, yy), line, font=f_hsub, fill=MUTED)
                    yy += 28
            else:
                ev, nl, fl, fk, dl = p
                colr, name = LVC[LEVEL[ev["level"]]]
                d.rounded_rectangle((x + 4, y + 4, x + 4 + TAGW, y + 30), radius=6, fill=colr)
                tw = f_tag.getlength(name)
                d.text((x + 4 + (TAGW - tw) / 2, y + 9), name, font=f_tag, fill=(255, 255, 255))
                num = str(ev["idx"])
                d.text((x + TAGW + 18 + 44 - f_num.getlength(num), y + 4), num, font=f_num, fill=MUTED)
                yy = y + 2
                for line in nl:
                    d.text((x + INDENT, yy), line, font=f_name, fill=TEXT)
                    yy += 31
                for line in fl:
                    d.text((x + INDENT, yy), line, font=f_fate, fill=FATE[fk])
                    yy += 27
                yy += 3
                for tag, lab, lw, ls in dl:
                    d.text((x + INDENT, yy + 3), lab, font=f_dtag, fill=SRC.get(tag, DTXT))
                    for line in ls:
                        d.text((x + INDENT + lw, yy), line, font=f_desc, fill=DTXT if tag != "none" else SRC["none"])
                        yy += 24
            y += hgt
    for k, line in enumerate(foot):
        d.text((MARGIN, H - MARGIN + 10 - 28 * (len(foot) - 1 - k)), line, font=f_hsub, fill=MUTED)
    return img


FOOT = ("Sources: PrePre 8 TOMBA.EXE; Hyper 12, PrePre 9, Dengeki D5 and JP demo title blocks; JP retail SLPS-01144 "
        "per-area title tables; PAL retail event code; Tomba! 2 USA MAIN.EXE. Descriptions: PrePre 8 dialogue; JP "
        "demo, Yoiko 2 and Dengeki D5 hint banks; PAL English and JP retail event hints. Details: "
        "docs/PREPRE8_EVENTS.md and docs/wiki/PrePre8-Events.md. Drawn by tools/prepre8_poster.py.")


def source_hash():
    """sha256 of the inputs: the audited doc and this script."""
    h = hashlib.sha256()
    for p in (W.DOC, os.path.abspath(__file__)):
        h.update(open(p, "rb").read())
    return h.hexdigest()


def stored_hash(path):
    try:
        return Image.open(path).text.get(HASH_KEY)
    except (OSError, AttributeError):
        return None


def ramp_palette(steps=6):
    """Every pixel is an ink blended over a fill (anti-aliased text, rounded corners): a palette of short ramps
    from each fill to each ink keeps every hue exact and the PNG small."""
    tags = [c for c, _n in LVC.values()]
    inks = [TEXT, MUTED, DTXT] + list(FATE.values()) + list(SRC.values())
    pairs = [(bg, ink) for bg in (BG, PANEL) for ink in inks]
    pairs += [(t, (255, 255, 255)) for t in tags] + [(BG, t) for t in tags + list(FATE.values()) + [PANEL]]
    pal = []
    for a, b in pairs:
        pal += [tuple(round(a[j] + (b[j] - a[j]) * k / (steps - 1)) for j in range(3)) for k in range(steps)]
    pal = list(dict.fromkeys(pal))[:256]
    p = Image.new("P", (1, 1))
    p.putpalette([v for c in pal for v in c] + [0] * (768 - 3 * len(pal)))
    return p


def build(out=OUT):
    groups, events, _top = W.parse(open(W.DOC, encoding="utf-8").read())
    img = draw(groups, events)
    q = img.quantize(palette=ramp_palette(), dither=Image.Dither.NONE)
    info = PngImagePlugin.PngInfo()
    info.add_text(HASH_KEY, source_hash())
    os.makedirs(os.path.dirname(out), exist_ok=True)
    q.save(out, optimize=True, pnginfo=info)
    return out, img.size


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("-o", "--out", default=OUT)
    a = ap.parse_args()
    out, size = build(a.out)
    print("wrote %s (%dx%d, %d bytes)" % (os.path.relpath(out, W.ROOT), size[0], size[1], os.path.getsize(out)))


if __name__ == "__main__":
    main()
