/* runtime/ws_backdrop.c: AREA09 s1, the Lake (the beach with the cave in the cliff): the two ends of its picture
 * (included there; docs/PORT.md "More rooms with picture strips").
 *
 * The beach is a 512 x 256 picture, CLUT (128, 510): its left half from page (896, 0) (rows 32-255 only: above them
 * the clear colour is the sky's) and its right half from page (0, 256), two sprites at frame row 4, moved by the
 * camera: the right half's sprite is at frame column 110 with the camera at its right limit and at 200 at the other
 * end of this screen while playing (then the room cuts to the island); the boat scene holds the camera further left
 * (about 228 on the owner's screenshot; not reached headless).  So the picture's right edge is 46 columns past the
 * 4:3 frame at the right limit (7 columns short of a 16:9 frame, 74 of a 21:9 one) and its left edge 56 columns past
 * it at the other end (8 columns short at 21:9; in the boat scene 25 at 16:9).  lk_take() takes any column from 110
 * on: it fills the strip of each end the wide frame looks past, from the picture's columns and that end's extension
 * (made once, when first needed), and leaves the other strip alone (port_ws_backdrop_side).  The left strip alone is
 * laid from the left half's first row with texels (LK_S), so the rows above stay the game's clear colour; with both
 * strips those rows are the right half's first column (a tone of the CLUT, one step of blue from the clear colour:
 * the step the game itself shows where the two halves meet).
 *
 * The world goes on to the left: the island's mosaic (ws_backdrop_island.inc) ends 128 columns left of this picture,
 * and the game draws it over the strip from there (seen at 32:9 only); nothing of the game fills those 128 columns.
 *
 * Measured on the picture (rows counted from frame row 16, as the strips are: picture row - 12), at its right edge:
 *   - sky to row 49: a gradient of flat tones without grain, level; on rows 39-49 the top of a cloud that rises
 *     towards the edge and is cut by it (and by the grass under it);
 *   - the cliff's grass from row 50 (its blades' tips) to 64, about level over the last 60 columns;
 *   - the cliff's rock from there to its foot on row 200: a mottled face without strata, lit evenly; the cave is 70
 *     columns in;
 *   - the sand from row 201; the foot's line comes down towards the edge, 0.2 rows per column over the last 40.
 * The edge cuts the cliff's face, which goes on: the same grass line, the same rock, the foot further down its line.
 * At its left edge:
 *   - sky to row 103: the same gradient of flat, level tones (rows 0-19: the clear colour);
 *   - the cumulus bank from row 104 to the horizon: the big cloud's left flank comes down to the edge (a lump 20
 *     columns in, top on row 91) and the edge cuts its body and the haze over the horizon;
 *   - the sea from row 152 (LK_H; the horizon is level within 4 rows over the picture) to the bottom: glints and
 *     ripples that grow towards the viewer, no sand (the shore starts 210 columns in).
 *
 * The extensions are made with the Singing Rock's tools (ws_backdrop_mermaid.inc: mb_band, a band's tone at the edge
 * carried on level plus its detail quilted from the picture's own texels next to the edge, never mirrored; the
 * edge's own columns go on first and the join's tone fades out).  Right:
 *   - the sky as a smooth band (the cloud's top goes on level behind the grass);
 *   - the grass and the rock as one band whose windows are the face's columns right of the cave, rows 50 to 186
 *     (above the foot's line there), each near its own row, so the grass stays the top fringe;
 *   - the sand as a band from its own rows under the cliff;
 *   - the rock comes down to the foot's line, 0.2 rows per column lower, the sand under it (one texel of coverage).
 * Left (LK_L, as the Singing Rock's beach):
 *   - the sky and the clouds as one smooth band: its tone the rows' own, its detail the clouds' lumps quilted from
 *     the picture's first 320 columns within 2 rows of their own (the gradient stays level), so the bank goes on in
 *     lumps of the picture's kind;
 *   - the far sea (to row 179) and the near sea as bands from their own rows of the open water (columns 4-189).
 * Every colour is then the nearest of the picture's CLUT.  Integers only; a texel depends on its place, not on the
 * strip's width. */
#define LK_HM 1227 /* the ROOMS row's mark (no picture has this height) */
#define LK_W 512
#define LK_G 50                            /* the grass' first row at the edge */
#define LK_F 201                           /* the sand's first row at the edge */
#define LK_X0 110                          /* the right half's frame column, camera at its right limit */
#define LK_X1 200                          /* and at the other end of the screen (playing) */
static void gen_lk(int pad) { (void)pad; } /* (the table's gen: lk_take makes the strip) */
static const MbBand LK_B[3] = {
    { 0, LK_G, MC_ANY, 440, LK_W, 0, LK_G, 0, 0, 3, 1, 3, 24, 24, 12, 8, 12, 4, 4, 2, 16, 0, 0, 0, 0 },           /* sky */
    { LK_G, PH, MC_ANY, 446, LK_W, LK_G, 187, LK_F - 1, 0, 40, 16, 16, 18, 14, 8, 4, 6, 1, 1, 0, 16, 1, 0, 0, 0 }, /* grass, rock */
    { LK_F, PH, MC_ANY, 420, LK_W, LK_F + 1, PH, 0, 0, 8, 2, 4, 12, 10, 6, 4, 8, 1, 2, 0, 16, 1, 0, 0, 0 },      /* sand */
};
/* the right end's extension: the sky, the grass and rock, the sand; then the rock brought down to the foot's line */
static void lk_gen(void)
{
    Rgb *rock = malloc((size_t)(PH - LK_F) * mm_cw * sizeof *rock);
    mb_band(1, &LK_B[0], 0);
    mb_band(1, &LK_B[1], 1);
    if (rock)
        for (int y = LK_F; y < PH; y++)
            for (int X = 1; X <= mm_cw; X++) rock[(size_t)(y - LK_F) * mm_cw + X - 1] = *mm_at(1, X, y);
    mb_band(1, &LK_B[2], 2);
    if (rock)
        for (int y = LK_F; y < PH; y++)
            for (int X = 1; X <= mm_cw; X++) {
                int cov = clampi(LK_F * 16 + X * 16 / 5 - y * 16, 0, 16); /* the rock down to the foot's line */
                if (cov) mm_put(1, X, y, mix(*mm_at(1, X, y), rock[(size_t)(y - LK_F) * mm_cw + X - 1], cov * 16));
            }
    free(rock);
}

#define LK_H 152 /* the sea's first row at the left edge (the horizon) */
#define LK_S 20  /* the left half's first row with texels */
static const MbBand LK_L[3] = {
    { 0, LK_H, MC_ANY, 0, 320, 0, LK_H, 0, 0, 400, 6, 6, 24, 24, 12, 8, 2, 4, 4, 2, 16, 0, 0, 0, 0 },            /* sky, clouds */
    { LK_H, 180, MC_ANY, 4, 190, LK_H, 180, 0, 0, 20, 1, 8, 16, 7, 8, 3, 1, 1, 2, 0, 16, 30, 0, 0, 1 },          /* the far sea */
    { 180, PH, MC_ANY, 4, 190, 180, PH, 0, 0, 20, 2, 8, 12, 11, 6, 4, 8, 1, 2, 0, 16, 1, 0, 0, 0 },              /* the near sea */
};
/* the left end's extension: the sky and clouds, the far sea, the near sea */
static void lk_gen_l(void)
{
    for (int i = 0; i < 3; i++) mb_band(0, &LK_L[i], i);
}

#ifndef WS_BACKDROP_MAIN
static u16 *lk_ext[2], *lk_buf;
static int lk_pad = -1,
           lk_ext_w[2], lk_made[2];
static u32 lk_sum;
/* the strips shims/gpu.c lays: 1 the left one, 2 the right one */
int port_ws_backdrop_side = 3;
static u16 lk_tex(const u16 *vram, int x, int y) /* the picture's column x on strip row y */
{
    return x < 256 ? (y >= LK_S ? tex8(vram, 896, x, y + 12, 128, 510) : tex8(vram, 0, 0, 256 + y + 12, 128, 510))
                   : tex8(vram, 0, x - 256, 256 + y + 12, 128, 510);
}
static u32 lk_check(const u16 *vram) /* the right half's page and the CLUT, sampled */
{
    u32 fnv = 2166136261u;
    for (int i = 0; i < 256; i++) fnv = (fnv ^ vram[(size_t)510 * 1024 + 128 + i]) * 16777619u;
    for (int y = 259; y < 512; y += 8)
        for (int x = 0; x < 128; x++) fnv = (fnv ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return fnv;
}
/* an end's extension, made when the frame first looks past it */
static void lk_build(const u16 *vram, int side)
{
    if (!ws_need_lk_build()) return;
    int w = lk_ext_w[side];
    Rgb sky = rgb15(tex8(vram, 0, 4, 256 + 16, 128, 510)); /* (the sky in the top left corner: this picture) */
    lk_made[side] = 1;
    if (sky.b < sky.r + 24) return;
    /* (the Singing Rock's picture is no longer in mm_src) */
    mm_w = LK_W;
    mm_ph = 0;
    mm_pad = -1;
    for (int i = 0; i < 256; i++) mm_clut[i] = vram[(size_t)510 * 1024 + 128 + i];
    memset(mm_qh, 0, (sizeof *mm_qh * (32768)));
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < LK_W; x++) mm_src[0][y][x] = lk_tex(vram, x, y);
    lk_ext[side] = calloc((size_t)PH * w, 2);
    if (!lk_ext[side] || !mm_canvas(w)) {
        free(lk_ext[side]);
        lk_ext[side] = 0;
        return;
    }
    if (side)
        lk_gen();
    else
        lk_gen_l();
    for (int y = 0; y < PH; y++)
        for (int X = 1; X <= w; X++) lk_ext[side][(size_t)y * w + X - 1] = mm_quant(*mm_at(side, X, y));
    mm_canvas_free();
}
/* lk_build through the cache (ws_cache), keyed by the whole picture and its CLUT (read from VRAM: no heap table is
   needed when the end is in the cache) */
static void lk_cached(const u16 *vram, int side)
{
    Rgb sky = rgb15(tex8(vram, 0, 4, 256 + 16, 128, 510));
    u32 key = lk_sum;
    WsCPart entry;
    int w = lk_ext_w[side];
    if (sky.b >= sky.r + 24) {
        u16 row[LK_W];
        for (int i = 0; i < 256; i++) {
            u16 c = vram[(size_t)510 * 1024 + 128 + i];
            key = wsc_hash(key, &c, 2);
        }
        for (int y = 0; y < PH; y++) {
            for (int x = 0; x < LK_W; x++) row[x] = lk_tex(vram, x, y);
            key = wsc_hash(key, row, sizeof row);
        }
        lk_made[side] = 1;
        free(lk_ext[side]);
        lk_ext[side] = calloc((size_t)PH * w, 2);
        entry.p = lk_ext[side];
        entry.n = (size_t)PH * w * 2;
        if (lk_ext[side] && wsc_get("lk", 9, 1, lk_pad, key, (u32)side, &entry, 1)) {
            mm_w = LK_W;
            mm_ph = 0;
            mm_pad = -1;
            return;
        }
        free(lk_ext[side]);
        lk_ext[side] = 0;
    }
    lk_build(vram, side);
    if (lk_ext[side] && sky.b >= sky.r + 24) {
        entry.p = lk_ext[side];
        entry.n = (size_t)PH * w * 2;
        wsc_put("lk", 9, 1, lk_pad, key, (u32)side, &entry, 1);
    }
}
/* the picture's right half (page (0, 256), CLUT (128, 510)) at frame column x: the strips of the ends the wide frame
   looks past, the picture's own columns and then the extension's */
static const u16 *lk_take(const u16 *vram, int pad, int x, int *rows)
{
    int need_left = x - 256 > -pad,
        need_right = x + 256 < 320 + pad,
        top;
    u32 sum;
    if (x < LK_X0 || (!need_left && !need_right)) return 0;
    sum = lk_check(vram);
    if (!lk_buf || lk_pad != pad || sum != lk_sum) {
        lk_pad = pad;
        lk_sum = sum;
        lk_ext_w[0] = pad < MQ_SW - 56 ? pad : MQ_SW - 56; /* (the quilter's canvas) */
        lk_ext_w[1] = pad - (LK_X0 + 256 - 320);           /* the columns ever needed */
        free(lk_ext[0]);
        free(lk_ext[1]);
        lk_ext[0] = lk_ext[1] = 0;
        lk_made[0] = lk_made[1] = 0;
        free(lk_buf);
        lk_buf = calloc((size_t)PH * 2 * pad, 2);
    }
    if (!lk_buf) return 0;
    if (need_left && !lk_made[0]) lk_cached(vram, 0);
    if (need_right && !lk_made[1]) lk_cached(vram, 1);
    if ((need_left && !lk_ext[0]) || (need_right && !lk_ext[1])) return 0;
    /* the left strip alone: the rows above it stay the clear colour */
    top = need_right ? 0 : LK_S;
    for (int j = top; j < PH; j++) {
        u16 *row = lk_buf + (size_t)j * 2 * pad;
        if (need_left)
            for (int c = 0; c < pad; c++) {
                /* the picture's column at the left strip's column c */
                int left_col = c - pad - (x - 256),
                    X = -left_col > lk_ext_w[0] ? lk_ext_w[0] : -left_col;
                row[c] = left_col >= 0 ? lk_tex(vram, left_col, j) : lk_ext[0][(size_t)j * lk_ext_w[0] + X - 1];
            }
        if (need_right)
            for (int c = 0; c < pad; c++) {
                int right_col = 320 + c - (x - 256); /* the picture's column at the right strip's column c */
                row[pad + c] = right_col < LK_W                 ? lk_tex(vram, right_col, j)
                               : right_col - LK_W < lk_ext_w[1] ? lk_ext[1][(size_t)j * lk_ext_w[1] + right_col - LK_W]
                                                                : 0;
            }
    }
    port_ws_backdrop_dy = 12 + top;
    port_ws_backdrop_side = (need_left ? 1 : 0) | (need_right ? 2 : 0);
    *rows = PH - top;
    return lk_buf + (size_t)top * 2 * pad;
}
#endif
