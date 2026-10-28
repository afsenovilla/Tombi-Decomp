// FUNC 8005bdbc 48 MAIN0
// MATCHING 8005bdbc 48
// Ported from dcb_decomp src/main/psyq/libc2_memcpy.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"
#include "psyq/sys/types.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *memset(u_char *p, u_char c, int n) {
    u_char *s;

    if (p == NULL) {
        return NULL;
    }
    if (n <= 0) {
        return NULL;
    }
    s = p;
    while (n > 0) {
        *p++ = c;
        n--;
    }
    return s;
}

extern u_long D_801DDC10;

int rand(void);

void srand(unsigned int seed);

