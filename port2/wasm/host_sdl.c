/* port2/wasm host: the bridge between runtime/sdl.c, compiled natively for the host (with sdl_bridge.h), and the
 * wasm module.
 *   - the module -> sdl.c: every sdl.c entry point is an import of module "port" (wasm_compat.h); the wrappers below
 *     turn the linear addresses into native pointers and call sdl.c;
 *   - sdl.c -> the module: the runtime functions sdl.c calls (port_printf, port_fatal, port_quit, the mixer pull, the
 *     panel's input, the fps overlay) call the module's exports (rt_wasm.c); the variables go through sdl_bridge.h;
 *   - Android: what the 32-bit build does with JNI inside the runtime (launcher_exit.c's quitToLauncher, panel.c's
 *     second-screen buffer for TombiActivity) is done here, on the host side of the bridge. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "host.h"
#include "bridge_vars.h"

#define PORT_HV_DEF(name) int *port_hv_##name;
PORT_WASM_BRIDGE_VARS(PORT_HV_DEF)
int (*port_hv_port_prompt_src)[4];
static int *vars[PORT_WASM_VAR_COUNT];
static u32 scratch;                      /* rt_wasm.c's scratch area (game thread only) */
#define inst port_w2c_inst

void port_host_bridge_init(void) {
    for (int i = 0; i < PORT_WASM_VAR_COUNT; i++) vars[i] = (int *)wptr(w2c_tombi_port_wasm_var(&inst, (u32)i));
#define PORT_HV_SET(name) port_hv_##name = vars[PORT_WASM_VAR_##name];
    PORT_WASM_BRIDGE_VARS(PORT_HV_SET)
    port_hv_port_prompt_src = (int (*)[4])vars[PORT_WASM_VAR_port_prompt_src];
    scratch = w2c_tombi_port_wasm_scratch(&inst);
}
/* the Android loader reads port_frame / port_exit_status / port_clean_exit (from its own threads too): NULL until the
   module is instantiated */
__attribute__((visibility("default"))) int *port_wasm_var_ptr(int id) {
    return id >= 0 && id < PORT_WASM_VAR_COUNT ? vars[id] : 0;
}

#ifndef PORT2_WASM_HEADLESS   /* (headless_stubs.py: a host without SDL) */
/* ------------------------------------------------------------------------------- runtime functions sdl.c calls */
static void to_scratch(const char *s) {
    size_t n = strlen(s); if (n > 8000) n = 8000;
    memcpy(WMEM + scratch, s, n); WMEM[scratch + n] = 0;
}
void port_printf(const char *fmt, ...) {
    char buf[2048]; va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof buf, fmt, ap); va_end(ap);
    if (scratch && port_host_on_game_thread()) { to_scratch(buf); w2c_tombi_port_wasm_log(&inst, scratch); }
    else { fputs(buf, stderr); fflush(stderr); }
}
void port_fatal(const char *fmt, ...) {
    char buf[2048]; va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof buf, fmt, ap); va_end(ap);
    if (scratch && port_host_on_game_thread()) { to_scratch(buf); w2c_tombi_port_wasm_fatal(&inst, scratch); }
    fprintf(stderr, "[port2] FATAL: %s\n", buf);
    exit(1);
}
void port_quit(int code) { w2c_tombi_port_wasm_quit(&inst, (u32)code); exit(code); }
/* the audio device's callback (SDL's audio thread): the second instance, its own shadow stack; the game thread holds
   the device lock whenever it touches the mixer (port_audio_lock), as in the 32-bit builds */
w2c_tombi *port_host_audio_instance(void);
int port_sound_pull(short *out, int frames) {
    static u32 buf;
    w2c_tombi *a = port_host_audio_instance();
    if (!buf) buf = w2c_tombi_port_wasm_audio_buf(a);
    int done = 0;
    while (frames > 0) {
        int k = frames < 4096 ? frames : 4096;
        int n = (int)w2c_tombi_port_wasm_sound_pull(a, (u32)k);
        if (n <= 0) break;
        memcpy(out, WMEM + buf, (size_t)n * 4);
        out += 2 * n; frames -= n; done += n;
    }
    return done;
}
void port_sound_pulled(int on) { w2c_tombi_port_wasm_sound_pulled(&inst, (u32)on); }
void port_panel_touch(int x, int y, int action) { w2c_tombi_port_wasm_panel_touch(&inst, (u32)x, (u32)y, (u32)action); }
void port_panel_key(int a) { w2c_tombi_port_wasm_panel_key(&inst, (u32)a); }
void port_panel_pinch(float f, int x, int y) { w2c_tombi_port_wasm_panel_pinch(&inst, f, (u32)x, (u32)y); }
const unsigned char *port_fps_overlay(int img_h, int *w, int *h, int *changed) {
    u32 px = w2c_tombi_port_wasm_fps_overlay(&inst, (u32)img_h, scratch);
    *w = (int)wget32(scratch); *h = (int)wget32(scratch + 4); *changed = (int)wget32(scratch + 8);
    return (const unsigned char *)wptr(px);
}

/* ------------------------------------------------------------------------------- sdl.c's entry points (imports) */
void port_audio_lock(void); void port_audio_unlock(void); int port_display_size(int *w, int *h);
void port_window_start(void); void port_window_stop(void); int port_window_gl_start(void);
void port_window_gl_size(int *w, int *h); void port_window_gl_swap(void); void port_window_keys(const char *spec);
void port_window_pad_map(const char *spec); void port_window_fit(int W, int H, int h, int *x, int *y, int *w, int *hh);
void port_window_frame(const unsigned char *rgb, int w, int h); int port_window_poll_pad(unsigned short *out);
void port_window_rumble(int small, int big); int port_window_refresh(void);
void port_panel_window_frame(const unsigned char *rgba, int w, int h);

#define PW struct w2c_port *u_
/* every entry point runs on the thread's own stack (host.c port_host_call): the module's main context lives on its
   own 64 MB stack, and SDL calls Java on Android (SDL_getenv, the event pump, rumble...), which ART refuses (abort)
   from a stack it does not know.  sdl.c already wraps some of these itself; port_host_call nests. */
typedef struct { u32 a[8]; int r; } HArgs;
#define THUNK(name, expr) static void name##_h(void *p) { HArgs *A = (HArgs *)p; (void)A; expr; }
#define CALL(name, ...) ({ HArgs A_ = { { __VA_ARGS__ }, 0 }; port_host_call(name##_h, &A_); A_.r; })
THUNK(lock, port_audio_lock())
THUNK(unlock, port_audio_unlock())
THUNK(dsize, A->r = port_display_size((int *)wptr(A->a[0]), (int *)wptr(A->a[1])))
THUNK(wstart, port_window_start())
THUNK(wstop, port_window_stop())
THUNK(glstart, A->r = port_window_gl_start(); if (A->r == 0 && port_host_gl_load() < 0) A->r = -1)
THUNK(glsize, port_window_gl_size((int *)wptr(A->a[0]), (int *)wptr(A->a[1])))
THUNK(glswap, port_window_gl_swap())
THUNK(keys, port_window_keys((const char *)wptr(A->a[0])))
THUNK(padmap, port_window_pad_map(strdup((const char *)wptr(A->a[0]))))   /* (sdl.c keeps the pointer: a native copy) */
THUNK(fit, port_window_fit((int)A->a[0], (int)A->a[1], (int)A->a[2], (int *)wptr(A->a[3]), (int *)wptr(A->a[4]),
                           (int *)wptr(A->a[5]), (int *)wptr(A->a[6])))
THUNK(frame, port_window_frame((const unsigned char *)wptr(A->a[0]), (int)A->a[1], (int)A->a[2]))
THUNK(pad, unsigned short m = 0; A->r = port_window_poll_pad(&m); memcpy(WMEM + A->a[0], &m, 2))
THUNK(rumble, port_window_rumble((int)A->a[0], (int)A->a[1]))
THUNK(refresh, A->r = port_window_refresh())
void w2c_port_port_audio_lock(PW) { (void)u_; CALL(lock, 0); }
void w2c_port_port_audio_unlock(PW) { (void)u_; CALL(unlock, 0); }
u32 w2c_port_port_display_size(PW, u32 w, u32 h) { (void)u_; return (u32)CALL(dsize, w, h); }
void w2c_port_port_window_start(PW) { (void)u_; CALL(wstart, 0); }
void w2c_port_port_window_stop(PW) { (void)u_; CALL(wstop, 0); }
u32 w2c_port_port_window_gl_start(PW) { (void)u_; return (u32)CALL(glstart, 0); }
void w2c_port_port_window_gl_size(PW, u32 w, u32 h) { (void)u_; CALL(glsize, w, h); }
void w2c_port_port_window_gl_swap(PW) { (void)u_; CALL(glswap, 0); }
void w2c_port_port_window_keys(PW, u32 s) { (void)u_; CALL(keys, s); }
void w2c_port_port_window_pad_map(PW, u32 s) { (void)u_; CALL(padmap, s); }
void w2c_port_port_window_fit(PW, u32 W, u32 H, u32 h, u32 x, u32 y, u32 w, u32 hh) { (void)u_; CALL(fit, W, H, h, x, y, w, hh); }
void w2c_port_port_window_frame(PW, u32 rgb, u32 w, u32 h) { (void)u_; CALL(frame, rgb, w, h); }
u32 w2c_port_port_window_poll_pad(PW, u32 out) { (void)u_; return (u32)CALL(pad, out); }
void w2c_port_port_window_rumble(PW, u32 s, u32 b) { (void)u_; CALL(rumble, s, b); }
u32 w2c_port_port_window_refresh(PW) { (void)u_; return (u32)CALL(refresh, 0); }

#ifdef __ANDROID__
#include <jni.h>
#include <SDL2/SDL_system.h>
/* launcher_exit.c: TombiActivity.quitToLauncher() (its onDestroy sends SDL_QUIT, the event pump ends the game) */
static void launcher_quit_h(void *a) {
    int *ok = (int *)a; *ok = 0;
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv(); jobject act = (jobject)SDL_AndroidGetActivity();
    if (!env || !act) return;
    jclass cls = (*env)->GetObjectClass(env, act);
    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "quitToLauncher", "()V");
    if (mid) { (*env)->CallStaticVoidMethod(env, cls, mid); *ok = 1; }
    if ((*env)->ExceptionCheck(env)) { (*env)->ExceptionClear(env); *ok = 0; }
    (*env)->DeleteLocalRef(env, cls); (*env)->DeleteLocalRef(env, act);
}
u32 w2c_port_port_host_launcher_quit(PW) { (void)u_; int ok = 0; port_host_call(launcher_quit_h, &ok); return (u32)ok; }

/* --second-screen: the panel's picture for TombiActivity's Presentation (nativePanelFrame polls it from the UI thread);
   its layout, touches, pinches and keys are queued and handed to the module on the game thread, at the next frame */
static pthread_mutex_t pmtx = PTHREAD_MUTEX_INITIALIZER;
static unsigned char *pout; static size_t pout_cap; static int pout_w, pout_h, pout_seq, preg;
static struct { int kind, a, x, y; float f; } pq[64]; static int pq_n, lay_w, lay_h, lay_set;
static void pq_push(int kind, int a, int x, int y, float f) {
    pthread_mutex_lock(&pmtx);
    if (pq_n < 64) { pq[pq_n].kind = kind; pq[pq_n].a = a; pq[pq_n].x = x; pq[pq_n].y = y; pq[pq_n].f = f; pq_n++; }
    pthread_mutex_unlock(&pmtx);
}
static jint JNICALL n_frame(JNIEnv *env, jclass c, jobject buf, jintArray dims, jint seq) {
    (void)c; jint r = 0;
    pthread_mutex_lock(&pmtx);
    if (pout_seq != seq && pout_w) {
        void *p = (*env)->GetDirectBufferAddress(env, buf); jlong cap = (*env)->GetDirectBufferCapacity(env, buf);
        if (p && cap >= (jlong)pout_w * pout_h * 4) {
            memcpy(p, pout, (size_t)pout_w * pout_h * 4);
            jint d[2] = { pout_w, pout_h }; (*env)->SetIntArrayRegion(env, dims, 0, 2, d);
            r = pout_seq;
        }
    }
    pthread_mutex_unlock(&pmtx);
    return r;
}
static void JNICALL n_layout(JNIEnv *env, jclass c, jint w, jint h) { (void)env; (void)c; pthread_mutex_lock(&pmtx); lay_w = w; lay_h = h; lay_set = 1; pthread_mutex_unlock(&pmtx); }
static void JNICALL n_touch(JNIEnv *env, jclass c, jint x, jint y, jint action) { (void)env; (void)c; pq_push(1, action, x, y, 0); }
static void JNICALL n_pinch(JNIEnv *env, jclass c, jfloat f, jint x, jint y) { (void)env; (void)c; pq_push(2, 0, x, y, f); }
static void JNICALL n_key(JNIEnv *env, jclass c, jint a) { (void)env; (void)c; if (a > 0 && a < 16) pq_push(0, a, 0, 0, 0); }
static void panel_register_h(void *u) {
    (void)u;
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv(); jobject act = (jobject)SDL_AndroidGetActivity();
    preg = 1;
    if (!env || !act) return;
    jclass cls = (*env)->GetObjectClass(env, act);
    JNINativeMethod m[] = { { "nativePanelFrame", "(Ljava/nio/ByteBuffer;[II)I", (void *)n_frame }, { "nativePanelLayout", "(II)V", (void *)n_layout },
                            { "nativePanelTouch", "(III)V", (void *)n_touch }, { "nativePanelKey", "(I)V", (void *)n_key },
                            { "nativePanelPinch", "(FII)V", (void *)n_pinch } };
    if ((*env)->RegisterNatives(env, cls, m, 5) != 0) { (*env)->ExceptionClear(env); port_printf("[panel] RegisterNatives failed\n"); }
    else port_printf("[panel] JNI ready\n");
    (*env)->DeleteLocalRef(env, cls); (*env)->DeleteLocalRef(env, act);
}
void w2c_port_port_panel_window_frame(PW, u32 rgba, u32 w, u32 h) {
    (void)u_;
    if (!preg) port_host_call(panel_register_h, 0);
    pthread_mutex_lock(&pmtx);
    size_t need = (size_t)w * h * 4;
    if (need > pout_cap) { unsigned char *n = realloc(pout, need); if (n) { pout = n; pout_cap = need; } }
    if (need <= pout_cap) { memcpy(pout, wptr(rgba), need); pout_w = (int)w; pout_h = (int)h; if (++pout_seq == 0) pout_seq = 1; }
    int n = pq_n, lw = lay_w, lh = lay_h, ls = lay_set; typeof(pq[0]) q[64]; memcpy(q, pq, sizeof pq[0] * (size_t)n);
    pq_n = 0; lay_set = 0;
    pthread_mutex_unlock(&pmtx);
    /* (the picture is the canvas x its scale, as on the PC: the module's port_panel_touch / pinch divide by it) */
    if (ls) w2c_tombi_port_wasm_panel_size(&inst, (u32)lw, (u32)lh);
    for (int i = 0; i < n; i++) {
        if (q[i].kind == 1) port_panel_touch(q[i].x, q[i].y, q[i].a);
        else if (q[i].kind == 2) port_panel_pinch(q[i].f, q[i].x, q[i].y);
        else port_panel_key(q[i].a);
    }
}
#else
u32 w2c_port_port_host_launcher_quit(PW) { (void)u_; return 0; }
void w2c_port_port_panel_window_frame(PW, u32 rgba, u32 w, u32 h) { (void)u_; port_panel_window_frame((const unsigned char *)wptr(rgba), (int)w, (int)h); }
#endif
#endif   /* PORT2_WASM_HEADLESS */
