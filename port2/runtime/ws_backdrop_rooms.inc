/* runtime/ws_backdrop.c: the generators of the rooms past AREA02 s5 (included there; docs/PORT.md "Room picture
 * strips").  The same rules as AREA02 s5's redraw: drawn by code, every colour sampled from the user's picture at run
 * time (ramps of a region's texels), integers only, position hashes, never two random draws in one argument list, a
 * structure depends on where it is (picture column) and not on the strip width.  The tables hold coordinates only:
 * where each element of the picture is (rows measured on the picture) and where to sample it.
 *
 * Shared helpers: per-region ramps (samp_box), a region map of the strip (rg) so balance_regions() can match each
 * region's brightness and contrast to the picture's columns next to the edge, the join and the quantisation to the
 * picture's own colours (emit), and simple elements: an irregular stone wall, planks, ground, foliage clumps. */

/* --- the canvas of a frontal room: cv / cw as AREA02 s5's (X = 1..cw outwards), plus the region of every pixel */
static u8 *rg;                                            /* region id per strip pixel (0 = none) */
static int wx_of(int side, int X) { return pcol(side, X); }   /* the picture column a strip column continues */
static void putr(int X, int y, Rgb c, int r) { Rgb *p = px_at(X, y); if (p) { *p = c; rg[(size_t)y * cw + X - 1] = (u8)r; } }
static int reg_at(int X, int y) { return (X >= 1 && X <= cw && y >= 0 && y < PH) ? rg[(size_t)y * cw + X - 1] : 0; }

/* samples of the texels in picture box [x0, x1) x [y0, y1) that pass pred (0: all) */
typedef int (*Pred)(u16 c);
static void samp_box(int x0, int x1, int y0, int y1, Pred pred)
{
    for (int y = clampi(y0, 0, PH); y < clampi(y1, 0, PH); y++)
        for (int x = clampi(x0, 0, PW); x < clampi(x1, 0, PW); x++)
            if (!pred || pred(pic[y][x])) samp_add(pic[y][x]);
}
/* both edges' boxes: columns [x0, x1) from the left edge and the mirror from the right one */
static void samp_edges(int x0, int x1, int y0, int y1, Pred pred) { samp_box(x0, x1, y0, y1, pred); samp_box(PW - x1, PW - x0, y0, y1, pred); }
static void samp_side(int side, int x0, int x1, int y0, int y1, Pred pred)
{
    if (side) samp_box(PW - x1, PW - x0, y0, y1, pred); else samp_box(x0, x1, y0, y1, pred);
}
static Rgb ramp_mean(const Ramp *rp) { Rgb s = { 0, 0, 0 }; for (int i = 0; i < NRAMP; i++) { s.r += rp->bin[i].r; s.g += rp->bin[i].g; s.b += rp->bin[i].b; } s.r /= NRAMP; s.g /= NRAMP; s.b /= NRAMP; return s; }

/* luminance statistics: trimmed mean and deviation (x 16) of a histogram (the lightest and darkest 10 % left out, so a
   barrel or a sign by the edge does not set the level) */
typedef struct { int h[256], n; } Hist;
static void hist_add(Hist *h, int l) { h->h[clampi(l, 0, 255)]++; h->n++; }
static void hist_stat(const Hist *h, int *mean16, int *dev16)
{
    int lo = h->n / 10, hi = h->n - h->n / 10, k = 0, n = 0; long long s = 0, sq = 0;
    for (int l = 0; l < 256; l++)
        for (int i = 0; i < h->h[l]; i++, k++) if (k >= lo && k < hi) { s += l; sq += l * l; n++; }
    if (!n) { *mean16 = *dev16 = 0; return; }
    *mean16 = (int)(s * 16 / n);
    *dev16 = isqrt_i((int)((sq * 256 - s * s * 256 / n) / n));
}

/* match each region of the strip to the picture next to the edge: the picture's texels of region r (pic_reg(side, x, y))
   in the 32 columns next to the edge (regions in the mask both: next to both edges, where one edge is mostly covered by an object) against the strip's region r in columns 4-15; L' = m + (L - ms) k, contrast k
   0.75..1.5.  pic_reg returns 0 for texels to leave out (objects by the edge). */
typedef int (*PicReg)(int side, int x, int y);
#define NREG 12
static void balance_regions(int side, PicReg pic_reg, int both)
{
    static Hist hp[NREG], hs[NREG]; int mp[NREG], dp[NREG], ms[NREG], ds[NREG], k[NREG];
    memset(hp, 0, sizeof hp); memset(hs, 0, sizeof hs);
    for (int sd = 0; sd < 2; sd++)
        for (int X = -31; X <= 0; X++)
            for (int y = 0; y < PH; y++) {
                int x = pcol(sd, X), r = pic_reg(sd, x, y);
                if (r > 0 && r < NREG && (sd == side || (both >> r & 1))) hist_add(&hp[r], lum(rgb15(pic[y][x])));
            }
    for (int X = 4; X <= 15 && X <= cw; X++)
        for (int y = 0; y < PH; y++) { int r = reg_at(X, y); if (r > 0 && r < NREG) hist_add(&hs[r], lum(cv[(size_t)y * cw + X - 1])); }
    for (int r = 1; r < NREG; r++) {
        hist_stat(&hp[r], &mp[r], &dp[r]); hist_stat(&hs[r], &ms[r], &ds[r]);
        k[r] = ds[r] && dp[r] ? clampi(dp[r] * 256 / ds[r], 192, 384) : 256;
        if (hp[r].n < 40 || hs[r].n < 20) ms[r] = 0;
#ifdef WS_BACKDROP_MAIN
        if (getenv("WSB_INFO") && hp[r].n) fprintf(stderr, "side %d region %d: picture %d (dev %d, n %d), strip %d (dev %d) -> k %d\n",
                                                   side, r, mp[r] / 16, dp[r] / 16, hp[r].n, ms[r] / 16, ds[r] / 16, k[r]);
#endif
    }
    for (int y = 0; y < PH; y++)
        for (int X = 1; X <= cw; X++) {
            Rgb *p = &cv[(size_t)y * cw + X - 1]; int r = reg_at(X, y), l = lum(*p), l2;
            if (r <= 0 || r >= NREG || !ms[r]) continue;
            l2 = clampi((mp[r] + (l * 16 - ms[r]) * k[r] / 256) / 16, 2, 255);
            *p = l ? scale(*p, clampi(l2 * 256 / l, 0, 1024)) : (Rgb){ l2, l2, l2 };
        }
}

/* the strip into the output: the three columns next to the picture blended with its edge column, every pixel the
   nearest colour the picture uses */
static void emit(int side)
{
    for (int y = 0; y < PH; y++)
        for (int X = 1; X <= cw; X++) {
            Rgb c = cv[(size_t)y * cw + X - 1];
            if (X <= 3) c = mix(rgb15(picx(side, 0, y)), c, X == 1 ? 110 : X == 2 ? 180 : 230);
            strip[(size_t)y * 2 * cw + (side ? cw + (X - 1) : cw - X)] = quant(c);
        }
}
static int canvas_open(int pad)
{
    pal_make();
    cw = pad; cv = calloc((size_t)pad * PH, sizeof *cv); rg = calloc((size_t)pad * PH, 1); occ = calloc((size_t)pad * PH, 1);
    if (!cv || !rg || !occ) { free(cv); free(rg); free(occ); cv = 0; rg = occ = 0; return 0; }
    return 1;
}
static void canvas_close(void) { free(cv); free(rg); free(occ); cv = 0; rg = occ = 0; }
static void canvas_clear(void) { memset(cv, 0, (size_t)cw * PH * sizeof *cv); memset(rg, 0, (size_t)cw * PH); }

/* a soft round blob of foliage (leaves / blossom / needles): centre (x16, y16) 1/16 px, radii, tone on rp; lit from
   the top left (the light of these rooms), darker rim at the bottom right; per-pixel grain from a position hash */
static void blob(int x16, int y16, int rx, int ry, const Ramp *rp, int tone, int r, int seed)
{
    int R = (rx > ry ? rx : ry) / 16 + 2, X0 = x16 / 16, Y0 = y16 / 16;
    for (int py = Y0 - R; py <= Y0 + R; py++)
        for (int X = X0 - R; X <= X0 + R; X++) {
            int dx = X * 16 + 8 - x16, dy = py * 16 + 8 - y16;
            long long q = (long long)dx * dx * 256 / (rx * rx) + (long long)dy * dy * 256 / (ry * ry);
            if (q > 256 || X < 1 || X > cw || py < 0 || py >= PH) continue;
            {   int t = tone - (dx * 64 / rx + dy * 96 / ry) / 2;              /* lit from the top left */
                if (q > 170 && dx / 2 + dy > 0) t -= 40;                        /* the rim away from the light */
                t += (int)(hash3(X * 7 + seed, py, seed) & 31) - 16;
                putr(X, py, ramp(rp, t), r);
            }
        }
}

/* ================================================================== AREA05 s1 / s3: the house with the stone wall
 * A frontal scene (a straight wall seen square on): every element is a horizontal band, so the strips continue each
 * band sideways, and at the join every band starts from what the picture has in its last columns.  Rows measured on
 * the picture (A5_FEN / A5_ETREE below hold the per-side ones):
 *   0-71   the wood: the pale sky between the tree tops in the first ~20 rows; conifers built of drooping branch tiers
 *          (the chevrons of dark gaps under each tier), the near ones bright and lit on their right flank, the far ones
 *          hazy blue-green and softer;
 *   72-102 the front fence: three khaki planks (left rows 72-77, 81-86, 93-99; one row lower on the right) on dark
 *          posts every 20-23 columns, the wood / path showing between them;
 *   103-185 the dry-stone wall: light cream stones (some pale blue, ~3-6 %) 7-30 columns by 5-7 rows in an irregular
 *          bond, 1-texel light-grey mortar; 186-~200 the orange path; the front bushes from ~200.
 * How each band is drawn (every colour from the user's picture, every size from the picture's own measure):
 *   - per-row ramps: the fence (planks and the gaps between them), the path, the wall's top and foot rows take the
 *     colour distribution of the same row in the picture's columns next to the edge (a ramp of that row's texels,
 *     objects left out), read through stretched value noise (grain along the planks, streaks on the path), so every
 *     row has the picture's tone and spread there;
 *   - the wall: stones laid outwards column by column (a skyline: where a stone ends, the free rows get new stones of
 *     the picture's sizes, so joints never line up); the first stones are the picture's own stones cut by the edge
 *     (rows and colour measured on its last columns, their remaining length from the visible one); every stone's
 *     colour drawn from the list of the picture's stone colours next to the edges, as often as their area (so the share
 *     of blue-grey ones is the picture's), the layout warped by smooth noise (irregular polygons, slanted joints; none
 *     at the join), the mortar the one-texel boundary of each stone, a darker lower and right rim, a softened corner;
 *     mortar from the picture's mortar ramp; the foot's shade per row as the picture's;
 *   - the trees: the tree cut by each edge continued (its centre, top and width in A5_ETREE, its tier lines measured
 *     on the edge columns so the chevrons meet), then new conifers of varying height and width, near (bright) and
 *     far (hazy) mixed, in front of a hazy back row; the tone per row matched to the edge columns over the first
 *     columns;
 *   - the bushes: mounds of leaf clusters (cellular: each cluster lit on top, dark crevices between them, the odd
 *     hole), the first mound's top starting at the picture's bush top at the edge;
 *   - each band softened to the picture's own sharpness next to the edge (the mean fine detail |L - box3(L)| matched by
 *     a 3x3 blur of the right strength: the distant trees and the path are soft, the wall crisp).
 * Not continued: the stepping stones jutting out of the wall, the barrels, the bench, the notice board, the pipe. */
enum { A5_SKY = 1, A5_TREE, A5_FENCE, A5_STONE, A5_PATH, A5_BUSH, A5_TREEN };
#define A5_WALL0 103
#define A5_WALL1 186
#define A5_STONE0 106                                    /* the stones' rows (the coping and its joint above them, the
                                                            foot row under them) */
#define A5_STONE1 185
/* the fence, measured on the picture's sunlit stretch (x 250-292, between the house and the pipe): two rails of 8 rows
   (71-78, 82-89: a lit top row, the board, a dark lower row), the wood seen between them (79-81) and under the lower
   one (90-91), the path from row 92; posts 5 columns wide (rows 71-93, lit on their right: the picture's at x 263-267
   and ~296-300), one every 33 columns.  What reads as a third plank by each edge is a separate low object in front of
   the fence (two boards, rows 80-87 / 93-100, with dark blocks under them; x 0-26 and 286-319, both cut by the edge):
   the strips end it in their first 2 columns and go on with the fence. */
#define A5_FEN0 71                                       /* the top rail's first row: the wood above it */
#define A5_RAIL1 82                                      /* the lower rail's first row */
#define A5_RAILH 8
#define A5_PATH0 92
#define A5_POSTD 33
#define A5_POSTW 5
typedef struct { short post0, obj0, obj1; } A5Fence;    /* a picture post (its screen-left column), the object's rows */
static const A5Fence A5_FEN[2] = { { -12, 80, 102 }, { 296, 80, 103 } };   /* (left: no post seen near the edge; the
                                                                              first one 12 columns out) */
typedef struct { short kind, cx, ty, wk; } A5ETree;      /* kind 0 far, 1 hazy, 2 near; centre column, top row, w x16 */
static const A5ETree A5_ETREE[2] = { { 2, 12, 3, 9 }, { 1, 309, 1, 9 } };

static Ramp A5_rrow[2][PH];                              /* per-row ramps (fence, wall top / foot, path) */
static Ramp A5_rsky[2], A5_rnear, A5_rhaze, A5_rpost, A5_rmortar, A5_rbush;
static Rgb A5_stc[256]; static int A5_nst, A5_sta[257]; /* the picture's stone colours, their areas (cumulative) */
static int A5_thr;                                       /* stone / mortar threshold (luminance) */
static int A5_railk[2];                                  /* the rails' light per side (x 256) */
static int A5_lk[2];
static Rgb A5_cblue, A5_cwarm; static int A5_pblue, A5_pwarm;   /* the pale blue / warm stones: colour, share (/1000) */                                     /* the stones' length per side (x 256) */
static int A5_foot[PH];                                  /* the wall's light per row (x 256) */
static int A5_tb[2][24], A5_ntb[2];                      /* tier lines (dark rows) of the edge tree at the edge */
static int A5_btop[2];                                   /* the bush top at the edge */
static Rgb A5_skyc;

static int a5x(int side, int i) { return side ? PW - 1 - i : i; }      /* picture column i in from the edge */
static Rgb a5p(int side, int i, int y) { return rgb15(pic[clampi(y, 0, PH - 1)][clampi(a5x(side, i), 0, PW - 1)]); }
static int a5_green(Rgb k) { return k.g >= k.r + 6 && k.g >= k.b + 16; }
static int a5_obj(Rgb k) { int l = lum(k); return k.r > k.g + 30 || k.g > k.r + 8 || l < 110 || k.r > k.b + 60; }  /* stepping
   stones, plants, shadows, barrels (in the wall) */
static int a5_sky(Rgb k) { return lum(k) > 170 && k.b + 24 >= k.g && k.g < k.r + 24 && !(k.g > k.r + 8); }
static int a5_plank(Rgb k) { return lum(k) > 120 && k.r >= k.b + 30 && k.g >= k.b + 20 && k.r < k.g + 40; }
static int a5_orange(Rgb k) { return k.r >= k.g + 30 && k.g >= k.b + 20; }

/* a ramp from any number (>= 1) of samples */
static void ramp_any(Ramp *rp)
{
    int n = nsamp;
    memset(rp, 0, sizeof *rp);
    if (n < 1) return;
    qsort(samp, (size_t)n, sizeof samp[0], samp_cmp);
    for (int i = 0; i < NRAMP; i++) {
        int a = n * i / NRAMP, b = n * (i + 1) / NRAMP; Rgb s = { 0, 0, 0 };
        if (b <= a) b = a + 1;
        if (b > n) { a = n - 1; b = n; }
        for (int j = a; j < b; j++) { Rgb c = rgb15((u16)samp[j]); s.r += c.r; s.g += c.g; s.b += c.b; }
        rp->bin[i].r = s.r / (b - a); rp->bin[i].g = s.g / (b - a); rp->bin[i].b = s.b / (b - a);
    }
    rp->ok = 1; nsamp = 0;
}
/* the ramp of row y: the texels of columns [i0, i1) from the edge that pass pred (widened to +-3 rows if too few) */
typedef int (*A5Pred)(int side, int i, int y);
static void a5_row_ramp(Ramp *rp, int side, int y, int i0, int i1, A5Pred pred)
{
    for (int d = 0; d <= 3; d++) {
        nsamp = 0;
        for (int yy = y - d; yy <= y + d; yy++)
            if (yy >= 0 && yy < PH)
                for (int i = i0; i < i1; i++) if (pred(side, i, yy)) samp_add(pic[yy][a5x(side, i)]);
        if (nsamp >= 10 || d == 3) break;
    }
    if (nsamp) ramp_any(rp); else memset(rp, 0, sizeof *rp);
}
/* the picture's sunlit fence stretch (x 250-260 and 269-291: no post, trunk, pipe or object) */
static int a5_fence_col(int x) { return (x >= 250 && x < 261) || (x >= 269 && x < 292); }
static int pu_rail(u16 c) { return a5_plank(rgb15(c)) || lum(rgb15(c)) >= 150; }
static int pu_gap(u16 c) { Rgb k = rgb15(c); return !a5_plank(k) && !(k.b > k.r + 8) && lum(k) < 150; }
static int pu_post(u16 c) { Rgb k = rgb15(c); return k.r > k.g + 8 && k.r > k.b + 24 && lum(k) < 175; }
static int pu_lit(u16 c) { Rgb k = rgb15(c); return lum(k) >= 150 && !a5_green(k); }
static int pr_path(int side, int i, int y) { Rgb k = a5p(side, i, y); return a5_orange(k) || (y < 190 && !a5_green(k) && lum(k) > 120); }
static int pr_lit(int side, int i, int y) { return pu_lit(pic[y][a5x(side, i)]); }
static int pr_wall(int side, int i, int y) { return !a5_obj(a5p(side, i, y)); }
static int pr_orange(int side, int i, int y) { return a5_orange(a5p(side, i, y)); }
static int pr_sky(int side, int i, int y) { return a5_sky(a5p(side, i, y)); }

/* Otsu's threshold of a luminance histogram */
static int otsu(const int *h)
{
    long long n = 0, s = 0, w0 = 0, s0 = 0, best = -1; int bt = 128;
    for (int l = 0; l < 256; l++) { n += h[l]; s += (long long)h[l] * l; }
    for (int t = 1; t < 256; t++) {
        w0 += h[t - 1]; s0 += (long long)h[t - 1] * (t - 1);
        if (!w0 || w0 == n) continue;
        {   long long m0 = s0 * 16 / w0, m1 = (s - s0) * 16 / (n - w0), d = m0 - m1, v = (w0 * (n - w0) / 64) * (d * d / 16);
            if (v > best) { best = v; bt = t; } }
    }
    return bt;
}

/* the picture's stones next to both edges: 4-connected bright cores in rows 104-180, columns 0-69 from each edge */
#define A5_SW 70
static void a5_stones(void)
{
    static u8 m[2][77][A5_SW]; static short stk[77 * A5_SW * 2]; int hist[256];
    memset(hist, 0, sizeof hist); A5_nst = 0; A5_sta[0] = 0;
    for (int s = 0; s < 2; s++) for (int y = 104; y < 181; y++) for (int i = 0; i < A5_SW; i++) {
        Rgb k = a5p(s, i, y); int ob = 0;
        for (int dy = -1; dy <= 1; dy++) for (int di = -1; di <= 1; di++) if (a5_obj(a5p(s, i + di, y + dy))) ob = 1;
        m[s][y - 104][i] = ob ? 0 : 1;
        if (!ob) hist[lum(k)]++;
    }
    A5_thr = otsu(hist);
    nsamp = 0;
    for (int s = 0; s < 2; s++) for (int y = 104; y < 181; y++) for (int i = 0; i < A5_SW; i++)
        if (m[s][y - 104][i] && lum(a5p(s, i, y)) < A5_thr - 20) samp_add(pic[y][a5x(s, i)]);
    ramp_any(&A5_rmortar);
    for (int s = 0; s < 2; s++) {
        for (int y = 0; y < 77; y++) for (int i = 0; i < A5_SW; i++) m[s][y][i] = m[s][y][i] && lum(a5p(s, i, y + 104)) >= A5_thr ? 1 : 0;
        for (int y0 = 0; y0 < 77; y0++) for (int i0 = 0; i0 < A5_SW; i0++) {
            int sp = 0, n = 0, border = 0; Rgb t = { 0, 0, 0 };
            if (m[s][y0][i0] != 1) continue;
            m[s][y0][i0] = 2; stk[sp++] = (short)y0; stk[sp++] = (short)i0;
            while (sp) {
                int i = stk[--sp], y = stk[--sp]; Rgb k = a5p(s, i, y + 104);
                t.r += k.r; t.g += k.g; t.b += k.b; n++;
                if (i == 0 || i == A5_SW - 1 || y == 0 || y == 76) border = 1;
                {   static const int D[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
                    for (int d = 0; d < 4; d++) {
                        int yy = y + D[d][0], ii = i + D[d][1];
                        if (yy >= 0 && yy < 77 && ii >= 0 && ii < A5_SW && m[s][yy][ii] == 1) { m[s][yy][ii] = 2; stk[sp++] = (short)yy; stk[sp++] = (short)ii; }
                    } }
            }
            if (!border && n >= 8 && n <= 160 && A5_nst < 256) { Rgb c = { t.r / n, t.g / n, t.b / n }; A5_stc[A5_nst] = c; A5_sta[A5_nst + 1] = A5_sta[A5_nst] + n; A5_nst++; }
        }
    }
}

static void a5_analyse(void)
{
    for (int s = 0; s < 2; s++) {
        for (int y = 0; y < 28; y++) ;
        nsamp = 0;
        for (int y = 0; y < 24; y++) for (int i = 0; i < 64; i++) for (int s2 = 0; s2 < 2; s2++) if (pr_sky(s2, i, y)) samp_add(pic[y][a5x(s2, i)]);   /* (both edges: the clouds too) */
        ramp_any(&A5_rsky[s]);
        /* the fence's rows: the rails and the gaps from the sunlit stretch (both sides; the left's rails then in the
           shade, below), the path per side from its orange texels next to the edge */
        for (int y = A5_FEN0; y < A5_PATH0; y++) {
            int rail = (y < A5_FEN0 + A5_RAILH) || (y >= A5_RAIL1 && y < A5_RAIL1 + A5_RAILH);
            nsamp = 0;
            for (int x = 250; x < 292; x++) if (a5_fence_col(x) && (rail ? pu_rail(pic[y][x]) : pu_gap(pic[y][x]))) samp_add(pic[y][x]);
            if (nsamp < 6) { nsamp = 0; for (int x = 250; x < 292; x++) if (a5_fence_col(x)) samp_add(pic[y][x]); }
            ramp_any(&A5_rrow[s][y]);
        }
        for (int y = A5_PATH0; y < A5_WALL0; y++) a5_row_ramp(&A5_rrow[s][y], s, y, 0, 70, pr_orange);
        for (int y = A5_PATH0; y < A5_WALL0; y++) if (!A5_rrow[s][y].ok) A5_rrow[s][y] = A5_rrow[s][y - 1];
        /* the wall's coping (rows 103-104, lit: the right edge lies in the object's shadow) and the joint under it */
        for (int y = A5_WALL0; y < A5_STONE0; y++) a5_row_ramp(&A5_rrow[s][y], s, y, 0, 70, y < A5_STONE0 - 1 ? pr_lit : pr_wall);
        for (int y = A5_STONE1; y < PH; y++) a5_row_ramp(&A5_rrow[s][y], s, y, 0, 40, pr_path);
        for (int y = A5_STONE1; y < PH; y++) if (!A5_rrow[s][y].ok) A5_rrow[s][y] = A5_rrow[s][y - 1];   /* rows hidden by bushes */
    }
    nsamp = 0;                                               /* the posts: the picture's at x 262-268 */
    for (int y = 72; y < 92; y++) for (int x = 262; x < 269; x++) if (pu_post(pic[y][x])) samp_add(pic[y][x]);
    ramp_any(&A5_rpost);
    {   int a = 0, na = 0, b = 0, nb = 0;                   /* the left's rails: as light as its top rail at the edge */
        for (int y = 72; y < 78; y++) {
            for (int x = 0; x < 16; x++) { a += lum(rgb15(pic[y][x])); na++; }
            for (int x = 250; x < 292; x++) if (a5_fence_col(x)) { b += lum(rgb15(pic[y][x])); nb++; }
        }
        A5_railk[0] = na && b ? clampi((int)((long long)a * nb * 256 / ((long long)b * na)), 160, 256) : 256; A5_railk[1] = 256;
    }
    {   int vj[2], n[2];                                     /* the stones' length per side: the density of vertical
                                                                joints in the 40 columns next to each edge */
        for (int s = 0; s < 2; s++) {
            vj[s] = n[s] = 0;
            for (int y = A5_STONE0; y < A5_STONE1; y++) for (int i = 1; i < 40; i++) {
                int l = lum(a5p(s, i, y));
                if (a5_obj(a5p(s, i - 1, y)) || a5_obj(a5p(s, i, y)) || a5_obj(a5p(s, i + 1, y))) continue;
                n[s]++; vj[s] += l <= lum(a5p(s, i - 1, y)) - 10 && l <= lum(a5p(s, i + 1, y)) - 10;
            }
        }
        for (int s = 0; s < 2; s++)
            A5_lk[s] = vj[s] && n[0] && n[1] ? clampi((int)((long long)(vj[0] + vj[1]) * n[s] * 256 / ((long long)vj[s] * (n[0] + n[1]))), 232, 288) : 256;
#ifdef WS_BACKDROP_MAIN
        if (getenv("WSB_INFO")) fprintf(stderr, "stone length k %d %d (vertical joints %d / %d, %d / %d), rails k %d\n", A5_lk[0], A5_lk[1], vj[0], n[0], vj[1], n[1], A5_railk[0]);
#endif
    }
    nsamp = 0;
    for (int y = 8; y < 66; y++) { for (int x = 250; x < 292; x++) if (a5_green(rgb15(pic[y][x]))) samp_add(pic[y][x]);
                                   for (int x = 0; x < 34; x++) if (a5_green(rgb15(pic[y][x]))) samp_add(pic[y][x]); }
    ramp_any(&A5_rnear);
    nsamp = 0;
    for (int y = 8; y < 66; y++) for (int x = 300; x < 320; x++) { Rgb k = rgb15(pic[y][x]); if (k.g > k.r && !(k.b > k.r + 8 && y > 48)) samp_add(pic[y][x]); }
    for (int y = 10; y < 44; y++) for (int x = 44; x < 80; x++) { Rgb k = rgb15(pic[y][x]); if (k.g > k.r) samp_add(pic[y][x]); }
    ramp_any(&A5_rhaze);
    nsamp = 0;
    for (int y = 204; y < PH; y++) for (int i = 0; i < 48; i++) for (int s = 0; s < 2; s++) if (a5_green(a5p(s, i, y))) samp_add(pic[y][a5x(s, i)]);
    ramp_any(&A5_rbush);
    A5_skyc = ramp_mean(&A5_rsky[1]);
    a5_stones();
    {   /* the pale blue stones and the warm (sandy) ones of the wall next to both edges: their colour and their share of
           the wall (the warm ones halved: the stepping stones' lit tops pass for them too) */
        int nb = 0, nw = 0, n = 0; Rgb cb = { 0, 0, 0 }, cwm = { 0, 0, 0 };
        for (int s = 0; s < 2; s++) for (int y = A5_STONE0; y < A5_STONE1; y++) for (int i = 0; i < 70; i++) {
            Rgb k = a5p(s, i, y); int l = lum(k), near = 0;
            for (int dy = -3; dy <= 3 && !near; dy += 3) for (int di = -3; di <= 3; di += 3) if (lum(a5p(s, i + di, y + dy)) < 110) near = 1;
            if (near || l < 120 || k.g > k.r + 8) continue;
            n++;
            if (k.b > k.r + 6 && l > 150) { nb++; cb.r += k.r; cb.g += k.g; cb.b += k.b; }
            else if (k.r - k.b > 40 && k.r - k.b <= 80 && l > 150 && k.g < k.r) { nw++; cwm.r += k.r; cwm.g += k.g; cwm.b += k.b; }
        }
        A5_pblue = n ? nb * 1000 / n : 0; A5_pwarm = n ? nw * 500 / n : 0;
        if (nb) { A5_cblue.r = cb.r / nb; A5_cblue.g = cb.g / nb; A5_cblue.b = cb.b / nb; } else A5_pblue = 0;
        if (nw) { A5_cwarm.r = cwm.r / nw; A5_cwarm.g = cwm.g / nw; A5_cwarm.b = cwm.b / nw; } else A5_pwarm = 0;
#ifdef WS_BACKDROP_MAIN
        if (getenv("WSB_INFO")) fprintf(stderr, "blue %d/1000 (%d %d %d), warm %d/1000 (%d %d %d)\n", A5_pblue, A5_cblue.r, A5_cblue.g, A5_cblue.b, A5_pwarm, A5_cwarm.r, A5_cwarm.g, A5_cwarm.b);
#endif
    }
    /* the wall's light per row: the mean of its stones and mortar (objects left out) next to both edges over the mean
       of rows 110-170 */
    {   int mean = 0, nm = 0, rowm[PH];
        for (int y = 104; y < A5_WALL1 - 1; y++) {
            int s = 0, n = 0;
            for (int sd = 0; sd < 2; sd++) for (int i = 0; i < 40; i++) { Rgb k = a5p(sd, i, y); if (!a5_obj(k)) { s += lum(k); n++; } }
            rowm[y] = n ? s / n : 0;
            if (y >= 110 && y < 170 && n) { mean += rowm[y]; nm++; }
        }
        mean = nm ? mean / nm : 1;
        for (int y = 0; y < PH; y++) A5_foot[y] = 256;
        for (int y = 172; y < A5_WALL1 - 1; y++) A5_foot[y] = rowm[y] ? clampi(rowm[y] * 256 / mean, 128, 256) : 256;
    }
    /* the edge tree's tier lines: the dark rows of the edge columns (local minima of their luminance) */
    for (int s = 0; s < 2; s++) {
        int P[PH]; A5_ntb[s] = 0;
        for (int y = 0; y < 76; y++) { int t = 0; for (int i = 0; i < 3; i++) t += lum(a5p(s, i, y - 1)) + 2 * lum(a5p(s, i, y)) + lum(a5p(s, i, y + 1)); P[y] = t / 12; }
        for (int y = A5_ETREE[s].ty + 5; y < 70; y++) {
            int loc = (P[y - 3] + P[y + 3] + P[y - 2] + P[y + 2]) / 4;
            if (P[y] <= P[y - 1] && P[y] <= P[y + 1] && P[y] <= P[y - 2] && P[y] <= P[y + 2] && P[y] < loc - 3
                && (!A5_ntb[s] || y - A5_tb[s][A5_ntb[s] - 1] >= 4) && A5_ntb[s] < 24) A5_tb[s][A5_ntb[s]++] = y;
        }
        /* the bush top at the edge */
        A5_btop[s] = PH;
        for (int y = 194; y < PH; y++) { int g = 0; for (int i = 0; i < 3; i++) g += a5_green(a5p(s, i, y)); if (g >= 2) { A5_btop[s] = y; break; } }
    }
}

/* value noise stretched to an even spread of 0..255 (a rank in a ramp) plus a share of per-texel grain */
static int a5_rank(int wx, int y, int sx, int sy, int seed, int grain)
{
    int n = noise(wx, y, sx, sy, seed), h = (int)(hash3(wx, y, seed + 7) & 255);
    int t = 128 + (n - 128) * 7 / 4;
    return clampi((t * (256 - grain) + h * grain) / 256, 0, 255);
}

/* --- the trees: conifers built of clumps of needles along drooping branches, tier by tier from the bottom up (so each
   tier's dark underside lies on the lit top of the one below: the chevrons), over the dark shade inside the crown */
typedef struct { int cx, ty, h, wmax, sp, kind, seed, nb, adx0; const int *B; } A5Tree;
#define A5_DROOP 10                                      /* the branches' droop: rows per column x 16 */
static int a5_tw(const A5Tree *t, int rel)               /* the crown's half-width (columns x 16) rel rows below the top */
{
    if (rel < 0) return 0;
    if (rel >= t->h) return t->wmax * 16;
    return t->wmax * isqrt_i(rel * 256 / t->h) * (rel < 3 ? 2 + rel : 5) / 5;   /* a rounded cone */
}
static Rgb a5_tcol(const A5Tree *t, int v)
{
    if (t->kind == 2) return ramp(&A5_rnear, v);
    v = 128 + (v - 128) * 3 / 5;
    {   Rgb c = ramp(&A5_rhaze, v); return t->kind == 0 ? mix(c, A5_skyc, 40) : c; }
}
static void a5_clump(const A5Tree *t, int side, int x16, int y16, int rx, int ry, int base, int seed, int dir)
{
    int R = (rx > ry ? rx : ry) / 16 + 2, xc = x16 / 16, yc = y16 / 16;
    for (int y = yc - R; y <= yc + R; y++) for (int wx = xc - R; wx <= xc + R; wx++) {
        int X = side ? wx - (PW - 1) : -wx, dx = wx * 16 + 8 - x16, dy = y * 16 + 8 - y16;
        int u = (dx * dir * 217 + dy * 136) / 256, v = (dy * 217 - dx * dir * 136) / 256;   /* along / across the branch */
        long long q;
        if (X < 1 || X > cw || y < 0 || y >= A5_WALL0) continue;
        q = (long long)u * u * 256 / (rx * rx) + (long long)v * v * 256 / (ry * ry);
        if (q > 256 + (int)(hash3(wx, y, seed) & 127) - 80) continue;                      /* a ragged rim */
        {   int c = base - v * (t->kind == 2 ? 28 : 16) / ry + dx * 10 / rx + (int)(hash3(wx, y, seed + 1) & 15) - 8 + (noise(wx, y, 2, 2, seed + 3) - 128) / 4;
            if (q > 150 && v > 0) c -= 34;                                                 /* the underside */
            if ((hash3(wx, y, seed + 5) & 7) == 0) c -= 24;                                /* gaps between the needles */
            if (t->kind == 2 && v < 0 && noise(wx, y, 7, 6, 821) > 160) c += 56;             /* sunlit patches */
            putr(X, y, a5_tcol(t, c), t->kind == 2 ? A5_TREEN : A5_TREE);
        }
    }
}
static void a5_tree(const A5Tree *t, int side)
{
    int xs = t->cx * 16 + 8;
    /* the shade inside the crown */
    for (int y = t->ty + 2; y < A5_WALL0; y++) {
        int W = a5_tw(t, y - t->ty) * 13 / 16;
        for (int wx = (xs - W) / 16 - 2; wx <= (xs + W) / 16 + 2; wx++) {
            int X = side ? wx - (PW - 1) : -wx, dx = wx * 16 + 8 - xs;
            if (X < 1 || X > cw || (dx < 0 ? -dx : dx) > W + (noise(wx, y, 3, 2, 811 + t->seed) - 128) / 4) continue;
            putr(X, y, a5_tcol(t, (noise(wx, y, 5, 4, 823 + t->seed) < 80 ? -20 : 24) + noise(wx, y, 3, 3, 801 + t->seed) / 4 + dx * 16 / (W + 16)), t->kind == 2 ? A5_TREEN : A5_TREE);
        }
    }
    /* the tiers, bottom first: tier rows from the edge's measured tier lines (the edge tree) or every sp rows */
    {   int rows[40], nr = 0;
        if (t->nb) {
            int off = t->adx0 * A5_DROOP / 256 + 2, y;
            for (int k = 0; k < t->nb && nr < 40; k++) rows[nr++] = t->B[k] - off;
            for (y = rows[0] - t->sp; y > t->ty && nr < 40; y -= t->sp) { for (int k = nr; k > 0; k--) rows[k] = rows[k - 1]; rows[0] = y; nr++; }
            for (y = rows[nr - 1] + t->sp; y < A5_WALL0 && nr < 40; y += t->sp) rows[nr++] = y;
        } else
            for (int y = t->ty + 1 + (int)(hash3(t->seed, 0, 803) % 3); y < A5_WALL0 && nr < 40; y += t->sp + (int)(hash3(t->seed, y, 805) % 4) - 1) rows[nr++] = y;
        for (int k = nr - 1; k >= 0; k--) {
            int yk = rows[k], rel = yk - t->ty, W0 = a5_tw(t, rel + t->sp) * (200 + (int)(hash3(t->seed, k, 813) % 90)) / 256, base = 128 + 14 - rel / 6;
            if (rel < 0) continue;
            for (int s = -1; s <= 1; s += 2) {
                Rng r = rng_at(t->seed * 64 + k, s + 1, 807);
                int W = W0 * (s > 0 ? 224 + (int)(hash3(t->seed, k, 815) % 64) : 224 + (int)(hash3(t->seed, k, 817) % 64)) / 256;   /* uneven sides */
                int step = 20 + rndi(&r, 12);
                for (int i = rndi(&r, 16); i <= W + 6; i += step) {
                    int x16 = xs + s * i, y16 = yk * 16 + i * A5_DROOP / 16 + rndi(&r, 48) - 24;
                    int rx = 28 + rndi(&r, 26), ry = 16 + rndi(&r, 10), v = base + rndi(&r, 70) - 35;
                    int wx = x16 / 16;
                    v += (noise(wx, yk, 9, 11, 809 + t->seed) - 128) * 3 / 5;          /* patches of sun and shade */
                    v += s > 0 ? 18 + i * 40 / (W + 16) : -18 - i * 22 / (W + 16);   /* the right flank in the light */
                    if (s > 0 && i > W / 2 && rndi(&r, 2)) v += 34;                  /* its bright tips */
                    if (i > W - 8 && rndi(&r, 2)) ry += 8;                           /* drooping tips */
                    if (rndi(&r, 6)) a5_clump(t, side, x16, y16, rx, ry, v, t->seed * 16 + k, s);   /* (the odd one missing: a deep hole) */
                    step = 16 + rndi(&r, 18);
                }
            }
        }
    }
}
static void a5_wood(int side)
{
    A5Tree tr[96]; int n = 0, dir = side ? 1 : -1, x0 = side ? PW - 8 : -cw - 40, x1 = side ? PW + cw + 40 : 8;
    for (int X = 1; X <= cw; X++)                                            /* the sky */
        for (int y = 0; y < A5_FEN0; y++) {
            int wx = wx_of(side, X);
            putr(X, y, ramp(&A5_rsky[side], 24 + a5_rank(wx, y, 20, 9, 621, 30) * 7 / 8), A5_SKY);
        }
    /* the hazy back row: far trees every ~13 columns */
    for (int k = fdiv(x0, 13) - 3; k * 13 < x1 + 30 && n < 96; k++) {
        u32 h = hash3(k, 0, 631); A5Tree *t = &tr[n];
        t->cx = k * 13 + (int)(h % 7) - 3; t->ty = 2 + (int)(h >> 8) % 14; t->h = 30 + (int)(h >> 16) % 16; t->wmax = 16 + (int)(h >> 20) % 9;
        t->sp = 6; t->kind = 0; t->seed = k * 7 + 3; t->nb = 0; t->B = 0; t->adx0 = 0;
        if ((side && t->cx > PW - 4) || (!side && t->cx < 4)) n++;
    }
    {   int c = A5_ETREE[side].cx, i = 0, ne;
        A5Tree *t = &tr[n]; ne = n++;                                        /* the tree cut by the edge */
        t->cx = c; t->ty = A5_ETREE[side].ty; t->h = 70; t->wmax = A5_ETREE[side].wk * 70 / 16; t->sp = 7;
        t->kind = A5_ETREE[side].kind; t->seed = 900 + side; t->nb = A5_ntb[side]; t->B = A5_tb[side];
        t->adx0 = (side ? PW - 1 : 0) - c; if (t->adx0 < 0) t->adx0 = -t->adx0; t->adx0 *= 16;
        /* new conifers outwards, each its own: near (bright) or hazy, top -5..15, crown 26-60 wide, spaced by their
           widths (overlapping by a third or so), a gap now and then where the back row and the sky show */
        {   int pw = t->wmax;
            while (n < 96) {
                u32 h = hash3(side, i, 641), h2 = hash3(side, i, 643); A5Tree *q = &tr[n];
                q->kind = h % 9 < 5 ? 2 : 1; q->wmax = 20 + (int)(h >> 4) % 18;
                c += dir * ((pw + q->wmax) * 2 / 3 + (int)(h2 % 9) - 4 + ((h2 >> 4) % 4 == 0 ? 10 : 0));
                if ((side && c > x1) || (!side && c < x0)) break;
                q->cx = c; q->ty = (int)(h >> 10) % 17 - 6 + (q->kind == 1 ? 3 : 0);
                q->h = 36 + (int)(h >> 16) % 26; q->sp = 6 + (int)(h >> 22) % 3;
                q->seed = 700 + i * 5 + side; q->nb = 0; q->B = 0; q->adx0 = 0;
                pw = q->wmax; n++; i++;
            }
        }
        /* draw: the back row, the hazy trees, the near ones; the edge tree last within its kind */
        for (int pass = 0; pass < 3; pass++) {
            for (int j = 0; j < n; j++) if (tr[j].kind == pass && j != ne) a5_tree(&tr[j], side);
            if (tr[ne].kind == pass) a5_tree(&tr[ne], side);
        }
    }
}

/* the fence: two rails (per-row ramps of the picture's sunlit rails, grain along them), the wood between and under
   them, the path; posts every 31-35 columns continuing the picture's (A5_FEN), 5 columns wide, lit on their right and
   casting a shadow to their left on the rails and the path; the object by each edge ended in the strip's first columns */
static int a5_post_at(int side, int wx)                  /* the column (0..4, screen left to right) of a post, or -1 */
{
    int p = A5_FEN[side].post0, dir = side ? 1 : -1;
    for (int k = side ? 1 : 0; k < 64; k++) {
        int q = p + dir * A5_POSTD * k + (k ? (int)(hash3(side, k, 651) % 5) - 2 : 0);
        if (wx >= q && wx < q + A5_POSTW) return wx - q;
        if ((side && q > wx) || (!side && q + A5_POSTW < wx)) break;
    }
    return -1;
}
static void a5_fence(int side)
{
    static const int lit[A5_POSTW] = { 18, 90, 205, 225, 120 };   /* across a post, as the picture's (x 263-267) */
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X), pc = a5_post_at(side, wx), sh = a5_post_at(side, wx + 1) == 0 ? 1 : a5_post_at(side, wx + 2) == 0 ? 2 : 0;
        for (int y = A5_FEN0; y < A5_WALL0; y++) {
            Rgb c; int rail = y < A5_FEN0 + A5_RAILH ? 0 : y >= A5_RAIL1 && y < A5_RAIL1 + A5_RAILH ? 1 : -1;
            if (X <= 2 && y >= A5_FEN[side].obj0 && y < A5_FEN[side].obj1) {      /* the object's end */
                c = a5p(side, 0, y); if (X == 2) c = scale(c, 216);
                putr(X, y, c, A5_FENCE); continue;
            }
            if (pc >= 0 && y < A5_PATH0 + 2) {                                    /* a post */
                int t = lit[pc] + (noise(wx * 4, y, 1, 7, 653) - 128) / 4 + (int)(hash3(wx, y, 655) & 15) - 8;
                if (y == A5_FEN0) t += 16;
                if (y >= A5_PATH0) t -= 24;
                c = ramp(&A5_rpost, t);
            } else if (rail >= 0) {                                              /* the rails: grain along them */
                int t = a5_rank(wx, y, 9, 1, 661 + rail, 70) + (noise(wx, y, 23, 4, 667 + rail) - 128) / 4;
                c = scale(ramp(&A5_rrow[side][y], t), A5_railk[side]);
                if (sh) c = scale(c, sh == 1 ? 200 : 228);                        /* the post's shadow */
            } else if (y < A5_PATH0) {                                           /* the wood behind the fence */
                c = ramp(&A5_rrow[side][y], a5_rank(wx, y, 3, 2, 681, 90));
                if (sh) c = scale(c, 220);
            } else {                                                             /* the path */
                c = ramp(&A5_rrow[side][y], a5_rank(wx, y * 3, 4, 4, 691, 60));
                if (y < A5_PATH0 + 3 && (sh || a5_post_at(side, wx + 3) == 0)) c = scale(c, 200);
            }
            putr(X, y, c, A5_FENCE);
        }
    }
}

/* --- the wall: a skyline of stones (see the comment at the top of this generator) */
typedef struct { short y0, hs, jh, x0, x1, slant; Rgb c; } A5Slot;
static A5Slot *a5s; static int a5ns, a5cap;
static int a5_new(int y0, int hs, int jh, int x0, int len, Rgb c, int slant)
{
    if (a5ns >= a5cap) { int nc = a5cap ? a5cap * 2 : 1024; A5Slot *p = realloc(a5s, (size_t)nc * sizeof *p); if (!p) return -1; a5s = p; a5cap = nc; }
    a5s[a5ns].y0 = (short)y0; a5s[a5ns].hs = (short)hs; a5s[a5ns].jh = (short)jh; a5s[a5ns].x0 = (short)x0; a5s[a5ns].x1 = (short)(x0 + len);
    a5s[a5ns].slant = (short)slant; a5s[a5ns].c = c;
    return a5ns++;
}
static Rgb a5_stone_col(u32 h)                          /* a stone colour of the picture's, as often as its area */
{
    int a, i = 0;
    if (!A5_nst) return ramp(&A5_rmortar, 255);
    a = (int)(h % (u32)A5_sta[A5_nst]);
    while (i < A5_nst - 1 && A5_sta[i + 1] <= a) i++;
    return A5_stc[i];
}
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
#define A5_WSX 21
#define A5_BLUE(c) ((c).b > (c).r + 6)
static void a5_wall(int side)
{
    enum { Y0 = A5_STONE0, Y1 = A5_STONE1 };            /* stone rows 106..184 */
    int act[PH]; size_t N = (size_t)cw * PH;
    short *own = calloc(N, sizeof *own), *o2 = calloc(N, sizeof *o2); u8 *jt = calloc(N, 1);
    if (!own || !o2 || !jt) { free(own); free(o2); free(jt); return; }
    a5ns = 0;
    for (int y = 0; y < PH; y++) act[y] = -1;
    for (size_t i = 0; i < N; i++) own[i] = -1;
    /* the picture's stones cut by the edge: runs of rows between the horizontal joints of its last columns */
    {   int y = Y0;
        while (y < Y1) {
            int j0 = 0;
            for (int i = 0; i < 3; i++) j0 += lum(a5p(side, i, y)) < A5_thr - 12;
            if (j0 >= 2) { y++; continue; }
            {   int a = y, b = y, jh = 0, v = 0, n = 0, vj; Rgb t = { 0, 0, 0 };
                while (b < Y1) { int j = 0; for (int i = 0; i < 3; i++) j += lum(a5p(side, i, b)) < A5_thr - 12; if (j >= 2) break; b++; }
                while (b + jh < Y1 && jh < 2) { int j = 0; for (int i = 0; i < 3; i++) j += lum(a5p(side, i, b + jh)) < A5_thr - 12; if (j < 2) break; jh++; }
                if (!jh && b < Y1) jh = 1;
                {   int ym = (a + b - 1) / 2;
                    while (v < 30 && lum(a5p(side, v, ym)) >= A5_thr - 12) v++; }
                vj = 0; for (int yy = a; yy < b; yy++) vj += lum(a5p(side, 0, yy)) < A5_thr - 12;   /* a joint right at the edge */
                for (int yy = a; yy < b; yy++) for (int i = 0; i < 6 && i < v + 1; i++) { Rgb k = a5p(side, i, yy); if (lum(k) >= A5_thr && !a5_obj(k)) { t.r += k.r; t.g += k.g; t.b += k.b; n++; } }
                if (b - a > 5) { b = a + 3 + (int)(hash3(side, a, 703) % 2); jh = 1; }   /* a joint too faint to see: split */
                if (n && vj * 2 < b - a) {
                    u32 h = hash3(side, a, 701); int T = 6 + (int)(h % 5) + (int)(h >> 8) % 6, rem = T - v;
                    Rgb c = { t.r / n, t.g / n, t.b / n };
                    if (rem < 2) rem = 2 + (int)(h >> 16) % 2;
                    {   int s = a5_new(a, b - a, jh, 1, rem, c, 0); if (s >= 0) for (int yy = a; yy < b + jh && yy < Y1; yy++) act[yy] = s; }
                }
                y = b + jh;
            }
        }
    }
    /* outwards: where stones end, new ones fill the free rows: 4-6 rows (incl. the joint), 6-15 columns, now and then a
       longer one (the picture's: median ~10 x 5) */
    for (int X = 1; X <= cw; X++) {
        int y = Y0;
        while (y < Y1) {
            int a, len;
            if (act[y] >= 0 && a5s[act[y]].x1 > X) { y++; continue; }
            a = y; while (y < Y1 && !(act[y] >= 0 && a5s[act[y]].x1 > X)) y++;
            for (int m = 0; m < 2; m++) {                /* no course runs on: a stone over / under the free rows that
                                                            would end within 4 columns ends here, its rows join them */
                int t = m ? (y < Y1 ? act[y] : -1) : (a > Y0 ? act[a - 1] : -1);
                if (t < 0 || a5s[t].x1 - X > 6 || X - a5s[t].x0 < 4 || hash3(wx_of(side, X), m ? y : a, 719 + side) % 4 == 0) continue;
                a5s[t].x1 = (short)X;
                if (m) while (y < Y1 && (act[y] == t || !(act[y] >= 0 && a5s[act[y]].x1 > X))) y++;
                else while (a > Y0 && act[a - 1] == t) a--;
            }
            len = y - a;
            if (len <= 2) {                              /* a sliver: the stone above (or under) grows into it */
                int sa = a > Y0 && act[a - 1] >= 0 && a5s[act[a - 1]].x1 > X ? act[a - 1] : y < Y1 && act[y] >= 0 && a5s[act[y]].x1 > X ? act[y] : -1;
                if (sa >= 0) { for (int q = a; q < y; q++) act[q] = sa; continue; }
            }
            {   int yy = a;
                while (len > 0) {
                    u32 h = hash3(wx_of(side, X), yy, 711 + side);
                    int H = len <= 6 ? len : (int)(h % 20) < 5 ? 4 : (int)(h % 20) < 12 ? 5 : (int)(h % 20) < 17 ? 6 : 7;
                    int L = 5 + (int)(h >> 8) % 5 + (int)(h >> 12) % 6 + ((h >> 24) % 4 == 0 ? 4 + (int)(h >> 27) % 7 : 0);
                    Rgb sc;
                    if (len > 6 && len - H < 3) H = len - 3 >= 3 ? len - 3 : len;
                    if (X > 1 && len > H + 2) for (int tr = 0; tr < 2; tr++) {   /* no joint in line with one just before */
                        int jr = yy + H - 1; size_t o = (size_t)jr * cw + X - 2;
                        if (!(jr + 1 < Y1 && own[o] != own[o + cw])) break;
                        H += (len - H - 1 >= 3 && H < 7) ? 1 : -1;
                        if (H < 3) { H = 3; break; }
                    }
                    L = L * A5_lk[side] / 256;
                    {   int h2 = (int)(hash3(wx_of(side, X), yy, 713) % 1000);   /* the pale blue and the warm stones as
                                                                                     often as the picture's, the others from
                                                                                     its list of stone colours */
                        if (h2 < A5_pblue) { sc = A5_cblue; L = 8 + (int)(h >> 26) % 6; if (H > 5 && len - 5 >= 3) H = 5; }
                        else if (h2 < A5_pblue + A5_pwarm) sc = A5_cwarm;
                        else { sc = a5_stone_col(h >> 4); for (int r = 0; r < 4 && A5_BLUE(sc); r++) sc = a5_stone_col(hash3(wx_of(side, X), yy, 715 + r)); }
                    }
                    {   int s = a5_new(yy, H > 1 ? H - 1 : H, H > 1 ? 1 : 0, X, L, sc, 0);
                        if (s >= 0) for (int q = yy; q < yy + H; q++) act[q] = s; }
                    yy += H; len -= H;
                }
            }
        }
        for (int yy = Y0; yy < Y1; yy++) own[(size_t)yy * cw + X - 1] = (short)act[yy];
    }
    /* irregular slabs: straight joints, but the vertical ones slanted (one column over the stone's height) and the
       horizontal ones stepping by a row along some stones, as the picture's; none at the join */
    memcpy(o2, own, N * sizeof *o2);
    for (int s = 0; s < a5ns; s++) {
        const A5Slot *k = &a5s[s]; int x0 = k->x0, x1 = k->x1, H = k->hs + k->jh, e = x1 - 1, len = x1 - x0;
        u32 h = hash3(wx_of(side, x0), k->y0, 761 + side);
        int vs = (int)(h % 2) ? 1 : -1, D = 1 + (int)(h >> 4) % 2, hd = (int)(h >> 6) % 3, xs = x0 + len * (1 + (int)(h >> 9) % 3) / 4, hside = (int)(h >> 12) & 1;
        if (x0 < 4 || x1 > cw || k->y0 < Y0 || k->y0 + H > Y1 || A5_BLUE(k->c)) continue;   /* (the blue-grey ones stay neat slabs) */
        /* the outer joint slanted: shifted by -D/2..D/2 columns from the top row to the bottom one */
        for (int y = k->y0; y < k->y0 + H && vs && e + 1 <= cw; y++) {
            int sh = vs * (fdiv((2 * (y - k->y0) + 1) * (D + 1), 2 * H) - (D + 1) / 2), t = own[(size_t)y * cw + e];   /* (X e + 1 is column index e) */
            if (!sh || t < 0 || t == s || a5s[t].x0 != e + 1 || A5_BLUE(a5s[t].c)) continue;
            if (sh > 0 && a5s[t].x1 - a5s[t].x0 >= sh + 4) for (int q = 1; q <= sh; q++) o2[(size_t)y * cw + e + q - 1] = (short)s;
            if (sh < 0 && len >= 4 - sh) for (int q = 0; q < -sh; q++) o2[(size_t)y * cw + e - 1 - q] = (short)t;
        }
        if (hd < 2 && H >= 4) {
            int yb = k->y0 + H - 1;
            for (int X = x0; X < x1 - 1; X++) {
                int t; if ((X >= xs) != hside || yb + 1 >= Y1) continue;   /* (one side of xs) */
                t = own[(size_t)(yb + 1) * cw + X - 1];
                if (t < 0 || t == s || A5_BLUE(a5s[t].c)) continue;
                if (hd == 0 && a5s[t].hs + a5s[t].jh >= 5 && a5s[t].y0 == yb + 1) o2[(size_t)(yb + 1) * cw + X - 1] = (short)s;
                if (hd == 1 && H >= 5) o2[(size_t)yb * cw + X - 1] = (short)t;
            }
        }
    }
    /* the courses drift: the layout shifted up / down by a slow vertical offset (up to +-2 rows over ~20 columns, none
       at the join), so the horizontal joints run as the picture's: slanted pixel lines stepping a row every few columns
       instead of ruled ones */
    for (int X = 1; X <= cw; X++) for (int y = Y0; y < Y1; y++) {
        int wx = wx_of(side, X), A = X <= 3 ? 0 : X >= 10 ? 256 : (X - 3) * 36;
        int oy = fdiv((noise(wx, y, A5_WSX, 47, 737) - 128) * 4 * A + 32768, 65536);
        {   int a = o2[(size_t)y * cw + X - 1], b = o2[(size_t)clampi(y + oy, Y0, Y1 - 1) * cw + X - 1];   /* (the blue-grey slabs stay put) */
            own[(size_t)y * cw + X - 1] = (short)((a >= 0 && A5_BLUE(a5s[a].c)) || (b >= 0 && A5_BLUE(a5s[b].c)) ? a : b); }
    }
    memcpy(o2, own, N * sizeof *o2);
    /* the mortar: the one-texel boundary of each stone (its lower row and outer column) */
    for (int X = 1; X <= cw; X++) for (int y = Y0; y < Y1; y++) {
        size_t o = (size_t)y * cw + X - 1; int s0 = o2[o];
        jt[o] = s0 >= 0 && ((y + 1 < Y1 && o2[o + cw] != s0) || (X < cw && o2[o + 1] != s0) || (X == cw && a5s[s0].x1 - 1 == X)) ? 1 : 0;
    }
    /* draw: each stone lit from the top left as the picture's: a near-white top row, the body a little darker downwards
       with soft streaks along it, a grey lower row and right column falling into the mortar, rounded corners */
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X), dxr = side ? 1 : -1;                              /* screen right in X */
        for (int y = A5_WALL0; y < A5_WALL1; y++) {
            Rgb c;
            if (y < Y0 || y >= Y1) {                                               /* the coping, the foot */
                c = ramp(&A5_rrow[side][y], a5_rank(wx, y, 5, 2, 721, 80));
                if (y == Y0 - 1 && (hash3(fdiv(wx, 1), 0, 719) % 9) == 0) c = scale(c, 236);
                if (y < Y0 - 1 && fmodi(wx + (int)(hash3(fdiv(wx, 23), 1, 717) % 23), 23) == 0) c = ramp(&A5_rrow[side][Y0 - 1], 128);   /* the coping's joints */
                putr(X, y, c, A5_STONE); continue;
            }
            {   size_t o = (size_t)y * cw + X - 1; int s = o2[o];
                if (s < 0) continue;
                if (jt[o]) {                                     /* mortar; darker in the shadow under a stone */
                    int vert = y + 1 < Y1 && o2[o + cw] == s && !jt[o + cw];      /* a vertical joint (else under the stone) */
                    c = ramp(&A5_rmortar, (vert ? 10 : 40) + (int)(hash3(s, 1, 747) % 50) + a5_rank(wx, y, 3, 3, 723, 120) / 2);
                    if (!vert && (hash3(s, 1, 743) % 10) < 3) c = mix(c, a5s[s].c, 110);   /* a faint joint */
                } else {
                    Rgb m = ramp(&A5_rmortar, 110);
                    int du = 1, dd = 1, dl = 1, dr = 1, d;
                    while (y - du >= Y0 && o2[o - (size_t)du * cw] == s && !jt[o - (size_t)du * cw]) du++;
                    while (y + dd < Y1 && o2[o + (size_t)dd * cw] == s && !jt[o + (size_t)dd * cw]) dd++;
                    for (;;) { int Xn = X + dr * dxr; size_t on; if (Xn < 1 || Xn > cw) break; on = (size_t)y * cw + Xn - 1; if (o2[on] != s || jt[on]) break; dr++; }
                    for (;;) { int Xn = X - dl * dxr; size_t on; if (Xn < 1 || Xn > cw) break; on = (size_t)y * cw + Xn - 1; if (o2[on] != s || jt[on]) break; dl++; }
                    d = (int)(hash3(s, 0, 741) % 21) - 15;                         /* each stone its own tone */
                    if (a5s[s].hs >= 3 && (hash3(s, 2, 745) % 10) < 3 && y == a5s[s].y0 + 1 + (int)(hash3(s, 3, 745) % (u32)(a5s[s].hs - 2))
                        && X >= a5s[s].x0 + (int)(hash3(s, 4, 745) % 3) && X < a5s[s].x1 - 1 - (int)(hash3(s, 5, 745) % 4)) d -= 16;   /* a layer line */
                    d += (noise(wx, y, 6, 2, 727 + s % 7) - 128) / 7 + (int)(hash3(wx, y, 725) % 9) - 4;
                    /* rounded like a pillow (the picture's profiles across its stones): the middle rows lightest, the
                       first row a little and the last one more below them, both end columns ~15 darker */
                    if (A5_BLUE(a5s[s].c)) d = (du == 1 ? 18 : du == 2 ? 10 : 2 - 6 * (du - 3)) - (dd == 1 ? 12 : 0);   /* the blue-grey slabs: lightest on top */
                    else if (du == 1 && dd == 1) d += 2;
                    else if (du + dd == 3) d += du == 1 ? 6 : 3;
                    else d += du == 1 ? 0 : dd == 1 ? -6 : du == 2 ? 18 : 12;
                    if (du == 2 && dl >= 2 && dr >= 3 && (hash3(s, 6, 749) % 3) == 0) d += 14;      /* a sunlit patch */
                    d -= dl == 1 ? 15 : 0;
                    d += dr == 1 ? -17 : dr == 2 ? 4 : 0;
                    c = a5s[s].c; c.r += d; c.g += d; c.b += d;
                    (void)m;
                }
            }
            c = scale(c, A5_foot[y]);
            putr(X, y, c, A5_STONE);
        }
    }
    free(own); free(o2); free(jt);
}

/* the path (per-row ramps, streaks along it) and the bushes */
static void a5_ground(int side)
{
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X);
        for (int y = A5_WALL1; y < PH; y++) putr(X, y, ramp(&A5_rrow[side][y], a5_rank(wx, y * 3, 4, 4, 731, 60)), A5_PATH);
    }
}
static int a5_bush_top(int side, int wx, int X)
{
    int best = PH + 20, dir = side ? 1 : -1, p = side ? PW - 30 : 30;
    for (int k = 0; k < 64; k++) {                       /* mounds: 22-50 columns apart, half-width 12-27, top 198-211 */
        u32 h = hash3(side, k, 741); int hw = 12 + (int)(h % 16), top = 198 + (int)(h >> 8) % 14, d = wx - p, t;
        t = top + d * d * 13 / (hw * hw);
        if (t < best) best = t;
        p += dir * (22 + (int)(h >> 12) % 29);
        if ((side && p - 40 > wx) || (!side && p + 40 < wx)) break;
    }
    best += (noise(wx, 0, 4, 1, 743) - 128) / 36;                                  /* leaf clusters on the outline */
    if (X <= 14 && A5_btop[side] < PH) best += (A5_btop[side] - best) * (15 - X) / 14;   /* from the picture's top at the edge */
    return best;
}
static void a5_bushes(int side)
{
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X), top = a5_bush_top(side, wx, X);
        for (int y = top; y < PH; y++) {
            int d = y - top, best = 1 << 30, sec = 1 << 30, bh = 0, cy = 0, cx = 0;
            int gx = fdiv(wx, 6), gy = fdiv(y, 5);
            for (int j = gy - 1; j <= gy + 1; j++) for (int i = gx - 1; i <= gx + 1; i++) {   /* leaf clusters on a jittered grid */
                u32 h = hash3(i, j, 751); int px = i * 6 + (int)(h % 6), py = j * 5 + (int)(h >> 8) % 5;
                int dx = (wx - px) * 16, dy = (y - py) * 19, dd = dx * dx + dy * dy;
                if (dd < best) { sec = best; best = dd; bh = (int)(h >> 16); cy = (y - py) * 16; cx = (wx - px) * 16; } else if (dd < sec) sec = dd;
            }
            {   int r1 = isqrt_i(best), r2 = isqrt_i(sec), tone = 150 - d * 3;
                tone += -cy - cx / 3;                                                 /* each cluster lit from the top left */
                tone += (int)(bh & 127) - 64 + (noise(wx, y, 13, 9, 755) - 128) / 2;  /* cluster by cluster, patches of sun */
                if (r2 - r1 < 12) tone -= 40;                                         /* the crevices between them */
                if ((bh >> 6 & 7) == 0 && d > 4) tone = 56 + d / 2;                   /* a hole into the bush */
                if (d < 9 && (bh >> 9 & 1)) tone += 44;                          /* bright leaves on top */
                tone += (int)(hash3(wx, y, 753) & 15) - 8;
                putr(X, y, ramp(&A5_rbush, clampi(tone, 6, 255)), A5_BUSH);
            }
        }
    }
}

/* the picture's trees and bushes next to the edge (for balance_regions: the strip's as light and as contrasted) */
static int a5_picreg(int side, int x, int y)
{
    Rgb k = rgb15(pic[y][x]); (void)side;
    if (y < 70 && (side ? x >= 296 : x < 24)) return a5_green(k) || k.g > k.r ? (A5_ETREE[side].kind == 2 ? A5_TREEN : A5_TREE) : 0;
    if (y >= 206) return a5_green(k) ? A5_BUSH : 0;
    return 0;
}
/* the strip's region reg as light and as contrasted as the picture's texels in box [x0, x1) x [y0, y1) that pass pred */
static void a5_match(int reg, int x0, int x1, int y0, int y1, int (*pred)(Rgb))
{
    static Hist hp, hs; int mp, dp, ms, ds, k;
    memset(&hp, 0, sizeof hp); memset(&hs, 0, sizeof hs);
    for (int y = y0; y < y1; y++) for (int x = x0; x < x1; x++) { Rgb c = rgb15(pic[y][x]); if (pred(c)) hist_add(&hp, lum(c)); }
    for (int y = 0; y < PH; y++) for (int X = 1; X <= cw && X <= 53; X++) if (reg_at(X, y) == reg) hist_add(&hs, lum(cv[(size_t)y * cw + X - 1]));   /* (the 16:9 strip's columns: a wider one starts the same) */
    if (hp.n < 40 || hs.n < 40) return;
    hist_stat(&hp, &mp, &dp); hist_stat(&hs, &ms, &ds);
    k = ds ? clampi(dp * 256 / ds, 160, 512) : 256;
    for (int y = 0; y < PH; y++) for (int X = 1; X <= cw; X++) if (reg_at(X, y) == reg) {
        Rgb *p = &cv[(size_t)y * cw + X - 1]; int l = lum(*p), l2 = clampi((mp + (l * 16 - ms) * k / 256) / 16, 2, 255);
        *p = l ? scale(*p, clampi(l2 * 256 / l, 0, 1024)) : (Rgb){ l2, l2, l2 };
    }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) fprintf(stderr, "match region %d: picture %d dev %d, strip %d dev %d\n", reg, mp / 16, dp / 16, ms / 16, ds / 16);
#endif
}
static int a5_greenish(Rgb k) { return k.g > k.r && k.g > k.b + 8; }
/* the tone per row next to the join matched to the picture's last columns, fading out over n columns */
static void a5_seam(int side, int y0, int y1, int n)
{
    int lp[PH], ls[PH];
    for (int y = y0; y < y1; y++) {
        int a = 0, b = 0;
        for (int i = 0; i < 3; i++) a += lum(a5p(side, i, y));
        for (int X = 1; X <= 3 && X <= cw; X++) b += lum(cv[(size_t)y * cw + X - 1]);
        lp[y] = a; ls[y] = b;
    }
    for (int y = y0; y < y1; y++) {
        int a = 0, b = 0;
        for (int d = -2; d <= 2; d++) { int yy = clampi(y + d, y0, y1 - 1); a += lp[yy]; b += ls[yy]; }
        if (b <= 0) continue;
        {   int k = clampi(a * 256 / b, 160, 400);
            for (int X = 1; X <= n && X <= cw; X++) { Rgb *p = px_at(X, y); *p = scale(*p, 256 + (k - 256) * (n + 1 - X) / (n + 1)); }
        }
    }
}
/* soften rows [y0, y1) to the picture's sharpness next to the edge: the mean |L - box3(L)| of its columns 1-24 against
   the strip's 2-28 after a 3x3 blur of strength w (0: none, 256: one box, 512: two) */
static int a5_fine(const Rgb *b, int w, int h, int x0, int x1)
{
    long long s = 0; int n = 0;
    for (int y = 1; y < h - 1; y++) for (int x = x0; x < x1 && x < w - 1; x++) {
        int t = 0;
        for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) t += lum(b[(size_t)(y + dy) * w + x + dx]);
        t = lum(b[(size_t)y * w + x]) * 9 - t; s += t < 0 ? -t : t; n++;
    }
    return n ? (int)(s * 16 / 9 / n) : 0;
}
static void a5_box(const Rgb *in, Rgb *out, int w, int h)
{
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        int r = 0, g = 0, b = 0, n = 0;
        for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
            int yy = y + dy, xx = x + dx;
            if (yy < 0 || yy >= h || xx < 0 || xx >= w) continue;
            r += in[(size_t)yy * w + xx].r; g += in[(size_t)yy * w + xx].g; b += in[(size_t)yy * w + xx].b; n++;
        }
        out[(size_t)y * w + x].r = r / n; out[(size_t)y * w + x].g = g / n; out[(size_t)y * w + x].b = b / n;
    }
}
/* the same for the strip's texels of region reg only, against the picture's box [px0, px1) x [y0, y1) (columns from the
   left), measured on the strip's texels of that region whose 3x3 is all that region */
static void a5_soften_reg(int side, int y0, int y1, int reg, int px0, int px1, int py0, int py1, int wmin)
{
    int h = y1 - y0, ep, best = 0, bd = 1 << 30, pw = px1 - px0, ph = py1 - py0;
    Rgb *p = malloc((size_t)pw * ph * sizeof *p), *b1 = malloc((size_t)cw * h * sizeof *b1), *b2 = malloc((size_t)cw * h * sizeof *b2);
    if (!p || !b1 || !b2) { free(p); free(b1); free(b2); return; }
    (void)side;
    for (int y = 0; y < ph; y++) for (int i = 0; i < pw; i++) p[(size_t)y * pw + i] = rgb15(pic[py0 + y][px0 + i]);
    ep = a5_fine(p, pw, ph, 1, pw - 1);
    a5_box(&cv[(size_t)y0 * cw], b1, cw, h); a5_box(b1, b2, cw, h);
    for (int w = 0; w <= 512; w += 32) {
        long long sum = 0; int n = 0;
        for (int y = 1; y < h - 1; y++) for (int X = 2; X < cw && X <= 53; X++) {
            int t = 0, ok = 1, c = 0;
            for (int dy = -1; dy <= 1 && ok; dy++) for (int dx = -1; dx <= 1; dx++) {
                size_t i = (size_t)(y + dy) * cw + X - 1 + dx; Rgb q;
                if (rg[(size_t)y0 * cw + i] != reg) { ok = 0; break; }
                q = w <= 256 ? mix(cv[(size_t)y0 * cw + i], b1[i], w) : mix(b1[i], b2[i], w - 256);
                t += lum(q); if (!dx && !dy) c = lum(q);
            }
            if (ok) { t = c * 9 - t; sum += t < 0 ? -t : t; n++; }
        }
        if (n < 30) { free(p); free(b1); free(b2); return; }
        {   int e = (int)(sum * 16 / 9 / n), d = e - ep; d = d < 0 ? -d : d; if (d < bd && w >= wmin) { bd = d; best = w; } }
    }
    for (int y = 0; y < h; y++) for (int X = 1; X <= cw; X++) {
        size_t i = (size_t)y * cw + X - 1;
        if (rg[(size_t)y0 * cw + i] == reg || (reg == A5_TREE && rg[(size_t)y0 * cw + i] == A5_SKY)) cv[(size_t)y0 * cw + i] = best <= 256 ? mix(cv[(size_t)y0 * cw + i], b1[i], best) : mix(b1[i], b2[i], best - 256);
    }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) fprintf(stderr, "side %d region %d: picture fine %d, blur %d (left %d)\n", side, reg, ep, best, bd);
#endif
    free(p); free(b1); free(b2);
}
static void a5_soften(int side, int y0, int y1)
{
    int h = y1 - y0, pw = 26, ep, best = 0, bd = 1 << 30;
    Rgb *p = malloc((size_t)pw * h * sizeof *p), *b1 = malloc((size_t)cw * h * sizeof *b1), *b2 = malloc((size_t)cw * h * sizeof *b2), *t = malloc((size_t)cw * h * sizeof *t);
    if (!p || !b1 || !b2 || !t || cw < 30) { free(p); free(b1); free(b2); free(t); return; }
    for (int y = 0; y < h; y++) for (int i = 0; i < pw; i++) p[(size_t)y * pw + i] = a5p(side, i, y0 + y);
    ep = a5_fine(p, pw, h, 1, 25);
    a5_box(&cv[(size_t)y0 * cw], b1, cw, h); a5_box(b1, b2, cw, h);
    for (int w = 0; w <= 512; w += 32) {
        for (size_t i = 0; i < (size_t)cw * h; i++) t[i] = w <= 256 ? mix(cv[(size_t)y0 * cw + i], b1[i], w) : mix(b1[i], b2[i], w - 256);
        {   int e = a5_fine(t, cw, h, 2, 28), d = e - ep; d = d < 0 ? -d : d;
            if (d < bd) { bd = d; best = w; } }
    }
    for (size_t i = 0; i < (size_t)cw * h; i++) cv[(size_t)y0 * cw + i] = best <= 256 ? mix(cv[(size_t)y0 * cw + i], b1[i], best) : mix(b1[i], b2[i], best - 256);
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) fprintf(stderr, "side %d rows %d-%d: picture fine %d, blur %d (left %d)\n", side, y0, y1, ep, best, bd);
#endif
    free(p); free(b1); free(b2); free(t);
}

static void gen_a5(int pad)
{
    pal_make(); a5_analyse();
    if (!A5_rsky[0].ok || !A5_rsky[1].ok || !A5_rnear.ok || !A5_rhaze.ok || !A5_rpost.ok || !A5_rmortar.ok || !A5_rbush.ok
        || !canvas_open(pad)) return;
    for (int side = 0; side < 2; side++) {
        canvas_clear();
        a5_wood(side); a5_fence(side); a5_wall(side); a5_ground(side); a5_bushes(side);
        a5_match(A5_TREEN, 252, 288, 8, 64, a5_greenish);
        a5_match(A5_TREE, 298, 320, 8, 64, a5_greenish);
        a5_match(A5_BUSH, 0, 48, 208, PH, a5_green);
        balance_regions(side, a5_picreg, 0);
        a5_seam(side, 0, A5_FEN0, 12);
        a5_seam(side, A5_FEN0, A5_FEN0 + A5_RAILH, 10);
        a5_seam(side, 196, PH, 10);
        a5_soften_reg(side, 0, A5_WALL0, A5_TREEN, 252, 288, 8, 64, 128);
        a5_soften_reg(side, 0, A5_WALL0, A5_TREE, 298, 320, 8, 64, 128);   /* (the sky with it: soft tree tops) */
        a5_soften(side, A5_FEN0, A5_WALL0);
        a5_soften(side, A5_WALL0 + 1, A5_WALL1 - 1);
        a5_soften(side, A5_WALL1, 200);
        a5_soften(side, 200, PH);
        emit(side);
    }
    free(a5s); a5s = 0; a5ns = a5cap = 0;
    canvas_close();
}

/* ================================================================== AREA19 s1 / AREA02 s1: the Dwarf Village
 * Two pictures with one layout (the same pieces), a frontal scene: bands by row, measured on the pictures:
 *   0-~100  the canopy: cherry blossom (AREA19: dark pink with light flower clusters) or the balloon trees (AREA02:
 *           big soft spheres, cream / pink / violet / blue, lit from the upper left);
 *   ~95-112 behind the canopy's lower edge: grey rock (AREA19) / the hazy wood (AREA02);
 *   112-138 the fence: posts 4-6 columns wide, tops at rows 112-122, two rails (rows 124-127, 130-134);
 *   138-150 the ledge: grass (AREA19) / golden leaves (AREA02), 150-155 its dark lip;
 *   155-182 the palisade under it: stakes 4 columns wide, in the ledge's shade at the top;
 *   182-~205 the ground (grass / leaves); ~198-223 the front: blossom mounds (AREA19) / big spheres (AREA02), as the
 *           picture's own front quads.
 * The huts, the dwarves, the jukebox, the bench and the barrel are objects of the picture and are not continued. */
enum { V_CANOPY = 1, V_BACK, V_FENCE, V_RAIL, V_LEDGE, V_LIP, V_PALI, V_GROUND, V_FRONT };
#define V_FENCE0 112
#define V_LEDGE0 138
#define V_LIP0 150
#define V_PALI0 155
#define V_GROUND0 182
static Ramp V_rcan, V_rback, V_rfence, V_rledge, V_rpali, V_rground, V_rfront, V_rball[4];
static int V_nball;
static int v_cherry(void)                                /* the canopy mostly pink: the blossom (AREA19), else balloons */
{
    int n = 0;
    for (int y = 0; y < 88; y += 2) for (int x = 0; x < PW; x += 2) { Rgb k = rgb15(pic[y][x]); n += k.r > k.g + 20 && k.b > k.g + 4; }
    return n * 10 > 44 * 160 * 6;
}
static int v_not_cherry(void) { return !v_cherry(); }
static int v_balloons(void) { return !v_cherry(); }
static int v_sat(u16 c) { Rgb k = rgb15(c); int mx = k.r > k.g ? (k.r > k.b ? k.r : k.b) : (k.g > k.b ? k.g : k.b), mn = k.r < k.g ? (k.r < k.b ? k.r : k.b) : (k.g < k.b ? k.g : k.b); return mx - mn; }
static int v_pinkish(u16 c) { Rgb k = rgb15(c); return k.r > k.g + 20 && k.b > k.g + 4; }
static int v_grey(u16 c) { return v_sat(c) < 48 && lum(rgb15(c)) < 150; }
static int v_wood(u16 c) { Rgb k = rgb15(c); return k.r > k.b + 40 && k.g > k.b + 16 && lum(k) > 40; }
static int v_notdark(u16 c) { return lum(rgb15(c)) > 36; }
static int v_cream(u16 c) { Rgb k = rgb15(c); return v_sat(c) < 40 && lum(k) > 110; }
static int v_blue(u16 c) { Rgb k = rgb15(c); return k.b > k.r + 12 && k.b > k.g + 24; }
static int v_violet(u16 c) { Rgb k = rgb15(c); return k.b >= k.r - 8 && k.b <= k.r + 12 && k.r > k.g + 24; }
static int v_pink(u16 c) { Rgb k = rgb15(c); return k.r > k.b + 12 && k.b > k.g + 12; }
static int v_jukebox(int x, int y) { return x >= 258 && x < 317 && y >= 112 && y < 192; }   /* (and its lever) */

static int v_picreg(int side, int x, int y)
{
    u16 c = pic[y][x]; (void)side;
    if (v_jukebox(x, y)) return 0;
    if (y < 88) return V_CANOPY;
    if (y < V_FENCE0) return y >= 92 && v_grey(c) ? V_BACK : 0;
    if (y < V_LEDGE0) return v_wood(c) ? V_FENCE : 0;
    if (y < V_LIP0) return y > 142 ? V_LEDGE : 0;
    if (y < V_PALI0) return 0;
    if (y < V_GROUND0 - 2) return x < 34 || x > 290 ? V_PALI : 0;    /* (the barrel) */
    if (y < 196) return x < 30 || x > 290 ? V_GROUND : 0;
    if (y >= 214) return V_FRONT;
    return 0;
}

static void v_analyse(void)
{
    samp_edges(0, 48, 0, 88, v_balloons() ? 0 : v_pinkish); ramp_make(&V_rcan);
    samp_box(0, 48, 95, V_FENCE0, v_grey); samp_box(PW - 40, PW, 100, V_FENCE0, v_grey); ramp_make(&V_rback);
    if (!V_rback.ok) { samp_edges(0, 48, 95, V_FENCE0, 0); ramp_make(&V_rback); }
    samp_box(0, 48, V_FENCE0, V_LEDGE0, v_wood); samp_box(PW - 3, PW, V_FENCE0, V_LEDGE0, v_wood); ramp_make(&V_rfence);
    samp_box(0, 48, 142, V_LIP0, v_notdark); samp_box(PW - 3, PW, 142, V_LIP0, v_notdark); ramp_make(&V_rledge);
    samp_box(0, 30, V_PALI0, V_GROUND0 - 2, 0); samp_box(PW - 3, PW, V_PALI0, V_GROUND0 - 2, 0); ramp_make(&V_rpali);
    samp_box(0, 28, V_GROUND0 + 2, 196, 0); samp_box(PW - 3, PW, V_GROUND0 + 2, 194, 0); ramp_make(&V_rground);
    samp_edges(0, 48, 214, PH, 0); ramp_make(&V_rfront);
    {   static const Pred cls[4] = { v_cream, v_pink, v_violet, v_blue };   /* the balloons' colours, as the picture has them */
        V_nball = 0;
        for (int i = 0; i < 4; i++) {
            samp_box(0, PW, 0, 88, cls[i]); samp_box(0, 60, 196, PH, cls[i]); samp_box(PW - 60, PW, 196, PH, cls[i]);
            if (nsamp >= 600) ramp_make(&V_rball[V_nball++]); else nsamp = 0;
        }
    }
}

/* a soft sphere (a balloon tree's crown): lit from the upper left and the front, a darker rim, a cottony grain */
static void sphere(int cx16, int cy16, int r16, const Ramp *rp, int base, int r, int seed)
{
    int R = r16 / 16 + 1, X0 = cx16 / 16, Y0 = cy16 / 16;
    for (int py = Y0 - R; py <= Y0 + R; py++)
        for (int X = X0 - R; X <= X0 + R; X++) {
            int dx = X * 16 + 8 - cx16, dy = py * 16 + 8 - cy16, q = r16 * r16 - dx * dx - dy * dy, dz, dot, t;
            if (q <= 0 || X < 1 || X > cw || py < 0 || py >= PH) continue;
            dz = isqrt_i(q);
            if (!cur_side) dx = -dx;                                                 /* X grows leftwards on the left strip */
            dot = (-dx * 120 - dy * 150 + dz * 170) / r16;                           /* n . light, -256..256 */
            t = base + dot * 3 / 4 - 40 + noise(X * 16 + seed, py * 16, 40, 40, seed) / 6 - 21;
            if (dz < r16 / 5) t -= 30;                                               /* the soft rim */
            if (q < r16 * 24) { Rgb *p = px_at(X, py); putr(X, py, mix(*p, ramp(rp, t), 128), r); }   /* its edge blended */
            else putr(X, py, ramp(rp, t), r);
        }
}

static void v_back(int side)
{
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X);
        for (int y = 0; y < V_FENCE0 + 30; y++) {
            int t = 110 + noise(wx, y, 17, 23, 501) / 3 - 42 + (y - 100) / 2;
            putr(X, y, ramp(&V_rback, t), V_BACK);
        }
    }
}

/* the blossom's texture: flower clusters, small fat stars (radius 3) on a jittered 8 x 7 grid, more of them and
   lighter on the lit masses (mass 0..255) */
static int v_flower(int wx, int y, int mass, int seed)
{
    int best = 0;
    for (int gy = fdiv(y, 7) - 1; gy <= fdiv(y, 7) + 1; gy++)
        for (int gx = fdiv(wx, 8) - 1; gx <= fdiv(wx, 8) + 1; gx++) {
            u32 h = hash3(gx, gy, seed); int fx = gx * 8 + (int)(h & 7), fy = gy * 7 + (int)(h >> 3) % 7, dx = wx - fx, dy = y - fy, t;
            dx = dx < 0 ? -dx : dx; dy = dy < 0 ? -dy : dy;
            if ((int)(h >> 8 & 255) > mass + 20 || dx + dy > 3 || (dx == 0 && dy == 3) || (dy == 0 && dx == 3)) continue;
            t = 16 + mass / 5 - (dx + dy) * 3;
            if (dx + dy == 3 && wx >= fx && y >= fy) t = 2;                      /* its rim away from the light */
            if (t > best) best = t;
        }
    return best;
}
static void v_canopy(int side)
{
    int x0 = side ? PW - 30 : -cw - 40, x1 = side ? PW + cw + 40 : 30;
    if (!v_balloons()) {
        /* the blossom, soft as the picture's: light masses and darker hollows (two scales of value noise), small light
           flower clusters over them, brightest on the lit masses; a ragged, darker lower edge (rows 92-106) */
        for (int X = 1; X <= cw; X++) {
            int wx = wx_of(side, X), lo = 96 + noise(wx, 0, 13, 1, 511) * 12 / 256 - 2;
            for (int y = 0; y < lo; y++) {
                int mass = noise(wx, y, 38, 28, 513), t = 20 + mass * 3 / 4 + noise(wx, y, 12, 9, 515) / 4;
                t += v_flower(wx, y, mass, 517);
                if (y > lo - 7) t -= (y - lo + 7) * 7;                                  /* the shaded underside */
                putr(X, y, ramp(&V_rcan, t), V_CANOPY);
            }
        }
    } else {
        /* the balloon trees: spheres of radius 13-25, the far (small, darker) ones first, the picture's own colours */
        for (int pass = 0; pass < 2; pass++)
            for (int k = fdiv(x0, 30) - 1; k * 30 < x1 + 30; k++)
                for (int row = 0; row < 3; row++) {
                    u32 h = hash3(k, row * 2 + pass, 531);
                    int cx = k * 30 + (int)(h % 24) - 12 + (pass ? 15 : 0), cy = row * 30 + (int)(h >> 5) % 16 - 4 + (pass ? 10 : 0);
                    int rad = pass ? 18 + (int)(h >> 10 & 7) : 13 + (int)(h >> 10 & 7), X = side ? cx - (PW - 1) : -cx;
                    if (row == 2 && pass == 0 && (h >> 14 & 1)) continue;
                    sphere(X * 16 + 8, cy * 16 + 8, rad * 16, &V_rball[(h >> 16) % (u32)V_nball], pass ? 150 : 110, V_CANOPY, 533 + k);
                }
    }
}

static void v_fence(int side)
{
    int x0 = side ? PW - 10 : -cw - 10, x1 = side ? PW + cw + 10 : 10;
    for (int k = fdiv(x0, 8) - 1; k * 8 < x1; k++) {                      /* the posts, one every ~8 columns */
        u32 h = hash3(k, 0, 541); int px = k * 8 + (int)(h % 3), w = 5 + (int)(h >> 4) % 3, top = V_FENCE0 + (int)(h >> 8) % 11, base = (int)(h >> 12 & 63) - 32;
        for (int lx = px; lx < px + w; lx++) {
            int X = side ? lx - (PW - 1) : -lx, u = lx - px;
            for (int y = top; y < V_LEDGE0 + 2; y++) {
                int t = 130 + base + noise(lx * 8, y, 9, 7, 543) / 5 - 25;
                if (u == 0) t += 30; else if (u == w - 1) t -= 60;                    /* lit from the left */
                if (y == top) t += 25;
                putr(X, y, ramp(&V_rfence, t), V_FENCE);
            }
        }
    }
    for (int X = 1; X <= cw; X++) {                                          /* the rails */
        int wx = wx_of(side, X);
        for (int y = 124; y < 134; y++) {
            int t;
            if (y >= 128 && y < 130) continue;
            t = 85 + noise(wx, y * 4, 21, 9, 547) / 5 - 25 + (y == 124 || y == 130 ? 25 : y == 127 || y == 133 ? -35 : 0);
            putr(X, y, ramp(&V_rfence, t), V_RAIL);
        }
    }
}

static void v_ground(int side)
{
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X), top = V_LEDGE0 + (noise(wx, 0, 5, 1, 551) >> 7) - 1;
        for (int y = top; y < PH; y++) {
            int t, r; Rgb c;
            if (y < V_LIP0) { t = 140 + noise(wx, y, 3, 2, 553) / 2 - 64 + (int)(hash3(wx, y, 554) & 63) - 32 - (y >= V_LIP0 - 3 ? 60 : 0) + (y == top ? 30 : 0); c = ramp(&V_rledge, t); r = V_LEDGE; }
            else if (y < V_PALI0) { c = scale(ramp(&V_rpali, 10 + noise(wx, y, 4, 2, 555) / 8), 200); r = V_LIP; }
            else if (y < V_GROUND0) {                                         /* stakes 4 columns wide */
                int n = fdiv(wx, 4), u = wx - n * 4; u32 h = hash3(n, 0, 557);
                t = 140 + (int)(h & 127) - 64 + noise(wx * 6, y, 13, 9, 559) / 2 - 64 + (y - V_PALI0) * 3 - 40;
                if (u == 0) t -= 45; else if (u == 1) t += 15;
                if (y >= V_GROUND0 - 2) t -= 50;
                c = ramp(&V_rpali, t); r = V_PALI;
            } else {
                t = 130 + noise(wx, y, 4, 2, 561) / 2 - 64 + noise(wx, y, 23, 7, 563) / 3 - 42 + (int)(hash3(wx, y, 565) & 63) - 32;
                if (y < V_GROUND0 + 2) t -= 40;
                c = ramp(&V_rground, t); r = V_GROUND;
            }
            putr(X, y, c, r);
        }
    }
}

static void v_front(int side)
{
    int x0 = side ? PW - 30 : -cw - 40, x1 = side ? PW + cw + 40 : 30;
    if (!v_balloons()) {
        /* blossom mounds: a wavy top (rows 195-210, rounder bumps every ~20 columns), the blossom texture inside, the
           mound's lower part in its own shade */
        for (int X = 1; X <= cw; X++) {
            int wx = wx_of(side, X), k = fdiv(wx, 20), u = wx - k * 20, top;
            u32 h = hash3(k, 0, 571);
            top = 195 + (int)(h % 8) + (u - 10) * (u - 10) / 24 + noise(wx, 0, 5, 1, 573) / 50;
            for (int y = top; y < PH; y++) {
                int mass = noise(wx, y, 22, 16, 575), t = 70 + mass / 2 + v_flower(wx, y, mass + 40, 577) - (y - top) * 2;
                if (y == top) t += 20;
                putr(X, y, ramp(&V_rfront, t), V_FRONT);
            }
        }
    } else {
        for (int k = fdiv(x0, 34) - 1; k * 34 < x1; k++) {                   /* big spheres, half under the frame */
            u32 h = hash3(k, 0, 581); int cx = k * 34 + (int)(h % 14) - 7, cy = 228 + (int)(h >> 5) % 10, rad = 22 + (int)(h >> 10 & 7);
            int X = side ? cx - (PW - 1) : -cx;
            sphere(X * 16 + 8, cy * 16 + 8, rad * 16, &V_rball[(h >> 16) % (u32)V_nball], 150, V_FRONT, 583 + k);
        }
    }
}

static void gen_village(int pad)
{
    pal_make(); v_analyse();
    if (!V_rcan.ok || !V_rback.ok || !V_rfence.ok || !V_rledge.ok || !V_rpali.ok || !V_rground.ok || !V_rfront.ok
        || (v_balloons() && !V_nball) || !canvas_open(pad)) return;
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side;
        v_back(side); v_canopy(side); v_fence(side); v_ground(side); v_front(side);
        balance_regions(side, v_picreg, 1 << V_FENCE | 1 << V_RAIL | 1 << V_LEDGE | 1 << V_PALI | 1 << V_GROUND);
        emit(side);
    }
    canvas_close();
}
