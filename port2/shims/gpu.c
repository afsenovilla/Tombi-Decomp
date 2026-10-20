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
 * Written for this project from the public GPU documentation (psx-spx); no emulator code is copied. */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32;
typedef struct { s16 x, y, w, h; } RECT;

static u16 vram[512][1024];
static int gpu_prims, gpu_lists;
int port_gpu_packets(void) { return gpu_prims; }
unsigned port_ot_signature;          /* hash of the packets of the last DrawOTag (watchdog.c) */
int port_gpu_lists(void) { return gpu_lists; }
u16 *port_vram(void) { return &vram[0][0]; }

/* ------------------------------------------------------------------ GPU drawing state (GP0 E1..E6) */
static struct {
    int tx, ty;            /* texture page base in VRAM (x multiple of 64, y 0/256) */
    int abr;               /* semi-transparency mode 0..3 */
    int tp;                /* texture depth: 0 = 4-bit CLUT, 1 = 8-bit CLUT, 2 = 15-bit direct */
    int dither, dfe;
    int twmx, twmy, twox, twoy;   /* texture window mask/offset in 8-pixel units */
    int x1, y1, x2, y2;    /* drawing area, inclusive */
    int ox, oy;            /* drawing offset */
    int set_mask, check_mask;
} gs = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1023, 511, 0, 0, 0, 0 };

static void set_drawmode(u32 w) {
    gs.tx = (w & 15) * 64; gs.ty = ((w >> 4) & 1) * 256;
    gs.abr = (w >> 5) & 3; gs.tp = (w >> 7) & 3; if (gs.tp == 3) gs.tp = 2;
    gs.dither = (w >> 9) & 1; gs.dfe = (w >> 10) & 1;
}
static void set_texpage16(u32 t) { set_drawmode((gs.dither << 9) | (gs.dfe << 10) | (t & 0x1FF)); }

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
static inline void plot(int x, int y, u16 c, int semi) {
    u16 *d = &vram[y][x];
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
static void use_texture(u32 clut, u32 tpage) {
    clut_x = (clut & 0x3F) * 16; clut_y = (clut >> 6) & 511;
    set_texpage16(tpage);
    tp_x = gs.tx; tp_y = gs.ty; tp_depth = gs.tp;
}

/* ------------------------------------------------------------------ triangles */
typedef struct { int x, y, r, g, b, u, v; } Vtx;

static void tri(const Vtx *a, const Vtx *b, const Vtx *c, int shade, int tex, int semi, int raw) {
    /* edge functions on integer pixel centres, top-left fill rule */
    long long area = (long long)(b->x - a->x) * (c->y - a->y) - (long long)(c->x - a->x) * (b->y - a->y);
    if (area == 0) return;
    if (area < 0) { const Vtx *t = b; b = c; c = t; area = -area; }
    /* the GPU drops primitives larger than 1023x511 */
    int minx = a->x, maxx = a->x, miny = a->y, maxy = a->y;
    if (b->x < minx) minx = b->x; if (b->x > maxx) maxx = b->x; if (c->x < minx) minx = c->x; if (c->x > maxx) maxx = c->x;
    if (b->y < miny) miny = b->y; if (b->y > maxy) maxy = b->y; if (c->y < miny) miny = c->y; if (c->y > maxy) maxy = c->y;
    if (maxx - minx > 1023 || maxy - miny > 511) return;
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
static void rect(int x0, int y0, int w, int h, u32 col, int tex, int u0, int v0, int semi, int raw) {
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
static void line(int x0, int y0, u32 c0, int x1, int y1, u32 c1, int shade, int semi) {
    int dx = x1 - x0, dy = y1 - y0, adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy;
    if (adx > 1023 || ady > 511) return;
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
static void fill(u32 col, u32 xy, u32 wh) {
    int x = xy & 0x3F0, y = (xy >> 16) & 0x1FF, w = ((wh & 0x3FF) + 15) & ~15, h = (wh >> 16) & 0x1FF;
    u16 c = rgb24to15(col);
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) vram[(y + j) & 511][(x + i) & 1023] = c;
}
static void copy(u32 s, u32 d, u32 wh) {
    int sx = s & 1023, sy = (s >> 16) & 511, dx = d & 1023, dy = (d >> 16) & 511;
    int w = ((wh & 0xFFFF) - 1) & 1023, h = (((wh >> 16) - 1) & 511); w++; h++;
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++)
        vram[(dy + j) & 511][(dx + i) & 1023] = vram[(sy + j) & 511][(sx + i) & 1023];
}

/* ------------------------------------------------------------------ GP0 decoder */
static inline int sx11(u32 v) { return ((s32)(v << 21)) >> 21; }
static void vertex(Vtx *o, u32 w) { o->x = sx11(w) + gs.ox; o->y = sx11(w >> 16) + gs.oy; }
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
        Vtx v[4]; int k = 1; u32 clut = 0, tpage = 0;
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
        if (tex) use_texture(clut, tpage);
        if (tex && raw) shade = 0;
        tri(&v[0], &v[1], &v[2], shade, tex, semi, raw);
        if (quad) tri(&v[1], &v[2], &v[3], shade, tex, semi, raw);
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
            line(a.x, a.y, c0, b.x, b.y, c1, shade, semi);
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
        int k = 2, u = 0, v = 0, w, h;
        if (tex) {
            u32 uv = p[k++]; u = uv & 255; v = (uv >> 8) & 255;
            clut_x = ((uv >> 16) & 0x3F) * 16; clut_y = (uv >> 22) & 511;
            tp_x = gs.tx; tp_y = gs.ty; tp_depth = gs.tp;
        }
        if (size == 0) { w = p[k] & 0x3FF; h = (p[k] >> 16) & 0x1FF; k++; }
        else { w = h = size == 1 ? 1 : size == 2 ? 8 : 16; }
        rect(a.x, a.y, w, h, p[0] & 0xFFFFFF, tex, u, v, semi, raw);
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
        for (int j = 0; j < h; j++) for (int q = 0; q < w; q++, i++) if (i < avail) vram[(y + j) & 511][(x + q) & 1023] = s[i];
        return n;
    }
    if (cmd >= 0xC0 && cmd < 0xE0) return n;                 /* VRAM -> CPU: nothing to do */
    if (cmd >= 0xE7 || (cmd >= 0x03 && cmd < 0x1F)) return 1; /* E7-FF, 03-1E: the GPU ignores them (one word) */
    if (unknown_cmds++ < 20) port_printf("[gpu] unknown GP0 command %02x (frame %d)\n", cmd, port_frame);
    return 0;                                                 /* the rest of the packet is dropped, no state is touched */
}

static void packet(const u32 *p, int n) {
    while (n > 0) {
        int used = gp0(p, n);
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

/* DRAWENV: RECT clip; s16 ofs[2]; RECT tw; u16 tpage; u8 dtd, dfe, isbg, r0, g0, b0; DR_ENV dr_env */
static u8 cur_drawenv[0x5C], cur_dispenv[0x14];
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
    if (e[24]) {                                         /* isbg: clear the drawing area */
        u16 c = rgb24to15((u32)e[25] | ((u32)e[26] << 8) | ((u32)e[27] << 16));
        for (int y = gs.y1; y <= gs.y2; y++) for (int x = gs.x1; x <= gs.x2; x++) vram[y][x] = c;
    }
    return env;
}
void *GetDrawEnv(void *env) { memcpy(env, cur_drawenv, sizeof cur_drawenv); return env; }
void *PutDispEnv(void *env) { memcpy(cur_dispenv, env, sizeof cur_dispenv); return env; }
void *GetDispEnv(void *env) { memcpy(env, cur_dispenv, sizeof cur_dispenv); return env; }

/* --patch-tex FILE (mods: button prompts, docs/PORT.md "Mods"): VRAM rects re-applied after every LoadImage that
   overlaps them, so a texture the game uploads (a font/HUD page) shows our pixels instead.  File: "TBTX", u32 count,
   then per entry u16 x, y, w, h (VRAM words) followed by w*h u16 words. */
const char *port_patch_tex_path;
static struct { u16 x, y, w, h; const u16 *data; } tex_patches[64];
static int n_tex_patches, tex_patch_loaded;
static u8 tex_patch_buf[256 * 1024];
static void tex_patch_load(void) {
    tex_patch_loaded = 1;
    FILE *f = fopen(port_patch_tex_path, "rb");
    if (!f) { port_printf("[gpu] cannot open %s\n", port_patch_tex_path); return; }
    int n = (int)fread(tex_patch_buf, 1, sizeof tex_patch_buf, f);
    fclose(f);
    if (n < 8 || tex_patch_buf[0] != 'T' || tex_patch_buf[1] != 'B' || tex_patch_buf[2] != 'T' || tex_patch_buf[3] != 'X') { port_printf("[gpu] %s: not a TBTX file\n", port_patch_tex_path); return; }
    u32 count = *(u32 *)(tex_patch_buf + 4), off = 8;
    for (u32 i = 0; i < count && n_tex_patches < 64; i++) {
        if (off + 8 > (u32)n) break;
        u16 *h = (u16 *)(tex_patch_buf + off);
        u32 bytes = (u32)h[2] * h[3] * 2;
        if (off + 8 + bytes > (u32)n) break;
        tex_patches[n_tex_patches].x = h[0]; tex_patches[n_tex_patches].y = h[1];
        tex_patches[n_tex_patches].w = h[2]; tex_patches[n_tex_patches].h = h[3];
        tex_patches[n_tex_patches].data = (const u16 *)(tex_patch_buf + off + 8);
        n_tex_patches++;
        off += 8 + bytes;
    }
    port_printf("[gpu] %s: %d texture patches\n", port_patch_tex_path, n_tex_patches);
}
static void tex_patch_apply(int x, int y, int w, int h) {
    if (!port_patch_tex_path) return;
    if (!tex_patch_loaded) tex_patch_load();
    for (int i = 0; i < n_tex_patches; i++) {
        int px = tex_patches[i].x, py = tex_patches[i].y, pw = tex_patches[i].w, ph = tex_patches[i].h;
        if (px >= x + w || px + pw <= x || py >= y + h || py + ph <= y) continue;
        const u16 *d = tex_patches[i].data;
        for (int j = 0; j < ph; j++) for (int k = 0; k < pw; k++) vram[(py + j) & 511][(px + k) & 1023] = *d++;
    }
}
int LoadImage(RECT *r, u32 *p) {
    u16 *s = (u16 *)p;
    for (int j = 0; j < r->h; j++) for (int i = 0; i < r->w; i++) vram[(r->y + j) & 511][(r->x + i) & 1023] = *s++;
    tex_patch_apply(r->x, r->y, r->w, r->h);
    return 0;
}
int StoreImage(RECT *r, u32 *p) {
    u16 *d = (u16 *)p;
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
    for (int j = 0; j < r->h; j++) for (int i = 0; i < r->w; i++)
        vram[(y + j) & 511][(x + i) & 1023] = vram[(r->y + j) & 511][(r->x + i) & 1023];
    return 0;
}
int ClearImage(RECT *r, u8 R, u8 G, u8 B) {
    u16 c = (R >> 3) | ((G >> 3) << 5) | ((B >> 3) << 10);
    for (int j = 0; j < r->h; j++) for (int i = 0; i < r->w; i++) vram[(r->y + j) & 511][(r->x + i) & 1023] = c;
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
void DrawOTag(u32 *ot) {
    u32 *p = ot; int n = 0;
    unsigned h = 2166136261u;
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
    port_ot_signature = h * 31u + (unsigned)n;
    if (port_verbose > 1) port_printf("[port] DrawOTag %p: %d packets\n", ot, n);
}
void DrawOTagEnv(u32 *ot, void *env) { PutDrawEnv(env); DrawOTag(ot); }
void DrawPrim(void *p) { u32 *w = p; if (w[0] >> 24) packet(w + 1, w[0] >> 24); }

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
static u8 png_buf[(1024 * 3 + 1) * 512 + 65536];
static u8 raw_rows[(1024 * 3 + 1) * 512];
static int write_png(const char *path, const u8 *rgb, int w, int h) {
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
/* converts the displayed area (DISPENV.disp, 15-bit or 24-bit mode) to RGB8; returns its size */
int port_gpu_frame(u8 *rgb, int *w, int *h) {
    RECT *d = (RECT *)cur_dispenv;
    int rgb24 = cur_dispenv[0x11];
    int W = d->w > 0 ? d->w : 320, H = d->h > 0 ? d->h : 240;
    if (W > 1024) W = 1024; if (H > 512) H = 512;
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
void port_gpu_vblank(void) {
    if (port_window) {
        int w, h;
        port_gpu_frame(frame_rgb, &w, &h);
        port_window_frame(frame_rgb, w, h);
    }
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
    if (port_dump_vram) {
        for (int y = 0; y < 512; y++) for (int x = 0; x < 1024; x++) {
            u16 c = vram[y][x]; u8 *o = frame_rgb + (y * 1024 + x) * 3;
            o[0] = (c & 31) << 3; o[1] = ((c >> 5) & 31) << 3; o[2] = ((c >> 10) & 31) << 3;
        }
        path_for(path, "vram_");
        write_png(path, frame_rgb, 1024, 512);
    }
}
