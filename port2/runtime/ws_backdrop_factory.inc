/* runtime/ws_backdrop.c: AREA11 s1, the Lumberjack Factory (the hut, the cage and the shed with the iron door)
 * (included there; docs/PORT.md "More rooms with picture strips").
 *
 * One 320 x 224 picture, fixed camera.  Measured on it:
 *   - sky to the tree line: on the right 20 columns a plain gradient, flat on rows 12-44; a cloud's haze at the top
 *     left corner;
 *   - trees behind everything, their crowns' tops between rows 32 and 45, down to the deck (row 118); clean foliage
 *     shows between the hut and the cage (columns 100-136), under the hut's brace at the left edge and over the shed;
 *   - the hut's roof: its front eave (rows 64-71) is cut by the left edge 9 columns before its corner (the hip comes
 *     down 0.85 rows per column), and so is the end of the brace under it (rows 75-78, 0.45 rows per column down);
 *   - the deck's front, rows 118-175 (top, rim, planks, bottom rail), runs behind the table to the shed; its boards
 *     repeat every 76 columns (the best match of the edge's own columns);
 *   - the sand from row 176;
 *   - the shed (rim rows 52-58, front to row 183): the door's left jamb is 10 columns of planks (248-257); of the
 *     right jamb the picture has 4 columns, so 6 are missing; its right side does not show (it faces away).
 * The rock in front of the door is in the picture; once broken, the game draws the open door over columns ..317,
 * and the picture's last two columns keep the rock's dark texels (the original's seam, left as it is).
 *
 * The strips:
 *   - sky: every texel one of the plain gradient's own on that row; the left corner's haze fades out over 28 columns;
 *   - trees: crowns are ragged discs lit from the upper left, in six rows, the upper ones behind; the foliage's
 *     texels are 8 x 8 windows of the picture's clean foliage, each chosen for its overlap with what is already laid
 *     and for the tone the crowns ask for there, then lit by the crowns through the foliage's own colour ramp;
 *   - the deck's top (rows 118-123) in the sun as at the picture's left edge (the hut shades the rest of it); its
 *     front carried on at its period, the sand under it from the same columns (cross-faded where it wraps);
 *   - left: the eave to its corner and the brace's end;
 *   - right: the jamb's 6 missing columns (the left jamb's planks, not mirrored), the shed's edge, its shadow on the
 *     deck and the sand; the deck and the trees go on behind it.  The rock's shadow on the sand is not carried on
 *     (it would stay as a dark post under the shed once the rock is broken).
 * Integers only; a texel depends on its place, not on the strip's width. */
#define LF_PS 8
#define LF_OV 3
#define LF_Y0 24 /* the foliage's first row */
#define LF_D 118 /* the deck's first row */
#define LF_SAND 176
#define LF_FEATHER 28
#define LF_JF 175 /* the right jamb's foot: the picture's columns 316-317 are sand from row 174 */
#define LF_PER 76 /* the deck's period */
#define LF_NS 1024
static int (*lf_cv)[3];
static int lf_pad;
static u8 (*lf_fm)[PW]; /* 1: foliage */
/* the windows, their mean tone x 16 */
static short (*lf_sx), (*lf_sy);
static int lf_ns, (*lf_sl);
static int lf_lo, lf_hi, (*lf_ramp)[3];
static int lf_etop; /* the first foliage row at the picture's left edge */
/* the crowns' tone (-1: sky) and height */
static short *lf_T;
static int *lf_Z;
static int lf_wd;

static int *lf_px(int j, int y) { return lf_cv[(size_t)y * lf_pad + j]; }
static void lf_set(int j, int y, Rgb c)
{
    int *p = lf_px(j, y);
    p[0] = c.r;
    p[1] = c.g;
    p[2] = c.b;
}
static int lf_cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

/* the foliage mask (lf_fm), the foliage's colour ramp by luminance (lf_ramp) and the source windows of the quilt
   (lf_sx / lf_sy, their tones lf_sl; lf_lo / lf_hi: the tones' 3rd and 97th percentiles) */
static int (*lf_sources__sum)[3];
static int (*lf_sources__cnt);
static int (*lf_sources__srt);
static void lf_sources(void)
{
    static const short RECT[4][4] = {
        { 116, 38, 137, 95 }, { 100, 38, 116, 57 }, { 0, 92, 12, 118 }, { 248, 36, 296, 52 }
    };
    int (*const sum)[3] = lf_sources__sum,
        (*const cnt) = lf_sources__cnt,
        (*const srt) = lf_sources__srt;
    memset(sum, 0, (sizeof *sum * (256)));
    memset(cnt, 0, (sizeof *cnt * (256)));
    for (int y = 0; y < LF_D; y++)
        for (int x = 0; x < PW; x++) {
            Rgb c = rgb15(pic[y][x]);
            int leafy = (c.g >= c.r + 8 && c.g >= c.b + 8) || (c.g >= c.r && c.g > c.b && c.r + c.g + c.b < 120);
            lf_fm[y][x] = (u8)leafy;
            if (leafy) {
                int luma = lum(c);
                sum[luma][0] += c.r;
                sum[luma][1] += c.g;
                sum[luma][2] += c.b;
                cnt[luma]++;
            }
        }
    /* the ramp: the mean of the 80 foliage texels nearest in tone */
    for (int L = 0; L < 256; L++) {
        int a = L,
            b = L,
            n = cnt[L],
            s[3] = { sum[L][0], sum[L][1], sum[L][2] };
        while (n < 80 && (a > 0 || b < 255)) {
            if (a > 0) {
                a--;
                n += cnt[a];
                for (int k = 0; k < 3; k++) s[k] += sum[a][k];
            }
            if (n < 80 && b < 255) {
                b++;
                n += cnt[b];
                for (int k = 0; k < 3; k++) s[k] += sum[b][k];
            }
        }
        for (int k = 0; k < 3; k++) lf_ramp[L][k] = n ? s[k] / n : 0;
    }
    for (lf_etop = 0; lf_etop < LF_D - 1 && !lf_fm[lf_etop][0]; lf_etop++);
    lf_ns = 0;
    for (int rect = 0; rect < 4; rect++)
        for (int y = RECT[rect][1]; y <= RECT[rect][3] - LF_PS; y++)
            for (int x = RECT[rect][0]; x <= RECT[rect][2] - LF_PS; x++) {
                int ok = 1,
                    s = 0;
                for (int v = 0; v < LF_PS && ok; v++)
                    for (int u = 0; u < LF_PS; u++) {
                        if (!lf_fm[y + v][x + u]) {
                            ok = 0;
                            break;
                        }
                        s += lum(rgb15(pic[y + v][x + u]));
                    }
                if (ok && lf_ns < LF_NS) {
                    lf_sx[lf_ns] = (short)x;
                    lf_sy[lf_ns] = (short)y;
                    lf_sl[lf_ns] = s * 16 / (LF_PS * LF_PS);
                    lf_ns++;
                }
            }
    lf_lo = lf_hi = 0;
    if (lf_ns) {
        memcpy(srt, lf_sl, sizeof(int) * (size_t)lf_ns);
        qsort(srt, (size_t)lf_ns, sizeof(int), lf_cmp);
        lf_lo = srt[lf_ns * 3 / 100];
        lf_hi = srt[lf_ns * 97 / 100];
    }
}

/* the crowns: ragged discs, lit from the upper left; the lower rows stand in front */
static void lf_crowns(int side)
{
    static const int CY[6] = { 50, 61, 72, 83, 94, 105 },
                     SP[6] = { 15, 12, 12, 12, 12, 12 };
    int wd = lf_wd;
    for (int i = 0; i < LF_D * wd; i++) {
        lf_T[i] = -1;
        lf_Z[i] = 0;
    }
    for (int row = 0; row < 6; row++) {
        int x = -12 + (int)(hash3(side, row, 1) % (u32)SP[row]);
        for (int k = 0; x < wd + 16; k++) {
            u32 rnd = hash3(side, row * 64 + k, 9);
            int rad = (row ? 5 : 8) + (int)(rnd % (row ? 8u : 9u));
            int cy = CY[row] + (int)((rnd >> 8) % (row ? 11u : 17u)) - (row ? 5 : 8);
            int twist = (int)((rnd >> 20) % 32),
                reach = rad + 3;
            for (int y = cy - reach; y <= cy + reach; y++) {
                if (y < 0 || y >= LF_D) continue;
                for (int px = x - reach; px <= x + reach; px++) {
                    int n, rad16, dx, dy, d, z, light, tone, depth;
                    if (px < 0 || px >= wd) continue;
                    n = noise(px + side * 500, y, 4, 4, row * 7 + x + 1000) - 128;
                    rad16 = rad * 16 + n * rad * 5 / 128;
                    dx = (px - x) * 16 * (26 + twist % 8) / 32;
                    dy = (y - cy) * 16 * (34 - twist % 8) / 32;
                    d = rad16 * rad16 - dx * dx - dy * dy;
                    if (d < 0) continue;
                    z = isqrt_i(d);
                    light = (-(px - x) * 8 - (y - cy) * 13) * 16 + z * 5;
                    tone = 118 + light * 62 / (rad * 256) - row * 4 + twist - 16;
                    depth = z + row * 30 + twist * 2;
                    if (lf_T[y * wd + px] < 0 || depth >= lf_Z[y * wd + px]) {
                        lf_T[y * wd + px] = (short)clampi(tone, 0, 255);
                        lf_Z[y * wd + px] = depth;
                    }
                }
            }
            x += SP[row] - 5 + (int)((rnd >> 14) % 11);
        }
    }
    for (int x = 0; x < wd; x++) { /* no sky under a crown, none below row 64 */
        int seen = 0;
        for (int y = 0; y < LF_D; y++) {
            if (lf_T[y * wd + x] >= 0)
                seen = 1;
            else if (seen || y >= 64 || (!side && x < 8 && y >= lf_etop + x / 2))
                lf_T[y * wd + x] = (short)(60 - (y > 64 ? (y - 64) / 2 : 0));
        }
    }
}

/* one strip's foliage (rows LF_Y0 to LF_D): the picture's foliage windows quilted at the crowns' tones (lf_crowns),
   then laid on the canvas through the ramp */
static void lf_foliage(int side)
{
    int wd = lf_wd,
        pad = lf_pad,
        nrows = LF_D - LF_Y0,
        step = LF_PS - LF_OV;
    /* the picture's foliage tone at the left edge, per row */
    int edge_lum[LF_D], edge_n[LF_D];
    int (*out)[3] = calloc((size_t)(nrows + LF_PS) * wd, sizeof *out);
    u8 *done = calloc((size_t)(nrows + LF_PS) * wd, 1);
    if (!out || !done || !lf_ns) {
        free(out);
        free(done);
        return;
    }
    lf_crowns(side);
    for (int y = LF_Y0; y < LF_D; y++) {
        edge_lum[y] = edge_n[y] = 0;
        if (side) continue;
        for (int c = 0; c < LF_OV; c++) /* canvas columns 0..LF_OV-1: the picture's columns 2, 1, 0 */
            if (lf_fm[y][LF_OV - 1 - c]) {
                Rgb k = rgb15(pic[y][LF_OV - 1 - c]);
                int *o = out[(y - LF_Y0) * wd + c];
                o[0] = k.r;
                o[1] = k.g;
                o[2] = k.b;
                done[(y - LF_Y0) * wd + c] = 2;
            }
        for (int v = -3; v <= 3; v++)
            for (int x = 0; x < 4; x++)
                if (y + v >= LF_Y0 && y + v < LF_D && lf_fm[y + v][x]) {
                    edge_lum[y] += lum(rgb15(pic[y + v][x]));
                    edge_n[y]++;
                }
    }
    for (int by = 0; by < nrows; by += step)
        for (int bx = 0; bx < wd - LF_OV; bx += step) {
            int yy = LF_Y0 + by,
                tile_w = wd - bx < LF_PS ? wd - bx : LF_PS,
                tile_h = LF_PS;
            int tone_sum = 0,
                tone_n = 0,
                want,
                ndone = 0,
                best[5], best_i[5],
                nbest = 0,
                pick;
            for (int v = 0; v < tile_h && yy + v < LF_D; v++)
                for (int u = 0; u < tile_w; u++) {
                    int tone = bx + u >= LF_OV ? lf_T[(yy + v) * wd + bx + u - LF_OV] : -1;
                    if (tone >= 0) {
                        tone_sum += tone;
                        tone_n++;
                    }
                }
            if (!tone_n) continue;
            want = lf_lo + (lf_hi - lf_lo) * (tone_sum / tone_n) / 255;
            for (int v = 0; v < tile_h; v++)
                for (int u = 0; u < tile_w; u++) ndone += done[(by + v) * wd + bx + u] != 0;
            for (int src = 0; src < lf_ns; src++) {
                int err = 0,
                    dlum = lf_sl[src] - want,
                    i;
                if (ndone) {
                    for (int v = 0; v < tile_h; v++)
                        for (int u = 0; u < tile_w; u++) {
                            const int *o;
                            Rgb c;
                            if (!done[(by + v) * wd + bx + u]) continue;
                            o = out[(by + v) * wd + bx + u];
                            c = rgb15(pic[lf_sy[src] + v][lf_sx[src] + u]);
                            err +=
                                (c.r - o[0]) * (c.r - o[0]) + (c.g - o[1]) * (c.g - o[1]) + (c.b - o[2]) * (c.b - o[2]);
                        }
                    err = err / (2 * ndone);
                }
                err += dlum * dlum / 128;
                for (i = nbest; i > 0 && best[i - 1] > err; i--)
                    if (i < 5) {
                        best[i] = best[i - 1];
                        best_i[i] = best_i[i - 1];
                    }
                if (i < 5) {
                    best[i] = err;
                    best_i[i] = src;
                    if (nbest < 5) nbest++;
                }
            }
            pick = best_i[hash3(side, bx, by) % (u32)nbest];
            for (int v = 0; v < tile_h; v++)
                for (int u = 0; u < tile_w; u++) {
                    int *o = out[(by + v) * wd + bx + u],
                        blend = 16;
                    Rgb c = rgb15(pic[lf_sy[pick] + v][lf_sx[pick] + u]);
                    if (done[(by + v) * wd + bx + u] == 2) continue;
                    if (done[(by + v) * wd + bx + u]) {
                        if (bx > 0 && u < LF_OV) blend = blend * (u + 1) / (LF_OV + 1);
                        if (by > 0 && v < LF_OV) blend = blend * (v + 1) / (LF_OV + 1);
                    }
                    o[0] = (c.r * blend + o[0] * (16 - blend)) / 16;
                    o[1] = (c.g * blend + o[1] * (16 - blend)) / 16;
                    o[2] = (c.b * blend + o[2] * (16 - blend)) / 16;
                    done[(by + v) * wd + bx + u] = 1;
                }
        }
    for (int y = LF_Y0; y < LF_D; y++) {
        int join_sum = 0,
            join_n = 0,
            dedge = 0;
        /* the strip's tone next to the join, to meet the picture's */
        for (int j = 0; j < 6 && j < pad; j++)
            if (lf_T[y * wd + j] >= 0) {
                const int *o = out[(y - LF_Y0) * wd + j + LF_OV];
                join_sum += (o[0] * 3 + o[1] * 6 + o[2]) / 10;
                join_n++;
            }
        if (!side && join_n && edge_n[y]) dedge = edge_lum[y] / edge_n[y] - join_sum / join_n;
        for (int j = 0; j < pad; j++) {
            const int *o = out[(y - LF_Y0) * wd + j + LF_OV];
            int tone = lf_T[y * wd + j],
                luma, *p,
                near = 256;
            if (tone < 0 || !done[(y - LF_Y0) * wd + j + LF_OV]) continue;
            /* the trees come up from behind the shed */
            if (side && y * 2 < 104 - (j - 6) * 3 + (noise(j, y, 3, 3, 77) - 128) / 24) continue;
            /* the picture's tree top at the join */
            if (!side && j < 8 && y < lf_etop - j) continue;
            /* the picture's own tree goes on */
            if (!side && j < 20 && edge_n[y]) near = j * 256 / 20;
            luma = (o[0] * 3 + o[1] * 6 + o[2]) / 10;
            luma = clampi(luma + (tone - 104) * near / 512 + dedge * (256 - near) / 256 +
                              (noise(j + side * 300, y, 24, 24, 5) - 128) / 8,
                          0, 255);
            p = lf_px(j, y);
            p[0] = lf_ramp[luma][0];
            p[1] = lf_ramp[luma][1];
            p[2] = lf_ramp[luma][2];
        }
    }
    free(out);
    free(done);
}

/* the deck's front and the sand at the picture's column u (any) */
static Rgb lf_ground(int y, int u)
{
    int phase = fmodi(u, LF_PER);
    if (y >= LF_SAND) { /* the sand: columns 8..83, the last 8 faded into 0..7 */
        phase = fmodi(u - 8, LF_PER);
        if (phase >= LF_PER - 8)
            return mix(rgb15(pic[y][8 + phase]), rgb15(pic[y][8 + phase - LF_PER]), (phase - (LF_PER - 9)) * 256 / 9);
        return rgb15(pic[y][8 + phase]);
    }
    return rgb15(pic[y][phase]);
}

/* The picture's last two columns keep the dark texels of the rock that stood in the door (rows 130-199) once the
   rock is broken: the open door the game then draws (a quad from page 256, texel (64, 0), 80 columns at 238) ends at
   column 317.  On 4:3 that was a ragged frame edge; with the strips it was a black line up the middle of the right
   jamb (the user, 2026-11-17).  lf_fix holds those two columns' dark texels done again (bit 15 set; 0: leave the
   picture's): the jamb's planks (the left jamb's, as the strip's columns) and the sand under it; shims/gpu.c draws
   them when the game draws the open door (port_ws_backdrop_fix). */
static u16 lf_fix[PH][2];
/* the lit sand (a texel 12 rows further down) with the tone the picture's sand has on row y in front of the door
   (the open door's columns 311-317, clean sand; the door is in pic as an over piece), per channel */
static int lf_lt[3], (*lf_dt)[3];
static void lf_door_tone(void)
{
    int n = 0;
    lf_lt[0] = lf_lt[1] = lf_lt[2] = 0;
    for (int y = LF_SAND + 12; y < LF_SAND + 24; y++)
        for (int u = 28; u < 60; u++) {
            Rgb c = lf_ground(y, u);
            lf_lt[0] += c.r;
            lf_lt[1] += c.g;
            lf_lt[2] += c.b;
            n++;
        }
    for (int k = 0; k < 3; k++) lf_lt[k] = lf_lt[k] / n > 0 ? lf_lt[k] / n : 1;
    for (int y = 0; y < PH; y++) {
        int s[3] = { 0, 0, 0 },
            m = 0;
        for (int d = -1; d <= 1; d++)
            for (int x = 311; x < 318; x++) {
                Rgb c = rgb15(pic[clampi(y + d, LF_JF, PH - 1)][x]);
                s[0] += c.r;
                s[1] += c.g;
                s[2] += c.b;
                m++;
            }
        for (int k = 0; k < 3; k++) lf_dt[y][k] = s[k] / m;
    }
}
static Rgb lf_door(Rgb c, int y)
{
    Rgb o = { clampi(c.r * lf_dt[y][0] / lf_lt[0], 0, 255), clampi(c.g * lf_dt[y][1] / lf_lt[1], 0, 255),
              clampi(c.b * lf_dt[y][2] / lf_lt[2], 0, 255) };
    return o;
}
static void lf_fix_make(void)
{
    memset(lf_fix, 0, sizeof lf_fix);
    for (int y = 120; y < 200; y++)
        for (int k = 0; k < 2; k++) {
            Rgb c = rgb15(pic[y][318 + k]),
                fixed;
            if (lum(c) >= (y < LF_JF ? 56 : 132)) continue;
            if (y < LF_JF) {
                fixed = rgb15(pic[y < 146 ? y : y - 36][250 + k]);
                if (y >= LF_JF - 3) fixed = scale(fixed, 256 - (y - (LF_JF - 4)) * 40);
            }
            /* (as the strip's first columns) */
            else
                fixed = y < LF_SAND + 12 ? lf_door(lf_ground((y < LF_SAND ? LF_SAND : y) + 12, 28 + k), y)
                                         : lf_ground(y, 28 + k);
            lf_fix[y][k] = (u16)(0x8000 | clampi(fixed.r, 0, 248) >> 3 | (clampi(fixed.g, 0, 248) >> 3) << 5 |
                                 (clampi(fixed.b, 0, 248) >> 3) << 10);
        }
}

/* both strips: the sky, the deck and the sand, the foliage, then each side's own pieces (left: the eave and the
   brace; right: the jamb, the shed's shadow and the deck's shadow on the sand) */
static void gen_lf(int pad)
{
    if (!ws_need_gen_lf()) return;
    lf_door_tone();
    lf_fix_make();
    lf_cv = calloc((size_t)PH * (size_t)pad, sizeof *lf_cv);
    lf_pad = pad;
    lf_wd = pad + LF_PS + LF_OV;
    lf_T = malloc(sizeof(short) * (size_t)LF_D * lf_wd);
    lf_Z = malloc(sizeof(int) * (size_t)LF_D * lf_wd);
    if (!lf_cv || !lf_T || !lf_Z) {
        free(lf_cv);
        free(lf_T);
        free(lf_Z);
        lf_cv = 0;
        return;
    }
    lf_sources();
    for (int side = 0; side < 2; side++) {
        for (int y = 0; y < PH; y++) {
            int sky_y = y < 49 ? y : 49;
            if (y < LF_D) {
                Rgb ref = { 0, 0, 0 },
                    edge = { 0, 0, 0 };
                for (int x = 300; x < 320; x++) {
                    Rgb c = rgb15(pic[sky_y][x]);
                    ref.r += c.r;
                    ref.g += c.g;
                    ref.b += c.b;
                }
                for (int x = 0; x < 3; x++) {
                    Rgb c = rgb15(pic[y][x]);
                    edge.r += c.r;
                    edge.g += c.g;
                    edge.b += c.b;
                }
                edge.r = edge.r / 3 - ref.r / 20;
                edge.g = edge.g / 3 - ref.g / 20;
                edge.b = edge.b / 3 - ref.b / 20;
                for (int j = 0; j < pad; j++) {
                    Rgb c = rgb15(pic[sky_y][300 + hash3(y, j, side) % 20]);
                    if (!side && j < 28 && y < 60 && !lf_fm[y][0]) {
                        c.r += edge.r * (28 - j) / 29;
                        c.g += edge.g * (28 - j) / 29;
                        c.b += edge.b * (28 - j) / 29;
                    }
                    lf_set(j, y, c);
                }
            } else
                for (int j = 0; j < pad; j++) /* the deck's top in the sun, as left of the hut */
                    lf_set(j, y,
                           y < 124 ? rgb15(pic[y][(y < 120 ? 1 : 0) + hash3(y, j / 3, side + 5) % (y < 120 ? 3 : 6)])
                                   : lf_ground(y, side ? j + 30 : -1 - j));
        }
        lf_foliage(side);
        if (!side) {
            for (int j = 0; j < 9; j++) { /* the eave to its corner */
                int eave_y = 64 + ((j + 1) * 85 + 50) / 100;
                for (int y = eave_y; y < 72; y++) lf_set(j, y, rgb15(pic[y][0]));
                if (eave_y < 72) lf_set(j, 72, rgb15(pic[72][1]));
            }
            for (int j = 0; j < 7; j++) { /* the brace's end */
                int drop = ((j + 1) * 45 + 50) / 100;
                for (int y = 75; y < 79; y++)
                    if (j < 6 || y == 76 || y == 77)
                        lf_set(j, y + drop, j < 5 ? rgb15(pic[y][0]) : scale(rgb15(pic[y][0]), 170));
            }
        } else {
            /* the jamb's missing columns; the table hides the left one's foot */
            for (int y = 52; y < LF_JF; y++)
                for (int j = 0; j < 6; j++) {
                    Rgb c = rgb15(pic[y < 146 ? y : y - 36][252 + j]);
                    if (y >= LF_JF - 3) c = scale(c, 256 - (y - (LF_JF - 4)) * 40);
                    if (j == 5) c = scale(c, 154);
                    lf_set(j, y, c);
                }
            /* (sand, not the deck's foot, under it) */
            for (int j = 0; j < 6; j++) lf_set(j, LF_JF, lf_ground(LF_SAND, j + 30));
            for (int y = LF_D; y < LF_JF; y++) /* the shed's shadow on the deck */
                for (int j = 6; j < 20; j++) {
                    int *p = lf_px(j, y),
                        shade = 256 - (20 - j) * 110 / 14;
                    p[0] = p[0] * shade / 256;
                    p[1] = p[1] * shade / 256;
                    p[2] = p[2] * shade / 256;
                }
            /* the deck's shadow on the sand along its foot (rows 176-187 of the carried-on sand) started as a straight
               edge at the jamb, next to the lit sand in front of the door: it comes in over LF_FEATHER columns, from
               the lit sand 12 rows further down (the user, 2026-11-17: "feather it on the left, where it starts") */
            for (int y = LF_JF; y < LF_SAND + 12; y++)
                for (int j = 0; j < LF_FEATHER; j++) {
                    Rgb lit = lf_door(lf_ground((y < LF_SAND ? LF_SAND : y) + 12, j + 30), y),
                        c;
                    int *p = lf_px(j, y),
                        wt = (j + 1) * 256 / (LF_FEATHER + 1);
                    wt = wt * wt / 256; /* (eased) */
                    c.r = p[0];
                    c.g = p[1];
                    c.b = p[2];
                    c = mix(lit, c, wt);
                    p[0] = c.r;
                    p[1] = c.g;
                    p[2] = c.b;
                }
        }
        for (int y = 0; y < PH; y++)
            for (int j = 0; j < pad; j++) {
                const int *p = lf_px(j, y);
                strip[(size_t)y * 2 * pad + (side ? pad + j : pad - 1 - j)] =
                    (u16)(clampi(p[0], 0, 248) >> 3 | (clampi(p[1], 0, 248) >> 3) << 5 |
                          (clampi(p[2], 0, 248) >> 3) << 10);
            }
    }
    free(lf_cv);
    free(lf_T);
    free(lf_Z);
    lf_cv = 0;
    lf_T = 0;
    lf_Z = 0;
}
