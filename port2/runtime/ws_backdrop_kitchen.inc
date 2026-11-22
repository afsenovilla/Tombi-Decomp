/* runtime/ws_backdrop.c: AREA18 s2, the witch's kitchen (gen_kt; included there; docs/PORT.md "More rooms with
 * picture strips").  An attic seen from inside: two rafters of the truss next to the camera cross the upper corners
 * (each cut by its edge), a log runs out through each top corner behind them, the cupboard's side stands under the
 * left rafter, a bunch of grapes hangs under the right one, the gritty floor below.
 *
 * Measured on the picture (i = columns in from the edge, both sides alike):
 *   - the rafters are parallel bands: the lit band's edges fit rows 81.74 / 91.98 at the right edge with slopes 1.160 /
 *     1.169 rows per column, 95.8 with 1.163 at the left one; its width stays 9.8-10.3 rows over 50 columns, so there
 *     is no vanishing point to speak of: the rafter is a translation along (6 columns, 7 rows).  The profile across it,
 *     averaged along that direction, is the same next to the edge and 20-32 columns in from edge row 55 (left) / 51
 *     (right) down to the lit band's lower edge;
 *   - the log's lower edge (luminance 50) fits row 22.1 + 0.411 i (left) / 12.1 + 0.432 i (right): a translation along
 *     (12 columns, 5 rows), rising outwards; its shadow follows it for 14 rows;
 *   - between the two, the roof's underside: the same grit as the roof between the beams at the top of the picture
 *     (columns 122-158, rows 0-16: relative contrast 0.19 against 0.19 / 0.15 in the corners), dark under the log;
 *   - the floor has no boards: an even grit (no direction in its structure tensor, so no plank line to continue),
 *     lighter in a pool round the fire, 2-5 % darker every 8 columns towards the edges;
 *   - the cupboard's side is a frontal panel of vertical grain, columns 0-47 (three rungs at columns 17-31), ending at
 *     the right in a lighter corner board (columns 40-47) and a dark edge (48-49), standing on row 181;
 *   - the grapes are one disc, 10 x 10 texels, lit top-left: the two the edge cuts are the whole one (columns 302-311,
 *     rows 97-105) moved by (9, 3) and (9, 13), each missing just its rim column; the leaves above them end on the
 *     rafter's lower edge (the bunch hangs from the rafter: the picture shows no stem);
 *   - the wall under the right rafter is lit plaster; the dark band at its foot by the edge (rows 158-181) and on the
 *     floor (rows 183-189) is the pot's shadow.
 *
 * The strips, every texel the picture's own:
 *   - a rafter texel is the texel 6 k columns in and 7 k rows up along its own line (k as large as the line's length in
 *     the picture allows: 42-54 columns, so the 16:9 strip repeats nothing), a log texel the one (12, 5) further in;
 *     each line keeps the tone it has at the edge (the mean of its 6 texels there over the mean round the source), so
 *     the grain and every edge go on as the same lines; the rafter's upper and lower edges keep their sub-row place;
 *   - grit (roof, plaster, floor) is the picture's grit, taken in cells with hashed offsets from a clean box (the floor
 *     from its own row, so the grain keeps its perspective size) and set to the tone wanted there: the roof by its
 *     distance to the log and to the rafter as at the edge, the wall and the floor row by row as at the edge (evened
 *     out over 7-9 rows further out), the pot's shadow fading out, the floor falling off as measured but at most 14 %;
 *   - the cupboard goes on for 6 columns of its own grain, then closes with its corner board's texels (in their order,
 *     at the panel's tone there) and its dark edge; nothing is put on it; beyond it the wall in the shade;
 *   - the two cut grapes get their rim column, one more grape (the whole one, moved by (18, 9), touching both) closes
 *     the bunch; the leaves go on under the rafter with its own translation and end on it.
 * Integers only; every choice depends on the texel's place, not on the strip's width. */

#define KT_FLOOR_L 182 /* the first floor row at each edge */
#define KT_FLOOR_R 183
typedef struct {
    /* the rafter: 6 x (edge row) of its first line / of the lit band's lower edge (the line's index is 6 y - 7 X) */
    int top6, lo6;
    /* the log and its shadow: 12 x (edge row) below them (the line's index is 12 y + 5 X) */
    int qbot;
} KtSide;
static const KtSide KT_S[2] = { { 331, 575, 432 }, { 306, 552, 312 } };
static Rgb (*kt_prof)[PH];             /* each edge's tone per row (6 columns, smoothed) */
static Rgb (*kt_pr)[PH], (*kt_pl)[PH]; /* ... along the rafter's / the log's direction (8 texels) */
static Rgb (*kt_fl)[PH];               /* ... the floor's (8 columns) */
static Rgb (*kt_pan);                   /* the cupboard panel's tone per row */

static Rgb kt_px(int side, int i, int y)
{
    return rgb15(pic[clampi(y, 0, PH - 1)][clampi(side ? PW - 1 - i : i, 0, PW - 1)]);
}
static int kt_sstep(int v, int a, int b) /* smoothstep, 0..256 */
{
    int t = clampi((v - a) * 256 / (b - a), 0, 256);
    return t * t * (768 - 2 * t) / 65536;
}
/* s x e / a per channel, the gain within lo..hi / 256 */
static Rgb kt_gain(Rgb s, const int *e, const int *a, int lo, int hi)
{
    int target_l = e[0] * 3 + e[1] * 6 + e[2],
        local_l = a[0] * 3 + a[1] * 6 + a[2],
        gain_l = local_l > 0 ? target_l * 256 / local_l : 256,
        v[3] = { s.r, s.g, s.b };
    Rgb o;
    for (int c = 0; c < 3; c++) {
        int gain = a[c] >= 16 ? e[c] * 256 / a[c] : gain_l;
        v[c] = clampi(v[c] * clampi(gain, lo, hi) / 256, 0, 255);
    }
    o.r = v[0];
    o.g = v[1];
    o.b = v[2];
    return o;
}

/* a texel of a band of parallel lines (the rafter: n 6, dn 7; the log: n 12, dn -5): the picture's texel K-steps of (n
   columns in, dn rows up) along its line, at the tone the line has at the edge */
static Rgb kt_fib(int side, int X, int y, int n, int dn, int K)
{
    int k = K * ((X + n * K - 1) / (n * K)),
        is = n * k - X,
        ys = y - dn * k,
        target[3] = { 0, 0, 0 },
        local[3] = { 0, 0, 0 },
        target_n = 0,
        local_n = 0;
    Rgb src = kt_px(side, is, ys);
    for (int j = 0; j < 6; j++) {
        int yy = y - fdiv(2 * dn * (X + j) + n, 2 * n);
        if (yy >= 0 && yy < PH) {
            Rgb c = kt_px(side, j, yy);
            target[0] += c.r;
            target[1] += c.g;
            target[2] += c.b;
            target_n++;
        }
    }
    for (int j = -3; j <= 3; j++) {
        int yy = ys - fdiv(2 * dn * j + n, 2 * n);
        if (is + j >= 0 && yy >= 0 && yy < PH) {
            Rgb c = kt_px(side, is + j, yy);
            local[0] += c.r;
            local[1] += c.g;
            local[2] += c.b;
            local_n++;
        }
    }
    if (!target_n || !local_n) return src;
    for (int c = 0; c < 3; c++) {
        target[c] = target[c] * 4 / target_n;
        local[c] = local[c] * 4 / local_n;
    }
    return kt_gain(src, target, local, 150, 420);
}

/* grit: the picture's texel of the box (x0, y0, w, h) (screen columns), the cell's hashed offset, over the box's own
   local mean, at tone t */
static Rgb kt_grit(int wx, int y, int x0, int y0, int w, int h, Rgb t, int seed)
{
    int jx = (int)(hash3(y >> 1, seed, 11) % 5) - 2,
        jy = (int)(hash3(wx >> 1, seed, 12) % 5) - 2;
    int cell_x = fdiv(wx + jx, 9),
        cell_y = fdiv(y + jy, 7);
    u32 rnd = hash3(cell_x, cell_y, seed);
    int sx = x0 + fmodi(wx + (int)(rnd & 1023), w),
        sy = y0 + fmodi(y + (int)(rnd >> 10 & 1023), h),
        local[3] = { 0, 0, 0 },
        target[3],
        n = 0;
    Rgb src = rgb15(pic[sy][sx]);
    for (int v = sy - 2; v <= sy + 2; v++)
        for (int u = sx - 2; u <= sx + 2; u++)
            if (u >= x0 && u < x0 + w && v >= y0 && v < y0 + h) {
                Rgb c = rgb15(pic[v][u]);
                local[0] += c.r;
                local[1] += c.g;
                local[2] += c.b;
                n++;
            }
    for (int c = 0; c < 3; c++) local[c] = local[c] * 4 / n;
    target[0] = t.r * 4;
    target[1] = t.g * 4;
    target[2] = t.b * 4;
    return kt_gain(src, target, local, 40, 640);
}
#define KT_TH_X 122 /* the roof's grit between the beams */
#define KT_TH_Y 0
#define KT_TH_W 37
#define KT_TH_H 17
static Rgb kt_thatch(int side, int X, int y, Rgb t)
{
    return kt_grit(side ? PW - 1 + X : -X, y, KT_TH_X, KT_TH_Y, KT_TH_W, KT_TH_H, t, 71 + side);
}
static Rgb kt_plaster(int X, int y, Rgb t) /* the lit plaster right of the niche / under the grapes */
{
    return (hash3(fdiv(X, 14), fdiv(y, 11), 5) & 1) ? kt_grit(PW - 1 + X, y, 290, 131, 29, 14, t, 81)
                                                    : kt_grit(PW - 1 + X, y, 272, 102, 24, 27, t, 82);
}
/* the floor: the texel of its own row (rows next to the wall: the first clean row's), 8-87 columns in */
static Rgb kt_floor(int side, int X, int y, Rgb t)
{
    int wx = side ? PW - 1 + X : -X,
        jx = (int)(hash3(y >> 1, side, 21) % 5) - 2,
        jy = (int)(hash3(wx >> 1, side, 22) % 3) - 1;
    u32 rnd = hash3(fdiv(wx + jx, 11), fdiv(y + jy, 5), 23 + side);
    int is = 8 + fmodi(wx + (int)(rnd & 1023), 80),
        ys = clampi((y < 190 ? y + 7 : y) + (int)(rnd >> 10 & 1), 190, PH - 1);
    int local[3] = { 0, 0, 0 },
        target[3],
        n = 0;
    Rgb src = kt_px(side, is, ys);
    for (int v = ys - 1; v <= ys + 1; v++)
        for (int u = is - 3; u <= is + 3; u++)
            if (v >= 190 && v < PH) {
                Rgb c = kt_px(side, u, v);
                local[0] += c.r;
                local[1] += c.g;
                local[2] += c.b;
                n++;
            }
    for (int c = 0; c < 3; c++) local[c] = local[c] * 4 / n;
    target[0] = t.r * 4;
    target[1] = t.g * 4;
    target[2] = t.b * 4;
    return kt_gain(src, target, local, 60, 560);
}

static Rgb kt_avg(const Rgb *p, int y0, int y1)
{
    int r = 0,
        g = 0,
        b = 0,
        n = 0;
    Rgb o;
    for (int y = y0; y <= y1; y++)
        if (y >= 0 && y < PH) {
            r += p[y].r;
            g += p[y].g;
            b += p[y].b;
            n++;
        }
    o.r = r / n;
    o.g = g / n;
    o.b = b / n;
    return o;
}
/* the edges' profiles: the tone per row at each edge (kt_prof) and of the floor (kt_fl), along the rafter's and the
   log's lines (kt_pr, kt_pl), and the cupboard panel's (kt_pan) */
static Rgb (*kt_analyse__raw);
static void kt_analyse(void)
{
    Rgb (*const raw) = kt_analyse__raw;
    for (int side = 0; side < 2; side++)
        for (int kind = 0; kind < 2; kind++) {
            int ncols = kind ? 8 : 6;
            for (int y = 0; y < PH; y++) {
                int r = 0,
                    g = 0,
                    b = 0;
                for (int i = 0; i < ncols; i++) {
                    Rgb c = kt_px(side, i, y);
                    r += c.r;
                    g += c.g;
                    b += c.b;
                }
                raw[y].r = r / ncols;
                raw[y].g = g / ncols;
                raw[y].b = b / ncols;
            }
            for (int y = 0; y < PH; y++) (kind ? kt_fl : kt_prof)[side][y] = kt_avg(raw, y - 1, y + 1);
        }
    for (int side = 0; side < 2; side++)
        for (int kind = 0; kind < 2; kind++)
            for (int y = 0; y < PH; y++) {
                int r = 0,
                    g = 0,
                    b = 0,
                    n = 0;
                Rgb o = { 0, 0, 0 };
                for (int j = 0; j < 8; j++) {
                    int yy = y + (kind ? fdiv(10 * j + 12, 24) : -fdiv(14 * j + 6, 12));
                    if (yy >= 0 && yy < PH) {
                        Rgb c = kt_px(side, j, yy);
                        r += c.r;
                        g += c.g;
                        b += c.b;
                        n++;
                    }
                }
                if (n) {
                    o.r = r / n;
                    o.g = g / n;
                    o.b = b / n;
                }
                (kind ? kt_pl : kt_pr)[side][y] = o;
            }
    for (int y = 0; y < PH; y++) {
        int r = 0,
            g = 0,
            b = 0;
        for (int i = 0; i < 4; i++) {
            Rgb c = kt_px(0, i, y);
            r += c.r;
            g += c.g;
            b += c.b;
        }
        raw[y].r = r / 4;
        raw[y].g = g / 4;
        raw[y].b = b / 4;
    }
    for (int y = 0; y < PH; y++) kt_pan[y] = kt_avg(raw, y - 2, y + 2);
}

/* the roof's underside between the log's shadow and the rafter: its tone by the distance to each, as at the edge */
static Rgb kt_roof(int side, int X, int y)
{
    const KtSide *s = &KT_S[side];
    /* 1/12 row */
    int log_bot = s->qbot,
        raft_top = s->top6 * 2,
        d_log = 12 * y + 5 * X - log_bot,
        d_raft = raft_top + 14 * X - 12 * y,
        span = raft_top - log_bot,
        near12 = 8 * 12,
        edge_row;
    Rgb a, b;
    if (d_log < 0) d_log = 0;
    if (d_raft < 0) d_raft = 0;
    if (d_log < near12)
        edge_row = log_bot + d_log;
    else if (d_raft < near12)
        edge_row = raft_top - d_raft;
    else
        edge_row =
            log_bot + near12 + (int)((long long)(span - 2 * near12) * (d_log - near12) / (d_log + d_raft - 2 * near12));
    edge_row = clampi(edge_row, 0, (raft_top / 12 - 1) * 12); /* (the last row is the rafter's edge) */
    {
        int r0 = edge_row / 12,
            r1 = clampi(r0 + 1, 0, raft_top / 12 - 1),
            frac = edge_row % 12 * 256 / 12,
            wt = d_log + d_raft > 0 ? d_log * 256 / (d_log + d_raft) : 0;
        a = mix(kt_pl[side][r0], kt_pl[side][r1], frac);
        b = mix(kt_pr[side][r0], kt_pr[side][r1], frac);
        return kt_thatch(side, X, y, mix(a, b, wt));
    }
}

static int kt_purple(Rgb c) { return c.b > c.g + 30 && c.r > c.g + 10; }
static int kt_leaf(Rgb c) { return c.g + 30 >= c.r && c.g >= c.b + 40 && lum(c) > 90; }
#define KT_GX 302 /* the whole grape: columns 302-311, rows 97-105 */
#define KT_GY 97
static int kt_grape(int x, int y, Rgb *o) /* its texel at (x, y), if any */
{
    Rgb c;
    if (x < KT_GX || x > KT_GX + 9 || y < KT_GY || y > KT_GY + 8) return 0;
    c = rgb15(pic[y][x]);
    if (!kt_purple(c)) return 0;
    *o = c;
    return 1;
}

/* the floor's tone on a row: the edge's, evened out over 9 rows further out (a streak of the edge does not run on) */
static Rgb kt_ftone(int side, int X, int y)
{
    int floor0 = side ? KT_FLOOR_R : KT_FLOOR_L;
    return mix(kt_fl[side][y], kt_avg(kt_fl[side], y - 4 < floor0 + 1 ? floor0 + 1 : y - 4, y + 4), kt_sstep(X, 2, 30));
}
/* under the right rafter: the plaster wall, the pot's shadow fading out, the bunch closed */
static Rgb kt_under_r(int X, int y)
{
    Rgb tone, o;
    int wx = PW - 1 + X,
        fade = kt_sstep(X, 3, 22);
    if (wx == 320 &&
        ((y >= 101 && y <= 105 && kt_grape(311, y - 3, &o)) || (y >= 111 && y <= 115 && kt_grape(311, y - 13, &o))))
        return o;
    if (kt_grape(wx - 18, y - 9, &o)) return o;
    if (X <= 6 && y <= 100 - X) {
        o = kt_px(1, 6 - X, y - 7);
        if (kt_leaf(o)) return o;
    }
    if (y >= KT_FLOOR_R) {
        Rgb lit = kt_avg(kt_fl[1], 190, 193);
        tone = y < 190 ? mix(kt_fl[1][y], lit, fade) : kt_ftone(1, X, y);
        return kt_floor(1, X, y, scale(tone, 256 - clampi(X, 0, 40) * 9 / 10));
    }
    {
        Rgb lit = kt_avg(kt_prof[1], 129, 134),
            under;
        if (y < 132)
            tone = lit;
        else if (y <= 148)
            tone = mix(kt_prof[1][y], kt_avg(kt_prof[1], y - 3 < 132 ? 132 : y - 3, y + 3), kt_sstep(X, 2, 30));
        else {
            under = scale(kt_avg(kt_prof[1], 146, 150), 256 - (y - 148) * 5 / 2);
            if (y >= KT_FLOOR_R - 3) under = scale(under, 256 - (y - (KT_FLOOR_R - 4)) * 40);
            tone = mix(kt_prof[1][y], under, fade);
        }
    }
    return kt_plaster(X, y, tone);
}
/* under the left rafter: the cupboard's side closed, then the wall in the shade */
static Rgb kt_under_l(int X, int y)
{
    Rgb tone;
    if (y >= KT_FLOOR_L) return kt_floor(0, X, y, scale(kt_ftone(0, X, y), 256 - clampi(X, 0, 40) * 9 / 10));
    tone = kt_pan[y];
    if (X <= 16) {
        int is = X <= 6 ? 16 - X : X <= 14 ? 54 - X : 33 + X,
            local[3] = { 0, 0, 0 },
            target[3],
            n = 0;
        Rgb src = kt_px(0, is, y);
        if (X > 14) tone = scale(tone, 120);
        for (int v = y - 1; v <= y + 1; v++)
            for (int u = is - 2; u <= is + 2; u++)
                if (v >= 97 && v < KT_FLOOR_L && (X > 14 ? u >= 48 && u <= 49 : X > 6 ? u >= 38 && u <= 47 : 1)) {
                    Rgb c = kt_px(0, u, v);
                    local[0] += c.r;
                    local[1] += c.g;
                    local[2] += c.b;
                    n++;
                }
        if (!n) return src;
        for (int c = 0; c < 3; c++) local[c] = local[c] * 4 / n;
        target[0] = tone.r * 4;
        target[1] = tone.g * 4;
        target[2] = tone.b * 4;
        return kt_gain(src, target, local, 100, 480);
    }
    tone = scale(kt_avg(kt_pan, y - 6, y + 6), 256 - 60 + kt_sstep(X, 17, 26) * 30 / 256);
    return kt_thatch(0, X, y, tone);
}

/* a strip texel X columns out of the edge: above the rafter the log or the roof, then the rafter, under it the
   wall side (kt_under_r / kt_under_l) */
static Rgb kt_texel(int side, int X, int y)
{
    const KtSide *s = &KT_S[side];
    int raft6 = 6 * y - 7 * X,
        log12 = 12 * y + 5 * X,
        cov = clampi(raft6 - s->top6 + 3, 0, 6);
    Rgb tex;
    if (cov < 6) { /* above the rafter (its upper edge at its sub-row place) */
        tex = log12 < s->qbot ? kt_fib(side, X, y, 12, -5, 1) : kt_roof(side, X, y);
        return cov ? mix(tex, kt_fib(side, X, y + 1, 6, 7, 1), cov * 256 / 6) : tex;
    }
    cov = clampi(s->lo6 - raft6 + 3, 0, 6);
    /* (its first rows from next to the edge: the rafter's upper edge is not straight to a row further in; the right lit
       band from its even pale part, the last 36 columns) */
    if (cov == 6)
        return kt_fib(side, X, y, 6, 7,
                      raft6 < s->top6 + 18          ? 1
                      : side && raft6 > s->lo6 - 66 ? 6
                                                    : clampi(fdiv(raft6, 42), 1, 9));
    tex = side ? kt_under_r(X, y) : kt_under_l(X, y);
    if (!cov) return tex;
    return mix(tex, kt_fib(side, X, y - 1, 6, 7, 9), cov * 256 / 6);
}

static void gen_kt(int pad)
{
    if (!ws_need_gen_kt()) return;
    pal_make();
    kt_analyse();
    for (int side = 0; side < 2; side++)
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(kt_texel(side, X, y));
}
