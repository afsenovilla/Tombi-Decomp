// FUNC 800ee18c 208 X002
// MATCHING 800ee18c 208
#include "raw7.h"
extern unsigned short g;
extern void f1(), f2(), f3();
void FUN_800ee18c(char *o)
{
    unsigned char s = U8(o, 4);
    switch (s) {
    case 0:
        U8(o, 0) = 2;
        U8(o, 0xa) = 0x10;
        U8(o, 4)++;
        break;
    case 1:
        if (g == 4 || g == 0xc) {
            U8(o, 1) = 1;
            f1();
        } else {
            f2();
        }
        break;
    case 2:
        U8(o, 4) = s + 1;
        break;
    case 3:
        f3();
        break;
    }
}
