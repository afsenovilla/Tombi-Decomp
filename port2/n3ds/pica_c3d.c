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
#include "port_runtime.h"
#include "gpu_pica.h"
#include "pica_vsh_shbin.h"

static C3D_Tex vram_tex, slot_tex[PICA_SLOTS];
static C3D_RenderTarget *vram_rt, *top_rt;
static shaderProgram_s prog;
static DVLB_s *vsh;
static int u_proj, in_frame, scr_fmt;
static unsigned frame_no, slot_used[PICA_SLOTS];
static unsigned *lin;                     /* 1024 x 512 linear words: read-backs */
#define ARENA_BYTES (2u << 20)
static unsigned char *arena; static unsigned arena_off;
static unsigned long long st_syncs, st_writes, st_frames;

/* ---- the frame */
static void begin(void) {   /* (C3D_FrameBegin waits for the frame submitted before: its memory is free then) */
    if (in_frame) return;
    C3D_FrameBegin(0);
    C3D_FrameDrawOn(vram_rt);
    in_frame = 1;
}
static void end_and_wait(void) {   /* run what was recorded and wait for it: memory may be reused afterwards */
    if (in_frame) { C3D_FrameEnd(GX_CMDLIST_FLUSH); in_frame = 0; }
    C3D_FrameBegin(0); C3D_FrameEnd(GX_CMDLIST_FLUSH);   /* (C3D_FrameBegin waits for the queue of the frame before) */
    arena_off = 0; frame_no++; st_syncs++;
}

/* ---- state */
static void proj_vram(void) {
    /* (the PICA writes clip y = +1 to memory row 0: the start-up self test reported FLIPPED with 0 .. 512 in Azahar) */
    C3D_Mtx m; Mtx_Ortho(&m, 0.0f, 1024.0f, 512.0f, 0.0f, 1.0f, -1.0f, true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &m);
}
static void set_env(int textured, int mod) {
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
    if (at == AT_NOTZERO) C3D_AlphaTest(true, GPU_NOTEQUAL, 0);
    else if (at == AT_OPAQUE) C3D_AlphaTest(true, GPU_EQUAL, PICA_A_OPAQUE);
    else if (at == AT_SEMI) C3D_AlphaTest(true, GPU_EQUAL, PICA_A_SEMI);
    else C3D_AlphaTest(false, GPU_ALWAYS, 0);
}
static void vtx(float x, float y, float s, float t, float r, float g, float b, float a) {
    C3D_ImmSendAttrib(x, y, 0.5f, 0.0f);
    C3D_ImmSendAttrib(s, t, 0.0f, 0.0f);
    C3D_ImmSendAttrib(r, g, b, a);
}

/* ---- textures: RGBA8 words (red low) -> the PICA's texel word (red high), 8x8 tiles in Morton order */
static inline unsigned pix(unsigned c) { return (c << 24) | ((c << 8) & 0xFF0000u) | ((c >> 8) & 0xFF00u) | (c >> 24); }
static inline unsigned morton(int x, int y) {
    return (x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3);
}
static inline unsigned tiled(int x, int y, int w) { return ((y >> 3) * (w >> 3) + (x >> 3)) * 64 + morton(x & 7, y & 7); }
static void c3d_tex_upload(int slot, const unsigned *rgba) {
    begin();                                                  /* the frames before have finished */
    if (slot_used[slot] == frame_no) { end_and_wait(); begin(); }   /* the open frame samples this slot */
    unsigned *d = slot_tex[slot].data;
    for (int v = 0; v < 256; v++) for (int u = 0; u < 256; u++) d[tiled(u, v, 256)] = pix(rgba[v * 256 + u]);
    GSPGPU_FlushDataCache(d, 256 * 256 * 4);
}

/* ---- primitives */
static void c3d_tris(const PState *s, const PV *v, int n) {
    begin();
    /* the scissor counts rows from the other edge than the projection (the self test: an area of y 256..511 drew nothing) */
    C3D_SetScissor(GPU_SCISSOR_NORMAL, s->x1, 511 - s->y2, s->x2 + 1, 512 - s->y1);
    set_env(s->tex >= 0, s->mod);
    set_atest(s->atest);
    set_blend(s->blend);
    if (s->tex >= 0) { C3D_TexBind(0, &slot_tex[s->tex]); slot_used[s->tex] = frame_no; }
    C3D_ImmDrawBegin(GPU_TRIANGLES);
    for (int i = 0; i < n; i++) {
        /* +0.5: the PICA samples pixel centres, the PS1 integer positions */
        vtx(v[i].x + 0.5f, v[i].y + 0.5f, v[i].u / 256.0f, 1.0f - v[i].v / 256.0f, v[i].r / 255.0f, v[i].g / 255.0f, v[i].b / 255.0f,
            s->tex >= 0 ? 1.0f : 0.0f);   /* (untextured: alpha 0, bit 15 clear on a read-back) */
    }
    C3D_ImmDrawEnd();
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
    for (int j0 = 0; j0 < h; j0 += 256)   /* (in pieces of at most 256 x 256: the arena) */
        for (int i0 = 0; i0 < w; i0 += 256) {
            int pw = w - i0 < 256 ? w - i0 : 256, ph = h - j0 < 256 ? h - j0 : 256;
            C3D_Tex *tp = &stg;
            if (stage_tex(tp, pw, ph)) continue;
            C3D_Tex t = *tp;
            unsigned *d = t.data;
            for (int j = 0; j < ph; j++) for (int i = 0; i < pw; i++) d[tiled(i, j, t.width)] = pix(src[(j0 + j) * stride + i0 + i]);
            GSPGPU_FlushDataCache(d, (unsigned)t.width * t.height * 4);
            begin();
            C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
            set_env(1, 0); set_atest(AT_OFF); set_blend(-1);
            C3D_TexBind(0, tp);
            float X0 = x + i0, Y0 = y + j0, X1 = X0 + pw, Y1 = Y0 + ph, S1 = (float)pw / t.width, T0 = 1.0f, T1 = 1.0f - (float)ph / t.height;
            C3D_ImmDrawBegin(GPU_TRIANGLES);
            vtx(X0, Y0, 0, T0, 1, 1, 1, 1); vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X0, Y1, 0, T1, 1, 1, 1, 1);
            vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(X0, Y1, 0, T1, 1, 1, 1, 1);
            C3D_ImmDrawEnd();
            st_writes++;
        }
}
static void c3d_fill(int x, int y, int w, int h, unsigned c) {
    begin();
    C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
    set_env(0, 0); set_atest(AT_OFF); set_blend(-1);
    float r = (c & 255) / 255.0f, g = ((c >> 8) & 255) / 255.0f, b = ((c >> 16) & 255) / 255.0f, a = (c >> 24) / 255.0f;
    float X0 = x, Y0 = y, X1 = x + w, Y1 = y + h;
    C3D_ImmDrawBegin(GPU_TRIANGLES);
    vtx(X0, Y0, 0, 0, r, g, b, a); vtx(X1, Y0, 0, 0, r, g, b, a); vtx(X0, Y1, 0, 0, r, g, b, a);
    vtx(X1, Y0, 0, 0, r, g, b, a); vtx(X1, Y1, 0, 0, r, g, b, a); vtx(X0, Y1, 0, 0, r, g, b, a);
    C3D_ImmDrawEnd();
}
static void c3d_read(int x, int y, int w, int h, unsigned *dst, int stride) {
    end_and_wait();
    C3D_SyncDisplayTransfer((u32 *)vram_tex.data, GX_BUFFER_DIM(1024, 512), (u32 *)lin, GX_BUFFER_DIM(1024, 512),
                            GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
                            GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) |
                            GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
    GSPGPU_InvalidateDataCache(lin, 1024 * 512 * 4);
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) dst[j * stride + i] = pix(lin[(y + j) * 1024 + x + i]);
}

/* ---- the top screen: the display area, 4:3 at the screen's height, centred */
static int c3d_present(int x, int y, int w, int h, int masked) {
    begin();
    C3D_RenderTargetClear(top_rt, C3D_CLEAR_ALL, 0x000000FF, 0);
    C3D_FrameDrawOn(top_rt);
    C3D_Mtx m; Mtx_OrthoTilt(&m, 0.0f, 400.0f, 240.0f, 0.0f, 1.0f, -1.0f, true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, u_proj, &m);
    C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
    if (!masked && w > 0 && h > 0) {
        set_env(1, 0); set_atest(AT_OFF); set_blend(-1);
        C3D_TexSetFilter(&vram_tex, GPU_LINEAR, GPU_LINEAR);
        C3D_TexBind(0, &vram_tex);
        float X0 = 40, X1 = 360, Y0 = 0, Y1 = 240;   /* 320 x 240 */
        float S0 = x / 1024.0f, S1 = (x + w) / 1024.0f, T0 = y / 512.0f, T1 = (y + h) / 512.0f;   /* (a render target: t = y / 512, seen in Azahar) */
        C3D_ImmDrawBegin(GPU_TRIANGLES);
        vtx(X0, Y0, S0, T0, 1, 1, 1, 1); vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
        vtx(X1, Y0, S1, T0, 1, 1, 1, 1); vtx(X1, Y1, S1, T1, 1, 1, 1, 1); vtx(X0, Y1, S0, T1, 1, 1, 1, 1);
        C3D_ImmDrawEnd();
    }
    C3D_FrameEnd(GX_CMDLIST_FLUSH);
    in_frame = 0;
    C3D_TexSetFilter(&vram_tex, GPU_NEAREST, GPU_NEAREST);
    proj_vram();
    frame_no++; st_frames++;
    arena_off = 0;   /* (the next C3D_FrameBegin waits for this frame: the arena is free then) */
    if (st_frames % 600 == 0)
        port_printf("[3ds] pica: %llu frames, %llu syncs, %llu host writes\n", st_frames, st_syncs, st_writes);
    return 1;
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
    for (int i = 0; i < PICA_SLOTS; i++) {
        if (!C3D_TexInit(&slot_tex[i], 256, 256, GPU_RGBA8)) { port_printf("[3ds] pica: texture slot %d failed\n", i); return -1; }
        C3D_TexSetFilter(&slot_tex[i], GPU_NEAREST, GPU_NEAREST);
        C3D_TexSetWrap(&slot_tex[i], GPU_REPEAT, GPU_REPEAT);
    }
    lin = linearAlloc(1024 * 512 * 4);
    arena = linearAlloc(ARENA_BYTES);
    if (!lin || !arena) { port_printf("[3ds] pica: no linear memory\n"); return -1; }
    vsh = DVLB_ParseFile((u32 *)pica_vsh_shbin, pica_vsh_shbin_size);
    shaderProgramInit(&prog);
    shaderProgramSetVsh(&prog, &vsh->DVLE[0]);
    C3D_BindProgram(&prog);
    u_proj = shaderInstanceGetUniformLocation(prog.vertexShader, "projection");
    C3D_AttrInfo *ai = C3D_GetAttrInfo();
    AttrInfo_Init(ai);
    AttrInfo_AddLoader(ai, 0, GPU_FLOAT, 3);   /* v0 position */
    AttrInfo_AddLoader(ai, 1, GPU_FLOAT, 2);   /* v1 texcoord */
    AttrInfo_AddLoader(ai, 2, GPU_FLOAT, 4);   /* v2 colour */
    C3D_DepthTest(false, GPU_ALWAYS, GPU_WRITE_COLOR);
    C3D_CullFace(GPU_CULL_NONE);
    proj_vram();
    port_printf("[3ds] pica: citro3d renderer, top screen format %d\n", scr_fmt);
    self_test();
    return 0;
}
const PicaDev pd_c3d = { c3d_start, c3d_tex_upload, c3d_tris, c3d_fill, c3d_read, c3d_write, c3d_present };
#endif
