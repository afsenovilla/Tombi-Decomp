// FUNC 8005d190 108 MAIN0
// MATCHING 8005d190 108
// Ported from sotn-decomp src/main/psxsdk/libc/memmove.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "types.h"

void* memmove(u_char* dest, u_char* src, int count) {
    if (dest >= src) {
        while (count-- > 0) {
            dest[count] = src[count];
        }
    } else {
        while (count-- > 0) {
            *dest++ = *src++;
        }
    }

    return dest;
}
