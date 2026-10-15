"""Enemy / NPC / object animation catalogue: which animation scripts each object handler uses, and which SPR table draws them.

How the data is found (static analysis, no emulation):
 - every placed object has a handler (tools/objects.py: layer/type -> code pointer in the area overlay `X0nn.BIN`, base 0x800E8028);
 - the handler and the functions it calls (jal inside the overlay, depth <= CALL_DEPTH) are disassembled (capstone, MIPS32 LE) and their
   constants tracked (`lui` + `addiu`/`ori`/load/store offsets): an address that holds an animation script (8-byte entries, see anim.py) or
   a pointer array of scripts (`o->anim = D_8013xxxx[k]`, `AnimAdvanceWithBox`) is one of the object's animation tables;
 - `lw rX, 0x1F8002C8+4n` is `o->d3c = SPR_DATA[n]`: the SPR table (load kind 0x30+n of the area load list) whose frame number is the first
   u16 of every script entry (frames are SPR atlas cuts, rendered like `tools/vram.py export_spr`).
Scripts that only the shared MAIN0 code selects are not found (reported without a frame source).
"""
import os, struct, sys, collections

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import area000 as A
import objects as O

BASE = O.BASE
CALL_DEPTH = 6
SPR_BASE = 0x1F8002C8


ZERO_REGS = set()


def overlay_bytes(ovl, reader=None):
    if reader:
        return reader("AREA%02d/X0%02d.BIN" % (ovl, ovl))
    return open(os.path.join(A.FS, "AREA%02d" % ovl, "X0%02d.BIN" % ovl), "rb").read()


_md = None


def _cs():
    global _md
    if _md is None:
        import capstone
        _md = capstone.Cs(capstone.CS_ARCH_MIPS, capstone.CS_MODE_MIPS32 | capstone.CS_MODE_LITTLE_ENDIAN)
        _md.detail = True
    return _md


class Overlay:
    def __init__(self, ovl, reader=None):
        self.ovl = ovl
        self.d = overlay_bytes(ovl, reader)
        self.funcs = self._functions()
        self.fstarts = {a for a, _ in self.funcs}
        self._cache = {}
        self.w1e = {}

    def _functions(self):
        """[(addr, size)]: notes/functions_x0nn.csv, widened by the `// FUNC addr size` headers of src/x0nn/*.c (some functions are merged)."""
        import glob
        sizes = {}
        try:
            for line in open(os.path.join(ROOT, "notes", "functions_x%03d.csv" % self.ovl)):
                f = line.strip().split(",")
                if len(f) >= 3 and len(f[0]) == 8:
                    try:
                        sizes[int(f[0], 16)] = int(f[1])
                    except ValueError:
                        pass
        except OSError:
            pass
        for fn in glob.glob(os.path.join(ROOT, "src", "x%03d" % self.ovl, "*.c")):
            try:
                t = open(fn).readline().split()
                if len(t) >= 4 and t[1] == "FUNC":
                    sizes[int(t[2], 16)] = max(sizes.get(int(t[2], 16), 0), int(t[3]))
            except (OSError, ValueError):
                pass
        return sorted(sizes.items())

    def size_of(self, addr):
        for a, s in self.funcs:
            if a == addr:
                return s
        # unknown function: up to the next known one, at most 0x800 bytes
        nxt = [a for a, _ in self.funcs if a > addr]
        return min(0x800, (nxt[0] - addr) if nxt else 0x400)

    def u16(self, a):
        return struct.unpack_from("<H", self.d, a - BASE)[0]

    def u32(self, a):
        return struct.unpack_from("<I", self.d, a - BASE)[0]

    def inside(self, a, n=1):
        return BASE <= a and a - BASE + n <= len(self.d)

    def scan(self, addr):
        """(constants, calls, spr kinds) of the function at addr: constants = set of addresses built by lui/addiu/ori or used as lui+offset."""
        if addr in self._cache:
            return self._cache[addr]
        size = self.size_of(addr)
        code = self.d[addr - BASE:addr - BASE + size]
        consts, calls, sprs = set(), set(), set()
        hi, imm = {}, {}
        w1e = set()
        for ins in _cs().disasm(code, addr):
            m, ops = ins.mnemonic, ins.operands
            try:
                if m in ("addiu", "ori", "li") and ops[0].type == 1 and (m == "li" or (ops[1].type == 1 and ops[1].reg in ZERO_REGS)):
                    imm[ops[0].reg] = ops[-1].imm
                elif m in ("sh", "sw", "sb") and ops[1].type == 3 and ops[1].mem.disp == 0x1E and ops[0].reg in imm:
                    w1e.add(imm[ops[0].reg])
                elif ops and ops[0].type == 1 and m not in ("sw", "sh", "sb", "jal", "jr", "beq", "bne", "j", "lui"):
                    imm.pop(ops[0].reg, None)
                if m == "lui":
                    hi[ops[0].reg] = ops[1].imm << 16
                    continue
                if m in ("addiu", "ori", "addi") and ops[1].type == 1 and ops[1].reg in hi:
                    v = (hi[ops[1].reg] + (ops[2].imm if m != "ori" else ops[2].imm & 0xFFFF)) & 0xFFFFFFFF
                    consts.add(v)
                    hi[ops[0].reg] = v
                    continue
                if m in ("lw", "lhu", "lh", "lbu", "lb", "sw", "sh", "sb") and ops[1].type == 3 and ops[1].mem.base in hi:
                    v = (hi[ops[1].mem.base] + ops[1].mem.disp) & 0xFFFFFFFF
                    consts.add(v)
                    if m == "lw" and SPR_BASE <= v < SPR_BASE + 0x40 and (v - SPR_BASE) % 4 == 0:
                        sprs.add(0x30 + (v - SPR_BASE) // 4)
                if m == "jal":
                    calls.add(ops[0].imm)
                if ops and ops[0].type == 1 and m not in ("sw", "sh", "sb", "jal", "jr", "beq", "bne", "j") and ops[0].reg in hi:
                    hi.pop(ops[0].reg, None)    # destination overwritten (loads/ALU)
            except (IndexError, AttributeError):
                pass
        self.w1e[addr] = w1e
        r = (consts, calls, sprs)
        self._cache[addr] = r
        return r

    def reach_w1e(self, funcs):
        out = set()
        for f in funcs:
            out |= self.w1e.get(f, set())
        return sorted(v for v in out if 0 <= v < 32)

    def is_func(self, a):
        return a in self.fstarts

    def reach(self, addr, depth=CALL_DEPTH):
        """consts, sprs, functions of the function and of its callees (jal, function pointers, tables of function pointers)."""
        consts, sprs, seen, todo = set(), set(), set(), [(addr, 0)]
        while todo:
            a, dep = todo.pop()
            if a in seen or not self.inside(a, 4):
                continue
            seen.add(a)
            c, calls, s = self.scan(a)
            consts |= c
            sprs |= s
            if dep < depth:
                nxt = set(calls)
                for k in c:
                    if self.is_func(k):
                        nxt.add(k)
                    elif BASE <= k < BASE + len(self.d) - 4 and not k & 3:      # table of function pointers?
                        j = k
                        while self.inside(j, 4) and self.is_func(self.u32(j)) and j < k + 4 * 32:
                            nxt.add(self.u32(j))
                            j += 4
                todo += [(x, dep + 1) for x in nxt]
        return consts, sprs, seen


# ------------------------------------------------------------------ script / table validation
def script_at(ov, a, maxn=80):
    """Decode a script at overlay address a like anim.decode_script, but defensively: None unless it looks like animation data."""
    if a & 3 or not ov.inside(a, 8):
        return None
    ents, seen, loop, end, signal = [], {}, None, "hold", False
    while len(ents) < maxn:
        if not ov.inside(a, 8):
            return None
        if a in seen:
            loop, end = seen[a], "loop"
            break
        fr, box, arg, v = (ov.u16(a + 2 * k) for k in range(4))
        if fr > 1200 or box > 0xFF:
            return None
        seen[a] = len(ents)
        ents.append(dict(addr=a, frame=fr, box=box, arg=arg - 0x10000 if arg >= 0x8000 else arg, dur=v & 0x3FFF, flags=v & 0xC000))
        f = v & 0xC000
        if f == 0:
            a += 8
        elif f == 0x8000:
            end, signal = "hold", True
            break
        else:
            if not ov.inside(a + 8, 4):
                return None
            tgt = ov.u32(a + 8)
            signal = f == 0xC000
            end = "loop"
            if tgt in seen:
                loop = seen[tgt]
                break
            if not ov.inside(tgt, 8) or tgt & 3:
                return None
            ents[-1]["jump"] = tgt
            a = tgt
    else:
        return None
    if not ents or all(e["dur"] == 0 for e in ents) or sum(e["dur"] for e in ents) > 3000:
        return None
    return dict(entries=ents, loop=loop, end=end, signal=signal)


def table_at(ov, a, maxn=64):
    """Pointer array of scripts starting at a: [script addr, ...] (stops at the first non-script pointer)."""
    out = []
    while len(out) < maxn and ov.inside(a, 4):
        p = ov.u32(a)
        if not (BASE <= p < BASE + len(ov.d)) or script_at(ov, p) is None:
            break
        out.append(p)
        a += 4
    return out


def handler_anims(ov, addr):
    """{'tables': {table addr: [script addrs]}, 'scripts': [direct script addrs], 'sprs': [kinds]} of one handler."""
    consts, sprs, funcs = ov.reach(addr)
    tables, scripts = {}, []
    inov = sorted(c for c in consts if BASE <= c < BASE + len(ov.d) and not c & 3)
    for c in inov:
        t = table_at(ov, c)
        if len(t) >= 2:
            tables[c] = t
        elif script_at(ov, c) is not None:
            scripts.append(c)
        elif len(t) == 1:
            tables[c] = t
    return dict(tables=tables, scripts=scripts, sprs=sorted(sprs), funcs=sorted(funcs))


# ------------------------------------------------------------------ owners: handler of an area -> animation scripts -> SPR table
# The "area dispatcher" (layer 2 type 7, MAIN0 table -> X0nn 0x80111AE0) calls one function per area in the order of its `selectedArea` tests.
AREA_DISPATCH = {0x80111AE0: {0: 0, 4: 1, 10: 2, 12: 1}}
# handlers that only wrap a shared state machine of MAIN0/X000: nothing to find in the overlay
_ovs = {}


def overlay(ovl, reader=None):
    if (ovl, id(reader)) not in _ovs:
        _ovs[(ovl, id(reader))] = Overlay(ovl, reader)
        if not ZERO_REGS:
            import capstone.mips as cm
            ZERO_REGS.add(cm.MIPS_REG_ZERO if hasattr(cm, "MIPS_REG_ZERO") else cm.MIPS_REG_0)
    return _ovs[(ovl, id(reader))]


def all_starts(ov):
    """Addresses of the overlay data referenced anywhere in its code that hold a script or a script table (cut points between tables)."""
    if not hasattr(ov, "_starts"):
        s = set()
        for a, _ in ov.funcs:
            if ov.inside(a, 4):
                for c in ov.scan(a)[0]:
                    if BASE <= c < BASE + len(ov.d) - 8 and not c & 3 and (script_at(ov, c) or table_at(ov, c)):
                        s.add(c)
        ov._starts = sorted(s)
    return ov._starts


def _next_start(ov, c):
    import bisect
    st = all_starts(ov)
    i = bisect.bisect_right(st, c)
    return st[i] if i < len(st) else 1 << 32


def handler_scripts(ov, addr, area):
    """(ordered scripts [(addr, table addr or None, index)], sprs) of one handler.  Tables are cut where another table starts."""
    entry = addr
    if addr in AREA_DISPATCH:
        calls = []
        for ins in _cs().disasm(ov.d[addr - BASE:addr - BASE + ov.size_of(addr)], addr):
            if ins.mnemonic == "jal":
                calls.append(ins.operands[0].imm)
        k = AREA_DISPATCH[addr].get(area)
        if k is None or k >= len(calls):
            return [], [], []
        entry = calls[k]
    consts, sprs, funcs = ov.reach(entry)
    w1e = ov.reach_w1e(funcs)
    got, seen = [], set()
    for c in sorted(x for x in consts if BASE <= x < BASE + len(ov.d) - 8 and not x & 3):
        t = table_at(ov, c)
        if t:
            lim = _next_start(ov, c)
            for i, p in enumerate(t):
                if c + 4 * i >= lim and i:
                    break
                if p not in seen:
                    seen.add(p)
                    got.append((p, c, i))
        elif c not in seen and script_at(ov, c):
            seen.add(c)
            got.append((c, None, 0))
    return got, sorted(sprs), w1e


# ------------------------------------------------------------------ SPR frames in memory
_tables = {}


def spr_table(ld, idx, kind):
    """bytes of SPR table `kind` in list idx of load list ld, or None."""
    for r, d in A.list_blocks(ld, idx):
        if (r["kind"] & 0xF0) == 0x30 and r["kind"] == kind and d and len(d) >= 4:
            return d
    return None


def table_frames(ld, idx, kind):
    d = spr_table(ld, idx, kind)
    return struct.unpack_from("<H", d, 2)[0] // 4 if d else 0


class SprFrames:
    """The frames of one SPR table rendered like tools/vram.py export_spr (same context/offset inference), kept as PIL images.
    frame(i) -> (RGBA image, (dx, dy)) with (dx, dy) = top-left of the image relative to the object position, or None for an empty frame."""

    def __init__(self, ld, idx, kind, bases=(), clut=None):
        import vram
        from PIL import Image
        self.Image = Image
        self.ld, self.idx, self.kind = ld, idx, kind
        d = spr_table(ld, idx, kind)
        self.frames_n = 0
        self.imgs = {}
        self.info = {}
        if d is None:
            return
        frames = vram.parse_spr(d)
        self.frames_n = len(frames)
        lst = A.load_lists()[ld][idx]
        link = next((t for t in vram.wpp_tables(lst) if len(t[2]) == len(frames)), None)
        if link is None and idx:      # the WPP of the area-wide list
            link = next((t for t in vram.wpp_tables(A.load_lists()[ld][0]) if len(t[2]) == len(frames)), None)
        planes, alts = None, ()
        if link:
            v, base = vram.ctx_get(ld, idx), 0
            planes = [vram.wpp_plane(o) for o in link[2]]
            self.info = {"wpp": "%s table %d" % (link[0], link[1])}
        else:
            import hashlib
            parts = [p for f in frames for p in f if p[8] and p[9]]
            if (kind, hashlib.md5(d).hexdigest()) in vram.shared_tables():
                cs = vram.shared_candidates(ld)
            else:
                cs = vram.candidates(ld, idx)
            res = vram.infer_base(cs, parts) if parts else None
            if parts and bases:
                # TObj.w1e as the handler really sets it (tools/objinit.py): only the context is searched.  A part page the handler names may hold
                # 4 bpp art although the load lists pair it with a 256 colour CLUT (AREA03 horned enemy, w1e 11), so any loaded data counts.  Among equal fits the
                # list's own pages (context 0) win over the SYS atlas lists, which can hold other art in the same VRAM area (AREA03 thrower).
                ps = vram.sample_parts(parts, 150)
                best = max(((round(vram.score_candidate(cv, ps, b, True), 2), ci == 0, res is not None and res[0] == ci, -ci, b) for ci, (_, cv) in enumerate(cs) for b in bases), default=None)
                if best and best[0] >= 0.25:
                    res = (-best[3], best[4], dict(res[2] if res else dict(ties=[[0, best[2]]]), alt_offsets=[b for b in bases if b != best[4]]))
            if res is None:
                self.info = {"error": "no loaded page data covers the part rectangles"}
                return
            ci, base, inf = res
            v, alts = cs[ci][1], inf.get("alt_offsets", ())
            self.info = {"sys_list": cs[ci][0][2], "tpage_offset": base, "ambiguous": len(inf["ties"]) > 1}
        for fi, parts in enumerate(frames):
            items = []
            fbase = base
            if planes is None and alts:
                live = [p for p in parts if p[8] and p[9]]
                if live and not all(vram.covered(v, p, base, True) for p in live):
                    fbase = next((a for a in alts if all(vram.covered(v, p, a, True) for p in live)), base)
            for p in parts:
                if p[8] == 0 or p[9] == 0:
                    continue
                if clut is not None:        # o->b0d & 1: the object's CLUT replaces the one of the part
                    p = (p[0], p[1], clut) + tuple(p[3:])
                w, h, png, rgba, blank, note = vram.render_part(v, p, fbase, planes[fi] if planes else None)
                if not blank:
                    items.append((p[12], p[13], w, h, rgba))
            if not items:
                continue
            ox, oy, W, H, rows = vram.compose_rgba(items)
            im = Image.frombytes("RGBA", (W, H), b"".join(rows))
            self.imgs[fi] = (im, (-ox, -oy))

    def frame(self, i):
        return self.imgs.get(i)


_sprc = {}


def spr_frames(ld, idx, kind, bases=(), clut=None):
    k = (ld, idx, kind, tuple(bases), clut)
    if k not in _sprc:
        _sprc[k] = SprFrames(ld, idx, kind, bases, clut)
    return _sprc[k]


def pick_source(ld, rooms, kinds, frames_per_script):
    """Choose (list idx, kind, alternatives) of the SPR table that draws the scripts: for each kind the handler loads, the table of the
    object's room (the room with most placements first, else the area list); the one in which most scripts have every frame inside the
    table wins.  rooms = {list idx: placements}, frames_per_script = [[frame, ...], ...]."""
    L = A.load_lists()[ld]
    order = sorted(rooms, key=lambda r: -rooms[r]) + [0]
    scored = []
    for kind in kinds:
        for idx in order:
            if idx < len(L) and spr_table(ld, idx, kind) is not None:
                n = struct.unpack_from("<H", spr_table(ld, idx, kind), 2)[0] // 4
                scored.append((sum(1 for f in frames_per_script if f and max(f) < n), idx, kind))
                break
    scored.sort(key=lambda c: -c[0])
    if not scored or scored[0][0] == 0:
        return None, None, []
    return scored[0][1], scored[0][2], [(i, k) for _, i, k in scored[1:]]


def handler_label(hname, addr):
    return hname.split(">")[-1] if hname.split(">")[-1][:1].isalpha() else "func_%08X" % addr


def owner_id(area, hname):
    import re
    n = hname
    return "a%02d_%s" % (area, re.sub(r"[^a-z0-9]+", "_", n.lower()).strip("_"))


def emulated_init(area, handler, rows, reader=None):
    """[dict(w1e, clut, slot, b0d)] (most placed first) from running the handler on its placement records; [] when it sets no SPR bank.
    Records that differ only in position give the same answer, so each distinct one is run once."""
    import objinit
    seen = {}
    for r in rows:
        raw = bytes.fromhex(r["raw"])
        seen.setdefault(raw[:6] + raw[12:], raw)
    try:
        return [i for i, _ in objinit.owner_inits(area, handler, list(seen.values()), reader)]
    except Exception:
        return []


def build_owners(area=None, reader=None, rows=None):
    """[owner dict] for every placed object handler (per area) that selects animation scripts.  No rendering here."""
    rows = rows if rows is not None else O.build(area)
    groups = collections.OrderedDict()
    for r in rows:
        if not r.get("handler"):
            continue
        if area is not None and r["area"] != area:
            continue
        g = groups.setdefault((r["area"], int(r["handler"], 16)), dict(rows=[], name=r["handler_name"]))
        g["rows"].append(r)
    out = []
    for (a, h), g in sorted(groups.items()):
        ov = overlay(O.overlay_of(a), reader)
        if not ov.inside(h, 4):
            continue
        scripts, sprs, w1e = handler_scripts(ov, h, a)
        init = emulated_init(a, h, g["rows"], reader)
        if init:    # the values the handler really sets (tools/objinit.py) replace the guess read from its immediates
            w1e, sprs = [i["w1e"] for i in init if i["w1e"] < 32], sorted({0x30 + i["slot"] for i in init})
        if not scripts:
            continue
        anims = []
        for n, (addr, tab, idx) in enumerate(scripts):
            s = script_at(ov, addr)
            ents = s["entries"]
            anims.append(dict(id=n, name="anim_%02d" % n, start="%08X" % addr, table=None if tab is None else "%08X" % tab, index=idx,
                              entries=ents, loop=s["loop"], end=s["end"], signal=s["signal"], ticks=sum(e["dur"] for e in ents),
                              frames=[e["frame"] for e in ents]))
        rooms = collections.Counter(r["list"] for r in g["rows"])
        types = sorted({(r["layer"], r["type"]) for r in g["rows"]})
        out.append(dict(owner=owner_id(a, handler_label(g["name"], h)), handler=handler_label(g["name"], h), handler_addr="%08X" % h, area=a,
                        types=["L%d/%02X" % t for t in types], placements=len(g["rows"]), rooms=dict(rooms), sprs=sprs, w1e=w1e, init=init, animations=anims))
    return out


def resolve_sources(owners):
    """Attach `spr` = {list, kind, ...} to every owner (the SPR table that draws its frames)."""
    for ow in owners:
        idx, kind, alts = pick_source("AR%02d" % ow["area"], {int(k): v for k, v in ow["rooms"].items()}, ow["sprs"],
                                      [a["frames"] for a in ow["animations"]])
        ow["spr"] = None if idx is None else dict(list=idx, kind="0x%02X" % kind, w1e=ow["w1e"], clut=next((i["clut"] for i in ow.get("init", ()) if i["slot"] == kind - 0x30 and i["clut"] is not None), None), alternatives=["list %d kind 0x%02X" % x for x in alts])
    return owners
