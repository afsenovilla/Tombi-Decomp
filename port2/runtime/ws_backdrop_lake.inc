/* runtime/ws_backdrop.c: AREA09 s1, the Lake (the beach with the cave in the cliff): the right end of its picture
 * (included there; docs/PORT.md "More rooms with picture strips").
 *
 * The beach is a 512 x 256 picture, CLUT (128, 510): its left half from page (896, 0) (rows 32-255 only: above them
 * the clear colour is the sky's) and its right half from page (0, 256), two sprites at frame row 4, moved by the
 * camera: the right half's sprite is at frame column 110 with the camera at its right limit and at 200 at the other
 * end of this screen (then the room cuts to the island).  So the picture's right edge is 46 columns past the 4:3
 * frame at the right limit: 7 columns short of a 16:9 frame, 74 of a 21:9 one.  Only that end needs content: lk_take()
 * fills the frame's right strip from the picture's columns and the extension (made once), wherever the camera is,
 * and leaves the left strip alone (port_ws_backdrop_side).
 *
 * Measured on the picture (rows counted from frame row 16, as the strips are: picture row - 12), at its right edge:
 *   - sky to row 49: a gradient of flat tones without grain, level; on rows 39-49 the top of a cloud that rises
 *     towards the edge and is cut by it (and by the grass under it);
 *   - the cliff's grass from row 50 (its blades' tips) to 64, about level over the last 60 columns;
 *   - the cliff's rock from there to its foot on row 200: a mottled face without strata, lit evenly; the cave is 70
 *     columns in;
 *   - the sand from row 201; the foot's line comes down towards the edge, 0.2 rows per column over the last 40.
 * The edge cuts the cliff's face, which goes on: the same grass line, the same rock, the foot further down its line.
 *
 * The extension is made with the Singing Rock's tools (ws_backdrop_mermaid.inc: mb_band, a band's tone at the edge
 * carried on level plus its detail quilted from the picture's own texels next to the edge, never mirrored; the
 * edge's own columns go on first and the join's tone fades out):
 *   - the sky as a smooth band (the cloud's top goes on level behind the grass);
 *   - the grass and the rock as one band whose windows are the face's columns right of the cave, rows 50 to 186
 *     (above the foot's line there), each near its own row, so the grass stays the top fringe;
 *   - the sand as a band from its own rows under the cliff;
 *   - the rock comes down to the foot's line, 0.2 rows per column lower, the sand under it (one texel of coverage).
 * Every colour is then the nearest of the picture's CLUT.  Integers only; a texel depends on its place, not on the
 * strip's width. */
#define LK_HM 1227                                        /* the ROOMS row's mark (no picture has this height) */
#define LK_W 512
#define LK_G 50                                           /* the grass' first row at the edge */
#define LK_F 201                                          /* the sand's first row at the edge */
#define LK_X0 110                                         /* the right half's frame column, camera at its right limit */
#define LK_X1 200                                         /* and at the other end of the screen */
static void gen_lk(int pad) { (void)pad; }               /* (the table's gen: lk_take makes the strip) */
static const MbBand LK_B[3] = {
    { 0, LK_G, MC_ANY, 440, LK_W, 0, LK_G, 0, 0, 3, 1, 3, 24, 24, 12, 8, 12, 4, 4, 2, 16, 0, 0, 0, 0 },           /* sky */
    { LK_G, PH, MC_ANY, 446, LK_W, LK_G, 187, LK_F - 1, 0, 40, 16, 16, 18, 14, 8, 4, 6, 1, 1, 0, 16, 1, 0, 0, 0 }, /* grass, rock */
    { LK_F, PH, MC_ANY, 420, LK_W, LK_F + 1, PH, 0, 0, 8, 2, 4, 12, 10, 6, 4, 8, 1, 2, 0, 16, 1, 0, 0, 0 },      /* sand */
};
static void lk_gen(void)
{
    Rgb *rock = malloc((size_t)(PH - LK_F) * mm_cw * sizeof *rock);
    mb_band(1, &LK_B[0], 0);
    mb_band(1, &LK_B[1], 1);
    if (rock) for (int y = LK_F; y < PH; y++) for (int X = 1; X <= mm_cw; X++) rock[(size_t)(y - LK_F) * mm_cw + X - 1] = *mm_at(1, X, y);
    mb_band(1, &LK_B[2], 2);
    if (rock) for (int y = LK_F; y < PH; y++)
        for (int X = 1; X <= mm_cw; X++) {
            int cov = clampi(LK_F * 16 + X * 16 / 5 - y * 16, 0, 16);       /* the rock down to the foot's line */
            if (cov) mm_put(1, X, y, mix(*mm_at(1, X, y), rock[(size_t)(y - LK_F) * mm_cw + X - 1], cov * 16));
        }
    free(rock);
}

#ifndef WS_BACKDROP_MAIN
static u16 *lk_ext, *lk_buf; static int lk_pad = -1, lk_w; static u32 lk_sum;
int port_ws_backdrop_side = 3;                            /* the strips shims/gpu.c lays: 1 the left one, 2 the right one */
static u16 lk_tex(const u16 *vram, int x, int y)          /* the picture's column x on strip row y */
{
    return x < 256 ? (y + 12 >= 32 ? tex8(vram, 896, x, y + 12, 128, 510) : tex8(vram, 0, 0, 256 + y + 12, 128, 510))
                   : tex8(vram, 0, x - 256, 256 + y + 12, 128, 510);
}
static u32 lk_check(const u16 *vram)                      /* the right half's page and the CLUT, sampled */
{
    u32 h = 2166136261u;
    for (int i = 0; i < 256; i++) h = (h ^ vram[(size_t)510 * 1024 + 128 + i]) * 16777619u;
    for (int y = 259; y < 512; y += 8) for (int x = 0; x < 128; x++) h = (h ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return h;
}
static int lk_build(const u16 *vram, int w)
{
    mm_w = LK_W; mm_ph = 0; mm_pad = -1;                 /* (the Singing Rock's picture is no longer in mm_src) */
    for (int i = 0; i < 256; i++) mm_clut[i] = vram[(size_t)510 * 1024 + 128 + i];
    memset(mm_qh, 0, sizeof mm_qh);
    for (int y = 0; y < PH; y++) for (int x = 0; x < LK_W; x++) mm_src[0][y][x] = lk_tex(vram, x, y);
    free(lk_ext); lk_ext = calloc((size_t)PH * w, 2);
    if (!lk_ext || !mm_canvas(w)) { free(lk_ext); lk_ext = 0; return 0; }
    lk_gen();
    for (int y = 0; y < PH; y++) for (int X = 1; X <= w; X++) lk_ext[(size_t)y * w + X - 1] = mm_quant(*mm_at(1, X, y));
    mm_canvas_free();
    return 1;
}
/* the picture's right half (page (0, 256), CLUT (128, 510)) at frame column x: the right strip when the wide frame
   looks past the picture's right edge */
static const u16 *lk_take(const u16 *vram, int pad, int x, int *rows)
{
    int w = pad - (LK_X0 + 256 - 320), over = 320 + pad - (x + 256);   /* the columns ever needed; needed now */
    u32 s;
    if (w <= 0 || x < LK_X0 || x > LK_X1 || over <= 0) return 0;
    s = lk_check(vram);
    if (!lk_buf || lk_pad != pad || s != lk_sum) {
        Rgb t = rgb15(tex8(vram, 0, 4, 256 + 16, 128, 510));               /* (the sky in the top left corner: this picture) */
        lk_pad = pad; lk_sum = s; lk_w = w;
        free(lk_buf); lk_buf = calloc((size_t)PH * 2 * pad, 2);
        if (t.b < t.r + 24 || !lk_buf || !lk_build(vram, w)) { free(lk_ext); lk_ext = 0; }
    }
    if (!lk_ext || !lk_buf) return 0;
    for (int j = 0; j < PH; j++) {
        u16 *row = lk_buf + (size_t)j * 2 * pad + pad;
        for (int c = 0; c < pad; c++) {
            int f = 320 + c - (x - 256);                  /* the picture's column at the right strip's column c */
            row[c] = f < LK_W ? lk_tex(vram, f, j) : f - LK_W < lk_w ? lk_ext[(size_t)j * lk_w + f - LK_W] : 0;
        }
    }
    port_ws_backdrop_dy = 12; port_ws_backdrop_side = 2;
    *rows = PH;
    return lk_buf;
}
#endif
