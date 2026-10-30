#!/usr/bin/env python3
"""RECONSTRUCTION: top-down sprites for the three cut Civilization residents seen only in MOVIE/BUNMEI.STR.

  python3 tools/civ_fmv_sprites.py [--out DIR] [--disc game/disc/Tombi.bin]

Their sprite pages did not survive, so these are NOT original sprites. Each one is rebuilt from a surviving resident
of the same scale (the frames, outline style and shading of a real page) recoloured with the real CLUT row the
community palette strips matched to that FMV resident (docs/CIVILIZATION.md, "Real resident palettes"):

  redshirt  red shirt, curly orange hair, pale green shorts (BUNMEI frames 85-89)  <- worker of SYSTEM/A06100 (30 frames)
            AREA16/CLUT07 row 54 (= AREA17/CLUT03 row 49); the hard hat becomes the hair puff
  purple    orange hair, purple dress, red shoes, seen from the side (frames 62-68) <- Stonetown woman of A07600 (27 frames)
            AREA16/CLUT07 row 38 (= AREA17/CLUT03 row 33)
  bald      bald brown man in grey (frames 200-214)                                <- worker of A06100, hat -> bald scalp
            row 255 of every area CLUT file (AREA16/CLUT07 too; VRAM (368, 495)), the docs' old "row 15"

The hand-authored data is only the index remap tables below (source page index -> index of the target row, one table
for the head band, the body and the feet) and the FMV frame numbers / boxes. Every output pixel is a real page texel
re-indexed into a real 16-colour row: no invented colours, index 0 transparent. Frame order is the port's
(tools/civ_anim.py, port2/runtime/main.c port_civ_*): idle back/side/front, walk 3x6 (back, side, front; mirrored
frames as on the worker), talk. The FMV crops (median of the aligned frames, 1 FMV px = 1 px) are drawn next to the
sprites only for comparison. Everything is read from YOUR disc at run time; the PNG/GIF output contains disc art and
must not be committed.

Output: DIR/<name>_frames.png (all frames 1x, indexed), DIR/<name>_sheet.png (FMV crop + idle 4x + all frames),
DIR/<name>_walk_{back,side,front}.gif, DIR/<name>_talk.gif, DIR/contact.png.
"""
import argparse, os, sys
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import civ_sheets as S
import civ_anim as CA
import str as STR
from PIL import Image, ImageDraw

CLUT = 'AREA16/CLUT07'   # row 255 (the bald man's) is the same in every area's CLUT files
# FMV reference: (frames, box in the reference frame, per-frame offset) -> median crop
FMV = {
    'redshirt': (range(85, 90), (80, 88, 135, 150), 89, lambda n: (0, -8 * (n - 89))),
    'purple': (range(62, 69), (0, 12, 48, 76), 66, lambda n: (0, -8 * (n - 66))),
    'bald': (range(200, 215), (14, 122, 72, 186), 201, lambda n: (8 * (n - 201), 0)),
}

# remaps: {source index: target index}; 'head' applies above HEAD*height, 'feet' below FEET*height, 'body' elsewhere.
# Worker (A06100, AREA11/CLUT02 row 4): 3/4/5 hat + shoes (yellow..brown), b/c/d skin, 2 white, 9/a greys (pupils,
# trousers), 6/7/8 dark red (vest, sleeves), e/f green shirt.
# Woman (A07600, row 21): a/b/c blonde hair (and shoes), 3/4/5 skin, 1 black, 2 white, 6..9 lilac dress,
# d/e grey necklace, f orange belt.
CHARS = {
    'redshirt': dict(src='worker', row=54, head=0.40, feet=0.80,
                     body={2: 2, 0xb: 5, 0xc: 6, 0xd: 7, 0xe: 8, 0xf: 9, 6: 9, 7: 10, 8: 10, 9: 15, 0xa: 14, 3: 11, 4: 12, 5: 13},
                     headm={3: 11, 4: 11, 5: 13, 9: 1, 0xa: 1}, feetm={3: 9, 4: 10, 5: 1, 9: 6, 0xa: 6}, brim=12),
    'purple': dict(src='woman', row=38, head=0.45, feet=0.86,
                   body={1: 5, 2: 2, 3: 6, 4: 7, 5: 1, 6: 8, 7: 9, 8: 10, 9: 5, 0xa: 14, 0xb: 15, 0xc: 13, 0xd: 3, 0xe: 4, 0xf: 11},
                   headm={}, feetm={0xa: 11, 0xb: 12, 0xc: 13}),
    'bald': dict(src='worker', row=255, head=0.40, feet=0.80,
                 body={2: 15, 0xb: 5, 0xc: 7, 0xd: 6, 0xe: 11, 0xf: 8, 6: 12, 7: 10, 8: 3, 9: 3, 0xa: 12, 3: 5, 4: 7, 5: 6},
                 headm={3: 5, 4: 5, 5: 6, 9: 1, 0xa: 1}, feetm={3: 7, 4: 6, 5: 1}, brim=7),
}
TALK = {'worker': (21, 30), 'woman': (21, 27)}


def src_frames(src):
    """[(page, (x0,y0,x1,y1), flip)] in port frame order, from the real page (same boxes as tools/civ_anim.py)."""
    if src == 'worker':
        p = S.page('A06100'); c = S.comps(p); seq = CA.WORKER; pick = lambda k: c[k]
    else:
        p = S.page('A07600'); c = S.comps(p); seq = CA.WOMAN; pick = lambda k: c[k]
    return p, [(pick(abs(int(k))), isinstance(k, str) or int(k) < 0) for k in seq]


def build(name):
    ch = CHARS[name]; p, fr = src_frames(ch['src']); out = []
    for (x0, y0, x1, y1), flip in fr:
        w, h = x1 - x0 + 1, y1 - y0 + 1
        g = [[p[y0 + y][x0 + x] for x in range(w)] for y in range(h)]
        o = [[0] * w for _ in range(h)]
        for y in range(h):
            m = ch['headm'] if y < ch['head'] * h else ch['feetm'] if y >= ch['feet'] * h else {}
            for x in range(w):
                v = g[y][x]
                if not v: continue
                t = m.get(v, ch['body'].get(v, v))
                # the hat's inner dark line (brim) becomes the mid shade (hair / bald scalp); only the silhouette edge stays dark
                if 'brim' in ch and y < ch['head'] * h and v == 5 and all(
                        0 <= y + dy < h and 0 <= x + dx < w and g[y + dy][x + dx] for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    t = ch['brim']
                o[y][x] = t
        if flip: o = [r[::-1] for r in o]
        out.append(o)
    return out


def pal(row):
    return S.clut_row(CLUT, row)


def to_img(f, P, bg=None):
    h, w = len(f), len(f[0])
    im = Image.new('P', (w, h)); im.putpalette([c for rgb in P for c in rgb] + [0] * (768 - 48))
    im.putdata([v for r in f for v in r]); im.info['transparency'] = 0
    if bg is None: return im
    rgb = Image.new('RGB', (w, h), bg); rgb.paste(im.convert('RGBA'), (0, 0), im.convert('RGBA')); return rgb


def fmv_crops(disc):
    lba, size = STR.str_files(disc)['MOVIE/BUNMEI.STR']
    want = {n for v in FMV.values() for n in v[0]}; got = {}
    for num, w, h, bs in STR.frames(disc, lba, size):
        if num in want: got[num] = Image.frombytes('RGB', (w, h), bytes(STR.decode_frame(bs, w, h)))
        if num > max(want): break
    res = {}
    for k, (rng, (x0, y0, x1, y1), ref, off) in FMV.items():
        st = []
        for n in rng:
            dx, dy = off(n)
            if x0 + dx < 0 or y0 + dy < 0: continue
            b = got[n].crop((x0 + dx, y0 + dy, x1 + dx, y1 + dy)).tobytes(); st.append([tuple(b[i:i + 3]) for i in range(0, len(b), 3)])
        med = [tuple(sorted(q[i] for q in col)[len(col) // 2] for i in range(3)) for col in zip(*st)]
        im = Image.new('RGB', (x1 - x0, y1 - y0)); im.putdata(med); res[k] = im
    return res


def anim_gif(frames, P, path, bg, scale=4, ms=120):
    W = max(len(f[0]) for f in frames) + 8; H = max(len(f) for f in frames) + 4
    ims = []
    for f in frames:
        c = Image.new('RGB', (W, H), bg); im = to_img(f, P).convert('RGBA')
        c.paste(im, ((W - im.width) // 2, H - 2 - im.height), im)
        ims.append(c.resize((W * scale, H * scale), Image.NEAREST))
    ims[0].save(path, save_all=True, append_images=ims[1:], duration=ms, loop=0)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--out', default='build/civ_fmv_sprites'); ap.add_argument('--disc', default=os.path.join(HERE, '..', 'game', 'disc', 'Tombi.bin'))
    a = ap.parse_args(); os.makedirs(a.out, exist_ok=True)
    crops = fmv_crops(a.disc); bg = (40, 120, 40); sheets = []
    fnt = S.F(14, True); fs = S.F(11)
    for name, ch in CHARS.items():
        P = pal(ch['row']); fr = build(name); n = len(fr)
        # all frames 1x, indexed
        W = sum(len(f[0]) + 2 for f in fr); H = max(len(f) for f in fr)
        strip = Image.new('P', (W, H)); strip.putpalette([c for rgb in P for c in rgb] + [0] * (768 - 48)); x = 0
        for f in fr: strip.paste(to_img(f, P), (x, H - len(f))); x += len(f[0]) + 2
        strip.info['transparency'] = 0; strip.save(os.path.join(a.out, name + '_frames.png'), transparency=0)
        # GIFs
        for k, nm in enumerate(('back', 'side', 'front')):
            anim_gif(fr[3 + 6 * k: 9 + 6 * k], P, os.path.join(a.out, '%s_walk_%s.gif' % (name, nm)), bg)
        t0, t1 = TALK[ch['src']]; tk = fr[t0:t1]; per = len(tk) // 3
        for k, nm in enumerate(('back', 'side', 'front')):
            anim_gif(tk[k * per:(k + 1) * per] * 2, P, os.path.join(a.out, '%s_talk_%s.gif' % (name, nm)), bg, ms=200)
        # sheet: FMV crop 4x | idle back/side/front 4x | palette | all frames 2x
        c = crops[name]; s = 4
        idle = [to_img(fr[i], bg=bg, P=P) for i in (2, 1, 0)]
        top_w = c.width * s + 20 + sum(i.width * s + 12 for i in idle) + 140
        allw = sum(len(f[0]) * 2 + 4 for f in fr)
        SW = max(top_w, allw) + 20; SH = 28 + max(c.height, H) * s + 30 + H * 2 + 30
        sh = Image.new('RGB', (SW, SH), (235, 235, 235)); d = ImageDraw.Draw(sh)
        d.text((10, 6), '%s: RECONSTRUCTION (not original data). %s/%s row %d, frames rebuilt from the %s page' % (
            name, CLUT.split('/')[0], CLUT.split('/')[1], ch['row'], 'A06100 worker' if ch['src'] == 'worker' else 'A07600 woman'), fill=(0, 0, 0), font=fnt)
        x = 10; y = 28
        sh.paste(c.resize((c.width * s, c.height * s), Image.NEAREST), (x, y)); d.text((x, y + c.height * s + 2), 'BUNMEI.STR (median, 4x)', fill=(0, 0, 0), font=fs)
        x += c.width * s + 20
        for lab, im in zip(('front', 'side', 'back'), idle):
            sh.paste(im.resize((im.width * s, im.height * s), Image.NEAREST), (x, y + (H - im.height) * s)); d.text((x, y + H * s + 2), 'idle ' + lab, fill=(0, 0, 0), font=fs)
            x += im.width * s + 12
        for i, rgb in enumerate(P):
            d.rectangle([x + (i % 4) * 30, y + (i // 4) * 30, x + (i % 4) * 30 + 26, y + (i // 4) * 30 + 26], fill=rgb, outline=(0, 0, 0))
            d.text((x + (i % 4) * 30 + 2, y + (i // 4) * 30 + 2), str(i), fill=(255, 0, 255), font=fs)
        y += max(c.height, H) * s + 26; x = 10
        for f in fr:
            im = to_img(f, P, bg); sh.paste(im.resize((im.width * 2, im.height * 2), Image.NEAREST), (x, y + (H - im.height) * 2)); x += im.width * 2 + 4
        d.text((10, y + H * 2 + 4), 'frames 0-2 idle back/side/front, 3-20 walk back/side/front (6 each), %d-%d talk (port order, tools/civ_anim.py)' % (t0, t1 - 1), fill=(0, 0, 0), font=fs)
        sh.save(os.path.join(a.out, name + '_sheet.png')); sheets.append(sh)
        print(name, n, 'frames, row', ch['row'])
    W = max(i.width for i in sheets); ct = Image.new('RGB', (W, sum(i.height for i in sheets)), (235, 235, 235)); y = 0
    for i in sheets: ct.paste(i, (0, y)); y += i.height
    ct.save(os.path.join(a.out, 'contact.png')); print('->', a.out)


if __name__ == '__main__':
    main()
