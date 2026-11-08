// VERSION SLPS_01144 info3 800EB868 576
// JP version of PAL i0nfo2 800eac60 (topdownPlayerLandStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
extern void func_80020E38(TObj *o);
extern void func_80020D0C(TObj *o);
extern void func_8001E458(int a, int b);
extern TObj *func_8001798C(void);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void func_800220F0(TObj *o);
typedef struct { char p0[8]; unsigned char b8; char p9[0x20 - 9]; short w20; char p22[6]; unsigned short w28, w2a, w2c, w2e; } P330;
extern P330 *D_8009DEE0;
extern unsigned char D_8009E4E2;
extern unsigned char D_8009E4E7;
extern unsigned char D_8009E4EA;
extern unsigned char D_8009EBA1;
extern unsigned char D_8009EE58;

void func_800EB868(TObj *o)
{
    TObj *n;
    P330 *p;
    int k;
    switch (o->state) {
    case 0:
        o->active = 4;
        func_800220F0(o);
        S8(o, 0xF) = -0xC;
        o->d8c = 0;
        p = D_8009DEE0;
        p->b8 = 0;
        p->w20 = 0;
        D_8009DEE0->w2e = 0xFFFF;
        D_8009DEE0->w28 = 0xFFFF;
        k = 0xE;
        D_8009DEE0->w2a = 0xFFFF;
        o->ba4 = 0;
        o->ba5 = 0;
        o->b9c = 2;
        U8(o, 0xAC) = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0x1000;
        D_8009DEE0->w20 = 0;
        U8(o, 1) = 1;
        o->animFrame = k;
        o->wb2 = 0;
        o->velX = 0;
        o->d8c = 0;
        o->w22 = 0;
        D_8009DEE0->w2c = 0xE;
        overheadPlayerSetMoveAnim(o);
        D_8009E4EA = 1;
        D_8009E4E7 = 1;
        o->state++;
        if (D_8009EBA1 != 0) {
            n = func_8001798C();
            if (n != 0) {
                n->active = 2;
                n->type = 0x54;
                n->subtype = 1;
            }
            o->state = 3;
        }
        break;
    case 1:
        func_80020E38(o);
        D_8009DEE0->b8 = 1;
        o->b9e = 0;
        o->wb0 = 0;
        S16(o, 0xB6) = 0;
        o->velY -= 0x3000;
        if (o->velY < 0x200) {
            o->velY = 0x200;
        }
        func_80020D0C(o);
        if (o->y.p.whole >= S16(o, 0xE2)) {
            o->y.p.whole = S16(o, 0xE2);
            func_8001E458(0x1C, 0x7F);
            o->state++;
        }
        break;
    case 2:
        o->active = 1;
        o->velX = 0;
        o->velY = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        D_8009E4E2 = 1;
        D_8009EE58 = 0;
        D_8009E4EA = 0;
        D_8009E4E7 = 0;
        break;
    }
}
#endif
