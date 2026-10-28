// FUNC 80066b78 80 MAIN0
// MATCHING 80066b78 80
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libc2/puts.c, puts); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

void putchar(char c);

void puts(char* s) {
    char c;

    if (s == NULL) {
        s = "<NULL>";
    }
    while ((c = *s++) != 0) {
        putchar(c);
    }
}
