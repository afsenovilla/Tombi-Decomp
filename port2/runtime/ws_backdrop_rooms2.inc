/* runtime/ws_backdrop.c: more room generators (included after ws_backdrop_rooms.inc; docs/PORT.md "More rooms with
 * picture strips").  The same rules: every texel from the user's picture at run time, integers only, position
 * hashes, a structure depends on where it is and not on the strip width; the tables hold coordinates only. */

/* ================================================================== a 2-D quilter (rq_quilt)
 * Fills rows [y0, y1) of the canvas (cv, vb_ow columns, X = 1.. outwards) block by block: block columns from the join
 * outwards, each top to bottom, so a wider strip starts with the same blocks.  A block (BW x BH, plus OV columns of
 * overlap inwards and OT rows above) is a window of the source image rq_src (any position whose texels all pass
 * rq_ok, on a grid of 2), the one whose overlap with what is already there (the picture's columns at the join, given
 * by rq_ctx, or the canvas) differs least; among those within 1/8 of the best one by a position hash; windows near
 * where they go (a copy of the picture next to itself) or near the last ones used are penalised.  Pasted along
 * minimum-error cuts (vertical through the column overlap, horizontal through the row overlap); the texels are never
 * mirrored (a window keeps its left / right order).  rq_map gets each canvas texel's source (y * PW + x). */
static Rgb (*rq_src)[PW];                                /* the source (any space: raw texels or texels over light) */
static int (*rq_ok)(int x, int y);                      /* may a source texel be copied */
static int (*rq_ctx)(int side, int x, int y, Rgb *c);   /* the picture's texel at the join in rq_src's space (0: none) */
static u8 *rq_have;                                      /* canvas texels filled */
static int *rq_map;
static int rq_get(int side, int wx, int y, Rgb *c)
{
    if (y < 0 || y >= PH) return 0;
    if (wx >= 0 && wx < PW) return rq_ctx(side, wx, y, c);
    if (!vb_in(side, wx, y) || !rq_have[(size_t)y * vb_ow + vb_X(side, wx) - 1]) return 0;
    *c = *vb_px(side, wx, y); return 1;
}
typedef struct { short y0, y1, BW, BH, OV, OT, sx0, sx1, sy0, sy1, dy; } RqBand;   /* source box [sx0, sx1) x [sy0, sy1);
                                                            dy: windows at most dy rows off (-1: anywhere) */
static void rq_quilt(int side, const RqBand *b, int seed)
{
    int W = b->BW + b->OV, nbk = 0, used[4] = { -999, -999, -999, -999 };
    static int cand[(PW / 2 + 1) * (PH / 2 + 1)], ncand;
    static int em[VB_MAXH][VB_MAXW], path[VB_MAXH], tcut[VB_MAXW];
    static int errs[(PW / 2 + 1) * (PH / 2 + 1)];
    ncand = 0;
    for (int sy = b->sy0; sy + b->BH + b->OT <= b->sy1; sy += 2)
        for (int sx = b->sx0; sx + W <= b->sx1; sx += 2) {
            int ok = 1;
            for (int y = 0; y < b->BH + b->OT && ok; y++) for (int j = 0; j < W; j++) if (!rq_ok(sx + j, sy + y)) { ok = 0; break; }
            if (ok) cand[ncand++] = sy * PW + sx;
        }
    if (!ncand) return;
    for (int X0 = 1; X0 <= vb_ow; X0 += b->BW)
        for (int yb = b->y0; yb < b->y1; yb += b->BH) {
            int H = b->BH + b->OT, ytop = yb - b->OT, bh = yb + b->BH > b->y1 ? b->y1 - yb : b->BH;
            /* the window's picture columns: [wx0, wx0 + W), the new ones the BW outer ones */
            int wx0 = side ? PW - 1 + X0 - b->OV : -(X0 + b->BW - 1), mn = 1 << 30, nc = 0, k, pick = -1;
            for (int c = 0; c < ncand; c++) {
                int sx = cand[c] % PW, sy = cand[c] / PW, n = 0; long long e = 0;
                if (b->dy >= 0 && (sy - ytop > b->dy || ytop - sy > b->dy)) { errs[c] = -1; continue; }
                for (int y = 0; y < H; y++)
                    for (int j = 0; j < W; j++) {
                        int ov = side ? j < b->OV : j >= b->BW; Rgb o, s;
                        if (!ov && y >= b->OT) continue;
                        if (!rq_get(side, wx0 + j, ytop + y, &o)) continue;
                        s = rq_src[sy + y][sx + j];
                        e += vb_d2(o, s); n++;
                    }
                e = n ? e * 64 / n : 0;
                {   int d = sx - wx0; if (d < 0) d = -d;
                    if (d < 64) e += e / 2 + 2000;           /* (no copy of the picture next to itself) */
                    for (int u = 0; u < 4; u++) { int dx = sx - used[u] % PW, dy = sy - used[u] / PW; if (dx < 0) dx = -dx; if (dy < 0) dy = -dy;
                        if (used[u] >= 0 && dx < W && dy < H) e += e / 2 + 2000; }
                }
                errs[c] = (int)(e > (1 << 29) ? (1 << 29) : e);
                if (errs[c] < mn) mn = errs[c];
            }
            for (int c = 0; c < ncand; c++) if (errs[c] >= 0 && errs[c] <= mn + mn / 8) nc++;
            if (!nc) { nbk++; continue; }
            k = (int)(hash3(side * 977 + nbk, X0, seed) % (u32)nc);
            for (int c = 0; c < ncand && pick < 0; c++) if (errs[c] >= 0 && errs[c] <= mn + mn / 8 && k-- == 0) pick = c;
            {   int sx = cand[pick] % PW, sy = cand[pick] / PW, j0 = side ? 0 : b->BW, vc[VB_MAXH];
                used[nbk % 4] = cand[pick];
                for (int y = 0; y < H; y++) for (int j = 0; j < W; j++) { Rgb o; em[y][j] = rq_get(side, wx0 + j, ytop + y, &o) ? vb_d2(o, rq_src[sy + y][sx + j]) : 0; }
                {   static int ev[VB_MAXH][VB_MAXW];
                    for (int y = 0; y < H; y++) for (int j = 0; j < b->OV; j++) ev[y][j] = em[y][j0 + j];
                    vb_cutpath(ev, H, b->OV, 1, path);
                    for (int y = 0; y < H; y++) vc[y] = j0 + path[y];
                }
                for (int j = 0; j < W; j++) tcut[j] = 0;
                if (b->OT > 0) vb_cutpath(em, b->OT, W, 0, tcut);
                for (int y = 0; y < b->OT + bh; y++)
                    for (int j = 0; j < W; j++) {
                        int tx = wx0 + j, yy = ytop + y; Rgb o;
                        if ((tx >= 0 && tx < PW) || yy < 0 || yy >= PH || !vb_in(side, tx, yy)) continue;
                        if (side ? j < vc[y] : j > vc[y]) { if (rq_get(side, tx, yy, &o)) continue; }   /* (the old side of the cut) */
                        if (y < tcut[j] && rq_get(side, tx, yy, &o)) continue;
                        {   size_t i = (size_t)yy * vb_ow + vb_X(side, tx) - 1;
                            cv[i] = rq_src[sy + y][sx + j]; rq_have[i] = 1;
                            if (rq_map) rq_map[i] = (sy + y) * PW + sx + j;
                        }
                    }
            }
            nbk++;
        }
}

/* ================================================================== AREA09 s4: the rock wall with the stair down to
 * the right (gen_rw)
 * A rock wall lit by five sconces (each on a rock mound, the mounds in a row along the stair, one every (50, 30.5)),
 * crossed by a straight stair from the top left to the bottom right (rail, balusters, red treads, the marble stringer)
 * in perspective (the balusters 19.5 columns apart at the left edge, 25 at the right one).  Measured: the rail's top
 * row yr(x) = 9.6 + 0.6167 x, the stringer's lower edge 71.6 + 0.032 x rows under it; one step along the rail is
 * (20.4, 12.6) at the left edge and (24.5, 15.1) at the right one (2-D matches of the stair band against itself).
 *   - the wall: quilted (rq_quilt) on the texels over their light (the light: the wall's luminance averaged over
 *     17 x 17, stair and sconces out), so a window from the glow and one from the shade join by their texture, then
 *     lit with the light wanted where it lands.  On the right (above the stair) the windows come from the wall over
 *     the row of mounds only (more than 52 rows over the rail, the sconces and their glow's core out): no mound or
 *     sconce is copied, so the strip has none; the light wanted is the edge column's light of the row with the last
 *     sconce's glow falling off (flame (283, 125)), and in the mound cut by the edge (its rows at the edge) eased
 *     over 24 columns to the wall's light over it, so that mound closes.  On the left the windows come from the
 *     rock under the stringer, lit as the edge column where the texel's line parallel to the stair meets it (the
 *     shadow under the stringer runs along it); then its contrast per row matched to the picture's next to the edge
 *     (rw_contrast);
 *   - the stair: the picture's own stair k steps further in (k the fewest that land 3 columns inside), its rows below
 *     the rail scaled by the band's widths there and here, so rail, balusters, treads and the stringer's edges go on
 *     along their lines (the stringer's face cross-faded where k changes; rw_src);
 *   - the join: per row, the wall's first 8 columns' mean onto the picture's last 8 (fading by 40) and its first column
 *     onto the picture's last one (fading over 6).
 * All of it from the picture as the renderer shows it: the stair's quads span a texel less than their pixels, so the
 * right edge's stair is up to a column / row off the picture there (th_shown, gen_th's model). */
static int rw_yr16(int x16) { return 154 + fdiv(x16 * 9867, 16000); }   /* the rail's top row (x 16) at
                                                            column x16 / 16 */
static int rw_h16(int x16) { return 1146 + fdiv(x16 * 32, 1000); }   /* the band's height (x 16) */
static int rw_t16(int x, int y) { return y * 16 + 8 - rw_yr16(x * 16 + 8); }        /* rows (x 16) under the rail top */
static int rw_band(int x, int y) { int t = rw_t16(x, y); return t >= -16 && t < rw_h16(x * 16 + 8) + 16; }
static const short RW_FL[5][2] = { { 80, 3 }, { 129, 33 }, { 180, 62 }, { 232, 93 }, { 283, 125 } };   /* the flames */
static int rw_sconce(int x, int y)                       /* a sconce or its glow's core */
{
    for (int i = 0; i < 5; i++) { int dx = x - RW_FL[i][0], dy = y - RW_FL[i][1]; if (dx * dx + dy * dy < 22 * 22 || (dx * dx < 100 && dy > 0 && dy < 22)) return 1; }
    return 0;
}
static short rw_lf[PH][PW];                              /* the wall's light (0: no wall texel near) */
static Rgb rw_alb[PH][PW];                               /* the picture's texels over their light (x 128) */
static int rw_le[PH];                                    /* the light at the edge per row (the side being made) */
static int rw_okR(int x, int y)
{
    return x >= 0 && x < PW && y >= 0 && y < PH && rw_lf[y][x] && !rw_sconce(x, y) && rw_t16(x, y) < -52 * 16;
}
static int rw_okL(int x, int y)
{
    return x >= 0 && x < PW && y >= 0 && y < PH && rw_lf[y][x] && !rw_sconce(x, y) && rw_t16(x, y) > rw_h16(x * 16 + 8) + 3 * 16;
}
static int rw_ctx(int side, int x, int y, Rgb *c)
{
    (void)side;
    if (!rw_lf[y][x] || rw_sconce(x, y) || rw_band(x, y)) return 0;
    *c = rw_alb[y][x]; return 1;
}
static void rw_light(void)
{
    static int S[PH + 1][PW + 1], C[PH + 1][PW + 1];
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int m = !rw_band(x, y) && !rw_sconce(x, y);
            S[y + 1][x + 1] = S[y][x + 1] + S[y + 1][x] - S[y][x] + (m ? lum(rgb15(pic[y][x])) : 0);
            C[y + 1][x + 1] = C[y][x + 1] + C[y + 1][x] - C[y][x] + m;
        }
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int x0 = x - 8 < 0 ? 0 : x - 8, x1 = x + 9 > PW ? PW : x + 9, y0 = y - 8 < 0 ? 0 : y - 8, y1 = y + 9 > PH ? PH : y + 9;
            int s = S[y1][x1] - S[y0][x1] - S[y1][x0] + S[y0][x0], c = C[y1][x1] - C[y0][x1] - C[y1][x0] + C[y0][x0];
            Rgb k = rgb15(pic[y][x]); int l;
            rw_lf[y][x] = (short)(c >= 16 ? (s / c > 4 ? s / c : 4) : 0);
            l = rw_lf[y][x] ? rw_lf[y][x] : 64;
            rw_alb[y][x].r = k.r * 128 / l; rw_alb[y][x].g = k.g * 128 / l; rw_alb[y][x].b = k.b * 128 / l;
        }
}
static void rw_edge(int side)                            /* rw_le: the light of the 4 edge columns per row */
{
    for (int y = 0; y < PH; y++) {
        int s = 0, c = 0;
        for (int i = 0; i < 4; i++) { int x = side ? PW - 1 - i : i; if (!rw_band(x, y) && !rw_sconce(x, y) && rw_lf[y][x]) { s += rw_lf[y][x]; c++; } }
        rw_le[y] = c ? s / c : -1;
    }
    for (int y = PH - 2; y >= 0; y--) if (rw_le[y] < 0) rw_le[y] = rw_le[y + 1];
    for (int y = 1; y < PH; y++) if (rw_le[y] < 0) rw_le[y] = rw_le[y - 1];
}
#define RW_MR0 138                                       /* the mound cut by the right edge: its rows there */
#define RW_MR1 PH
#define RW_ME 24                                         /* columns over which it closes */
static int rw_wall_r;                                    /* the wall's light over that mound at the edge */
static int rw_want(int side, int X, int y)
{
    if (side) {
        int fx = RW_FL[4][0], fy = RW_FL[4][1], dx = PW - 1 + X - fx, ex = PW - 1 - fx, dy = y - fy, S2 = 30 * 30, l = rw_le[y], amb = rw_wall_r * 7 / 8;
        if (y >= RW_MR0 && y < RW_MR1 && l < rw_wall_r) {          /* (the mound closes: its shade eased away) */
            int w = X >= RW_ME ? 256 : X * 256 / RW_ME;
            w = w * w / 256 * (3 * 256 - 2 * w) / 256;
            l += (rw_wall_r - l) * w / 256;
        }
        if (l > amb) l = amb + (int)((long long)(l - amb) * (S2 + ex * ex + dy * dy) / (S2 + dx * dx + dy * dy));
        return l;
    } else {                                             /* (along the stair to the edge column) */
        int ye = y + fdiv(X * 101, 160);                 /* (the stringer's slope, 0.63) */
        if (rw_t16(-X, y) < -16) return rw_le[0];        /* (over the rail: the wall above it at the edge) */
        return rw_le[clampi(ye, 0, PH - 1)];
    }
}
/* the stair's texel for strip column X of side `side`, row y, from k steps further in: x and the rows under the rail
   scaled by the band's widths (x 16 arithmetic) */
/* left: the source column is the texel the shifted position falls in, the row taken at that column's own rail line
   (+3/16 row: the rail's top measured at the join) linear between the two nearest rows, so the rail and the stringer's
   edges land within 0.25 row of their continuation (nearest texels: up to 0.5 off).  Right: nearest texels (the
   picture there is the stretched quad as shown; interpolated, the rail came out 0.7 row thicker at the join) */
#define RW_RAIL (7 * 16)                                 /* the rail's rows under its top */
static int rw_src(int x16, int t, int xs16, Rgb *o)
{
    int ts = t > 0 ? (int)((long long)t * rw_h16(xs16) / rw_h16(x16)) : t, xs = fdiv(xs16, 16), yc, ys, f = 0;
    if (xs16 > x16) {
        yc = rw_yr16(xs * 16 + 8) + ts - 5;
        if (t < RW_RAIL) ys = fdiv(yc + 8, 16);           /* (the rail: nearest rows; blended, its 1-row highlight
                                                            came out dimmer than the picture's at the join) */
        else { ys = fdiv(yc, 16); f = yc - ys * 16; }
    }
    else ys = fdiv(rw_yr16(xs16) + ts, 16);
    if (xs < 0 || xs >= PW || ys < 0 || ys >= PH) return 0;
    *o = rgb15(pic[ys][xs]);
    if (f && ys + 1 < PH) *o = mix(*o, rgb15(pic[ys + 1][xs]), f * 16);
    return 1;
}
#define RW_FACE (34 * 16)                                /* the stringer's face: rows under the rail from here */
#define RW_XF (6 * 16)                                   /* where k changes, the face's copies cross-faded over this */
static int rw_stair(int side, int X, int y, Rgb *o)
{
    int x16 = (side ? PW - 1 + X : -X) * 16 + 8, st = side ? -392 : 326, t = y * 16 + 8 - rw_yr16(x16), k, room = 0;
    if (t < -16 || t >= rw_h16(x16) + 16) return 0;
    for (k = 1; k < 8; k++) {                            /* the fewest steps that land 3 columns inside */
        room = side ? (PW - 4) * 16 - (x16 + st * k) : x16 + st * k - 3 * 16;
        if (room >= 0) break;
    }
    if (k == 8 || !rw_src(x16, t, x16 + st * k, o)) return 0;
    if (t >= RW_FACE && room < RW_XF) {                  /* (the marble's shading is not periodic: no seam where k
                                                            changes) */
        Rgb q;
        if (rw_src(x16, t, x16 + st * (k + 1), &q)) *o = mix(q, *o, room * 256 / RW_XF);
    }
    return 1;
}
static void rw_join(int side)                            /* the wall at the join (stair texels left alone) */
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
                    if (rw_band(x, yy) || rw_sconce(x, yy) || rw_band(xo, yy)) continue;
                    a = cv[(size_t)yy * vb_ow + i]; s[1][0] += a.r; s[1][1] += a.g; s[1][2] += a.b; n[1]++;
                    a = rgb15(pic[yy][x]); s[0][0] += a.r; s[0][1] += a.g; s[0][2] += a.b; n[0]++;
                }
            }
            for (int c = 0; c < 3; c++) jd[y][st][c] = n[0] && n[1] ? (s[0][c] / n[0] - s[1][c] / n[1]) * (st ? 3 : 4) / 4 : 0;
        }
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= (st ? 6 : 40); X++) {
                Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int w = st ? (7 - X) * 256 / 6 : X <= 8 ? 256 : (40 - X) * 256 / 32;
                if (rw_band(side ? PW - 1 + X : -X, y)) continue;
                p->r = clampi(p->r + jd[y][st][0] * w / 256, 0, 255); p->g = clampi(p->g + jd[y][st][1] * w / 256, 0, 255);
                p->b = clampi(p->b + jd[y][st][2] * w / 256, 0, 255);
            }
    }
}
/* the wall's contrast per row (rows y +- 16): the spread of the strip's luminance (its first 40 columns) onto the
   picture's (its 32 columns next to the edge), stair and sconces out, at most x 0.8 .. x 1.6 */
static void rw_contrast(int side)
{
    static int k256[PH];
    for (int y = 0; y < PH; y++) {
        long long s[2] = { 0, 0 }, q[2] = { 0, 0 }; int n[2] = { 0, 0 };
        for (int yy = y - 16; yy <= y + 16; yy++) {
            if (yy < 0 || yy >= PH) continue;
            for (int i = 0; i < 40; i++) {
                int x = side ? PW - 1 - i : i, l;
                if (i < 32 && !rw_band(x, yy) && !rw_sconce(x, yy)) { l = lum(rgb15(pic[yy][x])); s[0] += l; q[0] += l * l; n[0]++; }
                if (!rw_band(side ? PW + i : -1 - i, yy)) { l = lum(cv[(size_t)yy * vb_ow + i]); s[1] += l; q[1] += l * l; n[1]++; }
            }
        }
        k256[y] = 256;
        if (n[0] > 64 && n[1] > 64) {
            long long v0 = q[0] / n[0] - (s[0] / n[0]) * (s[0] / n[0]), v1 = q[1] / n[1] - (s[1] / n[1]) * (s[1] / n[1]);
            if (v0 > 0 && v1 > 0) k256[y] = clampi(isqrt_i((int)(v0 * 65536 / v1)), 205, 410);
        }
    }
    for (int y = 0; y < PH; y++) {
        int m = 0, c = 0;
        for (int X = 1; X <= 40; X++) if (!rw_band(side ? PW - 1 + X : -X, y)) { m += lum(cv[(size_t)y * vb_ow + X - 1]); c++; }
        if (!c) continue;
        m /= c;
        for (int X = 1; X <= vb_ow; X++) {
            Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int l = lum(*p), t;
            if (l <= 0) continue;
            t = m + (l - m) * k256[y] / 256; if (t < 0) t = 0;
            p->r = clampi(p->r * t / l, 0, 255); p->g = clampi(p->g * t / l, 0, 255); p->b = clampi(p->b * t / l, 0, 255);
        }
    }
}
static u16 rw_raw[PH][PW];                               /* the picture, given back to pic at the end */
static const RqBand RW_BR ={ 0, PH, 16, 28, 6, 6, 0, PW, 0, PH, -1 };
static const RqBand RW_BL = { 0, PH, 16, 28, 6, 6, 0, PW, 0, PH, -1 };
static void gen_rw(int pad)
{
    pal_make();
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    rq_have = (u8 *)malloc((size_t)vb_ow * PH);
    if (!rq_have) { canvas_close(); return; }
    memcpy(th_p0, pic, sizeof th_p0); th_shown();       /* the picture as shown (the stair's quads: gen_th's th_shown) */
    memcpy(rw_raw, pic, sizeof rw_raw); memcpy(pic, th_p0, sizeof pic);
    rw_light();
    rq_src = rw_alb; rq_ctx = rw_ctx; rq_map = 0;
    for (int side = 0; side < 2; side++) {
        canvas_clear(); cur_side = side; memset(rq_have, 0, (size_t)vb_ow * PH);
        rw_edge(side);
        if (side) {                                      /* the wall's light over the mound at the edge */
            int s = 0, n = 0;
            for (int y = RW_MR0 - 30; y < RW_MR0 - 4; y++) { s += rw_le[y]; n++; }
            rw_wall_r = n ? s / n : 128;
        }
        rq_ok = side ? rw_okR : rw_okL;
        rq_quilt(side, side ? &RW_BR : &RW_BL, 4901 + side);
        for (int y = 0; y < PH; y++)                     /* lit */
            for (int X = 1; X <= vb_ow; X++) {
                Rgb *p = &cv[(size_t)y * vb_ow + X - 1]; int l = rw_want(side, X, y);
                p->r = clampi(p->r * l / 128, 0, 255); p->g = clampi(p->g * l / 128, 0, 255); p->b = clampi(p->b * l / 128, 0, 255);
            }
        rw_contrast(side);
        rw_join(side);
        for (int y = 0; y < PH; y++)                     /* the stair */
            for (int X = 1; X <= vb_ow; X++) { Rgb o; if (rw_stair(side, X, y, &o)) cv[(size_t)y * vb_ow + X - 1] = o; }
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(cv[(size_t)y * vb_ow + X - 1]);
    }
    memcpy(pic, rw_raw, sizeof pic);
    free(rq_have); rq_have = 0;
    canvas_close();
}
