/* runtime/ws_backdrop.c: AREA09 s1, the island screen of the Lake (the base of the Familiar Looking Mansion): both
 * ends of its picture (included there; docs/PORT.md "More rooms with picture strips").
 *
 * The room is one sprite world moved by the camera (CAMERA_SCROLL 160..1074): the beach picture at world columns
 * 768..1279 (ws_backdrop_lake.inc) and, at 0..639, the island: a mosaic of ten sprites of CLUT (128, 511), 640 x 448
 * (IS_P; rows 448 on of its pages are the shore's wave tiles), drawn every frame.  Walking left from the beach the
 * room cuts to it (updateAreaScrollB: the camera is held at 480 or less): the mushroom of rock with the mansion on
 * top, its trunk with the two cave doors on the sand, and the sea.  The picture is exactly two 4:3 frames wide, so
 * each end of the screen is a picture ending: with the camera at 160 the left strip is the clear colour (the sky's)
 * over the whole height, with the camera at 480 (swimming in from the beach) the right one.  The camera's height
 * is fixed while playing (picture rows 220..443 on frame rows 16..239; a jump does not move it); only the first
 * visit's pan (the camera rises 342 rows to show the mansion) shows the rows above.
 *
 * The game itself draws an object past the picture's left edge: a ledge in the foreground, quads of page (256, 0)
 * from frame column -53 with the camera at its left limit (grass on top, rock under it, its underside a slope that
 * comes down to the left; it moves about 1.5 times as fast as the picture): an object sprite, nearer than the
 * picture, that at 4:3 only comes in from the frame's left edge while the camera pans up.  With the strips on its
 * quads are drawn a strip's width further left (is_fg_face, port_ws_island_fg in ws_backdrop.c, called from
 * drawObjectSprite), so it keeps that place against the frame's edge and does not hang in the corner while playing.
 *
 * Measured on the picture, rows counted from 220 (the frame's): at its left edge the trunk's wall, lit evenly,
 * without strata, down to its foot on row 170, level over the first 16 columns; sand from 171.  The edge cuts the
 * wall, which goes on.  At its right edge the sky's gradient to row 77, the top of a cloud that rises to a lump 13
 * columns in and comes down to the edge, its body and the haze over the horizon to row 133, the sea from 134.
 *
 * The extensions are made once with the Singing Rock's tools (ws_backdrop_mermaid.inc: mb_band, a band's tone at
 * the edge carried on level plus its detail quilted from the picture's own texels, never mirrored):
 *   - left: the wall as one band whose windows are the wall over the doors (columns 0-199, rows 12-99); the sand
 *     is the texels of its own rows at the edge (is_sand); the ledge at the top of the play view is a band of its
 *     own (IS_LD, laid over the wall's);
 *   - right: the sky as a smooth band, the cloud as a band of its own texels whose top goes on in lumps (arcs, as
 *     the picture's: is_lump), the sea from its own rows.
 * Every colour is then the nearest of the picture's CLUT.  Integers only; a texel depends on its place, not on the
 * strip's width.  The rows above 220: is_gen_up (left) and the clear colour, the picture's top right texel (right). */
#define IS_HM 1228 /* the ROOMS row's mark (no picture has this height) */
#define IS_W 640
#define IS_H 448
#define IS_Y0 220 /* the picture's row on frame row 16 while playing */
#define IS_F 171  /* the sand's first row at the left edge (from IS_Y0) */
#define IS_CT 78  /* the cloud's top at the right edge */
#define IS_HZ 134 /* the sea's first row */
static const Piece IS_P[10] = { { 128, 0, 0, 256, 256, 0, 0 },       { 256, 0, 0, 24, 120, 256, 0 },
                                { 256, 0, 120, 88, 136, 256, 120 },  { 256, 24, 0, 128, 120, 512, 328 },
                                { 256, 88, 120, 64, 40, 512, 216 },  { 256, 88, 160, 64, 40, 576, 216 },
                                { 256, 88, 216, 168, 40, 344, 216 }, { 384, 0, 0, 256, 256, 0, 256 },
                                { 512, 0, 0, 256, 256, 256, 256 },   { 768, 0, 184, 128, 72, 512, 256 } };
static void gen_is(int pad) { (void)pad; } /* (the table's gen: is_take makes the strips) */
/* the foreground ledge's two quads (an object sprite): page (256, 0) 8 bpp, the picture's CLUT, texels (152..247,
   0..151) (the door beside them is rows 152 on) */
static int is_fg_face(int tpage, int clut, int uv)
{
    return clut == 0x7fc8 && (tpage & 0x19f) == 0x84 && (uv & 255) >= 152 && (uv >> 8 & 255) < 152;
}
/* the sand past the left edge.  The picture's sand is two CLUT tones (three at the wall's foot) dithered without a
   pattern, lighter down the rows and level along them near the edge; the wall's foot is level over the first 13
   columns, with one row of shade under it.  So a texel is one of its own row's (or of the row above or under it)
   among the picture's first columns, left of the rock on the sand and of its shade: each row keeps its tones in
   the picture's proportions, and nothing is repeated as a block. */
static void is_sand(void)
{
    for (int y = IS_F - 1; y < PH; y++)
        for (int X = 1; X <= mm_cw; X++) {
            u32 rnd = hash3(X, y, 0x15a);
            int ncols = y < IS_F + 3 ? 5 : 10,
                src_y = y < IS_F + 5 ? y : clampi(y + (int)((rnd >> 16) % 3) - 1, IS_F + 5, PH - 1);
            mm_put(0, X, y, mm_px((int)((rnd >> 4) % (u32)ncols), src_y));
        }
}
static const MbBand IS_L[1] = {
    { 0, PH, MC_ANY, 0, 200, 12, 100, IS_F - 1, 0, 40, 16, 16, 18, 14, 8, 4, 6, 1, 1, 0, 16, 1, 0, 0, 0 },         /* the wall */
};
static const MbBand IS_R[3] = {
    { 0, IS_CT + 14, MC_ANY, 600, IS_W, 0, 60, IS_CT - 2, 0, 3, 1, 3, 24, 24, 12, 8, 12, 4, 4, 2, 16, 0, 0, 0, 0 },       /* sky */
    { IS_CT, IS_HZ, MC_ANY, 544, IS_W, IS_CT + 2, IS_HZ, 0, 0, 8, 2, 6, 20, 16, 8, 4, 8, 2, 2, 1, 16, 1, 0, 0, 0 }, /* cloud */
    { IS_HZ, PH, MC_ANY, 400, IS_W, IS_HZ, PH, 0, 0, 20, 2, 8, 12, 11, 6, 4, 8, 1, 2, 0, 16, 1, 0, 0, 0 },         /* sea */
};
/* the cloud's top past the right edge, rows (x 16) above IS_CT at strip column X: lumps, arcs of an ellipse each,
   the first starting at the edge (where the picture's last lump has come down) */
static int is_lump(int X)
{
    /* centre, half width, height */
    static const signed char LUMPS[6][3] = { { 13, 12, 5 }, { 34, 11, 7 },  { 57, 13, 4 },
                                             { 80, 12, 6 }, { 103, 12, 5 }, { 125, 11, 7 } };
    int x = (X - 1) % 136 + 1,
        highest = 0;
    for (int i = 0; i < 6; i++) {
        int d = x - LUMPS[i][0],
            r = LUMPS[i][1],
            height;
        if (d <= -r || d >= r) continue;
        height = LUMPS[i][2] * isqrt_i((r * r - d * d) * 256) / r; /* height x 16 */
        if (height > highest) highest = height;
    }
    return highest;
}
/* both ends' extensions: on the left the wall and the sand; on the right the sky, the cloud (its top raised in
   lumps, is_lump) and the sea */
static void is_gen(void)
{
    Rgb *sky = malloc((size_t)(IS_CT + 14) * mm_cw * sizeof *sky);
    mb_band(0, &IS_L[0], 0);
    is_sand();
    mb_band(1, &IS_R[0], 2);
    if (sky)
        for (int y = 0; y < IS_CT + 14; y++)
            for (int X = 1; X <= mm_cw; X++) sky[(size_t)y * mm_cw + X - 1] = *mm_at(1, X, y);
    mb_band(1, &IS_R[1], 3);
    if (sky) {
        Rgb *col = malloc((size_t)PH * sizeof *col);
        for (int X = 1; X <= mm_cw && col; X++) {
            int lump = is_lump(X); /* the lump's height here, x 16 */
            for (int y = 0; y < IS_HZ; y++) col[y] = *mm_at(1, X, y);
            for (int y = IS_CT - 8; y < IS_CT + 14; y++) {
                /* the cloud's rows moved up by the lump, less and less down to 32 rows under its top */
                int top16 = IS_CT * 16 - lump,
                    cov = clampi(y * 16 + 16 - top16, 0, 16);
                int shift16 = lump * clampi(IS_CT + 32 - y, 0, 32) / 32,
                    src_y = clampi(y + (shift16 + 8) / 16, IS_CT, IS_HZ - 1);
                Rgb cloud_px = col[src_y],
                    behind_px = sky[(size_t)y * mm_cw + X - 1];
                mm_put(1, X, y, cov >= 16 ? cloud_px : mix(behind_px, cloud_px, cov * 16));
            }
            for (int y = IS_CT + 14; y < IS_CT + 32; y++) {
                int shift16 = lump * clampi(IS_CT + 32 - y, 0, 32) / 32;
                mm_put(1, X, y, col[clampi(y + (shift16 + 8) / 16, IS_CT, IS_HZ - 1)]);
            }
        }
        free(col);
    }
    free(sky);
    mb_band(1, &IS_R[2], 4);
}

/* the rows above (seen only while the first visit's pan rises), rows counted from the picture's: at the left edge the
   sky to row 17, the crown of the tree beside the mansion and the bush at its foot to 125 (the trunk is 18 columns
   in, the crown reaches 32 columns right of it), the cap's grass and its rock to 186, the grass of the ledge under
   it to 218, the trunk's wall from there.  The cap goes on level, a band of its own rows (the ledge, rows 186 on, is
   IS_LD's, made with the play view's rows); the crown
   closes round 16 columns out (an ellipse about the trunk, its outline in lumps like the leaves' clusters), the bush
   within 8, and past them the sky is the clear colour. */
#define IS_UT 126 /* the cap's grass at the left edge: its first row */
static const MbBand IS_U[2] = {
    { 16, IS_UT + 2, MC_ANY, 0, 36, 18, 100, 0, 0, 6, 3, 6, 12, 10, 6, 4, 8, 1, 1, 0, 16, 1, 0, 0, 0 },           /* the crown */
    { IS_UT - 2, 190, MC_ANY, 0, 200, IS_UT - 2, 188, 0, 0, 24, 2, 4, 18, 14, 8, 4, 4, 1, 1, 0, 16, 1, 0, 0, 0 }, /* the cap */
};
static void is_gen_up(Rgb clear)
{
    mb_band(0, &IS_U[0], 0);
    for (int y = 0; y < IS_UT; y++) {
        /* the crown's reach, x 16 */
        int d = y - 62,
            reach16 = d > -46 && d < 46 ? 14 * isqrt_i((46 * 46 - d * d) * 256) / 46 : 0;
        /* the bush's */
        int e = IS_UT - y,
            bush16 = e < 26 ? 8 * isqrt_i((26 * 26 - e * e) * 256) / 26 : 0;
        if (bush16 > reach16) reach16 = bush16;
        if (reach16) reach16 += is_lump(y + 3) / 3;
        for (int X = 1; X <= mm_cw; X++) {
            int cov = y < 16 ? 0 : clampi(reach16 - X * 16 + 16, 0, 16);
            mm_put(0, X, y, cov >= 16 ? *mm_at(0, X, y) : cov ? mix(clear, *mm_at(0, X, y), cov * 16) : clear);
        }
    }
    mb_band(0, &IS_U[1], 1);
}

/* the ledge under the cap at the left edge (picture rows 186..264: its grass, the fringe the grass hangs over the
   rock, the lit rock under it and its shaded underside), one band of its own rows in a canvas of the picture's rows
   from IS_LY, so the fringe and the rock under it go on as one piece on both sides of the play view's top row */
#define IS_LY 150
#define IS_LA 186
#define IS_LB 265
static const MbBand IS_LD = { IS_LA - IS_LY, IS_LB - IS_LY, MC_ANY, 0, 64, IS_LA - IS_LY, IS_LB - IS_LY,
                              0, 0, 12, 2, 4, 16, 14, 6, 4, 2, 1, 1, 0, 16, 1, 0, 0, 0 };

#ifndef WS_BACKDROP_MAIN
static u16 *is_ext[2], *is_buf;
static int is_pad = -1,
           is_ext_w;
static u32 is_sum;
static u16 is_clear;
static u16 (*is_pic)[IS_W];
static int is_up;
static u32 is_check(const u16 *vram) /* the CLUT and the pages, sampled */
{
    u32 fnv = 2166136261u;
    for (int i = 0; i < 256; i++) fnv = (fnv ^ vram[(size_t)511 * 1024 + 128 + i]) * 16777619u;
    for (int y = 3; y < 256; y += 8)
        for (int x = 128; x < 640; x++) fnv = (fnv ^ vram[(size_t)y * 1024 + x]) * 16777619u;
    return fnv;
}
/* the mosaic read from VRAM into is_pic, both extensions made from it, then the left one's ledge rows (IS_LD) */
static int is_build(const u16 *vram, int w)
{
    if (!ws_need_is_build()) return 0;
    if (!is_pic) is_pic = malloc(sizeof(u16[IS_H][IS_W]));
    for (int side = 0; side < 2; side++) {
        free(is_ext[side]);
        is_ext[side] = calloc((size_t)IS_H * w, 2);
    }
    if (!is_pic || !is_ext[0] || !is_ext[1]) return 0;
    memset(is_pic, 0, sizeof(u16[IS_H][IS_W]));
    for (int i = 0; i < 10; i++) {
        const Piece *p = &IS_P[i];
        for (int y = 0; y < p->h && p->y + y < IS_H; y++)
            for (int x = 0; x < p->w; x++) is_pic[p->y + y][p->x + x] = tex8(vram, p->tx, p->u + x, p->v + y, 128, 511);
    }
    is_clear = is_pic[0][279];
    /* (the Singing Rock's picture is no longer in mm_src) */
    mm_w = IS_W;
    mm_ph = 0;
    mm_pad = -1;
    lk_pad = -1; /* (nor the beach's) */
    for (int i = 0; i < 256; i++) mm_clut[i] = vram[(size_t)511 * 1024 + 128 + i];
    memset(mm_qh, 0, (sizeof *mm_qh * (32768)));
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < IS_W; x++) mm_src[0][y][x] = is_pic[IS_Y0 + y][x];
    if (!mm_canvas(w)) return 0;
    is_gen();
    for (int side = 0; side < 2; side++)
        for (int pic_row = 0; pic_row < IS_H; pic_row++)
            for (int X = 1; X <= w; X++) {
                int y = pic_row - IS_Y0;
                is_ext[side][(size_t)pic_row * w + X - 1] =
                    y < 0 ? is_clear : mm_quant(*mm_at(side, X, y < PH ? y : PH - 1));
            }
    mm_canvas_free();
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < IS_W; x++) mm_src[0][y][x] = is_pic[IS_LY + y][x];
    if (!mm_canvas(w)) return 0;
    mb_band(0, &IS_LD, 0);
    for (int pic_row = IS_LA; pic_row < IS_LB; pic_row++)
        for (int X = 1; X <= w; X++) is_ext[0][(size_t)pic_row * w + X - 1] = mm_quant(*mm_at(0, X, pic_row - IS_LY));
    mm_canvas_free();
    is_up = 0;
    return 1;
}
/* the left strip's rows above (made when the camera first rises) */
static void is_build_up(int w)
{
    if (!ws_need_is_build_up()) return;
    is_up = 1;
    mm_w = IS_W;
    mm_ph = 0;
    mm_pad = -1;
    lk_pad = -1;
    for (int y = 0; y < PH; y++)
        for (int x = 0; x < IS_W; x++) mm_src[0][y][x] = is_pic[y][x];
    if (!mm_canvas(w)) return;
    is_gen_up(rgb15(is_clear));
    for (int pic_row = 0; pic_row < IS_LA; pic_row++)
        for (int X = 1; X <= w; X++) is_ext[0][(size_t)pic_row * w + X - 1] = mm_quant(*mm_at(0, X, pic_row));
    mm_canvas_free();
}
/* is_build / is_build_up through the cache (ws_cache), keyed by the whole mosaic and the CLUT (is_pic is no heap
   table: nothing of the generator's is needed when the strips are in the cache) */
static u32 is_key;
static int is_cached(const u16 *vram, int w)
{
    WsCPart parts[2];
    size_t size = (size_t)IS_H * w * 2;
    if (!is_pic) is_pic = malloc(sizeof(u16[IS_H][IS_W]));
    for (int side = 0; side < 2; side++) {
        free(is_ext[side]);
        is_ext[side] = calloc((size_t)IS_H * w, 2);
    }
    if (is_pic && is_ext[0] && is_ext[1]) {
        memset(is_pic, 0, sizeof(u16[IS_H][IS_W]));
        for (int i = 0; i < 10; i++) {
            const Piece *piece = &IS_P[i];
            for (int y = 0; y < piece->h && piece->y + y < IS_H; y++)
                for (int x = 0; x < piece->w; x++)
                    is_pic[piece->y + y][piece->x + x] = tex8(vram, piece->tx, piece->u + x, piece->v + y, 128, 511);
        }
        is_key = wsc_hash(wsc_hash(is_sum, &vram[(size_t)511 * 1024 + 128], 512), is_pic, sizeof(u16[IS_H][IS_W]));
        parts[0].p = is_ext[0];
        parts[1].p = is_ext[1];
        parts[0].n = parts[1].n = size;
        if (wsc_get("is", 9, 1, is_pad, is_key, (u32)w, parts, 2)) {
            is_clear = is_pic[0][279];
            mm_w = IS_W;
            mm_ph = 0;
            mm_pad = -1;
            lk_pad = -1;
            is_up = 0;
            return 1;
        }
    }
    if (!is_build(vram, w)) return 0;
    parts[0].p = is_ext[0];
    parts[1].p = is_ext[1];
    parts[0].n = parts[1].n = size;
    wsc_put("is", 9, 1, is_pad, is_key, (u32)w, parts, 2);
    return 1;
}
static void is_cached_up(int w)
{
    WsCPart entry = { is_ext[0], (size_t)IS_LA * w * 2 };
    if (wsc_get("isu", 9, 1, is_pad, is_key, (u32)w, &entry, 1)) {
        is_up = 1;
        mm_w = IS_W;
        mm_ph = 0;
        mm_pad = -1;
        lk_pad = -1;
        return;
    }
    is_build_up(w);
    entry.p = is_ext[0];
    wsc_put("isu", 9, 1, is_pad, is_key, (u32)w, &entry, 1);
}
/* the beach picture's right half (the first sprite of the room's backdrop, drawn every frame) at frame (x, y) while
   the camera is on the island: the strips' rows for frame rows 16-239 */
static const u16 *is_take(const u16 *vram, int pad, int x, int y, int *rows)
{
    /* the frame's first world column; the picture's row 0 on the frame */
    int first_col = (1024 - x) & 2047,
        pic_top = y - 192,
        w = pad < MQ_SW - 56 ? pad : MQ_SW - 56;
    int need_left = first_col < pad,
        need_right = first_col + 320 + pad > IS_W;
    u32 check;
    if (first_col > IS_W - 320 || (!need_left && !need_right) || pic_top > 16 || pic_top + IS_H < 240 - 4) return 0;
    check = is_check(vram);
    if (!is_buf || is_pad != pad || check != is_sum) {
        is_pad = pad;
        is_sum = check;
        is_ext_w = w;
        free(is_buf);
        is_buf = calloc((size_t)PH * 2 * pad, 2);
        if (!is_buf || !is_cached(vram, w)) {
            free(is_ext[0]);
            free(is_ext[1]);
            is_ext[0] = is_ext[1] = 0;
        } else {
            /* (sky top right, sand bottom left: this picture) */
            Rgb sky_px = rgb15(is_pic[IS_Y0 + 4][IS_W - 2]),
                sand_px = rgb15(is_pic[IS_H - 8][4]);
            if (sky_px.b < sky_px.r + 64 || sand_px.r < sand_px.b + 16) {
                free(is_ext[0]);
                free(is_ext[1]);
                is_ext[0] = is_ext[1] = 0;
            }
        }
    }
    if (!is_ext[0] || !is_ext[1] || !is_buf) return 0;
    if (need_left && pic_top > 16 - IS_Y0 && !is_up) is_cached_up(is_ext_w);
    for (int j = 0; j < PH; j++) {
        u16 *row = is_buf + (size_t)j * 2 * pad;
        int pic_row = j + 16 - pic_top;
        for (int c = 0; c < pad; c++) {
            /* past the left edge; past the right one */
            int left_x = pad - c - first_col,
                right_x = first_col + 320 + c - (IS_W - 1);
            row[c] = left_x >= 1 && left_x <= is_ext_w && pic_row >= 0 && pic_row < IS_H
                         ? is_ext[0][(size_t)pic_row * is_ext_w + left_x - 1]
                         : is_clear;
            row[pad + c] = right_x >= 1 && right_x <= is_ext_w && pic_row >= 0 && pic_row < IS_H
                               ? is_ext[1][(size_t)pic_row * is_ext_w + right_x - 1]
                               : is_clear;
        }
    }
    port_ws_backdrop_dy = 16 - y;
    port_ws_backdrop_side = (need_left ? 1 : 0) | (need_right ? 2 : 0);
    *rows = PH;
    return is_buf;
}
#endif
