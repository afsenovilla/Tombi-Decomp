"""CARGANDO: the Spanish redraw of the NOW LOADING logo (--lang 5, port only).

Built from the user's own SYSTEM/A00900.000 (the 4bpp page NOW LOADING is cut from, 64 words x 72 rows, CLUT at
(16,71) inside it). The letters A G A N D O are cut from the original logo; the R is drawn in the same style
(supersampled 8x, palette-shaded with the shared rim rules); the C is LOADING's own G with its bar removed (top half
mirrored), scaled x1.95 and rotated 12 degrees to follow the arc, then re-shaded so its rims stay 1 px. Each letter
keeps its own dark outline + drop shadow and overlaps its neighbour by 3 px on an arch, like the original.

The result is one 144x71 block at VRAM (0,72), right under NOW LOADING in the same texture page / CLUT, unused while the
loading screen shows; drawNowLoadingSprite draws it as sprite 0 (port/fixes.txt). Needs Pillow.
"""
import struct

VRAM_X, VRAM_Y = 0, 72
OUT_W, OUT_H = 144, 71
PW, PH = 112, 64          # logo area of the page
BG = {0, 2, 3, 10, 11}    # transparent / outline / shadow indices of the original logo
K = 8                     # supersampling


def _letters(raw):
    px = [[(raw[y*128+(x >> 1)] >> (4*(x & 1))) & 15 for x in range(PW)] for y in range(PH)]

    def comp(seed):
        st = [seed]; seen = {seed}
        while st:
            a, b = st.pop()
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                p = (a+dx, b+dy)
                if 0 <= p[0] < PW and 0 <= p[1] < PH and p not in seen and px[p[1]][p[0]] not in BG:
                    seen.add(p); st.append(p)
        return seen

    def auto_holes(c):
        xs = [p[0] for p in c]; ys = [p[1] for p in c]
        x0, x1, y0, y1 = min(xs)-1, max(xs)+1, min(ys)-1, max(ys)+1
        out = {(x0, y0)}; st = [(x0, y0)]
        while st:
            a, b = st.pop()
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                p = (a+dx, b+dy)
                if x0 <= p[0] <= x1 and y0 <= p[1] <= y1 and p not in out and p not in c: out.add(p); st.append(p)
        return {(x, y) for x in range(x0, x1+1) for y in range(y0, y1+1) if (x, y) not in out and (x, y) not in c}

    def near(p, cols, rad=3):
        x, y = p; best = 99
        for yy in range(y-rad, y+rad+1):
            for xx in range(x-rad, x+rad+1):
                if 0 <= xx < PW and 0 <= yy < PH and px[yy][xx] in cols: best = min(best, abs(xx-x)+abs(yy-y))
        return best

    def grab(seed, keep=None):
        c = comp(seed)
        if keep: c = {p for p in c if keep(p)}
        allp = c | auto_holes(c)
        x0 = min(p[0] for p in allp); y0 = min(p[1] for p in allp)
        return {(x-x0, y-y0): (px[y][x] if (x, y) in c else 2) for (x, y) in allp}

    return {
        'O': grab((32, 45)),
        'A': grab((48, 45), lambda p: near(p, {6, 15}) <= near(p, {7, 14})),
        'D': grab((60, 45), lambda p: near(p, {7, 14}) < near(p, {6, 15})),
        'N': grab((80, 45)),
        'G': grab((93, 47)),
    }


def _shade(m, fill, shd):
    cols = {}
    for v in m.values(): cols[v] = cols.get(v, 0) + 1
    old = sorted((c for c in cols if c not in (2, 9)), key=lambda c: -cols[c])
    mp = {old[0]: fill}
    for c in old[1:]: mp[c] = shd
    return {p: mp.get(v, v) for p, v in m.items()}


def _P(*pts): return [(x*K, y*K) for x, y in pts]


def _bez(p0, p1, p2, n=12):
    return [((1-t)**2*p0[0]+2*(1-t)*t*p1[0]+t*t*p2[0], (1-t)**2*p0[1]+2*(1-t)*t*p1[1]+t*t*p2[1])
            for t in [i/n for i in range(n+1)]]


def _ss_mask(draw_fn, w, h, shear=0.0):
    from PIL import Image, ImageDraw
    im = Image.new('L', (w*K, h*K), 0); draw_fn(ImageDraw.Draw(im))
    if shear: im = im.transform(im.size, Image.AFFINE, (1, shear, -shear*h*K, 0, 1, 0), Image.BICUBIC)
    sm = im.resize((w, h), Image.BOX)
    return {(x, y) for y in range(h) for x in range(w) if sm.getpixel((x, y)) >= 128}


def _colour(body, holes, fill, shd):
    m = {}
    for (x, y) in body:
        n4 = [(x+1, y), (x-1, y), (x, y+1), (x, y-1)]
        if any(q not in body and q not in holes for q in n4): m[(x, y)] = 9
        elif any(q in holes for q in n4): m[(x, y)] = shd
        elif (x+1, y) in body and (x+2, y) not in body and (x+2, y) not in holes: m[(x, y)] = shd
        else: m[(x, y)] = fill
    for p in holes: m[p] = 2
    return m


def _despike(body):
    for _ in range(2):
        body = {p for p in body if sum((p[0]+a, p[1]+b) in body for a, b in ((1, 0), (-1, 0), (0, 1), (0, -1))) >= 2}
    return body


def _upscale(m, sc):
    """supersampled scale of a letter: smooth silhouette, colours sampled from the source"""
    from PIL import Image, ImageFilter
    W0 = max(x for x, y in m)+1; H0 = max(y for x, y in m)+1
    im = Image.new('L', (W0*8, H0*8), 0); px = im.load()
    for (x, y) in m:
        for i in range(8):
            for j in range(8): px[x*8+i, y*8+j] = 255
    W1, H1 = round(W0*sc), round(H0*sc)
    sm = im.filter(ImageFilter.GaussianBlur(5)).resize((W1, H1), Image.BOX)
    out = {}
    for y in range(H1):
        for x in range(W1):
            if sm.getpixel((x, y)) >= 128:
                sx, sy = min(W0-1, int((x+0.5)/sc)), min(H0-1, int((y+0.5)/sc))
                v = m.get((sx, sy))
                if v is None:
                    v = min(m.items(), key=lambda kv: (kv[0][0]-sx)**2+(kv[0][1]-sy)**2)[1]
                out[(x, y)] = v
    return out


def _rot_mask(pts, deg, k=8):
    from PIL import Image, ImageDraw
    W0 = max(x for x, y in pts)+1; H0 = max(y for x, y in pts)+1
    im = Image.new('L', (W0*k, H0*k), 0); d = ImageDraw.Draw(im)
    for x, y in pts: d.rectangle((x*k, y*k, x*k+k-1, y*k+k-1), 255)
    im = im.rotate(deg, Image.BICUBIC, expand=True)
    W1, H1 = im.width//k, im.height//k; sm = im.resize((W1, H1), Image.BOX)
    out = {(x, y) for y in range(H1) for x in range(W1) if sm.getpixel((x, y)) >= 128}
    x0 = min(x for x, y in out); y0 = min(y for x, y in out)
    return {(x-x0, y-y0) for x, y in out}


def _orange_streak(m):
    for (x, y), v in list(m.items()):
        if v == 8 and m.get((x-1, y)) == 9: m[(x, y)] = 12
    return m


def _big_c(G):
    top = {(x, y): v for (x, y), v in G.items() if y <= 8}
    c = dict(top); c.update({(x, 17-y): v for (x, y), v in top.items()})
    c = {p: {6: 8, 15: 12}.get(v, v) for p, v in c.items()}
    c = _upscale(c, 1.95)
    return _orange_streak(_colour(_despike(_rot_mask(set(c), 12)), set(), 8, 12))


def _big_r():
    def draw(d):
        d.rectangle(_P((1, 0.5), (6.5, 20)), 255)
        d.rounded_rectangle(_P((1, 0.3), (15.5, 12.2)), radius=5.8*K, fill=255)
        d.polygon(_P(*(_bez((6.5, 10), (11.5, 12), (16.5, 20)) + _bez((11.3, 20), (8.8, 14.6), (6.5, 14)))), 255)

    def hole(d): d.rounded_rectangle(_P((6.3, 4), (11, 8.5)), radius=1.9*K, fill=255)
    body = _ss_mask(draw, 18, 21, shear=-0.12); hh = _ss_mask(hole, 18, 21, shear=-0.12)
    body = _despike(body - hh); hh = {p for p in hh if p not in body}
    r = _colour(body, hh, 7, 14); x0 = min(p[0] for p in r)
    return {(x-x0, y): v for (x, y), v in r.items()}


def _render(items, w, h, sh=3):
    """each letter with its own dark ring + drop shadow, composited back to front"""
    can = {}
    for m, ox, oy in items:
        for (x, y) in m:
            for k in range(sh+1):
                for dx in (-1, 0, 1):
                    for dy in (-1, 0, 1):
                        can[(ox+x+dx-(k+1)//3, oy+y+dy+k)] = 2
        for (x, y), v in m.items(): can[(ox+x, oy+y)] = v
    return [[can.get((x, y), 0) for x in range(w)] for y in range(h)]


def image(raw):
    """the 144x71 grid of palette indices"""
    L = _letters(raw)
    A1 = _upscale(_shade(L['A'], 5, 4), 1.18)
    A2 = _shade(L['A'], 1, 9)
    row = [_big_c(L['G']), A1, _big_r(), L['G'], A2, L['N'], L['D'], L['O']]
    base = [-14, 1, 1, 3, 4, 3, 6, 6]
    arc = [16, 7, 2, 0, 0, 2, 7, 16]
    gap = -3
    wd = lambda m: max(p[0] for p in m) + 1
    x = (OUT_W - (sum(wd(m) for m in row) + gap*7))//2; its = []
    for m, b, a in zip(row, base, arc):
        its.append((m, x, 12 + b + a)); x += wd(m) + gap
    return _render(its[::-1], OUT_W, OUT_H)


def key(raw):
    """the whole NOW LOADING page, uploaded as is at VRAM (0,0) (64 words x 72)"""
    from gfx_es import page_key
    return page_key(raw, 128, 0, 0, 0, 0, 256, 72)


def build(raw):
    g = image(raw)
    words = []
    for r in g:
        for x in range(0, OUT_W, 4):
            words.append(r[x] | r[x+1] << 4 | r[x+2] << 8 | r[x+3] << 12)
    return [(VRAM_X, VRAM_Y, OUT_W//4, OUT_H, words)]
