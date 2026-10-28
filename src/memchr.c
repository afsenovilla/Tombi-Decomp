// FUNC 8005c830 80 MAIN0
// MATCHING 8005c830 80
// Ported from dcb_decomp src/main/psyq/libc2_memchr.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"
#include "psyq/sys/types.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *memchr(unsigned char *s, int c, int n) {
    if (s == NULL || n <= 0) {
        return NULL;
    }
    while (--n >= 0) {
        if (*s++ == (u_char)c) {
            return s - 1;
        }
    }
    return NULL;
}
