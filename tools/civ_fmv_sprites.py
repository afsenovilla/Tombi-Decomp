#!/usr/bin/env python3
"""RECONSTRUCTION: top-down sprites for the three cut Civilization residents seen only in MOVIE/BUNMEI.STR.

  python3 tools/civ_fmv_sprites.py [--out DIR] [--disc game/disc/Tombi.bin] [--town /dev/shm/maps/AREA16/picture.png]

Their sprite pages did not survive. Method (docs/CIVILIZATION.md, "FMV residents"):
 1. Still: the camera pans at a constant 8 px/frame over a static resident, so every frame of the range is aligned to
    the reference frame (SSD search +-3 px around the predicted offset on a core box, frames with a bad match
    dropped) and the per-pixel median is taken, 1 FMV px = 1 picture px (the towns' scale; the figures are already
    35-45 px tall, like the A07600/A08000 residents, so nothing is rescaled).
 2. Sprite: drawn FROM that still, pixel by pixel, in the resident's real 16-colour row (index 0 transparent): same
    silhouette, pose, proportions, clothes and hair, 1-px dark outline, eyes, flat shading bands. The grids are the
    hand-authored data in civ_fmv_sprites.json (labelled RECONSTRUCTION). No pixels are taken from other sprites.
Only the view the FMV shows is drawn (one standing pose each). The purple-dressed resident is cut by the left screen
edge in every frame: her front half (face, chest, front shoe) is continued by hand.

Output (contains disc art, never commit): DIR/<name>_sheet.png (FMV still 4x | sprite 4x | sprite 1x on the town
picture | palette), DIR/<name>.png (indexed sprite), DIR/contact.png.
"""
import argparse, json, os, sys
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import numpy as np
import civ_sheets as S
import str as STR
from PIL import Image, ImageDraw

DATA = json.load(open(os.path.join(HERE, 'civ_fmv_sprites.json')))


def grid(ch):
    g = ch['grid']; w = max(map(len, g))
    return [[0 if c == '.' else int(c, 16) for c in r.ljust(w, '.')] for r in g]


def to_img(f, P):
    h, w = len(f), len(f[0])
    im = Image.new('P', (w, h)); im.putpalette([c for rgb in P for c in rgb] + [0] * (768 - 48))
    im.putdata([v for r in f for v in r]); im.info['transparency'] = 0
    return im


def stills(disc):
    lba, size = STR.str_files(disc)['MOVIE/BUNMEI.STR']
    want = {n for ch in DATA['chars'].values() for n in range(*ch['fmv']['frames'])}; got = {}
    for num, w, h, bs in STR.frames(disc, lba, size):
        if num in want: got[num] = np.frombuffer(bytes(STR.decode_frame(bs, w, h)), np.uint8).reshape(h, w, 3).astype(float)
        if num >= max(want): break
    res = {}
    for name, ch in DATA['chars'].items():
        f = ch['fmv']; ref = got[f['ref']]; x0, y0, x1, y1 = f['box']; c0, d0, c1, d1 = f['core']
        T = ref[d0:d1, c0:c1]; st = []
        for n in range(*f['frames']):
            if n not in got: continue
            F = got[n]; best = None; px, py = f['vel'][0] * (n - f['ref']), f['vel'][1] * (n - f['ref'])
            for dy in range(py - 3, py + 4):
                for dx in range(px - 3, px + 4):
                    if x0 + dx < 0 or y0 + dy < 0 or x1 + dx > F.shape[1] or y1 + dy > F.shape[0]: continue
                    e = ((F[d0 + dy:d1 + dy, c0 + dx:c1 + dx] - T) ** 2).mean()
                    if best is None or e < best[0]: best = (e, dx, dy)
            if best and best[0] < 900: st.append(F[y0 + best[2]:y1 + best[2], x0 + best[1]:x1 + best[1]])
        m = Image.fromarray(np.median(np.array(st), 0).clip(0, 255).astype(np.uint8))
        res[name] = (m.crop(tuple(f['fig'])), len(st))
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--out', default='build/civ_fmv_sprites')
    ap.add_argument('--disc', default=os.path.join(HERE, '..', 'game', 'disc', 'Tombi.bin'))
    ap.add_argument('--town', default='/dev/shm/maps/AREA16/picture.png', help='town picture for the 1x preview (tools/civ_rebuild.py)')
    a = ap.parse_args(); os.makedirs(a.out, exist_ok=True)
    town = Image.open(a.town).convert('RGB') if os.path.exists(a.town) else None
    st = stills(a.disc); fnt = S.F(14, True); fs = S.F(11); sheets = []; s = 4
    for name, ch in DATA['chars'].items():
        P = S.clut_row(DATA['clut'], ch['row']); g = grid(ch); spr = to_img(g, P); spr.save(os.path.join(a.out, name + '.png'), transparency=0)
        fm, nf = st[name]; sr = spr.convert('RGBA')
        tile = None
        if town:
            tile = town.crop((170, 230, 330, 350)); tile.paste(sr, (40, 50), sr); tile.paste(sr.transpose(Image.FLIP_LEFT_RIGHT), (100, 40), sr.transpose(Image.FLIP_LEFT_RIGHT))
        H = max(fm.height, spr.height) * s
        W = fm.width * s + spr.width * s + (tile.width * 2 if tile else 0) + 400
        W = max(W, 1150); sh = Image.new('RGB', (W, H + 60), (235, 235, 235)); d = ImageDraw.Draw(sh)
        d.text((10, 6), '%s: RECONSTRUCTION drawn from BUNMEI.STR (%s/%s row %d) - %s' % (name, *DATA['clut'].split('/'), ch['row'], ch['desc']), fill=(0, 0, 0), font=fnt)
        x, y = 10, 28
        sh.paste(fm.resize((fm.width * s, fm.height * s), Image.NEAREST), (x, y)); d.text((x, y + H + 2), 'FMV median of %d frames, 4x' % nf, fill=(0, 0, 0), font=fs); x += max(fm.width * s, 150) + 16
        bg = Image.new('RGB', spr.size, (60, 140, 60)); bg.paste(sr, (0, 0), sr)
        sh.paste(bg.resize((spr.width * s, spr.height * s), Image.NEAREST), (x, y)); d.text((x, y + H + 2), 'sprite 4x', fill=(0, 0, 0), font=fs); x += max(spr.width * s, 70) + 16
        if tile:
            sh.paste(tile.resize((tile.width * 2, tile.height * 2), Image.NEAREST), (x, y)); d.text((x, y + H + 2), '1x on the AREA16 picture (shown 2x)', fill=(0, 0, 0), font=fs)
            x += tile.width * 2 + 16
        for i, rgb in enumerate(P):
            d.rectangle([x + (i % 4) * 30, y + (i // 4) * 30, x + (i % 4) * 30 + 26, y + (i // 4) * 30 + 26], fill=rgb, outline=(0, 0, 0))
            d.text((x + (i % 4) * 30 + 2, y + (i // 4) * 30 + 2), '%x' % i, fill=(255, 0, 255), font=fs)
        sh.save(os.path.join(a.out, name + '_sheet.png')); sheets.append(sh); print(name, '%dx%d' % spr.size, 'row', ch['row'], nf, 'frames')
    W = max(i.width for i in sheets); ct = Image.new('RGB', (W, sum(i.height for i in sheets)), (235, 235, 235)); y = 0
    for i in sheets: ct.paste(i, (0, y)); y += i.height
    ct.save(os.path.join(a.out, 'contact.png')); print('->', a.out)


if __name__ == '__main__':
    main()
