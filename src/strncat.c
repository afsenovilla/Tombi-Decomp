// FUNC 8005bfac 116 MAIN0
// MATCHING 8005bfac 116
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libc2/strncat.c, strncat); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

u_char* strncat(u_char* dest, u_char* src, int count) {
    u_char* temp;
    
    if (dest == NULL) {
        return NULL;
    }
    temp = dest;
    if (src == NULL) return NULL;
    while (*dest++) {
    }
    dest--;
    while ((*dest++ = *src++) != 0) {
        if (--count < 0) {
            dest[-1] = '\x00';
            break;
        }   
    }
    return temp;
}
