/* --widescreen room picture strips: AREA17 s5, the Iron Castle engine room (included by ws_backdrop.c; docs/PORT.md
 * "More rooms with picture strips").
 *
 * The scene is not one picture: the 320 x 224 sprite pair (pages 128 / 256, the hall's layout: 256 rows drawn) is only
 * the room's box (back wall, side walls, ceiling beams, floor, the furnace); the boilers, pipes, wheel, gears, hoses and
 * the coal are 19 more raw quads of the same CLUT from pages 128 / 256 / 384, drawn over it every frame (EN_Q, in the
 * game's order; six of them are animated by drawing other texels of the pages: steam, the wheel, the gears, the grate's
 * glow; the table has their idle texels and all six lie inside columns 71-256).  A quad of w pixels spans w - 1 texels,
 * so en_take() puts the scene together the way the renderer shows it (software: texel x (w - 1) / w rounded half up;
 * OpenGL: truncated, as gen_th's th_shown) and keeps the box alone in en_bk and the quad of every texel in pic_ov.
 * Checked: equal to the software frame's 4:3 columns outside the sprites and the animated quads.
 *
 * Measured on the box: both side walls' lines (the beam's edges, the moulding, the rail at row 148, the foot, the
 * pipe under the right beam) meet at (158, 155.6), the horizon; the floor is iron plates whose seams run to that
 * point, 1.05 wide in f = (x - 158) / (y - 155.6), and its light is flat past |f| 0.75 on the left and falls with |f|
 * on the right.  What the edges cut, and how each goes on:
 *   - the side walls and the floor, in perspective, from the box alone (so no pipe or boiler is copied): a strip
 *     column at dx columns from the vanishing point shows the wall EN_C further along it (1 / dx' = 1 / dx + n EN_C: a
 *     translation in the wall's own plane, never a mirror), its rows scaled about the horizon by dx / dx', so every
 *     line through the vanishing point goes on exactly and the texels grow towards the viewer.  The left step closes
 *     the cut panel with the wall's post and starts the next panel; the right one takes the 20 clean columns by the
 *     edge (the beam, the pipe under it and the riveted plate with its bolt, which so comes back once a block).  Where
 *     a block starts, its light per row is matched to the column before, fading over 24 columns.  Below the rail the
 *     walls are hidden in the picture (a bracket, the furnace's skirt): the one clean column goes on along the rays,
 *     with the grain of the wall 83 rows higher.  The floor: the same row one plate further in (so the seams keep
 *     their period and phase), lit by the measured light at the place it goes to over the place it came from;
 *   - the hoses on the floor (rows 184-195 left, 183-192 right): on along the side wall's foot, i.e. the rays;
 *   - left, the rust pipe (rows 91-125) and the grey pipe in front of it (145-182): both reach the edge at the end
 *     of an elbow (their upper outline and highlight still fall 0.3 row a column there, less every column), so they
 *     finish the turn (1.45 rows over 10 columns) and run level off the strip's outer edge with the cross-section
 *     they have at the edge;
 *   - left, the big grey pipe at the top (its highlight at column 11, 13 columns from its right limb as on the pipe
 *     beside it, the left limb 4 columns past the edge): the limb is closed, round, down to where the elbow's outline
 *     comes back into the picture at row 58;
 *   - right, the copper pipe: a three-piece elbow (upright, 46 degrees, 23 degrees) with a ring on each joint; the edge
 *     cuts the second ring and all but a wedge of the third piece.  The ring's rim, groove and top face go on along
 *     quarter ellipses to its end 8.5 columns out, where the groove meets the middle piece's lower outline (167.5 -
 *     1.05 X), each band with the texels it has at the edge, darkening to the end as the first ring's does.  The
 *     third piece runs off the outer edge along its measured upper outline (122 - 0.417 X) with the middle piece's
 *     cross-section (its texels between the rings averaged by their distance from its upper outline: the third
 *     piece's own seven rows at the edge match it within 8 levels), 38 rows high;
 *   - the runs carry the grain of an upright stretch of the same pipe (its texels less their column and row means).
 * Integers only; a column depends on its place alone (the canvas is 32 columns wider than the strip), so wider
 * strips start with the 16:9 columns.  0.01 s at 16:9. */
typedef struct { short tx, u, v, w, h, x, y; } EnQ;     /* page (tx, 0): w x h pixels from (u, v) at picture (x, y) */
static const EnQ EN_Q[] = {
    { 384, 96, 0, 56, 56, 80, 5 }, { 128, 0, 224, 72, 24, 200, 12 }, { 128, 144, 224, 32, 24, 224, 95 },
    { 256, 32, 224, 32, 16, 135, 0 }, { 256, 0, 224, 32, 16, 46, 0 }, { 256, 208, 56, 48, 120, 158, 72 },
    { 256, 64, 0, 144, 176, 14, 16 }, { 256, 64, 176, 64, 64, 71, 108 }, { 256, 208, 24, 16, 32, 198, 148 },
    { 384, 0, 0, 48, 40, 134, 130 }, { 384, 0, 208, 168, 16, 152, 177 }, { 384, 0, 224, 152, 24, 0, 172 },
    { 384, 0, 128, 32, 24, 133, 72 }, { 384, 152, 64, 56, 56, 264, 120 }, { 384, 136, 120, 64, 48, 256, 176 },
    { 384, 208, 0, 48, 176, 0, 48 }, { 384, 208, 176, 16, 72, 48, 72 }, { 384, 232, 184, 24, 48, 48, 176 },
    { 384, 176, 176, 32, 48, 0, 0 },
};
#define EN_NQ ((int)(sizeof EN_Q / sizeof EN_Q[0]))
static u16 en_bk[PH][PW];                                /* the back layer alone (the room's box) */
static void en_take(const u16 *vram, int cx, int cy, int gl)
{
    memcpy(en_bk, pic, sizeof en_bk);
    for (int i = 0; i < EN_NQ; i++) {
        const EnQ *p = &EN_Q[i];
        for (int r = 0; r < p->h; r++)
            for (int x = 0; x < p->w; x++) {
                int sx = gl ? (64 * x * (p->w - 1) + 2 * p->w - 1) / (64 * p->w) : (2 * x * (p->w - 1) + p->w) / (2 * p->w);
                int sy = gl ? (64 * r * (p->h - 1) + 2 * p->h - 1) / (64 * p->h) : (2 * r * (p->h - 1) + p->h) / (2 * p->h);
                u16 c = tex8(vram, p->tx, p->u + sx, p->v + sy, cx, cy);
                if (c && p->y + r < PH && p->x + x < PW) { pic[p->y + r][p->x + x] = c; pic_ov[p->y + r][p->x + x] = (u8)(i + 1); }
            }
    }
}

#define EN_VX (158 * 256)                                /* the vanishing point (1/256 texel; a texel's centre at + 128) */
#define EN_VY (155 * 256 + 154)
static const int EN_C[2] = { 26844, 13850 };             /* the walls' step along the wall, in 1 / dx (x 2^24) */
static const int EN_RAIL[2] = { 150, 150 };              /* below this row at the edge: the wall's foot band */
static const int EN_FOOT[2] = { 184 * 256, 186 * 256 };  /* the wall's foot at the edge (1/256 row) */
#define EN_FW 269                                        /* the floor plates' width in f = (x - VX) / (y - VY), x 256 */
static int en_dxe(int s) { return s ? 161 * 256 + 128 : 157 * 256 + 128; }   /* the edge column's centre from the VP */
static Rgb en_bil(const u16 (*src)[PW], int x, int y)    /* (1/256 texel, centres at + 128) */
{
    int x0, y0, x1, y1, fx, fy; Rgb a, b, c, d, o;
    x = clampi(x - 128, 0, (PW - 1) * 256); y = clampi(y - 128, 0, (PH - 1) * 256);
    x0 = x >> 8; y0 = y >> 8; fx = x & 255; fy = y & 255; x1 = x0 + 1 < PW ? x0 + 1 : x0; y1 = y0 + 1 < PH ? y0 + 1 : y0;
    a = rgb15(src[y0][x0]); b = rgb15(src[y0][x1]); c = rgb15(src[y1][x0]); d = rgb15(src[y1][x1]);
    o.r = ((a.r * (256 - fx) + b.r * fx) * (256 - fy) + (c.r * (256 - fx) + d.r * fx) * fy + 32768) >> 16;
    o.g = ((a.g * (256 - fx) + b.g * fx) * (256 - fy) + (c.g * (256 - fx) + d.g * fx) * fy + 32768) >> 16;
    o.b = ((a.b * (256 - fx) + b.b * fx) * (256 - fy) + (c.b * (256 - fx) + d.b * fx) * fy + 32768) >> 16;
    return o;
}
static int en_foot(int s, int X) { return EN_VY + (int)((long long)(EN_FOOT[s] - EN_VY) * (en_dxe(s) + X * 256) / en_dxe(s)); }
static Rgb en_wall_at(int s, int X, int yc)              /* the wall a block further in, at row yc (1/256) */
{
    int dxt = en_dxe(s) + X * 256, den, dxs, n;
    for (n = 1; ; n++) {
        den = 65536 + (int)(((long long)n * EN_C[s] * dxt) >> 16);
        dxs = (int)((long long)dxt * 65536 / den);
        if (dxs <= en_dxe(s)) break;
    }
    return en_bil(en_bk, s ? EN_VX + dxs : EN_VX - dxs, EN_VY + (int)((long long)(yc - EN_VY) * 65536 / den));
}
static Rgb en_wall(int s, int X, int y)
{
    int dxt = en_dxe(s) + X * 256, yc = y * 256 + 128, dxs, up, st, m[3] = { 0, 0, 0 };
    int rail = EN_VY + (int)((long long)(EN_RAIL[s] * 256 - EN_VY) * dxt / en_dxe(s));
    Rgb c, t;
    if (yc < rail) return en_wall_at(s, X, yc);
    /* the foot band: the one clean column along the rays, with the grain of the wall 83 rows (at the edge) higher */
    dxs = s ? en_dxe(s) : (158 - 33) * 256;
    c = en_bil(en_bk, s ? EN_VX + dxs : EN_VX - dxs, EN_VY + (int)((long long)(yc - EN_VY) * dxs / dxt));
    st = (int)((long long)256 * dxt / en_dxe(s)); up = yc - 83 * st;
    t = en_wall_at(s, X, up);
    for (int k = -3; k <= 3; k++) { Rgb q = en_wall_at(s, X, up + k * 3 * st); m[0] += q.r; m[1] += q.g; m[2] += q.b; }
    c.r = clampi(c.r + (t.r - m[0] / 7) * 3 / 4, 0, 255); c.g = clampi(c.g + (t.g - m[1] / 7) * 3 / 4, 0, 255); c.b = clampi(c.b + (t.b - m[2] / 7) * 3 / 4, 0, 255);
    return c;
}
static int en_fl[2][64];                                 /* the floor's light by |f| (1/8), per side */
static void en_flight(void)
{
    for (int s = 0; s < 2; s++) {
        int sum[64], n[64], last = 0, t[64];
        memset(sum, 0, sizeof sum); memset(n, 0, sizeof n);
        for (int y = 190; y < PH; y++)
            for (int x = s ? 160 : 0; x < (s ? PW : 160); x++) {
                int f = (x * 256 + 128 - EN_VX) * 8 / (y * 256 + 128 - EN_VY);
                if (f < 0) f = -f;
                if (f < 64) { sum[f] += lum(rgb15(en_bk[y][x])); n[f]++; }
            }
        for (int k = 0; k < 64; k++) { if (n[k] >= 24) last = sum[k] / n[k]; t[k] = last; }
        for (int k = 0; k < 64; k++) en_fl[s][k] = (t[k > 1 ? k - 2 : 0] + t[k > 0 ? k - 1 : 0] + t[k] + t[k < 63 ? k + 1 : 63] + t[k < 62 ? k + 2 : 63]) / 5;
    }
}
static int en_flat(int s, int xc, int h)                 /* the light at column xc (1/256), h below the horizon */
{
    int f = (xc - EN_VX) * 8 / (h >> 4), k; if (f < 0) f = -f;   /* |f| x 128 */
    k = f >> 4; if (k > 62) return en_fl[s][63];
    return (en_fl[s][k] * (16 - (f & 15)) + en_fl[s][k + 1] * (f & 15)) / 16;
}
static Rgb en_floor(int s, int X, int y)
{
    int yc = y * 256 + 128, h = yc - EN_VY, xt = s ? PW * 256 + X * 256 - 128 : -X * 256 + 128, xs = xt, a, b;
    for (int n = 1; n < 64; n++) {
        xs = s ? xt - n * (EN_FW * h >> 8) : xt + n * (EN_FW * h >> 8);
        if (xs >= 128 && xs <= PW * 256 - 128) break;
    }
    a = en_flat(s, xt, h); b = en_flat(s, xs, h);
    return scale(en_bil(en_bk, xs, yc), clampi(a * 256 / (b > 0 ? b : 1), 150, 300));
}

/* --- the things the edges cut */
static int en_edge(int s) { return s ? PW * 256 - 128 : 128; }
static void en_over(int X, int y, Rgb c, int a) { Rgb *p = px_at(X, y); if (p && a > 0) *p = a >= 256 ? c : mix(*p, c, a); }
/* the hoses on the floor along the back wall's foot: on along the side wall's foot (the rays from the VP) */
static void en_hoses(int s, int X)
{
    static const int H0[2] = { 184, 183 }, H1[2] = { 196, 193 };
    int dxt = en_dxe(s) + X * 256;
    for (int y = 150; y < PH; y++) {
        int ys = EN_VY + (int)((long long)(y * 256 + 128 - EN_VY) * en_dxe(s) / dxt);
        int a = clampi(ys - H0[s] * 256 + 128, 0, 256), b = clampi(H1[s] * 256 - ys + 128, 0, 256);
        if (b < a) a = b;
        if (a > 0) en_over(X, y, en_bil(pic, en_edge(s), clampi(ys, H0[s] * 256 + 128, H1[s] * 256 - 128)), a);
    }
}
/* a pipe that leaves by the edge on its way to level: its cross-section at the edge (rows [r0, r1)) carried on, the
   last of its turn (d: 1/256 row up) over the first columns */
static int en_ease(int X) { return X < 10 ? 77 * X - 4 * X * X : 370; }
/* the pipes' grain: an upright stretch of the same pipe less its shading (the columns' and rows' means), laid along
   the run: its rows along the run (two copies half a length apart, cross-faded so neither ends), its columns across */
typedef struct { short x0, y0, w, h, amp; } EnG;
static const EnG EN_G[3] = { { 6, 197, 18, 27, 80 }, { 34, 202, 22, 22, 80 }, { 268, 188, 28, 36, 150 } };
static short en_res[3][40][32][3];
static void en_grain(void)
{
    for (int g = 0; g < 3; g++) {
        const EnG *q = &EN_G[g]; int cm[32][3], rm[40][3], all[3] = { 0, 0, 0 };
        memset(cm, 0, sizeof cm); memset(rm, 0, sizeof rm);
        for (int j = 0; j < q->h; j++) for (int i = 0; i < q->w; i++) {
            Rgb c = rgb15(pic[q->y0 + j][q->x0 + i]); int v[3] = { c.r, c.g, c.b };
            for (int k = 0; k < 3; k++) { cm[i][k] += v[k]; rm[j][k] += v[k]; all[k] += v[k]; }
        }
        for (int j = 0; j < q->h; j++) for (int i = 0; i < q->w; i++) {
            Rgb c = rgb15(pic[q->y0 + j][q->x0 + i]); int v[3] = { c.r, c.g, c.b };
            for (int k = 0; k < 3; k++)
                en_res[g][j][i][k] = (short)((v[k] * 16 - cm[i][k] * 16 / q->h - rm[j][k] * 16 / q->w + all[k] * 16 / (q->w * q->h)) * q->amp / 256);
        }
    }
}
static Rgb en_grained(Rgb c, int g, int t, int v, int H)  /* t along (columns), v of H across (1/256) */
{
    const EnG *q = &EN_G[g];
    int blk = fdiv(t, 3), j = (int)((hash3(blk, g, 911) >> 8) % (u32)q->h), l = lum(c), k;
    int i = clampi((int)((long long)v * q->w / (H > 0 ? H : 1)) + (int)((hash3(blk, g, 913) >> 8) % 3u) - 1, 0, q->w - 1);
    j = (j + fmodi(t, 3)) % q->h;
    k = l < 24 ? 0 : l < 56 ? (l - 24) * 8 : 256;         /* (none in the deep shadow) */
    c.r = clampi(c.r + en_res[g][j][i][0] * k / 4096, 0, 255);
    c.g = clampi(c.g + en_res[g][j][i][1] * k / 4096, 0, 255);
    c.b = clampi(c.b + en_res[g][j][i][2] * k / 4096, 0, 255);
    return c;
}
static void en_run(int s, int X, int r0, int r1, int d, int g)
{
    for (int y = r0 - 3; y <= r1; y++) {
        int v = y * 256 + 128 + d, a = clampi(v - r0 * 256 + 128, 0, 256), b = clampi(r1 * 256 - v + 128, 0, 256);
        if (b < a) a = b;
        if (y >= 0 && y < PH && a > 0)
            en_over(X, y, en_grained(en_bil(pic, en_edge(s), clampi(v, r0 * 256 + 128, r1 * 256 - 128)), g, X, clampi(v - r0 * 256, 0, (r1 - r0) * 256 - 1), (r1 - r0) * 256), a);
    }
}
/* the big grey pipe at the top left: its left limb */
static void en_limb(int X)
{
    for (int y = 0; y < 59; y++) {
        int w = y <= 30 ? 4 * 256 : 4 * 256 * isqrt_i((29 * 29 - (y - 30) * (y - 30)) * 256) / (29 * 16);
        int a = clampi(w - (X - 1) * 256, 0, 256);
        if (a > 0) en_over(X, y, scale(rgb15(pic[y][0]), 256 - 150 * (2 * X - 1) * 128 / (w + 128)), a);
    }
}
/* the copper pipe at the right: the run after its second ring (C) with the cross-section of the run between the rings
   (B), and the ring's end */
static Rgb en_bp[64];                                    /* B's cross-section (texels from its upper left outline) */
static int en_lerp(const int *xs, const int *ys, int n, int x256)   /* piecewise linear (x in texels, y in 1/256) */
{
    int i = 0;
    if (x256 <= xs[0] * 256) return ys[0];
    while (i < n - 2 && x256 > xs[i + 1] * 256) i++;
    if (x256 >= xs[n - 1] * 256) return ys[n - 1];
    return ys[i] + (int)((long long)(ys[i + 1] - ys[i]) * (x256 - xs[i] * 256) / ((xs[i + 1] - xs[i]) * 256));
}
static void en_bprof(void)
{
    static const int GX[] = { 280, 294, 300, 306, 319 }, GY[] = { 134 * 256, 134 * 256, 135 * 256, 138 * 256, 151 * 256 };
    static const int RX[] = { 260, 270, 285, 295, 300, 305, 309, 312, 320 },
                     RY[] = { 149 * 256, 149 * 256, 150 * 256, 153 * 256 + 128, 157 * 256, 161 * 256, 166 * 256, 172 * 256, 190 * 256 };
    int sum[64][3], n[64], last = -1;
    memset(sum, 0, sizeof sum); memset(n, 0, sizeof n);
    for (int y = 126; y < 180; y++)
        for (int x = 268; x < PW; x++) {
            int t = ((x - 289) * 184 + (y - 130) * 179) >> 8; Rgb c = rgb15(pic[y][x]);
            if (pic_ov[y][x] != 14 || t < 0 || t > 63) continue;
            if (y * 256 <= en_lerp(GX, GY, 5, x * 256) + 384 || y * 256 >= en_lerp(RX, RY, 9, x * 256) - 384) continue;
            sum[t][0] += c.r; sum[t][1] += c.g; sum[t][2] += c.b; n[t]++;
        }
    for (int t = 0; t < 64; t++) {
        if (n[t] >= 4) { en_bp[t].r = sum[t][0] / n[t]; en_bp[t].g = sum[t][1] / n[t]; en_bp[t].b = sum[t][2] / n[t]; last = t; }
        else if (last >= 0) en_bp[t] = en_bp[last];
    }
    for (int t = 62; t >= 0; t--) if (n[t] < 4 && t < 8) en_bp[t] = en_bp[t + 1];
}
#define EN_CTOP(X) (122 * 256 - 107 * (X))               /* C's upper outline (1/256 row) */
#define EN_CH (38 * 256)                                 /* its height down the column */
static void en_crun(int X)
{
    int top = EN_CTOP(X);
    for (int y = 0; y < PH; y++) {
        int v = y * 256 + 128 - top, a = clampi(v + 128, 0, 256), b = clampi(EN_CH - v + 128, 0, 256), t, f;
        if (b < a) a = b;
        if (a <= 0) continue;
        t = 8 * 256 + clampi(v, 0, EN_CH) * 236 / 256; f = t & 255; t >>= 8;
        en_over(X, y, en_grained(mix(en_bp[clampi(t, 0, 63)], en_bp[clampi(t + 1, 0, 63)], f), 2, X, clampi(v, 0, EN_CH - 1), EN_CH), a);
    }
}
#define EN_XT (8 * 256 + 128)                            /* the ring's end: columns past the edge, row */
#define EN_YT (159 * 256 + 128)
static void en_ring(int X)
{
    int x = X * 256 - 128, q, ytf, yrim, yg, ylow;
    if (x >= EN_XT) return;
    q = 256 - isqrt_i((int)(((long long)EN_XT * EN_XT - (long long)x * x) * 256 / ((long long)EN_XT * EN_XT / 256)));   /* 1 - sqrt(1 - (x / XT)^2), x 256 */
    ytf = 128 * 256 + 128 + (EN_YT - 128 * 256 - 128) * q / 256;
    yrim = 137 * 256 + 128 + 141 * x / 256 * (EN_XT - x) / EN_XT + (EN_YT - 137 * 256 - 128) * q / 256;
    yg = 151 * 256 + x; ylow = 167 * 256 + 128 - 269 * x / 256;
    if (yg > ylow) yg = ylow;
    if (yrim > yg) yrim = yg;
    if (ytf > yrim) ytf = yrim;
    for (int y = 120; y < 175; y++) {
        int yc = y * 256 + 128, ys, a = 256; Rgb c;
        if (yc < ytf - 128 || yc > ylow + 128) continue;
        if (yc < ytf) { a = clampi(yc - ytf + 256, 0, 256); ys = 128 * 256 + 128; }
        else if (yc < yrim) ys = 128 * 256 + 128 + (int)((long long)(yc - ytf) * (9 * 256) / (yrim - ytf + 1));
        else if (yc < yg) ys = 137 * 256 + 128 + (int)((long long)(yc - yrim) * (13 * 256 + 128) / (yg - yrim + 1));
        else { ys = 151 * 256 + (int)((long long)(yc - yg) * (16 * 256 + 128) / (ylow - yg + 1)); if (yc > ylow) a = clampi(ylow + 128 - yc + 128, 0, 256); }
        c = en_bil(pic, en_edge(1), ys);
        if (x > 4 * 256) c = scale(c, 256 - 170 * (x - 4 * 256) / (EN_XT - 4 * 256));
        en_over(X, y, c, a);
    }
}
/* the walls' blocks: where a block starts (the join, and every EN_C further along the wall) its texels are lit to
   meet the column before (the light per row over +-6 rows), fading out over 24 columns */
static int en_block(int s, int X)
{
    int dxt = en_dxe(s) + X * 256, n;
    for (n = 1; ; n++) if ((int)((long long)dxt * 65536 / (65536 + (int)(((long long)n * EN_C[s] * dxt) >> 16))) <= en_dxe(s)) return n;
}
static void en_seams(int s, int pad)
{
    static int g[PH], la[PH], lb[PH];
    for (int X = 1; X <= pad; X++) {
        if (X > 1 && en_block(s, X) == en_block(s, X - 1)) continue;
        for (int y = 0; y < PH; y++) {
            la[y] = X == 1 ? lum(rgb15(en_bk[y][s ? PW - 1 : 0])) + lum(rgb15(en_bk[y][s ? PW - 2 : 1])) : lum(*px_at(X - 1, y)) + lum(*px_at(X > 2 ? X - 2 : 1, y));
            lb[y] = lum(*px_at(X, y)) + lum(*px_at(X < pad ? X + 1 : X, y));
        }
        for (int y = 0; y < PH; y++) {
            int a = 0, b = 0;
            for (int k = -6; k <= 6; k++) { int yy = clampi(y + k, 0, PH - 1); a += la[yy]; b += lb[yy]; }
            g[y] = clampi(a * 256 / (b > 0 ? b : 1), 190, 340);
        }
        for (int k = 0; k < 24 && X + k <= pad; k++) {
            int ft = en_foot(s, X + k);
            for (int y = 0; y < PH && y * 256 + 128 < ft; y++) { Rgb *p = px_at(X + k, y); *p = scale(*p, 256 + (g[y] - 256) * (24 - k) / 24); }
        }
    }
}
static void gen_en(int pad)
{
    if (!canvas_open(pad + 32)) return;                  /* (a margin: a column never depends on the strip's width) */
    en_bprof(); en_flight(); en_grain();
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side;
        for (int X = 1; X <= cw; X++) {
            int ft = en_foot(side, X);
            for (int y = 0; y < PH; y++) put(X, y, y * 256 + 128 >= ft ? en_floor(side, X, y) : en_wall(side, X, y));
        }
        en_seams(side, cw - 2);
        for (int X = 1; X <= pad; X++) {
            en_hoses(side, X);
            if (!side) { en_run(0, X, 91, 126, en_ease(X), 0); en_run(0, X, 145, 183, en_ease(X), 1); en_limb(X); }
            else { en_crun(X); en_ring(X); }
        }
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(cv[(size_t)y * cw + X - 1]);
    }
    canvas_close();
}
