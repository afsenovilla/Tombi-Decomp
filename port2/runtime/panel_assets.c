/* Second screen panel: the game's own graphics, read from the user's disc when the panel starts (docs/PORT.md "Second
 * screen"; nothing of them is in the repository).  The panel keeps a private copy of the VRAM areas the sub-screen and
 * the HUD use, filled as the game fills them:
 *
 *   SYSTEM/A10301.000  -> (64, 0)    64 x 256   sub-screen frames: stone, wood planks, cream windows, Tomba face icons
 *   SYSTEM/A00300.000  -> (128, 0)   192 x 256  sub-screen font, item icons (192, 0), event icons (256, 0)
 *   SYSTEM/A10003.GAM  -> (256, 256) 64 x 256   HUD page 1
 *   SYSTEM/A00000.GAM  -> (320, 256) 64 x 256   HUD page 2: vitality fruit, lives, AP box, digits
 *   AREA00/CLUT01.GAM  -> (128, 480) 256 x 32   the area palettes (HUD rows included)
 *   SYSTEM/D0SPSUB.BIN +0x98B8 -> (288, 480) 48 x 31   the sub-screen palettes (D0SPSUB uploads them when it opens)
 *
 * Sprites are rects of those pages (4 bpp, a 16 colour CLUT row); their coordinates come from the primitives the game
 * sends (port --dump-draws) and from the sub-screen code (src/d0spsub):
 *   event icon of event k: frame ICONS[k] (D_800FE974, D0SPSUB +0x1694C: pointers to {u16 frame, ..}) of the sub-screen
 *     sprite bank (AREA19/A103.GAM, PAL), first part {u, v, clut, .., w, h} on the page at (256, 0), with the 8 CLUTs
 *     subScreenDrawEventIcons forces (tools/event_icons.py);
 *   item icon n: 16 x 16 cell (n % 15 * 16 + 8, n / 15 * 16 + 8) of the page at (192, 0), CLUT row ITEM_ICON_CLUTS[n]
 *     (D0SPSUB +0x21C60) at x 304 (subScreenDrawItemIcon).
 * Text: the game's WFM fonts (AREAnn/MFNTnnnL.WFM 8 / 16 px, CFNT999L.WFM 24 px event titles, L = the language letter);
 * a glyph is a bitmap, named by the SHA-1 of its image (panel_charmap.h, from tools/wfm_charmap.json). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "panel_assets.h"
#include "panel_charmap.h"
#include "panel_addrs.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32; typedef short s16;

int pa_ok;              /* pages + palettes read */
int pa_font_ok;         /* WFM glyphs read */
static u16 *vram;       /* 1024 x 512, malloc'd when the panel starts */

static unsigned rd32(const u8 *d, unsigned o) { return d[o] | d[o + 1] << 8 | d[o + 2] << 16 | (u32)d[o + 3] << 24; }
static unsigned rd16(const u8 *d, unsigned o) { return d[o] | d[o + 1] << 8; }

/* `GAM\0`, u32 size, then 16-bit flag words (bit 0 first; 1 = copy: back offset byte, length byte) */
static u8 *gam_unpack(const u8 *d, unsigned n, unsigned *out_n) {
    if (n < 8 || memcmp(d, "GAM", 4)) return NULL;
    unsigned size = rd32(d, 4), p = 8, o = 0, flags = 0, left = 0;
    if (size > (8u << 20)) return NULL;
    u8 *out = malloc(size ? size : 1);
    while (o < size) {
        if (!left) { if (p + 2 > n) break; flags = rd16(d, p); p += 2; left = 16; }
        int bit = flags & 1; flags >>= 1; left--;
        if (bit) {
            if (p + 2 > n) break;
            unsigned off = d[p], ln = d[p + 1]; p += 2;
            if (!off || off > o) break;
            for (unsigned k = 0; k < ln && o < size; k++, o++) out[o] = out[o - off];
        } else { if (p >= n) break; out[o++] = d[p++]; }
    }
    if (o < size) { free(out); return NULL; }
    *out_n = size;
    return out;
}
u8 *pa_read(const char *name, unsigned *n) {
    u8 *d = port_cd_read_named(name, n);
    if (d && *n >= 8 && !memcmp(d, "GAM", 4)) { unsigned m; u8 *u = gam_unpack(d, *n, &m); free(d); d = u; *n = u ? m : 0; }
    return d;
}
static int load_rect(const char *name, int x, int y, int w, unsigned off, int h) {
    unsigned n; u8 *d = pa_read(name, &n);
    if (!d) { port_printf("[panel] %s not readable: game graphics off\n", name); return 0; }
    unsigned words = h > 0 ? (unsigned)(w * h) : (n - off) / 2;
    if (off + 2 * words > n) { free(d); port_printf("[panel] %s: unexpected size %u\n", name, n); return 0; }
    for (unsigned i = 0; i < words; i++) {
        int X = x + (int)(i % w), Y = y + (int)(i / w);
        if (Y < 512 && X < 1024) vram[Y * 1024 + X] = (u16)rd16(d, off + 2 * i);
    }
    free(d);
    return 1;
}

/* ---------------------------------------------------------------- event / item icon tables */
#define NEV_ICONS 192
static struct { u8 u, v, w, h; u16 clut; } ev_icon[NEV_ICONS];
static u8 item_clut[200];
static int icons_ok;
static void load_icons(void) {
    static const struct { u8 e; u16 c; } force[] = { {0x56, 0x7D12}, {0x57, 0x7D52}, {0x58, 0x7D92}, {0x74, 0x7DD2},
        {0x97, 0x7A93}, {0x9D, 0x7AD3}, {0x9F, 0x7B13}, {0xA4, 0x7B53} };
    unsigned nd, nb; u8 *d = pa_read(PA_F_SUB, &nd), *b0 = pa_read(PA_F_SPRITES, &nb), *b = b0;
    const unsigned icons = PV_EVENT_ICONS - PORT_OVL_BASE, cluts = PV_ITEM_ICON_CLUTS - PORT_OVL_BASE;
    if (b && nb > PA_SPRITES_OFF + 16) { b += PA_SPRITES_OFF; nb -= PA_SPRITES_OFF; } else nb = 0;
    if (d && b && nd > cluts + 200 && nb > 16) {
        int nf = (int)(rd16(b, 2) / 4), good = 0;
        for (int k = 0; k < NEV_ICONS; k++) {
            unsigned p = rd32(d, icons + 4 * k) - PORT_OVL_BASE;
            if (p + 2 > nd) continue;
            int f = (int)rd16(d, p);
            if (f >= nf) continue;
            unsigned o = rd16(b, 4 * f + 2);
            if (o + 16 > nb) continue;
            const u8 *q = b + o;
            ev_icon[k].u = q[0]; ev_icon[k].v = q[1]; ev_icon[k].clut = (u16)rd16(q, 2); ev_icon[k].w = q[10]; ev_icon[k].h = q[11];
            good++;
        }
        for (unsigned i = 0; i < sizeof force / sizeof force[0]; i++) ev_icon[force[i].e].clut = force[i].c;
        memcpy(item_clut, d + cluts, sizeof item_clut);
        icons_ok = good > 100;
    }
    if (!icons_ok) port_printf("[panel] event icon tables not found (%s %u bytes, %s %u bytes)\n", PA_F_SUB, d ? nd : 0, PA_F_SPRITES, b ? nb : 0);
    free(d); free(b0);
}

/* ---------------------------------------------------------------- canvas (game pixels) */
u32 *pa_cv; int pa_cw, pa_ch;
int pa_clip_x0, pa_clip_y0, pa_clip_x1 = 1 << 20, pa_clip_y1 = 1 << 20;

static inline u32 c15(u16 v) {
    u32 r = (v & 31) * 255 / 31, g = (v >> 5 & 31) * 255 / 31, b = (v >> 10 & 31) * 255 / 31;
    return 0xFF000000u | b << 16 | g << 8 | r;
}
static inline int clipped(int x, int y) {
    return x < pa_clip_x0 || y < pa_clip_y0 || x >= pa_clip_x1 || y >= pa_clip_y1 || (unsigned)x >= (unsigned)pa_cw || (unsigned)y >= (unsigned)pa_ch;
}
void pa_put(int x, int y, u32 c) { if (!clipped(x, y)) pa_cv[y * pa_cw + x] = c; }
void pa_blend(int x, int y, u32 c, int a) {
    if (clipped(x, y) || a <= 0) return;
    u32 *p = &pa_cv[y * pa_cw + x];
    if (a >= 255) { *p = c | 0xFF000000u; return; }
    u32 d = *p;
    /* x / 255 for x <= 255 * 255 as (x + 1 + (x >> 8)) >> 8: exact over that range, no division (the 3DS has none) */
    #define DIV255(x) (((x) + 1 + ((x) >> 8)) >> 8)
    u32 r = DIV255((c & 0xFF) * a + (d & 0xFF) * (255 - a)), g = DIV255((c >> 8 & 0xFF) * a + (d >> 8 & 0xFF) * (255 - a)),
        bb = DIV255((c >> 16 & 0xFF) * a + (d >> 16 & 0xFF) * (255 - a));
    #undef DIV255
    *p = 0xFF000000u | bb << 16 | g << 8 | r;
}
void pa_fill(int x, int y, int w, int h, u32 c) { for (int j = y; j < y + h; j++) for (int i = x; i < x + w; i++) pa_put(i, j, c); }
u32 pa_clut_color(int cx, int cy, int i) { return vram ? c15(vram[(cy & 511) * 1024 + ((cx + i) & 1023)]) : 0xFF808080u; }

/* texel (4 bpp) of the page at word x tx, row ty */
static inline int texel(int tx, int ty, int u, int v) { return (vram[((ty + v) & 511) * 1024 + ((tx + (u >> 2)) & 1023)] >> ((u & 3) * 4)) & 15; }

/* draw rect (u, v, w, h) of the 4 bpp page at (tx, ty) words with the CLUT at (cx, cy) at canvas (x, y).
   flags: PA_FLIPX, PA_DIM (silhouette), PA_SHADE (darker), PA_OPAQUE (index 0 drawn) */
unsigned pa_cut_mask, pa_drop_mask;   /* PA_CUTBOX: indices flooded from the border / dropped anywhere */
void pa_spr(int x, int y, int tx, int ty, int u, int v, int w, int h, int cx, int cy, int flags) {
    if (!pa_ok || w <= 0 || h <= 0 || w > 256 || h > 256) return;
    u16 pal[16]; static u8 idx[258 * 258], vis[258 * 258];
    for (int i = 0; i < 16; i++) pal[i] = vram[(cy & 511) * 1024 + ((cx + i) & 1023)];
    /* PS1 GPU, textured sprite: a texel whose CLUT colour is 0x0000 is not drawn; any other (STP bit 0x8000 or not)
       is drawn (the panel draws opaque, like a primitive without semi-transparency) */
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) {
        int t = texel(tx, ty, u + ((flags & PA_FLIPX) ? w - 1 - i : i), v + j);
        idx[j * w + i] = (u8)t; vis[j * w + i] = pal[t] || (flags & PA_OPAQUE);
    }
    if (flags & PA_CUTBOX) {   /* flood from the border through the box's indices */
        static int st[258 * 258]; int n = 0;
        for (int j = 0; j < h; j++) for (int i = 0; i < w; i++)
            if ((!i || !j || i == w - 1 || j == h - 1) && (pa_cut_mask >> idx[j * w + i] & 1)) { st[n++] = j * w + i; idx[j * w + i] |= 0x80; }
        while (n) {
            int p = st[--n], i = p % w, j = p / w; vis[p] = 0;
            int nb[4] = { i > 0 ? p - 1 : -1, i < w - 1 ? p + 1 : -1, j > 0 ? p - w : -1, j < h - 1 ? p + w : -1 };
            for (int k = 0; k < 4; k++) if (nb[k] >= 0 && !(idx[nb[k]] & 0x80) && (pa_cut_mask >> idx[nb[k]] & 1)) { idx[nb[k]] |= 0x80; st[n++] = nb[k]; }
        }
        for (int k = 0; k < w * h; k++) { idx[k] &= 15; if (pa_drop_mask >> idx[k] & 1) vis[k] = 0; }
    }
    /* integer scale (PA_X2 / PA_X3, map markers): every texel, and the outline around them, an s x s block */
    int sc = flags & PA_X4 ? 4 : flags & PA_X3 ? 3 : flags & PA_X2 ? 2 : 1;
    if (flags & PA_OUTLINE)
        for (int j = -1; j <= h; j++) for (int i = -1; i <= w; i++) {
            if (i >= 0 && j >= 0 && i < w && j < h && vis[j * w + i]) continue;
            int on = (i > 0 && j >= 0 && j < h && i - 1 < w && vis[j * w + i - 1]) || (i + 1 < w && i + 1 >= 0 && j >= 0 && j < h && vis[j * w + i + 1]) ||
                     (j > 0 && i >= 0 && i < w && j - 1 < h && vis[(j - 1) * w + i]) || (j + 1 < h && j + 1 >= 0 && i >= 0 && i < w && vis[(j + 1) * w + i]);
            if (on) pa_fill(x + i * sc, y + j * sc, sc, sc, 0xFF101828u);
        }
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) {
        if (!vis[j * w + i]) continue;
        u32 col = c15(pal[idx[j * w + i]]);
        if (flags & PA_DIM) col = 0xFF463240u;
        else if (flags & PA_SHADE) col = 0xFF000000u | ((col >> 1) & 0x7F7F7Fu);
        if (sc == 1) pa_put(x + i, y + j, col); else pa_fill(x + i * sc, y + j * sc, sc, sc, col);
    }
}
/* tile a rect over (x, y, W, H) */
void pa_tile(int x, int y, int W, int H, int tx, int ty, int u, int v, int w, int h, int cx, int cy) {
    int sx0 = pa_clip_x0, sy0 = pa_clip_y0, sx1 = pa_clip_x1, sy1 = pa_clip_y1;
    if (x > pa_clip_x0) pa_clip_x0 = x; if (y > pa_clip_y0) pa_clip_y0 = y;
    if (x + W < pa_clip_x1) pa_clip_x1 = x + W; if (y + H < pa_clip_y1) pa_clip_y1 = y + H;
    for (int j = y; j < y + H; j += h) for (int i = x; i < x + W; i += w) pa_spr(i, j, tx, ty, u, v, w, h, cx, cy, PA_OPAQUE);
    pa_clip_x0 = sx0; pa_clip_y0 = sy0; pa_clip_x1 = sx1; pa_clip_y1 = sy1;
}
/* 9-slice of a framed rect (u, v, w, h) with corner c; fill: 0 = tile the middle, else that colour, 1 = transparent */
void pa_nine(int x, int y, int W, int H, int tx, int ty, int u, int v, int w, int h, int c, int cx, int cy, u32 fill) {
    if (fill == 0) pa_tile(x + c, y + c, W - 2 * c, H - 2 * c, tx, ty, u + c, v + c, w - 2 * c, h - 2 * c, cx, cy);
    else if (fill != 1) pa_fill(x + c, y + c, W - 2 * c, H - 2 * c, fill);
    int sx0 = pa_clip_x0, sy0 = pa_clip_y0, sx1 = pa_clip_x1, sy1 = pa_clip_y1;
    for (int i = x + c; i < x + W - c; i += w - 2 * c) {   /* top / bottom edges */
        int n = x + W - c - i < w - 2 * c ? x + W - c - i : w - 2 * c;
        pa_spr(i, y, tx, ty, u + c, v, n, c, cx, cy, 0);
        pa_spr(i, y + H - c, tx, ty, u + c, v + h - c, n, c, cx, cy, 0);
    }
    for (int j = y + c; j < y + H - c; j += h - 2 * c) {   /* left / right edges */
        int n = y + H - c - j < h - 2 * c ? y + H - c - j : h - 2 * c;
        pa_spr(x, j, tx, ty, u, v + c, c, n, cx, cy, 0);
        pa_spr(x + W - c, j, tx, ty, u + w - c, v + c, c, n, cx, cy, 0);
    }
    pa_spr(x, y, tx, ty, u, v, c, c, cx, cy, 0); pa_spr(x + W - c, y, tx, ty, u + w - c, v, c, c, cx, cy, 0);
    pa_spr(x, y + H - c, tx, ty, u, v + h - c, c, c, cx, cy, 0); pa_spr(x + W - c, y + H - c, tx, ty, u + w - c, v + h - c, c, c, cx, cy, 0);
    pa_clip_x0 = sx0; pa_clip_y0 = sy0; pa_clip_x1 = sx1; pa_clip_y1 = sy1;
}

/* ---------------------------------------------------------------- icons */
int pa_event_icon(int x, int y, int e, int flags) {   /* returns the width drawn, unscaled (0: none) */
    if (!pa_ok || !icons_ok || e < 0 || e >= NEV_ICONS || !ev_icon[e].w) return 0;
    u16 cw = ev_icon[e].clut;
    pa_spr(x, y, 256, 0, ev_icon[e].u, ev_icon[e].v, ev_icon[e].w, ev_icon[e].h, (cw & 63) * 16, cw >> 6, flags);
    return ev_icon[e].w;
}
int pa_event_icon_h(int e) { return pa_ok && icons_ok && e >= 0 && e < NEV_ICONS ? ev_icon[e].h : 0; }
int pa_event_icon_w(int e) { return pa_ok && icons_ok && e >= 0 && e < NEV_ICONS ? ev_icon[e].w : 0; }
void pa_item_icon(int x, int y, int n, int flags) {
    if (!pa_ok || !icons_ok || n < 0 || n >= 200) return;
    pa_spr(x, y, 192, 0, n % 15 * 16 + 8, n / 15 * 16 + 8, 16, 16, 304, 480 + item_clut[n], flags);
}

/* ---------------------------------------------------------------- WFM fonts */
typedef struct { u8 w, h; const u8 *img; } Glyph;
static Glyph glyphs[3][256];         /* heights 8, 16, 24 */
static u8 *font_blobs[48]; static int nblobs;

/* SHA-1 (only to name the glyph bitmaps) */
static void sha1(const u8 *m, unsigned n, u8 out[20]) {
    u32 h[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
    unsigned total = ((n + 8) / 64 + 1) * 64;
    u8 *b = calloc(total, 1);
    memcpy(b, m, n); b[n] = 0x80;
    unsigned long long bits = (unsigned long long)n * 8;
    for (int i = 0; i < 8; i++) b[total - 1 - i] = (u8)(bits >> (8 * i));
    for (unsigned o = 0; o < total; o += 64) {
        u32 w[80];
        for (int i = 0; i < 16; i++) w[i] = (u32)b[o + 4 * i] << 24 | b[o + 4 * i + 1] << 16 | b[o + 4 * i + 2] << 8 | b[o + 4 * i + 3];
        for (int i = 16; i < 80; i++) { u32 t = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]; w[i] = t << 1 | t >> 31; }
        u32 a = h[0], bb = h[1], c = h[2], d = h[3], e = h[4];
        for (int i = 0; i < 80; i++) {
            u32 f, k;
            if (i < 20) { f = (bb & c) | (~bb & d); k = 0x5A827999; } else if (i < 40) { f = bb ^ c ^ d; k = 0x6ED9EBA1; }
            else if (i < 60) { f = (bb & c) | (bb & d) | (c & d); k = 0x8F1BBCDC; } else { f = bb ^ c ^ d; k = 0xCA62C1D6; }
            u32 t = (a << 5 | a >> 27) + f + e + k + w[i];
            e = d; d = c; c = bb << 30 | bb >> 2; bb = a; a = t;
        }
        h[0] += a; h[1] += bb; h[2] += c; h[3] += d; h[4] += e;
    }
    free(b);
    for (int i = 0; i < 20; i++) out[i] = (u8)(h[i / 4] >> (24 - 8 * (i % 4)));
}
static int hidx(int h) { return h == 8 ? 0 : h == 16 ? 1 : h == 24 ? 2 : -1; }
static int charmap_exact(int h, int w, const u8 *img) {
    u8 s[20]; sha1(img, (unsigned)(w * h + 1) / 2, s);
    for (unsigned j = 0; j < sizeof panel_charmap / sizeof panel_charmap[0]; j++)
        if (panel_charmap[j].h == h && panel_charmap[j].w == w && !memcmp(panel_charmap[j].sha, s, 6)) return panel_charmap[j].ch;
    return -1;
}
/* The Latin-1 character of a WFM glyph bitmap (w x h, 4 bpp), -1 when not known.  The files of a text patch (the improved
   Spanish translation: --text-patch, translation.tpatch, or data/ written by the PC launcher) move some glyphs inside their
   cell (tools/wfm_py.py shift_glyphs: the 16 px ',' and u-acute dx pixels sideways, the 8 px accented letters one row up
   by dropping the empty row under the accent), so their hashes are not the disc's: when there is no exact match the
   move is undone (a sideways shift of up to 2 columns that loses no ink; an empty row put back at each place, the last
   row being empty) and the disc glyph looked up again.  *norm (when not NULL) gets a malloc'd copy of the disc's bitmap
   for a moved glyph (the panel draws its fonts as the disc has them, its own redrawn accents included), else NULL. */
static int glyph_char(int h, int w, const u8 *img, u8 **norm) {
    int c = charmap_exact(h, w, img), n = w * h, sz = (n + 1) / 2;
    if (norm) *norm = NULL;
    if (c >= 0 || n <= 0 || n > 64 * 32) return c;
    u8 px[64 * 32], out[64 * 32];
    u8 *pk = malloc((size_t)sz);
    if (!pk) return -1;
    for (int p = 0; p < n; p++) px[p] = (img[p >> 1] >> ((p & 1) * 4)) & 15;
    for (int t = 0; t < 4 + h && c < 0; t++) {
        memset(out, 0, (size_t)n);
        if (t < 4) {   /* sideways by dx = 1, -1, 2, -2 (the glyph went -dx) */
            static const int dxs[4] = { 1, -1, 2, -2 };
            int dx = dxs[t], lost = 0;
            for (int r = 0; r < h; r++) for (int x = 0; x < w; x++) {
                int v = px[r * w + x], nx = x + dx;
                if (nx < 0 || nx >= w) lost |= v; else out[r * w + nx] = (u8)v;
            }
            if (lost) continue;
        } else {       /* an empty row put back at row t - 4, the rest one row lower */
            int r0 = t - 4, last = 0;
            for (int x = 0; x < w; x++) last |= px[(h - 1) * w + x];
            if (last) break;
            for (int r = 0; r < h; r++) if (r != r0) memcpy(out + r * w, px + (r < r0 ? r : r - 1) * w, (size_t)w);
        }
        memset(pk, 0, (size_t)sz);
        for (int p = 0; p < n; p++) pk[p >> 1] |= (u8)(out[p] << ((p & 1) * 4));
        c = charmap_exact(h, w, pk);
    }
    if (c >= 0 && norm) *norm = pk; else free(pk);
    return c;
}
/* the glyphs of the WFM3 bank at d (n bytes); returns how many were new */
static unsigned load_wfm_bank(u8 *d, unsigned n) {
    unsigned ng = rd16(d, 14), used = 0;
    for (unsigned i = 0; i < ng && 144 + 2 * i + 2 <= n; i++) {
        unsigned p = rd16(d, 144 + 2 * i);
        if (p + 8 > n) continue;
        int h = (int)rd16(d, p + 2), w = (int)rd16(d, p + 4), k = hidx(h);
        unsigned sz = (unsigned)(w * h + 1) / 2;
        if (k < 0 || !w || w > 255 || p + 8 + sz > n) continue;
        u8 *norm; int ch = glyph_char(h, w, d + p + 8, &norm);
        if (ch >= 0 && !glyphs[k][ch].img) { glyphs[k][ch].w = (u8)w; glyphs[k][ch].h = (u8)h; glyphs[k][ch].img = norm ? norm : d + p + 8; used++; }
        else free(norm);
    }
    return used;
}
static void load_wfm(const char *name) {
    unsigned n, used = 0; u8 *d;
#if PORT_VERSION_USA
    /* USA: no WFM files; the dialogue banks (8 / 16 px) and the event title bank (24 px) are inside the area's GAM files */
    d = pa_read(name, &n);
    if (!d) return;
    if (nblobs >= 48) { free(d); return; }
    for (unsigned o = 0; o + 144 <= n; o += 4) if (!memcmp(d + o, "WFM3", 4)) used += load_wfm_bank(d + o, n - o);
#else
    d = port_cd_read_named(name, &n);
    if (!d) return;
    if (n < 144 || memcmp(d, "WFM3", 4) || nblobs >= 48) { free(d); return; }
    used = load_wfm_bank(d, n);
#endif
    if (used) font_blobs[nblobs++] = d; else free(d);
}
static void load_fonts(void) {
#if PORT_VERSION_USA
    {   /* the banks of AREA01 (dialogue + titles), AREA02, AREA00, then the other areas until the letters are there */
        static const char *const usa[] = { "AREA01/A008.GAM", "AREA02/A003.GAM", "AREA00/D003.GAM", "AREA03/A005.GAM", "AREA04/A007.GAM",
            "AREA05/A003.GAM", "AREA06/A002.GAM", "AREA08/A003.GAM", "AREA09/A003.GAM", "AREA10/A003.GAM", "AREA11/A002.GAM" };
        for (unsigned i = 0; i < sizeof usa / sizeof usa[0]; i++) {
            int have = 0;
            for (const char *s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; *s; s++) have += glyphs[0][(u8)*s].img && glyphs[1][(u8)*s].img;
            if (i >= 2 && have >= 62) break;
            load_wfm(usa[i]);
        }
    }
#else
    {   static const char L[9] = { 'B', 'F', 'G', 'I', 'H', 'S', 'P', 'W', 'D' };
    char l = port_lang >= 0 && port_lang < 9 ? L[port_lang] : 'B', name[40];
    snprintf(name, sizeof name, "AREA00/CFNT999%c.WFM", l); load_wfm(name);
    snprintf(name, sizeof name, "AREA00/MFNT000%c.WFM", l); load_wfm(name);
    snprintf(name, sizeof name, "AREA00/MFNT001%c.WFM", l); load_wfm(name);
    for (int a = 1; a < 20; a++) {
        int have = 0;
        for (const char *s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\xE1\xE9\xED\xF3\xFA\xF1"; *s; s++) have += glyphs[0][(u8)*s].img && glyphs[1][(u8)*s].img;
        if (have >= 68) break;
        snprintf(name, sizeof name, "AREA%02d/MFNT%02d0%c.WFM", a, a, l); load_wfm(name);
    }
    }
#endif
    /* the fonts that lack the Spanish opening marks: draw them as the ? / ! glyph turned 180 degrees */
    for (int k = 0; k < 3; k++) for (int m = 0; m < 2; m++) {
        int inv = m ? 0xA1 : 0xBF, src = m ? '!' : '?';
        const Glyph *g = &glyphs[k][src];
        if (glyphs[k][inv].img || !g->img) continue;
        int n = g->w * g->h; u8 *img = calloc((size_t)(n + 1) / 2, 1);
        if (!img) continue;
        for (int p = 0; p < n; p++) { int v = (g->img[p >> 1] >> ((p & 1) * 4)) & 15, q = n - 1 - p; img[q >> 1] |= (u8)(v << ((q & 1) * 4)); }
        glyphs[k][inv].w = g->w; glyphs[k][inv].h = g->h; glyphs[k][inv].img = img;
    }
    /* the 8 px font's acute accents: the game draws them in its light edge shades, which vanish on the cream windows, its
       i-acute has a smeared accent and u-acute is missing.  Its accented small letters sit one row lower than the plain
       ones, the accent in rows 0-1: u-acute is built that way from u, i-acute from i on its own line (the accent in
       place of the dot, so it lines up with the plain letters), and every acute gets a solid stroke. */
    {   static const u8 acute[][2] = { {0xE1, 'a'}, {0xE9, 'e'}, {0xED, 'i'}, {0xF3, 'o'}, {0xFA, 'u'}, {0xC1, 'A'} };
        static const u8 stroke[2][8] = { {0, 0, 0, 0, 1, 1, 2, 0}, {0, 0, 0, 1, 1, 2, 0, 0} };
        for (unsigned a = 0; a < sizeof acute / sizeof acute[0]; a++) {
            Glyph *g = &glyphs[0][acute[a][0]]; const Glyph *b = &glyphs[0][acute[a][1]];
            int build = acute[a][0] == 0xED || acute[a][0] == 0xFA;
            if (build ? !b->img || b->w != 8 || b->h != 8 : !g->img || g->w != 8 || g->h != 8) continue;
            u8 *img = calloc(32, 1);
            if (!img) continue;
            for (int p = 16; p < 64; p++) {
                int q = acute[a][1] == 'i' ? p : p - 8;   /* the i keeps its line (its dot row is where the accent goes) */
                int v = build ? (p < 24 ? 0 : (b->img[q >> 1] >> ((q & 1) * 4)) & 15) : (g->img[p >> 1] >> ((p & 1) * 4)) & 15;
                img[p >> 1] |= (u8)(v << ((p & 1) * 4));
            }
            for (int j = 0; j < 2; j++) for (int i = 0; i < 8; i++) img[(j * 8 + i) >> 1] |= (u8)(stroke[j][i] << ((i & 1) * 4));
            g->w = 8; g->h = 8; g->img = img;
        }
    }
    /* accented capitals the fonts lack (8 px: A-acute with the improved translation, which moves the disc's one out of
       its cell, E I O U-acute, N-tilde; 16 px: I-acute, N-tilde): built from the game's own capital as its accented
       capitals are (the disc's 8 px A-acute, 16 px E O U-acute): the accent on top, the letter one row (8 px, then on
       rows 2-7 like the accented small letters) or two rows (16 px, rows 4-13) shorter, a row where the letter is
       straight dropped.  The accent: 8 px, the solid stroke above (the I's one column left, over its stem) or the
       n-tilde's rows; 16 px, the rows of the game's own small accented letter (i-acute, n-tilde). */
    {   static const struct { u8 ch, base, alt, small, drop8, drop16a, drop16b; } caps[] = {
            {0xC1, 'A', 0, 0xE1, 3, 3, 12}, {0xC9, 'E', 0, 0xE9, 2, 3, 12}, {0xCD, 'I', 0, 0xED, 3, 3, 12},
            {0xD3, 'O', '0', 0xF3, 3, 3, 12}, {0xDA, 'U', 0, 0xFA, 3, 3, 12}, {0xD1, 'N', 0, 0xF1, 3, 3, 12} };
        static const u8 stroke[2][8] = { {0, 0, 0, 0, 1, 1, 2, 0}, {0, 0, 0, 1, 1, 2, 0, 0} };
        for (int k = 0; k < 2; k++) for (unsigned a = 0; a < sizeof caps / sizeof caps[0]; a++) {
            int h = k ? 16 : 8, top = k ? 4 : 2;
            Glyph *g = &glyphs[k][caps[a].ch]; const Glyph *b = &glyphs[k][caps[a].base], *sm = &glyphs[k][caps[a].small];
            if (g->img) continue;
            if (!b->img && caps[a].alt) b = &glyphs[k][caps[a].alt];
            if (!b->img || b->h != h || (k && (!sm->img || sm->w != b->w || sm->h != 16)) || (!k && b->w != 8)) continue;
            if (!k && caps[a].ch == 0xD1 && (!sm->img || sm->w != 8 || sm->h != 8)) continue;
            int w = b->w, n = w * h;
            u8 *img = calloc((size_t)(n + 1) / 2, 1);
            if (!img) continue;
            #define PX(gl, x, y) (((gl)->img[((y) * w + (x)) >> 1] >> ((((y) * w + (x)) & 1) * 4)) & 15)
            #define PUT(x, y, v) (img[((y) * w + (x)) >> 1] |= (u8)((v) << ((((y) * w + (x)) & 1) * 4)))
            /* the letter: rows top .. h-1 (8 px) / 13 (16 px) take its rows 0..6 / 2..13 less the dropped ones */
            for (int r = k ? 2 : 0, y = top; r < (k ? 14 : 7); r++) {
                if (k ? r == caps[a].drop16a || r == caps[a].drop16b : r == caps[a].drop8) continue;
                for (int x = 0; x < w; x++) PUT(x, y, PX(b, x, r));
                y++;
            }
            /* the accent: rows 0-1 (8 px) / 2-3 (16 px) */
            for (int j = 0; j < 2; j++) for (int x = 0; x < w; x++) {
                int v;
                if (k) v = PX(sm, x, 2 + j);
                else if (caps[a].ch == 0xD1) v = PX(sm, x, j);
                else { int sx = caps[a].ch == 0xCD ? x + 1 : x; v = sx < 8 ? stroke[j][sx] : 0; }
                PUT(x, (k ? 2 : 0) + j, v);
            }
            #undef PX
            #undef PUT
            g->w = (u8)w; g->h = (u8)h; g->img = img;
        }
    }
    /* a font with no 8 px hyphen (the English disc's): the 16 px one's bar on the 8 px font's hyphen row (3) */
    if (!glyphs[0]['-'].img && glyphs[1]['-'].img && glyphs[1]['-'].h == 16) {
        const Glyph *b = &glyphs[1]['-']; int w = b->w, row = -1;
        for (int y = 0; y < 16 && row < 0; y++) for (int x = 0; x < w; x++) if ((b->img[(y * w + x) >> 1] >> (((y * w + x) & 1) * 4)) & 15) { row = y; break; }
        u8 *img = row >= 0 ? calloc((size_t)(w * 8 + 1) / 2, 1) : NULL;
        if (img) {
            for (int x = 0; x < w; x++) {
                int v = (b->img[(row * w + x) >> 1] >> (((row * w + x) & 1) * 4)) & 15, p = 3 * w + x;
                img[p >> 1] |= (u8)(v << ((p & 1) * 4));
            }
            glyphs[0]['-'].w = (u8)w; glyphs[0]['-'].h = 8; glyphs[0]['-'].img = img;
        }
    }
    int n8 = 0, n16 = 0, n24 = 0;
    for (int c = 0; c < 256; c++) { n8 += !!glyphs[0][c].img; n16 += !!glyphs[1][c].img; n24 += !!glyphs[2][c].img; }
    pa_font_ok = n16 > 40;
    port_printf("[panel] WFM glyphs: %d (8 px), %d (16 px), %d (24 px) from %d files\n", n8, n16, n24, nblobs);
}
static const Glyph *find_glyph(int k, int c) {
    static const char *base = "AAAAAAACEEEEIIIIDNOOOOOxOUUUUYTsaaaaaaaceeeeiiiidnooooo/ouuuuyty";
    if (glyphs[k][c].img) return &glyphs[k][c];
    if (c >= 'a' && c <= 'z' && glyphs[k][c - 32].img) return &glyphs[k][c - 32];
    if (c >= 'A' && c <= 'Z' && glyphs[k][c + 32].img) return &glyphs[k][c + 32];
    if (c >= 0xC0 && glyphs[k][(u8)base[c - 0xC0]].img) return &glyphs[k][(u8)base[c - 0xC0]];
    if (k == 2) return find_glyph(1, c);
    return NULL;
}
static int space_w(int k) { return k == 0 ? 4 : k == 1 ? 5 : 10; }
/* 1 when every letter of s has a glyph of its own in the h px font (the 24 px font does not borrow the 16 px one here) */
int pa_has_glyphs(int h, const char *s) {
    int k = hidx(h);
    if (k < 0 || !pa_font_ok) return 0;
    static const char *base = "AAAAAAACEEEEIIIIDNOOOOOxOUUUUYTsaaaaaaaceeeeiiiidnooooo/ouuuuyty";
    for (; *s; s++) {
        int c = (u8)*s;
        if (c == ' ') continue;
        if (glyphs[k][c].img || (c >= 'a' && c <= 'z' && glyphs[k][c - 32].img) || (c >= 'A' && c <= 'Z' && glyphs[k][c + 32].img) ||
            (c >= 0xC0 && glyphs[k][(u8)base[c - 0xC0]].img)) continue;
        return 0;
    }
    return 1;
}
int pa_text_w(int h, const char *s) {
    int k = hidx(h), w = 0;
    if (k < 0) return 0;
    for (; *s; s++) {
        int c = (u8)*s;
        if (c == ' ') { w += space_w(k); continue; }
        if (c == ':') { const Glyph *g = find_glyph(k, '.'); w += g ? g->w : 3; continue; }
        const Glyph *g = find_glyph(k, c);
        w += g ? g->w : space_w(k);
    }
    return w;
}
static void glyph(int x, int y, const Glyph *g, int k, u32 body, u32 edge) {
    static const u32 title[8] = { 0, 0xFF000000u, 0xFFFFFFFFu, 0xFF7B7B7Bu, 0xFF4A4A4Au, 0xFFADADADu, 0xFF003A84u, 0xFF0063C6u };
    for (int j = 0; j < g->h; j++) for (int i = 0; i < g->w; i++) {
        int p = j * g->w + i, v = (g->img[p >> 1] >> ((p & 1) * 4)) & 15;
        if (!v) continue;
        u32 c;
        if (g->h == 24) { if (v >= 8) continue; c = title[v]; }
        else if (v == 1) c = body;
        else {
            int t = (v - 2) * 255 / 5; if (t > 255) t = 255;
            u32 r = ((edge & 0xFF) * (255 - t * 2 / 5) + (body & 0xFF) * (t * 2 / 5)) / 255;
            u32 gg = ((edge >> 8 & 0xFF) * (255 - t * 2 / 5) + (body >> 8 & 0xFF) * (t * 2 / 5)) / 255;
            u32 b = ((edge >> 16 & 0xFF) * (255 - t * 2 / 5) + (body >> 16 & 0xFF) * (t * 2 / 5)) / 255;
            c = 0xFF000000u | b << 16 | gg << 8 | r;
        }
        pa_put(x + i, y + (k == 2 ? 24 : k == 1 ? 16 : 8) - g->h + j, c);
    }
}
/* text in the game's glyphs; h = 8, 16 or 24 (event titles); (x, y) = top left; returns the end x */
extern u32 pa_outline;
int pa_text(int x, int y, int h, const char *s, u32 body, u32 edge) {
    int k = hidx(h);
    if (k < 0) return x;
    if (pa_outline && k < 2) {
        static const int d[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };
        u32 o = pa_outline; pa_outline = 0;
        for (int i = 0; i < 8; i++) pa_text(x + d[i][0], y + d[i][1], h, s, o, o);
        pa_outline = o;
    }
    for (; *s; s++) {
        int c = (u8)*s;
        if (c == ' ') { x += space_w(k); continue; }
        if (c == ':') {   /* no colon glyph: two dots */
            const Glyph *g = find_glyph(k, '.');
            if (g) { glyph(x - 1, y, g, k, body, edge); glyph(x - 1, y - h / 3, g, k, body, edge); x += g->w; } else x += 3;
            continue;
        }
        const Glyph *g = find_glyph(k, c);
        if (!g) { x += space_w(k); continue; }
        glyph(x, y, g, k, body, edge);
        x += g->w;
    }
    return x;
}
/* the same with the game's 1 px dark outline (light text over wood, green or the side map); pa_outline != 0 makes
   pa_text / pa_text_fit draw it too */
u32 pa_outline;
int pa_text_ol(int x, int y, int h, const char *s, u32 body, u32 edge, u32 outline) {
    u32 o = pa_outline; pa_outline = outline;
    int r = pa_text(x, y, h, s, body, edge);
    pa_outline = o; return r;
}
/* the same, cut with "..." to fit w */
int pa_text_fit(int x, int y, int h, int w, const char *s, u32 body, u32 edge) {
    char b[160]; int n = (int)strlen(s);
    if (n > 150) n = 150;
    memcpy(b, s, n); b[n] = 0;
    if (pa_text_w(h, b) <= w) return pa_text(x, y, h, b, body, edge);
    while (n > 0) {
        b[--n] = 0;
        while (n > 0 && b[n - 1] == ' ') b[--n] = 0;
        char t[164]; snprintf(t, sizeof t, "%s...", b);
        if (pa_text_w(h, t) <= w) return pa_text(x, y, h, t, body, edge);
    }
    return x;
}

void pa_init(void) {
    static int done;
    if (done) return;
    done = 1;
    vram = calloc(1024 * 512, 2);
    if (!vram) return;
#if PORT_VERSION_USA
    /* USA: one page at (64, 0) holds PAL's A10301 + A00300 (its sub-screen font differs; the item / event icons are the same) */
    pa_ok = load_rect("SYSTEM/A00014.000", 64, 0, 256, 0, 256) && load_rect("SYSTEM/A00003.GAM", 256, 256, 64, 0, 224) &&
#else
    pa_ok = load_rect("SYSTEM/A10301.000", 64, 0, 64, 0, 256) && load_rect("SYSTEM/A00300.000", 128, 0, 192, 0, 256) &&
            load_rect("SYSTEM/A10003.GAM", 256, 256, 64, 0, 224) &&
#endif
            load_rect("SYSTEM/A00000.GAM", 320, 256, 64, 0, 224) && load_rect("AREA00/CLUT01.GAM", 128, 480, 256, 0, 32) &&
            load_rect(PA_F_SUB, 288, 480, 48, PV_SUBSCREEN_CLUT_IMAGE - PORT_OVL_BASE, 31);
    if (pa_ok) load_icons();
    load_fonts();
    port_printf("[panel] game graphics %s, icons %s\n", pa_ok ? "read" : "missing", icons_ok ? "read" : "missing");
}

/* dialogue idx of a WFM3 message bank as Latin-1 text (its glyphs named through panel_charmap like the fonts above;
   NL -> '\n', the other control codes dropped, an unknown glyph -> '?'); returns the length, -1 when unreadable.
   Used by the item tab for the item sub-screen's own messages (DnSPSUB3's bank: "Can't use this here.", ...). */
int pa_wfm_dialogue(const unsigned char *d, unsigned n, int idx, char *out, int max) {
    if (!d || n < 144 || memcmp(d, "WFM3", 4) || max < 2) return -1;
    unsigned dpt = (unsigned)d[8] | d[9] << 8 | d[10] << 16 | (unsigned)d[11] << 24, nd = rd16(d, 12), ng = rd16(d, 14), k = 0;
    if (idx < 0 || (unsigned)idx >= nd || dpt + 2 * nd > n) return -1;
    for (unsigned p = dpt + rd16(d, dpt + 2 * idx); p + 2 <= n && k + 2 < (unsigned)max; p += 2) {
        unsigned v = rd16(d, p);
        if (v >= 0xFFFE) break;
        if (v >= 0xFFF2) {
            static const unsigned char nargs[12] = { 1, 0, 0, 0, 2, 1, 2, 1, 2, 0, 0, 0 };   /* POSE HALT F4 CHOICE MOVE COLOR VOICE PAUSE BOX CLEAR WAIT NL */
            if (v == 0xFFFD) out[k++] = '\n';
            p += 2 * nargs[v - 0xFFF2];
            continue;
        }
        if (v < 0x8000 || v - 0x8000 >= ng) continue;
        unsigned gp = rd16(d, 144 + 2 * (v - 0x8000)), h, w, sz; int ch = '?';
        if (gp + 8 > n) continue;
        h = rd16(d, gp + 2); w = rd16(d, gp + 4); sz = (w * h + 1) / 2;
        if (gp + 8 + sz <= n) { int c = glyph_char((int)h, (int)w, d + gp + 8, NULL); if (c >= 0) ch = c; }
        out[k++] = (char)ch;
    }
    out[k] = 0;
    return (int)k;
}

/* text drawn at an integer scale sc (1..4), optionally shaded (each colour halved, like PA_SHADE): the h px glyphs (and
   their outline when pa_outline is set) rendered off the canvas at 1x, then every pixel put as an sc x sc block (pixel
   doubled, never resampled).  Returns the end x. */
int pa_text_scaled(int x, int y, int h, const char *s, u32 body, u32 edge, int sc, int shade) {
    int w = pa_text_w(h, s) + 4, hh = h + 6;
    if (w <= 4 || w > 512 || sc < 1 || sc > 4) return x;
    static u32 buf[512 * 32];
    u32 *cv = pa_cv; int cw = pa_cw, ch = pa_ch, c0 = pa_clip_x0, c1 = pa_clip_y0, c2 = pa_clip_x1, c3 = pa_clip_y1;
    memset(buf, 0, sizeof buf);
    pa_cv = buf; pa_cw = w; pa_ch = hh; pa_clip_x0 = pa_clip_y0 = 0; pa_clip_x1 = w; pa_clip_y1 = hh;
    pa_text(2, 3, h, s, body, edge);
    pa_cv = cv; pa_cw = cw; pa_ch = ch; pa_clip_x0 = c0; pa_clip_y0 = c1; pa_clip_x1 = c2; pa_clip_y1 = c3;
    for (int j = 0; j < hh; j++) for (int i = 0; i < w; i++) {
        u32 c = buf[j * w + i];
        if (!(c >> 24)) continue;
        if (shade) c = 0xFF000000u | ((c >> 1) & 0x7F7F7Fu);
        pa_fill(x + sc * (i - 2), y + sc * (j - 3), sc, sc, c);
    }
    return x + sc * (w - 4);
}
