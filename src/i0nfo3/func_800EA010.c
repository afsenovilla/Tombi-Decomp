// FUNC 800ea010 584 I0NFO3
// MATCHING 800ea010 584
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FEC0(TObj *o);
extern void func_8001FD94(TObj *o);
extern void func_8001E5F4(int a, int b);
extern TObj *func_800183B8(void);
extern void func_800E96D4(TObj *o);
extern void func_8002171C(TObj *o);
typedef struct { char p0[8]; unsigned char b8; char p9[0x20 - 9]; short w20; char p22[6]; unsigned short w28, w2a, w2c, w2e; } P330;
extern P330 *D_8009C330;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CFF9;
extern unsigned char D_8009D2B0;

void func_800EA010(TObj *o)
{
    TObj *n;
    P330 *p;
    int k;
    switch (o->state) {
    case 0:
        o->active = 4;
        func_8002171C(o);
        S8(o, 0xF) = -0xC;
        o->d8c = 0;
        p = D_8009C330;
        p->b8 = 0;
        p->w20 = 0;
        D_8009C330->w2e = 0xFFFF;
        D_8009C330->w28 = 0xFFFF;
        k = 0xE;
        D_8009C330->w2a = 0xFFFF;
        o->ba4 = 0;
        o->ba5 = 0;
        o->b9c = 2;
        U8(o, 0xAC) = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0x1000;
        D_8009C330->w20 = 0;
        U8(o, 1) = 1;
        o->animFrame = k;
        o->wb2 = 0;
        o->velX = 0;
        o->d8c = 0;
        o->w22 = 0;
        D_8009C330->w2c = 0xE;
        func_800E96D4(o);
        D_8009C942 = 1;
        D_8009C93F = 1;
        o->state++;
        if (D_8009CFF9 != 0) {
            n = func_800183B8();
            if (n != 0) {
                n->active = 2;
                n->type = 0x54;
                n->subtype = 1;
            }
            o->state = 3;
        }
        break;
    case 1:
        func_8001FEC0(o);
        D_8009C330->b8 = 1;
        o->b9e = 0;
        o->wb0 = 0;
        S16(o, 0xB6) = 0;
        o->velY -= 0x3000;
        if (o->velY < 0x200) {
            o->velY = 0x200;
        }
        func_8001FD94(o);
        if (o->y.p.whole >= S16(o, 0xF2)) {
            o->y.p.whole = S16(o, 0xF2);
            func_8001E5F4(0x1C, 0x7F);
            o->state++;
        }
        break;
    case 2:
        o->active = 1;
        S8(o, 0xF) = -8;
        o->velX = 0;
        o->velY = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        D_8009C93A = 1;
        D_8009D2B0 = 0;
        D_8009C942 = 0;
        D_8009C93F = 0;
        break;
    }
}
