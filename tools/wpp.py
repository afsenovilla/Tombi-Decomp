#!/usr/bin/env python3
"""Tombi! (PAL) WPP sprite frames (load kind 0x60/0x62 `.000` files, WPP_DATA[n]): decode, unpack to PNG, pack, check.
Format notes: notes/formats.md (section "WPP"). Decoder = FUN_800371c0 (src/FUN_800371c0.c) + FUN_80037454 (run fill).

Each table item is a flag-byte RLE: a flag byte, then 8 tokens (bit 0 first): bit 0 = literal byte, bit 1 = (count, value) = `count`
copies of `value`. The output is `u16 W, u16 H` then H rows of W 16-bit words of 4 bpp pixels (low nibble first): 4*W x H pixels,
index 0 = transparent. The game adds a TIM prefix and LoadImage()s it, the 16 colour palette is a CLUT in VRAM.

  tools/wpp.py info FILE                  frame table (sizes) of a WPP .000
  tools/wpp.py unpack FILE OUT [--clut NAME] [--row R] [--x0 N]
                                          manifest.json + frame_NNNN.4bpp (decoded stream) + frame_NNNN.png (indexed, index 0
                                          transparent). Palette: 16 colours of row R of CLUT block NAME (default: the first CLUTnn of
                                          the area, row 0, colour 0 = VRAM 128,480 = the Tombi palette; --x0 16 = next palette).
  tools/wpp.py pack DIR [-o OUT]          re-encode the frames (greedy RLE reproduces the original bytes)
  tools/wpp.py --check [PATH...]          unpack+pack every WPP file (default game/disc/fs): identical count
"""
import argparse, hashlib, json, os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import area000 as A


def wpp_decode(it):
    out, p, n = bytearray(), 0, len(it)
    while p < n:
        c = it[p]
        p += 1
        for b in range(8):
            if p >= n:
                return bytes(out)
            if c >> b & 1:
                out += bytes([it[p + 1]]) * it[p]
                p += 2
            else:
                out.append(it[p])
                p += 1
    return bytes(out)


def wpp_encode(d):
    toks, i, n = [], 0, len(d)
    while i < n:
        j = i
        while j < n and d[j] == d[i] and j - i < 255:
            j += 1
        if j - i >= 2:
            toks.append((1, bytes([j - i, d[i]])))
            i = j
        else:
            toks.append((0, d[i:i + 1]))
            i += 1
    out = bytearray()
    for k in range(0, len(toks), 8):
        g = toks[k:k + 8]
        out.append(sum(t << b for b, (t, _) in enumerate(g)))
        for _, x in g:
            out += x
    return bytes(out)


def frame_dims(o):
    w, h = struct.unpack_from("<HH", o)
    return w * 4, h


def frame_png(o, pal):
    w, h = struct.unpack_from("<HH", o)
    rows = []
    for y in range(h):
        line = bytearray()
        for v in o[4 + y * w * 2:4 + (y + 1) * w * 2]:
            line += bytes((v & 15, v >> 4))
        rows.append(bytes(line))
    return A.png_indexed(w * 4, h, rows, pal, True)


def is_wpp(nm):
    r = A.get_rec(nm)
    return r if r and (r[1] & 0xF0) == 0x60 else None


def unpack_wpp(d, nm, rec):
    man, files = A.unpack_bytes(d, nm, rec)
    frames = {}
    for pt in man["parts"]:
        for s in pt["sections"]:
            if "item_" in s["file"]:
                o = wpp_decode(files.pop(s["file"]))
                assert len(o) == 4 + 2 * struct.unpack_from("<HH", o)[0] * struct.unpack_from("<HH", o)[1]
                s["file"] = s["file"].replace("item_", "frame_").replace(".bin", ".4bpp")
                s["note"] = "wpp frame: u16 W words, u16 H, 4 bpp pixels"
                files[s["file"]] = frames[s["file"]] = o
    return man, files, frames


def pack_wpp(man, files):
    f = dict(files)
    for pt in man["parts"]:
        for s in pt["sections"]:
            if s["file"].endswith(".4bpp"):
                f[s["file"]] = wpp_encode(files[s["file"]])
    return A.pack_bytes(man, f)


def palette(nm, clut, row, x0):
    cr = {"name": clut, "w": 256} if clut else A.find_clut(nm)
    if not cr:
        return bytes(range(0, 256, 16)) * 3, "grey"
    blob = A.read_resource(cr["name"])
    w = cr.get("w", 256)
    return A.clut_palette(blob, row, w, x0, 16), "%s row %d colour %d" % (cr["name"], row, x0)


def cmd_info(a):
    for p in a.files:
        d = open(p, "rb").read()
        nm = A.rel_name(p)
        rec = is_wpp(nm)
        if not rec:
            print(nm, "not a WPP file")
            continue
        man, files, fr = unpack_wpp(d, nm, rec)
        ws = [frame_dims(o) for o in fr.values()]
        print("%s: %d frames, widths %d..%d px, heights %d..%d" % (nm, len(ws), min(w for w, _ in ws), max(w for w, _ in ws), min(h for _, h in ws), max(h for _, h in ws)))
    return 0


def cmd_unpack(a):
    d = open(a.file, "rb").read()
    nm = A.rel_name(a.file)
    rec = is_wpp(nm)
    if not rec:
        sys.exit("not a WPP file")
    man, files, frames = unpack_wpp(d, nm, rec)
    pal, desc = palette(nm, a.clut, a.row, a.x0)
    os.makedirs(a.out, exist_ok=True)
    for fn, b in files.items():
        open(os.path.join(a.out, fn), "wb").write(b)
    for fn, o in frames.items():
        open(os.path.join(a.out, fn.replace(".4bpp", ".png")), "wb").write(frame_png(o, pal))
    json.dump(man, open(os.path.join(a.out, "manifest.json"), "w"), indent=1)
    print("%s -> %s: %d frames, palette %s" % (a.file, a.out, len(frames), desc))
    return 0


def cmd_pack(a):
    man = json.load(open(os.path.join(a.dir, "manifest.json")))
    files = {s["file"]: open(os.path.join(a.dir, s["file"]), "rb").read() for p in man["parts"] for s in p["sections"]}
    out = pack_wpp(man, files)
    if a.o:
        open(a.o, "wb").write(out)
    print("packed %d bytes%s" % (len(out), " (identical)" if hashlib.sha1(out).hexdigest() == man["sha1"] else " (differs)"))
    return 0


def cmd_check(paths):
    todo = []
    for p in paths or [A.FS]:
        if os.path.isdir(p):
            for r, _, fs in os.walk(p):
                todo += [os.path.join(r, f) for f in fs if f.endswith(".000")]
        else:
            todo.append(p)
    ok = n = fr = 0
    for p in sorted(todo):
        nm = A.rel_name(p)
        rec = is_wpp(nm)
        if not rec:
            continue
        d = open(p, "rb").read()
        man, files, frames = unpack_wpp(d, nm, rec)
        n += 1
        fr += len(frames)
        if pack_wpp(man, files) == d:
            ok += 1
        else:
            print("  DIFF", nm)
    print("WPP files identical %d/%d (%d frames)" % (ok, n, fr))
    return 0 if ok == n else 1


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--check":
        return cmd_check(sys.argv[2:])
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("info"); p.add_argument("files", nargs="+"); p.set_defaults(f=cmd_info)
    p = sp.add_parser("unpack"); p.add_argument("file"); p.add_argument("out"); p.add_argument("--clut"); p.add_argument("--row", type=int, default=0); p.add_argument("--x0", type=int, default=0); p.set_defaults(f=cmd_unpack)
    p = sp.add_parser("pack"); p.add_argument("dir"); p.add_argument("-o"); p.set_defaults(f=cmd_pack)
    a = ap.parse_args()
    return a.f(a)


if __name__ == "__main__":
    sys.exit(main())
