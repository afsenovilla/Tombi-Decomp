/* libgpu replacement and a software PS1 GPU (port milestone M2, step 1).
 *
 * DrawOTag walks the ordering table exactly like GPU DMA channel 2 in linked-list mode and feeds every packet
 * word to a GP0 command decoder.  The decoder rasterises into an emulated 1024x512 16-bit VRAM: flat/gouraud/
 * textured triangles and quads, rectangles and sprites, lines and polylines, fills and VRAM copies, with
 * 4/8/15-bit texture pages, CLUTs, texture windows, the four semi-transparency modes, texture modulation,
 * the drawing area and offset, and the mask bit.  The displayed frame is whatever the current DISPENV points at;
 * port_gpu_vblank() converts it to RGB, hands it to the SDL2 window (runtime/sdl.c) and, with --dump-frames,
 * writes it as PNG (a minimal stored-deflate writer below: no zlib dependency).  Carried over from port/shims/gpu.c
 * to the native-pointer port: the ordering-table links are native pointers below 16 MB (port_otaddr).
 *
 * The rasteriser is one of two backends (gpu_backend.h): the software one below (port_gpu_soft) and the OpenGL one
 * of gpu_gl.c (port_gpu_gl, the default with a window; --renderer soft|gl).  The decoder, VRAM, the libgpu API and
 * the frame dumps are shared; the host-side VRAM operations tell the backend what they wrote and what they read.
 *
 * Written for this project from the public GPU documentation (psx-spx); no emulator code is copied. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include "port_runtime.h"
#include "gpu_backend.h"

typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32;
typedef struct { s16 x, y, w, h; } RECT;

extern const char *port_drawdump_path;
void port_drawdump(const u16 *vram, int tx, int ty, int depth, int cx, int cy, int tpage, int clut,
                   int u0, int v0, int w, int h, int sx, int sy, int sw, int sh, int semi, int kind);

static u16 vram[512][1024];
static int gpu_prims, gpu_lists;
int port_gpu_packets(void) { return gpu_prims; }
unsigned port_ot_signature;          /* hash of the packets of the last DrawOTag (watchdog.c) */
int port_gpu_lists(void) { return gpu_lists; }
u16 *port_vram(void) { return &vram[0][0]; }

/* ------------------------------------------------------------------ backend selection */
const GpuBackend *port_gpu_be = &port_gpu_soft;
int port_renderer = -1;
static int gpu_started;
/* after the window exists (or in headless mode): --renderer gl (the default with a window) tries OpenGL and falls
   back to the software rasteriser when no context can be made; headless runs are software unless gl is asked for */
void port_gpu_start(void) {
    if (gpu_started) return;
    gpu_started = 1;
    int want_gl = port_renderer == 1 || (port_renderer < 0 && port_window);
    if (want_gl && port_gpu_gl.start() == 0) port_gpu_be = &port_gpu_gl;
    else {
        if (want_gl) port_printf("[gpu] OpenGL renderer unavailable: using the software rasteriser\n");
        port_gpu_be = &port_gpu_soft;
        port_gpu_soft.start();
    }
    port_printf("[gpu] renderer: %s\n", port_gpu_be->name);
}
void port_gpu_stop(void) { if (gpu_started && port_gpu_be->stop) port_gpu_be->stop(); }
#define be port_gpu_be

/* ------------------------------------------------------------------ GPU drawing state (GP0 E1..E6) */
GpuState port_gs = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1023, 511, 0, 0, 0, 0 };
#define gs port_gs
static struct { int tx, ty, tp; } tpg;   /* texture page base in VRAM (x multiple of 64, y 0/256); depth 0 = 4-bit CLUT, 1 = 8-bit, 2 = 15-bit */

static void set_drawmode(u32 w) {
    tpg.tx = (w & 15) * 64; tpg.ty = ((w >> 4) & 1) * 256;
    gs.abr = (w >> 5) & 3; tpg.tp = (w >> 7) & 3; if (tpg.tp == 3) tpg.tp = 2;
    gs.dither = (w >> 9) & 1; gs.dfe = (w >> 10) & 1;
}
static void set_texpage16(u32 t) { set_drawmode((gs.dither << 9) | (gs.dfe << 10) | (t & 0x1FF)); }

/* ================================================================== the software backend (port_gpu_soft) */
/* ------------------------------------------------------------------ pixel pipeline */
static inline u16 blend(u16 bg, u16 fg, int mode) {
    int br = bg & 31, bgc = (bg >> 5) & 31, bb = (bg >> 10) & 31;
    int fr = fg & 31, fgc = (fg >> 5) & 31, fb = (fg >> 10) & 31, r, g, b;
    switch (mode) {
    case 0: r = (br + fr) >> 1; g = (bgc + fgc) >> 1; b = (bb + fb) >> 1; break;
    case 1: r = br + fr; g = bgc + fgc; b = bb + fb; break;
    case 2: r = br - fr; g = bgc - fgc; b = bb - fb; break;
    default: r = br + (fr >> 2); g = bgc + (fgc >> 2); b = bb + (fb >> 2); break;
    }
    r = r < 0 ? 0 : r > 31 ? 31 : r; g = g < 0 ? 0 : g > 31 ? 31 : g; b = b < 0 ? 0 : b > 31 ? 31 : b;
    return (u16)(r | (g << 5) | (b << 10) | (fg & 0x8000));
}
/* write one pixel; semi = the primitive is semi-transparent (for textured pixels: only if texel bit 15) */
/* --widescreen (docs/PORT.md "Widescreen"): primitives drawn into a 320-wide display buffer go to wvram instead,
   a side plane where that buffer is ws_pad columns wider on each side (the drawing offset shifted by ws_pad), so
   whatever the game draws past the 4:3 edges is kept.  plot() writes through tgt/tgt_stride: vram when off. */
int port_ws_cull;                    /* the game's screen-X culls widened by this (port_prelude.h PORT_WSX), = port_widescreen */
int port_widescreen;                 /* extra columns on each side of a 320-wide frame; 0 = off (53 = 16:9, 120 = 21:9, 267 = 32:9) */
#define WS_STRIDE 2048
static u16 wvram[512][WS_STRIDE];
static u16 *tgt = &vram[0][0];
static int tgt_stride = 1024;
static void ws_fill(int x, int y, int w, int h, u16 c);
static void ws_mirror_write(int x, int y, int w, int h);
static inline void plot(int x, int y, u16 c, int semi) {
    u16 *d = tgt + y * tgt_stride + x;
    if (gs.check_mask && (*d & 0x8000)) return;
    if (semi) c = blend(*d, c, gs.abr);
    *d = c | (gs.set_mask ? 0x8000 : 0);
}
static inline u16 rgb24to15(u32 c) { return (u16)(((c >> 3) & 31) | (((c >> 11) & 31) << 5) | (((c >> 19) & 31) << 10)); }

/* texel fetch for the current primitive's page/CLUT; returns 0 for "transparent" (raw 0x0000) */
static int tp_x, tp_y, tp_depth, clut_x, clut_y;
static inline u16 texel(int u, int v) {
    u &= 255; v &= 255;
    u = (u & ~(gs.twmx * 8)) | ((gs.twox & gs.twmx) * 8);
    v = (v & ~(gs.twmy * 8)) | ((gs.twoy & gs.twmy) * 8);
    if (tp_depth == 0) {
        u16 w = vram[(tp_y + v) & 511][(tp_x + (u >> 2)) & 1023];
        int i = (w >> ((u & 3) * 4)) & 15;
        return vram[clut_y][(clut_x + i) & 1023];
    } else if (tp_depth == 1) {
        u16 w = vram[(tp_y + v) & 511][(tp_x + (u >> 1)) & 1023];
        int i = (w >> ((u & 1) * 8)) & 255;
        return vram[clut_y][(clut_x + i) & 1023];
    }
    return vram[(tp_y + v) & 511][(tp_x + u) & 1023];
}
static inline u16 modulate(u16 t, int r, int g, int b) {
    int tr = ((t & 31) * r) >> 7, tg = (((t >> 5) & 31) * g) >> 7, tb = (((t >> 10) & 31) * b) >> 7;
    if (tr > 31) tr = 31; if (tg > 31) tg = 31; if (tb > 31) tb = 31;
    return (u16)(tr | (tg << 5) | (tb << 10) | (t & 0x8000));
}
static void soft_use_texture(const GpuTex *t) { tp_x = t->tx; tp_y = t->ty; tp_depth = t->depth; clut_x = t->cx; clut_y = t->cy; }

/* ------------------------------------------------------------------ triangles */
typedef GpuVtx Vtx;

static void soft_tri(const Vtx *a, const Vtx *b, const Vtx *c, int shade, const GpuTex *texp, int semi, int raw) {
    int tex = texp != 0;
    if (tex) soft_use_texture(texp);
    /* edge functions on integer pixel centres, top-left fill rule */
    long long area = (long long)(b->x - a->x) * (c->y - a->y) - (long long)(c->x - a->x) * (b->y - a->y);
    if (area == 0) return;
    if (area < 0) { const Vtx *t = b; b = c; c = t; area = -area; }
    int minx = a->x, maxx = a->x, miny = a->y, maxy = a->y;
    if (b->x < minx) minx = b->x; if (b->x > maxx) maxx = b->x; if (c->x < minx) minx = c->x; if (c->x > maxx) maxx = c->x;
    if (b->y < miny) miny = b->y; if (b->y > maxy) maxy = b->y; if (c->y < miny) miny = c->y; if (c->y > maxy) maxy = c->y;
    if (minx < gs.x1) minx = gs.x1; if (maxx > gs.x2) maxx = gs.x2;
    if (miny < gs.y1) miny = gs.y1; if (maxy > gs.y2) maxy = gs.y2;
    if (minx > maxx || miny > maxy) return;
    /* attribute gradients (float: exact enough for 8-bit colours/UVs) */
    float inv = 1.0f / (float)area;
    #define GRAD(f, dx, dy) \
        float dx = ((float)(b->f - a->f) * (c->y - a->y) - (float)(c->f - a->f) * (b->y - a->y)) * inv; \
        float dy = ((float)(c->f - a->f) * (b->x - a->x) - (float)(b->f - a->f) * (c->x - a->x)) * inv;
    GRAD(r, drdx, drdy) GRAD(g, dgdx, dgdy) GRAD(b, dbdx, dbdy) GRAD(u, dudx, dudy) GRAD(v, dvdx, dvdy)
    #undef GRAD
    /* edge e(p) = (q1-q0) x (p-q0); inside when e > 0, or e == 0 on a top/left edge */
    const Vtx *E[3][2] = { { b, c }, { c, a }, { a, b } };
    int bias[3];
    for (int k = 0; k < 3; k++) {
        int dx = E[k][1]->x - E[k][0]->x, dy = E[k][1]->y - E[k][0]->y;
        int topleft = (dy < 0) || (dy == 0 && dx > 0);
        bias[k] = topleft ? 0 : -1;
    }
    u16 flat = shade ? 0 : rgb24to15((u32)a->r | ((u32)a->g << 8) | ((u32)a->b << 16));
    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            int in = 1;
            for (int k = 0; k < 3 && in; k++) {
                long long e = (long long)(E[k][1]->x - E[k][0]->x) * (y - E[k][0]->y)
                            - (long long)(E[k][1]->y - E[k][0]->y) * (x - E[k][0]->x);
                /* with area > 0 every edge function is positive inside (cyclic cross products) */
                if (e + bias[k] < 0) in = 0;
            }
            if (!in) continue;
            float fx = (float)(x - a->x), fy = (float)(y - a->y);
            int r = a->r, g = a->g, bb = a->b;
            if (shade) {
                r = (int)(a->r + drdx * fx + drdy * fy + 0.5f); g = (int)(a->g + dgdx * fx + dgdy * fy + 0.5f);
                bb = (int)(a->b + dbdx * fx + dbdy * fy + 0.5f);
                if (r < 0) r = 0; if (r > 255) r = 255; if (g < 0) g = 0; if (g > 255) g = 255;
                if (bb < 0) bb = 0; if (bb > 255) bb = 255;
            }
            if (tex) {
                int u = (int)(a->u + dudx * fx + dudy * fy + 0.5f), v = (int)(a->v + dvdx * fx + dvdy * fy + 0.5f);
                u16 t = texel(u, v);
                if (!t) continue;
                if (!raw) t = modulate(t, r, g, bb);
                plot(x, y, t, semi && (t & 0x8000));
            } else {
                u16 c = shade ? rgb24to15((u32)r | ((u32)g << 8) | ((u32)bb << 16)) : flat;
                plot(x, y, c, semi);
            }
        }
    }
}

/* ------------------------------------------------------------------ rectangles, lines, fill, copy */
static void soft_rect(int x0, int y0, int w, int h, u32 col, const GpuTex *texp, int u0, int v0, int semi, int raw) {
    int tex = texp != 0;
    if (tex) soft_use_texture(texp);
    u16 flat = rgb24to15(col);
    int r = col & 255, g = (col >> 8) & 255, b = (col >> 16) & 255;
    for (int j = 0; j < h; j++) {
        int y = y0 + j;
        if (y < gs.y1 || y > gs.y2) continue;
        for (int i = 0; i < w; i++) {
            int x = x0 + i;
            if (x < gs.x1 || x > gs.x2) continue;
            if (tex) {
                u16 t = texel(u0 + i, v0 + j);
                if (!t) continue;
                if (!raw) t = modulate(t, r, g, b);
                plot(x, y, t, semi && (t & 0x8000));
            } else plot(x, y, flat, semi);
        }
    }
}
static void soft_line(int x0, int y0, u32 c0, int x1, int y1, u32 c1, int shade, int semi) {
    int dx = x1 - x0, dy = y1 - y0, adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy;
    int n = adx > ady ? adx : ady;
    for (int i = 0; i <= n; i++) {
        int x = n ? x0 + (dx * i + (dx >= 0 ? n / 2 : -n / 2)) / n : x0;
        int y = n ? y0 + (dy * i + (dy >= 0 ? n / 2 : -n / 2)) / n : y0;
        if (x < gs.x1 || x > gs.x2 || y < gs.y1 || y > gs.y2) continue;
        u32 c = c0;
        if (shade && n) {
            int r = (c0 & 255) + (((int)(c1 & 255) - (int)(c0 & 255)) * i) / n;
            int g = ((c0 >> 8) & 255) + (((int)((c1 >> 8) & 255) - (int)((c0 >> 8) & 255)) * i) / n;
            int b = ((c0 >> 16) & 255) + (((int)((c1 >> 16) & 255) - (int)((c0 >> 16) & 255)) * i) / n;
            c = r | (g << 8) | (b << 16);
        }
        plot(x, y, rgb24to15(c), semi);
    }
}
static void soft_fill(int x, int y, int w, int h, u16 c) {
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) vram[(y + j) & 511][(x + i) & 1023] = c;
}
static void soft_copy(int sx, int sy, int dx, int dy, int w, int h) {
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++)
        vram[(dy + j) & 511][(dx + i) & 1023] = vram[(sy + j) & 511][(sx + i) & 1023];
}
static int soft_start(void) { return 0; }
static void soft_nop_rect(int x, int y, int w, int h) { (void)x; (void)y; (void)w; (void)h; }
static void soft_nop(void) { }
static int soft_present(int dx, int dy, int dw, int dh, int masked) { (void)dx; (void)dy; (void)dw; (void)dh; (void)masked; return 0; }
const GpuBackend port_gpu_soft = { "software", soft_start, soft_nop, soft_tri, soft_rect, soft_line, soft_fill, soft_copy,
                                   soft_nop_rect, soft_nop_rect, soft_nop, soft_present, 0 };

/* ================================================================== the decoder (shared by the backends) */
static void tex_patch_pre(int x, int y, int w, int h);
static void tex_patch_idle(void);
static void fill(u32 col, u32 xy, u32 wh) {
    int x = xy & 0x3F0, y = (xy >> 16) & 0x1FF, w = ((wh & 0x3FF) + 15) & ~15, h = (wh >> 16) & 0x1FF;
    if (w && h) { tex_patch_pre(x, y, w, h); be->fill(x, y, w, h, rgb24to15(col)); ws_fill(x, y, w, h, rgb24to15(col)); }
}
static void copy(u32 s, u32 d, u32 wh) {
    int sx = s & 1023, sy = (s >> 16) & 511, dx = d & 1023, dy = (d >> 16) & 511;
    int w = ((wh & 0xFFFF) - 1) & 1023, h = (((wh >> 16) - 1) & 511); w++; h++;
    be->copy(sx, sy, dx, dy, w, h);
}
/* the GPU drops polygons and lines larger than 1023x511 (the distance between any two vertices) */
static int too_big(const Vtx *v, int n) {
    int minx = v[0].x, maxx = v[0].x, miny = v[0].y, maxy = v[0].y;
    for (int i = 1; i < n; i++) {
        if (v[i].x < minx) minx = v[i].x; if (v[i].x > maxx) maxx = v[i].x;
        if (v[i].y < miny) miny = v[i].y; if (v[i].y > maxy) maxy = v[i].y;
    }
    return maxx - minx > 1023 || maxy - miny > 511;
}
/* host-side VRAM writes (LoadImage & co.) go through here so the backend can mirror them */
static void vram_write_rect(int x, int y, int w, int h, const u16 *s) {
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) vram[(y + j) & 511][(x + i) & 1023] = *s++;
    be->vram_written(x, y, w, h);
    ws_mirror_write(x, y, w, h);
}

/* ------------------------------------------------------------------ GP0 decoder */
static inline int sx11(u32 v) { return ((s32)(v << 21)) >> 21; }

/* widescreen redirection: active while the drawing area is a 320-wide, >= 200-line buffer at (ax, ay) at the displayed x;
   its wide copy sits in wvram at columns 2*ax .. 2*ax + 320 + 2*pad - 1 (so buffers side by side never meet) */
static int ws_on, ws_drew, ws_dx, ws_ax, ws_ay, ws_ah, ws_saved[4];
static u8 cur_drawenv[0x5C], cur_dispenv[0x14];
static int ws_dispx[4], ws_ndispx;          /* every x a DISPENV has shown (Tombi flips between x 384 and 704) */
static struct { int x, y, h; } ws_areas[8]; static int ws_nareas;   /* the buffers that have a wide copy */
static void ws_note_disp(int x) {
    for (int i = 0; i < ws_ndispx; i++) if (ws_dispx[i] == x) return;
    if (ws_ndispx < 4) ws_dispx[ws_ndispx++] = x;
}
/* where a displayed buffer's wide copy starts in wvram: 2*ax while the copies fit side by side (16:9, 21:9);
   wider (32:9, 854 columns) Tombi's two buffers (x 384 and 704) would overlap, so each gets its own 1024-column half
   of the 2048-wide plane instead (by x: below 512 the left half) */
static int ws_base(int ax) {
    if (2 * port_widescreen <= 320) return 2 * ax;
    return ax < 512 ? 0 : 1024;
}
static int ws_area(void) {                 /* (only at a displayed x: a 320-wide off-screen target is a texture) */
    int shown = 0;
    if (!port_widescreen) return 0;
    for (int i = 0; i < ws_ndispx; i++) if (ws_dispx[i] == gs.x1) shown = 1;
    return shown && gs.x2 - gs.x1 + 1 == 320 && gs.y2 - gs.y1 + 1 >= 200 && (be == &port_gpu_soft || be->ws_present);
}
static void ws_begin(void) {
    ws_on = ws_area();
    if (!ws_on) return;
    ws_ax = gs.x1; ws_ay = gs.y1; ws_ah = gs.y2 - gs.y1 + 1;
    {   int i; for (i = 0; i < ws_nareas; i++) if (ws_areas[i].x == ws_ax && ws_areas[i].y == ws_ay) break;
        if (i == ws_nareas && i < 8) { ws_areas[i].x = ws_ax; ws_areas[i].y = ws_ay; ws_areas[i].h = ws_ah; ws_nareas++; } }
    ws_dx = ws_base(gs.x1) - gs.x1 + port_widescreen; ws_drew = 1;
    ws_saved[0] = gs.x1; ws_saved[1] = gs.x2;
    gs.x1 = ws_base(ws_ax); gs.x2 = ws_base(ws_ax) + 320 + 2 * port_widescreen - 1;
    tgt = &wvram[0][0]; tgt_stride = WS_STRIDE; gs.wide = 1;
}
static void ws_end(void) {
    if (!ws_on) return;
    if (gs.x1 == ws_base(ws_ax)) gs.x1 = ws_saved[0];           /* (an E3/E4 inside the packet keeps its new value) */
    if (gs.x2 == ws_base(ws_ax) + 320 + 2 * port_widescreen - 1) gs.x2 = ws_saved[1];
    tgt = &vram[0][0]; tgt_stride = 1024; gs.wide = 0;
    ws_on = 0;
}
/* the 4:3 middle of a wide buffer back into vram (after each ordering table: StoreImage, VRAM copies and textures
   made from the frame read vram), and host writes into a display buffer forward into the wide one */
static void ws_sync_down(void) {
    if (!ws_drew) return;
    ws_drew = 0;
    if (be->ws_copy_down) { be->ws_copy_down(ws_base(ws_ax) + port_widescreen, ws_ay, ws_ax, ws_ay, 320, ws_ah); return; }
    for (int j = 0; j < ws_ah; j++)
        memcpy(&vram[(ws_ay + j) & 511][ws_ax], &wvram[(ws_ay + j) & 511][ws_base(ws_ax) + port_widescreen], 320 * 2);
}
static void ws_mirror_write(int x, int y, int w, int h) {
    for (int k = 0; k < ws_nareas; k++) {
        int ax = ws_areas[k].x, ay = ws_areas[k].y, ah = ws_areas[k].h;
        for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) {
            int vx = x + i, vy = (y + j) & 511;
            if (vx < ax || vx >= ax + 320 || vy < ay || vy >= ay + ah) continue;
            wvram[vy][ws_base(ax) + port_widescreen + vx - ax] = vram[vy][vx & 1023];
        }
        if (be->ws_upload) {
            int x1 = x > ax ? x : ax, x2 = x + w < ax + 320 ? x + w : ax + 320, y1 = y > ay ? y : ay, y2 = y + h < ay + ah ? y + h : ay + ah;
            if (x1 < x2 && y1 < y2) {
                int wx = ws_base(ax) + port_widescreen + x1 - ax;
                be->ws_upload(wx, y1, x2 - x1, y2 - y1, &wvram[y1][wx], WS_STRIDE);
            }
        }
    }
}
/* a GP0 02 / isbg / ClearImage fill that covers a whole display buffer clears the wide copy too */
static void ws_fill(int x, int y, int w, int h, u16 c) {
    if (!port_widescreen || w < 320 || h < 200) return;
    for (int ax = x; ax + 320 <= x + w && ax + 320 <= 1024; ax += 320)
        if (be->ws_fill) be->ws_fill(ws_base(ax), y, 320 + 2 * port_widescreen, h, c); else
        for (int j = 0; j < h; j++) for (int i = 0; i < 320 + 2 * port_widescreen; i++) wvram[(y + j) & 511][ws_base(ax) + i] = c;
}
static void vertex(Vtx *o, u32 w) { o->x = sx11(w) + gs.ox; o->y = sx11(w >> 16) + gs.oy; if (ws_on) o->x += ws_dx; }
static void color(Vtx *o, u32 c) { o->r = c & 255; o->g = (c >> 8) & 255; o->b = (c >> 16) & 255; }

static int unknown_cmds;
/* executes one command; returns the number of words it used (0 = ignore the rest of the packet) */
static int gp0(const u32 *p, int n) {
    u32 cmd = p[0] >> 24;
    if (cmd >= 0x20 && cmd < 0x40) {                         /* polygon */
        int shade = cmd & 0x10, quad = cmd & 8, tex = cmd & 4, semi = cmd & 2, raw = cmd & 1;
        int nv = quad ? 4 : 3, per = 1 + (tex ? 1 : 0) + (shade ? 1 : 0);
        int need = 1 + nv * per - (shade ? 1 : 0);
        if (n < need) return 0;
        Vtx v[4]; int k = 1; u32 clut = 0, tpage = 0; GpuTex t;
        for (int i = 0; i < nv; i++) {
            if (i == 0 || !shade) color(&v[i], p[0]); else color(&v[i], p[k++]);
            vertex(&v[i], p[k++]);
            if (tex) {
                u32 uv = p[k++];
                v[i].u = uv & 255; v[i].v = (uv >> 8) & 255;
                if (i == 0) clut = uv >> 16;
                if (i == 1) tpage = uv >> 16;
            } else v[i].u = v[i].v = 0;
        }
        if (tex) {
            set_texpage16(tpage);
            t.tx = tpg.tx; t.ty = tpg.ty; t.depth = tpg.tp; t.cx = (clut & 0x3F) * 16; t.cy = (clut >> 6) & 511;
        }
        if (tex && port_drawdump_path) {                     /* --dump-draws (drawdump.c) */
            int u0 = 255, u1 = 0, v0 = 255, v1 = 0, x0 = 1 << 20, x1 = -(1 << 20), y0 = 1 << 20, y1 = -(1 << 20);
            for (int i = 0; i < nv; i++) {
                if (v[i].u < u0) u0 = v[i].u; if (v[i].u > u1) u1 = v[i].u; if (v[i].v < v0) v0 = v[i].v; if (v[i].v > v1) v1 = v[i].v;
                if (v[i].x < x0) x0 = v[i].x; if (v[i].x > x1) x1 = v[i].x; if (v[i].y < y0) y0 = v[i].y; if (v[i].y > y1) y1 = v[i].y;
            }
            port_drawdump(&vram[0][0], t.tx, t.ty, t.depth, t.cx, t.cy, tpage, clut, u0, v0, u1 - u0 + 1, v1 - v0 + 1, x0, y0, x1 - x0, y1 - y0, semi ? 1 : 0, quad ? 1 : 0);
        }
        if (tex && raw) shade = 0;
        if (!too_big(v, 3)) be->tri(&v[0], &v[1], &v[2], shade, tex ? &t : 0, semi, raw);
        if (quad && !too_big(v + 1, 3)) be->tri(&v[1], &v[2], &v[3], shade, tex ? &t : 0, semi, raw);
        return need;
    }
    if (cmd >= 0x40 && cmd < 0x60) {                         /* line / polyline */
        int shade = cmd & 0x10, poly = cmd & 8, semi = cmd & 2;
        int k = 1; u32 c0 = p[0] & 0xFFFFFF; Vtx a;
        if (n < 3) return 0;
        vertex(&a, p[k++]);
        for (;;) {
            u32 c1 = c0; Vtx b;
            if (shade) { if (k >= n) break; c1 = p[k] & 0xFFFFFF; if (poly && (p[k] & 0xF000F000u) == 0x50005000u) break; k++; }
            if (k >= n) break;
            if (poly && (p[k] & 0xF000F000u) == 0x50005000u) break;
            vertex(&b, p[k++]);
            if (!(b.x - a.x > 1023 || a.x - b.x > 1023 || b.y - a.y > 511 || a.y - b.y > 511))
                be->line(a.x, a.y, c0, b.x, b.y, c1, shade, semi);
            a = b; c0 = c1;
            if (!poly) break;
        }
        return poly ? n : k;
    }
    if (cmd >= 0x60 && cmd < 0x80) {                         /* rectangle / sprite */
        int size = (cmd >> 3) & 3, tex = cmd & 4, semi = cmd & 2, raw = cmd & 1;
        int need = 2 + (tex ? 1 : 0) + (size == 0 ? 1 : 0);
        if (n < need) return 0;
        Vtx a; vertex(&a, p[1]);
        int k = 2, u = 0, v = 0, w, h; GpuTex t;
        if (tex) {
            u32 uv = p[k++]; u = uv & 255; v = (uv >> 8) & 255;
            t.cx = ((uv >> 16) & 0x3F) * 16; t.cy = (uv >> 22) & 511;
            t.tx = tpg.tx; t.ty = tpg.ty; t.depth = tpg.tp;
        }
        if (size == 0) { w = p[k] & 0x3FF; h = (p[k] >> 16) & 0x1FF; k++; }
        else { w = h = size == 1 ? 1 : size == 2 ? 8 : 16; }
        if (tex && port_drawdump_path && w && h)            /* --dump-draws (drawdump.c) */
            port_drawdump(&vram[0][0], t.tx, t.ty, t.depth, t.cx, t.cy, (tpg.tx >> 6) | (tpg.ty >> 8) << 4 | tpg.tp << 7, (p[2] >> 16) & 0xFFFF, u, v, w, h, a.x, a.y, w, h, semi ? 1 : 0, 2);
        if (w && h) be->rect(a.x, a.y, w, h, p[0] & 0xFFFFFF, tex ? &t : 0, u, v, semi, raw);
        return need;
    }
    switch (cmd) {
    case 0x00: case 0x01: return 1;                          /* nop, cache clear */
    case 0x02: if (n < 3) return 0; fill(p[0] & 0xFFFFFF, p[1], p[2]); return 3;
    case 0xE1: set_drawmode(p[0]); return 1;
    case 0xE2: gs.twmx = p[0] & 31; gs.twmy = (p[0] >> 5) & 31; gs.twox = (p[0] >> 10) & 31; gs.twoy = (p[0] >> 15) & 31; return 1;
    case 0xE3: gs.x1 = p[0] & 1023; gs.y1 = (p[0] >> 10) & 511; return 1;
    case 0xE4: gs.x2 = p[0] & 1023; gs.y2 = (p[0] >> 10) & 511; return 1;
    case 0xE5: gs.ox = sx11(p[0]); gs.oy = sx11(p[0] >> 11); return 1;
    case 0xE6: gs.set_mask = p[0] & 1; gs.check_mask = (p[0] >> 1) & 1; return 1;
    }
    if (cmd >= 0x80 && cmd < 0xA0) { if (n < 4) return 0; copy(p[1], p[2], p[3]); return 4; }
    if (cmd >= 0xA0 && cmd < 0xC0) {                         /* CPU -> VRAM inside a packet */
        if (n < 3) return 0;
        int x = p[1] & 1023, y = (p[1] >> 16) & 511, w = p[2] & 0xFFFF, h = p[2] >> 16, i = 0;
        const u16 *s = (const u16 *)(p + 3); int avail = (n - 3) * 2;
        if (w && h) tex_patch_pre(x, y, w, h);
        for (int j = 0; j < h; j++) for (int q = 0; q < w; q++, i++) if (i < avail) vram[(y + j) & 511][(x + q) & 1023] = s[i];
        if (w && h) be->vram_written(x, y, w, h);
        return n;
    }
    if (cmd >= 0xC0 && cmd < 0xE0) return n;                 /* VRAM -> CPU: nothing to do */
    if (cmd >= 0xE7 || (cmd >= 0x03 && cmd < 0x1F)) return 1; /* E7-FF, 03-1E: the GPU ignores them (one word) */
    if (unknown_cmds++ < 20) port_printf("[gpu] unknown GP0 command %02x (frame %d)\n", cmd, port_frame);
    return 0;                                                 /* the rest of the packet is dropped, no state is touched */
}

static void packet(const u32 *p, int n) {
    while (n > 0) {
        ws_begin();
        int used = gp0(p, n);
        ws_end();
        if (used <= 0) break;
        p += used; n -= used;
    }
}

/* ------------------------------------------------------------------ libgpu API */
int GetGraphType(void) { return 0; }          /* retail consoles: 1 MB VRAM GPU */
int GetGraphDebug(void) { return 0; }
int SetGraphDebug(int l) { return 0; }
int ResetGraph(int mode) { return 0; }
static int disp_mask;
void SetDispMask(int m) { disp_mask = m; }
int DrawSync(int mode) { port_trace("DrawSync", mode); return 0; }
void *DrawSyncCallback(void *f) { return 0; }

/* DRAWENV: RECT clip; s16 ofs[2]; RECT tw; u16 tpage; u8 dtd, dfe, isbg, r0, g0, b0; DR_ENV dr_env (cur_drawenv above) */
void *PutDrawEnv(void *env) {
    u8 *e = env; RECT *clip = (RECT *)e, *tw = (RECT *)(e + 12); s16 *ofs = (s16 *)(e + 8);
    memcpy(cur_drawenv, env, sizeof cur_drawenv);
    gs.x1 = clip->x < 0 ? 0 : clip->x; gs.y1 = clip->y < 0 ? 0 : clip->y;
    gs.x2 = clip->x + clip->w - 1; gs.y2 = clip->y + clip->h - 1;
    if (gs.x2 > 1023) gs.x2 = 1023; if (gs.y2 > 511) gs.y2 = 511;
    gs.ox = ofs[0]; gs.oy = ofs[1];
    gs.twmx = (-tw->w & 0xFF) >> 3; gs.twmy = (-tw->h & 0xFF) >> 3; gs.twox = (tw->x >> 3) & 31; gs.twoy = (tw->y >> 3) & 31;
    if (!tw->w) gs.twmx = 0; if (!tw->h) gs.twmy = 0;
    set_drawmode((*(u16 *)(e + 20) & 0x9FF) | (e[22] ? 0x200 : 0) | (e[23] ? 0x400 : 0));
    if (e[24] && gs.x2 >= gs.x1 && gs.y2 >= gs.y1)      /* isbg: clear the drawing area */
    {   u16 bg = rgb24to15((u32)e[25] | ((u32)e[26] << 8) | ((u32)e[27] << 16));
        be->fill(gs.x1, gs.y1, gs.x2 - gs.x1 + 1, gs.y2 - gs.y1 + 1, bg);
        ws_fill(gs.x1, gs.y1, gs.x2 - gs.x1 + 1, gs.y2 - gs.y1 + 1, bg); }
    return env;
}
void *GetDrawEnv(void *env) { memcpy(env, cur_drawenv, sizeof cur_drawenv); return env; }
void *PutDispEnv(void *env) { memcpy(cur_dispenv, env, sizeof cur_dispenv); if (port_widescreen) ws_note_disp(((RECT *)env)->x & 1023); return env; }
void *GetDispEnv(void *env) { memcpy(env, cur_dispenv, sizeof cur_dispenv); return env; }

/* --patch-tex FILE (mods: button prompts, docs/PORT.md "Mods"; Spanish redraws, tools/gfx_es): VRAM rects re-applied
   after every LoadImage that overlaps them, so a texture the game uploads (a font/HUD page) shows our pixels instead.
   Repeatable (up to 4 files).  File: "TBTX", u32 count, then per entry u16 x, y, w, h (VRAM words) followed by w*h u16 words.
   "TBTK" (tools/gfx_es redraws): each entry also has u16 kx, ky, kw, kh and u32 hash after x, y, w, h: a key rect inside
   the original graphic the redraw replaces and the FNV-1a (bytes, row by row) of its original texels.  Such a rect is not
   tied to uploads: it is written only while it is being drawn, i.e. when port_tex_patch_has() (asked by the fixes.txt rule
   that draws the redraw) finds the key in VRAM holding exactly the original texels (the owning page is resident).  What
   it covered is saved first and put back as soon as the game writes over the rect or the key, or the rule stops asking
   (a few frames after the screen ends), so the screens that share that VRAM (the title...) never see our pixels. */
const char *port_patch_tex_path;
const char *port_patch_tex_paths[4];
int port_patch_tex_count;
static struct { u16 x, y, w, h, kx, ky, kw, kh; u32 khash; u8 keyed, resident; int used; const u16 *data; u16 *saved; } tex_patches[64];
static int n_tex_patches, tex_patch_loaded, tex_patch_resident;
static u8 tex_patch_buf[512 * 1024];
static u32 tex_patch_used;
static void tex_patch_load_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { port_printf("[gpu] cannot open %s\n", path); return; }
    u8 *buf = tex_patch_buf + tex_patch_used;
    int n = (int)fread(buf, 1, sizeof tex_patch_buf - tex_patch_used, f), before = n_tex_patches;
    fclose(f);
    int keyed = n >= 8 && !memcmp(buf, "TBTK", 4);
    if (n < 8 || (!keyed && memcmp(buf, "TBTX", 4))) { port_printf("[gpu] %s: not a TBTX file\n", path); return; }
    u32 count, off = 8, hdr = keyed ? 20 : 8;
    memcpy(&count, buf + 4, 4);
    for (u32 i = 0; i < count && n_tex_patches < 64; i++) {
        if (off + hdr > (u32)n) break;
        u16 h[8]; u32 kh = 0;
        memcpy(h, buf + off, keyed ? 16 : 8);
        if (keyed) memcpy(&kh, buf + off + 16, 4);
        u32 bytes = (u32)h[2] * h[3] * 2;
        if (off + hdr + bytes > (u32)n) break;
        if (keyed && !(tex_patches[n_tex_patches].saved = (u16 *)malloc(bytes ? bytes : 1))) break;
        tex_patches[n_tex_patches].x = h[0]; tex_patches[n_tex_patches].y = h[1];
        tex_patches[n_tex_patches].w = h[2]; tex_patches[n_tex_patches].h = h[3];
        tex_patches[n_tex_patches].keyed = (u8)keyed;
        if (keyed) {
            tex_patches[n_tex_patches].kx = h[4]; tex_patches[n_tex_patches].ky = h[5];
            tex_patches[n_tex_patches].kw = h[6]; tex_patches[n_tex_patches].kh = h[7];
            tex_patches[n_tex_patches].khash = kh;
        }
        tex_patches[n_tex_patches].data = (const u16 *)(buf + off + hdr);
        n_tex_patches++;
        off += hdr + bytes;
    }
    tex_patch_used += (off + 3) & ~3u;
    port_printf("[gpu] %s: %d texture patches\n", path, n_tex_patches - before);
}
static void tex_patch_load(void) {
    tex_patch_loaded = 1;
    for (int i = 0; i < port_patch_tex_count; i++) tex_patch_load_file(port_patch_tex_paths[i]);
}
static int rect_overlap(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {
    return !(ax >= bx + bw || ax + aw <= bx || ay >= by + bh || ay + ah <= by);
}
/* a keyed rect goes: what it covered comes back */
static void tex_patch_evict(int i) {
    tex_patches[i].resident = 0;
    tex_patch_resident--;
    vram_write_rect(tex_patches[i].x, tex_patches[i].y, tex_patches[i].w, tex_patches[i].h, tex_patches[i].saved);
}
/* 1 when a --patch-tex rect starting at VRAM word (x, y) is loaded and in VRAM: port/fixes.txt rules that draw a redrawn
   sprite ask it every frame they draw it, so the game keeps the original sprite otherwise.  A keyed rect is put in VRAM
   here, only while its key holds the original texels. */
int port_tex_patch_has(int x, int y) {
    if (!port_patch_tex_count) return 0;
    if (!tex_patch_loaded) tex_patch_load();
    for (int i = 0; i < n_tex_patches; i++) {
        if (tex_patches[i].x != x || tex_patches[i].y != y) continue;
        if (!tex_patches[i].keyed) return 1;
        if (!tex_patches[i].resident) {
            int kx = tex_patches[i].kx, ky = tex_patches[i].ky, kw = tex_patches[i].kw, kh = tex_patches[i].kh;
            int px = tex_patches[i].x, py = tex_patches[i].y, pw = tex_patches[i].w, ph = tex_patches[i].h;
            u32 hh = 2166136261u;
            be->vram_read(kx, ky, kw, kh);
            for (int j = 0; j < kh; j++) for (int q = 0; q < kw; q++) {
                u16 v = vram[(ky + j) & 511][(kx + q) & 1023];
                hh = (hh ^ (v & 255)) * 16777619u; hh = (hh ^ (v >> 8)) * 16777619u;
            }
            if (hh != tex_patches[i].khash) return 0;                /* the original graphic's page is not in VRAM */
            be->vram_read(px, py, pw, ph);
            for (int j = 0; j < ph; j++) for (int q = 0; q < pw; q++) tex_patches[i].saved[j * pw + q] = vram[(py + j) & 511][(px + q) & 1023];
            vram_write_rect(px, py, pw, ph, tex_patches[i].data);
            tex_patches[i].resident = 1;
            tex_patch_resident++;
        }
        tex_patches[i].used = port_frame;
        return 1;
    }
    return 0;
}
/* before the game writes VRAM (x, y, w, h) (LoadImage, fill, copy, GP0 upload): a keyed rect it touches (or whose key it
   touches) goes first, so the game's pixels land on what was there */
static void tex_patch_pre(int x, int y, int w, int h) {
    if (!tex_patch_resident) return;
    for (int i = 0; i < n_tex_patches; i++)
        if (tex_patches[i].resident &&
            (rect_overlap(x, y, w, h, tex_patches[i].x, tex_patches[i].y, tex_patches[i].w, tex_patches[i].h) ||
             rect_overlap(x, y, w, h, tex_patches[i].kx, tex_patches[i].ky, tex_patches[i].kw, tex_patches[i].kh))) tex_patch_evict(i);
}
/* DrawOTag: keyed rects no rule has drawn for a few frames go (the screen that showed them is over) */
static void tex_patch_idle(void) {
    if (!tex_patch_resident) return;
    for (int i = 0; i < n_tex_patches; i++)
        if (tex_patches[i].resident && port_frame - tex_patches[i].used > 2) tex_patch_evict(i);
}
/* after LoadImage: plain TBTX rects are re-applied over any upload that touches them */
static void tex_patch_apply(int x, int y, int w, int h) {
    if (!port_patch_tex_count) return;
    if (!tex_patch_loaded) tex_patch_load();
    for (int i = 0; i < n_tex_patches; i++) {
        int px = tex_patches[i].x, py = tex_patches[i].y, pw = tex_patches[i].w, ph = tex_patches[i].h;
        if (!tex_patches[i].keyed && rect_overlap(x, y, w, h, px, py, pw, ph)) vram_write_rect(px, py, pw, ph, tex_patches[i].data);
    }
}
int LoadImage(RECT *r, u32 *p) {
    if (r->w <= 0 || r->h <= 0) return 0;
    tex_patch_pre(r->x, r->y, r->w, r->h);
    vram_write_rect(r->x, r->y, r->w, r->h, (const u16 *)p);
    tex_patch_apply(r->x, r->y, r->w, r->h);
    return 0;
}
int StoreImage(RECT *r, u32 *p) {
    u16 *d = (u16 *)p;
    if (r->w <= 0 || r->h <= 0) return 0;
    be->vram_read(r->x, r->y, r->w, r->h);
    for (int j = 0; j < r->h; j++) for (int i = 0; i < r->w; i++) *d++ = vram[(r->y + j) & 511][(r->x + i) & 1023];
    return 0;
}
/* libgpu LoadClut / LoadClut2: upload a 256- / 16-entry CLUT row and return its CLUT id (GetClut) */
u16 LoadClut(u32 *clut, int x, int y) {
    RECT r; r.x = x; r.y = y; r.w = 256; r.h = 1;
    LoadImage(&r, clut);
    return (u16)((y << 6) | ((x >> 4) & 0x3F));
}
u16 LoadClut2(u32 *clut, int x, int y) {
    RECT r; r.x = x; r.y = y; r.w = 16; r.h = 1;
    LoadImage(&r, clut);
    return (u16)((y << 6) | ((x >> 4) & 0x3F));
}
int MoveImage(RECT *r, int x, int y) {
    if (r->w <= 0 || r->h <= 0) return 0;
    tex_patch_pre(x & 1023, y & 511, r->w, r->h);
    be->copy(r->x & 1023, r->y & 511, x & 1023, y & 511, r->w, r->h);
    return 0;
}
int ClearImage(RECT *r, u8 R, u8 G, u8 B) {
    u16 c = (R >> 3) | ((G >> 3) << 5) | ((B >> 3) << 10);
    if (r->w <= 0 || r->h <= 0) return 0;
    tex_patch_pre(r->x & 1023, r->y & 511, r->w, r->h);
    be->fill(r->x & 1023, r->y & 511, r->w, r->h, c);
    ws_fill(r->x & 1023, r->y & 511, r->w, r->h, c);
    return 0;
}
int ClearImage2(RECT *r, u8 R, u8 G, u8 B) { return ClearImage(r, R, G, B); }
u32 *ClearOTagR(u32 *ot, int n) {
    for (int i = n - 1; i > 0; i--) ot[i] = (u32)&ot[i - 1] & 0xFFFFFF;   /* port_otlink: below 16 MB (port_prelude.h) */
    ot[0] = 0xFFFFFF;
    return ot;
}
u32 *ClearOTag(u32 *ot, int n) {
    for (int i = 0; i < n - 1; i++) ot[i] = (u32)&ot[i + 1] & 0xFFFFFF;
    ot[n - 1] = 0xFFFFFF;
    return ot;
}
int port_dump_packets;   /* --dump-packets FRAME: print every GP0 packet drawn in that vblank */
/* GPU DMA linked-list walk: every node is (len << 24 | next), followed by len GP0 words */
unsigned long long port_prof_draw_us;   /* time spent walking + rasterizing ordering tables (the fps line, shims/psyq.c) */
void DrawOTag(u32 *ot) {
    u32 *p = ot; int n = 0;
    unsigned h = 2166136261u;
    unsigned long long t0 = port_time_us();
    tex_patch_idle();
    gpu_lists++;
    while (n < 200000) {
        u32 tag = *p, next = tag & 0xFFFFFF, len = tag >> 24;
        n++;
        if (len) {
            if (port_dump_packets && port_frame == port_dump_packets) {
                port_printf("PK@%p", (void *)p);
                for (u32 k = 1; k <= len; k++) port_printf(" %08x", p[k]);
                port_printf("\n");
            }
            packet(p + 1, len);
            for (u32 k = 1; k <= len; k++) h = h * 16777619u ^ p[k];     /* the watchdog's "same picture every frame" test */
        }
        if (next == 0xFFFFFF) break;
        p = (u32 *)next;                     /* a native pointer: the whole data image sits below 16 MB */
    }
    gpu_prims += n;
    ws_sync_down();
    port_ot_signature = h * 31u + (unsigned)n;
    be->list_end();
    port_prof_draw_us += port_time_us() - t0;
    if (port_verbose > 1) port_printf("[port] DrawOTag %p: %d packets\n", ot, n);
}
void DrawOTagEnv(u32 *ot, void *env) { PutDrawEnv(env); DrawOTag(ot); }
void DrawPrim(void *p) { u32 *w = p; if (w[0] >> 24) { packet(w + 1, w[0] >> 24); be->list_end(); } }

/* libgpu helpers that are not in LIB_KEEP (their Psy-Q C ports clash with the shared headers) */
u32 get_mode(int dfe, int dtd, u16 tpage) {
    return 0xE1000000u | (dtd ? 0x200 : 0) | (dfe ? 0x400 : 0) | (tpage & 0x9FF);
}
u32 get_cs(s16 x, s16 y) { x = x < 0 ? 0 : x > 1023 ? 1023 : x; y = y < 0 ? 0 : y > 1023 ? 1023 : y; return 0xE3000000u | ((y & 0x3FF) << 10) | (x & 0x3FF); }
u32 get_ce(s16 x, s16 y) { x = x < 0 ? 0 : x > 1023 ? 1023 : x; y = y < 0 ? 0 : y > 1023 ? 1023 : y; return 0xE4000000u | ((y & 0x3FF) << 10) | (x & 0x3FF); }
u32 get_ofs(s16 x, s16 y) { return 0xE5000000u | ((y & 0x7FF) << 11) | (x & 0x7FF); }
u32 get_tw(RECT *r) {
    if (!r) return 0;
    u32 a = (-r->w & 0xFF) >> 3, b = (-r->h & 0xFF) >> 3, c = (r->x >> 3) & 0x1F, d = (r->y >> 3) & 0x1F;
    return 0xE2000000u | (d << 15) | (c << 10) | (b << 5) | a;
}
typedef struct { u32 tag; u32 code[2]; } DR_MODE;
void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, RECT *tw) {
    p->tag = (p->tag & 0xFFFFFF) | (2u << 24);
    p->code[0] = get_mode(dfe, dtd, tpage);
    p->code[1] = get_tw(tw);
}
typedef struct { u32 tag; u32 code[1]; } DR_AREA;
void SetDrawArea(DR_AREA *p, RECT *r) { p->tag = (p->tag & 0xFFFFFF) | (2u << 24); p->code[0] = get_cs(r->x, r->y); }
void SetDrawOffset(DR_AREA *p, u16 *ofs) { p->tag = (p->tag & 0xFFFFFF) | (1u << 24); p->code[0] = get_ofs(ofs[0], ofs[1]); }
void SetTexWindow(DR_AREA *p, RECT *tw) { p->tag = (p->tag & 0xFFFFFF) | (1u << 24); p->code[0] = get_tw(tw); }

/* ------------------------------------------------------------------ display + PNG frame dumps */
const char *port_dump_dir;
int port_dump_every = 10;
int port_dump_vram;

static u32 crc_tab[256];
static u32 crc32(u32 c, const u8 *p, int n) {
    if (!crc_tab[1]) for (u32 i = 0; i < 256; i++) { u32 k = i; for (int j = 0; j < 8; j++) k = k & 1 ? 0xEDB88320u ^ (k >> 1) : k >> 1; crc_tab[i] = k; }
    c = ~c; while (n--) c = crc_tab[(c ^ *p++) & 255] ^ (c >> 8); return ~c;
}
static void be32(u8 *p, u32 v) { p[0] = v >> 24; p[1] = v >> 16; p[2] = v >> 8; p[3] = v; }
static void chunk(FILE *fd, const char *type, const u8 *data, u32 n) {
    u8 h[8]; be32(h, n); memcpy(h + 4, type, 4);
    fwrite(h, 1, 8, fd);
    u32 c = crc32(0, h + 4, 4);
    if (n) { fwrite(data, 1, n, fd); c = crc32(c, data, n); }
    be32(h, c); fwrite(h, 1, 4, fd);
}
/* RGB8 PNG with stored (uncompressed) deflate blocks: no zlib needed */
static u8 *png_buf, *raw_rows; static size_t png_cap;     /* grown for the --internal-res frames */
static int write_png(const char *path, const u8 *rgb, int w, int h) {
    size_t need = (size_t)(w * 3 + 1) * h;
    if (need > png_cap) {
        free(png_buf); free(raw_rows);
        png_buf = (u8 *)malloc(need + need / 65535 * 5 + 65536); raw_rows = (u8 *)malloc(need);
        png_cap = png_buf && raw_rows ? need : 0;
        if (!png_cap) return -1;
    }
    FILE *fd = fopen(path, "wb");
    if (!fd) return -1;
    static const u8 sig[8] = { 137, 'P', 'N', 'G', 13, 10, 26, 10 };
    fwrite(sig, 1, 8, fd);
    u8 ihdr[13]; be32(ihdr, w); be32(ihdr + 4, h); ihdr[8] = 8; ihdr[9] = 2; ihdr[10] = ihdr[11] = ihdr[12] = 0;
    chunk(fd, "IHDR", ihdr, 13);
    int stride = w * 3 + 1, total = stride * h;
    for (int y = 0; y < h; y++) { raw_rows[y * stride] = 0; memcpy(raw_rows + y * stride + 1, rgb + y * w * 3, w * 3); }
    u8 *o = png_buf; *o++ = 0x78; *o++ = 0x01;
    u32 a = 1, b = 0;
    for (int i = 0; i < total; i++) { a = (a + raw_rows[i]) % 65521; b = (b + a) % 65521; }
    for (int off = 0; off < total || off == 0;) {
        int n = total - off > 65535 ? 65535 : total - off;
        *o++ = off + n >= total; *o++ = n & 255; *o++ = n >> 8; *o++ = ~n & 255; *o++ = (~n >> 8) & 255;
        memcpy(o, raw_rows + off, n); o += n; off += n;
        if (!n) break;
    }
    be32(o, (b << 16) | a); o += 4;
    chunk(fd, "IDAT", png_buf, (u32)(o - png_buf));
    chunk(fd, "IEND", 0, 0);
    fclose(fd);
    return 0;
}
static u8 frame_rgb[1024 * 512 * 3];
/* the displayed area has a wide copy (--widescreen) */
static int ws_frame(const RECT *d, int W, int rgb24) {
    if (!port_widescreen || W != 320 || rgb24) return 0;
    for (int k = 0; k < ws_nareas; k++)
        if (ws_areas[k].x == (d->x & 1023) && ws_areas[k].y <= (d->y & 511) && (d->y & 511) < ws_areas[k].y + ws_areas[k].h) return 1;
    return 0;
}
/* converts the displayed area (DISPENV.disp, 15-bit or 24-bit mode) to RGB8; returns its size */
int port_gpu_frame(u8 *rgb, int *w, int *h) {
    RECT *d = (RECT *)cur_dispenv;
    int rgb24 = cur_dispenv[0x11];
    int W = d->w > 0 ? d->w : 320, H = d->h > 0 ? d->h : 240;
    if (W > 1024) W = 1024; if (H > 512) H = 512;
    if (ws_frame(d, W, rgb24)) {
        int bx = ws_base(d->x & 1023), WW = 320 + 2 * port_widescreen;
        if (be->ws_read && disp_mask) be->ws_read(bx, d->y & 511, WW, H, &wvram[d->y & 511][bx], WS_STRIDE);
        for (int y = 0; y < H; y++) for (int x = 0; x < WW; x++) {
            u8 *o = rgb + (y * WW + x) * 3;
            u16 c = disp_mask ? wvram[(d->y + y) & 511][bx + x] : 0;
            o[0] = (c & 31) << 3 | (c & 31) >> 2; o[1] = ((c >> 5) & 31) << 3 | ((c >> 5) & 31) >> 2;
            o[2] = ((c >> 10) & 31) << 3 | ((c >> 10) & 31) >> 2;
        }
        *w = WW; *h = H;
        return 0;
    }
    if (disp_mask) be->vram_read(d->x & 1023, d->y & 511, rgb24 ? W * 3 / 2 : W, H);
    for (int y = 0; y < H; y++) for (int x = 0; x < W; x++) {
        u8 *o = rgb + (y * W + x) * 3;
        if (!disp_mask) { o[0] = o[1] = o[2] = 0; continue; }
        if (rgb24) {
            const u8 *row = (const u8 *)&vram[(d->y + y) & 511][0];
            int bx = (d->x * 2 + x * 3) & 2047;
            o[0] = row[bx]; o[1] = row[(bx + 1) & 2047]; o[2] = row[(bx + 2) & 2047];
        } else {
            u16 c = vram[(d->y + y) & 511][(d->x + x) & 1023];
            o[0] = (c & 31) << 3 | (c & 31) >> 2; o[1] = ((c >> 5) & 31) << 3 | ((c >> 5) & 31) >> 2;
            o[2] = ((c >> 10) & 31) << 3 | ((c >> 10) & 31) >> 2;
        }
    }
    *w = W; *h = H;
    return 0;
}
static void path_for(char *buf, const char *kind) { snprintf(buf, 512, "%s/%s%06d.png", port_dump_dir, kind, port_frame); }
/* the window present of the displayed frame (port_gpu_vblank, and --fps60 when it has no interpolated frame) */
static void present_window(void) {
    {
        RECT *d = (RECT *)cur_dispenv;
        int w = d->w > 0 ? d->w : 320, h = d->h > 0 ? d->h : 240;
        if (w > 1024) w = 1024; if (h > 512) h = 512;
        /* the GL backend blits the display area itself; the software one goes through RGB24 and an SDL_Renderer;
           the 24-bit mode (MDEC movies) is read out as RGB24 on both and shown by the backend's present_rgb when it
           has one (GL: its own texture; an SDL_Renderer on the GL window would be a second device on it) */
        if (cur_dispenv[0x11]) {
            port_gpu_frame(frame_rgb, &w, &h);
            if (!be->present_rgb || !be->present_rgb(frame_rgb, w, h)) port_window_frame(frame_rgb, w, h);
        } else if (be->ws_present && disp_mask && ws_frame(d, w, 0) && be->ws_present(ws_base(d->x & 1023), d->y & 511, 320 + 2 * port_widescreen, h)) {
        } else if (!be->present(d->x & 1023, d->y & 511, w, h, !disp_mask)) {
            port_gpu_frame(frame_rgb, &w, &h);
            port_window_frame(frame_rgb, w, h);
        }
    }
}

/* --fps60 (experimental, OpenGL): the game still runs one logic step per 50 Hz vblank; the presents happen on their
   own clock (the monitor's refresh, or 60 Hz) while psyq.c's pacing waits, and show the last displayed frame
   re-drawn with every primitive's vertices interpolated from where the matching primitive of the frame displayed
   one vblank earlier was (gpu_gl.c "fps60").  One logic frame of extra display latency; menus, movies and static
   screens repeat the displayed frame.  Off (the default): nothing below runs. */
int port_fps60;
extern int port_gl_fps60_vblank(int x, int y, int w, int h, int wide, int plain);
extern int port_gl_fps60_render(float alpha);
extern int port_gl_fps60_show(void);
extern int port_gl_fps60_read(u8 *rgb, int *w, int *h);
static void fps60_vblank(void) {
    RECT *d = (RECT *)cur_dispenv;
    int w = d->w > 0 ? d->w : 320, h = d->h > 0 ? d->h : 240, rgb24 = cur_dispenv[0x11], wide;
    if (w > 1024) w = 1024; if (h > 512) h = 512;
    wide = be->ws_present && ws_frame(d, w, rgb24);
    if (wide) port_gl_fps60_vblank(ws_base(d->x & 1023), d->y & 511, 320 + 2 * port_widescreen, h, 1, !disp_mask);
    else port_gl_fps60_vblank(d->x & 1023, d->y & 511, w, h, 0, rgb24 || !disp_mask);
}
static void fps60_present(double alpha) {
    static const char *dump_dir; static int dump_from = -1, dump_to, dump_n;
    int interp = port_gl_fps60_render((float)alpha);
    if (port_window && (!interp || !port_gl_fps60_show())) present_window();
    if (dump_from < 0) {      /* PORT_FPS60_DUMP=DIR:FROM:TO (test hook): every present during vblanks FROM..TO as present_N.png */
        const char *e = getenv("PORT_FPS60_DUMP"); static char buf[400];
        dump_from = 0;
        if (e) { snprintf(buf, sizeof buf, "%s", e); char *c = strchr(buf, ':');
                 if (c) { *c = 0; dump_dir = buf; sscanf(c + 1, "%d:%d", &dump_from, &dump_to); } }
    }
    if (dump_dir && port_frame >= dump_from && port_frame <= dump_to) {
        char path[512]; int w, h;
        if (!interp || port_gl_fps60_read(frame_rgb, &w, &h)) port_gpu_frame(frame_rgb, &w, &h);
#ifdef _WIN32
        _mkdir(dump_dir);
#else
        mkdir(dump_dir, 0777);
#endif
        snprintf(path, sizeof path, "%s/present_%06d.png", dump_dir, dump_n++);
        write_png(path, frame_rgb, w, h);
        port_printf("[fps60] %s: vblank %d alpha %.3f%s\n", path, port_frame, alpha, interp ? " (interpolated)" : "");
    }
}
/* psyq.c port_vblank, in place of its pacing sleep: the presents due before the vblank at due_us (realtime: wall
   clock, one per display refresh) or, unpaced (headless), on a virtual clock of 60 presents per 50 vblanks; alpha
   runs from 0 at the previous vblank to 1 at this one */
extern int port_window_refresh(void);
void port_fps60_wait(unsigned long long due_us, unsigned long long period_us, int realtime) {
    if (be != &port_gpu_gl) { if (realtime && due_us > port_time_us()) port_sleep_until_us(due_us); return; }
    if (!realtime) {
        static unsigned long long k, vb;
        unsigned long long vdue = ++vb * 20000ull;
        for (; k * 1000000ull / 60 < vdue; k++) fps60_present(1.0 - (double)(vdue - k * 1000000ull / 60) / 20000.0);
        return;
    }
    static unsigned long long p, last;
    unsigned long long P = 1000000ull / (unsigned)(port_window_refresh() > 0 ? port_window_refresh() : 60), now = port_time_us();
    int n = 0;
    if (!p || p + 4 * P < now || p > due_us + 2 * P) p = now;
    while (p < due_us) {
        if (p > port_time_us()) port_sleep_until_us(p);
        double a = 1.0 - (double)(due_us - p) / (double)period_us;
        fps60_present(a < 0 ? 0 : a);
        n++; last = port_time_us();
        p += P;
        if (last > p + P) p = last;           /* the swap blocked for longer (vsync at a lower rate): follow it */
    }
    if (!n && port_time_us() > last + P) { fps60_present(1.0); last = port_time_us(); }
    if (due_us > port_time_us()) port_sleep_until_us(due_us);
}

void port_gpu_vblank(void) {
    if (port_fps60 && be == &port_gpu_gl) fps60_vblank();
    else if (port_window) present_window();
    if (!port_dump_dir || port_dump_every <= 0 || port_frame % port_dump_every) return;
#ifdef _WIN32
    _mkdir(port_dump_dir);
#else
    mkdir(port_dump_dir, 0777);
#endif
    char path[512]; int w, h;
    port_gpu_frame(frame_rgb, &w, &h);
    path_for(path, "frame_");
    if (write_png(path, frame_rgb, w, h)) port_printf("[gpu] cannot write %s\n", path);
    {   /* --internal-res N > 1 on OpenGL: the display area at N x as well (hd_*.png) */
        extern int port_internal_res;
        extern int port_gl_frame_hd(u8 *rgb, int x, int y, int w, int h, int wide, int *ow, int *oh);
        RECT *d = (RECT *)cur_dispenv;
        int W = d->w > 0 ? d->w : 320, H = d->h > 0 ? d->h : 240, ow, oh, wide;
        if (W > 1024) W = 1024; if (H > 512) H = 512;
        wide = ws_frame(d, W, cur_dispenv[0x11]);
        if (port_internal_res > 1 && be == &port_gpu_gl && disp_mask && !cur_dispenv[0x11]) {
            static u8 *hd; if (!hd) hd = (u8 *)malloc((size_t)2048 * 512 * 3 * 16);
            if (hd && !port_gl_frame_hd(hd, wide ? ws_base(d->x & 1023) : d->x & 1023, d->y & 511, wide ? 320 + 2 * port_widescreen : W, H, wide, &ow, &oh)) {
                /* GL rows are bottom-up in memory order = VRAM top-down: already in PNG order */
                path_for(path, "hd_");
                write_png(path, hd, ow, oh);
            }
        }
    }
    if (port_dump_vram) {
        be->vram_read(0, 0, 1024, 512);
        for (int y = 0; y < 512; y++) for (int x = 0; x < 1024; x++) {
            u16 c = vram[y][x]; u8 *o = frame_rgb + (y * 1024 + x) * 3;
            o[0] = (c & 31) << 3; o[1] = ((c >> 5) & 31) << 3; o[2] = ((c >> 10) & 31) << 3;
        }
        path_for(path, "vram_");
        write_png(path, frame_rgb, 1024, 512);
    }
}
