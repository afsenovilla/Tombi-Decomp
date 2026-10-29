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
    X(void, glLineWidth, (GLfloat)) \
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
    X(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void *)) \
    X(void, glActiveTexture, (GLenum))

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
   a_col: 8-bit colour; a_uv: texels, truncated like the PS1 does (+1/64 against float error): rounding to nearest made the sub-pixels
   of the last texel column/row past its middle (internal res > 1) sample the texel beyond the sprite (stray lines); a_tex: CLUT x/y,
   texture page x + depth * 1024, texture page y */
static const char vs_src[] =
    "uniform float u_hw;\n"
    "attribute vec2 a_pos; attribute vec4 a_col; attribute vec2 a_uv; attribute vec4 a_tex;\n"
    "varying vec4 v_col; varying vec2 v_uv; varying vec4 v_tex;\n"
    "void main() {\n"
    "  gl_Position = vec4(a_pos.x / u_hw - 1.0, a_pos.y / 256.0 - 1.0, 0.0, 1.0);\n"
    "  v_col = a_col; v_uv = a_uv; v_tex = a_tex;\n"
    "}\n";
/* u_tex: 0 flat/gouraud, 1 textured + modulated, 2 textured raw; u_pass: 0 all texels, 1 only the opaque ones
   (bit 15 clear), 2 only the semi-transparent ones; u_tw: texture window mask x, offset x, mask y, offset y in
   texels (the offset already masked); u_setmask: the mask bit to write.  All integer arithmetic is on floats
   (exact below 2^24): a VRAM word is rebuilt from the 8-bit channels and taken apart with floor/mod. */
static const char fs_src[] =
    "uniform sampler2D u_rep; uniform int u_reps; uniform vec4 u_repr;\n"     /* --texture-pack: the replacement, its texel box in the page */
    "uniform sampler2D u_vram; uniform int u_tex; uniform int u_pass; uniform vec4 u_tw; uniform float u_setmask; uniform float u_half;\n"
    "varying vec4 v_col; varying vec2 v_uv; varying vec4 v_tex;\n"
    "float vword(float x, float y) {\n"
    "  vec4 t = texture2D(u_vram, vec2((x + u_half) / 1024.0, (y + u_half) / 512.0));\n"
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
    "    float u = twin(mod(floor(v_uv.x + 0.015625), 256.0), u_tw.x, u_tw.y), v = twin(mod(floor(v_uv.y + 0.015625), 256.0), u_tw.z, u_tw.w);\n"
    "    if (u_reps > 0) {\n"          /* texel (u, v) of the box, the sub-texel position kept: the replacement is any multiple of it */
    "      vec2 rp = vec2(u, v) - u_repr.xy + fract(v_uv + 0.015625);\n"
    "      if (rp.x >= 0.0 && rp.y >= 0.0 && rp.x < u_repr.z && rp.y < u_repr.w) {\n"
    "        vec4 rc = texture2D(u_rep, rp / u_repr.zw);\n"
    "        if (rc.a < 0.25) discard;\n"
    "        float ra = rc.a < 0.75 ? 1.0 : 0.0;\n"
    "        if (u_pass == 1 && ra > 0.5) discard;\n"
    "        if (u_pass == 2 && ra < 0.5) discard;\n"
    "        o = rc.rgb * 31.0;\n"
    "        if (u_tex == 1) o = min(o * c / 128.0, 31.0);\n"
    "        gl_FragColor = vec4(o / 31.0, max(a, ra));\n"
    "        return;\n"
    "      }\n"
    "    }\n"
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
typedef struct { int semi, abr, tex, raw, twmx, twmy, twox, twoy, x1, y1, x2, y2, set_mask, check_mask, prim, wide;
                 unsigned rep; int rou, rov, rw, rh; } Key;     /* rep..: --texture-pack (PortTexRep), 0 = none */
enum { OP_DRAW, OP_FILL, OP_COPY, OP_UPLOAD, OP_WSDOWN };   /* (the last two only in --fps60 records) */
#define MAX_SAMPLED 12
typedef struct { int kind; Key key; int first, count; int r[6]; u16 col; int ns; short sr[MAX_SAMPLED][4]; } Op;   /* sr: VRAM rectangles the batch samples */

#define MAX_VTX 65536
#define MAX_OPS 8192
static GVtx vtx[MAX_VTX];
static Op ops[MAX_OPS];
static int n_vtx, n_ops;
static GLuint vram_tex, fbo, vbo, prog, bprog;
static GLint u_tex, u_pass, u_tw, u_setmask, bu_vram, u_hw, u_reps, u_repr;
/* --internal-res N (docs/PORT.md "HD"): the VRAM and wide targets are N times larger in both axes; primitives are
   rasterised at that resolution (the vertex shader works in normalised coordinates, so only the viewport, scissors
   and copies scale), texels and CLUT entries are read at the centre sub-pixel of each PS1 word (sub-pixel N / 2: the one
   whose centre is the native sample point when N is odd), host uploads are replicated N x N, readbacks take that
   same sub-pixel.  1 = the original path, bit for bit. */
int port_internal_res = 1;
static int R = 1;
/* --widescreen: the wide plane (gpu.c "widescreen"), 2048x512 RGBA8 with its own framebuffer; drawn into, never sampled */
static GLuint wide_tex, wide_fbo;
static int bound_wide = -1;
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
static u16 *stage;     /* 2048 x 512 x R x R words (uploads, replicated) */
static u8 *stage8;     /* the same in RGBA8 (readbacks) */
static unsigned long long stat_draws, stat_ops;

static const GpuState *gs = &port_gs;
static int same_key(const Key *a, const Key *b) { return memcmp(a, b, sizeof *a) == 0; }
static void make_key(Key *k, int semi, int tex, int raw, int prim) {
    memset(k, 0, sizeof *k);
    k->semi = semi ? 1 : 0; k->abr = semi ? gs->abr : 0; k->tex = tex; k->raw = tex ? (raw ? 1 : 0) : 0;
    if (tex) { k->twmx = gs->twmx; k->twmy = gs->twmy; k->twox = gs->twox; k->twoy = gs->twoy; }
    k->x1 = gs->x1; k->y1 = gs->y1; k->x2 = gs->x2; k->y2 = gs->y2;
    k->set_mask = gs->set_mask; k->check_mask = gs->check_mask; k->prim = prim; k->wide = gs->wide && wide_fbo;
}
static void key_rep(Key *k, const PortTexRep *r) { k->rep = r->gltex; k->rou = r->ou; k->rov = r->ov; k->rw = r->w; k->rh = r->h; }
static void submit(void);
static int rec_on;                  /* --fps60: the decoded primitives of this vblank are recorded (fps60 below) */
static void rec_prim(const GVtx *v, int n, const Key *k);
static void rec_hostop(int kind, int a, int b, int c, int d, int e, int f);
/* x0..y1: the primitive's bounding box.  A textured semi-transparent batch is drawn in two passes (all its opaque
   texels, then all its semi-transparent ones), which reorders overlapping primitives of one batch: a later sprite's
   opaque texels ended under an earlier sprite's blended ones (the HUD "AP" label, raw semi-transparent glyphs
   over the semi-transparent box tiles, came out washed out).  Such a batch is ended where a primitive overlaps
   what it already holds (r[0..3]: the batch's bounding box). */
static GVtx *alloc_vtx(int n, const Key *k, const GpuTex *t, int x0, int y0, int x1, int y1) {
    if (n_vtx + n > MAX_VTX || n_ops >= MAX_OPS) submit();
    Op *op = n_ops ? &ops[n_ops - 1] : 0;
    if (!op || op->kind != OP_DRAW || !same_key(&op->key, k)
        || (k->tex && k->semi && x0 < op->r[2] && x1 > op->r[0] && y0 < op->r[3] && y1 > op->r[1])) {
        op = &ops[n_ops++]; op->kind = OP_DRAW; op->key = *k; op->first = n_vtx; op->count = 0; op->ns = 0;
        op->r[0] = x0; op->r[1] = y0; op->r[2] = x1; op->r[3] = y1;
    } else {
        if (x0 < op->r[0]) op->r[0] = x0; if (y0 < op->r[1]) op->r[1] = y0;
        if (x1 > op->r[2]) op->r[2] = x1; if (y1 > op->r[3]) op->r[3] = y1;
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
   tie rule is (a top or left edge includes it, a bottom or right one does not).  At internal res R the GL samples
   sub-pixel centres (m + 0.5) / R: the shift is divided by R, so that sub-pixel m samples the PS1 point m / R and
   the R sub-pixels of PS1 pixel x land on x .. x + (R - 1) / R, inside it.  A full half-pixel shift at R > 1 put
   the first sub-pixels of a polygon before its first PS1 pixel: they were covered (edge moved left/up by
   0.5 - 0.5 / R) and their interpolated u/v fell below the first texel, so they read the texel before it (stray
   lines along dialogue box borders and under text rows). */
#define POLY_SHIFT ((0.5f - 1.0f / 64.0f) / (float)R)

static void gl_tri(const GpuVtx *a, const GpuVtx *b, const GpuVtx *c, int shade, const GpuTex *tex, int semi, int raw) {
    Key k; (void)shade;
    if (!gl_ready) return;
    make_key(&k, semi, tex ? (raw ? 2 : 1) : 0, raw, GL_TRIANGLES);
    if (tex && tex->rep) key_rep(&k, tex->rep);
    int bx0 = a->x < b->x ? a->x : b->x, bx1 = a->x > b->x ? a->x : b->x, by0 = a->y < b->y ? a->y : b->y, by1 = a->y > b->y ? a->y : b->y;
    if (c->x < bx0) bx0 = c->x; if (c->x > bx1) bx1 = c->x; if (c->y < by0) by0 = c->y; if (c->y > by1) by1 = c->y;
    GVtx *v = alloc_vtx(3, &k, tex, bx0, by0, bx1 + 1, by1 + 1);
    set_vtx(v + 0, a->x + POLY_SHIFT, a->y + POLY_SHIFT, a->r, a->g, a->b, (float)a->u, (float)a->v, tex);
    set_vtx(v + 1, b->x + POLY_SHIFT, b->y + POLY_SHIFT, b->r, b->g, b->b, (float)b->u, (float)b->v, tex);
    set_vtx(v + 2, c->x + POLY_SHIFT, c->y + POLY_SHIFT, c->r, c->g, c->b, (float)c->u, (float)c->v, tex);
    if (rec_on) rec_prim(v, 3, &k);
}
static void gl_rect(int x, int y, int w, int h, u32 col, const GpuTex *tex, int u0, int v0, int semi, int raw) {
    Key k;
    if (!gl_ready) return;
    make_key(&k, semi, tex ? (raw ? 2 : 1) : 0, raw, GL_TRIANGLES);
    if (tex && tex->rep) key_rep(&k, tex->rep);
    int r = col & 255, g = (col >> 8) & 255, b = (col >> 16) & 255;
    /* texel (u0 + i, v0 + j) for pixel (x + i, y + j): uv runs u0..u0 + w over the pixel span, truncated in the shader */
    float ua = (float)u0, ub = (float)(u0 + w), va = (float)v0, vb = (float)(v0 + h);
    float xa = (float)x, xb = (float)(x + w), ya = (float)y, yb = (float)(y + h);
    GVtx *v = alloc_vtx(6, &k, tex, x, y, x + w, y + h);
    set_vtx(v + 0, xa, ya, r, g, b, ua, va, tex); set_vtx(v + 1, xb, ya, r, g, b, ub, va, tex); set_vtx(v + 2, xa, yb, r, g, b, ua, vb, tex);
    set_vtx(v + 3, xb, ya, r, g, b, ub, va, tex); set_vtx(v + 4, xb, yb, r, g, b, ub, vb, tex); set_vtx(v + 5, xa, yb, r, g, b, ua, vb, tex);
    if (rec_on) rec_prim(v, 6, &k);
}
static void gl_line(int x0, int y0, u32 c0, int x1, int y1, u32 c1, int shade, int semi) {
    Key k;
    if (!gl_ready) return;
    if (!shade) c1 = c0;
    make_key(&k, semi, 0, 0, GL_LINES);
    GVtx *v = alloc_vtx(2, &k, 0, 0, 0, 0, 0);
    /* a GL line leaves out its last pixel (diamond exit); the PS1 draws both ends: extend by half a pixel */
    float dx = (float)(x1 - x0), dy = (float)(y1 - y0), len = (float)SDL_sqrt(dx * dx + dy * dy);
    float ex = len > 0 ? dx / len * 0.5f : 0, ey = len > 0 ? dy / len * 0.5f : 0;
    set_vtx(v + 0, x0 + 0.5f, y0 + 0.5f, c0 & 255, (c0 >> 8) & 255, (c0 >> 16) & 255, 0, 0, 0);
    set_vtx(v + 1, x1 + 0.5f + ex, y1 + 0.5f + ey, c1 & 255, (c1 >> 8) & 255, (c1 >> 16) & 255, 0, 0, 0);
    if (len == 0) { v[1].x += 0.75f; }        /* a single point: a short horizontal line */
    if (rec_on) rec_prim(v, 2, &k);
}
static void gl_fill(int x, int y, int w, int h, u16 c) {
    if (!gl_ready) return;
    if (n_ops >= MAX_OPS) submit();
    Op *op = &ops[n_ops++]; op->kind = OP_FILL; op->r[0] = x; op->r[1] = y; op->r[2] = w; op->r[3] = h; op->col = c; op->r[4] = 0;
}
static void gl_ws_fill(int x, int y, int w, int h, u16 c) {
    if (!gl_ready || !wide_fbo) return;
    if (n_ops >= MAX_OPS) submit();
    Op *op = &ops[n_ops++]; op->kind = OP_FILL; op->r[0] = x; op->r[1] = y; op->r[2] = w; op->r[3] = h; op->col = c; op->r[4] = 1;
}
static void gl_copy(int sx, int sy, int dx, int dy, int w, int h) {
    if (!gl_ready) return;
    if (n_ops >= MAX_OPS) submit();
    Op *op = &ops[n_ops++]; op->kind = OP_COPY; op->r[0] = sx; op->r[1] = sy; op->r[2] = dx; op->r[3] = dy; op->r[4] = w; op->r[5] = h;
}

/* ------------------------------------------------------------------ submitting the list */
static void bind_target(int wide) {
    if (bound_wide == wide) return;
    bound_wide = wide;
    p_glBindFramebuffer(GL_FRAMEBUFFER, wide ? wide_fbo : fbo);
    p_glViewport(0, 0, (wide ? 2048 : 1024) * R, 512 * R);
    p_glUseProgram(prog);
    p_glUniform1f(u_hw, wide ? 1024.0f : 512.0f);
}
static void bind_vram_fbo(void) { bound_wide = -1; bind_target(0); }
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
    bind_target(k->wide);
    p_glScissor(k->x1 * R, k->y1 * R, w * R, h * R);
    set_blend(k, pass);
    p_glUniform1i(u_tex, k->tex);
    p_glUniform1i(u_pass, pass);
    p_glUniform4f(u_tw, (float)(k->twmx * 8), (float)((k->twox & k->twmx) * 8), (float)(k->twmy * 8), (float)((k->twoy & k->twmy) * 8));
    p_glUniform1f(u_setmask, k->set_mask ? 1.0f : 0.0f);
    if (u_reps >= 0) {
        p_glUniform1i(u_reps, k->rep ? 1 : 0);
        if (k->rep) {
            p_glUniform4f(u_repr, (float)k->rou, (float)k->rov, (float)k->rw, (float)k->rh);
            p_glActiveTexture(GL_TEXTURE0 + 1); p_glBindTexture(GL_TEXTURE_2D, k->rep); p_glActiveTexture(GL_TEXTURE0);
        }
    }
    for (int i = 0; i < op->ns && dirty_any; i++) barrier_if(op->sr[i][0], op->sr[i][1], op->sr[i][2], op->sr[i][3]);
    p_glDrawArrays(k->prim, op->first, op->count);
    if (!k->wide) dirty_add(k->x1, k->y1, k->x2, k->y2);
    stat_draws++;
}
static void rec_append(void);
static void submit(void) {
    if (!gl_ready || !n_ops) { n_vtx = n_ops = 0; return; }
    if (rec_on) rec_append();
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
            bind_target(op->r[4]);
            p_glScissor(op->r[0] * R, op->r[1] * R, op->r[2] * R, op->r[3] * R);
            p_glClearColor((c & 31) / 31.0f, ((c >> 5) & 31) / 31.0f, ((c >> 10) & 31) / 31.0f, (c >> 15) ? 1.0f : 0.0f);
            p_glClear(GL_COLOR_BUFFER_BIT);
            if (!op->r[4]) dirty_add(op->r[0], op->r[1], op->r[0] + op->r[2] - 1, op->r[1] + op->r[3] - 1);
        } else if (op->kind == OP_COPY) {
            int sx = op->r[0], sy = op->r[1], dx = op->r[2], dy = op->r[3], w = op->r[4], h = op->r[5];
            if (sx + w > 1024) w = 1024 - sx;
            if (dx + w > 1024) w = 1024 - dx;
            if (sy + h > 512) h = 512 - sy;
            if (dy + h > 512) h = 512 - dy;
            if (w > 0 && h > 0) {
                bind_target(0);
                barrier_if(sx, sy, sx + w - 1, sy + h - 1);
                p_glCopyTexSubImage2D(GL_TEXTURE_2D, 0, dx * R, dy * R, sx * R, sy * R, w * R, h * R);
                dirty_add(dx, dy, dx + w - 1, dy + h - 1);
            }
        } else if (op->key.tex && op->key.semi) { draw_op(op, 1); draw_op(op, 2); }
        else draw_op(op, 0);
    }
    p_glDisable(GL_BLEND);
    bind_vram_fbo();
    n_vtx = n_ops = 0;
}
static void gl_list_end(void) { submit(); p_glFlush(); }

/* ------------------------------------------------------------------ coherence with vram[] */
/* stage[] (w x h words) into the bound texture at (x, y) PS1 pixels, each word replicated R x R */
static void upload_stage(int x, int y, int w, int h) {
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    if (R == 1) { p_glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, stage); return; }
    u16 *big = stage + (size_t)w * h;            /* after the source words (the buffer holds 2048x512xRxR + that) */
    for (int j = 0; j < h; j++) {
        u16 *row = big + (size_t)j * R * w * R;
        for (int i = 0; i < w; i++) for (int k = 0; k < R; k++) row[i * R + k] = stage[j * w + i];
        for (int k = 1; k < R; k++) memcpy(row + (size_t)k * w * R, row, (size_t)w * R * 2);
    }
    p_glTexSubImage2D(GL_TEXTURE_2D, 0, x * R, y * R, w * R, h * R, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, big);
}
/* the bound framebuffer's (x, y, w, h) PS1 pixels as 15-bit words (the centre sub-pixel of each) */
static void read_words(int x, int y, int w, int h, u16 *dst, int stride) {
    p_glPixelStorei(GL_PACK_ALIGNMENT, 4);
    p_glReadPixels(x * R, y * R, w * R, h * R, GL_RGBA, GL_UNSIGNED_BYTE, stage8);
    /* 8-bit channels back to 5 bits (round to nearest, the inverse of the upload's k * 255 / 31) */
    for (int j = 0; j < h; j++) {
        const u8 *s = stage8 + ((size_t)(j * R + R / 2) * w * R + R / 2) * 4; u16 *d = dst + (size_t)j * stride;
        for (int i = 0; i < w; i++, s += 4 * R)
            d[i] = (u16)(((s[0] * 31 + 127) / 255) | (((s[1] * 31 + 127) / 255) << 5) | (((s[2] * 31 + 127) / 255) << 10) | (s[3] >= 128 ? 0x8000 : 0));
    }
}
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
    upload_stage(x, y, w, h);
    if (rec_on) rec_hostop(OP_UPLOAD, x, y, w, h, 0, 0);
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
    read_words(x, y, w, h, vram + y * 1024 + x, 1024);
}

/* --texture-pack (texpack.c): a replacement image as its own texture (nearest: the alpha classes must stay exact) */
unsigned port_gl_rep_texture(const u8 *rgba, int w, int h) {
    GLuint t = 0;
    if (!gl_ready) return 0;
    submit();
    p_glGenTextures(1, &t);
    p_glBindTexture(GL_TEXTURE_2D, t);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
    if (p_glGetError() != GL_NO_ERROR) port_printf("[gpu] OpenGL: replacement texture %dx%d failed\n", w, h);
    return t;
}

/* ------------------------------------------------------------------ the --widescreen plane */
static void gl_ws_copy_down(int wx, int wy, int vx, int vy, int w, int h) {
    if (!gl_ready || !wide_fbo) return;
    submit();
    p_glBindFramebuffer(GL_FRAMEBUFFER, wide_fbo);
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
    p_glCopyTexSubImage2D(GL_TEXTURE_2D, 0, vx * R, vy * R, wx * R, wy * R, w * R, h * R);
    bind_vram_fbo();
    if (rec_on) rec_hostop(OP_WSDOWN, wx, wy, vx, vy, w, h);
}
static void gl_ws_upload(int wx, int wy, int w, int h, const u16 *src, int stride) {
    if (!gl_ready || !wide_fbo || w <= 0 || h <= 0) return;
    submit();
    for (int j = 0; j < h; j++) memcpy(stage + j * w, src + j * stride, (size_t)w * 2);
    p_glBindTexture(GL_TEXTURE_2D, wide_tex);
    upload_stage(wx, wy, w, h);
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
    if (rec_on) rec_hostop(OP_UPLOAD, wx, wy, w, h, 1, 0);
}
static void gl_ws_read(int wx, int wy, int w, int h, u16 *dst, int stride) {
    if (!gl_ready || !wide_fbo || w <= 0 || h <= 0) return;
    submit();
    bind_target(1);
    read_words(wx, wy, w, h, dst, stride);
    bind_vram_fbo();
}
static int gl_ws_present(int wx, int wy, int w, int h);

/* ------------------------------------------------------------------ the window */
/* the display area (a rectangle of the VRAM texture, or the whole 24-bit frame texture) onto the window: the
   logical frame is 320 pixels wide whatever the display mode (a 512-wide mode shows at the same size), integer-scaled
   to the largest multiple that fits, centred; smaller windows get the fit */
static void blit(GLuint tex, float u0, float v0, float u1, float v1, int dh, int W, int H) {
    int fx, fy, fw, fh;
    port_window_fit(W, H, dh, &fx, &fy, &fw, &fh);   /* 4:3 as a PAL TV (or square pixels), centred */
    float ow = (float)fw, oh = (float)fh;
    float x0 = -ow / W, x1 = ow / W, y0 = -oh / H, y1 = oh / H;
    /* a non-integer horizontal scale (4:3) samples between texels: filter the blit linearly, the VRAM texture
       goes back to nearest for the game's own sampling */
    p_glBindTexture(GL_TEXTURE_2D, tex);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, port_window_square ? GL_NEAREST : GL_LINEAR);
    if (R > 1) p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   /* N x internal in a smaller window */
    float q[24] = { x0, y1, u0, v0,  x1, y1, u1, v0,  x0, y0, u0, v1,   x1, y1, u1, v0,  x1, y0, u1, v1,  x0, y0, u0, v1 };
    p_glUseProgram(bprog);
    p_glBindBuffer(GL_ARRAY_BUFFER, vbo);
    p_glBufferData(GL_ARRAY_BUFFER, sizeof q, q, GL_STREAM_DRAW);
    p_glDisableVertexAttribArray(1); p_glDisableVertexAttribArray(3);
    p_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (const void *)0);
    p_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 16, (const void *)8);
    p_glDrawArrays(GL_TRIANGLES, 0, 6);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    p_glEnableVertexAttribArray(1); p_glEnableVertexAttribArray(3);
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
}
static int present_begin(int *W, int *H) {
    if (!gl_ready || !port_window) return 0;
    submit();
    *W = *H = 0;
    port_window_gl_size(W, H);
    p_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    p_glDisable(GL_SCISSOR_TEST);
    p_glDisable(GL_BLEND);
    p_glViewport(0, 0, *W, *H);
    p_glClearColor(0, 0, 0, 1);
    p_glClear(GL_COLOR_BUFFER_BIT);
    return 1;
}
static int gl_present(int dx, int dy, int dw, int dh, int masked) {
    int W, H;
    if (!present_begin(&W, &H)) return 0;
    if (W > 0 && H > 0 && !masked)
        blit(vram_tex, dx / 1024.0f, dy / 512.0f, (dx + dw) / 1024.0f, (dy + dh) / 512.0f, dh, W, H);
    port_window_gl_swap();
    return 1;
}
static int gl_ws_present(int wx, int wy, int w, int h) {
    extern int port_window_fit_wide;
    int W, H;
    if (!wide_fbo || !present_begin(&W, &H)) return 0;
    port_window_fit_wide = 1;
    if (W > 0 && H > 0) blit(wide_tex, wx / 2048.0f, wy / 512.0f, (wx + w) / 2048.0f, (wy + h) / 512.0f, h, W, H);
    port_window_fit_wide = 0;
    port_window_gl_swap();
    bound_wide = -1;
    return 1;
}
/* the 24-bit display mode (the MDEC movies): the frame gpu.c read out of VRAM as RGB8 goes to its own texture and
   is blitted the same way.  The software path hands it to an SDL_Renderer instead (port_window_frame); here that
   renderer would be a second device on the window that already has the GL context (on Windows a Direct3D one),
   and the picture ends up in whichever surface the compositor shows last.  Until this the movies were blank on the
   Windows build with the OpenGL renderer. */
static GLuint frame_tex; static int frame_w, frame_h;
static int gl_present_rgb(const gpu_u8 *rgb, int w, int h) {
    int W, H;
    if (!present_begin(&W, &H)) return 0;
    if (W > 0 && H > 0 && w > 0 && h > 0) {
        if (!frame_tex) {
            p_glGenTextures(1, &frame_tex);
            p_glBindTexture(GL_TEXTURE_2D, frame_tex);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else p_glBindTexture(GL_TEXTURE_2D, frame_tex);
        p_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if (w != frame_w || h != frame_h) { p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb); frame_w = w; frame_h = h; }
        else p_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, rgb);
        p_glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
        blit(frame_tex, 0.0f, 0.0f, 1.0f, 1.0f, h, W, H);
    }
    port_window_gl_swap();
    return 1;
}

/* --dump-frames with --internal-res N > 1: the display area (15-bit mode, VRAM or wide plane) at the internal
   resolution, as RGB8 (rgb holds w * R x h * R pixels); 0 = done, -1 = not available */
int port_gl_frame_hd(gpu_u8 *rgb, int x, int y, int w, int h, int wide, int *ow, int *oh) {
    if (!gl_ready || R == 1 || (wide && !wide_fbo)) return -1;
    submit();
    bind_target(wide);
    p_glPixelStorei(GL_PACK_ALIGNMENT, 1);
    *ow = w * R; *oh = h * R;
    p_glReadPixels(x * R, y * R, w * R, h * R, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    bind_vram_fbo();
    return 0;
}

/* ------------------------------------------------------------------ --fps60 (experimental): interpolated presents
   Every vblank's list (the batched ops and their vertices, one entry per primitive with its signature) is recorded,
   with a copy of VRAM (and the wide plane) taken before its first op.  At each vblank the record that drew the
   displayed area is paired with the one displayed a vblank earlier: each primitive is matched to one of the earlier
   frame (same signature: kind, state, texture page, CLUT and u/v; then a looser pass without u/v for animated sprites,
   moved as a whole by its centroid), positions compared relative to the drawing area so the two buffers of the double
   buffer line up.  A match that moved more than FPS60_JUMP pixels (a camera cut, a teleport, a wrong pairing) is not
   interpolated.  A present at alpha (0 = the earlier frame, 1 = the displayed one) replays the displayed record on
   work copies of the targets, every matched vertex moved back by (1 - alpha) of its delta, and shows that.  Only
   when --fps60 is on; nothing here runs otherwise. */
#define FPS60_JUMP 32.0f
#define FPS60_JUMP_LOOSE 24.0f
#define NREC 4
typedef struct { int v0, n; float ox, oy; u32 sig, lsig; } RPrim;
typedef struct {
    unsigned id;                        /* 0 = being recorded / empty */
    Op *ops; int nops, capops;
    GVtx *v; int nv, capv;
    RPrim *p; int np, capp;
    float *d;                           /* per vertex: (earlier - this) x, y; valid for the displayed record of a pair */
    GLuint snapV, snapVfbo, snapW, snapWfbo; int snapped;
} Rec;
static Rec recs[NREC];
static int rc, replaying, f60_init, pair_ok;
static unsigned rec_seq, d_cur;
static int d_x, d_y, d_w, d_h, d_wide;
static GLuint workV, workVfbo, workW, workWfbo;
static void *grow(void *p, int *cap, int need, size_t sz) {
    if (need <= *cap) return p;
    int c = *cap ? *cap : 1024;
    while (c < need) c *= 2;
    p = SDL_realloc(p, (size_t)c * sz);
    if (!p) port_fatal("fps60: out of memory");
    *cap = c;
    return p;
}
static u32 hmix(u32 h, u32 v) { return (h ^ v) * 16777619u; }
static void rec_prim(const GVtx *v, int n, const Key *k) {
    Rec *r = &recs[rc];
    r->p = (RPrim *)grow(r->p, &r->capp, r->np + 1, sizeof(RPrim));
    RPrim *p = &r->p[r->np++];
    p->v0 = r->nv + (int)(v - vtx); p->n = n; p->ox = (float)k->x1; p->oy = (float)k->y1;
    u32 h = 2166136261u;
    h = hmix(h, (u32)n); h = hmix(h, (u32)(k->semi | k->abr << 1 | k->tex << 3 | k->raw << 5 | k->wide << 6)); h = hmix(h, (u32)k->prim);
    h = hmix(h, (u32)v->cx | (u32)v->cy << 16); h = hmix(h, (u32)v->tx | (u32)v->ty << 16);
    p->lsig = h;
    if (k->tex) for (int i = 0; i < n; i++) h = hmix(h, (u32)(int)v[i].u | (u32)(int)v[i].v << 16);
    p->sig = h;
}
static void new_target(int w, int h, GLuint *tex, GLuint *fb) {
    p_glGenTextures(1, tex);
    p_glBindTexture(GL_TEXTURE_2D, *tex);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w * R, h * R, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    p_glGenFramebuffers(1, fb);
    p_glBindFramebuffer(GL_FRAMEBUFFER, *fb);
    p_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tex, 0);
    bound_wide = -1;
}
/* the whole of framebuffer src into texture dst (w x h PS1 pixels) */
static void copy_target(GLuint src_fb, GLuint dst_tex, int w, int h) {
    p_glBindFramebuffer(GL_FRAMEBUFFER, src_fb);
    p_glBindTexture(GL_TEXTURE_2D, dst_tex);
    p_glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, w * R, h * R);
    bound_wide = -1;
}
static void rec_append(void) {
    Rec *r = &recs[rc];
    if (!r->snapped) {
        if (!r->snapV) new_target(1024, 512, &r->snapV, &r->snapVfbo);
        copy_target(fbo, r->snapV, 1024, 512);
        if (wide_fbo) { if (!r->snapW) new_target(2048, 512, &r->snapW, &r->snapWfbo); copy_target(wide_fbo, r->snapW, 2048, 512); }
        p_glBindTexture(GL_TEXTURE_2D, vram_tex);
        r->snapped = 1;
    }
    r->ops = (Op *)grow(r->ops, &r->capops, r->nops + n_ops, sizeof(Op));
    r->v = (GVtx *)grow(r->v, &r->capv, r->nv + n_vtx, sizeof(GVtx));
    for (int i = 0; i < n_ops; i++) { Op *o = &r->ops[r->nops++]; *o = ops[i]; if (o->kind == OP_DRAW) o->first += r->nv; }
    memcpy(r->v + r->nv, vtx, (size_t)n_vtx * sizeof(GVtx));
    r->nv += n_vtx;
}
static void rec_hostop(int kind, int a, int b, int c, int d, int e, int f) {
    Rec *r = &recs[rc];
    if (!r->snapped) return;             /* (before the snapshot: already in it) */
    r->ops = (Op *)grow(r->ops, &r->capops, r->nops + 1, sizeof(Op));
    Op *o = &r->ops[r->nops++];
    memset(o, 0, sizeof *o);
    o->kind = kind; o->r[0] = a; o->r[1] = b; o->r[2] = c; o->r[3] = d; o->r[4] = e; o->r[5] = f;
}
static Rec *rec_by_id(unsigned id) {
    for (int i = 0; i < NREC; i++) if (id && recs[i].id == id) return &recs[i];
    return 0;
}
/* the deltas of C's vertices towards the matching primitives of P */
static void pair(Rec *C, Rec *P) {
    enum { HB = 4096, SCAN = 64 };
    static int head[HB], lhead[HB], *nx, *lnx, capn; static unsigned char *used, *done; static int capu, capd;
    C->d = (float *)SDL_realloc(C->d, (size_t)(C->nv ? C->nv : 1) * 2 * sizeof(float));
    if (!C->d) { pair_ok = 0; return; }
    memset(C->d, 0, (size_t)C->nv * 2 * sizeof(float));
    nx = (int *)grow(nx, &capn, P->np * 2 + 1, sizeof(int)); lnx = nx + P->np;
    used = (unsigned char *)grow(used, &capu, P->np + 1, 1); memset(used, 0, (size_t)P->np);
    done = (unsigned char *)grow(done, &capd, C->np + 1, 1); memset(done, 0, (size_t)C->np);
    for (int i = 0; i < HB; i++) head[i] = lhead[i] = -1;
    for (int i = P->np - 1; i >= 0; i--) {
        u32 b = P->p[i].sig % HB, lb = P->p[i].lsig % HB;
        nx[i] = head[b]; head[b] = i; lnx[i] = lhead[lb]; lhead[lb] = i;
    }
    int exact = 0, loose = 0;
    for (int pass = 0; pass < 2; pass++)
        for (int i = 0; i < C->np; i++) {
            const RPrim *c = &C->p[i];
            if (done[i]) continue;
            const GVtx *cv = C->v + c->v0;
            float ccx = 0, ccy = 0;
            for (int k = 0; k < c->n; k++) { ccx += cv[k].x - c->ox; ccy += cv[k].y - c->oy; }
            ccx /= c->n; ccy /= c->n;
            int best = -1, scan = 0; float bd = pass ? FPS60_JUMP_LOOSE : FPS60_JUMP, bdx = 0, bdy = 0;
            for (int j = pass ? lhead[c->lsig % HB] : head[c->sig % HB]; j >= 0 && scan < SCAN; j = pass ? lnx[j] : nx[j]) {
                const RPrim *q = &P->p[j];
                if (used[j] || q->n != c->n || (pass ? q->lsig != c->lsig : q->sig != c->sig)) continue;
                scan++;
                const GVtx *qv = P->v + q->v0;
                float m = 0, qcx = 0, qcy = 0;
                for (int k = 0; k < c->n; k++) {
                    float dx = (qv[k].x - q->ox) - (cv[k].x - c->ox), dy = (qv[k].y - q->oy) - (cv[k].y - c->oy);
                    qcx += qv[k].x - q->ox; qcy += qv[k].y - q->oy;
                    if (!pass) { if (dx < 0) dx = -dx; if (dy < 0) dy = -dy; if (dx > m) m = dx; if (dy > m) m = dy; }
                }
                if (pass) {
                    float dx = qcx / c->n - ccx, dy = qcy / c->n - ccy;
                    m = dx < 0 ? -dx : dx; if ((dy < 0 ? -dy : dy) > m) m = dy < 0 ? -dy : dy;
                    if (m < bd) { bdx = dx; bdy = dy; }
                }
                if (m < bd) { bd = m; best = j; if (m == 0) break; }
            }
            if (best < 0) continue;
            used[best] = 1; done[i] = 1;
            const RPrim *q = &P->p[best]; const GVtx *qv = P->v + q->v0;
            for (int k = 0; k < c->n; k++) {
                float *d = C->d + (size_t)(c->v0 + k) * 2;
                if (pass) { d[0] = bdx; d[1] = bdy; }
                else { d[0] = (qv[k].x - q->ox) - (cv[k].x - c->ox); d[1] = (qv[k].y - q->oy) - (cv[k].y - c->oy); }
            }
            if (pass) loose++; else exact++;
        }
    if (port_verbose > 1) port_printf("[fps60] frame %d: %d primitives, %d matched exactly, %d loosely (earlier frame %d)\n", port_frame, C->np, exact, loose, P->np);
}
/* gpu.c at every vblank: the displayed area (plain: not a 15-bit frame of ours: blank, 24-bit, ...) */
int port_gl_fps60_vblank(int x, int y, int w, int h, int wide, int plain) {
    if (!gl_ready) return 0;
    if (!f60_init) {
        f60_init = 1;
        new_target(1024, 512, &workV, &workVfbo);
        if (wide_fbo) new_target(2048, 512, &workW, &workWfbo);
        p_glBindTexture(GL_TEXTURE_2D, vram_tex);
        bind_vram_fbo();
        port_printf("[gpu] OpenGL: --fps60: interpolated presents between the 50 Hz logic frames (experimental)\n");
        rec_on = 1;
        return 0;
    }
    submit();
    if (recs[rc].nops) {                 /* close this vblank's record, start the next one */
        recs[rc].id = ++rec_seq;
        rc = (rc + 1) % NREC;
        recs[rc].id = 0; recs[rc].nops = recs[rc].nv = recs[rc].np = 0; recs[rc].snapped = 0;
    } else recs[rc].np = 0;
    unsigned nd = 0;
    if (!plain)
        for (int i = 0; i < NREC; i++) {
            const Rec *r = &recs[i];
            if (!r->id || r->id <= nd) continue;
            for (int j = 0; j < r->nops; j++) {
                const Key *k = &r->ops[j].key;
                if (r->ops[j].kind == OP_DRAW && k->wide == wide && k->x1 < x + w && k->x2 >= x && k->y1 < y + h && k->y2 >= y) { nd = r->id; break; }
            }
        }
    Rec *C = rec_by_id(nd), *P = rec_by_id(d_cur);
    pair_ok = 0;
    if (C && P && C != P) { pair_ok = 1; pair(C, P); }
    d_cur = nd; d_x = x; d_y = y; d_w = w; d_h = h; d_wide = wide;
    return 0;
}
/* a present at alpha: the displayed record replayed on the work targets with interpolated vertices; 0 = nothing to
   interpolate (the caller shows the displayed frame itself) */
int port_gl_fps60_render(float alpha) {
    Rec *C = rec_by_id(d_cur);
    if (!gl_ready || !pair_ok || !C || alpha >= 0.999f) return 0;
    float f = 1.0f - (alpha < 0 ? 0 : alpha);
    submit();
    GLuint sv_tex = vram_tex, sv_fbo = fbo, sv_wtex = wide_tex, sv_wfbo = wide_fbo;
    int sv_dirty[5] = { dirty_any, dirty_x1, dirty_y1, dirty_x2, dirty_y2 };
    copy_target(C->snapVfbo, workV, 1024, 512);
    if (sv_wfbo && C->snapW) copy_target(C->snapWfbo, workW, 2048, 512);
    rec_on = 0; replaying = 1;
    vram_tex = workV; fbo = workVfbo;
    if (sv_wfbo) { wide_tex = workW; wide_fbo = workWfbo; }
    dirty_any = 0; bound_wide = -1;
    for (int i = 0; i < C->nops; i++) {
        const Op *o = &C->ops[i];
        if (o->kind == OP_DRAW) {
            if (n_vtx + o->count > MAX_VTX || n_ops >= MAX_OPS) submit();
            Op *q = &ops[n_ops++]; *q = *o; q->first = n_vtx;
            memcpy(vtx + n_vtx, C->v + o->first, (size_t)o->count * sizeof(GVtx));
            const float *d = C->d + (size_t)o->first * 2;
            for (int k = 0; k < o->count; k++) { vtx[n_vtx + k].x += d[2 * k] * f; vtx[n_vtx + k].y += d[2 * k + 1] * f; }
            n_vtx += o->count;
        } else if (o->kind == OP_FILL || o->kind == OP_COPY) {
            if (n_ops >= MAX_OPS) submit();
            ops[n_ops++] = *o;
        } else if (o->kind == OP_UPLOAD) {       /* a host upload during the frame: taken from the live target */
            submit();
            int wd = o->r[4];
            if (wd && !sv_wfbo) continue;
            p_glBindFramebuffer(GL_FRAMEBUFFER, wd ? sv_wfbo : sv_fbo);
            p_glBindTexture(GL_TEXTURE_2D, wd ? workW : workV);
            p_glCopyTexSubImage2D(GL_TEXTURE_2D, 0, o->r[0] * R, o->r[1] * R, o->r[0] * R, o->r[1] * R, o->r[2] * R, o->r[3] * R);
            bound_wide = -1; dirty_any = 1; dirty_x1 = 0; dirty_y1 = 0; dirty_x2 = 1023; dirty_y2 = 511;
        } else if (o->kind == OP_WSDOWN && sv_wfbo) {
            submit();
            p_glBindFramebuffer(GL_FRAMEBUFFER, workWfbo);
            p_glBindTexture(GL_TEXTURE_2D, workV);
            p_glCopyTexSubImage2D(GL_TEXTURE_2D, 0, o->r[2] * R, o->r[3] * R, o->r[0] * R, o->r[1] * R, o->r[4] * R, o->r[5] * R);
            bound_wide = -1;
        }
    }
    submit();
    vram_tex = sv_tex; fbo = sv_fbo; wide_tex = sv_wtex; wide_fbo = sv_wfbo;
    dirty_any = sv_dirty[0]; dirty_x1 = sv_dirty[1]; dirty_y1 = sv_dirty[2]; dirty_x2 = sv_dirty[3]; dirty_y2 = sv_dirty[4];
    replaying = 0; rec_on = 1;
    p_glBindTexture(GL_TEXTURE_2D, vram_tex);
    bind_vram_fbo();
    return 1;
}
/* the interpolated frame onto the window; 0 = no window */
int port_gl_fps60_show(void) {
    extern int port_window_fit_wide;
    int W, H;
    if (!present_begin(&W, &H)) return 0;
    port_window_fit_wide = d_wide;
    if (W > 0 && H > 0) {
        if (d_wide) blit(workW, d_x / 2048.0f, d_y / 512.0f, (d_x + d_w) / 2048.0f, (d_y + d_h) / 512.0f, d_h, W, H);
        else blit(workV, d_x / 1024.0f, d_y / 512.0f, (d_x + d_w) / 1024.0f, (d_y + d_h) / 512.0f, d_h, W, H);
    }
    port_window_fit_wide = 0;
    port_window_gl_swap();
    bound_wide = -1;
    return 1;
}
/* the interpolated frame as RGB8 at the PS1 resolution (the PORT_FPS60_DUMP test hook); 0 = done */
int port_gl_fps60_read(gpu_u8 *rgb, int *w, int *h) {
    if (!gl_ready) return -1;
    p_glBindFramebuffer(GL_FRAMEBUFFER, d_wide ? workWfbo : workVfbo);
    p_glPixelStorei(GL_PACK_ALIGNMENT, 4);
    p_glReadPixels(d_x * R, d_y * R, d_w * R, d_h * R, GL_RGBA, GL_UNSIGNED_BYTE, stage8);
    for (int j = 0; j < d_h; j++) {
        const u8 *s = stage8 + ((size_t)(j * R + R / 2) * d_w * R + R / 2) * 4;
        for (int i = 0; i < d_w; i++, s += 4 * R)
            for (int c = 0; c < 3; c++) { int k = (s[c] * 31 + 127) / 255; rgb[(j * d_w + i) * 3 + c] = (u8)(k << 3 | k >> 2); }
    }
    *w = d_w; *h = d_h;
    bound_wide = -1; bind_vram_fbo();
    return 0;
}

/* ------------------------------------------------------------------ start / stop */
static int gl_start(void) {
    if (port_window_gl_start() < 0) return -1;
    if (gl_load() < 0) return -1;
    const char *ren = (const char *)p_glGetString(GL_RENDERER), *ver = (const char *)p_glGetString(GL_VERSION);
    port_printf("[gpu] OpenGL: %s, %s, GLSL %s%s\n", ren ? ren : "?", ver ? ver : "?",
                p_glGetString(GL_SHADING_LANGUAGE_VERSION) ? (const char *)p_glGetString(GL_SHADING_LANGUAGE_VERSION) : "?",
                p_glTextureBarrier ? ", texture barrier" : "");
    R = port_internal_res < 1 ? 1 : port_internal_res > 4 ? 4 : port_internal_res;
    {   GLint mx = 0; p_glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mx);
        while (R > 1 && 2048 * R > mx) R--;
        if (R != port_internal_res) port_printf("[gpu] OpenGL: internal resolution %dx (asked %dx)\n", R, port_internal_res);
        else if (R > 1) port_printf("[gpu] OpenGL: internal resolution %dx (%dx%d VRAM)\n", R, 1024 * R, 512 * R);
        port_internal_res = R; }
    stage = (u16 *)SDL_malloc((size_t)2048 * 512 * (R * R + 1) * 2);
    stage8 = (u8 *)SDL_malloc((size_t)2048 * 512 * R * R * 4);
    if (!stage || !stage8) return -1;
    prog = link(vs_src, fs_src, "gpu");
    bprog = link(bvs_src, bfs_src, "blit");
    if (!prog || !bprog) return -1;
    p_glUseProgram(prog);
    p_glUniform1i(p_glGetUniformLocation(prog, "u_vram"), 0);
    u_tex = p_glGetUniformLocation(prog, "u_tex"); u_pass = p_glGetUniformLocation(prog, "u_pass");
    u_tw = p_glGetUniformLocation(prog, "u_tw"); u_setmask = p_glGetUniformLocation(prog, "u_setmask");
    u_hw = p_glGetUniformLocation(prog, "u_hw"); p_glUniform1f(u_hw, 512.0f);
    u_reps = p_glGetUniformLocation(prog, "u_reps"); u_repr = p_glGetUniformLocation(prog, "u_repr");
    p_glUniform1i(p_glGetUniformLocation(prog, "u_rep"), 1);
    p_glUniform1f(p_glGetUniformLocation(prog, "u_half"), (R / 2 + 0.5f) / R);
    if (R > 1) p_glLineWidth((float)R);
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
    p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024 * R, 512 * R, 0, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, R == 1 ? port_vram() : 0);
    if (R > 1) { memcpy(stage, port_vram(), 1024 * 512 * 2); upload_stage(0, 0, 1024, 512); }
    p_glGenFramebuffers(1, &fbo);
    p_glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    p_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vram_tex, 0);
    GLenum st = p_glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (st != GL_FRAMEBUFFER_COMPLETE) { port_printf("[gpu] OpenGL: VRAM framebuffer incomplete (0x%x)\n", (unsigned)st); return -1; }
    {   extern int port_widescreen;
        if (port_widescreen) {
            static const u16 zero[2048 * 4];
            p_glGenTextures(1, &wide_tex);
            p_glBindTexture(GL_TEXTURE_2D, wide_tex);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2048 * R, 512 * R, 0, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, 0);
            for (int y = 0; y < 512 * R; y++) p_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, y, 2048 * R, 1, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, zero);
            p_glGenFramebuffers(1, &wide_fbo);
            p_glBindFramebuffer(GL_FRAMEBUFFER, wide_fbo);
            p_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, wide_tex, 0);
            if (p_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                port_printf("[gpu] OpenGL: widescreen framebuffer incomplete: 4:3 only\n");
                wide_fbo = 0; port_widescreen = 0;
            }
            p_glBindTexture(GL_TEXTURE_2D, vram_tex);
            p_glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        }
    }
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
                                 gl_vram_written, gl_vram_read, gl_list_end, gl_present, gl_present_rgb,
                                 gl_ws_fill, gl_ws_copy_down, gl_ws_upload, gl_ws_read, gl_ws_present };
