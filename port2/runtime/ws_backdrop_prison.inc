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
 *     windows from near their own rows, the wall's from any wall row), never from the cage, the candle, the boulder
 *     or the large cracks (UP_NS / UP_NL: they would come back as copies) nor from texels in too dark a light (UP_LM);
 *   - the right strip's light: the edge columns' light of the row (17 x 17), falling off as the candle's light
 *     (S^2 / (S^2 + r^2), S 60, over an ambient 0.4: the picture's own levels at the right edge and at the left one);
 *   - the left strip is drawn as more of the cave's wall, not as the edge's rows run on: larger blocks (20 x 37) from
 *     the whole wall, the thin cracks over the cage among them; its light is the edge's own rows at the join (the mean
 *     of the edge's 21 columns per row: the lit ledge, the floor's line) and within UP_SE columns the wall's smooth
 *     light (17 x 17), with the candle's falloff and a soft mottle;
 *   - what the left edge cuts ends inside the strip: the ledge's dark line (rows 74-75) goes on with the picture's own
 *     line texels (up_line) and ends as the picture's line does, by X UP_LZ; the dark face (rows 119-170: UP_FB) goes
 *     on along its own outline (one row per column at the edge, easing to level), filled with rock quilted from the
 *     face's own texels and lit with its light, a darker rim under the outline as in the picture, and dissolves into
 *     the wall between X UP_FA and UP_FZ (no far outline: a closed dark shape in the wall reads as a doorway);
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
#define UP_FS 142
#define UP_L0 73                                         /* the ledge's dark line at the left edge: its rows (the line
                                                            itself is rows 74-75 for x 0-25 and 35-61, a row higher from
                                                            x 44, broken by the cracks at x 26-34, fading from x 58) */
#define UP_L1 78
#define UP_SE 32                                         /* the left edge's strata end within this many columns */
static const short UP_EX[][4] = { { 74, 250, 66, 194 }, { 250, 272, 88, 124 } };   /* cage, candle: x0 x1 y0 y1 */
static const short UP_NS[][4] = { { 21, 62, 62, 172 }, { 266, 298, 80, 182 }, { 0, 34, 116, 171 } };   /* never a source
                                                            for the right strip: the cracks and the boulder */
static const short UP_NL[][4] = { { 18, 66, 60, 172 }, { 262, 300, 78, 184 }, { 0, 34, 112, 171 } };   /* for the left
                                                            one: the cracks next to it (they would come back as copies
                                                            beside themselves), the right one's X and the boulder (too easy
                                                            to know again), the dark face; the thin cracks over the
                                                            cage may come */
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
static Rgb up_alf[PH][PW];                               /* the dark face's texels over their light (x 128) */
static Rgb *up_fc;                                       /* the left strip's dark face, quilted from them (canvas) */
static u8 *up_fh;                                        /* its texels made */
static int up_fq;                                        /* 1: the dark face is being quilted */
static int up_ls[PH];                                    /* the wall's smooth light at the left edge (17 x 17) */
static int up_y0, up_y1, up_lm;                          /* the band being quilted: its source rows, their least light */
static int up_ok(int x, int y)
{
    if (up_fq) return y >= UP_FS && up_face(x + 5, y);   /* (the face's own texels: its even lower part, the rim left out) */
    return x >= 0 && x < PW && y >= up_y0 && y < up_y1 && up_lf[y][x] >= up_lm && !up_ex(x, y) && !up_in(cur_side ? UP_NS : UP_NL, 3, x, y);
}
static int up_ctx(int side, int x, int y, Rgb *c)
{
    if (up_fq) { if (!up_face(x, y)) return 0; *c = up_alf[y][x]; return 1; }
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
        if (up_fl < 4) up_fl = 4;
        for (int y = UP_F0; y < UP_F1; y++)             /* (over the face's light round it, 7 x 7: its grain) */
            for (int x = 0; x < UP_FB[y - UP_F0]; x++) {
                Rgb k = rgb15(pic[y][x]); int m = 0, c = 0;
                for (int yy = y - 3; yy <= y + 3; yy++) for (int xx = x - 3; xx <= x + 3; xx++)
                    if (xx >= 0 && up_face(xx, yy)) { m += lum(rgb15(pic[yy][xx])); c++; }
                m = c ? m / c : up_fl; if (m < 4) m = 4;
                up_alf[y][x].r = 32 + k.r * 96 / m; up_alf[y][x].g = 32 + k.g * 96 / m; up_alf[y][x].b = 32 + k.b * 96 / m;   /* (3/4 of it) */
            }
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
    if (!side)                                           /* (the ledge's line is drawn with its own texels: up_line) */
        for (int y = UP_L0; y < UP_L1; y++)
            up_le[y] = (up_le[UP_L0 - 1] * (UP_L1 - y) + up_le[UP_L1] * (y - UP_L0 + 1)) / (UP_L1 - UP_L0 + 1);
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
        for (int y = 0; y < PH; y++) {                   /* (the wall's light there, 17 x 17, the dark face left out) */
            int s = 0, n = 0;
            for (int x = 0; x < 4; x++) if (up_lf[y][x]) { s += up_lf[y][x]; n++; }
            up_ls[y] = n ? s / n : up_le[y];
        }
        for (int y = UP_F0; y < UP_F1; y++)              /* (behind the dark face: from the wall over it to the floor) */
            up_ls[y] = (up_ls[UP_F0 - 1] * (UP_F1 - y) + up_ls[UP_F1] * (y - UP_F0 + 1)) / (UP_F1 - UP_F0 + 1);
        for (int y = 0; y < PH; y++)
            for (int x = 0; x < UP_LW; x++) {
                Rgb k = rgb15(pic[y][x]); int l = up_face(x, y) ? up_fl : up_le[y];
                up_al1[y][x].r = k.r * 128 / l; up_al1[y][x].g = k.g * 128 / l; up_al1[y][x].b = k.b * 128 / l;
            }
    }
}
/* the left strip's strata do not run level: the edge's row light is read up_wob16 / 16 rows off (0 at the join, up
   to 2 rows either way from X 18 on, value noise along X) */
static int up_wob16(int X)
{
    int a = X < 2 ? 0 : X > 18 ? 32 : (X - 2) * 2;
    return (noise(X, 0, 28, 1, 91) - 128) * a / 128;
}
/* the dark face's outline in the left strip: its row (x 16) at strip column X.  In the picture it comes up to the
   edge at row 119 (its first row in column 0), one row per column there and flattening (2 per column further in); so
   on, the slope easing to level at X 20 (row 109), uneven like the picture's (value noise, +- 1.5 rows).  No far end
   is drawn (tried: a rounded end and a long wedge both read as a dark opening in the wall): the face dissolves into
   the wall between X UP_FA and UP_FZ (up_fw) */
#define UP_FA 14
#define UP_FZ 46
static int up_fo16(int X)
{
    int t = X < 20 ? X : 20, o = UP_F0 * 16 - 16 * t + 2 * t * t / 5;
    if (X > 20) o += (noise(X, 0, 24, 1, 77) - 128) / 6;
    if (X > 12) o += (noise(X, 0, 7, 1, 78) - 128) * (X > 28 ? 16 : X - 12) / 85;
    return o;
}
static int up_cov(int X, int y)                          /* the dark face's share of the strip's texel (0..16) */
{
    return y * 16 + up_wob16(X) >= UP_F1 * 16 ? 0 : clampi((y + 1) * 16 - up_fo16(X), 0, 16);
}
static int up_row(int X, int y)                          /* the left strip's light of row y at column X: at the join the
                                                            edge's own rows (its strata), read up_wob16 rows off; they
                                                            end within UP_SE columns, where the wall's smooth light
                                                            takes over (the strata are the edge's, not the wall's) */
{
    int w = clampi(X * 256 / UP_SE, 0, 256), y16 = y * 16 + up_wob16(X), y0 = fdiv(y16, 16), f = y16 - y0 * 16;
    int a = clampi(y0, 0, PH - 1), b = clampi(y0 + 1, 0, PH - 1);
    int l = (up_le[a] * (16 - f) + up_le[b] * f) / 16;
    w = w * w / 256 * (3 * 256 - 2 * w) / 256;
    return (l * (256 - w) + up_ls[y] * w) / 256;
}
static int up_fall(int side, int X, int y)               /* the candle's falloff (x 256) */
{
    int S2 = UP_CS * UP_CS, dy = y - UP_CY, ex = side ? PW - 1 - UP_CX : UP_CX, dx = ex + X;
    return 102 + (int)((long long)154 * (S2 + ex * ex + dy * dy) / (S2 + dx * dx + dy * dy));
}
static int up_want(int side, int X, int y)
{
    int l = side ? up_le[y] : up_row(X, y);
    if (!side) {                                         /* (a soft mottle: the light up to +- 12 %, in from the join) */
        int a = X > 12 ? 16 : X * 16 / 12;
        l += l * ((noise(X, y, 13, 9, 55) + noise(X, y, 29, 19, 56)) / 2 - 128) * a / 128 / 128;
    }
    return l * up_fall(side, X, y) / 256;
}
/* the dark face's texel at the left strip's (X, y), lit: its light, the two rows under the outline darker (the
   picture's rim), easing back to the wall's light at its thin far end */
static int up_fw(int X)                                  /* how much of the face is left at column X (0..256) */
{
    int w = clampi((X - UP_FA) * 256 / (UP_FZ - UP_FA), 0, 256);
    return 256 - w * w / 256 * (3 * 256 - 2 * w) / 256;
}
static Rgb up_flit(int X, int y, Rgb a)
{
    int d = y * 16 + 16 - up_fo16(X), fl = up_fl, m, k = 256;
    if (d < 40) fl = fl * (168 + d * 88 / 40) / 256;
    fl += fl * ((noise(X, y, 17, 11, 57) + noise(X, y, 37, 23, 58)) / 2 - 128) * 40 / 128 / 128;   /* (its soft patches) */
    fl = fl * up_fall(0, X, y) / 256;
    m = a.r > a.g ? a.r : a.g; if (a.b > m) m = a.b;
    if (m > UP_KNEE) k = (UP_KNEE + (m - UP_KNEE) * 40 / (m - UP_KNEE + 40)) * 256 / m;
    a.r = clampi(a.r * k / 256 * fl / 128, 0, 255); a.g = clampi(a.g * k / 256 * fl / 128, 0, 255);
    a.b = clampi(a.b * k / 256 * fl / 128, 0, 255);
    return a;
}
/* the ledge's dark line in the left strip, with the picture's own line texels: strip column X takes the line of
   picture column 23 - X (x 22..0, in their order: nothing mirrored), each texel as what it is of the rock over and
   under it in its column (rows 70-72 and 78-80), on the strip's rock; it follows the strata (up_wob16) and ends as
   the picture's own line does at x 56-69 (weaker over a dozen columns, gone by X UP_LZ) */
#define UP_LZ 23
static int up_lenv(int X) { return X <= 9 ? 256 : X >= UP_LZ ? 0 : (UP_LZ - X) * 256 / (UP_LZ - 9); }
static void up_line(void)
{
    for (int X = 1; X <= vb_ow && X < UP_LZ; X++) {
        int sx = 23 - X, e = up_lenv(X), ref[2][3] = { { 0 } };
        int sh = fdiv(up_wob16(X) + 8, 16);
        for (int i = 0; i < 3; i++) {
            Rgb a = rgb15(pic[UP_L0 - 3 + i][sx]), b = rgb15(pic[UP_L1 + i][sx]);
            ref[0][0] += a.r; ref[0][1] += a.g; ref[0][2] += a.b; ref[1][0] += b.r; ref[1][1] += b.g; ref[1][2] += b.b;
        }
        for (int r = UP_L0; r < UP_L1; r++) {            /* the picture's row r lands on the strip's row r - sh */
            int y = r - sh, t = r - UP_L0 + 1, n = UP_L1 - UP_L0 + 1; Rgb k = rgb15(pic[r][sx]), *p;
            int f[3], kk[3] = { k.r, k.g, k.b };
            if (y < 0 || y >= PH || up_cov(X, y)) continue;
            p = &cv[(size_t)y * vb_ow + X - 1];
            for (int c = 0; c < 3; c++) {
                int rf = (ref[0][c] * (n - t) + ref[1][c] * t) / n;      /* (x 3) */
                f[c] = rf > 0 ? clampi(kk[c] * 3 * 256 / rf, 0, 256) : 256;
                f[c] = 256 - (256 - f[c]) * e / 256;
            }
            p->r = p->r * f[0] / 256; p->g = p->g * f[1] / 256; p->b = p->b * f[2] / 256;
        }
    }
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
static const RqBand UP_BL[3] = { { 0, 62, 12, 31, 6, 6, 0, PW, 0, 66, 10 }, { 62, 172, 20, 37, 8, 6, 0, PW, 22, 172, -1 },
                                 { 172, PH, 12, 26, 6, 6, 0, PW, 166, PH, 8 } };
static const RqBand UP_BF = { 96, UP_F1 + 3, 6, 8, 3, 3, 0, 30, UP_F0, UP_F1, -1 };   /* the dark face (left) */
static const RqBand UP_BR[3] = { { 0, 62, 12, 31, 6, 6, 0, PW, 0, 66, 10 }, { 62, 158, 12, 32, 6, 6, 0, PW, 22, 172, -1 },
                                 { 158, PH, 12, 22, 6, 6, 0, PW, 150, PH, 10 } };
static void gen_up(int pad)
{
    pal_make();
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    rq_have = (u8 *)malloc((size_t)vb_ow * PH);
    if (!rq_have) { canvas_close(); return; }
    up_fc = (Rgb *)malloc((size_t)vb_ow * PH * sizeof *up_fc); up_fh = (u8 *)malloc((size_t)vb_ow * PH);
    if (!up_fc || !up_fh) { free(up_fc); free(up_fh); free(rq_have); rq_have = 0; canvas_close(); return; }
    up_light();
    rq_ctx = up_ctx; rq_map = 0; rq_ok = up_ok;
    canvas_clear(); cur_side = 0; memset(rq_have, 0, (size_t)vb_ow * PH);   /* the dark face's rock, from its own texels */
    up_fq = 1; rq_src = up_alf; rq_quilt(0, &UP_BF, 2290); up_fq = 0;
    memcpy(up_fc, cv, (size_t)vb_ow * PH * sizeof *up_fc); memcpy(up_fh, rq_have, (size_t)vb_ow * PH);
    rq_src = up_alb;
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
        if (!side) {
            up_line();
            for (int y = 0; y < PH; y++)                 /* the dark face over the wall */
                for (int X = 1; X <= vb_ow; X++) {
                    size_t i = (size_t)y * vb_ow + X - 1; int c = up_cov(X, y);
                    if (c && up_fh[i]) cv[i] = mix(cv[i], up_flit(X, y, up_fc[i]), c * up_fw(X) / 16);
                }
        }
        up_contrast(side);
        up_join(side);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(cv[(size_t)y * vb_ow + X - 1]);
    }
    free(rq_have); rq_have = 0; free(up_fc); free(up_fh); up_fc = 0; up_fh = 0;
    canvas_close();
}
