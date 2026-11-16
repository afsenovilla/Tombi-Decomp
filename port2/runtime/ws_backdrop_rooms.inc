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
 *   71-102 the front fence: two pale khaki rails (rows 71-78 and 82-89) on posts 5 columns wide every ~33 columns, the
 *          wood seen between and under the rails, the path from row 92; by each edge a short low object in front of it
 *          (two boards, dark blocks under them: x 0-26 and 286-319) that with the top rail behind it reads as three
 *          planks: not continued (the strips end it in their first 2 columns);
 *   103-185 the dry-stone wall: a lit coping (103-104) and its joint (105), then slabs of cream / pinkish limestone
 *          (median ~8-9 columns by 4-5 rows incl. the joint, some 15-20 long), ~4 % pale blue-white and ~3 % warm sandy
 *          ones, in irregular courses (no joint runs on for long: they step, slant and drift), 1-texel grey mortar
 *          (the vertical joints the darker); each slab rounded like a pillow (lightest in its middle rows, the ends ~15
 *          darker), often a white highlight on its top left, the odd faint joint or layer line inside; the foot row
 *          185; 186-~200 the orange path; the front bushes from ~200.
 * How each band is drawn (every colour from the user's picture, every size from the picture's own measure):
 *   - per-row ramps: the fence's rails (the sunlit stretch x 250-292; on the left the shaded top rail at the edge),
 *     the wood between them, the path (lit orange texels next to the edge), the coping and the wall's foot take the
 *     colour distribution of the same row in the picture (a ramp of that row's texels, objects left out), read through
 *     stretched value noise (grain along the rails, streaks on the path), so every row has the picture's tone and
 *     spread; posts from the picture post's ramp, lit across as it is (dark left column, light right half), casting
 *     a shadow on the rails and the path to their left;
 *   - the wall: stones laid outwards column by column (a skyline: where a stone ends, the free rows get new stones);
 *     the first stones are the picture's own stones cut by the edge (rows and colour measured on its last columns,
 *     their remaining length from the visible one); so that no course runs on, a stone over or under the free rows
 *     that would end within 6 columns ends with them (their rows join), a new stone's joint is never in line with the
 *     one just before it, and a 1-2 row sliver goes to the stone over it; lengths scaled per side by the density of
 *     the picture's vertical joints next to that edge; then the vertical joints slanted (-1..+1 columns over the
 *     stone), some horizontal ones stepped by a row, and the courses drifting up / down by a slow offset (+-2 rows
 *     over ~20 columns: slanted pixel lines); the mortar the one-texel boundary of each stone; stone colours from the
 *     list of the picture's stone cores (bluish ones left out), the pale blue and the warm stones from their own
 *     measured colour and share (the blue ones neat slabs, lightest on top); the foot's shade per row as the picture's;
 *   - the trees: the tree cut by each edge continued (its centre, top and width in A5_ETREE, its tier lines measured
 *     on the edge columns so the chevrons meet), then new conifers of varying height and width, near (bright) and
 *     far (hazy) mixed, in front of a hazy back row; the tone per row matched to the edge columns over the first
 *     columns;
 *   - the bushes: mounds of leaf clusters (cellular: each cluster lit on top, dark crevices between them, the odd
 *     hole), the first mound's top starting at the picture's bush top at the edge;
 *   - each band softened to the picture's own sharpness next to the edge (the mean fine detail |L - box3(L)| matched by
 *     a 3x3 blur of the right strength: the distant trees and the path are soft, the wall crisp).
 * Not continued: the stepping stones jutting out of the wall, the ferns in it, the barrels, the bench, the notice board,
 * the pipe, the low two-board object by each edge. */
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
static int pr_orange(int side, int i, int y) { Rgb k = a5p(side, i, y); return a5_orange(k) && lum(k) >= 128; }   /* (lit: no shadows of objects) */
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
        /* the fence's rows: the rails and the gaps from the sunlit stretch; the left's rails in the shade, as its top
           rail at the edge (x 0-15: the lower rail's rows from the same rows of it); the path per side from its lit
           orange texels next to the edge */
        for (int y = A5_FEN0; y < A5_PATH0; y++) {
            int rail = (y < A5_FEN0 + A5_RAILH) || (y >= A5_RAIL1 && y < A5_RAIL1 + A5_RAILH), yt = y < A5_RAIL1 ? y : y - (A5_RAIL1 - A5_FEN0);
            nsamp = 0;
            if (rail && !s) for (int x = 0; x < 16; x++) { if (pu_rail(pic[yt][x])) samp_add(pic[yt][x]); }
            else for (int x = 250; x < 292; x++) if (a5_fence_col(x) && (rail ? pu_rail(pic[y][x]) : pu_gap(pic[y][x]))) samp_add(pic[y][x]);
            if (nsamp < 6) { nsamp = 0; for (int x = 250; x < 292; x++) if (a5_fence_col(x)) samp_add(pic[y][x]); }   /* (a picture without the
                                                                                                             fence's tones there) */
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
            A5_lk[s] = vj[s] && n[0] && n[1] ? clampi((int)((long long)(vj[0] + vj[1]) * n[s] * 256 / ((long long)vj[s] * (n[0] + n[1]))), 232, 320) : 256;
#ifdef WS_BACKDROP_MAIN
        if (getenv("WSB_INFO")) fprintf(stderr, "stone length k %d %d (vertical joints %d / %d, %d / %d))\n", A5_lk[0], A5_lk[1], vj[0], n[0], vj[1], n[1]);
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
    int lr = lum(ramp_mean(&A5_rrow[1][A5_FEN0 + 2])), pk = lr ? clampi(lum(ramp_mean(&A5_rrow[side][A5_FEN0 + 2])) * 256 / lr, 160, 256) : 256;   /* (the posts in the
                                                                    shade with the rails: the left's) */
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
                c = scale(ramp(&A5_rpost, t), pk);
            } else if (rail >= 0) {                                              /* the rails: grain along them */
                int t = a5_rank(wx, y, 9, 1, 661 + rail, 70) + (noise(wx, y, 23, 4, 667 + rail) - 128) / 4;
                c = ramp(&A5_rrow[side][y], t);
                if (sh) c = scale(c, sh == 1 ? 200 : 228);                        /* the post's shadow */
            } else if (y < A5_PATH0) {                                           /* the wood behind the fence */
                c = ramp(&A5_rrow[side][y], a5_rank(wx, y, 3, 2, 681, 90));
                if (sh) c = scale(c, 220);
            } else {                                                             /* the path */
                c = ramp(&A5_rrow[side][y], 40 + a5_rank(wx, y * 3, 6, 3, 691, 50) * 11 / 16);   /* (the extremes left out: no specks) */
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
                        else { sc = a5_stone_col(h >> 4); for (int r = 0; r < 6 && sc.b >= sc.r - 2; r++) sc = a5_stone_col(hash3(wx_of(side, X), yy, 715 + r)); }
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
        if (x0 < 4 || k->y0 < Y0 || k->y0 + H > Y1 || A5_BLUE(k->c)) continue;   /* (the blue-grey ones stay neat slabs) */
        /* the outer joint slanted: shifted by -D/2..D/2 columns from the top row to the bottom one */
        for (int y = k->y0; y < k->y0 + H && vs && e + 1 <= cw; y++) {
            int sh = vs * (fdiv((2 * (y - k->y0) + 1) * (D + 1), 2 * H) - (D + 1) / 2), t = own[(size_t)y * cw + e];   /* (X e + 1 is column index e) */
            if (!sh || t < 0 || t == s || a5s[t].x0 != e + 1 || A5_BLUE(a5s[t].c)) continue;
            if (sh > 0 && a5s[t].x1 - a5s[t].x0 >= sh + 4 && e + sh <= cw) for (int q = 1; q <= sh; q++) o2[(size_t)y * cw + e + q - 1] = (short)s;
            if (sh < 0 && len >= 4 - sh) for (int q = 0; q < -sh; q++) o2[(size_t)y * cw + e - 1 - q] = (short)t;
        }
        if (hd < 2 && H >= 4) {
            int yb = k->y0 + H - 1;
            for (int X = x0; X < x1 - 1 && X <= cw; X++) {
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
                    c = ramp(&A5_rmortar, (vert ? 0 : 40) + (int)(hash3(s, 1, 747) % 50) + a5_rank(wx, y, 3, 3, 723, 120) / 2);
                    if (vert) c = scale(c, 244);                                  /* (the vertical joints the darker, as the picture's) */
                    else if (hash3(wx, y, 751) % 10 == 0) c = scale(c, 216);    /* deeper spots */
                    if (!vert && (hash3(s, 1, 743) % 10) < 3) c = mix(c, a5s[s].c, 110);   /* a faint joint */
                } else {
                    Rgb m = ramp(&A5_rmortar, 110);
                    int du = 1, dd = 1, dl = 1, dr = 1, d;
                    while (y - du >= Y0 && o2[o - (size_t)du * cw] == s && !jt[o - (size_t)du * cw]) du++;
                    while (y + dd < Y1 && o2[o + (size_t)dd * cw] == s && !jt[o + (size_t)dd * cw]) dd++;
                    for (;;) { int Xn = X + dr * dxr; size_t on; if (Xn < 1 || Xn > cw) break; on = (size_t)y * cw + Xn - 1; if (o2[on] != s || jt[on]) break; dr++; }
                    for (;;) { int Xn = X - dl * dxr; size_t on; if (Xn < 1 || Xn > cw) break; on = (size_t)y * cw + Xn - 1; if (o2[on] != s || jt[on]) break; dl++; }
                    d = (int)(hash3(s, 0, 741) % 21) - 15;                         /* each stone its own tone */
                    if (a5s[s].hs >= 3 && (hash3(s, 2, 745) % 10) < 2 && y == a5s[s].y0 + 1 + (int)(hash3(s, 3, 745) % (u32)(a5s[s].hs - 2))
                        && X >= a5s[s].x0 + (int)(hash3(s, 4, 745) % 3) && X < a5s[s].x1 - 1 - (int)(hash3(s, 5, 745) % 4)) d -= 11;   /* a layer line */
                    d += (noise(wx, y, 6, 2, 727 + s % 7) - 128) / 7 + (int)(hash3(wx, y, 725) % 9) - 4;
                    /* rounded like a pillow (the picture's profiles across its stones): the middle rows lightest, the
                       first row a little and the last one more below them, both end columns ~15 darker */
                    if (A5_BLUE(a5s[s].c)) d = (du == 1 ? 18 : du == 2 ? 10 : 2 - 6 * (du - 3)) - (dd == 1 ? 12 : 0);   /* the blue-grey slabs: lightest on top */
                    else if (du == 1 && dd == 1) d += 2;
                    else if (du + dd == 3) d += du == 1 ? 6 : 3;
                    else d += du == 1 ? 0 : dd == 1 ? -6 : du == 2 ? 18 : 12;
                    if (du <= 2 && dd >= 2 && dl >= 2 && dr >= 3 && dl <= 2 + (int)(hash3(s, 7, 749) % 8) && (hash3(s, 6, 749) % 2) == 0) d += du == 1 ? 18 : 12;   /* a white highlight on the top left */
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
 * Two pictures with one layout (the same pieces, huts, fence, ledge, bank, ladder and jukebox): AREA02 s1 with the
 * balloon trees (gen_vb below) and AREA19 s1 in cherry blossom (gen_cb, after gen_vb's machinery).  AREA19 s1 runs as
 * area 2 s1, so the table tells them apart by the picture: */
static int v_cherry(void)                                /* the canopy mostly pink: the blossom (AREA19), else balloons */
{
    int n = 0;
    for (int y = 0; y < 88; y += 2) for (int x = 0; x < PW; x += 2) { Rgb k = rgb15(pic[y][x]); n += k.r > k.g + 20 && k.b > k.g + 4; }
    return n * 10 > 44 * 160 * 6;
}
static int v_not_cherry(void) { return !v_cherry(); }

/* ================================================================== AREA02 s1: the Dwarf Village with the balloon trees
 * Third version (2026-11-16), after the user's review of the second: the strips are now made from the user's own
 * picture at run time instead of being drawn by hand-made procedures ("you invented the fence instead of looking at
 * the fences literally next to it"; the palisade "practically a repeating texture"; the spheres cut by the edges not
 * closed and not round; the right trunks "blobs").  Nothing of the picture is stored: the tables below are coordinates
 * (measured circles, row bands, column ranges, boxes to leave out), every texel is read from VRAM when the room is
 * entered.  Two techniques:
 *
 * 1. Image quilting (Efros & Freeman) for everything that is a texture: the trunks and the haze behind the canopy, the
 *    fence, the ledge of golden leaves and its lip, the straw bank, the ground.  Each band (VB_BANDS: its rows, the
 *    picture columns it may copy from, the boxes of objects to leave out) is filled outwards from the join in blocks
 *    of B columns.  A block is a window of the picture's own texels on the SAME rows (so the fence rails, the
 *    ledge's lip and the ground's perspective rows stay aligned; the back and the ground may shift a few rows), chosen
 *    so that its OV columns of overlap match what is already there: for the first block the picture's columns at the
 *    edge (the texture goes on with its phase), then the previous block; and its top rows match the band above.  Among
 *    the windows within 1/8 of the best match one is taken by a position hash (deterministic variety); windows
 *    near the ones just used are penalised (no visible repetition), and where a band has a clear horizontal period
 *    (the fence posts, the straw blades: autocorrelation of the band's rows next to the edge, vb_period) an off-phase
 *    window is penalised too.  The block is pasted along a minimum-error boundary cut (dynamic programming through
 *    the overlap, vertical against the previous block, horizontal against the band above), so no straight seam
 *    shows.  The picture's own columns are never changed: the first block meets the picture only through its match.
 *    Rows of the back band hidden by the canopy in the picture (above row ~88) are taken from the band's clean rows
 *    reflected upwards (the trunks are vertical): only their lowest part shows between the spheres.
 *
 * 2. Geometric completion for the spheres.  The circles of the spheres cut by each edge (canopy and front) were fitted
 *    on the picture (outline points at 8x, then refined to the strongest colour step along the visible arc:
 *    VB_PS).  At run time each sphere's shading is fitted on its visible texels (inside 0.9 r, not under a sphere in
 *    front, of its colour class): luminance = a + b * (normal . light), the light direction itself fitted over all
 *    the spheres (30 candidates, least error).  A completed sphere is drawn whole, in depth order with its neighbours:
 *    every hidden texel takes the residual texture (picture - model) of a visible texel of the same sphere (its
 *    reflection across the picture's edge first, so the cottony grain runs on through the join, then across the
 *    sphere's vertical axis, then rotated) on the model's tone at its own place, as a shift along the colour class's
 *    ramp (lighter pinks paler, as in the picture).  New spheres are drawn the same way from a donor sphere of the
 *    picture of the chosen class (scaled to the new radius, its texture rotated / mirrored by hash, its tone moved by
 *    depth), with a contact shade on what lies behind them.  A new sphere may reach the join only where the edge's
 *    completed spheres are in front of it there, so the join shows exactly the picture's spheres.
 *
 * Bands and objects (rows measured on the picture at 6-12x): the fence 108-137 (left in the shade, x 0-43; right
 * beside the jukebox, x 186-225), made first; then the trunks and haze above it (picture rows 90/96-109; left x 0-43,
 * right x 186-319), matched to the fence band's top rows; the ledge 137/138-155 (its top rows on the left are a log,
 * x 6-52, left out: taken from rows 144-155), the bank 156-180 (left x 0-215 without the barrel; right x 150-215,
 * before the ladder), the ground 181- (rows 181-197 of x 0-99 for both sides: the right edge's ground is under the
 * jukebox, the ladder and the front sphere), the front spheres from ~190.  Not continued: the huts, the dwarves, the
 * jukebox, the ladder, the log, the barrel, the stones. */
enum { VB_SPH = 1, VB_FRONTR = 5, VB_BACK = 9, VB_FENCE, VB_LEDGE, VB_BANK, VB_GROUND };
typedef struct { short x2, y2, r2, cls, dep, side, fl; } VbS;   /* centre / radius in half texels, colour class (cream 0,
                                                                   pink 1, blue 2), depth (more: in front), the edge that
                                                                   cuts it (-1: none) */
#define VBS_DONOR 1
#define VBS_FRONT 2
#define VBS_KEEP 16                                      /* completed: no new sphere in front of it in the strip (its
                                                            outline beyond the join stays its own, round) */
#define VBS_CAP 8                                        /* completed: its model's tone eased below the lightest of
                                                            its visible texels (a sphere fitted on a short dim rim) */
#define VBS_EDGE 32                                      /* completed front sphere: not drawn whole in the strip; the new
                                                            cream spheres behind it reach the join, then only its
                                                            outline zone goes on over them (vb_edge) */
#define VBS_SOFTJ 4                                      /* its contact shade fades out over the strip's first 8
                                                            columns (the picture shows none there) */
static const VbS VB_PS[] = {
    /* the canopy at the left edge */
    {  22,  19, 28, 0, 10, 0, VBS_DONOR },               /* cream (11, 9.5) r 14 */
    { -24,  72, 34, 1,  0, 0, 0 },                       /* the dark lilac one behind the pink, seen in columns 0-3 */
    {  28,  76, 32, 1, 30, 0, VBS_DONOR },               /* pink (14, 38) r 16 */
    {   5, 139, 38, 1, 20, 0, 0 },                       /* pink (2.5, 69.5) r 19, its lower right in a cast shade */
    /* the canopy at the right edge */
    { 592, -14, 24, 0,  5, 1, 0 },                       /* cream (296, -7) r 12 */
    { 674,  -4, 50, 0,  4, 1, VBS_SOFTJ | VBS_CAP },                       /* cream (337, -2) r 25, lit, at the top right: its outline
                                                            x 312 at rows 0-3 leaning to x 318 at row 16 */
    { 638,  36, 45, 0,  2, 1, 0 },                       /* the grey cream (319, 18) r 22.5, in the shade, behind the
                                                            others (its dark lower left at x 297-305, rows 7-14) */
    { 602,  69, 40, 0,  8, 1, VBS_SOFTJ },               /* the light cream lump (301, 34.5) r 20 between the pink
                                                            and the grey one, closing round in the strip's first
                                                            column (its edge to the grey, a 45-degree line from
                                                            (303, 16) to (318, 31), inside the circle: the grey one
                                                            never takes its texels) */
    { 570,  66, 37, 1, 15, -1, VBS_DONOR },              /* pink (285, 33) r 18.5 */
    { 546, 125, 46, 2, 30, -1, VBS_DONOR },              /* blue (273, 62.5) r 23 */
    { 631, 121, 45, 0, 25, 1, VBS_DONOR | VBS_KEEP },               /* cream (315.5, 60.5) r 22.5 */
    /* the front, cut by the frame's bottom */
    {  56, 496, 100, 1, 10, 0, VBS_FRONT | VBS_DONOR | VBS_EDGE },  /* pink (28, 248) r 50: its flank at the edge is dark
                                                            brown (x 0-15, luminance 40-100, darkest at x 0-3), its pink
                                                            only from x ~16 */
    { 590, 472, 94, 2, 10, 1, VBS_FRONT | VBS_DONOR },   /* blue (295, 236) r 47 */
    { 366, 492, 96, 0, 10, -1, VBS_FRONT | VBS_DONOR },  /* cream (183, 246) r 48 */
};
#define VB_NPS ((int)(sizeof VB_PS / sizeof VB_PS[0]))
typedef struct { short x0, y0, x1, y1; } VbBox;          /* [x0, x1) x [y0, y1) */
/* objects never copied: the huts, the log, the bucket, the barrel, the stones and their track, the plank stack, the
   ladder, the jukebox and its lever */
static const VbBox VB_EXCL[] = {
    { 44, 40, 190, 143 }, { 6, 138, 53, 144 }, { 138, 126, 162, 150 }, { 34, 164, 60, 192 }, { 100, 181, 216, 196 },
    { 226, 118, 254, 142 }, { 216, 133, 262, 188 }, { 252, 120, 268, 190 }, { 262, 108, 316, 196 }, { 0, 0, 0, 0 } };
typedef struct { short y0, y1, ry0, ry1, sx0, sx1, B, OV, OT, OB, dy, reg, per, up, mr, st; } VbBand;
/* rows [y0, y1) of the strip from the picture's rows [ry0, ry1) (others reflected into them), columns [sx0, sx1);
   blocks of B columns, OV of overlap, OT / OB rows of overlap with the band above / below, windows shifted up to dy rows, per: look
   for a horizontal period, up: rows above ry0 reflected into the first up rows only (the trunks: straight on up; -1:
   the row ry0 repeated), mr:
   the first mr columns the picture's last columns repeated with their own period (the quilt's first block then cut
   into them), st: VBB_STRICT, a window whose rows (shifted) leave [ry0, ry1) is not taken (no rows reflected);
   VBB_TIGHT, the first block is the best match of the picture's columns at the edge (not one within 1/8 of it);
   VBB_SOFT below */
#define VBB_STRICT 1
#define VBB_TIGHT 2
#define VBB_FEATHER 8                                    /* the cuts blended (below) */
#define VBB_FAR 16                                       /* a window is penalised when it lies less than 96 columns
                                                            from where it goes (a copy of the picture's texels next to
                                                            them), and taken only when nothing else fits when it goes on
                                                            from the block before (a run of the picture copied whole)
                                                            or overlaps one of the last three */
#define VBB_SOFT 4                                       /* objects in the overlap rows (OT / OB) do not rule a window
                                                            out: those texels are neither compared nor pasted */
#define VBB_OUTL 32                                      /* the bushes' outline (the first blossom row under the grass,
                                                            vb_hcol) goes on: a window is also scored on how its
                                                            outline, shifted, meets the one already there over the
                                                            VB_OCTX columns up to its overlap's end */
#define VBB_EDGEW 64                                     /* the overlap's columns next to the new block weigh more
                                                            (a busy texture: the columns at the cut matter most) */
#define VB_OCTX 20
#define VB_OUTW 40000
static const VbBand VB_BANDS[2][5] = {          /* (in the order they are made: the fence, then the back above it) */
    { { 108, 138, 108, 138, 0, 44, 9, 4, 0, 0, 0, VB_FENCE, 1, 0 },
      { 0, 108, 90, 110, 0, 44, 16, 6, 0, 6, 3, VB_BACK, 0, 6 },
      { 138, 156, 144, 156, 0, 216, 16, 5, 4, 0, 0, VB_LEDGE, 0, 0 },
      { 156, 181, 156, 181, 0, 216, 16, 5, 4, 0, 0, VB_BANK, 1, 0 },
      { 181, PH, 181, 198, 0, 100, 16, 5, 4, 0, 1, VB_GROUND, 0, 0 } },
    { { 108, 137, 108, 137, 186, 256, 12, 5, 0, 0, 0, VB_FENCE, 1, 0 },
      { 0, 108, 96, 110, 186, 320, 24, 8, 0, 6, 3, VB_BACK, 0, -2, 12 },
      { 137, 156, 143, 156, 150, 216, 16, 5, 4, 0, 0, VB_LEDGE, 0, 0 },
      { 156, 181, 156, 181, 150, 216, 16, 5, 4, 0, 0, VB_BANK, 1, 0 },
      { 181, PH, 181, 198, 0, 262, 16, 5, 4, 0, 1, VB_GROUND, 0, 0 } } };
static Ramp VB_rc[3], VB_rf[3];
static int VB_share[4];                                  /* the classes' shares of the canopy (cumulative, /256) */
static int VB_L[3];                                      /* the light (x 256) */
static int VB_a[VB_NPS], VB_b[VB_NPS], VB_n[VB_NPS], VB_m[VB_NPS], VB_hi[VB_NPS];   /* each sphere's shading: L = a + b (n . light) / 256;
                                                                       m: its visible texels' mean luminance */
static int vb_ow;                                        /* the working canvas' width (the strip + a margin, so a block
                                                            or sphere at the strip's end is the same at any width) */

static int vb_class(Rgb k)                               /* cream 0, pink / lilac 1, blue 2, else -1 */
{
    int mx = k.r > k.g ? (k.r > k.b ? k.r : k.b) : (k.g > k.b ? k.g : k.b), mn = k.r < k.g ? (k.r < k.b ? k.r : k.b) : (k.g < k.b ? k.g : k.b);
    if (k.b > k.r + 16 && k.b > k.g + 24) return 2;
    if (k.r > k.g + 20 && k.b > k.g + 8) return 1;
    if (mx - mn < 40 && lum(k) > 70 && k.r >= k.b) return 0;
    return -1;
}
static const VbBox *vb_ex = VB_EXCL;                     /* the room's objects (gen_cb sets its own) */
/* VBB_OUTL bands: an outline the strip's front should keep to, per strip column X (gen_cb's left side), or none:
   returns the row and sets *tol (rows off it that cost nothing), -1 for no target */
static int (*vb_otgt)(int side, int X, int *tol);
#define VB_TGTW 200000
static int (*vb_exf)(int x, int y);                     /* objects that are not boxes (gen_st's stair), or none */
static int *vb_srcmap;                                   /* where each canvas texel was copied from (y * PW + x), or
                                                            none: gen_st relights the copies */
static int vb_excl(int x, int y)
{
    if (x < 0 || x >= PW || y < 0 || y >= PH) return 1;
    if (vb_exf && vb_exf(x, y)) return 1;
    for (int i = 0; vb_ex[i].x1; i++)
        if (x >= vb_ex[i].x0 && x < vb_ex[i].x1 && y >= vb_ex[i].y0 && y < vb_ex[i].y1) return 1;
    return 0;
}

/* --- the canvas in picture columns: wx < 0 (left strip) or >= PW (right strip); X = 1.. outwards, up to vb_ow */
static int vb_X(int side, int wx) { return side ? wx - (PW - 1) : -wx; }
static int vb_in(int side, int wx, int y) { int X = vb_X(side, wx); return X >= 1 && X <= vb_ow && y >= 0 && y < PH; }
static Rgb *vb_px(int side, int wx, int y) { return vb_in(side, wx, y) ? &cv[(size_t)y * vb_ow + vb_X(side, wx) - 1] : 0; }
static int vb_reg(int side, int wx, int y) { return vb_in(side, wx, y) ? rg[(size_t)y * vb_ow + vb_X(side, wx) - 1] : 0; }
static void vb_put(int side, int wx, int y, Rgb c, int r)
{
    if (vb_in(side, wx, y)) { size_t i = (size_t)y * vb_ow + vb_X(side, wx) - 1; cv[i] = c; rg[i] = (u8)r; }
}
static int vb_lo(int side) { return side ? PW : -vb_ow; }            /* the canvas' columns [lo, hi) */
static int vb_hi(int side) { return side ? PW + vb_ow : 0; }
/* what is at picture column wx: the picture (when not an object), or the canvas where already filled */
static int vb_get(int side, int wx, int y, Rgb *c)
{
    if (wx >= 0 && wx < PW) { if (y < 0 || y >= PH || vb_excl(wx, y)) return 0; *c = rgb15(pic[y][wx]); return 1; }
    if (!vb_reg(side, wx, y)) return 0;
    *c = *vb_px(side, wx, y); return 1;
}
static int vb_d2(Rgb a, Rgb b) { int r = a.r - b.r, g = a.g - b.g, bb = a.b - b.b; return 3 * r * r + 4 * g * g + 2 * bb * bb; }

/* ================================================================== quilting */
static int vb_srow(const VbBand *b, int y)               /* a row of the band's clean rows: reflected into [ry0, ry1) */
{
    int n = b->ry1 - b->ry0, k;
    if (y >= b->ry0 && y < b->ry1) return y;
    if (b->up < 0) {                                     /* (the trunks straight on up: the first -up rows reflected,
                                                            few, so no haze repeats as bands) */
        if (y >= b->ry1) return b->ry1 - 1;
        k = fmodi(y - b->ry0, -2 * b->up); return b->ry0 + (k < -b->up ? k : -2 * b->up - 1 - k);
    }
    if (b->up && y >= b->ry1) { k = fmodi(b->ry1 - 1 - y, 2 * b->up); return b->ry1 - 1 - (k < b->up ? k : 2 * b->up - 1 - k); }
    if (b->up) n = b->up;
    k = fmodi(y - b->ry0, 2 * n);
    return b->ry0 + (k < n ? k : 2 * n - 1 - k);
}
/* the band's horizontal period next to the edge (autocorrelation of each row's luminance minus its mean over the
   window's columns), 0 if none is clear */
static int vb_period(int side, const VbBand *b)
{
    int x0 = b->sx0, w = b->sx1 - b->sx0 < 40 ? b->sx1 - b->sx0 : 40, best = 0, bc = 0, ok[PH]; long long den = 0;
    static int L[PH][40];
    for (int y = b->ry0; y < b->ry1; y++) {
        int s = 0; ok[y] = 1;
        for (int i = 0; i < w; i++) { if (vb_excl(x0 + i, y)) ok[y] = 0; L[y][i] = lum(rgb15(pic[y][x0 + i])); s += L[y][i]; }
        for (int i = 0; i < w; i++) { L[y][i] -= s / w; if (ok[y]) den += (long long)L[y][i] * L[y][i]; }
    }
    if (den <= 0) return 0;
    for (int lag = 4; lag <= 16; lag++) {
        long long s = 0; int c;
        for (int y = b->ry0; y < b->ry1; y++) if (ok[y]) for (int i = 0; i + lag < w; i++) s += (long long)L[y][i] * L[y][i + lag];
        c = (int)(s * 256 * w / (w - lag) / den);
        if (c > bc) { bc = c; best = lag; }
    }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) fprintf(stderr, "side %d band %d-%d: period %d (corr %d/256)\n", side, b->y0, b->y1, best, bc);
#endif
    (void)side;
    return bc >= 64 ? best : 0;
}
/* the minimum-error path through rows [ya, yb) x columns [0, w) of the error map e (indexed e[y - ya][j]): vertical
   (one column per row, path[y - ya]) when vert, else horizontal (one row per column, path[j]) */
#define VB_MAXH 128
#define VB_MAXW 40
static int vb_E[VB_MAXH][VB_MAXW];
static void vb_cutpath(int e[VB_MAXH][VB_MAXW], int rows, int w, int vert, int *path)
{
    int (*E)[VB_MAXW] = vb_E, n = vert ? rows : w, m = vert ? w : rows;   /* n steps, m positions each */
    for (int i = 0; i < n; i++)
        for (int p = 0; p < m; p++) {
            int c = vert ? e[i][p] : e[p][i], best = 1 << 30;
            if (i == 0) best = 0;
            else for (int d = -1; d <= 1; d++) if (p + d >= 0 && p + d < m && E[i - 1][p + d] < best) best = E[i - 1][p + d];
            E[i][p] = c + best;
        }
    {   int p = 0;
        for (int q = 1; q < m; q++) if (E[n - 1][q] < E[n - 1][p]) p = q;
        for (int i = n - 1; i >= 0; i--) {
            path[i] = p;
            if (i > 0) { int bp = p; for (int d = -1; d <= 1; d++) if (p + d >= 0 && p + d < m && E[i - 1][p + d] < E[i - 1][bp]) bp = p + d; p = bp; }
        }
    }
}
static short vb_hs[PW];                                  /* the picture's outline, per column (VBB_OUTL bands) */
/* the bushes' outline at picture column wx (picture or canvas): the first row from 184 of three texels that are not
   grass (green or dark), PH when the column is grass down to the bottom, -1 when it cannot be told (an object, a texel
   not made yet) */
static int vb_grass(Rgb k) { return (k.g > k.r + 8 && k.g > k.b + 8) || lum(k) < 60; }
static int vb_hcol(int side, int wx)
{
    int run = 0, ex = -9; Rgb c;
    for (int y = 184; y < PH; y++) {
        if (wx >= 0 && wx < PW && vb_excl(wx, y)) { run = 0; ex = y; continue; }   /* (an object in the grass above) */
        if (!vb_get(side, wx, y, &c)) return -1;
        run = vb_grass(c) ? 0 : run + 1;
        if (run == 3) return ex >= y - 4 ? -1 : y - 2;   /* (-1: the top hidden by the object) */
    }
    return ex >= PH - 2 ? -1 : PH;
}
/* a texel of the window's source an object covers, in the grass above the bushes (VBB_OUTL bands): a hole, neither
   compared nor pasted (the grass under it, made by the band before, shows) */
static int vb_hole(const VbBand *b, int x, int y)
{
    return (b->st & VBB_OUTL) && vb_excl(x, y) && x >= 0 && x < PW && vb_hs[x] >= 0 && y < vb_hs[x];
}
static void vb_quilt(int side, const VbBand *b, int seed)
{
    int W = b->B + b->OV, ytop = b->y0 - b->OT, ybot = b->y1 + b->OB, H = ybot - ytop, per = b->per ? vb_period(side, b) : 0;
    int pos = side ? PW : -1, nb = 0, used[3] = { -999, -999, -999 }, nd = 2 * b->dy + 1, ncand = (b->sx1 - b->sx0 - W + 1) * nd;
    static int err[320 * 128], em[VB_MAXH][VB_MAXW], vcut[VB_MAXH], tcut[VB_MAXW], bcut[VB_MAXW];
    static Rgb win[VB_MAXH][VB_MAXW]; static u8 pm[VB_MAXH][VB_MAXW];
    if (H > VB_MAXH || W > VB_MAXW || ncand <= 0 || ncand > 320 * 128) return;
    if (b->st & VBB_OUTL) for (int x = 0; x < PW; x++) vb_hs[x] = (short)vb_hcol(side, x);
    if (b->mr > 0) {                                     /* the trunk cut by the edge goes on: the picture's last k
                                                            columns repeated, k (mr/2+1 .. 2 mr) the shift at which
                                                            its last 3 columns best match 3 earlier ones (the stripes'
                                                            period; the first block is then cut into them) */
        long long be = -1; int kp = b->mr;
        for (int k = b->mr / 2 + 1; k <= 2 * b->mr; k++) {
            long long e = 0;
            for (int y = b->ry0; y < b->ry1; y++)
                for (int i = 0; i < 3; i++) {
                    int xa = side ? PW - 1 - i : i, xb = side ? xa - k : xa + k;
                    if (!vb_excl(xa, y) && !vb_excl(xb, y)) e += vb_d2(rgb15(pic[y][xa]), rgb15(pic[y][xb]));
                }
            if (be < 0 || e < be) { be = e; kp = k; }
        }
        for (int X = 1; X <= b->mr; X++)
            for (int y = b->y0; y < b->y1; y++)
                vb_put(side, side ? PW - 1 + X : -X, y, rgb15(pic[vb_srow(b, y)][side ? PW - kp + (X - 1) % kp : kp - 1 - (X - 1) % kp]), b->reg);
        pos += side ? b->mr : -b->mr;
    }
    while (side ? pos < vb_hi(side) : pos >= vb_lo(side)) {
        int t0 = side ? pos - b->OV : pos - b->B + 1;             /* the window's first picture column (target) */
        int first = nb == 0 && !b->mr, mn = 1 << 30, pick = -1, nc = 0, k, tol;
        int oc0 = side ? t0 + b->OV - VB_OCTX : t0 + b->B, oht[VB_OCTX];   /* the outline context's first column */
        int oref = -1, oslope = 0;                       /* the outline next to the new columns (its 2 nearest) and
                                                            its slope outwards there (x 4, at most 2 rows a column:
                                                            a valley's flat bottom goes on flat, not down its flank) */
        if (b->st & VBB_OUTL) {
            int a = 0, na = 0, c = 0, nc2 = 0;
            for (int j = 0; j < VB_OCTX; j++) oht[j] = vb_hcol(side, oc0 + j);
            for (int k = 0; k < 4; k++) {
                int h = oht[side ? VB_OCTX - 1 - k : k];
                if (h < 0 || (vb_otgt && h >= PH)) continue;   /* (with a target: a block's feathered outer
                                                                  column, grass-tinted, is no outline) */
                if (k < 2) { a += h; na++; } else { c += h; nc2++; }
            }
            if (na) oref = a / na;
            if (na && nc2) { oslope = (a * 4 / na - c * 4 / nc2) / 2; oslope = oslope > 8 ? 8 : oslope < -8 ? -8 : oslope; }
        }
        for (int c = 0; c < ncand; c++) {
            int sx = b->sx0 + c / nd, dy = c % nd - b->dy, ok = 1, n = 0; long long e = 0;
            if ((b->st & VBB_STRICT) && (ytop + dy < b->ry0 || ybot + dy > b->ry1)) ok = 0;
            for (int y = ytop; y < ybot && ok; y++) {
                if ((b->st & VBB_SOFT) && (y < b->y0 || y >= b->y1)) continue;
                for (int j = 0; j < W; j++) if (vb_excl(sx + j, vb_srow(b, y + dy)) && !vb_hole(b, sx + j, vb_srow(b, y + dy))) { ok = 0; break; }
            }
            if (!ok) { err[c] = -1; continue; }
            for (int y = ytop; y < ybot; y++)
                for (int j = 0; j < W; j++) {
                    int ov = side ? j < b->OV : j >= b->B; Rgb o;
                    if (!ov && y >= b->y0 && y < b->y1) continue;
                    if (first && ov && (y < b->ry0 || y >= b->ry1)) continue;   /* (the picture's canopy above the back) */
                    if (!vb_get(side, t0 + j, y, &o)) continue;
                    if ((b->st & (VBB_SOFT | VBB_OUTL)) && vb_excl(sx + j, vb_srow(b, y + dy))) continue;
                    {   int wv = 1;                      /* (VBB_EDGEW: the overlap columns next to the new ones
                                                            weigh 4, the 2 after them 2) */
                        if ((b->st & VBB_EDGEW) && ov && y >= b->y0 && y < b->y1) { int dj = side ? b->OV - 1 - j : j - b->B; wv = dj < 2 ? 4 : dj < 4 ? 2 : 1; }
                        e += (long long)vb_d2(o, rgb15(pic[vb_srow(b, y + dy)][sx + j])) * wv; n += wv;
                    }
                }
            e = n ? e * 64 / n : 0;
            if (b->st & VBB_OUTL) {                      /* (the outline: squared row offsets, the 8 columns next to
                                                            the window counted twice; unknown: as 6 rows off) */
                long long so = 0; int no = 0;
                for (int j = 0; j < VB_OCTX; j++) {
                    int sc = sx + oc0 + j - t0, hs = sc >= 0 && sc < PW ? vb_hs[sc] : -1, d, wj;
                    if (oht[j] < 0 || hs < 0) continue;
                    d = oht[j] - (hs - dy); if (d < 0) d = -d;
                    if (d > 12) d = 12;
                    wj = (side ? j >= VB_OCTX - 8 - b->OV : j < b->OV + 8) ? 2 : 1;
                    so += (long long)d * d * wj; no += wj;
                }
                e += (no ? so / no : 36) * VB_OUTW;
                so = 0; no = 0;
                if (oref > 0) for (int k = 1; k <= 6; k++) {   /* (the next 6 columns: on along the slope there,
                                                                  more and more freely, no notch or step) */
                    int sc = side ? sx + b->OV - 1 + k : sx + b->B - k, hs = sc >= 0 && sc < PW ? vb_hs[sc] : -1, d;
                    if (hs < 0) continue;
                    d = (hs - dy) - (oref + oslope * k / 4); if (d < 0) d = -d;
                    d -= 1 + k / 2;
                    if (d > 12) d = 12;
                    if (d > 0) so += (long long)d * d;
                    no++;
                }
                if (no) e += so / no * VB_OUTW;
                so = 0; no = 0;
                if (vb_otgt) for (int j = 0; j < W; j++) {     /* (the target outline over the window's strip
                                                                  columns: squared rows beyond its tolerance) */
                    int X = vb_X(side, t0 + j), hs = vb_hs[sx + j], tl, tg, d;
                    if (X < 1 || hs < 0 || (tg = vb_otgt(side, X, &tl)) < 0) continue;
                    d = hs - dy - tg; if (d < 0) d = -d;
                    d -= tl; if (d > 12) d = 12;
                    if (d > 0) so += (long long)d * d;
                    no++;
                }
                if (no) e += so / no * VB_TGTW;
            }
            if (b->up < 0) {                              /* (its top row repeated upwards: windows whose columns go
                                                             straight on up, not a trunk's top or a haze's edge) */
                long long v = 0;
                for (int j = 0; j < W; j++) v += vb_d2(rgb15(pic[vb_srow(b, b->ry0 + dy)][sx + j]), rgb15(pic[vb_srow(b, b->ry0 + dy + 8)][sx + j]));
                e += v * 512 / W;                          /* (weighed 8: no haze slab or trunk top) */
            }
            for (int u = 0; u < 3; u++) { int d = sx - used[u]; if (d < 0) d = -d; if (d < b->B) e += e / 2 + 2000; }
            if (b->st & VBB_FAR) {                       /* (no copy of the picture next to itself, no run of it) */
                int d = sx - t0, f = nb ? sx - (used[(nb - 1) % 3] + (side ? b->B : -b->B)) : 99;
                if (d < 0) d = -d;
                if (f < 0) f = -f;
                if (d < 96) e += e / 2 + 2000;
                if (f < b->B) e += 1 << 24;              /* (only when nothing else fits) */
                for (int u = 0; u < 3; u++) { int g = sx - used[u]; if (g < 0) g = -g; if (g < W) { e += 1 << 24; break; } }
            }
            if (per) { int ph = fmodi(t0 - sx, per); if (ph > per / 2) ph = per - ph; e += e * ph / per; }
            err[c] = (int)(e > (1 << 29) ? (1 << 29) : e);
            if (err[c] < mn) mn = err[c];
        }
        if (mn == 1 << 30) {
#ifdef WS_BACKDROP_MAIN
            if (getenv("WSB_INFO")) fprintf(stderr, "side %d band %d-%d: no window for block %d\n", side, b->y0, b->y1, nb);
#endif
            return;
        }
        tol = first && (b->st & VBB_TIGHT) ? 0 : mn / 8;
        for (int c = 0; c < ncand; c++) if (err[c] >= 0 && err[c] <= mn + tol) nc++;
        k = (int)(hash3(side * 64 + b->reg, nb, seed) % (u32)nc);
        for (int c = 0; c < ncand && pick < 0; c++) if (err[c] >= 0 && err[c] <= mn + tol && k-- == 0) pick = c;
        {   int sx = b->sx0 + pick / nd, dy = pick % nd - b->dy, j0 = side ? 0 : b->B;
#ifdef WS_BACKDROP_MAIN
            if (getenv("WSB_PICK") && (b->st & VBB_OUTL)) {
                fprintf(stderr, "side %d band %d block %d t0 %d: sx %d dy %d err %d (min %d, %d cands) oref %d slope %d | ctx", side, b->y0, nb, t0, sx, dy, err[pick], mn, nc, oref, oslope);
                for (int j = 0; j < VB_OCTX; j++) fprintf(stderr, " %d", oht[j]);
                fprintf(stderr, " | src");
                for (int j = 0; j < W; j++) fprintf(stderr, " %d", vb_hs[sx + j] < 0 ? -1 : vb_hs[sx + j] - dy);
                fprintf(stderr, "\n");
            }
#endif
            for (int y = 0; y < H; y++) for (int j = 0; j < W; j++) win[y][j] = rgb15(pic[vb_srow(b, ytop + y + dy)][sx + j]);
            used[nb % 3] = sx;
            for (int y = 0; y < H; y++) for (int j = 0; j < W; j++) { Rgb o; em[y][j] = vb_get(side, t0 + j, ytop + y, &o) ? vb_d2(o, win[y][j]) : 0; }
            /* the vertical cut through the overlap columns (rows y0..y1): the new block on the outer side */
            for (int y = 0; y < H; y++) vcut[y] = side ? -1 : W;
            if (!first) {
                static int ev[VB_MAXH][VB_MAXW]; int path[VB_MAXH];
                for (int y = b->y0; y < b->y1; y++) for (int j = 0; j < b->OV; j++) ev[y - b->y0][j] = em[y - ytop][j0 + j];
                vb_cutpath(ev, b->y1 - b->y0, b->OV, 1, path);
                for (int y = b->y0; y < b->y1; y++) vcut[y - ytop] = j0 + path[y - b->y0];
            }
            /* the horizontal cuts through the overlap rows with the band above (the new block below the cut) and below
               (the new block above it) */
            for (int j = 0; j < W; j++) { tcut[j] = ytop; bcut[j] = ybot; }
            if (b->OT > 0) {
                vb_cutpath(em, b->OT, W, 0, tcut);
                for (int j = 0; j < W; j++) tcut[j] += ytop;
            }
            if (b->OB > 0) {
                vb_cutpath(em + (b->y1 - ytop), b->OB, W, 0, bcut);
                for (int j = 0; j < W; j++) bcut[j] += b->y1 + 1;
            }
            for (int y = ytop; y < ybot; y++)
                for (int j = 0; j < W; j++) {
                    int tx = t0 + j, vc = vcut[y - ytop];
                    pm[y - ytop][j] = 0;
                    if (tx >= 0 && tx < PW) continue;                     /* never the picture */
                    if (y >= b->y0 && y < b->y1 && (side ? j <= vc : j >= vc)) continue;   /* the old block's side of the cut */
                    if (y < tcut[j] || y >= bcut[j]) continue;
                    if ((b->st & (VBB_SOFT | VBB_OUTL)) && vb_excl(sx + j, vb_srow(b, y + dy))) continue;
                    pm[y - ytop][j] = 1;
                    if (!(b->st & VBB_FEATHER)) vb_put(side, tx, y, win[y - ytop][j], b->reg);
                    if (vb_srcmap && vb_in(side, tx, y)) vb_srcmap[(size_t)y * vb_ow + vb_X(side, tx) - 1] = vb_srow(b, y + dy) * PW + sx + j;
                }
            if (b->st & VBB_FEATHER)                             /* the cuts blended over 5 x 5 texels (blurred bands:
                                                                    a cut between two smooth textures shows as an edge) */
                for (int y = ytop; y < ybot; y++)
                    for (int j = 0; j < W; j++) {
                        int tx = t0 + j, a = 0; Rgb o;
                        if (tx >= 0 && tx < PW) continue;
                        if (vb_hole(b, sx + j, vb_srow(b, y + dy))) continue;
                        for (int v = -2; v <= 2; v++) for (int u = -2; u <= 2; u++) {
                            int yy = y - ytop + v, jj = j + u;
                            if (yy >= 0 && yy < H && jj >= 0 && jj < W)   /* (VBB_OUTL: the picture's columns count as
                                                                              the new block's, so its edge columns are
                                                                              its own, not blended into the grass under) */
                                a += pm[yy][jj] || ((b->st & VBB_OUTL) && t0 + jj >= 0 && t0 + jj < PW);
                        }
                        if (!a) continue;
                        if (!vb_get(side, tx, y, &o)) { if (pm[y - ytop][j]) vb_put(side, tx, y, win[y - ytop][j], b->reg); continue; }
                        vb_put(side, tx, y, mix(o, win[y - ytop][j], a * 256 / 25), b->reg);
                        if (vb_srcmap && a * 2 > 25 && vb_in(side, tx, y)) vb_srcmap[(size_t)y * vb_ow + vb_X(side, tx) - 1] = vb_srow(b, y + dy) * PW + sx + j;
                    }
        }
        nb++; pos += side ? b->B : -b->B;
    }
}

/* ================================================================== the spheres */
static const short VB_COS[8] = { 256, 181, 0, -181, -256, -181, 0, 181 }, VB_SIN[8] = { 0, 181, 256, 181, 0, -181, -256, -181 };
/* n . light (-256..256) at texel (x, y) of the circle centre (x2, y2) radius r2 (half texels), or -9999 outside */
static int vb_dot(int x2, int y2, int r2, int x, int y, const int *L)
{
    int dx = (x * 2 + 1 - x2) * 8, dy = (y * 2 + 1 - y2) * 8, R = r2 * 8, q = R * R - dx * dx - dy * dy;
    if (q <= 0) return -9999;
    return (dx * L[0] + dy * L[1] + isqrt_i(q) * L[2]) / R;
}
/* is the picture's texel (x, y) a visible texel of sphere i: inside 0.9 r, under no sphere in front, of its class */
static int vb_vis(int i, int x, int y)
{
    const VbS *s = &VB_PS[i]; int dx = x * 2 + 1 - s->x2, dy = y * 2 + 1 - s->y2;
    if (x < 0 || x >= PW || y < 0 || y >= PH || (dx * dx + dy * dy) * 100 >= s->r2 * s->r2 * 81) return 0;
    for (int j = 0; j < VB_NPS; j++) {
        const VbS *t = &VB_PS[j]; int ex = x * 2 + 1 - t->x2, ey = y * 2 + 1 - t->y2;
        if (j == i || (t->fl & VBS_FRONT) != (s->fl & VBS_FRONT) || t->dep <= s->dep) continue;
        if (ex * ex + ey * ey < (t->r2 + 2) * (t->r2 + 2)) return 0;
    }
    if (vb_excl(x, y)) return 0;                                  /* (the huts, the jukebox) */
    return vb_class(rgb15(pic[y][x])) == s->cls;
}
/* the same by geometry only (any colour): inside 0.95 r, under no sphere in front, not an object */
static int vb_vis_geo(int i, int x, int y)
{
    const VbS *s = &VB_PS[i]; int dx = x * 2 + 1 - s->x2, dy = y * 2 + 1 - s->y2;
    if (vb_excl(x, y) || (dx * dx + dy * dy) * 400 >= s->r2 * s->r2 * 361) return 0;
    for (int j = 0; j < VB_NPS; j++) {
        const VbS *t = &VB_PS[j]; int ex = x * 2 + 1 - t->x2, ey = y * 2 + 1 - t->y2;
        if (j == i || (t->fl & VBS_FRONT) != (s->fl & VBS_FRONT) || t->dep <= s->dep) continue;
        if (ex * ex + ey * ey < (t->r2 + 2) * (t->r2 + 2)) return 0;
    }
    return 1;
}
/* each sphere's a, b by least squares on its visible texels for light L; returns the summed squared error */
static long long vb_fit(const int *L, int apply)
{
    long long tot = 0;
    for (int i = 0; i < VB_NPS; i++) {
        const VbS *s = &VB_PS[i]; long long n = 0, sd = 0, sdd = 0, sl = 0, sdl = 0, sll = 0, den; int a, b, hl[256];
        if (apply) memset(hl, 0, sizeof hl);
        for (int y = (s->y2 - s->r2) / 2 - 1; y <= (s->y2 + s->r2) / 2 + 1; y++)
            for (int x = (s->x2 - s->r2) / 2 - 1; x <= (s->x2 + s->r2) / 2 + 1; x++) {
                int d, l;
                if (!vb_vis(i, x, y)) continue;
                d = vb_dot(s->x2, s->y2, s->r2, x, y, L); l = lum(rgb15(pic[y][x]));
                n++; sd += d; sdd += (long long)d * d; sl += l; sdl += (long long)d * l; sll += (long long)l * l;
                if (apply) hl[clampi(l, 0, 255)]++;
            }
        den = n * sdd - sd * sd;
        if (n < 12) { a = n ? (int)(sl / n) : 120; b = 0; }
        else {
            b = den > 0 ? (int)((n * sdl - sd * sl) * 256 / den) : 0;   /* (x 256) */
            b = clampi(b, 40, 150);
            a = (int)((sl * 256 - (long long)b * sd) / (n * 256));
        }
        /* the error: sum (l - a - b d / 256)^2 */
        tot += sll - 2 * a * sl - 2 * (long long)b * sdl / 256 + n * a * a + 2 * (long long)a * b * sd / 256 + (long long)b * b / 256 * sdd / 256;
        if (apply) {
            int k = 0, l = 255;                          /* (the 90th percentile of its visible texels) */
            VB_a[i] = a; VB_b[i] = b; VB_n[i] = (int)n; VB_m[i] = n ? (int)(sl / n) : a;
            while (l > 0 && (k += hl[l]) * 10 < n) l--;
            VB_hi[i] = n ? l : 255;
        }
    }
    return tot;
}
static void vb_light(void)
{
    static const short LX[6] = { -32, 0, 32, 64, 96, 128 }, LY[5] = { -128, -96, -64, -32, 0 };
    long long be = -1; int L[3], bl[3] = { 64, -80, 230 };
    for (int i = 0; i < 6; i++) for (int j = 0; j < 5; j++) {
        long long e;
        L[0] = LX[i]; L[1] = LY[j]; L[2] = isqrt_i(65536 - LX[i] * LX[i] - LY[j] * LY[j]);
        e = vb_fit(L, 0);
        if (be < 0 || e < be) { be = e; bl[0] = L[0]; bl[1] = L[1]; bl[2] = L[2]; }
    }
    VB_L[0] = bl[0]; VB_L[1] = bl[1]; VB_L[2] = bl[2];
    vb_fit(VB_L, 1);
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) {
        fprintf(stderr, "light %d %d %d\n", VB_L[0], VB_L[1], VB_L[2]);
        for (int i = 0; i < VB_NPS; i++) fprintf(stderr, "sphere %d: a %d b %d (n %d, mean %d, p90 %d)\n", i, VB_a[i], VB_b[i], VB_n[i], VB_m[i], VB_hi[i]);
    }
#endif
}
/* a colour moved along its class's ramp from its luminance by dl (lighter pinks paler, as the picture's) */
static Rgb vb_at_lum(const Ramp *rp, int l)
{
    int i = 0;
    while (i < NRAMP - 1 && lum(rp->bin[i + 1]) <= l) i++;
    if (i >= NRAMP - 1 || l <= lum(rp->bin[0])) return rp->bin[l <= lum(rp->bin[0]) ? 0 : NRAMP - 1];
    {   int a = lum(rp->bin[i]), b = lum(rp->bin[i + 1]); return b > a ? mix(rp->bin[i], rp->bin[i + 1], (l - a) * 256 / (b - a)) : rp->bin[i]; }
}
static Rgb vb_shift(const Ramp *rp, Rgb c, int dl)
{
    int l = lum(c); Rgb a = vb_at_lum(rp, l), b = vb_at_lum(rp, clampi(l + dl, 4, 250));
    Rgb o = { clampi(c.r + b.r - a.r, 0, 255), clampi(c.g + b.g - a.g, 0, 255), clampi(c.b + b.b - a.b, 0, 255) };
    return o;
}
typedef struct { int x2, y2, r2, cls, dep, don, rot, mir, da, self, reg; } VbN;
static u16 *vb_id; static int vb_cur;                    /* which sphere is on top at each strip texel (1 + index) */   /* a sphere to draw in the strip */
/* source texel try t for sphere n (donor D) at offset (sdx, sdy) (1/16 texel): 0 the same place, 1 across the edge (a
   completed sphere: any texel of it there), 2-4 the same direction nearer the centre, 5 mirrored, 6-12 rotated */
static int vb_try(const VbN *n, const VbS *D, int side, int sdx, int sdy, int t, int *qx, int *qy)
{
    int rot = n->rot, mir = n->mir, ux, uy, q16x, q16y, k = 256;
    if (t == 1 && !n->self) return 0;
    if (t >= 2 && t <= 4) k = 256 - 51 * (t - 1);
    if (t == 5) mir ^= 1;
    if (t >= 6) rot = (rot + t - 5) & 7;
    ux = (mir ? -sdx : sdx) * k / 256; uy = sdy * k / 256;
    {   int rx = (ux * VB_COS[rot] - uy * VB_SIN[rot]) / 256, ry = (ux * VB_SIN[rot] + uy * VB_COS[rot]) / 256;
        q16x = D->x2 * 8 + rx * D->r2 / n->r2; q16y = D->y2 * 8 + ry * D->r2 / n->r2; }
    *qx = fdiv(q16x, 16); *qy = fdiv(q16y, 16);
    if (t == 1) { *qx = side ? 2 * PW - 1 - *qx : -1 - *qx; return vb_vis_geo(n->don, *qx, *qy); }
    return vb_vis(n->don, *qx, *qy);
}
/* the colour of source texel (qx, qy) of donor D moved to model tone lt at its new place: along the donor's ramp, or,
   for a sphere of another class (a texture transfer), its luminance on the target class's ramp */
static Rgb vb_col(const VbN *n, const VbS *D, const Ramp *rp, const Ramp *rpt, int qx, int qy, int lt, int front)
{
    int ls = VB_a[n->don] + VB_b[n->don] * vb_dot(D->x2, D->y2, D->r2, qx, qy, VB_L) / 256;
    Rgb c = rgb15(pic[qy][qx]);
    if (front) {                                          /* the front: the model's tone (a completed one never below its
                                                             mean by more than 28: its flank is not the brown of the
                                                             picture's rim), the texture's grain only (+-20) */
        int l = (!n->self || lt > VB_m[n->don] - 28 ? lt : VB_m[n->don] - 28) + clampi(lum(c) - ls, -20, 20);
        return vb_at_lum(rpt, clampi(l, 4, 250));
    }
    if (n->cls != D->cls) return vb_at_lum(rpt, clampi(lum(c) + lt - ls, 4, 250));
    return vb_shift(rp, c, lt - ls);
}
/* sphere n into the strip: each texel the donor's texel at the same place on the sphere (scaled, rotated, mirrored),
   or, where the donor's is hidden, another visible texel of the donor (across the edge, across its axis, rotated);
   its residual on n's model tone at that place */
static void vb_sphere(int side, const VbN *n)
{
    const VbS *D = &VB_PS[n->don]; const Ramp *rp = D->fl & VBS_FRONT ? &VB_rf[D->cls] : &VB_rc[D->cls];
    const Ramp *rpt = D->fl & VBS_FRONT ? &VB_rf[n->cls] : &VB_rc[n->cls];      /* the target class's */
    int R16 = n->r2 * 8, lo = vb_lo(side), hi = vb_hi(side), xa = (n->x2 - n->r2) / 2 - 7, xb = (n->x2 + n->r2) / 2 + 7;
    int ya = (n->y2 - n->r2) / 2 - 7, yb = (n->y2 + n->r2) / 2 + 7;
    /* the contact shade on the spheres behind it: a soft ring outside its outline, deeper at the lower left */
    for (int y = ya; y <= yb; y++)
        for (int wx = xa; wx <= xb; wx++) {
            int dx = (wx * 2 + 1 - n->x2) * 8, dy = (y * 2 + 1 - n->y2) * 8, o = isqrt_i(dx * dx + dy * dy) - R16, rr; Rgb *p;
            if (wx < lo || wx >= hi || y < 0 || y >= PH || o < 0 || o >= 72 || !(p = vb_px(side, wx, y))) continue;
            rr = vb_reg(side, wx, y);
            if (rr < VB_SPH || rr >= VB_BACK || occ[(size_t)y * vb_ow + vb_X(side, wx) - 1]) continue;   /* (once: not deeper
                                                                       where several spheres in front meet) */
            {   int sh = clampi(200 + o * 56 / 72 + (dx - dy) * 12 / (R16 + 1), 200, 256);
                if (n->self && (VB_PS[n->don].fl & VBS_SOFTJ)) sh = 256 - (256 - sh) * clampi((vb_X(side, wx) - 2) * 256 / 6, 0, 256) / 256;
                *p = scale(*p, sh); }
            occ[(size_t)y * vb_ow + vb_X(side, wx) - 1] = 1;
        }
    for (int y = ya; y <= yb; y++)
        for (int wx = xa; wx <= xb; wx++) {
            int dx = (wx * 2 + 1 - n->x2) * 8, dy = (y * 2 + 1 - n->y2) * 8, d = isqrt_i(dx * dx + dy * dy), dt, src = 0, qx = 0, qy = 0, sdx, sdy;
            Rgb c;
            if (wx < lo || wx >= hi || y < 0 || y >= PH || d >= R16 || (wx >= 0 && wx < PW)) continue;
            dt = vb_dot(n->x2, n->y2, n->r2, wx, y, VB_L);
            {   int rin = R16 * 86 / 100; if (d > rin) { sdx = dx * rin / d; sdy = dy * rin / d; } else { sdx = dx; sdy = dy; } }   /* (the rim: texture
                                                                       from just inside it, the tone of its own place) */
            {   int front = (D->fl & VBS_FRONT) != 0, li = n->don, lt;
                if (front && !n->self)                    /* a new front sphere: lit as the picture's front sphere of
                                                             its class (the cream one: lilac grey in the shade, warm
                                                             on the lit right), its grain from the donor */
                    for (int i = 0; i < VB_NPS; i++) if ((VB_PS[i].fl & VBS_FRONT) && VB_PS[i].cls == n->cls && VB_n[i] >= 60) li = i;
                lt = VB_a[li] + n->da + VB_b[li] * dt / 256;
                if (n->self && !front && (VB_PS[n->don].fl & VBS_CAP)) {   /* (eased above its visible texels' 90th
                                                                             percentile - 8: no flat white dome) */
                    int K = VB_hi[n->don] - 8;
                    if (lt > K) lt = K + (lt - K) / 4;
                }
                if (n->self && front) {
                    /* a completed front sphere: the texture reflected across the edge (else its other flank), on the
                       model's light; the first 12 columns blended from the exact continuation of the picture's texel */
                    int X = side ? wx - (PW - 1) : -wx, ok = vb_try(n, D, side, sdx, sdy, 1, &qx, &qy);
                    static const signed char ord[4] = { 5, 2, 3, 4 };
                    for (int i = 0; i < 4 && !ok; i++) ok = vb_try(n, D, side, sdx, sdy, ord[i], &qx, &qy);
                    if (!ok) c = vb_at_lum(rpt, clampi(lt > VB_m[n->don] - 28 ? lt : VB_m[n->don] - 28, 4, 250));
                    else {
                        c = vb_col(n, D, rp, rpt, qx, qy, lt, 1);
                        if (X <= 12) {
                            int ls = VB_a[n->don] + VB_b[n->don] * vb_dot(D->x2, D->y2, D->r2, qx, qy, VB_L) / 256;
                            c = mix(vb_shift(rp, rgb15(pic[qy][qx]), lt - ls), c, X * 256 / 13);
                        }
                    }
                } else {
                    for (int t = 0; t < 13 && !src; t++) src = vb_try(n, D, side, sdx, sdy, t, &qx, &qy);
                    if (src) c = vb_col(n, D, rp, rpt, qx, qy, lt, front);
                    else c = vb_at_lum(rpt, clampi(lt + (int)(hash3(wx, y, 977) & 15) - 8, 4, 250));
                }
            }
            if (d >= R16 - 24) { Rgb *p = vb_px(side, wx, y); if (p && vb_reg(side, wx, y)) c = mix(*p, c, 256 - (d - (R16 - 24)) * 150 / 24); }   /* (a soft outline) */
            vb_put(side, wx, y, c, n->reg);
            if (vb_in(side, wx, y)) { occ[(size_t)y * vb_ow + vb_X(side, wx) - 1] = 0; if (vb_id) vb_id[(size_t)y * vb_ow + vb_X(side, wx) - 1] = (u16)vb_cur; }
        }
}
static int vb_cls(u32 h) { int a = (int)(h & 255), c = 0; while (c < 2 && VB_share[c + 1] <= a) c++; return c; }
static int vb_ncmp(const void *a, const void *b)
{
    const VbN *p = a, *q = b;
    if (p->dep != q->dep) return p->dep - q->dep;       /* (a total order: any qsort gives the same) */
    if (p->x2 != q->x2) return p->x2 - q->x2;
    if (p->y2 != q->y2) return p->y2 - q->y2;
    if (p->r2 != q->r2) return p->r2 - q->r2;
    if (p->don != q->don) return p->don - q->don;
    if (p->cls != q->cls) return p->cls - q->cls;
    if (p->rot != q->rot) return p->rot - q->rot;
    return p->mir != q->mir ? p->mir - q->mir : p->da - q->da;
}
static int vb_donor(int cls, int front, u32 h)            /* a donor of the class (by hash among them) */
{
    int c[VB_NPS], n = 0;
    for (int i = 0; i < VB_NPS; i++)
        if ((VB_PS[i].fl & VBS_DONOR) && VB_PS[i].cls == cls && !(VB_PS[i].fl & VBS_FRONT) == !front && VB_n[i] >= 60) c[n++] = i;
    return n ? c[h % (u32)n] : -1;
}
/* the deepest edge sphere in front at the strip texel (wx, y) next to the join, or -1000 */
static int vb_edge_dep(int side, int front, int wx, int y)
{
    int m = -1000;
    for (int i = 0; i < VB_NPS; i++) {
        const VbS *s = &VB_PS[i]; int dx = wx * 2 + 1 - s->x2, dy = y * 2 + 1 - s->y2;
        if (s->side != side || !(s->fl & VBS_FRONT) != !front) continue;
        if (dx * dx + dy * dy < s->r2 * s->r2 && s->dep > m) m = s->dep;
    }
    return m;
}
/* may new sphere n stand where it is: in the strip's first 3 columns only under an edge sphere in front of it, and
   nowhere in front of a VBS_KEEP edge sphere */
static int vb_fits(int side, const VbN *n, int front)
{
    for (int i = 0; i < VB_NPS; i++) {
        const VbS *s = &VB_PS[i]; int dx = n->x2 - s->x2, dy = n->y2 - s->y2, rr = n->r2 + s->r2;
        if (!(s->fl & VBS_KEEP) || s->side != side || !(s->fl & VBS_FRONT) != !front || n->dep <= s->dep) continue;
        if (dx * dx + dy * dy < rr * rr) return 0;
    }
    for (int k = 0; k < 3; k++) {
        int wx = side ? PW + k : -1 - k;
        for (int y = (n->y2 - n->r2) / 2 - 1; y <= (n->y2 + n->r2) / 2 + 1; y++) {
            int dx = wx * 2 + 1 - n->x2, dy = y * 2 + 1 - n->y2;
            if (y < 0 || y >= PH || dx * dx + dy * dy >= n->r2 * n->r2) continue;
            if (vb_edge_dep(side, front, wx, y) <= n->dep) return 0;
        }
    }
    return 1;
}
static VbN vb_s[700];
static int vb_add(int side, int n, VbN t, int front, int filler)
{
    int dir = side ? 2 : -2;
    if (n >= 700 || t.don < 0) return n;
    if (!filler) {
        int k = 0;
        while (k < 120 && !vb_fits(side, &t, front)) { t.x2 += dir; k++; }
        if (k == 120) return n;
    }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_NONEW") && !filler) return n;
#endif
    if (side ? (t.x2 + t.r2) / 2 < PW : (t.x2 - t.r2) / 2 >= 0) return n;   /* (all inside the picture) */
    vb_s[n] = t; return n + 1;
}
/* draw the n spheres of vb_s in order; then leave out every new one of which less than a fifth shows (a sliver
   between the ones in front reads as a cut sphere) and draw them again */
static void vb_draw_all(int side, int n)
{
    size_t np = (size_t)vb_ow * PH; Rgb *c0 = malloc(np * sizeof *c0); u8 *r0 = malloc(np); int *cnt = calloc((size_t)n + 1, sizeof *cnt);
    vb_id = calloc(np, sizeof *vb_id);
    if (!c0 || !r0 || !cnt || !vb_id) { free(c0); free(r0); free(cnt); free(vb_id); vb_id = 0; for (int i = 0; i < n; i++) vb_sphere(side, &vb_s[i]); return; }
    memcpy(c0, cv, np * sizeof *c0); memcpy(r0, rg, np); memset(occ, 0, np);
    for (int i = 0; i < n; i++) { vb_cur = i + 1; vb_sphere(side, &vb_s[i]); }
    for (size_t k = 0; k < np; k++) if (vb_id[k]) cnt[vb_id[k]]++;
    {   int m = 0;
        for (int i = 0; i < n; i++) {
            int area = 0, X0 = vb_X(side, (vb_s[i].x2 - vb_s[i].r2) / 2), X1 = vb_X(side, (vb_s[i].x2 + vb_s[i].r2) / 2);
            for (int y = (vb_s[i].y2 - vb_s[i].r2) / 2; y <= (vb_s[i].y2 + vb_s[i].r2) / 2; y++)
                for (int wx = (vb_s[i].x2 - vb_s[i].r2) / 2; wx <= (vb_s[i].x2 + vb_s[i].r2) / 2; wx++) {
                    int dx = wx * 2 + 1 - vb_s[i].x2, dy = y * 2 + 1 - vb_s[i].y2;
                    if (vb_in(side, wx, y) && dx * dx + dy * dy < vb_s[i].r2 * vb_s[i].r2) area++;
                }
            (void)X0; (void)X1;
            if (vb_s[i].self || vb_s[i].dep < -40 || vb_s[i].y2 < 40 || cnt[i + 1] * 5 >= area) vb_s[m++] = vb_s[i];   /* (not the far ones
                                                                       nor the top row: nothing behind them) */
        }
        memcpy(cv, c0, np * sizeof *c0); memcpy(rg, r0, np); memset(occ, 0, np);
        for (int i = 0; i < m; i++) { vb_cur = i + 1; vb_sphere(side, &vb_s[i]); }
    }
    free(c0); free(r0); free(cnt); free(vb_id); vb_id = 0;
}
static void vb_canopy(int side)
{
    int n = 0, lo = vb_lo(side), hi = vb_hi(side);
    for (int i = 0; i < VB_NPS; i++) {
        const VbS *s = &VB_PS[i]; VbN t;
        if (s->side != side || (s->fl & VBS_FRONT)) continue;
        t.x2 = s->x2; t.y2 = s->y2; t.r2 = s->r2; t.cls = s->cls; t.dep = s->dep; t.don = i; t.rot = 0; t.mir = 0; t.da = 0; t.self = 1;
        t.reg = VB_SPH + s->cls; vb_s[n++] = t;
    }
    /* far balloons behind everything (no gaps between the spheres): darker */
    for (int k = fdiv(lo, 13) - 2; k * 13 < hi + 30; k++)
        for (int row = 0; row < 6; row++) {
            u32 h = hash3(k, row, 841); VbN t;
            t.x2 = 2 * (k * 13 + (int)(h % 13) + (row & 1) * 6); t.y2 = 2 * (row * 11 - 4 + (int)(h >> 4) % 6);
            t.r2 = 2 * (11 + (int)(h >> 8) % 5); t.cls = vb_cls(h >> 12); t.dep = -60 + row; t.don = vb_donor(t.cls, 0, h >> 20);
            t.rot = (int)(h >> 24) & 7; t.mir = (int)(h >> 27) & 1; t.da = -46 - (int)(h >> 28) % 8; t.self = 0; t.reg = VB_SPH + t.cls;
            n = vb_add(side, n, t, 0, 1);
        }
    /* three layers of new spheres, 26-column cells, the lower ones in front; the odd one lower still (ragged ends) */
    for (int k = fdiv(lo, 26) - 2; k * 26 < hi + 56; k++)
        for (int row = 0; row < 4; row++) {
            u32 h = hash3(k, row, 851), h2 = hash3(k, row, 853); VbN t;
            if (row == 3 && (h2 & 3) != 0) continue;
            t.x2 = 2 * (k * 26 + (int)(h % 26) + row * 9);
            t.y2 = 2 * (row == 0 ? -2 + (int)(h >> 5) % 12 : row == 1 ? 26 + (int)(h >> 5) % 14 : row == 2 ? 52 + (int)(h >> 5) % 14 : 72 + (int)(h >> 5) % 8);
            t.r2 = 2 * (row == 0 ? 16 + (int)(h >> 9) % 8 : row == 1 ? 15 + (int)(h >> 9) % 8 : row == 2 ? 14 + (int)(h >> 9) % 7 : 11 + (int)(h >> 9) % 4);
            t.r2 += (int)(h2 >> 4) & 1;
            t.cls = vb_cls(h >> 16); t.dep = t.y2 / 2 + (int)(h >> 24) % 12 - 30; t.don = vb_donor(t.cls, 0, h2 >> 8);
            t.rot = (int)(h2 >> 12) & 7; t.mir = (int)(h2 >> 15) & 1; t.da = (int)(h2 >> 16) % 17 - 8 - (row == 0 ? 6 : 0); t.self = 0;
            t.reg = VB_SPH + t.cls;
            n = vb_add(side, n, t, 0, 0);
        }
    qsort(vb_s, (size_t)n, sizeof vb_s[0], vb_ncmp);
    vb_draw_all(side, n);
}
static void vb_front(int side)
{
    int n = 0, lo = vb_lo(side), hi = vb_hi(side), fdon = -1;
    for (int i = 0; i < VB_NPS; i++)
        if ((VB_PS[i].fl & VBS_FRONT) && (VB_PS[i].fl & VBS_DONOR) && (fdon < 0 || VB_n[i] > VB_n[fdon])) fdon = i;
    for (int i = 0; i < VB_NPS; i++) {
        const VbS *s = &VB_PS[i]; VbN t;
        if (s->side != side || !(s->fl & VBS_FRONT) || (s->fl & VBS_EDGE)) continue;
        t.x2 = s->x2; t.y2 = s->y2; t.r2 = s->r2; t.cls = s->cls; t.dep = s->dep; t.don = i; t.rot = 0; t.mir = 0; t.da = 0; t.self = 1;
        t.reg = VB_FRONTR + s->cls; vb_s[n++] = t;
    }
    for (int k = fdiv(lo, 46) - 2; k * 46 < hi + 100; k++)
        for (int row = 0; row < 2; row++) {
            u32 h = hash3(k, side * 4 + row, 871), h2 = hash3(k, row, 873); VbN t;
            t.x2 = 2 * (k * 46 + (int)(h % 30) + row * 23); t.y2 = 2 * (row ? 252 + (int)(h >> 5) % 14 : 238 + (int)(h >> 5) % 12);
            t.r2 = 2 * (row ? 40 + (int)(h >> 10) % 12 : 42 + (int)(h >> 10) % 12);
            t.cls = vb_cls(h >> 16); t.dep = row ? 5 + (int)(h >> 24) % 4 : 1 + (int)(h >> 24) % 4;
            t.don = fdon;                                 /* (the front sphere best seen in the picture lends its
                                                                     texture to every class: the pink and cream ones are
                                                                     too hidden, their rims brown) */
            t.rot = 0; t.mir = (int)(h2 >> 11) & 1; t.self = 0;  /* (only the donors' tops are in the picture: not rotated) */
            t.da = (int)(h2 >> 12) % 21 - 10 - (row ? 0 : 8);
            t.reg = VB_FRONTR + t.cls;
            n = vb_add(side, n, t, 1, 0);
        }
    qsort(vb_s, (size_t)n, sizeof vb_s[0], vb_ncmp);
    vb_draw_all(side, n);
}
/* a VBS_EDGE front sphere's silhouette in the strip, over what vb_front drew behind it: only its outline zone, on the
   arc that leaves the picture where its outline meets the join (centre (33, 251) r 50 in picture texels: through
   (-0.5, 213.9), tangent to the picture's outline there, which runs about 1.15 columns left per row from row 206;
   near the bottom it curves 9 columns into the strip).  Inside: the sphere's own pink, not its dark brown flank
   (x 0-15): its tone a + b * (normal . light) fitted on its visible pink texels (the light of the other spheres),
   never below its darkest pink (the 5th percentile of those texels), and towards the outline eased down to that
   darkest pink from the tone of its darkest pink band (the mean of its pink texels darker than the median) over 6
   texels; the colour and grain of a pink texel of the same row (x 16-31, picked by position) moved along the front
   pink ramp to that tone, keeping its offset from the fit.  The outline anti-aliased over 1.5 texels; outside it a
   contact shade on the sphere behind (the cream one), 0.72 at the outline fading out over 5 texels, as the
   picture's overlaps darken the rear sphere along the front one's outline. */
#define VB_EDGE_X2 66
#define VB_EDGE_Y2 502
#define VB_EDGE_R2 100
static void vb_edge(int side)
{
    int R16 = VB_EDGE_R2 * 8, lo = vb_lo(side);
    if (side) return;
    for (int i = 0; i < VB_NPS; i++) {
        long long n = 0, sd = 0, sdd = 0, sl = 0, sdl = 0, den; int hl[256], fa, fb, l5 = 0, lmed = 0, lband, k;
        if (VB_PS[i].side != side || !(VB_PS[i].fl & VBS_EDGE)) continue;
        memset(hl, 0, sizeof hl);
        for (int y = 0; y < PH; y++) for (int x = 16; x < PW; x++) {        /* the fit on its pink texels */
            int d, l;
            if (!vb_vis(i, x, y) || (d = vb_dot(VB_EDGE_X2, VB_EDGE_Y2, VB_EDGE_R2, x, y, VB_L)) == -9999) continue;
            l = lum(rgb15(pic[y][x]));
            n++; sd += d; sdd += (long long)d * d; sl += l; sdl += (long long)d * l; hl[clampi(l, 0, 255)]++;
        }
        if (n < 40) continue;
        den = n * sdd - sd * sd;
        fb = den > 0 ? clampi((int)((n * sdl - sd * sl) * 256 / den), 0, 200) : 0;
        fa = (int)((sl * 256 - (long long)fb * sd) / (n * 256));
        for (k = 0, l5 = 0; l5 < 255 && (k += hl[l5]) * 20 < n; l5++) continue;
        for (k = 0, lmed = 0; lmed < 255 && (k += hl[lmed]) * 2 < n; lmed++) continue;
        {   long long s0 = 0, c0 = 0; for (int l = 0; l < lmed; l++) { s0 += (long long)hl[l] * l; c0 += hl[l]; } lband = c0 ? (int)(s0 / c0) : l5; }
#ifdef WS_BACKDROP_MAIN
        if (getenv("WSB_INFO")) fprintf(stderr, "edge sphere %d: n %lld a %d b %d l5 %d band %d\n", i, n, fa, fb, l5, lband);
#endif
        for (int y = (VB_EDGE_Y2 - VB_EDGE_R2) / 2; y < PH; y++)
            for (int wx = -1; wx >= lo; wx--) {
                int dx = (wx * 2 + 1 - VB_EDGE_X2) * 8, dy = (y * 2 + 1 - VB_EDGE_Y2) * 8, d = isqrt_i(dx * dx + dy * dy) - R16;
                int X = -wx, rr = vb_reg(side, wx, y), sphere = rr >= VB_SPH && rr < VB_BACK, a;
                Rgb *p = vb_px(side, wx, y), bg;
                if (!p || d >= 80) continue;
                bg = *p;
                if (sphere) bg = scale(bg, 256 - 72 * (80 - (d > 0 ? d : 0)) / 80);   /* (the contact shade) */
                a = clampi((12 - d) * 256 / 24, 0, 256);                               /* (the outline's coverage) */
                if (a > 0) {                                                           /* the user's call: the */
                    int yy = y < 214 ? 214 : y;                                        /* sphere's tone right at the */
                    bg = mix(bg, rgb15(pic[yy][(X - 1) % 3]), a);                      /* cut (picture cols 0-2, */
                }                                                                      /* same row), no gradient */
                if (sphere || a > 0) *p = bg;
            }
    }
}

static void vb_analyse(void)
{
    int cnt[3] = { 0, 0, 0 }, n = 0;
    for (int c = 0; c < 3; c++) {                        /* the classes' ramps (the hut's chimney and roof left out) */
        nsamp = 0;
        for (int y = 0; y < 90; y++) for (int x = 0; x < PW; x++) {
            Rgb k = rgb15(pic[y][x]);
            if (y >= 14 && x >= 60 && x < 200) continue;
            if (vb_class(k) == c && !(c == 0 && y > 62)) { samp_add(pic[y][x]); cnt[c]++; }
        }
        ramp_any(&VB_rc[c]);
        nsamp = 0;
        for (int i = 0; i < VB_NPS; i++)                   /* the front: the visible texels of its spheres of the class
                                                              (not the stones' greys nor the rims' browns) */
            if ((VB_PS[i].fl & VBS_FRONT) && VB_PS[i].cls == c)
                for (int y = 186; y < PH; y++) for (int x = 0; x < PW; x++) if (vb_vis(i, x, y)) samp_add(pic[y][x]);
        if (nsamp >= 200) ramp_any(&VB_rf[c]); else { nsamp = 0; VB_rf[c] = VB_rc[c]; }
        n += cnt[c];
    }
    VB_share[0] = 0;
    for (int c = 0; c < 3; c++) VB_share[c + 1] = VB_share[c] + (n ? cnt[c] * 256 / n : 85);
    vb_light();
}
static void gen_vb(int pad)
{
    pal_make(); vb_analyse();
    for (int c = 0; c < 3; c++) if (!VB_rc[c].ok || !VB_rf[c].ok) return;
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side;
        for (int i = 0; i < 5; i++) vb_quilt(side, &VB_BANDS[side][i], 701 + 13 * i);
        vb_canopy(side);
        vb_front(side);
        vb_edge(side);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                Rgb c = cv[(size_t)y * vb_ow + X - 1];
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(c);
            }
    }
    canvas_close();
}

/* ================================================================== AREA19 s1: the Dwarf Village in cherry blossom
 * The same layout as AREA02 s1 (the same huts, fence, ledge, straw bank, ladder and jukebox), with the canopy a pink
 * cherry-blossom mass over a grey rock band, a green ledge and green ground, and a front row of blurred blossom bushes
 * cut by the frame's bottom.  Everything is a texture here, so every band is quilted from the user's own picture at
 * run time with gen_vb's vb_quilt (blocks of the picture's texels on the same rows, chosen by the error of their
 * overlap with the picture's columns at the edge and with the block before, pasted along minimum-error cuts; nothing
 * mirrored, objects never copied):
 *   - the fence first (left x 0-43 rows 108-137, its first 9 columns the picture's own posts continued with their
 *     period (the post cut by the edge goes on), then cut into by the quilt; right x 186-255 rows 108-138);
 *   - the canopy's lower part with its fringe and the grey rocks under it (rows 60-107, windows shifted up to 8 rows so
 *     the fringe goes on at the height the edge has it: ~row 98 at the left edge, ~104 at the right one; left from any
 *     clean column (the rock towers by the left edge first), right from x 190-319), cut into the fence's top rows; then
 *     its upper part (rows 0-59, from x 0-43 and 192-319, the hut and its chimney left out), cut into the lower part:
 *     the windows keep their rows (up to 3 rows of shift), so the blossom's vertical structure (light flower clusters
 *     above, darker and greyer low, the fringe's brown rim on the rocks) goes on as the picture has it by each edge;
 *   - the green ledge: its lower rows (145-155, the grass and its dark lip) from x 0-215, its top (rows 136/137-144,
 *     where the left edge has the log, x 2-52) from the clean stretch between the hut's pole and the ladder (x 192-213),
 *     so its top edge under the fence is the picture's own; the straw bank (156-180), the ground (181-189);
 *   - the front blossom: one band, rows 186-223 (windows of the picture's front, any column, up to 6 rows of shift,
 *     the bottom rows reflected), on a grass underlay (rows 191-223 from the central valley's clean grass), scored
 *     on the bushes' outline too (VBB_OUTL: a window's scalloped top, shifted, against the one already there over 20
 *     columns, and its next 6 columns against the height and slope there), objects in the source's grass above the
 *     bushes (the stones, the barrel) holes that keep the underlay; then cb_warp bends the strip's first 12 columns
 *     so the outline starts where the picture's ends and goes on with its slope (the right edge's bush high, ~row
 *     189; the left edge's valley flat at ~213 and then up its other flank; on the left over 8 columns only), the
 *     left front keeps to a target outline (vb_otgt / cb_otgt: the edge's valley for a few columns, up to row ~203 by
 *     column 14, then the picture's left bushes' height, ~196-206; windows there shifted up to 10 rows), cb_clean
 *     drops loose blossom bits on its grass and fills grass enclosed by its bushes, and cb_ftone matches the tone per row
 *     and per kind (grass, blossom; R, G, B, so the pink, not only the lightness) over 20 columns, fading by 72,
 *     then the strip's first 2 columns onto the picture's last 2, fading over 12;
 *   - the blurred bands (canopy, front) have their cuts blended over 5 x 5 texels (VBB_FEATHER), and the canopy's tone
 *     is matched at the join per row (cb_tone: the strip's first 8 columns onto the picture's columns 2-9, fading out by
 *     column 64; its first column onto the picture's last one, fading out over 8: the picture's edge columns are a
 *     little darker, about 6 levels, and the strip goes on from them).
 * Objects never copied (CB_EXCL): the hut and its chimney and pole, the log, the bucket, the barrel, the stones and
 * their track, the stone pile, the ladder, the jukebox and its lever (and the front blossom under the jukebox, rows
 * 182-201, whose top the jukebox hides: a flat top). */
static const VbBox CB_EXCL[] = {
    { 44, 40, 192, 144 }, { 70, 12, 110, 52 }, { 2, 138, 53, 145 }, { 138, 126, 162, 150 }, { 32, 164, 60, 192 },
    { 98, 181, 218, 198 }, { 226, 118, 258, 142 }, { 214, 133, 262, 190 }, { 252, 120, 268, 191 }, { 284, 115, 302, 118 }, { 276, 118, 310, 124 },
    { 262, 124, 317, 191 }, { 252, 182, 316, 202 }, { 0, 0, 0, 0 } };
#define CB_S (VBB_STRICT | VBB_TIGHT)
#define CB_F (VBB_TIGHT | VBB_FEATHER | VBB_FAR | VBB_OUTL)
static const VbBand CB_BANDS[2][9] = {
    { { 108, 138, 108, 138, 0, 44, 9, 5, 0, 0, 0, VB_FENCE, 1, 0, 9, VBB_TIGHT },
      { 60, 108, 50, 124, 0, 320, 12, 6, 0, 6, 8, VB_BACK, 0, 0, 0, CB_S | VBB_SOFT | VBB_FEATHER | VBB_FAR },
      { 0, 60, 0, 70, 0, 320, 16, 8, 0, 6, 3, VB_BACK, 0, 0, 0, CB_S | VBB_FEATHER | VBB_FAR },
      { 145, 156, 145, 156, 0, 216, 16, 5, 0, 0, 0, VB_LEDGE, 0, 0, 0, VBB_TIGHT },
      { 138, 145, 136, 148, 192, 214, 8, 4, 2, 3, 0, VB_LEDGE, 0, 0, 0, CB_S },
      { 156, 181, 156, 181, 0, 216, 16, 5, 4, 0, 0, VB_BANK, 1, 0, 0, VBB_TIGHT },
      { 181, 190, 181, 197, 0, 100, 16, 5, 4, 0, 1, VB_GROUND, 0, 0, 0, VBB_TIGHT },
      { 191, PH, 188, 196, 60, 100, 16, 5, 1, 0, 0, VB_GROUND, 0, 0, 0, VBB_TIGHT },
      { 186, PH, 186, PH, 0, 320, 16, 8, 3, 0, 10, VB_FRONTR, 0, 0, 0, CB_F } },
    { { 108, 139, 108, 139, 186, 256, 12, 5, 0, 0, 0, VB_FENCE, 1, 0, 0, VBB_TIGHT },
      { 60, 108, 50, 124, 190, 320, 16, 8, 0, 6, 8, VB_BACK, 0, 0, 0, CB_S | VBB_SOFT | VBB_FEATHER | VBB_FAR },
      { 0, 60, 0, 70, 0, 320, 16, 8, 0, 6, 3, VB_BACK, 0, 0, 0, CB_S | VBB_FEATHER | VBB_FAR },
      { 145, 156, 145, 156, 150, 216, 16, 5, 0, 0, 0, VB_LEDGE, 0, 0, 0, VBB_TIGHT },
      { 139, 145, 136, 148, 192, 214, 8, 4, 2, 3, 0, VB_LEDGE, 0, 0, 0, CB_S },
      { 156, 181, 156, 181, 150, 216, 16, 5, 4, 0, 0, VB_BANK, 1, 0, 0, VBB_TIGHT },
      { 181, 190, 181, 197, 0, 262, 16, 5, 4, 0, 1, VB_GROUND, 0, 0, 0, VBB_TIGHT },
      { 191, PH, 188, 196, 60, 100, 16, 5, 1, 0, 0, VB_GROUND, 0, 0, 0, VBB_TIGHT },
      { 186, PH, 186, PH, 0, 320, 16, 8, 3, 0, 6, VB_FRONTR, 0, 0, 0, CB_F } } };
/* the tone at the join, per row (rows [y0, y1), means over rows y +- 4 of them): the strip's columns 1-8 moved onto
   the picture's columns 2-9 from the edge (full for 16 columns, gone at 64), and its first column onto the picture's
   last one (easing out over 8 columns), so the join is neither a tone step nor a line (the picture's last columns are
   often a little darker: the strip goes on from them and lightens as the picture does inwards) */
static void cb_tone(int side, int y0, int y1)
{
    static int d[PH][2][3];
    for (int y = y0; y < y1; y++) {
        int s[4][3] = { { 0 } }, n[4] = { 0, 0, 0, 0 };     /* picture col 0, cols 2-9, strip col 1, cols 1-8 */
        for (int yy = y - 4; yy <= y + 4; yy++) {
            if (yy < y0 || yy >= y1) continue;
            for (int i = 0; i < 10; i++) {
                int x = side ? PW - 1 - i : i, g = i == 0 ? 0 : i == 1 ? -1 : 1; Rgb k;
                if (g < 0 || vb_excl(x, yy)) continue;
                k = rgb15(pic[yy][x]); s[g][0] += k.r; s[g][1] += k.g; s[g][2] += k.b; n[g]++;
            }
            for (int X = 1; X <= 8; X++) {
                Rgb k = cv[(size_t)yy * vb_ow + X - 1];
                for (int g = X == 1 ? 2 : 3; g <= 3; g++) { s[g][0] += k.r; s[g][1] += k.g; s[g][2] += k.b; n[g]++; }
            }
        }
        for (int c = 0; c < 3; c++) {
            d[y][0][c] = n[1] && n[3] ? s[1][c] / n[1] - s[3][c] / n[3] : 0;
            d[y][1][c] = n[0] && n[2] ? s[0][c] / n[0] - s[2][c] / n[2] - d[y][0][c] : 0;
        }
    }
    for (int y = y0; y < y1; y++)
        for (int X = 1; X <= vb_ow; X++) {
            Rgb *p = &cv[(size_t)y * vb_ow + X - 1];
            int w2 = X <= 16 ? 256 : X >= 64 ? 0 : (64 - X) * 256 / 48, w1 = X <= 8 ? (9 - X) * 256 / 8 : 0;
            p->r = clampi(p->r + (d[y][0][0] * w2 + d[y][1][0] * w1) / 256, 0, 255);
            p->g = clampi(p->g + (d[y][0][1] * w2 + d[y][1][1] * w1) / 256, 0, 255);
            p->b = clampi(p->b + (d[y][0][2] * w2 + d[y][1][2] * w1) / 256, 0, 255);
        }
}
/* the bushes' outline at the join, bent: the strip's columns 1-CB_WN shifted up or down (rows 184 on, a column at a
   time, at most 6 rows) so its outline starts where the picture's ends and goes on with its slope there (the mean of
   the picture's 2 last known columns, the slope against the 2 before, at most 1 row a column), easing into the
   strip's own outline by column CB_WN: no notch or step in the outline on the join */
#define CB_WN 12
#define CB_WNL 8
static void cb_warp(int side)
{
    int h[4], n = 0, hs[CB_WN + 1], col[PH], wn = side ? CB_WN : CB_WNL;
    for (int i = 0; i < 24 && n < 4; i++) { int v = vb_hcol(side, side ? PW - 1 - i : i); if (v >= 0) h[n++] = v; }
    if (n < 2) return;
    for (int X = 1; X <= CB_WN; X++) hs[X] = vb_hcol(side, side ? PW - 1 + X : -X);
    {   int e4 = (h[0] + h[1]) * 2, sl = n == 4 ? e4 - (h[2] + h[3]) * 2 : 0;   /* (x 4) */
        sl = sl > 8 ? 8 : sl < -8 ? -8 : sl;                                   /* (2 columns apart: 1 row a column) */
        for (int X = 1; X <= wn; X++) {
            int t, sft, k = X * 256 / wn, sm = k * k / 256 * (768 - 2 * k) / 256;   /* (smoothstep) */
            if (hs[X] < 0 || hs[X] >= PH) continue;
            t = (e4 + sl * X / 2) / 4; t += (hs[X] - t) * sm / 256;
            sft = t - hs[X]; sft = sft > 6 ? 6 : sft < -6 ? -6 : sft;
            if (!sft) continue;
            for (int y = 184; y < PH; y++) { int sy = y - sft; col[y] = sy < 184 ? 184 : sy >= PH ? PH - 1 : sy; }
            {   Rgb tmp[PH];
                for (int y = 184; y < PH; y++) tmp[y] = cv[(size_t)col[y] * vb_ow + X - 1];
                for (int y = 184; y < PH; y++) cv[(size_t)y * vb_ow + X - 1] = tmp[y];
            }
        }
    }
}
/* the front's tone at the join, per row and per kind (grass, blossom), rows [y0, y1), means over rows y +- 2: the
   strip's first CB_FN columns moved onto the picture's last CB_FN (all three channels, so the blossom's pink, not
   only its lightness), fully up to column CB_FN, gone at column 72 */
#define CB_FN 20
#define CB_FH 12
static void cb_ftone(int side, int y0, int y1)
{
    static int d[PH][2][3];
    for (int y = y0; y < y1; y++) {
        int s[2][2][3] = { { { 0 } } }, n[2][2] = { { 0 } };     /* [picture, strip][grass, blossom] */
        for (int yy = y - 2; yy <= y + 2; yy++) {
            if (yy < y0 || yy >= y1) continue;
            for (int i = 0; i < CB_FN; i++) {
                int x = side ? PW - 1 - i : i; Rgb k;
                if (vb_excl(x, yy)) continue;
                k = rgb15(pic[yy][x]); int g = !vb_grass(k);
                s[0][g][0] += k.r; s[0][g][1] += k.g; s[0][g][2] += k.b; n[0][g]++;
            }
            for (int X = 1; X <= CB_FN; X++) {
                Rgb k = cv[(size_t)yy * vb_ow + X - 1]; int g = !vb_grass(k);
                s[1][g][0] += k.r; s[1][g][1] += k.g; s[1][g][2] += k.b; n[1][g]++;
            }
        }
        for (int g = 0; g < 2; g++)
            for (int c = 0; c < 3; c++)
                d[y][g][c] = n[0][g] >= 8 && n[1][g] >= 8 ? s[0][g][c] / n[0][g] - s[1][g][c] / n[1][g] : 0;
    }
    for (int y = y0; y < y1; y++)
        for (int X = 1; X <= vb_ow; X++) {
            Rgb *p = &cv[(size_t)y * vb_ow + X - 1];
            int g = !vb_grass(*p), w = X <= CB_FN ? 256 : X >= 72 ? 0 : (72 - X) * 256 / (72 - CB_FN);
            p->r = clampi(p->r + d[y][g][0] * w / 256, 0, 255);
            p->g = clampi(p->g + d[y][g][1] * w / 256, 0, 255);
            p->b = clampi(p->b + d[y][g][2] * w / 256, 0, 255);
        }
    /* then the hand-over: the strip's first 2 columns onto the picture's last 2 (rows y +- 1, texels of the same kind
       only), easing out over CB_FH columns (the picture's last columns are often a little darker or lighter than
       the ones before: the strip goes on from them) */
    for (int y = y0; y < y1; y++) {
        int s[3] = { 0, 0, 0 }, n = 0;
        for (int yy = y - 1; yy <= y + 1; yy++) {
            if (yy < y0 || yy >= y1) continue;
            for (int i = 0; i < 2; i++) {
                int x = side ? PW - 1 - i : i; Rgb k = rgb15(pic[yy][x]), q = cv[(size_t)yy * vb_ow + i];
                if (vb_excl(x, yy) || vb_grass(k) != vb_grass(q)) continue;
                s[0] += k.r - q.r; s[1] += k.g - q.g; s[2] += k.b - q.b; n++;
            }
        }
        for (int c = 0; c < 3; c++) d[y][0][c] = n >= 3 ? s[c] / n : 0;
    }
    for (int y = y0; y < y1; y++)
        for (int X = 1; X < CB_FH; X++) {
            Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int w = (CB_FH - X) * 256 / (CB_FH - 1);
            p->r = clampi(p->r + d[y][0][0] * w / 256, 0, 255);
            p->g = clampi(p->g + d[y][0][1] * w / 256, 0, 255);
            p->b = clampi(p->b + d[y][0][2] * w / 256, 0, 255);
        }
}
/* the left strip's front outline: the picture's valley at the edge (its height there) for a few columns, up its
   other flank by column 14, then the bush mass at the picture's height (its left bushes' tops, rows ~196-207) */
static int cb_h0 = 213;
static int cb_otgt(int side, int X, int *tol)
{
    if (side) return -1;
    if (X <= 4) { *tol = 2; return cb_h0; }
    if (X < 14) { *tol = 3; return cb_h0 + (203 - cb_h0) * (X - 4) / 10; }
    *tol = 5; return 201;
}
/* the left front cleaned (rows CB_CY on, the canvas' whole width): blossom bits loose on the grass (8-connected
   non-grass pieces under CB_BLOB texels that do not reach the bottom row) go back to the grass underlay (und, the
   canvas before the front band), with their 1-texel rim; grass enclosed by the bushes (4-connected grass not
   reaching row CB_CY: a valley window lifted shows its grass in the reflected bottom rows) is filled with the blossom
   above it (the hole's height up, moved down whole), or the mean of its blossom neighbours */
#define CB_CY 184
#define CB_BLOB 40
#define CB_RIM 8
static void cb_clean(const Rgb *und)
{
    int w = vb_ow, h = PH - CB_CY, n = w * h, *st = (int *)malloc(sizeof(int) * (size_t)n), *cm = (int *)malloc(sizeof(int) * (size_t)n);
    u8 *gr = (u8 *)malloc((size_t)n);
    if (!st || !cm || !gr) { free(st); free(cm); free(gr); return; }
#define CBP(i) cv[(size_t)((i) / w + CB_CY) * w + (i) % w]
    for (int i = 0; i < n; i++) { gr[i] = (u8)vb_grass(CBP(i)); cm[i] = -1; }
    for (int pass = 0; pass < 2; pass++)                 /* (0: blossom bits, 1: grass holes) */
        for (int i0 = 0; i0 < n; i0++) {
            int sp = 0, cnt = 0, edge = 0, kind = pass ? 1 : 0;
            if (cm[i0] >= 0 || gr[i0] != kind) continue;
            st[sp++] = i0; cm[i0] = i0;
            while (sp) {                                 /* (flood fill; cm: the piece's first texel) */
                int i = st[--sp], y = i / w, x = i % w;
                cnt++;
                if (pass ? y == 0 : y == h - 1) edge = 1;
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++) {
                        int yy = y + dy, xx = x + dx, j;
                        if ((!dx && !dy) || (pass && dx && dy) || yy < 0 || yy >= h || xx < 0 || xx >= w) continue;
                        j = yy * w + xx;
                        if (cm[j] < 0 && gr[j] == kind) { cm[j] = i0; st[sp++] = j; }
                    }
            }
            if (edge || (!pass && cnt >= CB_BLOB)) continue;
            for (int i = 0; i < n; i++) {
                if (cm[i] != i0) continue;
                if (!pass) {                             /* (the bit and its rim: the underlay) */
                    int y = i / w, x = i % w;
                    for (int dy = -1; dy <= 1; dy++)
                        for (int dx = -1; dx <= 1; dx++) {
                            int yy = y + dy, xx = x + dx, j;
                            if (yy < 0 || yy >= h || xx < 0 || xx >= w) continue;
                            j = yy * w + xx;
                            if (j == i || gr[j]) CBP(j) = und[(size_t)(yy + CB_CY) * w + xx];
                        }
                    gr[i] = 1; cm[i] = -1;
                } else cm[i] = -2;                       /* (filled below) */
            }
            if (pass) {                                  /* the hole with its rim (2 texels of the bushes' grey
                                                            edge round it, holes of CB_RIM texels on): the blossom
                                                            its height above it, moved down whole (no per-column
                                                            streaks) */
                int y0 = h, y1 = -1;
                for (int r = 0; r < 2 && cnt >= CB_RIM; r++) {
                    for (int i = 0; i < n; i++) {
                        int y = i / w, x = i % w;
                        if (cm[i] != -2) continue;
                        for (int dy = -1; dy <= 1; dy++)
                            for (int dx = -1; dx <= 1; dx++) {
                                int yy = y + dy, xx = x + dx, j;
                                if (yy < 0 || yy >= h || xx < 0 || xx >= w) continue;
                                j = yy * w + xx;
                                if (!gr[j] && cm[j] > -2) cm[j] = -6;
                            }
                    }
                    for (int i = 0; i < n; i++) if (cm[i] == -6) cm[i] = -2;
                }
                int x0 = w, x1 = -1;
                for (int i = 0; i < n; i++)
                    if (cm[i] == -2) {
                        if (i / w < y0) y0 = i / w;
                        if (i / w > y1) y1 = i / w;
                        if (i % w < x0) x0 = i % w;
                        if (i % w > x1) x1 = i % w;
                    }
                for (int i = 0; i < n; i++) {
                    int sh = x0 - (x1 - x0 + 1) >= 0 ? -(x1 - x0 + 1) : x1 + (x1 - x0 + 1) < w ? x1 - x0 + 1 : 0;
                    int j = sh ? i + sh : i - (y1 - y0 + 1) * w;
                    if (cm[i] != -2 || j < 0 || gr[j] || cm[j] <= -2) continue;
                    CBP(i) = CBP(j); cm[i] = -3;
                }
                for (int i = 0; i < n; i++) if (cm[i] == -2) cm[i] = -5;   /* (left for the mean) */
            }
        }
    for (int i = 0; i < n; i++) if (cm[i] == -5) cm[i] = -2;
    for (int it = 0, left = 1; it < 16 && left; it++) {  /* (what is left: its blossom neighbours' mean, peeled) */
        left = 0;
        for (int i = 0; i < n; i++) {
            int y = i / w, x = i % w, s[3] = { 0, 0, 0 }, c = 0;
            if (cm[i] != -2) continue;
            for (int d = 0; d < 4; d++) {
                int yy = y + (d == 0) - (d == 1), xx = x + (d == 2) - (d == 3), j;
                if (yy < 0 || yy >= h || xx < 0 || xx >= w) continue;
                j = yy * w + xx;
                if (cm[j] == -2 || (gr[j] && cm[j] != -3)) continue;
                s[0] += CBP(j).r; s[1] += CBP(j).g; s[2] += CBP(j).b; c++;
            }
            if (!c) { left = 1; continue; }
            CBP(i).r = s[0] / c; CBP(i).g = s[1] / c; CBP(i).b = s[2] / c; cm[i] = -3;
        }
    }
    {   Rgb *t = (Rgb *)malloc(sizeof(Rgb) * (size_t)n);    /* the fill and its edge softened (3 x 3, twice): a
                                                               copied patch has hard edges in the blurred bushes */
        for (int pass = 0; t && pass < 2; pass++) {
            for (int i = 0; i < n; i++) t[i] = CBP(i);
            for (int i = 0; i < n; i++) {
                int y = i / w, x = i % w, s[3] = { 0, 0, 0 }, c = 0, near = cm[i] == -3;
                if (gr[i] && cm[i] != -3) continue;
                for (int dy = -1; dy <= 1 && !near; dy++)
                    for (int dx = -1; dx <= 1; dx++) {
                        int yy = y + dy, xx = x + dx;
                        if (yy >= 0 && yy < h && xx >= 0 && xx < w && cm[yy * w + xx] == -3) near = 1;
                    }
                if (!near) continue;
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++) {
                        int yy = y + dy, xx = x + dx, j;
                        if (yy < 0 || yy >= h || xx < 0 || xx >= w) continue;
                        j = yy * w + xx;
                        if (gr[j] && cm[j] != -3) continue;
                        s[0] += t[j].r; s[1] += t[j].g; s[2] += t[j].b; c++;
                    }
                CBP(i).r = s[0] / c; CBP(i).g = s[1] / c; CBP(i).b = s[2] / c;
            }
        }
        free(t);
    }
#undef CBP
    free(st); free(cm); free(gr);
}
static void gen_cb(int pad)
{
    pal_make();
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    vb_ex = CB_EXCL;
    for (int side = 0; side < 2; side++) {
        Rgb *und = 0;
        canvas_clear(); cur_side = side;
        for (int i = 0; i < 9; i++) {
            if (i == 8 && !side) {                       /* (the edge's height: the picture's first known column) */
                for (int x = 0; x < 8; x++) { int v = vb_hcol(0, x); if (v >= 0 && v < PH) { cb_h0 = v; break; } }
                vb_otgt = cb_otgt;
            }
            vb_quilt(side, &CB_BANDS[side][i], 901 + 13 * i);
            vb_otgt = 0;
            if (i == 7 && !side && (und = (Rgb *)malloc(sizeof(Rgb) * (size_t)vb_ow * PH)) != 0)
                memcpy(und, cv, sizeof(Rgb) * (size_t)vb_ow * PH);
        }
        if (und) { cb_clean(und); free(und); und = 0; }
        cb_tone(side, 0, 108); cb_warp(side); cb_ftone(side, 181, PH);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(cv[(size_t)y * vb_ow + X - 1]);
    }
    vb_ex = VB_EXCL;
    canvas_close();
}

/* ================================================================== AREA11 s2: the cave with the brick tower
 * Everything by the edges is rock (the ceiling and its light lip, the wall of rock pillars, the mossy floor, on the
 * right the dark shadow along the wall's foot), so every band is quilted from the user's picture (vb_quilt, 28-row
 * bands made bottom-up, each cut into the one below; the floor from x 0-199 on the left and from the shadow's columns
 * on the right, so the shadow band goes on; the tower and the three niches never copied), then cv_tone matches the
 * light per row to the picture's columns next to the edge (the cave darkens towards its sides). */
enum { CV_CEIL = 1, CV_WALL, CV_FLOOR };
static const VbBox CV_EXCL[] = {
    { 26, 24, 124, 210 }, { 162, 22, 228, 70 }, { 204, 70, 268, 142 }, { 252, 90, 304, 152 }, { 0, 0, 0, 0 } };
#define CV_Q (VBB_STRICT | VBB_TIGHT | VBB_FEATHER | VBB_FAR | VBB_EDGEW)
#define CV_NB 8
static const VbBand CV_BANDS[2][8] = {
    { { 196, PH, 190, PH, 0, 200, 16, 6, 0, 0, 4, CV_FLOOR, 0, 0, 0, CV_Q },
      { 168, 196, 162, 210, 0, 200, 16, 6, 0, 8, 4, CV_FLOOR, 0, 0, 0, CV_Q },
      { 140, 168, 134, 182, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 112, 140, 106, 154, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 84, 112, 78, 126, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 56, 84, 50, 98, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 28, 56, 22, 70, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 0, 28, 0, 42, 0, 320, 16, 6, 0, 8, 4, CV_CEIL, 0, 0, 0, CV_Q } },
    { { 196, PH, 190, PH, 200, 320, 16, 6, 0, 0, 4, CV_FLOOR, 0, 0, 0, CV_Q },
      { 168, 196, 162, 210, 200, 320, 16, 6, 0, 8, 4, CV_FLOOR, 0, 0, 0, CV_Q },
      { 140, 168, 134, 182, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 112, 140, 106, 154, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 84, 112, 78, 126, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 56, 84, 50, 98, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 28, 56, 22, 70, 0, 320, 16, 6, 0, 8, 4, CV_WALL, 0, 0, 0, CV_Q },
      { 0, 28, 0, 42, 0, 320, 16, 6, 0, 8, 4, CV_CEIL, 0, 0, 0, CV_Q } } };
/* the strip's light per row, three steps (means over rows y +- R): (1) its luminance (all its columns) mapped onto the
   mean and spread of the picture's CV_TN columns next to the edge, the same map over the whole strip (the cave is
   darker towards its sides: windows of the lit middle would come out too light); (2) its first 8 columns' mean moved
   onto the picture's last 8 (R 4; fully up to column 8, gone at 40); (3) its first column onto the picture's last one
   (R 1, three quarters of the step, easing out over 6 columns) */
#define CV_TN 40
#define CV_TR 16
static void cv_tone(int side, int y0, int y1)
{
    static int mp[PH], dp[PH], ms[PH], ds[PH], jd[PH][2][3];
    for (int y = y0; y < y1; y++) {
        long long s[2] = { 0, 0 }, q[2] = { 0, 0 }; int n[2] = { 0, 0 };
        for (int yy = y - CV_TR; yy <= y + CV_TR; yy++) {
            if (yy < y0 || yy >= y1) continue;
            for (int i = 0; i < CV_TN; i++) {
                int x = side ? PW - 1 - i : i, l;
                if (vb_excl(x, yy)) continue;
                l = lum(rgb15(pic[yy][x])); s[0] += l; q[0] += l * l; n[0]++;
            }
            for (int X = 1; X <= vb_ow; X++) { int l = lum(cv[(size_t)yy * vb_ow + X - 1]); s[1] += l; q[1] += l * l; n[1]++; }
        }
        for (int k = 0; k < 2; k++) {
            int m = n[k] ? (int)(s[k] * 16 / n[k]) : 0, d = n[k] ? isqrt_i((int)((q[k] * 256 - s[k] * s[k] * 256 / n[k]) / n[k])) : 0;
            if (k) { ms[y] = m; ds[y] = d; } else { mp[y] = m; dp[y] = d; }
        }
    }
    for (int y = y0; y < y1; y++) {
        int k = ds[y] && dp[y] ? clampi(dp[y] * 256 / ds[y], 154, 358) : 256;
        if (!ms[y] || !mp[y]) continue;
        for (int X = 1; X <= vb_ow; X++) {
            Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int l = lum(*p), l2 = clampi((mp[y] + (l * 16 - ms[y]) * k / 256) / 16, 1, 255);
            *p = l ? scale(*p, clampi(l2 * 256 / l, 0, 1024)) : (Rgb){ l2, l2, l2 };
        }
    }
    for (int st = 0; st < 2; st++) {
        int R = st ? 1 : 4, N = st ? 1 : 8;
        for (int y = y0; y < y1; y++) {
            int s[2][3] = { { 0 } }, n[2] = { 0, 0 };
            for (int yy = y - R; yy <= y + R; yy++) {
                if (yy < y0 || yy >= y1) continue;
                for (int i = 0; i < N; i++) {
                    int x = side ? PW - 1 - i : i; Rgb a = cv[(size_t)yy * vb_ow + i];
                    s[1][0] += a.r; s[1][1] += a.g; s[1][2] += a.b; n[1]++;
                    if (vb_excl(x, yy)) continue;
                    a = rgb15(pic[yy][x]); s[0][0] += a.r; s[0][1] += a.g; s[0][2] += a.b; n[0]++;
                }
            }
            for (int c = 0; c < 3; c++) jd[y][st][c] = n[0] && n[1] ? (s[0][c] / n[0] - s[1][c] / n[1]) * (st ? 3 : 4) / 4 : 0;
        }
        for (int y = y0; y < y1; y++)
            for (int X = 1; X <= (st ? 6 : 40); X++) {
                Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int w = st ? (7 - X) * 256 / 6 : X <= 8 ? 256 : (40 - X) * 256 / 32;
                p->r = clampi(p->r + jd[y][st][0] * w / 256, 0, 255); p->g = clampi(p->g + jd[y][st][1] * w / 256, 0, 255);
                p->b = clampi(p->b + jd[y][st][2] * w / 256, 0, 255);
            }
    }
}
static void gen_cv(int pad)
{
    pal_make();
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    vb_ex = CV_EXCL;
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side;
        for (int i = 0; i < CV_NB; i++) vb_quilt(side, &CV_BANDS[side][i], 1101 + 13 * i);
        cv_tone(side, 0, PH);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(cv[(size_t)y * vb_ow + X - 1]);
    }
    vb_ex = VB_EXCL;
    canvas_close();
}

/* ================================================================== AREA09 s5: the brick wall with the staircase
 * A frontal brick wall lit by four sconces, crossed by a straight staircase from the bottom left to the top right
 * (rail, balusters, red steps, the marble stringer).  Measured on the picture: the rail's top row is
 * yr(x) = 203.6 - 55 x / 90 at every column (16-column profile matches along the stair, within a row), the marble from
 * yr + 35 to yr + 69, the wall in the stair's shadow from yr + 70; one step is (22.5, 13.75) texels, so four steps are
 * exactly (90, 55) (the shift with the least error against (89 / 91, 54 / 56)).  The strips:
 *   - the wall: quilted from the user's picture (vb_quilt, 28-row bands made bottom-up, windows of any clean column
 *     shifted up to 10 rows, the stair (its band and 2 rows round it) and the sconces never copied: vb_exf / ST_EXCL),
 *     then relit: every copied texel times the light wanted where it goes over the light where it came from (the
 *     light: the wall's luminance averaged over 17 x 17 texels, stair and sconces left out).  The light wanted: on the
 *     left, the edge's light per row with the glow of the sconce by the edge (flame at (26, 120)) falling off
 *     radially, S^2 / (S^2 + r^2), towards the light far from it (the edge's 20th percentile); on the right the wall is
 *     mostly in the stair's shadow, whose light runs in bands along the stair, so a texel takes the light of the edge
 *     column where its line parallel to the stair meets it;
 *   - the stair: the picture's own stair four steps further in ((x + 90, y - 55) on the left, (x - 90, y + 55) on the
 *     right; eight steps where that leaves the clean stretch), so rail, balusters, steps and stringer go on exactly on
 *     their lines; the rows below the rail scaled by the band's width at both places (the stringer's lower edge is
 *     73.3 - 0.013 x rows below the rail: the band widens a little towards the bottom left), so that edge goes on
 *     along its own line; its light per row of the stair's cross-section matched at the edge (the stair 90 columns in against
 *     the stair at the edge);
 *   - the join: per row, the wall's first 8 columns' mean onto the picture's last 8 (fading by 40) and its first column
 *     onto the picture's last one (fading over 6). */
enum { ST_WALL = 1 };
#define ST_PX 90
#define ST_PY 55
static int st_y16(int x) { return 3258 - fdiv(880 * x, ST_PX); }     /* the rail's top row (x 16) at column x */
static int st_t16(int x, int y) { return y * 16 + 8 - st_y16(x); }   /* rows (x 16) below it */
static int st_h16(int x) { return 1173 + fdiv(-215 * x, 1024); }   /* the stringer's lower edge (x 16) below it:
                                                            73.3 - 0.013 x rows (the band widens towards the bottom left) */
static int st_band(int x, int y) { int t = st_t16(x, y); return t >= -16 && t < st_h16(x) + 16; }
static int st_exf(int x, int y) { int t = st_t16(x, y); return t >= -48 && t < st_h16(x) + 48; }
/* the stair's texel for (x, y) from k * four steps further in: its row below the rail scaled by the band's widths
   there and here, so the stringer's lower edge goes on along its own line */
static int st_src(int side, int x, int y, int k, int *xs, int *ys)
{
    int t = st_t16(x, y);
    *xs = side ? x - ST_PX * k : x + ST_PX * k;
    if (t > 0) t = (int)((long long)t * st_h16(*xs) / st_h16(x));
    *ys = fdiv(st_y16(*xs) + t, 16);
    return *xs >= 0 && *xs < PW && *ys >= 0 && *ys < PH;
}
static const VbBox ST_EXCL[] = {                          /* the sconces (three lit, one bare) */
    { 186, 0, 244, 44 }, { 122, 20, 176, 78 }, { 75, 81, 102, 108 }, { 8, 96, 52, 150 }, { 0, 0, 0, 0 } };
#define ST_Q (VBB_STRICT | VBB_TIGHT | VBB_FEATHER | VBB_FAR | VBB_EDGEW)
#define ST_NB 8
static const VbBand ST_BANDS[8] = {
    { 196, PH, 184, PH, 0, 320, 16, 6, 0, 0, 10, ST_WALL, 0, 0, 0, ST_Q },
    { 168, 196, 156, PH, 0, 320, 16, 6, 0, 8, 10, ST_WALL, 0, 0, 0, ST_Q },
    { 140, 168, 128, 186, 0, 320, 16, 6, 0, 8, 10, ST_WALL, 0, 0, 0, ST_Q },
    { 112, 140, 100, 158, 0, 320, 16, 6, 0, 8, 10, ST_WALL, 0, 0, 0, ST_Q },
    { 84, 112, 72, 130, 0, 320, 16, 6, 0, 8, 10, ST_WALL, 0, 0, 0, ST_Q },
    { 56, 84, 44, 102, 0, 320, 16, 6, 0, 8, 10, ST_WALL, 0, 0, 0, ST_Q },
    { 28, 56, 16, 74, 0, 320, 16, 6, 0, 8, 10, ST_WALL, 0, 0, 0, ST_Q },
    { 0, 28, 0, 46, 0, 320, 16, 6, 0, 8, 10, ST_WALL, 0, 0, 0, ST_Q } };
#define ST_LX 26                                         /* the edge sconce's flame */
#define ST_LY 120
#define ST_S2 900                                        /* its glow's radius, squared */
static short st_lf[PH][PW];                              /* the wall's light (0: no wall texel near) */
static int st_le[PH], st_amb;                            /* the light at the edge per row (side being made), far light */
static void st_light(void)
{
    static int S[PH + 1][PW + 1], C[PH + 1][PW + 1];
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int m = !vb_excl(x, y);
            S[y + 1][x + 1] = S[y][x + 1] + S[y + 1][x] - S[y][x] + (m ? lum(rgb15(pic[y][x])) : 0);
            C[y + 1][x + 1] = C[y][x + 1] + C[y + 1][x] - C[y][x] + m;
        }
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int x0 = x - 8 < 0 ? 0 : x - 8, x1 = x + 9 > PW ? PW : x + 9, y0 = y - 8 < 0 ? 0 : y - 8, y1 = y + 9 > PH ? PH : y + 9;
            int s = S[y1][x1] - S[y0][x1] - S[y1][x0] + S[y0][x0], c = C[y1][x1] - C[y0][x1] - C[y1][x0] + C[y0][x0];
            st_lf[y][x] = (short)(c >= 16 ? (s / c > 4 ? s / c : 4) : 0);
        }
}
static void st_edge(int side)                             /* st_le: the light of the 4 edge columns per row, gaps filled */
{
    int h[PH], n = 0;
    for (int y = 0; y < PH; y++) {
        int s = 0, c = 0;
        for (int i = 0; i < 4; i++) { int x = side ? PW - 1 - i : i; if (!vb_excl(x, y) && st_lf[y][x]) { s += st_lf[y][x]; c++; } }
        st_le[y] = c ? s / c : -1;
        if (c) h[n++] = st_le[y];
    }
    for (int y = PH - 2; y >= 0; y--) if (st_le[y] < 0) st_le[y] = st_le[y + 1];   /* (the stair's rows: the light
                                                            under it, as the strip's texels there take it) */
    for (int y = 1; y < PH; y++) if (st_le[y] < 0) st_le[y] = st_le[y - 1];
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && h[j - 1] > h[j]; j--) { int t = h[j]; h[j] = h[j - 1]; h[j - 1] = t; }
    st_amb = n ? h[n / 5] : 32;
}
static int st_want(int side, int X, int y)               /* the light wanted at strip column X, row y */
{
    if (side) {                                          /* (along the stair to the edge column) */
        int ye = y + fdiv(X * ST_PY, ST_PX);
        return st_le[clampi(ye, 0, PH - 1)];
    } else {                                             /* (the edge's light, its sconce's glow falling off) */
        int dx = -X - ST_LX, dy = y - ST_LY, ex = -ST_LX, g = st_le[y] - st_amb;
        if (g <= 0) return st_le[y];
        return st_amb + (int)((long long)g * (ST_S2 + ex * ex + dy * dy) / (ST_S2 + dx * dx + dy * dy));
    }
}
static void st_stair(int side)
{
    int g[72][3], sm[72][3][2];
    memset(sm, 0, sizeof sm);
    for (int i = 0; i < 16; i++)                         /* the light per row of the cross-section: the edge's stair
                                                            against the stair 90 columns in */
        for (int y = 0; y < PH; y++) {
            int x = side ? PW - 1 - i : i, xs, ys, t = st_t16(x, y);
            Rgb a, b;
            if (!st_band(x, y) || t < 0 || !st_src(side, x, y, 1, &xs, &ys)) continue;
            a = rgb15(pic[y][x]); b = rgb15(pic[ys][xs]); t /= 16;
            sm[t][0][0] += a.r; sm[t][1][0] += a.g; sm[t][2][0] += a.b; sm[t][0][1] += b.r; sm[t][1][1] += b.g; sm[t][2][1] += b.b;
        }
    for (int t = 0; t < 72; t++)
        for (int c = 0; c < 3; c++) {
            int a = 0, b = 0;
            for (int u = t - 3; u <= t + 3; u++) if (u >= 0 && u < 72) { a += sm[u][c][0]; b += sm[u][c][1]; }
            g[t][c] = a > 0 && b > 0 ? clampi((int)((long long)a * 256 / b), 160, 400) : 256;
        }
    for (int X = 1; X <= vb_ow; X++)
        for (int y = 0; y < PH; y++) {
            int x = side ? PW - 1 + X : -X, k = 1, xs, ys, t;
            Rgb c;
            if (!st_band(x, y)) continue;
            while ((side ? x - ST_PX * k > 300 : x + ST_PX * k < 8) && k < 8) k++;
            if (!st_src(side, x, y, k, &xs, &ys)) continue;
            c = rgb15(pic[ys][xs]); t = clampi(st_t16(x, y) / 16, 0, 71);
            c.r = clampi(c.r * g[t][0] / 256, 0, 255); c.g = clampi(c.g * g[t][1] / 256, 0, 255); c.b = clampi(c.b * g[t][2] / 256, 0, 255);
            cv[(size_t)y * vb_ow + X - 1] = c;
        }
}
static void st_join(int side)                            /* the wall at the join (stair texels left alone) */
{
    static int jd[PH][2][3];
    for (int st = 0; st < 2; st++) {
        int R = st ? 1 : 4, N = st ? 1 : 8;
        for (int y = 0; y < PH; y++) {
            int s[2][3] = { { 0 } }, n[2] = { 0, 0 };
            for (int yy = y - R; yy <= y + R; yy++) {
                if (yy < 0 || yy >= PH) continue;
                for (int i = 0; i < N; i++) {
                    int x = side ? PW - 1 - i : i, xo = side ? PW + i : -1 - i; Rgb a;
                    if (vb_excl(x, yy) || st_exf(xo, yy)) continue;
                    a = cv[(size_t)yy * vb_ow + i]; s[1][0] += a.r; s[1][1] += a.g; s[1][2] += a.b; n[1]++;
                    a = rgb15(pic[yy][x]); s[0][0] += a.r; s[0][1] += a.g; s[0][2] += a.b; n[0]++;
                }
            }
            for (int c = 0; c < 3; c++) jd[y][st][c] = n[0] && n[1] ? (s[0][c] / n[0] - s[1][c] / n[1]) * (st ? 3 : 4) / 4 : 0;
        }
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= (st ? 6 : 40); X++) {
                Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int w = st ? (7 - X) * 256 / 6 : X <= 8 ? 256 : (40 - X) * 256 / 32;
                if (st_band(side ? PW - 1 + X : -X, y)) continue;
                p->r = clampi(p->r + jd[y][st][0] * w / 256, 0, 255); p->g = clampi(p->g + jd[y][st][1] * w / 256, 0, 255);
                p->b = clampi(p->b + jd[y][st][2] * w / 256, 0, 255);
            }
    }
}
static void gen_st(int pad)
{
    pal_make();
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    vb_srcmap = (int *)malloc(sizeof(int) * (size_t)vb_ow * PH);
    if (!vb_srcmap) { canvas_close(); return; }
    vb_ex = ST_EXCL; vb_exf = st_exf;
    st_light();
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side;
        for (int i = 0; i < vb_ow * PH; i++) vb_srcmap[i] = -1;
        for (int i = 0; i < ST_NB; i++) vb_quilt(side, &ST_BANDS[i], 1301 + 13 * i);
        st_edge(side);
        for (int y = 0; y < PH; y++)                     /* relight */
            for (int X = 1; X <= vb_ow; X++) {
                int s = vb_srcmap[(size_t)y * vb_ow + X - 1], ls, lt;
                if (s < 0) continue;
                ls = st_lf[s / PW][s % PW]; lt = st_want(side, X, y);
                if (ls > 0) cv[(size_t)y * vb_ow + X - 1] = scale(cv[(size_t)y * vb_ow + X - 1], clampi(lt * 256 / ls, 128, 512));
            }
        st_stair(side);
        st_join(side);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(cv[(size_t)y * vb_ow + X - 1]);
    }
    free(vb_srcmap); vb_srcmap = 0;
    vb_ex = VB_EXCL; vb_exf = 0;
    canvas_close();
}

/* ================================================================== AREA09 s3: the treasure hall (gen_th)
 * A box room seen frontally: the back wall with the relief, two side walls running towards the camera, each with an
 * arched door, a beam along its top, a recessed plaster panel, a wooden dado and the stone floor with the red carpet.
 * Nothing past the 4:3 edges but more of those side walls (and a sliver of floor), nearer the camera.
 * Measured on the picture: both side walls have their vanishing point at (159.625, 117.25) (the edge columns' row
 * profiles matched against columns 58-68 / 252-262 along the lines through it, best fit on both sides).  A homothety
 * about that point (screen) is a move along the depth (world): every element parallel to the depth axis (the beam,
 * the recess's top, the dado's rails and panel, the floor's border and the carpet's edge) maps onto itself.  So the
 * walls are rectified into depth / height coordinates where that move is a plain shift: column k of the rectified
 * picture is the picture column at distance D(k) = de q^k from the vanishing point (de: the edge's distance, q =
 * (de - 1) / de: one column per step at the edge), its row j the picture row vy + (j - vy) D(k) / de (j = the row at
 * the edge).  There the strips are quilted outwards with vb_quilt (rows kept: the elements stay on their lines; the
 * plaster's windows may move along the rows), the arches never copied, then mapped back: strip column X is at
 * distance de + X, its rectified column k with D(-k) = de + X, its rows j = vy + (y - vy) de / (de + X).  The
 * plaster: nearest texels both ways (its grain kept); below it (sill, dado, floor) the quilt's source texel is read
 * from the picture in one step (th_back: resampled once, the slats and lines as crisp as the picture's, no softer
 * band past the join).  The join: per row, the strip's first 8 rectified
 * columns' mean moved onto the picture's last 8 (fading by 40), its first column onto the edge's (fading over 6).
 * Everything is read from the picture as it is on the screen (th_shown: the arches' quads show the edge columns 1 row
 * lower from row 141, the right one 1 column further left too), and the other wall's texels from its row with the same
 * line (th_register: the right wall's lines lie 0.3-0.6 row higher): every sloped line of the lower wall goes on
 * across both joins within 0.3 row. */
#define TH_VX16 2554                                      /* the vanishing point (x 16) */
#define TH_VY16 1876
#define TH_NK 54                                          /* rectified columns of picture (inward) used */
#define TH_NKP 34                                         /* ... in the plaster's rows (texels magnified less) */
#define TH_PL0 38                                         /* the plaster's rows (nearest texels both ways) */
#define TH_PL1 132
static int th_plaster(int j) { return j >= TH_PL0 && j < TH_PL1; }
#define TH_NO 400                                         /* rectified columns outwards (tables) */
static u16 th_p0[PH][PW];                                 /* the picture as shown (th_shown; pic holds the rectified
                                                            one meanwhile) */
static u8 th_m[PH][PW];                                   /* rectified texel not to copy */
static long long th_D[2][TH_NO + TH_NK + 1];              /* per wall, D(k), k = -TH_NO..TH_NK, in 2^-20 px */
static int th_dj[2][PH];                                  /* side's rectified picture: the other wall's texels read
                                                            this much lower (1/256 row; th_register) */
static const VbBox TH_EXCL[] = { { 15, 66, 58, 203 }, { 262, 66, 305, 203 }, { 0, 0, 0, 0 } };   /* the arches */
static const VbBox TH_NONE[] = { { 0, 0, 0, 0 } };
static int th_exf(int x, int y) { return th_m[y][x]; }
static int th_de16(int side) { return side ? (PW - 1) * 16 - TH_VX16 : TH_VX16; }
static void th_table(int side)
{
    long long de = (long long)th_de16(side), *D = th_D[side] + TH_NO;
    D[0] = de << 16;
    for (int k = 1; k <= TH_NK; k++) D[k] = D[k - 1] - D[k - 1] * 16 / de;
    for (int k = 1; k <= TH_NO; k++) D[-k] = D[-k + 1] + D[-k + 1] * 16 / (de - 16);
}
static int th_bad(int x, int y)
{
    if (x < 0 || x >= PW || y < 0 || y >= PH) return 1;
    for (int i = 0; TH_EXCL[i].x1; i++)
        if (x >= TH_EXCL[i].x0 && x < TH_EXCL[i].x1 && y >= TH_EXCL[i].y0 && y < TH_EXCL[i].y1) return 1;
    return 0;
}
/* bilinear: the texel at (xf, yf) (2^-20 px) of the picture, 0 when one of the four is out or excluded */
static int th_bil(const u16 (*src)[PW], long long xf, long long yf, Rgb *o)
{
    int x0 = (int)(xf >> 20), y0 = (int)(yf >> 20), fx = (int)((xf >> 12) & 255), fy = (int)((yf >> 12) & 255);
    Rgb a, b;
    if (th_bad(x0, y0) || th_bad(x0 + (fx > 0), y0) || th_bad(x0, y0 + (fy > 0)) || th_bad(x0 + (fx > 0), y0 + (fy > 0))) return 0;
    a = mix(rgb15(src[y0][x0]), rgb15(src[y0][x0 + (fx > 0)]), fx); b = mix(rgb15(src[y0 + (fy > 0)][x0]), rgb15(src[y0 + (fy > 0)][x0 + (fx > 0)]), fx);
    *o = mix(a, b, fy); return 1;
}
static u16 th_15(Rgb c) { return (u16)(clampi((c.r + 4) >> 3, 0, 31) | clampi((c.g + 4) >> 3, 0, 31) << 5 | clampi((c.b + 4) >> 3, 0, 31) << 10); }
/* the rectified picture for the strip on side `side`: its own wall at columns k (left) / 319 - k (right), k < TH_NK,
   and the other wall's rectified columns beside them in the screen's order (more windows for the narrow plaster /
   dado / floor stretches between the edge and the arch; never mirrored: a texel keeps its left / right neighbours) */
#define TH_OTH 160                                        /* the other wall's columns: [160, 230) on the left, [90, 160)
                                                            on the right */
static void th_rectify(int side)
{
    for (int c = 0; c < PW; c++) {
        int w = side, k = side ? PW - 1 - c : c;
        if (!side && c >= TH_OTH && c < TH_OTH + TH_NK) { w = 1; k = TH_OTH + TH_NK - 1 - c; }
        if (side && c >= PW - TH_OTH - TH_NK && c < PW - TH_OTH) { w = 0; k = c - (PW - TH_OTH - TH_NK); }
        for (int j = 0; j < PH; j++) {
            Rgb o; int ok = 0; long long *D = th_D[w] + TH_NO, de = th_de16(w);
            if (k < (th_plaster(j) ? TH_NKP : TH_NK)) {
                long long xf = ((long long)TH_VX16 << 16) + (w ? D[k] : -D[k]);
                long long yf = ((long long)TH_VY16 << 16) + (long long)(j * 16 - TH_VY16) * D[k] / de;
                if (w != side && th_dj[side][j])          /* (the other wall: its row with the same line) */
                    yf = ((long long)TH_VY16 << 16) + ((((long long)j * 256 + th_dj[side][j]) * 16 - ((long long)TH_VY16 << 8)) * D[k] / de >> 8);
                if (th_plaster(j)) {                      /* (the plaster: nearest texels, its grain kept) */
                    int xs = (int)((xf + (1 << 19)) >> 20), ys = (int)((yf + (1 << 19)) >> 20);
                    ok = !th_bad(xs, ys);
                    if (ok) o = rgb15(th_p0[ys][xs]);
                } else ok = th_bil(th_p0, xf, yf, &o);
            }
            pic[j][c] = ok ? th_15(o) : 0;
            th_m[j][c] = (u8)!ok;
        }
    }
}
/* rectified column c of side `side`'s rectified picture: its wall's picture column (x, 2^-20 px) and D / de (as D
   in 2^-20 px and de x 16); 0 when it holds no rectified column of a wall */
static int th_col(int side, int c, long long *x, long long *Dk, long long *de, int *wo)
{
    int w = side, k = side ? PW - 1 - c : c;
    if (c < 0 || c >= PW) return 0;
    if (!side && c >= TH_OTH && c < TH_OTH + TH_NK) { w = 1; k = TH_OTH + TH_NK - 1 - c; }
    if (side && c >= PW - TH_OTH - TH_NK && c < PW - TH_OTH) { w = 0; k = c - (PW - TH_OTH - TH_NK); }
    if (k < 0 || k >= TH_NK) return 0;
    *Dk = th_D[w][TH_NO + k]; *de = th_de16(w); *wo = w;
    *x = ((long long)TH_VX16 << 16) + (w ? *Dk : -*Dk);
    return 1;
}
/* the picture's texel at the rectified position (column cf, row rf; 1/256 texel) of side `side` in one step (no
   rectified texel in between): the nearest column (the wall's vertical lines, the slats and the panel's frame, stay
   one texel wide), linear along it (a line along the depth keeps its slope: its row is the same rectified row in
   every column); 0 when the column holds no wall or a texel is out */
static int th_back(int side, int cf, int rf, Rgb *o)
{
    long long x, Dk, de, yf; int c = (cf + 128) >> 8, w;
    if (!th_col(side, c, &x, &Dk, &de, &w)) return 0;
    if (w != side) rf += th_dj[side][clampi((rf + 128) >> 8, 0, PH - 1)];
    yf = ((long long)TH_VY16 << 16) + (((long long)rf * 16 - ((long long)TH_VY16 << 8)) * Dk / de >> 8);
    return th_bil(th_p0, x, yf, o);
}
#define TH_Q (VBB_STRICT | VBB_TIGHT | VBB_EDGEW | VBB_FAR)
#define TH_NB 8
static const VbBand TH_BANDS[TH_NB] = {                   /* the left side (the right one: columns mirrored) */
    { 0, 40, 0, 40, 0, 214, 12, 5, 0, 0, 0, 1, 0, 0, 0, TH_Q },          /* the beam and the recess's top */
    { 40, 64, 40, 132, 0, 214, 9, 4, 6, 0, 16, 2, 0, 0, 0, TH_Q },      /* the recess's plaster */
    { 64, 88, 40, 132, 0, 214, 9, 4, 6, 0, 40, 2, 0, 0, 0, TH_Q },
    { 88, 110, 40, 132, 0, 214, 9, 4, 6, 0, 56, 2, 0, 0, 0, TH_Q },
    { 110, 132, 40, 132, 0, 214, 9, 4, 6, 0, 60, 2, 0, 0, 0, TH_Q },
    { 132, 166, 128, 166, 0, 214, 6, 3, 4, 0, 0, 3, 0, 0, 0, TH_Q },    /* the dado: the ledge, the rail, the panel */
    { 166, 203, 162, 203, 0, 214, 6, 3, 4, 0, 0, 3, 0, 0, 0, TH_Q },
    { 203, PH, 200, PH, 0, 214, 6, 3, 3, 0, 0, 4, 0, 0, 0, TH_Q } };    /* the floor */
static void th_join(int side)
{
    static int jd[PH][2][3];
    for (int st = 0; st < 2; st++) {
        int R = st ? 1 : 2, N = st ? 1 : 8;
        for (int y = 0; y < PH; y++) {
            int s[2][3] = { { 0 } }, n[2] = { 0, 0 };
            for (int yy = y - R; yy <= y + R; yy++) {
                if (yy < 0 || yy >= PH) continue;
                for (int i = 0; i < N; i++) {
                    int x = side ? PW - 1 - i : i; Rgb a;
                    if (th_m[yy][x]) continue;
                    a = cv[(size_t)yy * vb_ow + i]; s[1][0] += a.r; s[1][1] += a.g; s[1][2] += a.b; n[1]++;
                    a = rgb15(pic[yy][x]); s[0][0] += a.r; s[0][1] += a.g; s[0][2] += a.b; n[0]++;
                }
            }
            for (int c = 0; c < 3; c++) jd[y][st][c] = n[0] && n[1] ? (s[0][c] / n[0] - s[1][c] / n[1]) * (st ? 3 : 4) / 4 : 0;
        }
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= (st ? 6 : 40) && X <= vb_ow; X++) {
                Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int w = st ? (7 - X) * 256 / 6 : X <= 8 ? 256 : (40 - X) * 256 / 32;
                p->r = clampi(p->r + jd[y][st][0] * w / 256, 0, 255); p->g = clampi(p->g + jd[y][st][1] * w / 256, 0, 255);
                p->b = clampi(p->b + jd[y][st][2] * w / 256, 0, 255);
            }
    }
}
/* the plaster's light: its windows come from all over the recess, whose mottling is larger and stronger there than
   next to the edge; the strip's local light (luminance over 7 x 7) is pulled towards the edge's light of its row (the
   picture's 13 columns at the edge, rows +- 6), keeping 1/5 of its own variation; the grain stays */
static void th_plaster_light(int side)
{
    static int M[PH][TH_NO + 2], T[PH];
    for (int y = TH_PL0; y < TH_PL1; y++) {
        int s = 0, n = 0;
        for (int yy = y - 6; yy <= y + 6; yy++) {
            if (!th_plaster(yy)) continue;
            for (int i = 0; i < 13; i++) { int x = side ? PW - 1 - i : i; if (!th_m[yy][x]) { s += lum(rgb15(pic[yy][x])); n++; } }
        }
        T[y] = n ? s * 16 / n : 0;
        for (int X = 1; X <= vb_ow; X++) {
            int s2 = 0, n2 = 0;
            for (int yy = y - 3; yy <= y + 3; yy++) {
                if (!th_plaster(yy)) continue;
                for (int XX = X - 3; XX <= X + 3; XX++) if (XX >= 1 && XX <= vb_ow) { s2 += lum(cv[(size_t)yy * vb_ow + XX - 1]); n2++; }
            }
            M[y][X] = n2 ? s2 * 16 / n2 : 0;
        }
    }
    for (int y = TH_PL0; y < TH_PL1; y++)
        for (int X = 1; X <= vb_ow; X++) {
            int m = M[y][X], t;
            if (!m || !T[y]) continue;
            t = T[y] + (m - T[y]) / 5;
            cv[(size_t)y * vb_ow + X - 1] = scale(cv[(size_t)y * vb_ow + X - 1], clampi(t * 256 / m, 160, 400));
        }
}
/* the picture as it is on the screen: the arches are drawn again over the player as two quads of 40 x 136 pixels whose
   texture spans 39 x 135 texels (u 64-103 / 104-143, v 0-135), and the renderer takes the texel at the pixel's
   position rounded (half up), texel x * 39 / 40 at pixel x: from the quad's pixel 21 (columns 301-319 on the right) one
   column further left, from its row 69 (rows 141-207) one row higher (the left wall's sill, rail, inlay and dado foot
   at the edge 1 row lower than in the picture; the opaque texels only, the picture under a transparent one unmoved).
   The walls are rectified from this, so the strips continue what is on the screen (rectified from the picture, every
   sloped line of the lower wall had a step of about 1 row at both joins).  The OpenGL renderer truncates like the PS1
   (texel floor((x + 1/64) * 39 / 40 + 1/64), shader and half-pixel shift of shims/gpu_gl.c): one column further left
   from the quad's pixel 2, one row higher from its row 5 (rows 77-207); at internal resolution > 1 the same model
   (the HD picture's edges within a fraction of a row of it) */
static void th_shown(void)
{
    for (int i = 0; i < 6; i++) {
        const Piece *p = &room->over[i];
        for (int r = 0; r < p->h; r++)
            for (int x = 0; x < p->w; x++) {
                int sx = p->x + (shown_gl ? (64 * x * (p->w - 1) + 2 * p->w - 1) / (64 * p->w) : (2 * x * (p->w - 1) + p->w) / (2 * p->w));
                int sy = p->y + (shown_gl ? (64 * r * (p->h - 1) + 2 * p->h - 1) / (64 * p->h) : (2 * r * (p->h - 1) + p->h) / (2 * p->h));
                if (p->y + r < PH && p->x + x < PW && pic_ov[sy][sx]) th_p0[p->y + r][p->x + x] = pic[sy][sx];
            }
    }
}
/* the two walls are not drawn quite alike: below the plaster the right wall's lines lie 0.3-0.6 row higher (in the
   rectified rows, the row at the edge) than the left wall's, and the strip's quilt takes windows from both walls, so
   a run of the other wall's texels put every sloped line half a row off its continuation (a step where the run
   begins and ends).  Measured per row on the rectified picture as it is on the screen: the luminance profile down the
   edge's 13 rectified columns (the arches start at the 15th) of each wall, at 1/16 row; for every row from the sill
   down (TH_RG0..TH_RG1) the shift of the right wall's profile (-1.5..1.5 rows, 1/16) that best matches the left's
   over rows +-4 (least squares), kept where the left's profile changes enough over them (an edge), the rows between
   interpolated, then smoothed over 5 rows (measured: -0.41 at the sill's top, -0.31 at its foot and the ledge's
   shadow, -0.38 at the rail, -0.5 at the inlay and the lower rail, -0.56 at the dado's foot, the floor keeps -0.5).
   The other wall's texels are read that much lower (th_rectify, th_back) */
#define TH_RG0 132
#define TH_RG1 208
#define TH_RGB (TH_RG0 - 8)                               /* the profiles' first row */
#define TH_RGN (TH_RG1 - TH_RG0 + 16)
static int th_prof(int w, int j16)                        /* the sum over the edge's 13 rectified columns of wall w of
                                                            the luminance at row j16 / 16; -1 when a texel is missing */
{
    long long *D = th_D[w] + TH_NO, de = th_de16(w); int s = 0; Rgb o;
    for (int k = 0; k < 13; k++) {
        long long xf = ((long long)TH_VX16 << 16) + (w ? D[k] : -D[k]);
        long long yf = ((long long)TH_VY16 << 16) + (((long long)j16 * 256 - ((long long)TH_VY16 << 8)) * D[k] / de >> 8);
        if (!th_bil(th_p0, xf, yf, &o)) return -1;
        s += lum(o);
    }
    return s;
}
static void th_register(void)
{
    static int P[2][TH_RGN * 16], d[PH], ok[PH];
    for (int w = 0; w < 2; w++)
        for (int i = 0; i < TH_RGN * 16; i++) P[w][i] = th_prof(w, TH_RGB * 16 + i);
    for (int j = TH_RG0; j < TH_RG1; j++) {
        long long best = -1; int bt = 0, g = 0;
        for (int jj = j - 4; jj < j + 4; jj++) g += abs(P[0][(jj + 1 - TH_RGB) * 16] - P[0][(jj - TH_RGB) * 16]);
        for (int t = -24; t <= 24; t++) {
            long long e = 0; int bad = 0;
            for (int jj = j - 4; jj <= j + 4 && !bad; jj++) {
                int a = P[0][(jj - TH_RGB) * 16], b = P[1][(jj - TH_RGB) * 16 + t];
                if (a < 0 || b < 0) bad = 1; else e += (long long)(a - b) * (a - b);
            }
            if (!bad && (best < 0 || e < best)) { best = e; bt = t; }
        }
        ok[j] = best >= 0 && g >= 25 * 13; d[j] = bt * 16;
    }
    for (int j = TH_RG0; j < TH_RG1; j++) {                /* rows without an edge: between the nearest ones */
        int a = j, b = j;
        if (ok[j]) continue;
        while (a >= TH_RG0 && !ok[a]) a--;
        while (b < TH_RG1 && !ok[b]) b++;
        if (a >= TH_RG0 && b < TH_RG1) d[j] = d[a] + (d[b] - d[a]) * (j - a) / (b - a);
        else d[j] = a >= TH_RG0 ? d[a] : b < TH_RG1 ? d[b] : 0;
    }
    for (int j = 0; j < PH; j++) {
        int s = 0;
        if (j < TH_RG0) { th_dj[0][j] = th_dj[1][j] = 0; continue; }
        for (int jj = j - 2; jj <= j + 2; jj++) s += d[clampi(jj, TH_RG0, TH_RG1 - 1)];
        th_dj[0][j] = s / 5; th_dj[1][j] = -(s / 5);
    }
}
static u16 th_raw[PH][PW];                                /* the picture, given back to pic at the end */
static void gen_th(int pad)
{
    long long *D; int nk; Rgb *cv0;
    pal_make();
    th_table(0); th_table(1); D = th_D[0] + TH_NO;
    for (nk = 1; nk < TH_NO && D[-nk] < ((long long)th_de16(0) + 16 * (pad + 64)) << 16; nk++) ;
    vb_ow = nk + 2;
    if (!canvas_open(vb_ow)) return;
    vb_srcmap = (int *)malloc(sizeof(int) * (size_t)vb_ow * PH); cv0 = (Rgb *)malloc(sizeof(Rgb) * (size_t)vb_ow * PH);
    if (!vb_srcmap || !cv0) { free(vb_srcmap); free(cv0); vb_srcmap = 0; canvas_close(); return; }
    memcpy(th_raw, pic, sizeof pic); memcpy(th_p0, pic, sizeof pic); th_shown(); th_register();
    vb_ex = TH_NONE; vb_exf = th_exf;
    for (int side = 0; side < 2; side++) {
        long long de = th_de16(side);
        D = th_D[side] + TH_NO; th_rectify(side);
        canvas_clear(); cur_side = side;
        for (int i = 0; i < vb_ow * PH; i++) vb_srcmap[i] = -1;
        for (int i = 0; i < TH_NB; i++) {
            VbBand b = TH_BANDS[i];
            if (side) { short a = b.sx0; b.sx0 = (short)(PW - b.sx1); b.sx1 = (short)(PW - a); }
            vb_quilt(side, &b, 1401 + 13 * i);
        }
        th_plaster_light(side);
        for (size_t i = 0; i < (size_t)vb_ow * PH; i++) cv0[i] = cv[i];
        th_join(side);
        for (int X = 1; X <= pad; X++) {
            long long d = (de + 16 * X) << 16; int k = 0, fk;
            while (k < TH_NO - 1 && D[-k - 1] <= d) k++;
            fk = (int)((d - D[-k]) * 256 / (D[-k - 1] - D[-k]));      /* rectified column k + fk / 256 */
            for (int y = 0; y < PH; y++) {
                long long jf = ((long long)TH_VY16 << 16) + (long long)(y * 16 - TH_VY16) * (de << 16) / (de + 16 * X);
                int j = clampi((int)(jf >> 20), 0, PH - 2), fj = (int)((jf >> 12) & 255);
                Rgb c[2];
                if (th_plaster(j) && th_plaster(j + 1)) {      /* (nearest) */
                    int kk = clampi(k + (fk >= 128), 0, vb_ow), jj = j + (fj >= 128);
                    strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] =
                        quant(kk ? cv[(size_t)jj * vb_ow + kk - 1] : rgb15(pic[jj][side ? PW - 1 : 0]));
                    continue;
                }
                {   /* the dado and the floor: the quilt's source texel in one step from the picture (bilinear
                       once: the slats, rails and the inlay as crisp as the picture's), plus the join's correction */
                    int kr = k + (fk >= 128), jr = j + (fj >= 128), ox = k * 256 + fk - kr * 256, oy = j * 256 + fj - jr * 256, s;
                    Rgb o;
                    if (kr <= vb_ow && jr < PH) {
                        s = kr ? vb_srcmap[(size_t)jr * vb_ow + kr - 1] : jr * PW + (side ? PW - 1 : 0);
                        if (s >= 0 && th_back(side, (s % PW) * 256 + (side ? ox : -ox), (s / PW) * 256 + oy, &o)) {
                            if (kr) {
                                Rgb a = cv[(size_t)jr * vb_ow + kr - 1], b = cv0[(size_t)jr * vb_ow + kr - 1];
                                o.r += a.r - b.r; o.g += a.g - b.g; o.b += a.b - b.b;
                            }
                            strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(o);
                            continue;
                        }
                    }
                }
                for (int u = 0; u < 2; u++) {
                    int kk = clampi(k + u, 0, vb_ow);
                    Rgb a = kk ? cv[(size_t)j * vb_ow + kk - 1] : rgb15(pic[j][side ? PW - 1 : 0]);
                    Rgb b = kk ? cv[(size_t)(j + 1) * vb_ow + kk - 1] : rgb15(pic[j + 1][side ? PW - 1 : 0]);
                    c[u] = mix(a, b, fj);
                }
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(mix(c[0], c[1], fk));
            }
        }
    }
    memcpy(pic, th_raw, sizeof pic);
    free(vb_srcmap); vb_srcmap = 0; free(cv0);
    vb_ex = VB_EXCL; vb_exf = 0;
    canvas_close();
}


/* ================================================================== AREA17 s3 / s4: the Iron Castle hall (gen_hl)
 * (docs/PORT.md "More rooms with picture strips")  A hall seen from inside, the camera pitched up: the back wall
 * frontal (the windows, the shield panel, the balustrade), and two side walls running towards the camera past the
 * corner pilasters, each with the same panel (a gold / copper frame, a narrow pane, a bolted bar, then the wide pane
 * cut by the edge), the cornice above, the balustrade (handrail, slats, base) below, the tiled floor.
 * Measured on the picture: the camera is a pinhole pitched up (every vertical edge, the back wall's bars and frame,
 * the pilasters, the side panels' bars and frames, meets at (153.04, -1921.1); fitted on 8 edges, each slope within
 * 0.003), the horizon at row 155.5 (where each side wall's handrail top and foot meet), the left wall's lines along it
 * vanish at column 161, the right wall's at 160 (rectified rows: the cornice, the frame, the handrail, the base and
 * the foot level within 0.2 row per 10 columns).
 * So each side wall is rectified exactly: a point's rectified row j is the row where its line to the wall's vanishing
 * point meets the edge column (a homothety about it: the wall's lines are rows), its rectified column k its position
 * along the wall (the vertical through it, a line through (153.04, -1921.1), met with the line of row 100: k affine
 * in 1 / distance to the vanishing point), in picture columns at the edge (scaled by HL_KS: the slats' period, 13.25,
 * is 13 rectified columns).  In those coordinates the frame, the handrail, the slats and the base are a rectangle
 * pattern: verticals are columns, the wall's lines rows, the slats periodic.  The strips are quilted there with
 * vb_quilt (rows kept, the slats on their period) and read back from the picture in one step through the quilt's
 * source map (as gen_th).  The edge column is not a rectified column (the verticals lean): a rectified column at
 * the bottom shows 9 columns more of the edge than at the top, so the rectified picture starts at HL_KMIN (the edge
 * at the top) and the texels between there and the edge on the lower rows are the band's own texels one period /
 * shift further in (the gap).  The panels (rows 13-129) repeat the wall's unit (window, screwed bar, pane, frame)
 * with its period (hl_unit).  The vases are never copied. */
#define HL_HY (155 * 256 + 128)                           /* the horizon, 1/256 row */
static const int HL_HX[2] = { 161 * 256, 159 * 256 };     /* each wall's vanishing point (side-local column, 1/256) */
#define HL_VX 39178                                       /* the verticals' vanishing point (screen, 1/256 texel) */
#define HL_VY (-491802)
#define HL_JR (100 * 256)                                 /* the reference row of the along-wall coordinate */
#define HL_KS 251                                         /* rectified column = geometric column x 251 / 256 */
#define HL_KMIN (-9)                                      /* the first rectified column (the edge at the top) */
#define HL_NK 52                                          /* rectified columns of the wall: HL_KMIN .. */
#define HL_PA0 13                                         /* the panels: rectified rows [PA0, PA1) */
#define HL_PA1 129
#define HL_NO 300                                         /* canvas columns at most */
typedef struct { short j0, j1, ka, kb, kc, kd, gap; } HlBand;   /* rows [j0, j1): source columns [ka, kb) and [kc, kd)
                                                            (rectified), the gap filled from `gap` columns further in */
static const HlBand HL_BANDS[] = {
    { 0, 13, HL_KMIN, 41, 0, 0, 13 },                     /* the cornice */
    { HL_PA0, HL_PA1, 0, 0, 0, 0, 13 },                   /* the panels (hl_unit, not quilted) */
    { 129, 184, HL_KMIN, 40, 0, 0, 13 },                  /* the handrail and the slats (one band: hl_rbot) */
    { 184, 197, HL_KMIN, 40, 0, 0, 13 },                  /* the base */
    { 197, PH, HL_KMIN, 28, 0, 0, 18 } };                 /* the floor */
#define HL_NB ((int)(sizeof HL_BANDS / sizeof HL_BANDS[0]))
#define HL_QF (VBB_STRICT | VBB_TIGHT | VBB_EDGEW)
static const VbBand HL_VB[HL_NB] = {                      /* (left side; the right one: columns mirrored) */
    { 0, 13, 0, 13, 0, 56, 8, 3, 0, 0, 0, 1, 0, 0, 0, HL_QF },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 129, 184, 129, 184, 0, 56, 13, 4, 0, 0, 0, 1, 1, 0, 0, HL_QF },
    { 184, 197, 184, 197, 0, 56, 10, 4, 0, 0, 0, 1, 0, 0, 0, HL_QF },
    { 197, PH, 197, PH, 0, 56, 10, 4, 0, 0, 0, 1, 0, 0, 0, HL_QF } };
static const VbBox HL_VASE[] = { { 26, 146, 56, 204 }, { 264, 146, 294, 204 }, { 0, 0, 0, 0 } };   /* (screen) */
static u16 hl_p0[PH][PW];                                 /* the picture (pic holds the rectified one meanwhile) */
static u8 hl_m[PH][PW];                                   /* rectified texel not to copy */
static int hl_gsrc[PH][PW];                               /* a rectified texel's source texel (itself, or for a gap
                                                            texel the one further in), j * PW + c; -1: none */
static int hl_vx(int s) { return s ? 319 * 256 - HL_VX : HL_VX; }
/* the picture point (side-local, 1/256) of the wall's point at geometric column k, rectified row j (1/256) */
static void hl_pt(int s, long long k, long long j, long long *xo, long long *yo)
{
    long long hx = HL_HX[s], hy = HL_HY, vx = hl_vx(s), vy = HL_VY;
    long long xr = k * hx / (hx + k), yr = hy + (HL_JR - hy) * (hx - xr) / hx;   /* on the reference line */
    long long cn = vx - xr, cd = vy - yr, mn = j - hy;
    long long y = (hy * hx * cd + mn * ((hx - xr) * cd + yr * cn)) / (hx * cd + mn * cn);
    *yo = y; *xo = xr + (y - yr) * cn / cd;
}
/* the inverse: geometric column k and rectified row j (1/256) of the side-local point (x, y) (1/256) */
static void hl_kj(int s, long long x, long long y, long long *ko, long long *jo)
{
    long long hx = HL_HX[s], hy = HL_HY, vx = hl_vx(s), vy = HL_VY, xr;
    *jo = hy + (y - hy) * hx / (hx - x);
    xr = hx * (x * (vy - y) + (vx - x) * (HL_JR - y)) / (hx * (vy - y) + (vx - x) * (HL_JR - hy));
    *ko = hx * xr / (hx - xr);
}
static int hl_band(int j) { for (int i = 0; i < HL_NB; i++) if (j >= HL_BANDS[i].j0 && j < HL_BANDS[i].j1) return i; return -1; }
/* rectified column of side s's rectified picture column c and back (oriented like vb_quilt's picture: the right side
   mirrored) */
static int hl_k(int s, int c) { return (s ? PW - 1 - c : c) + HL_KMIN; }
static int hl_c(int s, int k) { return s ? PW - 1 - (k - HL_KMIN) : k - HL_KMIN; }
/* bilinear: the picture (screen) at (x, y) (1/256), 0 when a tap is out or on a vase */
static int hl_mode;                                      /* the read: 2 = bilinear with the fractions sharpened (x2 about the middle) */
static int hl_bil(long long x, long long y, Rgb *o)
{
    int x0 = (int)(x >> 8), y0 = (int)(y >> 8), fx = (int)(x & 255), fy = (int)(y & 255), x1 = x0 + (fx > 0), y1 = y0 + (fy > 0);
    Rgb a, b;
    if (x < 0 || y < 0 || x1 >= PW || y1 >= PH) return 0;
    for (int i = 0; HL_VASE[i].x1; i++)
        if (x1 >= HL_VASE[i].x0 && x0 < HL_VASE[i].x1 && y1 >= HL_VASE[i].y0 && y0 < HL_VASE[i].y1) return 0;
    if (hl_mode == 1) fx = fx >= 128 ? 256 : 0;
    if (hl_mode == 2) { fx = clampi((fx - 64) * 2, 0, 256); fy = clampi((fy - 64) * 2, 0, 256); }
    if (hl_mode == 3) fx = clampi((fx - 64) * 2, 0, 256);
    a = mix(rgb15(hl_p0[y0][x0]), rgb15(hl_p0[y0][x1]), fx); b = mix(rgb15(hl_p0[y1][x0]), rgb15(hl_p0[y1][x1]), fx);
    *o = mix(a, b, fy); return 1;
}
/* the picture at side s's rectified (k, j) (rectified column and row, 1/256) */
static int hl_at(int s, long long k, long long j, Rgb *o)
{
    long long x, y;
    hl_pt(s, k * 256 / HL_KS, j, &x, &y);
    return hl_bil(s ? 319 * 256 - x : x, y, o);
}
static void hl_rectify(int s)
{
    for (int j = 0; j < PH; j++)
        for (int c = 0; c < PW; c++) {
            int k = hl_k(s, c), b = hl_band(j), ok; Rgb o;
            const HlBand *hb = &HL_BANDS[b];
            pic[j][c] = 0; hl_m[j][c] = 1; hl_gsrc[j][c] = -1;
            if (k < HL_KMIN || k >= HL_KMIN + HL_NK) continue;
            ok = hl_at(s, (long long)k * 256, (long long)j * 256, &o);
            if (ok) { pic[j][c] = th_15(o); hl_gsrc[j][c] = j * PW + c; }
            if (ok && ((k >= hb->ka && k < hb->kb) || (k >= hb->kc && k < hb->kd))) hl_m[j][c] = 0;
        }
    /* the gap: the rectified texels left of the picture's edge (lower rows), from `gap` columns further in */
    for (int j = 0; j < PH; j++) {
        int b = hl_band(j);
        for (int k = HL_KMIN + HL_NK - 1; k >= HL_KMIN; k--) {
            int c = hl_c(s, k), k2 = k + HL_BANDS[b].gap, c2;
            long long x, y;
            if (hl_gsrc[j][c] >= 0 || k2 >= HL_KMIN + HL_NK) continue;
            hl_pt(s, (long long)k * 256 * 256 / HL_KS, (long long)j * 256, &x, &y);
            if (x >= 0) continue;                         /* (only outside the picture) */
            c2 = hl_c(s, k2);
            if (hl_gsrc[j][c2] < 0) continue;
            pic[j][c] = pic[j][c2]; hl_gsrc[j][c] = hl_gsrc[j][c2]; hl_m[j][c] = hl_m[j][c2];
        }
    }
}
static int hl_exf(int x, int y) { return hl_m[y][x]; }
/* the handrail's lower edge (its shadow on the slats) is no line along the wall: it is all but level on the screen
   (left: row 146.28 at the edge, rising 0.032 a column inwards; right: 146.35, 0.029; the handrail's top and the
   other lines of the wall go to the vanishing point), so in rectified rows it drifts from 146.3 at the edge to 144.3
   at the corner, and the strip's edge was 0.9 row higher than the line's continuation.  The quilt's windows come from
   all along the wall (the handrail and the slats are one band, so a window holds both): their rows are moved so the
   edge lies where the screen line puts it, at the source column and at the strip texel (fully at HL_RB1, fading to
   none at HL_RB0 above and HL_RB2 down the slats; at most row 150) */
#define HL_RB0 138
#define HL_RB1 146
#define HL_RB2 166                                        /* (back to the wall's rows on the slats) */
static const int HL_RBY[2] = { 37448, 37466 }, HL_RBM[2] = { 130, 117 };   /* the edge's row at the edge column (1/256), and
                                                            its slope (1/4096 row a column) */
static long long hl_rbot(int s, long long kg)             /* its rectified row (1/256) at geometric column kg (1/256) */
{
    long long x, y, hx = HL_HX[s], yl;
    hl_pt(s, kg, (long long)HL_RBY[s], &x, &y);
    yl = HL_RBY[s] + x * HL_RBM[s] / 4096;
    return HL_HY + (yl - HL_HY) * hx / (hx - x);
}
/* the strip texel of side s at strip column X, row y: the quilt's source, read from the picture in one step */
static int hl_src(int s, long long kg, long long jq, Rgb *o)
{
    long long kq = kg * HL_KS / 256; int kr, jr, ox, oy, src;
    {   int b = hl_band(clampi((int)(jq >> 8), 0, PH - 1));   /* (the nearest row of the band) */
        jr = clampi((int)((jq + 128) >> 8), HL_BANDS[b].j0, HL_BANDS[b].j1 - 1); }
    kr = fdiv((int)kq + 128, 256);
    ox = (int)(kq - (long long)kr * 256); oy = (int)(jq - (long long)jr * 256);
    if (kr >= HL_KMIN) { int c = hl_c(s, kr); if (c < 0 || c >= PW) return 0; src = hl_gsrc[jr][c]; }
    else {
        int Xc = HL_KMIN - kr;
        if (Xc > vb_ow) return 0;
        src = vb_srcmap[(size_t)jr * vb_ow + Xc - 1];
        if (src >= 0) src = hl_gsrc[src / PW][src % PW];
    }
    if (src < 0) return 0;
    if (jq > HL_RB0 * 256 && jq < HL_RB2 * 256) {        /* the handrail's lower edge: the source rows moved so it
                                                            lies on its own line (hl_rbot) at the source and here */
        int w = jq < HL_RB1 * 256 ? (int)((jq - HL_RB0 * 256) / (HL_RB1 - HL_RB0)) : (int)((HL_RB2 * 256 - jq) / (HL_RB2 - HL_RB1));
        long long et = hl_rbot(s, kg), es = hl_rbot(s, (long long)hl_k(s, src % PW) * 65536 / HL_KS);
        if (et > 150 * 256) et = 150 * 256;
        oy += (int)((es - et) * w / 256);
    }
    return hl_at(s, (long long)hl_k(s, src % PW) * 256 + ox, (long long)(src / PW) * 256 + oy, o)
        || hl_at(s, (long long)hl_k(s, src % PW) * 256, (long long)(src / PW) * 256 + oy, o)
        || hl_at(s, (long long)hl_k(s, src % PW) * 256, (long long)(src / PW) * 256, o);   /* (by an edge of the
                                                            source: its column, or the texel itself) */
}
/* the side wall's panels (rows HL_PA0..HL_PA1): a repeating unit, seen whole between the edge and the corner: the
   window (dark glass, lit grey at its outer side; k HL_KU0..7), the bar with the two screws (7..17), the grey pane
   (17..30) and the frame's wide bevel (30..43, up to its white rim).  Outwards the unit repeats with its period
   HL_UP (the window's outer end at the top rows' edge to the bevel's rim), each texel its own unit texel (same
   rectified row, so every line goes on); the window's texels the picture does not show (left of the slanted edge on
   the lower rows, at most 9 columns) take, on the glass, the edge's colour on their screen row (the glass's lit side
   and its dark are bands upright on the screen: a reflection, not on the wall), on the frame's rows the edge's texel
   on the wall's line (hl_uv).  Every unit texel is read at its exact position (no rounding to a rectified texel, so
   no stepped edges). */
#define HL_KU0 (-8)
#define HL_UP 51
#define HL_GL0 28                                         /* the window's glass: rectified rows [GL0, GL1) */
#define HL_GL1 113
/* the unit at rectified column kc and row jq (1/256): the picture where it shows the texel; the window's glass it
   does not show takes the picture's edge column on the texel's screen row (clamped to the edge: continuous, so the
   lit side and the dark band stay smooth bands upright on the screen); the frame's rows the nearest texel the
   picture shows on the wall's line */
static int hl_uv(int s, long long kc, long long jq, Rgb *o)
{
    long long x, y;
    if (hl_at(s, kc, jq, o)) return 1;
    hl_pt(s, kc * 256 / HL_KS, jq, &x, &y);
    if (jq >= HL_GL0 * 256 && jq < HL_GL1 * 256 && kc < 7 * 256) return hl_bil(s ? (PW - 1) * 256 : 0, y, o);
    for (int i = 1; i <= 48; i++) if (hl_at(s, kc + i * 64, jq, o)) return 1;
    return 0;
}
static int hl_unit(int s, long long kg, long long jq, Rgb *o)
{
    long long kq = kg * HL_KS / 256, P = (long long)HL_UP * 256, kend = (long long)(HL_KU0 + HL_UP) * 256;
    if (kq < HL_KU0 * 256) kq += (HL_KU0 * 256 - kq + P - 1) / P * P;
    if (kq >= kend) kq -= P;
    if (jq < HL_PA0 * 256) jq = HL_PA0 * 256;
    if (jq > HL_PA1 * 256 - 1) jq = HL_PA1 * 256 - 1;
    if (kq > kend - 128) {                                /* (the frame's outer rim meets the next window: blended
                                                            over half a column, no stepped edge) */
        Rgb a, b; int f = (int)((kq - (kend - 128)) * 2);
        if (!hl_uv(s, kq, jq, &a)) return hl_uv(s, kq - P, jq, o);
        if (!hl_uv(s, kq - P, jq, &b)) { *o = a; return 1; }
        *o = mix(a, b, f); return 1;
    }
    return hl_uv(s, kq, jq, o);
}
static u16 hl_raw[PH][PW];
static void gen_hl(int pad)
{
    long long kg, jq;
    pal_make();
    hl_kj(0, (long long)-(pad + 32) * 256, (long long)HL_HY, &kg, &jq);
    vb_ow = clampi((int)(-kg * HL_KS / 256 / 256) + HL_KMIN + 4, 8, HL_NO);
    if (!canvas_open(vb_ow)) return;
    vb_srcmap = (int *)malloc(sizeof(int) * (size_t)vb_ow * PH);
    if (!vb_srcmap) { canvas_close(); return; }
    memcpy(hl_raw, pic, sizeof pic); memcpy(hl_p0, pic, sizeof pic);
    vb_ex = TH_NONE; vb_exf = hl_exf;
    for (int side = 0; side < 2; side++) {
        hl_rectify(side);
        canvas_clear(); cur_side = side;
        for (int i = 0; i < vb_ow * PH; i++) vb_srcmap[i] = -1;
        for (int i = 0; i < HL_NB; i++) {
            VbBand b = HL_VB[i];
            if (b.y1 <= b.y0) continue;
            if (side) { short a = b.sx0; b.sx0 = (short)(PW - b.sx1); b.sx1 = (short)(PW - a); }
            vb_quilt(side, &b, 1701 + 13 * i);
        }
        hl_mode = 2;                                      /* (crisper: the slats' and rails' edges as in the picture) */
#ifdef WS_BACKDROP_MAIN
        if (getenv("WSB_HLMODE")) hl_mode = atoi(getenv("WSB_HLMODE"));
#endif
        for (int X = 1; X <= pad; X++)
            for (int y = 0; y < PH; y++) {
                Rgb o = { 0, 0, 0 };
                hl_kj(side, (long long)-X * 256, (long long)y * 256, &kg, &jq);
                if (jq >= HL_PA0 * 256 && jq < HL_PA1 * 256) {   /* the panels: the unit, repeated */
                    if (!hl_unit(side, kg, jq, &o)) o = rgb15(hl_p0[y][side ? PW - 1 : 0]);
                } else if (!hl_src(side, kg, jq, &o)) {
                    o = rgb15(hl_p0[y][side ? PW - 1 : 0]);
#ifdef WS_BACKDROP_MAIN
                    if (getenv("WSB_INFO")) fprintf(stderr, "side %d X %d y %d: no source (k %lld j %lld)\n", side, X, y, kg * HL_KS / 65536, jq >> 8);
#endif
                }
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(o);
            }
        hl_mode = 0;
    }
    memcpy(pic, hl_raw, sizeof pic);
    free(vb_srcmap); vb_srcmap = 0;
    vb_ex = VB_EXCL; vb_exf = 0;
    canvas_close();
}
