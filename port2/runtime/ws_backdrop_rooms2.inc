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
static int (*rq_quilt__cand);
static int (*rq_quilt__em)[VB_MAXW];
static int (*rq_quilt__errs);
static int (*rq_quilt__ev)[VB_MAXW];
static void rq_quilt(int side, const RqBand *b, int seed)
{
    int W = b->BW + b->OV, nbk = 0, used[4] = { -999, -999, -999, -999 };
    static int ncand; int (*const cand) = rq_quilt__cand;
    static int path[VB_MAXH], tcut[VB_MAXW]; int (*const em)[VB_MAXW] = rq_quilt__em;
    int (*const errs) = rq_quilt__errs;
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
                {   int (*const ev)[VB_MAXW] = rq_quilt__ev;
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
static short (*rw_lf)[PW];                              /* the wall's light (0: no wall texel near) */
static Rgb (*rw_alb)[PW];                               /* the picture's texels over their light (x 128) */
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
static int (*rw_light__S)[PW + 1];
static int (*rw_light__C)[PW + 1];
static void rw_light(void)
{
    int (*const S)[PW + 1] = rw_light__S, (*const C)[PW + 1] = rw_light__C;
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
static int (*rw_join__jd)[2][3];
static void rw_join(int side)                            /* the wall at the join (stair texels left alone) */
{
    int (*const jd)[2][3] = rw_join__jd;
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
static u16 (*rw_raw)[PW];                               /* the picture, given back to pic at the end */
static const RqBand RW_BR ={ 0, PH, 16, 28, 6, 6, 0, PW, 0, PH, -1 };
static const RqBand RW_BL = { 0, PH, 16, 28, 6, 6, 0, PW, 0, PH, -1 };
static void gen_rw(int pad)
{
    if (!ws_need_gen_rw()) return;
    pal_make();
    vb_ow = pad + 64;
    if (!canvas_open(vb_ow)) return;
    rq_have = (u8 *)malloc((size_t)vb_ow * PH);
    if (!rq_have) { canvas_close(); return; }
    memcpy(th_p0, pic, (sizeof *th_p0 * (PH))); th_shown();       /* the picture as shown (the stair's quads: gen_th's th_shown) */
    memcpy(rw_raw, pic, (sizeof *rw_raw * (PH))); memcpy(pic, th_p0, (sizeof *pic * (PH)));
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
    memcpy(pic, rw_raw, (sizeof *pic * (PH)));
    free(rq_have); rq_have = 0;
    canvas_close();
}

/* ================================================================== AREA16 s4: the green tower (gen_tw)
 * The picture is 672 rows (TW_H): three sets of sprites, each a 256-column part and a 64-column one, with CLUT
 * (128, 511): rows 0-255 from pages (128, 0) and (512, 0) u 0-63, rows 256-511 from pages (256, 0) and (512, 0)
 * u 64-127, rows 512-671 from page (384, 0) rows 0-159 and, for the right 64 columns, (384, 0) u 0-63 / 64-127 rows
 * 160-239 (80 rows each).  The game draws all three sets every frame, shifted by the camera's height (the set's left
 * sprite at frame row T + 256 k, T the picture's top row on the frame); tw_take() fills the frame rows 16-239 of the
 * strips from a 672-row strip made once per picture. */
#define TW_H 672
static u16 (*tw_pic)[PW];
static const Piece TW_P[7] = { { 128, 0, 0, 256, 256, 0, 0 }, { 512, 0, 0, 64, 256, 256, 0 }, { 256, 0, 0, 256, 256, 0, 256 },
    { 512, 64, 0, 64, 256, 256, 256 }, { 384, 0, 0, 256, 160, 0, 512 }, { 384, 0, 160, 64, 80, 256, 512 },
    { 384, 64, 160, 64, 80, 256, 592 } };
static u16 *tw_strip; static int tw_pad; static u32 tw_sum;
static u16 tex8(const u16 *vram, int tx, int u, int v, int cx, int cy);
static void gen_tw(int pad) { (void)pad; }              /* (the table's gen: tw_build makes the strips) */
static void tw_build(int pad);
static u32 tw_grab(const u16 *vram, int cx, int cy)
{
    u32 h = 2166136261u;
    for (int i = 0; i < 7; i++) {
        const Piece *p = &TW_P[i];
        for (int y = 0; y < p->h; y++) for (int x = 0; x < p->w; x++) tw_pic[p->y + y][p->x + x] = tex8(vram, p->tx, p->u + x, p->v + y, cx, cy);
    }
    for (int y = 0; y < TW_H; y++) for (int x = 0; x < PW; x++) h = (h ^ tw_pic[y][x]) * 16777619u;
    return h;
}
#ifndef WS_BACKDROP_MAIN
int port_ws_backdrop_dy;                                 /* shims/gpu.c: the strips' first row against the sprite's */
extern int port_frame;
/* the sprite of set k (page 128 / 256 / 384) at frame row fy: the strips' rows for frame rows 16-239 */
static const u16 *tw_take(const u16 *vram, int pad, int fy, int tx, int *rows)
{
    if (!ws_need_tw_take()) return 0;
    static int last = -1;
    int T = fy - 256 * (tx / 128 - 1), f0 = T > 16 ? T : 16, f1 = T + TW_H < 240 ? T + TW_H : 240;
    if (last != port_frame || !tw_strip || tw_pad != pad) {
        u32 s = tw_grab(vram, 128, 511);
        last = port_frame;
        {   Rgb a = rgb15(tw_pic[8][2]), b = rgb15(tw_pic[8][317]);   /* (the sky in both top corners: this picture) */
            if (a.b < a.r + 64 || b.b < b.r + 64) { free(tw_strip); tw_strip = 0; return 0; }
        }
        if (!tw_strip || tw_pad != pad || s != tw_sum) {
            tw_sum = s; tw_build(pad);
            {   const char *e = getenv("PORT_WS_BACKDROP_DUMP");      /* research: the picture, raw 15-bit LE, 672 rows */
                FILE *f = e ? fopen(e, "wb") : 0;
                if (f) {
                    for (int yy = 0; yy < TW_H; yy++) for (int xx = 0; xx < PW; xx++) { u8 b[2] = { (u8)tw_pic[yy][xx], (u8)(tw_pic[yy][xx] >> 8) }; fwrite(b, 1, 2, f); }
                    fclose(f);
                }
            }
        }
    }
    if (!tw_strip || f1 <= f0) return 0;
    port_ws_backdrop_dy = f0 - fy; *rows = f1 - f0;
    return tw_strip + (size_t)(f0 - T) * 2 * pad;
}
#endif
/* --- the generator.  Labels of the picture's texels (smoothed over 5 x 5): sky, cloud, foliage, ground (foliage-
   coloured texels from TW_G0 down); the tower, its bushes, path and the trees' feet are no source (TW_EX). */
enum { TL_OTHER, TL_SKY, TL_CLOUD, TL_FOL, TL_GROUND };
#define TW_Y0 288                                        /* the quilted part's sources: rows from here */
#define TW_G0 641                                        /* the grass from this row (the trees' foot at both edges) */
static const short TW_EX[][4] = { { 72, 248, 0, TW_H }, { 48, 272, 618, TW_H }, { 12, 50, 634, 654 }, { 256, 301, 634, 654 } };   /* x0 x1 y0 y1 */
static u8 (*tw_lab)[PW];
static Rgb (*tw_rgb)[PW];
static int (*tw_bad)[PW + 1];                     /* integral image of the texels that are no source */
static short (*tw_gs)[PW];                            /* the foliage's light: 9 x 9 mean luminance of its texels, -1 */
static int tw_cls(Rgb k, int y)
{
    if (k.b >= k.g + 48 && k.b >= k.r + 64) return TL_SKY;
    if (k.b >= k.r && k.b >= k.g && k.r >= 150) return TL_CLOUD;
    if (k.g > k.r && k.g >= k.b) return y >= TW_G0 ? TL_GROUND : TL_FOL;
    return TL_OTHER;
}
static int (*tw_analyse__cnt)[TW_H + 1][PW + 1];
static int (*tw_analyse__sl)[PW + 1];
static int (*tw_analyse__sn)[PW + 1];
static void tw_analyse(void)
{
    int (*const cnt)[TW_H + 1][PW + 1] = tw_analyse__cnt;                 /* integral images per label */
    for (int y = 0; y < TW_H; y++) for (int x = 0; x < PW; x++) tw_rgb[y][x] = rgb15(tw_pic[y][x]);
    memset(cnt, 0, (sizeof *cnt * (5)));
    for (int y = 0; y < TW_H; y++)
        for (int x = 0; x < PW; x++) {
            int l = tw_cls(tw_rgb[y][x], y);
            for (int k = 0; k < 5; k++) cnt[k][y + 1][x + 1] = cnt[k][y][x + 1] + cnt[k][y + 1][x] - cnt[k][y][x] + (k == l);
        }
    for (int y = 0; y < TW_H; y++)
        for (int x = 0; x < PW; x++) {
            int x0 = x - 2 < 0 ? 0 : x - 2, x1 = x + 3 > PW ? PW : x + 3, y0 = y - 2 < 0 ? 0 : y - 2, y1 = y + 3 > TW_H ? TW_H : y + 3, best = 0, bn = -1;
            for (int k = 0; k < 5; k++) {
                int n = cnt[k][y1][x1] - cnt[k][y0][x1] - cnt[k][y1][x0] + cnt[k][y0][x0];
                if (n > bn) { bn = n; best = k; }
            }
            tw_lab[y][x] = (u8)best;
        }
    {   int (*const sl)[PW + 1] = tw_analyse__sl, (*const sn)[PW + 1] = tw_analyse__sn;
        for (int y = 0; y < TW_H; y++)
            for (int x = 0; x < PW; x++) {
                int f = tw_lab[y][x] == TL_FOL;
                sl[y + 1][x + 1] = sl[y][x + 1] + sl[y + 1][x] - sl[y][x] + (f ? lum(tw_rgb[y][x]) : 0);
                sn[y + 1][x + 1] = sn[y][x + 1] + sn[y + 1][x] - sn[y][x] + f;
            }
        for (int y = 0; y < TW_H; y++)
            for (int x = 0; x < PW; x++) {
                int x0 = x - 4 < 0 ? 0 : x - 4, x1 = x + 5 > PW ? PW : x + 5, y0 = y - 4 < 0 ? 0 : y - 4, y1 = y + 5 > TW_H ? TW_H : y + 5;
                int n = sn[y1][x1] - sn[y0][x1] - sn[y1][x0] + sn[y0][x0];
                tw_gs[y][x] = (short)(n >= 12 ? (sl[y1][x1] - sl[y0][x1] - sl[y1][x0] + sl[y0][x0]) / n : -1);
            }
    }
    for (int y = 0; y < TW_H; y++)
        for (int x = 0; x < PW; x++) {
            int bad = y < TW_Y0 || tw_lab[y][x] == TL_OTHER;
            for (int i = 0; i < (int)(sizeof TW_EX / sizeof TW_EX[0]) && !bad; i++)
                bad = x >= TW_EX[i][0] && x < TW_EX[i][1] && y >= TW_EX[i][2] && y < TW_EX[i][3];
            tw_bad[y + 1][x + 1] = tw_bad[y][x + 1] + tw_bad[y + 1][x] - tw_bad[y][x] + bad;
        }
}
static int tw_okwin(int sx, int sy, int w, int h)
{
    if (sx < 0 || sy < 0 || sx + w > PW || sy + h > TW_H) return 0;
    return tw_bad[sy + h][sx + w] - tw_bad[sy][sx + w] - tw_bad[sy + h][sx] + tw_bad[sy][sx] == 0;
}

/* --- the clouds' top in the strips: a chain of round lumps, the first one through the picture's outline at the edge
   (its row and slope there), the next ones outwards by position hashes */
typedef struct { short c, t, r; } TwLump;                /* centre column X, top row, radius */
#define TW_NL 24
static TwLump tw_lumps[2][TW_NL];
static const short TW_FIRST[2][3] = {                    /* the first lump (X centre, top, radius), measured */
    { -5, 360, 26 },                                     /* left: the cloud peaks at x 4 */
    { -34, 389, 60 } };                                  /* right: the cloud falls outwards (400 at x 319, 0.7 a column) */
static void tw_outline_make(int side)
{
    TwLump *L = tw_lumps[side];
    L[0].c = TW_FIRST[side][0]; L[0].t = TW_FIRST[side][1]; L[0].r = TW_FIRST[side][2];
    for (int i = 1; i < TW_NL; i++) {
        u32 h = hash3(side * 31, i, 7151);
        int r = 16 + (int)(h % 19), gap = (L[i - 1].r + r) * 10 / 16;   /* radius 16-34, overlapping: a notch */
        L[i].r = (short)r; L[i].c = (short)(L[i - 1].c + gap);
        if (L[i].c < r + 10) L[i].c = (short)(r + 10);   /* (none reaches into the join) */
        L[i].t = (short)(344 + (int)((h >> 8) % 36));
    }
}
/* the outline per column, its slope limited to 1.5 rows a column (no sheer side where a lump meets a lower one), x 2 */
#define TW_OLW 512
static short (*tw_ol);
static void tw_outline_cols(int side)
{
    for (int X = 0; X < TW_OLW; X++) {
        int best = TW_H;
        for (int i = 0; i < TW_NL; i++) {
            const TwLump *l = &tw_lumps[side][i]; int d = X - l->c, y;
            if (d * d >= l->r * l->r) continue;
            y = l->t + l->r - isqrt_i(l->r * l->r - d * d);
            if (y < best) best = y;
        }
        tw_ol[X] = (short)(2 * best);
    }
    for (int X = 1; X < TW_OLW; X++) if (tw_ol[X] < tw_ol[X - 1] - 3) tw_ol[X] = (short)(tw_ol[X - 1] - 3);
    for (int X = TW_OLW - 2; X >= 0; X--) if (tw_ol[X] < tw_ol[X + 1] - 3) tw_ol[X] = (short)(tw_ol[X + 1] - 3);
}
static int tw_outline(int X) { return tw_ol[clampi(X, 0, TW_OLW - 1)] / 2; }   /* the clouds' top row at strip column X */

/* --- the trees in the strips, back to front per side (strip column X of the trunk's axis, X <= 0 inside the picture;
   the crown's tip row, its widest half-width W, rows from the tip to the widest row, the foot row; the picture's trunk
   column whose texels make its trunk, -1: hidden; the foot's reach either side).  The crown's half-width at d rows
   under the tip: W (2t - t^2), t = d / dm (a point at the tip, 2W/dm columns a row), then from the widest row
   narrowing by a third to the foot (the foliage hangs to the ground, as in the picture); 14 lumps of 6-11 rows on each
   flank, -3..+6 columns (the leaf clumps of the picture's outlines), none in the 14 rows at the tip.  Measured, the picture's trees: left, the back tree cut by the
   edge (its top 442-447 at x 0-10, falling inwards: 450 at x 10, 462 at 13, 474 at 23) and the front tree (trunk axis
   x 33, its top a dome of rows 484-488 at x 24-50; its right flank 503 at x 62, 520 at 70, 547 at 84, 564 at 96: W 62,
   dm 100); right, the tall tree (tip x 314 row 414, 419 at x 318: the spire is 8-10 columns wide at the join; its left
   flank 421 at x 310, 455-460 at 302-308, 484 at 300: W 26, dm 170) and the front tree (trunk axis x 284, dome 484-488 at x 266-300; its left flank 506 at x 250, 526 at 246,
   550 at 240, 568 at 232).  Both front trees' crowns hang to the grey band (row 641); their trunks show as a dark
   core 7-10 columns wide between the leaves (rows ~576-640: left x 29-38, right x 276-291) and a dark brown foot on the
   grey band (rows 641-648: left x 16-52, right x 258-302). */
typedef struct { short c, tip, W, dm, bot, src, hf; } TwTree;
#define TW_NT 3
static const TwTree TW_T[2][TW_NT] = {
    {   { 8, 428, 34, 140, 641, 284, 9 },                /* the back tree, completed */
        { 60, 482, 34, 112, 641, 33, 12 },               /* one more tree, shorter (a V of sky between the tips) */
        { -37, 484, 38, 100, 641, -1, 0 } },             /* the picture's front tree (its outer half) */
    {   { 0, 412, 26, 170, 641, -1, 0 },                 /* the tall tree, completed (its trunk behind the front one) */
        { 42, 466, 33, 122, 641, 33, 22 },               /* one more tree, broader, a longer point */
        { -35, 484, 58, 100, 641, -1, 0 } } };           /* the picture's front tree */
static int tw_halfw(int side, int i, int y, int fl)      /* the crown's half-width on row y (fl 1: the outer flank) */
{
    const TwTree *t = &TW_T[side][i]; int d = y - t->tip, w;
    if (d < 0 || y >= t->bot) return -1;
    if (d <= t->dm) w = t->W * (2 * d * t->dm - d * d) / (t->dm * t->dm);
    else { int L = t->bot - t->tip - t->dm, e = d - t->dm; w = t->W - t->W * e * e / (3 * L * L); }
    if (d >= 14)
        for (int k = 0; k < 14; k++) {                   /* the lumps */
            u32 h = hash3(side * 8 + i, k * 2 + fl, 4421);
            int L = 6 + (int)(h % 6), yc = t->tip + 20 + k * 16 + (int)((h >> 8) % 9), a = (int)((h >> 12) % 10) - 3, dy = y - yc;
            if (dy * dy < L * L) w += a * (L * L - dy * dy) * (d < 60 ? d : 60) / (L * L * 60);
        }
    return w < 0 ? 0 : w;
}
static int tw_owner(int side, int X, int y)               /* the front-most crown at (X, y), -1: none */
{
    for (int i = TW_NT - 1; i >= 0; i--) {
        int dx = X - TW_T[side][i].c, w = tw_halfw(side, i, y, dx > 0);
        if (w >= 0 && (dx < 0 ? -dx : dx) <= w) return i;
    }
    return -1;
}
/* the crowns' light, measured on the picture's two front trees (the foliage's luminance, 9 x 9 means): rows of the
   crown in eighths from the tip, columns from the outer flank to the flank towards the tower; the top lit, the lower
   half in shade, the side towards the tower lighter */
static const u8 TW_LIGHT[8][5] = {
    { 110, 125, 138, 140, 132 }, { 80, 92, 112, 130, 128 }, { 66, 72, 84, 110, 112 }, { 70, 66, 64, 80, 92 },
    { 54, 52, 50, 60, 72 }, { 60, 56, 50, 52, 56 }, { 62, 62, 57, 52, 58 }, { 68, 66, 60, 66, 74 } };
static int tw_light(int side, int i, int X, int y)
{
    const TwTree *t = &TW_T[side][i]; int dx = X - t->c, w = tw_halfw(side, i, y, dx > 0), v, u, vi, ui, vf, uf, a, b;
    if (w < 1) w = 1;
    v = clampi((y - t->tip) * 7 * 256 / (t->bot - t->tip), 0, 7 * 256 - 1);
    u = clampi((w - dx) * 4 * 128 / w, 0, 4 * 256 - 1);  /* (0 the outer flank, 4 the inner one) */
    vi = v >> 8; vf = v & 255; ui = u >> 8; uf = u & 255;
    a = TW_LIGHT[vi][ui] * (256 - uf) + TW_LIGHT[vi][ui + 1] * uf;
    b = TW_LIGHT[vi + 1][ui] * (256 - uf) + TW_LIGHT[vi + 1][ui + 1] * uf;
    return (a * (256 - vf) + b * vf) >> 16;
}
/* the target of one side: label and light per texel (X 0..tw_tw-1) */
static u8 *tw_tl; static short *tw_tg; static int tw_tw;
#define TW_HZ 644                                        /* the horizon where no crown is: the grey band's lower rows */
static int tw_open(int side, int X) { return tw_owner(side, X, 634) < 0; }
static void tw_targets(int side)
{
    for (int y = 0; y < TW_H; y++)
        for (int X = 0; X < tw_tw; X++) {
            int o = y < TW_G0 ? tw_owner(side, X, y) : -1, l;
            l = y >= (y >= TW_G0 && y < TW_HZ && tw_open(side, X) ? TW_HZ : TW_G0) ? TL_GROUND : o >= 0 ? TL_FOL : y < tw_outline(X) ? TL_SKY : TL_CLOUD;
            tw_tl[(size_t)y * tw_tw + X] = (u8)l;
            tw_tg[(size_t)y * tw_tw + X] = (short)(o >= 0 ? tw_light(side, o, X, y) : -1);
        }
}
static int tw_target(int side, int X, int y) { (void)side; return X < 0 || X >= tw_tw ? TL_CLOUD : tw_tl[(size_t)y * tw_tw + X]; }
static int tw_tlight(int X, int y) { return X < 0 || X >= tw_tw ? -1 : tw_tg[(size_t)y * tw_tw + X]; }

/* --- the canvas of one side: tw_cv (X = 1..tw_cw outwards) */
static Rgb *tw_cv; static u8 *tw_have; static int tw_cw;
static int tw_X(int side, int wx) { return side ? wx - (PW - 1) : -wx; }
static int tw_get(int side, int wx, int y, Rgb *c)
{
    int X;
    if (y < 0 || y >= TW_H) return 0;
    if (wx >= 0 && wx < PW) { *c = tw_rgb[y][wx]; return 1; }
    X = tw_X(side, wx);
    if (X < 1 || X > tw_cw || !tw_have[(size_t)y * tw_cw + X - 1]) return 0;
    *c = tw_cv[(size_t)y * tw_cw + X - 1]; return 1;
}
/* the sky over the clouds: per row, a texel of the 12 columns at the edge picked by a position hash, among their sky
   texels within one step of the row's middle tone (the haze a cloud's rim leaves in the sky came out as specks); a
   row the clouds fill at the edge takes the nearest row of open sky */
static void tw_sky(int side, int X, int y, Rgb *o)
{
    for (int dy = 0; dy < TW_H; dy++)
        for (int sg = -1; sg <= 1; sg += 2) {
            int yy = y + dy * sg, n = 0, x0 = side ? PW - 12 : 0, k, l[12], med, lo = 255, hi = 0;
            if (yy < 0 || yy >= TW_H) continue;
            for (int x = x0; x < x0 + 12; x++) if (tw_lab[yy][x] == TL_SKY) { l[n++] = lum(tw_rgb[yy][x]); }
            if (n < 11) continue;
            for (int i = 0; i < n; i++) { if (l[i] < lo) lo = l[i]; if (l[i] > hi) hi = l[i]; }
            med = lo;                                    /* (the tone at least half the texels reach) */
            for (int t = lo; t <= hi; t++) { int c = 0; for (int i = 0; i < n; i++) c += l[i] <= t; if (c * 2 >= n) { med = t; break; } }
            n = 0;
            for (int x = x0; x < x0 + 12; x++) { int d = lum(tw_rgb[yy][x]) - med; if (tw_lab[yy][x] == TL_SKY && d <= 10 && d >= -10) n++; }
            k = (int)(hash3(side * 4099 + X, y, 3307) % (u32)n);
            for (int x = x0; x < x0 + 12; x++) {
                int d = lum(tw_rgb[yy][x]) - med;
                if (tw_lab[yy][x] == TL_SKY && d <= 10 && d >= -10 && k-- == 0) { *o = tw_rgb[yy][x]; return; }
            }
        }
    o->r = 128; o->g = 152; o->b = 224;
}
#define TW_LW 200000                                      /* the cost of a texel of another label */
#define TW_GW 6                                           /* the weight of the crowns' light (squared luminance) */
typedef struct { short y0, y1, BW, BH, OV, OT, dy, near; } TwBand;
static int (*tw_quilt__em)[VB_MAXW];
static int (*tw_quilt__ev)[VB_MAXW];
static int (*tw_quilt__cand);
static int (*tw_quilt__errs);
static u8 (*tw_quilt__tl)[VB_MAXW];
static u8 (*tw_quilt__hv)[VB_MAXW];
static Rgb (*tw_quilt__ctx)[VB_MAXW];
static short (*tw_quilt__tg)[VB_MAXW];
static void tw_quilt(int side, const TwBand *b, int seed)
{
    int W = b->BW + b->OV, nbk = 0, used[4] = { -1, -1, -1, -1 };
    static int path[VB_MAXH], tcut[VB_MAXW]; int (*const em)[VB_MAXW] = tw_quilt__em, (*const ev)[VB_MAXW] = tw_quilt__ev;
    int (*const cand) = tw_quilt__cand, (*const errs) = tw_quilt__errs;
    u8 (*const tl)[VB_MAXW] = tw_quilt__tl, (*const hv)[VB_MAXW] = tw_quilt__hv; Rgb (*const ctx)[VB_MAXW] = tw_quilt__ctx; short (*const tg)[VB_MAXW] = tw_quilt__tg;
    for (int X0 = 1; X0 <= tw_cw; X0 += b->BW)
        for (int yb = b->y0; yb < b->y1; yb += b->BH) {
            int H = b->BH + b->OT, ytop = yb - b->OT, bh = yb + b->BH > b->y1 ? b->y1 - yb : b->BH;
            int wx0 = side ? PW - 1 + X0 - b->OV : -(X0 + b->BW - 1), mn = 1 << 30, nc = 0, ncand = 0, pick = -1, k, need = 0;
            for (int y = 0; y < H; y++)
                for (int j = 0; j < W; j++) {
                    int X = tw_X(side, wx0 + j), yy = ytop + y;
                    tl[y][j] = (u8)(X >= 1 && yy >= 0 && yy < TW_H ? tw_target(side, X, yy) : 0);
                    tg[y][j] = (short)(tl[y][j] == TL_FOL ? tw_tlight(X, yy) : -1);
                    if (y >= b->OT && X >= 1 && X <= tw_cw && yy < TW_H && !tw_have[(size_t)yy * tw_cw + X - 1]) need = 1;
                }
            if (!need) { nbk++; continue; }
            for (int sy = ytop - b->dy; sy <= ytop + b->dy; sy += 2)
                for (int sx = 0; sx + W <= PW; sx += 2)
                    if (ncand < PW * 64 && tw_okwin(sx, sy, W, H)) cand[ncand++] = sy * PW + sx;
            if (!ncand) { nbk++; continue; }
            {   int n = 0, nl = 0;                       /* the context (texels already there) of the overlap */
                for (int y = 0; y < H; y++)
                    for (int j = 0; j < W; j++) {
                        int ov = side ? j < b->OV : j >= b->BW;
                        hv[y][j] = (u8)((ov || y < b->OT) && tw_get(side, wx0 + j, ytop + y, &ctx[y][j]));
                        if ((y + j) & 1) hv[y][j] = 0;   /* (a checkerboard of it) */
                        n += hv[y][j]; nl += tl[y][j] && !((y | j) & 1);
                    }
                for (int c = 0; c < ncand; c++) {
                    int sx = cand[c] % PW, sy = cand[c] / PW, ml = 0, ng = 0, lim = mn < (1 << 28) ? mn + mn / 8 : 1 << 29;
                    long long e = 0, el, eg = 0;
                    for (int y = 0; y < H; y += 2)
                        for (int j = 0; j < W; j += 2) {
                            if (tl[y][j]) ml += tl[y][j] != tw_lab[sy + y][sx + j];
                            if (tg[y][j] >= 0 && tw_gs[sy + y][sx + j] >= 0) { int d = tw_gs[sy + y][sx + j] - tg[y][j]; eg += d * d; ng++; }
                        }
                    el = (nl ? (long long)ml * TW_LW * 64 / nl : 0) + (ng ? eg * 64 * TW_GW / ng : 0);
                    for (int y = 0; y < H && el + (n ? e * 64 / n : 0) <= lim; y++)
                        for (int j = 0; j < W; j++) if (hv[y][j]) e += vb_d2(ctx[y][j], tw_rgb[sy + y][sx + j]);
                    e = (n ? e * 64 / n : 0) + el;
                    if (e > lim) { errs[c] = 1 << 29; continue; }
                    {   int d = sx - wx0, dv = sy - ytop; if (d < 0) d = -d; if (dv < 0) dv = -dv;
                        if (d < b->near && dv < 48) e += e / 2 + 4000;        /* (no copy next to its source) */
                        for (int u = 0; u < 4; u++) {
                            int dx = sx - used[u] % PW, dz = sy - used[u] / PW; if (dx < 0) dx = -dx; if (dz < 0) dz = -dz;
                            if (used[u] >= 0 && dx < W && dz < H) e += e / 2 + 4000;
                        }
                    }
                    errs[c] = (int)(e > (1 << 29) ? (1 << 29) : e);
                    if (errs[c] < mn) mn = errs[c];
                }
            }
            for (int c = 0; c < ncand; c++) if (errs[c] <= mn + mn / 8) nc++;
            k = (int)(hash3(side * 977 + nbk, X0, seed) % (u32)nc);
            for (int c = 0; c < ncand && pick < 0; c++) if (errs[c] <= mn + mn / 8 && k-- == 0) pick = c;
            {   int sx = cand[pick] % PW, sy = cand[pick] / PW, j0 = side ? 0 : b->BW, vc[VB_MAXH];
                used[nbk % 4] = cand[pick];
                for (int y = 0; y < H; y++) for (int j = 0; j < W; j++) { Rgb o; em[y][j] = tw_get(side, wx0 + j, ytop + y, &o) ? vb_d2(o, tw_rgb[sy + y][sx + j]) : 0; }
                for (int y = 0; y < H; y++) for (int j = 0; j < b->OV; j++) ev[y][j] = em[y][j0 + j];
                vb_cutpath(ev, H, b->OV, 1, path);
                for (int y = 0; y < H; y++) vc[y] = j0 + path[y];
                for (int j = 0; j < W; j++) tcut[j] = 0;
                if (b->OT > 0) vb_cutpath(em, b->OT, W, 0, tcut);
                for (int y = 0; y < b->OT + bh; y++)
                    for (int j = 0; j < W; j++) {
                        int tx = wx0 + j, yy = ytop + y, X = tw_X(side, tx); Rgb o;
                        if ((tx >= 0 && tx < PW) || yy < 0 || yy >= TW_H || X < 1 || X > tw_cw) continue;
                        if ((side ? j < vc[y] : j > vc[y]) && tw_get(side, tx, yy, &o)) continue;
                        if (y < tcut[j] && tw_get(side, tx, yy, &o)) continue;
                        tw_cv[(size_t)yy * tw_cw + X - 1] = tw_rgb[sy + y][sx + j]; tw_have[(size_t)yy * tw_cw + X - 1] = 1;
                    }
            }
            nbk++;
        }
}
/* the trees' foot, the grey band and the grass under them (rows y0..y1): slices 2-5 columns wide of the clean columns
   at both edges (pool: no trunk, bush or path there), the same rows, each in its own left-to-right order; the next
   slice the one whose column before it matches the strip's last column best (among those within 1/4 of the best, not
   the last 3), so the foliage tips, the band's shading and the trees' shadow on the grass stay whole and nothing
   repeats at a period; cross-faded over 4 rows into what is above.  Two passes: the foot (TW_S0..TW_S1, the columns
   beside the trunks) and the grass (from TW_S1 - 4, wider pools) */
#define TW_S0 626
#define TW_S1 650
static const short TW_SP[2][2][2] = { { { 1, 12 }, { 302, 319 } }, { { 1, 47 }, { 273, 319 } } };   /* source columns
                                                            (a slice's column before it too) */
static int (*tw_slices__cs);
static int (*tw_slices__ce);
static void tw_slices(int side, int y0, int y1, const short pool[2][2], int gr, int seed)
{
    int X = 1, last[3] = { -1, -1, -1 }, nsl = 0;
    int (*const cs) = tw_slices__cs, (*const ce) = tw_slices__ce;
    while (X <= tw_cw) {
        int w = (gr == 1 && side ? 4 : 2) + (int)(hash3(side, X, seed) % (gr == 1 && side ? 5u : 4u)), best = 1 << 30, n = 0, k, pick = -1;
        for (int r = 0; r < 2; r++)
            for (int s0 = pool[r][0]; s0 <= pool[r][1]; s0++) {
                int sb = side ? s0 - 1 : s0 + 1, e = 0;    /* the column before the slice's first one */
                int se = side ? s0 + w - 1 : s0 - w + 1;
                if (se < pool[r][0] || se > pool[r][1] || sb < 0 || sb >= PW) continue;
                for (int ya = y0; ya < y1; ya += gr) {      /* (gr rows averaged: the grass's specks no guide) */
                    Rgb a = { 0, 0, 0 }, c = { 0, 0, 0 }; int m = 0;
                    for (int y = ya; y < ya + gr && y < y1; y++, m++) {
                        Rgb o = { 0, 0, 0 }; tw_get(side, side ? PW - 1 + X - 1 : -(X - 1), y, &o);
                        a.r += o.r; a.g += o.g; a.b += o.b; c.r += tw_rgb[y][sb].r; c.g += tw_rgb[y][sb].g; c.b += tw_rgb[y][sb].b;
                    }
                    a.r /= m; a.g /= m; a.b /= m; c.r /= m; c.g /= m; c.b /= m;
                    e += vb_d2(a, c) * m;
                }
                if (s0 == last[0] || s0 == last[1] || s0 == last[2]) e += e / 2 + 20000;
                cs[n] = s0; ce[n] = e; n++;
                if (e < best) best = e;
            }
        if (!n) break;
        { int m = 0, tol = 4; for (int i = 0; i < n; i++) m += ce[i] <= best + best / tol;
          k = (int)(hash3(side, X, seed + 66) % (u32)m);
          for (int i = 0; i < n && pick < 0; i++) if (ce[i] <= best + best / tol && k-- == 0) pick = cs[i]; }
        last[nsl % 3] = pick; nsl++;
        for (int i = 0; i < w && X <= tw_cw; i++, X++) {
            int ya = y0 > TW_HZ || !tw_open(side, X) ? y0 : TW_HZ;   /* (open sky: the clouds down to the band) */
            for (int y = ya; y < y1; y++) {
                size_t q = (size_t)y * tw_cw + X - 1; Rgb c = tw_rgb[y][side ? pick + i : pick - i];
                if (y < ya + 4 && tw_have[q]) c = mix(tw_cv[q], c, (y - ya + 1) * 256 / 5);
                tw_cv[q] = c; tw_have[q] = 1;
            }
        }
    }
}
/* the foliage's light per row (rows y +- 16): the luminance mean and spread of the strip's foliage texels (its first
   40 columns) onto the picture's (its 32 columns next to the edge), the spread at most x 0.8 .. x 1.5, fading out
   over the first 32 columns (further out the crowns keep their own light) */
static int tw_leafy(Rgb k, int y) { int c = tw_cls(k, y); return y < TW_G0 && c != TL_SKY && c != TL_CLOUD; }   /* (the lit leaves too) */
static int (*tw_contrast__mp);
static int (*tw_contrast__ms);
static int (*tw_contrast__k256);
static void tw_contrast(int side)
{
    int (*const mp) = tw_contrast__mp, (*const ms) = tw_contrast__ms, (*const k256) = tw_contrast__k256;
    for (int y = 0; y < TW_H; y++) {
        long long s[2] = { 0, 0 }, q[2] = { 0, 0 }; int n[2] = { 0, 0 };
        mp[y] = -1;
        for (int yy = y - 16; yy <= y + 16; yy++) {
            if (yy < 0 || yy >= TW_H) continue;
            for (int i = 0; i < 40; i++) {
                Rgb a = tw_rgb[yy][side ? PW - 1 - i : i], b = tw_cv[(size_t)yy * tw_cw + i]; int l;
                if (i < 32 && tw_leafy(a, yy)) { l = lum(a); s[0] += l; q[0] += l * l; n[0]++; }
                if (tw_leafy(b, yy)) { l = lum(b); s[1] += l; q[1] += l * l; n[1]++; }
            }
        }
        if (n[0] < 64 || n[1] < 64) continue;
        mp[y] = (int)(s[0] / n[0]); ms[y] = (int)(s[1] / n[1]); k256[y] = 256;
        {   long long v0 = q[0] / n[0] - (long long)mp[y] * mp[y], v1 = q[1] / n[1] - (long long)ms[y] * ms[y];
            if (v0 > 0 && v1 > 0) k256[y] = clampi(isqrt_i((int)(v0 * 65536 / v1)), 205, 384); }
    }
    for (int y = 0; y < TW_H; y++) {
        if (mp[y] < 0) continue;
        for (int X = 1; X <= tw_cw; X++) {
            Rgb *p = &tw_cv[(size_t)y * tw_cw + X - 1]; int l = lum(*p), t;
            if (X > 32) break;
            if (l <= 0 || !tw_leafy(*p, y)) continue;
            t = clampi(mp[y] + (l - ms[y]) * k256[y] / 256, 1, 255);
            t = l + (t - l) * (33 - X) / 32;              /* (fading out: the crowns' own light outwards) */
            p->r = clampi(p->r * t / l, 0, 255); p->g = clampi(p->g * t / l, 0, 255); p->b = clampi(p->b * t / l, 0, 255);
        }
    }
}
/* the join: per row and label (the texel's own class), the mean colour of the strip's first 6 columns onto the
   picture's last 6 (rows y +- 3), fading out over 24 columns */
static int (*tw_join__d)[5][3];
static void tw_join(int side)
{
    int (*const d)[5][3] = tw_join__d;
    for (int y = 0; y < TW_H; y++) {
        int sp[5][3], ss[5][3], np[5], ns[5];
        memset(sp, 0, sizeof sp); memset(ss, 0, sizeof ss); memset(np, 0, sizeof np); memset(ns, 0, sizeof ns);
        for (int yy = y - 3; yy <= y + 3; yy++) {
            if (yy < 0 || yy >= TW_H) continue;
            for (int i = 0; i < 6; i++) {
                Rgb a = tw_rgb[yy][side ? PW - 1 - i : i], b = tw_cv[(size_t)yy * tw_cw + i];
                int ka = tw_cls(a, yy), kb = tw_cls(b, yy);
                sp[ka][0] += a.r; sp[ka][1] += a.g; sp[ka][2] += a.b; np[ka]++;
                ss[kb][0] += b.r; ss[kb][1] += b.g; ss[kb][2] += b.b; ns[kb]++;
            }
        }
        for (int k = 0; k < 5; k++)
            for (int c = 0; c < 3; c++) d[y][k][c] = np[k] >= 4 && ns[k] >= 4 ? sp[k][c] / np[k] - ss[k][c] / ns[k] : 0;
    }
    for (int y = 0; y < TW_H; y++)
        for (int X = 1; X <= 24 && X <= tw_cw; X++) {
            Rgb *p = &tw_cv[(size_t)y * tw_cw + X - 1]; int k = tw_cls(*p, y), w = X <= 4 ? 256 : (24 - X) * 256 / 20;
            p->r = clampi(p->r + d[y][k][0] * w / 256, 0, 255); p->g = clampi(p->g + d[y][k][1] * w / 256, 0, 255);
            p->b = clampi(p->b + d[y][k][2] * w / 256, 0, 255);
        }
}
/* the soft parts at the join: the sky's texels the quilt brought from other rows are the sky of their own row (its
   gradient: a window from 40 rows off was a shade off, a visible box), and on sky and cloud texels the first column's
   difference to the picture's edge column (per row, rows y +- 2) fades out over 8 columns, so the clouds' soft shading
   goes on without a line */
#define TW_RW(X) tw_cv[(size_t)y * tw_cw + (X) - 1]
static int (*tw_soft__d)[3];
static void tw_soft(int side)
{
    int (*const d)[3] = tw_soft__d;
    for (int y = 0; y < TW_G0; y++) {                    /* the clouds up to the leaves (the quilt's windows brought
                                                            the sky round the picture's crowns: a blue rim): a run of
                                                            sky texels next to leaves where the target is cloud takes
                                                            the cloud texels beyond it, shifted by its length */
        for (int X = 1; X <= tw_cw; X++) {
            int f = 0, e = X, n;
            if (tw_target(side, X, y) != TL_CLOUD || tw_cls(TW_RW(X), y) != TL_SKY) continue;
            while (e < tw_cw && tw_target(side, e + 1, y) == TL_CLOUD && tw_cls(TW_RW(e + 1), y) == TL_SKY) e++;
            for (int xx = X - 4; xx <= e + 4 && !f; xx++) f = xx >= 1 && xx <= tw_cw && tw_leafy(TW_RW(xx), y);
            n = e - X + 1;
            if (f && n <= 10) {
                int up = e + n <= tw_cw && tw_cls(TW_RW(e + 1 <= tw_cw ? e + 1 : tw_cw), y) == TL_CLOUD, dn = X - n >= 1 && tw_cls(TW_RW(X - 1 >= 1 ? X - 1 : 1), y) == TL_CLOUD;
                for (int xx = X; xx <= e && (up || dn); xx++) {
                    Rgb c = up ? TW_RW(xx + n) : TW_RW(xx - n);
                    if (tw_cls(c, y) == TL_CLOUD) TW_RW(xx) = c;
                }
            }
            X = e;
        }
    }
    for (int y = 0; y < TW_H; y++)
        for (int X = 1; X <= tw_cw; X++) {
            Rgb *p = &tw_cv[(size_t)y * tw_cw + X - 1];
            int k = tw_cls(*p, y), ns = 0;                /* (sky all round: not a cloud's rim; or a lone
                                                            light texel of a cloud's rim in the open sky) */
            if (k != TL_SKY && k != TL_CLOUD) continue;
            for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++) {
                    int yy = y + dy, xx = X + dx;
                    if ((dx || dy) && (yy < 0 || yy >= TW_H || xx < 1 || xx > tw_cw || tw_cls(tw_cv[(size_t)yy * tw_cw + xx - 1], yy) == TL_SKY)) ns++;
                }
            if (ns == 8 || (k == TL_CLOUD && ns >= 7)) tw_sky(side, X, y, p);
        }
    for (int y = 0; y < TW_H; y++) {
        int s[3] = { 0, 0, 0 }, n = 0;
        for (int yy = y - 2; yy <= y + 2; yy++) {
            Rgb a, b; int ka, kb;
            if (yy < 0 || yy >= TW_H) continue;
            a = tw_rgb[yy][side ? PW - 1 : 0]; b = tw_cv[(size_t)yy * tw_cw]; ka = tw_cls(a, yy); kb = tw_cls(b, yy);
            if ((ka != TL_SKY && ka != TL_CLOUD) || (kb != TL_SKY && kb != TL_CLOUD)) continue;
            s[0] += a.r - b.r; s[1] += a.g - b.g; s[2] += a.b - b.b; n++;
        }
        for (int c = 0; c < 3; c++) d[y][c] = n >= 3 ? s[c] / n : 0;
    }
    for (int y = 0; y < TW_H; y++)
        for (int X = 1; X <= 8 && X <= tw_cw; X++) {
            Rgb *p = &tw_cv[(size_t)y * tw_cw + X - 1]; int k = tw_cls(*p, y), w = (9 - X) * 256 / 8;
            if (k != TL_SKY && k != TL_CLOUD) continue;
            p->r = clampi(p->r + d[y][0] * w / 256, 0, 255); p->g = clampi(p->g + d[y][1] * w / 256, 0, 255);
            p->b = clampi(p->b + d[y][2] * w / 256, 0, 255);
        }
}
#undef TW_RW
static u8 (*tw_pal__seen);
static void tw_pal(void)
{
    u8 (*const seen) = tw_pal__seen;
    memset(seen, 0, (sizeof *seen * (32768))); memset(qhave, 0, (sizeof *qhave * (32768))); npal = 0;
    for (int y = 0; y < TW_H; y++) for (int x = 0; x < PW; x++) {
        u16 c = tw_pic[y][x] & 0x7FFF;
        if (!seen[c] && npal < 256 && c) { seen[c] = 1; pal[npal++] = tw_pic[y][x]; }
    }
}
/* the trunks: the picture's trunk texels under the crown, at the same rows (the same perspective and scale), not
   mirrored, only where no crown in front of the tree's own is: the dark core between the leaves (rows 580-634: the
   source's dark or brown texels within 2-5 columns of its axis, the first 10 rows thinning in) and the foot on the grey
   band (rows 641-648: the source row's dark run through the axis, gaps of up to 2 grass texels bridged, at most hf
   columns either side) */
static int tw_bark(Rgb k) { return !(k.g > k.r); }
static void tw_trunk(int side, int i)
{
    const TwTree *t = &TW_T[side][i]; int wc = side ? PW - 1 + t->c : -t->c;
    if (t->src < 0) return;
    for (int y = 580; y < 649; y++) {
        int foot = y >= 641, a = 0, b = 0;
        if (y >= 635 && !foot) continue;
        if (foot) {                                      /* the run through the axis */
            int g = 0;
            for (a = 0; a < t->hf && t->src - a - 1 >= 0; a++) { if (tw_bark(tw_rgb[y][t->src - a - 1])) g = 0; else if (++g > 2) break; }
            a -= g; if (a < 0) a = 0; g = 0;
            for (b = 0; b < t->hf && t->src + b + 1 < PW; b++) { if (tw_bark(tw_rgb[y][t->src + b + 1])) g = 0; else if (++g > 2) break; }
            b -= g; if (b < 0) b = 0;
        } else a = b = 2 + (y - 580) / 18;
        for (int dx = -a; dx <= b; dx++) {
            int X = tw_X(side, wc + dx), sx = t->src + dx; Rgb k; size_t q;
            if (X < 3 || X > tw_cw || sx < 0 || sx >= PW) continue;
            k = tw_rgb[y][sx];
            if (foot ? !tw_bark(k) : !((tw_bark(k) && lum(k) < 110) || lum(k) < 40)) continue;
            if (!foot && y < 590 && (int)(hash3(side * 8 + i, dx, y) % 10u) >= y - 580) continue;
            if (!foot && tw_owner(side, X, y) > i) continue;
            q = (size_t)y * tw_cw + X - 1; tw_cv[q] = k; tw_have[q] = 1;
        }
    }
}
/* the flank of a tree whose tip is in the picture, in its first 48 rows (narrower than the quilt's windows): the
   picture's own leaves of the same row, 7-9 columns towards the tree's axis (moved, not mirrored), inside the
   crown's outline */
static void tw_flank(int side, int i)
{
    const TwTree *t = &TW_T[side][i];
    if (t->c > 0) return;
    for (int y = t->tip; y < t->tip + 48; y++)
        for (int X = 1; X <= 12 && X <= tw_cw; X++) {
            int sh = 7 + (int)(hash3(side, i, y) % 3u), sx = side ? PW - 1 + X - sh : -X + sh; Rgb k;
            if (tw_owner(side, X, y) != i || sx < 0 || sx >= PW) continue;
            k = tw_rgb[y][sx];
            if (tw_leafy(k, y)) { tw_cv[(size_t)y * tw_cw + X - 1] = k; tw_have[(size_t)y * tw_cw + X - 1] = 1; }
        }
}
static void tw_build(int pad)
{
    if (!ws_need_tw_build()) return;
    free(tw_strip); tw_strip = calloc((size_t)2 * pad * TW_H, 2); tw_pad = pad;
    if (!tw_strip) return;
    tw_cw = pad + 32; tw_tw = tw_cw + 64;
    tw_cv = (Rgb *)malloc(sizeof(Rgb) * (size_t)tw_cw * TW_H); tw_have = (u8 *)malloc((size_t)tw_cw * TW_H);
    tw_tl = (u8 *)malloc((size_t)tw_tw * TW_H); tw_tg = (short *)malloc(sizeof(short) * (size_t)tw_tw * TW_H);
    if (!tw_cv || !tw_have || !tw_tl || !tw_tg) { free(tw_cv); free(tw_have); free(tw_tl); free(tw_tg); tw_cv = 0; tw_have = 0; tw_tl = 0; tw_tg = 0; return; }
    tw_pal(); tw_analyse();
    for (int side = 0; side < 2; side++) {
        int ytop = TW_H;
        memset(tw_have, 0, (size_t)tw_cw * TW_H);
        tw_outline_make(side); tw_outline_cols(side); tw_targets(side);
        for (int X = 1; X < TW_OLW; X++) { int c = tw_outline(X); if (c < ytop) ytop = c; }   /* (not the strip's width: a wider one starts the same) */
        ytop -= 24;
        for (int y = 0; y < ytop + 8; y++)                   /* the sky (the quilt's first rows overlap it) */
            for (int X = 1; X <= tw_cw; X++) {
                tw_sky(side, X, y, &tw_cv[(size_t)y * tw_cw + X - 1]);
                if (y < ytop) tw_have[(size_t)y * tw_cw + X - 1] = 1;
            }
        {   TwBand a = { (short)ytop, 532, 14, 14, 8, 6, 48, 48 }, c = { 532, TW_HZ, 14, 14, 8, 6, 160, 48 };
            tw_quilt(side, &a, 8101 + side);
            tw_quilt(side, &c, 8201 + side);
            tw_slices(side, TW_S0, TW_S1, TW_SP[0], 1, 9311);
            tw_slices(side, TW_S1 - 4, TW_H, TW_SP[1], 6, 9411);
        }
        for (int i = 0; i < TW_NT; i++) { tw_flank(side, i); tw_trunk(side, i); }
        tw_contrast(side);
        tw_join(side);
        tw_soft(side);
#ifdef WS_BACKDROP_MAIN
        if (getenv("WSB_TL"))                            /* research: the target labels, blended */
            for (int y = 0; y < TW_H; y++)
                for (int X = 1; X <= tw_cw; X++) {
                    static const Rgb LC[5] = { { 0, 0, 0 }, { 0, 0, 255 }, { 255, 255, 255 }, { 0, 255, 0 }, { 255, 255, 0 } };
                    Rgb *p = &tw_cv[(size_t)y * tw_cw + X - 1]; *p = mix(*p, LC[tw_target(side, X, y)], 96);
                }
#endif
        for (int y = 0; y < TW_H; y++)
            for (int X = 1; X <= pad; X++)
                tw_strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(tw_cv[(size_t)y * tw_cw + X - 1]);
    }
    free(tw_cv); free(tw_have); free(tw_tl); free(tw_tg); tw_cv = 0; tw_have = 0; tw_tl = 0; tw_tg = 0;
}
#ifdef WS_BACKDROP_MAIN
/* standalone: wsb 16:4 PIC.raw PAD OUT.ppm (PIC.raw: 320 x 672 15-bit LE, PORT_WS_BACKDROP_DUMP) */
static int tw_main(int argc, char **argv)
{
    if (!ws_need_tw_main()) return 0;
    FILE *f; int pad;
    if (argc < 4 || !(f = fopen(argv[1], "rb"))) { fprintf(stderr, "usage: wsb 16:4 PIC.raw PAD OUT.ppm\n"); return 1; }
    for (int y = 0; y < TW_H; y++) for (int x = 0; x < PW; x++) { u8 b[2]; if (fread(b, 1, 2, f) != 2) return 1; tw_pic[y][x] = (u16)(b[0] | b[1] << 8); }
    fclose(f);
    pad = atoi(argv[2]);
    tw_build(pad);
    if (!tw_strip || !(f = fopen(argv[3], "wb"))) return 1;
    fprintf(f, "P6 %d %d 255\n", PW + 2 * pad, TW_H);
    for (int y = 0; y < TW_H; y++)
        for (int x = 0; x < PW + 2 * pad; x++) {
            u16 c = x < pad ? tw_strip[(size_t)y * 2 * pad + x] : x >= pad + PW ? tw_strip[(size_t)y * 2 * pad + x - PW] : tw_pic[y][x - pad];
            u8 rgb[3] = { (u8)(r5(c) << 3), (u8)(g5(c) << 3), (u8)(b5(c) << 3) }; fwrite(rgb, 1, 3, f);
        }
    fclose(f);
    { u32 h = 2166136261u; for (size_t i = 0; i < (size_t)2 * pad * TW_H; i++) h = (h ^ tw_strip[i]) * 16777619u; printf("strip hash %08x\n", h); }
    return 0;
}
#endif
