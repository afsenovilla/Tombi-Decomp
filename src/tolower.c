// FUNC 8005c800 48 MAIN0
// MATCHING 8005c800 48
// Portado de psx_tomba (psyq/libc2/ctype.c, tolower); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/ctype.h"

char toupper(char c);

char tolower(char c)
{
    if (isupper(c)) {
        c = _tolower(c);
    }
    return c;
}
