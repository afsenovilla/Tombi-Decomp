/* runtime/ws_backdrop.c: AREA17 s2, the Iron Castle main room (included there; docs/PORT.md "More rooms with
 * picture strips").
 *
 * The room is one 640 x 224 8 bpp picture, CLUT (128, 511), moved by the camera (range 320): two sprites of 256
 * columns from pages (128, 0) and (256, 0) and five pieces of 128 columns (IM_P) at frame row 16; its left edge is at
 * frame column 0 at the room's left end, its right edge at column 320 at the right end.  Nothing is culled there
 * (PORT_WSXTRA=300 draws the same frame): the picture ends, so the wide frame showed a black strip at each end.
 * im_take() fills the frame's strips from the 640 columns and the two extensions (made once), wherever the camera is.
 *
 * Measured on the picture: a box room in one-point perspective, vanishing point (320, 162).  Its last 52 columns at
 * each end are the room's end wall coming towards the viewer from the corner (column 52): a column x of that wall is
 * at depth 1 / u, u = 320 - x, and its rows are scaled by u / 268 from the horizon (the cornice's lower edge: row
 * 68.5 at the picture's edge, 82.5 at column 50; the rail and the balusters' white part stay on rows 148 / 162).
 * On that wall, from the top (heights in rows at the corner's scale, from the horizon): the dark ceiling; the gold
 * parapet, whose top line rises from the corner to its crest (121 at the edge, where it is level) and whose holes
 * (height 99) come every 0.0001418 of 1 / u (columns 36, 23.8, 11); the pink cornice (78 to 91); a window (13 to 78:
 * dark frame, the far member's gold face on columns 38-43, the mullion on 20-24, the near pane on 0-11, cut by the
 * edge); the rail (5.5 to 13); the balusters (same period and phase as the holes: columns 46.5, 35.5, 23.5, 10.5);
 * the baseboard (down to -24.8), its shadow and the floor, whose shading and faint plank lines follow the lines
 * through the vanishing point.  No pier shows past the window, and nothing of a second one: the wall's own window is
 * all the picture gives of it.  (The back wall's bay next to the corner is closed by a riveted pilaster, columns
 * 53-71: the first version put one of those after the window, and the user's verdict was "El pilar remachado no
 * está bien, continúa con ventanas.")
 *
 * More measured for the third version: the cornice's face has ONE joint on that wall (3.31 periods from the corner,
 * column 13, a rivet on each side of it on the right end), not one per hole; the window's members lean, their tops
 * towards the corner, 0.0046 periods per row of height (pane and mullion edges: 0.16 periods, 2 columns, over the
 * window), and are seen from the near side: after the corner a brown face (to 0.76 periods), that member's lit side
 * (to 1.3), a pane, the mullion's face (2.3 to 2.8) and its dark side (to 3.3), the second pane, cut by the
 * picture's edge (4.28: as wide there as the first at mid height, and no member in the last column); the rail
 * (heights 5.5 to 13) is red-brown near the corner and grey-brown from 2.5 periods on, at one lightness.
 *
 * The extensions, every texel the picture's own (bilinear, then the nearest colour of the picture's CLUT), nothing
 * mirrored; each end from its own texels:
 *   - a point of the parapet, the rail, the balusters, the baseboard, its shadow and the floor is the picture's point
 *     k periods further along the wall at the same height (im_shift: a translation in 1 / u, rows scaled about the
 *     horizon), k the smallest that lands in the picture: holes, balusters and the floor's lines go on at their
 *     period, phase and perspective, all along the wall.  The parapet's last quarter of each period crosses over to
 *     the next one's texels (no seam per period);
 *   - the parapet's top line stays at its crest's height (level along the wall: a line through the vanishing point),
 *     the gold under it stretched from the source column's own top line; over it the dark ceiling;
 *   - the cornice's face (heights 78.4 to 91) is one band with no joint (im_cornice): the picture's two clean
 *     stretches of it, before the joint (2.2 to 3.08 periods) and after it (3.55 to the edge), one after the other,
 *     crossed over 0.1 periods, the far one's light levelled to the block at the picture's edge (a line fitted to
 *     the face's light along the wall);
 *   - the window (im_window) goes on as one glazed band, a mullion every 2.28 periods (IM_WB): the picture's own
 *     mullion (its face with the bolt at its head, its dark side, its curled foot) and the second pane after it,
 *     moved along the wall by whole bays (a translation in 1 / u: every member's lean, width and edges are the
 *     picture's own, at the wall's perspective).  Where those texels end (the picture's edge) the pane goes on (the
 *     picture's last column) and closes on the next mullion with the first pane's own end, 0.26 periods of it (its
 *     rounded corners, the bolt's half over the pane), in the second pane's light (im_pg).  The bay is the shortest
 *     the picture allows: its last column (4.28 periods from the corner) is pane on every row and holds no bolt,
 *     and the mullion's bolt starts 2.02 to 2.04 periods from the corner, 0.3 before its face's edge (IM_WM), so
 *     the next mullion is at least 2.24 to 2.26 periods after the picture's.  (The
 *     third version closed the window there with the far member's brown face as a post between two windows; the
 *     user's verdict on it: it must be a member like the mullion, and fit.)  The 16:9 strip holds one mullion, one
 *     pane and the next mullion's face;
 *   - the rail keeps its texels (the wall's period) and goes back to its colour near the corner over 1.5 periods
 *     (per channel: the mean of the columns 32-47 over the mean of the columns 1-12, 5 % lighter than the edge).
 * Integers only; a texel depends on its place, not on the strip's width (a wider strip starts with the same
 * columns).
 *
 * Standalone (WS_BACKDROP_MAIN): PORT_WS_BACKDROP_DUMP=FILE writes the CLUT and the picture during a run; then
 * `wsb 17:2 FILE PAD OUT.ppm` writes the picture with both extensions. */
#define IM_H 1226 /* the ROOMS row's mark (no picture has this height) */
#define IM_W 640
static void gen_im(int pad) { (void)pad; } /* (the table's gen: im_take makes the strips) */
static u16 (*im_src)[IM_W];                /* the picture */
static u16 im_clut[256];
/* the two extensions: [y][side * pad + X - 1], X = 1.. outwards */
static u16 *im_ext;

#define IM_YH 41344                                 /* the horizon: row 162.0 as a texel coordinate (x 256) */
#define IM_TQ(u2) ((long long)((1LL << 36) / (u2))) /* a column's depth along the side wall: 2^35 / u, u = 320 - x */
#define IM_PQ 4872211LL                             /* the side wall's period (holes, balusters) */
#define IM_T0 IM_TQ(640)                            /* the picture's edge */
#define IM_TC IM_TQ(536)                            /* the room's corner (column 52) */
#define IM_PE (IM_TC - IM_T0)                       /* the picture's edge, from the corner (4.276 periods) */
#define IM_WM 11457491LL            /* the mullion's far edge (its face / the first pane) at height 45: 2.35 periods */
#define IM_WS 22412                 /* its lean, as every member's: 0.0046 periods per row of height */
#define IM_WB 11108641LL            /* from a mullion to the next: 2.28 periods */
#define IM_WQ1 (IM_TC - IM_TQ(639)) /* the window's texels end on the picture's last column */
#define IM_WR 1266775LL             /* the pane's end before a mullion: the first pane's, 0.26 periods ... */
#define IM_WF 389777LL              /* ... crossed over 0.08 with the pane going on */
#define IM_CB (IM_PQ * 22 / 10)     /* the cornice's texels: from 2.2 periods after the corner ... */
#define IM_CLB (IM_PQ * 78 / 100)   /* ... 0.78 periods; then from 3.65 on (its one joint is at 3.31, */
#define IM_CLA (IM_PQ * 52 / 100)   /*     a rivet on each side of it), 0.52 periods */
#define IM_CW (IM_PQ / 10)          /* the two stretches cross over 0.1 periods */
#define IM_HG (104 * 256)           /* wall heights (x 256): over the holes */
#define IM_HCB 20070                /* the cornice's lower edge (78.4) */
#define IM_HCT (91 * 256)           /* its upper edge */
#define IM_HRB 1408                 /* the rail's lower edge (5.5) */
#define IM_HRT 3328                 /* the rail's upper edge (13) */
#define IM_HFL (-6349)              /* the wall's foot (-24.8) */
/* the canvas: [side][y][X - 1] */
static Rgb *im_cv;
static int im_cw;
/* the parapet's top edge per column (texel coordinate x 256); its crest */
static int im_es[24], im_E;
/* the cornice's light per column (x 256): a + b x; at the picture's edge */
static int im_ca, im_cb, im_cr;
/* the rail: its colour near the corner over the edge's (x 256) */
static int im_rg[3];
/* the panes: the second at the picture's edge over the first's end (x 256) */
static int im_pg[3];
static u16 (*im_qc);
static u8 (*im_qh);
static int im_fl8(int v) { return v >= 0 ? v >> 8 : -((-v + 255) >> 8); }
/* a texel: column xn from the end inwards; xn < 0: the canvas */
static Rgb im_tx(int side, int xn, int y)
{
    y = clampi(y, 0, PH - 1);
    if (xn < 0) return im_cv[((size_t)side * PH + y) * im_cw + clampi(-xn, 1, im_cw) - 1];
    xn = clampi(xn, 0, IM_W - 1);
    return rgb15(im_src[y][side ? IM_W - 1 - xn : xn]);
}
static Rgb im_bil(int side, int xs, int ys)
{
    int x0 = im_fl8(xs),
        y0 = im_fl8(ys),
        fx = xs - x0 * 256,
        fy = ys - y0 * 256;
    Rgb a = im_tx(side, x0, y0),
        b = im_tx(side, x0 + 1, y0),
        c = im_tx(side, x0, y0 + 1),
        d = im_tx(side, x0 + 1, y0 + 1),
        o;
    o.r = ((a.r * (256 - fx) + b.r * fx) * (256 - fy) + (c.r * (256 - fx) + d.r * fx) * fy + 32768) >> 16;
    o.g = ((a.g * (256 - fx) + b.g * fx) * (256 - fy) + (c.g * (256 - fx) + d.g * fx) * fy + 32768) >> 16;
    o.b = ((a.b * (256 - fx) + b.b * fx) * (256 - fy) + (c.b * (256 - fx) + d.b * fx) * fy + 32768) >> 16;
    return o;
}
/* the nearest colour of the picture's CLUT (remembered per 15-bit colour in im_qc / im_qh) */
static u16 im_quant(Rgb c)
{
    int r = clampi(c.r + 4, 0, 255) >> 3,
        g = clampi(c.g + 4, 0, 255) >> 3,
        b = clampi(c.b + 4, 0, 255) >> 3,
        k = r | g << 5 | b << 10;
    if (!im_qh[k]) {
        int best_d = 1 << 30,
            best = k;
        for (int i = 0; i < 256; i++) {
            int q = im_clut[i] & 0x7FFF,
                dr, dg, db, d;
            if (!q) continue;
            dr = r5((u16)q) - r;
            dg = g5((u16)q) - g;
            db = b5((u16)q) - b;
            d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (d < best_d) {
                best_d = d;
                best = q;
            }
        }
        im_qc[k] = (u16)best;
        im_qh[k] = 1;
    }
    return im_qc[k];
}
/* the mean colour of the wall's band between two heights on the picture's column x (x 256) */
static void im_band(int side, int x, int h0, int h1, int *o)
{
    int y0 = (86564 - h1 * (639 - 2 * x) + 535) / 536,
        y1 = (86564 - h0 * (639 - 2 * x)) / 536,
        n = 0;
    o[0] = o[1] = o[2] = 0;
    for (int y = y0; y <= y1; y++, n++) {
        Rgb c = im_tx(side, x, y);
        o[0] += c.r;
        o[1] += c.g;
        o[2] += c.b;
    }
    for (int i = 0; i < 3; i++) o[i] = o[i] * 256 / (n ? n : 1);
}
/* one end of the picture measured: the parapet's top edge (im_es, im_E), the cornice's light (im_ca, im_cb, im_cr),
   the rail's and the panes' colour ratios (im_rg, im_pg) */
static void im_measure(int side)
{
    long long sx = 0,
              sy = 0,
              sxx = 0,
              sxy = 0;
    int near_c[3] = { 0, 0, 0 },
        edge_c[3] = { 0, 0, 0 },
        band[3], near_l, edge_l;
    for (int x = 0; x < 24; x++) {
        int y = 1;
        while (y < 100 && lum(im_tx(side, x, y)) < 96) y++;
        im_es[x] = y * 256 - 128 - clampi(lum(im_tx(side, x, y - 1)) * 256 / lum(im_tx(side, x, y)), 0, 256);
    }
    im_E = (int)((long long)(IM_YH - im_es[0]) * 536 / 639);
    /* the cornice's face (heights 81 to 88): its light along the wall, a line fitted over the columns 15-29 (the joint
       is on 13-14), and its level on the block at the picture's edge */
    for (int x = 15; x <= 29; x++) {
        int luma;
        im_band(side, x, 81, 88, band);
        luma = (band[0] * 77 + band[1] * 150 + band[2] * 29) >> 8;
        sx += x;
        sy += luma;
        sxx += x * x;
        sxy += (long long)x * luma;
    }
    im_cb = (int)((15 * sxy - sx * sy) / (15 * sxx - sx * sx));
    im_ca = (int)((sy - im_cb * sx) / 15);
    im_cr = 0;
    for (int x = 1; x <= 10; x++) {
        im_band(side, x, 81, 88, band);
        im_cr += (band[0] * 77 + band[1] * 150 + band[2] * 29) >> 8;
    }
    im_cr /= 10;
    /* the rail (heights 7 to 12): its colour near the corner (columns 32-47) and at the picture's edge (1-12) */
    for (int x = 1; x <= 47; x++) {
        if (x > 12 && x < 32) continue;
        im_band(side, x, 7, 12, band);
        for (int i = 0; i < 3; i++) {
            if (x <= 12)
                edge_c[i] += band[i] >> 4;
            else
                near_c[i] += band[i] >> 4;
        }
    }
    /* (16 columns, 12; 5 % lighter) */
    near_l = (near_c[0] * 77 + near_c[1] * 150 + near_c[2] * 29) >> 8;
    edge_l = ((edge_c[0] * 77 + edge_c[1] * 150 + edge_c[2] * 29) >> 8) * 7 / 5;
    for (int i = 0; i < 3; i++)
        im_rg[i] = clampi((int)((long long)near_c[i] * 3 / 4 * edge_l * 256 /
                                ((long long)(edge_c[i] ? edge_c[i] : 1) * (near_l ? near_l : 1))),
                          128, 512);
    /* the panes (heights 30 to 60): the second on the picture's last columns (0-2) over the first next to the
       mullion (27-30) */
    for (int i = 0; i < 3; i++) near_c[i] = edge_c[i] = 0;
    for (int x = 0; x <= 30; x++) {
        if (x > 2 && x < 27) continue;
        im_band(side, x, 30, 60, band);
        for (int i = 0; i < 3; i++) {
            if (x <= 2)
                edge_c[i] += band[i] >> 4;
            else
                near_c[i] += band[i] >> 4;
        }
    }
    for (int i = 0; i < 3; i++)
        im_pg[i] = clampi(edge_c[i] * 4 * 256 / (near_c[i] * 3 > 0 ? near_c[i] * 3 : 1), 256, 448);
}
/* the source of the wall's point at depth t + s, same height, of the pixel (u2, y): texel coordinates x 256 */
static void im_shift(int u2, int y, long long s, int *xs, int *ys)
{
    int u256 = (int)((1LL << 43) / (IM_TQ(u2) + s)); /* u x 256 */
    *xs = 320 * 256 - 128 - u256;
    *ys = IM_YH + (int)((long long)(2 * y - 323) * u256 / u2);
}
static Rgb im_mix(Rgb a, Rgb b, int w) /* a to b, w of 256 */
{
    Rgb o;
    o.r = (a.r * (256 - w) + b.r * w + 128) >> 8;
    o.g = (a.g * (256 - w) + b.g * w + 128) >> 8;
    o.b = (a.b * (256 - w) + b.b * w + 128) >> 8;
    return o;
}
static int im_smooth(int x)
{
    x = clampi(x, 0, 256);
    return x * x * (768 - 2 * x) >> 16;
}
/* the wall's point s further along it, same height: parapet (up to its crest), rail, balusters, baseboard, floor */
static Rgb im_wall(int side, int u2, int y, int h, long long s)
{
    int xs, ys;
    im_shift(u2, y, s, &xs, &ys);
    if (h > IM_HG) { /* up to the crest, level from the edge on */
        int us = 320 * 256 - 128 - xs,
            x0 = clampi(im_fl8(xs), 0, 22),
            fx = clampi(xs - x0 * 256, 0, 256);
        int edge_y = (im_es[x0] * (256 - fx) + im_es[x0 + 1] * fx) >> 8;
        int hole_y = IM_YH - (int)((long long)IM_HG * us / 68608);
        ys = hole_y - (int)((long long)(h - IM_HG) * (hole_y - edge_y) / (im_E - IM_HG));
        if (ys < 0) ys = 0;
    }
    return im_bil(side, xs, ys);
}
/* a window's point q from the corner: the picture's; past its edge the pane goes on (its last column) */
static Rgb im_winq(int side, int u2, int y, long long q)
{
    int xs, ys;
    if (q > IM_WQ1) q = IM_WQ1;
    im_shift(u2, y, IM_TC - q - IM_TQ(u2), &xs, &ys);
    return im_bil(side, xs, ys);
}
/* the window goes on, a mullion every IM_WB: the picture's own mullion (its face with the bolt, its dark side, its
   foot) and the second pane after it, moved along the wall by whole bays, their lean kept; where those texels end
   (the picture's edge) the pane goes on to the next mullion's edge, and closes on it with the first pane's end (its
   rounded corners, the bolt's near half), in the second pane's light */
static Rgb im_window(int side, int u2, int y, int h, long long t)
{
    /* (the lean taken out) */
    long long from_corner = IM_TC - t,
              unleant = from_corner + (long long)IM_WS * (h - 45 * 256) / 256;
    /* whole bays; up to the next mullion */
    long long bays = (unleant - IM_WM) / IM_WB,
              to_next = IM_WM + (bays + 1) * IM_WB - unleant;
    Rgb c = im_winq(side, u2, y, from_corner - bays * IM_WB);
    if (from_corner - bays * IM_WB > IM_WQ1 && to_next < IM_WR) {
        Rgb n = im_winq(side, u2, y, from_corner - (bays + 1) * IM_WB);
        int wt = clampi((int)(to_next * 256 / (IM_PQ / 10)), 0, 256) * clampi(h - 21 * 256, 0, 768) / 768 *
                 clampi(67 * 256 - h, 0, 768) / 768;
        n.r = clampi(n.r * (65536 + (im_pg[0] - 256) * wt) >> 16, 0, 255);
        n.g = clampi(n.g * (65536 + (im_pg[1] - 256) * wt) >> 16, 0, 255);
        n.b = clampi(n.b * (65536 + (im_pg[2] - 256) * wt) >> 16, 0, 255);
        c = im_mix(c, n, im_smooth((int)((IM_WR - to_next) * 256 / IM_WF)));
    }
    return c;
}
/* the cornice's face: one band, no joint.  The picture's two clean stretches of it (before its joint and after),
   one after the other again and again, crossed over; the far one levelled to the block at the picture's edge */
static Rgb im_cornq(int side, int u2, int y, long long q)
{
    int xs, ys;
    Rgb c;
    im_shift(u2, y, IM_TC - q - IM_TQ(u2), &xs, &ys);
    c = im_bil(side, xs, ys);
    if (xs > 13 * 256) {
        int gain = im_ca + (int)((long long)im_cb * xs / 256);
        gain = clampi(im_cr * 256 / (gain > 0 ? gain : 1), 208, 400);
        c.r = clampi(c.r * gain >> 8, 0, 255);
        c.g = clampi(c.g * gain >> 8, 0, 255);
        c.b = clampi(c.b * gain >> 8, 0, 255);
    }
    return c;
}
static Rgb im_cornice(int side, int u2, int y, long long t)
{
    long long phase = (IM_T0 - t) % (IM_CLB + IM_CLA + 2 * IM_CW);
    if (phase < IM_CLB) return im_cornq(side, u2, y, IM_CB + phase);
    if (phase < IM_CLB + IM_CW)
        return im_mix(im_cornq(side, u2, y, IM_CB + phase), im_cornq(side, u2, y, IM_PQ * 355 / 100 + phase - IM_CLB),
                      im_smooth((int)((phase - IM_CLB) * 256 / IM_CW)));
    if (phase < IM_CLB + IM_CW + IM_CLA) return im_cornq(side, u2, y, IM_PQ * 355 / 100 + phase - IM_CLB);
    return im_mix(im_cornq(side, u2, y, IM_PQ * 355 / 100 + phase - IM_CLB),
                  im_cornq(side, u2, y, IM_CB + phase - (IM_CLB + IM_CLA + 2 * IM_CW)),
                  im_smooth((int)((phase - (IM_CLB + IM_CW + IM_CLA)) * 256 / IM_CW)));
}
/* one extension, column by column: each texel by its height on the side wall (the window, the cornice, or the
   wall's period) */
static void im_gen_side(int side, int pad)
{
    im_measure(side);
    for (int X = 1; X <= pad; X++) {
        int u2 = 639 + 2 * X;
        long long depth = IM_TQ(u2);
        for (int y = 0; y < PH; y++) {
            int height = (323 - 2 * y) * 68608 / u2;
            Rgb c;
            if (height >= IM_HRT && height < IM_HCB)
                c = im_window(side, u2, y, height, depth);
            else if (height >= IM_HCB && height < IM_HCT)
                c = im_cornice(side, u2, y, depth);
            else { /* the wall's period: parapet, rail, balusters, floor */
                long long period = (IM_T0 - depth) / IM_PQ + 1,
                          into = (IM_T0 - depth) % IM_PQ;
                c = im_wall(side, u2, y, height, period * IM_PQ);
                /* the parapet: no seam per period */
                if (height >= IM_HCT && into > IM_PQ - IM_PQ / 4)
                    c = im_mix(c, im_wall(side, u2, y, height, (period + 1) * IM_PQ),
                               im_smooth((int)((into - (IM_PQ - IM_PQ / 4)) * 256 / (IM_PQ / 4))));
                else if (height >= IM_HRB && height < IM_HRT) { /* the rail: back to its colour near the corner */
                    int wt = im_smooth((int)((IM_T0 - depth) * 256 / (IM_PQ * 3 / 2))) * clampi(height - IM_HRB, 0, 256)
                             >> 8;
                    c.r = clampi((int)((long long)c.r * (65536 + (im_rg[0] - 256) * wt) >> 16), 0, 255);
                    c.g = clampi((int)((long long)c.g * (65536 + (im_rg[1] - 256) * wt) >> 16), 0, 255);
                    c.b = clampi((int)((long long)c.b * (65536 + (im_rg[2] - 256) * wt) >> 16), 0, 255);
                }
            }
            im_cv[((size_t)side * PH + y) * im_cw + X - 1] = c;
        }
    }
}
static int im_gen(int pad)
{
    if (!ws_need_im_gen()) return 0;
    im_cw = pad;
    im_cv = calloc((size_t)2 * PH * pad, sizeof *im_cv);
    if (!im_cv) return 0;
    memset(im_qh, 0, (sizeof *im_qh * (32768)));
    for (int side = 0; side < 2; side++) {
        im_gen_side(side, pad);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                im_ext[(size_t)y * 2 * pad + side * pad + X - 1] =
                    im_quant(im_cv[((size_t)side * PH + y) * im_cw + X - 1]);
    }
    free(im_cv);
    im_cv = 0;
    return 1;
}
#ifdef WS_BACKDROP_MAIN
static int im_main(int argc, char **argv) /* wsb 17:2 PIC.raw PAD OUT.ppm: the picture with both ends */
{
    if (!ws_need_im_main()) return 0;
    FILE *f = argc >= 4 ? fopen(argv[1], "rb") : 0;
    int pad;
    if (!f) {
        fprintf(stderr, "usage: wsb 17:2 PIC.raw PAD OUT.ppm\n");
        return 1;
    }
    if (fread(im_clut, 1, sizeof im_clut, f) != sizeof im_clut ||
        fread(im_src, 1, (sizeof *im_src * (PH)), f) != (sizeof *im_src * (PH)))
        return 1;
    fclose(f);
    pad = atoi(argv[2]);
    im_ext = calloc((size_t)PH * 2 * pad, 2);
    if (!im_ext || !im_gen(pad) || !(f = fopen(argv[3], "wb"))) return 1;
    fprintf(f, "P6 %d %d 255\n", IM_W + 2 * pad, PH);
    for (int y = 0; y < PH; y++)
        for (int x = -pad; x < IM_W + pad; x++) {
            u16 c = x < 0       ? im_ext[(size_t)y * 2 * pad - x - 1]
                    : x >= IM_W ? im_ext[(size_t)y * 2 * pad + pad + x - IM_W]
                                : im_src[y][x];
            u8 rgb[3] = { (u8)(r5(c) << 3), (u8)(g5(c) << 3), (u8)(b5(c) << 3) };
            fwrite(rgb, 1, 3, f);
        }
    fclose(f);
    {
        u32 h = 2166136261u;
        for (size_t i = 0; i < (size_t)2 * pad * PH; i++) h = (h ^ im_ext[i]) * 16777619u;
        printf("strip hash %08x\n", h);
    }
    return 0;
}
#endif
#ifndef WS_BACKDROP_MAIN
static const Piece IM_P[] = {                            /* the picture's columns 512-639: five pieces of 128 columns */
    { 128, 0, 224, 128, 32, 512, 0 }, { 128, 128, 224, 128, 32, 512, 32 }, { 256, 0, 224, 128, 32, 512, 64 },
    { 256, 128, 224, 128, 32, 512, 96 }, { 512, 0, 152, 128, 96, 512, 128 },
};
static void im_grab_vram(const u16 *vram)
{
    for (int i = 0; i < 256; i++) im_clut[i] = vram[(size_t)511 * 1024 + 128 + i];
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < 512; x++) im_src[y][x] = tex8(vram, 128, x, y, 128, 511);
    for (int i = 0; i < 5; i++) {
        const Piece *p = &IM_P[i];
        for (int y = 0; y < p->h; y++)
            for (int x = 0; x < p->w; x++) im_src[p->y + y][p->x + x] = tex8(vram, p->tx, p->u + x, p->v + y, 128, 511);
    }
}
static int im_pad = -1;
static u32 im_sum;
static u16 *im_buf;
static u32 im_check(const u16 *vram)
{
    u32 fnv = 2166136261u;
    for (int i = 0; i < 256; i++) fnv = (fnv ^ vram[(size_t)511 * 1024 + 128 + i]) * 16777619u;
    for (int y = 3; y < 256; y += 8)
        for (int x = 128; x < 384; x++) fnv = (fnv ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    for (int y = 155; y < 248; y += 8)
        for (int x = 512; x < 576; x++) fnv = (fnv ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return fnv;
}
/* the picture's left sprite (page (128, 0), 256 x 224) at frame (x, 16): the frame's strips */
static const u16 *im_take(const u16 *vram, int pad, int x, int y, int w, int h, int u, int v, int tx, int ty, int *rows)
{
    if (!ws_need_im_take()) return 0;
    u32 check;
    if (y != 16 || w != 256 || h != PH || u || v || ty || tx != 128) return 0;
    check = im_check(vram);
    if (!im_ext || im_pad != pad || check != im_sum) {
        im_pad = pad;
        im_sum = check;
        im_grab_vram(vram);
        free(im_ext);
        free(im_buf);
        im_ext = calloc((size_t)PH * 2 * pad, 2);
        im_buf = calloc((size_t)PH * 2 * pad, 2);
        if (!im_ext || !im_buf) {
            free(im_ext);
            free(im_buf);
            im_ext = im_buf = 0;
            return 0;
        }
        {
            WsCPart entry = { im_ext, (size_t)PH * 2 * pad * 2 }; /* (made once: ws_cache) */
            u32 key = wsc_hash(wsc_hash(check, im_clut, sizeof im_clut), im_src, sizeof *im_src * PH);
            if (!wsc_get("im", 17, 2, pad, key, 0, &entry, 1)) {
                if (!im_gen(pad)) {
                    free(im_ext);
                    im_ext = 0;
                    return 0;
                }
                wsc_put("im", 17, 2, pad, key, 0, &entry, 1);
            }
        }
        /* research: the CLUT, then the picture (raw 15-bit LE, IM_W columns) */
        {
            const char *e = getenv("PORT_WS_BACKDROP_DUMP");
            FILE *f = e ? fopen(e, "wb") : 0;
            if (f) {
                fwrite(im_clut, 1, sizeof im_clut, f);
                fwrite(im_src, 1, (sizeof *im_src * (PH)), f);
                fclose(f);
            }
        }
    }
    if (!im_ext) return 0;
    if (x <= -pad && x + IM_W >= 320 + pad) return 0; /* the picture covers the wide frame */
    *rows = PH;
    for (int j = 0; j < PH; j++) {
        u16 *row = im_buf + (size_t)j * 2 * pad;
        const u16 *ext = im_ext + (size_t)j * 2 * pad;
        for (int c = 0; c < pad; c++) {
            /* the picture's column at the left / right strip's column c */
            int e = c - pad - x,
                f = 320 + c - x;
            row[c] = e < 0 ? (-e <= pad ? ext[-e - 1] : 0) : e < IM_W ? im_src[j][e] : 0;
            row[pad + c] = f >= IM_W ? (f - IM_W < pad ? ext[pad + f - IM_W] : 0) : f >= 0 ? im_src[j][f] : 0;
        }
    }
    return im_buf;
}
#endif
