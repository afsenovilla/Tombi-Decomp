// FUNC 800e962c 580 I0NFO
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    unsigned char p0[8];
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    short w20;
    unsigned char p22[0x28 - 0x22];
    unsigned short w28, w2a, w2c, w2e;
} Cam;
extern TObj *func_800183B8(void);
extern void func_8001E5F4(int a, int b);
extern void func_8001FD94(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void func_8002171C(TObj *o);
extern void func_800E8D60(TObj *o);
extern Cam *D_8009C330;
extern signed char D_8009C93A, D_8009C93F, D_8009C942;
extern unsigned char D_8009CFF9;
extern signed char D_8009D2B0;

void func_800E962C(TObj *o)
{
    TObj *n;
    Cam *c;
    switch (o->state) {
    case 0:
        o->active = 5;
        func_8002171C(o);
        *(signed char *)&o->b0f = -0xC;
        o->b6b = 1;
        o->d8c = 0;
        c = D_8009C330;
        c->b8 = 0;
        c->w20 = 0;
        D_8009C330->w2e = 0xFFFF;
        D_8009C330->w28 = 0xFFFF;
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
        o->visible = 1;
        o->animFrame = 0xE;
        o->wb2 = 0;
        o->velX = 0;
        o->d8c = 0;
        o->w22 = 0;
        D_8009C330->w2c = 0xE;
        func_800E8D60(o);
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
        o->wb6 = 0;
        o->velY -= 0x3000;
        if (o->velY < 0x200) o->velY = 0x200;
        func_8001FD94(o);
        if (o->y.p.whole >= S16(o, 0xF2)) {
            o->y.p.whole = S16(o, 0xF2);
            func_8001E5F4(0x1C, 0x7F);
            o->state++;
        }
        break;
    case 2:
        o->velX = 0;
        o->velY = 0;
        o->active = 1;
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
