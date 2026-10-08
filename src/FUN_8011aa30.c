// FUNC 8011aa30 224 X000
// MATCHING 8011aa30 224
#include "raw7.h"
extern char *alloc();
extern int rnd();
void FUN_8011aa30(short x, short y, short z)
{
    int i;
    char *o;
    for (i = 0; i < 3; i++) {
        o = alloc();
        if (o != 0) {
            U8(o, 0) = 1;
            U8(o, 2) = 0xb;
            **(int **)(o + 0x40) = (x + i * 0x18 - 0x18) << 16;
            S32(o, 0x14) = (y + (i - 1) * 0x10) << 16;
            **(int **)(o + 0x44) = z << 16;
            U8(o, 3) = rnd() & 3;
            U8(o, 0xc) = rnd() & 1;
            U16(o, 0x20) = rnd() & 0x1f;
        }
    }
}
