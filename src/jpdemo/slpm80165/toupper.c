// JPDEMO slpm80165 80051D18 48
// Demo version of PAL main0 8005C800 (tolower.c): toupper, the twin of PAL tolower.
// Portado de psx_tomba (psyq/libc2/ctype.c, tolower); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/ctype.h"


char toupper(char c)
{
    if (islower(c)) {
        c = _toupper(c);
    }
    return c;
}
