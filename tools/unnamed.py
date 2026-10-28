#!/usr/bin/env python3
"""Exports the functions that still carry a generic name (`func_8xxxxxxx` / `FUN_8xxxxxxx`) to notes/unnamed.json, the list the
graphics editor shows under "Sin nombre / Unnamed" (tools/names_panel.py) so that a user can propose names without the decomp
toolchain; tools/apply_names.py applies the names the editor exports (names_user.json).

Scope: the matched sources of MAIN0 and X000 (src/<name>.c with a `// FUNC addr size MAIN0|X000` header) and of the SYSTEM code
overlays of tools/sysovl.py (src/d0spsub*, o0ptsub0, i0nfo*, g0over), and the own functions of the area overlays X001..X019
(src/x0NN/<name>.c whose header names X0NN).  The *_slpm80165 (JP demo) files and src/wip are skipped; the X000 twins in
src/x0NN are renamed with their X000 original.

Per function: address, program, file, size, the `// probably:` note of the source, its callers (files of the same program that
use the name), the object table slots that point at it (pointer scan of game/MAIN0.EXE and game/AREA00/X000.BIN: layer / pool /
unlayered type tables of tools/objects.py), and, when the function is an object handler of area 0 (or of an area that uses its
twin) or is reached from one, the SPR table that draws that handler's animation scripts, resolved exactly as the "Animaciones"
node does (tools/animobj.py): area, owner handler, load list / kind, frame count and the renderable animation ids.

The JSON is derived from src/ and the notes (addresses and names only, no game data) and is committed.

Usage: python3 tools/unnamed.py [--out notes/unnamed.json] [--no-spr] [--areas 0,4,9]
  --no-spr   skip the sprite resolution (no game files needed; the rest only needs src/ and notes/)
"""
import glob, json, os, re, struct, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import sysovl

GENERIC = re.compile(r"(?:FUN|func)_[0-9a-fA-F]{8}$")
HDR = re.compile(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)\s+(\w+)")
SYS_DIRS = {p.lower(): p for p in sysovl.PROGS}         # src/d0spsub -> D0SPSUB
AREA_DIRS = {os.path.basename(d): "X%03d" % int(os.path.basename(d)[1:]) for d in glob.glob(os.path.join(ROOT, "src", "x0[0-9][0-9]"))}  # src/x001 -> X001
AREA_PROGS = sorted(AREA_DIRS.values())
MAIN0_BASE, OVL_BASE = 0x80010000, sysovl.BASE
# MAIN0 handler tables (u32 code pointers indexed by object type), see tools/objects.py and notes/object_layout.md
TABLES = {"layer 1": 0x8007A10C, "layer 2": 0x80077D9C, "pool": 0x80079BAC, "layer 4": 0x80079D74, "layer 5": 0x8007B310,
          "layer 7": 0x80079FE4, "layer 8": 0x80079B88, "unlayered": 0x8007BF34, "unlayered variant": 0x8007C1C8}
TABLE_MAX = 0x100       # entries, unless another table starts first


def rel(p):
    return os.path.relpath(p, ROOT).replace(os.sep, "/")


def read_head(path, n=12):
    try:
        with open(path, errors="replace") as f:
            return [next(f) for _ in range(n)]
    except StopIteration:
        return open(path, errors="replace").read().splitlines(True)
    except OSError:
        return []


def note_of(lines):
    for l in lines:
        m = re.match(r"//\s*probably:\s*(.*)", l)
        if m:
            return m.group(1).strip()
    return ""


def scan_sources():
    """[entry] of every generic-named function of MAIN0 / X000 / SYSTEM overlays, in (prog, address) order."""
    out = []
    for f in sorted(glob.glob(os.path.join(ROOT, "src", "*.c"))):
        stem = os.path.basename(f)[:-2]
        if stem.endswith("_slpm80165") or not GENERIC.match(stem):
            continue
        lines = read_head(f)
        m = next((HDR.match(l) for l in lines if HDR.match(l)), None)
        if not m or m.group(3) not in ("MAIN0", "X000"):
            continue
        out.append(dict(address="%08x" % int(m.group(1), 16), name=stem, prog=m.group(3), file=rel(f), size=int(m.group(2)),
                        note=note_of(lines)))
    for d, prog in sorted(list(SYS_DIRS.items()) + list(AREA_DIRS.items())):
        for f in sorted(glob.glob(os.path.join(ROOT, "src", d, "*.c"))):
            stem = os.path.basename(f)[:-2]
            if not GENERIC.match(stem):
                continue
            lines = read_head(f)
            m = next((HDR.match(l) for l in lines if HDR.match(l)), None)
            if not m or m.group(3) != prog:
                continue
            out.append(dict(address="%08x" % int(m.group(1), 16), name=stem, prog=prog, file=rel(f), size=int(m.group(2)),
                            note=note_of(lines)))
    order = ["MAIN0", "X000"] + sysovl.PROGS + AREA_PROGS
    out.sort(key=lambda e: (order.index(e["prog"]), int(e["address"], 16)))
    return out


def scope_files(prog):
    """Source files whose identifiers belong to the program: MAIN0 and X000 share src/*.c (+ include/), each SYSTEM overlay has its dir."""
    if prog in ("MAIN0", "X000"):
        fs = [f for f in glob.glob(os.path.join(ROOT, "src", "*.c")) if not f.endswith("_slpm80165.c")]
        fs += glob.glob(os.path.join(ROOT, "src", "data", "**", "*.c"), recursive=True)
        fs += glob.glob(os.path.join(ROOT, "include", "**", "*.h"), recursive=True)
        return sorted(fs)
    if prog in AREA_PROGS:
        return sorted(glob.glob(os.path.join(ROOT, "src", prog.lower(), "*.c")) + glob.glob(os.path.join(ROOT, "src", "data", prog.lower(), "*.c")))
    return sorted(glob.glob(os.path.join(ROOT, "src", prog.lower(), "*.c")))


def find_callers(entries):
    by_scope = {}
    for e in entries:
        by_scope.setdefault("MAIN0" if e["prog"] in ("MAIN0", "X000") else e["prog"], []).append(e)
    for scope, ents in by_scope.items():
        names = {e["name"]: e for e in ents}
        rx = re.compile(r"\b(" + "|".join(sorted(map(re.escape, names), key=len, reverse=True)) + r")\b")
        for e in ents:
            e["callers"] = set()
        for f in scope_files(scope):
            try:
                t = open(f, errors="replace").read()
            except OSError:
                continue
            for nm in set(rx.findall(t)):
                e = names[nm]
                if rel(f) != e["file"]:
                    e["callers"].add(os.path.basename(f).rsplit(".", 1)[0])
        for e in ents:
            e["callers"] = sorted(e["callers"])


def find_twins(entries):
    """src/x0NN/<name>.c copies of the X000 functions (same code in the area overlays; renamed together)."""
    for e in entries:
        e["twins"] = []
        if e["prog"] == "X000":
            e["twins"] = sorted(os.path.basename(os.path.dirname(f)) for f in glob.glob(os.path.join(ROOT, "src", "x0[0-9][0-9]", e["name"] + ".c")))


def _bin(path, skip=0):
    try:
        return open(path, "rb").read()[skip:]
    except OSError:
        return None


def pointer_refs(entries):
    """Data words equal to the function address in MAIN0.EXE / X000.BIN / the overlay binary; MAIN0 words inside a known object table
    give (table, index) = the layer / pool / unlayered type the function handles."""
    bins = {"MAIN0": (_bin(os.path.join(ROOT, "game", "MAIN0.EXE"), 0x800), MAIN0_BASE),
            "X000": (_bin(os.path.join(ROOT, "game", "AREA00", "X000.BIN")), OVL_BASE)}
    for prog, (path, _, _) in sysovl.SYSOVL.items():
        bins[prog] = (_bin(os.path.join(ROOT, "game", path)), OVL_BASE)
    for prog in AREA_PROGS:
        g = sorted(glob.glob(os.path.join(ROOT, "game", "AREA*", prog + ".BIN")))
        if g:
            bins[prog] = (_bin(g[0]), OVL_BASE)
    bounds = sorted(TABLES.values())
    for e in entries:
        a = int(e["address"], 16)
        refs = []
        # MAIN0 words in the overlay range name the area overlay (X000) function at that address, never a SYSTEM overlay one
        progs = ["MAIN0"] if e["prog"] == "MAIN0" else ["MAIN0", "X000"] if e["prog"] == "X000" else [e["prog"]]
        for p in progs:
            d, base = bins.get(p, (None, 0))
            if not d:
                continue
            needle = struct.pack("<I", a)
            i = d.find(needle)
            while i >= 0:
                if i % 4 == 0:
                    at = base + i
                    r = dict(prog=p, at="%08x" % at)
                    if p == "MAIN0":
                        for tn, ta in TABLES.items():
                            nxt = min([b for b in bounds if b > ta] + [ta + 4 * TABLE_MAX])
                            if ta <= at < nxt:
                                r["table"], r["index"] = tn, (at - ta) // 4
                    refs.append(r)
                i = d.find(needle, i + 1)
        e["refs"] = refs
        e["object"] = next(({"table": r["table"], "type": r["index"], "slot": r["at"]} for r in refs if "table" in r), None)


def resolve_sprites(entries, areas=None, log=print):
    """Links X000 functions to the animation owners (tools/animobj.py) of area 0 and of the areas whose overlay holds a twin: the
    handler itself or any function its handler reaches; attaches the resolved SPR table and the renderable animation ids."""
    import anim, animobj, objects as O
    x000 = [e for e in entries if e["prog"] == "X000"]
    for e in entries:
        e["anim"] = None
    if not x000:
        return
    wanted = set([0])
    for e in x000:
        for t in e["twins"]:
            n = int(t[1:])
            wanted |= {a for a in O.SECTIONS if O.overlay_of(a) == n}
    if areas is not None:
        wanted &= set(areas)
    for area in sorted(wanted):
        t0 = time.time()
        try:
            owners = anim.object_owners(None, area)
        except Exception as ex:      # no game files for that area
            log("area %d: %s" % (area, ex))
            continue
        ov = animobj.overlay(O.overlay_of(area))
        n_ovl = O.overlay_of(area)
        for ow in owners:
            h = int(ow["handler_addr"], 16)
            try:
                _, _, funcs = ov.reach(h)
            except Exception:
                funcs = {h}
            for e in x000:
                a = int(e["address"], 16)
                if a not in funcs or (area and "x%03d" % n_ovl not in e["twins"]):
                    continue
                if e["anim"] and (e["anim"]["spr"] or not ow.get("spr")):
                    continue        # keep the first owner that has a frame source
                sp = ow.get("spr")
                e["anim"] = dict(area=area, owner=ow["owner"], handler=ow["handler"], handler_addr=ow["handler_addr"], direct=(a == h),
                                 types=ow["types"], placements=ow["placements"],
                                 spr=None if not sp else dict(list=sp["list"], kind=sp["kind"],
                                                              frames=animobj.table_frames("AR%02d" % area, sp["list"], int(sp["kind"], 16))),
                                 anims=[an["id"] for an in ow["animations"] if an.get("renderable", True)])
        log("area %d: %d owners, %.0f s" % (area, len(owners), time.time() - t0))
    # functions that select animation scripts themselves (their own constants: a script or a table of scripts, and the SPR slot they
    # read) but belong to no placed handler: the frames come from the area-0 list of the slot they read
    ov = animobj.overlay(0)
    for e in x000:
        if e["anim"] and e["anim"]["spr"]:
            continue
        a = int(e["address"], 16)
        scripts, sprs = own_scripts(ov, a)
        if not scripts:
            continue
        frames = [[x["frame"] for x in animobj.script_at(ov, s)["entries"]] for s in scripts]
        idx, kind, _ = animobj.pick_source("AR00", {}, sorted(sprs), frames) if sprs else (None, None, [])
        e["anim"] = dict(area=0, owner=None, handler=e["name"], handler_addr="%08X" % a, direct=True, types=[], placements=0,
                         spr=None if idx is None else dict(list=idx, kind="0x%02X" % kind, frames=animobj.table_frames("AR00", idx, kind)),
                         anims=list(range(len(scripts))) if idx is not None else [], scripts=["%08X" % s for s in scripts])


def own_scripts(ov, addr):
    """(scripts, SPR kinds) the function at addr references directly (no callees), as tools/animobj.py finds them for a handler."""
    import animobj
    consts, sprs, _ = ov.reach(addr, 0)
    got, seen = [], set()
    for c in sorted(x for x in consts if animobj.BASE <= x < animobj.BASE + len(ov.d) - 8 and not x & 3):
        t = animobj.table_at(ov, c)
        if t:
            lim = animobj._next_start(ov, c)
            for i, p in enumerate(t):
                if c + 4 * i >= lim and i:
                    break
                if p not in seen and animobj.script_at(ov, p):
                    seen.add(p)
                    got.append(p)
        elif c not in seen and animobj.script_at(ov, c):
            seen.add(c)
            got.append(c)
    return got, sprs


def synthetic_owner(entry, reader=None):
    """Owner dict (as tools/animobj.build_owners makes them) for an unnamed function that selects scripts itself (`anim.owner` is None):
    the graphics editor plays its scripts with the resolved SPR table of area 0."""
    import animobj
    an = entry["anim"]
    ov = animobj.overlay(0, reader)
    anims = []
    for n, s in enumerate(int(x, 16) for x in an.get("scripts", [])):
        sc = animobj.script_at(ov, s)
        if not sc:
            continue
        ents = sc["entries"]
        anims.append(dict(id=n, name="anim_%02d" % n, start="%08X" % s, table=None, index=0, entries=ents, loop=sc["loop"], end=sc["end"],
                          signal=sc["signal"], ticks=sum(e["dur"] for e in ents), frames=[e["frame"] for e in ents], alias_of=None))
    sp = an.get("spr")
    nfr = sp["frames"] if sp else 0
    for a in anims:
        a["renderable"] = bool(sp) and max(a["frames"]) < nfr
    return dict(owner="unnamed:" + entry["address"], name=entry["name"], handler=entry["name"], handler_addr=an["handler_addr"], area=0, types=[],
                placements=0, rooms={}, sprs=[int(sp["kind"], 16)] if sp else [], w1e=[], init=[], animations=anims,
                spr=None if not sp else dict(list=sp["list"], kind=sp["kind"], w1e=[], clut=None, alternatives=[]))


# ------------------------------------------------------------------ the editor side: the exported list and the user's names
def json_path():
    """notes/unnamed.json: bundled in the exe (PyInstaller --add-data "notes/unnamed.json;notes") or in the repository."""
    for base in (os.path.join(getattr(sys, "_MEIPASS", ""), "notes"), os.path.join(ROOT, "notes")):
        p = os.path.join(base, "unnamed.json")
        if os.path.isfile(p):
            return p
    return os.path.join(ROOT, "notes", "unnamed.json")


def load(path=None):
    """[entry] of notes/unnamed.json (empty when missing)."""
    try:
        with open(path or json_path(), encoding="utf-8") as f:
            d = json.load(f)
        return d.get("entries", []) if isinstance(d, dict) else []
    except (OSError, ValueError):
        return []


def key_of(e):
    return "%s:%s" % (e["prog"], e["address"])


def user_path():
    """Where the editor keeps the names typed so far (autosaved): %APPDATA%/TombiTools/names_user.json or ~/.config/TombiTools/."""
    base = os.environ.get("APPDATA") if os.name == "nt" else os.path.join(os.path.expanduser("~"), ".config")
    return os.path.join(base or os.path.expanduser("~"), "TombiTools", "names_user.json")


def read_user(path):
    """{key: {"new": str, "comment": str}} from a names_user.json (the export format, a list of {address, prog, old, new, comment})."""
    try:
        with open(path, encoding="utf-8") as f:
            d = json.load(f)
    except (OSError, ValueError):
        return {}
    if isinstance(d, dict):
        d = d.get("names") or d.get("entries") or []
    out = {}
    for r in d:
        if isinstance(r, dict) and r.get("prog") and r.get("address"):
            try:
                k = "%s:%08x" % (str(r["prog"]).upper(), int(str(r["address"]), 16))
            except ValueError:
                continue
            out[k] = dict(new=str(r.get("new", "")).strip(), comment=str(r.get("comment", "")).strip())
    return out


def write_user(path, names, entries):
    """Writes the export format: one row per entry with a name or a comment, in the order of notes/unnamed.json.  Returns the row count."""
    by = {key_of(e): e for e in entries}
    rows = []
    for k, v in names.items():
        if not (v.get("new") or v.get("comment")):
            continue
        prog, addr = k.split(":")
        e = by.get(k)
        rows.append(dict(address=addr, prog=prog, old=e["name"] if e else "", new=v.get("new", ""), comment=v.get("comment", "")))
    order = {key_of(e): i for i, e in enumerate(entries)}
    rows.sort(key=lambda r: order.get("%s:%s" % (r["prog"], r["address"]), 1 << 30))
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        json.dump(rows, f, indent=1, ensure_ascii=False)
        f.write("\n")
    return len(rows)


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", default=os.path.join(ROOT, "notes", "unnamed.json"))
    ap.add_argument("--no-spr", action="store_true")
    ap.add_argument("--areas", help="comma separated area numbers for the sprite resolution (default: every area with a twin)")
    a = ap.parse_args(argv)
    entries = scan_sources()
    find_callers(entries)
    find_twins(entries)
    pointer_refs(entries)
    if not a.no_spr:
        resolve_sprites(entries, None if not a.areas else [int(x) for x in a.areas.split(",")])
    else:
        for e in entries:
            e["anim"] = None
    doc = dict(generated_by="tools/unnamed.py", count=len(entries), progs={p: sum(1 for e in entries if e["prog"] == p) for p in ["MAIN0", "X000"] + sysovl.PROGS + AREA_PROGS},
               entries=entries)
    with open(a.out, "w", encoding="utf-8") as f:
        json.dump(doc, f, indent=1, ensure_ascii=False)
        f.write("\n")
    n_spr = sum(1 for e in entries if e["anim"] and e["anim"]["spr"])
    n_obj = sum(1 for e in entries if e["object"])
    print("%s: %d functions (%s), %d with a table slot, %d with sprite frames" % (rel(a.out), len(entries), ", ".join("%s %d" % kv for kv in doc["progs"].items()), n_obj, n_spr))
    return 0


if __name__ == "__main__":
    sys.exit(main())
