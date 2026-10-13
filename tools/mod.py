#!/usr/bin/env python3
"""Mod support: applies mods/<name>/mod.json (PNG patches) onto the extracted PAL files and rebuilds the disc.
The mod layout (mod.json with "patchs" -> file + targets[pattern], mods/load_order.json) follows the idea of TombaPatcher
(US-only tool by its authors; only the layout idea is reused, no code); here the targets are PAL files of game/disc/fs.

  mods/load_order.json          {"order": ["modA", "modB"]}   later mods overwrite earlier ones (mods not listed come last, sorted)
  mods/<name>/mod.json          {"name","description","version","patchs":[{"file":"page.png","targets":[
                                  {"pattern":"AREA00/B000.GAM"},                 fnmatch on the path relative to fs (GRPX page / CLUT)
                                  {"pattern":"AREA00/A004.000","frame":12}]}]}   WPP sprite frame number (frame_0012)
PNG rules: GRPX page 8 bpp = indexed PNG of 2*w x h, 4 bpp = indexed PNG with <= 16 colours of 4*w x h (index = palette index,
the palette is NOT written back: edit the CLUT file with a 16 bit PNG patch), 16 bpp / CLUT = RGB(A) PNG of w x h (RGB -> 5 bit
linear, a fully transparent pixel = 0x0000, anything else gets a nonzero word).  WPP frame = indexed PNG, 4*W x H, indices 0..15.
GAM files are unpacked/repacked with tools/gam.py, WPP frames re-encoded with tools/wpp.py.

  tools/mod.py list                       mods found and their order
  tools/mod.py apply [--mods a,b] [--fs DIR] [--out DIR]   patched copy of fs (untouched files hard-linked) in build/mod/fs
  tools/mod.py build [--mods a,b] [-o OUT.bin]             apply + mkdisc.py build --relayout (the file sizes change)
"""
import argparse, fnmatch, json, os, shutil, struct, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODS = os.path.join(ROOT, "mods")
FS = os.path.join(ROOT, "game", "disc", "fs")


def order():
    names = sorted(d for d in os.listdir(MODS) if os.path.isfile(os.path.join(MODS, d, "mod.json"))) if os.path.isdir(MODS) else []
    lo = os.path.join(MODS, "load_order.json")
    first = json.load(open(lo)).get("order", []) if os.path.isfile(lo) else []
    return [n for n in first if n in names] + [n for n in names if n not in first]


def png_words(path, w, h, bpp):
    from PIL import Image
    im = Image.open(path)
    if bpp in (4, 8):
        if im.mode != "P":
            sys.exit("%s: indexed (palette) PNG required" % path)
        px = w * (4 if bpp == 4 else 2)
        if im.size != (px, h):
            sys.exit("%s: size %s, expected %dx%d" % (path, im.size, px, h))
        ix = list(im.getdata())
        if bpp == 8:
            return bytes(ix)
        if max(ix) > 15:
            sys.exit("%s: more than 16 colours" % path)
        return bytes(ix[i] | ix[i + 1] << 4 for i in range(0, len(ix), 2))
    im = im.convert("RGBA")
    if im.size != (w, h):
        sys.exit("%s: size %s, expected %dx%d" % (path, im.size, w, h))
    out = bytearray()
    for r, g, b, a in im.getdata():
        v = (r * 31 + 127) // 255 | ((g * 31 + 127) // 255) << 5 | ((b * 31 + 127) // 255) << 10
        if a == 0:
            v = 0
        elif v == 0:
            v = 0x8000   # opaque black = STP bit only
        out += struct.pack("<H", v)
    return bytes(out)


def patch_file(path, nm, png, frame, tmp):
    import area000 as A, wpp, gam
    d = open(path, "rb").read()
    is_gam = path.upper().endswith(".GAM")
    base = nm.rsplit(".", 1)[0]
    if is_gam:
        raw = os.path.join(tmp, "raw.bin")
        if not gam.unpack_one(path, raw):
            sys.exit("gam unpack failed: " + nm)
        d = open(raw, "rb").read()
    rec = A.get_rec(nm)
    if frame is not None:
        r = wpp.is_wpp(nm)
        if not r:
            sys.exit("%s: not a WPP file" % nm)
        man, files, frames = wpp.unpack_wpp(d, nm, r)
        key = "frame_%04d.4bpp" % frame
        if key not in frames:
            sys.exit("%s: no frame %d" % (nm, frame))
        w, h = struct.unpack_from("<HH", frames[key])
        files[key] = struct.pack("<HH", w, h) + png_words(png, w, h, 4)
        new = wpp.pack_wpp(man, files)
    else:
        lr = None
        for lists in A.load_lists().values():
            for lst in lists:
                for x in lst:
                    if x["name"].rsplit(".", 1)[0] == base and (x["kind"] & 0xF0) == 0x10:
                        lr = lr or x
        if not lr:
            sys.exit("%s: no GRPX load record" % nm)
        w, h = lr["w"], lr["h"]
        small = lr["h"] <= 32 and w in (16, 48, 256)
        bpp = 16 if small else (8 if A.find_clut(base) else 4)
        new = png_words(png, w, h, bpp)
    if len(new) != len(d):
        sys.exit("%s: patched size %d != %d" % (nm, len(new), len(d)))
    if is_gam:
        open(raw, "wb").write(new)
        if not gam.pack_one(raw, path):
            sys.exit("gam pack failed: " + nm)
    else:
        open(path, "wb").write(new)


def cmd_apply(a):
    import area000 as A
    sel = a.mods.split(",") if a.mods else order()
    src, out = os.path.abspath(a.fs), os.path.abspath(a.out)
    if os.path.exists(out):
        shutil.rmtree(out)
    for d, _, fs in os.walk(src):
        o = os.path.join(out, os.path.relpath(d, src))
        os.makedirs(o, exist_ok=True)
        for f in fs:
            s, t = os.path.join(d, f), os.path.join(o, f)
            if os.path.relpath(d, src).split(os.sep)[0] in ("EXE", "SYS", "."):
                shutil.copy2(s, t)   # mkdisc --relayout patches these in place
            else:
                try: os.link(s, t)
                except OSError: shutil.copy2(s, t)
    A.FS = src
    n = 0
    with tempfile.TemporaryDirectory() as tmp:
        for m in sel:
            md = os.path.join(MODS, m)
            cfg = json.load(open(os.path.join(md, "mod.json")))
            for p in cfg.get("patchs", []):
                png = os.path.join(md, p["file"])
                for t in p["targets"]:
                    hits = [os.path.relpath(os.path.join(d, f), out).replace(os.sep, "/") for d, _, fs in os.walk(out) for f in fs]
                    hits = [h for h in hits if fnmatch.fnmatch(h, t["pattern"])]
                    if not hits:
                        sys.exit("%s: pattern %s matches nothing" % (m, t["pattern"]))
                    for h in hits:
                        f = os.path.join(out, h)
                        os.unlink(f) if os.stat(f).st_nlink > 1 else None   # never write through a hard link
                        shutil.copy2(os.path.join(src, h), f) if not os.path.exists(f) else None
                        patch_file(f, h, png, t.get("frame"), tmp)
                        n += 1
            print("mod %s applied" % m)
    print("%d patches -> %s" % (n, out))
    return 0


def cmd_build(a):
    a.out = os.path.join(ROOT, "build", "mod", "fs")
    a.fs = FS
    cmd_apply(a)
    cmd = [sys.executable, os.path.join(ROOT, "tools", "mkdisc.py"), "build", "--relayout", "--fs", a.out]
    if a.o:
        cmd += ["-o", a.o]
    return subprocess.call(cmd)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="c", required=True)
    sp.add_parser("list")
    for n in ("apply", "build"):
        s = sp.add_parser(n)
        s.add_argument("--mods")
        if n == "apply":
            s.add_argument("--fs", default=FS)
            s.add_argument("--out", default=os.path.join(ROOT, "build", "mod", "fs"))
        else:
            s.add_argument("-o")
    a = ap.parse_args()
    if a.c == "list":
        for n in order():
            print(n, json.load(open(os.path.join(MODS, n, "mod.json"))).get("name", ""))
        return 0
    return cmd_apply(a) if a.c == "apply" else cmd_build(a)


if __name__ == "__main__":
    sys.exit(main())
