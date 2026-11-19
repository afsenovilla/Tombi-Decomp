/* ws_topdown.c: a top-down map picture that does not fill the wide frame (docs/PORT.md "Top-down map margins").
 *
 * Two cases, both previews (off by default: TG_ROOMS `approved`, --ws-topdown-gen on):
 *
 * TG_TRIM (Dwarf Village, AREA02 s0 / AREA19 s0): the TIM is padded to whole tiles with black columns (448 wide,
 * the picture is 432).  The padding is dropped from the picture's width, so the wide frame stops at the picture's
 * real edge.  Nothing is generated.
 *
 * TG_MARGIN (Hidden Village, AREA19 s2): the picture (384 px) is narrower than the wide frame, which the game's
 * centring left with black columns on both sides everywhere.  The picture is a rock pinnacle under a sky, and both
 * side edges cut the same two things: the sky with its clouds, and the rock face below the pinnacle's outline.  A
 * margin is built once per picture from the picture's own texels (palette indices) next to that edge:
 *   - the band of TG_SB columns at the edge is classed sky / rock / other (the trees, the palisade, the houses:
 *     never a source);
 *   - the outline (first rock row per column) is continued straight out with the slope it has in its last 12
 *     columns: above it the margin wants sky, below it rock;
 *   - the margin is quilted in TG_B blocks with TG_O overlap, outwards from the picture and top to bottom: for each
 *     block the source block of the band (same rows +- TG_DY, further up along the outline's drop) whose overlap
 *     with what is already there differs least and whose classes match the wanted ones, picked by a hash among
 *     the candidates within TG_TOL % of the best (and a small price for re-used texels) so that no period builds
 *     up; minimum-error cuts through the overlaps between blocks;
 *   - the join with the picture cannot be cut (the picture stays as it is), so the overlap's weight grows towards
 *     the join, and what still differs there is carried into the margin: the row's mean tone difference fading
 *     over the block, the join column's own difference over TG_D columns; sky over sky is feathered.  Changed
 *     pixels take the nearest colour of the picture's palette among those the class uses.
 * No mirroring: the right edge is worked on in flipped coordinates and written back flipped, each texel keeps its
 * orientation.  Integer only and in a fixed order, so every build gives the same margin, and the block columns
 * never depend on the ones further out: a wider frame's margin starts with the narrower frame's columns.
 * The margin is part of the picture (port_ws_overhead_map uploads from the widened copy), so it scrolls with it. */

#define TG_B 16                           /* block */
#define TG_O 6                            /* overlap */
#define TG_NW (TG_B - TG_O)
#define TG_SB 96                          /* source band: columns at the picture's edge */
#define TG_DY 40
#define TG_TOL 10
#define TG_USEW 300
#define TG_D 4
#define TG_MAXM 90                        /* generated columns per side at most (21:9 needs 88) */
#define TG_MAXH 512
#define TG_MAXW 448
#define TG_CW (TG_MAXM + TG_SB)
#define TG_NX (TG_SB - TG_B + 1)

enum { TG_TRIM = 1, TG_MARGIN = 2 };
int port_ws_topdown_gen = 1;              /* --ws-topdown-gen: 1 the approved rooms (default), 0 off, 3 every room of the table */
static const struct { u8 area, sec, approved, kind; } TG_ROOMS[] = {
    {19, 2, 1, TG_MARGIN},                /* Hidden Village */
    { 2, 0, 1, TG_TRIM},                  /* Dwarf Village */
    {19, 0, 1, TG_TRIM},                  /* Dwarf Village, the AREA19 copy (runs as AREA02 s0) */
};
static int tg_room(void)
{
    if (!port_ws_topdown_gen) return 0;
    const u8 *g = port_ram_ptr(PORT_A_GAME);
    int a = *(const u16 *)(g + 0), sc = *(const u16 *)(g + 2);
    for (unsigned i = 0; i < sizeof TG_ROOMS / sizeof TG_ROOMS[0]; i++)
        if (TG_ROOMS[i].area == a && TG_ROOMS[i].sec == sc)
            return port_ws_topdown_gen == 3 || TG_ROOMS[i].approved ? TG_ROOMS[i].kind : 0;
    return 0;
}

/* TG_TRIM: the picture's width without its trailing black columns (kept even, at least 320) */
static int tg_trim(const u8 *tim, const u8 *pic, int stride, int pw, int ph)
{
    int w = pw;
    while (w > 320) {
        int y = 0;
        for (; y < ph; y++) if (rd16(tim + 0x14 + pic[(size_t)y * stride + w - 1] * 2) & 0x7FFF) break;
        if (y < ph) break;
        w--;
    }
    return (w + 1) & ~1;
}

static u8 tgA[TG_MAXH][TG_SB], tgK[TG_MAXH][TG_SB];
static u8 tgC[TG_MAXH][TG_CW], tgKC[TG_MAXH][TG_CW], tgKA[TG_MAXH][TG_CW], tgF[TG_MAXH][TG_CW];
static int tgI[4][TG_MAXH + 1][TG_SB + 1];                /* integrals: class 0 / 1 / 2 counts, re-use counts */
static u16 tgUse[TG_MAXH][TG_SB];
static int tgP[256][3], tgD2[256][256];
static u8 tgSet[2][256];
static long long tgCost[TG_MAXH * TG_NX];
static u8 tgG[2][TG_MAXH][TG_MAXM];                      /* the margins: side, row, column outwards */
static u8 tgE[TG_MAXH * (TG_MAXW + 2 * TG_MAXM)];        /* the widened picture handed to the uploads */

static int tg_fdiv(int a, int b) { return a >= 0 ? a / b : -((-a + b - 1) / b); }
static int tg_near(const int *c, int k)
{
    int best = 0x7FFFFFFF, bi = 0;
    for (int i = 0; i < 256; i++) {
        if (!tgSet[k][i]) continue;
        int r = tgP[i][0] - c[0], g = tgP[i][1] - c[1], b = tgP[i][2] - c[2], d = 3 * r * r + 4 * g * g + 2 * b * b;
        if (d < best) { best = d; bi = i; }
    }
    return bi;
}
static void tg_integral(int n, int H)
{
    for (int x = 0; x <= TG_SB; x++) tgI[n][0][x] = 0;
    for (int y = 0; y < H; y++) {
        int row = 0;
        tgI[n][y + 1][0] = 0;
        for (int x = 0; x < TG_SB; x++) {
            row += n < 3 ? tgK[y][x] == n : tgUse[y][x];
            tgI[n][y + 1][x + 1] = tgI[n][y][x + 1] + row;
        }
    }
}
static int tg_box(int n, int sy, int sx)
{
    return tgI[n][sy + TG_B][sx + TG_B] - tgI[n][sy][sx + TG_B] - tgI[n][sy + TG_B][sx] + tgI[n][sy][sx];
}

/* the band's classes (tgK: 0 sky, 1 rock, 2 other) and the outline row per column */
static void tg_classes(int H, int *ys)
{
    static u8 skyc[TG_MAXH][TG_SB], okc[TG_MAXH][TG_SB];
    for (int y = 0; y < H; y++) for (int x = 0; x < TG_SB; x++) {
        const int *p = tgP[tgA[y][x]];
        int mn = p[0] < p[1] ? p[0] : p[1];
        if (p[2] < mn) mn = p[2];
        skyc[y][x] = (p[2] - p[0] >= 8 && p[2] >= p[1]) || mn >= 184;
    }
    for (int x = 0; x < TG_SB; x++) {
        int run = 0, y = H;
        for (int yy = 0; yy < H; yy++) {
            if (!skyc[yy][x]) { if (++run == 3) { y = yy - 2; break; } }
            else run = 0;
        }
        ys[x] = y;
    }
    for (int y = 0; y < H; y++) for (int x = 0; x < TG_SB; x++) {
        const int *p = tgP[tgA[y][x]];
        int mn = p[0], mx = p[0], sky = y < ys[x];
        for (int c = 1; c < 3; c++) { if (p[c] < mn) mn = p[c]; if (p[c] > mx) mx = p[c]; }
        okc[y][x] = sky || (mx - mn <= 56 && p[1] <= p[0] + 8 && p[2] <= p[0] && p[1] >= p[2]);
    }
    for (int y = 0; y < H; y++) {
        int gap = 0, open = 1;
        for (int x = 0; x < TG_SB; x++) {
            tgK[y][x] = 2;
            if (y < ys[x]) { tgK[y][x] = 0; continue; }
            if (!open) continue;
            int vote = 0;
            for (int j = -4; j <= 4; j++) {
                int yy = y + j < 0 ? 0 : y + j >= H ? H - 1 : y + j;
                for (int i = -4; i <= 4; i++) {
                    int xx = x + i < 0 ? 0 : x + i >= TG_SB ? TG_SB - 1 : x + i;
                    vote += okc[yy][xx];
                }
            }
            if (vote < 73) { if (++gap >= 6) open = 0; continue; }
            gap = 0; tgK[y][x] = 1;
        }
    }
}

/* one side's margin: nb block columns (TG_NW columns each) into tgG[side] */
static void tg_side(const u8 *pic, int stride, int pw, int H, int side, int nb)
{
    static int ys[TG_SB], ysil[TG_MAXM + 1];
    static struct { short i, j, w; } ov[TG_B * TG_B];
    static u8 cbv[TG_B * TG_B];
    int M = nb * TG_NW, CW = M + TG_SB;
    for (int y = 0; y < H; y++) for (int x = 0; x < TG_SB; x++)
        tgA[y][x] = pic[(size_t)y * stride + (side ? pw - 1 - x : x)];
    tg_classes(H, ys);
    int s1 = (ys[0] - ys[12]) * 256 / 12;
    if (s1 < 0) s1 = 0;
    for (int o = 0; o <= M; o++) ysil[o] = ys[0] + ((s1 * o + 128) >> 8);
    memset(tgSet, 0, sizeof tgSet);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < CW; x++) {
            if (x >= M) { tgC[y][x] = tgA[y][x - M]; tgKC[y][x] = tgK[y][x - M]; tgF[y][x] = 1; }
            else { tgC[y][x] = 0; tgKC[y][x] = y >= ysil[M - x]; tgF[y][x] = 0; }
            tgKA[y][x] = tgKC[y][x];
        }
        for (int x = 0; x < TG_SB; x++) { if (tgK[y][x] < 2) tgSet[tgK[y][x]][tgA[y][x]] = 1; tgUse[y][x] = 0; }
    }
    for (int n = 0; n < 3; n++) tg_integral(n, H);

    for (int x1 = M + TG_O; x1 > TG_O; x1 -= TG_NW) {
        int x0 = x1 - TG_B, first = x1 == M + TG_O, drop = ysil[M - x0] - ys[0];
        for (int y0 = 0;; y0 += TG_NW) {
            if (y0 + TG_B > H) y0 = H - TG_B;
            /* what is already there under the block, with its weight; the wanted classes */
            int nov = 0, uni = tgKC[y0][x0], tov = 0;
            for (int i = 0; i < TG_B; i++) for (int j = 0; j < TG_B; j++) {
                if (tgKC[y0 + i][x0 + j] != uni) uni = -1;
                if (!tgF[y0 + i][x0 + j]) continue;
                static const int SEAMW[TG_O] = {4, 3, 2, 1, 1, 1};
                ov[nov].i = (short)i; ov[nov].j = (short)j;
                ov[nov].w = (short)(first && j >= TG_NW ? SEAMW[j - TG_NW] : 1);
                nov++;
            }
            if (uni > 1) uni = -1;
            for (int a = 1; a < nov; a++)                 /* heaviest first (the sum stops early) */
                for (int b = a; b > 0 && ov[b].w > ov[b - 1].w; b--) { short t;
                    t = ov[b].i; ov[b].i = ov[b - 1].i; ov[b - 1].i = t; t = ov[b].j; ov[b].j = ov[b - 1].j; ov[b - 1].j = t;
                    t = ov[b].w; ov[b].w = ov[b - 1].w; ov[b - 1].w = t; }
            for (int q = 0; q < nov; q++) cbv[q] = tgC[y0 + ov[q].i][x0 + ov[q].j];
            if (y0 > 0) for (int i = 0; i < TG_B; i++) tov += tgF[y0 + i][x0];
            int sy0 = y0 - TG_DY - drop, sy1 = y0 + TG_DY;
            if (sy0 < 0) sy0 = 0;
            if (sy1 > H - TG_B) sy1 = H - TG_B;
            tg_integral(3, H);
            long long mn = -1, lim = 0;
            int nc = 0;
            /* (a first pass over every 4th candidate only finds a bound, so that the full pass can stop early) */
            for (int pass = 0; pass < 2; pass++)
            for (int sy = sy0; sy <= sy1; sy += pass ? 1 : 4) for (int sx = 0; sx < TG_NX; sx += pass ? 1 : 4, nc += pass) {
                long long c = (long long)tg_box(2, sy, sx) * 10000000 + (long long)tg_box(3, sy, sx) * TG_USEW;
                if (pass) tgCost[nc] = -1;
                if (mn >= 0 && c > lim) continue;
                int mism = 0, q = 0;
                if (uni >= 0) c += (long long)(TG_B * TG_B - tg_box(uni, sy, sx)) * 60000;
                if (mn >= 0 && c > lim) continue;
                {                                            /* (the sum fits 32 bits: 256 px * 4 * 9 * 248^2) */
                    long long room = mn >= 0 ? lim - c : 0x7FFFFFFF;
                    int acc = 0, rm = room > 0x7FFFFFFF ? 0x7FFFFFFF : (int)room;
                    for (; q < nov; q++) {
                        acc += tgD2[cbv[q]][tgA[sy + ov[q].i][sx + ov[q].j]] * ov[q].w;
                        if (acc > rm) break;
                    }
                    c += acc;
                }
                if (q < nov) continue;
                if (uni < 0) {                               /* mixed classes wanted: count them last */
                    for (int i = 0; i < TG_B; i++) for (int j = 0; j < TG_B; j++) mism += tgK[sy + i][sx + j] != tgKC[y0 + i][x0 + j];
                    c += (long long)mism * 60000;
                    if (mn >= 0 && c > lim) continue;
                }
                if (pass) tgCost[nc] = c;
                if (mn < 0 || c < mn) { mn = c; lim = mn + mn * TG_TOL / 100; }
            }
            int ok = 0, pick, sy, sx;
            for (int q = 0; q < nc; q++) ok += tgCost[q] >= 0 && tgCost[q] <= lim;
            {
                u32 h = ((u32)(M + TG_O - x1) * 73856093u) ^ ((u32)y0 * 19349663u) ^ ((u32)side * 83492791u);
                h = h * 1664525u + 1013904223u;
                pick = (int)((h >> 8) % (u32)ok);
            }
            for (sy = 0; sy < nc; sy++) if (tgCost[sy] >= 0 && tgCost[sy] <= lim && !pick--) break;
            sx = sy % TG_NX; sy = sy0 + sy / TG_NX;
            for (int i = 0; i < TG_B; i++) for (int j = 0; j < TG_B; j++) tgUse[sy + i][sx + j]++;

            /* minimum-error cuts: through the inner overlap (not at the picture) and the top one */
            static int E[TG_B][TG_B], dp[TG_B][TG_B];
            static u8 take[TG_B][TG_B], fm[TG_B][TG_B], old[TG_B][TG_B], oka[TG_B][TG_B];
            for (int i = 0; i < TG_B; i++) for (int j = 0; j < TG_B; j++) {
                fm[i][j] = tgF[y0 + i][x0 + j]; old[i][j] = tgC[y0 + i][x0 + j]; oka[i][j] = tgKA[y0 + i][x0 + j];
                E[i][j] = tgD2[old[i][j]][tgA[sy + i][sx + j]];
                take[i][j] = !fm[i][j];
            }
            if (!first) {
                for (int i = 0; i < TG_B; i++) for (int j = 0; j < TG_O; j++) {
                    int m = 0;
                    if (i) { m = dp[i - 1][j]; if (j && dp[i - 1][j - 1] < m) m = dp[i - 1][j - 1]; if (j + 1 < TG_O && dp[i - 1][j + 1] < m) m = dp[i - 1][j + 1]; }
                    dp[i][j] = E[i][TG_NW + j] + m;
                }
                int xx = 0;
                for (int j = 1; j < TG_O; j++) if (dp[TG_B - 1][j] < dp[TG_B - 1][xx]) xx = j;
                for (int i = TG_B - 1; i >= 0; i--) {
                    for (int j = 0; j < TG_B; j++) take[i][j] = j < TG_NW + xx;
                    if (i) {
                        int lo = xx > 0 ? xx - 1 : 0, hi = xx + 1 < TG_O ? xx + 1 : TG_O - 1, b = lo;
                        for (int j = lo + 1; j <= hi; j++) if (dp[i - 1][j] < dp[i - 1][b]) b = j;
                        xx = b;
                    }
                }
            }
            if (tov > 0) {
                for (int j = 0; j < TG_B; j++) for (int i = 0; i < tov; i++) {
                    int m = 0;
                    if (j) {
                        int lo = i > 0 ? i - 1 : 0, hi = i + 1 < tov ? i + 1 : tov - 1;
                        m = dp[lo][j - 1];
                        for (int q = lo + 1; q <= hi; q++) if (dp[q][j - 1] < m) m = dp[q][j - 1];
                    }
                    dp[i][j] = E[i][j] + m;
                }
                int yy = 0;
                for (int i = 1; i < tov; i++) if (dp[i][TG_B - 1] < dp[yy][TG_B - 1]) yy = i;
                for (int j = TG_B - 1; j >= 0; j--) {
                    for (int i = 0; i < yy; i++) take[i][j] = 0;
                    if (j) {
                        int lo = yy > 0 ? yy - 1 : 0, hi = yy + 1 < tov ? yy + 1 : tov - 1, b = lo;
                        for (int i = lo + 1; i <= hi; i++) if (dp[i][j - 1] < dp[b][j - 1]) b = i;
                        yy = b;
                    }
                }
            }
            for (int i = 0; i < TG_B; i++) for (int j = 0; j < TG_B; j++) if (!fm[i][j]) take[i][j] = 1;

            /* per-row tone difference at the inner overlap (same class on both sides) */
            static int dl[TG_B][2][3], dn[TG_B][2];
            memset(dl, 0, sizeof dl); memset(dn, 0, sizeof dn);
            for (int i = 0; i < TG_B; i++) for (int j = TG_NW; j < TG_B; j++) {
                int k = oka[i][j];
                if (k > 1 || tgK[sy + i][sx + j] != k) continue;
                for (int c = 0; c < 3; c++) dl[i][k][c] += tgP[old[i][j]][c] - tgP[tgA[sy + i][sx + j]][c];
                dn[i][k]++;
            }
            for (int i = 0; i < TG_B; i++) {
                int i0 = i - 2 < 0 ? 0 : i - 2, i1 = i + 3 > TG_B ? TG_B : i + 3;
                for (int j = 0; j < TG_B; j++) {
                    int k = tgK[sy + i][sx + j], s = tgA[sy + i][sx + j], c[3];
                    u8 *out = &tgC[y0 + i][x0 + j], *ka = &tgKA[y0 + i][x0 + j];
                    if (fm[i][j]) {
                        if (j >= TG_NW && first) continue;                  /* the picture itself */
                        if (k == 0 && oka[i][j] == 0) {                     /* sky over sky: feather */
                            int w = TG_O + 1;
                            if (j >= TG_NW && TG_B - j < w) w = TG_B - j;
                            if (i < tov && i + 1 < w) w = i + 1;
                            for (int q = 0; q < 3; q++) c[q] = (tgP[old[i][j]][q] * (TG_O + 1 - w) + tgP[s][q] * w + (TG_O + 1) / 2) / (TG_O + 1);
                            *out = (u8)tg_near(c, 0);
                        } else if (take[i][j]) { *out = (u8)s; *ka = (u8)k; }
                        continue;
                    }
                    *ka = (u8)k; *out = (u8)s;
                    if (k > 1 || tgKC[y0 + i][x0 + j] != k) continue;
                    if (k == 1 && !first) continue;
                    int n = 0, dsum[3] = {0, 0, 0}, t = TG_NW - 1 - j;
                    for (int q = i0; q < i1; q++) { n += dn[q][k]; for (int e = 0; e < 3; e++) dsum[e] += dl[q][k][e]; }
                    if (!n) continue;
                    for (int q = 0; q < 3; q++) {
                        int low = tg_fdiv(dsum[q], n);
                        c[q] = tgP[s][q] + tg_fdiv(dsum[q] * (j + 1), n * (TG_NW + 1));
                        if (first && t < TG_D && oka[i][TG_NW] == k && tgK[sy + i][sx + TG_NW] == k)
                            c[q] += tg_fdiv((tgP[old[i][TG_NW]][q] - tgP[tgA[sy + i][sx + TG_NW]][q] - low) * (TG_D - t), TG_D + 1);
                    }
                    *out = (u8)tg_near(c, k);
                }
            }
            for (int i = 0; i < TG_B; i++) for (int j = 0; j < TG_B; j++) tgF[y0 + i][x0 + j] = 1;
            if (y0 + TG_B >= H) break;
        }
    }
    for (int y = 0; y < H; y++) for (int o = 1; o <= M; o++) tgG[side][y][o - 1] = tgC[y][M - o];
}

/* TG_MARGIN: the picture with `need` more columns on each side (stride pw + 2 * need), or 0.  The margins are
   built once per picture (and again, from the first column on, when a wider frame asks for more). */
static const u8 *tg_extend(const u8 *tim, const u8 *pic, int stride, int pw, int ph, int need)
{
    if (!PORT_LAZY_OK()) return 0;          /* (the 3DS: the tables below are not in the program, runtime/lazy_bss.h) */
    static const u8 *k_tim; static u32 k_hash; static int k_pw, k_ph, k_nb, k_need;
    if (need < 2 || need > TG_MAXM || (need & 1) || pw < TG_SB * 2 || pw > TG_MAXW || ph < 2 * TG_B || ph > TG_MAXH) return 0;
    u32 h = 2166136261u;
    for (int i = 0x14; i < 0x214; i++) h = (h ^ tim[i]) * 16777619u;
    for (int y = 0; y < ph; y++) {
        const u8 *r = pic + (size_t)y * stride;
        for (int x = 0; x < TG_SB; x++) h = (h ^ r[x]) * 16777619u;
        for (int x = pw - TG_SB; x < pw; x++) h = (h ^ r[x]) * 16777619u;
    }
    int nb = (need + TG_NW - 1) / TG_NW, same = k_tim == tim && k_hash == h && k_pw == pw && k_ph == ph;
    if (!same || k_nb < nb) {
        for (int i = 0; i < 256; i++) {
            int c = rd16(tim + 0x14 + i * 2);
            tgP[i][0] = (c & 31) << 3; tgP[i][1] = ((c >> 5) & 31) << 3; tgP[i][2] = ((c >> 10) & 31) << 3;
        }
        for (int a = 0; a < 256; a++) for (int b = 0; b < 256; b++) {
            int r = tgP[a][0] - tgP[b][0], g = tgP[a][1] - tgP[b][1], bb = tgP[a][2] - tgP[b][2];
            tgD2[a][b] = 3 * r * r + 4 * g * g + 2 * bb * bb;
        }
        tg_side(pic, stride, pw, ph, 0, nb);
        tg_side(pic, stride, pw, ph, 1, nb);
        k_tim = tim; k_hash = h; k_pw = pw; k_ph = ph; k_nb = nb; k_need = 0;
        port_printf("[ws] top-down map: margins of %d columns generated for the %d x %d picture\n", nb * TG_NW, pw, ph);
    }
    if (k_need != need) {
        int ew = pw + 2 * need;
        for (int y = 0; y < ph; y++) {
            u8 *e = tgE + (size_t)y * ew;
            for (int o = 1; o <= need; o++) { e[need - o] = tgG[0][y][o - 1]; e[need + pw - 1 + o] = tgG[1][y][o - 1]; }
            memcpy(e + need, pic + (size_t)y * stride, (size_t)pw);
        }
        k_need = need;
    }
    return tgE;
}
