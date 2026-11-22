/* runtime/ws_backdrop.c: AREA06 s2 and s1, the Mermaid's Singing Rock (included there; docs/PORT.md "More rooms with
 * picture strips").
 *
 * s2, the mermaid on her rock: a fixed 320 x 224 picture, CLUT (128, 511): 256 columns from page (128, 0) and 64 from
 * page (256, 0).  Over it the game draws quads of the same pages: six cloud pieces (one of them runs 64 columns out of
 * the picture's left edge: the game closes that cloud itself) and the sea, a loop of four pictures of the waves
 * (MM2_Q: phase 4 is the picture itself; 15 to 35 vblanks each).  The strips are made once per phase, every one from
 * the same source texels of its own phase, so the sea in them moves with the picture's; the phase is read from the
 * wave objects (mm2_phase).
 *
 * s1, the beach: a 640 x 224 picture, CLUT (128, 510), three sprites from pages (576, 0), (704, 0) and (832, 0) at
 * frame row 16, moved by the camera: its left edge is at frame column -2 at the room's left end, its right edge at
 * column 320 at the right end.  Only the two ends need content: mm_take() fills the frame's strips from the 640
 * columns and the two extensions (made once), wherever the camera is.
 *
 * Measured on the pictures:
 *   s2: the horizon is the dark line on row 134, level over the whole width; at the left edge the sea ends on the sand
 *       on row 185 (the shore is level there and falls to the right: row 186 at column 20, 190 at 70, 200 at 120, the
 *       same in all four phases), the sand goes down to the rock mound in the corner, whose top line runs from row
 *       204 at column 40 to row 215 at the edge (11 rows in 40 columns) with a lit rim of 3 rows over it; at the right
 *       edge the cloud is closed (it ends at column 316), the small rock's tail is cut on rows 171-175 (its ridge falls
 *       6 rows in 8 columns, its waterline is row 176 with two rows of foam) and the flat rock in the corner under
 *       rows 211-213 (a wavy level top; its other end, in the picture, comes down 5 rows in 20 columns and then
 *       steeply).  The sky is a gradient of flat tones (no grain) whose streaks are arcs round (145, -62) at the left
 *       edge (a circle through (0, 33), (50, 83), (167, 110)), about level at the right one.  The wave quads end at
 *       column 317: in phases 1-3 the picture's columns 318-319 still show phase 4.  A quad of w pixels spans w - 1
 *       texels (the software renderer rounds half up, OpenGL truncates): mm_grab samples the quads the same way, and
 *       the picture it makes is the frame's, texel for texel, in every phase.
 *   s1: sky to row 151, the sea to row 179, the sand below, all level at the left end; the right end is the arch's
 *       pillar from row 0 to its foot: strata rising to the right (the long lit ridges at 0.58 rows per column at the
 *       top, 0.52 at row 64, 0.42 at the lip; the short dark strokes between them are steeper, 0.8) down to the lip,
 *       the bulge's lower edge: the neck's first row is 163, level over the last 56 columns, and the rock's last rows
 *       over it lie along the lip, not along the strata; a smooth dark neck; the foot from its lit rim (row 178 at
 *       the edge, 180-184 further in: found per column) to its underside (the sand's first row, 205 at the edge,
 *       0.09 rows per column higher further in: found per column).
 *
 * The strips, every texel the picture's own (plus a tone), nothing mirrored:
 *   - a band (sky, sea, sand) is its tone + its detail (mb_band).  The tone is the band's mean at the edge per row,
 *     carried outwards level and eased into the row's mean further out (the sea's: over the columns the windows come
 *     from, the open sea, so the wash round a rock the edge cuts does not run on).  The detail is the band's texels
 *     less the mean round them, quilted (mq_quilt): the edge's own columns first, then windows of the same rows cut
 *     along minimum-error seams, the same map for the four phases.  The sky's detail is its texels less the row's
 *     mean over the whole width, smoothed over 5 x 5 (the picture's sky has no grain: this melts the seams, and the
 *     CLUT gives it the picture's flat tones back); s2's left sky is quilted with the arcs laid level, so the streaks
 *     the edge cuts go on round their centre;
 *   - s2's cut rocks are closed with their own texels, quilted in blocks that move as one piece (so their outlines
 *     and grain are the picture's own texels'): the mound's flank on down its top line until it leaves the frame, the
 *     flat rock level and then its end, shaped as its other end; the small rock's tail is its last 8 columns again,
 *     6 rows down its ridge, to the waterline, its foam fading out;
 *   - s1's pillar is closed with its own rock in four layers (mp_gen), each quilted in its own space and put back
 *     along the pillar's lines in blocks that move as one piece: the strata (rows sheared along the ridges), the
 *     rock's last rows over the lip (level), the neck (the edge's tone per row counted from the lip to the rim, the
 *     grain quilted) and the foot (rows counted from the rim to the sand).  At the join the picture's own columns go
 *     on first (per row those that follow the place, 14 to 38 columns in, where its last 3 columns are found again)
 *     and the first blocks are cut against them at least 4 columns out, so no seam lies on the join's line;
 *     the join's tone (the picture's last 6 columns against the strip's first 6, smoothed) fades out over 22
 *     columns, and where the edge cuts a smooth face the strip starts as smooth and gets its grain over 12.  The far
 *     outline is a smooth curve (MP_OL: the picture has no source for its shape) drawn with sub-texel coverage
 *     against the sky, sea and sand behind; towards it the rock turns away (more of it per column) and falls into
 *     shade, as the arch's other leg; behind it the sky, sea and sand are the picture's columns 8 on (its far end,
 *     never in the same frame), the sand under the foot quilted from the arch's own.
 * Every colour is then the nearest of the picture's CLUT.  Integers only; a texel depends on its place, not on the
 * strip's width (the quilts go outwards block column by block column). */
#define MM_H2 1224                                        /* the ROOMS rows' marks (no picture has these heights) */
#define MM_H1 1225
#define MM_W 640
#define MM_NPH 4
#define MB_NMAP 6                                         /* the bands' maps (mb_band) */
static void gen_mm(int pad) { (void)pad; }              /* (the table's gen: mm_take makes the strips) */
static u16 (*mm_src)[PH][MM_W];                     /* s2: the picture with each phase's quads; s1: [0] */
static int mm_w, mm_ph;                                  /* the picture's columns; the phase being made */
static u16 mm_clut[256]; static u16 (*mm_qc); static u8 (*mm_qh);
static Rgb *mm_cv; static int mm_cw;                     /* the canvas: [side][y][X - 1] */

static Rgb mm_px(int x, int y) { return rgb15(mm_src[mm_ph][clampi(y, 0, PH - 1)][clampi(x, 0, mm_w - 1)]); }
static u16 mm_quant(Rgb c)
{
    int r = clampi(c.r + 4, 0, 255) >> 3, g = clampi(c.g + 4, 0, 255) >> 3, b = clampi(c.b + 4, 0, 255) >> 3, k = r | g << 5 | b << 10;
    if (!mm_qh[k]) {
        int bd = 1 << 30, best = k;
        for (int i = 0; i < 256; i++) {
            int q = mm_clut[i] & 0x7FFF, dr, dg, db, d;
            if (!q) continue;
            dr = r5((u16)q) - r; dg = g5((u16)q) - g; db = b5((u16)q) - b; d = 3 * dr * dr + 4 * dg * dg + 2 * db * db;
            if (d < bd) { bd = d; best = q; }
        }
        mm_qc[k] = (u16)best; mm_qh[k] = 1;
    }
    return mm_qc[k];
}
#define MQ_SW 320                                         /* a quilt's source: at most this many columns */
static int *mb_map[MB_NMAP], mb_tw;
static int mm_canvas(int pad)                            /* the canvas and the bands' maps for strips of pad columns */
{
    if (pad > MQ_SW - 56) return 0;                       /* (mb_band's rows) */
    mm_cw = pad; mb_tw = pad + 48;
    mm_cv = calloc((size_t)2 * PH * pad, sizeof *mm_cv); mb_map[0] = malloc((size_t)MB_NMAP * PH * mb_tw * sizeof(int));
    if (!mm_cv || !mb_map[0]) { free(mm_cv); free(mb_map[0]); mm_cv = 0; mb_map[0] = 0; return 0; }
    for (int i = 1; i < MB_NMAP; i++) mb_map[i] = mb_map[0] + (size_t)i * PH * mb_tw;
    return 1;
}
static void mm_canvas_free(void) { free(mm_cv); free(mb_map[0]); mm_cv = 0; mb_map[0] = 0; }
static Rgb *mm_at(int side, int X, int y) { return &mm_cv[((size_t)side * PH + y) * mm_cw + X - 1]; }
static void mm_put(int side, int X, int y, Rgb c) { if (X >= 1 && X <= mm_cw && y >= 0 && y < PH) *mm_at(side, X, y) = c; }
static int mm_d2(Rgb a, Rgb b) { int r = a.r - b.r, g = a.g - b.g, bb = a.b - b.b; return 3 * r * r + 4 * g * g + 2 * bb * bb; }

/* --- bands */
enum { MC_ANY, MC_SEA, MC_SAND, MC_SKY };
static int mm_is(int cls, Rgb c)
{
    return cls == MC_SEA ? c.b > c.r : cls == MC_SAND ? c.r > c.b + 24 && c.r >= 208 : cls == MC_SKY ? c.b >= c.r + 16 && c.b >= c.g + 16 : 1;
}
/* the mean (x 16) of the class' texels in the box (x +- R, y +- r), rows within [y0, y1); the texels counted */
static int mm_lp(int cls, int x, int y, int R, int r, int y0, int y1, int *o)
{
    int s[3] = { 0, 0, 0 }, n = 0, xa = x - R < 0 ? 0 : x - R, xb = x + R >= mm_w ? mm_w - 1 : x + R, ya = y - r < y0 ? y0 : y - r,
        yb = y + r >= y1 ? y1 - 1 : y + r;
    for (int v = ya; v <= yb; v++)
        for (int u = xa; u <= xb; u++) {
            Rgb c = rgb15(mm_src[mm_ph][v][u]);
            if (cls != MC_ANY && !mm_is(cls, c)) continue;
            s[0] += c.r; s[1] += c.g; s[2] += c.b; n++;
        }
    if (n) for (int c = 0; c < 3; c++) o[c] = s[c] * 16 / n;
    return n;
}
#define MB_JW 16                                          /* columns over which the join's tone fades out */
#define MM_FAR 40                                         /* columns over which the edge's own rows fade out */

/* --- a quilter in a layer's own space (mq_quilt)
 * The source mq_s (mq_sw columns: the picture's columns next to the edge in the layer's space, in the picture's own
 * left to right order; mq_k: 1 a texel that may be copied, 2 one the join is matched against) is quilted into a map
 * (mq_tw columns, X = 1.. outwards; a texel's source, row * MQ_SW + column, -1: none): blocks of BW x BH from the join
 * outwards, each column top to bottom, so a wider strip starts with the same blocks.  A block is the window (of
 * copyable texels only) whose OV columns and OT rows of overlap differ least from what is there (the picture at the
 * join); windows more than dy rows from where they go, and windows on top of the neighbours' (which would grow into
 * a verbatim copy), cost more; among the ones within 1/8 of the best, one by a position hash (at the join: the best).
 * Pasted along the minimum-error cuts through both overlaps.  Never mirrored.  mq_bmap gets each texel's block and
 * mq_bx / mq_by / mq_bp each block's place and window: a layer whose space is not the picture's puts a block's texels
 * back as one piece of the picture (a column never slides against the next one). */
#define MQ_MB 48                                          /* a block's most columns / rows, the overlaps included */
static Rgb (*mq_s)[MQ_SW], (*mq_l)[MQ_SW]; static u8 (*mq_k)[MQ_SW]; static int mq_sw, mq_tw, mq_nx;   /* (mq_l: mq_s smoothed,
                                                             5 x 3; mq_nx: the columns wanted) */
static int (*mq_bad)[MQ_SW + 1];
#define MQ_NB 1024
static short *mq_bmap, (*mq_bx), (*mq_by); static int (*mq_bp);   /* a texel's block; a block's window: where its first
                                                             column and row go (the column counted as the source's:
                                                             mq_sw is X = 1 on the right, -X on the left) and where
                                                             it is from */
typedef struct { short y0, y1, BW, BH, OV, OT, dy, step, wl, pre, strict; } MqBand;   /* step: every step-th window is tried (the
                                                             join's blocks: every one); wl: the smoothed texels'
                                                             weight in the match (the texels': 1); pre: OV of the
                                                             picture's last columns go on first (see mq_quilt) and the
                                                             first blocks are cut against them (the cut wanders; pre is
                                                             a column's cost in it: the sooner the cheaper);
                                                             strict: no window more than dy rows off while there is one */
static Rgb mq_tx(const int *map, int X, int y) { int m = map[y * mq_tw + X - 1]; return mq_s[m / MQ_SW][m % MQ_SW]; }
static int mq_get(int side, const int *map, int wx, int y, Rgb *c)
{
    if (y < 0 || y >= PH) return 0;
    if (wx >= 0 && wx < mq_sw) { if (!(mq_k[y][wx] & 2)) return 0; *c = mq_s[y][wx]; return 1; }
    {   int X = side ? wx - mq_sw + 1 : -wx;
        if (X < 1 || X > mq_tw || map[y * mq_tw + X - 1] < 0) return 0;
        *c = mq_tx(map, X, y); return 1;
    }
}
static int mq_getl(int side, const int *map, int wx, int y, Rgb *c)
{
    if (wx >= 0 && wx < mq_sw) *c = mq_l[y][wx];
    else { int X = side ? wx - mq_sw + 1 : -wx, m = map[y * mq_tw + X - 1]; *c = mq_l[m / MQ_SW][m % MQ_SW]; }
    return 1;
}
/* the cheapest path through e: n steps of m places each (vert: a step is a row, a place a column) */
static int (*mq_cut__E)[MQ_MB];
static void mq_cut(int e[MQ_MB][MQ_MB], int n, int m, int vert, int *path)
{
    int (*const E)[MQ_MB] = mq_cut__E;
    int p = 0;
    for (int i = 0; i < n; i++)
        for (int q = 0; q < m; q++) {
            int c = vert ? e[i][q] : e[q][i], best = i ? E[i - 1][q] : 0;
            if (i && q > 0 && E[i - 1][q - 1] < best) best = E[i - 1][q - 1];
            if (i && q + 1 < m && E[i - 1][q + 1] < best) best = E[i - 1][q + 1];
            E[i][q] = c + best;
        }
    for (int q = 1; q < m; q++) if (E[n - 1][q] < E[n - 1][p]) p = q;
    for (int i = n - 1; i >= 0; i--) {
        path[i] = p;
        if (i) { int bp = p; if (p > 0 && E[i - 1][p - 1] < E[i - 1][bp]) bp = p - 1; if (p + 1 < m && E[i - 1][p + 1] < E[i - 1][bp]) bp = p + 1; p = bp; }
    }
}
/* the first cut's cost j columns from the join: never in the first MQ_PRE0 (the cut is not the join's line), then the
   sooner the cheaper */
#define MQ_PL 24                                          /* the pre columns: from up to OV + MQ_PL columns in */
#define MQ_PJ 40000                                       /* a step of their place between two rows costs this */
#define MQ_PRE0 4
static int mq_prec(int pre, int j) { return j < MQ_PRE0 ? 40000 * (MQ_PRE0 - j) : pre * (j - MQ_PRE0); }
static int (*mq_quilt__e)[MQ_MB];
static int (*mq_quilt__errs);
static int (*mq_quilt__picks);
static int (*mq_quilt__pe)[MQ_PL + 1];
static int (*mq_quilt__ev)[MQ_MB];
typedef struct { short j, y; Rgb c, l; } MqOv;
static MqOv (*mq_quilt__ov);
static void mq_quilt(int side, const MqBand *b, int seed, int *map)
{
    int (*const e)[MQ_MB] = mq_quilt__e, (*const errs) = mq_quilt__errs, (*const picks) = mq_quilt__picks;
    MqOv (*const ov) = mq_quilt__ov;
    int W = b->BW + b->OV, nb = 0, np = 0;
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < mq_sw; x++) mq_bad[y + 1][x + 1] = mq_bad[y][x + 1] + mq_bad[y + 1][x] - mq_bad[y][x] + !(mq_k[y][x] & 1);
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < mq_sw; x++) {
            int t[3] = { 0, 0, 0 }, n = 0;
            for (int v = y - 1; v <= y + 1; v++) for (int u = x - 2; u <= x + 2; u++)
                if (v >= 0 && v < PH && u >= 0 && u < mq_sw && mq_k[v][u]) { t[0] += mq_s[v][u].r; t[1] += mq_s[v][u].g; t[2] += mq_s[v][u].b; n++; }
            mq_l[y][x] = n ? (Rgb){ t[0] / n, t[1] / n, t[2] / n } : mq_s[y][x];
        }
    if (b->pre) {
        /* the picture's columns that go on first: on every row the OV columns that follow the place, OV to OV + MQ_PL
           columns in, where the picture's last 3 columns are found again (so the row goes on as it does there); the
           place changes from a row to the next only where that pays MQ_PJ (few, well placed steps) */
        int (*const pe)[MQ_PL + 1] = mq_quilt__pe; static short lag[PH];
        int l = 0, seg = -1;
        for (int y = b->y0; y < b->y1; y++)
            for (int q = 0; q <= MQ_PL; q++) {
                int c = q * 64, best = 1 << 30;
                for (int j = 0; j < 3; j++) {
                    int a = side ? mq_sw - 1 - j : j, s = side ? mq_sw - (b->OV + q) - 1 - j : b->OV + q + j;
                    if (s < 0 || s >= mq_sw) { c += 1 << 20; continue; }
                    if (!(mq_k[y][a] & 2)) continue;
                    c += mq_k[y][s] & 1 ? mm_d2(mq_s[y][a], mq_s[y][s]) : 60000;
                }
                if (y > b->y0) for (int p = 0; p <= MQ_PL; p++) { int e = pe[y - 1][p] + (p == q ? 0 : MQ_PJ); if (e < best) best = e; }
                pe[y][q] = c + (y > b->y0 ? best : 0);
            }
        for (int q = 1; q <= MQ_PL; q++) if (pe[b->y1 - 1][q] < pe[b->y1 - 1][l]) l = q;
        for (int y = b->y1 - 1; y >= b->y0; y--) {
            lag[y] = (short)(b->OV + l);
            if (y > b->y0) { int bp = l; for (int p = 0; p <= MQ_PL; p++) if (pe[y - 1][p] + (p == l ? 0 : MQ_PJ) < pe[y - 1][bp] + (bp == l ? 0 : MQ_PJ)) bp = p; l = bp; }
        }
        for (int y = b->y0; y < b->y1; y++) {
            if (seg < 0 || lag[y] != lag[y - 1] || y - seg >= b->BH) {   /* (a piece: rows of one place) */
                if (seg >= 0) nb++;
                seg = y;
                if (nb < MQ_NB) { mq_bx[nb] = (short)(side ? mq_sw : -lag[y]); mq_by[nb] = (short)y; mq_bp[nb] = y * MQ_SW + (side ? mq_sw - lag[y] : 0); }
            }
            for (int X = 1; X <= b->OV; X++) {
                int sx = side ? mq_sw - lag[y] + X - 1 : lag[y] - X;
                if (!(mq_k[y][sx] & 1)) continue;
                map[y * mq_tw + X - 1] = y * MQ_SW + sx;
                if (mq_bmap) mq_bmap[y * mq_tw + X - 1] = (short)(nb < MQ_NB ? nb : MQ_NB - 1);
            }
        }
        nb++;
    }
    for (int X0 = b->pre ? 1 + b->OV : 1; X0 <= mq_nx && X0 + b->BW - 1 <= mq_tw; X0 += b->BW) {
        for (int yb = b->y0; yb < b->y1; yb += b->BH, nb++) {
            int ya = yb - b->OT < b->y0 ? b->y0 : yb - b->OT, yz = yb + b->BH > b->y1 ? b->y1 : yb + b->BH, H = yz - ya;
            int wx0 = side ? mq_sw + X0 - 1 - b->OV : -(X0 + b->BW - 1), nov = 0, mn = 1 << 30, nc = 0, pick = -1, k, lo, hi, st = X0 <= 1 + b->OV ? 1 : b->step;   /* (the join's blocks: every window) */
            for (int y = 0; y < H; y++)
                for (int j = 0; j < W; j++) {
                    if (ya + y >= yb && (side ? j >= b->OV : j < b->BW)) continue;
                    if (W * H > 160 && ((j ^ y) & 1)) continue;   /* (a big block: every other texel of its overlap) */
                    if (mq_get(side, map, wx0 + j, ya + y, &ov[nov].c)) { mq_getl(side, map, wx0 + j, ya + y, &ov[nov].l); ov[nov].j = (short)j; ov[nov].y = (short)y; nov++; }
                }
            for (int R = b->strict ? b->dy : b->dy + 6; ; R = PH) {
                lo = ya - R < 0 ? 0 : ya - R; hi = ya + R > PH - H ? PH - H : ya + R;
                for (int sy = lo; sy <= hi; sy++)
                    for (int sx = 0; sx + W <= mq_sw; sx += st) {
                        long long s = 0; int dv = sy > ya ? sy - ya : ya - sy, er, *o = &errs[(sy - lo) * MQ_SW + sx];
                        *o = -1;
                        if (mq_bad[sy + H][sx + W] - mq_bad[sy][sx + W] - mq_bad[sy + H][sx] + mq_bad[sy][sx]) continue;
                        {   long long lim = mn < 1 << 28 ? ((long long)mn + mn / 8 + 1) * nov / 16 + 1 : (long long)1 << 60;
                            for (int i = 0; i < nov && s <= lim; i++) s += mm_d2(ov[i].c, mq_s[sy + ov[i].y][sx + ov[i].j]) + b->wl * mm_d2(ov[i].l, mq_l[sy + ov[i].y][sx + ov[i].j]);
                            if (s > lim) { *o = 1 << 29; continue; }
                        }
                        s = nov ? s * 16 / nov : 0;
                        if (dv > b->dy) s += (dv - b->dy) * (s / 8 + 400);
                        for (int u = 0; u < np; u++) {           /* (windows used before: by how much of this one they hold) */
                            int dx = sx - picks[u] % MQ_SW, dy = sy - picks[u] / MQ_SW;
                            if (dx < 0) dx = -dx;
                            if (dy < 0) dy = -dy;
                            if (dx < W && dy < H) s += (s / 2 + 3000) * (W - dx) * (H - dy) / (W * H);
                        }
                        er = s > (1 << 28) ? 1 << 28 : (int)s;
                        *o = er; if (er < mn) mn = er;
                    }
                if (mn < 1 << 30 || R >= PH) break;
            }
            if (mn == 1 << 30) continue;
            for (int sy = lo; sy <= hi; sy++) for (int sx = 0; sx + W <= mq_sw; sx += st) { int er = errs[(sy - lo) * MQ_SW + sx]; if (er >= 0 && er <= (X0 <= 1 + b->OV ? mn : mn + mn / 8)) nc++; }
            k = X0 <= 1 + b->OV ? 0 : (int)(hash3(X0, yb, seed) % (u32)nc);
            for (int sy = lo; sy <= hi && pick < 0; sy++)
                for (int sx = 0; sx + W <= mq_sw; sx += st) { int er = errs[(sy - lo) * MQ_SW + sx]; if (er >= 0 && er <= (X0 <= 1 + b->OV ? mn : mn + mn / 8) && k-- == 0) { pick = sy * MQ_SW + sx; break; } }
            {   int sx = pick % MQ_SW, sy = pick / MQ_SW, vc[MQ_MB], tc[MQ_MB], j0 = side ? 0 : b->BW, ot = yb - ya;
                int (*const ev)[MQ_MB] = mq_quilt__ev;
                if (np < 512) picks[np++] = pick;
                if (nb < MQ_NB) { mq_bx[nb] = (short)wx0; mq_by[nb] = (short)ya; mq_bp[nb] = pick; }
                for (int y = 0; y < H; y++)
                    for (int j = 0; j < W; j++) {
                        int X = side ? wx0 + j - mq_sw + 1 : -(wx0 + j);
                        e[y][j] = X >= 1 && X <= mq_tw && map[(ya + y) * mq_tw + X - 1] >= 0 ? mm_d2(mq_tx(map, X, ya + y), mq_s[sy + y][sx + j]) : 0;
                    }
                for (int y = 0; y < H; y++) for (int j = 0; j < b->OV; j++) ev[y][j] = e[y][j0 + j] + (b->pre && X0 == 1 + b->OV ? mq_prec(b->pre, side ? j : b->OV - 1 - j) : 0);
                mq_cut(ev, H, b->OV, 1, vc);
                for (int j = 0; j < W; j++) tc[j] = 0;
                if (ot > 0) mq_cut(e, W, ot + 1, 0, tc);
                for (int y = 0; y < H; y++)
                    for (int j = 0; j < W; j++) {
                        int X = side ? wx0 + j - mq_sw + 1 : -(wx0 + j), *m;
                        if (X < 1 || X > mq_tw) continue;
                        m = &map[(ya + y) * mq_tw + X - 1];
                        if (*m >= 0 && ((side ? j < vc[y] : j > b->BW + vc[y]) || y < tc[j])) continue;   /* (the old side of a cut) */
                        *m = (sy + y) * MQ_SW + sx + j;
                        if (mq_bmap) mq_bmap[(ya + y) * mq_tw + X - 1] = (short)(nb < MQ_NB ? nb : MQ_NB - 1);
                    }
            }
        }
    }
}

/* --- a band (sky, sea, sand) continued: its tone + its detail (mb_band)
 * The tone is the band's mean at the edge per row (a box of the band's own texels), carried outwards level: its own
 * row's at the join, the mean of more rows further out (so a foam line the edge cuts fades out).  The detail is the
 * band's texels less the mean round them, quilted (mq_quilt: the edge's own columns first, then windows of the same
 * rows cut along minimum-error seams), so the strip has the picture's texture at the picture's density everywhere.
 * A smooth band (the sky: a gradient of flat tones) has its detail smoothed, which also melts the seams.  The map is
 * made once (the first phase) and used for every phase: the sea in the strips moves with the picture's. */
typedef struct {
    short y0, y1, cls;                                    /* rows; the class of its texels */
    short sx0, sx1, sy0, sy1;                             /* the windows' columns and rows (of the picture) */
    short ye, in;                                         /* the last row the edge shows the band on (0: y1 - 1); the
                                                             edge's column, counted from the picture's */
    short R, r, rf;                                       /* the mean's box (+- columns, rows); the rows far out */
    short BW, BH, OV, OT, dy, wl, step;                   /* the quilt (MqBand) */
    short blur;                                           /* the detail smoothed (+- columns and rows) */
    short gain;                                           /* the detail x gain / 16 */
    short pre;                                            /* the edge's own columns go on first (mq_quilt's pre: the cost) */
    short acx, acy;                                       /* its streaks are arcs round this point (acx 0: level): the
                                                             detail is quilted with the arcs laid level */
    short open;                                           /* far out its tone is the row's over the windows' columns
                                                             (the open sea), not the edge's */
} MbBand;
/* the row at which the arc through (x, y) meets the edge column (x counted from the edge, negative in the strip) */
static int mb_arc(const MbBand *b, int x, int y)
{
    return b->acx ? b->acy + isqrt_i((x - b->acx) * (x - b->acx) + (y - b->acy) * (y - b->acy) - b->acx * b->acx) : y;
}
static int (*mb_ii)[PH + 1][MQ_SW + 1];                   /* the class' texels summed (r, g, b, count) */
static int mb_x0;                                         /* the picture's column of the source's first */
static int mb_box(int x, int y, int R, int r, int y0, int y1, int *o)   /* the mean (x 16) round source texel (x, y) */
{
    int xa = x - R < 0 ? 0 : x - R, xb = x + R >= mq_sw ? mq_sw : x + R + 1, ya = y - r < y0 ? y0 : y - r, yb = y + r >= y1 ? y1 : y + r + 1, n;
    if (ya >= yb) return 0;
    n = mb_ii[3][yb][xb] - mb_ii[3][ya][xb] - mb_ii[3][yb][xa] + mb_ii[3][ya][xa];
    if (n) for (int c = 0; c < 3; c++) o[c] = (mb_ii[c][yb][xb] - mb_ii[c][ya][xb] - mb_ii[c][yb][xa] + mb_ii[c][ya][xa]) * 16 / n;
    return n;
}
static int (*mb_band__tn)[3];
static int (*mb_band__tf)[3];
static int (*mb_band__jc)[3];
static Rgb (*mb_band__dt)[MQ_SW + 8];
static Rgb (*mb_band__sh)[MQ_SW];
static u8 (*mb_band__shk)[MQ_SW];
static void mb_band(int side, const MbBand *b, int slot)
{
    int (*const tn)[3] = mb_band__tn, (*const tf)[3] = mb_band__tf, (*const jc)[3] = mb_band__jc;
    Rgb (*const dt)[MQ_SW + 8] = mb_band__dt, (*const sh)[MQ_SW] = mb_band__sh; u8 (*const shk)[MQ_SW] = mb_band__shk;
    int ye = b->ye ? b->ye : b->y1 - 1, *map = mb_map[slot], ya = b->sy0 < b->y0 ? b->sy0 : b->y0, yz = b->sy1 > b->y1 ? b->sy1 : b->y1;
    MqBand q = { b->y0, b->acx ? PH : b->y1, b->BW, b->BH, b->OV, b->OT, b->dy, b->step, b->wl, b->pre };
    mq_sw = mm_w - b->in < MQ_SW ? mm_w - b->in : MQ_SW; mq_tw = mb_tw; mq_nx = mm_cw + 4; mq_bmap = 0;
    mb_x0 = side ? mm_w - b->in - mq_sw : b->in;
    memset(mq_k, 0, (sizeof *mq_k * (PH)));
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < mq_sw; x++) {
            Rgb c = mm_px(mb_x0 + x, y); int k = y >= ya && y < yz && mm_is(b->cls, c);
            mq_s[y][x] = c;
            mb_ii[0][y + 1][x + 1] = mb_ii[0][y][x + 1] + mb_ii[0][y + 1][x] - mb_ii[0][y][x] + (k ? c.r : 0);
            mb_ii[1][y + 1][x + 1] = mb_ii[1][y][x + 1] + mb_ii[1][y + 1][x] - mb_ii[1][y][x] + (k ? c.g : 0);
            mb_ii[2][y + 1][x + 1] = mb_ii[2][y][x + 1] + mb_ii[2][y + 1][x] - mb_ii[2][y][x] + (k ? c.b : 0);
            mb_ii[3][y + 1][x + 1] = mb_ii[3][y][x + 1] + mb_ii[3][y + 1][x] - mb_ii[3][y][x] + k;
        }
    for (int y = ya; y < yz; y++)
        for (int x = 0; x < mq_sw; x++) {
            int px = mb_x0 + x, e = side ? mq_sw - 1 - x : x, g[3], in = y >= b->y0 && y <= ye;
            Rgb c = mq_s[y][x];
            if (!mm_is(b->cls, c) || !mb_box(x, y, b->R, b->r, in ? b->y0 : b->sy0, in ? ye + 1 : b->sy1, g)) continue;
            mq_s[y][x] = (Rgb){ c.r - (g[0] + 8) / 16, c.g - (g[1] + 8) / 16, c.b - (g[2] + 8) / 16 };
            if (px >= b->sx0 && px < b->sx1 && y >= b->sy0 && y < b->sy1) mq_k[y][x] |= 1;
            if (e < b->OV && in) mq_k[y][x] |= 3;
        }
    for (int y = 0; y < PH; y++)                         /* (the other texels: no detail) */
        for (int x = 0; x < mq_sw; x++) if (!mq_k[y][x]) mq_s[y][x] = (Rgb){ 0, 0, 0 };
    if (b->acx) {                                        /* the arcs laid level: row v is the arc that meets the edge on row v */
        memcpy(sh, mq_s, (sizeof *sh * (PH))); memcpy(shk, mq_k, (sizeof *shk * (PH))); memset(mq_k, 0, (sizeof *mq_k * (PH)));
        for (int v = b->y0; v < PH; v++)
            for (int x = 0; x < mq_sw; x++) {
                int dx = side ? mq_sw - 1 - x : x, k, y;
                /* the arc through the edge's row v: (dx - acx)^2 + (y - acy)^2 = acx^2 + (v - acy)^2 */
                k = b->acx * b->acx + (v - b->acy) * (v - b->acy) - (dx - b->acx) * (dx - b->acx);
                if (k < 0) continue;
                y = b->acy + isqrt_i(k);
                if (y < 0 || y >= PH) continue;
                mq_s[v][x] = sh[y][x]; mq_k[v][x] = shk[y][x];
            }
    }
    if (!mm_ph) { for (int i = 0; i < PH * mb_tw; i++) map[i] = -1; mq_quilt(side, &q, 40 + slot, map); }
    /* the tone at the edge */
    for (int y = b->y0; y < b->y1; y++) {
        int yy = y > ye ? ye : y, ex = side ? mq_sw - 1 : 0;
        if (!mb_box(ex, yy, b->R, b->r, b->y0, ye + 1, tn[y])) {
            if (y > b->y0) memcpy(tn[y], tn[y - 1], sizeof tn[y]); else { Rgb e = mm_px(mb_x0 + ex, yy); tn[y][0] = e.r * 16; tn[y][1] = e.g * 16; tn[y][2] = e.b * 16; }
        }
        {   /* far out: the band's tone on this row where the windows come from (the open sea, not what the edge cuts) */
            int xa = clampi(b->sx0 - mb_x0, 0, mq_sw), xb = clampi(b->sx1 - mb_x0, 0, mq_sw), va = yy - b->rf < b->y0 ? b->y0 : yy - b->rf, vb = yy + b->rf + 1 > ye + 1 ? ye + 1 : yy + b->rf + 1, n = 0;
            if (xa < xb && va < vb) n = mb_ii[3][vb][xb] - mb_ii[3][va][xb] - mb_ii[3][vb][xa] + mb_ii[3][va][xa];
            if (b->open && n >= 60) for (int c = 0; c < 3; c++) tf[y][c] = (mb_ii[c][vb][xb] - mb_ii[c][va][xb] - mb_ii[c][vb][xa] + mb_ii[c][va][xa]) * 16 / n;
            else if (!mb_box(ex, yy, b->R, b->rf, b->y0, ye + 1, tf[y])) memcpy(tf[y], tn[y], sizeof tf[y]);
        }
    }
    /* the detail, columns -3..0 the picture's */
    for (int y = b->y0; y < b->y1; y++)
        for (int X = -3; X <= mm_cw + 4 && X <= mb_tw; X++) {
            Rgb d = { 0, 0, 0 };
            int v = clampi(mb_arc(b, -X, y), b->y0, PH - 1);
            if (X < 1) { int x = side ? mq_sw - 1 + X : -X; if (mq_k[v][x] & 2) d = mq_s[v][x]; else if (y > b->y0) d = dt[y - 1][X + 3]; }
            else if (map[v * mb_tw + X - 1] >= 0) {
                int u = mb_arc(b, -X, y) - (b->y1 - 16), k = 256 - clampi(u * 4, 0, 160);   /* (arcs that meet the edge under the
                                                             band: the picture shows none of them; fainter) */
                d = mq_tx(map, X, v);
                if (b->acx) d = (Rgb){ d.r * k / 256, d.g * k / 256, d.b * k / 256 };
            }
            dt[y][X + 3] = d;
        }
    for (int y = b->y0; y < b->y1; y++) {                /* the join: the picture's last 4 columns against the first 4 made */
        int a[3] = { 0, 0, 0 }, c[3] = { 0, 0, 0 }, n = 0, m = 0;
        for (int w = y - 2; w <= y + 2; w++)
            if (w >= b->y0 && w <= ye)
                for (int i = 0; i < 4; i++) {
                    Rgb p = mm_px(mb_x0 + (side ? mq_sw - 1 - i : i), w);
                    if (mm_is(b->cls, p)) { a[0] += p.r; a[1] += p.g; a[2] += p.b; n++; }
                    c[0] += tn[w][0] + dt[w][4 + i].r * b->gain; c[1] += tn[w][1] + dt[w][4 + i].g * b->gain; c[2] += tn[w][2] + dt[w][4 + i].b * b->gain; m++;
                }
        for (int k = 0; k < 3; k++) jc[y][k] = n >= 6 ? a[k] * 16 / n - c[k] / m : 0;
    }
    for (int y = b->y0; y < b->y1; y++)                  /* (into the detail, fading out over MB_JW columns) */
        for (int X = 1; X <= MB_JW && X <= mb_tw; X++) {
            int jq = X * 256 / MB_JW, jw = 256 - jq * jq / 256 * (768 - 2 * jq) / 256;
            dt[y][X + 3].r += fdiv(jc[y][0] * jw, 256 * b->gain); dt[y][X + 3].g += fdiv(jc[y][1] * jw, 256 * b->gain); dt[y][X + 3].b += fdiv(jc[y][2] * jw, 256 * b->gain);
        }
    for (int y = b->y0; y < b->y1; y++)
        for (int X = 1; X <= mm_cw; X++) {
            int far = X >= MM_FAR ? 256 : X * 256 / MM_FAR, d[3] = { 0, 0, 0 }, n = 0;
            Rgb o;
            for (int v = y - b->blur; v <= y + b->blur; v++)
                for (int u = X - b->blur; u <= X + b->blur; u++) {
                    int vv = clampi(v, b->y0, b->y1 - 1), uu = clampi(u, -3, mb_tw);
                    d[0] += dt[vv][uu + 3].r; d[1] += dt[vv][uu + 3].g; d[2] += dt[vv][uu + 3].b; n++;
                }
            for (int k = 0; k < 3; k++) d[k] = fdiv(d[k] * (b->acx ? b->gain - (b->gain - 10) * (X > 40 ? 40 : X) / 40 : b->gain), n);   /* (arcs: calmer further out) */
            o.r = (tn[y][0] + (tf[y][0] - tn[y][0]) * far / 256 + d[0] + 8) >> 4;
            o.g = (tn[y][1] + (tf[y][1] - tn[y][1]) * far / 256 + d[1] + 8) >> 4;
            o.b = (tn[y][2] + (tf[y][2] - tn[y][2]) * far / 256 + d[2] + 8) >> 4;
            mm_put(side, X, y, o);
        }
}

/* ================================================================== s2: the mermaid on her rock */
#define MM2_HZ 134                                        /* the horizon: the sea's first row */
#define MM2_SH 185                                        /* the sand's first row at the left edge */
static const Piece MM2_Q[MM_NPH][6] = {                   /* the waves' quads per phase (y: picture rows); [0] is phase 4 */
    { { 256, 64, 0, 16, 16, 270, 176 }, { 384, 56, 232, 40, 24, 0, 136 } },
    { { 256, 64, 144, 56, 24, 142, 177 }, { 256, 128, 120, 128, 40, 198, 168 }, { 256, 192, 168, 64, 88, 254, 136 },
      { 384, 0, 160, 184, 80, 0, 136 }, { 384, 184, 160, 32, 32, 184, 136 }, { 384, 184, 200, 72, 32, 184, 192 } },
    { { 256, 64, 112, 56, 24, 142, 176 }, { 256, 128, 80, 128, 40, 198, 168 }, { 256, 128, 168, 64, 88, 254, 136 },
      { 384, 0, 80, 184, 80, 0, 136 }, { 384, 184, 120, 72, 32, 184, 192 }, { 384, 184, 80, 32, 32, 184, 136 } },
    { { 256, 64, 88, 56, 24, 150, 180 }, { 256, 128, 40, 112, 40, 206, 168 }, { 256, 64, 168, 64, 88, 254, 136 },
      { 384, 0, 0, 176, 80, 0, 136 }, { 384, 176, 40, 80, 32, 176, 192 }, { 384, 176, 0, 40, 32, 176, 136 } },
};
#define MM2_ACX 145                                       /* the sky's streaks at the left edge: arcs round this point */
#define MM2_ACY (-62)
static const MbBand MM2_B[5] = {
    { 0, MM2_HZ, MC_SKY, 0, 110, 0, MM2_HZ, 0, 0, 400, 6, 6, 24, 24, 12, 8, 12, 4, 4, 2, 16, 0, MM2_ACX, MM2_ACY },
    { MM2_HZ, MM2_SH, MC_SEA, 8, 150, MM2_HZ, MM2_SH, 0, 0, 20, 1, 8, 16, 7, 8, 3, 1, 1, 2, 0, 16, 30, 0, 0, 1 },
    { MM2_SH, PH, MC_SAND, 2, 120, MM2_SH + 1, 210, 209, 0, 6, 2, 4, 12, 10, 6, 4, 12, 1, 2, 0, 16, 1, 0, 0 },
    { 0, MM2_HZ, MC_SKY, 20, 200, 0, MM2_HZ, 0, 0, 400, 6, 6, 24, 24, 12, 8, 12, 4, 4, 2, 16, 0, 0, 0 },
    { MM2_HZ, PH, MC_SEA, 10, 318, MM2_HZ, 213, 212, 2, 20, 1, 8, 16, 7, 8, 3, 1, 1, 2, 0, 16, 30, 0, 0, 1 },   /* (the wave quads end at column 317) */
};
static int mm2_rock(Rgb c) { return c.r >= c.b; }
/* the cut rocks: closed with their own texels, quilted in blocks that move as one piece (mq_quilt), so their outlines
   are the picture's own texels' */
#define M2_TW 64
#define M2_MW 44                                          /* the mound's flank: the picture's first columns */
static short m2_top[M2_MW], (*m2_mb), (*m2_fb), (*m2_mt)[2], (*m2_ft)[2];
static int m2_ts(int x) { return (215 * 16 - x * 44 / 10 + 8) / 16; }        /* the mound's top line: its row at column x */
static int m2_td(int X) { return (215 * 16 + X * 44 / 10 + X * X / 16 + 8) / 16; }   /* and X columns out of the picture */
static int m2_fe16(int X) { return (X > 24 ? (X - 24) * (X - 24) * 16 / 80 : 0) + (X > 40 ? (X - 40) * (X - 40) * 4 : 0); }   /* the
                                                             flat rock's top, rows (x 16) under its level: its end (as
                                                             its other end in the picture: 5 rows in 20 columns, then down) */
static int (*mm2_gen__map);
static void mm2_gen(void)
{
    int (*const map) = mm2_gen__map;
    static const MqBand BM = { 8, 17, 8, 9, 6, 0, 0, 1, 2, 800, 1 }, BF = { 214, PH, 10, 10, 6, 0, 0, 1, 2, 800, 1 };
    for (int i = 0; i < 3; i++) mb_band(0, &MM2_B[i], i);
    for (int i = 3; i < 5; i++) mb_band(1, &MM2_B[i], i);
    /* the mound in the left corner: its flank goes on down its top line (11 rows in 40 columns, a little steeper
       further out) until it leaves the frame; rows counted from that line */
    for (int x = 0; x < M2_MW; x++) {
        int y = 200;
        while (y < PH && lum(mm_px(x, y)) >= 120) y++;
        m2_top[x] = (short)y;
    }
    if (!mm_ph) {
        memset(mq_k, 0, (sizeof *mq_k * (PH))); for (int i = 0; i < PH * M2_TW; i++) map[i] = -1;
        mq_sw = 48; mq_tw = M2_TW; mq_nx = 40; mq_bmap = m2_mb;
        for (int v = 0; v < 32; v++)
            for (int x = 0; x < M2_MW; x++) {
                int y = v - 8 + m2_ts(x);
                mq_s[v][x] = mm_px(x, y);
                if (y < PH && y >= m2_top[x] - 3) mq_k[v][x] = 3;
            }
        mq_quilt(0, &BM, 81, map);
        for (int b = 0; b < MQ_NB; b++) {
            int sx = mq_bp[b] % MQ_SW, sy = mq_bp[b] / MQ_SW;
            m2_mt[b][0] = (short)(sx - mq_bx[b]); m2_mt[b][1] = (short)(sy + m2_ts(sx) - (mq_by[b] + m2_td(-mq_bx[b])));
        }
        for (int i = 0; i < PH * M2_TW; i++) if (map[i] < 0) m2_mb[i] = -1;
    }
    for (int X = 1; X <= mm_cw && X <= M2_TW; X++)
        for (int y = 200; y < PH; y++) {
            int b = m2_mb[clampi(y + 8 - m2_td(X), 8, 16) * M2_TW + X - 1], px, py;
            if (b < 0) continue;
            px = -X + m2_mt[b][0]; py = y + m2_mt[b][1];
            if (px >= 0 && px < M2_MW && py < PH && py >= m2_top[px] - 3) mm_put(0, X, y, mm_px(px, py));
        }
    /* the small rock's tail at the right edge: its texels 8 columns in and 6 rows up (its ridge falls 6 rows in 8
       columns), down to its waterline; its foam fades out */
    for (int X = 1; X <= mm_cw && X <= 8; X++) {
        for (int y = 160; y < 176; y++) if (mm2_rock(mm_px(311 + X, y - 6))) mm_put(1, X, y, mm_px(311 + X, y - 6));
        for (int y = 176; y < 178; y++) mm_put(1, X, y, mix(mm_px(311 + X, y), *mm_at(1, X, y), X <= 4 ? 0 : (X - 4) * 64));
    }
    /* the flat rock in the right corner: level, then its end */
    if (!mm_ph) {
        memset(mq_k, 0, (sizeof *mq_k * (PH))); for (int i = 0; i < PH * M2_TW; i++) map[i] = -1;
        mq_sw = 64; mq_tw = M2_TW; mq_nx = 48; mq_bmap = m2_fb;
        for (int y = 200; y < PH; y++)
            for (int x = 0; x < 64; x++) { mq_s[y][x] = mm_px(PW - 64 + x, y); if (x >= 20 && y >= 214 && mm2_rock(mq_s[y][x])) mq_k[y][x] = 3; }
        mq_quilt(1, &BF, 82, map);
        for (int b = 0; b < MQ_NB; b++) { m2_ft[b][0] = (short)(mq_bp[b] % MQ_SW - mq_bx[b]); m2_ft[b][1] = (short)(mq_bp[b] / MQ_SW - mq_by[b]); }
        for (int i = 0; i < PH * M2_TW; i++) if (map[i] < 0) m2_fb[i] = -1;
    }
    mq_bmap = 0;
    for (int X = 1; X <= mm_cw && X <= 48; X++)
        for (int y = 206; y < PH; y++) {
            int b = m2_fb[clampi(y, 214, PH - 1) * M2_TW + X - 1], px, py, cov = clampi(y * 16 + 16 - (211 * 16 + m2_fe16(X)), 0, 16);
            if (b < 0 || !cov) continue;
            px = PW - 1 + X + m2_ft[b][0]; py = y + m2_ft[b][1];
            if (px >= PW - 44 && px < PW && py >= 206 && py < PH && mm2_rock(mm_px(px, py))) mm_put(1, X, y, mix(*mm_at(1, X, y), mm_px(px, py), cov * 16));
        }
}

/* ================================================================== s1: the beach */
static const MbBand MM1_B[3] = {
    { 0, 152, MC_ANY, 8, 300, 0, 152, 0, 0, 400, 6, 6, 24, 24, 12, 8, 12, 4, 4, 2, 16, 0, 0, 0 },
    { 152, 180, MC_ANY, 8, 300, 152, 180, 0, 0, 20, 1, 8, 16, 7, 8, 3, 1, 1, 2, 0, 16, 30, 0, 0, 1 },
    { 180, PH, MC_ANY, 8, 300, 180, PH, 0, 0, 20, 2, 8, 12, 11, 6, 4, 8, 1, 2, 0, 16, 1, 0, 0 },
};
#define MM1_SAND 205                                     /* the sand under the foot at the edge: from this row */
static const MbBand MM1_BF = { MM1_SAND, PH, MC_ANY, 540, 640, MM1_SAND + 1, PH, 0, 0, 8, 2, 4, 12, 10, 6, 4, 8, 1, 2, 0, 16, 1, 0, 0 };   /* the sand under the foot */
#define MM1_BG0 8                                         /* the sky, sea and sand behind it: the picture's columns from here */
#define MM1_BGW 192
/* --- the pillar's closing.  Three layers, each quilted (mq_quilt) in its own space from the pillar's own texels and
   put back along the pillar's lines:
     A  the stratified rock over the lip (the bulge's lower edge): rows sheared along the strata (mp_slope);
     L  the rock's last rows over the lip (MP_LW columns in, where the lip is level): the lip's shade lies along the
        lip, not along the strata; quilted level;
     N  the smooth neck between the lip and the foot's lit rim: its rows counted from the lip to the rim, its tone the
        edge's, its grain quilted;
     F  the foot from its rim to its underside: rows counted from the rim to the sand. */
#define MP_SW 144                                         /* A's source: the picture's last columns */
#define MP_FW 41                                          /* N's and F's (further in the neck is lit and the foot ends) */
#define MP_NX 66                                          /* the columns they are read up to */
#define MP_TW 90                                          /* the layers' columns (the pillar closes within 51) */
#define MP_LIP 163                                        /* the neck's first row (level over the last 56 columns) */
#define MP_JW 22                                          /* columns over which the join's tone fades out */
#define MP_LW 57                                          /* L's source */
#define MP_L0 146                                         /* L's first row; from MP_L1 on it alone */
#define MP_L1 152
#define MP_GW 12                                          /* columns over which the strip gets its grain where the edge is smooth */
#define MP_NH 15
#define MP_FH 26
static Rgb mp_nt[MP_NH], (*mp_a)[MP_TW], (*mp_n)[MP_TW], (*mp_f)[MP_TW];
static short (*mp_ab), (*mp_fbk), (*mp_lb), (*mp_lt);   /* A's, F's and L's blocks; L's move */
static Rgb (*mp_l)[MP_TW];
static short (*mp_at)[2], (*mp_fu)[2];            /* a block of A: its texels' move (columns, rows); of F: its
                                                             move in columns, its source's column */
static Rgb (*mp_al)[MP_TW]; static short mp_ak[PH];     /* A smoothed (5 x 3); its grain at the join against the picture's (x 256) */
static Rgb mp_fc[MP_FH], (*mp_ac), (*mp_lc);                      /* the join's tone: the picture's last columns less the layer's first ones */
static int (*mp_fd);                                  /* F: the block's rows down from where they go (x 16) */
static short mp_ft[MP_FW], mp_fb[MP_FW];                  /* the foot's rim and the sand's first row, u columns in */
static int mp_slope(int v) { return v < 64 ? 148 - v * 15 / 64 : v < MP_LIP ? 133 - (v - 64) * 25 / 99 : 108; }   /* rows per
                                                             column (x 256) the strata rise to the right by, at edge row v */
static int mp_lip(int u) { return u <= 56 ? MP_LIP : MP_LIP - (u - 56) * 15 / 32; }   /* the lip u columns in */
static int mp_rock(Rgb c) { return c.r >= c.b; }
/* the far outline: its column (x 16) on row y; a smooth curve through MP_OL (one point per 16 rows, from row -16) */
static const short MP_OL[] = { 0, 192, 368, 480, 560, 624, 672, 712, 736, 736, 704, 648, 592, 768, 816, 816, 816, 816 };
static int mp_ol16(int y)
{
    int i = y / 16, t = (y % 16) * 16 + 8, p0 = MP_OL[i], p1 = MP_OL[i + 1], p2 = MP_OL[i + 2], p3 = MP_OL[i + 3];
    int c1 = p2 - p0, c2 = 2 * p0 - 5 * p1 + 4 * p2 - p3, c3 = 3 * p1 - p0 - 3 * p2 + p3;
    int v = (2 * p1 + c1 * t / 256 + c2 * t / 256 * t / 256 + c3 * t / 256 * t / 256 * t / 256) / 2;
    return v + (noise(0, y, 8, 22, 61) - 128) * 14 / 128;
}
static int mp_ft16(int X) { return mp_ft[0] * 16 - (X > 10 ? 10 : X) * 3; }   /* the foot's rim at column X (x 16) */
static int mp_fb16(int X)                                /* its underside */
{
    return mp_fb[0] * 16 + (X > 16 ? 16 : X) * 3 / 2 - (X > 36 ? (X - 36) * (X - 36) : 0);
}
/* the smoothed tone (6 columns x 5 rows) of the source's last columns less the canvas' first ones, per row */
static void mp_join(Rgb *out, const Rgb *cv, int rows)
{
    for (int v = 0; v < rows; v++) {
        int a[3] = { 0, 0, 0 }, b[3] = { 0, 0, 0 }, na = 0, nb = 0;
        for (int w = v - 2; w <= v + 2; w++) {
            if (w < 0 || w >= rows) continue;
            for (int i = 0; i < 6; i++) {
                if (mq_k[w][mq_sw - 1 - i]) { a[0] += mq_s[w][mq_sw - 1 - i].r; a[1] += mq_s[w][mq_sw - 1 - i].g; a[2] += mq_s[w][mq_sw - 1 - i].b; na++; }
                b[0] += cv[w * MP_TW + i].r; b[1] += cv[w * MP_TW + i].g; b[2] += cv[w * MP_TW + i].b; nb++;
            }
        }
        out[v] = na >= 6 ? (Rgb){ a[0] / na - b[0] / nb, a[1] / na - b[1] / nb, a[2] / na - b[2] / nb } : (Rgb){ 0, 0, 0 };
    }
}
static Rgb mp_toned(Rgb c, Rgb d, int X)                 /* c with the join's tone d, fading out over MP_JW columns */
{
    int q = X >= MP_JW ? 256 : X * 256 / MP_JW, w = 256 - q * q / 256 * (768 - 2 * q) / 256;
    return (Rgb){ clampi(c.r + d.r * w / 256, 0, 255), clampi(c.g + d.g * w / 256, 0, 255), clampi(c.b + d.b * w / 256, 0, 255) };
}
static void mp_clear(int *map) { memset(mq_k, 0, (sizeof *mq_k * (PH))); for (int i = 0; i < PH * MP_TW; i++) map[i] = -1; mq_tw = MP_TW; mq_nx = MP_NX; }
static int (*mp_gen__map);
static void mp_gen(void)
{
    int (*const map) = mp_gen__map;
    static const MqBand BL = { MP_L0, MP_LIP, 12, MP_LIP - MP_L0, 8, 0, 0, 1, 4, 300, 1 };
    static const MqBand BA = { 0, 200, 18, 14, 14, 5, 5, 2, 8, 300 }, BN = { 0, MP_NH, 8, 8, 4, 3, 1, 1, 0, 0 }, BF = { 0, MP_FH, 10, 9, 8, 3, 2, 1, 4, 800 };
    for (int u = 0; u < MP_FW; u++) {
        int y = 174, n;
        while (y < 190 && lum(mm_px(mm_w - 1 - u, y)) < 130) y++;
        mp_ft[u] = (short)y;
        for (y = 196; y < PH - 4; y++) {
            for (n = 0; n < 4; n++) { Rgb c = mm_px(mm_w - 1 - u, y + n); if (lum(c) < 125 || c.r < c.b + 40) break; }
            if (n == 4) break;
        }
        mp_fb[u] = (short)y;
    }
    /* A */
    mp_clear(map); mq_sw = MP_SW; mq_bmap = mp_ab;
    for (int v = 0; v < PH; v++)
        for (int i = 0; i < MP_SW; i++) {
            int u = MP_SW - 1 - i, y = v + (mp_slope(v) * u + 128) / 256;
            mq_s[v][i] = mm_px(mm_w - 1 - u, y);
            if (y < mp_lip(u) && mp_rock(mq_s[v][i])) mq_k[v][i] = 3;
        }
    mq_quilt(1, &BA, 71, map);
    for (int v = 0; v < PH; v++) for (int X = 1; X <= MP_TW; X++) mp_a[v][X - 1] = map[v * MP_TW + X - 1] >= 0 ? mq_tx(map, X, v) : mq_s[v][mq_sw - 1];
    mp_join(mp_ac, &mp_a[0][0], PH);
    /* where the edge cuts a smooth face, the strip starts as smooth and gets its grain over MP_GW columns: the grain
       (texel less the 5 x 3 mean round it) of the picture's last 6 columns against that of A's first 6, per row */
    for (int v = 0; v < PH; v++)
        for (int X = 1; X <= MP_TW; X++) {
            int t[3] = { 0, 0, 0 }, n = 0;
            for (int w = v - 1; w <= v + 1; w++) for (int u = X - 2; u <= X + 2; u++)
                if (w >= 0 && w < PH && u >= 1 && u <= MP_TW) { t[0] += mp_a[w][u - 1].r; t[1] += mp_a[w][u - 1].g; t[2] += mp_a[w][u - 1].b; n++; }
            mp_al[v][X - 1] = (Rgb){ t[0] / n, t[1] / n, t[2] / n };
        }
    for (int v = 0; v < PH; v++) {
        int a = 0, c = 0, na = 0, nc = 0;
        for (int w = v - 2; w <= v + 2; w++) {
            if (w < 0 || w >= PH) continue;
            for (int i = 0; i < 6; i++) {
                if (mq_k[w][mq_sw - 1 - i]) { a += abs(lum(mq_s[w][mq_sw - 1 - i]) - lum(mq_l[w][mq_sw - 1 - i])); na++; }
                c += abs(lum(mp_a[w][i]) - lum(mp_al[w][i])); nc++;
            }
        }
        mp_ak[v] = (short)(na >= 6 ? clampi((a * 256 / na + 512) * 256 / (c * 256 / nc + 512), 64, 256) : 256);
    }
    for (int b = 0; b < MQ_NB; b++) {                    /* a block's texels all move as its first one */
        int sx = mq_bp[b] % MQ_SW, sy = mq_bp[b] / MQ_SW, Xa = mq_bx[b] - MP_SW + 1, ya = mq_by[b];
        mp_at[b][0] = (short)(sx - MP_SW + 1 - Xa);
        mp_at[b][1] = (short)(sy + (mp_slope(sy) * (MP_SW - 1 - sx) + 128) / 256 - (ya - fdiv(mp_slope(ya) * Xa + 128, 256)));
    }
    /* L */
    mp_clear(map); mq_sw = MP_LW; mq_bmap = mp_lb;
    for (int v = MP_L0; v < MP_LIP; v++)
        for (int i = 0; i < MP_LW; i++) { mq_s[v][i] = mm_px(mm_w - MP_LW + i, v); if (mp_rock(mq_s[v][i])) mq_k[v][i] = 3; }
    mq_quilt(1, &BL, 74, map);
    for (int v = MP_L0; v < MP_LIP; v++) for (int X = 1; X <= MP_TW; X++) mp_l[v][X - 1] = map[v * MP_TW + X - 1] >= 0 ? mq_tx(map, X, v) : (Rgb){ 0, 0, 0 };
    mp_join(mp_lc, &mp_l[0][0], PH);
    for (int b = 0; b < MQ_NB; b++) mp_lt[b] = (short)(mq_bp[b] % MQ_SW - mq_bx[b]);
    /* N: the grain (a texel less the mean of the 9 columns round it) */
    mp_clear(map); mq_sw = MP_FW; mq_bmap = 0;
    for (int v = 0; v < MP_NH; v++) {
        int t[3] = { 0, 0, 0 };
        for (int i = 0; i < MP_FW; i++) {
            int s[3] = { 0, 0, 0 }, n = 0; Rgb c = { 0, 0, 0 };
            for (int d = -4; d <= 4; d++) {
                int u = MP_FW - 1 - i + d; Rgb q;
                if (u < 0 || u >= MP_FW) continue;
                q = mm_px(mm_w - 1 - u, MP_LIP + v * (mp_ft[u] - MP_LIP) / MP_NH);
                s[0] += q.r; s[1] += q.g; s[2] += q.b; n++; if (!d) c = q;
            }
            mq_s[v][i] = (Rgb){ c.r - s[0] / n, c.g - s[1] / n, c.b - s[2] / n }; mq_k[v][i] = 3;
            if (i >= MP_FW - 8) { t[0] += c.r; t[1] += c.g; t[2] += c.b; }
        }
        mp_nt[v] = (Rgb){ t[0] / 8, t[1] / 8, t[2] / 8 };
    }
    mq_quilt(1, &BN, 72, map);
    for (int v = 0; v < MP_NH; v++) for (int X = 1; X <= MP_TW; X++) mp_n[v][X - 1] = map[v * MP_TW + X - 1] >= 0 ? mq_tx(map, X, v) : (Rgb){ 0, 0, 0 };
    /* F */
    mp_clear(map); mq_bmap = mp_fbk;
    for (int v = 0; v < MP_FH; v++)
        for (int i = 0; i < MP_FW; i++) {
            int u = MP_FW - 1 - i;
            mq_s[v][i] = mm_px(mm_w - 1 - u, mp_ft[u] + v * (mp_fb[u] - mp_ft[u]) / MP_FH); mq_k[v][i] = 3;
        }
    mq_quilt(1, &BF, 73, map);
    for (int v = 0; v < MP_FH; v++) for (int X = 1; X <= MP_TW; X++) mp_f[v][X - 1] = map[v * MP_TW + X - 1] >= 0 ? mq_tx(map, X, v) : mq_s[v][mq_sw - 1];
    mp_join(mp_fc, &mp_f[0][0], MP_FH);
    for (int b = 0; b < MQ_NB; b++) {
        int sx = mq_bp[b] % MQ_SW, sy = mq_bp[b] / MQ_SW, Xa = mq_bx[b] - MP_FW + 1;
        mp_fu[b][0] = (short)(sx - MP_FW + 1 - Xa); mp_fu[b][1] = (short)clampi(MP_FW - 1 - sx - (BF.BW + BF.OV) / 2, 0, MP_FW - 1);
        mp_fd[b] = (sy - mq_by[b]) * 16;
    }
    mq_bmap = 0;
}
/* the pillar's rock at (X, y), unlit by the far side's shade.  A block's texels are the picture's moved as one piece
   (a column never slides against the next one) */
static Rgb mp_px(int X, int y, int ol)
{
    int t = ol - (X * 16 - 8), wc = ol - 16 > 256 ? 256 : ol - 16, U, y16 = y * 16 + 8, ft = mp_ft16(X), fb = mp_fb16(X);
    Rgb n, f;
    /* towards the outline the rock turns away: more of it per column */
    U = (X * 16 - 8 + (t < wc && wc > 0 ? (wc - t) * (wc - t) * 9 / (wc * 16) : 0)) / 16 + 1;
    U = clampi(U, 1, MP_NX);
    if (y < MP_LIP) {
        int v = y, b, px, py;
        for (int i = 0; i < 2; i++) v = y + (mp_slope(v) * X + 128) / 256;
        v = clampi(v, 0, PH - 1);
        b = mp_ab[v * MP_TW + U - 1]; px = mm_w - 1 + U + mp_at[b][0]; py = y + (mp_slope(v) * X + 128) / 256 - (mp_slope(v) * U + 128) / 256 + mp_at[b][1];
        {   Rgb c = px >= mm_w - MP_SW && px < mm_w && py >= 0 && py < mp_lip(mm_w - 1 - px) && mp_rock(mm_px(px, py)) ? mm_px(px, py) : mp_a[v][U - 1];
            if (X < MP_GW && mp_ak[v] < 256) {
                int q = X * 256 / MP_GW, k = mp_ak[v] + (256 - mp_ak[v]) * (q * q / 256 * (768 - 2 * q) / 256) / 256;
                c = mix(mp_al[v][U - 1], c, k);
            }
            c = mp_toned(c, mp_ac[v], X);
            if (y >= MP_L0) {                            /* L: alone from MP_L1 on; over it, the lighter of the two (the
                                                            strata's strokes end in the lip's shade) */
                int lx = mm_w - 1 + U + mp_lt[mp_lb[y * MP_TW + U - 1]];
                Rgb l = lx >= mm_w - MP_LW && lx < mm_w && mp_rock(mm_px(lx, y)) ? mm_px(lx, y) : mp_l[y][U - 1];
                l = mp_toned(l, mp_lc[y], X);
                if (y >= MP_L1 || lum(l) > lum(c)) c = l;
            }
            return c;
        }
    }
    {   int vn = clampi((y16 - MP_LIP * 16) * MP_NH / (ft - MP_LIP * 16), 0, MP_NH - 1), vf16 = clampi((y16 - ft) * MP_FH * 16 / (fb - ft), 0, MP_FH * 16 - 1);
        int b = mp_fbk[vf16 / 16 * MP_TW + U - 1], u = mp_fu[b][1], px = mm_w - 1 + U + mp_fu[b][0], py = mp_ft[u] + (vf16 + mp_fd[b]) * (mp_fb[u] - mp_ft[u]) / (MP_FH * 16);
        n = mp_nt[vn]; n.r += mp_n[vn][U - 1].r; n.g += mp_n[vn][U - 1].g; n.b += mp_n[vn][U - 1].b;
        f = px >= mm_w - MP_FW && px < mm_w && py >= mp_ft[mm_w - 1 - px] - 1 && py < mp_fb[mm_w - 1 - px] ? mm_px(px, py) : mp_f[vf16 / 16][U - 1];
        return mix(mp_toned(f, mp_fc[vf16 / 16], X), n, clampi(ft - y * 16, 0, 16) * 16);
    }
}
static int (*mm1_gen__sand)[3];
static void mm1_gen(void)
{
    int (*const sand)[3] = mm1_gen__sand;                              /* the sand at the edge against the sand copied next to it,
                                                            per row (x 16; above the foot's underside: fading out) */
    for (int y = PH - 1; y >= 181; y--) {
        int a[3], b[3];
        if (y >= MM1_SAND) {
            mm_lp(MC_ANY, mm_w - 9, y, 8, 2, MM1_SAND, PH, a); mm_lp(MC_ANY, MM1_BG0 + 8, y, 8, 2, MM1_SAND, PH, b);
            for (int c = 0; c < 3; c++) sand[y][c] = a[c] - b[c];
        } else for (int c = 0; c < 3; c++) sand[y][c] = sand[MM1_SAND][c] * (y - 181) / (MM1_SAND - 181);
    }
    for (int i = 0; i < 3; i++) mb_band(0, &MM1_B[i], i);
    mb_band(1, &MM1_BF, 3);
    mp_gen();
    for (int y = 0; y < PH; y++) {
        int ol = mp_ol16(y), wl = ol - 16 > 28 * 16 ? 28 * 16 : ol - 16;
        for (int X = 1; X <= mm_cw; X++) {
            int cov = clampi(ol - (X - 1) * 16, 0, 16) * clampi(mp_fb16(X) - y * 16, 0, 16);
            Rgb bg = y >= MM1_SAND ? *mm_at(1, X, y) : mm_px(MM1_BG0 + fmodi(X - 1, MM1_BGW), y);
            if (y >= 181 && y < MM1_SAND) {
                bg.r = clampi(bg.r + fdiv(sand[y][0] + 8, 16), 0, 255); bg.g = clampi(bg.g + fdiv(sand[y][1] + 8, 16), 0, 255);
                bg.b = clampi(bg.b + fdiv(sand[y][2] + 8, 16), 0, 255);
            }
            if (cov) {
                int q = clampi((ol - (X * 16 - 8)) * 256 / (wl > 0 ? wl : 1), 0, 256), k = 100 + 156 * (q * q / 256 * (768 - 2 * q) / 256) / 256;
                bg = mix(bg, scale(mp_px(X, y, ol), k), cov);
            }
            mm_put(1, X, y, bg);
        }
    }
}

#ifndef WS_BACKDROP_MAIN
static u16 *mm_strip; static int mm_pad = -1, mm_room, mm_gl; static u32 mm_sum;
extern int port_gpu_is_gl(void);
static u16 *mm_buf;                                      /* s1: this frame's strips */
static void mm_grab(const u16 *vram, int sc)
{
    if (sc == 2) {
        mm_w = PW;
        for (int i = 0; i < 256; i++) mm_clut[i] = vram[(size_t)511 * 1024 + 128 + i];
        for (int p = 0; p < MM_NPH; p++) {
            for (int y = 0; y < PH; y++) {
                for (int x = 0; x < 256; x++) mm_src[p][y][x] = tex8(vram, 128, x, y, 128, 511);
                for (int x = 0; x < 64; x++) mm_src[p][y][256 + x] = tex8(vram, 256, x, y, 128, 511);
            }
            for (int i = 0; i < 6; i++) {
                const Piece *q = &MM2_Q[p][i];
                /* (a quad of w pixels spans w - 1 texels: the software renderer rounds half up, OpenGL truncates) */
                for (int y = 0; y < q->h; y++) for (int x = 0; x < q->w; x++) {
                    u16 c = tex8(vram, q->tx, q->u + (x * (q->w - 1) + (mm_gl ? 0 : q->w / 2)) / q->w, q->v + (y * (q->h - 1) + (mm_gl ? 0 : q->h / 2)) / q->h, 128, 511);
                    if (c && q->y + y < PH && q->x + x < PW) mm_src[p][q->y + y][q->x + x] = c;
                }
            }
        }
    } else {
        mm_w = MM_W;
        for (int i = 0; i < 256; i++) mm_clut[i] = vram[(size_t)510 * 1024 + 128 + i];
        for (int y = 0; y < PH; y++) for (int x = 0; x < MM_W; x++) mm_src[0][y][x] = tex8(vram, 576, x, y, 128, 510);
    }
    {   const char *e = getenv("PORT_WS_BACKDROP_DUMP");      /* research: the CLUT, then the pictures (raw 15-bit LE, MM_W columns) */
        FILE *f = e ? fopen(e, "wb") : 0;
        if (f) { fwrite(mm_clut, 1, sizeof mm_clut, f); fwrite(mm_src, 1, sc == 2 ? (sizeof *mm_src * (MM_NPH)) : sizeof mm_src[0], f); fclose(f); }
    }
}
/* the picture's pages and CLUT, sampled (every 8th row): has the picture changed */
static u32 mm_check(const u16 *vram, int sc)
{
    u32 h = 2166136261u; int x0 = sc == 2 ? 128 : 576, x1 = sc == 2 ? 512 : 896, cy = sc == 2 ? 511 : 510;
    for (int i = 0; i < 256; i++) h = (h ^ vram[(size_t)cy * 1024 + 128 + i]) * 16777619u;
    for (int y = 3; y < 256; y += 8) for (int x = x0; x < x1; x++) h = (h ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return h;
}
static int mm_build(const u16 *vram, int pad, int sc)
{
    if (!ws_need_mm_build()) return 0;
    int nph = sc == 2 ? MM_NPH : 1;
    mm_grab(vram, sc);
    memset(mm_qh, 0, (sizeof *mm_qh * (32768)));        /* (the quantiser's cache: this picture's CLUT) */
    free(mm_strip); free(mm_buf); mm_buf = 0;
    mm_strip = calloc((size_t)nph * PH * 2 * pad, 2);
    if (sc == 1) mm_buf = calloc((size_t)PH * 2 * pad, 2);
    if (!mm_strip || (sc == 1 && !mm_buf) || !mm_canvas(pad)) { free(mm_strip); free(mm_buf); mm_strip = 0; mm_buf = 0; return 0; }
    for (mm_ph = 0; mm_ph < nph; mm_ph++) {
        if (sc == 2) mm2_gen(); else mm1_gen();
        for (int y = 0; y < PH; y++)
            for (int X = 1; X <= pad; X++) {
                u16 *row = mm_strip + ((size_t)mm_ph * PH + y) * 2 * pad;
                row[pad - X] = mm_quant(*mm_at(0, X, y)); row[pad + X - 1] = mm_quant(*mm_at(1, X, y));
            }
    }
    mm_ph = 0;
    mm_canvas_free();
    return 1;
}
/* the waves' phase: the picture the wave objects (type 74, subtypes 2-4: drawArea6SpriteQuad) show now */
static int mm2_phase(void)
{
    static int last;
    u8 *pool = PORT_M0(PORT_A_OBJECT_POOL);
    for (int k = 0; k < 200; k++) {
        u8 *o = pool + k * 0xD4; u32 anim = *(u32 *)(o + 0x24), d3c = *(u32 *)(o + 0x3C);
        if (!o[0] || o[2] != 74 || o[3] < 2 || o[3] > 4 || !anim || !d3c) continue;
        {   const short *f = (const short *)((size_t)d3c + (*(const short *)(size_t)anim << 2));
            const u8 *e = (const u8 *)((size_t)d3c + f[1]);
            int tx = ((*(const u16 *)(e + 6) + *(const short *)(o + 0x1E)) & 15) * 64;
            if (f[0] <= 0) continue;
            for (int p = 0; p < MM_NPH; p++) for (int i = 0; i < 6; i++)
                if (MM2_Q[p][i].w && MM2_Q[p][i].tx == tx && MM2_Q[p][i].u == e[0] && MM2_Q[p][i].v == e[1]) return last = p;
        }
    }
    return last;
}
/* mm_build through the cache (ws_cache): the strips of every phase, keyed by the pictures and the CLUT */
static int mm_cached(const u16 *vram, int pad, int sc)
{
    int nph = sc == 2 ? MM_NPH : 1; size_t n = (size_t)nph * PH * 2 * pad * 2; WsCPart p; u32 h;
    mm_grab(vram, sc);
    h = wsc_hash(wsc_hash(mm_sum, mm_clut, sizeof mm_clut), mm_src, sizeof *mm_src * (size_t)nph);
    free(mm_strip); free(mm_buf); mm_buf = 0;
    mm_strip = calloc(n / 2, 2);
    if (sc == 1) mm_buf = calloc((size_t)PH * 2 * pad, 2);
    p.p = mm_strip; p.n = n;
    if (mm_strip && (sc != 1 || mm_buf) && wsc_get("mm", 6, sc, pad, h, (u32)mm_gl, &p, 1)) return 1;
    if (!mm_build(vram, pad, sc)) return 0;
    p.p = mm_strip; wsc_put("mm", 6, sc, pad, h, (u32)mm_gl, &p, 1);
    return 1;
}
/* a 256-column sprite of CLUT (128, 510 / 511) at frame (x, y) in section sc: the strips when it is that picture's */
static const u16 *mm_take(const u16 *vram, int pad, int sc, int x, int y, int w, int h, int u, int v, int tx, int ty, int cy, int *rows)
{
    if (!ws_need_mm_take()) return 0;
    u32 s;
    if (y != 16 || w != 256 || h != PH || u || v || ty) return 0;
    if (sc == 2 ? x != 0 || tx != 128 || cy != 511 : tx != 576 || cy != 510) return 0;
    s = mm_check(vram, sc);
    if (!mm_strip || mm_pad != pad || mm_room != sc || s != mm_sum || mm_gl != port_gpu_is_gl()) {
        Rgb t = rgb15(tex8(vram, sc == 2 ? 128 : 576, 4, 4, 128, cy));    /* (the sky in the top left corner: this picture) */
        mm_pad = pad; mm_room = sc; mm_sum = s; mm_gl = port_gpu_is_gl();
        if (t.b < t.r + 24 || !mm_cached(vram, pad, sc)) { free(mm_strip); mm_strip = 0; }
    }
    if (!mm_strip) return 0;
    *rows = PH;
    if (sc == 2) {
        return mm_strip + (size_t)mm2_phase() * PH * 2 * pad;
    }
    if (x <= -pad && x + MM_W >= 320 + pad) return 0;    /* the picture covers the wide frame */
    for (int j = 0; j < PH; j++) {
        u16 *row = mm_buf + (size_t)j * 2 * pad; const u16 *ext = mm_strip + (size_t)j * 2 * pad;
        for (int c = 0; c < pad; c++) {
            int e = c - pad - x, f = 320 + c - x;      /* the picture's column at the left / right strip's column c */
            row[c] = e < 0 ? (-e <= pad ? ext[pad + e] : 0) : e < MM_W ? mm_src[0][j][e] : 0;
            row[pad + c] = f >= MM_W ? (f - MM_W < pad ? ext[pad + f - MM_W] : 0) : f >= 0 ? mm_src[0][j][f] : 0;
        }
    }
    return mm_buf;
}
#endif
