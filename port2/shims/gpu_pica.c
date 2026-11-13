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
#include <stdint.h>
#include <string.h>
#include "port_runtime.h"
#include "gpu_backend.h"

#include "gpu_pica.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
#define A_OPAQUE PICA_A_OPAQUE
#define A_SEMI PICA_A_SEMI
#define SLOTS PICA_SLOTS

/* ================================================================== the reference device (CPU) */
extern unsigned long long pica_cnt[8];
extern int pica_gpu_clips;
static u32 (*rt)[1024];                  /* the render target, 512 rows (allocated by pica_start: the image stays small) */
static u32 (*wrt)[1024];                 /* the wide plane (--widescreen) */
static u32 (*ert)[1024];                 /* the right eye's target (--stereo) */
/* (the changes of target a 3DS would make, by what made them: 0 triangles, 1 a fill, 2 a write, 3 a wide fill, 4 a
   wide write, 5 the copy down; logged at the end) */
static int ref_cur; static unsigned long long ref_sw[6];
#define REF_T(w, why) do { if (ref_cur != (w)) { ref_cur = (w); ref_sw[why]++; } } while (0)
static u32 (*atlas[3])[PICA_ATLAS];      /* the texture atlases (both PICA_ATLAS squared here) */
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
    u32 *d = s->wide == 2 ? &ert[y][x] : s->wide ? &wrt[y][x] : &rt[y][x];
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
int pica_stereo_frame;   /* --stereo: this table's batches are drawn again for the right eye (x + sh), gpu.c sets it */
static void ref_tris(const PState *s, const PV *v, int n) {
    if (pica_stereo_frame && ert && !s->wide && s->x2 - s->x1 >= 255 && s->y2 - s->y1 >= 199) {   /* (the device: a replay at the frame's end) */
        PState e = *s; e.wide = 2; PV t[3];
        for (int i = 0; i + 2 < n; i += 3) { for (int k = 0; k < 3; k++) { t[k] = v[i + k]; t[k].x += t[k].sh; } ref_tri(&e, &t[0], &t[1], &t[2]); }
    }
    REF_T(s->wide, 0); for (int i = 0; i + 2 < n; i += 3) ref_tri(s, &v[i], &v[i + 1], &v[i + 2]); }
static void ref_fill(int x, int y, int w, int h, u32 c) {
    REF_T(0, 1);
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) rt[(y + j) & 511][(x + i) & 1023] = c;
}
static void ref_read(int x, int y, int w, int h, u32 *d, int stride) {
    for (int j = 0; j < h; j++) memcpy(d + (size_t)j * stride, &rt[y + j][x], (size_t)w * 4);
}
static void ref_write(int x, int y, int w, int h, const u32 *s, int stride) {
    REF_T(0, 2);
    for (int j = 0; j < h; j++) memcpy(&rt[y + j][x], s + (size_t)j * stride, (size_t)w * 4);
}
static int ref_start(void) {
    if (!rt) rt = calloc(512, sizeof *rt);
    for (int i = 0; i < 3; i++) if (!atlas[i]) atlas[i] = calloc(PICA_ATLAS, sizeof *atlas[i]);
    if (getenv("PICA_GPU_CLIPS")) pica_gpu_clips = atoi(getenv("PICA_GPU_CLIPS"));   /* (the sides a 3DS's probe would report: 15 = all) */
    pica_n128x = getenv("PICA_NO128X") ? 0 : PICA_N128X;   /* (PICA_NO128X: a device without the third atlas) */
    return rt && atlas[0] && atlas[1] ? 0 : -1;
}
static u32 c15to32_(u16 c);
static int ref_wide_start(void) { if (!wrt) wrt = calloc(512, sizeof *wrt); return wrt ? 0 : -1; }
static void ref_wide_fill(int x, int y, int w, int h, u32 c) { REF_T(1, 3); for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) wrt[(y + j) & 511][(x + i) & 1023] = c; }
static void ref_wide_write(int x, int y, int w, int h, const u16 *s, int stride) {
    REF_T(1, 4);
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) wrt[y + j][x + i] = c15to32_(s[j * stride + i]);
}
static void ref_wide_down(int wx, int wy, int x, int y, int w, int h) { REF_T(0, 5); for (int j = 0; j < h; j++) memcpy(&rt[y + j][x], &wrt[wy + j][wx], (size_t)w * 4); }
static void ref_wide_read(int x, int y, int w, int h, u32 *d, int stride) { for (int j = 0; j < h; j++) memcpy(d + (size_t)j * stride, &wrt[y + j][x], (size_t)w * 4); }
static int ref_eye_start(void) { if (!ert) ert = calloc(512, sizeof *ert); return ert ? 0 : -1; }
static void ref_eye_fill(int x, int y, int w, int h, u32 c) { for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) ert[(y + j) & 511][(x + i) & 1023] = c; }
static const PicaDev pd_ref = { ref_start, ref_tex_upload, ref_tris, ref_fill, ref_read, ref_write, 0, 0, 0,
                                ref_wide_start, ref_wide_fill, ref_wide_write, ref_wide_down, 0, ref_wide_read, 0, ref_eye_start, ref_eye_fill };
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
static u32 c15to32_(u16 c) { return c15to32(c); }
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
/* per page, a count that moves when something is newly marked drawn in it (a cell, or a noted area over it): a cached
   block is looked at again (sync_pages) only when a page its texels or its CLUT are in has moved.  One count for the
   whole of VRAM sent every block through sync_pages once a frame in a scene that draws over a palette each frame,
   200 look-ups a frame each scanning a page's cells: 6 ms of a drawn frame on an Old 3DS. */
static unsigned page_gen[2][16];
static void pages_drawn(int x1, int y1, int x2, int y2) {   /* (inclusive pixel bounds) */
    if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0; if (x2 > 1023) x2 = 1023; if (y2 > 511) y2 = 511;
    if (x1 > x2 || y1 > y2) return;
    drawn_epoch++;
    for (int j = y1 >> CSH; j <= y2 >> CSH; j++) memset(&cell_drawn[j][x1 >> 4], 1, (size_t)((x2 >> 4) - (x1 >> 4) + 1));
    for (int j = y1 >> 8; j <= y2 >> 8; j++) for (int i = x1 >> 6; i <= x2 >> 6; i++) { page_drawn[j][i] = 1; page_gen[j][i]++; }
}
static void gens_bump(int x1, int y1, int x2, int y2) {   /* (an area newly noted, bulk_area: no cells yet) */
    if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0; if (x2 > 1023) x2 = 1023; if (y2 > 511) y2 = 511;
    for (int j = y1 >> 8; j <= y2 >> 8; j++) for (int i = x1 >> 6; i <= x2 >> 6; i++) page_gen[j][i]++;
}
/* the sum of the counts of a rectangle's pages (they only go up: the same sum, nothing moved) */
static unsigned gens_of(int x, int y, int w, int h) {
    x &= 1023; y &= 511;
    if (x + w > 1024) w = 1024 - x; if (y + h > 512) h = 512 - y;
    unsigned g = 0;
    for (int j = y >> 8; j <= (y + h - 1) >> 8; j++) for (int i = x >> 6; i <= (x + w - 1) >> 6; i++) g += page_gen[j][i];
    return g;
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
static struct { int x1, y1, x2, y2, ok; } bulk_memo;   /* the last area bulk_area() found noted (bulk_in) */
static int bulk_note(int ax1, int ay1, int ax2, int ay2) {   /* the same of any area: 1 = noted */
    bulk_memo.ok = 0;
    int w = ax2 - ax1 + 1, h = ay2 - ay1 + 1;
    if (w < 256 || w > 640 || h < 200 || h > 256) return 0;
    for (int i = 0; i < nhot; i++)
        if (hot[i].x1 == ax1 && hot[i].y1 == ay1 && hot[i].x2 == ax2 && hot[i].y2 == ay2) {
            if (port_frame < hot[i].until) return 0;
            hot[i] = hot[--nhot]; break;
        }
    for (int i = 0; i < nbulk; i++) if (bulk[i].x1 == ax1 && bulk[i].y1 == ay1 && bulk[i].x2 == ax2 && bulk[i].y2 == ay2) goto noted;
    if (nbulk == 4) { pages_drawn(bulk[0].x1, bulk[0].y1, bulk[0].x2, bulk[0].y2); memmove(&bulk[0], &bulk[1], 3 * sizeof bulk[0]); nbulk = 3; }
    drawn_epoch++;
    gens_bump(ax1, ay1, ax2, ay2);
    bulk[nbulk].x1 = ax1; bulk[nbulk].y1 = ay1; bulk[nbulk].x2 = ax2; bulk[nbulk].y2 = ay2; nbulk++;
noted:
    bulk_memo.x1 = ax1; bulk_memo.y1 = ay1; bulk_memo.x2 = ax2; bulk_memo.y2 = ay2; bulk_memo.ok = 1;
    return 1;
}
static int bulk_area(void) {   /* 1: the drawing area is noted, the primitive needs no cells */
    /* (--widescreen: the wide plane is drawn, not VRAM: nothing of VRAM goes stale until its 4:3 middle is copied
       down, pica_ws_copy_down) */
    if (gs->wide) { bulk_memo.ok = 0; return 1; }
    return bulk_note(gs->x1, gs->y1, gs->x2, gs->y2);
}
/* (the same answer without the search while the drawing area is the one last found noted) */
static inline int bulk_in(void) {
    return (bulk_memo.ok && bulk_memo.x1 == gs->x1 && bulk_memo.y1 == gs->y1 && bulk_memo.x2 == gs->x2 && bulk_memo.y2 == gs->y2) || bulk_area();
}
static void bulk_resolve(int x, int y, int w, int h) {   /* the noted areas this rectangle touches become cells */
    for (int i = 0; i < nbulk; i++) {
        if (x > bulk[i].x2 || x + w - 1 < bulk[i].x1 || y > bulk[i].y2 || y + h - 1 < bulk[i].y1) continue;
        bulk_memo.ok = 0;
        pages_drawn(bulk[i].x1, bulk[i].y1, bulk[i].x2, bulk[i].y2);
        if (nhot == 4) nhot = 3;
        hot[nhot].x1 = bulk[i].x1; hot[nhot].y1 = bulk[i].y1; hot[nhot].x2 = bulk[i].x2; hot[nhot].y2 = bulk[i].y2;
        hot[nhot++].until = port_frame + 120;
        memmove(&bulk[i], &bulk[i + 1], (size_t)(nbulk - i - 1) * sizeof bulk[0]); nbulk--; i--;
    }
}

/* ---- the batch: triangles with one state */
#define MAX_PV 6144
extern int pica_dry;
static unsigned long long use_clock, batch_clock, frame_clock;   /* slot uses: now, at the last flush, at the last present */
static PV bv_own[MAX_PV], *bv = bv_own;   /* (or the device's memory: PicaDev.vbuf) */
static int nbv, bv_room = MAX_PV;
static PState bst;
static unsigned long long st_batches, st_tris, st_decodes, st_readbacks, st_rb_tex, st_rb_host, st_rb_px;
static void flush(void) {
    if (!nbv) return;
    if (pica_dry) { nbv = 0; batch_clock = use_clock; return; }
    dev->tris(&bst, bv, nbv);
    st_batches++; st_tris += nbv / 3;
    nbv = 0; batch_clock = use_clock;
}
/* Host writes wait: what the game loads into VRAM is in vram[] at once (the texture cache and the host's reads use
   that), and goes to the device's target only when the target needs it: before a primitive whose drawing area the
   rectangle touches, a fill over part of it, a read-back or a present of it.  Most of what a game loads is textures
   and palettes, which the target never needs: a top-down area that draws its floor into a page every frame wrote it
   to the GPU for nothing each time (6 ms a frame on an Old 3DS), and a movie's frame arrives as twenty strips, which
   merge here into one write.  A rectangle written again is the same wait (its words are read when it goes). */
static u32 *stage;                       /* (defined below: 1024 x 512) */
#define NPEND 24
static struct { int x, y, w, h; } pend[NPEND];
static int npend;
/* --widescreen: the wide plane's 4:3 middle is owed to VRAM's target after every ordering table (pica_ws_copy_down),
   and waits like a host write: it goes when the target's rectangle is read, drawn into, written or shown.  A frame is
   then drawn and shown on the wide plane alone; the copy each time was a change of target and back in every frame,
   which a 3DS's GPU pays for (5 to 11 ms a frame waited for it, measured). */
static struct { int wx, wy, x, y, w, h, fill; u32 col; } wd[4];   /* (fill: a clear of the buffer waits instead, pica_fill) */
static int nwd;
static struct { int x, y, w, h; } wdbuf[4]; static int nwdbuf;    /* the buffers that have had a copy: the frame's */
static void wd_resolve(int x, int y, int w, int h) {   /* those this rectangle touches (no batch is open here) */
    for (int i = 0; i < nwd; i++)
        if (x < wd[i].x + wd[i].w && x + w > wd[i].x && y < wd[i].y + wd[i].h && y + h > wd[i].y) {
            if (wd[i].fill) dev->fill(wd[i].x, wd[i].y, wd[i].w, wd[i].h, wd[i].col);
            else dev->wide_down(wd[i].wx, wd[i].wy, wd[i].x, wd[i].y, wd[i].w, wd[i].h);
            wd[i] = wd[--nwd]; i--;
        }
}
static void pend_go(int i) {   /* (no batch is open here) */
    const u16 *vram = port_vram();
    const int x = pend[i].x, y = pend[i].y, w = pend[i].w, h = pend[i].h;
    memmove(&pend[i], &pend[i + 1], (size_t)(npend - i - 1) * sizeof pend[0]); npend--;
    if (nwd) wd_resolve(x, y, w, h);
    /* (a movie's frame is one of these, 15 times a second: the pass to RGBA8 here and the device's own to its texel
       order were as long as decoding the picture) */
    if (dev->write15) { dev->write15(x, y, w, h, vram + y * 1024 + x, 1024); return; }
    for (int j = 0; j < h; j++) for (int k = 0; k < w; k++) stage[j * w + k] = c15to32(vram[(y + j) * 1024 + x + k]);
    dev->write(x, y, w, h, stage, w);
}
static void pend_flush(int x, int y, int w, int h) {   /* those this rectangle touches */
    for (int i = 0; i < npend; i++)
        if (x < pend[i].x + pend[i].w && x + w > pend[i].x && y < pend[i].y + pend[i].h && y + h > pend[i].y) { pend_go(i); i--; }
}
static void pend_add(int x, int y, int w, int h) {
    for (int i = 0; i < npend; i++) {
        if (x >= pend[i].x && y >= pend[i].y && x + w <= pend[i].x + pend[i].w && y + h <= pend[i].y + pend[i].h) return;   /* inside one that waits */
        /* beside one of the same height or width (a movie's strips, a picture loaded in pieces): one rectangle */
        if (y == pend[i].y && h == pend[i].h && x <= pend[i].x + pend[i].w && x + w >= pend[i].x) {
            int x2 = x + w > pend[i].x + pend[i].w ? x + w : pend[i].x + pend[i].w;
            if (x < pend[i].x) pend[i].x = x;
            pend[i].w = x2 - pend[i].x; return;
        }
        if (x == pend[i].x && w == pend[i].w && y <= pend[i].y + pend[i].h && y + h >= pend[i].y) {
            int y2 = y + h > pend[i].y + pend[i].h ? y + h : pend[i].y + pend[i].h;
            if (y < pend[i].y) pend[i].y = y;
            pend[i].h = y2 - pend[i].y; return;
        }
    }
    if (npend == NPEND) pend_go(0);
    pend[npend].x = x; pend[npend].y = y; pend[npend].w = w; pend[npend].h = h; npend++;
}
static PV *batch(const PState *s, int n) {
    if (nbv && (nbv + n > bv_room || s->tex != bst.tex || s->mod != bst.mod || s->atest != bst.atest || s->blend != bst.blend
                || s->x1 != bst.x1 || s->y1 != bst.y1 || s->x2 != bst.x2 || s->y2 != bst.y2 || s->wide != bst.wide)) flush();
    if (!nbv && npend && !pica_dry && !s->wide) pend_flush(s->x1, s->y1, s->x2 - s->x1 + 1, s->y2 - s->y1 + 1);
    if (!nbv && nwd && !pica_dry && !s->wide) wd_resolve(s->x1, s->y1, s->x2 - s->x1 + 1, s->y2 - s->y1 + 1);
    if (!nbv && dev->vbuf) bv = dev->vbuf(&bv_room);
    bst = *s;
    PV *p = bv + nbv; nbv += n;
    return p;
}

/* ---- coherence with vram[] */
static u32 *stage;                       /* 1024 x 512 */
static void read_back(int x, int y, int w, int h) {   /* the target's rectangle into vram[] */
    u16 *vram = port_vram();
    if (npend) pend_flush(x, y, w, h);
    if (nwd) wd_resolve(x, y, w, h);
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
    int any = 0;   /* (most textures: no page of theirs drawn into, nothing to scan; a flag can outlive its cells, and
                      then only the rectangle's own cells are looked at below, not the page's) */
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
static int pend_now;   /* 1: a host write goes to the device at once */
static void pica_vram_written(int x, int y, int w, int h) {
    clip_rect(&x, &y, &w, &h);
    if (w <= 0 || h <= 0) return;
    flush();
    if (pend_now) {   /* (the whole of VRAM at the start) */
        const u16 *vram = port_vram();
        for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) stage[j * w + i] = c15to32(vram[(y + j) * 1024 + x + i]);
        dev->write(x, y, w, h, stage, w);
    } else pend_add(x, y, w, h);
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
typedef struct { int tx, ty, depth, cx, cy, tw, bx, by; unsigned ver, checked, tver, chash, updated, synced, srch; unsigned long long used; short live, next; } Slot;
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
static unsigned long long st_same;   /* blocks whose words were written again, the same: not decoded */
static unsigned src_hash(int x, int y, int w, int h) {   /* a block's words in vram[] */
    const u16 *vram = port_vram(); unsigned hsh = 2166136261u;
    for (int j = 0; j < h; j++) {
        const u16 *row = &vram[((y + j) & 511) * 1024], *r = row + (x & 1023);
        int i = 0;
        if (x + w > 1024) { for (; i < w; i++) hsh = (hsh ^ row[(x + i) & 1023]) * 16777619u; continue; }
        if (!((uintptr_t)r & 3)) for (; i + 2 <= w; i += 2) hsh = (hsh ^ *(const u32 *)(r + i)) * 16777619u;
        for (; i < w; i++) hsh = (hsh ^ r[i]) * 16777619u;
    }
    return hsh;
}
static unsigned clut_hash(const GpuTex *t) {
    if (t->depth == 2 || t->cy == PORT_CLUT_Y) return 0;
    const u16 *vram = port_vram();
    unsigned h = 2166136261u;
    for (int i = 0, n = t->depth ? 256 : 16; i < n; i++) h = (h ^ vram[t->cy * 1024 + ((t->cx + i) & 1023)]) * 16777619u;
    return h;
}
static inline u32 texel(u16 c) { return !c ? 0 : (c15to32(c) & 0x00FFFFFFu) | ((u32)(c & 0x8000 ? A_SEMI : A_OPAQUE) << 24); }
static int dec_kinds;   /* (set by decode: see there) */
static void decode(const GpuTex *t, int tw, int bx, int by, int z) {   /* the block into dec[], z x z */
    const u16 *vram = port_vram();
    if (!tw && t->cy != PORT_CLUT_Y && (t->depth == 2 || t->cx + (t->depth ? 256 : 16) <= 1024)) {
        /* the usual case (no texture window, a CLUT in vram[] that does not wrap): the CLUT converted once, whole
           words of indices at a time; the same texels as the general loop below */
        static u32 clut[256];
        /* dec_kinds: the classes of texel the CLUT can give (1 opaque, 2 semi-transparent); one class alone, and every
           part of the block has that one or nothing: the look at each texel for its parts' classes is not needed
           (a top-down area decodes three 256 x 256 blocks a frame) */
        dec_kinds = 0;
        if (t->depth < 2) for (int i = 0; i < (t->depth ? 256 : 16); i++) { clut[i] = texel(vram[t->cy * 1024 + t->cx + i]); u32 a = clut[i] >> 24; if (a) dec_kinds |= a == A_SEMI ? 2 : 1; }
        else dec_kinds = 3;
        const int depth = t->depth, tx = t->tx, ty = t->ty;   /* (locals: the stores to dec[] could be to *t for all the compiler knows) */
        u32 *const out = dec;
        for (int v = 0; v < z; v++) {
            const u16 *row = &vram[((ty + by + v) & 511) * 1024];
            u32 *d = &out[v * z];
            if (depth == 0) {
                const int x0 = tx + bx / 4;
                if (x0 + z / 4 <= 1024) { const u16 *r = row + x0; for (int k = 0; k < z / 4; k++) { const u16 w = r[k]; d[0] = clut[w & 15]; d[1] = clut[(w >> 4) & 15]; d[2] = clut[(w >> 8) & 15]; d[3] = clut[w >> 12]; d += 4; } }
                else for (int k = 0; k < z / 4; k++) { const u16 w = row[(x0 + k) & 1023]; d[0] = clut[w & 15]; d[1] = clut[(w >> 4) & 15]; d[2] = clut[(w >> 8) & 15]; d[3] = clut[w >> 12]; d += 4; }
            } else if (depth == 1) {
                const int x0 = tx + bx / 2;
                if (x0 + z / 2 <= 1024) { const u16 *r = row + x0; for (int k = 0; k < z / 2; k++) { const u16 w = r[k]; d[0] = clut[w & 255]; d[1] = clut[w >> 8]; d += 2; } }
                else for (int k = 0; k < z / 2; k++) { const u16 w = row[(x0 + k) & 1023]; d[0] = clut[w & 255]; d[1] = clut[w >> 8]; d += 2; }
            } else for (int k = 0; k < z; k++) d[k] = texel(row[(tx + bx + k) & 1023]);
        }
        return;
    }
    dec_kinds = 3;
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
int pica_lazy_clut;
static unsigned present_no;
/* A semi-transparent textured primitive is drawn in a pass for each class of texel it has, opaque then blended, and
   so ends the batch twice.  Most have one class only (an area's fog was 220 translucent triangles a frame, two
   batches each): the classes are kept for the 64 parts of each block, and a primitive looks at the parts its texels
   are in (all of them with a texture window, which maps texels elsewhere). */
static unsigned char slot_kinds[SLOTS][64];
static int prim_kinds(int slot, int u0, int v0, int u1, int v1, int bx, int by) {
    const unsigned char *kc = slot_kinds[slot];
    const int z = pica_slot_size(slot), sh = z == 64 ? 3 : z == 128 ? 4 : 5;
    int a = (u0 - bx) >> sh, b = (v0 - by) >> sh, c = (u1 - bx) >> sh, d = (v1 - by) >> sh, k = 0;
    if (memo.tw || a < 0 || b < 0 || c > 7 || d > 7 || a > c || b > d) { a = b = 0; c = d = 7; }
    for (int j = b; j <= d; j++) for (int i = a; i <= c; i++) k |= kc[j * 8 + i];
    return k;
}
/* The slot for a new block of a size whose slots are in two atlases (a, n: the first one's; b, m: the other's): a
   change of atlas ends a batch, so the first atlas is filled first, and a block in it that no frame has used for a
   while (a second or so of presents) leaves before a slot of the other atlas is opened; with both full, the least
   recently used of all.  (Slots never empty: without this a scene that fits one atlas ended up spread over both.) */
static unsigned long long clock_hist[64];   /* use_clock at the last 64 presents */
static int slot_pick(int a, int n, int b, int m) {
    int i = a;
    for (int k = a; k < a + n; k++) {
        if (!slots[k].live) return k;
        if (slots[k].used < slots[i].used) i = k;
    }
    if (!m || slots[i].used < clock_hist[(present_no + 1) & 63]) return i;
    for (int k = b; k < b + m; k++) {
        if (!slots[k].live) return k;
        if (slots[k].used < slots[i].used) i = k;
    }
    return i;
}
/* why blocks were decoded (logged): [0] their texels were written, [1] their CLUT changed, [2..4] a block of another
   texture left its slot for them (256, 128, 64), [5..7] the same, the block that left drawn with in this very frame
   (more textures in a frame than slots of the size), [8] into a free slot */
unsigned long long pica_dec_why[9];
int pica_n128x;
static int tex_slot(const GpuTex *t, int u0, int v0, int u1, int v1, int *obx, int *oby) {
    int tw = gs->twmx | gs->twmy << 5 | gs->twox << 10 | gs->twoy << 15;
    if (memo.ok && memo.ver == ver_clock && memo.epoch == drawn_epoch && memo.tx == t->tx && memo.ty == t->ty && memo.depth == t->depth
        && memo.tw == tw && (t->depth == 2 || (memo.cx == t->cx && memo.cy == t->cy))
        && u0 >= memo.bx && v0 >= memo.by && u1 < memo.bx + memo.z && v1 < memo.by + memo.z) {
        *obx = memo.bx; *oby = memo.by; slots[memo.slot].used = ++use_clock;
        return memo.slot;
    }
    pica_cnt[5]++;
    int z = 256, bx = 0, by = 0;
    if (u0 >= 0 && v0 >= 0 && u1 < 256 && v1 < 256) {
        /* origins on a grid of half the size: any primitive up to 32 texels fits a 64 block, up to 64 a 128 one */
        for (int c = 64; c < 256; c *= 2) {
            int ox = u0 & ~(c / 2 - 1), oy = v0 & ~(c / 2 - 1);
            if (ox + c > 256) ox = 256 - c; if (oy + c > 256) oy = 256 - c;
            if (u1 < ox + c && v1 < oy + c) { z = c; bx = ox; by = oy; break; }
        }
        /* up to 128 texels but across that grid's lines: a 128 block from where the primitive's texels start (to 8
           texels), not a whole page.  (The save screen's background is a picture 97 texels wide from u 56 drawn
           with 15 CLUTs, a page each where there are 8 such slots: 15 pages decoded every frame) */
        if (z == 256 && u1 - (u0 & ~7) < 128 && v1 - (v0 & ~7) < 128) {
            z = 128; bx = u0 & ~7; by = v0 & ~7;
            if (bx + 128 > 256) bx = 128; if (by + 128 > 256) by = 128;
        }
    }
    *obx = bx; *oby = by;
    int cx = t->depth == 2 ? 0 : t->cx, cy = t->depth == 2 ? 0 : t->cy;
    if (!hinit) { memset(hhead, -1, sizeof hhead); hinit = 1; }
    unsigned hk = ((unsigned)(t->tx >> 6) * 7 + (unsigned)(t->ty >> 8) * 3 + (unsigned)cx * 13 + (unsigned)cy * 31 + (unsigned)(bx >> 5) * 5
                   + (unsigned)(by >> 5) * 17 + (unsigned)z + (unsigned)tw * 11 + (unsigned)t->depth) & 255;
    /* (a block can have more than one slot: a top-down room's wide frame uploads three windows of its map into the
       same VRAM in a frame, one after the other, so each block alternates between three pictures in a frame; each
       picture keeps its slot, found again by its words' hash, instead of one slot decoded three times a frame) */
    int cand[4], nc = 0;
    for (int k = hhead[hk]; k >= 0 && nc < 4; k = slots[k].next) {
        Slot *s = &slots[k];
        if (s->tx == t->tx && s->ty == t->ty && s->depth == t->depth && s->tw == tw && s->cx == cx && s->cy == cy
            && s->bx == bx && s->by == by && pica_slot_size(k) == z) cand[nc++] = k;
    }
    int i = nc ? cand[0] : -1;
    /* found valid, and nothing was written to vram[] or newly drawn since: nothing to read back or to compare
       (primitives that alternate between two textures miss the memo every time) */
    int x, y, w, h; tex_rect(t, tw, bx, by, z, &x, &y, &w, &h);
    const int has_clut = t->depth < 2 && t->cy != PORT_CLUT_Y;
    unsigned gsum = gens_of(x, y, w, h) + (has_clut ? gens_of(t->cx, t->cy, t->depth ? 256 : 16, 1) : 0);
    for (int c = 0; c < nc; c++)
        if (slots[cand[c]].checked == ver_clock + 1 && slots[cand[c]].synced == gsum + 1) { i = cand[c]; slots[i].used = ++use_clock; goto found; }
    pica_cnt[6]++;
    sync_pages(x, y, w, h);
    if (has_clut) sync_pages(t->cx, t->cy, t->depth ? 256 : 16, 1);
    gsum = gens_of(x, y, w, h) + (has_clut ? gens_of(t->cx, t->cy, t->depth ? 256 : 16, 1) : 0);   /* (a noted area made cells moves them) */
    unsigned v = 0;
    unsigned long long was = 0;   /* the slot's last use, by this block or the one it replaces */
    int fresh = 0;                /* 1: decoded into a slot of its own, the others kept */
    if (nc) {
        for (int c = 1; c < nc; c++) if (slots[cand[c]].used > slots[i].used) i = cand[c];   /* (the latest used) */
        Slot *s = &slots[i];
        if (s->checked == ver_clock + 1) { s->used = ++use_clock; goto found; }   /* (nothing in vram[] changed since) */
        v = tex_ver(t, tw, bx, by, z);
        if (s->ver >= v) { s->checked = ver_clock + 1; s->used = ++use_clock; goto found; }
        { int x2, y2, w2, h2; tex_rect(t, tw, bx, by, z, &x2, &y2, &w2, &h2);
          if (blocks_ver(x2, y2, w2, h2) <= s->tver) {
              if (clut_hash(t) == s->chash) {   /* only the CLUT was written, the same */
                  s->ver = v; s->checked = ver_clock + 1; s->used = ++use_clock; goto found;
              }
              /* --lazy-clut N (an Old 3DS): a palette animated every frame costs a decode and an upload of every
                 block that uses it, 4 ms a page there and three pages a frame in the first village.  The block keeps
                 its colours for N presents, and one such block a present is redone (none waits more than 3 N). */
              if (pica_lazy_clut) {
                  static unsigned at, done;
                  unsigned age = present_no - s->updated;
                  if (at != present_no) { at = present_no; done = 0; }
                  if (age < (unsigned)pica_lazy_clut || (done && age < 3u * (unsigned)pica_lazy_clut)) { s->used = ++use_clock; goto found; }
                  done++;
              }
          } }
        {   /* its words written again but the same as a slot of it has (a top-down room uploads its map window every
               frame, moved or not: four 256 x 256 blocks decoded and uploaded each time, 25 ms a drawn frame on an
               Old 3DS): that slot is kept */
            int x3, y3, w3, h3; tex_rect(t, tw, bx, by, z, &x3, &y3, &w3, &h3);
            const unsigned hs = src_hash(x3, y3, w3, h3), hc = clut_hash(t);
            for (int c = 0; c < nc; c++) {
                Slot *o = &slots[cand[c]];
                if (o->srch == hs && o->chash == hc) {
                    i = cand[c]; o->ver = v; o->tver = blocks_ver(x3, y3, w3, h3); o->checked = ver_clock + 1; o->used = ++use_clock; st_same++;
                    goto found;
                }
            }
            pica_dec_why[blocks_ver(x3, y3, w3, h3) <= s->tver ? 1 : 0]++;
        }
        if (s->used > frame_clock && nc < 3) fresh = 1;   /* (used in this very frame with another picture: alternating) */
        else was = s->used;   /* (stale: decoded again in its slot) */
    }
    if (!nc || fresh) {
        if (z == 256) i = slot_pick(0, PICA_N256, 0, 0);
        else if (z == 128) i = slot_pick(PICA_N256, PICA_N128, PICA_X128, pica_n128x);
        else i = slot_pick(PICA_N256 + PICA_N128, 128, PICA_N256 + PICA_N128 + 128, PICA_N64 - 128);
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
        { int zi = z == 256 ? 0 : z == 128 ? 1 : 2; pica_dec_why[!slots[i].live ? 8 : was > frame_clock ? 5 + zi : 2 + zi]++; }
        slots[i] = (Slot){ .tx = t->tx, .ty = t->ty, .depth = t->depth, .cx = cx, .cy = cy, .tw = tw, .bx = bx, .by = by, .live = 1, .next = hhead[hk] };
        hhead[hk] = (short)i;
    }
    if (nbv && was > batch_clock) flush();   /* the pending batch samples the slot about to change */
    if (port_verbose > 1) port_printf("[gpu] DEC %s z%d tex %d,%d d%d clut %d,%d b %d,%d tw %x (frame %d)\n", slots[i].ver ? "stale" : "new", z, t->tx, t->ty, t->depth, t->cx, t->cy, bx, by, tw, port_frame);
    unsigned long long t0 = port_time_us();
    decode(t, tw, bx, by, z);
    {   /* the texel classes (1: opaque, 2: semi-transparent) of the block's 8 x 8 parts: see prim_kinds */
        unsigned char *kc = slot_kinds[i]; const int sh = z == 64 ? 3 : z == 128 ? 4 : 5;
        memset(kc, dec_kinds == 3 ? 0 : dec_kinds, 64);
        if (dec_kinds == 3) for (int yy = 0; yy < z; yy++) {
            const u32 *q = dec + yy * z; unsigned char *row = kc + (yy >> sh) * 8;
            for (int xx = 0; xx < z; xx++) { u32 a = q[xx] >> 24; if (a) row[xx >> sh] |= a == A_SEMI ? 2 : 1; }
        }
    }
    pica_decode_us += port_time_us() - t0;
    dev->tex_upload(i, dec, was > frame_clock);
    st_decodes++;
    slots[i].ver = v; slots[i].checked = ver_clock + 1; slots[i].used = ++use_clock;
    slots[i].tver = blocks_ver(x, y, w, h); slots[i].chash = clut_hash(t); slots[i].updated = present_no; slots[i].srch = src_hash(x, y, w, h);
found:
    slots[i].synced = gsum + 1;
    memo.ok = 1; memo.tx = t->tx; memo.ty = t->ty; memo.depth = t->depth; memo.cx = t->cx; memo.cy = t->cy; memo.tw = tw;
    memo.bx = bx; memo.by = by; memo.z = z; memo.slot = i; memo.ver = ver_clock; memo.epoch = drawn_epoch;
    return i;
}

/* ---- primitives */
static PState base_state(void) {
    PState s = { -1, 0, AT_OFF, -1, gs->x1, gs->y1, gs->x2, gs->y2, gs->wide };
    return s;
}
static void put(PV *p, int x, int y, int r, int g, int b, int u, int v) {
    p->x = (float)x; p->y = (float)y; p->u = (float)u; p->v = (float)v; p->r = (u8)r; p->g = (u8)g; p->b = (u8)b; p->a = 0; p->sh = 0;   /* (a: what an untextured pixel's alpha is, bit 15 clear) */
}
/* emit n vertices (a triangle or two) with the state the primitive needs: textured semi-transparent ones twice */
/* Clipping, here: a PICA200 does not clip in x / y, and a console draws nothing of a triangle with a vertex on or
   past the target's right edge (x = 1024; gputest.bmp: the full-width fills missing, an 8 x 8 mark at x 952 right)
   where Azahar draws it.  The second frame buffer ends at that edge, and sprites cross it.  So triangles are cut to
   the target (the far bounds short of the edge, past the last pixel's centre); the drawing area inside it is the
   device's scissor (cutting to the area itself sent a fifth of a scene's triangles through the clipper).
   On by default on the 3DS; PICA_CLIP=1 in the environment tries it on the reference device. */
static int clip_on = -1;
/* profiling on the 3DS (n3ds/pica_c3d.c logs it): one ordering table in sixteen is timed, in system ticks:
   [0] its packets, [1] emit(), [2] the clipping, [3] the texture look-up, [4] the device's triangles */
int pica_prof_on; unsigned long long pica_prof[6]; unsigned pica_prof_n;
/* pica_dry (shims/gpu.c DrawOTag, a profiled table): the table's polygons are run through twice before they are
   drawn, with whole-pass timers and none per primitive (a timer read is a system call on the 3DS, several times a
   primitive's own cost): 1 the decoder alone (the backend's entry points return at once), 2 the backend as well, its
   batches dropped where the device would get them */
int pica_dry;
unsigned long long pica_ot[4];   /* (shims/gpu.c DrawOTag: ticks in drawn / skipped tables, and how many of each) */
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
            o->b = (u8)(a->b + (b->b - a->b) * t + 0.5f); o->a = a->a; o->sh = a->sh + (b->sh - a->sh) * t;
            if (axis) o->y = bound; else o->x = bound;
        }
    }
    return m;
}
/* the triangles of v[0..n) inside the target into out (15 vertices a triangle at most); the count.  The drawing area
   is the device's scissor; what the CPU must cut is what a PICA200 cannot be given: a vertex outside the target */
#define CLIP_X2 1023.4f
#define CLIP_Y2 511.4f
/* pica_gpu_clips: the sides of the target past which the device's GPU clips a triangle itself (bit 0 left, 1 top,
   2 right, 3 bottom), as the 3DS device's start-up probe found by drawing across each and reading back.  Nothing is
   cut on the CPU there, and such a primitive takes the short path like one inside the target: the clipper and the
   general path cost several times the short one on an Old 3DS, and a scrolling scene always has a column and a row of
   primitives across the left and upper edges.  kx1..ky2: the bounds a vertex must be inside for the short path. */
int pica_gpu_clips;
unsigned long long pica_cnt[8];   /* (logged) [0] quads and [1] triangles by the short path, [2] triangles by the general one, [3] rectangles, [4] line pixels, [5] texture look-ups past the memo */
static int kx1 = 0, ky1 = 0, kx2 = 1023, ky2 = 511;
static void clip_sides(void) {
    kx1 = pica_gpu_clips & 1 ? -30000 : 0; ky1 = pica_gpu_clips & 2 ? -30000 : 0;
    kx2 = pica_gpu_clips & 4 ? 30000 : 1023; ky2 = pica_gpu_clips & 8 ? 30000 : 511;
}
static int clip_tris(const PV *v, int n, PV *out) {
    const float X1 = pica_gpu_clips & 1 ? -1e9f : 0.0f, Y1 = pica_gpu_clips & 2 ? -1e9f : 0.0f;
    const float X2 = pica_gpu_clips & 4 ? 1e9f : CLIP_X2, Y2 = pica_gpu_clips & 8 ? 1e9f : CLIP_Y2;
    int m = 0;
    for (int t = 0; t + 2 < n; t += 3) {
        const PV *q = &v[t];
        int l = 0, r = 0, u = 0, d = 0;   /* vertices past each bound */
        for (int i = 0; i < 3; i++) { l += q[i].x < X1; r += q[i].x > X2; u += q[i].y < Y1; d += q[i].y > Y2; }
        if (!(l | r | u | d)) { out[m++] = q[0]; out[m++] = q[1]; out[m++] = q[2]; continue; }
        if (l == 3 || r == 3 || u == 3 || d == 3) continue;
        PV a[8], b[8]; const PV *in = q; PV *o = a; int k = 3;
        if (l) { k = clip_poly(in, k, o, 0, X1, 0); in = o; o = o == a ? b : a; }
        if (r) { k = clip_poly(in, k, o, 0, X2, 1); in = o; o = o == a ? b : a; }
        if (u) { k = clip_poly(in, k, o, 1, Y1, 0); in = o; o = o == a ? b : a; }
        if (d) { k = clip_poly(in, k, o, 1, Y2, 1); in = o; o = o == a ? b : a; }
        for (int i = 1; i + 1 < k; i++) { out[m++] = in[0]; out[m++] = in[i]; out[m++] = in[i + 1]; }
    }
    return m;
}
static void emit(const PV *v0, int n, const GpuTex *tex, int semi, int raw) {
    PState s = base_state();
    PV cl[30], vv[30]; const PV *v; int skinds = 3;
    if (clip_on < 0) {
#ifdef __3DS__
        clip_on = 1;
#else
        clip_on = getenv("PICA_CLIP") != 0;
#endif
    }
    const PV *orig = v0; int norig = n;
    if (clip_on) { n = clip_tris(v0, n, cl); if (!n) return; v0 = cl; }
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
        int slot = tex_slot(tex, u0, w0, u1, w1, &bx, &by), ax, ay;
        if (semi) skinds = prim_kinds(slot, u0, w0, u1, w1, bx, by);
        pica_slot_pos(slot, &ax, &ay);
        s.tex = pica_slot_atlas(slot); s.mod = 1;
        for (int i = 0; i < n; i++) { vv[i] = v0[i]; vv[i].u += (float)(ax - bx); vv[i].v += (float)(ay - by); if (raw) vv[i].r = vv[i].g = vv[i].b = 128; }
        v = vv;
    }
    if (!tex) { s.blend = semi ? gs->abr : -1; memcpy(batch(&s, n), v, n * sizeof *v); return; }
    if (!semi || !(skinds & 2)) { s.atest = AT_NOTZERO; memcpy(batch(&s, n), v, n * sizeof *v); return; }
    if (skinds & 1) { s.atest = AT_OPAQUE; memcpy(batch(&s, n), v, n * sizeof *v); }
    s.atest = AT_SEMI; s.blend = gs->abr; memcpy(batch(&s, n), v, n * sizeof *v);
}
/* the common primitive: wholly inside the drawing area, so nothing to clip, and its vertices go to the batch once
   (the same triangles as emit() makes of it).  v is changed.  u0..w1: the texels it reads (its vertices' bounds) */
static void emit_in(PV *v, int n, const GpuTex *tex, int semi, int raw, int u0, int w0, int u1, int w1) {
    PState s = base_state();
    if (!tex) { s.blend = semi ? gs->abr : -1; memcpy(batch(&s, n), v, n * sizeof *v); return; }
    int bx, by, ax, ay;
    if (u1 > 255 && u0 < 256) u1 = 255; if (w1 > 255 && w0 < 256) w1 = 255;
    int slot = tex_slot(tex, u0, w0, u1, w1, &bx, &by);
    pica_slot_pos(slot, &ax, &ay);
    s.tex = pica_slot_atlas(slot); s.mod = 1;
    if (raw) for (int i = 0; i < n; i++) v[i].r = v[i].g = v[i].b = 128;   /* (see pica_quad) */
    float du = (float)(ax - bx), dv = (float)(ay - by);
    for (int i = 0; i < n; i++) { v[i].u += du; v[i].v += dv; }
    const int kinds = semi ? prim_kinds(slot, u0, w0, u1, w1, bx, by) : 1;
    if (!semi || !(kinds & 2)) { s.atest = AT_NOTZERO; memcpy(batch(&s, n), v, n * sizeof *v); return; }
    if (!(kinds & 1)) { s.atest = AT_SEMI; s.blend = gs->abr; memcpy(batch(&s, n), v, n * sizeof *v); return; }
    if (n == 6 && v[3].x == v[1].x && v[3].y == v[1].y && v[4].x == v[2].x && v[4].y == v[2].y) {
        /* a quad's two triangles in the PS1's order, each whole before the next (a folded quad's overlap) */
        for (int i = 0; i < 6; i += 3) {
            s.atest = AT_OPAQUE; s.blend = -1; memcpy(batch(&s, 3), v + i, 3 * sizeof *v);
            s.atest = AT_SEMI; s.blend = gs->abr; memcpy(batch(&s, 3), v + i, 3 * sizeof *v);
        }
        return;
    }
    s.atest = AT_OPAQUE; memcpy(batch(&s, n), v, n * sizeof *v);
    s.atest = AT_SEMI; s.blend = gs->abr; memcpy(batch(&s, n), v, n * sizeof *v);
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
    int area = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);   /* (coordinates are 12 bits: every product here fits 32, and a 64-bit division is a long library call on the 3DS) */
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
        int lo = x1 >> 4, hi = x2 >> 4;
        for (int k = 0; k < 3 && lo <= hi; k++) {   /* inside: (q1 - q0) x (p - q0) >= 0 */
            int k1 = (k + 1) % 3; int ex = x[k1] - x[k], ey = y[k1] - y[k], n = ex * (cy - y[k]);
            if (!ey) { if (n < 0) lo = hi + 1; continue; }
            /* ey > 0: px - x0 <= n / ey (floor); ey < 0: px - x0 >= n / ey (ceil) */
            if (ey > 0) { int q = n >= 0 ? n / ey : -((-n + ey - 1) / ey); int c = (x[k] + q) >> 4; if (c < hi) hi = c; }
            else { int d = -ey, m = -n; int q = m >= 0 ? (m + d - 1) / d : -((-m) / d); int c = (x[k] + q) >> 4; if (c > lo) lo = c; }
        }
        if (lo > hi) continue;
        drawn_epoch++;
        memset(&cell_drawn[cy][lo], 1, (size_t)(hi - lo + 1));
        for (int pi = lo >> 2; pi <= hi >> 2; pi++) { page_drawn[cy >> 8][pi] = 1; page_gen[cy >> 8][pi]++; }
    }
}
static void pica_tri(const GpuVtx *a, const GpuVtx *b, const GpuVtx *c, int shade, const GpuTex *tex, int semi, int raw) {
    if (pica_dry == 1) return;
    PV v[3]; const GpuVtx *p[3] = { a, b, c };
    for (int i = 0; i < 3; i++) {
        const GpuVtx *q = shade ? p[i] : a;
        put(&v[i], p[i]->x, p[i]->y, q->r, q->g, q->b, p[i]->u, p[i]->v);
        if (pica_stereo_frame) v[i].sh = (float)p[i]->sh;
    }
    {   /* (inside the target: clip_tris would pass it whole) */
        if (a->x >= kx1 && b->x >= kx1 && c->x >= kx1 && a->x <= kx2 && b->x <= kx2 && c->x <= kx2
            && a->y >= ky1 && b->y >= ky1 && c->y >= ky1 && a->y <= ky2 && b->y <= ky2 && c->y <= ky2 && bulk_in()) {
            pica_cnt[1]++;
            int u0 = a->u, u1 = u0, w0 = a->v, w1 = w0;
            if (b->u < u0) u0 = b->u; if (b->u > u1) u1 = b->u; if (c->u < u0) u0 = c->u; if (c->u > u1) u1 = c->u;
            if (b->v < w0) w0 = b->v; if (b->v > w1) w1 = b->v; if (c->v < w0) w0 = c->v; if (c->v > w1) w1 = c->v;
            emit_in(v, 3, tex, semi, raw, u0, w0, u1, w1);
            return;
        }
    }
    pica_cnt[2]++;
    tri_drawn(v);
    emit(v, 3, tex, semi, raw);
}
/* a quad inside the target, its drawing area noted: one texture look-up, and its two triangles written once, where
   the batch has them (the PS1's order: (0, 1, 2), (1, 2, 3); a flat triangle has its first vertex's colour, as
   pica_tri; du, dv: the block's place in its atlas) */
static inline void quad_put(PV *d, const GpuVtx *g, int shade, int du, int dv) {
    float x[4], y[4], u[4], v[4]; u32 c[4];
    for (int i = 0; i < 4; i++) {
        x[i] = (float)g[i].x; y[i] = (float)g[i].y; u[i] = (float)(g[i].u + du); v[i] = (float)(g[i].v + dv);
        c[i] = (u32)g[i].r | ((u32)g[i].g << 8) | ((u32)g[i].b << 16);   /* (alpha 0: an untextured pixel's, bit 15 clear) */
    }
    static const u8 order[6] = { 0, 1, 2, 1, 2, 3 };
    for (int k = 0; k < 6; k++) {
        int i = order[k]; u32 col = shade ? c[i] : c[k < 3 ? 0 : 1];
        d[k].x = x[i]; d[k].y = y[i]; d[k].u = u[i]; d[k].v = v[i]; d[k].sh = pica_stereo_frame ? (float)g[i].sh : 0.0f;
        memcpy(&d[k].r, &col, 4);
    }
}
static int pica_quad(const GpuVtx *g, int shade, const GpuTex *tex, int semi, int raw) {
    if (pica_dry == 1) return 1;
    for (int i = 0; i < 4; i++) if (g[i].x < kx1 || g[i].x > kx2 || g[i].y < ky1 || g[i].y > ky2) return 0;
    if (!bulk_in()) return 0;
    pica_cnt[0]++;
    PState s = base_state();
    if (!tex) { s.blend = semi ? gs->abr : -1; quad_put(batch(&s, 6), g, shade, 0, 0); return 1; }
    int u0 = g[0].u, u1 = u0, w0 = g[0].v, w1 = w0, bx, by, ax, ay;
    for (int i = 1; i < 4; i++) {
        if (g[i].u < u0) u0 = g[i].u; if (g[i].u > u1) u1 = g[i].u;
        if (g[i].v < w0) w0 = g[i].v; if (g[i].v > w1) w1 = g[i].v;
    }
    int slot = tex_slot(tex, u0, w0, u1, w1, &bx, &by);
    pica_slot_pos(slot, &ax, &ay);
    /* a raw texture is the modulated one with the colour that leaves it as it is (texel x 128 / 128): one state for
       both, where raw and shaded primitives one after the other ended a batch each time */
    s.tex = pica_slot_atlas(slot); s.mod = 1;
    GpuVtx g128[4];
    if (raw) { for (int i = 0; i < 4; i++) { g128[i] = g[i]; g128[i].r = g128[i].g = g128[i].b = 128; } g = g128; }
    const int kinds = semi ? prim_kinds(slot, u0, w0, u1, w1, bx, by) : 1;
    if (!semi || !(kinds & 2)) { s.atest = AT_NOTZERO; quad_put(batch(&s, 6), g, shade, ax - bx, ay - by); return 1; }
    if (!(kinds & 1)) { s.atest = AT_SEMI; s.blend = gs->abr; quad_put(batch(&s, 6), g, shade, ax - bx, ay - by); return 1; }
    PV v[6]; quad_put(v, g, shade, ax - bx, ay - by);
    for (int i = 0; i < 6; i += 3) {   /* each triangle whole before the next (a folded quad's overlap) */
        s.atest = AT_OPAQUE; s.blend = -1; memcpy(batch(&s, 3), v + i, 3 * sizeof *v);
        s.atest = AT_SEMI; s.blend = gs->abr; memcpy(batch(&s, 3), v + i, 3 * sizeof *v);
    }
    return 1;
}
static void pica_rect(int x, int y, int w, int h, u32 col, const GpuTex *tex, int u0, int v0, int semi, int raw) {
    if (pica_dry == 1) return;
    int r = col & 255, g = (col >> 8) & 255, b = (col >> 16) & 255;
    PV v[6];
    put(&v[0], x, y, r, g, b, u0, v0); put(&v[1], x + w, y, r, g, b, u0 + w, v0); put(&v[2], x, y + h, r, g, b, u0, v0 + h);
    put(&v[4], x + w, y + h, r, g, b, u0 + w, v0 + h);
    if (pica_stereo_frame) { extern int port_stereo_sh; v[0].sh = v[1].sh = v[2].sh = v[4].sh = (float)port_stereo_sh; }
    v[3] = v[1]; v[5] = v[2];
    pica_cnt[w == 1 && h == 1 && !tex ? 4 : 3]++;
    {   if (w >= 0 && h >= 0 && x >= kx1 && y >= ky1 && x + w <= kx2 && y + h <= ky2 && bulk_in()) {
            emit_in(v, 6, tex, semi, raw, u0, v0, u0 + w, v0 + h);
            return;
        }
    }
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
/* --stereo on a PC (gpu.c's frame dumps): the right eye's display area as RGB8; 0 = none */
int pica_eye_frame(unsigned char *rgb, int x, int y, int w, int h) {
    extern int port_stereo_flat;
    if (!ert || w <= 0 || h <= 0 || w > 1024 || h > 512 || port_stereo_flat) return 0;
    flush();
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) {
        u32 c = ert[(y + j) & 511][(x + i) & 1023]; unsigned char *o = rgb + (j * w + i) * 3;
        o[0] = c & 255; o[1] = (c >> 8) & 255; o[2] = (c >> 16) & 255;
    }
    return 1;
}
int pica_eye_ok;   /* --stereo: the right eye's target is there (gpu.c draws a second pass into it) */
static void pica_fill(int x, int y, int w, int h, u16 c) {
    u16 *vram = port_vram();
    flush();
    if (gs->wide == 2) { dev->eye_fill(x, y, w, h, c15to32(c)); return; }   /* (the right eye's pass: its own target alone) */
    if (pica_eye_ok && w >= 320 && h >= 200) dev->eye_fill(x, y, w, h, c15to32(c));   /* (a frame's clear: the right eye's too) */
    if (npend) {   /* (a host write the fill covers whole never needs to go; one it cuts goes first) */
        for (int i = 0; i < npend; i++)
            if (pend[i].x >= x && pend[i].y >= y && pend[i].x + pend[i].w <= x + w && pend[i].y + pend[i].h <= y + h) { memmove(&pend[i], &pend[i + 1], (size_t)(npend - i - 1) * sizeof pend[0]); npend--; i--; }
        pend_flush(x, y, w, h);
    }
    if (nwd) {   /* (the same of the wide plane's copies) */
        for (int i = 0; i < nwd; i++)
            if (wd[i].x >= x && wd[i].y >= y && wd[i].x + wd[i].w <= x + w && wd[i].y + wd[i].h <= y + h) { wd[i] = wd[--nwd]; i--; }
        wd_resolve(x, y, w, h);
    }
    /* --widescreen: the frame's clear of a buffer whose picture is the wide plane's (the plane gets its own, gpu.c
       ws_fill): it waits as the copy does, and the copy after the frame replaces it.  (Sent, it was a change of
       target and back in every ordering table, drawn or skipped.) */
    for (int i = 0; i < nwdbuf; i++)
        if (wdbuf[i].x == x && wdbuf[i].y == y && wdbuf[i].w == w && wdbuf[i].h == h) {
            for (int k = 0; k < nbulk; k++)
                if (x >= bulk[k].x1 && y >= bulk[k].y1 && x + w - 1 <= bulk[k].x2 && y + h - 1 <= bulk[k].y2 && nwd < 4) {
                    wd[nwd].x = x; wd[nwd].y = y; wd[nwd].w = w; wd[nwd].h = h; wd[nwd].fill = 1; wd[nwd].col = c15to32(c); nwd++;
                    return;
                }
        }
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
/* the end of a drawn ordering table (gpu.c DrawOTag): the frame's drawing is handed to the GPU now, so it runs while
   the program waits for the vblank and runs the game, not from the present on (a 3DS waited 2.5 to 7 ms a frame for
   the GPU at the next table's start) */
void pica_submit(void) { flush(); if (dev->submit && !pica_dry) dev->submit(); }
/* ---- --widescreen (gpu.c "widescreen"): the wide plane is the device's second target; a displayed buffer's wide copy
   is in its left or right half (gpu.c ws_base) at the buffer's own rows */
static void pica_ws_fill(int x, int y, int w, int h, u16 c) { flush(); dev->wide_fill(x, y, w, h, c15to32(c)); }
static void pica_ws_upload(int wx, int wy, int w, int h, const u16 *src, int stride) { flush(); dev->wide_write(wx, wy, w, h, src, stride); }
static void pica_ws_copy_down(int wx, int wy, int vx, int vy, int w, int h) {
    flush();
    if (npend) pend_flush(vx, vy, w, h);   /* (a host write waiting for this rectangle goes first, as before a primitive) */
    {   /* it waits (wd_resolve): the newest copy of a rectangle is the one that counts */
        int i;
        for (i = 0; i < nwd; i++) if (wd[i].x == vx && wd[i].y == vy && wd[i].w == w && wd[i].h == h) break;
        if (i == nwd) { if (nwd == 4) wd_resolve(wd[0].x, wd[0].y, wd[0].w, wd[0].h); i = nwd++; }
        wd[i].wx = wx; wd[i].wy = wy; wd[i].x = vx; wd[i].y = vy; wd[i].w = w; wd[i].h = h; wd[i].fill = 0;
        for (i = 0; i < nwdbuf; i++) if (wdbuf[i].x == vx && wdbuf[i].y == vy && wdbuf[i].w == w && wdbuf[i].h == h) break;
        if (i == nwdbuf && nwdbuf < 4) { wdbuf[i].x = vx; wdbuf[i].y = vy; wdbuf[i].w = w; wdbuf[i].h = h; nwdbuf++; }
    }
    /* the buffer in VRAM was drawn: vram[] is stale there, as after its primitives */
    if (!bulk_note(vx, vy, vx + w - 1, vy + h - 1)) pages_drawn(vx, vy, vx + w - 1, vy + h - 1);
    bulk_memo.ok = 0;
}
static void pica_ws_read(int wx, int wy, int w, int h, u16 *dst, int stride) {
    flush();
    if (!dev->wide_read) return;
    dev->wide_read(wx, wy, w, h, stage, w);
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) dst[j * stride + i] = c32to15(stage[j * w + i]);
}
static int pica_ws_present(int wx, int wy, int w, int h) {
    flush();
    frame_clock = use_clock; present_no++;
    clock_hist[present_no & 63] = use_clock;
    return dev->wide_present ? dev->wide_present(wx, wy, w, h) : 0;
}
static int pica_start(void) {
    if (!stage) stage = malloc(1024 * 512 * 4);
    if (!dec) dec = malloc(256 * 256 * 4);
    if (!stage || !dec || dev->start()) return -1;
    {   extern int port_stereo_on, port_widescreen, port_ws_cull;
        if (port_stereo_on) {
            if (port_widescreen) { port_printf("[gpu] pica: --stereo: --widescreen off (one or the other)\n"); port_widescreen = port_ws_cull = 0; }
            if (dev->eye_start && dev->eye_start() == 0) pica_eye_ok = 1;
            else { port_printf("[gpu] pica: no right eye's target on this device: --stereo off\n"); port_stereo_on = 0; }
        }
    }
    {   extern int port_widescreen, port_ws_cull;
        if (port_widescreen > 96) port_widescreen = port_ws_cull = 96;
        if (port_widescreen && (!dev->wide_start || dev->wide_start())) {
            port_printf("[gpu] pica: no wide plane on this device: --widescreen off\n");
            port_widescreen = port_ws_cull = 0;
        }
    }
    clip_sides();
    pend_now = 1; pica_vram_written(0, 0, 1024, 512); pend_now = getenv("PICA_WRITE_NOW") != 0;   /* (PICA_WRITE_NOW: no waiting, to compare) */
    return 0;
}
static void pica_stop(void) {
    port_printf("[gpu] pica: decodes: %llu texels written, %llu CLUT changed, %llu/%llu/%llu took a slot (256/128/64), %llu/%llu/%llu took one used in the frame, %llu free\n",
                pica_dec_why[0], pica_dec_why[1], pica_dec_why[2], pica_dec_why[3], pica_dec_why[4], pica_dec_why[5], pica_dec_why[6], pica_dec_why[7], pica_dec_why[8]);
    if (ref_sw[0] | ref_sw[3]) port_printf("[gpu] pica: changes of target: %llu by triangles, %llu fills, %llu writes, %llu wide fills, %llu wide writes, %llu copies down\n",
                                           ref_sw[0], ref_sw[1], ref_sw[2], ref_sw[3], ref_sw[4], ref_sw[5]);
    port_printf("[gpu] pica: %llu blocks written again the same, not decoded\n", st_same);
    port_printf("[gpu] pica: %llu batches, %llu triangles, %llu texture decodes, %llu read-backs (%llu for textures, %llu for the host: %llu pixels)\n",
                st_batches, st_tris, st_decodes, st_readbacks, st_rb_tex, st_rb_host, st_rb_px);
}
static int pica_present(int dx, int dy, int dw, int dh, int masked) {
    flush();

    if (npend) pend_flush(dx, dy, dw, dh);
    if (nwd) wd_resolve(dx, dy, dw, dh);
    frame_clock = use_clock; present_no++;
    clock_hist[present_no & 63] = use_clock;
    return dev->present ? dev->present(dx, dy, dw, dh, masked) : 0;
}
const GpuBackend port_gpu_pica = { "pica (3DS model, CPU reference)", pica_start, pica_stop, pica_tri, pica_rect, pica_line, pica_fill,
                                   pica_copy, pica_vram_written, pica_vram_read, pica_list_end, pica_present, .quad = pica_quad,
                                   .ws_fill = pica_ws_fill, .ws_copy_down = pica_ws_copy_down, .ws_upload = pica_ws_upload,
                                   .ws_read = pica_ws_read, .ws_present = pica_ws_present };
