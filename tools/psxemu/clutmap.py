"""Which palette the game draws each piece of a texture page with: runs MAIN0.EXE in the interpreter through a few scenes, records every
textured primitive the game sends (texture page, CLUT word, UV box, semi transparency), and resolves both sides to disc files: the GRPX
record that holds the texels, and the record (+ word offset) that holds the 16 / 256 colours found at the CLUT address at draw time.

  tools/psxemu/clutmap.py OUT.json [--scenes title,load,area00,menu,item,map] [--state-dir DIR]

The result (`tools/ui_cluts.json` in the repository, only VRAM coordinates and file names, no game data) is what tools/vram.py and
tools/screens.py use for the pieces that code draws with a palette chosen at run time (HUD counters, menus, fonts, the inventory, the
title): the SPR tables only cover objects, see tools/sprclut.py.

Entry: {"page": file, "page_xy": [x, y] (words, VRAM), "bpp": 4|8, "rect": [x0, y0, x1, y1] (texels inside the page record, inclusive),
"clut": [x, y], "pal": [file, first word(, note)] | null, "abr": n, "semi": 0|1, "scenes": [...], "count": n}.
"""
import json
import os
import sys

if __package__ in (None, ""):
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from psxemu import capture as C, gpu as G
else:
    from . import capture as C, gpu as G

SELECT, START, CROSS, RIGHT = 0x0001, 0x0008, 0x4000, 0x0020


class Recorder:
    """on_draw hook: decodes the GP0 packets of every frame (no rasterizing needed) into {(tpage, clut, u0, v0, u1, v1, semi): count}
    + the palette words at the CLUT address the first time each key is seen."""

    def __init__(self):
        self.scene = None
        self.prims = {}         # scene -> {key: count}
        self.pal = {}           # (scene, key) -> tuple of words
        self.tpage = 0

    def _add(self, m, tp, clut, us, vs, semi):
        bpp = (tp >> 7) & 3
        if bpp > 1:
            return
        key = (tp & 0x1FF, clut, min(us), min(vs), max(us), max(vs), semi)
        d = self.prims.setdefault(self.scene, {})
        if key not in d:
            x, y = (clut & 0x3F) * 16, (clut >> 6) & 0x1FF
            n = 16 if bpp == 0 else 256
            self.pal[(self.scene, key)] = tuple(int(w) for w in m.gpu.vram[y, x:x + n])
        d[key] = d.get(key, 0) + 1

    def packet(self, m, w):
        c = w[0] >> 24
        if c == 0xE1:
            self.tpage = w[0] & 0xFFFF
        elif 0x20 <= c < 0x40 and c & 4:
            q = 4 if c & 8 else 3
            g = bool(c & 0x10)
            uvs, k = [], 1
            for i in range(q):
                if g and i:
                    k += 1
                uvs.append(w[k + 1])
                k += 2
            self._add(m, uvs[1] >> 16, uvs[0] >> 16, [u & 0xFF for u in uvs], [(u >> 8) & 0xFF for u in uvs], (c >> 1) & 1)
        elif 0x60 <= c < 0x80 and c & 4:
            uv = w[2]
            sz = (c >> 3) & 3
            ww, hh = (w[3] & 0xFFFF, w[3] >> 16) if sz == 0 else {1: (1, 1), 2: (8, 8), 3: (16, 16)}[sz]
            u, v = uv & 0xFF, (uv >> 8) & 0xFF
            if ww and hh:
                self._add(m, self.tpage, uv >> 16, [u, u + ww - 1], [v, v + hh - 1], (c >> 1) & 1)

    def __call__(self, m, pk):
        if self.scene is None:
            return
        for _, p in pk:
            i = 0
            while i < len(p):
                n = G.cmd_len(p, i)
                if n and p[i:i + n]:
                    self.packet(m, p[i:i + n])
                i += n


def run(scenes=("title", "load", "area00", "menu", "item", "map"), log=print, state_dir=None):
    """(recorder, {scene: VRAM numpy array at the end of the scene})."""
    import numpy as np
    rec = Recorder()
    vr = {}
    m = C.boot()
    m.render = False
    C.title(m, log=log)                     # title() installs its own on_draw
    m.on_draw = rec
    state_dir = state_dir or os.path.join(os.path.expanduser("~"), ".cache", "tombi_clutmap")
    os.makedirs(state_dir, exist_ok=True)
    st_title = os.path.join(state_dir, "title.state")
    m.save_state(st_title)

    def scene(name, script):
        rec.scene = name
        C.play(m, script)
        vr[name] = np.array(m.gpu.vram)
        log("%s: frame %d, %d primitive kinds" % (name, m.frame_count, len(rec.prims.get(name, {}))))

    if "title" in scenes:
        scene("title", [(0, 30)])
    if "load" in scenes:            # title menu: NEW GAME -> LOAD, the memory card screen (SYS list 4)
        scene("load", [(RIGHT, 3), (0, 20), (CROSS, 3), (0, 200)])
        m.load_state(st_title)
        m.on_draw = rec
    if any(s in scenes for s in ("area00", "menu", "item", "map")):
        rec.scene = None
        C.play(m, [(START, 4), (0, 300)])
        scene("area00", [(0, 60)])
        rec.scene = None
        C.play(m, C.TALK)
        st_room = os.path.join(state_dir, "room.state")
        m.save_state(st_room)
        if "menu" in scenes or "item" in scenes:
            scene("menu", [(SELECT, 3), (0, 120)])
            if "item" in scenes:
                scene("item", [(CROSS, 3), (0, 120), (RIGHT, 3), (0, 60)])
            m.load_state(st_room)
            m.on_draw = rec
        if "map" in scenes:
            scene("map", C.TO_MAP)
    rec.scene = None
    return rec, vr


def resolve(rec, vr):
    """Entries of the JSON (see the module doc): texels and palette resolved to the GRPX records of the load lists by content."""
    import numpy as np
    import area000 as A
    import vram as V
    recs = {}
    for lists in A.load_lists().values():
        for lst in lists:
            for r in lst:
                if V.is_grpx(r) and r["name"] not in recs:
                    d = V.resource(r["name"])
                    if d is not None and r["w"] > 0 and len(d) >= r["w"] * r["h"] * 2:
                        a = np.frombuffer(bytes(d[:r["w"] * r["h"] * 2]), dtype="<u2").reshape(r["h"], r["w"])
                        recs[r["name"]] = (r["arg"] & 0xFFFF, r["arg"] >> 16, a)

    def page_of(scene, x0, y0, x1, y1):
        v = vr[scene]
        best = None
        for name, (rx, ry, a) in recs.items():
            h, w = a.shape
            if rx <= x0 and x1 < rx + w and ry <= y0 and y1 < ry + h:
                if np.array_equal(a[y0 - ry:y1 - ry + 1, x0 - rx:x1 - rx + 1], v[y0:y1 + 1, x0:x1 + 1]):
                    if best is None or w * h < best[3]:
                        best = (name, rx, ry, w * h)
        return best

    def pal_of(cx, cy, words):
        if not any(words[1:]):
            return None
        best = None
        for name, (rx, ry, a) in recs.items():          # placed record covering the CLUT address with the same words
            h, w = a.shape
            if rx <= cx and cx + len(words) <= rx + w and ry <= cy < ry + h:
                if tuple(int(x) for x in a[cy - ry, cx - rx:cx - rx + len(words)]) == tuple(words):
                    k = (0 if h <= 32 else 1, w * h)
                    if best is None or k < best[0]:
                        best = (k, [name, (cy - ry) * w + (cx - rx)])
        if best:
            return best[1]
        for name, (rx, ry, a) in recs.items():          # palette changed at run time (fades, highlight): the record loaded there
            h, w = a.shape
            if h <= 32 and rx <= cx and cx + len(words) <= rx + w and ry <= cy < ry + h and any(a[cy - ry, cx - rx:cx - rx + len(words)]):
                return [name, (cy - ry) * w + (cx - rx), "changed at run time: colours as loaded"]
        ws = np.array(words, dtype="<u2").tobytes()      # anywhere in a record (palettes uploaded by code from a file)
        for name, (rx, ry, a) in recs.items():
            b = a.tobytes()
            i = b.find(ws)
            while i >= 0 and i & 1:
                i = b.find(ws, i + 1)
            if i >= 0:
                return [name, i // 2]
        return None

    out = {}
    for scene, d in rec.prims.items():
        for key, n in d.items():
            tp, clut, u0, v0, u1, v1, semi = key
            bpp = (tp >> 7) & 3
            px, py = (tp & 15) * 64, ((tp >> 4) & 1) * 256
            sh = 2 if bpp == 0 else 1
            x0, x1 = px + (u0 >> sh), px + (u1 >> sh)
            y0, y1 = py + v0, py + v1
            pg = page_of(scene, x0, y0, x1, y1)
            if pg is None:
                continue           # texels drawn by the game itself (fonts rendered into VRAM, movie frames)
            name, rx, ry, _ = pg
            cx, cy = (clut & 0x3F) * 16, (clut >> 6) & 0x1FF
            ok = (name, (rx * (4 if bpp == 0 else 2)), (u0 + (px - rx) * (4 if bpp == 0 else 2)), v0 + py - ry,
                  u1 + (px - rx) * (4 if bpp == 0 else 2), v1 + py - ry, cx, cy, semi, (tp >> 5) & 3)
            e = out.get(ok)
            if e is None:
                e = out[ok] = {"page": name, "page_xy": [rx, ry], "bpp": 4 if bpp == 0 else 8, "rect": list(ok[2:6]), "clut": [cx, cy],
                               "pal": pal_of(cx, cy, rec.pal[(scene, key)]), "abr": ok[9], "semi": semi, "scenes": [], "count": 0}
            if scene not in e["scenes"]:
                e["scenes"].append(scene)
            e["count"] += n
    return sorted(out.values(), key=lambda e: (e["page"], e["rect"][1], e["rect"][0], e["clut"][1], e["clut"][0]))


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("--scenes", default="title,load,area00,menu,item,map")
    ap.add_argument("--state-dir")
    a = ap.parse_args(argv)
    rec, vr = run(tuple(a.scenes.split(",")), state_dir=a.state_dir)
    ents = resolve(rec, vr)
    write(a.out, ents)
    print(len(ents), "entries ->", a.out)


def write(path, ents):
    """JSON, or a Python module `ENTRIES = [...]` when `path` ends in .py (tools/ui_cluts.py: importable from the frozen exes)."""
    body = "[\n" + ",\n".join("    " + json.dumps(e) for e in ents) + "\n]\n"
    with open(path, "w") as f:
        if path.endswith(".py"):
            f.write('"""Generated by tools/psxemu/clutmap.py (do not edit): the palette the game draws each piece of a texture page with,\n'
                    'recorded from the primitives MAIN0.EXE sends while it runs (see that module).  VRAM coordinates and file names only."""\n'
                    "null = None\nENTRIES = " + body)
        else:
            f.write(body)


if __name__ == "__main__":
    main()
