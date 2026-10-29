"""CARGAR / GUARDAR: the Spanish redraw of the memory card screen title (LOAD / SAVE), --lang 5, port only.

The originals are 40x16 sprites at u=168 v=136 (LOAD) / v=120 (SAVE) of the SYSTEM/A00400 page (768x256 4bpp at VRAM x 64,
CLUT (304,499)). C, G, R, U are drawn on the letters' horizontal colour bands; A and D are cut from LOAD. CARGAR (64x16)
goes to u=0 v=160 and GUARDAR (72x16) to u=0 v=176, free space of the same page (port/fixes.txt: memCardDrawModeTitle).
"""
VRAM_X = 64
BAND = {4: 9, 5: 7, 6: 7, 7: 6, 8: 6, 9: 6, 10: 1, 11: 1, 12: 8, 13: 8}   # row -> colour of the letters' bands
AA = {9: 6, 7: 6, 6: 5, 1: 5, 8: 5}                                        # band colour -> its antialias colour

C = [".+######", "+#######", "##+.....", "##......", "##......", "##......", "##......", "###.....", "########", ".#######"]
G = [".+######", "+#######", "##+.....", "##......", "##..####", "##..####", "##....##", "###..###", "########", ".######."]
R = ["######..", "#######.", "##...###", "##....##", "##...###", "#######.", "######..", "##..###.", "##...###", "##....##"]
U = ["##....##"] * 7 + ["###..###", "########", ".######."]


def build(page):
    """page: SYSTEM/A00400.000 as stored (384 bytes a row). Returns [(vram_x, vram_y, w_words, h, words)]."""
    idx = lambda u, v: (page[v*384+(u >> 1)] >> (4*(u & 1))) & 15

    def tomask(v0, u0=168, w=40):
        m = []
        for r in range(16):
            s = ''
            for x in range(w):
                c = idx(u0+x, v0+r); b = BAND.get(r)
                if c in (1, 6, 7, 8, 9) and c == b: s += '#'
                elif b and c == AA[b] and c != 5: s += '+'
                elif c in (1, 6, 7, 8, 9): s += '?'
                else: s += '.'
            m.append(s)
        return m

    def render(m):
        w = len(m[0]); out = []
        end = max(x for r in m for x, ch in enumerate(r) if ch in '#+') + 1          # the band stops at the shadow
        ink = lambda a, b: 0 <= b < len(m) and 0 <= a < w and m[b][a] in '#+'
        for r in range(16):
            row = []
            for x in range(w):
                ch = m[r][x]
                if ch == '#': row.append(BAND[r])
                elif ch == '+': row.append(AA[BAND[r]])
                elif ink(x-1, r) or (ink(x, r-1) and (ink(x-1, r-1) or r == 14)): row.append(5)     # drop shadow
                else: row.append(12 if r >= 10 and x <= end else 0)
            out.append(row)
        return out

    load = tomask(136)
    cut = lambda a, b: [r[a:b+1].replace('?', '#') for r in load[4:14]]
    A, D = cut(21, 29), cut(31, 38)

    def narrow(L, w=7):                                          # drop repeated interior columns down to w wide
        while len(L[0]) > w:
            i = next((i for i in range(2, len(L[0]) - 2) if all(r[i] == r[i+1] for r in L)), len(L[0]) // 2)
            L = [r[:i] + r[i+1:] for r in L]
        return L

    def word(ls, W, lw):                                         # lw-wide letters, 1 px gap (the shadow): fits left of the dialog box
        rows = [''] * 10
        for L in (narrow(L, lw) for L in ls):
            for i in range(10): rows[i] += L[i] + '.'
        x0 = 1                                                  # left edge of the ink at column 1, as in LOAD
        rows = ['.' * x0 + r for r in rows]; rows = [(r + '.' * W)[:W] for r in rows]
        return ['.' * W] * 4 + rows + ['.' * W] * 2

    out = []
    for g, v in ((render(word([C, A, R, G, A, R], 64, 8)), 160), (render(word([G, U, A, R, D, A, R], 72, 7)), 176)):
        words = [sum(r[x+k] << (4*k) for k in range(4)) for r in g for x in range(0, len(r), 4)]
        out.append((VRAM_X, v, len(g[0]) // 4, 16, words))
    return out
