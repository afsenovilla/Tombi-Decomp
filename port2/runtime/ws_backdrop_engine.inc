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
 *     finish the turn (1.45 rows over 10 columns) and run level with the cross-section they have at the edge.  Not
 *     for long (the first version ran them off the outer edge, 53 to 120 columns of plain pipe: "la sala de las
 *     tuberías se prolonga de una forma poco natural y aburrida"): each makes the offset the big grey pipe makes in
 *     the picture (down, across, down) and goes up through the top.  The rust pipe after 13 columns: a quarter turn
 *     as tight as its elbow in the picture (outer outline: a circle of the pipe's width about the inner corner),
 *     then upright, 32 columns wide as its stretch under the flange (its mottle full past the flange, lighter on
 *     the elbow as in the picture), with that flange (the picture's texels, bolts
 *     and all) 7 rows from the turn as in the picture.  The grey pipe after 62 columns (so past the 16:9 strip, where
 *     it is a run under the rust pipe), with the joints the grey pipes have (a light line across: every 28 columns
 *     on the run, as 21 rows on the 29 wide pipe; two on the upright stretch).  A turn's texel takes the run's
 *     section and the upright stretch's section (from the picture: rust columns 7-38 under the flange, grey columns
 *     30-50 of the upright pipe beside it), each at its distance from the inner corner, weighed by where its normal
 *     points; never a mirror: the upright stretch keeps the picture's left and right;
 *   - left, the big grey pipe at the top (its highlight at column 11, 13 columns from its right limb as on the pipe
 *     beside it, the left limb 4 columns past the edge): the limb is closed, round, down to where the elbow's outline
 *     comes back into the picture at row 58;
 *   - right, the copper pipe: a three-piece elbow (upright, 46 degrees, 23 degrees) with a ring on each joint; the edge
 *     cuts the second ring and all but a wedge of the third piece.  The ring goes on with the first ring's own end
 *     (its rim, second bolt, groove and dark end: en_ring), its top face closing along a quarter ellipse.  The
 *     third piece goes on along its measured upper outline (122 - 0.417 X) with the middle piece's cross-section
 *     (its texels between the rings averaged by their distance from its upper outline: the third piece's own seven
 *     rows at the edge match it within 8 levels), 38 rows high, and then turns back up the way it came (see EN_T):
 *     a copy of the second ring 32 columns on (the first place where all of it is in the strip), a 46 degree piece,
 *     a copy of the first ring, and upright through the top (39 columns, the first piece's section and mottle);
 *   - every new stretch is its section plus the painted mottle of the same pipe's upright stretches, texel for texel
 *     (en_grain: tapes); the rust pipe's turn starts at the picture's seam turned half round (en_seam).
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
static u16 (*en_bk)[PW];                                /* the back layer alone (the room's box) */
static void en_take(const u16 *vram, int cx, int cy, int gl)
{
    if (!ws_need_en_take()) return;
    memcpy(en_bk, pic, (sizeof *en_bk * (PH)));
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
/* the pipes' texture: the picture's own.  A tape per pipe is its clean upright stretches end to end, each texel less
   its stretch's shading (the columns' and rows' means), so a new stretch is its section plus the painted mottle, texel
   for texel (no scaling, so the same density): along the pipe the tape's rows, across it the tape's columns; a pipe
   wider than the tape takes a second lane half a tape on, and every other tile is the tape turned half round (a
   rotation, never a mirror), so no blotch comes back the same way up within a strip */
typedef struct { short g, x0, y0, w, h; } EnS;
static const EnS EN_S[] = {
    { 0, 5, 208, 20, 16 }, { 0, 8, 141, 20, 9 }, { 0, 18, 141, 20, 9 },          /* rust: the stretch by the floor, under the flange */
    { 1, 4, 2, 19, 25 }, { 1, 31, 86, 19, 13 }, { 1, 4, 31, 19, 13 }, { 1, 38, 203, 19, 20 },   /* grey: the big pipe above and under its joint, the upright one, the one by the floor */
    { 2, 261, 188, 38, 36 },                                                    /* copper: the upright piece */
};
#define EN_TL 96
static short (*en_tp)[EN_TL][40][3]; static int en_tl[3], en_tw[3];
static void en_grain(void)
{
    memset(en_tl, 0, sizeof en_tl);
    for (int n = 0; n < (int)(sizeof EN_S / sizeof EN_S[0]); n++) {
        const EnS *q = &EN_S[n]; int cm[40][3], rm[40][3], all[3] = { 0, 0, 0 }, g = q->g;
        memset(cm, 0, sizeof cm); memset(rm, 0, sizeof rm);
        for (int j = 0; j < q->h; j++) for (int i = 0; i < q->w; i++) {
            Rgb c = rgb15(pic[q->y0 + j][q->x0 + i]); int v[3] = { c.r, c.g, c.b };
            for (int k = 0; k < 3; k++) { cm[i][k] += v[k]; rm[j][k] += v[k]; all[k] += v[k]; }
        }
        for (int j = 0; j < q->h && en_tl[g] < EN_TL; j++, en_tl[g]++) for (int i = 0; i < q->w; i++) {
            Rgb c = rgb15(pic[q->y0 + j][q->x0 + i]); int v[3] = { c.r, c.g, c.b };
            for (int k = 0; k < 3; k++)
                en_tp[g][en_tl[g]][i][k] = (short)(v[k] * 16 - cm[i][k] * 16 / q->h - rm[j][k] * 16 / q->w + all[k] * 16 / (q->w * q->h));
        }
        en_tw[g] = q->w;
    }
}
static int en_amp = 256;                                 /* the mottle's strength (x 256): the rust pipe's elbow is smoother than its stretch under the flange */
static void en_tile(int g, int n, int lane, int a, int i, int f, int *m)
{
    if ((n + lane) & 1) { a = en_tl[g] - 1 - a; i = en_tw[g] - 1 - i; }
    for (int k = 0; k < 3; k++) m[k] += en_tp[g][a][i][k] * f;
}
static Rgb en_grained(Rgb c, int g, int t, int v, int H)  /* t along (texels), v of H across (1/256) */
{
    int w = en_tw[g], L = en_tl[g], P = L - 4, i = clampi(v, 0, H > 0 ? H - 1 : 0) >> 8, lane = i / w, n, a, l = lum(c), k, m[3] = { 0, 0, 0 };
    i -= lane * w; t += lane * (L / 2 + 5);
    n = fdiv(t, P); a = fmodi(t, P);                     /* (tiles overlap by 4 texels, cross-faded) */
    if (a < 4) { en_tile(g, n - 1, lane, a + P, i, 4 - a, m); en_tile(g, n, lane, a, i, a + 1, m); }
    else en_tile(g, n, lane, a, i, 5, m);
    k = (l < 24 ? 0 : l < 56 ? (l - 24) * 8 : 256) * en_amp / 256;   /* (none in the deep shadow) */
    c.r = clampi(c.r + m[0] * k / (5 * 4096), 0, 255);
    c.g = clampi(c.g + m[1] * k / (5 * 4096), 0, 255);
    c.b = clampi(c.b + m[2] * k / (5 * 4096), 0, 255);
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
/* the copper pipe's way on: after the picture's vertical - ring - 46 degrees - ring - 23 degrees, the same turn back
   (23 degrees - ring - 46 degrees - ring - upright), i.e. an offset, as the big grey pipe makes at the top left.  A
   straight piece looks the same whichever way the pipe bends (its shading is its axis' direction's) and so does a ring
   (its plane's), so the pieces are the picture's by translation: the second ring's copy EN_T[0] from it, the first
   ring's EN_T[1].  en_jn(): a texel's rows below the middle line of ring copy k (so above it: < 0) */
static const int EN_T[2][2] = { { 32, -13 }, { 67, -49 } };
static int en_lerpx(const int *xs, const int *ys, int n, int x256)   /* as en_lerp, the end pieces going on */
{
    int i = 0;
    while (i < n - 2 && x256 > xs[i + 1] * 256) i++;
    return ys[i] + (int)((long long)(ys[i + 1] - ys[i]) * (x256 - xs[i] * 256) / ((xs[i + 1] - xs[i]) * 256));
}
static int en_jn(int k, int X, int y)
{
    static const int J2X[] = { 288, 304, 315, 320, 326, 330 }, J2Y[] = { 33664, 34304, 35328, 35968, 37888, 39168 };
    static const int J1X[] = { 260, 275, 285, 290, 296, 301, 306, 310, 314 }, J1Y[] = { 39552, 39552, 39552, 39936, 40704, 41600, 42624, 43776, 44928 };
    int xs = PW * 256 - 128 + (X - EN_T[k][0]) * 256, ys = (y - EN_T[k][1]) * 256 + 128;
    return ys - (k ? en_lerpx(J1X, J1Y, 9, xs) : en_lerpx(J2X, J2Y, 6, xs));
}
static void en_crun(int X)
{
    int top = EN_CTOP(X);
    for (int y = 0; y < PH; y++) {
        int v = y * 256 + 128 - top, a = clampi(v + 128, 0, 256), b = clampi(EN_CH - v + 128, 0, 256), t, f;
        if (b < a) a = b;
        if (a <= 0 || en_jn(0, X, y) < 0) continue;      /* (it ends under the ring's copy) */
        t = 8 * 256 + clampi(v, 0, EN_CH) * 236 / 256; f = t & 255; t >>= 8;
        en_over(X, y, en_grained(mix(en_bp[clampi(t, 0, 63)], en_bp[clampi(t + 1, 0, 63)], f), 2, X, clampi(v, 0, EN_CH - 1), EN_CH), a);
    }
}
/* the second ring past the edge: the first ring's own end.  Both rings are the same fitting and the second is cut where
   its rim's lower line falls 1 row a column, as the first one's does at column 300: its rim and groove are the first
   ring's texels from there on (EN_RT: 19 columns right, 17 rows up; the rim's upper line there is at row 138, the
   second ring's own at the edge 137.5, the groove 152 for 151), with its second bolt and its dark end round the
   pipe, 15 columns in all.  Above the rim the second ring shows its top face (rows 128.5-137.5 at the edge, the first
   one almost none): it closes along a quarter ellipse over 7 columns with the texels it has at the edge.  Under the
   ring the middle piece's last wedge goes on to its lower outline (167.5 - 1.05 X). */
static const int EN_T1X[] = { 266, 270, 285, 290, 296, 301, 306, 310, 314, 316 }, EN_T1Y[] = { 38656, 38221, 38349, 38656, 39040, 39680, 40448, 41344, 42496, 42752 };
static const int EN_B1X[] = { 266, 268, 271, 275, 283, 288, 294, 300, 305, 308, 311, 316 }, EN_B1Y[] = { 39936, 41088, 41600, 40832, 40704, 41344, 42240, 43264, 44288, 45312, 46592, 46592 };
#define EN_RTX 300
#define EN_RTY 17
#define EN_RTN 15
static int en_ring_geo(int X, int *ytf, int *yrim, int *yg, int *ylow)
{
    int x = X * 256 - 128, q, xt = 8 * 256 + 128;
    if (X > EN_RTN) return 0;
    *yrim = en_lerp(EN_T1X, EN_T1Y, 10, (EN_RTX + X) * 256 + 128) - EN_RTY * 256;
    *yg = en_lerp(EN_B1X, EN_B1Y, 12, (EN_RTX + X) * 256 + 128) - EN_RTY * 256;
    q = x >= xt ? 256 : 256 - isqrt_i((int)(((long long)xt * xt - (long long)x * x) * 256 / ((long long)xt * xt / 256)));   /* 1 - sqrt(1 - (x / xt)^2), x 256 */
    *ytf = 128 * 256 + 128 + 31 * q;
    if (*ytf > *yrim) *ytf = *yrim;
    *ylow = 167 * 256 + 128 - 269 * x / 256;
    return 1;
}
static void en_ring(int X)
{
    int ytf, yrim, yg, ylow, sx = EN_RTX + X;
    if (!en_ring_geo(X, &ytf, &yrim, &yg, &ylow)) return;
    for (int y = 120; y < 175; y++) {
        int yc = y * 256 + 128, sy = y + EN_RTY, a;
        if (yc < ytf - 128) continue;
        if (yc < yrim) {                                 /* the top face */
            a = clampi(yc - ytf + 128, 0, 256);
            en_over(X, y, en_bil(pic, en_edge(1), 128 * 256 + 128 + (int)((long long)(yc - ytf) * (9 * 256) / (yrim - ytf + 1))), a);
        }
        if (yc >= yrim - 128 && yc <= yg + 128) {        /* the rim, the groove, the end */
            a = clampi(yc - yrim + 128, 0, 256);
            if ((pic_ov[sy][sx] == 14 || pic_ov[sy][sx] == 15) && sx < PW) en_over(X, y, rgb15(pic[sy][sx]), a);
        } else if (yc > yg && yc <= ylow + 128)           /* the middle piece under it */
            en_over(X, y, en_bil(pic, en_edge(1), 152 * 256 + (int)((long long)(yc - yg) * (15 * 256 + 128) / (ylow - yg + 1))), clampi(ylow + 128 - yc, 0, 256));
    }
}
/* --- the pipes' turns and fittings: the picture's own parts
   An upright stretch's cross-section, limb to limb from the left (columns [x0, x1) over rows [y0, y1)) */
static Rgb (*en_vp)[64];
static void en_vprof(int g, int x0, int x1, int y0, int y1)
{
    for (int k = 0; k < 64; k++) {
        int xs = x0 * 256 + 128 + (x1 - x0 - 1) * 256 * k / 63, m[3] = { 0, 0, 0 };
        for (int y = y0; y < y1; y++) { Rgb c = en_bil(pic, xs, y * 256 + 128); m[0] += c.r; m[1] += c.g; m[2] += c.b; }
        en_vp[g][k].r = m[0] / (y1 - y0); en_vp[g][k].g = m[1] / (y1 - y0); en_vp[g][k].b = m[2] / (y1 - y0);
    }
}
static Rgb en_vat(int g, int v, int D)                   /* v of D from the left limb (1/256) */
{
    int t = clampi((int)((long long)v * 63 * 256 / D), 0, 63 * 256 - 1);
    return mix(en_vp[g][t >> 8], en_vp[g][(t >> 8) + 1], t & 255);
}
/* a left run (en_run's r0, r1, d) that turns up after strip column xc: a quarter turn about the end of its upper
   outline (as tight as the picture's elbows: the rust pipe's outer outline is a circle about its inner corner), then
   upright, w columns wide, to the top.  A texel of the turn has the run's section and the upright section, each at
   its distance from the corner, weighed by where its normal points (up / sideways). */
static void en_rise(int X, int xc, int r0, int r1, int d, int w, int g)
{
    int D = (r1 - r0) * 256, dx = ((X - xc) * 256 - 128) * (r1 - r0) / w, yt = r0 * 256 - d;
    if (dx < 0 || dx > D + 128) return;
    for (int y = 0; y < PH; y++) {
        int dy = y * 256 + 128 - yt, v, t; Rgb c;
        if (dy > D + 256) break;
        if (dy <= 0) { v = dx; c = en_vat(g, D - v, D); t = xc + 28 - dy / 256; }
        else {
            v = isqrt_i(dx * dx + dy * dy); if (v > D + 128) continue;
            c = mix(en_bil(pic, en_edge(0), clampi(r0 * 256 + v, r0 * 256 + 128, r1 * 256 - 128)), en_vat(g, D - v, D), dx * 256 / (dx + dy));
            t = xc + dx * 28 / (dx + dy);
        }
        if (!g) en_amp = dy <= 0 && y < 74 ? 256 : 150;  /* (the rust pipe past its flange: as its stretch under the picture's) */
        en_over(X, y, en_grained(c, g, t, clampi(v, 0, D - 1), D), clampi(D - v + 128, 0, 256));
    }
}
/* the rust pipe's seam: in the picture its elbow starts at a seam, a light line across the pipe with a rivet either
   side (columns 14-22, rows 94-122: an arc, 21 at mid height).  The strip's turn starts at one too: the picture's
   seam turned half round (so it bows the other way, as a turn the other way does), each texel as its luminance over
   the pipe's either side of it (the line between the texels 5 columns before and 4 after), laid over the run */
static short en_sx[32], en_sr[32][8];
static void en_seam_k(void)
{
    for (int y = 94; y <= 122; y++) {
        int best = -1, xa = 18;
        for (int x = 13; x <= 23; x++) {
            int h = 2 * lum(rgb15(pic[y][x])) - lum(rgb15(pic[y][x - 2])) - lum(rgb15(pic[y][x + 2]));
            if (h > best && (y == 94 || abs(x - en_sx[y - 95]) <= 2)) { best = h; xa = x; }
        }
        en_sx[y - 94] = (short)xa;
        for (int d = -4; d <= 3; d++) {
            int a = lum(rgb15(pic[y][xa - 5])) + lum(rgb15(pic[y][xa - 6])), b = lum(rgb15(pic[y][xa + 4])) + lum(rgb15(pic[y][xa + 5]));
            int base = (a * (4 - d) + b * (d + 5)) / 18;
            en_sr[y - 94][d + 4] = (short)clampi(lum(rgb15(pic[y][xa + d])) * 256 / (base > 4 ? base : 4), 96, 400);
        }
    }
}
static void en_seam(int xc)
{
    for (int y = 96; y <= 115; y++) {                    /* (the picture's rows 101-120: the pipe's outlines left out) */
        int ys = 216 - y;
        for (int d = -4; d <= 3; d++) {
            Rgb *p = px_at(xc + en_sx[ys - 94] + d - 18, y);
            if (p && abs(d) <= 2 ? 1 : p && abs(en_sr[ys - 94][d + 4] - 256) > 60) *p = scale(*p, en_sr[ys - 94][d + 4]);
        }
    }
}
/* the rust pipe's flange (picture columns 1-45, rows 133-140, around the pipe's columns 7-38), on its upright stretch
   in the strip: its texels, column for column (xo: the strip column of picture column 0, yo: rows up) */
static void en_flange(int X, int xo, int yo)
{
    int sx = xo - X;
    if (sx < 1 || sx > 45) return;
    for (int sy = 133; sy <= 140; sy++) {
        int e = sx < 7 ? 7 - sx : sx > 38 ? sx - 38 : 0;  /* (the ends: rounded, no top line) */
        if (e && (sy == 133 || (e >= 5 && (sy < 135 || sy > 139)) || (e >= 6 && sx < 7 && (sy < 136 || sy > 138)) || (e >= 7 && (sy < 136 || sy > 138)))) continue;
        en_over(X, sy - yo, rgb15(pic[sy][sx]), 256);
    }
}
/* a joint, as the grey pipes have: a light line across, a paler texel before it and a darker one after (the upright
   pipe's at rows 111-113, columns 33-47, over the rows around: the measured contrast) */
static int en_jk[3];
static void en_joint_k(void)
{
    int m[3] = { 0, 0, 0 }, b = 0;
    for (int x = 33; x < 48; x++) {
        for (int k = 0; k < 3; k++) m[k] += lum(rgb15(pic[111 + k][x]));
        b += lum(rgb15(pic[108][x])) + lum(rgb15(pic[109][x])) + lum(rgb15(pic[115][x])) + lum(rgb15(pic[116][x]));
    }
    for (int k = 0; k < 3; k++) en_jk[k] = clampi(m[k] * 1024 / (b > 0 ? b : 1), 200, 340);
}
static void en_joint(int X, int y, int k) { Rgb *p = px_at(X, y); if (p) *p = scale(*p, en_jk[k]); }
/* the copper pipe's pieces after the second ring's copy: the 46 degree piece (the middle piece's cross-section, 40
   texels, about the line through the ring copies' centres) and the upright one (the first piece's, columns 260-299) */
#define EN_BX ((PW + 21) * 256)                          /* the piece's centre line: through here (1/256) */
#define EN_BY (133 * 256)
#define EN_AX ((PW + 22) * 256 + 128)                    /* the upright piece's left limb */
#define EN_AW (39 * 256)
static void en_cturn(int X)
{
    int xe = PW * 256 - 128 + X * 256;
    for (int y = 0; y < PH; y++) {
        int ye = y * 256 + 128, j0 = en_jn(0, X, y), j1, w, a, b; Rgb c;
        if (j0 >= 0) continue;
        j1 = en_jn(1, X, y);
        if (j1 >= 0) {
            w = ((xe - EN_BX) * 184 + (ye - EN_BY) * 179) / 256 + 20 * 256;
            a = clampi(w + 128, 0, 256); b = clampi(40 * 256 - w + 128, 0, 256); if (b < a) a = b;
            if (a <= 0) continue;
            w = clampi(w, 0, 40 * 256 - 1);
            b = 8 * 256 + w * 7 / 8; c = mix(en_bp[b >> 8], en_bp[(b >> 8) + 1], b & 255);
            en_over(X, y, en_grained(c, 2, ((xe - EN_BX) * 179 - (ye - EN_BY) * 184) >> 16, w, 40 * 256), a);
        } else {
            w = xe - EN_AX;
            a = clampi(w + 128, 0, 256); b = clampi(EN_AW - w + 128, 0, 256); if (b < a) a = b;
            if (a <= 0) continue;
            w = clampi(w, 0, EN_AW - 1);
            en_over(X, y, en_grained(en_vat(2, w, EN_AW), 2, 300 - y, w, EN_AW), a);
        }
    }
}
/* a ring's copy: the picture's texels between its upper and lower outline (the second ring's last columns are the
   strip's own, en_ring) */
static void en_ringcopy(int k, int X)
{
    static const int T2X[] = { 288, 291, 304, 320 }, T2Y[] = { 33280, 32845, 33152, 32896 };
    static const int B2X[] = { 288, 289, 292, 303, 307, 311, 315, 319, 320 }, B2Y[] = { 34048, 35200, 35456, 35200, 35789, 36480, 37376, 38528, 38912 };
    int sx = PW - 1 + X - EN_T[k][0], top, bot, a, b, rim = 0, low;
    if (k) { if (sx < 266 || sx > 315) return; top = en_lerp(EN_T1X, EN_T1Y, 10, sx * 256 + 128); bot = en_lerp(EN_B1X, EN_B1Y, 12, sx * 256 + 128); }
    else if (sx < 288) return;
    else if (sx < PW) { top = en_lerp(T2X, T2Y, 4, sx * 256 + 128); bot = en_lerp(B2X, B2Y, 9, sx * 256 + 128); }
    else if (!en_ring_geo(sx - PW + 1, &top, &rim, &bot, &low)) return;
    for (int sy = top / 256 - 1; sy <= bot / 256 + 1; sy++) {
        Rgb c;
        a = clampi(sy * 256 + 256 - top, 0, 256); b = clampi(bot - sy * 256, 0, 256); if (b < a) a = b;
        if (a <= 0 || sy < 0 || sy >= PH) continue;
        if (sx < PW) { if (pic_ov[sy][sx] != 14 && pic_ov[sy][sx] != 15) continue; c = rgb15(pic[sy][sx]); }
        else if (sy * 256 + 128 < rim) c = *px_at(sx - PW + 1, sy);          /* (its top face: the strip's own) */
        else {                                           /* (its end: the first ring's, as en_ring) */
            int tx = EN_RTX + sx - PW + 1, ty = sy + EN_RTY;
            if (tx >= PW || (pic_ov[ty][tx] != 14 && pic_ov[ty][tx] != 15)) continue;
            c = rgb15(pic[ty][tx]);
        }
        en_over(X, sy + EN_T[k][1], c, a);
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
/* The pipes the edges cut are in front of the player in the picture: the game draws EN_Q's last six quads (the left
   48 columns' pipes, the copper elbow at the right) after the actors.  Their continuation in the strips was back
   layer, so an actor jumping across the 4:3 edge was hidden on one side of it and whole on the other (the user's
   video, 2026-11-17).  en_ov keeps the strips' pipe texels (bit 15 set; 0 elsewhere), and shims/gpu.c draws them
   again when the game draws the first of those six quads (port_ws_backdrop_over). */
static u16 *en_ov; static int en_ov_pad;
#define EN_RINGS 0                                       /* 1: the copper pipe's turn with copies of the picture's two riveted rings on
                                                            its joints (the first version; the user, 2026-11-17: "the joints of the
                                                            right pipe are horrible, take the rivets off"): now a plain turn */
#define EN_RX 13                                         /* the rust pipe turns up after this strip column */
#define EN_GX 62                                         /* the grey pipe */
static void gen_en(int pad)
{
    if (!ws_need_gen_en()) return;
    Rgb *bk;
    if (!canvas_open(pad + 32)) return;                  /* (a margin: a column never depends on the strip's width) */
    bk = malloc((size_t)cw * PH * sizeof *bk);
    free(en_ov); en_ov = calloc((size_t)2 * pad * PH, 2); en_ov_pad = pad;
    en_bprof(); en_flight(); en_grain(); en_joint_k(); en_seam_k();
    en_vprof(0, 7, 39, 141, 147); en_vprof(1, 30, 51, 88, 108); en_vprof(2, 260, 300, 204, 224);
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side;
        for (int X = 1; X <= cw; X++) {
            int ft = en_foot(side, X);
            for (int y = 0; y < PH; y++) put(X, y, y * 256 + 128 >= ft ? en_floor(side, X, y) : en_wall(side, X, y));
        }
        en_seams(side, cw - 2);
        for (int X = 1; X <= pad; X++) en_hoses(side, X);
        if (bk) memcpy(bk, cv, (size_t)cw * PH * sizeof *cv);   /* the back layer: what follows stands in front of the player */
        for (int X = 1; X <= pad; X++) {
            if (!side) {
                en_amp = 150;
                if (X <= EN_RX) en_run(0, X, 91, 126, en_ease(X), 0); else en_rise(X, EN_RX, 91, 126, en_ease(X), 32, 0);
                en_amp = 256;
                en_flange(X, EN_RX + 39, 59);
                if (X <= EN_GX) en_run(0, X, 145, 183, en_ease(X), 1); else en_rise(X, EN_GX, 145, 183, en_ease(X), 38, 1);
                for (int k = 0; k < 3; k++) {            /* (its joints: 28 columns apart on the run, i.e. 21 rows on the 29 wide pipe) */
                    if (X == 21 + k || X == 49 + k) for (int y = 144; y < 182; y++) en_joint(X, y, k);
                    if (X > EN_GX && X <= EN_GX + 38) { en_joint(X, 106 - k, k); en_joint(X, 38 - k, k); }
                }
                en_limb(X);
            } else { en_crun(X); en_ring(X); }
        }
        if (!side) en_seam(EN_RX - 1);
        if (side) {
            for (int X = 1; X <= pad; X++) en_cturn(X);
            if (EN_RINGS) for (int X = 1; X <= pad; X++) { en_ringcopy(0, X); en_ringcopy(1, X); }
        }
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
            {   size_t at = (size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X), c = (size_t)y * cw + X - 1;
                strip[at] = quant(cv[c]);
                if (en_ov) en_ov[at] = bk && memcmp(&bk[c], &cv[c], sizeof *cv) ? (u16)(strip[at] | 0x8000) : 0;
            }
    }
    free(bk);
    canvas_close();
}
