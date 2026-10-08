// FUNC 800eb29c 248 X000
#include "raw7.h"
extern int adv(void *);
extern void h(int, int);
void FUN_800eb29c(char *o)
{
    unsigned char s;
    int v;
    switch (U8(o, 3)) {
    case 0:
        s = U8(o, 6);
        if (s == 0) { U8(o, 6) = s + 1; }
        else if (s == 1) adv(o);
        break;
    case 1:
        s = U8(o, 6);
        v = 4;
        goto common;
    case 2:
        s = U8(o, 6);
        v = 0;
    common:
        if (s != 0) {
            if (s == 1 && adv(o)) U8(o, 4) = 2;
        } else {
            h(0xe, v);
            U8(o, 6)++;
        }
        break;
    }
}
