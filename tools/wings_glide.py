#!/usr/bin/env python3
"""The cut top-down Charity Wings glide of the Japanese demo (SLPM-80165), rebuilt from the demo's own disc files.

The demo's overhead Tomba sprite set has 22 frames retail never kept (docs/CUT_CONTENT.md, "Top-down Tomba"); frames
77, 78 and 79 are Tomba with the right arm raised, the pose of holding the Charity Wings feather overhead, and no player
script of the demo uses them.  The feather item sprite (retail item 7, resident SPR table 0x34 frames 159 static, 160 and
161 flapping) is on the demo's resident item page at the same page coordinates as in retail, with the same palette.
This tool puts them together: the feather's quill is pinned to the raised hand (positions measured from the pixels, not
guessed), the feather turns like a rotor through its three frames and their mirror images, Tomba cycles 77-78-79-78
and the whole figure floats.  Only the assembly (spin order, 70 ms per phase, the +-2 px float) is ours; every pixel and
every position comes from the disc.

  python3 -I tools/wings_glide.py [--demo game_jp_demo/fs] [--out OUT_DIR] [--zoom 5]

Writes OUT_DIR/wings_glide_x<zoom>.gif, wings_glide_1x.gif, wings_glide_steps.png (the 8 phases of one spin) and the
three feather frames as PNGs.  The PNG/GIF outputs are disc graphics: never commit them (CI keeps them as artifacts).

Data used (all paths inside the extracted demo disc):
 - Tomba top-down frames: AREA08/D002.000 +0x3FA28 (WPP table, 106 frames), SPR parts AREA08/D003.GAM (unpacked, table
   at +0, 106 frames, two parts per frame with CLUT rows 0 and 1), palette rows 0/1 of AREA08/CLUT01.GAM (256 words per row);
 - feather: SYSTEM/A00000.GAM (unpacked, a 4 bpp page 512 px wide): frame 159 at (224,104) 24x24, frame 160 at (200,80)
   24x16, frame 161 at (216,64) 16x16; palette row 11 of SYSTEM/A00002.000 (16 rows of 16 colours = retail SYSTEM/A10004.000
   at VRAM (352,480), so the feather's CLUT word is 0x7AD6 = (352,491)).  PrePre 9 holds the same three frames in
   TOMBA/A00002.GAM (256 px wide page) at the same coordinates, with the same row 11 in TOMBA/A00003.000.
"""
import argparse, math, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import area000 as A      # noqa: E402
import vram              # noqa: E402
import wpp               # noqa: E402

TD_WPP, TD_WPP_OFF, TD_SPR, TD_CLUT = "AREA08/D002.000", 0x3FA28, "AREA08/D003.GAM", "AREA08/CLUT01.GAM"
FEATHER_PAGE, FEATHER_CLUT, FEATHER_ROW = "SYSTEM/A00000.GAM", "SYSTEM/A00002.000", 11
FEATHER_FRAMES = {159: (224, 104, 24, 24), 160: (200, 80, 24, 16), 161: (216, 64, 16, 16)}
TOMBA_FRAMES = [77, 78, 79, 78]
PHASE_MS = 70            # one spin phase; a full turn is 8 phases
FLOAT_PX = 2             # vertical float amplitude
SPIN_STEPS = 32          # frames of the GIF (one float cycle)


def unpack(path):
    d = open(path, "rb").read()
    return A.gam_unpack(d) if d[:4] == b"GAM\0" else d


def rgb(v):
    return tuple(A.rgb555(v))


def topdown_frames(fs):
    """{frame: RGBA image} of the demo's overhead Tomba set, each part coloured with its own CLUT row."""
    from PIL import Image
    d = open(os.path.join(fs, TD_WPP), "rb").read()[TD_WPP_OFF:]
    n = struct.unpack_from("<I", d)[0]
    offs = struct.unpack_from("<%dI" % (n + 1), d, 4)
    frames = [wpp.wpp_decode(d[offs[k]:offs[k + 1]]) for k in range(n)]
    parts = vram.parse_spr(unpack(os.path.join(fs, TD_SPR)))
    clut = unpack(os.path.join(fs, TD_CLUT))
    pals = {}

    def pal(r):
        if r not in pals:
            pals[r] = [(0, 0, 0, 0)] + [rgb(v) + (255,) for v in struct.unpack_from("<15H", clut, (r * 256 + 1) * 2)]
        return pals[r]

    out = {}
    for i in TOMBA_FRAMES:
        rows, w, h = vram.wpp_plane(frames[i])
        ps = parts[i]
        r0 = ((ps[0][2] >> 6) & 0x1FF) - 480 if ps else 0
        px = [pal(r0)[b] for r in rows for b in r]
        for p in ps[1:]:
            pr = ((p[2] >> 6) & 0x1FF) - 480
            if pr == r0 or not 0 <= pr < 32 or not p[8] or not p[9]:
                continue
            u0, v0 = min(p[0], p[3], p[6], p[10]), min(p[1], p[4], p[7], p[11])
            for y in range(max(v0, 0), min(v0 + p[9], h)):
                for x in range(max(u0, 0), min(u0 + p[8], w)):
                    px[y * w + x] = pal(pr)[rows[y][x]]
        im = Image.new("RGBA", (w, h))
        im.putdata(px)
        out[i] = im
    return out


def feather_frames(fs):
    """{159: image, 160: image, 161: image} cut from the resident item page with the item palette row 11."""
    from PIL import Image
    page = unpack(os.path.join(fs, FEATHER_PAGE))
    W = len(page) // 256                      # bytes per row (2 px per byte)
    clut = open(os.path.join(fs, FEATHER_CLUT), "rb").read()
    pal = [rgb(v) for v in struct.unpack_from("<16H", clut, FEATHER_ROW * 32)]
    out = {}
    for n, (x0, y0, w, h) in FEATHER_FRAMES.items():
        im = Image.new("RGBA", (w, h))
        px = []
        for y in range(y0, y0 + h):
            for x in range(x0, x0 + w):
                i = (page[y * W + x // 2] >> (4 if x & 1 else 0)) & 15
                px.append(pal[i] + (0 if i == 0 else 255,))
        im.putdata(px)
        out[n] = im
    return out


def hand_top(im):
    """(x, y) of the topmost opaque pixel of the raised hand: the right third of the sprite."""
    px = im.load()
    for y in range(im.height):
        xs = [x for x in range(im.width * 2 // 3, im.width) if px[x, y][3]]
        if xs:
            return (sum(xs) // len(xs), y)
    return (im.width - 4, 0)


def quill(im):
    """(x, y) of the lowest opaque pixel of a feather frame: the end of the quill."""
    px = im.load()
    for y in range(im.height - 1, -1, -1):
        xs = [x for x in range(im.width) if px[x, y][3]]
        if xs:
            return (sum(xs) // len(xs), y)
    return (im.width // 2, im.height - 1)


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--demo", default=os.path.join(os.path.dirname(HERE), "game_jp_demo", "fs"))
    ap.add_argument("--out", default="out")
    ap.add_argument("--zoom", type=int, default=5)
    a = ap.parse_args(argv)
    from PIL import Image, ImageDraw, ImageOps
    tomba = topdown_frames(a.demo)
    fe = feather_frames(a.demo)
    os.makedirs(a.out, exist_ok=True)
    for n, im in fe.items():
        im.save(os.path.join(a.out, "feather_%d.png" % n))
    # the rotor: flat, angled, edge-on, angled mirrored, flat mirrored, angled mirrored, edge-on, angled (8 phases)
    cycle = [fe[159], fe[160], fe[161], ImageOps.mirror(fe[160]), ImageOps.mirror(fe[159]), ImageOps.mirror(fe[160]), fe[161], fe[160]]
    hands = {f: hand_top(tomba[f]) for f in set(TOMBA_FRAMES)}
    print("hand top per frame:", hands, "; quill end per feather frame:", {n: quill(im) for n, im in fe.items()})
    W, H = 72, 84

    def frame(step, z):
        f = TOMBA_FRAMES[(step // 2) % len(TOMBA_FRAMES)]
        ft = cycle[step % len(cycle)]
        bob = round(FLOAT_PX * math.sin(2 * math.pi * step / SPIN_STEPS))
        cv = Image.new("RGBA", (W, H), (40, 40, 40, 255))
        tx, ty = 16, 30 + bob
        hx, hy = tx + hands[f][0], ty + hands[f][1]
        qx, qy = quill(ft)
        cv.alpha_composite(ft, (hx - qx, hy - 1 - qy))      # quill end 1 px above the hand's top pixel
        cv.alpha_composite(tomba[f], (tx, ty))
        return cv if z == 1 else cv.resize((W * z, H * z), Image.NEAREST)

    for z, name in ((a.zoom, "wings_glide_x%d.gif" % a.zoom), (1, "wings_glide_1x.gif")):
        seq = [frame(s, z).convert("P") for s in range(SPIN_STEPS)]
        seq[0].save(os.path.join(a.out, name), save_all=True, append_images=seq[1:], duration=PHASE_MS, loop=0)
    z = 3
    sheet = Image.new("RGBA", (8 * (W * z + 6), H * z + 20), (40, 40, 40, 255))
    dr = ImageDraw.Draw(sheet)
    for k in range(8):
        sheet.alpha_composite(frame(k, z), (k * (W * z + 6), 0))
        dr.text((k * (W * z + 6) + 4, H * z + 4), "phase %d: Tomba %d" % (k, TOMBA_FRAMES[(k // 2) % 4]), fill=(255, 255, 255, 255))
    sheet.save(os.path.join(a.out, "wings_glide_steps.png"))
    print("written to", a.out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
