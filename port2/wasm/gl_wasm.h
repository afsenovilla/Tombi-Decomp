/* port2/wasm: the OpenGL / OpenGL ES API seen by shims/gpu_gl.c when it is compiled for wasm32 (in place of
 * <SDL2/SDL.h> + <SDL2/SDL_opengl.h>).  Every GL entry point is an import of module "port" served by the native host
 * (port2/wasm/host_gl.c), which calls the real GL / GLES of the context runtime/sdl.c made; pointer arguments are
 * linear-memory addresses that the host turns into native pointers (a GLsync is a small handle).  Constants: the
 * standard Khronos values gpu_gl.c uses. */
#ifndef PORT2_GL_WASM_H
#define PORT2_GL_WASM_H
#include <stdlib.h>
#include <math.h>
typedef unsigned int GLenum; typedef unsigned char GLboolean; typedef unsigned int GLbitfield; typedef int GLint;
typedef int GLsizei; typedef unsigned int GLuint; typedef float GLfloat; typedef unsigned char GLubyte; typedef char GLchar;
typedef long GLsizeiptr; typedef void *GLsync; typedef unsigned long long GLuint64;
#define APIENTRY
#define GL_ARRAY_BUFFER 0x8892
#define GL_BLEND 0xBE2
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_COMPILE_STATUS 0x8B81
#define GL_CONSTANT_COLOR 0x8001
#define GL_CULL_FACE 0xB44
#define GL_DEPTH_TEST 0xB71
#define GL_DITHER 0xBD0
#define GL_DST_ALPHA 0x304
#define GL_FALSE 0x0
#define GL_FLOAT 0x1406
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FRAMEBUFFER 0x8D40
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_LINEAR 0x2601
#define GL_LINES 0x1
#define GL_LINK_STATUS 0x8B82
#define GL_MAX_TEXTURE_SIZE 0xD33
#define GL_NEAREST 0x2600
#define GL_NO_ERROR 0x0
#define GL_ONE 0x1
#define GL_ONE_MINUS_DST_ALPHA 0x305
#define GL_ONE_MINUS_SRC_ALPHA 0x303
#define GL_PACK_ALIGNMENT 0xD05
#define GL_RENDERER 0x1F01
#define GL_RGB 0x1907
#define GL_RGB8 0x8051
#define GL_RGBA 0x1908
#define GL_RGBA8 0x8058
#define GL_SCISSOR_TEST 0xC11
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_SRC_ALPHA 0x302
#define GL_STREAM_DRAW 0x88E0
#define GL_SYNC_FLUSH_COMMANDS_BIT 0x1
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_2D 0xDE1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_TRIANGLES 0x4
#define GL_TRUE 0x1
#define GL_UNPACK_ALIGNMENT 0xCF5
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_SHORT 0x1403
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_VERSION 0x1F02
#define GL_VERTEX_SHADER 0x8B31
#define GL_WAIT_FAILED 0x911D
#define GL_ZERO 0x0

#define PGL(ret, name, args) __attribute__((import_module("port"), import_name(#name))) ret name args;
PGL(GLenum, glGetError, (void))
PGL(void, glGetIntegerv, (GLenum, GLint *))
PGL(void, glEnable, (GLenum))
PGL(void, glDisable, (GLenum))
PGL(void, glViewport, (GLint, GLint, GLsizei, GLsizei))
PGL(void, glScissor, (GLint, GLint, GLsizei, GLsizei))
PGL(void, glClearColor, (GLfloat, GLfloat, GLfloat, GLfloat))
PGL(void, glClear, (GLbitfield))
PGL(void, glColorMask, (GLboolean, GLboolean, GLboolean, GLboolean))
PGL(void, glBlendFuncSeparate, (GLenum, GLenum, GLenum, GLenum))
PGL(void, glBlendEquationSeparate, (GLenum, GLenum))
PGL(void, glBlendColor, (GLfloat, GLfloat, GLfloat, GLfloat))
PGL(void, glGenTextures, (GLsizei, GLuint *))
PGL(void, glBindTexture, (GLenum, GLuint))
PGL(void, glTexParameteri, (GLenum, GLenum, GLint))
PGL(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *))
PGL(void, glTexSubImage2D, (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *))
PGL(void, glCopyTexSubImage2D, (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))
PGL(void, glPixelStorei, (GLenum, GLint))
PGL(void, glReadPixels, (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *))
PGL(void, glDrawArrays, (GLenum, GLint, GLsizei))
PGL(void, glFlush, (void))
PGL(void, glLineWidth, (GLfloat))
PGL(void, glGenFramebuffers, (GLsizei, GLuint *))
PGL(void, glBindFramebuffer, (GLenum, GLuint))
PGL(void, glFramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint))
PGL(GLenum, glCheckFramebufferStatus, (GLenum))
PGL(void, glGenBuffers, (GLsizei, GLuint *))
PGL(void, glBindBuffer, (GLenum, GLuint))
PGL(void, glBufferData, (GLenum, GLsizeiptr, const void *, GLenum))
PGL(GLuint, glCreateShader, (GLenum))
PGL(void, glShaderSource, (GLuint, GLsizei, const GLchar *const *, const GLint *))
PGL(void, glCompileShader, (GLuint))
PGL(void, glGetShaderiv, (GLuint, GLenum, GLint *))
PGL(void, glGetShaderInfoLog, (GLuint, GLsizei, GLsizei *, GLchar *))
PGL(GLuint, glCreateProgram, (void))
PGL(void, glAttachShader, (GLuint, GLuint))
PGL(void, glBindAttribLocation, (GLuint, GLuint, const GLchar *))
PGL(void, glLinkProgram, (GLuint))
PGL(void, glGetProgramiv, (GLuint, GLenum, GLint *))
PGL(void, glGetProgramInfoLog, (GLuint, GLsizei, GLsizei *, GLchar *))
PGL(void, glUseProgram, (GLuint))
PGL(GLint, glGetUniformLocation, (GLuint, const GLchar *))
PGL(void, glUniform1i, (GLint, GLint))
PGL(void, glUniform1f, (GLint, GLfloat))
PGL(void, glUniform4f, (GLint, GLfloat, GLfloat, GLfloat, GLfloat))
PGL(void, glEnableVertexAttribArray, (GLuint))
PGL(void, glDisableVertexAttribArray, (GLuint))
PGL(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void *))   /* (an offset into the bound VBO) */
PGL(void, glActiveTexture, (GLenum))
PGL(void, glTextureBarrier, (void))                                     /* optional: port_gl_has() */
PGL(GLsync, glFenceSync, (GLenum, GLbitfield))                          /* optional; GLsync = a host handle */
PGL(GLenum, glClientWaitSync, (GLsync, GLbitfield, GLuint64))
PGL(void, glDeleteSync, (GLsync))
/* host helpers: whether the context has an entry point (EXT / ARB names included), a GL string copied into buf, the
   two SDL_GL queries gpu_gl.c makes */
PGL(int, port_gl_has, (const char *))
PGL(void, port_gl_string, (GLenum, char *, int))
PGL(int, SDL_GL_GetAttribute, (int, int *))
PGL(int, SDL_GL_ExtensionSupported, (const char *))
#undef PGL
static inline const GLubyte *glGetString(GLenum name) {
    static char buf[4][512]; static int k; char *b = buf[k++ & 3];
    port_gl_string(name, b, sizeof buf[0]);
    return b[0] ? (const GLubyte *)b : 0;
}
#define SDL_GL_CONTEXT_PROFILE_MASK 21
#define SDL_GL_CONTEXT_PROFILE_ES 4
#define SDL_atoi atoi
#define SDL_sqrt sqrt
#define SDL_malloc malloc
#endif
