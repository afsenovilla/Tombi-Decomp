#!/usr/bin/env python3
"""Static CLUT / texture-page resolution of the SPR sprites of every area, read from the decompiled game code (no game run).

How the game colours an SPR sprite (src/drawObjectSprite.c, MAIN0 8005296c):
  * the frame's parts are POLY_FT4 records of the table `o->d3c` points at; every part carries its own clut and tpage word;
  * `p->tpage += o->w1e`  - the object's texture page offset, a constant the type's handler sets;
  * `if (o->b0d & 1) p->clut = o->w08` - an optional per-object CLUT that replaces the part's own word.
`o->d3c` is set by the handler from one of the scratchpad SPR slots `SPAD.i2c8 + 4 * n` (`SPR_DATA[n]`, `DAT_1f8002xx`), and slot n
is the table of load kind 0x30 + n of the area / room load list (queueLoadList stores the pointer of a kind 0x3n block in slot n).
Example (AREA00, X000): updateKomaPig / updateA00PinkPiglet set `o->d3c = SPAD.i2d0` (slot 2 = the kind 0x32 table of the area
list LD0AR00 list 0), `o->w1e = 1`, `o->b0d = 0`, so the part clut words 0x7C13 / 0x7C53 / 0x7C93 / 0x7CD3 are used as they are:
CLUT (304, 496..499), a 16 colour strip of the room's CLUTnn.GAM (256 x 32 at 128, 480).  The table's pages are the room's
Bn00 atlas (x 64..), which the area list itself does not hold, so the sprites only resolve against a room list.

This module scans src/ (every function file: `// FUNC addr size OVERLAY` header) for the handlers' slot loads, w1e constants and
w08 CLUT constants (GetClut(x, y) / FUN_8005e420 / literal words) and returns, per area list, the code variants of every slot.

  variants(ld, k) -> {slot: [(w1e, clut override or None, [handler file names])]}
"""
import glob, os, re

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(os.path.dirname(HERE), "src")

_SLOT = re.compile(r"d3c\s*=\s*.*?(?:SPAD(?:\.|->)i|1[fF]8002|pad\[0x)([0-9a-fA-F]{2,3})\b")
_SPRD = re.compile(r"d3c\s*=\s*SPR_DATA\[(0x[0-9a-fA-F]+|\d+)\]")
_VAR = re.compile(r"d3c\s*=\s*([A-Za-z_]\w*)\s*;")
_W1E = re.compile(r"\bw1e\s*=\s*(-?0x[0-9a-fA-F]+|-?\d+)\s*;")
_W08 = re.compile(r"\bw08\s*=\s*(?:(0x[0-9a-fA-F]+)\s*;|(?:GetClut|FUN_8005e420)\(\s*(0x[0-9a-fA-F]+|\d+)\s*,\s*(0x[0-9a-fA-F]+|\d+)\s*\))")
_B0D = re.compile(r"\bb0d\s*(?:=\s*(?:1|0x81|0x01)\s*;|\|=\s*1\s*;)")

_CODE = None
_FUNCS = None
_IDENT = re.compile(r"\b[A-Za-z_]\w*\b")
_ADDRN = re.compile(r"^(?:func|FUN)_([0-9A-Fa-f]{8})$")


def functions(src=SRC):
    """{(overlay, name): dict(addr, file, refs=set of identifiers, var={slot: {(w1e, clut)}})} and {(overlay, addr): name} of every
    decompiled function (the `// FUNC addr size OVERLAY` header of its file)."""
    global _FUNCS
    if _FUNCS is not None and src == SRC:
        return _FUNCS
    funcs, byaddr = {}, {}
    for f in glob.glob(os.path.join(src, "**", "*.c"), recursive=True):
        try:
            t = open(f, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        h = t.split("\n", 1)[0].split()
        if len(h) < 5 or h[1] != "FUNC":
            continue
        ov, addr, name = h[4].upper(), int(h[2], 16), os.path.basename(f)[:-2]
        key = (ov, name)
        if key in funcs:
            continue
        funcs[key] = dict(addr=addr, file=f, refs=set(_IDENT.findall(t)) - {name}, var=_variants_of(t))
        byaddr[(ov, addr)] = name
    _FUNCS = (funcs, byaddr)
    return _FUNCS


def reachable(roots, ovs, src=SRC):
    """Names of the functions of overlays ovs reachable from roots [(overlay, addr)] through any reference by name (calls and the
    function pointer tables of the handlers: `static void (*D_801390F8[11])(TObj *) = {komaPigSubtype00Step, ...}`)."""
    funcs, byaddr = functions(src)
    order = [o for o in ovs if o != "MAIN0"] + ["MAIN0"]

    def resolve(ident):
        m = _ADDRN.match(ident)
        for ov in order:
            if m and (ov, int(m.group(1), 16)) in byaddr:
                return (ov, byaddr[(ov, int(m.group(1), 16))])
            if (ov, ident) in funcs:
                return (ov, ident)
        return None
    seen, todo = set(), []
    for ov, a in roots:
        for o in ([ov] if ov else order):
            if (o, a) in byaddr:
                todo.append((o, byaddr[(o, a)]))
                break
    while todo:
        k = todo.pop()
        if k in seen:
            continue
        seen.add(k)
        for ident in funcs[k]["refs"]:
            r = resolve(ident)
            if r and r not in seen:
                todo.append(r)
    return seen


def _variants_of(t):
    """{slot: {(w1e, clut or None)}} set by one function's source text."""
    if "d3c" not in t:
        return {}
    return _scan_text(t)


def scan(src=SRC):
    """{overlay: {slot: {(w1e, clut or None): set(file)}}} from the decompiled handlers."""
    global _CODE
    if _CODE is not None and src == SRC:
        return _CODE
    out = {}
    for f in glob.glob(os.path.join(src, "**", "*.c"), recursive=True):
        try:
            t = open(f, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        if "d3c" not in t:
            continue
        h = t.split("\n", 1)[0].split()
        if len(h) < 5 or h[1] != "FUNC":
            continue
        ov = h[4].upper()
        name = os.path.basename(f)[:-2]
        for sl, vs in _scan_text(t).items():
            for wc in vs:
                out.setdefault(ov, {}).setdefault(sl, {}).setdefault(wc, set()).add(name)
    if src == SRC:
        _CODE = out
    return out


def _scan_text(t):
    slots = set()
    idents = set(_VAR.findall(t))           # `o->d3c = t;`: the slot load is on the line that sets t
    for line in t.splitlines():
        if "d3c" not in line and not any(re.search(r"\b%s\s*=" % re.escape(v), line) for v in idents):
            continue
        line = re.sub(r"^.*?\b(%s)\s*=" % "|".join(map(re.escape, idents)), "d3c =", line) if idents and "d3c" not in line else line
        m = _SPRD.search(line)
        if m:
            slots.add(int(m.group(1), 0))
            continue
        m = _SLOT.search(line)
        if m:
            g = m.group(1)
            off = int(g if len(g) == 3 else "2" + g, 16)
            if 0x2C8 <= off < 0x308 and off % 4 == 0:
                slots.add((off - 0x2C8) // 4)
    if not slots:
        return {}
    w1es = {int(x, 0) & 0xFFFF for x in _W1E.findall(t)} or {0}
    cluts = set()
    if _B0D.search(t):                   # the handler turns the CLUT override on: its w08 words replace the parts' own
        for lit, x, y in _W08.findall(t):
            if lit:
                c = int(lit, 0)
                if c >= 0x7000:          # a CLUT word (VRAM y >= 448); smaller values are timers reusing the field
                    cluts.add(c & 0x7FFF)
            else:
                cluts.add((int(y, 0) << 6) | (int(x, 0) >> 4))
    cluts = cluts or {None}
    res = {}
    for sl in slots:
        for w in w1es:
            for c in cluts:
                res.setdefault(sl, set()).add((w, c))
    return res


def overlays(lists, ld, k):
    """Code that runs in room list k of area ld: MAIN0 + the X0nn / I0NFO* overlays its area list and room list load."""
    ovs = {"MAIN0"}
    for kk in {0, k}:
        if kk < len(lists[ld]):
            for r in lists[ld][kk]:
                if (r["kind"] & 0xF0) == 0xF0 and r["name"].upper().endswith(".BIN"):
                    ovs.add(r["name"].rsplit("/", 1)[-1][:-4].upper())
    return ovs


_ROOTS = {}


def room_roots(area, section):
    """[(overlay or None, handler address)] of the objects placed in room `section` of `area` (tools/objects.py: the area-wide list
    entries for that section + the room's own list), or None when the placement records cannot be read."""
    if (area, section) not in _ROOTS:
        try:
            import objects as O
            if area not in _ROOTS:
                _ROOTS[area] = O.build(area)
            rows = _ROOTS[area]
        except Exception:
            rows = None
        if rows is None:
            _ROOTS[(area, section)] = None
        else:
            out = set()
            for r in rows:
                if r.get("own") and r.get("section") == section and r.get("handler"):
                    try:
                        out.add((None, int(str(r["handler"]), 0)))
                    except ValueError:
                        pass
            _ROOTS[(area, section)] = sorted(out, key=lambda x: x[1])
    return _ROOTS[(area, section)]


def variants(lists, ld, k, src=SRC, by_room=True):
    """{slot: [(w1e, clut override or None, [handlers])]} for room list k of area ld.  by_room: only the handlers reachable from the
    objects placed in that room (section k - 1), plus all MAIN0 code (player, effects, items, HUD), so a variant of another
    room (AREA00: the hooded old man of B300, slot 3 + w1e 1 + CLUT 0x780C) is not applied to this room's pages."""
    funcs, _ = functions(src)
    ovs = overlays(lists, ld, k)
    roots = room_roots(int(ld[2:]), k - 1) if by_room and k > 0 else None
    allowed = reachable(roots, ovs, src) if roots else None
    out = {}
    for (ov, name), f in sorted(funcs.items(), key=lambda kv: (kv[0][0] == "MAIN0", kv[0])):
        if ov not in ovs or not f["var"]:
            continue
        if allowed is not None and ov != "MAIN0" and (ov, name) not in allowed:     # engine code (effects, items, HUD) runs anywhere
            continue
        for sl, vs in f["var"].items():
            for w, c in vs:
                lst = out.setdefault(sl, [])
                hit = next((e for e in lst if e[0] == w and e[1] == c), None)
                if hit:
                    hit[2].append(name)
                else:
                    lst.append((w, c, [name]))
    for sl in out:
        out[sl].sort(key=lambda e: (e[0], e[1] is None, e[1] or 0))
    return out


if __name__ == "__main__":
    import sys
    for ov, sl in sorted(scan().items()):
        if len(sys.argv) > 1 and ov not in sys.argv[1:]:
            continue
        for s, vs in sorted(sl.items()):
            for (w, c), fs in sorted(vs.items(), key=lambda kv: (kv[0][0], kv[0][1] or 0)):
                print("%-7s slot %2d (kind 0x%02X) w1e %3d clut %s  %s" % (ov, s, 0x30 + s, w, "%04X" % c if c is not None else "part",
                                                                         ", ".join(sorted(fs))[:100]))
