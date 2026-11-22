/* runtime/ws_backdrop.c: AREA16 s5, the bell tower (gen_bt; included there; docs/PORT.md "More rooms with picture
 * strips").  The same rules as the other rooms: integers only, position hashes, every colour one of the picture's own
 * (read at run time), nothing mirrored, a strip column does not depend on the strip's width.
 *
 * The picture (320 x 224 of a 256-row sprite pair at frame (0, 16) / (256, 16); it does not move with the camera): a
 * round tower seen from inside, looking up a little.  Both edges cut a square pillar that stands square to the view:
 * its shaded inner face (24 columns, foreshortened) and, at the edge, the sunlit face that looks at the camera (cream
 * at the left, burnt out to white at the right), 12 columns of it at the top, 5 at row 110, none from row 186.  The
 * strips go on with that face.  Measured on the picture (m = columns outwards from the edge texel):
 *   - the verticals meet at (160, -2200): the edge between the two faces is m = -12.0 + 0.067 y on both sides
 *     (rows 0-113), the inner face's far edge 24.4 columns further in (x 0.833 of the distance to column 159.4), so
 *     the pillar is 0.20 of its distance deep;
 *   - the horizon is row 147 (the capital's lines stand at rows 115.5 / 128 / 132 on the side pillars and at 128 /
 *     135.5 / 138 on the back ones, 1.67 times further away), which with the verticals gives a focal length of
 *     250-300 columns: the lit face, as wide as the pillar is deep, is 56 +- 6 columns.  Its far edge is
 *     m = 44.0 + 0.0924 y: inside a 16:9 strip down to row 97, and the sky stands beyond it;
 *   - the capital's lines are level on the lit face (the same rows on both sides): each row goes on with its own tone;
 *   - under row 136 the edge of the lit face bends outwards (the foot's curved corner): 0.10 columns a row at row
 *     160, 0.28 at row 184, where it leaves the picture; it goes on with that bend (bt_low256), the shaded face below it;
 *   - the floor is a disc: its far rim is the ellipse centre (159, 216.1), half axes 168 x 32.4 (fitted on the rim at
 *     columns 0-319, within half a row): it ends 9 columns into the left strip and 8 into the right one, round. */
#define BT_A10 1680                                       /* the floor's ellipse (x 10) */
#define BT_B10 324
#define BT_YC10 2161
#define BT_BW 14                                          /* the sky's quilting block, its step, its overlap */
#define BT_BH 18
#define BT_SX 10
#define BT_SY 16

static int bt_lum(u16 c) { return lum(rgb15(c)); }
static u16 bt_e(int s, int i, int y) { return pic[y][s ? PW - 1 - i : i]; }   /* i columns in from the edge */
static int bt_ell(int dx, int y)                           /* inside the floor's disc, dx columns from column 159 */
{
    long long x = dx * 10, d = y * 10 - BT_YC10;
    return x * x * BT_B10 * BT_B10 + d * d * BT_A10 * BT_A10 < (long long)BT_A10 * BT_A10 * BT_B10 * BT_B10;
}
static int bt_dx(int s, int X) { return (s ? 160 : 159) + X; }
/* the lit face's far edge (m x 256): the shaft, the capital's shelf (rows 114-134) and the foot, which widen as the
   inner face's outline does (4 / 6.5 / 15 columns at rows 122 / 136 / 178, x 1.25 for the nearer face) */
static int bt_far256(int y)
{
    int f = 11264 + 2366 * y / 100;
    if (y >= 179) return f + 19 * 256 + (y - 178) * 230;
    if (y >= 159) return f + 8 * 256 + (y - 158) * (y - 158) * 704 / 100;
    return f + (y >= 135 ? 8 * 256 : y >= 128 ? 6 * 256 : y >= 114 ? 5 * 256 : 0);
}
/* the lower edge of the lit face from row 184 on (m x 256): where it left the picture, its slope and its bend */
static int bt_low256(int s, int y) { int d = y - 184; return (s ? -238 : -46) + 70 * d + d * d * 41 / 40; }

/* --- the lit face.  Its light: per row the clean texels next to the edge (the run brighter than the mean of the
   edge texel and the darkest of 14 columns, less the two next to the boundary), their mean (bt_mu, luminance x 16)
   and spread (bt_rd); a strip row's light (bt_tone) is the mean of the rows around it (+-2) that are the same tone
   within the row's own spread, so that a row's noise does not become a stripe and the mouldings' lines, level on
   this face, stay lines.  Away from the edge (8 columns) the foot's light band is held to BT_CAP, and under it
   (from row 173 at the left, where the edge columns show darker texels) the light is the shaft's (bt_shaft): those
   texels fade out as a shade in the corner.
   The right face is burnt out at the edge (plain white, 7 columns of it): there the light is a hot spot over the
   clip at the edge that falls, within BT_HOT columns, to the light the same face has on the left pillar (never
   over the right edge's own row tone, so the mouldings' lines keep theirs).
   Its stone: the marble of the picture's other stone faces (bt_stone: the side pillars' shaded inner faces,
   the mouldings' rows left out; laid twice as wide, as those faces are foreshortened), as texture over its own light (the texel's luminance over the mean
   of the 9 x 9 stone texels round it, x 256), quilted over the face (bt_tex_block) and lit again with the face's
   light.  At the join the texture has the contrast the picture's lit columns have (overexposed: a quarter of the
   shaded faces'), rising to BT_GAIN over 12 columns.  A texel's colour: that luminance on the face's own colours
   (bt_rc: per luminance step the face's most frequent colour, the picture's stone colours where it has none). */
#define BT_HOT 18
#define BT_GAIN 115
#define BT_CAP (236 * 16)
#define BT_TW 10                                          /* the stone's quilting block, its step */
#define BT_TS 7
static u16 (*bt_fc)[PH][14]; static u8 bt_fn[2][PH];
static int bt_last[2], bt_shaft[2], bt_g0, (*bt_mu)[PH], (*bt_rd)[PH], (*bt_tone)[PH];
static u16 (*bt_gp)[256]; static int bt_ngp[2], (*bt_gn)[256];
static u16 bt_rc[2][64], bt_rs[64];                       /* colours by luminance / 4: the lit faces', the stone's */
static u16 (*bt_sr)[PW];                                 /* the stone's texture (x 256), 0 = not stone */
static u16 (*bt_sa)[2 * PW];                             /* the same, twice as wide (the inner faces seen from the front) */
static short (*bt_cand)[2]; static int bt_ncand;      /* the windows a block may come from */
static long long *bt_cc; static u8 *bt_use;
static int bt_ow;
static u16 *bt_t;                                         /* the quilted texture: columns X 0..bt_ow-1, 0 = not set */
static int bt_row(int s, int y)                           /* the row whose texels stand for row y */
{
    int yr = y > bt_last[s] ? bt_last[s] : y;
    while (yr > 0 && bt_mu[s][yr] < 0) yr--;
    if (bt_mu[s][yr] < 0) { yr = y; while (yr < bt_last[s] && bt_mu[s][yr] < 0) yr++; }
    return yr;
}
static int bt_same(int s, int yr, int yy)
{
    if (yy < 0 || yy > bt_last[s] || bt_mu[s][yy] < 0) return 0;
    return abs(bt_mu[s][yy] - bt_mu[s][yr]) <= clampi(2 * bt_rd[s][yr], 3 * 16, 14 * 16);
}
static void bt_face(int s)
{
    int *mu = bt_mu[s], last = -1;
    bt_ngp[s] = 0;
    for (int y = 0; y < PH; y++) {
        int l0 = bt_lum(bt_e(s, 0, y)), mn = l0, n = 0, sum = 0, d = 0;
        Rgb k = rgb15(bt_e(s, 0, y));
        bt_fn[s][y] = 0; mu[y] = -1; bt_rd[s][y] = 0;
        for (int i = 1; i < 14; i++) { int l = bt_lum(bt_e(s, i, y)); if (l < mn) mn = l; }
        if (l0 < 130 || l0 - mn < 40 || k.b > k.r + 24) continue;   /* (not this face: the shaded one, the sky) */
        while (n < 14 && bt_lum(bt_e(s, n, y)) >= (l0 + mn) / 2) n++;
        n -= 2;                                            /* (the last ones are mixed with the edge) */
        if (n < 1) continue;
        for (int i = 0; i < n; i++) {
            u16 c = bt_e(s, i, y); int j = 0;
            bt_fc[s][y][i] = c; sum += bt_lum(c);
            while (j < bt_ngp[s] && bt_gp[s][j] != c) j++;
            if (j == bt_ngp[s] && j < 256) { bt_gp[s][j] = c; bt_gn[s][j] = 0; bt_ngp[s]++; }
            if (j < 256) bt_gn[s][j]++;
        }
        bt_fn[s][y] = (u8)n; mu[y] = sum * 16 / n; last = y;
        for (int i = 0; i < n; i++) d += abs(bt_lum(bt_fc[s][y][i]) * 16 - mu[y]);
        bt_rd[s][y] = d / n;
    }
    bt_last[s] = last;
    if (last < 0) return;
    for (int y = 0; y < PH; y++) {
        int yr = bt_row(s, y), a = 0, na = 0;
        for (int yy = yr - 2; yy <= yr + 2; yy++) if (bt_same(s, yr, yy)) { a += mu[yy]; na++; }
        bt_tone[s][y] = na ? a / na : mu[yr];
    }
    { int a = 0, n = 0; for (int y = 96; y <= 110; y++) if (mu[y] >= 0) { a += bt_tone[s][y]; n++; } bt_shaft[s] = n ? a / n : mu[last]; }
}
static int bt_under(int s, int y) { return bt_last[s] >= 6 && y >= bt_last[s] - 5; }   /* under the foot's light band */
static int bt_light(int s, int X, int y)                  /* the face's light at strip column X (luminance x 16) */
{
    int t = bt_under(s, y) ? bt_mu[s][bt_row(s, y)] : bt_tone[s][y], b, f;
    if (!s) {
        b = bt_under(0, y) ? bt_shaft[0] : t > BT_CAP ? BT_CAP : t;   /* away from the edge: the shaft's light under the
            band (the edge columns' darker texels there fade out), the band itself not burnt out */
        return X < 8 ? b + (t - b) * (8 - X) / 8 : b;
    }
    if (bt_last[0] < 0 || t >= 240 * 16) return t >= 240 * 16 ? t + 14 * 16 : t;   /* (the user's call: the face's
        burnt-out white, as the picture shows it at the edge, carries on across the strip) */
    b = bt_under(0, y) ? bt_shaft[0] : bt_tone[0][y] > BT_CAP ? BT_CAP : bt_tone[0][y];   /* the light the same face has at the left */
    if (b > t) b = t;
    if (X >= BT_HOT) return b;
    if (t >= 240 * 16) t += 14 * 16;                       /* (clipped in the picture: brighter than white) */
    f = (X - 1) * 256 / (BT_HOT - 1); f = f * f * (768 - 2 * f) / 65536;
    return t + (b - t) * f / 256;
}
/* the stone texels of the picture and their texture */
static int bt_is_stone(int x, int y)
{
    u16 c = pic[y][x]; int r = r5(c), g = g5(c), b = b5(c), l = bt_lum(c);
    if ((x >= 36 && x < 285) || (y >= 111 && y < 142)) return 0;     /* the side pillars' inner faces, less the mouldings */
    return r >= b && r - b <= 9 && abs(r - g) <= 3 && l >= (x < 36 ? 105 : 55) && l <= 236 && !bt_ell(abs(x - 159), y);   /* (not
        the dark groove along the left pillar's corner) */
}
static u8 (*bt_stone__st)[PW];
static int (*bt_stone__si)[PW + 1];
static int (*bt_stone__sl)[PW + 1];
static int (*bt_stone__sa)[2 * PW + 1];
static void bt_stone(void)
{
    u8 (*const st)[PW] = bt_stone__st; int (*const si)[PW + 1] = bt_stone__si, (*const sl)[PW + 1] = bt_stone__sl;
    long long dev = 0; int nd = 0;
    for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++) st[y][x] = (u8)bt_is_stone(x, y);
    for (int x = 0; x <= PW; x++) si[0][x] = sl[0][x] = 0;
    for (int y = 0; y < PH; y++) {
        si[y + 1][0] = sl[y + 1][0] = 0;
        for (int x = 0; x < PW; x++) {
            si[y + 1][x + 1] = si[y][x + 1] + si[y + 1][x] - si[y][x] + st[y][x];
            sl[y + 1][x + 1] = sl[y][x + 1] + sl[y + 1][x] - sl[y][x] + (st[y][x] ? bt_lum(pic[y][x]) : 0);
        }
    }
    for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++) {
        int x0 = x - 4, x1 = x + 5, y0 = y - 4, y1 = y + 5, n, l;
        bt_sr[y][x] = 0;
        if (!st[y][x] || x0 < 0 || y0 < 0 || x1 > PW || y1 > PH) continue;
        n = si[y1][x1] - si[y0][x1] - si[y1][x0] + si[y0][x0];
        l = sl[y1][x1] - sl[y0][x1] - sl[y1][x0] + sl[y0][x0];
        if (n < 81 || l <= 0) continue;                    /* (stone all round: no outline's shade in the mean) */
        bt_sr[y][x] = (u16)clampi(bt_lum(pic[y][x]) * 256 * n / l, 150, 370);
        dev += abs(bt_sr[y][x] - 256); nd++;
    }
    {
        int (*const sa)[2 * PW + 1] = bt_stone__sa;
        for (int y = 0; y < PH; y++) for (int x = 0; x < 2 * PW; x++) {
            int a = bt_sr[y][x / 2], c = x / 2 + 1 < PW ? bt_sr[y][x / 2 + 1] : 0;
            bt_sa[y][x] = (u16)(x & 1 ? (a && c ? (a + c) / 2 : 0) : a);
        }
        for (int x = 0; x <= 2 * PW; x++) sa[0][x] = 0;
        for (int y = 0; y < PH; y++) {
            sa[y + 1][0] = 0;
            for (int x = 0; x < 2 * PW; x++) sa[y + 1][x + 1] = sa[y][x + 1] + sa[y + 1][x] - sa[y][x] + (bt_sa[y][x] != 0);
        }
        for (int pass = 0; pass < 2; pass++) {
            int n = 0;
            for (int y = 0; y + BT_TW <= PH; y += 2) for (int x = 0; x + BT_TW <= 2 * PW; x++)
                if (sa[y + BT_TW][x + BT_TW] - sa[y][x + BT_TW] - sa[y + BT_TW][x] + sa[y][x] == BT_TW * BT_TW) {
                    if (pass && bt_cand) { bt_cand[n][0] = (short)x; bt_cand[n][1] = (short)y; }
                    n++;
                }
            if (!pass) {
                free(bt_cand); free(bt_cc); free(bt_use);
                bt_cand = malloc((size_t)(n + 1) * sizeof *bt_cand); bt_cc = malloc((size_t)(n + 1) * sizeof *bt_cc); bt_use = malloc((size_t)n + 1);
            }
            bt_ncand = bt_cand && bt_cc && bt_use ? n : 0;
        }
    }
    /* the texture's gain at the join: the lit face's own spread over its light against the stone's (x 256) */
    {
        long long a = 0, b = 0;
        for (int y = 0; y < 111; y++) if (bt_mu[0][y] > 0 && bt_fn[0][y] >= 3) { a += bt_rd[0][y]; b += bt_mu[0][y]; }
        bt_g0 = nd && dev && b ? clampi((int)(a * 256 * 256 / b * nd / dev), 32, BT_GAIN) : BT_GAIN / 2;
    }
}
static int bt_face_need(int X, int y) { return X >= 1 && y >= 0 && y < PH && X * 256 - 128 < bt_far256(y) + 256; }
static void bt_tex_block(int s, int Xa, int ya)
{
    int need = 0, nb = 0, pick, bi = -1, bx0, by0; long long best = -1;
    for (int by = 0; by < BT_TW; by++) for (int bx = 0; bx < BT_TW; bx++) need += bt_face_need(Xa + bx, ya + by);
    if (!need || !bt_ncand) return;
    for (int i = 0; i < bt_ncand; i++) {
        int sx = bt_cand[i][0], sy = bt_cand[i][1], n = 0; long long c = 0;
        for (int by = 0; by < BT_TW; by++) {
            int y = ya + by;
            if (y < 0 || y >= PH) continue;
            for (int bx = 0; bx < BT_TW; bx++) {
                int X = Xa + bx, o, d;
                if (bx >= BT_TW - BT_TS && by >= BT_TW - BT_TS) break;   /* (only the overlaps) */
                if (X < 1 || X >= bt_ow || !(o = bt_t[(size_t)y * bt_ow + X])) continue;
                d = o - bt_sa[sy + by][sx + (s ? bx : BT_TW - 1 - bx)];
                c += d * d; n++;
            }
        }
        c = (n ? c * 16 / n : 0) + (long long)bt_use[i] * 6000 + (long long)(hash3(i, Xa, ya + s) & 255);   /* (not the same
            window again and again; a first block: any) */
        bt_cc[i] = c;
        if (best < 0 || c < best) best = c;
    }
    for (int i = 0; i < bt_ncand; i++) nb += bt_cc[i] <= best + best / 6 + 64;
    pick = (int)(hash3(Xa, ya, 0x77 + s) % (u32)nb);
    for (int i = 0, k = 0; i < bt_ncand; i++) if (bt_cc[i] <= best + best / 6 + 64 && k++ == pick) { bi = i; break; }
    if (bi < 0) return;
    bx0 = bt_cand[bi][0]; by0 = bt_cand[bi][1];
    for (int i = 0; i < bt_ncand; i++)                     /* (it and the windows that share most of it) */
        if (abs(bt_cand[i][0] - bx0) < BT_TW / 2 && abs(bt_cand[i][1] - by0) < BT_TW / 2 && bt_use[i] < 40) bt_use[i]++;
    for (int by = 0; by < BT_TW; by++) {
        int y = ya + by;
        if (y < 0 || y >= PH) continue;
        for (int bx = 0; bx < BT_TW; bx++) {
            int X = Xa + bx, v = bt_sa[by0 + by][bx0 + (s ? bx : BT_TW - 1 - bx)], w = 256; u16 *o;
            if (X < 1 || X >= bt_ow) continue;
            o = &bt_t[(size_t)y * bt_ow + X];
            if (*o) {                                      /* the overlap: a cross-fade to the new block */
                int ov = BT_TW - BT_TS, wx = bx < ov ? (bx + 1) * 256 / (ov + 1) : 256, wy = by < ov ? (by + 1) * 256 / (ov + 1) : 256;
                w = wx < wy ? wx : wy;
            }
            *o = (u16)((*o * (256 - w) + v * w) / 256);
        }
    }
}
/* the colours by luminance (steps of 4): the stone's (the mean colour of the picture's stone texels of that
   luminance, as the nearest of the picture's colours), the cream face's (its own most frequent colour per step,
   the stone's where it has none), the white face's (the cream one's, its own from luminance 240) */
static void bt_ramp(void)
{
    static int n[64], sr[64], sg[64], sb[64]; int have[64];
    for (int i = 0; i < 64; i++) { n[i] = sr[i] = sg[i] = sb[i] = 0; have[i] = 0; }
    for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++) if (bt_sr[y][x]) {
        Rgb k = rgb15(pic[y][x]); int b = lum(k) / 4;
        n[b]++; sr[b] += k.r; sg[b] += k.g; sb[b] += k.b;
    }
    for (int i = 0; i < 64; i++) if (n[i] >= 8) { Rgb k = { sr[i] / n[i], sg[i] / n[i], sb[i] / n[i] }; bt_rs[i] = quant(k); have[i] = 1; }
    for (int s = 0; s < 2; s++) {
        int hv[64], cn[64];
        for (int i = 0; i < 64; i++) { hv[i] = s ? 1 : have[i]; cn[i] = 0; bt_rc[s][i] = s ? bt_rc[0][i] : bt_rs[i]; }
        for (int i = 0; i < bt_ngp[s]; i++) {
            int b = bt_lum(bt_gp[s][i]) / 4;
            if ((!s || b >= 60) && bt_gn[s][i] > cn[b]) { cn[b] = bt_gn[s][i]; bt_rc[s][b] = bt_gp[s][i]; hv[b] = 1; }
        }
        for (int i = 0; i < 64 && !s; i++) if (!hv[i]) {
            int d = 1;
            while (d < 64 && !((i - d >= 0 && hv[i - d]) || (i + d < 64 && hv[i + d]))) d++;
            bt_rc[0][i] = d < 64 ? bt_rc[0][i + d < 64 && hv[i + d] ? i + d : i - d] : bt_e(0, 0, 0);
        }
    }
    for (int i = 0; i < 64; i++) if (!have[i]) {
        int d = 1;
        while (d < 64 && !((i - d >= 0 && have[i - d]) || (i + d < 64 && have[i + d]))) d++;
        bt_rs[i] = d < 64 ? bt_rs[i + d < 64 && have[i + d] ? i + d : i - d] : bt_e(0, 0, 0);
    }
}
static u16 bt_face_col(int s, int X, int y)
{
    int l, r, g, t;
    if (bt_last[s] < 0) return bt_e(s, 0, y);              /* (no lit face in this picture: the edge texel) */
    r = bt_t[(size_t)y * bt_ow + X]; if (!r) r = 256;
    g = s && bt_tone[1][y] >= 240 * 16 ? bt_g0 : X >= 13 ? BT_GAIN : bt_g0 + (BT_GAIN - bt_g0) * (X - 1) / 12;   /* (the
        white face keeps the contrast its edge columns have) */
    t = bt_light(s, X, y) / 16;
    l = t * (65536 + (r - 256) * g) / 65536;
    if (l > 243 && t < 240) l = 243;                       /* (no burnt-out specks where the face is not) */
    if (X <= 2 || (s && y <= bt_last[1] && bt_tone[1][y] < 240 * 16)) {   /* at the join, and the white face's lines:
                                                                             the row's own texels */
        int yr = bt_row(s, y), bd = 5; u16 best = 0;
        for (int i = 0; i < bt_fn[s][yr]; i++) { int d = abs(bt_lum(bt_fc[s][yr][i]) - l); if (d < bd) { bd = d; best = bt_fc[s][yr][i]; } }
        if (bd < 5) return best;
    }
    l += (int)(hash3(pcol(s, X), y, 0xB9) & 3) - 1;         /* (between two colours of the ramp) */
    return bt_rc[s][clampi(l, 0, 252) / 4];
}

/* --- the shaded face under the lit one's lower edge: the stone's texture in the light the three edge columns have on
   that face per row (the rows where they are that face: bt_d0..bt_d1; below them the last rows' light) */
static int bt_d0[2], bt_d1[2];
static int bt_is_dark(int s, int i, int y)
{
    u16 c = bt_e(s, i, y);
    return bt_lum(c) < 130 && r5(c) >= b5(c) && !bt_ell((s ? 160 : 159) - i, y);
}
static void bt_dark_rows(int s)
{
    bt_d0[s] = bt_d1[s] = -1;
    for (int y = 150; y < PH; y++)
        if (bt_is_dark(s, 0, y) && bt_is_dark(s, 1, y) && bt_is_dark(s, 2, y)) { if (bt_d0[s] < 0) bt_d0[s] = y; bt_d1[s] = y; }
        else if (bt_d0[s] >= 0) break;
}
static int (*bt_dl)[PH];                                  /* that face's light per row (luminance x 16) */
static void bt_dark_light(int s)
{
    for (int y = 0; y < PH; y++) {
        int a = 0, n = 0, yc = clampi(y, bt_d0[s], bt_d1[s] - 2);
        for (int yy = yc - 2; yy <= yc + 2; yy++)
            if (yy >= bt_d0[s] && yy <= bt_d1[s]) for (int i = 0; i < 3; i++) { a += bt_lum(bt_e(s, i, yy)); n++; }
        bt_dl[s][y] = n ? a * 16 / n : 0;
    }
}
static u16 bt_dark_col(int s, int X, int y)               /* the stone in that light */
{
    int r = bt_t[(size_t)y * bt_ow + X], l;
    if (bt_d0[s] < 0) return bt_e(s, 0, y);
    if (!r) r = 256;
    l = bt_dl[s][y] * r / 4096 + (int)(hash3(pcol(s, X), y, 0xD4) & 3) - 1;
    return bt_rs[clampi(l, 0, 252) / 4];
}

/* --- the floor: the texel of the edge columns that lies as far inside the disc (the same ellipse through it, on the
   far rim's side), so the rim's rings go round the disc's end */
static u16 bt_floor_col(int s, int X, int y)
{
    int i = (int)(hash3(pcol(s, X), y, 0xF1) % 3), dxs = (s ? 160 : 159) - i, ys;
    long long dx = bt_dx(s, X) * 10, dy = y * 10 - BT_YC10, S = 1 << 20;
    long long q = dx * dx * S / ((long long)BT_A10 * BT_A10) + dy * dy * S / ((long long)BT_B10 * BT_B10)
                  - (long long)dxs * dxs * 100 * S / ((long long)BT_A10 * BT_A10);
    ys = (BT_YC10 - BT_B10 * isqrt_i((int)(q < 0 ? 0 : q)) / 1024 + 5) / 10;
    ys = clampi(ys, 0, PH - 1);
    while (ys < PH - 1 && !bt_ell(dxs, ys)) ys++;          /* (a row of the floor) */
    return bt_e(s, i, ys);
}

/* --- what stands beyond the pillar: the sky (and, in wider strips, the clouds and the land under it), quilted from
   the picture's own views between the pillars.  bt_m: the texels of those views (blue, green or the clouds' white,
   outside the floor, not next to anything else: the pillars' and the chains' mixed outlines stay out).  Blocks of
   BT_BW x BT_BH texels on a fixed grid, outwards and downwards; a block comes from the same rows of the picture (the
   sky's gradient, the horizon), from rows above only where the picture shows nothing of those rows (under row 176),
   the one that fits the blocks already there (among the best by a hash), joined to them along the cut of least error
   in the overlap's columns. */
static u8 (*bt_m)[PW];
static u16 *bt_o;                                         /* the quilted canvas: columns X 0..bt_ow-1, 0 = not set */
static int bt_need(int X, int y) { return X >= 1 && y >= 0 && y < PH && X * 256 + 384 > bt_far256(y); }
static u8 (*bt_mask__o)[PW];
static void bt_mask(void)
{
    u8 (*const o)[PW] = bt_mask__o;
    for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++) {
        u16 c = pic[y][x]; int r = r5(c), g = g5(c), b = b5(c);
        o[y][x] = x >= 20 && x < 300 && !bt_ell(abs(x - 159), y)
                  && (b > r || (g > r && g > b) || (r == g && g == b && r >= 27));
    }
    for (int y = 0; y < PH; y++) for (int x = 0; x < PW; x++)
        bt_m[y][x] = o[y][x] && (y == 0 || o[y - 1][x]) && (y == PH - 1 || o[y + 1][x]) && x > 0 && o[y][x - 1]
                     && x < PW - 1 && o[y][x + 1];
}
static int bt_d2(u16 a, u16 b) { int r = r5(a) - r5(b), g = g5(a) - g5(b), bb = b5(a) - b5(b); return 3 * r * r + 4 * g * g + 2 * bb * bb; }
/* a block from the picture's window at (sx, sy): the error against the texels already there (x 64 a texel), -1
   when the window does not hold every texel the block needs */
static long long bt_cost(int s, int Xa, int ya, int sx, int sy)
{
    long long cost = 0; int n = 0;
    for (int by = 0; by < BT_BH; by++) {
        int y = ya + by, yy = sy + by;
        if (y < 0 || y >= PH) continue;
        for (int bx = 0; bx < BT_BW; bx++) {
            int X = Xa + bx, x = sx + (s ? bx : BT_BW - 1 - bx); u16 o;
            if (!bt_need(X, y)) continue;
            if (yy < 0 || !bt_m[yy][x]) return -1;
            o = X < bt_ow ? bt_o[(size_t)y * bt_ow + X] : 0;
            if (o) { cost += bt_d2(o, pic[yy][x]); n++; }
        }
    }
    return n ? cost * 64 / n : 0;
}
static int bt_prev[PH / BT_SY + 2];                        /* the window column of the block further in, per block row */
static void bt_block(int s, int Xa, int ya)
{
    int need = 0, nb = 0, bx0 = 0, by0 = 0, pick, cut[BT_BH], *pv = &bt_prev[(ya + 2) / BT_SY];
    long long best = -1;
    for (int by = 0; by < BT_BH; by++) for (int bx = 0; bx < BT_BW; bx++) need += bt_need(Xa + bx, ya + by);
    if (!need) return;
    for (int dy = 0; dy <= 160 && best < 0; dy++)        /* the same rows; rows above when the picture has none */
        for (int sx = 20; sx + BT_BW <= 300; sx++) {
            long long c = bt_cost(s, Xa, ya, sx, ya - dy);
            if (c >= 0 && abs(sx - *pv) < BT_BW) c += 4000;   /* not the same texels again next to themselves */
            if (c >= 0 && (best < 0 || c < best)) { best = c; by0 = ya - dy; }
        }
    if (best < 0) return;
    for (int sx = 20; sx + BT_BW <= 300; sx++) {
        long long c = bt_cost(s, Xa, ya, sx, by0);
        if (c >= 0 && abs(sx - *pv) < BT_BW) c += 4000;
        nb += c >= 0 && c <= best + best / 8 + 16;
    }
    pick = (int)(hash3(Xa, ya, 0x51 + s) % (u32)nb);
    for (int sx = 20, k = 0; sx + BT_BW <= 300; sx++) {
        long long c = bt_cost(s, Xa, ya, sx, by0);
        if (c >= 0 && abs(sx - *pv) < BT_BW) c += 4000;
        if (c >= 0 && c <= best + best / 8 + 16 && k++ == pick) { bx0 = sx; break; }
    }
    *pv = bx0;
    /* the cut through the overlap with the blocks further in: per row the first column the new block takes, the
       path of least error (one column a row at most) */
    {
        static int e[BT_BH][BT_BW - BT_SX + 1], acc[BT_BH][BT_BW - BT_SX + 1];
        int W = BT_BW - BT_SX + 1;                         /* cut positions 0..overlap */
        for (int by = 0; by < BT_BH; by++) for (int c = 0; c < W; c++) {
            int y = ya + by, v = 0;                       /* the cost of cutting before column c: the old texels kept
                                                             on columns < c, the new from c: the difference at c */
            int X = Xa + (c < W - 1 ? c : W - 2); u16 o = 0;
            if (y >= 0 && y < PH && X >= 1 && X < bt_ow) o = bt_o[(size_t)y * bt_ow + X];
            if (o && by0 + by >= 0) v = bt_d2(o, pic[by0 + by][bx0 + (s ? X - Xa : BT_BW - 1 - (X - Xa))]);
            e[by][c] = v;
        }
        for (int c = 0; c < W; c++) acc[0][c] = e[0][c];
        for (int by = 1; by < BT_BH; by++) for (int c = 0; c < W; c++) {
            int m = acc[by - 1][c];
            if (c > 0 && acc[by - 1][c - 1] < m) m = acc[by - 1][c - 1];
            if (c < W - 1 && acc[by - 1][c + 1] < m) m = acc[by - 1][c + 1];
            acc[by][c] = m + e[by][c];
        }
        {
            int c = 0;
            for (int k = 1; k < W; k++) if (acc[BT_BH - 1][k] < acc[BT_BH - 1][c]) c = k;
            for (int by = BT_BH - 1; by >= 0; by--) {
                int m = c;
                cut[by] = c;
                if (!by) break;
                if (c > 0 && acc[by - 1][c - 1] < acc[by - 1][m]) m = c - 1;
                if (c < W - 1 && acc[by - 1][c + 1] < acc[by - 1][m]) m = c + 1;
                c = m;
            }
        }
    }
    for (int by = 0; by < BT_BH; by++) {
        int y = ya + by;
        if (y < 0 || y >= PH || by0 + by < 0) continue;
        for (int bx = 0; bx < BT_BW; bx++) {
            int X = Xa + bx; u16 *o;
            if (X < 1 || X >= bt_ow || !bt_need(X, y)) continue;
            o = &bt_o[(size_t)y * bt_ow + X];
            if (*o && (bx < cut[by] || by < BT_BH - BT_SY)) continue;   /* the blocks already there: up to the cut, and
                                                                           the rows shared with the block above */
            if (!bt_m[by0 + by][bx0 + (s ? bx : BT_BW - 1 - bx)]) continue;
            *o = pic[by0 + by][bx0 + (s ? bx : BT_BW - 1 - bx)] | 0x8000;
        }
    }
}

static void gen_bt(int pad)
{
    if (!ws_need_gen_bt()) return;
    pal_make(); bt_mask();
    bt_ow = pad + BT_BW + 2; bt_o = malloc((size_t)bt_ow * PH * sizeof *bt_o); bt_t = malloc((size_t)bt_ow * PH * sizeof *bt_t);
    if (!bt_o || !bt_t) { free(bt_o); free(bt_t); bt_o = bt_t = 0; return; }
    bt_face(0); bt_face(1); bt_stone(); bt_ramp();
    for (int s = 0; s < 2; s++) {
        bt_dark_rows(s); bt_dark_light(s);
        memset(bt_o, 0, (size_t)bt_ow * PH * sizeof *bt_o); memset(bt_t, 0, (size_t)bt_ow * PH * sizeof *bt_t);
        if (bt_ncand) memset(bt_use, 0, (size_t)bt_ncand);
        for (int i = 0; i < PH / BT_SY + 2; i++) bt_prev[i] = -1000;
        for (int Xa = 1 - (BT_TW - BT_TS); Xa <= pad; Xa += BT_TS)
            for (int ya = -(BT_TW - BT_TS); ya < PH; ya += BT_TS) bt_tex_block(s, Xa, ya);
        for (int Xa = -3; Xa <= pad; Xa += BT_SX)
            for (int ya = -2; ya < PH; ya += BT_SY) bt_block(s, Xa, ya);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                u16 c;
                if (bt_ell(bt_dx(s, X), y)) c = bt_floor_col(s, X, y);
                else {
                    int a = y > 184 ? clampi(X * 256 + 128 - bt_low256(s, y), 0, 256) : 256;   /* lit, over the lower edge */
                    int b = clampi(bt_far256(y) - (X * 256 - 128), 0, 256);                   /* inside the far edge */
                    u16 f = bt_face_col(s, X, y), o = b < 256 ? bt_o[(size_t)y * bt_ow + X] : 0;
                    c = f;
                    if (a < 256) c = a ? quant(mix(rgb15(bt_dark_col(s, X, y)), rgb15(f), a)) : bt_dark_col(s, X, y);
                    if (b < 256 && o) c = b ? quant(mix(rgb15(o), rgb15(c), b)) : o;
                }
                strip[(size_t)y * 2 * pad + (s ? pad + (X - 1) : pad - X)] = (u16)(c & 0x7FFF);
            }
    }
    free(bt_o); free(bt_t); free(bt_cand); free(bt_cc); free(bt_use); bt_o = bt_t = 0; bt_cand = 0; bt_cc = 0; bt_use = 0; bt_ncand = 0;
}
