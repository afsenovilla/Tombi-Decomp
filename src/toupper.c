// FUNC 8005c7d0 48 MAIN0
// MATCHING 8005c7d0 48
// Ported from dcb_decomp src/main/psyq/libc2_setjmp.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"
#include "psyq/sys/types.h"

extern u_char D_8008070D[];

char toupper(char c) {
    if (D_8008070D[(u_char)c] & 2) {
        c -= 0x20;
    }
    return c;
}

