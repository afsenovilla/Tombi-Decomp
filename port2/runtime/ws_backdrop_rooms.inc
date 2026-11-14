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
        balance_regions(side, a5_picreg, 0);
        emit(side);
    }
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
    if (y < V_FENCE0) return 0;
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
            putr(X, py, ramp(rp, t), r);
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
static void gen_hall(int pad) { (void)pad; }
