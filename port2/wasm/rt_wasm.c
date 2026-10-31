/* wasm32 build (port2/wasm): the host-only parts of the runtime.  The spike runs headless with the software
 * rasteriser: no SDL window or audio device (runtime/sdl.c), no OpenGL (shims/gpu_gl.c), no signal handlers
 * (runtime/lowmem.c: null-page accesses are plain linear-memory accesses redirected by patch_w2c.py, division by
 * zero gets the R3000 result there too), no watchdog thread (runtime/watchdog.c). */
#include <stdio.h>
#include "port_runtime.h"
#include "../shims/gpu_backend.h"

/* lowmem.c */
volatile unsigned long port_low_count, port_low_writes;
void port_low_second(char *buf, int size) { if (size) buf[0] = 0; }
void port_low_report(void) {}
int port_exec_from_ram_calls(void) { return 0; }
void port_dump_stack(unsigned long esp) { (void)esp; }
void port_lowmem_install(void) {}
/* watchdog.c */
static const char *trace_what; static int trace_arg, trace_frame;
void port_trace(const char *what, int arg) { trace_what = what; trace_arg = arg; trace_frame = port_frame; }
const char *port_trace_last(int *arg, int *frame) { *arg = trace_arg; *frame = trace_frame; return trace_what; }
int port_watchdog_seconds = 3;
void port_watchdog_start(void) {}
/* sdl.c */
int port_window, port_window_scale = 2, port_window_fullscreen, port_window_square, port_window_fit_wide;
void port_audio_lock(void) {}
void port_audio_unlock(void) {}
int port_display_size(int *w, int *h) { *w = *h = 0; return -1; }
void port_window_start(void) { port_printf("[wasm] no window in the wasm32 spike (headless only)\n"); port_window = 0; }
void port_window_stop(void) {}
int port_window_gl_start(void) { return -1; }
void port_window_gl_size(int *w, int *h) { *w = *h = 0; }
void port_window_gl_swap(void) {}
void port_window_keys(const char *spec) { (void)spec; }
void port_window_fit(int W, int H, int h, int *x, int *y, int *w, int *hh) { *x = *y = 0; *w = W; *hh = H; (void)h; }
void port_window_frame(const unsigned char *rgb, int w, int h) { (void)rgb; (void)w; (void)h; }
int port_window_poll_pad(unsigned short *out) { (void)out; return 0; }
int port_window_refresh(void) { return 0; }
/* gpu_gl.c: a backend whose start() fails, so gpu.c keeps the software rasteriser */
static int gl_none(void) { return -1; }
const GpuBackend port_gpu_gl = { "none", gl_none };
int port_internal_res = 1;
unsigned port_gl_rep_texture(const unsigned char *rgba, int w, int h) { (void)rgba; (void)w; (void)h; return 0; }
int port_gl_fps60_vblank(int x, int y, int w, int h, int wide, int plain) { return 0; }
int port_gl_fps60_render(float alpha) { return 0; }
int port_gl_fps60_show(void) { return 0; }
int port_gl_fps60_read(unsigned char *rgb, int *w, int *h) { return 1; }
int port_gl_frame_hd(unsigned char *rgb, int x, int y, int w, int h, int wide, int *ow, int *oh) { return 0; }

/* the host passes its working directory as PWD (port2/wasm/host.c): relative paths as on the PC build */
#include <stdlib.h>
#include <unistd.h>
__attribute__((constructor)) static void port_wasm_cwd(void) { const char *p = getenv("PWD"); if (p && *p) chdir(p); }
