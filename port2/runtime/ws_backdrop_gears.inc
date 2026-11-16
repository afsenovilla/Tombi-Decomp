/* runtime/ws_backdrop.c: AREA16 s3 / s6, the gears room (gen_gr; included after ws_backdrop_rooms2.inc; docs/PORT.md
 * "More rooms with picture strips").  The same rules as the other generators: every texel from the user's picture at
 * run time, integers only, position hashes, a structure depends on where it is and not on the strip width; the
 * tables hold coordinates only.
 *
 * Both sections draw the same picture (pages (128, 0) / (256, 0), 256 rows drawn, 224 seen): a brick room with grey
 * stone gears on the back wall, a sandstone corbel sticking out of each side wall, and two big blue cog wheels in
 * the foreground whose hubs are under the frame (the "blue beams" of the first note are their rims and spokes).  The
 * wheels are in the picture and are drawn again over the player as quads of page (256, 0) (u 64 v 0 144 x 144 the
 * left one, u 64 v 144 72 x 96 the right one, s6 also u 184 v 216 64 x 40): at their place in the picture in s6, 24
 * rows higher in s3 (the wheels have turned: s3 shows the quads' wheels over the picture's, cut at the quads' last
 * row, 200).  The right quad lies 3 columns left of the picture's wheel and ends 4 columns before the edge.
 *
 * Measured (wheel coordinates = the picture's, s6):
 *   - left wheel: its rim's outer edge is a circle, centre (-34, 291), radius 182 (the edge's 21 columns x 0-84 within
 *     1 row); unwrapped about that centre the rim is a straight band (inner edge 147.5 at 22-36 degrees from the
 *     vertical, 144 at 40, 142 from 44; outer 182 to 42 degrees, sagging to 174.5 at 52: the wheel is low-poly and
 *     seen in perspective), with a tooth every 44 degrees (lit face 12-19 and 56-63 degrees, to radius 212, its side
 *     face towards the picture's centre) and a spoke under each tooth (a bar: the lit face's right edge is
 *     the line x = 11.5 - 0.458 (y - 161), its side face 7 wide right of it; its left edge is off the picture);
 *   - right wheel: centre (389, 337), radius 182 (the picture's edge x 277-310), inner edge 142.5; the tooth cut by
 *     the edge has its side face at -24 .. -22 degrees and its lit face from -22 on (the left wheel's tooth turned
 *     the other way round: axis -18.5 degrees);
 *   - the corbels: left, top edge 47.5 + 0.09 x, lower edge 84.3 - 0.26 x (straight), 3 rows of underside; right, top
 *     edge row 55.5, lower edge 95 at the edge falling 1 row a column over its last 12 columns (a 45-degree brace),
 *     15 rows of dark underside; brick shows above and below both at the edges, so they stick out of the side walls
 *     and end on them;
 *   - the walls' foot: row 186 at x 8 rising to 178 at x 48 on the left (the side wall's foot, 0.2 rows a column).
 *     No brick period could be measured (the autocorrelation of the wall has no peak over 0.15 outside one row), so
 *     the wall is quilted on overlap, not laid by a lattice.
 *
 * The strips, back to front:
 *   - wall and floor: quilted (rq_quilt): the wall on its texels over their light (17 x 17 mean luminance of the wall
 *     texels, sources with an even light only) and lit as each row's first wall texels from the edge; the floor from
 *     floor windows at most 10 rows off (its texture
 *     grows towards the viewer); the foot lines go on with their slope;
 *   - the corbels: their faces go on between their measured edges, quilted from both corbels' lit texels: the left one
 *     to a root on the side wall 75 columns out (past 16:9), the right one's brace runs on at its 45 degrees behind the
 *     wheel (no end in the strip); the left underside is the picture's own, slid along the lower edge, the right one
 *     the picture's tone by depth under the edge;
 *   - the wheels, round: the left rim is the picture's rim next to the edge turned about the centre by the angle at
 *     which it matches itself best (gr_join, about 30 degrees: a facet period), its radii scaled by the band's edges
 *     there and here, its facet's tone at the join the picture's there, eased back over 12 degrees of arc; the teeth
 *     are the picture's a tooth period (44 degrees) on (the next one lands at -28 degrees, 95 columns out); the spokes
 *     are bars along the measured edge, textured with rim texels (the rim band laid straight) at the visible spoke's
 *     tone and contrast, their side face the picture's slid along the bar; the outlines anti-aliased (gr_cover); the right rim is the left wheel's rim turned onto it (its own clean rim is 12
 *     degrees long), at the right rim's tone; the cut tooth's lit face is the left tooth's turned onto it.
 *     s3: the wheel layer is drawn twice, at the picture's place and 24 rows higher down to row 200, as the game does.
 * No texel is mirrored: every map is a rotation, a slide or a radial scale. */

static const short GR_SIN[91] = {
    0, 286, 572, 857, 1143, 1428, 1713, 1997, 2280, 2563, 2845, 3126, 3406, 3686, 3964, 4240, 4516, 4790, 5063, 5334,
    5604, 5872, 6138, 6402, 6664, 6924, 7182, 7438, 7692, 7943, 8192, 8438, 8682, 8923, 9162, 9397, 9630, 9860, 10087,
    10311, 10531, 10749, 10963, 11174, 11381, 11585, 11786, 11982, 12176, 12365, 12551, 12733, 12911, 13085, 13255,
    13421, 13583, 13741, 13894, 14044, 14189, 14330, 14466, 14598, 14726, 14849, 14968, 15082, 15191, 15296, 15396,
    15491, 15582, 15668, 15749, 15826, 15897, 15964, 16026, 16083, 16135, 16182, 16225, 16262, 16294, 16322, 16344,
    16362, 16374, 16382, 16384 };
/* angles are sixteenths of a degree, clockwise from straight up; sin / cos x 16384 */
static int gr_sin90(int a) { int d = a >> 4, f = a & 15; return d >= 90 ? 16384 : GR_SIN[d] + (GR_SIN[d + 1] - GR_SIN[d]) * f / 16; }
static int gr_sinq(int a)
{
    a = fmodi(a, 360 * 16);
    if (a < 90 * 16) return gr_sin90(a);
    if (a < 180 * 16) return gr_sin90(180 * 16 - a);
    if (a < 270 * 16) return -gr_sin90(a - 180 * 16);
    return -gr_sin90(360 * 16 - a);
}
static int gr_cosq(int a) { return gr_sinq(a + 90 * 16); }
static int gr_ang(int dx, int dy)                        /* the angle of (dx, dy) from straight up (-180 .. 180 degrees) */
{
    int ax = dx < 0 ? -dx : dx, ay = dy < 0 ? -dy : dy, lo = 0, hi = 90, f0, f1, a;
    if (!ax && !ay) return 0;
    while (hi - lo > 1) { int m = (lo + hi) / 2; if (ax * GR_SIN[90 - m] - ay * GR_SIN[m] >= 0) lo = m; else hi = m; }
    f0 = ax * GR_SIN[90 - lo] - ay * GR_SIN[lo]; f1 = ax * GR_SIN[90 - hi] - ay * GR_SIN[hi];
    a = lo * 16 + (f0 > f1 ? clampi(f0 * 16 / (f0 - f1), 0, 16) : 0);
    if (dy > 0) a = 180 * 16 - a;
    return dx < 0 ? -a : a;
}
static int gr_isqrt(int v) { int r = 0, b = 1 << 30; while (b > v) b >>= 2; while (b) { if (v >= r + b) { v -= r + b; r = (r >> 1) + b; } else r >>= 1; b >>= 2; } return r; }
static int gr_rnd(int v, int sh) { return fdiv(v + (1 << (sh - 1)), 1 << sh); }

#define GR_LCX (-34)                                     /* the left wheel's centre and radii (x 16) */
#define GR_LCY 291
#define GR_RCX 389
#define GR_RCY 337
#define GR_ROUT (182 * 16)
#define GR_LIN (147 * 16 + 8)
#define GR_RIN (142 * 16 + 8)
#define GR_TOP (212 * 16)
#define GR_PER (44 * 16)                                 /* a tooth period */
/* the spokes are bars: t along the bar (x 16; 147 at the point given, growing towards the rim), p across it from the
   edge of its lit face that the side face is on (x 16, positive to the right).  Left: that edge is the picture's line
   x = 11.5 - 0.458 (y - 161) (24.6 degrees off the vertical), the lit face 18 wide on its left, the side face 6.8 wide
   on its right; right: the same bar the other way round under the cut tooth */
#define GR_SPK (24 * 16 + 10)
#define GR_SPKR (-(27 * 16 + 10))
#define GR_LEX (-795)                                    /* the bars' origins (x 16): 147 down the edge from (11.5, 161) */
#define GR_LEY 4714
#define GR_REX 6478                                      /* ... from (336.8, 209.6) */
#define GR_REY 5437
#define GR_SPW (18 * 16)
#define GR_SPD 109
#define GR_AXR (-(18 * 16 + 8))                          /* the right tooth's axis */

static int gr_dyo;                                       /* the quads' wheels: rows above the picture's (s3: -24) */
static int gr_blue(Rgb k) { return k.b > k.r + 25 && k.b >= k.g; }
/* a wheel texel of the quads' wheel at wheel coordinates (x, y): sx columns off (the right quad: -3) */
static int gr_wtex(int x, int y, Rgb *o)
{
    y += gr_dyo;
    if (x < 0 || x >= PW || y < 0 || y >= PH) return 0;
    *o = rgb15(pic[y][x]);
    return pic_ov[y][x] || (gr_blue(*o) && lum(*o) < 60);     /* (the dark faces are the picture's only) */
}
static int gr_polar(int cx, int cy, int r16, int a, int sx, Rgb *o)
{
    int x = cx + gr_rnd(r16 * (gr_sinq(a) >> 2), 16), y = cy - gr_rnd(r16 * (gr_cosq(a) >> 2), 16);
    return gr_wtex(x + sx, y, o);
}
static int gr_lin(int a, const short (*t)[2], int n)     /* piecewise linear over degrees */
{
    if (a <= t[0][0] * 16) return t[0][1];
    for (int i = 1; i < n; i++)
        if (a <= t[i][0] * 16) return t[i - 1][1] + (t[i][1] - t[i - 1][1]) * (a - t[i - 1][0] * 16) / ((t[i][0] - t[i - 1][0]) * 16);
    return t[n - 1][1];
}
static const short GR_PIN[][2] = { { 36, 2360 }, { 40, 2304 }, { 44, 2272 } };            /* the left rim's inner edge (x 16) */
static const short GR_POUT[][2] = { { 42, 2912 }, { 44, 2880 }, { 46, 2840 }, { 48, 2824 }, { 50, 2808 }, { 52, 2792 } };
static int gr_pin(int a) { return gr_lin(a, GR_PIN, 3); }
static int gr_pout(int a) { return gr_lin(a, GR_POUT, 6); }
/* the left rim's face is low-poly: its facets differ in tone (mean luminance 69 .. 101 over 3 degrees).  gr_rm: the
   face's mean luminance (x 16) per degree 8 .. 71, over 3 degrees and radii 152-178 (0: off the picture) */
static short gr_rm[64]; static int gr_rmj;               /* gr_rmj: the same of the picture's first 3 degrees at the join */
static int gr_rmean(int a)
{
    int d = clampi(fdiv(a, 16) - 8, 0, 62), f = clampi(a - (d + 8) * 16, 0, 16), u = gr_rm[d], v = gr_rm[d + 1];
    if (!u) u = v ? v : gr_rmj;
    if (!v) v = u;
    return u + (v - u) * f / 16;
}
static void gr_rmake(void)
{
    Rgb k; int sj = 0, nj = 0;
    for (int d = 0; d < 64; d++) {
        int s = 0, n = 0;
        for (int a = (d + 8) * 16 - 24; a <= (d + 8) * 16 + 24; a += 4)
            for (int r = 152 * 16; r <= 178 * 16; r += 24)
                if (gr_polar(GR_LCX, GR_LCY, r, a, 0, &k) && gr_blue(k)) { s += lum(k); n++; }
        gr_rm[d] = (short)(n >= 60 ? s * 16 / n : 0);
    }
    for (int a = 11 * 16 + 8; a <= 14 * 16 + 8; a += 4)
        for (int r = 152 * 16; r <= 178 * 16; r += 24)
            if (gr_polar(GR_LCX, GR_LCY, r, a, 0, &k) && gr_blue(k)) { sj += lum(k); nj++; }
    gr_rmj = nj >= 30 ? sj * 16 / nj : gr_rm[6] ? gr_rm[6] : 80 * 16;
}
static int gr_rta;                                       /* the angle gr_rimtex took its texel at */
/* a texel of the left rim's face laid straight: u columns along (0 .. about 80), v 0 .. 255 across */
static int gr_rimtex(int u, int v, Rgb *o)
{
    int a = 22 * 16 + 8 + fmodi(u, 82) * 89 / 16, in = gr_pin(a) + 40, out = gr_pout(a) - 24;
    gr_rta = a;
    if (gr_polar(GR_LCX, GR_LCY, in + (out - in) * clampi(v, 0, 255) / 255, a, 0, o) && gr_blue(*o)) return 1;
    return gr_polar(GR_LCX, GR_LCY, (in + out) / 2, a, 0, o);
}
static Rgb gr_gain(Rgb c, const int *g) { Rgb o = { clampi(c.r * g[0] / 256, 0, 255), clampi(c.g * g[1] / 256, 0, 255), clampi(c.b * g[2] / 256, 0, 255) }; return o; }
static int gr_gS0[3], gr_gR[3], gr_gT[3], gr_dl;        /* tones over the left rim's (x 256): the left spoke, the right rim;
                                                            the right tooth over the left tooth; gr_dl: gr_join */
static Rgb gr_sm; static int gr_sl, gr_sd, gr_rd;       /* the visible spoke's lit face: mean colour, its luminance and
                                                            mean deviation (x 16); the deviation of the rim texels laid on the bar */
typedef struct { int r, g, b, n; } GrSum;
static void gr_add(GrSum *s, Rgb k) { s->r += k.r; s->g += k.g; s->b += k.b; s->n++; }
static void gr_ratio(const GrSum *a, const GrSum *b, int *g)     /* a over b */
{
    if (a->n < 12 || b->n < 12) { g[0] = g[1] = g[2] = 256; return; }
    g[0] = clampi((a->r * 256 / a->n) / (b->r / b->n + 1), 180, 360);
    g[1] = clampi((a->g * 256 / a->n) / (b->g / b->n + 1), 180, 360);
    g[2] = clampi((a->b * 256 / a->n) / (b->b / b->n + 1), 180, 360);
}
/* a bar at angle a0 from (cx, cy) (x 16): t along it, p across (right positive), x 16, of the point (x, y) (x 16) */
static void gr_bar(int cx, int cy, int a0, int x, int y, int *t, int *p)
{
    int s = gr_sinq(a0), c = gr_cosq(a0), dx = x - cx, dy = y - cy;
    *t = gr_rnd(dx * s - dy * c, 14); *p = gr_rnd(dx * c + dy * s, 14);
}
static int gr_barpt(int cx, int cy, int a0, int t, int p, int sx, Rgb *o)     /* the wheel texel at (t, p) of the bar */
{
    int s = gr_sinq(a0) >> 2, c = gr_cosq(a0) >> 2;
    return gr_wtex(gr_rnd(cx * 4096 + t * s + p * c, 16) + sx, gr_rnd(cy * 4096 - t * c + p * s, 16), o);
}
/* the left rim at the join: the strip's rim is the picture's rim next to the edge turned gr_dl about the centre, gr_dl
   the angle (17 .. 34 degrees, quarter degrees) at which the rim's first 4 degrees inside the picture differ least from
   the rim that far on (so its facets and mottling go on across the join) */
static void gr_join(void)
{
    long long best = -1; Rgb k1, k2;
    gr_dl = GR_PER;
    for (int d = 17 * 16; d <= 34 * 16; d += 4) {
        long long e = 0; int n = 0;
        for (int a = 13 * 16 + 8; a <= 17 * 16 + 8; a += 4)
            for (int r = 150 * 16; r <= 180 * 16; r += 24)
                if (gr_polar(GR_LCX, GR_LCY, r, a, 0, &k1) && gr_blue(k1) && gr_polar(GR_LCX, GR_LCY, r, a + d, 0, &k2) && gr_blue(k2)) { e += vb_d2(k1, k2); n++; }
        if (n < 200) continue;
        e = e * 64 / n;
        if (best < 0 || e < best) { best = e; gr_dl = d; }
    }
}
static int gr_spktex(int t, int p, int j, Rgb *o) { return gr_rimtex((t >> 4) - 75 + 37 * j, 40 + (p + GR_SPW) * 175 / GR_SPW, o); }
/* a spoke's lit texel: the rim texel there at the visible spoke's tone and contrast (g: a further tone, x 256) */
static void gr_spklit(int t, int p, int j, const int *g, Rgb *o)
{
    gr_spktex(t, p, j, o);
    if (gr_sl) {
        int l = clampi(gr_sl + (lum(*o) - gr_rmean(gr_rta) / 16) * clampi(gr_sd * 256 / gr_rd, 64, 256) / 256, 8, 255);
        o->r = clampi(gr_sm.r * l / gr_sl, 0, 255); o->g = clampi(gr_sm.g * l / gr_sl, 0, 255); o->b = clampi(gr_sm.b * l / gr_sl, 0, 255);
    } else *o = gr_gain(*o, gr_gS0);
    if (g) *o = gr_gain(*o, g);
}
static void gr_tones(void)
{
    GrSum rim = { 0 }, spk = { 0 }, rrim = { 0 }, rt = { 0 }, lt = { 0 }; Rgb k;
    gr_rmake(); gr_join();
    for (int a = 24 * 16; a <= 42 * 16; a += 8) for (int r = 152 * 16; r <= 178 * 16; r += 16) if (gr_polar(GR_LCX, GR_LCY, r, a, 0, &k) && gr_blue(k)) gr_add(&rim, k);
    for (int a = -36 * 16; a <= -26 * 16; a += 8) for (int r = 150 * 16; r <= 178 * 16; r += 16) if (gr_polar(GR_RCX, GR_RCY, r, a, -3, &k) && gr_blue(k)) gr_add(&rrim, k);
    for (int y = 150; y < 200; y++) for (int x = 0; x < 16; x++) {
        int t, p; gr_bar(GR_LEX, GR_LEY, GR_SPK, x * 16, y * 16, &t, &p);
        if (p >= -10 * 16 && p <= -2 * 16 && t >= 122 * 16 && t <= 145 * 16 && gr_wtex(x, y, &k) && gr_blue(k)) gr_add(&spk, k);
    }
    for (int a = 13 * 16 + 8; a <= 18 * 16; a += 8) for (int r = 187 * 16; r <= 207 * 16; r += 16) if (gr_polar(GR_LCX, GR_LCY, r, a, 0, &k) && gr_blue(k)) gr_add(&lt, k);
    for (int y = 141; y < 168; y++) for (int x = PW - 4; x < PW; x++) {      /* the picture's own (the quad ends before) */
        k = rgb15(pic[y][x]);
        if (!pic_ov[y][x] && gr_blue(k) && lum(k) > 90) gr_add(&rt, k);
    }
    gr_ratio(&spk, &rim, gr_gS0); gr_ratio(&rrim, &rim, gr_gR); gr_ratio(&rt, &lt, gr_gT);
    {   /* the spoke's lit face: its texels in the picture, and the rim texels the strip lays on the bar */
        GrSum pa = { 0 }, st = { 0 }; int da = 0, ds = 0;
        gr_sl = 0;
        for (int pass = 0; pass < 2; pass++) {
            for (int y = 158; y < PH; y++) for (int x = -40; x < 14; x++) {
                int t, p; gr_bar(GR_LEX, GR_LEY, GR_SPK, x * 16, y * 16, &t, &p);
                if (p < -GR_SPW + 16 || p > -24 || t > 146 * 16) continue;
                if (x >= 0) {
                    if (!(gr_wtex(x, y, &k) && gr_blue(k) && lum(k) >= 60)) continue;
                    if (!pass) gr_add(&pa, k); else da += abs(lum(k) - gr_sl);
                } else if (gr_spktex(t, p, 0, &k)) { if (!pass) gr_add(&st, k); else ds += abs(lum(k) - gr_rmean(gr_rta) / 16); }
            }
            if (pa.n < 12 || st.n < 12) { gr_sl = 0; break; }
            if (!pass) { gr_sm.r = pa.r / pa.n; gr_sm.g = pa.g / pa.n; gr_sm.b = pa.b / pa.n; gr_sl = lum(gr_sm); }
            else { gr_sd = da * 16 / pa.n + 1; gr_rd = ds * 16 / st.n + 1; }
        }
    }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) fprintf(stderr, "gears: rim turned %d/16 degrees, join lum %d/16; spoke lum %d dev %d/16 (rim texels %d/16); right rim %d %d %d (n %d), right tooth %d %d %d (n %d / %d)\n",
        gr_dl, gr_rmj, gr_sl, gr_sd, gr_rd, gr_gR[0], gr_gR[1], gr_gR[2], rrim.n, gr_gT[0], gr_gT[1], gr_gT[2], rt.n, lt.n);
#endif
}
/* the wheels' layer at wheel coordinates (x, y) in eighths of a pixel, outside the picture: 1 and the texel when a
   wheel is there */
static int gr_wheel(int side, int x, int y, Rgb *o)
{
    int t, p;
    if (!side) {
        int dx = x - GR_LCX * 8, dy = y - GR_LCY * 8, r = gr_isqrt((dx * dx + dy * dy) << 2), a = gr_ang(dx, dy);
        if (r > GR_TOP + 32) return 0;
        if (r > GR_ROUT) {                               /* a tooth: the picture's, a period on each */
            for (int k = 1; k <= 3; k++) {
                int as = a + GR_PER * k, ph = fmodi(as, GR_PER);     /* (teeth: 10.5 .. 22 degrees of each period) */
                if (GR_LCX * 16384 + (GR_ROUT >> 4) * gr_sinq(as) < 16384) continue;     /* (the source is off the picture) */
                if (ph < 10 * 16 + 8 || ph > 22 * 16 || as > 23 * 16) return 0;
                return gr_polar(GR_LCX, GR_LCY, gr_pout(as) + (r - GR_ROUT), as, 0, o);
            }
            return 0;
        }
        if (r >= GR_LIN - 24)                            /* the rim: the picture's next to the edge, turned gr_dl each time */
            for (int k = 1; k <= 12; k++) {
                int as = a + gr_dl * k, in = gr_pin(as), out = gr_pout(as), rs, w, g[3];
                rs = r < GR_LIN ? in + 32 : in + (r - GR_LIN) * (out - in) / (GR_ROUT - GR_LIN);
                if (GR_LCX * 16384 + (rs >> 4) * gr_sinq(as) < 2 * 16384) continue;      /* (the source is off the picture) */
                if (!(gr_polar(GR_LCX, GR_LCY, rs, as, 0, o) && gr_blue(*o)) && !(gr_polar(GR_LCX, GR_LCY, clampi(rs, in + 24, out - 24), as, 0, o) && gr_blue(*o)))
                    gr_rimtex(as * 16 / 89, (r - GR_LIN) * 255 / (GR_ROUT - GR_LIN), o);
                /* at the join the facet's tone is the picture's there, eased back to its own over 12 degrees of arc */
                w = k == 1 ? clampi(256 - (11 * 16 - a) * 256 / (12 * 16), 0, 256) : 0;
                g[0] = g[1] = g[2] = 256 + (clampi(gr_rmj * 256 / gr_rmean(as), 190, 345) - 256) * w / 256;
                *o = gr_gain(*o, g);
                if (r < GR_LIN) *o = scale(*o, 112);     /* the dark line under the rim */
                return 1;
            }
        if (r > GR_LIN + 16) return 0;
        for (int j = 0; j < 3; j++) {                    /* the spokes: this one's, then one a period out each (the
                                                            point turned back onto this one's bar) */
            int sn = gr_sinq(GR_PER * j), cs = gr_cosq(GR_PER * j);
            gr_bar(GR_LEX, GR_LEY, GR_SPK, GR_LCX * 16 + gr_rnd(dx * cs - dy * sn, 13), GR_LCY * 16 + gr_rnd(dx * sn + dy * cs, 13), &t, &p);
            if (t < 40 * 16) continue;
            if (p >= -GR_SPW && p <= 0) {
                int sh = 256 - clampi((t - 147 * 16) * 110 / (6 * 16), 0, 110);    /* (the rim's shadow on the spoke's top) */
                gr_spklit(t, p, j, 0, o); *o = scale(*o, sh); return 1;
            }
            if (p > 0 && p <= GR_SPD) {                  /* the side face: this spoke's own, slid along the bar */
                if (gr_barpt(GR_LEX, GR_LEY, GR_SPK, 108 * 16 + fmodi(t, 36 * 16), p, 0, o) && lum(*o) < 60) return 1;
                gr_rimtex(t >> 4, 128, o); *o = scale(*o, 60); return 1;
            }
        }
        return 0;
    } else {
        int dx = x - GR_RCX * 8, dy = y - GR_RCY * 8, r = gr_isqrt((dx * dx + dy * dy) << 2), a = gr_ang(dx, dy);
        if (r > GR_TOP + 32) return 0;
        if (r >= GR_RIN && r <= GR_ROUT + 8) {           /* the rim: the left wheel's, turned onto it */
            int as = a + (a < -4 * 16 ? 46 * 16 + 5 : a < 16 * 16 ? 26 * 16 : 6 * 16), in = gr_pin(as) + 16, out = gr_pout(as);
            if (!(gr_polar(GR_LCX, GR_LCY, in + (r - GR_RIN) * (out - in) / (GR_ROUT + 8 - GR_RIN), as, 0, o) && gr_blue(*o)))
                gr_rimtex(as * 16 / 89, (r - GR_RIN) * 255 / (GR_ROUT - GR_RIN), o);
            *o = gr_gain(*o, gr_gR); return 1;
        }
        if (r > GR_ROUT) {                               /* the tooth cut by the edge: its lit face */
            int al = gr_rnd((r >> 4) * (gr_sinq(a - GR_AXR) >> 2), 8);     /* columns off its axis (x 16) */
            if (r > GR_TOP - 8 || al < -12 * 16 || al > 14 * 16 + 5 - 3 * (r - GR_ROUT) / 16) return 0;
            if (!(gr_polar(GR_LCX, GR_LCY, r, a + 32 * 16 + 8, 0, o) && gr_blue(*o) && lum(*o) > 80)) {
                gr_rimtex(al / 16 + 20, (r - GR_ROUT) * 255 / (GR_TOP - GR_ROUT), o);
                *o = gr_gain(*o, gr_gR); return 1;
            }
            *o = gr_gain(*o, gr_gT); return 1;
        }
        gr_bar(GR_REX, GR_REY, GR_SPKR, x * 2, y * 2, &t, &p);
        if (t < 40 * 16) return 0;
        if (p >= 0 && p <= GR_SPW) {
            gr_spklit(t, p - GR_SPW, 0, gr_gR, o); *o = scale(*o, 256 - clampi((t - 140 * 16) * 110 / (6 * 16), 0, 110));
            return 1;
        }
        if (p < 0 && p >= -GR_SPD) {                     /* its side face: the tooth's side face, slid inwards */
            int rs = 186 * 16 + fmodi(-t, 24 * 16), al = -16 * 16 + (p + GR_SPD) * 64 / GR_SPD;
            if (gr_polar(GR_RCX, GR_RCY, rs, GR_AXR + al * 917 / rs, -3, o) && lum(*o) < 60) return 1;
            gr_rimtex(t >> 4, 128, o); *o = scale(*o, 60); return 1;
        }
        return 0;
    }
}
/* the wheel's cover of the pixel (x, y), 0 .. 16 (4 x 4 samples where its corners and centre disagree), and its texel
   there (the centre's, else the first sample's on the wheel): the wheels' outlines are drawn anti-aliased */
static int gr_cover(int side, int x, int y, Rgb *o)
{
    static const signed char O[4] = { -3, -1, 1, 3 };
    Rgb k; int c = gr_wheel(side, x * 8, y * 8, o), have = c, same = 1, n = 0;
    for (int j = 0; j < 4; j += 3) for (int i = 0; i < 4; i += 3) if (gr_wheel(side, x * 8 + O[i], y * 8 + O[j], &k) != c) same = 0;
    if (same) return c ? 16 : 0;
    for (int j = 0; j < 4; j++) for (int i = 0; i < 4; i++)
        if (gr_wheel(side, x * 8 + O[i], y * 8 + O[j], &k)) { n++; if (!have) { *o = k; have = 1; } }
    return n;
}

/* --- the background: wall, floor and the corbels */
static int gr_cbox(int x, int y) { return (x <= 62 && y >= 44 && y <= 90) || (x >= 256 && y >= 52 && y <= 116); }
static int gr_wallc(int x, int y)
{
    Rgb k;
    if (x < 0 || x >= PW || y < 0 || y >= 178 || pic_ov[y][x] || gr_cbox(x, y)) return 0;
    k = rgb15(pic[y][x]);
    return k.r - k.b >= 24 && k.r - k.g >= 8;
}
static int gr_floorc(int x, int y)
{
    Rgb k;
    if (x < 0 || x >= PW || y < 188 || y >= PH || pic_ov[y][x]) return 0;
    k = rgb15(pic[y][x]);
    return k.r >= 136 && k.g >= 96 && k.r - k.b >= 40;
}
static int gr_sandc(int x, int y)
{
    Rgb k;
    if (x < 0 || x >= PW || y < 0 || y >= PH || pic_ov[y][x] || !gr_cbox(x, y)) return 0;
    k = rgb15(pic[y][x]);
    return k.r >= 152 && k.g >= 104 && k.r - k.b >= 48;
}
static short gr_lf[PH][PW];                              /* the wall's light (0: no wall texel near) */
static Rgb gr_alb[PH][PW], gr_raw[PH][PW];               /* the wall's texels over their light (x 128); the raw texels */
static int gr_le[PH];                                    /* the wall's light at the edge per row (the side being made) */
static u8 gr_flat[PH][PW];                               /* the wall's light is even around (no shadow edge) */
static int gr_okw(int x, int y) { return gr_wallc(x, y) && gr_lf[y][x]; }
static int gr_srcw(int x, int y) { return gr_okw(x, y) && gr_flat[y][x]; }
static int gr_ctxw(int side, int x, int y, Rgb *c) { (void)side; if (!gr_okw(x, y)) return 0; *c = gr_alb[y][x]; return 1; }
static int gr_ctxf(int side, int x, int y, Rgb *c) { (void)side; if (!gr_floorc(x, y)) return 0; *c = gr_raw[y][x]; return 1; }
static int gr_ctxs(int side, int x, int y, Rgb *c) { (void)side; if (!gr_sandc(x, y)) return 0; *c = gr_raw[y][x]; return 1; }
static void gr_light(void)
{
    static int S[PH + 1][PW + 1], C[PH + 1][PW + 1];
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int m = gr_wallc(x, y);
            gr_raw[y][x] = rgb15(pic[y][x]);
            S[y + 1][x + 1] = S[y][x + 1] + S[y + 1][x] - S[y][x] + (m ? lum(gr_raw[y][x]) : 0);
            C[y + 1][x + 1] = C[y][x + 1] + C[y + 1][x] - C[y][x] + m;
        }
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int x0 = x - 8 < 0 ? 0 : x - 8, x1 = x + 9 > PW ? PW : x + 9, y0 = y - 8 < 0 ? 0 : y - 8, y1 = y + 9 > PH ? PH : y + 9;
            int s = S[y1][x1] - S[y0][x1] - S[y1][x0] + S[y0][x0], c = C[y1][x1] - C[y0][x1] - C[y1][x0] + C[y0][x0], l;
            Rgb k = gr_raw[y][x];
            gr_lf[y][x] = (short)(c >= 12 ? (s / c > 4 ? s / c : 4) : 0);
            l = gr_lf[y][x] ? gr_lf[y][x] : 64;
            gr_alb[y][x].r = k.r * 128 / l; gr_alb[y][x].g = k.g * 128 / l; gr_alb[y][x].b = k.b * 128 / l;
        }
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < PW; x++) {
            int mn = 999, mx = 0;
            for (int j = -6; j <= 6; j += 6) for (int i = -6; i <= 6; i += 6) {
                int xx = clampi(x + i, 0, PW - 1), yy = clampi(y + j, 0, PH - 1), l = gr_lf[yy][xx];
                if (!l) continue;
                if (l < mn) mn = l;
                if (l > mx) mx = l;
            }
            gr_flat[y][x] = mx * 4 <= mn * 5;
        }
}
static void gr_edge(int side)                            /* gr_le: the light of the row's first 12 wall texels from the
                                                            edge (within 48 columns: the wall seen through the wheel),
                                                            rows without any between their neighbours' */
{
    int last = -1;
    for (int y = 0; y < PH; y++) {
        int s = 0, c = 0;
        for (int i = 0; i < 48 && c < 12; i++) { int x = side ? PW - 1 - i : i; if (gr_okw(x, y)) { s += gr_lf[y][x]; c++; } }
        gr_le[y] = c >= 2 ? s / c : -1;
    }
    for (int y = 0; y < PH; y++) {
        if (gr_le[y] < 0) continue;
        if (last < 0) for (int i = 0; i < y; i++) gr_le[i] = gr_le[y];
        else for (int i = last + 1; i < y; i++) gr_le[i] = gr_le[last] + (gr_le[y] - gr_le[last]) * (i - last) / (y - last);
        last = y;
    }
    if (last < 0) last = 0, gr_le[0] = 64;
    for (int i = last + 1; i < PH; i++) gr_le[i] = gr_le[last];
    {   int t[PH];                                       /* smoothed over 5 rows */
        for (int y = 0; y < PH; y++) { int s = 0, n = 0; for (int d = -2; d <= 2; d++) if (y + d >= 0 && y + d < PH) { s += gr_le[y + d]; n++; } t[y] = s / n; }
        memcpy(gr_le, t, sizeof t);
    }
}
static const RqBand GR_BW = { 0, PH, 12, 16, 5, 4, 0, PW, 0, 178, -1 };
static const RqBand GR_BF = { 184, PH, 12, 12, 5, 4, 0, PW, 188, PH, 10 };
static const RqBand GR_BS = { 36, 180, 8, 8, 4, 3, 0, PW, 44, 100, -1 };
#define GR_ROOTL 75                                      /* the left corbel's root: strip columns */
/* the corbels at strip column X (1 ..), row y: 0 none, 1 the lit face, 2 the underside */
static int gr_corbel(int side, int X, int y)
{
    int y16 = y * 16 + 8;
    if (!side) {
        int top = 760 - X * 23 / 16, bot = 1349 + X * 67 / 16;       /* 47.5 + 0.09 x, 84.3 - 0.26 x, x = -X + 0.5 */
        if (X > GR_ROOTL) return 0;
        return y16 < top ? 0 : y16 <= bot ? 1 : y16 <= bot + 56 ? 2 : 0;
    } else {
        int top = 55 * 16 + 8, bot = 95 * 16 + 8 + X * 16;     /* (its brace runs off the strip, behind the wheel) */
        if (bot > 172 * 16) bot = 172 * 16;
        return y16 < top ? 0 : y16 <= bot ? 1 : y16 <= bot + 15 * 16 ? 2 : 0;
    }
}
/* the lit face's tone in the picture's 3 edge columns over its tone in the 12 after them (x 256): the left corbel is
   darker in its first columns, so the strip starts as dark and eases to the face's tone over 12 columns */
static int gr_cedge(int side)
{
    int a = 0, na = 0, b = 0, nb = 0;
    for (int y = 44; y < 100; y++)
        for (int i = 0; i < 15; i++) {
            int x = side ? PW - 1 - i : i;
            if (!gr_sandc(x, y) && !(i < 3 && gr_cbox(x, y) && !pic_ov[y][x] && lum(rgb15(pic[y][x])) > 100)) continue;
            if (i < 3) { a += lum(rgb15(pic[y][x])); na++; } else { b += lum(rgb15(pic[y][x])); nb++; }
        }
    return na >= 20 && nb >= 40 ? clampi((a * 256 / na) / (b / nb + 1), 200, 280) : 256;
}
static int gr_foot16(int side, int X) { return side ? 187 * 16 + 13 + X * 16 / 5 : 187 * 16 + 10 + X * 16 / 5; }

static void gen_gr(int pad)
{
    Rgb *lw, *lf, *ls; size_t n;
    pal_make();
    vb_ow = pad + 64; n = (size_t)vb_ow * PH;
    if (!canvas_open(vb_ow)) return;
    rq_have = (u8 *)malloc(n); lw = (Rgb *)malloc(3 * n * sizeof *lw);
    if (!rq_have || !lw) { free(rq_have); free(lw); rq_have = 0; canvas_close(); return; }
    lf = lw + n; ls = lf + n;
    gr_dyo = room->sec == 3 ? -24 : 0;
    gr_light(); gr_tones();
    rq_map = 0;
    for (int side = 0; side < 2; side++) {
        int ce = gr_cedge(side); Rgb us[16];
        for (int d = 0; d < 16; d++) {                   /* the right corbel's underside by depth */
            int r = 0, g = 0, b = 0, n = 0;
            for (int x = PW - 10; x < PW; x++) {
                int y = 95 - (PW - 1 - x) + 1 + d; Rgb k;
                if (y >= PH || pic_ov[y][x]) continue;
                k = rgb15(pic[y][x]); r += k.r; g += k.g; b += k.b; n++;
            }
            us[d].r = n ? r / n : 0; us[d].g = n ? g / n : 0; us[d].b = n ? b / n : 0;
        }
        cur_side = side; gr_edge(side);
        canvas_clear(); memset(rq_have, 0, n);
        rq_src = gr_alb; rq_ctx = gr_ctxw; rq_ok = gr_srcw; rq_quilt(side, &GR_BW, 6101 + side);
        memcpy(lw, cv, n * sizeof *lw);
        canvas_clear(); memset(rq_have, 0, n);
        rq_src = gr_raw; rq_ctx = gr_ctxf; rq_ok = gr_floorc; rq_quilt(side, &GR_BF, 6111 + side);
        memcpy(lf, cv, n * sizeof *lf);
        canvas_clear(); memset(rq_have, 0, n);
        rq_src = gr_raw; rq_ctx = gr_ctxs; rq_ok = gr_sandc; rq_quilt(side, &GR_BS, 6121 + side);
        memcpy(ls, cv, n * sizeof *ls);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                size_t i = (size_t)y * vb_ow + X - 1; int x = side ? PW - 1 + X : -X, cb = gr_corbel(side, X, y), got; Rgb c, w = { 0, 0, 0 };
                if (y * 16 + 8 < gr_foot16(side, X)) {
                    int l = gr_le[y]; c = lw[i];
                    c.r = clampi(c.r * l / 128, 0, 255); c.g = clampi(c.g * l / 128, 0, 255); c.b = clampi(c.b * l / 128, 0, 255);
                } else c = lf[i];
                if (!side && X > GR_ROOTL && X <= GR_ROOTL + 6 && y >= 43 && y < 109) c = scale(c, 150 + (X - GR_ROOTL) * 16);
                if (cb == 1) {
                    c = scale(ls[i], 236 + noise(x, y, 13, 9, 77 + side) * 40 / 255);   /* (the quilt's repeats broken up) */
                    if (X <= 12) c = scale(c, ce + (256 - ce) * (X - 1) / 12);
                }
                else if (cb == 2 && side) {              /* the right underside: the picture's tone at that depth under
                                                            the lower edge (its last 10 columns), a low noise over it */
                    int d = clampi(y - (95 + X), 0, 15);
                    if (us[d].r + us[d].g + us[d].b) c = scale(us[d], 232 + noise(x, y, 5, 4, 91) * 48 / 255);
                } else if (cb == 2) {                    /* the left one: the picture's, slid along the lower edge */
                    int k = (X + 11) / 12 * 12, sx = x + k, sy = y - k * 67 / 256;
                    if (sx >= 0 && sx < PW && sy >= 0 && sy < PH && !pic_ov[sy][sx]) c = rgb15(pic[sy][sx]);
                }
                /* the wheels: the picture's (s3: under the quads' one), then the quads' one, down to its last row */
                got = gr_cover(side, x, y, &w);
                if (got) c = mix(c, w, got * 16);
                if (gr_dyo && y < 200 && (got = gr_cover(side, x + 3 * side, y - gr_dyo, &w)) != 0) c = mix(c, w, got * 16);
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(c);
            }
    }
    free(rq_have); rq_have = 0; free(lw);
    canvas_close();
}
