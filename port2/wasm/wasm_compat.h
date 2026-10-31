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
#endif
