/* wasm32 build (port2/wasm): the host-only parts of the runtime, and the module's side of the bridge to the native host.
 *  - runtime/lowmem.c (signal handlers): not needed; null-page accesses are linear-memory accesses redirected by
 *    patch_w2c.py, division by zero gets the R3000 result there too.  runtime/watchdog.c: no watchdog thread.
 *  - runtime/sdl.c is compiled for the HOST (natively, with sdl_bridge.h): its entry points are imports of module
 *    "port" (wasm_compat.h), the variables it shares with the game are defined here (they live in linear memory)
 *    and the host reaches them through port_wasm_var().
 *  - shims/gpu_gl.c is compiled into the module; its GL calls are imports (gl_wasm.h, served by host_gl.c).
 *  - exports for the host: the mixer pull of the audio callback (run on a second instance with its own shadow stack,
 *    host.c), log / fatal / quit, the panel's input, the fps overlay. */
#include <stdio.h>
#include <string.h>
#include "port_runtime.h"
#include "../shims/gpu_backend.h"
#include "bridge_vars.h"

#define EXPORT(name) __attribute__((export_name(#name), used))

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

/* sdl.c's variables (the host's sdl.c uses them through port_wasm_var) */
int port_window, port_window_scale = 2, port_window_square, port_window_fullscreen, port_window_fit_wide, port_window_fill;
int port_rumble_pct = 100;

extern int port_hfr_fps, port_prompt_option, port_prompt_style, port_prompt_map_fixed, port_prompt_src[4], port_dualshock,
    port_pad_lx, port_pad_ly, port_debug_key, port_exit_status, port_clean_exit, port_renderer, port_widescreen;
EXPORT(port_wasm_var) int *port_wasm_var(int i) {
#define PORT_WASM_VAR_CASE(name) case PORT_WASM_VAR_##name: return &name;
    switch (i) {
    PORT_WASM_BRIDGE_VARS(PORT_WASM_VAR_CASE)
    case PORT_WASM_VAR_port_prompt_src: return port_prompt_src;
    }
    return 0;
}

/* a scratch area the host writes call arguments into (game thread only) and the audio callback's buffers */
static char scratch[8192] __attribute__((aligned(16)));
EXPORT(port_wasm_scratch) char *port_wasm_scratch(void) { return scratch; }
static short audio_buf[4096 * 2] __attribute__((aligned(16)));
static char audio_stack[256 * 1024] __attribute__((aligned(16)));
EXPORT(port_wasm_audio_buf) short *port_wasm_audio_buf(void) { return audio_buf; }
EXPORT(port_wasm_audio_stack_top) char *port_wasm_audio_stack_top(void) { return audio_stack + sizeof audio_stack; }
EXPORT(port_wasm_sound_pull) int port_wasm_sound_pull(int frames) { return port_sound_pull(audio_buf, frames > 4096 ? 4096 : frames); }
EXPORT(port_wasm_sound_pulled) void port_wasm_sound_pulled(int on) { port_sound_pulled(on); }

EXPORT(port_wasm_log) void port_wasm_log(const char *s) { port_printf("%s", s); }
EXPORT(port_wasm_fatal) void port_wasm_fatal(const char *s) { port_fatal("%s", s); }
EXPORT(port_wasm_quit) void port_wasm_quit(int code) { port_quit(code); }

void port_panel_touch(int x, int y, int action), port_panel_key(int a), port_panel_pinch(float f, int x, int y), port_panel_size(int w, int h);
EXPORT(port_wasm_panel_touch) void port_wasm_panel_touch(int x, int y, int action) { port_panel_touch(x, y, action); }
EXPORT(port_wasm_panel_key) void port_wasm_panel_key(int a) { port_panel_key(a); }
EXPORT(port_wasm_panel_pinch) void port_wasm_panel_pinch(float f, int x, int y) { port_panel_pinch(f, x, y); }
EXPORT(port_wasm_panel_size) void port_wasm_panel_size(int w, int h) { port_panel_size(w, h); }
extern int port_panel_canvas_out;
EXPORT(port_wasm_panel_canvas_out) void port_wasm_panel_canvas_out(int on) { port_panel_canvas_out = on; }
extern const unsigned char *port_fps_overlay(int img_h, int *w, int *h, int *changed);
/* out[0..2] = w, h, changed; returns the RGBA8 pixels (linear address) or 0 */
EXPORT(port_wasm_fps_overlay) const unsigned char *port_wasm_fps_overlay(int img_h, int *out) { return port_fps_overlay(img_h, &out[0], &out[1], &out[2]); }

/* the host passes its working directory as PWD (port2/wasm/host.c): relative paths as on the PC build */
#include <stdlib.h>
#include <unistd.h>
__attribute__((constructor)) static void port_wasm_cwd(void) { const char *p = getenv("PWD"); if (p && *p) chdir(p); }
