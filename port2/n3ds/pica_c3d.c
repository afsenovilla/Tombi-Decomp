/* The 3DS device of the "pica" GPU backend (shims/gpu_pica.h, shims/gpu_pica.c; docs/PORT.md "Nintendo 3DS"): the
 * PS1 VRAM as a 1024x512 RGBA8 texture the PICA200 renders into, through citro3d.
 *
 * Conventions (one orientation everywhere, so nothing is flipped): VRAM pixel (x, y) is texel column x, memory row y of
 * the render target (the PICA's row 0 is the first in memory and t = 0); the projection maps VRAM y = 0 to clip
 * y = +1, which the rasteriser writes to row 0.  The cache slots are swizzled on the CPU with texel (u, v) in row v, so
 * texture coordinates are u / 256, v / 256 and the VRAM's x / 1024, y / 512.  A read-back is a display transfer of the
 * render target (tiled -> linear, no flip): linear row y = VRAM row y.  At start-up a self test draws two marks and
 * reads them back; the log says whether the orientation holds.
 *
 * Commands are recorded and run when the frame is split or ended, so the CPU must not change memory the GPU is still
 * to read: before a cache slot sampled in the open frame is re-filled, and before a read-back, the frame is ended and
 * waited for.  Host writes go through a per-frame staging arena (a fresh texture each, drawn into the target with a
 * quad), reset when the frame has been waited for. */
#ifdef __3DS__
#include <3ds.h>
#include <citro3d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "port_runtime.h"
#include "gpu_pica.h"
#include "pica_vsh_shbin.h"

static C3D_Tex vram_tex, atlas_tex[3];   /* the texture cache: 1024 x 1024 each, the third if there is memory (shims/gpu_pica.h) */
static C3D_RenderTarget *vram_rt, *top_rt, *bot_rt;
static C3D_Tex panel_tex;                 /* the bottom screen's panel (runtime/panel.c), 512 x 256, its canvas in the corner */
static int panel_w, panel_h;
static shaderProgram_s prog;
static DVLB_s *vsh;
static int u_proj, u_tsc, u_eye, in_frame, scr_fmt;
static int atlas2_h = PICA_ATLAS;           /* the third atlas's height (half when memory is short) */
static int c3d_up;                        /* citro3d is running (port_n3ds_gpu_fini) */
static void c3d_progress(int done, int total);   /* (below: the translation's progress bar) */
static int vb_n, last_ok;                 /* (the vertex buffer's fill and the state cache, below) */
static unsigned frame_no;
static unsigned *lin;                     /* 1024 x 512 linear words: read-backs */
#define ARENA_BYTES (2u << 20)
static unsigned char *arena; static unsigned arena_off;
static unsigned long long st_syncs, st_writes, st_frames;
/* where a frame's time goes (system ticks; the log line every 600 frames gives milliseconds per frame) */
static unsigned long long tk_last, tk_frame, tk_upload, tk_write, tk_read, tk_panel, tk_present, tk_draw;
static unsigned long long tk_begin, tk_eye; static unsigned st_switch;   /* the wait at a frame's start (the GPU still on the frame before), the changes of target */
static double gpu_ms_sum, gpu_ms_max; static unsigned gpu_ms_n;   /* the GPU's own time for each submitted frame (C3D_GetDrawingTime) */
#define TK_N 120   /* frames per log line */
#define TK_MS(t) ((double)(t) * 1000.0 / SYSCLOCK_ARM11 / TK_N)
static unsigned long long n_up, n_rd, n_wait;
static short rd_log[4][4];   /* the first read-backs since the last log line */
extern unsigned long long pica_decode_us;

/* Written memory out of the CPU's cache before the GPU reads it: a request to the gsp service, a round trip between
   processes each time, so as few as can be (a texture upload made one per tile row, 2.5 ms an upload on an Old 3DS: one
   for the block's whole span now).  The kernel's own call (svcFlushProcessDataCache) was tried instead: a console drew
   the panel's texture in torn bands and then stopped in a kernel exception. */
static void dcache_flush(const void *p, u32 size) { GSPGPU_FlushDataCache(p, size); }

/* ---- the frame */
/* --widescreen: the wide plane, a second target of the first one's size (shims/gpu_pica.h PicaDev.wide_start) */
static C3D_Tex wide_tex; static C3D_RenderTarget *wide_rt;
/* --stereo: the right eye's target (the same size: the frame at VRAM's coordinates) and the top screen's right image */
static C3D_Tex eye_tex; static C3D_RenderTarget *eye_rt, *top_r;
static int cur_wide;                      /* the target a draw goes to now: 0 VRAM's, 1 the wide plane */
static int lin_fresh;                     /* lin[] holds the target as it is (c3d_read): nothing drawn since */
static void begin(void) {   /* (C3D_FrameBegin waits for the frame submitted before: its memory is free then) */
    if (in_frame) return;
    u64 tb = svcGetSystemTick();
    int began = C3D_FrameBegin(0);
    tk_begin += svcGetSystemTick() - tb;
    if (!began) { static int said; if (!said++) port_printf("[3ds] pica: C3D_FrameBegin refused a frame\n"); }
    C3D_FrameDrawOn(vram_rt);
    cur_wide = 0;
    in_frame = 1;
    lin_fresh = 0;   /* (something may be drawn: the copy of the target c3d_read holds is old) */
}
static void on_target(int wide) {   /* (in a frame) */
    if (cur_wide == wide) return;
    C3D_FrameDrawOn(wide == 2 ? eye_rt : wide ? wide_rt : vram_rt);
    cur_wide = wide; st_switch++;
}
static void vb_flush(void);
static void replay(void);
static void end_and_wait(void) {   /* run what was recorded and wait for it: memory may be reused afterwards */
    if (in_frame && !replaying) replay();
    if (in_frame) { vb_flush(); C3D_FrameEnd(GX_CMDLIST_FLUSH); in_frame = 0; }
    C3D_FrameBegin(0); C3D_FrameEnd(GX_CMDLIST_FLUSH);   /* (C3D_FrameBegin waits for the queue of the frame before) */
    arena_off = 0; frame_no++; st_syncs++; vb_n = 0; last_ok = 0;
}

/* The GPU's command buffer is one frame's, and libctru stops the program (svcBreak) when a command does not fit: a
   console ended that way in play.  Before anything is recorded, a frame that has filled most of it is run and waited
   for, as when the vertex buffer or the staging arena fills. */
static unsigned st_cmd_splits;
static void cmd_room(void) {
    u32 *buf, size, off;
    GPUCMD_GetBuffer(&buf, &size, &off);
    if (in_frame && off + 4096 > size) { end_and_wait(); begin(); st_cmd_splits++; port_printf("[3ds] pica: the command buffer was nearly full (%u of %u words): the frame was split\n", (unsigned)off, (unsigned)size); }
}

/* ---- state */
static void proj_vram(void) {
    /* (the PICA writes clip y = +1 to memory row 0: the start-up self test reported FLIPPED with 0 .. 512 in Azahar) */
    C3D_Mtx m; Mtx_Ortho(&m, 0.0f, 1024.0f, 512.0f, 0.0f, 1.0f, -1.0f, true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &m);
}
static int last_ok;
static void set_env(int textured, int mod) {
    last_ok = 0;
    C3D_TexEnv *e = C3D_GetTexEnv(0);
    C3D_TexEnvInit(e);
    if (!textured) {
        C3D_TexEnvSrc(e, C3D_Both, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(e, C3D_Both, GPU_REPLACE);
    } else if (!mod) {
        C3D_TexEnvSrc(e, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(e, C3D_Both, GPU_REPLACE);
    } else {   /* the PS1's texel x colour / 128: modulate, scaled x 2; the alpha (the texel's class) unchanged */
        C3D_TexEnvSrc(e, C3D_RGB, GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(e, C3D_RGB, GPU_MODULATE);
        C3D_TexEnvScale(e, C3D_RGB, GPU_TEVSCALE_2);
        C3D_TexEnvSrc(e, C3D_Alpha, GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(e, C3D_Alpha, GPU_REPLACE);
    }
}
static void set_blend(int mode) {
    last_ok = 0;
    switch (mode) {
    case 0:  C3D_BlendingColor(0x80000000u);  /* 0.5 B + 0.5 F */
             C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_CONSTANT_ALPHA, GPU_CONSTANT_ALPHA, GPU_ONE, GPU_ZERO); break;
    case 1:  C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_ONE, GPU_ONE, GPU_ONE, GPU_ZERO); break;
    case 2:  C3D_AlphaBlend(GPU_BLEND_REVERSE_SUBTRACT, GPU_BLEND_ADD, GPU_ONE, GPU_ONE, GPU_ONE, GPU_ZERO); break;
    case 3:  C3D_BlendingColor(0x40000000u);  /* B + 0.25 F */
             C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_CONSTANT_ALPHA, GPU_ONE, GPU_ONE, GPU_ZERO); break;
    default: C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_ONE, GPU_ZERO, GPU_ONE, GPU_ZERO); break;
    }
}
static void set_atest(int at) {
    last_ok = 0;
    if (at == AT_NOTZERO) C3D_AlphaTest(true, GPU_NOTEQUAL, 0);
    else if (at == AT_OPAQUE) C3D_AlphaTest(true, GPU_EQUAL, PICA_A_OPAQUE);
    else if (at == AT_SEMI) C3D_AlphaTest(true, GPU_EQUAL, PICA_A_SEMI);
    else C3D_AlphaTest(false, GPU_ALWAYS, 0);
}
/* Every triangle goes through the vertex buffer (linear memory the GPU reads when the frame runs: a frame's vertices
   in a row, from the start again once the frame has been waited for).  Immediate-mode draws between buffer draws
   reached a console's GPU with their attributes mixed up (a fill of 0 read back as 00800080), which Azahar does not
   show: the host writes, fills and the screens' quads are six vertices in the buffer like the rest. */
/* A vertex is the backend's PV, 20 bytes, and the GPU reads it as it is (pica.v.pica does the arithmetic the CPU did
   for each one: 36 bytes of floats written per vertex, a third of a drawn frame's time on an Old 3DS).  The backend
   builds its batches in this buffer (c3d_vbuf), so a batch is drawn where it lies. */
#define VB_MAX 32768
#define POS_OFF (0.5f - 1.0f / 64.0f)   /* (what the shader adds to a position) */
#define TEX_EPS (1.0f / 32.0f)           /* (and to a texture coordinate, in texels: pica.v.pica) */
static PV *vb;
static int quad0;
static float tsc_x, tsc_y;
static void set_tscale(float x, float y) {   /* texels -> texture coordinates (pica.v.pica) */
    if (x == tsc_x && y == tsc_y) return;
    tsc_x = x; tsc_y = y;
    C3D_FVUnifSet(GPU_VERTEX_SHADER, u_tsc, x, -y, 1.0f / 255.0f, 1.0f / 255.0f);
}
/* a vertex of the device's own quads: the position as it is to be, s, t the texture coordinate, the colour 0..1
   (call set_tscale(1, 1) for them) */
static void vtx(float x, float y, float s, float t, float r, float g, float b, float a) {
    PV *d = vb + vb_n++;
    d->x = x - POS_OFF; d->y = y - POS_OFF; d->u = s - TEX_EPS; d->v = (1.0f - t) - TEX_EPS;   /* (the shader adds TEX_EPS to both) */
    d->r = (u8)(r * 255.0f + 0.5f); d->g = (u8)(g * 255.0f + 0.5f); d->b = (u8)(b * 255.0f + 0.5f); d->a = (u8)(a * 255.0f + 0.5f);
    d->sh = 0;
}
#define EDGE_X 1023.9f   /* (past the last pixel's centre, short of the target's edge) */
#define EDGE_Y 511.9f
#define QUAD_BEGIN() (set_tscale(1.0f, 1.0f), quad0 = vb_n)
#define QUAD_END() C3D_DrawArrays(GPU_TRIANGLES, quad0, vb_n - quad0)

/* ---- textures: RGBA8 words (red low) -> the PICA's texel word (red high), 8x8 tiles in Morton order */
static inline unsigned pix(unsigned c) { return (c << 24) | ((c << 8) & 0xFF0000u) | ((c >> 8) & 0xFF00u) | (c >> 24); }
static inline unsigned morton(int x, int y) {
    return (x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3);
}
static inline unsigned tiled(int x, int y, int w) { return ((y >> 3) * (w >> 3) + (x >> 3)) * 64 + morton(x & 7, y & 7); }
static void c3d_tex_upload(int slot, const unsigned *rgba, int busy) {
    u64 t0 = svcGetSystemTick();
    begin();                                                  /* the frames before have finished */
    if (busy) { end_and_wait(); begin(); n_wait++; }          /* the open frame samples this slot */
    n_up++;
    /* 8x8 tiles, Morton order inside: one tile row of source rows at a time */
    static unsigned char my[8]; if (!my[1]) for (int i = 0; i < 8; i++) my[i] = (unsigned char)morton(0, i);
    int z = pica_slot_size(slot), ax, ay; pica_slot_pos(slot, &ax, &ay);
    unsigned *base = atlas_tex[pica_slot_atlas(slot)].data;
    for (int v = 0; v < z; v++) {
        unsigned *drow = base + (((ay + v) >> 3) * (PICA_ATLAS >> 3) + (ax >> 3)) * 64 + my[v & 7];
        const unsigned *s = rgba + v * z;
        for (int tx = 0; tx < z >> 3; tx++, s += 8) {
            unsigned *t = drow + tx * 64;
            t[0] = pix(s[0]); t[1] = pix(s[1]); t[4] = pix(s[2]); t[5] = pix(s[3]);
            t[16] = pix(s[4]); t[17] = pix(s[5]); t[20] = pix(s[6]); t[21] = pix(s[7]);
        }
    }
    {   /* (the slot's tile rows are a whole atlas row apart: from its first tile to its last, in one request) */
        unsigned *first = base + (((ay >> 3) * (PICA_ATLAS >> 3)) + (ax >> 3)) * 64;
        unsigned *last = base + ((((ay >> 3) + (z >> 3) - 1) * (PICA_ATLAS >> 3)) + (ax >> 3) + (z >> 3)) * 64;
        dcache_flush(first, (u32)((char *)last - (char *)first));
    }
    tk_upload += svcGetSystemTick() - t0;
}

/* ---- primitives */
/* (state is only set when it changes) */
/* the GPU reads the vertices from memory, not through the CPU's cache: written out before the frame is submitted (a
   console drew torn sprites, some vertices still the frame before's; an emulator has no cache to show it) */
static void vb_flush(void) { if (vb_n) dcache_flush(vb, (u32)vb_n * sizeof *vb); }
static PState last_st;                       /* (last_ok 0: something else set state since) */
/* only what differs from the last batch's state is set (a semi-transparent textured primitive is two batches that
   differ in the alpha test and the blend alone); dx: the scissor moved left (the right eye's target, EYE_DX) */
static int last_dx;
static void apply_state(const PState *s, int dx) {
    const int all = !last_ok || dx != last_dx;
    const PState *l = &last_st;
    /* the scissor counts rows from the other edge than the projection (the self test: an area of y 256..511 drew nothing) */
    if (all || s->x1 != l->x1 || s->y1 != l->y1 || s->x2 != l->x2 || s->y2 != l->y2) {
        int x1 = s->x1 - dx, x2 = s->x2 - dx; if (x1 < 0) x1 = 0; if (x2 < 0) x2 = 0;
        C3D_SetScissor(GPU_SCISSOR_NORMAL, x1, 511 - s->y2, x2 + 1, 512 - s->y1);
    }
    if (all || (s->tex >= 0) != (l->tex >= 0) || s->mod != l->mod) set_env(s->tex >= 0, s->mod);
    if (all || s->atest != l->atest) set_atest(s->atest);
    if (all || s->blend != l->blend) set_blend(s->blend);
    if (s->tex >= 0 && (all || s->tex != l->tex)) {
        C3D_TexBind(0, &atlas_tex[s->tex]);
        set_tscale(1.0f / (float)PICA_ATLAS, 1.0f / (float)(s->tex == 2 ? atlas2_h : PICA_ATLAS));
    }
    last_st = *s; last_ok = 1; last_dx = dx;
}
/* --stereo: the batches of a frame drawn for the left eye (into VRAM's target) are drawn again, at the end, into the
   right eye's target, where the shader moves each vertex by 2 sh and the whole picture EYE_DX to the left (so nothing
   crosses the target's right edge: a console draws nothing of such a triangle).  Only a screen's drawing area's. */
#define EYE_DX 64
#define NREC 4096
static struct { PState s; int first, n; } rec[NREC];
static int nrec, replaying;
static void replay(void);
static void c3d_tris(const PState *s, const PV *v, int n) {
    u64 t0 = svcGetSystemTick();
    begin();
    cmd_room();
    if (vb_n + n > VB_MAX - 256) { end_and_wait(); begin(); }
    apply_state(s, 0);
    /* the vertices are in the buffer already when the backend built the batch there (c3d_vbuf) and the frame was not
       ended since.  The position's + 0.5 (the PICA samples pixel centres, the PS1 integer positions), a hair less in
       both axes (an edge exactly on the centres takes the row below, and a console left a column undrawn between two
       sprites side by side), is the shader's; an untextured vertex's alpha is 0 (bit 15 clear on a read-back) */
    if (v != vb + vb_n) memmove(vb + vb_n, v, (size_t)n * sizeof *v);
    on_target(s->wide);
    C3D_DrawArrays(GPU_TRIANGLES, vb_n, n);
    {   extern int pica_stereo_frame;
        if (pica_stereo_frame && eye_rt && !s->wide && s->x2 - s->x1 >= 255 && s->y2 - s->y1 >= 199) {
            if (nrec && rec[nrec - 1].first + rec[nrec - 1].n == vb_n && !memcmp(&rec[nrec - 1].s, s, sizeof *s)) rec[nrec - 1].n += n;
            else { if (nrec == NREC) replay(); rec[nrec].s = *s; rec[nrec].first = vb_n; rec[nrec].n = n; nrec++; }
        }
    }
    vb_n += n;
    { extern int pica_prof_on; extern unsigned long long pica_prof[6]; if (pica_prof_on) pica_prof[4] += svcGetSystemTick() - t0; }
    tk_draw += svcGetSystemTick() - t0;
}
static void replay(void) {
    if (!nrec || !in_frame || replaying) { nrec = 0; return; }
    u64 t0 = svcGetSystemTick();
    replaying = 1;
    on_target(2);
    C3D_FVUnifSet(GPU_VERTEX_SHADER, u_eye, 2.0f, -(float)EYE_DX, 0.0f, 0.0f);
    last_ok = 0;
    for (int i = 0; i < nrec; i++) {
        u32 *buf, size, off; GPUCMD_GetBuffer(&buf, &size, &off);
        if (off + 4096 > size) {   /* (the command buffer nearly full: what is recorded is run and waited for; the
                                      vertices stay where they are, nothing is written to the buffer meanwhile) */
            vb_flush(); C3D_FrameEnd(GX_CMDLIST_FLUSH); in_frame = 0;
            C3D_FrameBegin(0); C3D_FrameEnd(GX_CMDLIST_FLUSH);
            C3D_FrameBegin(0); C3D_FrameDrawOn(eye_rt); in_frame = 1; cur_wide = 2; proj_vram();
            C3D_FVUnifSet(GPU_VERTEX_SHADER, u_eye, 2.0f, -(float)EYE_DX, 0.0f, 0.0f);
            last_ok = 0; st_cmd_splits++;
        }
        apply_state(&rec[i].s, EYE_DX);
        C3D_DrawArrays(GPU_TRIANGLES, rec[i].first, rec[i].n);
    }
    C3D_FVUnifSet(GPU_VERTEX_SHADER, u_eye, 0.0f, 0.0f, 0.0f, 0.0f);
    last_ok = 0; nrec = 0; replaying = 0;
    tk_eye += svcGetSystemTick() - t0;
}
/* where the backend builds its next batch: the free end of the vertex buffer (at least 2048 vertices of it) */
static PV *c3d_vbuf(int *room) {
    begin();   /* (the vertices are written before the batch is drawn: the frames submitted before must be done with them) */
    if (vb_n > VB_MAX - 2048 - 256) { end_and_wait(); begin(); }
    *room = VB_MAX - 256 - vb_n;
    return vb + vb_n;
}
/* the staging texture, its memory from the arena: w x h rounded up to powers of two (>= 8).  (One C3D_Tex for all:
   citro3d keeps the pointer of the bound texture, and the draw has copied its fields into the command list) */
static C3D_Tex stg;
static int stage_tex(C3D_Tex *t, int w, int h) {
    int tw = 8, th = 8;
    while (tw < w) tw <<= 1;
    while (th < h) th <<= 1;
    unsigned bytes = (unsigned)tw * th * 4;
    if (bytes > ARENA_BYTES) return -1;
    begin();
    if (arena_off + bytes > ARENA_BYTES) { end_and_wait(); begin(); }
    memset(t, 0, sizeof *t);
    t->data = arena + arena_off;
    t->width = (u16)tw; t->height = (u16)th;
    t->param = GPU_TEXTURE_MODE(GPU_TEX_2D);
    t->fmt = GPU_RGBA8;
    t->size = bytes;
    arena_off += bytes;
    C3D_TexSetFilter(t, GPU_NEAREST, GPU_NEAREST);
    C3D_TexSetWrap(t, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
    return 0;
}
static void c3d_write(int x, int y, int w, int h, const unsigned *src, int stride) {
    u64 t0 = svcGetSystemTick();
    for (int j0 = 0; j0 < h; j0 += 256)   /* (in pieces of at most 256 x 256: the arena) */
        for (int i0 = 0; i0 < w; i0 += 256) {
            int pw = w - i0 < 256 ? w - i0 : 256, ph = h - j0 < 256 ? h - j0 : 256;
            C3D_Tex *tp = &stg;
            begin(); cmd_room();   /* (before the piece is staged: a split frees the arena) */
            if (stage_tex(tp, pw, ph)) continue;
            C3D_Tex t = *tp;
            unsigned *d = t.data;
            {   /* (a tile of 8 x 8 at a time, its texels' places from a table: the place worked out per pixel was most
                   of an upload's time: a movie's frame, a menu's pictures) */
                static unsigned char mo[64];
                if (!mo[1]) for (int k = 0; k < 64; k++) mo[k] = (unsigned char)morton(k & 7, k >> 3);
                const int tpr = t.width >> 3;
                for (int ty = 0; ty < ph; ty += 8) for (int tx = 0; tx < pw; tx += 8) {
                    unsigned *tl = d + ((ty >> 3) * tpr + (tx >> 3)) * 64;
                    const int nx = pw - tx < 8 ? pw - tx : 8, ny = ph - ty < 8 ? ph - ty : 8;
                    for (int yy = 0; yy < ny; yy++) {
                        const unsigned *sr = src + (j0 + ty + yy) * stride + i0 + tx; const unsigned char *m = mo + yy * 8;
                        for (int xx = 0; xx < nx; xx++) tl[m[xx]] = pix(sr[xx]);
                    }
                }
            }
            dcache_flush(d, (unsigned)t.width * t.height * 4);
            if (vb_n + 6 > VB_MAX - 64) end_and_wait();
            begin(); on_target(0);
            C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
            set_env(1, 0); set_atest(AT_OFF); set_blend(-1);
            C3D_TexBind(0, tp);
            float X0 = x + i0, Y0 = y + j0, X1 = X0 + pw, Y1 = Y0 + ph, S1 = (float)pw / t.width, T0 = 1.0f, T1 = 1.0f - (float)ph / t.height;
            /* (no vertex on the target's far edges: a console draws nothing of such a triangle, see shims/gpu_pica.c
               clip_tris; the texture coordinate cut by as much) */
            if (X1 > EDGE_X) { S1 *= (EDGE_X - X0) / (X1 - X0); X1 = EDGE_X; }
            if (Y1 > EDGE_Y) { T1 = T0 + (T1 - T0) * (EDGE_Y - Y0) / (Y1 - Y0); Y1 = EDGE_Y; }
            QUAD_BEGIN();
            vtx(X0, Y0, 0, T0, 1, 1, 1, 1); vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X0, Y1, 0, T1, 1, 1, 1, 1);
            vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(X0, Y1, 0, T1, 1, 1, 1, 1);
            QUAD_END();
            st_writes++;
        }
    tk_write += svcGetSystemTick() - t0;
}
/* the same from the PS1's words (bit 15, blue, green, red: 5 bits each), staged as RGBA5551 texels: one pass, half the
   bytes.  The PICA widens 5 bits to 8 as the backend does (the high bits repeated below), bit 15 is the alpha. */
static void write15_on(int wide, int x, int y, int w, int h, const unsigned short *src, int stride) {
    u64 t0 = svcGetSystemTick();
    static unsigned char mo[64];
    if (!mo[1]) for (int k = 0; k < 64; k++) mo[k] = (unsigned char)morton(k & 7, k >> 3);
    for (int j0 = 0; j0 < h; j0 += 256)
        for (int i0 = 0; i0 < w; i0 += 256) {
            int pw = w - i0 < 256 ? w - i0 : 256, ph = h - j0 < 256 ? h - j0 : 256;
            C3D_Tex *tp = &stg;
            begin(); cmd_room();
            if (stage_tex(tp, pw, ph)) continue;   /* (4 bytes a texel of the arena taken, 2 used) */
            tp->fmt = GPU_RGBA5551; tp->size = (u32)tp->width * tp->height * 2;
            C3D_Tex t = *tp;
            u16 *d = t.data;
            const int tpr = t.width >> 3;
            for (int ty = 0; ty < ph; ty += 8) for (int tx = 0; tx < pw; tx += 8) {
                u16 *tl = d + ((ty >> 3) * tpr + (tx >> 3)) * 64;
                const int nx = pw - tx < 8 ? pw - tx : 8, ny = ph - ty < 8 ? ph - ty : 8;
                for (int yy = 0; yy < ny; yy++) {
                    const unsigned short *sr = src + (j0 + ty + yy) * stride + i0 + tx; const unsigned char *m = mo + yy * 8;
                    for (int xx = 0; xx < nx; xx++) {
                        const unsigned c = sr[xx];
                        tl[m[xx]] = (u16)((c & 31) << 11 | (c & 0x3E0) << 1 | (c >> 9 & 0x3E) | c >> 15);
                    }
                }
            }
            dcache_flush(d, (unsigned)t.width * t.height * 2);
            if (vb_n + 6 > VB_MAX - 64) end_and_wait();
            begin(); on_target(wide);
            C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
            set_env(1, 0); set_atest(AT_OFF); set_blend(-1);
            C3D_TexBind(0, tp);
            float X0 = x + i0, Y0 = y + j0, X1 = X0 + pw, Y1 = Y0 + ph, S1 = (float)pw / t.width, T0 = 1.0f, T1 = 1.0f - (float)ph / t.height;
            if (X1 > EDGE_X) { S1 *= (EDGE_X - X0) / (X1 - X0); X1 = EDGE_X; }
            if (Y1 > EDGE_Y) { T1 = T0 + (T1 - T0) * (EDGE_Y - Y0) / (Y1 - Y0); Y1 = EDGE_Y; }
            QUAD_BEGIN();
            vtx(X0, Y0, 0, T0, 1, 1, 1, 1); vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X0, Y1, 0, T1, 1, 1, 1, 1);
            vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(X0, Y1, 0, T1, 1, 1, 1, 1);
            QUAD_END();
            st_writes++;
        }
    tk_write += svcGetSystemTick() - t0;
}
static void c3d_write15(int x, int y, int w, int h, const unsigned short *src, int stride) { write15_on(0, x, y, w, h, src, stride); }
static void c3d_wide_write(int x, int y, int w, int h, const unsigned short *src, int stride) { write15_on(1, x, y, w, h, src, stride); }
static void fill_on(int wide, int x, int y, int w, int h, unsigned c);
static void c3d_fill(int x, int y, int w, int h, unsigned c) { fill_on(0, x, y, w, h, c); }
static void c3d_wide_fill(int x, int y, int w, int h, unsigned c) { fill_on(1, x, y, w, h, c); }
/* the wide plane's rectangle into VRAM's target, texel for pixel (the frame's 4:3 middle, after every ordering table:
   what the game reads back, copies or uses as a texture is there) */
static void c3d_eye_fill(int x, int y, int w, int h, unsigned c) {
    begin(); replay();   /* (after the batches before it) */
    x -= EYE_DX; if (x < 0) { w += x; x = 0; }
    if (w > 0) fill_on(2, x, y, w, h, c);
}
static int c3d_eye_start(void) {
    if (eye_rt) return 0;
    if (!C3D_TexInitVRAM(&eye_tex, 1024, 512, GPU_RGBA8)) { port_printf("[3ds] pica: no VRAM for the right eye (%u KB free)\n", (unsigned)(vramSpaceFree() >> 10)); return -1; }
    C3D_TexSetFilter(&eye_tex, GPU_NEAREST, GPU_NEAREST);
    eye_rt = C3D_RenderTargetCreateFromTex(&eye_tex, GPU_TEXFACE_2D, 0, -1);
    top_r = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, -1);
    if (!eye_rt || !top_r) { port_printf("[3ds] pica: no render targets for the right eye\n"); eye_rt = 0; top_r = 0; return -1; }
    C3D_RenderTargetClear(eye_rt, C3D_CLEAR_ALL, 0x000000FF, 0);
    C3D_RenderTargetSetOutput(top_r, GFX_TOP, GFX_RIGHT,
                              GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
                              GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(scr_fmt) |
                              GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
    gfxSet3D(true);
    port_printf("[3ds] pica: stereo: the right eye's target is ready (%u KB of VRAM left)\n", (unsigned)(vramSpaceFree() >> 10));
    return 0;
}
static void c3d_wide_down(int wx, int wy, int x, int y, int w, int h) {
    begin(); cmd_room();
    if (vb_n + 6 > VB_MAX - 64) { end_and_wait(); begin(); }
    on_target(0);
    C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
    set_env(1, 0); set_atest(AT_OFF); set_blend(-1);
    C3D_TexBind(0, &wide_tex);
    float X0 = x, Y0 = y, X1 = x + w, Y1 = y + h;
    float S0 = wx / 1024.0f, S1 = (wx + w) / 1024.0f, T0 = 1.0f - wy / 512.0f, T1 = 1.0f - (wy + h) / 512.0f;
    if (X1 > EDGE_X) { S1 = S0 + (S1 - S0) * (EDGE_X - X0) / (X1 - X0); X1 = EDGE_X; }
    if (Y1 > EDGE_Y) { T1 = T0 + (T1 - T0) * (EDGE_Y - Y0) / (Y1 - Y0); Y1 = EDGE_Y; }
    QUAD_BEGIN();
    vtx(X0, Y0, S0, T0, 1, 1, 1, 1); vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
    vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
    QUAD_END();
}
static int c3d_wide_start(void) {
    if (wide_rt) return 0;
    if (!C3D_TexInitVRAM(&wide_tex, 1024, 512, GPU_RGBA8)) { port_printf("[3ds] pica: no VRAM for the wide plane (%u KB free)\n", (unsigned)(vramSpaceFree() >> 10)); return -1; }
    C3D_TexSetFilter(&wide_tex, GPU_NEAREST, GPU_NEAREST);
    wide_rt = C3D_RenderTargetCreateFromTex(&wide_tex, GPU_TEXFACE_2D, 0, -1);
    if (!wide_rt) { port_printf("[3ds] pica: no render target for the wide plane\n"); return -1; }
    C3D_RenderTargetClear(wide_rt, C3D_CLEAR_ALL, 0x000000FF, 0);
    port_printf("[3ds] pica: widescreen: the wide plane is ready (%u KB of VRAM left)\n", (unsigned)(vramSpaceFree() >> 10));
    return 0;
}
static void fill_on(int wide, int x, int y, int w, int h, unsigned c) {
    begin();
    cmd_room();
    on_target(wide);
    C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
    set_env(0, 0); set_atest(AT_OFF); set_blend(-1);
    float r = (c & 255) / 255.0f, g = ((c >> 8) & 255) / 255.0f, b = ((c >> 16) & 255) / 255.0f, a = (c >> 24) / 255.0f;
    float X0 = x, Y0 = y, X1 = x + w, Y1 = y + h;
    if (X1 > EDGE_X) X1 = EDGE_X;
    if (Y1 > EDGE_Y) Y1 = EDGE_Y;
    QUAD_BEGIN();
    vtx(X0, Y0, 0, 0, r, g, b, a); vtx(X1, Y0, 0, 0, r, g, b, a); vtx(X0, Y1, 0, 0, r, g, b, a);
    vtx(X1, Y0, 0, 0, r, g, b, a); vtx(X1, Y1, 0, 0, r, g, b, a); vtx(X0, Y1, 0, 0, r, g, b, a);
    QUAD_END();
}
static void c3d_read(int x, int y, int w, int h, unsigned *dst, int stride) {
    u64 t0 = svcGetSystemTick();
    if (n_rd < 4) { rd_log[n_rd][0] = (short)x; rd_log[n_rd][1] = (short)y; rd_log[n_rd][2] = (short)w; rd_log[n_rd][3] = (short)h; }
    n_rd++;
    /* (nothing drawn since the last read: its copy of the target serves.  A texture's drawn cells are read back a row
       at a time, four whole-target transfers where one does: the logos at the start read 1.8 times a frame, 12 ms) */
    if (!in_frame && lin_fresh) {
        for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) dst[j * stride + i] = pix(lin[(y + j) * 1024 + x + i]);
        tk_read += svcGetSystemTick() - t0;
        return;
    }
    end_and_wait();
    /* Twice: the wait inside is for "a transfer has ended", and one of an earlier frame's transfers to the screens
       can have ended unseen, so the first wait may return before this transfer has run (a console's start-up probe
       read a pixel as not drawn and, a moment later, its neighbour as drawn).  The second call's wait cannot end
       before the first transfer has. */
    for (int k = 0; k < 2; k++)
    C3D_SyncDisplayTransfer((u32 *)vram_tex.data, GX_BUFFER_DIM(1024, 512), (u32 *)lin, GX_BUFFER_DIM(1024, 512),
                            GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
                            GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) |
                            GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
    GSPGPU_InvalidateDataCache(lin, 1024 * 512 * 4);
    lin_fresh = 1;
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) dst[j * stride + i] = pix(lin[(y + j) * 1024 + x + i]);
    tk_read += svcGetSystemTick() - t0;
}

/* ---- the top screen: the display area, 4:3 at the screen's height, centred */
static C3D_Tex *pres_tex = &vram_tex; static int pres_w = 320;   /* what a present shows, and how wide on the screen */
static int c3d_present(int x, int y, int w, int h, int masked) {
    u64 t0 = svcGetSystemTick();
    begin();
    replay();
    C3D_RenderTargetClear(top_rt, C3D_CLEAR_ALL, 0x000000FF, 0);
    C3D_FrameDrawOn(top_rt);
    C3D_Mtx m; Mtx_OrthoTilt(&m, 0.0f, 400.0f, 240.0f, 0.0f, 1.0f, -1.0f, true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &m);
    C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
    if (!masked && w > 0 && h > 0) {
        set_env(1, 0); set_atest(AT_OFF); set_blend(-1);
        C3D_TexSetFilter(pres_tex, GPU_LINEAR, GPU_LINEAR);
        C3D_TexBind(0, pres_tex);
        float X0 = 200 - pres_w / 2, X1 = 200 + pres_w / 2, Y0 = 0, Y1 = 240;   /* 320 x 240 (the wide plane's frame: 400) */
        float S0 = x / 1024.0f, S1 = (x + w) / 1024.0f, T0 = 1.0f - y / 512.0f, T1 = 1.0f - (y + h) / 512.0f;
        QUAD_BEGIN();
        vtx(X0, Y0, S0, T0, 1, 1, 1, 1); vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
        vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
        QUAD_END();
    }
    if (top_r) {   /* --stereo: the right eye's picture, its target's display area (or the left one's while the slider is down) */
        extern float port_stereo_k;
        port_stereo_k = osGet3DSliderState();
        extern int port_stereo_flat;
        const int eye = port_stereo_k > 0.01f && !port_stereo_flat;
        C3D_Tex *rt = eye ? &eye_tex : pres_tex;
        const int ex = eye ? EYE_DX : 0;
        C3D_RenderTargetClear(top_r, C3D_CLEAR_ALL, 0x000000FF, 0);
        C3D_FrameDrawOn(top_r);
        if (!masked && w > 0 && h > 0) {
            C3D_TexSetFilter(rt, GPU_LINEAR, GPU_LINEAR);
            C3D_TexBind(0, rt);
            float X0 = 200 - pres_w / 2, X1 = 200 + pres_w / 2, Y0 = 0, Y1 = 240;
            float S0 = (x - ex) / 1024.0f, S1 = (x - ex + w) / 1024.0f, T0 = 1.0f - y / 512.0f, T1 = 1.0f - (y + h) / 512.0f;
            QUAD_BEGIN();
            vtx(X0, Y0, S0, T0, 1, 1, 1, 1); vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
            vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
            QUAD_END();
            C3D_TexSetFilter(rt, GPU_NEAREST, GPU_NEAREST);
        }
    }
    if (bot_rt && panel_w) {   /* the bottom screen: the panel's canvas, 1:1 (320 x 240) */
        C3D_RenderTargetClear(bot_rt, C3D_CLEAR_ALL, 0x000000FF, 0);
        C3D_FrameDrawOn(bot_rt);
        Mtx_OrthoTilt(&m, 0.0f, 320.0f, 240.0f, 0.0f, 1.0f, -1.0f, true);
        C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &m);
        set_env(1, 0); set_atest(AT_OFF); set_blend(-1);
        C3D_TexBind(0, &panel_tex);
        float X1 = panel_w, Y1 = panel_h, S1 = panel_w / 512.0f, T1 = 1.0f - panel_h / 256.0f;
        QUAD_BEGIN();
        vtx(0, 0, 0, 1, 1, 1, 1, 1); vtx(X1, 0, S1, 1, 1, 1, 1, 1); vtx(0, Y1, 0, T1, 1, 1, 1, 1);
        vtx(X1, 0, S1, 1, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(0, Y1, 0, T1, 1, 1, 1, 1);
        QUAD_END();
    }
    vb_flush();
    {   /* (the frame before's: measured from its submission to the end of its queue) */
        double g = C3D_GetDrawingTime(); gpu_ms_sum += g; gpu_ms_n++; if (g > gpu_ms_max) gpu_ms_max = g;
    }
    C3D_FrameEnd(GX_CMDLIST_FLUSH);
    in_frame = 0;
    C3D_TexSetFilter(pres_tex, GPU_NEAREST, GPU_NEAREST);
    proj_vram();
    frame_no++; st_frames++;
    arena_off = 0; vb_n = 0; last_ok = 0;   /* (the next C3D_FrameBegin waits for this frame: the arena is free then) */
    u64 t1 = svcGetSystemTick();
    tk_present += t1 - t0;
    if (tk_last) tk_frame += t1 - tk_last;
    tk_last = t1;
    if (st_frames % TK_N == 0) {
        port_printf("[3ds] pica: per frame %.2f texture uploads (%.2f waited for the GPU; decoding %.2f ms), %.2f read-backs\n",
                    (double)n_up / TK_N, (double)n_wait / TK_N, pica_decode_us / 1000.0 / TK_N, (double)n_rd / TK_N);
        {   extern unsigned long long pica_dec_why[9]; extern int port_current_area(void);
            static unsigned long long last[9]; unsigned long long d[9];
            for (int i = 0; i < 9; i++) { d[i] = pica_dec_why[i] - last[i]; last[i] = pica_dec_why[i]; }
            port_printf("[3ds] pica: area %d; decodes in these %d presents: %llu texels written, %llu CLUT changed, %llu/%llu/%llu took a slot (256/128/64), %llu/%llu/%llu took one used in the frame, %llu free\n",
                        port_current_area(), TK_N, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8]);
        }
        for (unsigned i = 0; i < 4 && i < n_rd; i++) port_printf("[3ds] pica: read-back %d,%d %dx%d\n", rd_log[i][0], rd_log[i][1], rd_log[i][2], rd_log[i][3]);
        { extern unsigned long long port_sound_underruns(void); unsigned long long u = port_sound_underruns();
          if (u) port_printf("[3ds] sound: the mixer thread was %llu samples short in these %d presents\n", u, TK_N); }
        { extern unsigned long long port_frames_skipped; static unsigned long long last;
          port_printf("[3ds] frameskip: %llu ordering tables not drawn in these %d presents\n", port_frames_skipped - last, TK_N); last = port_frames_skipped; }
        {   /* the sampled ordering tables' time (shims/gpu.c, shims/gpu_pica.c), in ms per sampled table */
            extern unsigned long long pica_prof[6]; extern unsigned pica_prof_n;
            {   extern unsigned long long pica_cnt[8], pica_ot[4];
                static unsigned long long lastc[8]; unsigned long long d[8], nt = pica_ot[2] ? pica_ot[2] : 1;
                for (int i = 0; i < 8; i++) { d[i] = pica_cnt[i] - lastc[i]; lastc[i] = pica_cnt[i]; }
                port_printf("[3ds] pica: a drawn table: %llu quads and %llu triangles by the short path, %llu triangles by the general one, %llu rectangles, %llu line pixels, %llu texture look-ups past the memo (%llu looked at the drawn cells)\n",
                            d[0] / nt, d[1] / nt, d[2] / nt, d[3] / nt, d[4] / nt, d[5] / nt, d[6] / nt);
            }
            if (pica_prof_n) {
                double k = 1000.0 / SYSCLOCK_ARM11 / pica_prof_n;
                /* (the decoder alone and with the backend are dry passes over the table's primitives before it is drawn) */
                port_printf("[3ds] profile of %u ordering tables, ms each: drawn %.2f (the device's part %.2f); its primitives through the decoder alone %.2f, the decoder and the backend %.2f\n",
                            pica_prof_n, pica_prof[0] * k, pica_prof[4] * k, pica_prof[1] * k, pica_prof[2] * k);
            }
            { extern unsigned long long pica_ot[4]; double q = 1000.0 / SYSCLOCK_ARM11;
              port_printf("[3ds] ordering tables: %llu drawn, %.2f ms each; %llu skipped, %.2f ms each\n", pica_ot[2], pica_ot[2] ? pica_ot[0] * q / pica_ot[2] : 0.0,
                          pica_ot[3], pica_ot[3] ? pica_ot[1] * q / pica_ot[3] : 0.0);
              memset(pica_ot, 0, sizeof pica_ot); }
            memset(pica_prof, 0, sizeof pica_prof); pica_prof_n = 0;
        }
        n_up = n_rd = n_wait = 0; pica_decode_us = 0;
        port_printf("[3ds] pica: %llu frames, %llu syncs, %llu host writes; ms per frame: %.2f total, %.2f draws, %.2f texture uploads, "
                    "%.2f host writes, %.2f read-backs, %.2f panel, %.2f present (the rest: the game, the core, sound)\n",
                    st_frames, st_syncs, st_writes, TK_MS(tk_frame), TK_MS(tk_draw), TK_MS(tk_upload), TK_MS(tk_write), TK_MS(tk_read),
                    TK_MS(tk_panel), TK_MS(tk_present));
        port_printf("[3ds] pica: per frame %.2f ms waiting for the GPU to end the frame before, %.2f changes of target; the GPU's own time a frame %.2f ms (the longest %.2f); %.2f ms recording the right eye\n",
                    TK_MS(tk_begin), (double)st_switch / TK_N, gpu_ms_n ? gpu_ms_sum / gpu_ms_n : 0.0, gpu_ms_max, TK_MS(tk_eye));
        tk_eye = 0;
        gpu_ms_sum = gpu_ms_max = 0; gpu_ms_n = 0;
        tk_begin = 0; st_switch = 0;
        tk_frame = tk_draw = tk_upload = tk_write = tk_read = tk_panel = tk_present = 0;
    }
    return 1;
}

/* the open frame to the GPU (pica_submit): its memory, the vertex buffer and the arena, is free again once the next
   begin() has waited for it, and every writer of them begins first */
static void c3d_submit(void) {
    if (!in_frame) return;
    replay();
    vb_flush();
    { double g = C3D_GetDrawingTime(); gpu_ms_sum += g; gpu_ms_n++; if (g > gpu_ms_max) gpu_ms_max = g; }
    C3D_FrameEnd(GX_CMDLIST_FLUSH);
    in_frame = 0;
    frame_no++; arena_off = 0; vb_n = 0; last_ok = 0;
}
static int c3d_wide_present(int x, int y, int w, int h) {
    pres_tex = &wide_tex; pres_w = w > 400 ? 400 : w;
    int r = c3d_present(x, y, w, h, 0);
    pres_tex = &vram_tex; pres_w = 320;
    return r;
}
/* ---- the bottom screen: the panel's canvas (runtime/sdl.c port_panel_window_frame) and its touches */
void port_n3ds_panel(const unsigned char *rgba, int w, int h) {
    if (!bot_rt || w <= 0 || h <= 0) return;
    if (w > 320) w = 320; if (h > 240) h = 240;
    /* only the 8 x 8 tiles of the canvas that changed (the panel redraws mostly the same picture, and a touch changes
       a corner of it), their texels' places from a table, and only those tiles' rows flushed.  Without a wait for the
       frames submitted before: the bottom screen is drawn from this texture at every present, so one caught half
       written is put right a frame later, and the wait was the GPU's whole queue when the game's screen is a dear one
       (a touch took 70 ms to show: 60 of them here) */
    static unsigned last[320 * 240]; static int lw, lh;
    static unsigned char mo[64];
    if (!mo[1]) for (int k = 0; k < 64; k++) mo[k] = (unsigned char)morton(k & 7, k >> 3);
    u64 t0 = svcGetSystemTick();
    const int all = w != lw || h != lh;
    const unsigned *src = (const unsigned *)rgba; unsigned *d = panel_tex.data;
    int r0 = -1, r1 = -1;   /* the tile rows written */
    for (int ty = 0; ty < h; ty += 8) {
        const int ny = h - ty < 8 ? h - ty : 8; int row = 0;
        for (int tx = 0; tx < w; tx += 8) {
            const int nx = w - tx < 8 ? w - tx : 8; int diff = all;
            for (int yy = 0; yy < ny && !diff; yy++) diff = memcmp(src + (ty + yy) * w + tx, last + (ty + yy) * w + tx, (size_t)nx * 4) != 0;
            if (!diff) continue;
            unsigned *tl = d + ((ty >> 3) * 64 + (tx >> 3)) * 64;
            for (int yy = 0; yy < ny; yy++) {
                const unsigned *sr = src + (ty + yy) * w + tx; const unsigned char *m = mo + yy * 8;
                memcpy(last + (ty + yy) * w + tx, sr, (size_t)nx * 4);
                for (int xx = 0; xx < nx; xx++) tl[m[xx]] = pix(sr[xx]) | 0xFF;
            }
            row = 1;
        }
        if (row) { if (r0 < 0) r0 = ty >> 3; r1 = ty >> 3; }
    }
    lw = w; lh = h;
    if (r0 >= 0) dcache_flush(d + r0 * 64 * 64, (unsigned)(r1 - r0 + 1) * 64 * 64 * 4);
    panel_w = w; panel_h = h;
    tk_panel += svcGetSystemTick() - t0;
}
void port_n3ds_touch(void) {
    extern void port_panel_touch(int, int, int);
    static int down, lx, ly;
    if (hidKeysHeld() & KEY_TOUCH) {
        touchPosition p; hidTouchRead(&p);
        port_panel_touch(p.px, p.py, down ? 2 : 0);
        down = 1; lx = p.px; ly = p.py;
    } else if (down) { port_panel_touch(lx, ly, 1); down = 0; }
}

/* ---- start-up */
static void self_test(void) {
    unsigned red = 0xFF0000FFu, green = 0xFF00FF00u, got[2];
    c3d_fill(0, 0, 16, 16, red);
    c3d_fill(0, 496, 16, 16, green);
    c3d_read(4, 4, 1, 1, &got[0], 1);
    c3d_read(4, 500, 1, 1, &got[1], 1);
    port_printf("[3ds] pica self test: (4,4) %08x (want %08x), (4,500) %08x (want %08x): %s\n", got[0], red, got[1], green,
                got[0] == red && got[1] == green ? "ok" : got[0] == green && got[1] == red ? "FLIPPED" : "WRONG");
    c3d_fill(0, 0, 16, 16, 0); c3d_fill(0, 496, 16, 16, 0);
    /* the primitive path: a flat quad at y 300..315 with the drawing area the whole VRAM, then with y 256..511;
       read at y 304 (where it belongs) and at y 207 (511 - 304: mirrored) */
    PV q[6] = { { 32, 300, 0, 0, 255, 0, 0, 255 }, { 48, 300, 0, 0, 255, 0, 0, 255 }, { 32, 316, 0, 0, 255, 0, 0, 255 },
                { 48, 300, 0, 0, 255, 0, 0, 255 }, { 48, 316, 0, 0, 255, 0, 0, 255 }, { 32, 316, 0, 0, 255, 0, 0, 255 } };
    for (int k = 0; k < 2; k++) {
        PState st = { -1, 0, AT_OFF, -1, 0, k ? 256 : 0, 1023, 511 };
        c3d_fill(0, 0, 1024, 512, 0);
        c3d_tris(&st, q, 6);
        unsigned a1, a2; c3d_read(40, 304, 1, 1, &a1, 1); c3d_read(40, 207, 1, 1, &a2, 1);
        port_printf("[3ds] pica self test, triangles (drawing area y %d..511): at y 304 %08x, at y 207 %08x\n", k ? 256 : 0, a1, a2);
    }
    c3d_fill(0, 0, 1024, 512, 0);
    /* where things land: 8 x 8 marks near the target's corners through the triangle path, then the same through the
       host-write path; the whole target read back and each mark's place logged (a console showed a frame whose right
       and lower edges were not drawn) */
    static const short mk[4][2] = { { 64, 64 }, { 952, 64 }, { 64, 440 }, { 952, 440 } };
    static unsigned whole[16 * 16];
    for (int pass = 0; pass < 2; pass++) {
        for (int i = 0; i < 4; i++) {
            float x = mk[i][0], y = mk[i][1];
            if (!pass) {
                PV m[6] = { { x, y, 0, 0, 0, 255, 0, 255 }, { x + 8, y, 0, 0, 0, 255, 0, 255 }, { x, y + 8, 0, 0, 0, 255, 0, 255 },
                            { x + 8, y, 0, 0, 0, 255, 0, 255 }, { x + 8, y + 8, 0, 0, 0, 255, 0, 255 }, { x, y + 8, 0, 0, 0, 255, 0, 255 } };
                PState st = { -1, 0, AT_OFF, -1, 0, 0, 1023, 511 };
                c3d_tris(&st, m, 6);
            } else {
                static unsigned blk[64]; for (int k = 0; k < 64; k++) blk[k] = 0xFF00FF00u;
                c3d_write(mk[i][0], mk[i][1], 8, 8, blk, 8);
            }
        }
        for (int i = 0; i < 4; i++) {   /* a 16 x 16 window around each mark: its first marked pixel */
            c3d_read(mk[i][0] - 4, mk[i][1] - 4, 16, 16, whole, 16);
            int fx = -1, fy = -1, n = 0;
            for (int k = 0; k < 256; k++) if ((whole[k] & 0x00FFFFFFu) == 0x0000FF00u) { if (fx < 0) { fx = k & 15; fy = k >> 4; } n++; }
            port_printf("[3ds] pica self test, %s mark at %d,%d: %d pixels, first at %d,%d%s\n", pass ? "write" : "triangle", mk[i][0], mk[i][1], n,
                        fx < 0 ? -1 : mk[i][0] - 4 + fx, fy < 0 ? -1 : mk[i][1] - 4 + fy, n == 64 && fx == 4 && fy == 4 ? "" : "  <-- NOT WHERE IT BELONGS");
        }
        c3d_fill(0, 0, 1024, 512, 0);
    }
    {   /* does the GPU clip a triangle that goes past a side of the target?  One across each side, a vertex far outside
           (a PS1 coordinate's reach); two pixels inside it must be drawn and one beside it left alone.  The backend cuts
           on the CPU the sides that fail (shims/gpu_pica.c pica_gpu_clips); a console drew nothing of a quad with
           vertices exactly on the right edge, which is why it cut them all so far. */
        static const struct { short v[3][2], in[2][2], out[2]; } t[4] = {
            { { { -1100, 100 }, { 40, 100 }, { 40, 140 } }, { { 20, 110 }, { 2, 105 } }, { 60, 110 } },
            { { { 100, -1100 }, { 140, 40 }, { 100, 40 } }, { { 110, 20 }, { 105, 2 } }, { 150, 20 } },
            { { { 1000, 100 }, { 2100, 100 }, { 1000, 140 } }, { { 1010, 105 }, { 1020, 102 } }, { 990, 105 } },
            { { { 100, 480 }, { 100, 1600 }, { 140, 480 } }, { { 105, 490 }, { 102, 508 } }, { 150, 490 } } };
        extern int pica_gpu_clips;
        PState st = { -1, 0, AT_OFF, -1, 0, 0, 1023, 511 };
        int sides = 0;
        for (int i = 0; i < 4; i++) {
            PV m[3]; unsigned g[3];
            c3d_fill(0, 0, 1024, 512, 0);
            for (int k = 0; k < 3; k++) { PV z = { t[i].v[k][0], t[i].v[k][1], 0, 0, 0, 255, 0, 255 }; m[k] = z; }
            c3d_tris(&st, m, 3);
            c3d_read(t[i].in[0][0], t[i].in[0][1], 1, 1, &g[0], 1); c3d_read(t[i].in[1][0], t[i].in[1][1], 1, 1, &g[1], 1);
            c3d_read(t[i].out[0], t[i].out[1], 1, 1, &g[2], 1);
            int ok = (g[0] & 0x00FFFFFFu) == 0x0000FF00u && (g[1] & 0x00FFFFFFu) == 0x0000FF00u && (g[2] & 0x00FFFFFFu) == 0;
            if (ok) sides |= 1 << i;
            port_printf("[3ds] pica self test, a triangle across the %s side: %08x %08x inside, %08x beside: %s\n",
                        (const char *[]){ "left", "upper", "right", "lower" }[i], g[0], g[1], g[2], ok ? "the GPU clips it" : "NOT drawn right: cut on the CPU");
        }
        pica_gpu_clips = sides;
        c3d_fill(0, 0, 1024, 512, 0);
    }
}
/* What the GPU of this machine really draws: a pattern through every path (fill, host write, triangles, the drawing
   area), read back whole and saved as sdmc:/3ds/tombi/gputest.bmp, once at the start.  An emulator and a console
   disagree on details no log line has pinned down; the picture shows them all at once.
   On a dark blue fill, in cells of 128 x 64 from (0,0): at +16,+16 a fill of colour 0 (8 x 8); at +32,+16 a red fill;
   at +48,+16 a white host write; at +64,+16 green triangles; at +80,+16 a green triangle pair with a drawing area of
   that cell's 4 x 4 centre only.  Row 448..: a yellow fill 1024 x 2 at y 448, a white write 256 x 2 at 0,456. */
static void gpu_test_picture(void) {
    unsigned *buf = malloc(1024 * 512 * 4);
    if (!buf) return;
    static unsigned blk[256 * 2];
    c3d_fill(0, 0, 1024, 512, 0xFF400000u);
    for (int cy = 0; cy < 7; cy++) for (int cx = 0; cx < 8; cx++) {
        int x = cx * 128, y = cy * 64 + 16;
        c3d_fill(x + 16, y, 8, 8, 0);
        c3d_fill(x + 32, y, 8, 8, 0xFF0000FFu);
        for (int k = 0; k < 64; k++) blk[k] = 0xFFFFFFFFu;
        c3d_write(x + 48, y, 8, 8, blk, 8);
        for (int k = 0; k < 2; k++) {
            float X = x + 64 + 16 * k, Y = y;
            PV m[6] = { { X, Y, 0, 0, 0, 255, 0, 255 }, { X + 8, Y, 0, 0, 0, 255, 0, 255 }, { X, Y + 8, 0, 0, 0, 255, 0, 255 },
                        { X + 8, Y, 0, 0, 0, 255, 0, 255 }, { X + 8, Y + 8, 0, 0, 0, 255, 0, 255 }, { X, Y + 8, 0, 0, 0, 255, 0, 255 } };
            PState st = { -1, 0, AT_OFF, -1, 0, 0, 1023, 511 };
            if (k) { st.x1 = (int)X + 2; st.y1 = (int)Y + 2; st.x2 = (int)X + 5; st.y2 = (int)Y + 5; }
            c3d_tris(&st, m, 6);
        }
    }
    c3d_fill(0, 448, 1024, 2, 0xFF00FFFFu);
    for (int k = 0; k < 512; k++) blk[k] = 0xFFFFFFFFu;
    c3d_write(0, 456, 256, 2, blk, 256);
    c3d_read(0, 0, 1024, 512, buf, 1024);
    FILE *f = fopen("sdmc:/3ds/tombi/gputest.bmp", "wb");
    if (f) {
        unsigned char h[54] = { 'B', 'M' };
        unsigned size = 54 + 1024 * 512 * 3, w = 1024, hh = 512;
        memcpy(h + 2, &size, 4); h[10] = 54; h[14] = 40; memcpy(h + 18, &w, 4); memcpy(h + 22, &hh, 4); h[26] = 1; h[28] = 24;
        fwrite(h, 1, 54, f);
        static unsigned char row[1024 * 3];
        for (int y = 511; y >= 0; y--) {   /* (a BMP's rows run upwards) */
            for (int x = 0; x < 1024; x++) { unsigned c = buf[y * 1024 + x]; row[3 * x] = (unsigned char)(c >> 16); row[3 * x + 1] = (unsigned char)(c >> 8); row[3 * x + 2] = (unsigned char)c; }
            fwrite(row, 1, sizeof row, f);
        }
        fclose(f);
        port_printf("[3ds] pica: gputest.bmp written\n");
    }
    free(buf);
    c3d_fill(0, 0, 1024, 512, 0);
}
static int c3d_start(void) {
    if (!C3D_Init(C3D_DEFAULT_CMDBUF_SIZE)) { port_printf("[3ds] pica: C3D_Init failed\n"); return -1; }
    if (!C3D_TexInitVRAM(&vram_tex, 1024, 512, GPU_RGBA8)) { port_printf("[3ds] pica: no VRAM for the render target\n"); return -1; }
    C3D_TexSetFilter(&vram_tex, GPU_NEAREST, GPU_NEAREST);
    vram_rt = C3D_RenderTargetCreateFromTex(&vram_tex, GPU_TEXFACE_2D, 0, -1);
    top_rt = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, -1);
    if (!vram_rt || !top_rt) { port_printf("[3ds] pica: render targets failed\n"); return -1; }
    scr_fmt = gfxGetScreenFormat(GFX_TOP);   /* (whatever SDL set up: GX transfer formats have the same values) */
    C3D_RenderTargetSetOutput(top_rt, GFX_TOP, GFX_LEFT,
                              GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
                              GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(scr_fmt) |
                              GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
    for (int i = 0; i < 2; i++) {
        int h = PICA_ATLAS;
        if (!C3D_TexInit(&atlas_tex[i], PICA_ATLAS, (u16)h, GPU_RGBA8)) { port_printf("[3ds] pica: texture atlas %d failed\n", i); return -1; }
        C3D_TexSetFilter(&atlas_tex[i], GPU_NEAREST, GPU_NEAREST);
        C3D_TexSetWrap(&atlas_tex[i], GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
        memset(atlas_tex[i].data, 0, (size_t)PICA_ATLAS * h * 4);
    }
    bot_rt = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, -1);
    if (bot_rt && C3D_TexInit(&panel_tex, 512, 256, GPU_RGBA8)) {
        C3D_TexSetFilter(&panel_tex, GPU_NEAREST, GPU_NEAREST);
        C3D_RenderTargetSetOutput(bot_rt, GFX_BOTTOM, GFX_LEFT,
                                  GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
                                  GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(gfxGetScreenFormat(GFX_BOTTOM)) |
                                  GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
    } else { port_printf("[3ds] pica: no bottom screen target: no panel\n"); bot_rt = 0; }
    lin = linearAlloc(1024 * 512 * 4);
    arena = linearAlloc(ARENA_BYTES);
    if (!lin || !arena) { port_printf("[3ds] pica: no linear memory\n"); return -1; }
    vsh = DVLB_ParseFile((u32 *)pica_vsh_shbin, pica_vsh_shbin_size);
    shaderProgramInit(&prog);
    shaderProgramSetVsh(&prog, &vsh->DVLE[0]);
    C3D_BindProgram(&prog);
    u_proj = shaderInstanceGetUniformLocation(prog.vertexShader, "projection");
    u_tsc = shaderInstanceGetUniformLocation(prog.vertexShader, "tscale");
    u_eye = shaderInstanceGetUniformLocation(prog.vertexShader, "eye");
    C3D_FVUnifSet(GPU_VERTEX_SHADER, u_eye, 0.0f, 0.0f, 0.0f, 0.0f);
    C3D_AttrInfo *ai = C3D_GetAttrInfo();
    AttrInfo_Init(ai);
    AttrInfo_AddLoader(ai, 0, GPU_FLOAT, 2);           /* v0 position (PV x, y) */
    AttrInfo_AddLoader(ai, 1, GPU_FLOAT, 2);           /* v1 texels (u, v) */
    AttrInfo_AddLoader(ai, 2, GPU_UNSIGNED_BYTE, 4);   /* v2 colour */
    AttrInfo_AddLoader(ai, 3, GPU_FLOAT, 1);           /* v3 sh (--stereo) */
    vb = linearAlloc(VB_MAX * sizeof *vb);
    if (!vb) { port_printf("[3ds] pica: no vertex buffer\n"); return -1; }
    /* the third atlas, last and only with room to spare in the linear heap (the SDL screens, the staging arena and
       the read-back buffer come first) */
    {   /* (whole, 64 slots, with 6 MB free; an Old 3DS had 5.9 MB left: half of it there, 32 slots in 2 MB) */
        u32 free = linearSpaceFree();
        int h = free > (6u << 20) ? PICA_ATLAS : free > (7u << 19) ? PICA_ATLAS / 2 : 0;
        if (h && C3D_TexInit(&atlas_tex[2], PICA_ATLAS, (u16)h, GPU_RGBA8)) {
            C3D_TexSetFilter(&atlas_tex[2], GPU_NEAREST, GPU_NEAREST);
            C3D_TexSetWrap(&atlas_tex[2], GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
            memset(atlas_tex[2].data, 0, (size_t)PICA_ATLAS * h * 4);
            atlas2_h = h;
            pica_n128x = h == PICA_ATLAS ? PICA_N128X : PICA_N128X / 2;
        }
    }
    port_printf("[3ds] pica: %d slots of 128 x 128, %u KB of linear memory left\n", PICA_N128 + pica_n128x, (unsigned)(linearSpaceFree() >> 10));
    C3D_BufInfo *bi = C3D_GetBufInfo();
    BufInfo_Init(bi);
    BufInfo_Add(bi, vb, sizeof *vb, 4, 0x3210);
    C3D_DepthTest(false, GPU_ALWAYS, GPU_WRITE_COLOR);
    C3D_CullFace(GPU_CULL_NONE);
    proj_vram();
    port_printf("[3ds] pica: citro3d renderer, top screen format %d\n", scr_fmt);
    c3d_up = 1;
    { extern void (*port_tpatch_progress)(int, int); port_tpatch_progress = c3d_progress; }
    port_n3ds_main_thread();
    /* both screens black at once: until the game's first frame they showed what the memory held, the text of the
       language chooser read as another pixel format, for the seconds the disc and the game take to start */
    for (int k = 0; k < 2; k++) {
        C3D_FrameBegin(0);
        C3D_RenderTargetClear(top_rt, C3D_CLEAR_ALL, 0x000000FF, 0); C3D_FrameDrawOn(top_rt);
        if (bot_rt) { C3D_RenderTargetClear(bot_rt, C3D_CLEAR_ALL, 0x000000FF, 0); C3D_FrameDrawOn(bot_rt); }
        C3D_FrameEnd(0);
    }
    C3D_FrameBegin(0); C3D_FrameEnd(0);   /* (waits for them) */
    self_test();
    /* (the test picture is 1.5 MB written to the SD card, seconds of every start: only when gputest.txt asks for it) */
    { FILE *g = fopen("sdmc:/3ds/tombi/gputest.txt", "r"); if (g) { fclose(g); gpu_test_picture(); } }
    return 0;
}
/* a progress bar on the top screen (shims/tpatch.c port_tpatch_progress: the improved translation's files being made,
   minutes at the first start with it, the screens black otherwise) */
static void c3d_progress(int done, int total) {
    static int last = -1;
    if (!c3d_up || total <= 0) return;
    int w = 240 * done / total;
    if (done == last) return;
    last = done;
    end_and_wait();
    C3D_FrameBegin(0);
    C3D_RenderTargetClear(top_rt, C3D_CLEAR_ALL, 0x000000FF, 0);
    C3D_FrameDrawOn(top_rt);
    C3D_Mtx m; Mtx_OrthoTilt(&m, 0.0f, 400.0f, 240.0f, 0.0f, 1.0f, -1.0f, true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &m);
    C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
    set_env(0, 0); set_atest(AT_OFF); set_blend(-1);
    for (int k = 0; k < 2; k++) {   /* the whole bar dim, what is done bright */
        float X0 = 80, X1 = k ? 80 + w : 320, Y0 = 112, Y1 = 128, c = k ? 1.0f : 0.25f;
        if (X1 <= X0) continue;
        QUAD_BEGIN();
        vtx(X0, Y0, 0, 0, c, c, c, 1); vtx(X1, Y0, 0, 0, c, c, c, 1); vtx(X0, Y1, 0, 0, c, c, c, 1);
        vtx(X1, Y0, 0, 0, c, c, c, 1); vtx(X1, Y1, 0, 0, c, c, c, 1); vtx(X0, Y1, 0, 0, c, c, c, 1);
        QUAD_END();
    }
    {   /* what it is doing, in words (the debug menu's 5 x 7 letters, a quad a run of lit pixels; capitals, no accents) */
        extern const unsigned char *port_font5x7(int c);
        char count[40]; snprintf(count, sizeof count, "ARCHIVO %d DE %d", done, total);
        const struct { const char *t; int y, sc; float c; } line[4] = {
            { "PREPARANDO LA TRADUCCI\xD3N", 62, 2, 1.0f }, { "SOLO PASA LA PRIMERA VEZ. PUEDE TARDAR UNOS MINUTOS.", 90, 1, 0.8f },
            { count, 138, 1, 0.8f }, { "NO APAGUES LA CONSOLA", 156, 1, 0.6f } };
        QUAD_BEGIN();
        for (int l = 0; l < 4; l++) {
            const int sc = line[l].sc, len = (int)strlen(line[l].t); const float c = line[l].c;
            int x = 200 - len * 6 * sc / 2;
            for (const char *t = line[l].t; *t; t++, x += 6 * sc) {
                extern int port_font5x7_mark(int c);
                const unsigned char *g = port_font5x7((unsigned char)*t);
                const int mark = port_font5x7_mark((unsigned char)*t);
                for (int r = mark ? -2 : 0; r < 7; r++) for (int b = 0; b < 5; b++) {
                    const int row = r == -2 ? mark >> 5 : r == -1 ? mark & 31 : g[r];   /* (the mark: the two rows above) */
                    if (!(row >> (4 - b) & 1)) continue;
                    int e = b; while (e + 1 < 5 && (row >> (4 - (e + 1)) & 1)) e++;
                    float X0 = x + b * sc, X1 = x + (e + 1) * sc, Y0 = line[l].y + r * sc, Y1 = Y0 + sc;
                    vtx(X0, Y0, 0, 0, c, c, c, 1); vtx(X1, Y0, 0, 0, c, c, c, 1); vtx(X0, Y1, 0, 0, c, c, c, 1);
                    vtx(X1, Y0, 0, 0, c, c, c, 1); vtx(X1, Y1, 0, 0, c, c, c, 1); vtx(X0, Y1, 0, 0, c, c, c, 1);
                    b = e;
                }
            }
        }
        QUAD_END();
    }
    vb_flush();
    C3D_FrameEnd(0);
    proj_vram();
    vb_n = 0; last_ok = 0;
}
/* the way out (runtime/sdl.c): citro3d closed */
void port_n3ds_gpu_fini(void) {
    if (!c3d_up) return;
    c3d_up = 0;
    /* (nothing is submitted or waited for here: when the game is closed from the Home menu or with the power button
       the system has taken the GPU back, and the wait for the open frame never returned: a console froze at this
       step.  The frame begun and not ended holds nothing the GPU was given) */
    in_frame = 0;
    C3D_Fini();
}
const PicaDev pd_c3d = { c3d_start, c3d_tex_upload, c3d_tris, c3d_fill, c3d_read, c3d_write, c3d_present, c3d_vbuf, c3d_write15,
                         c3d_wide_start, c3d_wide_fill, c3d_wide_write, c3d_wide_down, c3d_wide_present, 0, c3d_submit,
                         c3d_eye_start, c3d_eye_fill };
#endif

/* ---- a worker thread for the panel's renders (runtime/panel.c): on another core where there is one (New 3DS: core 2;
   the Old 3DS's second core with the time the system grants), else under the main thread, in the time it sleeps */
static LightEvent wk_go;
static void (*wk_job)(void);
static volatile int wk_quit; static Thread wk_thread, c1_thread;
static void wk_main(void *arg) { (void)arg; for (;;) { LightEvent_Wait(&wk_go); if (wk_quit) return; wk_job(); } }
int port_n3ds_worker_start(void (*job)(void)) {
    LightEvent_Init(&wk_go, RESET_ONESHOT);
    wk_job = job;
    s32 prio = 0x30; svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    bool n3 = false; APT_CheckNew3DS(&n3);
    /* the system core's share: an Old 3DS's panel worker gets 30 % (more starved the system's services there: the SD
       card, gsp); a New 3DS has SDL's audio thread there and mixes 44100 samples a second in 50 % (Azahar) */
    APT_SetAppCpuTimeLimit(n3 ? 50 : 30);
    /* (an Old 3DS: on either core, -1: the second gives the program a third of its time, which the sound's mixer
       mostly takes (a render waited 100 to 250 ms for its 10), and the game's core is idle while the game waits for
       its next frame; below the game's thread, which takes that core back the moment it wakes) */
    static const int cores[] = { 2, -1, 1, -2 };
    Thread t = 0; int c = n3 ? 0 : 1, used = 0;
    for (; c < 4 && !t; c++) { if (n3 && cores[c] == -1) continue; t = threadCreate(wk_main, 0, 256 * 1024, prio + 1, cores[c], false); used = cores[c]; }
    wk_thread = t;
    port_printf("[3ds] panel render thread: %s (core %d)\n", t ? "started" : "none", used);
    return t != 0;
}
void port_n3ds_worker_go(void) { LightEvent_Signal(&wk_go); }

/* ---- for the rest of the port: the raw tick counter (profiling), and the audio thread's priority.  SDL's audio thread
   runs the mixer in its callback at the main thread's priority or below: on an Old 3DS, where the game leaves no idle
   time, it only got the scraps and produced 60 % of the samples.  The first callback raises its own thread above the
   main one (runtime/sdl.c audio_cb): the mixer's two milliseconds a frame are taken when the DSP needs them. */
unsigned long long port_ticks(void) { return svcGetSystemTick(); }
static s32 main_prio = -1;
void port_n3ds_main_thread(void) { svcGetThreadPriority(&main_prio, CUR_THREAD_HANDLE); }
void port_n3ds_audio_thread_boost(void) {
    static int done;
    if (done || main_prio < 0) return;
    done = 1;
    s32 mine = 0x30; svcGetThreadPriority(&mine, CUR_THREAD_HANDLE);
    Result r = mine >= main_prio ? svcSetThreadPriority(CUR_THREAD_HANDLE, main_prio > 0x19 ? main_prio - 1 : main_prio) : 0;
    port_printf("[3ds] audio thread: priority %ld, the main thread's %ld%s\n", (long)mine, (long)main_prio, mine >= main_prio ? (R_SUCCEEDED(r) ? ": raised above it" : ": could not be raised") : "");
}

/* a thread on the second core for the sound mixer of an Old 3DS (runtime/sdl.c), above the panel's worker */
static void (*c1_fn)(void);
static void c1_main(void *arg) { (void)arg; c1_fn(); }
int port_n3ds_thread_core1(void (*fn)(void)) {
    s32 prio = 0x30; svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    APT_SetAppCpuTimeLimit(30);   /* (the share the panel's worker has run with; 80 starved the system's services) */
    c1_fn = fn;
    Thread t = threadCreate(c1_main, 0, 64 * 1024, prio, 1, false);
    c1_thread = t;
    port_printf("[3ds] sound mixer thread on core 1: %s\n", t ? "started" : "none (the callback mixes)");
    return t != 0;
}
/* the way out (runtime/sdl.c, first of all): the two threads end and are waited for, two seconds each at most.  (A
   console crashed at every exit with them left running: one is on the system's core, which the system takes back
   when the program leaves, and both use what the steps after this one close.) */
void port_n3ds_threads_stop(void) {
    extern volatile int port_sound_mixer_quit;
    port_sound_mixer_quit = 1; wk_quit = 1;
    if (wk_thread) LightEvent_Signal(&wk_go);
    if (c1_thread) { Result r = threadJoin(c1_thread, 2000000000ULL); port_printf("[3ds] quit: mixer thread %s\n", R_SUCCEEDED(r) ? "ended" : "did not end"); if (R_SUCCEEDED(r)) threadFree(c1_thread); c1_thread = 0; }
    if (wk_thread) { Result r = threadJoin(wk_thread, 2000000000ULL); port_printf("[3ds] quit: panel thread %s\n", R_SUCCEEDED(r) ? "ended" : "did not end"); if (R_SUCCEEDED(r)) threadFree(wk_thread); wk_thread = 0; }
}
