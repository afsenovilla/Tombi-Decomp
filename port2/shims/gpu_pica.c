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
static u32 (*atlas[2])[PICA_ATLAS];      /* the texture atlases (both PICA_ATLAS squared here) */
static void ref_tex_upload(int slot, const u32 *rgba, int busy) {
    int z = pica_slot_size(slot), ax, ay; pica_slot_pos(slot, &ax, &ay); (void)busy;
    for (int j = 0; j < z; j++) memcpy(&atlas[pica_slot_atlas(slot)][ay + j][ax], rgba + j * z, (size_t)z * 4);
}
static inline u8 sat(int v) { return v < 0 ? 0 : v > 255 ? 255 : (u8)v; }
static inline void ref_frag(const PState *s, int x, int y, int r, int g, int b, int u, int v) {
    u32 c = 0xFF000000u | (u32)r | ((u32)g << 8) | ((u32)b << 16);
    if (s->tex >= 0) {
        u32 t = atlas[s->tex][v & (PICA_ATLAS - 1)][u & (PICA_ATLAS - 1)];
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
    for (int i = 0; i < 2; i++) if (!atlas[i]) atlas[i] = calloc(PICA_ATLAS, sizeof *atlas[i]);
    return rt && atlas[0] && atlas[1] ? 0 : -1;
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
/* versions of vram[] by 16x16 block (a CLUT animated every frame then only costs the textures that use it, not every
   texture of its 64x256 page) */
static unsigned blk_ver[32][64], ver_clock;
#define CSH 0                         /* cells are 16 x (1 << CSH) pixels (tri_drawn assumes 0) */
static u8 cell_drawn[512 >> CSH][64];   /* cells the GPU drew into since their last read-back: vram[] is stale there */
static u8 page_drawn[2][16];            /* pages with a cell_drawn set (may stay set after its cells are clean) */
static void pages_bump(int x, int y, int w, int h) {
    x &= 1023; y &= 511;
    int a = x >> 4, b = y >> 4, c = (x + w - 1) >> 4, d = (y + h - 1) >> 4;
    if (c > 63) c = 63; if (d > 31) d = 31;
    ++ver_clock;
    for (int j = b; j <= d; j++) for (int i = a; i <= c; i++) blk_ver[j][i] = ver_clock;
}
static unsigned blocks_ver(int x, int y, int w, int h) {   /* the newest version in a rectangle */
    x &= 1023; y &= 511;
    int a = x >> 4, b = y >> 4, c = (x + w - 1) >> 4, d = (y + h - 1) >> 4;
    if (c > 63) c = 63; if (d > 31) d = 31;
    unsigned v = 0;
    for (int j = b; j <= d; j++) for (int i = a; i <= c; i++) if (blk_ver[j][i] > v) v = blk_ver[j][i];
    return v;
}
static unsigned drawn_epoch;             /* moves when a cell or an area is newly marked drawn */
static void pages_drawn(int x1, int y1, int x2, int y2) {   /* (inclusive pixel bounds) */
    if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0; if (x2 > 1023) x2 = 1023; if (y2 > 511) y2 = 511;
    if (x1 > x2 || y1 > y2) return;
    drawn_epoch++;
    for (int j = y1 >> CSH; j <= y2 >> CSH; j++) memset(&cell_drawn[j][x1 >> 4], 1, (size_t)((x2 >> 4) - (x1 >> 4) + 1));
    for (int j = y1 >> 8; j <= y2 >> 8; j++) for (int i = x1 >> 6; i <= x2 >> 6; i++) page_drawn[j][i] = 1;
}

/* Primitives drawn with a screen-sized drawing area (nearly all of them) only note that area: finding the cells each
   one covers costs more than everything else the backend does per primitive, and those cells only matter when a
   texture, a CLUT or a copy later reads from the area, which marks all of it then (more than was drawn, so at worst
   a read-back of pixels vram[] already had). */
static struct { int x1, y1, x2, y2; } bulk[4];
static int nbulk;
/* an area something read from (a CLUT or a texture inside a frame buffer: a debug-select scene does it every frame)
   has its primitives' cells found one by one for a while, or each read would mark, and read back, the whole area */
static struct { int x1, y1, x2, y2, until; } hot[4];
static int nhot;
static int bulk_area(void) {   /* 1: the drawing area is noted, the primitive needs no cells */
    int w = gs->x2 - gs->x1 + 1, h = gs->y2 - gs->y1 + 1;
    if (w < 256 || w > 640 || h < 200 || h > 256) return 0;
    for (int i = 0; i < nhot; i++)
        if (hot[i].x1 == gs->x1 && hot[i].y1 == gs->y1 && hot[i].x2 == gs->x2 && hot[i].y2 == gs->y2) {
            if (port_frame < hot[i].until) return 0;
            hot[i] = hot[--nhot]; break;
        }
    for (int i = 0; i < nbulk; i++) if (bulk[i].x1 == gs->x1 && bulk[i].y1 == gs->y1 && bulk[i].x2 == gs->x2 && bulk[i].y2 == gs->y2) return 1;
    if (nbulk == 4) { pages_drawn(bulk[0].x1, bulk[0].y1, bulk[0].x2, bulk[0].y2); memmove(&bulk[0], &bulk[1], 3 * sizeof bulk[0]); nbulk = 3; }
    drawn_epoch++;
    bulk[nbulk].x1 = gs->x1; bulk[nbulk].y1 = gs->y1; bulk[nbulk].x2 = gs->x2; bulk[nbulk].y2 = gs->y2; nbulk++;
    return 1;
}
static void bulk_resolve(int x, int y, int w, int h) {   /* the noted areas this rectangle touches become cells */
    for (int i = 0; i < nbulk; i++) {
        if (x > bulk[i].x2 || x + w - 1 < bulk[i].x1 || y > bulk[i].y2 || y + h - 1 < bulk[i].y1) continue;
        pages_drawn(bulk[i].x1, bulk[i].y1, bulk[i].x2, bulk[i].y2);
        if (nhot == 4) nhot = 3;
        hot[nhot].x1 = bulk[i].x1; hot[nhot].y1 = bulk[i].y1; hot[nhot].x2 = bulk[i].x2; hot[nhot].y2 = bulk[i].y2;
        hot[nhot++].until = port_frame + 120;
        memmove(&bulk[i], &bulk[i + 1], (size_t)(nbulk - i - 1) * sizeof bulk[0]); nbulk--; i--;
    }
}

/* ---- the batch: triangles with one state */
#define MAX_PV 6144
static unsigned long long use_clock, batch_clock, frame_clock;   /* slot uses: now, at the last flush, at the last present */
static PV bv[MAX_PV];
static int nbv;
static PState bst;
static unsigned long long st_batches, st_tris, st_decodes, st_readbacks, st_rb_tex, st_rb_host, st_rb_px;
static void flush(void) {
    if (!nbv) return;
    dev->tris(&bst, bv, nbv);
    st_batches++; st_tris += nbv / 3;
    nbv = 0; batch_clock = use_clock;
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
    if (nbulk) bulk_resolve(x, y, w, h);
    int any = 0;   /* (most textures: no page of theirs drawn into, nothing to scan) */
    for (int pj = y >> 8; pj <= (y + h - 1) >> 8; pj++) for (int pi = x >> 6; pi <= (x + w - 1) >> 6; pi++) {
        if (!page_drawn[pj][pi]) continue;
        /* a flag can outlive its cells (a fill cleaned them): look at the whole page once and drop it then */
        int left = 0;
        for (int j = pj * 256; j < pj * 256 + 256 && !left; j++) left = cell_drawn[j][pi * 4] | cell_drawn[j][pi * 4 + 1] | cell_drawn[j][pi * 4 + 2] | cell_drawn[j][pi * 4 + 3];
        if (left) any = 1; else page_drawn[pj][pi] = 0;
    }
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
    {   /* vram[] is what the game wrote unless the GPU drew into the rectangle since: only then is the target read.
           (The game stores the CLUT rows it is about to fade and loads them back afterwards; on a console the
           target's copy of those rows was not what had been written, and the ground came back transparent.) */
        int drawn = 0;
        for (int i = 0; i < nbulk && !drawn; i++) drawn = !(x > bulk[i].x2 || x + w - 1 < bulk[i].x1 || y > bulk[i].y2 || y + h - 1 < bulk[i].y1);
        for (int j = y; j < y + h && !drawn; j++) for (int i = x >> 4; i <= (x + w - 1) >> 4 && !drawn; i++) drawn = cell_drawn[j][i];
        if (!drawn) return;
    }
    if (nbulk) bulk_resolve(x, y, w, h);
    read_back(x, y, w, h);
    cells_clean(x, y, w, h);
    st_rb_host++; st_rb_px += (unsigned long long)w * h;
    if (port_verbose && st_rb_host < 60) port_printf("[gpu] pica: read-back %d,%d %dx%d (frame %d)\n", x, y, w, h, port_frame);
}

/* ---- the texture cache */
/* a slot: the block (bx, by, size z, texels of the page) of a page + CLUT + texture window.
   checked: ver_clock + 1 when it was last found valid */
typedef struct { int tx, ty, depth, cx, cy, tw, bx, by; unsigned ver, checked, tver, chash; unsigned long long used; short live, next; } Slot;
static Slot slots[SLOTS];
static short hhead[256];                 /* slots by key (chains through Slot.next), -1 ends */
static int hinit;
static u32 *dec;                         /* 256 x 256 */
/* the vram[] rectangle a block's texels come from (the whole page with a texture window: it maps texels elsewhere) */
static void tex_rect(const GpuTex *t, int tw, int bx, int by, int z, int *x, int *y, int *w, int *h) {
    int sh = t->depth == 0 ? 2 : t->depth == 1 ? 1 : 0;
    if (tw) { *x = t->tx; *y = t->ty; *w = 256 >> sh; *h = 256; }
    else { *x = t->tx + (bx >> sh); *y = t->ty + by; *w = z >> sh; *h = z; }
}
static unsigned tex_ver(const GpuTex *t, int tw, int bx, int by, int z) {   /* the newest version of what the block reads */
    int x, y, w, h; tex_rect(t, tw, bx, by, z, &x, &y, &w, &h);
    unsigned v = blocks_ver(x, y, w, h);
    if (t->depth < 2 && t->cy != PORT_CLUT_Y) {
        unsigned c = blocks_ver(t->cx, t->cy, t->depth ? 256 : 16, 1);
        if (c > v) v = c;
    }
    return v;
}
/* a CLUT rewritten every frame with the same colours (the game uploads its palettes whole) must not cost a decode
   of every texture that uses it: a slot also keeps the version of its texels alone and a hash of its CLUT */
static unsigned clut_hash(const GpuTex *t) {
    if (t->depth == 2 || t->cy == PORT_CLUT_Y) return 0;
    const u16 *vram = port_vram();
    unsigned h = 2166136261u;
    for (int i = 0, n = t->depth ? 256 : 16; i < n; i++) h = (h ^ vram[t->cy * 1024 + ((t->cx + i) & 1023)]) * 16777619u;
    return h;
}
static inline u32 texel(u16 c) { return !c ? 0 : (c15to32(c) & 0x00FFFFFFu) | ((u32)(c & 0x8000 ? A_SEMI : A_OPAQUE) << 24); }
static void decode(const GpuTex *t, int tw, int bx, int by, int z) {   /* the block into dec[], z x z */
    const u16 *vram = port_vram();
    if (!tw && t->cy != PORT_CLUT_Y && (t->depth == 2 || t->cx + (t->depth ? 256 : 16) <= 1024)) {
        /* the usual case (no texture window, a CLUT in vram[] that does not wrap): the CLUT converted once, whole
           words of indices at a time; the same texels as the general loop below */
        static u32 clut[256];
        if (t->depth < 2) for (int i = 0; i < (t->depth ? 256 : 16); i++) clut[i] = texel(vram[t->cy * 1024 + t->cx + i]);
        for (int v = 0; v < z; v++) {
            const u16 *row = &vram[((t->ty + by + v) & 511) * 1024];
            u32 *d = &dec[v * z];
            if (t->depth == 0) for (int k = 0; k < z / 4; k++) {
                u16 w = row[(t->tx + bx / 4 + k) & 1023];
                d[0] = clut[w & 15]; d[1] = clut[(w >> 4) & 15]; d[2] = clut[(w >> 8) & 15]; d[3] = clut[w >> 12]; d += 4;
            } else if (t->depth == 1) for (int k = 0; k < z / 2; k++) {
                u16 w = row[(t->tx + bx / 2 + k) & 1023];
                d[0] = clut[w & 255]; d[1] = clut[w >> 8]; d += 2;
            } else for (int k = 0; k < z; k++) d[k] = texel(row[(t->tx + bx + k) & 1023]);
        }
        return;
    }
    int twmx = tw & 31, twmy = (tw >> 5) & 31, twox = (tw >> 10) & 31, twoy = (tw >> 15) & 31;
    for (int j = 0; j < z; j++) for (int i = 0; i < z; i++) {
        int u0 = bx + i, v0 = by + j;
        int u = (u0 & ~(twmx * 8)) | ((twox & twmx) * 8), v = (v0 & ~(twmy * 8)) | ((twoy & twmy) * 8);
        u16 c;
        if (t->depth == 0) {
            u16 w = vram[((t->ty + v) & 511) * 1024 + ((t->tx + (u >> 2)) & 1023)];
            int k = (w >> ((u & 3) * 4)) & 15;
            c = t->cy == PORT_CLUT_Y ? port_prompt_clut[(t->cx + k) % (PORT_CLUT_ROWS * 16)] : vram[t->cy * 1024 + ((t->cx + k) & 1023)];
        } else if (t->depth == 1) {
            u16 w = vram[((t->ty + v) & 511) * 1024 + ((t->tx + (u >> 1)) & 1023)];
            int k = (w >> ((u & 1) * 8)) & 255;
            c = t->cy == PORT_CLUT_Y ? port_prompt_clut[(t->cx + (k & 15)) % (PORT_CLUT_ROWS * 16)] : vram[t->cy * 1024 + ((t->cx + k) & 1023)];
        } else c = vram[((t->ty + v) & 511) * 1024 + ((t->tx + u) & 1023)];
        dec[j * z + i] = texel(c);
    }
}
extern unsigned long long port_time_us(void);
unsigned long long pica_decode_us;   /* (time in decode(): n3ds/pica_c3d.c logs it) */
/* the slot of the block that holds texels u0..u1 x v0..v1 (inclusive) of the texture: the smallest of the 64 / 128 /
   256 blocks that does; *obx, *oby: the block's origin */
/* the last answer: runs of primitives read the same block (a tile map, a line of text, a sprite's parts), and
   nothing has to be looked at again while neither vram[] nor the drawn cells have changed */
static struct { int ok, tx, ty, depth, cx, cy, tw, bx, by, z, slot; unsigned ver, epoch; } memo;
static int tex_slot(const GpuTex *t, int u0, int v0, int u1, int v1, int *obx, int *oby) {
    int tw = gs->twmx | gs->twmy << 5 | gs->twox << 10 | gs->twoy << 15;
    if (memo.ok && memo.ver == ver_clock && memo.epoch == drawn_epoch && memo.tx == t->tx && memo.ty == t->ty && memo.depth == t->depth
        && memo.tw == tw && (t->depth == 2 || (memo.cx == t->cx && memo.cy == t->cy))
        && u0 >= memo.bx && v0 >= memo.by && u1 < memo.bx + memo.z && v1 < memo.by + memo.z) {
        *obx = memo.bx; *oby = memo.by; slots[memo.slot].used = ++use_clock;
        return memo.slot;
    }
    int z = 256, bx = 0, by = 0;
    if (u0 >= 0 && v0 >= 0 && u1 < 256 && v1 < 256) {
        /* origins on a grid of half the size: any primitive up to 32 texels fits a 64 block, up to 64 a 128 one */
        for (int c = 64; c < 256; c *= 2) {
            int ox = u0 & ~(c / 2 - 1), oy = v0 & ~(c / 2 - 1);
            if (ox + c > 256) ox = 256 - c; if (oy + c > 256) oy = 256 - c;
            if (u1 < ox + c && v1 < oy + c) { z = c; bx = ox; by = oy; break; }
        }
    }
    *obx = bx; *oby = by;
    int cx = t->depth == 2 ? 0 : t->cx, cy = t->depth == 2 ? 0 : t->cy;
    int x, y, w, h; tex_rect(t, tw, bx, by, z, &x, &y, &w, &h);
    sync_pages(x, y, w, h);
    if (t->depth < 2 && t->cy != PORT_CLUT_Y) sync_pages(t->cx, t->cy, t->depth ? 256 : 16, 1);
    if (!hinit) { memset(hhead, -1, sizeof hhead); hinit = 1; }
    unsigned hk = ((unsigned)(t->tx >> 6) * 7 + (unsigned)(t->ty >> 8) * 3 + (unsigned)cx * 13 + (unsigned)cy * 31 + (unsigned)(bx >> 5) * 5
                   + (unsigned)(by >> 5) * 17 + (unsigned)z + (unsigned)tw * 11 + (unsigned)t->depth) & 255;
    int i;
    for (i = hhead[hk]; i >= 0; i = slots[i].next) {
        Slot *s = &slots[i];
        if (s->tx == t->tx && s->ty == t->ty && s->depth == t->depth && s->tw == tw && s->cx == cx && s->cy == cy
            && s->bx == bx && s->by == by && pica_slot_size(i) == z) break;
    }
    unsigned v;
    unsigned long long was = 0;   /* the slot's last use, by this block or the one it replaces */
    if (i >= 0) {
        Slot *s = &slots[i];
        if (s->checked == ver_clock + 1) { s->used = ++use_clock; goto found; }   /* (nothing in vram[] changed since) */
        v = tex_ver(t, tw, bx, by, z);
        if (s->ver >= v) { s->checked = ver_clock + 1; s->used = ++use_clock; goto found; }
        { int x2, y2, w2, h2; tex_rect(t, tw, bx, by, z, &x2, &y2, &w2, &h2);
          if (blocks_ver(x2, y2, w2, h2) <= s->tver && clut_hash(t) == s->chash) {   /* only the CLUT was written, the same */
              s->ver = v; s->checked = ver_clock + 1; s->used = ++use_clock; goto found;
          } }
        was = s->used;   /* (stale: decoded again in its slot) */
    } else {
        int a = z == 256 ? 0 : z == 128 ? PICA_N256 : PICA_N256 + PICA_N128;
        int n = z == 256 ? PICA_N256 : z == 128 ? PICA_N128 : PICA_N64;
        i = a;
        for (int k = a; k < a + n; k++) {
            if (!slots[k].live) { i = k; break; }
            if (slots[k].used < slots[i].used) i = k;
        }
        if (memo.slot == i) memo.ok = 0;
        if (slots[i].live) {   /* out of its chain */
            Slot *o = &slots[i];
            int ocx = o->cx, ocy = o->cy;
            unsigned ok = ((unsigned)(o->tx >> 6) * 7 + (unsigned)(o->ty >> 8) * 3 + (unsigned)ocx * 13 + (unsigned)ocy * 31 + (unsigned)(o->bx >> 5) * 5
                           + (unsigned)(o->by >> 5) * 17 + (unsigned)z + (unsigned)o->tw * 11 + (unsigned)o->depth) & 255;
            short *q = &hhead[ok];
            while (*q >= 0 && *q != i) q = &slots[*q].next;
            if (*q == i) *q = o->next;
        }
        v = tex_ver(t, tw, bx, by, z);
        was = slots[i].live ? slots[i].used : 0;
        slots[i] = (Slot){ .tx = t->tx, .ty = t->ty, .depth = t->depth, .cx = cx, .cy = cy, .tw = tw, .bx = bx, .by = by, .live = 1, .next = hhead[hk] };
        hhead[hk] = (short)i;
    }
    if (nbv && was > batch_clock) flush();   /* the pending batch samples the slot about to change */
    if (port_verbose > 1) port_printf("[gpu] DEC %s z%d tex %d,%d d%d clut %d,%d b %d,%d tw %x (frame %d)\n", slots[i].ver ? "stale" : "new", z, t->tx, t->ty, t->depth, t->cx, t->cy, bx, by, tw, port_frame);
    unsigned long long t0 = port_time_us();
    decode(t, tw, bx, by, z);
    pica_decode_us += port_time_us() - t0;
    dev->tex_upload(i, dec, was > frame_clock);
    st_decodes++;
    slots[i].ver = v; slots[i].checked = ver_clock + 1; slots[i].used = ++use_clock;
    slots[i].tver = blocks_ver(x, y, w, h); slots[i].chash = clut_hash(t);
found:
    memo.ok = 1; memo.tx = t->tx; memo.ty = t->ty; memo.depth = t->depth; memo.cx = t->cx; memo.cy = t->cy; memo.tw = tw;
    memo.bx = bx; memo.by = by; memo.z = z; memo.slot = i; memo.ver = ver_clock; memo.epoch = drawn_epoch;
    return i;
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
/* Clipping to the drawing area, here and not left to the device's scissor: a PICA200 does not clip in x / y, and a
   console draws nothing of a triangle with a vertex on or past the target's right edge (x = 1024; gputest.bmp: the
   full-width fills missing, an 8 x 8 mark at x 952 right) where Azahar draws it.  The second frame buffer ends at that
   edge, and sprites cross it.  The far bounds stop short of the edge, past the last pixel's centre.
   On by default on the 3DS; PICA_CLIP=1 in the environment tries it on the reference device. */
static int clip_on = -1;
/* profiling on the 3DS (n3ds/pica_c3d.c logs it): one ordering table in sixteen is timed, in system ticks:
   [0] its packets, [1] emit(), [2] the clipping, [3] the texture look-up, [4] the device's triangles */
int pica_prof_on; unsigned long long pica_prof[6]; unsigned pica_prof_n;
unsigned long long pica_ot[4];   /* (shims/gpu.c DrawOTag: ticks in drawn / skipped tables, and how many of each) */
#ifdef __3DS__
extern unsigned long long port_ticks(void);
#define PT() (pica_prof_on ? port_ticks() : 0)
#define PA(i, t) do { if (pica_prof_on) pica_prof[i] += port_ticks() - (t); } while (0)
#else
#define PT() 0
#define PA(i, t) do { (void)(t); } while (0)
#endif
static int clip_poly(const PV *in, int n, PV *out, int axis, float bound, int keep_less) {
    int m = 0;
    for (int i = 0; i < n; i++) {
        const PV *a = &in[i], *b = &in[(i + 1) % n];
        float da = (axis ? a->y : a->x) - bound, db = (axis ? b->y : b->x) - bound;
        int ia = keep_less ? da <= 0 : da >= 0, ib = keep_less ? db <= 0 : db >= 0;
        if (ia) out[m++] = *a;
        if (ia != ib) {
            float t = da / (da - db);
            PV *o = &out[m++];
            o->x = a->x + (b->x - a->x) * t; o->y = a->y + (b->y - a->y) * t;
            o->u = a->u + (b->u - a->u) * t; o->v = a->v + (b->v - a->v) * t;
            o->r = (u8)(a->r + (b->r - a->r) * t + 0.5f); o->g = (u8)(a->g + (b->g - a->g) * t + 0.5f);
            o->b = (u8)(a->b + (b->b - a->b) * t + 0.5f); o->a = a->a;
            if (axis) o->y = bound; else o->x = bound;
        }
    }
    return m;
}
/* the triangles of v[0..n) inside the drawing area into out (15 vertices a triangle at most); the count */
static int clip_tris(const PV *v, int n, PV *out) {
    float X1 = (float)gs->x1, Y1 = (float)gs->y1;
    float X2 = gs->x2 + 1 > 1023 ? 1023.4f : (float)(gs->x2 + 1), Y2 = gs->y2 + 1 > 511 ? 511.4f : (float)(gs->y2 + 1);
    int m = 0;
    for (int t = 0; t + 2 < n; t += 3) {
        const PV *q = &v[t];
        int in = 1;
        for (int i = 0; i < 3; i++) if (q[i].x < X1 || q[i].x > X2 || q[i].y < Y1 || q[i].y > Y2) in = 0;
        if (in) { out[m++] = q[0]; out[m++] = q[1]; out[m++] = q[2]; continue; }
        if ((q[0].x < X1 && q[1].x < X1 && q[2].x < X1) || (q[0].x > X2 && q[1].x > X2 && q[2].x > X2) ||
            (q[0].y < Y1 && q[1].y < Y1 && q[2].y < Y1) || (q[0].y > Y2 && q[1].y > Y2 && q[2].y > Y2)) continue;
        PV a[8], b[8]; int k;
        k = clip_poly(q, 3, a, 0, X1, 0);
        k = clip_poly(a, k, b, 0, X2, 1);
        k = clip_poly(b, k, a, 1, Y1, 0);
        k = clip_poly(a, k, b, 1, Y2, 1);
        for (int i = 1; i + 1 < k; i++) { out[m++] = b[0]; out[m++] = b[i]; out[m++] = b[i + 1]; }
    }
    return m;
}
static void emit(const PV *v0, int n, const GpuTex *tex, int semi, int raw) {
    PState s = base_state();
    PV cl[30], vv[30]; const PV *v;
    if (clip_on < 0) {
#ifdef __3DS__
        clip_on = 1;
#else
        clip_on = getenv("PICA_CLIP") != 0;
#endif
    }
    const PV *orig = v0; int norig = n;
    unsigned long long pt = PT(), pc = pt;
    if (clip_on) { n = clip_tris(v0, n, cl); PA(2, pc); if (!n) { PA(1, pt); return; } v0 = cl; }
    v = v0;
    if (tex) {
        /* the texels the primitive reads: its vertices' bounds */
        int u0 = (int)orig[0].u, u1 = u0, w0 = (int)orig[0].v, w1 = w0, bx, by;
        for (int i = 1; i < norig; i++) {
            int u = (int)orig[i].u, w = (int)orig[i].v;
            if (u < u0) u0 = u; if (u > u1) u1 = u; if (w < w0) w0 = w; if (w > w1) w1 = w;
        }
        /* (the far edge kept when it is in the page: a magnified sprite's last pixels can sample it) */
        if (u1 > 255 && u0 < 256) u1 = 255; if (w1 > 255 && w0 < 256) w1 = 255;
        unsigned long long px = PT();
        int slot = tex_slot(tex, u0, w0, u1, w1, &bx, &by), ax, ay;
        PA(3, px);
        pica_slot_pos(slot, &ax, &ay);
        s.tex = pica_slot_atlas(slot); s.mod = !raw;
        for (int i = 0; i < n; i++) { vv[i] = v0[i]; vv[i].u += (float)(ax - bx); vv[i].v += (float)(ay - by); }
        v = vv;
    }
    if (!tex) { s.blend = semi ? gs->abr : -1; memcpy(batch(&s, n), v, n * sizeof *v); PA(1, pt); return; }
    if (!semi) { s.atest = AT_NOTZERO; memcpy(batch(&s, n), v, n * sizeof *v); PA(1, pt); return; }
    s.atest = AT_OPAQUE; memcpy(batch(&s, n), v, n * sizeof *v);
    s.atest = AT_SEMI; s.blend = gs->abr; memcpy(batch(&s, n), v, n * sizeof *v);
    PA(1, pt);
}
static void bbox_drawn(const PV *v, int n) {
    if (bulk_area()) return;
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
    if (bulk_area()) return;
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
    /* per row (cells are 16 x 1, CSH 0), each edge allows px >= L (ey < 0) or px <= R (ey > 0), px a cell's corner:
       a cell [16 ci, 16 ci + 15] is outside an edge when both its corners are, so the cells kept are
       ci >= floor(L / 16) and ci <= floor(R / 16) over the three edges; the same cells as testing every corner */
    for (int cy = y1; cy <= y2; cy++) {
        long long lo = x1 >> 4, hi = x2 >> 4;
        for (int k = 0; k < 3 && lo <= hi; k++) {   /* inside: (q1 - q0) x (p - q0) >= 0 */
            int k1 = (k + 1) % 3; long long ex = x[k1] - x[k], ey = y[k1] - y[k], n = ex * (cy - y[k]);
            if (!ey) { if (n < 0) lo = hi + 1; continue; }
            /* ey > 0: px - x0 <= n / ey (floor); ey < 0: px - x0 >= n / ey (ceil) */
            if (ey > 0) { long long q = n >= 0 ? n / ey : -((-n + ey - 1) / ey); long long c = (x[k] + q) >> 4; if (c < hi) hi = c; }
            else { long long d = -ey, m = -n; long long q = m >= 0 ? (m + d - 1) / d : -((-m) / d); long long c = (x[k] + q) >> 4; if (c > lo) lo = c; }
        }
        if (lo > hi) continue;
        drawn_epoch++;
        memset(&cell_drawn[cy][lo], 1, (size_t)(hi - lo + 1));
        for (long long pi = lo >> 2; pi <= hi >> 2; pi++) page_drawn[cy >> 8][pi] = 1;
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
    /* a fill inside a noted drawing area (the frame's clear): vram[] is stale there anyway and stays marked so, the
       device alone fills (82000 words a frame otherwise, for nothing) */
    for (int i = 0; i < nbulk; i++)
        if (x >= bulk[i].x1 && y >= bulk[i].y1 && x + w - 1 <= bulk[i].x2 && y + h - 1 <= bulk[i].y2) { dev->fill(x, y, w, h, c15to32(c)); return; }
    for (int j = 0; j < h; j++) {
        u16 *row = &vram[((y + j) & 511) * 1024];
        if (x >= 0 && x + w <= 1024) { u16 *d = row + x; for (int i = 0; i < w; i++) d[i] = c; }
        else for (int i = 0; i < w; i++) row[(x + i) & 1023] = c;
    }
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
    frame_clock = use_clock;
    return dev->present ? dev->present(dx, dy, dw, dh, masked) : 0;
}
const GpuBackend port_gpu_pica = { "pica (3DS model, CPU reference)", pica_start, pica_stop, pica_tri, pica_rect, pica_line, pica_fill,
                                   pica_copy, pica_vram_written, pica_vram_read, pica_list_end, pica_present, 0 };
