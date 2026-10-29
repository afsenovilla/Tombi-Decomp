#!/usr/bin/env python3
"""How many sprites does the graphics editor draw like the game?  Compares the editor's own decoding with a port capture.

For every sprite part the editor renders (Tombi's frames, the SPR frames of every enemy / NPC / object owner of tools/anim.py,
the WPP frame items, the UiSpriteDef entries) the texels of its UV box and the palette the editor applies are compared with the
draws of tools/gfx_capture.py that have the same UV box (and, for an area's objects, the same area):

  ok        a captured draw has the same texels and the same colours
  palette   same texels, other colours (wrong CLUT / palette data)
  texels    the editor's texels match no captured draw with that UV box: wrong page / bit depth / page offset / garbled
  (uncovered: the game drew nothing with that UV box during the capture; not counted)

A sprite (frame) is wrong when any of its covered parts is.  Pages: the bit depth and default palette the page item opens with,
against the draws whose texels lie on that page.

  python3 tools/gfx_measure.py [--capture FILE] [--json OUT] [--sheet PNG] [--only anim|wpp|ui|pages]"""
import argparse, collections, json, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import area000 as A
import gfx_capture as GC

VW = 1024


class Index:
    """Captured 4/8 bpp draws by UV box and by size.  A textured quad's UV box is one texel wider and higher than the
    part (its far edge is exclusive), so (w, h) parts are looked up as (w, h) and (w + 1, h + 1)."""

    def __init__(self, draws):
        self.by_box = collections.defaultdict(list)
        self.by_size = collections.defaultdict(list)
        for d in draws:
            if d.depth < 2:
                self.by_box[(d.u, d.v, d.w, d.h)].append(d)
                self.by_size[(d.w, d.h)].append(d)

    @staticmethod
    def _area(c, area):
        return c if area is None else [d for d in c if d.area == area]

    def at(self, box, area=None):
        u, v, w, h = box
        return self._area(self.by_box.get((u, v, w, h), []) + self.by_box.get((u, v, w + 1, h + 1), []), area)

    def sized(self, w, h, area=None):
        return self._area(self.by_size.get((w, h), []) + self.by_size.get((w + 1, h + 1), []), area)


def crops(d, w, h):
    """The w x h texel blocks of a captured draw (it may be one texel wider / higher)."""
    t = bytes(d.tex)
    if d.w == w and d.h == h:
        yield t
        return
    for oy in range(d.h - h + 1):
        for ox in range(d.w - w + 1):
            yield b"".join(t[(oy + j) * d.w + ox:(oy + j) * d.w + ox + w] for j in range(h))


def judge(cands, depth, tex, w, h, pal):
    """('ok'|'palette'|'texels'|'uncovered', matching draw or None)."""
    if not cands:
        return "uncovered", None
    up = {i: pal[i] for i in set(tex)}
    best = None
    for d in cands:
        if d.depth != depth or not any(c == tex for c in crops(d, w, h)):
            continue
        if all(d.pal[i] == wd for i, wd in up.items()):
            return "ok", d
        best = best or d
    if best:
        return "palette", best
    return "texels", cands[0]


def vram_box(v, tp, u, vv, w, h):
    """Indices of a UV box of texture page word tp in the editor's VRAM model (wrap at 256 like the GPU)."""
    mode = (tp >> 7) & 3
    q = tp & 0x1F
    px, py = (q & 15) * 64, (q >> 4) * 256
    out = bytearray()
    if mode == 0:
        src, W = v.nib, VW * 4
        for j in range(h):
            a = ((py + ((vv + j) & 255)) & 511) * W + px * 4
            out += bytes(src[a + ((u + i) & 255)] for i in range(w))
    elif mode == 1:
        src, W = v.d, VW * 2
        for j in range(h):
            a = ((py + ((vv + j) & 255)) & 511) * W + px * 2
            out += bytes(src[a + ((u + i) & 255)] for i in range(w))
    else:
        return None, 2
    return bytes(out), mode


def plane_box(plane, u, vv, w, h):
    rows, pw, ph = plane
    out = bytearray()
    for j in range(h):
        r = rows[vv + j] if 0 <= vv + j < ph else b""
        out += bytes(r[u + i] if 0 <= u + i < len(r) else 0 for i in range(w))
    return bytes(out)


def vram_rgba(w):
    import vram
    return vram.RGBA[w]


def part_box(p):
    us = (p[0], p[3], p[6], p[10])
    vs = (p[1], p[4], p[7], p[11])
    return min(us), min(vs), max(us) - min(us) + 1, max(vs) - min(vs) + 1


class Recorder:
    """Wraps vram.render_part to log, per part the editor renders, its texels + palette (the editor's view)."""

    def __init__(self):
        import vram
        self.vram = vram
        self.orig = vram.render_part
        self.log = None
        vram.render_part = self.hook

    def hook(self, v, p, base, plane=None):
        r = self.orig(v, p, base, plane)
        if self.log is not None:
            vm = self.vram
            u, vv, w, h = part_box(p)
            if plane is not None:
                tex, depth = plane_box(plane, u, vv, w, h), 0
                ws = v.palette(vm.wpp_clut(p[2]), 16)[2]
                if not any(ws[1:]):
                    ws = vm.fallback_palette(v, vm.wpp_clut(p[2]), 16)[2]
            else:
                tex, depth = vram_box(v, p[5] + base, u, vv, w, h)
                if tex is None:
                    return r
                n = 16 if depth == 0 else 256
                ws = v.palette(p[2], n)[2]
                if not any(ws[1:]):
                    ws = vm.fallback_palette(v, p[2], n)[2]
            rows = r[3]
            fx, fy = p[3] < p[0], p[7] < p[1]
            rows = [(row[::-1] if False else row) for row in (rows[::-1] if fy else rows)]
            if fx:
                rows = [b"".join(row[k:k + 4] for k in range(len(row) - 4, -1, -4)) for row in rows]
            self.log.append(((u, vv, w, h), depth, tex, ws, plane is not None, b"".join(rows), (p[8], p[9])))
        return r


def measure_anims(core, idx, res, samples):
    import anim, animobj
    rec = Recorder()
    # Tombi: WPP frames + palette row 0 of the area CLUT, part boxes of SPR 0x30 (the frame plane is the uploaded texture)
    import vram, wpp
    sp = anim.Sprites()
    lst = A.find_list(anim.TOMBI_WPP)[2]
    frames = next(f for _, ti, f in vram.wpp_tables(lst) if _ == anim.TOMBI_WPP and len(f) == 358)
    spr = next(d for k, n, d in wpp.spr_candidates(anim.TOMBI_WPP) if k == 0x30 and n == 358)
    parts = vram.parse_spr(spr)
    blob = A.read_resource(A.find_clut(anim.TOMBI_WPP)["name"])
    pal0 = (0,) + struct.unpack_from("<15H", blob, 2)        # anim.Sprites before the fix: row 0 for every part
    anim_part_clut = "pals.setdefault" in open(anim.__file__).read()   # anim.Sprites colours each part with its own CLUT row
    for fi, o in enumerate(frames):
        plane = vram.wpp_plane(o)
        out = []
        for p in parts[fi]:
            if not p[8] or not p[9]:
                continue
            box = part_box(p)
            tex = plane_box(plane, *box)
            pr = ((p[2] >> 6) & 0x1FF) - 480
            pal = (0,) + struct.unpack_from("<15H", blob, (pr * 256 + 1) * 2) if anim_part_clut else pal0
            out.append((box, 0, tex, pal, True))
        add_sprite(res, idx, "anim", "tombi frame %d" % fi, out, None, samples, lambda fi=fi: sp.frame(fi)[0])
    # objects of every area
    for ld in sorted(A.load_lists()):
        if not ld.startswith("AR"):
            continue
        area = int(ld[2:])
        try:
            owners = anim.object_owners(core.disc.raw if core else None, area)
        except Exception as e:
            print("area", area, "owners failed:", e)
            continue
        for ow in owners:
            if not ow.get("spr"):
                continue
            rec.log = []
            try:
                src = anim.owner_source(ow)
            except Exception as e:
                print(ow["owner"], "source failed:", e)
                continue
            # SprFrames renders every frame at construction: split the log per frame by re-rendering frame by frame is costly; the
            # parts are logged in frame order, so map them with the frame part counts
            log, rec.log = rec.log, None
            if src is None:
                continue
            fr = vram.parse_spr(animobj.spr_table("AR%02d" % ow["area"], ow["spr"]["list"], int(ow["spr"]["kind"], 16)))
            if len(log) != sum(1 for ps in fr for p in ps if p[8] and p[9]):
                print(ow["owner"], "parts logged %d != table parts: skipped" % len(log))
                continue
            k = 0
            used = set(f for a in ow["animations"] for f in a["frames"])
            for fi, ps in enumerate(fr):
                n = sum(1 for p in ps if p[8] and p[9])
                chunk = log[k:k + n]
                k += n
                if fi not in used:
                    continue
                add_sprite(res, idx, "anim", "%s frame %d" % (ow["owner"], fi), chunk, area, samples,
                           lambda src=src, fi=fi: (src.frame(fi) or (None,))[0])


def add_sprite(res, idx, cat, name, parts, area, samples, image=None):
    verdicts = []
    for part in parts:
        box, depth, tex, ws = part[:4]
        from_plane = len(part) > 4 and part[4]
        if tex is None:
            continue
        if from_plane:      # streamed frame (WPP): its VRAM slot is chosen at run time, only the colours can be wrong
            cands = idx.sized(box[2], box[3], area)
            v, d = judge(cands, depth, tex, box[2], box[3], ws)
            if v == "texels":
                v, d = "uncovered", None
        else:
            v, d = judge(idx.at(box, area), depth, tex, box[2], box[3], ws)
        if len(part) > 5 and v != "uncovered":      # what the editor really shows (it may come from the capture itself)
            shown, (pw, ph) = part[5], part[6]
            cands = idx.sized(box[2], box[3], area) if from_plane else idx.at(box, area)
            for c in cands:
                pr = [vram_rgba(c.pal[i]) for i in bytes(c.tex)]
                for oy in range(c.h - ph + 1):
                    for ox in range(c.w - pw + 1):
                        if b"".join(pr[(oy + j) * c.w + ox + i] for j in range(ph) for i in range(pw)) == shown:
                            v, d = "ok", c
                            break
                    if v == "ok":
                        break
                if v == "ok":
                    break
        verdicts.append((v, d))
    cov = [v for v, d in verdicts if v != "uncovered"]
    if not cov:
        res[cat]["uncovered"] += 1
        return
    worst = "texels" if "texels" in cov else "palette" if "palette" in cov else "ok"
    res[cat][worst] += 1
    if worst != "ok" and len(samples[cat]) < 400:
        samples[cat].append(dict(name=name, verdict=worst, image=image, draws=[d for v, d in verdicts if d is not None and v == worst][:4]))


def measure_wpp(core, idx, res, samples):
    import vram
    for name in sorted({n for ld, lists in core.lists.items() for lst in lists for r in lst for n in [r["name"]] if (r["kind"] & 0xF0) == 0x60 and n.endswith(".000")}):
        try:
            man, files, frames = core.wpp(name)
        except Exception:
            continue
        lst = A.find_list(name)
        area = int(lst[0][2:]) if lst and lst[0].startswith("AR") else None
        # SPR tables linked to this WPP (same frame count): their part boxes are the boxes the game cuts from each frame
        tabs = {}
        if lst:
            for r, d in A.list_blocks(lst[0], lst[1]):
                if (r["kind"] & 0xF0) == 0x30 and d and len(d) >= 4:
                    tabs[struct.unpack_from("<H", d, 2)[0] // 4] = vram.parse_spr(d)
        n = len([k for k in files if k.startswith("frame_")])
        spr = tabs.get(n)
        for fi in range(n):
            key = "%s#%d" % (name, fi)
            d = core.get(key)
            w, h = struct.unpack_from("<HH", d)
            plane = vram.wpp_plane(d)
            sf = core.surface_for(("wppframe", name, fi))
            ref = sf.pal
            ws = core.pal_words(ref) if ref else (0,) * 16
            ws = tuple(ws[:16]) + (0,) * (16 - len(ws[:16]))
            parts = []
            if spr and fi < len(spr):
                for p in spr[fi]:
                    if p[8] and p[9]:
                        box = part_box(p)
                        pw = ws
                        if sf.multi_pal:     # the editor paints each part rect with its own palette
                            r = next((r for r in sf.rects if (r.x, r.y) == box[:2] and r.pal), None)
                            if r:
                                pw = tuple(core.pal_words(r.pal)[:16])
                        parts.append((box, 0, plane_box(plane, *box), pw, True))
            else:
                parts.append(((0, 0, w * 4, h), 0, plane_box(plane, 0, 0, w * 4, h), ws, True))
            add_sprite(res, idx, "wpp", key, parts, area, samples, lambda k=("wppframe", name, fi): core.surface_for(k).render())


def measure_ui(core, idx, res, samples):
    s = core._page_surface("SYSTEM/A10003.GAM", "SYS", 0)
    for r in s.rects:
        ws = tuple(core.pal_words(r.pal)) if r.pal else (0,) * 16
        p = core.page_info("SYSTEM/A10003.GAM", "SYS", 0)
        tp = (p["x"] // 64) | (p["y"] // 256) << 4
        import vram
        v = vram.Vram()
        v.load(p["x"], p["y"], p["w"], p["h"], core.get("SYSTEM/A10003.GAM"))
        tex, depth = vram_box(v, tp, r.x, r.y, r.w, r.h)
        add_sprite(res, idx, "ui", r.label, [((r.x, r.y, r.w, r.h), 0, tex, ws)], None, samples)


def measure_pages(core, truth, res, samples):
    """Per page item: render it as the editor opens it and compare, pixel by pixel, every sprite the game draws from it."""
    import vram
    pages = truth.pages(core)
    done = set()
    for ld, lists in core.lists.items():
        for k, lst in enumerate(lists):
            for i, r, bpp, cl in core.grpx(lst):
                name = r["name"]
                if bpp == 16 or name in done:
                    continue
                done.add(name)
                ents = pages.get(name)
                if not ents:
                    res["pages"]["uncovered"] += 1
                    continue
                try:
                    s = core._page_surface(name, ld, k)
                    im = s.render().tobytes()
                    pl = s.plane()
                except Exception as e:
                    res["pages"]["error"] += 1
                    continue
                W = s.W
                good = tot = 0
                depth_bad = 0
                okrect = set()
                for dep, cx, cy, x, y, w, h, hx, n in ents:
                    if (4, 8)[dep] != s.bpp:
                        continue
                    ws = [int(hx[q:q + 4], 16) for q in range(0, len(hx), 4)]
                    ok = True
                    for yy in range(y, min(y + h, s.H)):
                        for xx in range(x, min(x + w, W)):
                            o = (yy * W + xx) * 4
                            if im[o:o + 4] != vram.RGBA[ws[pl[yy * W + xx]]]:
                                ok = False
                                break
                        if not ok:
                            break
                    if ok:
                        okrect.add((x, y, w, h))
                # a rect the game draws with several palettes (fades, palette swaps) is right when the editor shows one of them
                for dep, cx, cy, x, y, w, h, hx, n in ents:
                    tot += n
                    if (4, 8)[dep] != s.bpp:
                        depth_bad += n
                    elif (x, y, w, h) in okrect:
                        good += n
                v_ = "ok" if good >= 0.95 * tot else "depth" if depth_bad > tot / 2 else "palette"
                res["pages"][v_] += 1
                if v_ != "ok" and len(samples["pages"]) < 400:
                    samples["pages"].append(dict(name="%s (%s list %d)" % (name, ld, k), verdict=v_, image=None, draws=[]))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--capture", default=GC.default_store())
    ap.add_argument("--disc", default=os.path.join(os.path.dirname(HERE), "game", "disc", "fs"))
    ap.add_argument("--json")
    ap.add_argument("--sheet", help="contact sheet PNG of a sample of the wrong sprites (editor | game)")
    ap.add_argument("--only", action="append")
    ap.add_argument("--no-truth", action="store_true", help="measure the editor without the capture as ground-truth source")
    a = ap.parse_args(argv)
    import gfx_core
    draws = GC.load(a.capture)
    idx = Index(draws)
    core = gfx_core.Core(a.disc)
    import vram
    if a.no_truth:       # the editor without the capture (its static decoding alone)
        core._truth = None
        vram.TRUTH = None
    else:
        vram.TRUTH = core.truth()
    res = collections.defaultdict(collections.Counter)
    samples = collections.defaultdict(list)
    only = set(a.only or ("anim", "wpp", "ui", "pages"))
    if "ui" in only:
        measure_ui(core, idx, res, samples)
    if "wpp" in only:
        measure_wpp(core, idx, res, samples)
    if "anim" in only:
        measure_anims(core, idx, res, samples)
    if "pages" in only:
        measure_pages(core, GC.Truth(a.capture), res, samples)
    tot = collections.Counter()
    for cat in sorted(res):
        c = res[cat]
        cov = sum(n for k, n in c.items() if k != "uncovered")
        bad = cov - c["ok"]
        tot["covered"] += cov
        tot["wrong"] += bad
        print("%-6s covered %5d  ok %5d  wrong %5d  (%s)  uncovered %d" % (cat, cov, c["ok"], bad, ", ".join("%s %d" % (k, n) for k, n in sorted(c.items()) if k not in ("ok", "uncovered")), c["uncovered"]))
    print("TOTAL  covered %d  wrong %d" % (tot["covered"], tot["wrong"]))
    if a.json:
        json.dump(dict(result={k: dict(v) for k, v in res.items()}, wrong={k: [dict(name=s["name"], verdict=s["verdict"]) for s in v] for k, v in samples.items()}),
                  open(a.json, "w"), indent=1)
    if a.sheet:
        sheet(samples, a.sheet)
    return 0


def sheet(samples, path, n=40):
    from PIL import Image, ImageDraw
    rows = []
    for cat in ("anim", "wpp", "ui"):
        for s in samples.get(cat, [])[:n]:
            try:
                im = s["image"]() if s["image"] else None
            except Exception:
                im = None
            g = [d.image() for d in s["draws"][:3]]
            rows.append((s["name"] + " [" + s["verdict"] + "]", im, g))
    W, y = 900, 0
    hs = [max([16] + [(i.height if i else 0) for i in [r[1]] + r[2]]) * 2 + 14 for r in rows]
    out = Image.new("RGBA", (W, sum(hs) + 4), (40, 40, 56, 255))
    dr = ImageDraw.Draw(out)
    for (name, im, g), h in zip(rows, hs):
        dr.text((2, y), name[:120], fill=(255, 255, 255, 255))
        x = 2
        for i in [im] + g:
            if i is None:
                x += 20
                continue
            i2 = i.resize((i.width * 2, i.height * 2), Image.NEAREST)
            out.alpha_composite(i2, (x, y + 12)) if x + i2.width < W else None
            x += i2.width + 12
        y += h
    out.save(path)


if __name__ == "__main__":
    sys.exit(main())
