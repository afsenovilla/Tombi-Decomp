/* Second screen panel: the top-down map (included by panel_side.inc).  In the top-down sections (AREA05 s0/s2 Baccus
 * Village, AREA08 s0/s2 Baccus Lake, AREA19 s2, and AREA02 / AREA19 s0 of the I0NFO engine) the game keeps the whole
 * map picture in RAM and streams it to VRAM in 32 x 32 blocks (docs/CIVILIZATION.md "Map picture"): the section's
 * kind 0x8F block, APD_DATA[15] (scratch 0x1F800394), a plain 8 bpp TIM (CLUT at +0x14, the image block after it),
 * read from the user's disc by the game's loader.  The panel shows that picture whole (zoomed out: the mean of n x n
 * blocks; in: each picture pixel an m x m block);
 * world (x, y) is picture pixel (x + 128, y - 40), the origin of the cell map (APD_DATA[14], 8 x 8 px cells:
 * topdownCollideMapTile), whose size the picture must match. */
static const struct { u8 area, sec; } td_secs[] = { {2, 0}, {5, 0}, {5, 2}, {8, 0}, {8, 2}, {19, 0}, {19, 2} };
static int td_section(int a, int s) { for (unsigned i = 0; i < sizeof td_secs / sizeof td_secs[0]; i++) if (td_secs[i].area == a && td_secs[i].sec == s) return 1; return 0; }
static u32 *td_img; static int td_w, td_h, td_x0, td_y0, td_key = -1, td_model, td_ok, td_since, td_next;
static const u8 *td_ptr(u32 off) {
    u32 a = SPAD32(off);
    const u8 *d = (const u8 *)(uintptr_t)a;
    return a && port_is_ram_ptr((void *)d) ? d : NULL;
}
static int td_read(void) {
    const u8 *t = td_ptr(0x394), *m = td_ptr(0x390);
    if (!t || !m || rd32(t, 0) != 0x10 || (rd32(t, 4) & 0xF) != 9) return 0;
    unsigned clen = rd32(t, 8), p = 8 + clen;
    if (clen != 0x20C) return 0;
    int w = (u16)rd16(t, p + 8) * 2, h = (u16)rd16(t, p + 10), cw = (u16)rd16(m, 0), ch = (u16)rd16(m, 2);
    if (w < 64 || h < 64 || w > 2048 || h > 2048 || !port_is_ram_ptr((void *)(t + p + 12 + w * h - 1))) return 0;
    if (cw * 8 > w + 32 || ch * 8 > h + 32 || cw * 8 < w - 64 || ch * 8 < h - 64) return 0;   /* the picture of this cell map */
    free(td_img); td_img = malloc((size_t)w * h * 4);
    u32 pal[256];
    for (int i = 0; i < 256; i++) { u16 c = (u16)rd16(t, 0x14 + 2 * i); pal[i] = c ? c15(c) : 0xFF000000u; }
    const u8 *px = t + p + 12;
    for (int i = 0; i < w * h; i++) td_img[i] = pal[px[i]];
    td_w = w; td_h = h; td_x0 = -128; td_y0 = 40; td_model++;
    port_printf("[panel] top-down map: area %d section %d, picture %d x %d, cells %d x %d\n", G_AREA, G_SECTION, w, h, cw, ch);
    return 1;
}
static int td_update(int area, int sec) {
    int key = area * 32 + sec;
    if (key != td_key) { td_key = key; td_ok = 0; td_since = port_frame; }
    if (!td_ok && port_frame - td_since > 30 && port_frame >= td_next) { td_next = port_frame + 16; td_ok = td_read(); }
    return td_ok;
}
/* the picture into dst: world (wx0, wy0) at the top left, upp world units (= picture pixels) per output pixel; upp is
   a whole number n (each output pixel the mean of an n x n block) or 1 / m (each picture pixel an m x m block) */
static void td_render(u32 *dst, int vw, int vh, float wx0, float wy0, float upp) {
    int n = upp >= 1 ? (int)(upp + .5f) : 1, m = upp < 1 ? (int)(1 / upp + .5f) : 1;
    if (n > 6) n = 6;
    int bx = (int)(wx0 * m + (wx0 < 0 ? -.5f : .5f)), by = (int)(wy0 * m + (wy0 < 0 ? -.5f : .5f));   /* in 1/m picture pixels */
    for (int y = 0; y < vh; y++) for (int x = 0; x < vw; x++) {
        int ux = n > 1 ? bx + x * n : bx + x, uy = n > 1 ? by + y * n : by + y;
        int sx = (ux >= 0 ? ux / m : -((-ux + m - 1) / m)) - td_x0, sy = (uy >= 0 ? uy / m : -((-uy + m - 1) / m)) - td_y0, r = 0, g = 0, b = 0, k = 0;
        for (int j = 0; j < n; j++) for (int i = 0; i < n; i++) {
            int X = sx + i, Y = sy + j;
            if (X < 0 || Y < 0 || X >= td_w || Y >= td_h) continue;
            u32 c = td_img[Y * td_w + X]; r += c & 255; g += c >> 8 & 255; b += c >> 16 & 255; k++;
        }
        dst[y * vw + x] = k ? 0xFF000000u | (u32)(b / k) << 16 | (u32)(g / k) << 8 | (u32)(r / k) : RGB(24, 16, 32);
    }
}
