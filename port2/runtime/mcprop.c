/* Proportional spacing for the place names of the memory card save list (on by default, --memcard-mono turns it off, --memcard-prop GAP,SPACE tunes it; port/fixes.txt
 * hooks in memCardDrawFileLocation and memCardDrawString).
 *
 * The retail memCardDrawString advances every glyph of MEMCARD_FONT by its cell width (8 px), so a place name is
 * monospaced and the longest Spanish ones do not fit the row (230 px from x 64 to the frame's inner edge at x 293).
 * In the 8 x 8 cells a wide letter fills columns 1-7 (ink, then its shadow), a narrow one sits in the middle ('i' in
 * columns 3-4, 'l' in 2-4).  With the option on (the default), a glyph of a place name is drawn from its first opaque column and
 * advances by its opaque width (first to last opaque texel column of the glyph as it is in VRAM: ink plus shadow, read
 * from the user's own font upload at run time, so no glyph data in the port) plus port_mc_prop_gap; a glyph with no
 * opaque texel (the space) advances port_mc_prop_space.  Only memCardDrawFileLocation turns it on, so the messages,
 * the footer and the empty-slot label keep the retail spacing. */

extern const unsigned short *port_gpu_vram(int x, int y, int w, int h);

int port_mc_prop = 1;           /* on by default; --memcard-mono: off (retail 8-px cells) */
int port_mc_prop_gap = 1;       /* px after a glyph's shadow */
int port_mc_prop_space = 4;     /* advance of an empty glyph (the space) */
int port_mc_prop_on;            /* set around memCardDrawFileLocation's memCardDrawString call */
int port_mc_last_x;             /* start of the last advancing glyph's cell (a flag glyph is placed from it) */
int port_mc_left;               /* first opaque column of the glyph port_mc_advance measured (drawn that much left) */

static struct { unsigned key; signed char l, r; } cache[256];

/* first opaque column (*l) and last + 1 (return value; 0 = empty) of the w x h glyph at (u, v) of texture page
   tpage (4 / 8 bpp), CLUT clut */
static int ink_span(int tpage, int clut, int u, int v, int w, int h, int *l) {
    int tx = (tpage & 15) * 64, ty = ((tpage >> 4) & 1) * 256, depth = (tpage >> 7) & 3;
    int cx = (clut & 0x3F) * 16, cy = (clut >> 6) & 0x1FF;
    *l = 0;
    if (depth > 1 || w <= 0 || h <= 0) return w;
    int sh = depth == 0 ? 2 : 1, vw = ((u + w - 1) >> sh) - (u >> sh) + 1;
    const unsigned short *vram = port_gpu_vram(tx + (u >> sh), ty + v, vw, h);
    port_gpu_vram(cx, cy, depth == 0 ? 16 : 256, 1);
    const unsigned short *pal = vram + cy * 1024 + cx;
    int left = w, right = 0;
    for (int j = 0; j < h; j++) {
        const unsigned short *row = vram + ((ty + v + j) & 511) * 1024;
        for (int i = 0; i < w; i++) {
            int uu = u + i, idx;
            unsigned short word = row[(tx + (uu >> sh)) & 1023];
            idx = depth == 0 ? (word >> ((uu & 3) * 4)) & 15 : (word >> ((uu & 1) * 8)) & 255;
            if (pal[idx]) { if (i < left) left = i; if (i + 1 > right) right = i + 1; }
        }
    }
    *l = right ? left : 0;
    return right;
}

/* the advance of glyph c (cell w x h at u, v) under the option; port_mc_left = how far left to draw it */
int port_mc_advance(int tpage, int clut, int u, int v, int w, int h, int c) {
    unsigned key = ((unsigned)tpage << 22) ^ ((unsigned)clut << 6) ^ ((unsigned)u << 14) ^ ((unsigned)v << 4) ^ (unsigned)w ^ ((unsigned)h << 24) ^ 1u;
    int l, r;
    c &= 255;
    port_mc_left = 0;
    if (w > 8) return w;                    /* the 12 x 12 button icons keep their cell */
    if (cache[c].key == key) { l = cache[c].l; r = cache[c].r; }
    else {
        r = ink_span(tpage, clut, u, v, w, h, &l);
        if (r) { cache[c].key = key; cache[c].l = (signed char)l; cache[c].r = (signed char)r; }   /* an empty read is never cached */
    }
    if (r <= 0) return port_mc_prop_space;
    port_mc_left = l;
    return r - l + port_mc_prop_gap;
}
