// VERSION SCUS_94236 scus94236 80043B3C 496
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/game/collision.c, func_80043B3C); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
// INCLUDE_ASM("nonmatchings/main/game/collision", func_8004339C);
extern u16 D_8009B0D0;
extern s16 D_8009B0D4, D_8009B0D8, D_8009B0DC, D_8009B0E0, D_8009B0E4, D_8009B0E8;
extern u16 D_8009B0EC;
extern s16 D_8009B0F0, D_8009B0F4, D_8009B0F8;
#define x (x_ & 7)
#define xi ((s16)(x_ & 7))
#undef x
#undef xi
// INCLUDE_ASM("nonmatchings/main/game/collision", func_80043AB0);
typedef struct {
    char pad[0x44];
    s16 *p44;
} O8004065C_43AB0;
extern s32 func_80043740(GameObject *, s32, s32, s32);
// INCLUDE_ASM("nonmatchings/main/game/collision", func_80043B3C);
s32 func_80043B3C_scus94236(obj, x, y)
    GameObject *obj;
    s16 x;
    s16 y;
{
    s16 n;
    u16 fl, w;
    s32 a;
    s16 b;
    s32 lo, len, h;
    s16 d;
    char pad;

    n = *D_1F800278++;
    if (n == 0)
        return 0;
    do {
        fl = *D_1F800278++;
        n--;
        if (!(fl & 2))
            goto skip;
        if (fl & 0x10) {
        skip:
            D_1F800278 += 3;
            continue;
        }
        a = *D_1F800278++;
        b = *D_1F800278++;
        w = *D_1F800278++;
        lo = w & 0xF;
        len = (w >> 4) & 0xF;
        if (len == 0)
            continue;
        if (y < (s16)a - 0x10)
            continue;
        if (((u8)x & 7) < lo)
            continue;
        if (lo + len < ((u8)x & 7))
            continue;
        if (b == 0)
            h = a;
        else
            h = a + b * ((x - lo) % len) / len;
        d = h - y;
        if (d > 0) {
            obj->y.p.frac = 0;
            D_1F80027E = b;
            D_1F800280 = a;
            (*(s16 *)&SCRATCHPAD.hitFlags) = fl;
            obj->y.p.whole += d;
            return 1;
            do {
            } while (0);
        }
    } while (n != 0);
    return 0;
}
#endif
