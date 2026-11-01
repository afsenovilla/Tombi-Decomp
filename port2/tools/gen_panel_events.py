#!/usr/bin/env python3
"""Writes port2/runtime/panel_eventmap.h and docs/EVENTS_MAP.md: where each event (SCRIPT_FLAGS id 0..0xBF) happens, for the
second-screen panel's event browser.  Ids and coordinates only (area, section, x, y of the object, a kind), no game data.

Sources, in order of preference:
  1. the room object lists (tools/objects.py list --format csv, run on the user's disc): an object placed in a room whose
     handler / sequence starts or completes the event -> area, section, x, y, kind from the handler name;
  2. the decomp: setEventStarted / setEventComplete call sites in src/ (src/xNNN/ = the overlay of AREA NNN, src/aNN* = AREA NN)
     -> area only (section unknown).
The header is committed (CI has no disc); re-run after the decomp or objects.py learn more:

  python3 tools/objects.py list --format csv -o /tmp/objs.csv
  python3 port2/tools/gen_panel_events.py /tmp/objs.csv
"""
import csv, os, re, sys, collections

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
EVENTS = 0xC0
KINDS = ["EK_UNKNOWN", "EK_NPC", "EK_CHEST", "EK_ITEM", "EK_SCENE", "EK_OBJECT"]


# updateL4Type1AObject (the signs and boards) starts one event per subtype (src/updateL4Type1AObject.c), not the three
# objects.py lists for the handler: 0x11 the NO TRESPASSING sign (AREA09), 0x13 / 0x14 the Baccus Village board.
SIGN_EVENTS = {0x11: 37, 0x13: 46, 0x14: 49}
# updateItemObject: subtype = item id; picking it up runs its reward (src/updateL5Type09Item.c table), which starts the event:
# rewardCrystalBalls (0x8E, the first ball starts 19 Lost and Found), rewardJewel (0x63 / 0x64 below blue / green level 9
# start 90 / 129; 0x62 completes 58, started by the 1,000 Year Old Man's scene).
ITEM_EVENTS = {0x8E: 19, 0x63: 90, 0x64: 129}


def kind_of(handler, note):
    h = handler.lower()
    if "l4type1a" in h:
        return 5
    if "chest" in h:
        return 2
    if "item" in h:
        return 3
    if note or "scene" in h or "seq" in h or "cutscene" in h:
        return 4
    if any(k in h for k in ("npc", "villager", "man", "woman", "elder", "dwarf", "girl", "boy", "kid", "baby", "pig", "chief", "granny", "grandpa", "talk")):
        return 1
    return 5


def from_objects(path):
    out = {}
    rd = csv.reader(open(path, encoding="utf-8"))
    hdr = next(rd)
    for row in rd:
        if len(row) == len(hdr) - 1:   # objects.py writes no field for an empty handler name
            row.insert(hdr.index("handler_name"), "")
        r = dict(zip(hdr, row))
        if r.get("own", "True") not in ("True", "1", "true"):
            continue
        evs = [int(e) for e in (r.get("events") or "").split() if e.isdigit()]
        try:
            sub = int(r.get("subtype", "-1"), 0)
        except ValueError:
            sub = -1
        if r.get("handler_name") == "updateL4Type1AObject":
            evs = [SIGN_EVENTS[sub]] if sub in SIGN_EVENTS else []
        elif r.get("handler_name") == "updateItemObject" and sub in ITEM_EVENTS:
            evs = [ITEM_EVENTS[sub]]
        for e in evs:
            if e >= EVENTS or e in out:
                continue
            try:
                k = 3 if r.get("handler_name") == "updateItemObject" else kind_of(r.get("handler_name", ""), r.get("event_note", ""))
                out[e] = (int(r["area"]), int(r["section"]), int(r["x"]), int(r["y"]), k,
                          "objects: AREA%02d:%s %s" % (int(r["area"]), r["section"], r.get("handler_name", ""))
                          + (" subtype 0x%02X" % sub if r.get("handler_name") in ("updateL4Type1AObject", "updateItemObject") else ""))
            except (KeyError, ValueError):
                pass
    return out


def from_npcset09():
    """The Baccus Village board: spawnNpcSet09 (src/x005/spawnNpcSet09.c) places a layer-4 sign (type 0x1A) from the table of
    D_80117AC8[GCD.bceed]; the sign subtype picks the event (SIGN_EVENTS).  It runs in AREA05 section 1 (--log-events:
    'start 46 ... at area 5 section 1').  event -> (area, section, x, y, kind, why)."""
    import struct
    out = {}
    try:
        d = open(os.path.join(ROOT, "game", "disc", "fs", "AREA05", "X005.BIN"), "rb").read()
    except OSError:
        return out
    base = 0x800E8028
    for k in range(4):
        p = struct.unpack_from("<I", d, 0x80117AC8 - base + 4 * k)[0]
        for i in range(64):
            e = struct.unpack_from("<14h", d, p - base + 28 * i)
            if e[0] & 0xFF == 0xFF:
                break
            if e[0] & 0x80 and e[12] in SIGN_EVENTS and SIGN_EVENTS[e[12]] not in out:
                out[SIGN_EVENTS[e[12]]] = (5, 1, e[9], e[10], 5, "objects: X005 spawnNpcSet09 board, sign subtype 0x%02X" % e[12])
    return out


def from_src():
    out = collections.defaultdict(set)
    pat = re.compile(r"setEvent(?:Started|Complete)\(\s*(0x[0-9A-Fa-f]+|\d+|EVENT_\w+)\b")
    names = {m.group(2): int(m.group(1), 16) for m in re.finditer(r"/\* 0x([0-9A-F]+) \*/ (EVENT_\w+)", open(os.path.join(ROOT, "include/tomba/game.h")).read())}
    for dp, _, fs in os.walk(os.path.join(ROOT, "src")):
        rel = os.path.relpath(dp, os.path.join(ROOT, "src"))
        m = re.match(r"x(\d{3})", rel)
        for f in fs:
            if not f.endswith(".c"):
                continue
            area = int(m.group(1)) if m else None
            if area is None:
                mm = re.match(r"a(\d\d)[A-Z]", f) or re.match(r"update[A]?(\d\d)", f.replace("updateA", "updateA"))
                mm = re.match(r"a(\d\d)[A-Z]", f) or re.match(r"updateA(\d\d)", f)
                area = int(mm.group(1)) if mm else None
            if area is None or area >= 20:
                continue
            for v in pat.findall(open(os.path.join(dp, f), encoding="utf-8", errors="replace").read()):
                e = names.get(v, -1) if v.startswith("EVENT_") else int(v, 0)
                if 0 <= e < EVENTS:
                    out[e].add((area, "src/%s/%s" % (rel, f) if rel != "." else "src/" + f))
    return out


def from_bin():
    """setEventStarted / setEventComplete calls with an immediate event id in the area overlays on the disc (also the
    functions the decomp has not reached yet): event -> {area}."""
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    import areas
    calls = {0x0C000000 | (0x8005A8A8 >> 2 & 0x3FFFFFF), 0x0C000000 | (0x8005A9A4 >> 2 & 0x3FFFFFF)}
    out = collections.defaultdict(set)
    for prog in areas.OVERLAYS:
        if not re.match(r"X\d{3}$", prog):
            continue
        try:
            W = areas.words(areas.load(prog))
        except OSError:
            continue
        for i, w in enumerate(W):
            if w not in calls:
                continue
            for j in [i + 1] + list(range(i - 1, max(0, i - 14), -1)):   # the delay slot, then back to the last write of a0
                x = W[j]; op = x >> 26
                if (x >> 16) in (0x2404, 0x3404):
                    if (x & 0xFFFF) < EVENTS:
                        out[x & 0xFFFF].add(int(prog[1:]))
                    break
                if (op == 0 and ((x >> 11) & 0x1F) == 4) or (op in (8, 9, 0xD, 0xF, 0x20, 0x21, 0x23, 0x24, 0x25) and ((x >> 16) & 0x1F) == 4):
                    break
    return out


# 3. code outside the area overlays, attributed by who loads it (the event ids are the immediates of the setEventStarted /
#    setEventComplete calls, or of game opcode 0x9a in the WSS scripts): area, section (255 unknown), evidence
OWNER = {}
for _e in (125, 145, 146, 161):
    OWNER[_e] = (2, 3, "I0NFO3 (the overlay of AREA02 section 3, the tunnel: notes/names.md tunnel*)")
for _e in (38, 43, 47, 53):
    OWNER[_e] = (5, 0, "I0NFO2 stepNpcSet07 / npcSet07Scene* (the AREA05 actors; I0NFO2 runs in AREA05 / AREA08 section 0)")
OWNER[82] = (19, 255, "I0NFO2 stepNpcSet11 (the AREA19 actor: notes/names.md)")
OWNER[50] = (8, 0, "I0NFO2 npcSet0CMessageScene (NPC set 0C = AREA08: tools/dialogue_refs.py NPCSET_AREA)")
OWNER[44] = (5, 0, "I0NFO2 topdownActorTalkStep: its dialogues (2, 0x22 / 0x23) exist only in AREA05's bank (the fortune teller)")
# area 254: no room; x = the item whose use from the pause menu completes it (D0SPSUB func_800E89F8 case 0x3F)
OWNER[15] = (254, 255, "D0SPSUB func_800E89F8: using item 0x3F from the pause menu completes it, anywhere")
USE_ITEM = {15: 0x3F}
# titled events nothing in PAL starts or completes: no setEventStarted / setEventComplete with the id in any overlay or EXE,
# no op 0x9a in the AREA15 scripts, no table entry of the data-driven setters (topdown actors +0xAC, hooded old man and
# scene sequence scripts, signs, item rewards)
NEVER_SET = {77: "never set: only read, by the cut Civilization towns' spawners (I0NFO2 spawnNpcSet0D / 0E / 0F, D_8009CDF1)"}
# 4. the area scripts: loadAreaSoundBank (src/loadAreaSoundBank.c, the PAL branch) loads one AREA15 script per room and
#    initAreaScripts runs it there; game opcode 0x9a (awardEventProgress) inside it starts / completes the event in that room.
#    (area, section) per script; section 255 when the script runs in several sections of the area.
WSS_ROOMS = {"0000": (0, 255), "SCR0100": (1, 255), "0120": (1, 2), "0130": (1, 3), "0140": (1, 4),
             "0200": (2, 0), "0201": (2, 0), "0210": (2, 1), "0220": (2, 2)}


def from_wss():
    """event -> [(area, section, 'start' / 'complete', script)] from the op 0x9a awards (05 02 01 id 01 flag 9a)."""
    out = collections.defaultdict(list)
    d15 = os.path.join(ROOT, "game", "disc", "fs", "AREA15")
    for name, (a, sec) in WSS_ROOMS.items():
        try:
            d = open(os.path.join(d15, name + ".WSS"), "rb").read()
        except OSError:
            continue
        for m in re.finditer(rb"\x05\x02\x01(....)\x01(....)\x9a", d, re.S):
            e, fl = int.from_bytes(m.group(1), "little"), int.from_bytes(m.group(2), "little")
            if e < EVENTS:
                out[e].append((a, sec, "complete" if fl else "start", name))
    return out


def main():
    objs = from_objects(sys.argv[1]) if len(sys.argv) > 1 else {}
    if objs:
        for e, v in from_npcset09().items():
            objs.setdefault(e, v)
    src = from_src()
    wss = from_wss() if objs else {}
    for e, ars in (from_bin().items() if objs else ()):   # the disc is there: add the overlay call sites
        for a in ars:
            if not any(x == a for x, _ in src[e]):
                src[e].add((a, "X%03d.BIN (call site)" % a))
    rows, doc = [], []
    for e in range(EVENTS):
        if e in objs:
            a, s, x, y, k, why = objs[e]
            rows.append((e, a, s, x, y, k)); doc.append((e, "AREA%02d section %d (x %d, y %d)" % (a, s, x, y), KINDS[k][3:].lower(), why))
        elif e in src:
            areas = sorted({a for a, _ in src[e]})
            a = areas[0]
            rows.append((e, a, 255, -32768, -32768, 4))
            doc.append((e, "AREA%02d (section unknown)" % a + (" / also " + ", ".join("AREA%02d" % b for b in areas[1:]) if len(areas) > 1 else ""),
                        "scene", "decomp: " + ", ".join(sorted({f for _, f in src[e]}))[:120]))
        elif e in wss:   # where the script that starts it (else completes it) runs
            a, sec, k, name = sorted(wss[e], key=lambda t: t[2] != "start")[0]
            rows.append((e, a, sec, -32768, -32768, 4))
            doc.append((e, "AREA%02d" % a + (" section %d" % sec if sec != 255 else " (section unknown)"), "scene",
                        "script: AREA15/%s.WSS (%s), run in that room by initAreaScripts" % (name, k)))
        elif e in OWNER:
            a, sec, why = OWNER[e]
            rows.append((e, a, sec, USE_ITEM.get(e, -32768), -32768, 3 if e in USE_ITEM else 4))
            doc.append((e, "AREA%02d" % a + (" section %d" % sec if sec != 255 else " (section unknown)"), "scene", "owner: " + why))
        elif e in NEVER_SET:
            doc.append((e, "none", "-", NEVER_SET[e]))
        else:
            doc.append((e, "unknown", "-", "-"))
    h = ["/* generated by port2/tools/gen_panel_events.py: event -> area, section (255 unknown), object x, y (-32768 unknown), kind.",
         "   Ids and coordinates only; coverage in docs/EVENTS_MAP.md. */",
         "enum { " + ", ".join(KINDS) + " };",
         "typedef struct { unsigned char ev, area, sec, kind; short x, y; } PanelEventLoc;",
         "static const PanelEventLoc panel_event_locs[] = {"]
    for e, a, s, x, y, k in rows:
        h.append("    {%d, %d, %d, %s, %d, %d}," % (e, a, s, KINDS[k], x, y))
    h.append("};")
    open(os.path.join(ROOT, "port2/runtime/panel_eventmap.h"), "w").write("\n".join(h) + "\n")
    n_obj, n_src = sum(1 for d in doc if d[3].startswith("objects")), sum(1 for d in doc if d[3].startswith(("decomp", "owner", "script")))
    md = ["# Event locations (second-screen panel)", "",
          "Where each event (`SCRIPT_FLAGS` id 0..0xBF) happens, as the second-screen event browser shows it "
          "(`port2/runtime/panel_eventmap.h`, generated by `port2/tools/gen_panel_events.py`).", "",
          "- **room + position**: an object placed in that room starts / completes it (`tools/objects.py`); the panel marks the room on the map.",
          "- **area only**: a `setEventStarted` / `setEventComplete` call site in that area's code (`src/xNNN/`, `src/aNN*`); the room is not known.",
          "- **script**: started / completed by game opcode 0x9a in the AREA15 script that `loadAreaSoundBank` loads for that room.",
          "- **owner**: a call site in a shared overlay (I0NFO2 / I0NFO3), attributed to the area that loads it.",
          "- **none**: nothing in PAL starts or completes it (only read by cut content).",
          "- **unknown**: none of these yet (data-driven ids, the other WSS scripts); the panel says the zone is not extracted yet.", "",
          "Coverage: %d with room and position, %d with the area only, %d never set, %d unknown, of %d." % (n_obj, n_src, len(NEVER_SET), EVENTS - n_obj - n_src - len(NEVER_SET), EVENTS), "",
          "| event | where | kind | source |", "|---|---|---|---|"]
    for e, w, k, why in doc:
        md.append("| %d (0x%02X) | %s | %s | %s |" % (e, e, w, k, why.replace("|", "/")))
    open(os.path.join(ROOT, "docs/EVENTS_MAP.md"), "w").write("\n".join(md) + "\n")
    print("objects %d, decomp %d, unknown %d" % (n_obj, n_src, EVENTS - n_obj - n_src))


if __name__ == "__main__":
    main()
