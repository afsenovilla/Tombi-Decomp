// FUNC 8011e4ec 168 X004
// MATCHING 8011e4ec 168
#include "raw7.h"
extern char *FUN_80018568();
void FUN_8011e4ec(char *p, char *q, int c)
{
    char *o = FUN_80018568();
    if (o != 0) {
        U8(o, 0) = 4;
        U8(o, 2) = 7;
        U8(o, 3) = c;
        U8(o, 0xc) = 0;
        *(signed char *)&U8(o, 0xf) = -10;
        S16(o, 0x2e) = 0;
        *(int *)(o + 0x10) = *(short *)(q + 2) << 16;
        *(int *)(o + 0x14) = *(short *)(q + 6) << 16;
        *(int *)(o + 0x18) = *(short *)(q + 10) << 16;
        *(int *)(o + 0x94) = (int)p;
        S16(o, 0xb4) = *(unsigned short *)(p + 0xb6);
    }
}
