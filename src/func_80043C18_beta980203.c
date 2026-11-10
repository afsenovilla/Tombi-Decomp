// VERSION BETA_980203 beta980203 80043C18 1520
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_BETA_980203)
// Beta version of JP func_80042280 (src/func_80042280_slps01144.c), same body under the beta names; JP version of USA func_80044B0C (src/func_80044B0C_scus94236.c); portado de psx_tomba (main/game/collision.c, func_80044B0C); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
// beta names
extern __typeof__(D_8007EB44) D_8007BF84;
#define D_8007EB44 D_8007BF84
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
// INCLUDE_ASM("nonmatchings/main/game/collision", func_80044694);

typedef struct O_44694 { char pad[0x44]; s16 *h; } O_44694;
// INCLUDE_ASM("nonmatchings/main/game/collision", func_80044B0C);
s32 angleFromDelta(s32, s32);
s32 func_80043C18_beta980203(obj, x, y, plane)
    GameObject *obj;
    s16 x;
    u16 y;
    s16 plane;
{
    u16 x0;
    s16 n;
    s16 i;
    s16 q;
    u16 f, h, k, v;
    s16 bs;
    s16 lo, cnt;
    s32 xm;
    s16 d;
    s32 ys;
    s16 xs;
    u16 e;
    s16 ang;
    s32 w;

    n = *D_1F800278++;
    if (n == 0)
        return 0;
    {
        s32 sel;
        u8 **tbl;
        s32 base;
        s32 idx;

        i = 0;
        sel = (s16)plane;
        x0 = x;
        idx = (u8)plane;
        base = (s32)&D_8007EB44;
        tbl = (u8 **)(base + idx * 4);
        while (n != 0) {
            f = *D_1F800278++;
            h = *D_1F800278++;
            k = *D_1F800278++;
            v = *D_1F800278++;
            n--;
            bs = k;
            if (bs != 0) {
                lo = v & 0xF;
                cnt = (v >> 4) & 0xF;
                d = y - h;
                if (bs < 0) {
                    if (d < bs)
                        continue;
                    if (d > 0)
                        return 0;
                } else {
                    if (bs < d)
                        return 0;
                    if (d < 0)
                        continue;
                }
                xm = x & 7;
                xs = xm;
                if (obj->animFrame & 1) {
                    if ((f & 0x18) == 0x18) {
                        x += lo + cnt - xm;
                        if (++i >= 8)
                            return 0;
                        if (sel != -1) {
                            u8 *bb = *tbl;
                            s32 i2;
                            u32 o2 = *(u16 *)(bb + 8);
                            u16 *p;

                            bb += o2;
                            i2 = ((s16)x / 8) * 2;
                            o2 = *(u16 *)(bb + i2);
                            p = (u16 *)(bb + o2);
                            D_1F800278 = p;
                        } else {
                            D_1F800278 = (u16 *)getCollisionPlaneAt((s16)x, obj->d->p.whole);
                        }
                        n = *D_1F800278++;
                        if (n == 0)
                            return 0;
                    } else if ((f & 0x4008) == 0x4008 && (w = cnt, lo + w >= xm)) {
                        s32 hs;

                        hs = (s16)h;
                        ys = y << 16;
                        ys >>= 16;
                        ys -= hs;
                        q = (xm - lo) - w * ys / (s16)k;
                        if (q > 0)
                            return 0;
                        x -= q;
                        n = angleFromDelta(w, (s16)k);
                        ang = n;
                        D_1F80027C = ang;
                        if (ang < 0x40) {
                            D_1F80027C = ang + 0xC0;
                        } else if (ang > 0xC0) {
                            D_1F80027C = ang - 0xC0;
                        }
                        obj->h->p.whole -= x0 - x;
                        xs -= q;
                        if (!(v & 0xFF00))
                            return 1;
                        if ((s16)k <= 0) {
                            e = y - h - k;
                        } else {
                            e = y - h;
                        }
                        if (e >= 9)
                            return 1;
                        obj->y.p.whole -= e;
                        if (cnt != 0) {
                            if ((s16)k <= 0) {
                                obj->h->p.whole += lo + cnt - xs;
                            } else {
                                obj->h->p.whole += lo - xs;
                            }
                        }
                        return 2;
                    }
                } else {
                    if ((f & 0x14) == 0x14) {
                        {
                            s16 t = x - 1;

                            x = t - (xm - lo);
                        }
                        if (++i >= 8)
                            return 0;
                        if (sel != -1) {
                            u8 *bb = *tbl;
                            s32 i2;
                            u32 o2 = *(u16 *)(bb + 8);
                            u16 *p;

                            bb += o2;
                            i2 = ((s16)x / 8) * 2;
                            o2 = *(u16 *)(bb + i2);
                            p = (u16 *)(bb + o2);
                            D_1F800278 = p;
                        } else {
                            D_1F800278 = (u16 *)getCollisionPlaneAt((s16)x, obj->d->p.whole);
                        }
                        n = *D_1F800278++;
                        if (n == 0)
                            return 0;
                    } else if ((f & 0x4004) == 0x4004 && xm >= lo) {
                        s32 hs;

                        w = cnt;
                        hs = (s16)h;
                        ys = y << 16;
                        ys >>= 16;
                        ys -= hs;
                        q = (xm - lo) - w * ys / (s16)k;
                        if (q < 0)
                            return 0;
                        x -= q;
                        ang = angleFromDelta(w, (s16)k);
                        D_1F80027C = ang;
                        if (ang < 0x40) {
                            D_1F80027C = ang + 0xC0;
                        } else if (ang > 0xC0) {
                            D_1F80027C = ang - 0xC0;
                        }
                        obj->h->p.whole -= x0 - x;
                        xs -= q;
                        if (!(v & 0xFF00))
                            return 1;
                        if ((s16)k <= 0) {
                            e = y - h - k;
                        } else {
                            e = y - h;
                        }
                        if (e < 9) {
                            obj->y.p.whole -= e;
                            if (cnt != 0) {
                                if ((s16)k <= 0) {
                                    obj->h->p.whole += lo + cnt - xs;
                                } else {
                                    obj->h->p.whole += lo - xs;
                                }
                            }
                            return 2;
                        }
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
#endif
