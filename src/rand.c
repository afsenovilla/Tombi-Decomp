// FUNC 8005bdec 48 MAIN0
// MATCHING 8005bdec 48
// Ported from dcb_decomp src/main/psyq/libc2_memcpy.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"
#include "psyq/sys/types.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *memset(u_char *p, u_char c, int n);

extern u_long RAND_SEED;

int rand(void) {
    RAND_SEED = RAND_SEED * 0x41C64E6D + 12345;
    return (RAND_SEED >> 16) & 0x7FFF;
}

void srand(unsigned int seed);

