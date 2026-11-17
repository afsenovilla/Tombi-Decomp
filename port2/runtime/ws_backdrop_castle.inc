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
 * The strips:
 *   - sky: the row's own blue, clouds as two octaves of noise low over the horizon; the edge's own cloud fades into
 *     them over 40 columns;
 *   - grass: the row's own green with grain, its tone at the edge fading out over 30 columns;
 *   - the skirt down to the grass on a curve, shaded top to bottom as the edge's own copper is (the picture's copper
 *     colour ramp), darker outwards, with grain;
 *   - the chains: each texel is the chain's own texel one or more link periods back (never mirrored); the left big
 *     one stops on the grass over a shadow, the thin one and the right one go on to the frame's edge.
 * 2026-11-17, from the user's marks on a device capture: the skirt's tone goes by the distance from the front
 * plate's upper edge carried on (IC_PL: the plate, its edge and the plate behind it cross the join; it was one ramp
 * top to bottom, a jump at the join); the thin chain's copies are filled (only its dark texels were copied: hollow
 * links); where the edge is plain sky the strip has the edge's own blue and no cloud haze (a lighter block).  Two
 * later versions (a dome's outline, a cloud bank / puffs, lobes) were rejected: "you were doing much better here".
 * Integers only; a texel depends on its place, not on the strip's width. */
#define IC_HM 1229                                        /* the ROOMS row's mark (no picture has this height) */
#define IC_H 384
static void gen_ic(int pad) { (void)pad; }               /* (the table's gen: ic_take makes the strips) */
static u16 ic_p[IC_H][PW];
static u8 ic_k[IC_H][PW];                                 /* 1 grass, 2 copper, 4 sky, 8 chain / grey, 16 dark chain */
static int ic_l[IC_H][PW];
static int ic_sky[IC_H][3], ic_grow[IC_H][3], ic_g0, ic_g1, ic_cr[256][3];
static const int IC_GT[2] = { 346, 353 }, IC_T0[2] = { 264, 254 };
#define IC_LW 15                                          /* the plates' edges: columns apart */
static const int IC_LO[2] = { 12, 1 };                    /* the first edge's foot in each strip (column) */
static const int IC_PL[2] = { 289, 279 };                 /* the front plate's upper edge at the picture's edges (rows) */

static int ic_icmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }
static void ic_measure(void)
{
    static int sum[256][3], cnt[256], tmp[256][3], srt[PW], ok[IC_H];
    memset(sum, 0, sizeof sum); memset(cnt, 0, sizeof cnt);
    for (int y = 0; y < IC_H; y++)
        for (int x = 0; x < PW; x++) {
            Rgb c = rgb15(ic_p[y][x]); int l = lum(c), k = 0;
            if (c.g > c.r + 30 && c.g > c.b + 30) k = 1;
            else if (c.r > c.g + 12 && c.g >= c.b) k = 2;
            else if (c.b - c.r >= 45) k = 4;
            else k = 8 | ((l < 150 && c.b - c.r < 30) ? 16 : 0);
            ic_k[y][x] = (u8)k; ic_l[y][x] = l;
            if (k == 2) { sum[l][0] += c.r; sum[l][1] += c.g; sum[l][2] += c.b; cnt[l]++; }
        }
    for (int L = 0; L < 256; L++) {                       /* the copper's ramp: the mean of the 400 texels nearest in tone */
        int a = L, b = L, n = cnt[L], s[3] = { sum[L][0], sum[L][1], sum[L][2] };
        while (n < 400 && (a > 0 || b < 255)) {
            if (a > 0) { a--; n += cnt[a]; for (int k = 0; k < 3; k++) s[k] += sum[a][k]; }
            if (n < 400 && b < 255) { b++; n += cnt[b]; for (int k = 0; k < 3; k++) s[k] += sum[b][k]; }
        }
        for (int k = 0; k < 3; k++) tmp[L][k] = n ? s[k] / n : 0;
    }
    for (int L = 0; L < 256; L++) for (int k = 0; k < 3; k++) {
        int s = 0, n = 0;
        for (int d = -3; d <= 3; d++) if (L + d >= 0 && L + d < 256) { s += tmp[L + d][k]; n++; }
        ic_cr[L][k] = s / n;
    }
    for (int y = 0; y < IC_H; y++) {                      /* the sky's blue per row: its darkest quarter (no cloud) */
        int n = 0, m, s[3] = { 0, 0, 0 }, q = 0;
        for (int x = 0; x < PW; x++) { Rgb c = rgb15(ic_p[y][x]); if (c.b - c.r >= 90) srt[n++] = ic_l[y][x]; }
        ok[y] = n >= 12;
        if (!ok[y]) continue;
        qsort(srt, (size_t)n, sizeof(int), ic_icmp); m = srt[(n / 4 > 4 ? n / 4 : 4) - 1];
        for (int x = 0; x < PW; x++) { Rgb c = rgb15(ic_p[y][x]); if (c.b - c.r >= 90 && ic_l[y][x] <= m) { s[0] += c.r; s[1] += c.g; s[2] += c.b; q++; } }
        for (int k = 0; k < 3; k++) ic_sky[y][k] = s[k] / q;
    }
    for (int y = 0; y < IC_H; y++)
        if (!ok[y])
            for (int d = 1; d < IC_H; d++) {
                int t = y - d >= 0 && ok[y - d] ? y - d : y + d < IC_H && ok[y + d] ? y + d : -1;
                if (t >= 0) { for (int k = 0; k < 3; k++) ic_sky[y][k] = ic_sky[t][k]; break; }
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

/* the skirt's tone from its top to the grass, as the edge's own copper (the chain in front of it bridged) */
static void ic_prof(int side, int *pr, int n)
{
    int v[128], last = -1;
    for (int i = 0; i < n; i++) {
        int s = 0, c = 0;
        for (int x = 0; x < 4; x++) { int px = side ? PW - 4 + x : x; if (ic_k[IC_T0[side] + i][px] == 2) { s += ic_l[IC_T0[side] + i][px]; c++; } }
        v[i] = c >= 3 ? s / c : -1;
    }
    for (int i = 0; i < n; i++) {
        if (v[i] < 0) continue;
        if (last < 0) for (int k = 0; k < i; k++) v[k] = v[i];
        else for (int k = last + 1; k < i; k++) v[k] = v[last] + (v[i] - v[last]) * (k - last) / (i - last);
        last = i;
    }
    for (int k = last + 1; k < n; k++) v[k] = last >= 0 ? v[last] : 120;
    for (int i = 0; i < n; i++) {
        int s = 0;
        for (int d = -4; d <= 4; d++) s += v[clampi(i + d, 0, n - 1)];
        pr[i] = s / 9;
    }
}

static int ic_chain(int side, int which, int x, int y)   /* the picture's texel (x, y) is that chain's */
{
    if (x < 0 || x >= PW || y < 0 || y >= IC_H) return 0;
    if (!side && which == 0) {                            /* the thin chain: its dark texels and what lies between them (its
                                                            light texels: left out, the copies were hollow) */
        int up = 0, dn = 0;
        if (x >= 12 || abs(y - (240 - x)) > 9) return 0;
        if (ic_k[y][x] & 16) return 1;
        for (int a = 1; a <= 4; a++) {
            if (y - a >= 0 && abs(y - a - (240 - x)) <= 9 && (ic_k[y - a][x] & 16)) up = 1;
            if (y + a < IC_H && abs(y + a - (240 - x)) <= 9 && (ic_k[y + a][x] & 16)) dn = 1;
        }
        return up && dn;
    }
    if (!side) {
        Rgb c = rgb15(ic_p[y][x]);
        return x < 17 && abs(y - (298 - 2 * x)) <= 27 && (ic_k[y][x] & 8) && !(ic_l[y][x] > 150 && c.b - c.r >= 24 && y < 270);
    }
    return x >= PW - 31 && abs((y - 335) - fdiv((x - 319) * 34, 31)) <= 24 && (ic_k[y][x] & 8);
}

static void ic_gen(u16 *ext, int pad)
{
    static const int WHITE[3] = { 232, 236, 248 };
    static const int VEC[2][2][2] = { { { 12, 11 }, { 17, 34 } }, { { 31, 34 }, { 31, 34 } } };
    int (*cv)[3] = malloc(sizeof(int[3]) * (size_t)IC_H * (size_t)pad);
    if (!cv) return;
    for (int side = 0; side < 2; side++) {
        int ex = side ? PW - 1 : 0, gt = IC_GT[side], t0 = IC_T0[side], n0 = gt - t0, pr[128], ea[IC_H];
        int pd[160], pdl = -1;
        for (int y = 0; y < IC_H; y++) {
            int sl = (ic_sky[y][0] * 3 + ic_sky[y][1] * 6 + ic_sky[y][2]) / 10, k = ic_k[y][ex];
            ea[y] = (k == 4 || k == 8) && ic_l[y][ex] - sl > 14 ? clampi((ic_l[y][ex] - sl) * 256 / (236 - sl > 1 ? 236 - sl : 1), 0, 256) : 0;   /* (plain sky is no cloud: it left a lighter block by the join) */
        }
        for (int y = 0; y < IC_H; y++) {
            int e = 0, en = 0, ge = 0, gn = 0, gl;
            for (int d = -4; d <= 4; d++) if (y + d >= 0 && y + d < IC_H) { e += ea[y + d]; en++; }
            e /= en;
            for (int j = 0; j < pad; j++) {              /* sky and clouds */
                int n = noise(j + side * 700, y, 56, 22, 3) + noise(j + side * 700, y, 19, 9, 4) / 3 - 42;
                int band = 70 - abs(y - 300), a, *p = cv[(size_t)y * pad + j];
                if (band < 0) band = 0;
                a = clampi((n - 168 + band) * 3, 0, 230);
                if (j < 40) a = (e * (40 - j) + a * j) / 40;
                {   Rgb ec = rgb15(ic_p[y][ex]);            /* the edge's own blue where the edge is plain sky (no seam) */
                    int own = ic_k[y][ex] == 4 && ec.b - ec.r >= 80 && !e, sk[3] = { own ? ec.r : ic_sky[y][0], own ? ec.g : ic_sky[y][1], own ? ec.b : ic_sky[y][2] };
                    for (int k = 0; k < 3; k++) p[k] = (sk[k] * (256 - a) + WHITE[k] * a) / 256; }
            }
            if (y < gt - 2 || ic_g0 < 0) continue;
            {   int yy = clampi(y, ic_g0 + 4, ic_g1);     /* grass */
                gl = (ic_grow[yy][0] * 3 + ic_grow[yy][1] * 6 + ic_grow[yy][2]) / 10;
                for (int x = 0; x < 3; x++) { int px = side ? PW - 1 - x : x; if (ic_k[y][px] == 1) { ge += ic_l[y][px]; gn++; } }
                for (int j = 0; j < pad; j++) {
                    int *p = cv[(size_t)y * pad + j], g = (noise(j + side * 900, y, 3, 2, 9) - 128) * 14 / 128, f = 256;
                    if (y < gt + (noise(j + side * 50, 0, 5, 1, 12) - 128) / 64) continue;
                    if (gn && gl > 0 && j < 30) f = 256 + (ge / gn - gl) * 256 / gl * (30 - j) / 30;
                    for (int k = 0; k < 3; k++) p[k] = clampi((ic_grow[yy][k] + g) * f / 256, 0, 255);
                }
            }
        }
        ic_prof(side, pr, n0);
        {   /* the skirt's plates: on the picture the front plate's upper edge comes down outwards 0.61 rows a column and
               crosses the edge on row IC_PL (the line the user drew, 2026-11-17: "the line of continuity that piece
               has to have; now there is a jump").  The copper's tone by rows from that line (pd, row - line + 64),
               from the 24 columns next to the edge: the strip's skirt has that tone at the same distance from the
               line carried on, so the plate, its edge and the darker plate behind it go on across the join. */
            static int sm[160], cn[160];
            memset(sm, 0, sizeof sm); memset(cn, 0, sizeof cn);
            for (int i = 0; i < 24; i++)
                for (int y = 0; y < IC_H; y++) {
                    int x = side ? PW - 1 - i : i, d = y - (IC_PL[side] - 61 * i / 100) + 64;
                    if (d < 0 || d >= 160 || ic_k[y][x] != 2) continue;
                    sm[d] += ic_l[y][x]; cn[d]++;
                }
            for (int d = 0, last = -1; d < 160; d++) {
                if (cn[d] < 3) { pd[d] = -1; continue; }
                pd[d] = sm[d] / cn[d];
                if (last < 0) for (int k = 0; k < d; k++) pd[k] = pd[d];
                else for (int k = last + 1; k < d; k++) pd[k] = pd[last] + (pd[d] - pd[last]) * (k - last) / (d - last);
                last = d;
                pdl = d;
            }
            for (int d = pdl + 1; d < 160; d++) pd[d] = pdl >= 0 ? pd[pdl] : 120;
            if (pdl < 0) for (int d = 0; d < 160; d++) pd[d] = 120;
            for (int it = 0; it < 2; it++) {              /* (smooth: single rows' means came out as streaks) */
                int t[160];
                for (int d = 0; d < 160; d++) { int q = 0; for (int k = -3; k <= 3; k++) q += pd[clampi(d + k, 0, 159)]; t[d] = q / 7; }
                memcpy(pd, t, sizeof t);
            }
        }
        for (int j = 0; j < pad; j++) {                   /* the copper skirt */
            int top = side ? t0 + j * 75 / 100 + j * j * 8 / 1000 : t0 + j * 90 / 100 + j * j * 12 / 1000, hg = gt + 2 - top;
            if (top >= gt + 1) break;
            for (int y = top; y < gt + 2; y++) {
                int v = (y - top) * (n0 - 1) / (hg > 1 ? hg : 1), *p = cv[(size_t)y * pad + j];
                int d = clampi(y - (IC_PL[side] + 61 * j / 100) + 64, 0, 159);
                int tone = pd[d] - j * 10 / 80 + (noise(j + side * 333, y, 2, 2, 21) - 128) * 6 / 128
                           + (noise(j + side * 333, y, 10, 34, 22) - 128) * 8 / 128 - (y - top < 2 ? 25 : 0);
                (void)v;
                {   /* The skirt's plates, as on the picture (the user drew two of their edges on each side, 2026-11-17:
                       "maybe a similar pattern can be generated"): each plate's outer edge is an S: from its foot it
                       leans in, steps back out at a kink two thirds up and then sweeps in towards the castle (IC_SX:
                       columns inwards by frame row, read off those lines); the edges stand 14-15 columns apart.  On the
                       castle's side of an edge the plate is lit up to a bright rim; outside it lies its shadow. */
                    static const short SY[] = { 60, 90, 100, 110, 125, 140, 155, 182, 250 }, SXv[] = { 58, 25, 13, 8, 10, 13, 7, 0, 0 };
                    int r = y - 144, c = 0, u, t, f = j < 8 ? j : 8, m;
                    for (int k = 0; k < 8; k++)
                        if (r <= SY[k + 1] || k == 7) { int rr = clampi(r, SY[k], SY[k + 1]); c = SXv[k] + (SXv[k + 1] - SXv[k]) * (rr - SY[k]) / (SY[k + 1] - SY[k]); break; }
                    if (r < SY[0]) c = SXv[0];
                    u = (j + c - IC_LO[side]) * 256 / IC_LW;
                    t = fmodi(u, 256);
                    m = t < 20 ? -60 + t : t < 200 ? -40 + (t - 20) * 75 / 180 : t < 244 ? 35 + (t - 200) * 25 / 44 : 75 - (t - 244) * 9;
                    m += (int)(hash3(fdiv(u, 256), side, 31) % 13) - 6;
                    if (side) m = m * 5 / 8;
                    tone += m * f / 8;
                }
                if (j < 12 && ic_k[y][ex] == 2)            /* (from the edge texel's own tone: the picture's last columns can be darker than the 24's mean) */
                    tone = (ic_l[y][ex] * (12 - j) + tone * j) / 12;
                tone = clampi(tone, 0, 255);
                for (int k = 0; k < 3; k++) p[k] = ic_cr[tone][k];
            }
        }
        if (!side)                                        /* the big chain's shadow where it meets the grass */
            for (int y = 347; y < 358; y++)
                for (int j = 12; j < pad && j < 46; j++) {
                    int dx = (j - 29) * 16 / 16, dy = (y - 352) * 16 / 5, d = dx * dx + dy * dy, *p = cv[(size_t)y * pad + j];
                    if (d >= 256) continue;
                    for (int k = 0; k < 3; k++) p[k] = p[k] * (256 - (256 - d) * 110 / 256) / 256;
                }
        for (int which = 0; which < (side ? 1 : 2); which++) {   /* the chains: their own texels, link periods back */
            int dx = VEC[side][which][0], dy = VEC[side][which][1], ylim = (!side && which) ? 352 : IC_H;
            for (int j = 0; j < pad; j++) {
                int x = side ? PW + j : -1 - j;
                for (int y = 0; y < ylim; y++)
                    for (int k = 1; k < 14; k++) {
                        int xs = side ? x - dx * k : x + dx * k, ys = y - dy * k;
                        if (xs < 0 || xs >= PW) continue;
                        if (ic_chain(side, which, xs, ys)) { Rgb c = rgb15(ic_p[ys][xs]); int *p = cv[(size_t)y * pad + j]; p[0] = c.r; p[1] = c.g; p[2] = c.b; }
                        break;
                    }
            }
        }
        for (int y = 0; y < IC_H; y++)
            for (int j = 0; j < pad; j++) {
                const int *p = cv[(size_t)y * pad + j];
                ext[(size_t)y * 2 * pad + (side ? pad + j : pad - 1 - j)] =
                    (u16)(clampi(p[0], 0, 248) >> 3 | (clampi(p[1], 0, 248) >> 3) << 5 | (clampi(p[2], 0, 248) >> 3) << 10);
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
