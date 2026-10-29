"""VUELVE A / INTENTARLO: the Spanish redraw of the TRY AGAIN prompt (--lang 5, port only).

Built from the user's own SYSTEM/A10003.GAM (the 256x256 4bpp page the TRY / AGAIN sprites are cut from, CLUT (352,509)):
the letters A G A I N T R are cut from the original sprites and recoloured, V U E L O are drawn in the same chunky,
hand-painted style (thick wobbly strokes supersampled 8x, palette-shaded like the originals). The result is one 116x32
block (two lines) at VRAM (896,104) = tpage 0x0E u 0 v 104, free while the prompt shows; drawNowLoading draws it as one
sprite (port/fixes.txt). Needs Pillow.
"""
import math, random, struct

VRAM_X, VRAM_Y = 896, 104
HL = {13, 15}
FAM = {'red': (8, 9), 'yellow': (5, 4), 'orange': (4, 14), 'pink': (1, 10), 'green': (6, 11), 'blue': (7, 12)}
S = 8


def comps(px, x0, y0, w, h):
    """8-connected components of the non-zero pixels of a rect"""
    seen = set(); out = []
    for y in range(y0, y0+h):
        for x in range(x0, x0+w):
            if not px(x, y) or (x, y) in seen: continue
            st = [(x, y)]; seen.add((x, y)); c = []
            while st:
                a, b = st.pop(); c.append((a, b))
                for dx in (-1, 0, 1):
                    for dy in (-1, 0, 1):
                        p = (a+dx, b+dy)
                        if x0 <= p[0] < x0+w and y0 <= p[1] < y0+h and p not in seen and px(*p): seen.add(p); st.append(p)
            out.append(c)
    return out


def cut_letters(px):
    def cut(c, keep=lambda p, v: True):
        m = {p: px(*p) for p in c if keep(p, px(*p))}
        x0 = min(p[0] for p in m); return {(x-x0, y): v for (x, y), v in m.items()}
    t1 = comps(px, 184, 192, 40, 16); t2 = comps(px, 184, 208, 56, 16)
    big = max(t1, key=len)
    L = {}
    L['T'] = cut(big, lambda p, v: v in (8, 9) or (v in HL and p[0] <= 198))
    L['R'] = cut(big, lambda p, v: not (v in (8, 9) or (v in HL and p[0] <= 198)))
    L['Y'] = cut([c for c in t1 if c is not big][0])
    a1, g, a2, i, n = sorted(t2, key=lambda c: min(p[0] for p in c))
    L['A1'], L['G'], L['A2'], L['I'], L['N'] = map(cut, (a1, g, a2, i, n))
    for k in L:   # y relative to the line top (192 / 208)
        top = 192 if k in 'TRY' else 208
        L[k] = {(x, y-top): v for (x, y), v in L[k].items()}
    return L


def recolour(m, fam):
    cnt = {}
    for v in m.values():
        if v not in HL: cnt[v] = cnt.get(v, 0) + 1
    fill = max(cnt, key=cnt.get); f, s = FAM[fam]
    hl = 15 if fam == 'blue' else 13
    return {p: (hl if v in HL else f if v == fill else s) for p, v in m.items()}


def kern(row, gap=1, space=5, DY=(-1, 0, 1)):
    """place each letter as far left as possible with `gap` empty pixels to everything already placed"""
    out = {}; x = 0; sp = 0
    for m in row:
        if m is None: sp = max(p[0] for p in out) + 1 + space; continue
        occ = {(a+dx, b+dy) for (a, b) in out for dx in range(-gap, gap+1) for dy in DY}
        lo = (max(p[0] for p in out) - 12) if out else 0
        ox = max(lo, x - 6, sp)
        while any((a+ox, b) in occ for (a, b) in m): ox += 1
        for (a, b), v in m.items(): out[(a+ox, b)] = v
        x = ox + 1
    return out


def render(strokes, w, h, rot, seed, wid):
    """strokes: polylines in letter units (x right, y down, box w x h). Thick round strokes, jittered, rotated."""
    from PIL import Image, ImageDraw
    rnd = random.Random(seed)
    im = Image.new('L', ((w+6)*S, (h+6)*S), 0); dr = ImageDraw.Draw(im)
    cx, cy = w/2, h/2; ca, sa = math.cos(math.radians(rot)), math.sin(math.radians(rot))
    def tf(p):
        x, y = p[0]+rnd.uniform(-.35, .35)-cx, p[1]+rnd.uniform(-.35, .35)-cy
        return ((x*ca-y*sa+cx+3)*S, (x*sa+y*ca+cy+3)*S)
    for pl in strokes:
        sw = wid*rnd.uniform(.9, 1.12)
        pts = [tf(p) for p in pl]
        for a, b in zip(pts, pts[1:]):     # densified so the width varies along the stroke (blobby)
            n = max(2, int(math.dist(a, b)/S*2))
            for k in range(n+1):
                t = k/n; x = a[0]+(b[0]-a[0])*t; y = a[1]+(b[1]-a[1])*t
                r = sw*S/2*(1+.12*math.sin(t*5+seed))
                dr.ellipse((x-r, y-r, x+r, y+r), fill=255)
    small = im.resize((w+6, h+6), Image.BOX)
    m = {(x, y) for y in range(h+6) for x in range(w+6) if small.getpixel((x, y)) >= 128}
    for _ in range(2):   # clean: drop spurs, fill pinholes
        m = {p for p in m if sum((p[0]+dx, p[1]+dy) in m for dx, dy in ((1,0),(-1,0),(0,1),(0,-1))) >= 2}
        xs = range(min(p[0] for p in m), max(p[0] for p in m)+1); ys = range(min(p[1] for p in m), max(p[1] for p in m)+1)
        m |= {(x, y) for x in xs for y in ys if (x, y) not in m and sum((x+dx, y+dy) in m for dx, dy in ((1,0),(-1,0),(0,1),(0,-1))) >= 4}
    x0 = min(p[0] for p in m); y0 = min(p[1] for p in m)
    return {(x-x0, y-y0) for x, y in m}


def colour(body, fam, seed, dy):
    rnd = random.Random(seed)
    f, s = FAM[fam]; hl = 15 if fam == 'blue' else 13
    h = max(y for _, y in body)+1; m = {}
    for (x, y) in body:
        left = (x-1, y) not in body; below = (x, y+1) not in body; right = (x+1, y) not in body
        m[(x, y)] = s if (left and rnd.random() < .8) or (below and (left or right or rnd.random() < .5)) else f
    for y in range(1, int(h*.55)+1):   # white streak: 2nd pixel of the leftmost run, upper ~55%
        xs = sorted(x for (x, yy) in body if yy == y)
        if len(xs) >= 3 and xs[1] == xs[0]+1 and xs[2] == xs[0]+2:
            m[(xs[0]+1, y)] = hl
            if y == 2 and len(xs) >= 4 and xs[3] == xs[0]+3: m[(xs[0]+2, y)] = hl
    return {(x, y+dy): v for (x, y), v in m.items()}


G = {   # letter skeletons (stroke centres), box ~ w x 10
 'V': ([[(0, 0), (1.2, 4.5), (3.6, 9.4)], [(3.6, 9.4), (5.6, 4.8), (7.4, 0.2)]], 8, 10),
 'U': ([[(0, 0), (0.1, 6.8), (1.6, 9.4), (3.8, 9.8), (5.8, 8.8), (6.5, 6), (6.6, 0)]], 7, 10),
 'E': ([[(5.8, 0.2), (0.4, 0), (0, 4.6), (0.2, 9.6), (6, 9.8)], [(0.2, 4.8), (4.6, 4.6)]], 6, 10),
 'L': ([[(0.2, 0), (0, 5), (0.4, 9.6), (5.6, 9.8)]], 6, 10),
 'O': ([[(3.6, 0), (6.4, 1.2), (7.2, 5), (6.2, 8.8), (3.4, 9.9), (0.8, 8.6), (0, 4.6), (1.0, 1.0), (3.6, 0)]], 7, 10),
}


def letter(ch, fam, seed, rot, dy, wid=3.7):
    st, w, h = G[ch]; k = 1 if ch == 'E' else .85; st = [[(x, y*k) for x, y in pl] for pl in st]; h = round(10*k)
    return colour(render(st, w, h, rot, seed, wid), fam, seed, dy)


def build(page):
    """page: the unpacked A10003 page (256x256 4bpp, 32768 bytes). Returns [(vram_x, vram_y, w_words, h, words)]."""
    px = lambda x, y: (page[y*128+(x >> 1)] >> (4*(x & 1))) & 15
    L = cut_letters(px); rc = recolour
    line1 = [letter('V', 'pink', 3, -6, 1), letter('U', 'green', 7, 5, 2), letter('E', 'yellow', 11, -4, 1, 3.0),
             letter('L', 'orange', 5, 7, 2), letter('V', 'blue', 9, 4, 1), letter('E', 'red', 13, -6, 2, 3.0), None,
             rc(L['A1'], 'pink')]
    line2 = [L['I'], L['N'], L['T'], letter('E', 'yellow', 21, 5, 2, 3.0), rc(L['N'], 'green'), rc(L['T'], 'pink'),
             L['A2'], rc(L['R'], 'red'), letter('L', 'blue', 23, -5, 2), letter('O', 'orange', 29, 8, 2)]
    r1, r2 = kern(line1), kern(line2, gap=1, DY=(0,))
    wd = lambda r: max(p[0] for p in r)+1
    top = lambda r: min(p[1] for p in r)
    W = max(112, (max(wd(r1), wd(r2))+2+3)//4*4); H = 32
    g = [[0]*W for _ in range(H)]
    for r, y0 in ((r1, 1 - top(r1)), (r2, 17 - top(r2))):
        x0 = (W-wd(r))//2
        for (x, y), v in r.items():
            g[y+y0][x+x0] = v
    words = [sum(g[y][xw*4+k] << (4*k) for k in range(4)) for y in range(H) for xw in range(W//4)]
    return [(VRAM_X, VRAM_Y, W//4, H, words)]
