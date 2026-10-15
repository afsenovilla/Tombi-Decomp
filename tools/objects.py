#!/usr/bin/env python3
"""Room object spawn/placement records of every area/room (APD records, load kind 0x80..0x8F).  Notes: notes/formats.md ("Object placement records").

Spawner = MAIN0 `FUN_8004c570` (section load, calls `FUN_8004cb00` per record; `func_8004C1EC` is the unlayered "section manager" object that
calls it).  A record is 16 bytes, the list ends at the first record with `type >= 0xFF`:

  +0  b0   section filter (area-wide lists: spawn only when b0 == section or b0 == ~section) -> obj+0x1D
  +1  b1   layer/kind (&0x7F: 2,3,4,5 = main pool, 7, 8 = pool of layer 1; &0x80 -> obj category bit) -> picks the allocator AND the handler table
  +2  b2   type = index into the handler table of that layer (obj+0x02)
  +3  b3   -> obj+0x0C
  +4  b4   subtype -> obj+0x03 (selects the variant inside one handler: door/switch 0/4/8, NPC kind ...)
  +5  b5   -> obj+0x2E (animFrame)
  +6  s16  x   (obj+0x10 whole part)
  +8  s16  y   (obj+0x14)
  +10 s16  z   (obj+0x18)
  +12 bc   -> obj+0x0A  (bit 0x10: be indexes a sprite/model table)
  +13 bd   -> obj+0x0F
  +14 be   index into a pointer table (WPP/SPR header, kind 4/8) or box (types 0x18,0x19,0x24,0x33 of kind 3)
  +15 bf   bits 0-1 link mode (0 free, 1 parent, 2 child offset, 3 last child offset); bits 2-7 id: obj+0x6B (room-local) / persistent flag
           number FUN_80020a78(bf>>2) (area-wide list: spawned only while the flag is 0)

Which records a room runs: slot k = kind & 0xF of the APD in the load list; a room (section S) spawns slot S & 7 (`FUN_8004c570` first loop) and
the area-wide slot min(sections, 15) (second loop).  The other slots in a list belong to neighbouring sections (scroll spawn) and are listed with
own=0.

usage:
  tools/objects.py list [--area N] [--room S] [--type T] [--layer K] [--event E] [--all-slots] [--format table|csv|json] [-o FILE]
  tools/objects.py events            event id -> where it is placed (rooms/objects), and the 19 data-driven events status
  tools/objects.py dialogues         dialogue (k, dx) pairs that room objects can show (from the handler sources)
Needs game/disc (load lists) and game/MAIN0.EXE; never writes game data.
"""
import argparse, collections, csv, glob, json, os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import area000 as A

# MAIN0 handler tables (u32 code pointers indexed by obj type), taken from updateObjects* (asm): layer -> table address
TABLES = {1: 0x8007A10C, 2: 0x80077D9C, 3: 0x80079BAC, 4: 0x80079D74, 5: 0x8007B310, 7: 0x80079FE4, 8: 0x80079B88}
# record kind (b1 & 0x7F) -> handler table layer (allocObjectLayerN in FUN_8004cb00; kind 8 = allocObjectLayer1)
KIND_LAYER = {2: 2, 3: 3, 4: 4, 5: 5, 7: 7, 8: 1}
AREAS_WITH_OVERLAY = {7: 1, 12: 4}     # area -> overlay number when it shares (AREA07 uses X001, AREA12 uses X004)
SECTIONS = {0: 6, 1: 5, 2: 6, 3: 6, 4: 20, 5: 4, 6: 3, 7: 5, 8: 4, 9: 7, 10: 9, 11: 5, 12: 20, 13: 2, 14: 8, 15: 1, 16: 7, 17: 11, 18: 3, 19: 3}
# MAIN0 `dispatchAreaInitB` (layer 4, type 7): the area's own handler lives at a fixed address of its overlay
AREA_INIT_B = {0: 0x8011E30C, 1: 0x8011BD84, 7: 0x8011BD84, 2: 0x800E8DA4, 3: 0x8011A874, 4: 0x8011B7C4, 12: 0x8011B7C4,
               9: 0x8011AB44, 10: 0x80118558, 18: 0x801169D0}

# ------------------------------------------------------------------ data-driven event rules
# (decoded from the handler sources, see notes/secrets.md): the object id is a record subtype or the section
DOOR_EVENTS = {   # layer 4 type 7 = dispatchAreaInitB -> func_8011A874 (X003) / func_8011B7C4 (X004), subtype -> event (setEventStarted/Complete(o->wac))
    3: {0: 0x29, 4: 0xAA, 8: 0x37},
    4: {0: 0x29, 4: 0x33, 8: 0x37, 10: 0x2A},
    12: {0: 0x29, 4: 0x33, 8: 0x37, 10: 0x2A},
}
AREA14_BOSS_EVENT = [30, 111, 124, 34, 80, 85, 54, 160]     # X014 D_801256A8[section], completed by func_801163BC (layer 2 types 0x3B..0x42)
SEQ_TYPES = {  # layer 2 "scene sequence" objects: overlay -> {type: handler}; their sequences are pointer tables in the overlay (see seq_events)
    0: {0x34: 0x80134E5C}, 4: {0x35: 0x8012B438}, 12: {0x35: 0x8012B438}, 2: {0x4A: 0x80118684}, 19: {0x4A: 0x80118684},
    6: {0x51: 0x8011E54C}, 13: {0x56: 0x80116FE0}}
# overlay -> (sequence pointer table address, #sequences, start ops, complete ops, {op: {arg: event}} remapped start ops)
# op/arg are s16 pairs, a sequence ends at op 0.  Op numbers differ per overlay (read from each handler's `case N: setEvent...(e[1])`).
SEQ_TABLES = {0: (0x801396A0, 9, (9,), (10,), {}), 4: (0x80131370, 6, (8,), (9, 10), {}),
              6: (0x8012037C, 7, (9,), (11, 12), {10: {0x57: 0x85, 0x58: 0x86, 0x59: 0x87}}),
              2: (0x8011C8E8, 3, (7,), (8,), {}), 13: (0x801181FC, 1, (6,), (7,), {})}
BASE = 0x800E8028

_main = None


def main0():
    global _main
    if _main is None:
        p = os.path.join(A.FS, "EXE", "MAIN0.EXE")       # the disc copy (also there in the exe, which has no game/ folder)
        if not os.path.exists(p):
            p = os.path.join(ROOT, "game", "MAIN0.EXE")
        _main = open(p, "rb").read()[0x800:]
    return _main


def w32(addr):
    m = main0()
    return struct.unpack_from("<I", m, addr - 0x80010000)[0]


def handler_addr(layer, typ):
    return w32(TABLES[layer] + 4 * typ)


_names = {}
_csv = {}


def _load_csv(path):
    if path not in _csv:
        d = {}
        try:
            for line in open(os.path.join(ROOT, path)):
                f = line.strip().split(",")
                if len(f) >= 3 and re.fullmatch(r"[0-9a-f]{8}", f[0]):
                    # functions_x0nn.csv: address,size,name,twin ; functions_main0.csv: address,name,...
                    nm = f[2] if path != "notes/functions_main0.csv" else f[1]
                    d[int(f[0], 16)] = nm or None
        except OSError:
            pass
        _csv[path] = d
    return _csv[path]


def func_name(addr, ovl):
    """Name of the function at addr: overlay csv (notes/functions_x0nn.csv) or MAIN0 csv, else func_<addr>."""
    paths = (["notes/functions_x%03d.csv" % ovl] if addr >= BASE else []) + ["notes/functions_main0.csv"]
    for p in paths:
        n = _load_csv(p).get(addr)
        if n:
            return n
    return "func_%08X" % addr


def src_of(addr, ovl):
    """src file of the handler, preferring src/x0NN/ then src/."""
    pats = ["src/x%03d/*%08x*.c" % (ovl, addr), "src/*%08x*.c" % addr]
    for pat in pats:
        for f in glob.glob(os.path.join(ROOT, pat)) + glob.glob(os.path.join(ROOT, pat.replace("%08x" % addr, "%08X" % addr))):
            return f
    # renamed functions: find the file by its `// FUNC addr` header
    for d in ("src/x%03d" % ovl, "src"):
        for f in sorted(glob.glob(os.path.join(ROOT, d, "*.c"))):
            with open(f, errors="replace") as fh:
                if re.match(r"//\s*FUNC\s+%08x\b" % addr, fh.read(64), re.I):
                    return f
    return None


def overlay_of(area):
    return AREAS_WITH_OVERLAY.get(area, area)


# ------------------------------------------------------------------ record parsing
def room_lists(area):
    return A.load_lists().get("AR%02d" % area, [])


def parse_records(blob):
    out = []
    for i in range(0, len(blob) - 15, 16):
        b = blob[i:i + 16]
        if b[2] >= 0xFF:
            break
        x, y, z = struct.unpack_from("<hhh", b, 6)
        out.append(dict(index=i // 16, b0=b[0], b1=b[1], type=b[2], b3=b[3], subtype=b[4], b5=b[5], x=x, y=y, z=z,
                        bc=b[12], bd=b[13], be=b[14], bf=b[15], raw=b.hex()))
    return out


def is_record_block(rec, blob):
    if rec["kind"] & 0xF0 != 0x80 or blob is None or len(blob) % 16:
        return False
    if rec["kind"] == 0x8E or len(blob) > 0x10000:      # 0x8E = byte grid, huge 0x8F = opaque table (AREA02/05/08/19)
        return False
    return True


def collect(area=None):
    """All records of the (selected) areas: list of dicts with room/slot info."""
    rows = []
    for ld in sorted(A.load_lists()):
        if not ld.startswith("AR"):
            continue
        a = int(ld[2:])
        if area is not None and a != area:
            continue
        nsec = SECTIONS.get(a, 0)
        area_slot = min(nsec, 15)
        for li, lst in enumerate(A.load_lists()[ld]):
            for rec, blob in A.list_blocks(ld, li):
                if not is_record_block(rec, blob):
                    continue
                slot = rec["kind"] & 0xF
                for r in parse_records(blob):
                    r.update(area=a, list=li, section=li - 1 if li else None, slot=slot, apd_kind=rec["kind"],
                             role="area" if slot == area_slot else "room")
                    if r["role"] == "area":
                        r["section"] = r["b0"] if r["b0"] < 0x80 else (0xFF - r["b0"])
                        r["own"] = True
                    elif li == 0:
                        r["own"] = False
                    else:
                        r["own"] = (r["role"] == "room" and slot == ((li - 1) & 7)) or r["role"] == "area"
                    rows.append(r)
    return rows


# ------------------------------------------------------------------ decoding
_seq_cache = {}


READER = None    # set by animobj.build_owners when reading from a disc image


def seq_events(ovl):
    """{sequence index: {'start': [ids], 'complete': [ids]}} of the scene-sequence tables of overlay ovl."""
    if ovl in _seq_cache:
        return _seq_cache[ovl]
    out = {}
    if ovl in SEQ_TABLES:
        tab, n, sops, cops, smap = SEQ_TABLES[ovl]
        rel = "AREA%02d/X0%02d.BIN" % (ovl, ovl)
        try:    # the graphics editor reads the disc through READER (no extracted overlay on disk)
            d = READER(rel) if READER else open(os.path.join(A.FS, rel), "rb").read()
        except (OSError, KeyError):
            _seq_cache[ovl] = out
            return out
        for i in range(n):
            p = struct.unpack_from("<I", d, tab + 4 * i - BASE)[0]
            if not BASE <= p < BASE + len(d):
                break
            a, st, co = p, [], []
            for _ in range(80):
                op, arg = struct.unpack_from("<hh", d, a - BASE)
                a += 4
                if op in sops:
                    st.append(arg)
                elif op in cops:
                    co.append(arg)
                elif op in smap and arg in smap[op]:
                    st.append(smap[op][arg])
                if op == 0 and a > p + 4:
                    break
            out[i] = dict(start=st, complete=co)
    _seq_cache[ovl] = out
    return out


def decode(r):
    """Fill handler / event / dialogue fields of a record dict."""
    area, kind, typ = r["area"], r["b1"] & 0x7F, r["type"]
    ovl = overlay_of(area)
    layer = KIND_LAYER.get(kind)
    r["layer"] = layer
    r["handler"] = None
    r["handler_name"] = ""
    r["events"] = []
    r["event_note"] = ""
    r["dialogues"] = []
    if layer is None:
        return r
    try:
        h = handler_addr(layer, typ)
    except struct.error:
        return r
    note = ""
    if layer == 4 and typ == 7 and area in AREA_INIT_B:      # dispatchAreaInitB -> area handler
        h = AREA_INIT_B[area]
        note = "dispatchAreaInitB"
    r["handler"] = "0x%08X" % h
    r["handler_name"] = (note + ">" if note else "") + func_name(h, ovl)
    ev = []
    if layer == 4 and typ == 7 and area in DOOR_EVENTS and r["subtype"] in DOOR_EVENTS[area]:
        ev = [DOOR_EVENTS[area][r["subtype"]]]
        r["event_note"] = "door/switch subtype %d (start+complete)" % r["subtype"]
    elif area == 14 and layer == 2 and 0x3B <= typ <= 0x42:
        ev = [AREA14_BOSS_EVENT[typ - 0x3B]]
        r["event_note"] = "room controller, completed on clear of section %d" % (typ - 0x3B)
    elif layer == 2 and typ in SEQ_TYPES.get(area, SEQ_TYPES.get(ovl, {})):
        seqs = seq_events(ovl)
        evs = sorted({e for s in seqs.values() for e in s["start"] + s["complete"]})
        ev = evs
        r["event_note"] = "scene sequence object: events of ANY of its sequences (chosen by game state at run time)"
    r["events"] = ev
    # constants in the handler source: setEvent* literals and dialogue (k,dx) pairs
    f = src_of(h, ovl)
    if f:
        s = open(f).read()
        for m in re.finditer(r"setEvent(?:Started|Complete)\(\s*(0x[0-9a-fA-F]+|\d+)\s*,", s):
            v = int(m.group(1), 0)
            if v not in r["events"]:
                r["events"].append(v)
                r["event_note"] = r["event_note"] or "literal in handler"
        r["dialogues"] = sorted(dialogue_consts(s))
    return r


def dialogue_consts(s):
    """(k, dx) literal pairs: showMessageBox*/openMessageBox calls and `m = K; n = DX;` assignments in one function body."""
    out = set()
    num = r"(0x[0-9a-fA-F]+|\d+)"
    for m in re.finditer(r"(?:showMessageBox\w*|openMessageBox)\(\s*%s\s*,\s*%s\s*[,)]" % (num, num), s):
        out.add((int(m.group(1), 0), int(m.group(2), 0)))
    for m in re.finditer(r"\bm\s*=\s*%s;\s*(?:\n\s*)?n\s*=\s*%s;" % (num, num), s):
        out.add((int(m.group(1), 0), int(m.group(2), 0)))
    for m in re.finditer(r"SPAWN\([^,]+,\s*%s\)" % num, s):
        out.add((2, int(m.group(1), 0)))
    return out


def build(area=None):
    rows = collect(area)
    for r in rows:
        decode(r)
    return rows


# ------------------------------------------------------------------ output
COLS = ["area", "section", "list", "slot", "role", "own", "index", "layer", "type", "subtype", "handler", "handler_name", "x", "y", "z",
        "b0", "b3", "b5", "bc", "bd", "be", "bf", "events", "event_note", "dialogues", "raw"]


def flat(r):
    o = {c: r.get(c) for c in COLS}
    o["events"] = " ".join(str(e) for e in r["events"])
    o["dialogues"] = " ".join("%d/%d" % d for d in r["dialogues"])
    return o


def select(rows, a):
    out = []
    for r in rows:
        if a.room is not None and r.get("section") != a.room:
            continue
        if a.type is not None and r["type"] != a.type:
            continue
        if a.layer is not None and r["layer"] != a.layer:
            continue
        if a.event is not None and a.event not in r["events"]:
            continue
        if not a.all_slots and not r["own"]:
            continue
        out.append(r)
    return out


def cmd_list(a):
    rows = select(build(a.area), a)
    if a.format == "json":
        txt = json.dumps([flat(r) for r in rows], indent=1)
    elif a.format == "csv":
        import io
        b = io.StringIO()
        wr = csv.DictWriter(b, COLS)
        wr.writeheader()
        for r in rows:
            wr.writerow(flat(r))
        txt = b.getvalue()
    else:
        lines = []
        for r in rows:
            f = flat(r)
            lines.append("AREA%02d s%-2s %s L%s t=0x%02X st=%-3d (%5d,%5d,%4d) %-26s ev=[%s] dlg=[%s]" % (
                r["area"], "-" if r["section"] is None else r["section"], r["role"][0], r["layer"], r["type"], r["subtype"], r["x"], r["y"], r["z"],
                r["handler_name"][:26], f["events"], f["dialogues"]))
        txt = "\n".join(lines)
    if a.output:
        open(a.output, "w").write(txt + "\n")
    else:
        print(txt)


DATA_DRIVEN = [0, 4, 9, 10, 11, 32, 34, 41, 42, 44, 57, 77, 85, 111, 124, 133, 135, 170, 172]


def cmd_events(a):
    rows = build(None)
    where = collections.defaultdict(list)
    for r in rows:
        if not r["own"]:
            continue
        for e in r["events"]:
            where[e].append("AREA%02d:%s %s t=0x%02X st=%d" % (r["area"], r["section"], r["handler_name"], r["type"], r["subtype"]))
    for e in sorted(where):
        uniq = sorted(set(where[e]))
        mark = "*" if e in DATA_DRIVEN else " "
        print("%s event %3d (0x%02X): %d object(s): %s" % (mark, e, e, len(where[e]), "; ".join(uniq[:6])))
    print()
    for e in DATA_DRIVEN:
        print("data-driven event %3d: %s" % (e, "placed in %s" % ", ".join(sorted({w.split()[0] for w in where[e]})) if e in where else "NOT placed by any decoded mechanism"))


def cmd_dialogues(a):
    rows = build(None)
    seen = collections.defaultdict(set)
    for r in rows:
        if r["own"] and r["dialogues"]:
            for d in r["dialogues"]:
                seen[d].add("AREA%02d:%s" % (r["area"], r["section"]))
    for d in sorted(seen):
        print("k=%d dx=%d  rooms: %s" % (d[0], d[1], " ".join(sorted(seen[d]))))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("list")
    p.add_argument("--area", type=int)
    p.add_argument("--room", type=int)
    p.add_argument("--type", type=lambda s: int(s, 0))
    p.add_argument("--layer", type=int)
    p.add_argument("--event", type=int)
    p.add_argument("--all-slots", action="store_true")
    p.add_argument("--format", choices=("table", "csv", "json"), default="table")
    p.add_argument("-o", "--output")
    p.set_defaults(fn=cmd_list)
    sp.add_parser("events").set_defaults(fn=cmd_events)
    sp.add_parser("dialogues").set_defaults(fn=cmd_dialogues)
    a = ap.parse_args()
    a.fn(a)


if __name__ == "__main__":
    main()
