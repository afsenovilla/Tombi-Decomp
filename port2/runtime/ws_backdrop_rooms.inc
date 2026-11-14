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
   in the 32 columns next to the edge against the strip's region r in columns 4-15; L' = m + (L - ms) k, contrast k
   0.75..1.5.  pic_reg returns 0 for texels to leave out (objects by the edge). */
typedef int (*PicReg)(int side, int x, int y);
#define NREG 12
static void balance_regions(int side, PicReg pic_reg)
{
    static Hist hp[NREG], hs[NREG]; int mp[NREG], dp[NREG], ms[NREG], ds[NREG], k[NREG];
    memset(hp, 0, sizeof hp); memset(hs, 0, sizeof hs);
    for (int X = -31; X <= 0; X++)
        for (int y = 0; y < PH; y++) { int x = pcol(side, X), r = pic_reg(side, x, y); if (r > 0 && r < NREG) hist_add(&hp[r], lum(rgb15(pic[y][x]))); }
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
 * band sideways.  Rows measured on the picture (the same at both edges within a row or two):
 *   0-71   the wood behind: conifers, lit yellow-green on their upper left, hazier ones behind, the pale sky between
 *          their tops in the first ~20 rows;
 *   72-100 the fence: three khaki planks (rows 72-77, 81-87, 92-100) on dark posts, the shade under each plank;
 *   100-102 the shadow of the wall's top edge on the path behind; 103-185 the dry-stone wall: light beige and pale blue
 *          stones 8-25 columns long in courses of 5-8 rows, grey mortar lines, each stone lit on its upper edge; the
 *          wall's foot 184-186 in shade;
 *   186-~205 the orange path; ~205-223 the front bushes, mounds of dark leaves.
 * The stepping stones jutting out of the wall, the barrels, the bench, the notice board and the pipe are objects of the
 * picture and are not continued. */
enum { A5_SKY = 1, A5_TREE, A5_TREE2, A5_PLANK, A5_GAP, A5_PATH, A5_STONE, A5_MORTAR, A5_GROUND, A5_BUSH };
#define A5_FENCE0 72
#define A5_WALL0 103
#define A5_WALL1 186
static Ramp A5_rsky, A5_rtree, A5_rplank, A5_rpost, A5_rstone, A5_rblue, A5_rmortar, A5_rground, A5_rbush, A5_rpath;
static int a5_green(u16 c) { Rgb k = rgb15(c); return k.g >= k.r + 6 && k.g >= k.b + 16; }
static int a5_sky(u16 c) { Rgb k = rgb15(c); return lum(k) > 170 && k.b + 24 >= k.g && k.g < k.r + 24; }
static int a5_plank(u16 c) { Rgb k = rgb15(c); return lum(k) > 130 && k.r >= k.b + 30 && k.g >= k.b + 20 && k.r < k.g + 40; }
static int a5_dark(u16 c) { return lum(rgb15(c)) < 90; }
static int a5_stone(u16 c) { Rgb k = rgb15(c); return lum(k) > 170 && k.r >= k.b - 8 && k.r < k.b + 50; }
static int a5_bluestone(u16 c) { Rgb k = rgb15(c); return lum(k) > 140 && k.b > k.r + 12; }
static int a5_mortar(u16 c) { Rgb k = rgb15(c); int l = lum(k); return l > 95 && l < 160 && k.r < k.b + 30 && k.g + 16 > k.r; }
static int a5_orange(u16 c) { Rgb k = rgb15(c); return k.r >= k.g + 40 && k.g >= k.b + 30; }

/* the picture's region of a texel by the edge (for balance_regions): bands by row, objects left out by colour */
static int a5_picreg(int side, int x, int y)
{
    u16 c = pic[y][x]; (void)side;
    if (y < A5_FENCE0) return a5_sky(c) ? A5_SKY : a5_green(c) ? A5_TREE : 0;
    if (y < 101) return a5_plank(c) ? A5_PLANK : 0;
    if (y < A5_WALL0) return 0;
    if (y < A5_WALL1 - 2) return a5_stone(c) || a5_bluestone(c) ? A5_STONE : a5_mortar(c) ? A5_MORTAR : 0;
    if (y < 200) return a5_orange(c) ? A5_GROUND : 0;
    if (y >= 210) return a5_green(c) ? A5_BUSH : 0;
    return 0;
}

static void a5_analyse(void)
{
    samp_edges(0, 64, 0, 30, a5_sky); ramp_make(&A5_rsky);
    samp_edges(0, 48, 0, 70, a5_green); ramp_make(&A5_rtree);
    samp_edges(0, 24, 72, 100, a5_plank); ramp_make(&A5_rplank);
    samp_edges(0, 24, 72, 101, a5_dark); ramp_make(&A5_rpost);
    samp_edges(0, 80, A5_WALL0, A5_WALL1 - 2, a5_stone); ramp_make(&A5_rstone);
    samp_edges(0, 80, A5_WALL0, A5_WALL1 - 2, a5_bluestone); ramp_make(&A5_rblue);
    samp_edges(0, 80, A5_WALL0, A5_WALL1 - 2, a5_mortar); ramp_make(&A5_rmortar);
    samp_edges(0, 64, 188, 200, a5_orange); ramp_make(&A5_rground);
    samp_edges(0, 64, 92, 100, a5_orange); samp_edges(0, 40, 96, 101, a5_orange); ramp_make(&A5_rpath);
    samp_edges(0, 64, 210, PH, a5_green); ramp_make(&A5_rbush);
    if (!A5_rpath.ok) A5_rpath = A5_rground;
    if (!A5_rblue.ok) A5_rblue = A5_rstone;
}

/* the wood: hazy trees behind (a band of soft clumps), then conifers in front: each a stack of drooping tiers of
   needle clumps, narrow at the top, widest low down; the sky (its ramp) above them */
static void a5_wood(int side)
{
    int x0 = side ? PW : -cw - 40, x1 = side ? PW + cw + 40 : 0;     /* picture columns the strip spans (+ margin) */
    for (int X = 1; X <= cw; X++)
        for (int y = 0; y < A5_FENCE0; y++) {
            int wx = wx_of(side, X), t = 150 + y * 2 + noise(wx, y, 23, 9, 301) / 4 - 32;
            putr(X, y, ramp(&A5_rsky, t), A5_SKY);
        }
    /* the hazy row behind: clumps on a wavy line (top row 10-24) */
    for (int wx = (x0 / 6) * 6 - 12; wx < x1; wx += 3) {
        int top = 12 + noise(wx, 0, 37, 1, 311) * 14 / 256;
        for (int y = top; y < A5_FENCE0 + 4; y += 4) {
            u32 h = hash3(wx, y, 313); int X = side ? wx - (PW - 1) : -wx;
            blob(X * 16 + (int)(h & 31) - 16, y * 16, 40 + (int)(h >> 8 & 31), 28 + (int)(h >> 16 & 15), &A5_rtree,
                 60 + (int)(h >> 20 & 31) - (y - top), A5_TREE2, 317);
        }
    }
    {   Rgb sky = ramp_mean(&A5_rsky);                                       /* the haze on the trees behind */
        for (int y = 0; y < A5_FENCE0; y++) for (int X = 1; X <= cw; X++) if (reg_at(X, y) == A5_TREE2) { Rgb *p = px_at(X, y); *p = mix(*p, sky, 80); }
    }
    /* the conifers: one every ~34 columns, top row -8..22; a dark silhouette first (the shade inside the crown), then
       its tiers from the bottom up, so each tier's drooping tips lie over the top of the one below */
    for (int k = fdiv(x0, 34) - 1; k * 34 < x1 + 34; k++) {
        u32 h = hash3(k, 0, 331);
        int cx = k * 34 + (int)(h % 16) - 8, top = (int)(h >> 8) % 30 - 8, wmax = 17 + (int)(h >> 16 & 7), ntier = (A5_FENCE0 + 8 - top) / 7;
        for (int y = top + 2; y < A5_FENCE0 + 4; y++) {
            int w = clampi((y - top) * wmax / 44 + 1, 1, wmax) - 1;
            for (int lx = cx - w; lx <= cx + w; lx++) {
                int X = side ? lx - (PW - 1) : -lx;
                if (X >= 1 && X <= cw && y >= 0) putr(X, y, ramp(&A5_rtree, 8 + noise(lx, y, 4, 4, 333) / 8), A5_TREE);
            }
        }
        for (int j = ntier - 1; j >= 0; j--) {
            int y = top + j * 6, w = clampi((y + 5 - top) * wmax / 44 + 3, 3, wmax), n = w / 2 + 2;
            Rng r = rng_at(k, j, 339);
            for (int q = 0; q < n; q++) {
                int i = rndr(&r, -w, w), dy = rndi(&r, 5), jx = rndi(&r, 32), sz = rndi(&r, 24), lt = rndi(&r, 48);
                int lx = cx + i, ly = y + dy + (i < 0 ? -i : i) * 3 / 8;               /* the tier droops outwards */
                int X = side ? lx - (PW - 1) : -lx;
                int tone = 165 - j * 3 - i * 5 + lt - 24;                               /* lit on the upper left */
                blob(X * 16 + jx - 8, ly * 16 + 8, 44 + sz, 34 + sz / 2, &A5_rtree, tone, A5_TREE, 347);
            }
        }
    }
}

/* the fence: three planks on posts, dark between them (the posts and the shade of the wood) */
static const int A5_PLANKS[3][2] = { { 72, 78 }, { 81, 88 }, { 92, 101 } };
static void a5_fence(int side)
{
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X);
        for (int y = A5_FENCE0; y < A5_WALL0; y++) {
            int t = 70 + noise(wx, y, 7, 3, 351) / 3 - 40, r = A5_GAP; Rgb c = ramp(&A5_rpost, t);
            if (y >= 98) { c = ramp(&A5_rpath, 110 + noise(wx, y, 9, 2, 353) / 4 - 32 - (y >= 100 ? 70 : 0)); r = A5_PATH; }
            for (int p = 0; p < 3; p++) {
                int y0 = A5_PLANKS[p][0], y1 = A5_PLANKS[p][1], seg = fdiv(wx + p * 23, 61), u = wx + p * 23 - seg * 61;
                if (y < y0 || y >= y1) continue;
                {   u32 h = hash3(seg, p, 357);
                    int tone = 130 + (int)(h & 63) - 32 + noise(wx * 1, y * 6, 23, 16, 359 + p) / 4 - 32;   /* grain along the board */
                    if (y == y0) tone += 40; else if (y == y1 - 1) tone -= 70; else if (y == y1 - 2) tone -= 25;
                    if (u == 0) tone -= 70; else if (u == 1) tone += 20;                 /* the board's ends */
                    c = ramp(&A5_rplank, tone); r = A5_PLANK;
                }
            }
            putr(X, y, c, r);
        }
    }
}

/* the dry-stone wall: irregular stones laid as a jittered grid (cells 12 x 6, every other row shifted by half a cell)
   and each texel given to its nearest stone centre (a squarish distance, stretched sideways, so the stones are long,
   flat and roughly rectangular);
   where the two nearest centres are almost as near, the grey mortar; each stone its own tone (one in seven pale blue),
   lit on its upper edge and shaded on its lower one (the neighbouring texels above / below belong to another stone),
   the wall's lit top edge and its foot in shade */
#define A5_CW 12
#define A5_CH 6
static int a5_stone_at(int wx, int y, int *edge)
{
    int gy = fdiv(y - A5_WALL0, A5_CH), best = 1 << 30, second = 1 << 30, id = 0;
    for (int j = gy - 1; j <= gy + 1; j++) {
        int sh = (j & 1) * A5_CW / 2, gx = fdiv(wx - sh, A5_CW);
        for (int i = gx - 1; i <= gx + 1; i++) {
            u32 h = hash3(i, j, 441);
            int px = i * A5_CW + sh + (int)(h % 9) - 4, py = A5_WALL0 + j * A5_CH + (int)(h >> 8) % 3 - 1;
            int dx = (wx - px) * 16 * 2 / 5, dy = (y - py) * 16, d;                    /* sideways stretched */
            dx = dx < 0 ? -dx : dx; dy = dy < 0 ? -dy : dy; d = (dx > dy ? dx : dy) * 3 + (dx + dy);   /* squarish */
            if (d < best) { second = best; best = d; id = (int)(hash3(i, j, 443) & 0x7FFFFFFF); }
            else if (d < second) second = d;
        }
    }
    *edge = (second - best) / 4;
    return id;
}
static void a5_wall(int side)
{
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X);
        for (int y = A5_WALL0; y < A5_WALL1; y++) {
            int e, eu, ed, id = a5_stone_at(wx, y, &e), up = a5_stone_at(wx, y - 1, &eu), dn = a5_stone_at(wx, y + 1, &ed);
            Rgb c; int r;
            if (y >= A5_WALL1 - 3) {                                            /* the foot in shade */
                c = scale(ramp(&A5_rmortar, 60 + noise(wx, y, 5, 2, 403) / 4), y == A5_WALL1 - 1 ? 150 : 190); putr(X, y, c, A5_MORTAR); continue;
            }
            if (e < 11) { c = ramp(&A5_rmortar, 115 + noise(wx, y, 3, 3, 413) / 3 - 42); r = A5_MORTAR; }
            else {
                u32 h = (u32)id; int blue = (h >> 4) % 7 == 0;
                int tone = 140 + (int)(h >> 8 & 63) - 32 + noise(wx, y, 6, 4, 415) / 5 - 25;
                if (up != id || eu < 11) tone += 45;                            /* the lit upper edge */
                else if (dn != id || ed < 11) tone -= 55;                       /* the shaded lower edge */
                else if (e < 26) tone -= 12;
                if ((h >> 12 & 15) == 0) tone -= 40;                            /* a weathered stone */
                c = ramp(blue ? &A5_rblue : &A5_rstone, tone); r = A5_STONE;
            }
            if (y == A5_WALL0) c = scale(c, 290);                               /* the lit top edge of the wall */
            putr(X, y, c, r);
        }
    }
}

/* the path: orange, mottled, a little shade under the wall; the bushes in front: mounds of leaf clumps */
static void a5_ground(int side)
{
    for (int X = 1; X <= cw; X++) {
        int wx = wx_of(side, X);
        for (int y = A5_WALL1; y < PH; y++) {
            int t = 150 + noise(wx, y, 19, 4, 421) / 4 - 32 + noise(wx, y, 3, 2, 423) / 6 - 21;
            if (y < A5_WALL1 + 2) t -= 60; else if (y < A5_WALL1 + 4) t -= 25;
            putr(X, y, ramp(&A5_rground, t), A5_GROUND);
        }
    }
    {   int x0 = side ? PW - 20 : -cw - 30, x1 = side ? PW + cw + 30 : 20;
        for (int k = fdiv(x0, 22) - 1; k * 22 < x1; k++) {                   /* a mound every ~22 columns */
            u32 h = hash3(k, 0, 431); int mx = k * 22 + (int)(h % 10) - 5, top = 198 + (int)(h >> 8) % 9, w = 12 + (int)(h >> 12 & 7);
            for (int y = top; y < PH + 6; y += 4)
                for (int i = -w; i <= w; i += 4) {
                    int hw = w * isqrt_i(clampi(256 - (top + 14 - y) * (top + 14 - y) * 256 / 196, 0, 256)) / 16;   /* rounded top */
                    u32 g = hash3(mx * 64 + i, y, 433); int lx, X, tone;
                    if ((i < 0 ? -i : i) > hw + 2) continue;
                    lx = mx + i + (int)(g & 3) - 1; X = side ? lx - (PW - 1) : -lx;
                    tone = 120 - (y - top) * 5 - i * 3 + (int)(g >> 8 & 63) - 32;
                    blob(X * 16 + 8, (y + (int)(g >> 4 & 1)) * 16, 56 + (int)(g >> 16 & 15), 40 + (int)(g >> 20 & 7), &A5_rbush, tone, A5_BUSH, 437);
                }
        }
    }
}

static void gen_a5(int pad)
{
    pal_make(); a5_analyse();
    if (!A5_rsky.ok || !A5_rtree.ok || !A5_rplank.ok || !A5_rpost.ok || !A5_rstone.ok || !A5_rmortar.ok || !A5_rground.ok
        || !A5_rbush.ok || !canvas_open(pad)) return;
    for (int side = 0; side < 2; side++) {
        canvas_clear();
        a5_wood(side); a5_fence(side); a5_wall(side); a5_ground(side);
        balance_regions(side, a5_picreg);
        emit(side);
    }
    canvas_close();
}

static void gen_village(int pad) { (void)pad; }
static void gen_hall(int pad) { (void)pad; }
