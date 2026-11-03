/* Force-included (-include) into the runtime and shims when port2 is compiled for wasm32 (port2/wasm/build.sh).
 * It papers over what the PC build takes from glibc/Linux without touching the shared runtime files. */
#ifndef PORT2_WASM_COMPAT_H
#define PORT2_WASM_COMPAT_H
#include <stddef.h>
#include <wasi/api.h>
/* main.c's raw log write: syscall(SYS_write, 2, buf, n) -> WASI fd_write (never the game's Psy-Q `write`) */
#define SYS_write 4
static inline long port_wasm_syscall_write(long nr, long fd, const void *b, long n) {
    (void)nr; __wasi_ciovec_t v = { (const uint8_t *)b, (size_t)n }; size_t w = 0;
    return __wasi_fd_write((__wasi_fd_t)fd, &v, 1, &w) ? -1 : (long)w;
}
#define syscall port_wasm_syscall_write
/* the Psy-Q bcopy/bzero/memcpy of shims/libc.c and psyq.c have the PS1 prototypes: hide wasi-libc's declarations */
#define bcopy port_wasm_libc_bcopy
#define bzero port_wasm_libc_bzero
#include <strings.h>
#undef bcopy
#undef bzero
/* runtime/sdl.c runs on the native host (port2/wasm/host_sdl.c): its entry points are imports of module "port" */
#define PORT_SDL_IMPORT(ret, name, args) __attribute__((import_module("port"), import_name(#name))) ret name args;
PORT_SDL_IMPORT(void, port_audio_lock, (void))
PORT_SDL_IMPORT(void, port_audio_unlock, (void))
PORT_SDL_IMPORT(int, port_display_size, (int *w, int *h))
PORT_SDL_IMPORT(void, port_window_start, (void))
PORT_SDL_IMPORT(void, port_window_stop, (void))
PORT_SDL_IMPORT(int, port_window_gl_start, (void))
PORT_SDL_IMPORT(void, port_window_gl_size, (int *w, int *h))
PORT_SDL_IMPORT(void, port_window_gl_swap, (void))
PORT_SDL_IMPORT(void, port_window_keys, (const char *spec))
PORT_SDL_IMPORT(void, port_window_pad_map, (const char *spec))
PORT_SDL_IMPORT(void, port_window_fit, (int W, int H, int h, int *x, int *y, int *w, int *hh))
PORT_SDL_IMPORT(void, port_window_frame, (const unsigned char *rgb, int w, int h))
PORT_SDL_IMPORT(int, port_window_poll_pad, (unsigned short *out))
PORT_SDL_IMPORT(void, port_window_rumble, (int small, int big))
PORT_SDL_IMPORT(int, port_window_refresh, (void))
PORT_SDL_IMPORT(void, port_panel_window_frame, (const unsigned char *rgba, int w, int h))
/* runtime/launcher_exit.c: 1 when the host ended the game its own way (Android: TombiActivity.quitToLauncher) */
PORT_SDL_IMPORT(int, port_host_launcher_quit, (void))
#undef PORT_SDL_IMPORT
#endif
