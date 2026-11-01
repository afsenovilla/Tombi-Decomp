/* Spanish improved translation (patches/es_mejorada.tombipatch.json): the Select sub-screens' "Elmto." tab and the
 * "Usar elmto" leaf button say "Objetos" / "Usar objeto", like the pause menu entry the patch renames ("Elmto" ->
 * "Objetos").  Both labels are drawn into the sub-screen texture page (4 bpp, page x 64, y 0, uploaded with the
 * D0SPSUB* overlays), so they are rewritten in VRAM right after that upload, from the texture itself:
 *   - the tab (64x24 at u 0x98, v 0x38): the old letters are painted over with the plank's own grain (each text texel
 *     takes the nearest wood texel to its left on the same row), then "Objetos" is stamped in the tab font: e, t, o,
 *     s cut from the "Evento" / "Estado" tabs of the same page, b = their d mirrored, and the capital O and the j
 *     (the font has neither) drawn here with the same 2-px brush;
 *   - the leaf (48x39 at u 0x28, v 0x40): the "elmto" line is cleared with the leaf behind it (taken from the "Ver" and
 *     "Ordenar" leaves, which share the same leaf picture) and "objeto" is drawn in its place with the leaf font's
 *     white 2-px strokes.
 * Nothing is shipped: every pixel comes from the disc's texture or from the small stroke patterns below.  Only with
 * --lang 5 and a text patch whose MAIN5.EXE pause list holds "Objetos" (the improved translation); the texture is
 * recognised by a hash of the two label rects, so any other data is left alone. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int port_lang;
extern unsigned char *port_tpatch_named(const char *path, unsigned *size);
extern void port_printf(const char *, ...);

#define PX 64                                  /* texture page x (in 16-bit words), y 0 */
static int es_state = -1;                      /* -1 not checked, 0 off, 1 on */

static int es_on(void) {
    if (es_state < 0) {
        static const unsigned char obj[7] = { 0x20, 0x21, 0x13, 0x0F, 0x1B, 0x17, 0x1A };   /* "Objetos" (tools/pause_text.py) */
        unsigned n = 0; unsigned char *d = port_lang == 5 ? port_tpatch_named("EXE/MAIN5.EXE", &n) : 0;
        if (getenv("PORT_SUBSCREEN_ES_LOG")) port_printf("[subscreen_es] MAIN5.EXE patched: %u bytes\n", n);
        es_state = 0;
        for (unsigned i = 0; d && i + 8 <= n; i++) if (!memcmp(d + i, obj, 7) && (d[i + 7] == 0xFF || !d[i + 7])) { es_state = 1; break; }
        free(d);
    }
    return es_state;
}

static const unsigned short *g_vram;
static int tx(int u, int v) { return (g_vram[v * 1024 + PX + (u >> 2)] >> ((u & 3) * 4)) & 15; }
static unsigned char buf[64 * 64];             /* a working copy of the rect being rebuilt (one index a texel) */

static unsigned hash_rect(int u0, int v0, int w, int h) {
    unsigned hs = 2166136261u;
    for (int v = 0; v < h; v++) for (int u = 0; u < w; u++) hs = (hs ^ (unsigned)tx(u0 + u, v0 + v)) * 16777619u;
    return hs;
}
/* writes buf (w x h texels, w a multiple of 4, u0 too) back as 16-bit words */
static void put_rect(int u0, int v0, int w, int h, void (*write)(int, int, int, int, const unsigned short *)) {
    unsigned short words[16 * 64];
    for (int v = 0; v < h; v++) for (int k = 0; k < w / 4; k++) {
        const unsigned char *b = &buf[v * w + k * 4];
        words[v * (w / 4) + k] = (unsigned short)(b[0] | b[1] << 4 | b[2] << 8 | b[3] << 12);
    }
    write(PX + u0 / 4, v0, w / 4, h, words);
}

/* ---- the tab */
#define TAB_U 0x98
#define TAB_V 0x38
static const char *tab_O[12] = {               /* rows 6..17 of the tab */
    "..####..", ".##..##.", "##....##", "##....##", "##....##", "##....##",
    "##....##", "##....##", "##....##", "##....##", ".##..##.", "..####.." };
static const char *tab_j[12] = {               /* rows 7..18 */
    "..##", "..##", "....", "..##", "..##", "..##", "..##", "..##", "..##", "..##", "..##", "###." };
static void tab_stamp_pattern(const char **p, int rows, int row0, int x) {
    for (int r = 0; r < rows; r++) for (int c = 0; p[r][c]; c++) if (p[r][c] == '#') buf[(row0 + r) * 64 + x + c] = 1;
}
/* copies the text texels (index 1) of a glyph of another tab: (u, v) its top left in the page, w x h */
static void tab_stamp_glyph(int u, int v, int w, int h, int row0, int x, int mirror) {
    for (int r = 0; r < h; r++) for (int c = 0; c < w; c++)
        if (tx(u + c, v + r) == 1) buf[(row0 + r) * 64 + x + (mirror ? w - 1 - c : c)] = 1;
}
static void fix_tab(void (*write)(int, int, int, int, const unsigned short *)) {
    const int EV = 0x50, ES = 0x68;            /* v of the "Evento" (u 0x98) and "Estado" (u 0x78) tabs */
    for (int v = 0; v < 24; v++) {
        int last = tx(TAB_U, TAB_V + v);
        for (int u = 0; u < 64; u++) {
            int t = tx(TAB_U + u, TAB_V + v);
            if (t == 1 && v >= 4 && v < 20) t = last; else last = t;
            buf[v * 64 + u] = (unsigned char)t;
        }
    }
    tab_stamp_pattern(tab_O, 12, 6, 6);                          /* O  6..13 */
    tab_stamp_glyph(0x78 + 43, ES + 6, 8, 12, 6, 15, 1);         /* b  15..22: Estado's d, mirrored */
    tab_stamp_pattern(tab_j, 12, 7, 23);                         /* j  23..26 */
    tab_stamp_glyph(0x98 + 27, EV + 9, 8, 9, 9, 28, 0);          /* e  28..35: Evento's */
    tab_stamp_glyph(0x78 + 26, ES + 8, 8, 10, 8, 35, 0);         /* t  35..42: Estado's */
    tab_stamp_glyph(0x78 + 52, ES + 9, 6, 9, 9, 44, 0);          /* o  44..49 */
    tab_stamp_glyph(0x78 + 19, ES + 8, 7, 10, 8, 51, 0);         /* s  51..57 */
    put_rect(TAB_U, TAB_V, 64, 24, write);
}

/* ---- the leaf */
#define LEAF_U 0x28
#define LEAF_V 0x40
#define LEAF_W 48                               /* the picture is 48 texels wide (the sprite shows 40 + its frame) */
static const char *leaf_glyph[6][10] = {       /* rows 22..31 of the leaf: o b j e t o */
    { ".....", ".....", ".###.", "##.##", "#...#", "#...#", "#...#", "#...#", "##.##", ".###." },
    { "##...", "##...", "####.", "##.##", "##..#", "##..#", "##..#", "##..#", "##.##", "####." },
    { "..##", "....", "..##", "..##", "..##", "..##", "..##", "..##", "..##", "###." },
    { ".....", ".....", ".###.", "##.##", "#...#", "#####", "#....", "#....", "##..#", ".###." },
    { ".##..", ".##..", "#####", ".##..", ".##..", ".##..", ".##..", ".##..", ".##.#", "..##." },
    { ".....", ".....", ".###.", "##.##", "#...#", "#...#", "#...#", "#...#", "##.##", ".###." } };
static void fix_leaf(void (*write)(int, int, int, int, const unsigned short *)) {
    /* the same leaf picture without text: Ver (u 0x08) and Ordenar (u 0x40), both at v 0x68 */
    static const int other_u[2] = { 0x08, 0x40 };
    for (int v = 0; v < 39; v++) for (int u = 0; u < LEAF_W; u++) {
        int t = tx(LEAF_U + u, LEAF_V + v);
        if (v >= 20 && v < 33) {               /* the "elmto" line: the leaf behind it */
            if (t == 1 || t == 2) {
                int k, s = -1;
                for (k = 0; k < 2 && s < 0; k++) { int o = tx(other_u[k] + u, 0x68 + v); if (o != 1 && o != 2) s = o; }
                if (s < 0) s = u ? buf[v * LEAF_W + u - 1] : 0;
                t = s;
            }
        }
        buf[v * LEAF_W + u] = (unsigned char)t;
    }
    {   static const int x0[6] = { 7, 13, 18, 24, 30, 37 };
        for (int g = 0; g < 6; g++) for (int r = 0; r < 10; r++) for (int c = 0; leaf_glyph[g][r][c]; c++)
            if (leaf_glyph[g][r][c] == '#') buf[(22 + r) * LEAF_W + x0[g] + c] = 1;
    }
    put_rect(LEAF_U, LEAF_V, LEAF_W, 39, write);
}

/* gpu.c, after every host / GP0 upload into VRAM (x, y in words) */
#define TAB_HASH 0x30e7c66bu
#define LEAF_HASH 0x8573daffu
void port_subscreen_es_upload(const unsigned short *vram, int x, int y, int w, int h,
                              void (*write)(int, int, int, int, const unsigned short *)) {
    static int logged;
    if (port_lang != 5) return;
    /* the whole page area holding the labels and their sources: words 66..90, lines 0x38..0x9f */
    if (x > PX + 2 || x + w < PX + 26 || y > 0x38 || y + h < 0xA0) return;
    if (!es_on()) return;
    g_vram = vram;
    {   unsigned th = hash_rect(TAB_U, TAB_V, 64, 24), lh = hash_rect(LEAF_U, LEAF_V, LEAF_W, 39);
        if (getenv("PORT_SUBSCREEN_ES_LOG") && logged++ < 20) port_printf("[subscreen_es] upload %d,%d %dx%d: tab 0x%08x leaf 0x%08x\n", x, y, w, h, th, lh);
        if (th == TAB_HASH) fix_tab(write);
        if (lh == LEAF_HASH) fix_leaf(write);
    }
}
