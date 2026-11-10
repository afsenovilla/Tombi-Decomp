/* The "pica" GPU backend (gpu_backend.h; docs/PORT.md "Nintendo 3DS"): the PS1 GPU mapped onto a fixed-function
 * pipeline with the limits of the 3DS's PICA200, so the renderer of a 3DS build can be written and checked on the PC.
 *
 * The PICA200 has no fragment shaders and no indexed textures, so nothing samples VRAM through a CLUT on the GPU:
 *   - textures: a texture page + CLUT (+ texture window) is decoded on the CPU from vram[] into a 256x256 RGBA8 slot
 *     of a small cache.  The alpha of a texel holds its class: 0 = transparent (raw 0x0000), A_OPAQUE = drawn, not
 *     semi-transparent (bit 15 clear), A_SEMI = drawn, semi-transparent when the primitive is (bit 15 set);
 *   - a slot is valid while the VRAM pages it was decoded from (64x256-word pages, as the PS1's texture pages, and the
 *     CLUT's page) keep the version they had; a page's version moves when vram[] changes there (a host write, or a
 *     read-back of what the GPU drew: a page drawn into is read back before a texture is decoded from it);
 *   - per-primitive state is what the PICA's texture combiner, alpha test and blender express: texture replace (raw)
 *     or modulate x2 (the PS1's colour / 128), an EQUAL / NOTEQUAL alpha test on the classes, and the four PS1
 *     semi-transparency modes as blend equations (0: 0.5 B + 0.5 F through a constant alpha, 1: B + F, 2: B - F
 *     (reverse subtract), 3: B + 0.25 F).  A semi-transparent textured primitive is drawn twice: its opaque texels
 *     (alpha test EQUAL A_OPAQUE) without blending, then its semi-transparent ones (EQUAL A_SEMI) blended;
 *   - the render target is the whole VRAM (1024x512 RGBA8); alpha keeps bit 15 for read-backs.
 * Not mapped (the PICA cannot): the mask bits (set / check), dithering.
 *
 * The device (a "PicaDev": textures, triangles with that state, fill, read and write of the target) is the part a 3DS
 * build implements with citro3d.  The one here, pd_ref, is a CPU reference that follows the same rules, so headless
 * runs (--renderer pica) can be compared with the software rasteriser frame by frame. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"
#include "gpu_backend.h"

#include "gpu_pica.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
#define A_OPAQUE PICA_A_OPAQUE
#define A_SEMI PICA_A_SEMI
#define SLOTS PICA_SLOTS

/* ================================================================== the reference device (CPU) */
static u32 (*rt)[1024];                  /* the render target, 512 rows (allocated by pica_start: the image stays small) */
static u32 (*slot_px)[256 * 256];        /* SLOTS textures */
static void ref_tex_upload(int slot, const u32 *rgba) { memcpy(slot_px[slot], rgba, 256 * 256 * 4); }
static inline u8 sat(int v) { return v < 0 ? 0 : v > 255 ? 255 : (u8)v; }
static inline void ref_frag(const PState *s, int x, int y, int r, int g, int b, int u, int v) {
    u32 c = 0xFF000000u | (u32)r | ((u32)g << 8) | ((u32)b << 16);
    if (s->tex >= 0) {
        u32 t = slot_px[s->tex][(v & 255) * 256 + (u & 255)];
        int a = t >> 24;
        if (s->atest == AT_NOTZERO && !a) return;
        if (s->atest == AT_OPAQUE && a != A_OPAQUE) return;
        if (s->atest == AT_SEMI && a != A_SEMI) return;
        int tr = t & 255, tg = (t >> 8) & 255, tb = (t >> 16) & 255;
        if (s->mod) { tr = sat(tr * r / 128); tg = sat(tg * g / 128); tb = sat(tb * b / 128); }
        c = (u32)tr | ((u32)tg << 8) | ((u32)tb << 16) | ((u32)a << 24);
    } else c &= 0x00FFFFFFu;
    u32 *d = &rt[y][x];
    if (s->blend >= 0) {
        int dr = *d & 255, dg = (*d >> 8) & 255, db = (*d >> 16) & 255, fr = c & 255, fg = (c >> 8) & 255, fb = (c >> 16) & 255;
        switch (s->blend) {
        case 0: dr = (dr + fr) >> 1; dg = (dg + fg) >> 1; db = (db + fb) >> 1; break;
        case 1: dr += fr; dg += fg; db += fb; break;
        case 2: dr -= fr; dg -= fg; db -= fb; break;
        default: dr += fr >> 2; dg += fg >> 2; db += fb >> 2; break;
        }
        c = (u32)sat(dr) | ((u32)sat(dg) << 8) | ((u32)sat(db) << 16) | (c & 0xFF000000u);
    }
    *d = c;
}
/* one triangle: edge functions on integer sample points with the top-left rule, attributes interpolated linearly
   (no perspective: the PS1's affine mapping), as the software rasteriser of gpu.c does */
static void ref_tri(const PState *s, const PV *a, const PV *b, const PV *c) {
    int ax = (int)a->x, ay = (int)a->y, bx = (int)b->x, by = (int)b->y, cx = (int)c->x, cy = (int)c->y;
    long long area = (long long)(bx - ax) * (cy - ay) - (long long)(cx - ax) * (by - ay);
    if (!area) return;
    if (area < 0) { const PV *t = b; b = c; c = t; int tx = bx, ty = by; bx = cx; by = cy; cx = tx; cy = ty; area = -area; }
    int minx = ax, maxx = ax, miny = ay, maxy = ay;
    if (bx < minx) minx = bx; if (bx > maxx) maxx = bx; if (cx < minx) minx = cx; if (cx > maxx) maxx = cx;
    if (by < miny) miny = by; if (by > maxy) maxy = by; if (cy < miny) miny = cy; if (cy > maxy) maxy = cy;
    if (minx < s->x1) minx = s->x1; if (maxx > s->x2) maxx = s->x2;
    if (miny < s->y1) miny = s->y1; if (maxy > s->y2) maxy = s->y2;
    if (minx > maxx || miny > maxy) return;
    float inv = 1.0f / (float)area;
    #define GRAD(f, dx, dy) \
        float dx = ((b->f - a->f) * (cy - ay) - (c->f - a->f) * (by - ay)) * inv; \
        float dy = ((c->f - a->f) * (bx - ax) - (b->f - a->f) * (cx - ax)) * inv;
    GRAD(r, drdx, drdy) GRAD(g, dgdx, dgdy) GRAD(b, dbdx, dbdy) GRAD(u, dudx, dudy) GRAD(v, dvdx, dvdy)
    #undef GRAD
    int X[3] = { bx, cx, ax }, Y[3] = { by, cy, ay }, X1[3] = { cx, ax, bx }, Y1[3] = { cy, ay, by }, ex[3], ey[3], bias[3];
    for (int k = 0; k < 3; k++) {
        ex[k] = X1[k] - X[k]; ey[k] = Y1[k] - Y[k];
        bias[k] = (ey[k] < 0 || (ey[k] == 0 && ex[k] > 0)) ? 0 : -1;
    }
    for (int y = miny; y <= maxy; y++) {
        int e[3];
        for (int k = 0; k < 3; k++) e[k] = ex[k] * (y - Y[k]) - ey[k] * (minx - X[k]) + bias[k];
        int seen = 0;
        for (int x = minx; x <= maxx; x++, e[0] -= ey[0], e[1] -= ey[1], e[2] -= ey[2]) {
            if ((e[0] | e[1] | e[2]) < 0) { if (seen) break; continue; }
            seen = 1;
            float fx = (float)(x - ax), fy = (float)(y - ay);
            int r = sat((int)(a->r + drdx * fx + drdy * fy + 0.5f)), g = sat((int)(a->g + dgdx * fx + dgdy * fy + 0.5f));
            int bb = sat((int)(a->b + dbdx * fx + dbdy * fy + 0.5f));
            int u = (int)(a->u + dudx * fx + dudy * fy + 0.5f), v = (int)(a->v + dvdx * fx + dvdy * fy + 0.5f);
            ref_frag(s, x, y, r, g, bb, u, v);
        }
    }
}
static void ref_tris(const PState *s, const PV *v, int n) { for (int i = 0; i + 2 < n; i += 3) ref_tri(s, &v[i], &v[i + 1], &v[i + 2]); }
static void ref_fill(int x, int y, int w, int h, u32 c) {
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) rt[(y + j) & 511][(x + i) & 1023] = c;
}
static void ref_read(int x, int y, int w, int h, u32 *d, int stride) {
    for (int j = 0; j < h; j++) memcpy(d + (size_t)j * stride, &rt[y + j][x], (size_t)w * 4);
}
static void ref_write(int x, int y, int w, int h, const u32 *s, int stride) {
    for (int j = 0; j < h; j++) memcpy(&rt[y + j][x], s + (size_t)j * stride, (size_t)w * 4);
}
static int ref_start(void) {
    if (!rt) rt = calloc(512, sizeof *rt);
    if (!slot_px) slot_px = malloc(SLOTS * sizeof *slot_px);
    return rt && slot_px ? 0 : -1;
}
static const PicaDev pd_ref = { ref_start, ref_tex_upload, ref_tris, ref_fill, ref_read, ref_write, 0 };
#ifdef __3DS__
extern const PicaDev pd_c3d;              /* port2/n3ds/pica_c3d.c */
static const PicaDev *dev = &pd_c3d;
#else
static const PicaDev *dev = &pd_ref;
#endif

/* ================================================================== the core */
static const GpuState *gs = &port_gs;
static inline u32 c15to32(u16 c) {   /* 5-bit channels to 8 (k * 255 / 31, as the GL backend's upload), bit 15 to alpha */
    u32 r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31;
    return ((r << 3) | (r >> 2)) | (((g << 3) | (g >> 2)) << 8) | (((b << 3) | (b >> 2)) << 16) | (c & 0x8000 ? 0xFF000000u : 0);
}
static inline u16 c32to15(u32 c) {
    return (u16)((((c & 255) * 31 + 127) / 255) | ((((c >> 8) & 255) * 31 + 127) / 255) << 5 | ((((c >> 16) & 255) * 31 + 127) / 255) << 10
                 | ((c >> 24) > A_OPAQUE ? 0x8000 : 0));   /* bit 15: A_SEMI (or a host word with it); A_OPAQUE texels clear it */
}

/* ---- VRAM pages: 16 x 2 of 64 x 256 words */
static unsigned page_ver[2][16], ver_clock;
#define CSH 0                         /* cells are 16 x (1 << CSH) pixels */
static u8 cell_drawn[512 >> CSH][64];   /* cells the GPU drew into since their last read-back: vram[] is stale there */
static u8 page_drawn[2][16];            /* pages with a cell_drawn set (may stay set after its cells are clean) */
static void pages_of(int x, int y, int w, int h, int *px0, int *py0, int *px1, int *py1) {
    *px0 = (x & 1023) >> 6; *px1 = ((x & 1023) + w - 1) >> 6; *py0 = (y & 511) >> 8; *py1 = ((y & 511) + h - 1) >> 8;
    if (*px1 > 15) *px1 = 15; if (*py1 > 1) *py1 = 1;
}
static void pages_bump(int x, int y, int w, int h) {
    int a, b, c, d; pages_of(x, y, w, h, &a, &b, &c, &d);
    for (int j = b; j <= d; j++) for (int i = a; i <= c; i++) page_ver[j][i] = ++ver_clock;
}
static void pages_drawn(int x1, int y1, int x2, int y2) {   /* (inclusive pixel bounds) */
    if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0; if (x2 > 1023) x2 = 1023; if (y2 > 511) y2 = 511;
    if (x1 > x2 || y1 > y2) return;
    for (int j = y1 >> CSH; j <= y2 >> CSH; j++) memset(&cell_drawn[j][x1 >> 4], 1, (size_t)((x2 >> 4) - (x1 >> 4) + 1));
    for (int j = y1 >> 8; j <= y2 >> 8; j++) for (int i = x1 >> 6; i <= x2 >> 6; i++) page_drawn[j][i] = 1;
}

/* ---- the batch: triangles with one state */
#define MAX_PV 6144
static PV bv[MAX_PV];
static int nbv;
static PState bst;
static unsigned long long st_batches, st_tris, st_decodes, st_readbacks, st_rb_tex, st_rb_host, st_rb_px;
static void flush(void) {
    if (!nbv) return;
    dev->tris(&bst, bv, nbv);
    st_batches++; st_tris += nbv / 3;
    nbv = 0;
}
static PV *batch(const PState *s, int n) {
    if (nbv && (nbv + n > MAX_PV || memcmp(s, &bst, sizeof *s))) flush();
    bst = *s;
    PV *p = bv + nbv; nbv += n;
    return p;
}

/* ---- coherence with vram[] */
static u32 *stage;                       /* 1024 x 512 */
static void read_back(int x, int y, int w, int h) {   /* the target's rectangle into vram[] */
    u16 *vram = port_vram();
    dev->read(x, y, w, h, stage, w);
    int changed = 0;
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) {
        u16 c = c32to15(stage[j * w + i]), *p = &vram[(y + j) * 1024 + x + i];
        changed |= *p ^ c; *p = c;
    }
    /* (vram[] unchanged: the textures decoded from these pages are still right, keep them) */
    if (changed) pages_bump(x, y, w, h);
    st_readbacks++;
}
static void clip_rect(int *x, int *y, int *w, int *h);
/* vram[] and the target agree again on this rectangle (a host write, a read-back): its whole cells are not stale */
static void cells_clean(int x, int y, int w, int h) {
    for (int j = (y + (1 << CSH) - 1) >> CSH; j < (y + h) >> CSH; j++)
        for (int i = (x + 15) >> 4; i < (x + w) >> 4; i++) cell_drawn[j][i] = 0;
}
/* read back the drawn cells of this rectangle (a run of cells in a row at a time) */
static void sync_pages(int x, int y, int w, int h) {
    clip_rect(&x, &y, &w, &h);
    if (w <= 0 || h <= 0) return;
    int any = 0;   /* (most textures: no page of theirs drawn into, nothing to scan) */
    for (int pj = y >> 8; pj <= (y + h - 1) >> 8; pj++) for (int pi = x >> 6; pi <= (x + w - 1) >> 6; pi++) any |= page_drawn[pj][pi];
    if (!any) return;
    for (int j = y >> CSH; j <= (y + h - 1) >> CSH; j++)
        for (int i = x >> 4; i <= (x + w - 1) >> 4; i++) {
            if (!cell_drawn[j][i]) continue;
            int k = i; while (k + 1 <= (x + w - 1) >> 4 && cell_drawn[j][k + 1]) k++;
            flush(); read_back(i * 16, j << CSH, (k - i + 1) * 16, 1 << CSH);
            memset(&cell_drawn[j][i], 0, (size_t)(k - i + 1)); st_rb_tex++;
            if (port_verbose) port_printf("[gpu] pica: texture read-back %d,%d %dx%d (frame %d)\n", i * 16, j << CSH, (k - i + 1) * 16, 1 << CSH, port_frame);
            i = k;
        }
    /* the pages this rectangle covers whole have no drawn cell left */
    for (int pj = y >> 8; pj <= (y + h - 1) >> 8; pj++) for (int pi = x >> 6; pi <= (x + w - 1) >> 6; pi++)
        if (x <= pi * 64 && x + w >= pi * 64 + 64 && y <= pj * 256 && y + h >= pj * 256 + 256) page_drawn[pj][pi] = 0;
}
static void clip_rect(int *x, int *y, int *w, int *h) {
    *x &= 1023; *y &= 511;
    if (*x + *w > 1024) *w = 1024 - *x;
    if (*y + *h > 512) *h = 512 - *y;
}
static void pica_vram_written(int x, int y, int w, int h) {
    clip_rect(&x, &y, &w, &h);
    if (w <= 0 || h <= 0) return;
    flush();
    const u16 *vram = port_vram();
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) stage[j * w + i] = c15to32(vram[(y + j) * 1024 + x + i]);
    dev->write(x, y, w, h, stage, w);
    pages_bump(x, y, w, h);
    cells_clean(x, y, w, h);
}
static void pica_vram_read(int x, int y, int w, int h) {
    clip_rect(&x, &y, &w, &h);
    if (w <= 0 || h <= 0) return;
    flush();
    read_back(x, y, w, h);
    cells_clean(x, y, w, h);
    st_rb_host++; st_rb_px += (unsigned long long)w * h;
    if (port_verbose && st_rb_host < 60) port_printf("[gpu] pica: read-back %d,%d %dx%d (frame %d)\n", x, y, w, h, port_frame);
}

/* ---- the texture cache */
typedef struct { int tx, ty, depth, cx, cy, tw; unsigned ver; unsigned long long used; int live; } Slot;
static Slot slots[SLOTS];
static unsigned long long use_clock;
static u32 *dec;                         /* 256 x 256 */
static unsigned tex_ver(const GpuTex *t, int *x, int *y, int *w) {   /* the newest version of the pages the texture reads */
    int pw = t->depth == 0 ? 64 : t->depth == 1 ? 128 : 256;
    *x = t->tx; *y = t->ty; *w = pw;
    unsigned v = 0; int a, b, c, d;
    pages_of(t->tx, t->ty, pw, 256, &a, &b, &c, &d);
    for (int j = b; j <= d; j++) for (int i = a; i <= c; i++) if (page_ver[j][i] > v) v = page_ver[j][i];
    if (t->depth < 2 && t->cy != PORT_CLUT_Y) {
        pages_of(t->cx, t->cy, t->depth ? 256 : 16, 1, &a, &b, &c, &d);
        for (int j = b; j <= d; j++) for (int i = a; i <= c; i++) if (page_ver[j][i] > v) v = page_ver[j][i];
    }
    return v;
}
static inline u32 texel(u16 c) { return !c ? 0 : (c15to32(c) & 0x00FFFFFFu) | ((u32)(c & 0x8000 ? A_SEMI : A_OPAQUE) << 24); }
static void decode(const GpuTex *t, int tw) {
    const u16 *vram = port_vram();
    if (!tw && t->cy != PORT_CLUT_Y && (t->depth == 2 || t->cx + (t->depth ? 256 : 16) <= 1024)) {
        /* the usual case (no texture window, a CLUT in vram[] that does not wrap): the CLUT converted once, whole
           words of indices at a time; the same texels as the general loop below */
        static u32 clut[256];
        if (t->depth < 2) for (int i = 0; i < (t->depth ? 256 : 16); i++) clut[i] = texel(vram[t->cy * 1024 + t->cx + i]);
        for (int v = 0; v < 256; v++) {
            const u16 *row = &vram[((t->ty + v) & 511) * 1024];
            u32 *d = &dec[v * 256];
            if (t->depth == 0) for (int k = 0; k < 64; k++) {
                u16 w = row[(t->tx + k) & 1023];
                d[0] = clut[w & 15]; d[1] = clut[(w >> 4) & 15]; d[2] = clut[(w >> 8) & 15]; d[3] = clut[w >> 12]; d += 4;
            } else if (t->depth == 1) for (int k = 0; k < 128; k++) {
                u16 w = row[(t->tx + k) & 1023];
                d[0] = clut[w & 255]; d[1] = clut[w >> 8]; d += 2;
            } else for (int k = 0; k < 256; k++) d[k] = texel(row[(t->tx + k) & 1023]);
        }
        return;
    }
    int twmx = tw & 31, twmy = (tw >> 5) & 31, twox = (tw >> 10) & 31, twoy = (tw >> 15) & 31;
    for (int v0 = 0; v0 < 256; v0++) for (int u0 = 0; u0 < 256; u0++) {
        int u = (u0 & ~(twmx * 8)) | ((twox & twmx) * 8), v = (v0 & ~(twmy * 8)) | ((twoy & twmy) * 8);
        u16 c;
        if (t->depth == 0) {
            u16 w = vram[((t->ty + v) & 511) * 1024 + ((t->tx + (u >> 2)) & 1023)];
            int i = (w >> ((u & 3) * 4)) & 15;
            c = t->cy == PORT_CLUT_Y ? port_prompt_clut[(t->cx + i) % (PORT_CLUT_ROWS * 16)] : vram[t->cy * 1024 + ((t->cx + i) & 1023)];
        } else if (t->depth == 1) {
            u16 w = vram[((t->ty + v) & 511) * 1024 + ((t->tx + (u >> 1)) & 1023)];
            int i = (w >> ((u & 1) * 8)) & 255;
            c = t->cy == PORT_CLUT_Y ? port_prompt_clut[(t->cx + (i & 15)) % (PORT_CLUT_ROWS * 16)] : vram[t->cy * 1024 + ((t->cx + i) & 1023)];
        } else c = vram[((t->ty + v) & 511) * 1024 + ((t->tx + u) & 1023)];
        dec[v0 * 256 + u0] = texel(c);
    }
}
extern unsigned long long port_time_us(void);
unsigned long long pica_decode_us;   /* (time in decode(): n3ds/pica_c3d.c logs it) */
static int tex_slot(const GpuTex *t) {
    int x, y, w;
    int tw = gs->twmx | gs->twmy << 5 | gs->twox << 10 | gs->twoy << 15;
    sync_pages(t->tx, t->ty, t->depth == 0 ? 64 : t->depth == 1 ? 128 : 256, 256);
    if (t->depth < 2 && t->cy != PORT_CLUT_Y) sync_pages(t->cx, t->cy, t->depth ? 256 : 16, 1);
    unsigned v = tex_ver(t, &x, &y, &w);
    int lru = 0;
    for (int i = 0; i < SLOTS; i++) {
        Slot *s = &slots[i];
        if (s->live && s->tx == t->tx && s->ty == t->ty && s->depth == t->depth && s->tw == tw
            && (t->depth == 2 || (s->cx == t->cx && s->cy == t->cy))) {
            if (s->ver >= v) { s->used = ++use_clock; return i; }
            lru = i; goto load;     /* the same texture, stale: reuse its slot */
        }
        if (!s->live || s->used < slots[lru].used) lru = i;
        if (!s->live) break;
    }
load:
    if (nbv && bst.tex == lru) flush();   /* the pending batch samples the slot about to change */
    unsigned long long t0 = port_time_us();
    decode(t, tw);
    pica_decode_us += port_time_us() - t0;
    dev->tex_upload(lru, dec);
    st_decodes++;
    slots[lru] = (Slot){ t->tx, t->ty, t->depth, t->cx, t->cy, tw, v, ++use_clock, 1 };
    return lru;
}

/* ---- primitives */
static PState base_state(void) {
    PState s = { -1, 0, AT_OFF, -1, gs->x1, gs->y1, gs->x2, gs->y2 };
    return s;
}
static void put(PV *p, int x, int y, int r, int g, int b, int u, int v) {
    p->x = (float)x; p->y = (float)y; p->u = (float)u; p->v = (float)v; p->r = (u8)r; p->g = (u8)g; p->b = (u8)b; p->a = 255;
}
/* emit n vertices (a triangle or two) with the state the primitive needs: textured semi-transparent ones twice */
static void emit(const PV *v, int n, const GpuTex *tex, int semi, int raw) {
    PState s = base_state();
    if (tex) { s.tex = tex_slot(tex); s.mod = !raw; }
    if (!tex) { s.blend = semi ? gs->abr : -1; memcpy(batch(&s, n), v, n * sizeof *v); return; }
    if (!semi) { s.atest = AT_NOTZERO; memcpy(batch(&s, n), v, n * sizeof *v); return; }
    s.atest = AT_OPAQUE; memcpy(batch(&s, n), v, n * sizeof *v);
    s.atest = AT_SEMI; s.blend = gs->abr; memcpy(batch(&s, n), v, n * sizeof *v);
}
static void bbox_drawn(const PV *v, int n) {
    float x1 = v[0].x, y1 = v[0].y, x2 = x1, y2 = y1;
    for (int i = 1; i < n; i++) {
        if (v[i].x < x1) x1 = v[i].x; if (v[i].x > x2) x2 = v[i].x;
        if (v[i].y < y1) y1 = v[i].y; if (v[i].y > y2) y2 = v[i].y;
    }
    int a = (int)x1 > gs->x1 ? (int)x1 : gs->x1, b = (int)y1 > gs->y1 ? (int)y1 : gs->y1;
    int c = (int)x2 < gs->x2 ? (int)x2 : gs->x2, d = (int)y2 < gs->y2 ? (int)y2 : gs->y2;
    pages_drawn(a, b, c, d);
}
/* the cells a triangle covers: those of its bounding box not wholly outside one of its edges (a thin
   diagonal sliver would otherwise mark a whole box, and a later texture or CLUT in it would be read back for nothing) */
static void tri_drawn(const PV *v) {
    int x[3], y[3];
    for (int i = 0; i < 3; i++) { x[i] = (int)v[i].x; y[i] = (int)v[i].y; }
    long long area = (long long)(x[1] - x[0]) * (y[2] - y[0]) - (long long)(x[2] - x[0]) * (y[1] - y[0]);
    if (!area) return;
    if (area < 0) { int t = x[1]; x[1] = x[2]; x[2] = t; t = y[1]; y[1] = y[2]; y[2] = t; }
    int x1 = x[0], x2 = x[0], y1 = y[0], y2 = y[0];
    for (int i = 1; i < 3; i++) { if (x[i] < x1) x1 = x[i]; if (x[i] > x2) x2 = x[i]; if (y[i] < y1) y1 = y[i]; if (y[i] > y2) y2 = y[i]; }
    if (x1 < gs->x1) x1 = gs->x1; if (y1 < gs->y1) y1 = gs->y1; if (x2 > gs->x2) x2 = gs->x2; if (y2 > gs->y2) y2 = gs->y2;
    if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0; if (x2 > 1023) x2 = 1023; if (y2 > 511) y2 = 511;
    if (x1 > x2 || y1 > y2) return;
    for (int cj = y1 >> CSH; cj <= y2 >> CSH; cj++) for (int ci = x1 >> 4; ci <= x2 >> 4; ci++) {
        int out = 0;
        for (int k = 0; k < 3 && !out; k++) {   /* inside: (q1 - q0) x (p - q0) >= 0; outside an edge when all 4 corners are < 0 */
            int k1 = (k + 1) % 3, ex = x[k1] - x[k], ey = y[k1] - y[k], neg = 0;
            for (int c = 0; c < 4; c++) {
                long long px = ci * 16 + (c & 1) * 15, py = (cj << CSH) + (c >> 1) * ((1 << CSH) - 1);
                if ((long long)ex * (py - y[k]) - (long long)ey * (px - x[k]) < 0) neg++;
            }
            out = neg == 4;
        }
        if (!out) { cell_drawn[cj][ci] = 1; page_drawn[(cj << CSH) >> 8][ci >> 2] = 1; }
    }
}
static void pica_tri(const GpuVtx *a, const GpuVtx *b, const GpuVtx *c, int shade, const GpuTex *tex, int semi, int raw) {
    PV v[3]; const GpuVtx *p[3] = { a, b, c };
    for (int i = 0; i < 3; i++) {
        const GpuVtx *q = shade ? p[i] : a;
        put(&v[i], p[i]->x, p[i]->y, q->r, q->g, q->b, p[i]->u, p[i]->v);
    }
    tri_drawn(v);
    emit(v, 3, tex, semi, raw);
}
static void pica_rect(int x, int y, int w, int h, u32 col, const GpuTex *tex, int u0, int v0, int semi, int raw) {
    int r = col & 255, g = (col >> 8) & 255, b = (col >> 16) & 255;
    PV v[6];
    put(&v[0], x, y, r, g, b, u0, v0); put(&v[1], x + w, y, r, g, b, u0 + w, v0); put(&v[2], x, y + h, r, g, b, u0, v0 + h);
    v[3] = v[1]; put(&v[4], x + w, y + h, r, g, b, u0 + w, v0 + h); v[5] = v[2];
    bbox_drawn(v, 6);
    emit(v, 6, tex, semi, raw);
}
/* a line: its pixels (the DDA of gpu.c's soft_line) as 1x1 quads */
static void pica_line(int x0, int y0, u32 c0, int x1, int y1, u32 c1, int shade, int semi) {
    int dx = x1 - x0, dy = y1 - y0, adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy, n = adx > ady ? adx : ady;
    for (int i = 0; i <= n; i++) {
        int x = n ? x0 + (dx * i + (dx >= 0 ? n / 2 : -n / 2)) / n : x0;
        int y = n ? y0 + (dy * i + (dy >= 0 ? n / 2 : -n / 2)) / n : y0;
        u32 c = c0;
        if (shade && n) {
            int r = (c0 & 255) + (((int)(c1 & 255) - (int)(c0 & 255)) * i) / n;
            int g = ((c0 >> 8) & 255) + (((int)((c1 >> 8) & 255) - (int)((c0 >> 8) & 255)) * i) / n;
            int b = ((c0 >> 16) & 255) + (((int)((c1 >> 16) & 255) - (int)((c0 >> 16) & 255)) * i) / n;
            c = r | (g << 8) | (b << 16);
        }
        pica_rect(x, y, 1, 1, c, 0, 0, 0, semi, 1);
    }
}
/* fills and copies are done on vram[] as well (cheap on the CPU) and the result written to the target, so a page they
   touch stays in step and needs no read-back (palette animation copies CLUT rows every frame); a copy from a page the
   GPU drew into reads that page back first */
static void put_rect(int x, int y, int w, int h) {   /* vram[] -> the target, in pieces that do not wrap */
    for (int j = 0; j < h; ) {
        int yy = (y + j) & 511, hh = h - j < 512 - yy ? h - j : 512 - yy;
        for (int i = 0; i < w; ) {
            int xx = (x + i) & 1023, ww = w - i < 1024 - xx ? w - i : 1024 - xx;
            pica_vram_written(xx, yy, ww, hh);
            i += ww;
        }
        j += hh;
    }
}
static void pica_fill(int x, int y, int w, int h, u16 c) {
    u16 *vram = port_vram();
    flush();
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) vram[((y + j) & 511) * 1024 + ((x + i) & 1023)] = c;
    /* the device fills the same rectangle (its colour converts back to exactly c): no upload of the words */
    for (int j = 0; j < h; ) {
        int yy = (y + j) & 511, hh = h - j < 512 - yy ? h - j : 512 - yy;
        for (int i = 0; i < w; ) {
            int xx = (x + i) & 1023, ww = w - i < 1024 - xx ? w - i : 1024 - xx;
            dev->fill(xx, yy, ww, hh, c15to32(c));
            pages_bump(xx, yy, ww, hh);
            cells_clean(xx, yy, ww, hh);
            i += ww;
        }
        j += hh;
    }
}
static void pica_copy(int sx, int sy, int dx, int dy, int w, int h) {
    static u16 *tmp;
    if (!tmp) tmp = malloc(1024 * 512 * 2);
    u16 *vram = port_vram();
    flush();
    sync_pages(sx, sy, w, h);
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) tmp[j * w + i] = vram[((sy + j) & 511) * 1024 + ((sx + i) & 1023)];
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) vram[((dy + j) & 511) * 1024 + ((dx + i) & 1023)] = tmp[j * w + i];
    put_rect(dx, dy, w, h);
}
static void pica_list_end(void) { flush(); }
static int pica_start(void) {
    if (!stage) stage = malloc(1024 * 512 * 4);
    if (!dec) dec = malloc(256 * 256 * 4);
    if (!stage || !dec || dev->start()) return -1;
    pica_vram_written(0, 0, 1024, 512);
    return 0;
}
static void pica_stop(void) {
    port_printf("[gpu] pica: %llu batches, %llu triangles, %llu texture decodes, %llu read-backs (%llu for textures, %llu for the host: %llu pixels)\n",
                st_batches, st_tris, st_decodes, st_readbacks, st_rb_tex, st_rb_host, st_rb_px);
}
static int pica_present(int dx, int dy, int dw, int dh, int masked) {
    flush();
    return dev->present ? dev->present(dx, dy, dw, dh, masked) : 0;
}
const GpuBackend port_gpu_pica = { "pica (3DS model, CPU reference)", pica_start, pica_stop, pica_tri, pica_rect, pica_line, pica_fill,
                                   pica_copy, pica_vram_written, pica_vram_read, pica_list_end, pica_present, 0 };
