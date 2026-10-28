// FUNC 8005bed4 100 MAIN0
// MATCHING 8005bed4 100
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libc2/strcmp.c, strcmp); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

int strcmp(const char *a, const char *b)
{
    if ((a == NULL) || (b == NULL)) {
        if (a == b) {
            return 0;
        }
        return (a == NULL) ? -1 : 1;
    }
    while (*a == *b++) {
        if (*a++ == '\0') {
            return 0;
        }
    }
    b--;
    return *a - *b;
}
