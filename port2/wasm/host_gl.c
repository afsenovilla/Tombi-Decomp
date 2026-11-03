/* port2/wasm host: the GL imports of shims/gpu_gl.c (gl_wasm.h).  The entry points are loaded with
 * SDL_GL_GetProcAddress once runtime/sdl.c has made the context current (OpenGL 2.1+ on the PC, OpenGL ES on Android),
 * the same lookups gpu_gl.c makes in the 32-bit builds; a pointer argument is a linear-memory address (0 stays NULL;
 * glVertexAttribPointer's is an offset into the bound buffer and is passed through), a GLsync is a small handle. */
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "host.h"

#ifndef APIENTRY
#define APIENTRY
#endif
typedef void *PSync;
#define GL_LIST(X) \
    X(GLenum, glGetError, (void)) X(const GLubyte *, glGetString, (GLenum)) X(void, glGetIntegerv, (GLenum, GLint *)) \
    X(void, glEnable, (GLenum)) X(void, glDisable, (GLenum)) X(void, glViewport, (GLint, GLint, GLsizei, GLsizei)) \
    X(void, glScissor, (GLint, GLint, GLsizei, GLsizei)) X(void, glClearColor, (GLfloat, GLfloat, GLfloat, GLfloat)) \
    X(void, glClear, (GLbitfield)) X(void, glColorMask, (GLboolean, GLboolean, GLboolean, GLboolean)) \
    X(void, glBlendFuncSeparate, (GLenum, GLenum, GLenum, GLenum)) X(void, glBlendEquationSeparate, (GLenum, GLenum)) \
    X(void, glBlendColor, (GLfloat, GLfloat, GLfloat, GLfloat)) X(void, glGenTextures, (GLsizei, GLuint *)) \
    X(void, glBindTexture, (GLenum, GLuint)) X(void, glTexParameteri, (GLenum, GLenum, GLint)) \
    X(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *)) \
    X(void, glTexSubImage2D, (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *)) \
    X(void, glCopyTexSubImage2D, (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei)) \
    X(void, glPixelStorei, (GLenum, GLint)) X(void, glReadPixels, (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *)) \
    X(void, glDrawArrays, (GLenum, GLint, GLsizei)) X(void, glFlush, (void)) X(void, glLineWidth, (GLfloat)) \
    X(void, glGenFramebuffers, (GLsizei, GLuint *)) X(void, glBindFramebuffer, (GLenum, GLuint)) \
    X(void, glFramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint)) X(GLenum, glCheckFramebufferStatus, (GLenum)) \
    X(void, glGenBuffers, (GLsizei, GLuint *)) X(void, glBindBuffer, (GLenum, GLuint)) \
    X(void, glBufferData, (GLenum, GLsizeiptr, const void *, GLenum)) X(GLuint, glCreateShader, (GLenum)) \
    X(void, glShaderSource, (GLuint, GLsizei, const GLchar *const *, const GLint *)) X(void, glCompileShader, (GLuint)) \
    X(void, glGetShaderiv, (GLuint, GLenum, GLint *)) X(void, glGetShaderInfoLog, (GLuint, GLsizei, GLsizei *, GLchar *)) \
    X(GLuint, glCreateProgram, (void)) X(void, glAttachShader, (GLuint, GLuint)) \
    X(void, glBindAttribLocation, (GLuint, GLuint, const GLchar *)) X(void, glLinkProgram, (GLuint)) \
    X(void, glGetProgramiv, (GLuint, GLenum, GLint *)) X(void, glGetProgramInfoLog, (GLuint, GLsizei, GLsizei *, GLchar *)) \
    X(void, glUseProgram, (GLuint)) X(GLint, glGetUniformLocation, (GLuint, const GLchar *)) X(void, glUniform1i, (GLint, GLint)) \
    X(void, glUniform1f, (GLint, GLfloat)) X(void, glUniform4f, (GLint, GLfloat, GLfloat, GLfloat, GLfloat)) \
    X(void, glEnableVertexAttribArray, (GLuint)) X(void, glDisableVertexAttribArray, (GLuint)) \
    X(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void *)) X(void, glActiveTexture, (GLenum))
#define GL_OPT(X) X(void, glTextureBarrier, (void)) X(PSync, glFenceSync, (GLenum, GLbitfield)) \
    X(GLenum, glClientWaitSync, (PSync, GLbitfield, unsigned long long)) X(void, glDeleteSync, (PSync))
#define DECL(ret, name, args) static ret (APIENTRY *q_##name) args;
GL_LIST(DECL)
GL_OPT(DECL)

static void *lookup(const char *name) {
    char n[96]; void *p = SDL_GL_GetProcAddress(name);
    if (!p) { snprintf(n, sizeof n, "%sEXT", name); p = SDL_GL_GetProcAddress(n); }
    if (!p) { snprintf(n, sizeof n, "%sARB", name); p = SDL_GL_GetProcAddress(n); }
    return p;
}
int port_host_gl_load(void) {
#define LOAD(ret, name, args) q_##name = (ret (APIENTRY *) args)lookup(#name);
    GL_LIST(LOAD)
    GL_OPT(LOAD)
    if (!q_glTextureBarrier) q_glTextureBarrier = (void (APIENTRY *)(void))SDL_GL_GetProcAddress("glTextureBarrierNV");
    return 0;     /* (gpu_gl.c asks port_gl_has for each one and reports what is missing) */
}
#define PW struct w2c_port *u_
u32 w2c_port_port_gl_has(PW, u32 name) {
    (void)u_; const char *n = (const char *)wptr(name);
#define HAS(ret, nm, args) if (!strcmp(n, #nm)) return q_##nm != 0;
    GL_LIST(HAS)
    GL_OPT(HAS)
    if (!strcmp(n, "glTextureBarrierNV")) return q_glTextureBarrier != 0;
    return 0;
}
void w2c_port_port_gl_string(PW, u32 name, u32 buf, u32 cap) {
    (void)u_; const char *s = q_glGetString ? (const char *)q_glGetString(name) : 0;
    char *b = (char *)wptr(buf); if (!cap) return;
    snprintf(b, cap, "%s", s ? s : "");
}
u32 w2c_port_SDL_GL_GetAttribute(PW, u32 attr, u32 out) {
    (void)u_; int v = 0; int r = SDL_GL_GetAttribute((SDL_GLattr)attr, &v); wput32(out, (u32)v); return (u32)r;
}
u32 w2c_port_SDL_GL_ExtensionSupported(PW, u32 name) { (void)u_; return SDL_GL_ExtensionSupported((const char *)wptr(name)) ? 1 : 0; }

#define I(x) ((GLint)(x))
#define F(x) (x)
u32 w2c_port_glGetError(PW) { (void)u_; return q_glGetError(); }
void w2c_port_glGetIntegerv(PW, u32 e, u32 p) { (void)u_; q_glGetIntegerv(e, (GLint *)wptr(p)); }
void w2c_port_glEnable(PW, u32 e) { (void)u_; q_glEnable(e); }
void w2c_port_glDisable(PW, u32 e) { (void)u_; q_glDisable(e); }
void w2c_port_glViewport(PW, u32 x, u32 y, u32 w, u32 h) { (void)u_; q_glViewport(I(x), I(y), I(w), I(h)); }
void w2c_port_glScissor(PW, u32 x, u32 y, u32 w, u32 h) { (void)u_; q_glScissor(I(x), I(y), I(w), I(h)); }
void w2c_port_glClearColor(PW, f32 r, f32 g, f32 b, f32 a) { (void)u_; q_glClearColor(r, g, b, a); }
void w2c_port_glClear(PW, u32 m) { (void)u_; q_glClear(m); }
void w2c_port_glColorMask(PW, u32 r, u32 g, u32 b, u32 a) { (void)u_; q_glColorMask((GLboolean)r, (GLboolean)g, (GLboolean)b, (GLboolean)a); }
void w2c_port_glBlendFuncSeparate(PW, u32 a, u32 b, u32 c, u32 d) { (void)u_; q_glBlendFuncSeparate(a, b, c, d); }
void w2c_port_glBlendEquationSeparate(PW, u32 a, u32 b) { (void)u_; q_glBlendEquationSeparate(a, b); }
void w2c_port_glBlendColor(PW, f32 r, f32 g, f32 b, f32 a) { (void)u_; q_glBlendColor(r, g, b, a); }
void w2c_port_glGenTextures(PW, u32 n, u32 p) { (void)u_; q_glGenTextures(I(n), (GLuint *)wptr(p)); }
void w2c_port_glBindTexture(PW, u32 t, u32 x) { (void)u_; q_glBindTexture(t, x); }
void w2c_port_glTexParameteri(PW, u32 t, u32 p, u32 v) { (void)u_; q_glTexParameteri(t, p, I(v)); }
void w2c_port_glTexImage2D(PW, u32 t, u32 l, u32 ifmt, u32 w, u32 h, u32 b, u32 f, u32 ty, u32 px) {
    (void)u_; q_glTexImage2D(t, I(l), I(ifmt), I(w), I(h), I(b), f, ty, wptr(px));
}
void w2c_port_glTexSubImage2D(PW, u32 t, u32 l, u32 x, u32 y, u32 w, u32 h, u32 f, u32 ty, u32 px) {
    (void)u_; q_glTexSubImage2D(t, I(l), I(x), I(y), I(w), I(h), f, ty, wptr(px));
}
void w2c_port_glCopyTexSubImage2D(PW, u32 t, u32 l, u32 xo, u32 yo, u32 x, u32 y, u32 w, u32 h) {
    (void)u_; q_glCopyTexSubImage2D(t, I(l), I(xo), I(yo), I(x), I(y), I(w), I(h));
}
void w2c_port_glPixelStorei(PW, u32 p, u32 v) { (void)u_; q_glPixelStorei(p, I(v)); }
void w2c_port_glReadPixels(PW, u32 x, u32 y, u32 w, u32 h, u32 f, u32 t, u32 px) { (void)u_; q_glReadPixels(I(x), I(y), I(w), I(h), f, t, wptr(px)); }
void w2c_port_glDrawArrays(PW, u32 m, u32 first, u32 n) { (void)u_; q_glDrawArrays(m, I(first), I(n)); }
void w2c_port_glFlush(PW) { (void)u_; q_glFlush(); }
void w2c_port_glLineWidth(PW, f32 w) { (void)u_; q_glLineWidth(w); }
void w2c_port_glGenFramebuffers(PW, u32 n, u32 p) { (void)u_; q_glGenFramebuffers(I(n), (GLuint *)wptr(p)); }
void w2c_port_glBindFramebuffer(PW, u32 t, u32 f) { (void)u_; q_glBindFramebuffer(t, f); }
void w2c_port_glFramebufferTexture2D(PW, u32 t, u32 a, u32 tt, u32 tex, u32 l) { (void)u_; q_glFramebufferTexture2D(t, a, tt, tex, I(l)); }
u32 w2c_port_glCheckFramebufferStatus(PW, u32 t) { (void)u_; return q_glCheckFramebufferStatus(t); }
void w2c_port_glGenBuffers(PW, u32 n, u32 p) { (void)u_; q_glGenBuffers(I(n), (GLuint *)wptr(p)); }
void w2c_port_glBindBuffer(PW, u32 t, u32 b) { (void)u_; q_glBindBuffer(t, b); }
void w2c_port_glBufferData(PW, u32 t, u32 size, u32 data, u32 usage) { (void)u_; q_glBufferData(t, (GLsizeiptr)(int32_t)size, wptr(data), usage); }
u32 w2c_port_glCreateShader(PW, u32 t) { (void)u_; return q_glCreateShader(t); }
void w2c_port_glShaderSource(PW, u32 s, u32 n, u32 strs, u32 lens) {
    (void)u_; const GLchar *p[16]; GLint l[16]; int k = (int)n > 16 ? 16 : (int)n;
    for (int i = 0; i < k; i++) { p[i] = (const GLchar *)wptr(wget32(strs + 4 * i)); if (lens) l[i] = (GLint)wget32(lens + 4 * i); }
    q_glShaderSource(s, k, p, lens ? l : 0);
}
void w2c_port_glCompileShader(PW, u32 s) { (void)u_; q_glCompileShader(s); }
void w2c_port_glGetShaderiv(PW, u32 s, u32 p, u32 out) { (void)u_; q_glGetShaderiv(s, p, (GLint *)wptr(out)); }
void w2c_port_glGetShaderInfoLog(PW, u32 s, u32 n, u32 len, u32 buf) { (void)u_; q_glGetShaderInfoLog(s, I(n), (GLsizei *)wptr(len), (GLchar *)wptr(buf)); }
u32 w2c_port_glCreateProgram(PW) { (void)u_; return q_glCreateProgram(); }
void w2c_port_glAttachShader(PW, u32 p, u32 s) { (void)u_; q_glAttachShader(p, s); }
void w2c_port_glBindAttribLocation(PW, u32 p, u32 i, u32 name) { (void)u_; q_glBindAttribLocation(p, i, (const GLchar *)wptr(name)); }
void w2c_port_glLinkProgram(PW, u32 p) { (void)u_; q_glLinkProgram(p); }
void w2c_port_glGetProgramiv(PW, u32 p, u32 e, u32 out) { (void)u_; q_glGetProgramiv(p, e, (GLint *)wptr(out)); }
void w2c_port_glGetProgramInfoLog(PW, u32 p, u32 n, u32 len, u32 buf) { (void)u_; q_glGetProgramInfoLog(p, I(n), (GLsizei *)wptr(len), (GLchar *)wptr(buf)); }
void w2c_port_glUseProgram(PW, u32 p) { (void)u_; q_glUseProgram(p); }
u32 w2c_port_glGetUniformLocation(PW, u32 p, u32 name) { (void)u_; return (u32)q_glGetUniformLocation(p, (const GLchar *)wptr(name)); }
void w2c_port_glUniform1i(PW, u32 l, u32 v) { (void)u_; q_glUniform1i(I(l), I(v)); }
void w2c_port_glUniform1f(PW, u32 l, f32 v) { (void)u_; q_glUniform1f(I(l), v); }
void w2c_port_glUniform4f(PW, u32 l, f32 a, f32 b, f32 c, f32 d) { (void)u_; q_glUniform4f(I(l), a, b, c, d); }
void w2c_port_glEnableVertexAttribArray(PW, u32 i) { (void)u_; q_glEnableVertexAttribArray(i); }
void w2c_port_glDisableVertexAttribArray(PW, u32 i) { (void)u_; q_glDisableVertexAttribArray(i); }
void w2c_port_glVertexAttribPointer(PW, u32 i, u32 n, u32 t, u32 norm, u32 stride, u32 off) {
    (void)u_; q_glVertexAttribPointer(i, I(n), t, (GLboolean)norm, I(stride), (const void *)(uintptr_t)off);
}
void w2c_port_glActiveTexture(PW, u32 t) { (void)u_; q_glActiveTexture(t); }
void w2c_port_glTextureBarrier(PW) { (void)u_; if (q_glTextureBarrier) q_glTextureBarrier(); }
/* GLsync handles: 1..16 */
static PSync syncs[17];
u32 w2c_port_glFenceSync(PW, u32 cond, u32 flags) {
    (void)u_;
    for (u32 i = 1; i < 17; i++) if (!syncs[i]) { syncs[i] = q_glFenceSync(cond, flags); return syncs[i] ? i : 0; }
    return 0;
}
u32 w2c_port_glClientWaitSync(PW, u32 s, u32 flags, u64 timeout) {
    (void)u_; if (s < 1 || s > 16 || !syncs[s]) return 0x911D;   /* GL_WAIT_FAILED */
    return q_glClientWaitSync(syncs[s], flags, timeout);
}
void w2c_port_glDeleteSync(PW, u32 s) { (void)u_; if (s >= 1 && s <= 16 && syncs[s]) { q_glDeleteSync(syncs[s]); syncs[s] = 0; } }
