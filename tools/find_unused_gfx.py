#!/usr/bin/env python3
"""Cut-content hunt: SPR sprite frames, VRAM texture regions and PAL/JP differences that no game code or data reaches.

Static analysis only (no emulation, no port capture):

 1. "Used" frames of every SPR table (load kind 0x3n) of every area load list:
    - strong: the frames of every animation script that a placed object's handler reaches (tools/animobj.py: handler + callees,
      constants that point at scripts / script tables, SPR slot from the code or tools/objinit.py), applied to every table of that
      SPR slot in the area (any room list), plus Tombi's own scripts (tools/anim.py) for slot 0;
    - weak: every 8-byte animation script found ANYWHERE in the area overlay or in MAIN0 (brute scan, every aligned address that
      decodes as a valid script, tools/animobj.script_at), whatever references it.  This over-approximates on purpose: data that
      merely looks like a script also marks frames used, so the candidates are only frames nothing at all can point at.
    A frame not in strong or weak is a candidate.  certainty "high": the table has strong coverage, the frame is not empty, and it
    is not next to (<= 3 frames after) a used frame, since `frame = base + counter` code (frames drawn without a script) would land there;
    "medium": the rest (inside runs, or tables only the weak scan reaches).  Tables no script reaches at all are listed as
    "sin_scripts" (drawn by code with computed frame numbers or not at all): never candidates, too uncertain.
 2. VRAM: in the 4 bpp texture pages that an area / room list loads (GRPX, not SYS), texels that no SPR part of any table of the
    area (every tpage offset tried, plus the w1e offsets the code sets) and no model polygon (room + object WMD, UV boxes) samples.
    Connected regions of non-zero texels >= 12x12 are reported ("medium" at best: code-drawn primitives with hard-coded UVs,
    e.g. particles, layer-7 backdrops or the HUD, are not modelled).
 3. PAL vs JP: SPR tables are found by shape in every (unpacked) file of both discs; frames whose 16-byte part records occur in only
    one release are listed; the JP-only frames of a table whose other frames match a PAL table are rendered with that PAL table's
    VRAM context (palette = the CLUT word of the part, as read in the PAL VRAM of that area: marked guessed).

  python3 tools/find_unused_gfx.py OUT_DIR [--jp game_jp/disc/tomba_jp.bin] [--areas 0,1,...] [--no-render]
OUT_DIR gets cut_<area>_*.png contact sheets, INDICE.txt (Spanish) and candidates.json.
"""
import argparse, collections, hashlib, json, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import area000 as A
import animobj as M

AREA_NAMES = {0: "Pueblo de Charles / inicio", 1: "Bosque/aldea", 2: "Pueblo de los enanos", 3: "Mansion del Cerdo / Ciudad Fantasma",
              4: "Bosque Encantado", 5: "Montana de Fuego", 6: "Lago / Dwarf forest", 7: "Area 7", 8: "Area 8", 9: "Pais de la Civilizacion?",
              }


# ------------------------------------------------------------------ script scanning
class Blob:
    """Minimal overlay-like view (u16/u32/inside) over a memory image for animobj.script_at."""

    def __init__(self, d, base):
        self.d, self.base = d, base

    def inside(self, a, n=1):
        return self.base <= a and a - self.base + n <= len(self.d)

    def u16(self, a):
        return struct.unpack_from("<H", self.d, a - self.base)[0]

    def u32(self, a):
        return struct.unpack_from("<I", self.d, a - self.base)[0]


def main0_blob():
    d = open(os.path.join(A.FS, "EXE", "MAIN0.EXE"), "rb").read()
    taddr = struct.unpack_from("<I", d, 0x18)[0]
    return Blob(d[0x800:], taddr)


def referenced_frames(blob, skip=frozenset()):
    """{frame} of every script that something points at: a constant the code builds (lui + addiu/ori, or lui + load/store
    offset; tracked linearly over the whole blob, never reset: over-approximates) or an aligned data word holding its address;
    pointer arrays of scripts (tables) are followed, scripts are followed through their jumps (animobj.script_at)."""
    d, base = blob.d, blob.base
    n = len(d) & ~3
    words = struct.unpack_from("<%dI" % (n // 4), d)
    refs = set()
    hi = {}
    for i, w in enumerate(words):
        op = w >> 26
        rs, rt, imm = w >> 21 & 31, w >> 16 & 31, w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0x0F:                                   # lui
            hi[rt] = imm << 16
        elif op in (0x09, 0x08) and rs in hi:            # addiu / addi
            refs.add((hi[rs] + simm) & 0xFFFFFFFF)
        elif op == 0x0D and rs in hi:                    # ori
            refs.add(hi[rs] | imm)
        elif op in (0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B) and rs in hi:   # loads / stores
            refs.add((hi[rs] + simm) & 0xFFFFFFFF)
        if base <= w < base + n:
            refs.add(w)
    out = set()
    done = set()
    for c in sorted(refs):
        if c & 3 or not blob.inside(c, 8):
            continue
        todo = [c]
        k = c
        while blob.inside(k, 4) and k < c + 4 * 64:      # pointer array of scripts
            p = blob.u32(k)
            if not blob.inside(p, 8) or p & 3 or M.script_at(blob, p) is None:
                break
            todo.append(p)
            k += 4
        for a in todo:
            if a in done:
                continue
            done.add(a)
            if a in skip:
                continue
            s = M.script_at(blob, a)
            if s:
                out.update(e["frame"] for e in s["entries"])
    return out


def tombi_frames():
    import anim
    try:
        out = set()
        for a in anim.list_anims() if hasattr(anim, "list_anims") else []:
            out.update(a.get("frames", []))
        return out
    except Exception:
        return set()


# ------------------------------------------------------------------ SPR tables
def area_tables(area):
    """{md5: dict(kind, lists=[idx], d=bytes)} of the SPR tables of the area load lists."""
    ld = "AR%02d" % area
    out = collections.OrderedDict()
    for idx in range(len(A.load_lists().get(ld, []))):
        src = None
        for r, d in A.list_blocks(ld, idx):
            if r["type"] != 0xFFFFFFFF:
                src = r["name"]
            if (r["kind"] & 0xF0) == 0x30 and d and len(d) >= 4:
                h = hashlib.md5(d).hexdigest()
                t = out.setdefault((r["kind"], h), dict(kind=r["kind"], lists=[], d=d, file=src))
                t["lists"].append(idx)
    return out


def frame_empty(fr):
    return not any(p[8] and p[9] for p in fr)


SHARED_KINDS = (0x34, 0x39, 0x3F, 0x31)   # same table in every area list 0, drawn by MAIN0 code (items, effects, HUD)


CODE_INDEXED = {0x3B}   # town / house-interior tables (CLM 8E/8F lists): frame = house style index, chosen by the INFO overlay code


def analyse(owners, areas, log=print):
    """Per SPR table of every area: the candidate frames (see the module doc)."""
    import vram, anim
    mb = main0_blob()
    tombi = {e["frame"] for an in anim.catalog() for e in an["entries"]}
    known = collections.defaultdict(set)
    for o in owners:
        for an in o["animations"]:
            known[o["area"]].update(e["addr"] for e in an["entries"])
    log("scanning MAIN0 / overlays for scripts ...")
    orphan = {"main": referenced_frames(mb)}
    for ar in areas:
        orphan[ar] = referenced_frames(Blob(M.overlay(M.O.overlay_of(ar)).d, M.BASE), frozenset(known[ar]))
    all_orphan = set().union(*orphan.values())
    res, stats, seen_tombi = [], collections.Counter(), {}
    for area in areas:
        ld = "AR%02d" % area
        if ld not in A.load_lists():
            continue
        strong, who = collections.defaultdict(set), collections.defaultdict(list)
        for o in owners:
            if o["area"] != area:
                continue
            kinds = {int(o["spr"]["kind"], 16)} if o.get("spr") else set()
            kinds |= set(o.get("sprs") or [])
            for k in kinds:
                for an in o["animations"]:
                    strong[k].update(an["frames"])
                who[k].append(o["owner"])
        loc_orphan = orphan[area] | orphan["main"]
        for (kind, h), t in area_tables(area).items():
            frames = vram.parse_spr(t["d"])
            n = len(frames)
            stats["tables"] += 1
            stats["frames"] += n
            base = dict(area=area, kind=kind, lists=t["lists"], file=t["file"], frames=n, md5=h, owners=sorted(set(who.get(kind, []))))
            if kind == 0x30:      # Tombi: MAIN0's 90 scripts (tools/anim.py) + any script of any overlay / MAIN0
                st, wk = {f for f in tombi if f < n}, {f for f in all_orphan | set().union(*strong.values()) if f < n}
                if h in seen_tombi:
                    continue
                seen_tombi[h] = area
            elif kind in CODE_INDEXED or kind in SHARED_KINDS:
                stats["tables_code_indexed"] += 1
                res.append(dict(base, status="code_indexed"))
                continue
            else:
                st, wk = {f for f in strong.get(kind, ()) if f < n}, {f for f in loc_orphan if f < n}
            if not st:
                stats["tables_sin_scripts"] += 1
                res.append(dict(base, status="sin_scripts"))
                continue
            cov = len(st) / max(1, n)
            cands = []
            for f in range(n):
                if f in st or frame_empty(frames[f]):
                    continue
                near = any((f - k) in st for k in range(1, 4))
                if f not in wk and not near:
                    cert = "high"
                elif f not in wk or (cov >= 0.5 and not near):
                    cert = "medium"
                else:
                    stats["descartados_quiza_usados"] += 1
                    continue
                cands.append(dict(frame=f, certainty=cert, parts=len(frames[f]), near_used=near, in_orphan_script=f in wk))
            stats["candidates"] += len(cands)
            stats["high"] += sum(1 for c in cands if c["certainty"] == "high")
            res.append(dict(base, status="ok", strong=len(st), coverage=round(cov, 2), candidates=cands))
            if cands:
                log("  %s kind %02X lists %s %s: %d frames, script frames %d, candidates %d (high %d)" % (
                    ld, kind, t["lists"], t["file"], n, len(st), len(cands), sum(1 for c in cands if c["certainty"] == "high")))
    return res, stats


# ------------------------------------------------------------------ VRAM regions
def vram_unused(area, owners, log=print):
    """[(list idx, page record name, x0, y0, w, h texels in VRAM nibble space, nonzero texel count)]"""
    import numpy as np
    import vram, room as R
    ld = "AR%02d" % area
    L = A.load_lists().get(ld, [])
    w1es = collections.defaultdict(set)      # SPR kind -> tpage offsets (TObj.w1e) the area code sets for objects drawn from it
    for o in owners:
        if o["area"] == area:
            for k in set(o.get("sprs") or []) | ({int(o["spr"]["kind"], 16)} if o.get("spr") else set()):
                w1es[k] |= {w for w in (o.get("w1e") or []) if w < 32}
    out = []
    for idx, lst in enumerate(L):
        pages = [r for r in lst if vram.is_grpx(r) and not vram.is_clut_rec(r) and vram.page_bpp(r["name"]) == 4]
        if not pages:
            continue
        v = vram.ctx_get(ld, idx)
        cover = np.zeros((512, 4096), bool)
        pclut = {}
        tabs = [(r, d) for i in {0, idx} for r, d in A.list_blocks(ld, i) if (r["kind"] & 0xF0) == 0x30 and d and len(d) >= 4]
        cands = vram.candidates(ld, idx)
        wpp_n = {len(t[2]) for i in {0, idx} for t in vram.wpp_tables(L[i])}
        for r, d in tabs:
            frs = vram.parse_spr(d)
            if len(frs) in wpp_n:      # parts cut from streamed WPP pictures, not from the pages
                continue
            offs = set(w1es.get(r["kind"], ())) or {0}
            live = [p for f in frs for p in f if p[8] and p[9]]
            try:
                ib = vram.infer_base(cands, live) if live else None
                if ib:
                    offs |= {ib[1]} | {b for _c, b in ib[2].get("ties", [])}
            except Exception:
                pass
            for f in frs:
                for p in f:
                    u0, v0, clut, u1, v1, tpage, u2, v2, w, h = p[:10]
                    if not (w and h):
                        continue
                    for off in offs:
                        tp = (tpage + off) & 0xFFFF
                        px, py = (tp & 15) * 64 * 4, (tp >> 4 & 1) * 256
                        if not (tp >> 7 & 3):
                            pclut.setdefault((px // 256, py // 256), collections.Counter())[clut] += 1
                        mode = tp >> 7 & 3
                        uu, vv = min(u0, u1), min(v0, v2)
                        if mode == 0:
                            cover[py + vv:py + vv + h, px + uu:px + uu + w] = True
                        elif mode == 1:
                            cover[py + vv:py + vv + h, px + 2 * uu:px + 2 * (uu + w)] = True
        # models
        try:
            for mr, b in [(r, d) for i in {0, idx} for r, d in A.list_blocks(ld, i) if r["kind"] in (0x50, 0x51) and d]:
                kind, pks = mr["kind"], []
                if kind == 0x50:
                    try:
                        pks = R.parse_wmd_ex(b, lenient=True)
                    except Exception:
                        pks = []
                else:
                    n = struct.unpack_from("<I", b)[0]
                    if 0 < n < 1000 and len(b) >= 4 + 4 * n:
                        o = list(struct.unpack_from("<%dI" % n, b, 4)) + [len(b)]
                        for i in range(n):
                            if o[i] < o[i + 1] <= len(b):
                                try:
                                    pks += R.parse_wmd_ex(struct.pack("<II", 1, 8) + b[o[i]:o[i + 1]], lenient=True)
                                except Exception:
                                    pass
                for cl, tp, _c, vs, _cols in pks:
                    us, ws = [q[3] for q in vs], [q[4] for q in vs]
                    px, py, mode = (tp & 15) * 256, (tp >> 4 & 1) * 256, tp >> 7 & 3
                    sc = 1 if mode == 0 else 2
                    cover[py + min(ws):py + max(ws) + 1, px + sc * min(us):px + sc * (max(us) + 1)] = True
        except Exception as e:
            log("  models %s:%d: %r" % (ld, idx, e))
        nib = np.frombuffer(v.nib, np.uint8).reshape(512, 4096)
        for r in pages:
            x, y, w, h = r["arg"] & 0xFFFF, r["arg"] >> 16, r["w"], r["h"]
            sub = (nib[y:y + h, 4 * x:4 * (x + w)] != 0) & ~cover[y:y + h, 4 * x:4 * (x + w)]
            if sub.sum() < 100:
                continue
            for (y0, x0, hh, ww, cnt) in components(sub):
                if hh >= 12 and ww >= 12 and cnt >= 300:
                    X, Y = 4 * x + int(x0), y + int(y0)
                    c = pclut.get((X // 256, Y // 256))
                    clut = c.most_common(1)[0][0] if c else None
                    crop = nib[Y:Y + int(hh), X:X + int(ww)].copy()
                    out.append(dict(list=idx, page=r["name"], x=X, y=Y, w=int(ww), h=int(hh), texels=int(cnt), clut=clut, crop=crop,
                                    pal=(v.palette(clut, 16)[2] if clut is not None else None)))
    return out


def components(mask):
    """Bounding boxes (y, x, h, w, count) of 8-connected regions of mask (dilated by 2 so nearby pixels group)."""
    import numpy as np
    try:
        from scipy import ndimage
        lab, n = ndimage.label(ndimage.binary_dilation(mask, iterations=2), structure=np.ones((3, 3)))
        out = []
        for i, sl in enumerate(ndimage.find_objects(lab), 1):
            if sl is None:
                continue
            m = mask[sl] & (lab[sl] == i)
            out.append((sl[0].start, sl[1].start, sl[0].stop - sl[0].start, sl[1].stop - sl[1].start, int(m.sum())))
        return out
    except ImportError:
        pass
    H, W = mask.shape
    seen = np.zeros_like(mask)
    out = []
    ys, xs = np.nonzero(mask)
    for sy, sx in zip(ys, xs):
        if seen[sy, sx]:
            continue
        st, cnt, y0, y1, x0, x1 = [(sy, sx)], 0, sy, sy, sx, sx
        seen[sy, sx] = True
        while st:
            y, x = st.pop()
            cnt += 1
            y0, y1, x0, x1 = min(y0, y), max(y1, y), min(x0, x), max(x1, x)
            for dy in range(-2, 3):
                for dx in range(-2, 3):
                    yy, xx = y + dy, x + dx
                    if 0 <= yy < H and 0 <= xx < W and mask[yy, xx] and not seen[yy, xx]:
                        seen[yy, xx] = True
                        st.append((yy, xx))
        out.append((y0, x0, y1 - y0 + 1, x1 - x0 + 1, cnt))
    return out


# ------------------------------------------------------------------ PAL vs JP
def find_spr_tables(d, min_frames=4):
    """[(offset, nframes, [frame part bytes])] of byte ranges of d shaped like an SPR table."""
    out = []
    n = len(d)
    off = 0
    while off + 8 <= n:
        cnt0, o0 = struct.unpack_from("<HH", d, off)
        if o0 % 4 or o0 < 4 * min_frames or o0 > 4 * 1200 or off + o0 > n or cnt0 > 64:
            off += 4
            continue
        nf = o0 // 4
        ok, pos, fr = True, o0, []
        for f in range(nf):
            c, o = struct.unpack_from("<HH", d, off + 4 * f)
            if o != pos or c > 64 or off + o + 16 * c > n:
                ok = False
                break
            parts = d[off + o:off + o + 16 * c]
            for k in range(c):
                p = parts[16 * k:16 * k + 16]
                if p[13] == 0 and p[12] == 0 and c:      # w,h zero is allowed (hidden parts) but rare in all parts
                    pass
            fr.append(bytes(parts))
            pos = o + 16 * c
        if ok and sum(1 for x in fr if x) >= min_frames // 2 and pos <= 0xFFFF:
            # parts sanity: u/v sizes within 256
            if all(all(fr_[16 * k + 8] <= 256 for k in range(len(fr_) // 16)) for fr_ in fr):
                out.append((off, nf, fr))
                off += (pos + 3) & ~3
                continue
        off += 4
    return out


def disc_files(img):
    import jp_text, gam_py
    files = jp_text.read_disc(img)
    out = {}
    for k, v in files.items():
        if k.endswith(".GAM"):
            try:
                v = gam_py.unpack(v)
            except Exception:
                try:
                    v = A.gam_unpack(v)
                except Exception:
                    continue
        out[k] = v
    return out


def pal_files():
    out = {}
    for dp, _dn, fns in os.walk(A.FS):
        for fn in fns:
            if fn.endswith((".STR", ".WVD", ".SEQ", ".WSS", ".XA", ".WFM")):
                continue
            nm = os.path.relpath(os.path.join(dp, fn), A.FS).replace(os.sep, "/")
            try:
                out[nm] = A.read_resource(nm) if nm.endswith((".GAM", ".000")) else open(os.path.join(dp, fn), "rb").read()
            except Exception:
                pass
    return out


def frame_key(b):
    """part records without the CLUT/tpage words (those move between releases with the VRAM layout)."""
    return hashlib.md5(b"".join(b[16 * k:16 * k + 2] + b[16 * k + 4:16 * k + 6] + b[16 * k + 8:16 * k + 10] + b[16 * k + 12:16 * k + 16]
                                for k in range(len(b) // 16))).hexdigest()


def compare_jp(jp_img, log=print):
    log("reading PAL files ...")
    pal = pal_files()
    log("reading JP disc ...")
    jp = disc_files(jp_img)
    res = dict(pal_tables=[], jp_tables=[])
    for name, files, key in (("pal", pal, "pal_tables"), ("jp", jp, "jp_tables")):
        for fn, d in files.items():
            if not d or fn.endswith((".EXE", ".BIN")) and "INFO" not in fn and "DSPSUB" not in fn:
                continue
            for off, nf, fr in find_spr_tables(d):
                res[key].append(dict(file=fn, off=off, frames=nf, keys=[frame_key(b) if b else None for b in fr]))
        log("  %s: %d SPR-shaped tables" % (name, len(res[key])))
    pk = {k for t in res["pal_tables"] for k in t["keys"] if k}
    jk = {k for t in res["jp_tables"] for k in t["keys"] if k}
    out = []
    for side, tabs, other in (("jp_only", res["jp_tables"], pk), ("pal_only", res["pal_tables"], jk)):
        for t in tabs:
            miss = [i for i, k in enumerate(t["keys"]) if k and k not in other]
            shared = sum(1 for k in t["keys"] if k and k in other)
            if miss:
                out.append(dict(side=side, file=t["file"], off=t["off"], frames=t["frames"], missing=miss, shared=shared))
    # WPP / whole file content: decoded WPP frames
    return out, res


def parse_spr_bytes(d, off, n):
    out = []
    for f in range(n):
        c, o = struct.unpack_from("<HH", d, off + 4 * f)
        out.append([struct.unpack_from("<BBHBBHBBBBBBbb", d, off + o + 16 * k) for k in range(c)])
    return out


def compose(v, parts, plane=None, base=0):
    import vram
    from PIL import Image
    items = []
    for p in parts:
        if not (p[8] and p[9]):
            continue
        try:
            w, h, _png, rgba, blank, _n = vram.render_part(v, p, base, plane)
        except Exception:
            continue
        if not blank:
            items.append((p[12], p[13], w, h, rgba))
    if not items:
        return None
    ox, oy, W, H, rows = vram.compose_rgba(items)
    return Image.frombytes("RGBA", (W, H), b"".join(rows))


def pal_spr_index():
    """[(ld, idx, kind, frames(parsed), keys, wpp planes or None)] of every distinct SPR table of the PAL load lists."""
    import vram
    out, seen = [], set()
    for ld, lists in A.load_lists().items():
        if not ld.startswith("AR"):
            continue
        for idx, lst in enumerate(lists):
            for r, d in A.list_blocks(ld, idx):
                if (r["kind"] & 0xF0) != 0x30 or not d or len(d) < 4:
                    continue
                h = hashlib.md5(d).hexdigest()
                if h in seen:
                    continue
                seen.add(h)
                fr = vram.parse_spr(d)
                keys = []
                for f in range(len(fr)):
                    c, o = struct.unpack_from("<HH", d, 4 * f)
                    keys.append(frame_key(d[o:o + 16 * c]) if c else None)
                link = next((t for t in vram.wpp_tables(lst) if len(t[2]) == len(fr)), None)
                if link is None and idx:
                    link = next((t for t in vram.wpp_tables(lists[0]) if len(t[2]) == len(fr)), None)
                out.append((ld, idx, r["kind"], fr, keys, [vram.wpp_plane(o) for o in link[2]] if link else None))
    return out


def jp_wpp_tables(jp):
    """{frame count: [(file, [decoded frame])]} of the JP WPP containers (.000 files, both tables when there are two)."""
    import wpp
    out = collections.defaultdict(list)
    for fn, d in jp.items():
        if not fn.endswith(".000"):
            continue
        t = A.parse_table(d)
        if not t:
            continue
        b = t[2]
        try:
            fr = [wpp.wpp_decode(d[b[i]:b[i + 1]]) for i in range(len(b) - 1)]
            if all(len(x) >= 4 and len(x) >= 4 + 2 * struct.unpack_from("<HH", x)[0] * struct.unpack_from("<HH", x)[1] for x in fr):
                out[len(fr)].append((fn, fr))
        except Exception:
            pass
        end = b[-1]
        if end < len(d) - 8:       # second table (A004.000 style)
            t2 = A.parse_table(d[end:])
            if t2:
                b2 = t2[2]
                try:
                    fr = [wpp.wpp_decode(d[end + b2[i]:end + b2[i + 1]]) for i in range(len(b2) - 1)]
                    out[len(fr)].append((fn + "#2", fr))
                except Exception:
                    pass
    return out


def render_jp(out_dir, jp_img, log=print):
    """JP-only / PAL-only frames: index entries + contact sheets jp_*.png."""
    import vram
    log("JP: reading discs ...")
    jp = disc_files(jp_img)
    pal_idx = pal_spr_index()
    pal_keys = {k for t in pal_idx for k in t[4] if k}
    jwpp = jp_wpp_tables(jp)
    entries, seen = [], set()
    jtabs = []
    for fn, d in sorted(jp.items()):
        if not d or fn.endswith((".EXE", ".BIN")) or fn == "SLPS_011.44":
            continue
        for off, nf, fr in find_spr_tables(d):
            keys = tuple(frame_key(b) if b else None for b in fr)
            if keys in seen:
                continue
            seen.add(keys)
            jtabs.append((fn, off, nf, keys, d))
    jp_keys = {k for t in jtabs for k in t[3] if k}
    for fn, off, nf, keys, d in jtabs:
        miss = [i for i, k in enumerate(keys) if k and k not in pal_keys]
        if not miss:
            continue
        share = sum(1 for k in keys if k and k in pal_keys)
        best = max(pal_idx, key=lambda t: sum(1 for k in t[4] if k and k in set(keys)))
        bshare = sum(1 for k in best[4] if k and k in set(keys))
        if bshare < 0.5 * sum(1 for k in keys if k):
            entries.append(dict(side="jp_only", file=fn, off=off, frames=nf, missing=miss, matched=None,
                                note="tabla sin equivalente PAL (texto/HUD localizado u otra disposicion): no se renderiza"))
            continue
        ld, idx, kind, pfr, pkeys, planes = best
        v = vram.ctx_get(ld, idx)
        frames = parse_spr_bytes(d, off, nf)
        jplanes = None
        if planes is not None:
            cand = jwpp.get(nf, [])
            same_dir = [c for c in cand if c[0].split("/")[0] == fn.split("/")[0]]
            if same_dir or cand:
                jplanes = [vram.wpp_plane(o) for o in (same_dir or cand)[0][1]]
        import numpy as np
        pal_ims = {}
        for f in range(len(pfr)):
            im = compose(v, pfr[f], planes[f] if planes and f < len(planes) else None, 0)
            if im is not None:
                pal_ims.setdefault(im.size, []).append((f, np.asarray(im)))
        items, real_new, near = [], [], {}
        for f in miss:
            if planes is not None and jplanes is None:
                im = None
            elif jplanes is not None:
                im = compose(v, frames[f], jplanes[f] if f < len(jplanes) else None)
            else:
                im = compose(v, frames[f], None, 0)
            if im is None:
                continue
            a = np.asarray(im)
            best = min(((np.abs(b.astype(int) - a).sum(axis=2) > 0).mean(), g) for g, b in pal_ims.get(im.size, [])) if im.size in pal_ims else (1.0, None)
            if best[0] < 0.05:
                near[f] = (best[1], round(float(best[0]) * 100, 1))
                continue
            real_new.append(f)
            items.append(("JP f%d%s" % (f, "" if best[1] is None else " ~P%d %d%%" % (best[1], best[0] * 100)), im))
        e = dict(side="jp_only", file=fn, off=off, frames=nf, missing=miss, new_images=real_new, near_pal=near, matched="%s list %d SPR %02X (%d frames)" % (ld, idx, kind, len(pfr)),
                 wpp=jplanes is not None, palette=("WPP JP + CLUT del part leida en la VRAM PAL del area equivalente (probable)" if jplanes is not None
                                                  else "texeles y paleta de la VRAM PAL equivalente: ADIVINADO (la VRAM JP puede diferir)"))
        if items:
            name = "jp_%s_%s_off%d.png" % (fn.split("/")[0], fn.split("/")[-1].split(".")[0], off)
            e["sheet"] = name
            render_sheet(os.path.join(out_dir, name), items, "JP %s @%d (%d fr) ~ PAL %s" % (fn, off, nf, e["matched"]))
        entries.append(e)
        log("  JP %s @%d: %d frames, %d not in PAL (%d new images) ~ %s" % (fn, off, nf, len(miss), len(real_new), e["matched"]))
    # PAL-only frames of PAL tables whose JP twin exists (frames the JP release does not have)
    for ld, idx, kind, pfr, pkeys, planes in pal_idx:
        miss = [i for i, k in enumerate(pkeys) if k and k not in jp_keys]
        share = sum(1 for k in pkeys if k and k in jp_keys)
        if miss and share >= 0.5 * sum(1 for k in pkeys if k):
            entries.append(dict(side="pal_only", file="%s list %d SPR %02X" % (ld, idx, kind), frames=len(pfr), missing=miss))
    return entries


# ------------------------------------------------------------------ rendering
def render_sheet(path, items, title, cols=8, cell=96):
    """items [(label, PIL RGBA image or None)] -> one PNG contact sheet."""
    from PIL import Image, ImageDraw
    rows = (len(items) + cols - 1) // cols
    W, H = cols * cell, rows * (cell + 12) + 14
    sheet = Image.new("RGB", (W, H), (48, 48, 56))
    dr = ImageDraw.Draw(sheet)
    dr.text((4, 1), title[:W // 6], fill=(255, 255, 200))
    for i, (lab, im) in enumerate(items):
        x, y = i % cols * cell, 14 + i // cols * (cell + 12)
        dr.rectangle([x + 1, y + 1, x + cell - 2, y + cell - 2], fill=(96, 96, 108))
        if im is not None:
            im = im.copy()
            sc = min((cell - 4) / max(1, im.width), (cell - 4) / max(1, im.height), 2.0)
            if sc != 1:
                im = im.resize((max(1, int(im.width * sc)), max(1, int(im.height * sc))), Image.NEAREST)
            sheet.paste(im, (x + (cell - im.width) // 2, y + (cell - im.height) // 2), im)
        dr.text((x + 2, y + cell), lab[:cell // 6], fill=(230, 230, 230))
    sheet.save(path, optimize=True)


def render_spr(out, res, owners, log=print):
    """cut_AREAnn_spr<kind>_list<idx>.png per table with candidates (+ up to 4 used frames of the same table as reference)."""
    sheets = []
    for t in res:
        if not t.get("candidates"):
            continue
        area, kind, idx = t["area"], t["kind"], t["lists"][0]
        inits = [i for o in owners if o["area"] == area for i in (o.get("init") or []) if 0x30 + i["slot"] == kind]
        bases = sorted({i["w1e"] for i in inits if i["w1e"] is not None and i["w1e"] < 32}) or sorted(
            {w for o in owners if o["area"] == area and kind in (o.get("sprs") or []) for w in (o.get("w1e") or [])})
        cl = {i.get("clut") if i.get("b0d", 0) & 1 else None for i in inits}
        clut = cl.pop() if len(cl) == 1 else None      # the object's CLUT replaces the parts' only when every placement does so
        try:
            sf = M.spr_frames("AR%02d" % area, idx, kind, tuple(bases), clut)
        except Exception as e:
            log("render %d %02X: %r" % (area, kind, e))
            continue
        info = sf.info
        t["palette"] = ("objeto (init, CLUT %d)" % clut) if clut is not None else (
            "WPP: CLUT del part (exacta)" if "wpp" in info else
            "CLUT del part; pagina inferida%s" % (" (AMBIGUA: paleta/pagina adivinada)" if info.get("ambiguous") else ""))
        t["render_info"] = info
        st_frames = [f for f in range(t["frames"]) if f not in {c["frame"] for c in t["candidates"]}]
        refs = [f for f in st_frames if sf.frame(f)][:: max(1, len(st_frames) // 4)][:4]
        items = [("REF f%d (usado)" % f, sf.frame(f)[0]) for f in refs]
        for c in t["candidates"]:
            fr = sf.frame(c["frame"])
            c["rendered"] = fr is not None
            items.append(("%s f%d" % ("ALTA" if c["certainty"] == "high" else "media", c["frame"]), fr[0] if fr else None))
        name = "cut_AREA%02d_spr%02X_list%02d.png" % (area, kind, idx)
        t["sheet"] = name
        render_sheet(os.path.join(out, name), items, "AREA%02d SPR %02X list %d (%s) - %s" % (area, kind, idx, t["file"], t["palette"]))
        sheets.append(name)
    return sheets


def render_vram(out_dir, area, regs, seen):
    """cut_AREAnn_vram.png: the unsampled regions (deduplicated by texels across areas)."""
    import vram
    from PIL import Image
    items, kept = [], []
    for g in sorted(regs, key=lambda g: -g["texels"]):
        h = hashlib.md5(g["crop"].tobytes()).hexdigest()
        if h in seen:
            continue
        seen[h] = "AREA%02d" % area
        ws = g["pal"] or tuple(0x421 * min(i * 2, 31) if i else 0 for i in range(16))
        rgba = [(0, 0, 0, 0)] + [((w & 31) * 255 // 31, (w >> 5 & 31) * 255 // 31, (w >> 10 & 31) * 255 // 31, 255) if w else (0, 0, 0, 0) for w in ws[1:16]]
        im = Image.new("RGBA", (g["w"], g["h"]))
        im.putdata([rgba[i] for i in g["crop"].flatten()])
        g["palette"] = "CLUT mas usada por los parts SPR de esa pagina (adivinada)" if g["pal"] else "rampa gris (sin paleta conocida)"
        items.append(("%s %d,%d" % (g["page"].split("/")[-1].split(".")[0], g["x"], g["y"]), im))
        kept.append(g)
        if len(items) >= 48:
            break
    if items:
        name = "cut_AREA%02d_vram.png" % area
        render_sheet(os.path.join(out_dir, name), items, "AREA%02d: zonas de VRAM (4 bpp) que ningun part SPR ni poligono de modelo muestrea" % area, cols=8, cell=112)
        for g in kept:
            g["sheet"] = name
    return kept


CERT = {"high": "alta", "medium": "media"}


def write_index(out_dir, res, stats, vr, jp, notes=None):
    """INDICE.txt (Spanish): one line per candidate; notes = {(sheet, frame or None): text} written by hand after looking at the sheets."""
    notes = notes or {}
    L = []
    w = L.append
    w("CONTENIDO GRAFICO POSIBLEMENTE NO USADO - Tombi! (PAL) y comparacion con Ore! Tomba (JP)")
    w("Generado por tools/find_unused_gfx.py (analisis estatico, sin emulacion).")
    w("")
    w("COMO SE DECIDE 'NO USADO'")
    w(" * Fotogramas SPR: un fotograma se considera usado si aparece en algun guion de animacion que alcanza el codigo de un objeto")
    w("   colocado en el area (tools/animobj.py) para esa tabla SPR, o (Tombi) en sus 90 guiones de MAIN0 (tools/anim.py).")
    w("   certeza ALTA: ademas ningun guion referenciado en todo el overlay del area ni en MAIN0 (punteros en codigo o datos) lo nombra,")
    w("   y no esta a 1-3 fotogramas detras de uno usado (codigo 'fotograma = base + contador').")
    w("   certeza MEDIA: no lo usa ningun guion del objeto, pero algun guion huerfano lo podria nombrar o esta junto a uno usado.")
    w("   LIMITACION: muchos objetos fijan el fotograma desde codigo (sin guion) y eso no se modela; las tablas sin ningun guion")
    w("   (%d) y las que el codigo indexa directamente (casas/pueblo, items, efectos compartidos: %d) NO se listan." % (
        stats.get("tables_sin_scripts", 0), stats.get("tables_code_indexed", 0)))
    w("   Varias hojas 'media' muestran claramente graficos que si salen en el juego (p.ej. cerdos de AREA03): son falsos positivos.")
    w(" * VRAM: zonas de paginas 4 bpp de un area/sala con pixeles no nulos que ningun part SPR (con los desplazamientos de pagina")
    w("   inferidos o fijados por el codigo) ni ningun poligono de modelo WMD muestrea. Certeza MEDIA como mucho: el HUD, fondos de")
    w("   capa 7, particulas y otras primitivas dibujadas con coordenadas fijas en el codigo no se modelan. Paleta adivinada.")
    w(" * JP/PAL: se buscan tablas SPR por su forma en todos los ficheros (descomprimidos) de ambos discos y se compara cada")
    w("   fotograma por sus parts (sin CLUT/tpage). Los fotogramas JP sin igual en PAL se renderizan; si su imagen coincide (<5%%")
    w("   de pixeles distintos) con un fotograma PAL del mismo tamano no se cuentan como nuevos.")
    w("")
    w("RESUMEN: %d tablas SPR, %d fotogramas; candidatos SPR %d (alta %d); zonas VRAM %d; tablas JP con fotogramas propios %d" % (
        stats.get("tables", 0), stats.get("frames", 0), stats.get("candidates", 0), stats.get("high", 0),
        sum(len(v) for v in vr.values()), sum(1 for e in jp if e["side"] == "jp_only" and e.get("new_images"))))
    w("")
    w("=== 1. FOTOGRAMAS SPR SIN REFERENCIA (por area) ===")
    w("area | fichero | tabla SPR (lista) | fotograma | certeza | motivo | notas | hoja")
    for t in res:
        for c in t.get("candidates", []):
            why = "ningun guion lo nombra" if not c["in_orphan_script"] else "ningun guion del objeto; solo un guion huerfano de otro objeto podria"
            if c["near_used"]:
                why += "; junto a un fotograma usado (posible base+contador)"
            if not c.get("rendered", True):
                why += "; no se pudo renderizar"
            n = notes.get((t.get("sheet"), c["frame"])) or notes.get((t.get("sheet"), None)) or ""
            w("AREA%02d | %s | SPR %02X (lista %s) | f%d | %s | %s | %s | %s" % (t["area"], t["file"], t["kind"], ",".join(map(str, t["lists"])),
                                                                         c["frame"], CERT[c["certainty"]], why, n, t.get("sheet", "-")))
    w("")
    w("Paleta de cada hoja: " + "; ".join("%s: %s" % (t["sheet"], t.get("palette", "?")) for t in res if t.get("sheet")))
    w("")
    w("=== 2. ZONAS DE VRAM SIN MUESTREAR (certeza media) ===")
    w("area | pagina | lista | x,y (texeles 4bpp en VRAM) | tamano | texeles | paleta | hoja | notas")
    for ar, regs in sorted(vr.items(), key=lambda kv: int(kv[0])):
        for g in regs:
            n = notes.get((g.get("sheet"), None), "")
            w("AREA%02d | %s | %d | %d,%d | %dx%d | %d | %s | %s | %s" % (int(ar), g["page"], g["list"], g["x"], g["y"], g["w"], g["h"], g["texels"],
                                                                g.get("palette", "?"), g.get("sheet", "-"), n))
    w("")
    w("=== 3. JP frente a PAL ===")
    for e in jp:
        if e["side"] == "jp_only":
            n = notes.get((e.get("sheet"), None), "")
            w("SOLO JP | %s @%d (%d fotogramas) ~ %s | fotogramas sin igual en PAL: %s | imagenes nuevas: %s | casi iguales a PAL: %s | paleta: %s | hoja: %s | %s" % (
                e["file"], e["off"], e["frames"], e.get("matched") or "-", e["missing"][:60], e.get("new_images", "-"),
                ", ".join("f%d~P%d(%s%%)" % (int(k), v[0], v[1]) for k, v in (e.get("near_pal") or {}).items()) or "-",
                e.get("palette", e.get("note", "-")), e.get("sheet", "-"), n))
    for e in jp:
        if e["side"] == "pal_only":
            w("SOLO PAL | %s (%d fotogramas) | fotogramas cuyos parts no estan en JP: %s" % (e["file"], e["frames"], e["missing"][:60]))
    open(os.path.join(out_dir, "INDICE.txt"), "w", encoding="utf-8").write("\n".join(L) + "\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--areas", default=None)
    ap.add_argument("--jp", default=os.path.join(ROOT, "game_jp", "disc", "tomba_jp.bin"))
    ap.add_argument("--owners", default=None, help="cached animobj owners json")
    ap.add_argument("--no-render", action="store_true")
    ap.add_argument("--skip-jp", action="store_true")
    ap.add_argument("--skip-vram", action="store_true")
    ap.add_argument("--index-only", action="store_true", help="rewrite INDICE.txt from OUT/candidates.json (+ OUT/notas.json)")
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    if a.index_only:
        j = json.load(open(os.path.join(a.out, "candidates.json")))
        nf = os.path.join(a.out, "notas.json")
        notes = {}
        if os.path.exists(nf):
            for k, v in json.load(open(nf)).items():
                sh, _, fr = k.partition(":")
                notes[(sh, int(fr) if fr else None)] = v
        write_index(a.out, j["spr"], j["stats"], j["vram"], j["jp"], notes)
        return
    areas = [int(x) for x in a.areas.split(",")] if a.areas else sorted(int(k[2:]) for k in A.load_lists() if k.startswith("AR"))
    if a.owners and os.path.exists(a.owners):
        owners = json.load(open(a.owners))
    else:
        owners = M.resolve_sources(M.build_owners())
        if a.owners:
            json.dump(owners, open(a.owners, "w"), default=str)
    res, stats = analyse(owners, areas)
    vr, seen = {}, {}
    for ar in ([] if a.skip_vram else areas):
        try:
            regs = vram_unused(ar, owners)
            kept = render_vram(a.out, ar, regs, seen) if not a.no_render else regs
            vr[ar] = [{k: v for k, v in g.items() if k not in ("crop", "pal")} for g in kept]
        except Exception as e:
            print("vram %d: %r" % (ar, e))
    jp = []
    if not a.skip_jp and os.path.exists(a.jp):
        jp = render_jp(a.out, a.jp)
    if not a.no_render:
        render_spr(a.out, res, owners)
    json.dump(dict(spr=res, stats=stats, vram={str(k): v for k, v in vr.items()}, jp=jp), open(os.path.join(a.out, "candidates.json"), "w"), indent=0, default=str)
    write_index(a.out, res, stats, {str(k): v for k, v in vr.items()}, jp)
    print(dict(stats))
    print("vram regions", sum(len(v) for v in vr.values()), "jp diffs", len(jp))


if __name__ == "__main__":
    main()
