// FUNC 8005bd58 48 MAIN0
// MATCHING 8005bd58 48
// Ported from dcb_decomp src/main/psyq/libc2_bzero.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *bzero(unsigned char *p, int n) {
    unsigned char *s;

    if (p == NULL) {
        return NULL;
    }
    if (n <= 0) {
        return NULL;
    }
    s = p;
    while (n > 0) {
        *p++ = 0;
        n--;
    }
    return s;
}
