// JPDEMO x00 8011BC38 200
// Demo version of PAL x000 80125984 (weaponHitCat4Type02.c): also flags the target, spawns a hit effect and plays sound 5;
// no type 10 exception.
#include "raw7.h"
extern short FUN_8004461c(char *, char *);
extern void FUN_800428c0(char *, char *);
extern short DAT_1f80019e;
extern void func_80020054(int, int, int, int);
extern void func_8001E3D8(int);
void weaponHitCat4Type02(char *a, char *b)
{
    if (S32(b, 0x94) == 0 && FUN_8004461c(a, b) >= 0) {
        FUN_800428c0(a, b);
        U8(b, 0x68) = 1;
        func_80020054(1, *(short *)(a + 0x12), *(short *)(a + 0x16), *(short *)(a + 0x1a));
        if (U8(a, 2) != 1) {
            unsigned short x = U16(a, 0x2e);
            U16(b, 0x2e) = x < 4 ? x & 1 : 3;
            func_8001E3D8(5);
        } else {
            U16(b, 0x2e) = U16(a, 0x2e) & 1;
        }
        DAT_1f80019e = 0;
    }
}
