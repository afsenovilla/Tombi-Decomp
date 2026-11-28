#!/usr/bin/env python3
"""Build the PrePre 8 ("Pure 8") event wiki pages (docs/wiki/PrePre8-*.md) from docs/PREPRE8_EVENTS.md.

The audited doc is the only source: its per-area tables (one row per event, with the "Description (source)" column)
and its Overview, Evidence levels, Tomba! 2, Candies and Village of Civilization sections. Nothing is read from a
disc, so the pages can be rebuilt anywhere. The events poster (docs/wiki/img/prepre8_events_poster.png, drawn by
tools/prepre8_poster.py from the same doc, needs Pillow) is redrawn with the pages:

  python3 tools/prepre8_wiki.py               write the pages and the poster
  python3 tools/prepre8_wiki.py --no-poster   write the pages only
  python3 tools/prepre8_wiki.py --check       write both, then check every relative link and #anchor of docs/wiki
                                              (Markdown and HTML src/href) and that the poster is linked and current
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOC = os.path.join(ROOT, "docs", "PREPRE8_EVENTS.md")
WIKI = os.path.join(ROOT, "docs", "wiki")
INDEX = "PrePre8-Events.md"
POSTER = "img/prepre8_events_poster.png"     # relative to docs/wiki

# page names of the groups of "## By area" (heading prefix -> page suffix)
PAGES = [
    ("Village of All Beginnings", "Village-of-All-Beginnings"),
    ("Forest of 100 Flowers", "Forest-and-Dwarf-Village"),
    ("Phoenix Mountain", "Phoenix-Mountain"),
    ("Haunted Mansion", "Haunted-Mansion"),
    ("Baccus Village", "Baccus-Village-and-Lake"),
    ("Mushroom Forest", "Mushroom-Forest"),
    ("Mermaid's Singing Rock", "Mermaids-Singing-Rock"),
    ("Masakari Jungle", "Masakari-Jungle"),
    ("Village of Civilization", "Civilization-Towns"),
    ("Evil Pigs", "Evil-Pigs"),
    ("Pig Town", "Pig-Town"),
    ("Title cluster: the island", "Cluster-Island"),
    ("Title cluster: light, sun", "Cluster-Light-Sun-and-Cold"),
    ("Title cluster: sky", "Cluster-Sky"),
    ("Title cluster: caves", "Cluster-Caves"),
    ("Title cluster: light (", "Cluster-Light"),
    ("Title cluster: grand finale", "Cluster-Grand-Finale"),
    ("Data for more than one area", "Several-Areas"),
    ("Area unknown", "Area-Unknown"),
]
EXTRA = [("Tomba! 2", "PrePre8-Tomba2-Links.md", "PrePre 8 titles and Tomba! 2"),
         ("Candies", "PrePre8-Candies.md", "Candies in the Tomba! 1 builds"),
         ("Village of Civilization: proto material in the list", "PrePre8-Civilization-Candidates.md",
          "Proto Village of Civilization candidates in the PrePre 8 list")]
TAGS = [
    ("Pure 8 dialogue", "PrePre 8's own dialogue (TOMBA.EXE) talks about the event; translated. The forest bank's "
     "unlabelled kanji are read from the same lines in the JP demo's early forest bank."),
    ("Pure 9", "PrePre 9 (PCPX-96093, 1997-10-20). It has no event hint bank (its DSPSUB.BIN holds no WFM text "
     "block), so no description carries this tag."),
    ("JP demo / Yoiko 2 / Dengeki D5", "The hint bank of a 1997 JP demo (SLPM-80165 and Yoi Ko 2 `SYSTEM/A00014` / "
     "`A00013`, 1997-10-10; Dengeki D5 `SYSTEM/A00014`, 1997-11-06), translated. Shown only where it differs from the "
     "JP retail hint of the same id. Hyper 12 (1997-09-22) has no hint bank."),
    ("beta 980203", "The English hint of the 1998-02-03 prototype, quoted where its wording differs from retail."),
    ("retail EN", "The PAL English hint of the retail id (the balloon of the event list, `D0SPSUB.BIN`), quoted."),
    ("JP hint (translated)", "The JP retail hint (`DSPSUB.BIN` block 1) where the PAL English is a placeholder."),
    ("none", "No description in any build; one factual line says what is known (a placeholder, or what the id "
     "became in retail)."),
]


def slug(text):
    """GitHub's heading anchor: lower case, punctuation dropped (letters of any script kept), spaces to hyphens."""
    s = text.strip().lower()
    s = re.sub(r"[^\w\- ]", "", s)
    return s.replace(" ", "-")


def sections(md):
    """[(level, title, body_lines)] split at ## and ### headings."""
    out, cur = [], None
    for ln in md.split("\n"):
        m = re.match(r"^(#{2,3}) (.*)$", ln)
        if m:
            cur = [len(m.group(1)), m.group(2).strip(), []]
            out.append(cur)
        elif cur:
            cur[2].append(ln)
    return out


def cells(line):
    return [c.strip() for c in line.strip().strip("|").split("|")]


def parse(md):
    secs = sections(md)
    groups, events, top = [], {}, {}
    in_area = False
    for lvl, title, body in secs:
        if lvl == 2:
            in_area = title == "By area"
            top[title] = body
            continue
        if not in_area:
            continue
        page = next((p for pre, p in PAGES if title.startswith(pre)), slug(title).title())
        summary = next((ln for ln in body if ln.strip() and not ln.startswith("|")), "")
        g = {"title": title, "page": "PrePre8-Events-%s.md" % page, "summary": summary.strip(), "ids": []}
        groups.append(g)
        for ln in body:
            if not re.match(r"^\| \d+ \|", ln):
                continue
            c = cells(ln)
            i = int(c[0])
            ev = dict(idx=i, jp=c[1], en=c[2], fate=c[3], retail=c[4], level=c[5], arg=c[6], t2=c[7],
                      desc=[d for d in c[8].split("<br>") if d] if len(c) > 8 else [], group=g)
            ev["head"] = "%d. %s" % (i, ev["en"])
            ev["anchor"] = slug(ev["head"])
            events[i] = ev
            g["ids"].append(i)
    return groups, events, top


def fix_links(text):
    """Links of the doc, rewritten for a page in docs/wiki."""
    def f(m):
        label, target = m.group(1), m.group(2)
        if re.match(r"^[a-z]+:", target):
            return m.group(0)
        if target.startswith("#"):
            return "[%s](../PREPRE8_EVENTS.md%s)" % (label, target)
        return "[%s](../%s)" % (label, target)
    return re.sub(r"\[([^\]]+)\]\(([^)\s]+)\)", f, text)


def link(ev):
    return "[%s](%s#%s)" % (ev["head"], ev["group"]["page"], ev["anchor"])


def short_fate(f):
    if f.startswith("retail (same)"):
        return "same event"
    if f.startswith("retail (renamed)"):
        return "renamed"
    if f.startswith("retail, other id"):
        return "same title, other id"
    return "cut, id reused" if re.match(r"^cut, (id reused|its id went)", f) else "cut"


def fmt_desc(d):
    m = re.match(r"^\*\*(.+?)\*\*: (.*)$", d)
    if not m:
        return "- " + d
    return "- **%s**: %s" % (m.group(1), m.group(2))


def event_section(ev, prev, nxt):
    g = ev["group"]
    r, f = ev["retail"], ev["fate"]
    m = re.match(r"^cut, (?:id reused for|its id went to) (.*?)(?:; Tomba! 2 .*)?$", f)
    reuse = m.group(1) if m else ""
    rows = [("Japanese title", ev["jp"]), ("English title", ev["en"]),
            ("Retail id: PAL title (JP retail title)", r),
            ("Fate", ev["fate"]),
            ("Id reused for", reuse or "-"),
            ("Area", "[%s](%s)" % (g["title"], g["page"])),
            ("Evidence level", "**%s**" % ev["level"]),
            ("Argument", ev["arg"]),
            ("Tomba! 2", ev["t2"] or "-")]
    out = ["### " + ev["head"], "", "| | |", "|---|---|"]
    out += ["| %s | %s |" % (k, v) for k, v in rows]
    out += ["", "Descriptions (source):", ""]
    out += [fmt_desc(d) for d in ev["desc"]] or ["- **none**: No description in any build."]
    nav = []
    nav.append("previous: " + link(prev) if prev else "previous: none (start of the list)")
    nav.append("next: " + link(nxt) if nxt else "next: none (end of the list)")
    out += ["", "In list order: " + " · ".join(nav) + " · [all events](%s#all-169-events)" % INDEX, ""]
    return out


def area_page(g, events, order):
    out = ["# PrePre 8 events: %s" % g["title"], "",
           "[All PrePre 8 events](%s) · %s" % (INDEX, g["summary"]), "",
           "Source tags, evidence levels and the id rule are explained on the [index](%s#source-tags)." % INDEX, "",
           "| # | English | Fate | Level |", "|---|---|---|---|"]
    for i in g["ids"]:
        ev = events[i]
        out.append("| %d | [%s](#%s) | %s | %s |" % (i, ev["en"], ev["anchor"], short_fate(ev["fate"]), ev["level"]))
    out.append("")
    for i in g["ids"]:
        k = order.index(i)
        prev = events[order[k - 1]] if k > 0 else None
        nxt = events[order[k + 1]] if k + 1 < len(order) else None
        out += event_section(events[i], prev, nxt)
    return "\n".join(out).rstrip("\n") + "\n"


def first_table(lines):
    t, started = [], False
    for ln in lines:
        if ln.startswith("|"):
            t.append(ln)
            started = True
        elif started:
            break
    return t


def bullets(lines):
    """The "- " items of a section, each joined with its continuation lines."""
    out = []
    for ln in lines:
        if ln.startswith("- "):
            out.append(ln)
        elif out and ln.startswith("  ") and ln.strip():
            out[-1] += " " + ln.strip()
        elif out and not ln.strip():
            break
    return out


def tags(ev):
    return [re.match(r"^\*\*(.+?)\*\*", d).group(1) for d in ev["desc"] if re.match(r"^\*\*(.+?)\*\*", d)]


def primary_tag(ev):
    """The source of an event's best description; the beta's English is a draft of the retail line, never first."""
    t = [x for x in tags(ev) if x != "beta 980203"] or tags(ev) or ["none"]
    return t[0]


def paragraph_after(lines, start):
    """The paragraph that begins with `start`."""
    buf, on = [], False
    for ln in lines:
        if ln.startswith(start):
            on = True
        if on:
            if not ln.strip():
                break
            buf.append(ln)
    return " ".join(x.strip() for x in buf)


def index_page(groups, events, top):
    order = sorted(events)
    src_counts = {}
    for ev in events.values():
        tag = primary_tag(ev)
        src_counts[tag] = src_counts.get(tag, 0) + 1
    out = ["# PrePre 8 (\"Pure 8\") events", "",
           "PrePre Vol. 8 (PCPX-96084, built 1997-06-24; the community calls it \"Pure 8\") is the oldest known Tomba! "
           "build. Its executable holds a list of 169 event titles, the earliest state of the game's event table. "
           "These pages place every title in an area where data supports it, rate that evidence, say what became of "
           "it in retail, and give every description of it found in any build.", "",
           "Generated by `tools/prepre8_wiki.py` from the audited doc [PREPRE8_EVENTS.md](../PREPRE8_EVENTS.md) "
           "(the arguments and the method in full). Build background: [PREPRE8.md](../PREPRE8.md). "
           "Wiki home: [Home](Home.md).", "",
           '<a href="%s"><img src="%s" width="480" alt="Poster: all 169 PrePre 8 event titles by area, with fate, '
           'evidence level and description"></a>' % (POSTER, POSTER), "",
           "The poster above shows all 169 titles on one sheet; click it to open it at full size. It is drawn by "
           "`tools/prepre8_poster.py` from the same doc.", "",
           "## The id rule", "",
           "Retail event id = PrePre 8 index - 1, for the whole list: 69 titles are the JP retail title of that id, "
           "only two match a retail title at another id (43 and 129). A cut title is an id that retail left untitled, "
           "or reused for a different event; retail ids 166 and up were added after the old list.", "",
           "## Evidence levels", ""]
    out += [b.replace("(see below)", "(see [the list order](../PREPRE8_EVENTS.md#is-the-list-ordered-by-area))")
            for b in bullets(top.get("Evidence levels", []))]
    out += ["", "## Source tags", "",
            "PrePre 8 has no event descriptions or hint bank: a scan of TOMBA.EXE and of every file of the disc (GAMs "
            "unpacked) for glyph tables of 5-byte records followed by u16 strings ending in 0xFFFE finds only the six "
            "known blocks (the title table and the five dialogue tables). So each description comes from the best "
            "source available, tagged. Priority: Pure 8 dialogue, Pure 9, the other demos, retail EN, JP hint "
            "(translated), none. Where an earlier build differs from retail, the earlier line comes first and says "
            "so.", "",
            "| Tag | Source |", "|---|---|"]
    out += ["| %s | %s |" % t for t in TAGS]
    out += ["", "First (best) description per event: " +
            ", ".join("%s %d" % (k, v) for k, v in sorted(src_counts.items(), key=lambda kv: -kv[1])) + ".", "",
            "## Overview", ""]
    ov = first_table(top.get("Overview", []))
    for ln in ov:
        c = cells(ln)
        g = next((g for g in groups if g["title"] == c[0]), None)
        if g:
            c[0] = "[%s](%s)" % (c[0], g["page"])
        out.append("| " + " | ".join(c) + " |")
    out += ["", paragraph_after(top.get("Overview", []), "By fate:"), "",
            "## Area pages", ""]
    out += ["- [%s](%s): %s" % (g["title"], g["page"], g["summary"]) for g in groups]
    out += ["", "Also: [Tomba! 2 links](PrePre8-Tomba2-Links.md) · [Candies](PrePre8-Candies.md) · "
            "[Proto Village of Civilization candidates](PrePre8-Civilization-Candidates.md)", "",
            "## All 169 events", "",
            "| # | English | Fate | Area | Level |", "|---|---|---|---|---|"]
    for i in order:
        ev = events[i]
        out.append("| %d | %s | %s | [%s](%s) | %s |" % (i, link(ev).replace("[%s]" % ev["head"], "[%s]" % ev["en"]),
                                                     short_fate(ev["fate"]), ev["group"]["title"], ev["group"]["page"],
                                                     ev["level"]))
    out += ["", "## Title clusters and places that are not in retail", ""]
    out += [fix_links(ln) for ln in top.get("Title clusters and places that are not in retail", [])]
    out += ["## Uncertain", ""] + [fix_links(ln) for ln in top.get("Uncertain", [])]
    return "\n".join(out).rstrip("\n") + "\n"


def extra_page(name, title, top):
    body = [fix_links(ln) for ln in top.get(name, [])]
    out = ["# " + title, "", "[All PrePre 8 events](%s) · from [PREPRE8_EVENTS.md](../PREPRE8_EVENTS.md)" % INDEX, ""]
    return "\n".join(out + body).rstrip("\n") + "\n"


def build():
    md = open(DOC, encoding="utf-8").read()
    groups, events, top = parse(md)
    if len(events) != 169:
        sys.exit("expected 169 events in %s, found %d" % (DOC, len(events)))
    order = sorted(events)
    files = {INDEX: index_page(groups, events, top)}
    for g in groups:
        files[g["page"]] = area_page(g, events, order)
    for name, fn, title in EXTRA:
        files[fn] = extra_page(name, title, top)
    for fn, text in files.items():
        open(os.path.join(WIKI, fn), "w", encoding="utf-8").write(text)
    return files


def check():
    """Every relative link of docs/wiki/*.md resolves, and every #anchor names a heading of its target page."""
    bad = 0
    anchors = {}

    def heads(path):
        if path not in anchors:
            seen, out = {}, set()
            for ln in open(path, encoding="utf-8"):
                m = re.match(r"^#{1,6} (.*)$", ln.rstrip("\n"))
                if m:
                    s = slug(m.group(1))
                    n = seen.get(s, 0)
                    seen[s] = n + 1
                    out.add(s if n == 0 else "%s-%d" % (s, n))
            anchors[path] = out
        return anchors[path]

    for fn in sorted(os.listdir(WIKI)):
        if not fn.endswith(".md"):
            continue
        src = os.path.join(WIKI, fn)
        text = open(src, encoding="utf-8").read()
        text = re.sub(r"`[^`\n]*`", "", text)
        targets = re.findall(r"\]\(([^)\s]+)\)", text) + re.findall(r'\b(?:src|href)="([^"]+)"', text)
        for target in targets:
            if re.match(r"^[a-z]+:", target):
                continue
            path, _, anchor = target.partition("#")
            dest = os.path.normpath(os.path.join(WIKI, path)) if path else src
            if not os.path.exists(dest):
                print("%s: missing file %s" % (fn, target))
                bad += 1
            elif anchor and dest.endswith(".md") and anchor not in heads(dest):
                print("%s: missing anchor %s" % (fn, target))
                bad += 1
    return bad


def check_poster():
    """The index links the poster, and the PNG was drawn from the current doc and poster script."""
    bad = 0
    idx = open(os.path.join(WIKI, INDEX), encoding="utf-8").read()
    if 'href="%s"' % POSTER not in idx or 'src="%s"' % POSTER not in idx:
        print("%s: no thumbnail link to %s" % (INDEX, POSTER))
        bad += 1
    path = os.path.join(WIKI, POSTER)
    try:
        import prepre8_poster
    except ImportError as e:
        print("poster: cannot check (%s)" % e)
        return bad + 1
    if not os.path.exists(path):
        print("poster: missing %s" % POSTER)
        bad += 1
    elif prepre8_poster.stored_hash(path) != prepre8_poster.source_hash():
        print("poster: %s is stale; rebuild it with python3 tools/prepre8_poster.py" % POSTER)
        bad += 1
    return bad


def poster():
    try:
        import prepre8_poster
    except ImportError as e:
        print("poster: not redrawn (%s)" % e)
        return
    out, size = prepre8_poster.build()
    print("wrote %s (%dx%d, %d bytes)" % (os.path.relpath(out, ROOT), size[0], size[1], os.path.getsize(out)))


def main():
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    files = build()
    print("wrote %d pages to %s" % (len(files), os.path.relpath(WIKI, ROOT)))
    if "--no-poster" not in sys.argv:
        poster()
    if "--check" in sys.argv:
        bad = check() + check_poster()
        print("links: %s" % ("all resolve" if not bad else "%d broken" % bad))
        sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
