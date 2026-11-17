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
 * The extensions, every texel the picture's own (bilinear, then the nearest colour of the picture's CLUT), nothing
 * mirrored; each end from its own texels:
 *   - a point of the end wall, its foot's shadow and the floor is the picture's point k periods further along the
 *     wall at the same height (im_shift: a translation in 1 / u, rows scaled about the horizon), k the smallest that
 *     lands in the picture: holes, cornice, rail, balusters, baseboard and the floor's lines go on at their period,
 *     phase and perspective, all along the wall;
 *   - the parapet's top line stays at its crest's height (level along the wall: a line through the vanishing point),
 *     the gold under it stretched from the source column's own top line; over it the dark ceiling;
 *   - the window is closed: one more column of pane, then its near member (the mullion's gold face), 5 columns out
 *     (u2 = 650);
 *   - then the same window again and again, abutting (IM_BQ, the bay: from the corner to that near member's end,
 *     4.62 periods of the wall): the dark casing (one post between two windows), the gold member, the pane, the
 *     mullion and the pane from the picture's columns 50 to 0, then the same closure, in the wall's perspective (a
 *     translation in 1 / u by whole bays), the texels as they are: the light along the wall is the picture's.
 *     The 16:9 strip ends in the second window's mullion.
 * Integers only; a texel depends on its place, not on the strip's width (a wider strip starts with the same
 * columns).
 *
 * Standalone (WS_BACKDROP_MAIN): PORT_WS_BACKDROP_DUMP=FILE writes the CLUT and the picture during a run; then
 * `wsb 17:2 FILE PAD OUT.ppm` writes the picture with both extensions. */
#define IM_H 1226                                         /* the ROOMS row's mark (no picture has this height) */
#define IM_W 640
static void gen_im(int pad) { (void)pad; }              /* (the table's gen: im_take makes the strips) */
static u16 im_src[PH][IM_W];                             /* the picture */
static u16 im_clut[256];
static u16 *im_ext;                                      /* the two extensions: [y][side * pad + X - 1], X = 1.. outwards */

#define IM_YH 41344                                       /* the horizon: row 162.0 as a texel coordinate (x 256) */
#define IM_TQ(u2) ((long long)((1LL << 36) / (u2)))      /* a column's depth along the side wall: 2^35 / u, u = 320 - x */
#define IM_PQ 4872211LL                                   /* the side wall's period (holes, balusters) */
#define IM_T0 IM_TQ(640)                                  /* the picture's edge */
#define IM_TC IM_TQ(536)                                  /* the room's corner (column 52) */
#define IM_TW IM_TQ(650)                                  /* the window's near end: the next one's casing starts */
#define IM_BQ (IM_TC - IM_TW)                             /* a window bay: the windows abut */
#define IM_HG (104 * 256)                                 /* wall heights (x 256): over the holes */
#define IM_HCB 20070                                      /* the cornice's lower edge (78.4) */
#define IM_HRT 3328                                       /* the rail's upper edge (13) */
#define IM_HFL (-6349)                                    /* the wall's foot (-24.8) */
static Rgb *im_cv; static int im_cw;                     /* the canvas: [side][y][X - 1] */
static int im_es[24], im_E;                              /* the parapet's top edge per column (texel coordinate x 256); its crest */
static u16 im_qc[32768]; static u8 im_qh[32768];
static int im_fl8(int v) { return v >= 0 ? v >> 8 : -((-v + 255) >> 8); }
static Rgb im_tx(int side, int xn, int y)                /* a texel: column xn from the end inwards; xn < 0: the canvas */
{
    y = clampi(y, 0, PH - 1);
    if (xn < 0) return im_cv[((size_t)side * PH + y) * im_cw + clampi(-xn, 1, im_cw) - 1];
    xn = clampi(xn, 0, IM_W - 1);
    return rgb15(im_src[y][side ? IM_W - 1 - xn : xn]);
}
static Rgb im_bil(int side, int xs, int ys)
{
    int x0 = im_fl8(xs), y0 = im_fl8(ys), fx = xs - x0 * 256, fy = ys - y0 * 256;
    Rgb a = im_tx(side, x0, y0), b = im_tx(side, x0 + 1, y0), c = im_tx(side, x0, y0 + 1), d = im_tx(side, x0 + 1, y0 + 1), o;
    o.r = ((a.r * (256 - fx) + b.r * fx) * (256 - fy) + (c.r * (256 - fx) + d.r * fx) * fy + 32768) >> 16;
    o.g = ((a.g * (256 - fx) + b.g * fx) * (256 - fy) + (c.g * (256 - fx) + d.g * fx) * fy + 32768) >> 16;
    o.b = ((a.b * (256 - fx) + b.b * fx) * (256 - fy) + (c.b * (256 - fx) + d.b * fx) * fy + 32768) >> 16;
    return o;
}
static u16 im_quant(Rgb c)
{
    int r = clampi(c.r + 4, 0, 255) >> 3, g = clampi(c.g + 4, 0, 255) >> 3, b = clampi(c.b + 4, 0, 255) >> 3, k = r | g << 5 | b << 10;
    if (!im_qh[k]) {
        int bd = 1 << 30, best = k;
        for (int i = 0; i < 256; i++) {
            int q = im_clut[i] & 0x7FFF, dr, dg, db, d;
            if (!q) continue;
            dr = r5((u16)q) - r; dg = g5((u16)q) - g; db = b5((u16)q) - b; d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (d < bd) { bd = d; best = q; }
        }
        im_qc[k] = (u16)best; im_qh[k] = 1;
    }
    return im_qc[k];
}
static void im_measure(int side)
{
    for (int x = 0; x < 24; x++) {
        int y = 1;
        while (y < 100 && lum(im_tx(side, x, y)) < 96) y++;
        im_es[x] = y * 256 - 128 - clampi(lum(im_tx(side, x, y - 1)) * 256 / lum(im_tx(side, x, y)), 0, 256);
    }
    im_E = (int)((long long)(IM_YH - im_es[0]) * 536 / 639);
}
/* the source of the wall's point at depth t + s, same height, of the pixel (u2, y): texel coordinates x 256 */
static void im_shift(int u2, int y, long long s, int *xs, int *ys)
{
    int us = (int)((1LL << 43) / (IM_TQ(u2) + s));        /* u x 256 */
    *xs = 320 * 256 - 128 - us;
    *ys = IM_YH + (int)((long long)(2 * y - 323) * us / u2);
}
static void im_gen_side(int side, int pad)
{
    im_measure(side);
    for (int X = 1; X <= pad; X++) {
        int u2 = 639 + 2 * X;
        long long t = IM_TQ(u2);
        for (int y = 0; y < PH; y++) {
            int h = (323 - 2 * y) * 68608 / u2, xs, ys;
            long long s;
            if (h >= IM_HRT && h < IM_HCB) {             /* the windows: the picture's one, closed; then it again */
                long long k = t < IM_TW ? (IM_TW - t - 1) / IM_BQ + 1 : 0, tw = t + k * IM_BQ;
                s = k * IM_BQ;
                if (tw >= IM_T0) ;                       /* the picture's part of it */
                else if (tw >= IM_TQ(642)) s += IM_TQ(634) - IM_TQ(642);            /* one more column of pane */
                else s += IM_TQ(591) - IM_TQ(642);                                  /* the near member: a gold face */
                im_shift(u2, y, s, &xs, &ys);
                if (xs > 50 * 256) xs = 50 * 256;        /* (its casing ends at the corner: not the back wall's plate) */
            } else {                                     /* the wall's period: parapet, cornice, rail, balusters, floor */
                s = ((IM_T0 - t) / IM_PQ + 1) * IM_PQ;
                im_shift(u2, y, s, &xs, &ys);
                if (h > IM_HG) {                         /* up to the crest, level from the edge on */
                    int us = 320 * 256 - 128 - xs, x0 = clampi(im_fl8(xs), 0, 22), fx = clampi(xs - x0 * 256, 0, 256);
                    int es = (im_es[x0] * (256 - fx) + im_es[x0 + 1] * fx) >> 8;
                    int yg = IM_YH - (int)((long long)IM_HG * us / 68608);
                    ys = yg - (int)((long long)(h - IM_HG) * (yg - es) / (im_E - IM_HG));
                    if (ys < 0) ys = 0;
                }
            }
            im_cv[((size_t)side * PH + y) * im_cw + X - 1] = im_bil(side, xs, ys);
        }
    }
}
static int im_gen(int pad)
{
    im_cw = pad; im_cv = calloc((size_t)2 * PH * pad, sizeof *im_cv);
    if (!im_cv) return 0;
    memset(im_qh, 0, sizeof im_qh);
    for (int side = 0; side < 2; side++) {
        im_gen_side(side, pad);
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++)
                im_ext[(size_t)y * 2 * pad + side * pad + X - 1] = im_quant(im_cv[((size_t)side * PH + y) * im_cw + X - 1]);
    }
    free(im_cv); im_cv = 0;
    return 1;
}
#ifdef WS_BACKDROP_MAIN
static int im_main(int argc, char **argv)                /* wsb 17:2 PIC.raw PAD OUT.ppm: the picture with both ends */
{
    FILE *f = argc >= 4 ? fopen(argv[1], "rb") : 0; int pad;
    if (!f) { fprintf(stderr, "usage: wsb 17:2 PIC.raw PAD OUT.ppm\n"); return 1; }
    if (fread(im_clut, 1, sizeof im_clut, f) != sizeof im_clut || fread(im_src, 1, sizeof im_src, f) != sizeof im_src) return 1;
    fclose(f);
    pad = atoi(argv[2]); im_ext = calloc((size_t)PH * 2 * pad, 2);
    if (!im_ext || !im_gen(pad) || !(f = fopen(argv[3], "wb"))) return 1;
    fprintf(f, "P6 %d %d 255\n", IM_W + 2 * pad, PH);
    for (int y = 0; y < PH; y++)
        for (int x = -pad; x < IM_W + pad; x++) {
            u16 c = x < 0 ? im_ext[(size_t)y * 2 * pad - x - 1] : x >= IM_W ? im_ext[(size_t)y * 2 * pad + pad + x - IM_W] : im_src[y][x];
            u8 rgb[3] = { (u8)(r5(c) << 3), (u8)(g5(c) << 3), (u8)(b5(c) << 3) }; fwrite(rgb, 1, 3, f);
        }
    fclose(f);
    { u32 h = 2166136261u; for (size_t i = 0; i < (size_t)2 * pad * PH; i++) h = (h ^ im_ext[i]) * 16777619u; printf("strip hash %08x\n", h); }
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
    for (int y = 0; y < PH; y++) for (int x = 0; x < 512; x++) im_src[y][x] = tex8(vram, 128, x, y, 128, 511);
    for (int i = 0; i < 5; i++) {
        const Piece *p = &IM_P[i];
        for (int y = 0; y < p->h; y++) for (int x = 0; x < p->w; x++) im_src[p->y + y][p->x + x] = tex8(vram, p->tx, p->u + x, p->v + y, 128, 511);
    }
}
static int im_pad = -1; static u32 im_sum; static u16 *im_buf;
static u32 im_check(const u16 *vram)
{
    u32 h = 2166136261u;
    for (int i = 0; i < 256; i++) h = (h ^ vram[(size_t)511 * 1024 + 128 + i]) * 16777619u;
    for (int y = 3; y < 256; y += 8) for (int x = 128; x < 384; x++) h = (h ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    for (int y = 155; y < 248; y += 8) for (int x = 512; x < 576; x++) h = (h ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return h;
}
/* the picture's left sprite (page (128, 0), 256 x 224) at frame (x, 16): the frame's strips */
static const u16 *im_take(const u16 *vram, int pad, int x, int y, int w, int h, int u, int v, int tx, int ty, int *rows)
{
    u32 s;
    if (y != 16 || w != 256 || h != PH || u || v || ty || tx != 128) return 0;
    s = im_check(vram);
    if (!im_ext || im_pad != pad || s != im_sum) {
        im_pad = pad; im_sum = s;
        im_grab_vram(vram);
        free(im_ext); free(im_buf); im_ext = calloc((size_t)PH * 2 * pad, 2); im_buf = calloc((size_t)PH * 2 * pad, 2);
        if (!im_ext || !im_buf) { free(im_ext); free(im_buf); im_ext = im_buf = 0; return 0; }
        if (!im_gen(pad)) { free(im_ext); im_ext = 0; return 0; }
        {   const char *e = getenv("PORT_WS_BACKDROP_DUMP");      /* research: the CLUT, then the picture (raw 15-bit LE, IM_W columns) */
            FILE *f = e ? fopen(e, "wb") : 0;
            if (f) { fwrite(im_clut, 1, sizeof im_clut, f); fwrite(im_src, 1, sizeof im_src, f); fclose(f); }
        }
    }
    if (!im_ext) return 0;
    if (x <= -pad && x + IM_W >= 320 + pad) return 0;    /* the picture covers the wide frame */
    *rows = PH;
    for (int j = 0; j < PH; j++) {
        u16 *row = im_buf + (size_t)j * 2 * pad; const u16 *ext = im_ext + (size_t)j * 2 * pad;
        for (int c = 0; c < pad; c++) {
            int e = c - pad - x, f = 320 + c - x;      /* the picture's column at the left / right strip's column c */
            row[c] = e < 0 ? (-e <= pad ? ext[-e - 1] : 0) : e < IM_W ? im_src[j][e] : 0;
            row[pad + c] = f >= IM_W ? (f - IM_W < pad ? ext[pad + f - IM_W] : 0) : f >= 0 ? im_src[j][f] : 0;
        }
    }
    return im_buf;
}
#endif
