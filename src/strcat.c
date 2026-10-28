// FUNC 8005be2c 168 MAIN0
// MATCHING 8005be2c 168
// Ported from dcb_decomp src/main/psyq/libc2_strcat.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"

char *strcat(char *dst, char *src) {
    char *ret;

    if (dst == NULL || src == NULL) {
        return NULL;
    }
    if (dst + strlen(dst) != src + strlen(src)) {
        ret = dst;
        while (*dst++ != 0) {
        }
        dst--;
        while ((*dst++ = *src++) != 0) {
        }
        return ret;
    }
    return NULL;
}

