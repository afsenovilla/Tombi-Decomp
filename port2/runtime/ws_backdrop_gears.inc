/* runtime/ws_backdrop.c: AREA16 s3 / s6, the gears room (gen_gr; included after ws_backdrop_rooms2.inc; docs/PORT.md
 * "More rooms with picture strips").  The same rules as the other generators: every texel from the user's picture at
 * run time, integers only, position hashes, a structure depends on where it is and not on the strip width; the
 * tables hold coordinates only.
 *
 * Both sections draw the same picture (pages (128, 0) / (256, 0), 256 rows drawn, 224 seen): a brick room with grey
 * stone gears on the back wall, a sandstone corbel sticking out of each side wall, and two big blue cog wheels in
 * the foreground whose hubs are under the frame (the "blue beams" of the first note are their rims and spokes).  The
 * wheels are in the picture and are drawn again over the player as quads of page (256, 0) (u 64 v 0 144 x 144 the
 * left one, u 64 v 144 72 x 96 the right one, s6 also u 184 v 216 64 x 40), on the picture's wheels.  (The PAL disc
 * places s3's quads 24 rows too high, a slip of its own that the port puts right: runtime/ntsc.c pal_apd_fix.)  The
 * right quad lies 3 columns left of the picture's wheel and ends 4 columns before the edge.
 *
 * Measured (wheel coordinates = the picture's):
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
 *     edge row 55.5, the lit face's lower edge 96.5 at the edge falling 1 row a column over its last 12 columns, the
 *     dark underside below it down to a level base at row 105.5 (the wall under it): a short block, closed where
 *     the face reaches the base, 9 columns out; their tips (columns 62 and 256) are mirror places about the room's
 *     axis, so the left one ends 9 columns out as well (gr_corbel);
 *   - the room is a box seen straight on: the side walls' feet (row 186 at x 8 rising to 178 at x 48 on the left,
 *     0.2 rows a column) and courses meet at (162, 155), the floor's joints at (159.6, 158.8); the back wall is
 *     columns 47-273 and its courses are 6.8 rows apart (the row profile's autocorrelation: 0.6 at 7 rows, 0.55 at
 *     14); the side walls' courses are the rays through them.
 *
 * The strips, back to front:
 *   - the side walls, in perspective: whole bricks of the back wall laid joint to joint along each course's ray
 *     (gr_courses, gr_side), their texels over their light (17 x 17 mean luminance of the wall texels), the picture's
 *     grain given back at one texel a pixel (gr_grain), lit as the picture's wall at the edge along the wall's level
 *     lines (gr_wlight); the foot lines go on with their slope, anti-aliased;
 *   - the floor, in perspective: each strip texel is the picture's floor texel of its row a whole number of slabs
 *     towards the middle (gr_floor: a shear about the joints' vanishing point), so the slabs, their joints and the
 *     grain go on at the picture's own scale;
 *   - the corbels: both closed 9 columns out on the side walls.  The left face goes on as its own shaded columns
 *     (gr_lface), its underside the picture's slid along the lower edge; the right face is laid from clean pieces
 *     of the faces (gr_face), its underside the picture's tone by depth under the edge; their edges anti-aliased;
 *   - the wheels, round: the left rim is the picture's rim next to the edge turned about the centre by the angle at
 *     which it matches itself best (gr_join, about 30 degrees: three of its segments), its radii scaled by the
 *     band's edges there and here, its facet's tone at the join the picture's there, eased back over 12 degrees of
 *     arc; the teeth are the picture's a tooth period (44 degrees) on (the next one lands at -28 degrees, 95 columns
 *     out); the spokes are bars along the measured edge, textured with rim texels (the rim band laid straight) at
 *     the visible spoke's tone and contrast, their side face the picture's slid along the bar; the outlines
 *     anti-aliased (gr_cover); the right rim is the left wheel's rim turned onto it (its own clean rim is 12 degrees
 *     long), 1.19 times as large (the nearer wheel's rim is that much wider), at the right rim's tone; the cut
 *     tooth's lit face is the inside of the left tooth's turned onto it.  Turned texels are taken cubic between the
 *     picture's (gr_polar).
 * No texel is mirrored: every map is a rotation, a slide, a shear or a scale. */

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
#define GR_RSC 305                                       /* the right rim's width over the left rim's (x 256) */

static int gr_blue(Rgb k) { return k.b > k.r + 25 && k.b >= k.g; }
/* a wheel texel of the quads' wheel at wheel coordinates (x, y): sx columns off (the right quad: -3) */
static int gr_wtex(int x, int y, Rgb *o)
{
    if (x < 0 || x >= PW || y < 0 || y >= PH) return 0;
    *o = rgb15(pic[y][x]);
    return pic_ov[y][x] || (gr_blue(*o) && lum(*o) < 60);     /* (the dark faces are the picture's only) */
}
static int gr_lit(int x, int y, Rgb *o) { return gr_wtex(x, y, o) && gr_blue(*o) && lum(*o) >= 60; }
/* Catmull-Rom weights (x 256) of the four texels around a point f sixteenths past the second */
static void gr_cr4(int f, int *w)
{
    int t = f * 16, t2 = t * t / 256, t3 = t2 * t / 256;
    w[0] = (-t3 + 2 * t2 - t) / 2; w[1] = (3 * t3 - 5 * t2 + 512) / 2; w[2] = (-3 * t3 + 4 * t2 + t) / 2; w[3] = 256 - w[0] - w[1] - w[2];
}
/* (the texel at radius r16, angle a: a turned face keeps the picture's mottling when it is taken between the texels
   around the point - cubic over the 4 x 4 around it when they are all lit faces (bilinear loses a fifth of the
   mottle's contrast - measured, the 5 x 5 high-pass deviation 6.6 for the picture's 8.1 - and nearest gives a
   speckle), bilinear over the 2 x 2 when only they are; else the nearest texel) */
static int gr_pq;                                        /* how gr_polar took its last texel: 2 cubic, 1 bilinear, 0 nearest */
static int gr_polar(int cx, int cy, int r16, int a, int sx, Rgb *o)
{
    int xf = cx * 16 + gr_rnd(r16 * (gr_sinq(a) >> 2), 12) - 8, yf = cy * 16 - gr_rnd(r16 * (gr_cosq(a) >> 2), 12) - 8;     /* (x 16, from the texel centres) */
    int x0 = fdiv(xf, 16), y0 = fdiv(yf, 16), fx = xf - x0 * 16, fy = yf - y0 * 16, all = 1; Rgb k[4][4];
    for (int j = 1; j < 3 && all; j++) for (int i = 1; i < 3 && all; i++) all = gr_lit(x0 - 1 + i + sx, y0 - 1 + j, &k[j][i]);
    gr_pq = 0;
    if (!all) return gr_wtex(x0 + (fx >= 8) + sx, y0 + (fy >= 8), o);
    for (int j = 0; j < 4 && all; j++) for (int i = 0; i < 4 && all; i++) if (j < 1 || j > 2 || i < 1 || i > 2) all = gr_lit(x0 - 1 + i + sx, y0 - 1 + j, &k[j][i]);
    if (all) {
        int wx[4], wy[4], r = 0, g = 0, b = 0;
        gr_cr4(fx, wx); gr_cr4(fy, wy);
        for (int j = 0; j < 4; j++) for (int i = 0; i < 4; i++) { int w = wx[i] * wy[j]; r += k[j][i].r * w; g += k[j][i].g * w; b += k[j][i].b * w; }
        o->r = clampi(gr_rnd(r, 16), 0, 255); o->g = clampi(gr_rnd(g, 16), 0, 255); o->b = clampi(gr_rnd(b, 16), 0, 255);
        gr_pq = 2;
    } else { gr_pq = 1; *o = mix(mix(k[1][1], k[1][2], fx * 16), mix(k[2][1], k[2][2], fx * 16), fy * 16); }
    return 1;
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
                /* (the band's edges are not quite gr_pin / gr_pout everywhere: a texel of the picture's dark outline
                   within 3 texels of an edge is taken further into the face, so the strip's outline is one clean line) */
                if (r >= GR_LIN && (r < GR_LIN + 48 || r > GR_ROUT - 48))
                    for (int q = 1; q <= 4 && lum(*o) < 62; q++) {
                        Rgb f;
                        if (gr_polar(GR_LCX, GR_LCY, clampi(rs + (r < GR_LIN + 48 ? q * 16 : -q * 16), in + 24, out - 24), as, 0, &f) && gr_blue(f)) *o = f;
                    }
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
            /* (the right rim is 40 texels wide, the left one 33.5: the nearer wheel.  The left rim's 24 - 40.8 degrees
               are laid on 20 degrees of it, 1.19 times as large along the rim as across it, from the outer edge in:
               a first version stretched them across only, which drew the mottle out into streaks) */
            int as = 24 * 16 + fmodi(a + 24 * 16, 20 * 16) * 256 / GR_RSC, in = gr_pin(as) + 16, out = gr_pout(as);
            int rs = clampi(out - (GR_ROUT + 8 - r) * 256 / GR_RSC, in, out);
            if (!(gr_polar(GR_LCX, GR_LCY, rs, as, 0, o) && gr_blue(*o)))
                gr_rimtex(as * 16 / 89, (r - GR_RIN) * 255 / (GR_ROUT - GR_RIN), o);
            if (r < GR_RIN + 48 || r > GR_ROUT - 40)
                for (int q = 1; q <= 4 && lum(*o) < 62; q++) {
                    Rgb f;
                    if (gr_polar(GR_LCX, GR_LCY, clampi(rs + (r < GR_RIN + 48 ? q * 16 : -q * 16), in + 24, out - 24), as, 0, &f) && gr_blue(f)) *o = f;
                }
            *o = gr_gain(*o, gr_gR); return 1;
        }
        if (r > GR_ROUT) {                               /* the tooth cut by the edge: its lit face */
            int al = gr_rnd((r >> 4) * (gr_sinq(a - GR_AXR) >> 2), 8);     /* columns off its axis (x 16) */
            if (r > GR_TOP - 8 || al < -12 * 16 || al > 14 * 16 + 5 - 3 * (r - GR_ROUT) / 16) return 0;
            int ok = 0;
            /* (the left tooth is cut by the picture's edge and its first columns there are darker: the source is taken
               from its sixth column on, a little further round where it would fall before that) */
            for (int q = 0; q < 8 && !ok; q++) {
                int as = a + 32 * 16 + 8 + q * 24;
                if (GR_LCX * 16384 + (r >> 4) * gr_sinq(as) < 6 * 16384) continue;
                ok = gr_polar(GR_LCX, GR_LCY, r, as, 0, o) && gr_pq == 2;      /* (inside the face: none of its outline's texels) */
            }
            if (!ok) {
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
static short (*gr_lf)[PW];                              /* the wall's light (0: no wall texel near) */
static Rgb (*gr_alb)[PW], (*gr_raw)[PW];               /* the wall's texels over their light (x 128); the raw texels */
static int gr_le[PH];                                    /* the wall's light at the edge per row (the side being made) */
static u8 (*gr_flat)[PW];                               /* the wall's light is even around (no shadow edge) */
static int gr_okw(int x, int y) { return gr_wallc(x, y) && gr_lf[y][x]; }
static int gr_srcw(int x, int y) { return gr_okw(x, y) && gr_flat[y][x]; }
static int (*gr_light__S)[PW + 1];
static int (*gr_light__C)[PW + 1];
static void gr_light(void)
{
    int (*const S)[PW + 1] = gr_light__S, (*const C)[PW + 1] = gr_light__C;
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
/* --- the side walls.  The room is a box seen straight on: both side walls' feet and courses meet at (162, 155); the
   back wall is columns 47-273 (its foot row 178) and what the edges cut are the side walls.  Their brick is the back
   wall's, the courses carried round the corners: measured, the left wall's mortar lines (columns 0-46) lie on the
   rays from that point through the back wall's (6.8 rows apart, rows 49 .. 172; the right wall's within a row).  So
   a strip texel at dx columns from the vanishing point shows the back wall's brick of row 155 + (y - 155) xc / dx
   (xc: the corner's distance, 115 / 111) at GR_F (1 - xc / dx) columns along the wall (GR_F: the picture's focal
   length, taken as 320: a 53 degree view), i.e. every course is a ray and the bricks grow towards the viewer as the
   picture's own side walls' do.
   The brick: the back wall is mostly hidden by the gears, so its whole bricks are collected (gr_courses: per course
   the columns whose every row is a wall texel in even light, cut at the upright joints = the maxima of the course's
   column profile; a brick is the stretch from one joint to the next, 7 .. 30 columns) and every strip course is laid
   from bricks of the whole pool, joint to joint, picked by a position hash (never one of the course's last two, nor
   the one beside it in the course above), each brick's rows scaled to the course's.  The texels are taken over their
   light (gr_alb).
   The side walls are nearer than the back wall, so their texels are magnified (rows x 1.4 at the join, x 1.9 at
   the 16:9 edge) and come out softer than the picture's own side walls, which keep the picture's grain (palette
   noise at 1 texel).  The grain is given back at its own scale (gr_grain): the wall's texels less their row's
   5-texel mean (no mortar line in it), one texel a strip pixel. */
#define GR_VY (155 * 256)
#define GR_F 320
static const int GR_XC[2] = { 115, 111 };
#define GR_NC 32
#define GR_NB 96
static short gr_mr[GR_NC + 1], (*gr_bm)[256]; static u8 (*gr_bd)[256]; static int gr_nc; static u8 gr_cr[PH];
static short gr_bk[GR_NB][3]; static int gr_nb;         /* the bricks: course, first column (its joint), columns */
static void gr_bricks(int even)
{
    gr_nb = 0;
    for (int c = 0; c < gr_nc; c++) {
        int p[PW], h = gr_mr[c + 1] - gr_mr[c], lastj = -1; u8 ok[PW];
        if (h < 5 || h > 9) continue;
        for (int x = 0; x < PW; x++) {
            int s = 0;
            ok[x] = x >= 52 && x < 267;
            for (int y = gr_mr[c]; ok[x] && y < gr_mr[c + 1]; y++) ok[x] = even ? gr_srcw(x, y) : gr_okw(x, y);
            for (int y = gr_mr[c] + 2; ok[x] && y < gr_mr[c + 1] - 1; y++) s += lum(gr_alb[y][x]);
            p[x] = ok[x] ? s * 16 / (h - 3) : 0;
        }
        for (int x = 56; x < 263; x++) {
            int m = 0;
            if (!ok[x]) { lastj = -1; continue; }
            for (int d = -4; d <= 4 && m >= 0; d++) m = ok[x + d] ? m + p[x + d] : -1;
            if (m < 0) continue;                         /* (a stretch's first and last 4 columns: no joint looked for) */
            if (p[x] * 9 - m > 9 * 9 * 16 && p[x] >= p[x - 1] && p[x] > p[x + 1]) {      /* an upright joint */
                if (lastj >= 0 && x - lastj >= 7 && x - lastj <= 30 && gr_nb < GR_NB) {
                    int clean = 1;
                    for (int i = lastj; i < x; i++) if (!ok[i]) clean = 0;
                    if (clean) { gr_bk[gr_nb][0] = (short)c; gr_bk[gr_nb][1] = (short)lastj; gr_bk[gr_nb][2] = (short)(x - lastj); gr_nb++; }
                }
                lastj = x;
            }
        }
    }
}
static void gr_courses(void)
{
    int bw[PH], hp[PH], n = 0, first = -1, last = 0, m[GR_NC];
    for (int y = 0; y < PH; y++) {
        int s = 0, c = 0;
        for (int x = 60; x < 265; x++) if (gr_okw(x, y)) { s += lum(gr_raw[y][x]); c++; }
        bw[y] = c >= 20 ? s * 16 / c : -1;
        if (bw[y] >= 0) { if (first < 0) first = y; last = y; }
    }
    if (first < 0) { first = 40; last = 177; }
    for (int y = 0; y < PH; y++) if (bw[y] < 0) bw[y] = bw[clampi(y, first, last)] >= 0 ? bw[clampi(y, first, last)] : 1600;
    for (int y = 0; y < PH; y++) { int s = 0; for (int d = -4; d <= 4; d++) s += bw[clampi(y + d, 0, PH - 1)]; hp[y] = bw[y] - s / 9; }
    for (int y = first + 1; y < last && n < GR_NC - 8; y++)     /* the mortar lines: the row profile's maxima */
        if (hp[y] > 32 && hp[y] > hp[y - 1] && hp[y] >= hp[y + 1] && (!n || y - m[n - 1] >= 5)) {
            while (n && y - m[n - 1] > 10 && n < GR_NC - 8) { m[n] = m[n - 1] + 7; n++; }     /* (one missed) */
            m[n++] = y;
        }
    if (n < 2) { n = 0; for (int y = 49; y < 178 && n < GR_NC - 8; y += 7) m[n++] = y; }
    gr_nc = 0;
    {   int k = 0, y0 = m[0]; while (y0 > 7) { y0 -= 7; k++; }           /* (and above the first, below the last) */
        for (int i = 0; i < k && gr_nc < GR_NC; i++) gr_mr[gr_nc++] = (short)(y0 + 7 * i);
    }
    for (int i = 0; i < n && gr_nc < GR_NC; i++) gr_mr[gr_nc++] = (short)m[i];
    while (gr_nc < GR_NC && gr_mr[gr_nc - 1] + 7 < 178) { gr_mr[gr_nc] = (short)(gr_mr[gr_nc - 1] + 7); gr_nc++; }
    gr_mr[gr_nc] = 178;
    for (int y = 0, c = 0; y < PH; y++) { while (c + 1 < gr_nc && y >= gr_mr[c + 1]) c++; gr_cr[y] = (u8)c; }
    gr_bricks(1);
    if (gr_nb < 24) gr_bricks(0);                        /* (too few in even light: any wall texel) */
    for (int c = 0; c < gr_nc; c++) {                    /* the courses' bricks, end to end */
        int u = -(int)(hash3(c, 7, 3) % 12u), last1 = -1, last2 = -1;
        for (int i = 0; u < 256 && gr_nb; i++) {
            int b = (int)(hash3(c, i, 911) % (u32)gr_nb), up = c ? gr_bd[c - 1][clampi(u + 4, 0, 255)] : 255, upx = c ? gr_bm[c - 1][clampi(u + 4, 0, 255)] : -1;
            for (int t = 0; t < 8; t++) {
                int same = b == last1 || b == last2 || (gr_bk[b][0] == up && upx >= gr_bk[b][1] && upx < gr_bk[b][1] + gr_bk[b][2]);
                if (!same) break;
                b = (b + 1 + (int)(hash3(c, i, 77 + t) % 5u)) % gr_nb;
            }
            for (int j = 0; j < gr_bk[b][2]; j++, u++) if (u >= 0 && u < 256) { gr_bm[c][u] = (short)(gr_bk[b][1] + j); gr_bd[c][u] = (u8)gr_bk[b][0]; }
            last2 = last1; last1 = b;
        }
    }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) fprintf(stderr, "gears: %d courses, %d bricks\n", gr_nc, gr_nb);
#endif
}
static int gr_brow(int c, int d, int y)                  /* the row of course d for row y of course c */
{
    int hc = gr_mr[c + 1] - gr_mr[c], hd = gr_mr[d + 1] - gr_mr[d];
    return gr_mr[d] + clampi(((y - gr_mr[c]) * hd + hd / 2) / hc, 0, hd - 1);
}
static Rgb gr_brick1(int u, int y)                       /* the wall's brick over its light: column u along the wall, row y */
{
    int c = gr_cr[y], d; Rgb o = { 128, 100, 80 };
    if (!gr_nb) return o;
    d = gr_bd[c][u & 255];
    return gr_alb[gr_brow(c, d, y)][gr_bm[c][u & 255]];
}
/* the wall's grain for strip column X, row y (v: its row on the back wall): a wall texel less its row's 5-texel mean,
   over the light, x GR_GRAIN / 256; one texel a pixel along the row, the next row of the brick a pixel down */
#define GR_GRAIN 150
static void gr_grain(int X, int y, int v, int *d)
{
    int c = gr_cr[clampi(v, 0, 177)], g, dd, gx, gy, h; Rgb m = { 0, 0, 0 }, k;
    d[0] = d[1] = d[2] = 0;
    if (!gr_nb) return;
    dd = gr_bd[c][(X + 37 * c) & 255]; h = gr_mr[dd + 1] - gr_mr[dd] - 1;
    g = (X + 37 * c + 53 * fdiv(y + 5 * c, h)) & 255;
    dd = gr_bd[c][g]; gx = gr_bm[c][g]; h = gr_mr[dd + 1] - gr_mr[dd] - 1;
    gy = gr_mr[dd] + 1 + fmodi(y + 5 * c, h);
    for (int i = -2; i <= 2; i++) {
        if (!gr_okw(gx + i, gy)) return;
        m.r += gr_alb[gy][gx + i].r; m.g += gr_alb[gy][gx + i].g; m.b += gr_alb[gy][gx + i].b;
    }
    k = gr_alb[gy][gx];
    d[0] = (k.r * 5 - m.r) * GR_GRAIN / (5 * 256); d[1] = (k.g * 5 - m.g) * GR_GRAIN / (5 * 256); d[2] = (k.b * 5 - m.b) * GR_GRAIN / (5 * 256);
}
static Rgb gr_side(int side, int X, int y)               /* the side wall at strip column X, row y: over its light (x 128) */
{
    int dx = side ? 157 * 256 + 128 + X * 256 : 162 * 256 - 128 + X * 256, k = GR_XC[side] * 256 * 256 / (dx >> 4) * 16;     /* xc / dx, x 65536 */
    int v = GR_VY + (int)(((long long)(y * 256 + 128 - GR_VY) * k) >> 16) - 128, u = (GR_F * (65536 - k)) >> 8;
    int y0 = clampi(v >> 8, 0, 176), fy = v < 0 ? 0 : v & 255, u0 = u >> 8, fu = u & 255, d[3];
    Rgb a, b;
    fy = clampi(fy * 2 - 128, 0, 256); fu = clampi(fu * 2 - 128, 0, 256);     /* (half as soft as bilinear: the picture's own side walls keep their mortar lines a texel wide) */
    a = mix(gr_brick1(u0, y0), gr_brick1(u0 + 1, y0), fu); b = mix(gr_brick1(u0, y0 + 1), gr_brick1(u0 + 1, y0 + 1), fu);
    a = mix(a, b, fy);
    gr_grain(X, y, (v + 128) >> 8, d);
    a.r += d[0]; a.g += d[1]; a.b += d[2];
    return a;
}
/* the side wall's light at strip column X, row y: the picture's wall at the edge (gr_le), followed along the wall's
   level lines (the rays), so a tone the edge cuts goes on in the wall's perspective and not across the screen */
static int gr_wlight(int side, int X, int y)
{
    int d0 = side ? 158 : 162, r = 155 * 256 + 128 + (y * 256 + 128 - (155 * 256 + 128)) * d0 / (d0 + X) - 128;
    int y0 = clampi(r >> 8, 0, PH - 2), f = r < 0 ? 0 : r & 255;
    return gr_le[y0] * 16 + (gr_le[y0 + 1] - gr_le[y0]) * f / 16;
}
/* --- the floor: sandstone slabs running away from the viewer, their joints straight lines of the picture that meet
   at (159.6, 158.8) (three of them fitted over 24-47 texels each: x = 237.6 - 0.490 y', 457.3 - 1.877 y', 14.6 +
   0.912 y' with y' the row; a fourth and a fifth within a texel of the same fan), 1.395 columns apart per row under
   that point.  The slabs' grain is streaks across the room, so a texel's scale depends on its row only: a strip texel
   takes the picture's floor texel of its own row, n slabs towards the middle (a shear about the vanishing row:
   x' = x - n 1.395 (y - 158.8)), n such that it lands on the one slab the picture shows whole (between the joints
   of slopes -0.490 and 0.905).  So the strips' slabs have the picture's texels at their own scale, the joints go on
   as the fan's next lines (the slab's left joint comes with it) and nothing is magnified or quilted.  The floor's
   light changes with the row (darker towards the wall) and a little with the side: each row's tone is the picture's
   floor next to that edge over the source slab's (9 rows' means). */
#define GR_FVX 2554
#define GR_FVY 2541
#define GR_FJ0 (-502)                                    /* the whole slab's left joint: slope x 1024 */
#define GR_FJP 1428                                      /* the joints' pitch */
static int gr_fg[PH]; static Rgb gr_fm;                  /* the row's tone (x 256) for the side being made; the floor's mean colour */
static int gr_fslab(int x, int y, int *sx)               /* the slab of (x, y); *sx: the column of the same point of the whole slab */
{
    int dy = y * 16 + 8 - GR_FVY, n;
    if (dy < 16) dy = 16;
    n = fdiv(fdiv((x * 16 + 8 - GR_FVX) * 1024, dy) - GR_FJ0 + 51, GR_FJP);      /* (51: the joint's own texels stay with the slab on its right) */
    *sx = x - fdiv(n * GR_FJP * dy + 8192, 16384);
    return n;
}
static void gr_ftone(int side)
{
    int le[PH], ls[PH], ne[PH], ns[PH], r = 0, g = 0, b = 0, n = 0;
    for (int y = 0; y < PH; y++) {
        le[y] = ls[y] = ne[y] = ns[y] = 0;
        for (int x = 0; x < PW; x++) {
            int sx, l;
            if (!gr_floorc(x, y)) continue;
            l = lum(gr_raw[y][x]);
            if (side ? x >= 200 : x < 72) { le[y] += l; ne[y]++; }
            if (!gr_fslab(x, y, &sx)) { ls[y] += l; ns[y]++; r += gr_raw[y][x].r; g += gr_raw[y][x].g; b += gr_raw[y][x].b; n++; }
        }
    }
    gr_fm.r = n ? r / n : 200; gr_fm.g = n ? g / n : 150; gr_fm.b = n ? b / n : 100;
    for (int y = 0; y < PH; y++) {
        int a = 0, na = 0, s = 0, nsum = 0;
        for (int d = -4; d <= 4; d++) if (y + d >= 0 && y + d < PH) { a += le[y + d]; na += ne[y + d]; s += ls[y + d]; nsum += ns[y + d]; }
        gr_fg[y] = na >= 40 && nsum >= 40 ? clampi((a * 256 / na) / (s / nsum + 1), 216, 296) : 256;
    }
}
static Rgb gr_floor(int x, int y)
{
    int sx; Rgb o;
    if (y < 188) y = 188;
    gr_fslab(x, y, &sx);
    if (!gr_floorc(sx, y)) {                             /* (something lies on the slab there: the nearest floor texel of the row) */
        int f = 0;
        for (int d = 1; d <= 24 && !f; d++) { if (gr_floorc(sx + d, y)) { sx += d; f = 1; } else if (gr_floorc(sx - d, y)) { sx -= d; f = 1; } }
        if (!f) return scale(gr_fm, gr_fg[y]);
    }
    o = scale(gr_raw[y][sx], gr_fg[y]);
    o.r = clampi(o.r, 0, 255); o.g = clampi(o.g, 0, 255); o.b = clampi(o.b, 0, 255);
    return o;
}
/* --- the corbels.  Their tips are at columns 62 and 256, mirror places about the room's axis (159.6, the floor
   joints' vanishing column: 97.6 and 96.4 columns from it); the right one's end is in reach: its lit face's lower
   edge comes down 1 row a column to a level base at row 105.5, which it meets 9 columns past the edge, so the block is
   73 columns long and ends on the side wall there.  The left one's edges are straight to the picture's edge; the same
   length (the same plane across the room) puts its end 9.3 columns past the left edge, on the side wall at the
   depth of the right one's: both are closed 9 columns out and the walls go on past them. */
#define GR_ROOTL 9                                       /* the left corbel's root: strip columns */
#define GR_ROOTR 9                                       /* the right one's */
/* the corbels at strip column X (1 ..), row y: how much of the texel is the lit face / the underside (0 .. 256; their
   edges are drawn anti-aliased) */
static void gr_corbel(int side, int X, int y, int *af, int *au)
{
    int y16 = y * 16 + 8, top, bot, ub, a, b;
    *af = *au = 0;
    if (!side) {
        top = 760 - X * 23 / 16; bot = 1349 + X * 67 / 16; ub = bot + 56;     /* 47.5 + 0.09 x, 84.3 - 0.26 x, x = -X + 0.5 */
        if (X > GR_ROOTL) return;
    } else {
        top = 55 * 16 + 8; ub = 105 * 16 + 8; bot = 96 * 16 + 8 + X * 16;
        if (X > GR_ROOTR) return;
        if (bot > ub) bot = ub;
    }
    a = clampi(y16 - top + 8, 0, 16); b = clampi(bot - y16 + 8, 0, 16); *af = (a < b ? a : b) * 16;
    a = clampi(y16 - bot + 8, 0, 16); b = clampi(ub - y16 + 8, 0, 16); *au = (a < b ? a : b) * 16;
}
/* the left corbel's face in the strip.  In the picture its first 5 columns are darker than the rest (mean luminance
   128 against 165, a sharp upright step at column 5 on every row: the part next to the wall is in shade), so the 9
   columns to the root are that shaded part going on: the face's own texels 14 columns in (columns 5-13, the nearest
   that are not the picture's edge columns again), their rows scaled to the face's height there (upright streaks: a
   row more or less does not show), at the shaded columns' tone (gr_lsh: per channel, shaded over lit, x 256) */
static int gr_lsh[3];
static void gr_lshade(void)
{
    GrSum a = { 0 }, b = { 0 };
    for (int y = 52; y < 80; y++) for (int x = 0; x < 14; x++) if (!pic_ov[y][x] && lum(gr_raw[y][x]) >= 96) gr_add(x < 5 ? &a : &b, gr_raw[y][x]);
    if (a.n < 40 || b.n < 80) { gr_lsh[0] = gr_lsh[1] = gr_lsh[2] = 256; return; }
    gr_lsh[0] = clampi((a.r * 256 / a.n) / (b.r / b.n + 1), 160, 256);
    gr_lsh[1] = clampi((a.g * 256 / a.n) / (b.g / b.n + 1), 160, 256);
    gr_lsh[2] = clampi((a.b * 256 / a.n) / (b.b / b.n + 1), 160, 256);
}
static Rgb gr_lface(int X, int y)
{
    int sx = 14 - X, td = 760 - X * 23 / 16, bd = 1349 + X * 67 / 16, ts = 760 + (sx * 16 + 8) * 9 / 100 + 24, bs = 1349 - (sx * 16 + 8) * 26 / 100 - 24;
    int ys = ts + (clampi(y * 16 + 8, td, bd) - td) * (bs - ts) / (bd - td);
    return gr_gain(gr_raw[clampi(ys >> 4, 0, PH - 1)][sx], gr_lsh);
}
/* the right corbel's lit face: sandstone in soft upright bands.  The face has a few clean 16 x 15 pieces (found at
   run time: windows of lit sandstone, apart from one another, of both faces).  The strip's face is laid from them
   14 x 13 apart (2 texels of overlap, cross-faded), each as its texels less its mean, on the tone of the face next
   to the edge (columns 3-14 from it); a piece and its neighbours are never the same one, and every other use is the
   piece turned half round (a rotation), so twice as many different pieces come before one repeats */
#define GR_NF 8
static short gr_fp[GR_NF][2], (*gr_fd)[15][16][3]; static int gr_nf; static Rgb gr_fb[2];
static int gr_fpok(int x0, int y0)                       /* a clean piece: lit sandstone (a few darker grains may be in it) */
{
    int bad = 0;
    for (int j = 0; j < 15; j++) for (int i = 0; i < 16; i++) {
        Rgb c;
        if (x0 + i < 0 || x0 + i >= PW || pic_ov[y0 + j][x0 + i] || !gr_cbox(x0 + i, y0 + j)) return 0;
        c = gr_raw[y0 + j][x0 + i];
        if (lum(c) < 96 || c.r < c.b + 32) return 0;
        if (!gr_sandc(x0 + i, y0 + j) && ++bad > 24) return 0;
    }
    return 1;
}
static void gr_faces(void)
{
    gr_nf = 0;
    for (int pass = 0; pass < 2; pass++)                 /* the pieces: by turns from the right face and the left one,
                                                            none within 11 columns and 10 rows of another */
        for (int t = 0; t < 2 * 24 && gr_nf < GR_NF; t++) {
            int right = !(t & 1), found = 0;
            for (int y0 = 44; y0 <= 100 && !found; y0 += 2)
                for (int x0 = right ? 256 : 0; x0 <= (right ? PW - 16 : 48) && !found; x0 += 2) {
                    int far = 1;
                    for (int k = 0; k < gr_nf; k++) if (abs(gr_fp[k][0] - x0) < (pass ? 7 : 11) && abs(gr_fp[k][1] - y0) < (pass ? 6 : 10)) far = 0;
                    if (far && gr_fpok(x0, y0)) { gr_fp[gr_nf][0] = (short)x0; gr_fp[gr_nf][1] = (short)y0; gr_nf++; found = 1; }
                }
        }
    for (int k = 0; k < gr_nf; k++) {
        int m[3] = { 0, 0, 0 }, x0 = gr_fp[k][0], y0 = gr_fp[k][1];
        for (int j = 0; j < 15; j++) for (int i = 0; i < 16; i++) { m[0] += gr_raw[y0 + j][x0 + i].r; m[1] += gr_raw[y0 + j][x0 + i].g; m[2] += gr_raw[y0 + j][x0 + i].b; }
        for (int j = 0; j < 15; j++) for (int i = 0; i < 16; i++) {
            Rgb c = gr_raw[y0 + j][x0 + i];
            gr_fd[k][j][i][0] = (short)(c.r - m[0] / 240); gr_fd[k][j][i][1] = (short)(c.g - m[1] / 240); gr_fd[k][j][i][2] = (short)(c.b - m[2] / 240);
        }
    }
#ifdef WS_BACKDROP_MAIN
    if (getenv("WSB_INFO")) { fprintf(stderr, "gears: %d face pieces:", gr_nf); for (int k = 0; k < gr_nf; k++) fprintf(stderr, " (%d, %d)", gr_fp[k][0], gr_fp[k][1]); fprintf(stderr, "\n"); }
#endif
    for (int s = 0; s < 2; s++) {
        int m[3] = { 0, 0, 0 }, n = 0;
        for (int y = 44; y < 100; y++) for (int i = 3; i < 15; i++) {
            int x = s ? PW - 1 - i : i;
            if (gr_sandc(x, y)) { m[0] += gr_raw[y][x].r; m[1] += gr_raw[y][x].g; m[2] += gr_raw[y][x].b; n++; }
        }
        gr_fb[s].r = n ? m[0] / n : 200; gr_fb[s].g = n ? m[1] / n : 150; gr_fb[s].b = n ? m[2] / n : 100;
    }
}
static void gr_face1(int cx, int cy, int i, int j, int w, int *d)
{
    int v, k;
    if (!gr_nf) return;
    v = fmodi(cx * 2 + cy * 5 + (cy >> 1), 2 * gr_nf); k = v >> 1;
    if (v & 1) { i = 15 - i; j = 14 - j; }
    for (int c = 0; c < 3; c++) d[c] += gr_fd[k][j][i][c] * w;
}
static void gr_facecol(int cx, int i, int y, int w, int *d)     /* (each column of pieces starts at its own row, so
                                                                   no line runs along the face) */
{
    int yy = y + fmodi(cx * 5, 13), cy = fdiv(yy, 13), j = fmodi(yy, 13), wy = j < 2 ? j + 1 : 3;
    gr_face1(cx, cy, i, j, w * wy, d);
    if (wy < 3) gr_face1(cx, cy - 1, i, j + 13, w * (3 - wy), d);
}
static Rgb gr_face(int side, int x, int y)
{
    int cx = fdiv(x, 14), i = fmodi(x, 14), d[3] = { 0, 0, 0 }, wx = i < 2 ? i + 1 : 3; Rgb o;   /* (thirds: this piece's share in the overlap) */
    gr_facecol(cx, i, y, wx, d);
    if (wx < 3) gr_facecol(cx - 1, i + 14, y, 3 - wx, d);
    o.r = clampi(gr_fb[side].r + d[0] / 9, 0, 255); o.g = clampi(gr_fb[side].g + d[1] / 9, 0, 255); o.b = clampi(gr_fb[side].b + d[2] / 9, 0, 255);
    return o;
}
/* what a texel is made of: 1 a wheel's lit face, 2 brick, 3 sandstone (corbel or floor), 0 anything else */
static int gr_kind(Rgb k)
{
    if (gr_blue(k)) return lum(k) >= 60;
    if (k.r >= 136 && k.g >= 96 && k.r - k.b >= 40) return 3;
    return k.r - k.b >= 24 && k.r - k.g >= 8 ? 2 : 0;
}
static int gr_foot16(int side, int X) { return side ? 187 * 16 + 13 + X * 16 / 5 : 187 * 16 + 10 + X * 16 / 5; }

static void gen_gr(int pad)
{
    if (!ws_need_gen_gr()) return;
    pal_make();
    gr_light(); gr_tones(); gr_courses(); gr_faces(); gr_lshade();
    for (int side = 0; side < 2; side++) {
        Rgb us[16];
        for (int d = 0; d < 16; d++) {                   /* the right corbel's underside by depth */
            int r = 0, g = 0, b = 0, n = 0;
            for (int x = PW - 10; x < PW; x++) {
                int y = 95 - (PW - 1 - x) + 1 + d; Rgb k;
                if (y >= PH || pic_ov[y][x]) continue;
                k = rgb15(pic[y][x]); r += k.r; g += k.g; b += k.b; n++;
            }
            us[d].r = n ? r / n : 0; us[d].g = n ? g / n : 0; us[d].b = n ? b / n : 0;
        }
        cur_side = side; gr_edge(side); gr_ftone(side);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                int x = side ? PW - 1 + X : -X, af, au, got; Rgb c = { 0, 0, 0 }, w = { 0, 0, 0 };
                gr_corbel(side, X, y, &af, &au);
                got = clampi(gr_foot16(side, X) - y * 16, 0, 16);       /* (the wall's foot: anti-aliased) */
                if (got < 16) c = gr_floor(x, y);
                if (got) {
                    int l = gr_wlight(side, X, y); Rgb k = gr_side(side, X, y);
                    k.r = clampi(k.r * l / (128 * 16), 0, 255); k.g = clampi(k.g * l / (128 * 16), 0, 255); k.b = clampi(k.b * l / (128 * 16), 0, 255);
                    c = got >= 16 ? k : mix(c, k, got * 16);
                }
                /* (the wall next to a corbel's root: in its shade) */
                if (!side && X > GR_ROOTL && X <= GR_ROOTL + 4 && y >= 46 && y < 90) c = scale(c, 176 + (X - GR_ROOTL) * 16);
                if (side && X > GR_ROOTR && X <= GR_ROOTR + 4 && y >= 54 && y < 107) c = scale(c, 176 + (X - GR_ROOTR) * 16);
                if (au && side) {                        /* the right underside: the picture's tone at that depth under
                                                            the lower edge (its last 10 columns), a low noise over it */
                    int d = clampi(y - (96 + X), 0, 15);
                    if (us[d].r + us[d].g + us[d].b) c = mix(c, scale(us[d], 232 + noise(x, y, 5, 4, 91) * 48 / 255), au);
                } else if (au) {                         /* the left one: the picture's, slid along the lower edge */
                    int sx = x + 12, sy = y - 3;
                    if (sx >= 0 && sx < PW && sy >= 0 && sy < PH && !pic_ov[sy][sx]) c = mix(c, rgb15(pic[sy][sx]), au);
                }
                if (af) c = mix(c, side ? gr_face(side, x, y) : gr_lface(X, y), af);
                got = gr_cover(side, x, y, &w);          /* the wheels */
                if (got) c = mix(c, w, got * 16);
                if (X <= 3) {                            /* the join: the first three columns lean on the picture's edge
                                                            texel of their row where both are the same material */
                    Rgb e = gr_raw[y][side ? PW - 1 : 0]; int k = gr_kind(e);
                    c.r = clampi(c.r, 0, 255); c.g = clampi(c.g, 0, 255); c.b = clampi(c.b, 0, 255);
                    if (k && k == gr_kind(c)) c = mix(e, c, X == 1 ? 110 : X == 2 ? 180 : 230);
                }
                strip[(size_t)y * 2 * pad + (side ? pad + (X - 1) : pad - X)] = quant(c);
            }
    }
}
