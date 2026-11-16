/* runtime/ws_backdrop.c: AREA06 s2 and s1, the Mermaid's Singing Rock (included there; docs/PORT.md "More rooms with
 * picture strips").
 *
 * s2, the mermaid on her rock: a fixed 320 x 224 picture, CLUT (128, 511): 256 columns from page (128, 0) and 64 from
 * page (256, 0).  Over it the game draws quads of the same pages: six cloud pieces (one of them runs 64 columns out of
 * the picture's left edge: the game closes that cloud itself) and the sea, a loop of four pictures of the waves
 * (MM2_Q: phase 4 is the picture itself; 15 to 35 vblanks each).  The strips are made once per phase, every one from
 * the same source texels of its own phase, so the sea in them moves with the picture's; the phase is read from the
 * wave objects (mm2_phase).
 *
 * s1, the beach: a 640 x 224 picture, CLUT (128, 510), three sprites from pages (576, 0), (704, 0) and (832, 0) at
 * frame row 16, moved by the camera: its left edge is at frame column -2 at the room's left end, its right edge at
 * column 320 at the right end.  Only the two ends need content: mm_take() fills the frame's strips from the 640
 * columns and the two extensions (made once), wherever the camera is.
 *
 * Measured on the pictures:
 *   s2: the horizon is the dark line on row 134, level over the whole width; at the left edge the sea ends on the sand
 *       on row 185 (the shore is level there and falls to the right: row 186 at column 20, 190 at 70, 200 at 120, the
 *       same in all four phases), the sand goes down to the rock mound in the corner, whose top runs from row 204 at
 *       column 40 to row 214 at the edge (10 rows in 40 columns) with a lit rim of 3 rows; at the right edge the cloud
 *       is closed (it ends at column 316), the small rock's tail is cut on rows 171-175 (its ridge falls 5 rows in 8
 *       columns, its waterline is row 176 with two rows of foam) and the flat rock in the corner on rows 213-223 (its
 *       top is level).  The sky's streaks are arcs round (145, -62) at the left edge (a circle through (0, 33),
 *       (50, 83), (167, 110)), about level at the right one.  The wave quads end at column 317: in phases 1-3 the
 *       picture's columns 318-319 still show phase 4.
 *   s1: sky to row 151, the sea to row 179, the sand below, all level at the left end; the right end is the arch's
 *       pillar from row 0 to its foot on row 205: strata rising to the right at 0.4 rows per column down to the
 *       bulge's lower edge (row 159 at the edge, falling 0.14 rows per column to the right), a smooth neck, the foot
 *       from row 180 (level) to its underside (row 205, falling 0.21 per column).
 *
 * The strips, every texel the picture's own (plus a tone), nothing mirrored:
 *   - a band (sky, sea, sand) is its tone + its detail: the tone is the band's mean at the edge per row (a box of the
 *     band's own texels), carried outwards level (its own row's at the join, the mean of more rows further out, so a
 *     streak or a foam line the edge cuts fades out); the detail is a picture texel of the same row from a clean
 *     window, translated (the sand of s2 and its sea under row 185, which the picture has no clean row of: of a box),
 *     minus the mean round it.  Next to the join the detail is the edge's own, cross-faded into the copied one over a
 *     few columns; in s2's left sky it is carried along the arcs and nothing is copied (the picture's streaks
 *     elsewhere run another way);
 *   - s2's cut rocks are closed with their own texels moved along their outline: the mound (40 columns, 10 rows) until
 *     it leaves the frame's bottom, the small rock's tail (8, 5) down to its waterline, the flat rock level for 30
 *     columns, then down through the bottom;
 *   - s1's pillar is closed with its own rock: next to the join the edge's texels carried on along the strata, then
 *     the rock 56 columns in (its rows moved so that the bulge's edge and the foot's lines go on as measured), the
 *     two cut along the cheapest path down the rows (a seam that wanders, no straight line; cross-faded in the smooth
 *     neck); inside an outline drawn here (MM1_OL: the picture has no source for its shape), darker towards the far
 *     side as the arch's other leg is; beyond it the sky, sea and sand are the picture's columns 8 on (its far end,
 *     never in the same frame), the sand under the foot the arch's own at the edge's tone.
 * Every colour is then the nearest of the picture's CLUT.  Integers only; a texel depends on its place, not on the
 * strip's width. */
#define MM_H2 1224                                        /* the ROOMS rows' marks (no picture has these heights) */
#define MM_H1 1225
#define MM_W 640
#define MM_NPH 4
static void gen_mm(int pad) { (void)pad; }              /* (the table's gen: mm_take makes the strips) */
static u16 mm_src[MM_NPH][PH][MM_W];                     /* s2: the picture with each phase's quads; s1: [0] */
static int mm_w, mm_ph;                                  /* the picture's columns; the phase being made */
static u16 mm_clut[256]; static u16 mm_qc[32768]; static u8 mm_qh[32768];
static Rgb *mm_cv; static int mm_cw;                     /* the canvas: [side][y][X - 1] */

static Rgb mm_px(int x, int y) { return rgb15(mm_src[mm_ph][clampi(y, 0, PH - 1)][clampi(x, 0, mm_w - 1)]); }
static u16 mm_quant(Rgb c)
{
    int r = clampi(c.r + 4, 0, 255) >> 3, g = clampi(c.g + 4, 0, 255) >> 3, b = clampi(c.b + 4, 0, 255) >> 3, k = r | g << 5 | b << 10;
    if (!mm_qh[k]) {
        int bd = 1 << 30, best = k;
        for (int i = 0; i < 256; i++) {
            int q = mm_clut[i] & 0x7FFF, dr, dg, db, d;
            if (!q) continue;
            dr = r5((u16)q) - r; dg = g5((u16)q) - g; db = b5((u16)q) - b; d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (d < bd) { bd = d; best = q; }
        }
        mm_qc[k] = (u16)best; mm_qh[k] = 1;
    }
    return mm_qc[k];
}
static Rgb *mm_at(int side, int X, int y) { return &mm_cv[((size_t)side * PH + y) * mm_cw + X - 1]; }
static void mm_put(int side, int X, int y, Rgb c) { if (X >= 1 && X <= mm_cw && y >= 0 && y < PH) *mm_at(side, X, y) = c; }
static int mm_d2(Rgb a, Rgb b) { int r = a.r - b.r, g = a.g - b.g, bb = a.b - b.b; return 3 * r * r + 4 * g * g + 2 * bb * bb; }

/* --- bands */
enum { MC_ANY, MC_SEA, MC_SAND };
static int mm_is(int cls, Rgb c)
{
    return cls == MC_SEA ? c.b > c.r : cls == MC_SAND ? c.r > c.b + 24 && c.r >= 208 : 1;
}
/* the mean (x 16) of the class' texels in the box (x +- R, y +- r), rows within [y0, y1); the texels counted */
static int mm_lp(int cls, int x, int y, int R, int r, int y0, int y1, int *o)
{
    int s[3] = { 0, 0, 0 }, n = 0, xa = x - R < 0 ? 0 : x - R, xb = x + R >= mm_w ? mm_w - 1 : x + R, ya = y - r < y0 ? y0 : y - r,
        yb = y + r >= y1 ? y1 - 1 : y + r;
    for (int v = ya; v <= yb; v++)
        for (int u = xa; u <= xb; u++) {
            Rgb c = rgb15(mm_src[mm_ph][v][u]);
            if (cls != MC_ANY && !mm_is(cls, c)) continue;
            s[0] += c.r; s[1] += c.g; s[2] += c.b; n++;
        }
    if (n) for (int c = 0; c < 3; c++) o[c] = s[c] * 16 / n;
    return n;
}
typedef struct {
    short y0, y1, cls;                                    /* rows; the class of its texels */
    short wx0, wx1;                                       /* the sources' columns */
    short by, sy0, sh, dk;                                /* rows from by on: sources of the sh rows from sy0, their
                                                             detail x dk / 16 (by 0: every row its own) */
    short R, r, jw;                                       /* the mean's box; the join's columns */
    short ye, in;                                         /* the last row the edge shows the band on (0: y1 - 1); the
                                                             edge's column, counted from the picture's */
    short dn;                                             /* the copied detail x dn / 16 */
    short acx, acy;                                       /* the streaks are arcs round (acx, acy) (acx 0: level rows) */
} MmBand;
#define MM_FAR 40                                         /* columns over which the edge's own rows fade out */
static void mm_band(int side, const MmBand *b)
{
    static int tn[PH][3], tf[PH][3], de[PH][3];
    int ex = side ? mm_w - 1 - b->in : b->in, ye = b->ye ? b->ye : b->y1 - 1, ww = b->wx1 - b->wx0;
    for (int y = b->y0; y < b->y1; y++) {
        int yy = y > ye ? ye : y, rf = clampi(yy - b->y0 < ye - yy ? yy - b->y0 : ye - yy, b->r, 8);
        Rgb e = mm_px(ex, yy);
        if (!mm_lp(b->cls, ex, yy, b->R, b->r, b->y0, ye + 1, tn[y])) {
            if (y > b->y0) memcpy(tn[y], tn[y - 1], sizeof tn[y]); else { tn[y][0] = e.r * 16; tn[y][1] = e.g * 16; tn[y][2] = e.b * 16; }
        }
        if (!mm_lp(b->cls, ex, yy, b->R, rf, b->y0, ye + 1, tf[y])) memcpy(tf[y], tn[y], sizeof tf[y]);
        if (y <= ye && mm_is(b->cls, e)) { de[y][0] = e.r * 16 - tn[y][0]; de[y][1] = e.g * 16 - tn[y][1]; de[y][2] = e.b * 16 - tn[y][2]; }
        else de[y][0] = de[y][1] = de[y][2] = 0;
    }
    for (int y = b->y0; y < b->y1; y++) {
        int box = b->by && y >= b->by;
        int blk = box ? (y - b->by) / b->sh : 0;         /* (a box' rows again: other columns of it) */
        for (int X = 1; X <= mm_cw; X++) {
            int sx = side ? b->wx0 + fmodi(X - 1 + 19 * blk, ww) : b->wx0 + fmodi(ww - X + 19 * blk, ww);
            int sy = box ? b->sy0 + (y - b->by) % b->sh : y;
            int far = X >= MM_FAR ? 256 : X * 256 / MM_FAR, w = X <= b->jw ? (b->jw + 1 - X) * 256 / (b->jw + 1) : 0, ls[3], d[3] = { 0, 0, 0 };
            int yt = y;                                  /* the edge's row whose detail goes on here: the arc's */
            Rgb s = mm_px(sx, sy), o;
            if (b->acx && w) {
                yt = b->acy + isqrt_i((X + b->acx) * (X + b->acx) + (y - b->acy) * (y - b->acy) - b->acx * b->acx);
                if (yt >= b->y1) { yt = y; w = 0; }
            }
            if (mm_is(b->cls, s) && mm_lp(b->cls, sx, sy, b->R, b->r, box ? b->sy0 : b->y0, box ? b->sy0 + b->sh : ye + 1, ls)) {
                d[0] = s.r * 16 - ls[0]; d[1] = s.g * 16 - ls[1]; d[2] = s.b * 16 - ls[2];
                for (int c = 0; c < 3; c++) d[c] = d[c] * (box ? b->dk : b->dn) / 16;
            }
            o.r = (tn[y][0] + (tf[y][0] - tn[y][0]) * far / 256 + (de[yt][0] * w + d[0] * (256 - w)) / 256 + 8) >> 4;
            o.g = (tn[y][1] + (tf[y][1] - tn[y][1]) * far / 256 + (de[yt][1] * w + d[1] * (256 - w)) / 256 + 8) >> 4;
            o.b = (tn[y][2] + (tf[y][2] - tn[y][2]) * far / 256 + (de[yt][2] * w + d[2] * (256 - w)) / 256 + 8) >> 4;
            mm_put(side, X, y, o);
        }
    }
}

/* ================================================================== s2: the mermaid on her rock */
#define MM2_HZ 134                                        /* the horizon: the sea's first row */
#define MM2_SH 185                                        /* the sand's first row at the left edge */
static const Piece MM2_Q[MM_NPH][6] = {                   /* the waves' quads per phase (y: picture rows); [0] is phase 4 */
    { { 256, 64, 0, 16, 16, 270, 176 }, { 384, 56, 232, 40, 24, 0, 136 } },
    { { 256, 64, 144, 56, 24, 142, 177 }, { 256, 128, 120, 128, 40, 198, 168 }, { 256, 192, 168, 64, 88, 254, 136 },
      { 384, 0, 160, 184, 80, 0, 136 }, { 384, 184, 160, 32, 32, 184, 136 }, { 384, 184, 200, 72, 32, 184, 192 } },
    { { 256, 64, 112, 56, 24, 142, 176 }, { 256, 128, 80, 128, 40, 198, 168 }, { 256, 128, 168, 64, 88, 254, 136 },
      { 384, 0, 80, 184, 80, 0, 136 }, { 384, 184, 120, 72, 32, 184, 192 }, { 384, 184, 80, 32, 32, 184, 136 } },
    { { 256, 64, 88, 56, 24, 150, 180 }, { 256, 128, 40, 112, 40, 206, 168 }, { 256, 64, 168, 64, 88, 254, 136 },
      { 384, 0, 0, 176, 80, 0, 136 }, { 384, 176, 40, 80, 32, 176, 192 }, { 384, 176, 0, 40, 32, 176, 136 } },
};
#define MM2_ACX 145                                       /* the sky's streaks at the left edge: arcs round this point */
#define MM2_ACY (-62)
static const MmBand MM2_B[2][3] = {
    { { 0, MM2_HZ, MC_ANY, 8, 168, 0, 0, 0, 0, 20, 2, 20, 0, 0, 0, MM2_ACX, MM2_ACY }, { MM2_HZ, MM2_SH, MC_SEA, 8, 150, 0, 0, 0, 0, 20, 1, 14, 0, 0, 16, 0, 0 },
      { MM2_SH, PH, MC_SAND, 2, 60, MM2_SH, 190, 14, 8, 6, 2, 3, 209, 0, 16, 0, 0 } },
    { { 0, MM2_HZ, MC_ANY, 40, 200, 0, 0, 0, 0, 20, 2, 6, 0, 0, 16, 0, 0 },
      { MM2_HZ, PH, MC_SEA, 10, 150, MM2_SH, 171, 14, 16, 20, 1, 8, 212, 2, 16, 0, 0 } },   /* (the wave quads end at column 317) */
};
static int mm2_rock(Rgb c) { return c.r >= c.b; }
static void mm2_gen(void)
{
    int mtop[40];
    for (int i = 0; i < 3; i++) mm_band(0, &MM2_B[0][i]);
    for (int i = 0; i < 2; i++) mm_band(1, &MM2_B[1][i]);
    /* the mound in the left corner: its texels 40 k columns in and 10 k rows up, from its lit rim down */
    for (int x = 0; x < 40; x++) {
        int y = 200;
        while (y < PH && lum(mm_px(x, y)) >= 120) y++;
        mtop[x] = y;
    }
    for (int X = 1; X <= mm_cw && X <= 80; X++) {
        int k = (X + 39) / 40, sx = 40 * k - X;
        for (int y = 200; y < PH; y++) if (y - 10 * k >= mtop[sx] - 3) mm_put(0, X, y, mm_px(sx, y - 10 * k));
    }
    /* the small rock's tail at the right edge: 8 columns in and 5 rows up, down to its waterline; its foam */
    for (int X = 1; X <= mm_cw && X <= 8; X++) {
        for (int y = 160; y < 176; y++) if (mm2_rock(mm_px(311 + X, y - 5))) mm_put(1, X, y, mm_px(311 + X, y - 5));
        for (int y = 176; y < 178; y++) mm_put(1, X, y, mm_px(311 + X, y));
    }
    /* the flat rock in the right corner: level for 30 columns, then down through the frame's bottom */
    for (int X = 1; X <= mm_cw && X <= 44; X++) {
        int top = 213 + (X > 30 ? (X - 30) * (X - 30) / 12 : 0);
        for (int y = top; y < PH; y++) if (mm2_rock(mm_px(275 + X, y - (top - 213)))) mm_put(1, X, y, mm_px(275 + X, y - (top - 213)));
    }
}

/* ================================================================== s1: the beach */
static const MmBand MM1_B[3] = {
    { 0, 152, MC_ANY, 8, 200, 0, 0, 0, 0, 20, 2, 6, 0, 0, 16, 0, 0 }, { 152, 180, MC_ANY, 8, 200, 0, 0, 0, 0, 20, 1, 10, 0, 0, 16, 0, 0 },
    { 180, PH, MC_ANY, 8, 200, 0, 0, 0, 0, 20, 2, 6, 0, 0, 16, 0, 0 },
};
#define MM1_SAND 208                                     /* the sand under the foot at the edge: from this row */
static const MmBand MM1_BF = { MM1_SAND, PH, MC_ANY, 540, 636, 0, 0, 0, 0, 8, 2, 4, 0, 0, 16, 0, 0 };   /* the sand under the foot */
/* the pillar's closing: its outline (row, last column), drawn here */
static const short MM1_OL[][2] = { { 0, 12 }, { 14, 22 }, { 34, 31 }, { 62, 39 }, { 100, 45 }, { 130, 46 }, { 155, 42 }, { 175, 37 },
                                   { 185, 38 }, { 195, 43 }, { 203, 48 }, { 208, 50 }, { 224, 50 } };
#define MM1_BG0 8                                         /* the sky, sea and sand behind it: the picture's columns from here */
#define MM1_BGW 192
#define MM1_SW 56                                         /* its texels: the rock this many columns in */
#define MM1_SEAM 14                                       /* the columns the cut between the two wanders in */
static int mm1_ol16(int y)                               /* its last column on row y (x 16) */
{
    int i = 0, v;
    while (MM1_OL[i + 1][0] <= y && MM1_OL[i + 1][0] < PH) i++;
    v = MM1_OL[i][1] * 16 + (MM1_OL[i + 1][1] - MM1_OL[i][1]) * 16 * (y - MM1_OL[i][0]) / (MM1_OL[i + 1][0] - MM1_OL[i][0]);
    return v + (noise(0, y, 8, 26, 61) - 128) * 40 / 128 + (noise(0, y, 8, 7, 62) - 128) * 20 / 128;
}
/* the pillar's lines at column X from the edge (negative: in the picture), rows x 16: the bulge's lower edge (159 at the
   edge, 0.14 rows per column) and the foot's underside (205, 0.21 per column); the foot's top is level, row 180 */
static int mm1_c1(int X) { return 159 * 16 + X * 9 / 4; }
static int mm1_c3(int X) { return 205 * 16 + (X > 12 ? 12 : X) * 27 / 8; }
/* the rock sw columns in, its rows moved so that those lines meet the ones at X; above the bulge's edge up more rows
   down (the strata rise to the right) */
static Rgb mm1_rock(int X, int y, int sw, int up)
{
    int y16 = y * 16 + 8, a1 = mm1_c1(X), a3 = mm1_c3(X), b1 = mm1_c1(X - sw), b3 = mm1_c3(X - sw), sy;
    if (y16 < a1) { sy = y16 * b1 / a1 + up * 16; if (sy > b1 - 8) sy = b1 - 8; }
    else if (y16 < 180 * 16) sy = b1 + (y16 - a1) * (180 * 16 - b1) / (180 * 16 - a1);
    else sy = 180 * 16 + (y16 - 180 * 16) * (b3 - 180 * 16) / (a3 - 180 * 16);
    return mm_px(mm_w - 1 + X - sw, fdiv(sy, 16));
}
static Rgb mm1_far(int X, int y) { return mm1_rock(X, y, MM1_SW, 0); }
/* the edge's own texels carried on: 5 columns in, along the strata (2 rows per 5 columns) */
static Rgb mm1_near(int X, int y) { int k = (X + 4) / 5; return mm1_rock(X, y, 5 * k, 2 * k); }
static void mm1_gen(void)
{
    static short cut[PH];
    static int cost[PH][MM1_SEAM + 2];
    static int sand[PH][3];                              /* the sand at the edge against the sand copied next to it,
                                                            per row (x 16; above the foot's underside: fading out) */
    for (int y = PH - 1; y >= 181; y--) {
        int a[3], b[3];
        if (y >= MM1_SAND) {
            mm_lp(MC_ANY, mm_w - 9, y, 8, 2, MM1_SAND, PH, a); mm_lp(MC_ANY, MM1_BG0 + 8, y, 8, 2, MM1_SAND, PH, b);
            for (int c = 0; c < 3; c++) sand[y][c] = a[c] - b[c];
        } else for (int c = 0; c < 3; c++) sand[y][c] = sand[MM1_SAND][c] * (y - 181) / (MM1_SAND - 181);
    }
    for (int i = 0; i < 3; i++) mm_band(0, &MM1_B[i]);
    /* the cut between the edge's texels carried on and the rock from further in: the cheapest path down the rows */
    for (int y = 0; y < PH; y++)
        for (int X = 1; X <= MM1_SEAM; X++) {
            int e = mm_d2(mm1_near(X, y), mm1_far(X, y)) + mm_d2(mm1_near(X + 1, y), mm1_far(X + 1, y)) + (X < 3 ? (3 - X) * 3000 : 0);
            if (y) {
                int m = cost[y - 1][X];
                if (X > 1 && cost[y - 1][X - 1] < m) m = cost[y - 1][X - 1];
                if (X < MM1_SEAM && cost[y - 1][X + 1] < m) m = cost[y - 1][X + 1];
                e += m;
            }
            cost[y][X] = e;
        }
    {   int X = 1;
        for (int i = 2; i <= MM1_SEAM; i++) if (cost[PH - 1][i] < cost[PH - 1][X]) X = i;
        for (int y = PH - 1; y >= 0; y--) {
            cut[y] = (short)X;
            if (y) {
                int n = X;
                if (X > 1 && cost[y - 1][X - 1] < cost[y - 1][n]) n = X - 1;
                if (X < MM1_SEAM && cost[y - 1][X + 1] < cost[y - 1][n]) n = X + 1;
                X = n;
            }
        }
    }
    mm_band(1, &MM1_BF);                                 /* (the rock goes over its first rows) */
    for (int y = 0; y < PH; y++) {
        int ol = mm1_ol16(y);
        for (int X = 1; X <= mm_cw; X++) {
            int bot = mm1_c3(X) - (X > 40 ? (X - 40) * (X - 40) * 16 / 20 : 0) + (noise(X, 0, 5, 8, 63) - 128) * 10 / 128;
            if (X * 16 - 8 > ol || y * 16 + 8 > bot) {
                Rgb c = mm_px(MM1_BG0 + fmodi(X - 1, MM1_BGW), y);
                if (y >= MM1_SAND) continue;             /* the sand under the foot: MM1_BF */
                if (y >= 181) {
                    c.r = clampi(c.r + fdiv(sand[y][0] + 8, 16), 0, 255); c.g = clampi(c.g + fdiv(sand[y][1] + 8, 16), 0, 255);
                    c.b = clampi(c.b + fdiv(sand[y][2] + 8, 16), 0, 255);
                }
                mm_put(1, X, y, c);
            }
            else {
                int t = clampi((X * 16 - (ol - 26 * 16)) * 256 / (26 * 16), 0, 256), k = 256 - 165 * t / 256 * t / 256;
                Rgb c = X <= cut[y] ? mm1_near(X, y) : mm1_far(X, y);
                if (y * 16 + 8 >= mm1_c1(X) && y < 180 && X <= MM1_SEAM)      /* the smooth neck: the two cross-faded */
                    c = mix(mm1_near(X, y), mm1_far(X, y), (X - 1) * 256 / (MM1_SEAM - 1));
                mm_put(1, X, y, scale(c, k));
            }
        }
    }
}

#ifndef WS_BACKDROP_MAIN
static u16 *mm_strip; static int mm_pad = -1, mm_room; static u32 mm_sum;
static u16 *mm_buf;                                      /* s1: this frame's strips */
static void mm_grab(const u16 *vram, int sc)
{
    if (sc == 2) {
        mm_w = PW;
        for (int i = 0; i < 256; i++) mm_clut[i] = vram[(size_t)511 * 1024 + 128 + i];
        for (int p = 0; p < MM_NPH; p++) {
            for (int y = 0; y < PH; y++) {
                for (int x = 0; x < 256; x++) mm_src[p][y][x] = tex8(vram, 128, x, y, 128, 511);
                for (int x = 0; x < 64; x++) mm_src[p][y][256 + x] = tex8(vram, 256, x, y, 128, 511);
            }
            for (int i = 0; i < 6; i++) {
                const Piece *q = &MM2_Q[p][i];
                for (int y = 0; y < q->h; y++) for (int x = 0; x < q->w; x++) {
                    u16 c = tex8(vram, q->tx, q->u + x, q->v + y, 128, 511);
                    if (c && q->y + y < PH && q->x + x < PW) mm_src[p][q->y + y][q->x + x] = c;
                }
            }
        }
    } else {
        mm_w = MM_W;
        for (int i = 0; i < 256; i++) mm_clut[i] = vram[(size_t)510 * 1024 + 128 + i];
        for (int y = 0; y < PH; y++) for (int x = 0; x < MM_W; x++) mm_src[0][y][x] = tex8(vram, 576, x, y, 128, 510);
    }
    memset(mm_qh, 0, sizeof mm_qh);
}
/* the picture's pages and CLUT, sampled (every 8th row): has the picture changed */
static u32 mm_check(const u16 *vram, int sc)
{
    u32 h = 2166136261u; int x0 = sc == 2 ? 128 : 576, x1 = sc == 2 ? 512 : 896, cy = sc == 2 ? 511 : 510;
    for (int i = 0; i < 256; i++) h = (h ^ vram[(size_t)cy * 1024 + 128 + i]) * 16777619u;
    for (int y = 3; y < 256; y += 8) for (int x = x0; x < x1; x++) h = (h ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return h;
}
static int mm_build(const u16 *vram, int pad, int sc)
{
    int nph = sc == 2 ? MM_NPH : 1;
    mm_grab(vram, sc);
    free(mm_strip); free(mm_buf); mm_buf = 0;
    mm_cw = pad; mm_cv = calloc((size_t)2 * PH * pad, sizeof *mm_cv); mm_strip = calloc((size_t)nph * PH * 2 * pad, 2);
    if (sc == 1) mm_buf = calloc((size_t)PH * 2 * pad, 2);
    if (!mm_cv || !mm_strip || (sc == 1 && !mm_buf)) { free(mm_strip); free(mm_cv); free(mm_buf); mm_strip = 0; mm_cv = 0; mm_buf = 0; return 0; }
    for (mm_ph = 0; mm_ph < nph; mm_ph++) {
        if (sc == 2) mm2_gen(); else mm1_gen();
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                u16 *row = mm_strip + ((size_t)mm_ph * PH + y) * 2 * pad;
                row[pad - X] = mm_quant(*mm_at(0, X, y)); row[pad + X - 1] = mm_quant(*mm_at(1, X, y));
            }
    }
    mm_ph = 0;
    free(mm_cv); mm_cv = 0;
    return 1;
}
/* the waves' phase: the picture the wave objects (type 74, subtypes 2-4: drawArea6SpriteQuad) show now */
static int mm2_phase(void)
{
    static int last;
    u8 *pool = PORT_M0(PORT_A_OBJECT_POOL);
    for (int k = 0; k < 200; k++) {
        u8 *o = pool + k * 0xD4; u32 anim = *(u32 *)(o + 0x24), d3c = *(u32 *)(o + 0x3C);
        if (!o[0] || o[2] != 74 || o[3] < 2 || o[3] > 4 || !anim || !d3c) continue;
        {   const short *f = (const short *)((size_t)d3c + (*(const short *)(size_t)anim << 2));
            const u8 *e = (const u8 *)((size_t)d3c + f[1]);
            int tx = ((*(const u16 *)(e + 6) + *(const short *)(o + 0x1E)) & 15) * 64;
            if (f[0] <= 0) continue;
            for (int p = 0; p < MM_NPH; p++) for (int i = 0; i < 6; i++)
                if (MM2_Q[p][i].w && MM2_Q[p][i].tx == tx && MM2_Q[p][i].u == e[0] && MM2_Q[p][i].v == e[1]) return last = p;
        }
    }
    return last;
}
/* a 256-column sprite of CLUT (128, 510 / 511) at frame (x, y) in section sc: the strips when it is that picture's */
static const u16 *mm_take(const u16 *vram, int pad, int sc, int x, int y, int w, int h, int u, int v, int tx, int ty, int cy, int *rows)
{
    u32 s;
    if (y != 16 || w != 256 || h != PH || u || v || ty) return 0;
    if (sc == 2 ? x != 0 || tx != 128 || cy != 511 : tx != 576 || cy != 510) return 0;
    s = mm_check(vram, sc);
    if (!mm_strip || mm_pad != pad || mm_room != sc || s != mm_sum) {
        Rgb t = rgb15(tex8(vram, sc == 2 ? 128 : 576, 4, 4, 128, cy));    /* (the sky in the top left corner: this picture) */
        mm_pad = pad; mm_room = sc; mm_sum = s;
        if (t.b < t.r + 24 || !mm_build(vram, pad, sc)) { free(mm_strip); mm_strip = 0; }
    }
    if (!mm_strip) return 0;
    *rows = PH;
    if (sc == 2) {
        return mm_strip + (size_t)mm2_phase() * PH * 2 * pad;
    }
    if (x <= -pad && x + MM_W >= 320 + pad) return 0;    /* the picture covers the wide frame */
    for (int j = 0; j < PH; j++) {
        u16 *row = mm_buf + (size_t)j * 2 * pad; const u16 *ext = mm_strip + (size_t)j * 2 * pad;
        for (int c = 0; c < pad; c++) {
            int e = c - pad - x, f = 320 + c - x;      /* the picture's column at the left / right strip's column c */
            row[c] = e < 0 ? (-e <= pad ? ext[pad + e] : 0) : e < MM_W ? mm_src[0][j][e] : 0;
            row[pad + c] = f >= MM_W ? (f - MM_W < pad ? ext[pad + f - MM_W] : 0) : f >= 0 ? mm_src[0][j][f] : 0;
        }
    }
    return mm_buf;
}
#endif
