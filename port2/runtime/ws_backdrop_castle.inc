/* runtime/ws_backdrop.c: AREA17 s1, the Iron Castle's entrance (included there; docs/PORT.md "More rooms with
 * picture strips").
 *
 * A 320 x 384 picture, CLUT (128, 511): rows 0-255 from pages (128, 0) (256 columns) and (256, 0) (64 more), rows
 * 256-383 from page (256, 0) u 64.. (192 columns from v 0, 128 from v 128); four sprites, the first at frame row
 * -144, so the frame shows the picture's rows 144-383.  ic_take() makes the strips for all 384 rows once and hands
 * the 224 the frame shows (port_ws_backdrop_dy), wherever the game puts the picture.
 *
 * Measured on the picture (its own rows), at the edges of the part the frame shows:
 *   - sky: a flat blue, clouds low over the horizon (at the left edge rows 205-262, at the right one 190-254);
 *   - left: a thin far chain leaves on rows 232-247, coming down 1 row per column (its links repeat every (12, 11));
 *     the big chain leaves on rows 276-320, coming down 2 rows per column (links every (17, 34)); its shadow lies on
 *     the grass at columns 0-20, row 350, so it meets the ground some 25 columns out; behind it the castle's copper
 *     skirt, its top on row 264 at the edge and falling 0.9 rows per column; grass from row 346;
 *   - right: the skirt's top on row 254, falling 0.75 rows per column; the big near chain leaves on rows 318-352
 *     (links every (31, 34)) and goes out of the frame's bottom; grass from row 353.
 *
 * The strips (every generated texel is one of the picture's own colours: the nearest of its CLUT after a little
 * dither by place, so ramps do not band):
 *   - sky: the picture's own sky texels of that side and row (its 16 columns next to the edge, the row's flat blue
 *     and the dither where two bands meet); under the clouds, the last clean row's;
 *   - clouds: one bank that carries on from the edge's own (its top there, the edge column's whiteness fading into
 *     the bank over 14 columns), the top wandering with smooth noise, a lumpy soft outline, gentle shading, down to
 *     the horizon; only the cloud colours the picture has next to both edges and the row's sky;
 *   - the skirt down to the grass on a curve (its top anti-aliased), toned top to bottom as the edge's own copper at
 *     the join and as the lit copper of the 60 columns next to it further out (never darker outwards), as lobes:
 *     creases that lean towards the dome's top, a shadow after each, a highlight before the next, mottling, grain,
 *     rivets beside the creases; the colours from the copper ramp of that side of the picture;
 *   - grass: the row's own green with grain, its tone at the edge fading out over 30 columns;
 *   - the chains: each texel is the chain's own texel one or more link periods back (never mirrored), from a mask
 *     with its holes filled and its specks dropped; all three go on to the frame's edge (the left big one in front of
 *     the grass and out of the bottom, as the right one does in the picture, over a soft shadow on the grass).
 * Third version (2026-11-17, after the user's "go back to the 4:3 picture, you are not seeing what has to be done"):
 * the skirt is the dome it is on the picture, an ellipse about the castle's axis that ends upright on the grass 60
 * columns out (IC_TOP16), darker towards its outline, not a ramp out to the frame's edge; the clouds are puffs with sky
 * between them, not a bank down to the ground; the edge's own cloud ends raggedly.
 * Integers only; a texel depends on its place, not on the strip's width. */
#define IC_HM 1229                                        /* the ROOMS row's mark (no picture has this height) */
#define IC_H 384
static void gen_ic(int pad) { (void)pad; }               /* (the table's gen: ic_take makes the strips) */
static u16 ic_p[IC_H][PW];
static u8 ic_k[IC_H][PW];                                 /* 1 grass, 2 copper, 4 sky, 8 chain / grey, 16 dark chain */
static int ic_l[IC_H][PW];
static int ic_grow[IC_H][3], ic_g0, ic_g1, ic_cr[2][256][3];
static const int IC_GT[2] = { 346, 353 }, IC_T0[2] = { 264, 254 };

static int ic_icmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }
static void ic_measure(void)
{
    static int sum[256][3], cnt[256], tmp[256][3], srt[PW];
    for (int y = 0; y < IC_H; y++)
        for (int x = 0; x < PW; x++) {
            Rgb c = rgb15(ic_p[y][x]); int l = lum(c), k = 0;
            if (c.g > c.r + 30 && c.g > c.b + 30) k = 1;
            else if (c.r > c.g + 12 && c.g >= c.b) k = 2;
            else if (c.b - c.r >= 45) k = 4;
            else k = 8 | ((l < 150 && c.b - c.r < 30) ? 16 : 0);
            ic_k[y][x] = (u8)k; ic_l[y][x] = l;
        }
    for (int side = 0; side < 2; side++) {                /* the copper's ramp next to each edge (its 100 columns): the mean
                                                             of the 300 texels nearest in tone */
        memset(sum, 0, sizeof sum); memset(cnt, 0, sizeof cnt);
        for (int y = 0; y < IC_H; y++)
            for (int x = side ? PW - 100 : 0; x < (side ? PW : 100); x++)
                if (ic_k[y][x] == 2) { Rgb c = rgb15(ic_p[y][x]); int l = ic_l[y][x]; sum[l][0] += c.r; sum[l][1] += c.g; sum[l][2] += c.b; cnt[l]++; }
        for (int L = 0; L < 256; L++) {
            int a = L, b = L, n = cnt[L], s[3] = { sum[L][0], sum[L][1], sum[L][2] };
            while (n < 300 && (a > 0 || b < 255)) {
                if (a > 0) { a--; n += cnt[a]; for (int k = 0; k < 3; k++) s[k] += sum[a][k]; }
                if (n < 300 && b < 255) { b++; n += cnt[b]; for (int k = 0; k < 3; k++) s[k] += sum[b][k]; }
            }
            for (int k = 0; k < 3; k++) tmp[L][k] = n ? s[k] / n : 0;
        }
        for (int L = 0; L < 256; L++) for (int k = 0; k < 3; k++) {
            int s = 0, n = 0;
            for (int d = -3; d <= 3; d++) if (L + d >= 0 && L + d < 256) { s += tmp[L + d][k]; n++; }
            ic_cr[side][L][k] = s / n;
        }
    }
    ic_g0 = -1; ic_g1 = -1;
    for (int y = 0; y < IC_H; y++) {                      /* the grass' green per row, without the shadows on it */
        int n = 0, m, s[3] = { 0, 0, 0 }, q = 0;
        for (int x = 0; x < PW; x++) if (ic_k[y][x] == 1) srt[n++] = ic_l[y][x];
        if (n <= 60) continue;
        qsort(srt, (size_t)n, sizeof(int), ic_icmp); m = srt[n / 2] - 6;
        for (int x = 0; x < PW; x++) if (ic_k[y][x] == 1 && ic_l[y][x] >= m) { Rgb c = rgb15(ic_p[y][x]); s[0] += c.r; s[1] += c.g; s[2] += c.b; q++; }
        for (int k = 0; k < 3; k++) ic_grow[y][k] = s[k] / q;
        if (ic_g0 < 0) ic_g0 = y;
        ic_g1 = y;
    }
}

/* nearest colour among those the picture uses (its CLUT), cached per 15-bit colour; a little dither by place first */
static u16 ic_pal[256], ic_qc[32768]; static u8 ic_qh[32768]; static int ic_np;
static void ic_pal_make(void)
{
    static u8 seen[32768];
    memset(seen, 0, sizeof seen); memset(ic_qh, 0, sizeof ic_qh); ic_np = 0;
    for (int y = 0; y < IC_H; y++) for (int x = 0; x < PW; x++) {
        u16 c = ic_p[y][x] & 0x7FFF;
        if (c && !seen[c] && ic_np < 256) { seen[c] = 1; ic_pal[ic_np++] = c; }
    }
}
static u16 ic_q(const int *p, int j, int y, int side)
{
    int d = (int)(hash3(j + side * 4000, y, 77) % 9) - 4;
    int r = clampi(p[0] + d + 4, 0, 255) >> 3, g = clampi(p[1] + d + 4, 0, 255) >> 3, b = clampi(p[2] + d + 4, 0, 255) >> 3, k = r | g << 5 | b << 10;
    if (!ic_qh[k]) {
        int bd = 1 << 30, best = 0;
        for (int i = 0; i < ic_np; i++) {
            int dr = r5(ic_pal[i]) - r, dg = g5(ic_pal[i]) - g, db = b5(ic_pal[i]) - b, e = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (e < bd) { bd = e; best = i; }
        }
        ic_qc[k] = ic_np ? ic_pal[best] : (u16)k; ic_qh[k] = 1;
    }
    return ic_qc[k];
}

/* v[0..n-1] with -1 where unknown: bridged, then smoothed over +-sm */
static void ic_bridge(int *v, int n, int sm, int def)
{
    static int t[IC_H]; int last = -1;
    for (int i = 0; i < n; i++) {
        if (v[i] < 0) continue;
        if (last < 0) for (int k = 0; k < i; k++) v[k] = v[i];
        else for (int k = last + 1; k < i; k++) v[k] = v[last] + (v[i] - v[last]) * (k - last) / (i - last);
        last = i;
    }
    for (int k = last + 1; k < n; k++) v[k] = last >= 0 ? v[last] : def;
    for (int i = 0; i < n; i++) {
        int s = 0;
        for (int d = -sm; d <= sm; d++) s += v[clampi(i + d, 0, n - 1)];
        t[i] = s / (2 * sm + 1);
    }
    for (int i = 0; i < n; i++) v[i] = t[i];
}
/* the skirt's tone from its top to the grass: pe as the edge's own copper (the chain in front of it bridged), pw as
   the mean copper of the 60 columns next to the edge */
static void ic_prof(int side, int *pe, int *pw, int n)
{
    for (int i = 0; i < n; i++) {
        int s = 0, c = 0, y = IC_T0[side] + i;
        for (int x = 0; x < 4; x++) { int px = side ? PW - 4 + x : x; if (ic_k[y][px] == 2) { s += ic_l[y][px]; c++; } }
        pe[i] = c >= 3 ? s / c : -1;
        s = 0; c = 0;
        for (int x = 0; x < 60; x++) { int px = side ? PW - 60 + x : x; if (ic_k[y][px] == 2) { s += ic_l[y][px]; c++; } }
        if (c >= 12) {
            int m = s / c; s = 0; c = 0;
            for (int x = 0; x < 60; x++) { int px = side ? PW - 60 + x : x; if (ic_k[y][px] == 2 && ic_l[y][px] >= m) { s += ic_l[y][px]; c++; } }
        }
        pw[i] = c >= 6 ? s / c : -1;
    }
    ic_bridge(pe, n, 2, 120); ic_bridge(pw, n, 5, 120);
}

/* the chains' texels: ic_cm bit which + 1 at the picture's texels of that chain (holes filled, specks dropped) */
static u8 ic_cm[IC_H][PW];
static int ic_chain0(int side, int which, int x, int y)
{
    if (x < 0 || x >= PW || y < 0 || y >= IC_H) return 0;
    if (!side && which == 0) return x < 14 && abs(y - (240 - x)) <= 9 && (ic_k[y][x] & 8) && ic_l[y][x] < 200 && !(ic_l[y][x] > 150 && rgb15(ic_p[y][x]).b - rgb15(ic_p[y][x]).r >= 16);
    if (!side) {
        Rgb c = rgb15(ic_p[y][x]);
        return x < 19 && abs(y - (298 - 2 * x)) <= 27 && (ic_k[y][x] & 8) && !(ic_l[y][x] > 150 && c.b - c.r >= 16 && y < 270);
    }
    return x >= PW - 33 && abs((y - 335) - fdiv((x - 319) * 34, 31)) <= 24 && (ic_k[y][x] & 8);
}
static void ic_chains(void)
{
    static u8 m[IC_H][PW];
    memset(ic_cm, 0, sizeof ic_cm);
    for (int side = 0; side < 2; side++)
        for (int which = 0; which < (side ? 1 : 2); which++) {
            int x0 = side ? PW - 34 : 0, x1 = side ? PW : 20;
            for (int y = 0; y < IC_H; y++) for (int x = x0; x < x1; x++) m[y][x] = (u8)ic_chain0(side, which, x, y);
            for (int y = 1; y < IC_H - 1; y++)
                for (int x = x0; x < x1; x++) {
                    int n = 0, in;
                    for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++)
                        if ((dx || dy) && x + dx >= x0 && x + dx < x1) n += m[y + dy][x + dx];
                    in = m[y][x] ? n >= 3 : n >= 6 && !(ic_k[y][x] & 5);
                    if (in) ic_cm[y][x] |= (u8)(1 << which);
                }
        }
}
static int ic_chain(int side, int which, int x, int y)   /* the picture's texel (x, y) is that chain's */
{
    (void)side;
    return x >= 0 && x < PW && y >= 0 && y < IC_H && (ic_cm[y][x] >> which & 1);
}

/* The skirt is a dome: on the picture its outline is an ellipse about the castle's axis (column 160), half axis 220
   columns (fitted on the right outline: rows 82 / 86 / 92 / 101 of the frame's picture rows at columns 282 / 295 /
   307 / 320), standing on the grass.  So it does not run out as a ramp: it comes down ever steeper and ends upright
   on the ground 60 columns past the edge.  Its top (x 16) j columns out, through the edge's own row; IC_END from
   where there is none. */
#define IC_AX 220
#define IC_END (IC_AX - 160)
static int ic_isqrt(int v) { int r = 0; while ((r + 1) * (r + 1) <= v) r++; return r; }
static int ic_rel(int j) { return j >= IC_END ? 0 : ic_isqrt((IC_AX * IC_AX - (160 + j) * (160 + j)) * 16) * 256 / ic_isqrt((IC_AX * IC_AX - 160 * 160) * 16); }   /* the dome's height j columns out over its height at the edge, x 256 */
static int ic_top16(int side, int j) { int b = IC_GT[side] + 2; return b * 16 - (b - IC_T0[side]) * ic_rel(j) / 16; }
#define IC_TOP16(side, j) ic_top16(side, j)
static void ic_gen(u16 *ext, int pad)
{
    static const int VEC[2][2][2] = { { { 12, 11 }, { 17, 34 } }, { { 31, 34 }, { 31, 34 } } };
    static const int LW[2] = { 26, 34 }, LO[2] = { -12, -10 };   /* the skirt's lobes: width and first crease on the ground */
    static u16 skc[IC_H], cp[48]; static int ew[IC_H], cc[32768]; int ncp = 0;
    int (*cv)[4] = malloc(sizeof(int[4]) * (size_t)IC_H * (size_t)pad), c1[3] = { 200, 208, 240 }, c2[3] = { 224, 232, 240 }, l1, l2;
    if (!cv) return;
    ic_pal_make(); ic_chains();
    memset(cc, 0, sizeof cc);
    {   long long s[3] = { 0, 0, 0 }, sl = 0; int n = 0, lm;   /* the clouds' colours next to both edges: all, the lighter half */
        for (int pass = 0; pass < 2; pass++) {
            lm = n ? (int)(sl / n) : 0;
            if (pass) { if (n >= 50) for (int k = 0; k < 3; k++) c1[k] = (int)(s[k] / n); s[0] = s[1] = s[2] = 0; n = 0; }
            for (int y = 190; y < 262; y++)
                for (int x = 0; x < 128; x++) {
                    int px = x < 64 ? x : PW - 128 + x; Rgb c = rgb15(ic_p[y][px]);
                    if (!(ic_k[y][px] & 12) || c.b - c.r < 8 || c.b - c.r > 72 || ic_l[y][px] < 170 || c.b < 200) continue;
                    if (!pass && ++cc[ic_p[y][px] & 0x7FFF] == 25 && ncp < 47) cp[ncp++] = ic_p[y][px] & 0x7FFF;
                    if (pass && ic_l[y][px] <= lm) continue;
                    s[0] += c.r; s[1] += c.g; s[2] += c.b; sl += ic_l[y][px]; n++;
                }
        }
        if (n >= 20) for (int k = 0; k < 3; k++) c2[k] = (int)(s[k] / n);
        l1 = (c1[0] * 3 + c1[1] * 6 + c1[2]) / 10; l2 = (c2[0] * 3 + c2[1] * 6 + c2[2]) / 10;
        if (l2 < l1 + 4) l2 = l1 + 4;
    }
    for (int side = 0; side < 2; side++) {
        int ex = side ? PW - 1 : 0, gt = IC_GT[side], t0 = IC_T0[side], n0 = gt - t0, pe[128], pw[128], te = side ? 207 : 203, last = -1;
        for (int y = 0; y < IC_H; y++) {                  /* the sky's own colour next to this edge, per row */
            u16 best = 0; int bn = 0, n = 0;
            for (int x = 0; x < 16; x++) {
                int px = side ? PW - 1 - x : x, q = 0; Rgb c = rgb15(ic_p[y][px]);
                if (ic_k[y][px] != 4 || c.b - c.r < 80) continue;
                n++;
                for (int z = 0; z < 16; z++) q += ic_p[y][side ? PW - 1 - z : z] == ic_p[y][px];
                if (q > bn) { bn = q; best = ic_p[y][px]; }
            }
            if (n >= 12) { skc[y] = best; last = y; ew[y] = 1; }
            else { skc[y] = last >= 0 ? skc[last] : 0; ew[y] = 0; }
        }
        for (int y = IC_H - 1, nx = -1; y >= 0; y--) { if (ew[y]) nx = y; else if (!skc[y]) skc[y] = nx >= 0 ? skc[nx] : 0x7210; }
        for (int y = 0; y < IC_H; y++)
            for (int j = 0; j < pad; j++) {
                int px = side ? PW - 1 - (int)(hash3(j, y, 5 + side) & 15) : (int)(hash3(j, y, 5 + side) & 15), *p = cv[(size_t)y * pad + j];
                u16 c = ic_p[y][px];                     /* (the rows' own dither where two bands meet, nothing else) */
                p[3] = 1 + ((ew[y] && (c == skc[y > 0 ? y - 1 : y] || c == skc[y < IC_H - 1 ? y + 1 : y]) ? c : skc[y]) & 0x7FFF);
            }
        for (int y = 150; y < 250; y++) {                 /* where the edge's cloud bank starts */
            int ok = 1;
            for (int d = 0; d < 4 && ok; d++) ok = (ic_k[y + d][ex] & 12) && ic_l[y + d][ex] > lum(rgb15(skc[y + d])) + 22;
            if (ok) { te = y; break; }
        }
        for (int y = 0; y < IC_H; y++) {                  /* the edge's own whiteness (0 sky, 256 cloud, more: its lights) */
            Rgb c = rgb15(ic_p[y][ex]); int l = ic_l[y][ex], sl = lum(rgb15(skc[y])), k = ic_k[y][ex];
            if (ic_cm[y][ex] || !(k & 12) || (k & 16) || c.b - c.r < 8) { ew[y] = -1; continue; }
            ew[y] = l <= l1 ? clampi((l - sl) * 256 / (l1 - sl > 1 ? l1 - sl : 1), 0, 256) : clampi(256 + (l - l1) * 128 / (l2 - l1), 256, 512);
        }
        for (int y = t0; y < IC_H; y++) ew[y] = -1;
        ic_bridge(ew, IC_H, 1, 256);
        for (int y = te - 60; y < gt + 2; y++)            /* the cloud bank */
            for (int j = 0; j < pad; j++) {
                int X = j + side * 700, f = j < 28 ? j : 28, *p = cv[(size_t)y * pad + j], top, h, wa, wi, w;
                Rgb sk = rgb15((u16)(p[3] - 1));
                top = te + ((noise(X, 0, 74, 1, 3) - 128) * 24 / 128 + (noise(X, 0, 25, 1, 4) - 128) * 10 / 128 + 4) * f / 28;
                if (j > 60) top += ((noise(X, 0, 150, 1, 9) - 128) * 56 / 128 + 12) * (j < 140 ? j - 60 : 80) / 80;
                h = (y - top) * 16 + (noise(X, y, 17, 11, 5) - 128) * 150 / 128 + (noise(X, y, 7, 5, 6) - 128) * 40 / 128;
                wa = clampi(h * 3, 0, 256);
                {   int g = clampi((noise(X, y, 70, 34, 11) - 96) * 3 + (noise(X, y, 22, 12, 14) - 128) / 2, 0, 256);   /* puffs with sky between them, as the picture's */
                    int L = 12 + noise(side * 91 + 7, y, 1, 17, 12) * 40 / 256 + noise(side * 91 + 7, y, 1, 6, 13) * 10 / 256;   /* (the edge's own cloud ends raggedly) */
                    wa = wa * (j < L ? (256 * (L - j) + g * j) / L : g) / 256; }
                wi = 262 + (noise(X, y, 40, 20, 7) - 128) * 76 / 128;
                wi += (noise(X, y, 13, 9, 8) - 128) * 26 / 128;
                wi += clampi(100 - h / 4, 0, 60) - clampi(y - (gt - 22), 0, 22) * 5;
                w = clampi(wi, 60, 480) * wa / 256;
                if (j < 14) w = (ew[y] * (14 - j) + w * j) / 14;
                if (w < 36) continue;
                p[3] = -1;
                if (w <= 256) { p[0] = sk.r + (c1[0] - sk.r) * w / 256; p[1] = sk.g + (c1[1] - sk.g) * w / 256; p[2] = sk.b + (c1[2] - sk.b) * w / 256; }
                else for (int k = 0; k < 3; k++) p[k] = clampi(c1[k] + (c2[k] - c1[k]) * (w - 256) / 128, 0, 255);
            }
        ic_prof(side, pe, pw, n0);
        for (int j = 0; j < pad; j++) {                   /* the copper skirt */
            int top16 = IC_TOP16(side, j);
            int top = top16 >> 4, hg = gt + 1 - top, D = 161 + j, f = j < 10 ? j : 10, fb = j < 12 ? j : 12;
            if (j >= IC_END || top >= gt) break;
            for (int y = top; y < gt + 3; y++) {
                int v = clampi((y - top) * (n0 - 1) / (hg > 1 ? hg : 1), 0, n0 - 1), *p = cv[(size_t)y * pad + j];
                int q = D * 576 / (y + 230) - 160 - LO[side], li = fdiv(q, LW[side]), t = 255 - fmodi(q, LW[side]) * 256 / LW[side];
                int m = t < 14 ? -170 : t < 60 ? -170 + (t - 14) * 130 / 46 : t < 210 ? -40 + (t - 60) * 70 / 150 : t < 242 ? 30 + (t - 210) * 40 / 32 : 70 - (t - 242) * 14;
                int tone = (pe[v] * (12 - fb) + pw[v] * fb) / 12, g, a = y == top ? 16 - (top16 & 15) : 16;
                m += (int)(hash3(li, side, 31) % 25) - 12;
                m += (noise(j + side * 333, y, 13, 21, 22) - 128) * 26 / 128;
                tone = tone * (256 + m * f / 10) / 256;
                tone = tone * (150 + 106 * ic_rel(j) / 256) / 256;   /* (the dome turns away: darker towards its outline) */
                g = (noise(j + side * 333, y, 3, 3, 21) - 128) * 7 / 128;
                tone += g + (int)(hash3(j + side * 333, y, 23) % 11) - 5;
                if (y >= gt) tone = tone * 5 / 8;
                tone = clampi(tone, 0, 255);
                if (p[3] > 0) { Rgb c = rgb15((u16)(p[3] - 1)); p[0] = c.r; p[1] = c.g; p[2] = c.b; }
                p[3] = 0;
                for (int k = 0; k < 3; k++) p[k] += (ic_cr[side][tone][k] - p[k]) * a / 16;
            }
        }
        for (int li = 0; li * LW[side] < pad + 200; li++)   /* rivets beside the creases */
            for (int n = 0; n < 7; n++) {
                int y = gt - 12 - n * 26 - (int)(hash3(li, n, 41 + side) % 9), j = (LO[side] + li * LW[side] + 6 + 160) * (y + 230) / 576 - 161;
                if (j < 3 || j >= pad - 1 || y < (IC_TOP16(side, j + 2) >> 4) + 5) continue;
                for (int dy = -1; dy <= 1; dy++)
                    for (int dj = -1; dj <= 1; dj++) {
                        int *p = cv[(size_t)(y + dy) * pad + j + dj], e = (side ? dj : -dj) + dy;
                        const int *c = ic_cr[side][e < 0 ? 235 : 165];
                        if (e > 0) for (int k = 0; k < 3; k++) p[k] = p[k] * (dy && dj ? 180 : 130) / 256;
                        else for (int k = 0; k < 3; k++) p[k] += (c[k] - p[k]) * (e < 0 ? 200 : 110) / 256;
                    }
            }
        if (ic_g0 >= 0)
            for (int y = gt - 3; y < IC_H; y++) {         /* grass */
                int yy = clampi(y, ic_g0 + 4, ic_g1), ge = 0, gn = 0, gl;
                gl = (ic_grow[yy][0] * 3 + ic_grow[yy][1] * 6 + ic_grow[yy][2]) / 10;
                for (int x = 0; x < 3; x++) { int px = side ? PW - 1 - x : x; if (ic_k[y][px] == 1) { ge += ic_l[y][px]; gn++; } }
                for (int j = 0; j < pad; j++) {
                    int *p = cv[(size_t)y * pad + j], g = (noise(j + side * 900, y, 3, 2, 9) - 128) * 14 / 128, f = 256;
                    if (y < gt + (noise(j + side * 50, 0, 5, 1, 12) - 128) / 50) continue;
                    if (gn && gl > 0 && j < 30) f = 256 + (ge / gn - gl) * 256 / gl * (30 - j) / 30;
                    for (int k = 0; k < 3; k++) p[k] = clampi((ic_grow[yy][k] + g) * f / 256, 0, 255);
                    p[3] = 0;
                }
            }
        if (!side)                                        /* the big chain's shadow where it goes into the grass */
            for (int y = 347; y < 364; y++)
                for (int j = 4; j < pad && j < 56; j++) {
                    int dx = (j - 26 - (y - 355)) * 16 / 24, dy = (y - 355) * 16 / 8, d = dx * dx + dy * dy, *p = cv[(size_t)y * pad + j];
                    if (d >= 256 || p[3]) continue;
                    for (int k = 0; k < 3; k++) p[k] = p[k] * (256 - (256 - d) * 150 / 256) / 256;
                }
        for (int which = 0; which < (side ? 1 : 2); which++) {   /* the chains: their own texels, link periods back */
            int dx = VEC[side][which][0], dy = VEC[side][which][1];
            for (int j = 0; j < pad; j++) {
                int x = side ? PW + j : -1 - j;
                for (int y = 0; y < IC_H; y++)
                    for (int k = 1; k < 30; k++) {
                        int xs = side ? x - dx * k : x + dx * k, ys = y - dy * k, *p = cv[(size_t)y * pad + j];
                        if (xs < 0 || xs >= PW) continue;
                        if (!ic_chain(side, which, xs, ys)) break;
                        p[3] = 1 + (ic_p[ys][xs] & 0x7FFF);
                        break;
                    }
            }
        }
        for (int y = 0; y < IC_H; y++)
            for (int j = 0; j < pad; j++) {
                const int *p = cv[(size_t)y * pad + j];
                u16 o = p[3] > 0 ? (u16)(p[3] - 1) : ic_q(p, j, y, side);
                if (p[3] < 0) {                           /* a cloud: the nearest of the picture's cloud colours and the row's sky */
                    int d = (int)(hash3(j + side * 4000, y, 78) % 7) - 3, bd = 1 << 30;
                    cp[ncp] = skc[y] & 0x7FFF;
                    for (int i = 0; i <= ncp; i++) {
                        Rgb c = rgb15(cp[i]); int dr = c.r - p[0] - d, dg = c.g - p[1] - d, db = c.b - p[2] - d, e = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
                        if (e < bd) { bd = e; o = cp[i]; }
                    }
                }
                ext[(size_t)y * 2 * pad + (side ? pad + j : pad - 1 - j)] = o;
            }
    }
    free(cv);
}

#ifndef WS_BACKDROP_MAIN
static u16 *ic_ext; static int ic_pad = -1; static u32 ic_sum;
static u16 ic_tex(const u16 *vram, int x, int y)
{
    if (y < 256) return x < 256 ? tex8(vram, 128, x, y, 128, 511) : tex8(vram, 256, x - 256, y, 128, 511);
    return x < 192 ? tex8(vram, 256, 64 + x, y - 256, 128, 511) : tex8(vram, 256, 64 + x - 192, 128 + y - 256, 128, 511);
}
static u32 ic_check(const u16 *vram)                      /* the pages and the CLUT, sampled */
{
    u32 h = 2166136261u;
    for (int i = 0; i < 256; i++) h = (h ^ vram[(size_t)511 * 1024 + 128 + i]) * 16777619u;
    for (int y = 3; y < 256; y += 8) for (int x = 128; x < 384; x++) h = (h ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return h;
}
/* the picture's first sprite at frame row y: both strips for the frame's rows 16..239 */
static const u16 *ic_take(const u16 *vram, int pad, int y, int *rows)
{
    u32 s = ic_check(vram); int r0 = 16 - y;
    if (!ic_ext || ic_pad != pad || s != ic_sum) {
        ic_pad = pad; ic_sum = s;
        free(ic_ext); ic_ext = calloc((size_t)IC_H * 2 * pad, 2);
        if (!ic_ext) return 0;
        for (int yy = 0; yy < IC_H; yy++) for (int x = 0; x < PW; x++) ic_p[yy][x] = ic_tex(vram, x, yy);
        ic_measure(); ic_gen(ic_ext, pad);
    }
    if (!ic_ext || r0 < 0 || r0 + PH > IC_H) return 0;
    port_ws_backdrop_dy = r0; port_ws_backdrop_side = 3;
    *rows = PH;
    return ic_ext + (size_t)r0 * 2 * pad;
}
#endif
