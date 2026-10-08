// FUNC 800f7a38 116 X006
// MATCHING 800f7a38 116
#include "raw7.h"
extern void f1(int, int);
extern void f2(void *, int);
extern unsigned char tab[];
extern char anim[];
void FUN_800f7a38(char *o)
{
    f1(0x1c, 0x7f);
    U8(o, 0x9c) = 0;
    U8(o, 0xaa) = 0;
    PTR(o, 0x24) = anim;
    f2(o, 4);
    S32(o, 0x88) = 0;
    S32(o, 0x8c) = tab[S16(o, 0xb0)];
    U8(o, 6)++;
}
