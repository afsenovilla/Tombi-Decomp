/* OpenGL backend of the PS1 GPU (gpu_backend.h; docs/PORT.md "M3: the OpenGL renderer").
 *
 * VRAM is one 1024x512 RGBA8 texture, also the colour attachment of a framebuffer object, so every primitive the
 * decoder of gpu.c hands over is drawn by the GPU of the host into VRAM itself: the 5-bit channels of a PS1 word
 * are the texture's 8-bit channels (k -> k*255/31), the mask bit is its alpha.  A fragment shader samples the
 * 4/8-bit texture pages and their CLUTs, or the 15-bit pages, from that same texture with PS1 semantics (texture
 * window, 0x0000 = transparent, modulation, 15-bit quantisation); the four semi-transparency modes are the GL blend
 * modes, textured semi-transparent primitives go in two passes (opaque texels, then the texels with bit 15 set).
 * Consecutive primitives with the same state (blend mode, texture window, drawing area, mask bits, textured or
 * not) share one draw call; the texture page and CLUT are vertex attributes, so they do not break a batch.
 *
 * The CPU mirror vram[] of gpu.c stays in step through two hooks: a rectangle the host writes (LoadImage, CLUTs,
 * the texture patches) is uploaded with glTexSubImage2D right away, a rectangle the host reads (StoreImage, the
 * frame readout for the PNG dumps, --dump-vram) is read back with glReadPixels first.  MoveImage and the GP0 copy
 * are glCopyTexSubImage2D, fills are scissored clears.  The window shows the display area with a textured quad,
 * integer-scaled, vsync through SDL_GL_SetSwapInterval.  Only GL 2.1-level features are used (shaders, FBO, VBO,
 * separate blend functions); the functions are loaded with SDL_GL_GetProcAddress (no GLEW, no link-time GL
 * dependency on Linux; opengl32 on Windows).  The differences from the software rasteriser are listed in
 * docs/PORT.md ("what is approximated"). */
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "port_runtime.h"
#include "gpu_backend.h"

typedef gpu_u8 u8; typedef gpu_u16 u16; typedef gpu_u32 u32;

/* ------------------------------------------------------------------ GL entry points (all through SDL_GL_GetProcAddress) */
#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER 0x8D40
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#define GL_STREAM_DRAW 0x88E0
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_CONSTANT_COLOR
#define GL_CONSTANT_COLOR 0x8001
#endif
#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#endif
#ifndef GL_UNSIGNED_SHORT_1_5_5_5_REV
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#endif
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif

#define GL_FUNCS(X) \
    X(GLenum, glGetError, (void)) \
    X(const GLubyte *, glGetString, (GLenum)) \
    X(void, glGetIntegerv, (GLenum, GLint *)) \
    X(void, glEnable, (GLenum)) \
    X(void, glDisable, (GLenum)) \
    X(void, glViewport, (GLint, GLint, GLsizei, GLsizei)) \
    X(void, glScissor, (GLint, GLint, GLsizei, GLsizei)) \
    X(void, glClearColor, (GLfloat, GLfloat, GLfloat, GLfloat)) \
    X(void, glClear, (GLbitfield)) \
    X(void, glColorMask, (GLboolean, GLboolean, GLboolean, GLboolean)) \
    X(void, glBlendFuncSeparate, (GLenum, GLenum, GLenum, GLenum)) \
    X(void, glBlendEquationSeparate, (GLenum, GLenum)) \
    X(void, glBlendColor, (GLfloat, GLfloat, GLfloat, GLfloat)) \
    X(void, glGenTextures, (GLsizei, GLuint *)) \
    X(void, glBindTexture, (GLenum, GLuint)) \
    X(void, glTexParameteri, (GLenum, GLenum, GLint)) \
    X(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *)) \
    X(void, glTexSubImage2D, (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *)) \
    X(void, glCopyTexSubImage2D, (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei)) \
    X(void, glPixelStorei, (GLenum, GLint)) \
    X(void, glReadPixels, (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *)) \
    X(void, glDrawArrays, (GLenum, GLint, GLsizei)) \
    X(void, glFlush, (void)) \
    X(void, glGenFramebuffers, (GLsizei, GLuint *)) \
    X(void, glBindFramebuffer, (GLenum, GLuint)) \
    X(void, glFramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint)) \
    X(GLenum, glCheckFramebufferStatus, (GLenum)) \
    X(void, glGenBuffers, (GLsizei, GLuint *)) \
    X(void, glBindBuffer, (GLenum, GLuint)) \
    X(void, glBufferData, (GLenum, GLsizeiptr, const void *, GLenum)) \
    X(GLuint, glCreateShader, (GLenum)) \
    X(void, glShaderSource, (GLuint, GLsizei, const GLchar *const *, const GLint *)) \
    X(void, glCompileShader, (GLuint)) \
    X(void, glGetShaderiv, (GLuint, GLenum, GLint *)) \
    X(void, glGetShaderInfoLog, (GLuint, GLsizei, GLsizei *, GLchar *)) \
    X(GLuint, glCreateProgram, (void)) \
    X(void, glAttachShader, (GLuint, GLuint)) \
    X(void, glBindAttribLocation, (GLuint, GLuint, const GLchar *)) \
    X(void, glLinkProgram, (GLuint)) \
    X(void, glGetProgramiv, (GLuint, GLenum, GLint *)) \
    X(void, glGetProgramInfoLog, (GLuint, GLsizei, GLsizei *, GLchar *)) \
    X(void, glUseProgram, (GLuint)) \
    X(GLint, glGetUniformLocation, (GLuint, const GLchar *)) \
    X(void, glUniform1i, (GLint, GLint)) \
    X(void, glUniform1f, (GLint, GLfloat)) \
    X(void, glUniform4f, (GLint, GLfloat, GLfloat, GLfloat, GLfloat)) \
    X(void, glEnableVertexAttribArray, (GLuint)) \
    X(void, glDisableVertexAttribArray, (GLuint)) \
    X(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void *))

#define GLF_DECL(ret, name, args) static ret (APIENTRY *p_##name) args;
GL_FUNCS(GLF_DECL)
static void (APIENTRY *p_glTextureBarrier)(void);     /* optional: ARB/NV_texture_barrier */
#define GLF_LOAD(ret, name, args) \
    p_##name = (ret (APIENTRY *) args)SDL_GL_GetProcAddress(#name); \
    if (!p_##name) p_##name = (ret (APIENTRY *) args)SDL_GL_GetProcAddress(#name "EXT"); \
    if (!p_##name) p_##name = (ret (APIENTRY *) args)SDL_GL_GetProcAddress(#name "ARB"); \
    if (!p_##name) { port_printf("[gpu] OpenGL: no %s\n", #name); return -1; }
static int gl_load(void) {
    GL_FUNCS(GLF_LOAD)
    p_glTextureBarrier = (void (APIENTRY *)(void))SDL_GL_GetProcAddress("glTextureBarrier");
    if (!p_glTextureBarrier) p_glTextureBarrier = (void (APIENTRY *)(void))SDL_GL_GetProcAddress("glTextureBarrierNV");
    return 0;
}

/* ------------------------------------------------------------------ shaders */
/* a_pos: VRAM pixels (continuous; polygons are shifted by half a pixel so the GL sample points are the PS1's);
   a_col: 8-bit colour; a_uv: texels (polygons: rounded to nearest, rects: already centred); a_tex: CLUT x/y,
   texture page x + depth * 1024, texture page y */
static const char vs_src[] =
    "attribute vec2 a_pos; attribute vec4 a_col; attribute vec2 a_uv; attribute vec4 a_tex;\n"
    "varying vec4 v_col; varying vec2 v_uv; varying vec4 v_tex;\n"
    "void main() {\n"
    "  gl_Position = vec4(a_pos.x / 512.0 - 1.0, a_pos.y / 256.0 - 1.0, 0.0, 1.0);\n"
    "  v_col = a_col; v_uv = a_uv; v_tex = a_tex;\n"
    "}\n";
/* u_tex: 0 flat/gouraud, 1 textured + modulated, 2 textured raw; u_pass: 0 all texels, 1 only the opaque ones
   (bit 15 clear), 2 only the semi-transparent ones; u_tw: texture window mask x, offset x, mask y, offset y in
   texels (the offset already masked); u_setmask: the mask bit to write.  All integer arithmetic is on floats
   (exact below 2^24): a VRAM word is rebuilt from the 8-bit channels and taken apart with floor/mod. */
static const char fs_src[] =
    "uniform sampler2D u_vram; uniform int u_tex; uniform int u_pass; uniform vec4 u_tw; uniform float u_setmask;\n"
    "varying vec4 v_col; varying vec2 v_uv; varying vec4 v_tex;\n"
    "float vword(float x, float y) {\n"
    "  vec4 t = texture2D(u_vram, vec2((x + 0.5) / 1024.0, (y + 0.5) / 512.0));\n"
    "  return floor(t.r * 31.0 + 0.5) + floor(t.g * 31.0 + 0.5) * 32.0 + floor(t.b * 31.0 + 0.5) * 1024.0 + floor(t.a + 0.5) * 32768.0;\n"
    "}\n"
    "float twin(float u, float mask, float off) {\n"          /* (u & ~mask) | off, bit by bit over the 5 window bits */
    "  if (mask < 1.0) return u;\n"
    "  float r = mod(u, 8.0), b = 8.0;\n"
    "  for (int i = 0; i < 5; i++) { float mb = mod(floor(mask / b), 2.0), ub = mod(floor(u / b), 2.0); r += (1.0 - mb) * ub * b; b *= 2.0; }\n"
    "  return r + off;\n"
    "}\n"
    "void main() {\n"
    "  vec3 c = floor(v_col.rgb * 255.0 + 0.5);\n"
    "  float a = u_setmask; vec3 o;\n"
    "  if (u_tex == 0) o = floor(c / 8.0);\n"
    "  else {\n"
    "    vec4 tp = floor(v_tex + 0.5);\n"
    "    float u = twin(mod(floor(v_uv.x + 0.5), 256.0), u_tw.x, u_tw.y), v = twin(mod(floor(v_uv.y + 0.5), 256.0), u_tw.z, u_tw.w);\n"
    "    float depth = floor(tp.z / 1024.0), tx = tp.z - depth * 1024.0, ty = tp.w, w;\n"
    "    if (depth < 0.5) {\n"
    "      w = vword(mod(tx + floor(u / 4.0), 1024.0), mod(ty + v, 512.0));\n"
    "      float sh = mod(u, 4.0), dv = sh < 0.5 ? 1.0 : sh < 1.5 ? 16.0 : sh < 2.5 ? 256.0 : 4096.0;\n"
    "      float idx = mod(floor(w / dv), 16.0);\n"
    "      w = vword(mod(tp.x + idx, 1024.0), tp.y);\n"
    "    } else if (depth < 1.5) {\n"
    "      w = vword(mod(tx + floor(u / 2.0), 1024.0), mod(ty + v, 512.0));\n"
    "      float idx = mod(floor(w / (mod(u, 2.0) < 0.5 ? 1.0 : 256.0)), 256.0);\n"
    "      w = vword(mod(tp.x + idx, 1024.0), tp.y);\n"
    "    } else w = vword(mod(tx + u, 1024.0), mod(ty + v, 512.0));\n"
    "    if (w < 0.5) discard;\n"
    "    float ta = floor(w / 32768.0);\n"
    "    if (u_pass == 1 && ta > 0.5) discard;\n"
    "    if (u_pass == 2 && ta < 0.5) discard;\n"
    "    o = vec3(mod(w, 32.0), mod(floor(w / 32.0), 32.0), mod(floor(w / 1024.0), 32.0));\n"
    "    if (u_tex == 1) o = min(floor(o * c / 128.0), 31.0);\n"
    "    a = max(a, ta);\n"
    "  }\n"
    "  gl_FragColor = vec4(o / 31.0, a);\n"
    "}\n";
/* the window blit: the display area of VRAM onto the window */
static const char bvs_src[] =
    "attribute vec2 a_pos; attribute vec2 a_uv; varying vec2 v_uv;\n"
    "void main() { gl_Position = vec4(a_pos, 0.0, 1.0); v_uv = a_uv; }\n";
static const char bfs_src[] =
    "uniform sampler2D u_vram; varying vec2 v_uv;\n"
    "void main() { gl_FragColor = vec4(texture2D(u_vram, v_uv).rgb, 1.0); }\n";

static GLuint compile(GLenum kind, const char *src, const char *what) {
    const char *parts[2]; int es = 0;
    { int profile = 0; SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile); es = profile == SDL_GL_CONTEXT_PROFILE_ES; }
    parts[0] = es ? "#version 100\nprecision highp float;\nprecision highp int;\n" : "#version 110\n";
    parts[1] = src;
    GLuint s = p_glCreateShader(kind);
    p_glShaderSource(s, 2, parts, 0);
    p_glCompileShader(s);
    GLint ok = 0; p_glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) { char log[2048]; GLsizei n = 0; p_glGetShaderInfoLog(s, sizeof log, &n, log); log[n < 2047 ? n : 2047] = 0; port_printf("[gpu] OpenGL: %s shader: %s\n", what, log); return 0; }
    return s;
}
static GLuint link(const char *vs, const char *fs, const char *what) {
    GLuint v = compile(GL_VERTEX_SHADER, vs, what), f = compile(GL_FRAGMENT_SHADER, fs, what);
    if (!v || !f) return 0;
    GLuint p = p_glCreateProgram();
    p_glAttachShader(p, v); p_glAttachShader(p, f);
    p_glBindAttribLocation(p, 0, "a_pos"); p_glBindAttribLocation(p, 1, "a_col"); p_glBindAttribLocation(p, 2, "a_uv"); p_glBindAttribLocation(p, 3, "a_tex");
    p_glLinkProgram(p);
    GLint ok = 0; p_glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) { char log[2048]; GLsizei n = 0; p_glGetProgramInfoLog(p, sizeof log, &n, log); log[n < 2047 ? n : 2047] = 0; port_printf("[gpu] OpenGL: %s program: %s\n", what, log); return 0; }
    return p;
}

/* ------------------------------------------------------------------ the command list */
typedef struct { float x, y; u8 r, g, b, a; float u, v; u16 cx, cy, tx, ty; } GVtx;     /* 28 bytes */
typedef struct { int semi, abr, tex, raw, twmx, twmy, twox, twoy, x1, y1, x2, y2, set_mask, check_mask, prim; } Key;
enum { OP_DRAW, OP_FILL, OP_COPY };
#define MAX_SAMPLED 12
typedef struct { int kind; Key key; int first, count; int r[6]; u16 col; int ns; short sr[MAX_SAMPLED][4]; } Op;   /* sr: VRAM rectangles the batch samples */

#define MAX_VTX 65536
#define MAX_OPS 8192
static GVtx vtx[MAX_VTX];
static Op ops[MAX_OPS];
static int n_vtx, n_ops;
static GLuint vram_tex, fbo, vbo, prog, bprog;
static GLint u_tex, u_pass, u_tw, u_setmask, bu_vram;
static int gl_ready, check_mask_semi_warned;
/* the VRAM rectangle drawn since the last texture barrier: a batch that samples inside it needs one first (the GL
   leaves sampling a texture that is also the render target undefined; the PS1 reads what was drawn before) */
static int dirty_any, dirty_x1, dirty_y1, dirty_x2, dirty_y2;
static unsigned long long stat_barriers;
static void dirty_add(int x1, int y1, int x2, int y2) {
    if (!dirty_any) { dirty_x1 = x1; dirty_y1 = y1; dirty_x2 = x2; dirty_y2 = y2; dirty_any = 1; return; }
    if (x1 < dirty_x1) dirty_x1 = x1;
    if (y1 < dirty_y1) dirty_y1 = y1;
    if (x2 > dirty_x2) dirty_x2 = x2;
    if (y2 > dirty_y2) dirty_y2 = y2;
}
static int dirty_hits(int x1, int y1, int x2, int y2) { return dirty_any && x1 <= dirty_x2 && x2 >= dirty_x1 && y1 <= dirty_y2 && y2 >= dirty_y1; }
static void barrier_if(int x1, int y1, int x2, int y2) {
    if (!dirty_hits(x1, y1, x2, y2)) return;
    if (port_verbose && stat_barriers < 40)
        port_printf("[gpu] texture barrier (frame %d): drawn %d,%d-%d,%d sampled %d,%d-%d,%d\n", port_frame, dirty_x1, dirty_y1, dirty_x2, dirty_y2, x1, y1, x2, y2);
    if (p_glTextureBarrier) p_glTextureBarrier();
    dirty_any = 0; stat_barriers++;
}
/* the VRAM rectangles a primitive with this texture page / CLUT can sample (the page is 256 rows, its width in
   words depends on the depth; the CLUT 16 or 256 words), kept as a short list per batch (a union would span the
   frame buffers between pages above and CLUTs below them); wrap-around at the VRAM edge is clamped */
static void op_sample_rect(Op *op, int x1, int y1, int x2, int y2) {
    if (x2 > 1023) x2 = 1023;
    if (y2 > 511) y2 = 511;
    for (int i = 0; i < op->ns; i++)
        if (op->sr[i][0] == x1 && op->sr[i][1] == y1 && op->sr[i][2] == x2 && op->sr[i][3] == y2) return;
    if (op->ns < MAX_SAMPLED) { op->sr[op->ns][0] = (short)x1; op->sr[op->ns][1] = (short)y1; op->sr[op->ns][2] = (short)x2; op->sr[op->ns][3] = (short)y2; op->ns++; return; }
    short *r = op->sr[MAX_SAMPLED - 1];     /* full: the last entry becomes a union */
    if (x1 < r[0]) r[0] = (short)x1;
    if (y1 < r[1]) r[1] = (short)y1;
    if (x2 > r[2]) r[2] = (short)x2;
    if (y2 > r[3]) r[3] = (short)y2;
}
static void op_sample(Op *op, const GpuTex *t) {
    int pw = t->depth == 0 ? 64 : t->depth == 1 ? 128 : 256;
    op_sample_rect(op, t->tx, t->ty, t->tx + pw - 1, t->ty + 255);
    if (t->depth < 2) op_sample_rect(op, t->cx, t->cy, t->cx + (t->depth == 0 ? 16 : 256) - 1, t->cy);
}
static u16 stage[1024 * 512];
static u8 stage8[1024 * 512 * 4];
static unsigned long long stat_draws, stat_ops;

static const GpuState *gs = &port_gs;
static int same_key(const Key *a, const Key *b) { return memcmp(a, b, sizeof *a) == 0; }
static void make_key(Key *k, int semi, int tex, int raw, int prim) {
    memset(k, 0, sizeof *k);
    k->semi = semi ? 1 : 0; k->abr = semi ? gs->abr : 0; k->tex = tex; k->raw = tex ? (raw ? 1 : 0) : 0;
    if (tex) { k->twmx = gs->twmx; k->twmy = gs->twmy; k->twox = gs->twox; k->twoy = gs->twoy; }
    k->x1 = gs->x1; k->y1 = gs->y1; k->x2 = gs->x2; k->y2 = gs->y2;
    k->set_mask = gs->set_mask; k->check_mask = gs->check_mask; k->prim = prim;
}
static void submit(void);
static GVtx *alloc_vtx(int n, const Key *k, const GpuTex *t) {
    if (n_vtx + n > MAX_VTX || n_ops >= MAX_OPS) submit();
    Op *op = n_ops ? &ops[n_ops - 1] : 0;
    if (!op || op->kind != OP_DRAW || !same_key(&op->key, k)) {
        op = &ops[n_ops++]; op->kind = OP_DRAW; op->key = *k; op->first = n_vtx; op->count = 0; op->ns = 0;
    }
    if (t) op_sample(op, t);
    op->count += n;
    GVtx *v = &vtx[n_vtx]; n_vtx += n;
    return v;
}
static void set_vtx(GVtx *o, float x, float y, int r, int g, int b, float u, float v, const GpuTex *t) {
    o->x = x; o->y = y; o->r = (u8)r; o->g = (u8)g; o->b = (u8)b; o->a = 255; o->u = u; o->v = v;
    if (t) { o->cx = (u16)t->cx; o->cy = (u16)t->cy; o->tx = (u16)(t->tx + t->depth * 1024); o->ty = (u16)t->ty; }
    else o->cx = o->cy = o->tx = o->ty = 0;
}
/* PS1 sample points are the integer pixel positions; GL samples pixel centres, so polygons move by +0.5 and by
   -1/64 (both axes) so that a centre exactly on an edge follows the PS1's top-left rule whatever the driver's own
   tie rule is (a top or left edge includes it, a bottom or right one does not) */
#define POLY_SHIFT (0.5f - 1.0f / 64.0f)

static void gl_tri(const GpuVtx *a, const GpuVtx *b, const GpuVtx *c, int shade, const GpuTex *tex, int semi, int raw) {
    Key k; (void)shade;
    if (!gl_ready) return;
    make_key(&k, semi, tex ? (raw ? 2 : 1) : 0, raw, GL_TRIANGLES);
    GVtx *v = alloc_vtx(3, &k, tex);
    set_vtx(v + 0, a->x + POLY_SHIFT, a->y + POLY_SHIFT, a->r, a->g, a->b, (float)a->u, (float)a->v, tex);
    set_vtx(v + 1, b->x + POLY_SHIFT, b->y + POLY_SHIFT, b->r, b->g, b->b, (float)b->u, (float)b->v, tex);
    set_vtx(v + 2, c->x + POLY_SHIFT, c->y + POLY_SHIFT, c->r, c->g, c->b, (float)c->u, (float)c->v, tex);
}
static void gl_rect(int x, int y, int w, int h, u32 col, const GpuTex *tex, int u0, int v0, int semi, int raw) {
    Key k;
    if (!gl_ready) return;
    make_key(&k, semi, tex ? (raw ? 2 : 1) : 0, raw, GL_TRIANGLES);
    int r = col & 255, g = (col >> 8) & 255, b = (col >> 16) & 255;
    /* texel (u0 + i, v0 + j) for pixel (x + i, y + j): the uv at the pixel centres are the integers themselves */
    float ua = u0 - 0.5f, ub = u0 + w - 0.5f, va = v0 - 0.5f, vb = v0 + h - 0.5f;
    float xa = (float)x, xb = (float)(x + w), ya = (float)y, yb = (float)(y + h);
    GVtx *v = alloc_vtx(6, &k, tex);
    set_vtx(v + 0, xa, ya, r, g, b, ua, va, tex); set_vtx(v + 1, xb, ya, r, g, b, ub, va, tex); set_vtx(v + 2, xa, yb, r, g, b, ua, vb, tex);
    set_vtx(v + 3, xb, ya, r, g, b, ub, va, tex); set_vtx(v + 4, xb, yb, r, g, b, ub, vb, tex); set_vtx(v + 5, xa, yb, r, g, b, ua, vb, tex);
}
static void gl_line(int x0, int y0, u32 c0, int x1, int y1, u32 c1, int shade, int semi) {
    Key k;
    if (!gl_ready) return;
    if (!shade) c1 = c0;
    make_key(&k, semi, 0, 0, GL_LINES);
    GVtx *v = alloc_vtx(2, &k, 0);
    /* a GL line leaves out its last pixel (diamond exit); the PS1 draws both ends: extend by half a pixel */
    float dx = (float)(x1 - x0), dy = (float)(y1 - y0), len = (float)SDL_sqrt(dx * dx + dy * dy);
    float ex = len > 0 ? dx / len * 0.5f : 0, ey = len > 0 ? dy / len * 0.5f : 0;
    set_vtx(v + 0, x0 + 0.5f, y0 + 0.5f, c0 & 255, (c0 >> 8) & 255, (c0 >> 16) & 255, 0, 0, 0);
    set_vtx(v + 1, x1 + 0.5f + ex, y1 + 0.5f + ey, c1 & 255, (c1 >> 8) & 255, (c1 >> 16) & 255, 0, 0, 0);
    if (len == 0) { v[1].x += 0.75f; }        /* a single point: a short horizontal line */
}
static void gl_fill(int x, int y, int w, int h, u16 c) {
    if (!gl_ready) return;
    if (n_ops >= MAX_OPS) submit();
    Op *op = &ops[n_ops++]; op->kind = OP_FILL; op->r[0] = x; op->r[1] = y; op->r[2] = w; op->r[3] = h; op->col = c;
}
static void gl_copy(int sx, int sy, int dx, int dy, int w, int h) {
    if (!gl_ready) return;
    if (n_ops >= MAX_OPS) submit();
    Op *op = &ops[n_ops++]; op->kind = OP_COPY; op->r[0] = sx; op->r[1] = sy; op->r[2] = dx; op->r[3] = dy; op->r[4] = w; op->r[5] = h;
}

/* ------------------------------------------------------------------ submitting the list */
static void bind_vram_fbo(void) {
    p_glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    p_glViewport(0, 0, 1024, 512);
}
static void set_blend(const Key *k, int pass) {
    if (k->semi && pass != 1) {
        p_glEnable(GL_BLEND);
        p_glBlendEquationSeparate(k->abr == 2 ? GL_FUNC_REVERSE_SUBTRACT : GL_FUNC_ADD, GL_FUNC_ADD);
        switch (k->abr) {
        case 0: p_glBlendColor(0.5f, 0.5f, 0.5f, 0); p_glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR, GL_ONE, GL_ZERO); break;
        case 1: case 2: p_glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO); break;
        default: p_glBlendColor(0.25f, 0.25f, 0.25f, 0); p_glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE, GL_ONE, GL_ZERO); break;
        }
        if (k->check_mask && !check_mask_semi_warned++) port_printf("[gpu] OpenGL: mask check on semi-transparent primitives is not emulated\n");
    } else if (k->check_mask) {
        /* mask check: a destination pixel with bit 15 set keeps its value (exact through destination alpha) */
        p_glEnable(GL_BLEND);
        p_glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        p_glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
    } else p_glDisable(GL_BLEND);
}
static void draw_op(const Op *op, int pass) {
    const Key *k = &op->key;
    int w = k->x2 - k->x1 + 1, h = k->y2 - k->y1 + 1;
    if (w <= 0 || h <= 0) return;
    p_glScissor(k->x1, k->y1, w, h);
    set_blend(k, pass);
    p_glUniform1i(u_tex, k->tex);
    p_glUniform1i(u_pass, pass);
    p_glUniform4f(u_tw, (float)(k->twmx * 8), (float)((k->twox & k->twmx) * 8), (float)(k->twmy * 8), (float)((k->twoy & k->twmy) * 8));
    p_glUniform1f(u_setmask, k->set_mask ? 1.0f : 0.0f);
    for (int i = 0; i < op->ns && dirty_any; i++) barrier_if(op->sr[i][0], op->sr[i][1], op->sr[i][2], op->sr[i][3]);
    p_glDrawArrays(k->prim, op->first, op->count);
    dirty_add(k->x1, k->y1, k->x2, k->y2);
    stat_draws++;
}
static void submit(void) {
    if (!gl_ready || !n_ops) { n_vtx = n_ops = 0; return; }
    bind_vram_fbo();
    p_glEnable(GL_SCISSOR_TEST);
    p_glUseProgram(prog);
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
    p_glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if (n_vtx) p_glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(n_vtx * sizeof(GVtx)), vtx, GL_STREAM_DRAW);
    p_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GVtx), (const void *)0);
    p_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GVtx), (const void *)8);
    p_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GVtx), (const void *)12);
    p_glVertexAttribPointer(3, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(GVtx), (const void *)20);
    for (int i = 0; i < n_ops; i++) {
        const Op *op = &ops[i];
        stat_ops++;
        if (op->kind == OP_FILL) {
            u16 c = op->col;
            p_glScissor(op->r[0], op->r[1], op->r[2], op->r[3]);
            p_glClearColor((c & 31) / 31.0f, ((c >> 5) & 31) / 31.0f, ((c >> 10) & 31) / 31.0f, (c >> 15) ? 1.0f : 0.0f);
            p_glClear(GL_COLOR_BUFFER_BIT);
            dirty_add(op->r[0], op->r[1], op->r[0] + op->r[2] - 1, op->r[1] + op->r[3] - 1);
        } else if (op->kind == OP_COPY) {
            int sx = op->r[0], sy = op->r[1], dx = op->r[2], dy = op->r[3], w = op->r[4], h = op->r[5];
            if (sx + w > 1024) w = 1024 - sx;
            if (dx + w > 1024) w = 1024 - dx;
            if (sy + h > 512) h = 512 - sy;
            if (dy + h > 512) h = 512 - dy;
            if (w > 0 && h > 0) {
                barrier_if(sx, sy, sx + w - 1, sy + h - 1);
                p_glCopyTexSubImage2D(GL_TEXTURE_2D, 0, dx, dy, sx, sy, w, h);
                dirty_add(dx, dy, dx + w - 1, dy + h - 1);
            }
        } else if (op->key.tex && op->key.semi) { draw_op(op, 1); draw_op(op, 2); }
        else draw_op(op, 0);
    }
    p_glDisable(GL_BLEND);
    n_vtx = n_ops = 0;
}
static void gl_list_end(void) { submit(); p_glFlush(); }

/* ------------------------------------------------------------------ coherence with vram[] */
static void gl_vram_written(int x, int y, int w, int h) {
    if (!gl_ready) return;
    submit();
    const u16 *vram = port_vram();
    x &= 1023; y &= 511;
    if (x + w > 1024) w = 1024 - x;      /* (a wrapping rectangle is clipped) */
    if (y + h > 512) h = 512 - y;
    if (w <= 0 || h <= 0) return;
    for (int j = 0; j < h; j++) memcpy(stage + j * w, vram + (y + j) * 1024 + x, (size_t)w * 2);
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    p_glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, stage);
}
static void gl_vram_read(int x, int y, int w, int h) {
    if (!gl_ready) return;
    submit();
    u16 *vram = port_vram();
    x &= 1023; y &= 511;
    if (x + w > 1024) w = 1024 - x;
    if (y + h > 512) h = 512 - y;
    if (w <= 0 || h <= 0) return;
    bind_vram_fbo();
    p_glPixelStorei(GL_PACK_ALIGNMENT, 4);
    p_glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, stage8);
    /* 8-bit channels back to 5 bits (round to nearest, the inverse of the upload's k * 255 / 31) */
    for (int j = 0; j < h; j++) {
        const u8 *s = stage8 + (size_t)j * w * 4; u16 *d = vram + (y + j) * 1024 + x;
        for (int i = 0; i < w; i++, s += 4)
            d[i] = (u16)(((s[0] * 31 + 127) / 255) | (((s[1] * 31 + 127) / 255) << 5) | (((s[2] * 31 + 127) / 255) << 10) | (s[3] >= 128 ? 0x8000 : 0));
    }
}

/* ------------------------------------------------------------------ the window */
static int gl_present(int dx, int dy, int dw, int dh, int masked) {
    if (!gl_ready || !port_window) return 0;
    submit();
    int W = 0, H = 0;
    port_window_gl_size(&W, &H);
    if (W <= 0 || H <= 0) return 1;
    p_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    p_glDisable(GL_SCISSOR_TEST);
    p_glDisable(GL_BLEND);
    p_glViewport(0, 0, W, H);
    p_glClearColor(0, 0, 0, 1);
    p_glClear(GL_COLOR_BUFFER_BIT);
    if (!masked) {
        /* the logical frame is 320 pixels wide whatever the display mode (a 512-wide mode shows at the same size),
           integer-scaled to the largest multiple that fits, centred; smaller windows get the fit */
        int lw = 320, lh = dh;
        int k = W / lw < H / lh ? W / lw : H / lh;
        float ow, oh;
        if (k >= 1) { ow = (float)(lw * k); oh = (float)(lh * k); }
        else { float s = (float)W / lw < (float)H / lh ? (float)W / lw : (float)H / lh; ow = lw * s; oh = lh * s; }
        float x0 = -ow / W, x1 = ow / W, y0 = -oh / H, y1 = oh / H;
        float u0 = dx / 1024.0f, u1 = (dx + dw) / 1024.0f, v0 = dy / 512.0f, v1 = (dy + dh) / 512.0f;
        float q[24] = { x0, y1, u0, v0,  x1, y1, u1, v0,  x0, y0, u0, v1,   x1, y1, u1, v0,  x1, y0, u1, v1,  x0, y0, u0, v1 };
        p_glUseProgram(bprog);
        p_glBindTexture(GL_TEXTURE_2D, vram_tex);
        p_glBindBuffer(GL_ARRAY_BUFFER, vbo);
        p_glBufferData(GL_ARRAY_BUFFER, sizeof q, q, GL_STREAM_DRAW);
        p_glDisableVertexAttribArray(1); p_glDisableVertexAttribArray(3);
        p_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (const void *)0);
        p_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 16, (const void *)8);
        p_glDrawArrays(GL_TRIANGLES, 0, 6);
        p_glEnableVertexAttribArray(1); p_glEnableVertexAttribArray(3);
    }
    port_window_gl_swap();
    return 1;
}

/* ------------------------------------------------------------------ start / stop */
static int gl_start(void) {
    if (port_window_gl_start() < 0) return -1;
    if (gl_load() < 0) return -1;
    const char *ren = (const char *)p_glGetString(GL_RENDERER), *ver = (const char *)p_glGetString(GL_VERSION);
    port_printf("[gpu] OpenGL: %s, %s, GLSL %s%s\n", ren ? ren : "?", ver ? ver : "?",
                p_glGetString(GL_SHADING_LANGUAGE_VERSION) ? (const char *)p_glGetString(GL_SHADING_LANGUAGE_VERSION) : "?",
                p_glTextureBarrier ? ", texture barrier" : "");
    prog = link(vs_src, fs_src, "gpu");
    bprog = link(bvs_src, bfs_src, "blit");
    if (!prog || !bprog) return -1;
    p_glUseProgram(prog);
    p_glUniform1i(p_glGetUniformLocation(prog, "u_vram"), 0);
    u_tex = p_glGetUniformLocation(prog, "u_tex"); u_pass = p_glGetUniformLocation(prog, "u_pass");
    u_tw = p_glGetUniformLocation(prog, "u_tw"); u_setmask = p_glGetUniformLocation(prog, "u_setmask");
    p_glUseProgram(bprog);
    bu_vram = p_glGetUniformLocation(bprog, "u_vram"); p_glUniform1i(bu_vram, 0);
    /* VRAM: RGBA8, nearest, uploaded from the mirror (whatever LoadImage wrote before the backend started) */
    p_glGenTextures(1, &vram_tex);
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 512, 0, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, port_vram());
    p_glGenFramebuffers(1, &fbo);
    p_glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    p_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vram_tex, 0);
    GLenum st = p_glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (st != GL_FRAMEBUFFER_COMPLETE) { port_printf("[gpu] OpenGL: VRAM framebuffer incomplete (0x%x)\n", (unsigned)st); return -1; }
    p_glGenBuffers(1, &vbo);
    p_glEnableVertexAttribArray(0); p_glEnableVertexAttribArray(1); p_glEnableVertexAttribArray(2); p_glEnableVertexAttribArray(3);
    p_glDisable(GL_DEPTH_TEST); p_glDisable(GL_CULL_FACE); p_glDisable(GL_DITHER);
    p_glColorMask(1, 1, 1, 1);
    GLenum err = p_glGetError();
    if (err) port_printf("[gpu] OpenGL: error 0x%x during setup\n", (unsigned)err);
    gl_ready = 1;
    return 0;
}
static void gl_stop(void) {
    if (!gl_ready) return;
    if (port_verbose) port_printf("[gpu] OpenGL: %llu draw calls, %llu list operations, %llu texture barriers in %d frames\n", stat_draws, stat_ops, stat_barriers, port_frame);
    gl_ready = 0;
}

const GpuBackend port_gpu_gl = { "OpenGL", gl_start, gl_stop, gl_tri, gl_rect, gl_line, gl_fill, gl_copy,
                                 gl_vram_written, gl_vram_read, gl_list_end, gl_present };
