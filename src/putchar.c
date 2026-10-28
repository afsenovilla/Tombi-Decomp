// FUNC 8005c880 180 MAIN0
// MATCHING 8005c880 180
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libc2/putchar.c, putchar); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/ctype.h"

extern u32 D_8009BDD8;

void putchar(char c) {
    if (c != '\t') {
        if (c == '\n') {
            putchar('\r');
            D_8009BDD8 = 0;
            write(1, &c, 1);
            return;
        }
    } else {
        do {
            putchar(' ');
        } while (D_8009BDD8 & 0x7);
        return;
    }
    if (isprint(c)) {
        D_8009BDD8++;
    }
    write(1, &c, 1);
}
