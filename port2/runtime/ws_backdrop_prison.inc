/* runtime/ws_backdrop.c: AREA02 s2, the Underground Prison (gen_up; docs/PORT.md "More rooms with picture strips").
 * One 320 x 224 picture (page (128, 0) + page (256, 0) u 0-63): a rock wall with the cage's niche in its middle
 * (x 78-246, rows 70-190), a candle in a small niche right of it (flame (260, 106)), the cave's ceiling in shade
 * over it and the sandy floor under it.  Both edges are rock: nothing but more wall, ceiling and floor past them.
 * The room has two states (X002 updateLightsOutDarkness, event 0xB3 "Where'd the Lights Go?", GCD.bce57 at
 * 0x8009CE57): while the event is open (1..3) the game draws a subtractive 320 x 256 rectangle (C0 C0 C0) over the
 * whole frame, so only the wall's lightest texels round the niche are left; once it is done (0xFF) the room is lit.
 * The picture and its CLUT are the same in both, so the strips are too, and the rectangle (widened with the frame)
 * darkens them like the picture (left strip black; a few dim texels of the candle's glow in the right one).
 *   - the rock: quilted with rq_quilt (rooms2.inc) on the texels over their light (the rock's luminance averaged over
 *     17 x 17, cage and candle out) in three bands per side (ceiling, wall, floor; the ceiling's and the floor's
 *     windows from near their own rows, the wall's from any wall row), never from the cage, the candle, the cracks or
 *     the boulder (UP_NS: they would come back as copies) nor from texels in too dark a light (UP_LM);
 *   - the light, right: the edge columns' light of the row (17 x 17); left: the row's mean over the edge's 21 columns
 *     (the rock there lies in level strata: the lit band over the ledge's dark line at rows 74-75, the floor's line),
 *     so those go on at their rows, the line fading out past the 16:9 strip; both falling off as the candle's light
 *     (S^2 / (S^2 + r^2), S 60, over an ambient 0.4: the picture's own levels at the right edge and at the left one);
 *   - the dark face the left edge cuts (rows 119-170: UP_FB): its outline goes on up to the left along its own curve
 *     (one row per column at the edge, easing to level), the rock under it lit with the face's light, a darker rim
 *     under the outline as in the picture; no far end is drawn (its shade eases back to the wall's light);
 *   - the contrast per row matched to the picture's next to the edge (up_contrast) and the join (the first 8 columns'
 *     mean onto the picture's last 8, the first column onto the last one; up_join).
 * Integers only; a block depends on where it is, so a wider strip starts with the 16:9 columns. */
#define UP_CX 260                                        /* the candle's flame */
#define UP_CY 106
#define UP_CS 60                                         /* its light falls as S^2 / (S^2 + r^2) over an ambient 0.4 */
#define UP_KNEE 144                                      /* texels over their light: eased from here (to 184) */
#define UP_LW 21                                         /* the left edge's columns that give its light per row */
#define UP_F0 119                                        /* the dark face at the left edge: its rows */
#define UP_F1 171
static const short UP_EX[][4] = { { 74, 250, 66, 194 }, { 250, 272, 88, 124 } };   /* cage, candle: x0 x1 y0 y1 */
static const short UP_NS[][4] = { { 21, 62, 62, 172 }, { 266, 298, 80, 182 }, { 0, 34, 116, 171 } };   /* never a source: the
                                                            cracks and the boulder (they would come back as copies) */
static const u8 UP_FB[UP_F1 - UP_F0] = {                 /* the dark face: its columns on each row (x < this) */
    1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 8, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 18, 18,
    18, 19, 19, 20, 20, 20, 21, 21, 22, 22, 23, 24, 24, 27, 28, 28, 28, 28, 28, 28 };
static int up_in(const short (*t)[4], int n, int x, int y)
{
    for (int i = 0; i < n; i++) if (x >= t[i][0] && x < t[i][1] && y >= t[i][2] && y < t[i][3]) return 1;
    return 0;
}
static int up_ex(int x, int y) { return up_in(UP_EX, 2, x, y); }
static int up_face(int x, int y) { return y >= UP_F0 && y < UP_F1 && x >= 0 && x < UP_FB[y - UP_F0]; }
static short up_lf[PH][PW];                              /* the rock's light (0: no rock texel near) */
static Rgb up_alb[PH][PW];                               /* the picture's texels over their light (x 128) */
static Rgb up_al1[PH][UP_LW];                            /* the left columns' texels over their row's light (x 128) */
static int up_le[PH];                                    /* the light at the edge per row (the side being made) */
static int up_fl;                                        /* the dark face's light */
static int up_ls[PH];                                    /* the same over 13 rows (the left side) */
static int up_y0, up_y1, up_lm;                          /* the band being quilted: its source rows, their least light */
static int up_ok(int x, int y)
{
    return x >= 0 && x < PW && y >= up_y0 && y < up_y1 && up_lf[y][x] >= up_lm && !up_ex(x, y) && !up_in(UP_NS, 3, x, y);
}
static int up_ctx(int side, int x, int y, Rgb *c)
{
    if (!up_lf[y][x] || up_ex(x, y)) return 0;
    if (!side) { if (x >= UP_LW || up_face(x, y)) return 0; *c = up_al1[y][x]; return 1; }
    *c = up_alb[y][x]; return 1;
}
static void up_light(void)
{
    static int S[PH + 1][PW + 1], C[PH + 1][PW + 1];
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int m = !up_ex(x, y) && !up_face(x, y);
            S[y + 1][x + 1] = S[y][x + 1] + S[y + 1][x] - S[y][x] + (m ? lum(rgb15(pic[y][x])) : 0);
            C[y + 1][x + 1] = C[y][x + 1] + C[y + 1][x] - C[y][x] + m;
        }
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int x0 = x - 8 < 0 ? 0 : x - 8, x1 = x + 9 > PW ? PW : x + 9, y0 = y - 8 < 0 ? 0 : y - 8, y1 = y + 9 > PH ? PH : y + 9;
            int s = S[y1][x1] - S[y0][x1] - S[y1][x0] + S[y0][x0], c = C[y1][x1] - C[y0][x1] - C[y1][x0] + C[y0][x0];
            Rgb k = rgb15(pic[y][x]); int l;
            up_lf[y][x] = (short)(c >= 16 ? (s / c > 4 ? s / c : 4) : 0);
            l = up_lf[y][x] ? up_lf[y][x] : 64;
            up_alb[y][x].r = k.r * 128 / l; up_alb[y][x].g = k.g * 128 / l; up_alb[y][x].b = k.b * 128 / l;
        }
    {   int s = 0, n = 0;                                 /* the dark face's light: its texels 3 columns inside */
        for (int y = UP_F0; y < UP_F1; y++) for (int x = 0; x < UP_FB[y - UP_F0] - 3; x++) { s += lum(rgb15(pic[y][x])); n++; }
        up_fl = n ? s / n : 64;
    }
}
static void up_edge(int side)
{
    for (int y = 0; y < PH; y++) {
        int s = 0, c = 0;
        if (side) { for (int i = 0; i < 4; i++) if (up_lf[y][PW - 1 - i]) { s += up_lf[y][PW - 1 - i]; c++; } }
        else for (int x = 0; x < UP_LW; x++) if (!up_face(x, y)) { s += lum(rgb15(pic[y][x])); c++; }
        up_le[y] = c >= 4 ? (s / c > 4 ? s / c : 4) : -1;
    }
    for (int y = 1; y < PH; y++) if (up_le[y] < 0) up_le[y] = up_le[y - 1];
    if (!side) {                                         /* (a row's light there is the mean of 21 texels: what it
                                                            differs by from the 5 rows round it is halved when small (the
                                                            rock's grain, which would run on as ruled lines) and kept
                                                            when over a quarter of it (the ledge's line, the floor's)) */
        static int sm[PH];
        for (int y = 0; y < PH; y++) {
            int s = 0, n = 0;
            for (int yy = y - 2; yy <= y + 2; yy++) if (yy >= 0 && yy < PH) { s += up_le[yy]; n++; }
            sm[y] = s / n;
        }
        for (int y = 0; y < PH; y++) {
            int d = up_le[y] - sm[y], a = d < 0 ? -d : d, k = clampi(a * 2048 / sm[y] - 256, 0, 256);
            up_le[y] = sm[y] + d * (256 + k) / 512;
        }
    }
    if (!side) {
        for (int y = 0; y < PH; y++) {
            int s = 0, n = 0;
            for (int yy = y - 6; yy <= y + 6; yy++) if (yy >= 0 && yy < PH) { s += up_le[yy]; n++; }
            up_ls[y] = s / n;
        }
        for (int y = 0; y < PH; y++)
            for (int x = 0; x < UP_LW; x++) {
                Rgb k = rgb15(pic[y][x]); int l = up_face(x, y) ? up_fl : up_le[y];
                up_al1[y][x].r = k.r * 128 / l; up_al1[y][x].g = k.g * 128 / l; up_al1[y][x].b = k.b * 128 / l;
            }
    }
}
/* the dark face's outline in the left strip: its row (x 16) at strip column X.  In the picture it comes up to the
   edge at row 119 (its first row in column 0), one row per column there and flattening (2 per column further in); so
   on, the slope easing to level at X 20 (row 109).  No
   far end is drawn for it (a closed dark shape in a wall reads as a doorway): its shade eases back to the wall's
   light between X UP_FA and UP_FZ */
#define UP_FA 36
#define UP_FZ 100
static int up_fo16(int X)
{
    int t = X < 20 ? X : 20;
    return UP_F0 * 16 - 16 * t + 2 * t * t / 5 + (X > 20 ? (noise(X, 0, 24, 1, 77) - 128) / 6 : 0);
}
static int up_cov(int X, int y)                          /* the dark face's share of the strip's texel (0..16) */
{
    return y >= UP_F1 ? 0 : clampi((y + 1) * 16 - up_fo16(X), 0, 16);
}
static int up_row(int X, int y)                          /* the left edge's light of row y at strip column X: its lines
                                                            (the ledge's) fade out between X 56 and 104 */
{
    int w = clampi((X - 56) * 256 / 48, 0, 256);
    return (up_le[y] * (256 - w) + up_ls[y] * w) / 256;
}
static int up_want(int side, int X, int y)
{
    int l = side ? up_le[y] : up_row(X, y), c, S2 = UP_CS * UP_CS, dy = y - UP_CY;
    int ex = side ? PW - 1 - UP_CX : UP_CX, dx = ex + X;
    if (!side && (c = up_cov(X, y)) > 0) {
        int d = y * 16 + 16 - up_fo16(X), fl = up_fl;    /* (its rim: the two rows under the outline are darker) */
        int w = clampi((X - UP_FA) * 256 / (UP_FZ - UP_FA), 0, 256), up = up_row(X, y < UP_F0 ? y : UP_F0 - 1);
        if (d < 40) fl = fl * (168 + d * 88 / 40) / 256;
        w = w * w / 256 * (3 * 256 - 2 * w) / 256;
        fl += (up - fl) * w / 256;
        l = (up * (16 - c) + fl * c) / 16;
    }
    return l * (102 + (int)((long long)154 * (S2 + ex * ex + dy * dy) / (S2 + dx * dx + dy * dy))) / 256;
}
#define UP_CW 512
#define UP_CMAX 384
static int up_cm(int X, int y)                           /* the strip's mean luminance over 9 x 9 (its texels on the
                                                            same side of the dark face's outline) */
{
    int s = 0, n = 0, f = up_cov(X, y) >= 8;
    for (int yy = y - 4; yy <= y + 4; yy++)
        for (int XX = X - 4; XX <= X + 4; XX++) {
            if (yy < 0 || yy >= PH || XX < 1 || XX > vb_ow || (!cur_side && (up_cov(XX, yy) >= 8) != f)) continue;
            s += lum(cv[(size_t)yy * vb_ow + XX - 1]); n++;
        }
    return n ? s / n : 0;
}
static int up_hp(int x, int y)                           /* the picture's luminance less its 9 x 9 mean, likewise */
{
    int s = 0, n = 0, f = up_face(x, y);
    for (int yy = y - 4; yy <= y + 4; yy++)
        for (int xx = x - 4; xx <= x + 4; xx++) {
            if (yy < 0 || yy >= PH || xx < 0 || xx >= PW || up_ex(xx, yy) || up_face(xx, yy) != f) continue;
            s += lum(rgb15(pic[yy][xx])); n++;
        }
    return lum(rgb15(pic[y][x])) - (n ? s / n : 0);
}
static void up_join(int side)
{
    static int jd[PH][2][3];
    for (int st = 0; st < 2; st++) {
        int R = st ? 1 : 4, N = st ? 1 : 8;
        for (int y = 0; y < PH; y++) {
            int s[2][3] = { { 0 } }, n = 0;
            for (int yy = y - R; yy <= y + R; yy++) {
                if (yy < 0 || yy >= PH) continue;
                for (int i = 0; i < N; i++) {
                    Rgb a = cv[(size_t)yy * vb_ow + i];
                    if (!side && (up_face(i, yy) != (up_cov(i + 1, yy) == 16) || (up_cov(i + 1, yy) & 15))) continue;
                    s[1][0] += a.r; s[1][1] += a.g; s[1][2] += a.b;
                    a = rgb15(pic[yy][side ? PW - 1 - i : i]); s[0][0] += a.r; s[0][1] += a.g; s[0][2] += a.b; n++;
                }
            }
            for (int c = 0; c < 3; c++) jd[y][st][c] = n ? (s[0][c] / n - s[1][c] / n) * (st ? 3 : 4) / 4 : 0;
        }
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= (st ? 6 : 40) && X <= vb_ow; X++) {
                Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int w = st ? (7 - X) * 256 / 6 : X <= 8 ? 256 : (40 - X) * 256 / 32;
                p->r = clampi(p->r + jd[y][st][0] * w / 256, 0, 255); p->g = clampi(p->g + jd[y][st][1] * w / 256, 0, 255);
                p->b = clampi(p->b + jd[y][st][2] * w / 256, 0, 255);
            }
    }
}
/* the contrast per row (rows y +- 16): the spread of the strip's luminance (its first 40 columns) onto the picture's
   (its 32 columns next to the edge; the dark face left out on both), at most x 0.8 .. x UP_CMAX / 256; each texel
   moved about the mean of its 9 x 9 surroundings, so the light stays as made */
static void up_contrast(int side)
{
    static int k256[PH]; static short lm[PH][UP_CW], hp[PH][32];
    int w = vb_ow < UP_CW ? vb_ow : UP_CW;
    for (int y = 0; y < PH; y++) {
        for (int X = 1; X <= w; X++) lm[y][X - 1] = (short)up_cm(X, y);
        for (int i = 0; i < 32; i++) hp[y][i] = (short)up_hp(side ? PW - 1 - i : i, y);
    }
    for (int y = 0; y < PH; y++) {
        long long s[2] = { 0, 0 }, q[2] = { 0, 0 }; int n[2] = { 0, 0 };
        for (int yy = y - 16; yy <= y + 16; yy++) {
            if (yy < 0 || yy >= PH) continue;
            for (int i = 0; i < 40 && i < vb_ow; i++) {
                int l, x = side ? PW - 1 - i : i;
                if (i < 32 && !up_ex(x, yy) && (side || !up_face(x, yy))) { l = hp[yy][i]; s[0] += l; q[0] += l * l; n[0]++; }
                if (side || !up_cov(i + 1, yy)) { l = lum(cv[(size_t)yy * vb_ow + i]) - lm[yy][i]; s[1] += l; q[1] += l * l; n[1]++; }
            }
        }
        k256[y] = 256;
        if (n[0] > 64 && n[1] > 64) {
            long long v0 = q[0] / n[0] - (s[0] / n[0]) * (s[0] / n[0]), v1 = q[1] / n[1] - (s[1] / n[1]) * (s[1] / n[1]);
            if (v0 > 0 && v1 > 0) k256[y] = clampi(isqrt_i((int)(v0 * 65536 / v1)), 205, UP_CMAX);
        }
    }
    for (int y = 0; y < PH; y++)
        for (int X = 1; X <= w; X++) {
            Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int l = lum(*p), m = lm[y][X - 1], t;
            if (l <= 0) continue;
            t = m + (l - m) * k256[y] / 256; if (t < 0) t = 0;
            p->r = clampi(p->r * t / l, 0, 255); p->g = clampi(p->g * t / l, 0, 255); p->b = clampi(p->b * t / l, 0, 255);
        }
}
/* the bands: rows, block, overlaps, and the source rows (y0 y1 BW BH OV OT - - sy0 sy1 dy); UP_LM: the least light
   of a band's sources (a dark texel lit several times over shows its 5-bit steps as flat greenish patches) */
static const u8 UP_LM[2][3] = { { 4, 48, 40 }, { 40, 56, 56 } };
static const RqBand UP_BL[3] = { { 0, 62, 12, 31, 6, 6, 0, PW, 0, 66, 10 }, { 62, 172, 12, 28, 6, 6, 0, PW, 22, 172, -1 },
                                 { 172, PH, 12, 26, 6, 6, 0, PW, 166, PH, 8 } };
static const RqBand UP_BR[3] = { { 0, 62, 12, 31, 6, 6, 0, PW, 0, 66, 10 }, { 62, 158, 12, 32, 6, 6, 0, PW, 22, 172, -1 },
                                 { 158, PH, 12, 22, 6, 6, 0, PW, 150, PH, 10 } };
static void gen_up(int pad)
{
    pal_make();
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    rq_have = (u8 *)malloc((size_t)vb_ow * PH);
    if (!rq_have) { canvas_close(); return; }
    up_light();
    rq_src = up_alb; rq_ctx = up_ctx; rq_map = 0; rq_ok = up_ok;
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side; memset(rq_have, 0, (size_t)vb_ow * PH);
        up_edge(side);
        for (int i = 0; i < 3; i++) {
            const RqBand *b = side ? &UP_BR[i] : &UP_BL[i];
            up_y0 = b->sy0; up_y1 = b->sy1; up_lm = UP_LM[side][i];
            rq_quilt(side, b, 2202 + side * 10 + i);
        }
        for (int y = 0; y < PH; y++)                     /* lit */
            for (int X = 1; X <= vb_ow; X++) {
                Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int l = up_want(side, X, y), m, k = 256;
                m = p->r > p->g ? p->r : p->g; if (p->b > m) m = p->b;
                if (m > UP_KNEE) k = (UP_KNEE + (m - UP_KNEE) * 40 / (m - UP_KNEE + 40)) * 256 / m;   /* (a texel far
                                                            lighter than its surroundings, in a bright light: no flat white) */
                p->r = clampi(p->r * k / 256 * l / 128, 0, 255); p->g = clampi(p->g * k / 256 * l / 128, 0, 255);
                p->b = clampi(p->b * k / 256 * l / 128, 0, 255);
            }
        up_contrast(side);
        up_join(side);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(cv[(size_t)y * vb_ow + X - 1]);
    }
    free(rq_have); rq_have = 0;
    canvas_close();
}
