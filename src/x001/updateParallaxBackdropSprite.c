// FUNC 801210e8 440 X001
// MATCHING 801210e8 440
#include "spad.h"
#include "TOBJ.H"

extern void *D_8013E6B0[];
void func_80018DA4(TObj *o);
void freeObjectLayer7(TObj *o);

void updateParallaxBackdropSprite(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int x, d;
        o->b0d = 0;
        x = o->a.p.whole;
        o->w1e = 0xb;
        o->y.p.whole += 0x180;
        o->anim = D_8013E6B0[o->b0c];
        d = ((int*)&SPAD.i2d4)[0];
        o->b0f = 0;
        o->d30 = x;
        o->b.p.whole = 0;
        o->d34 = o->y.p.whole;
        o->d38 = 0x840;
        o->b04++;
        o->d3c = d;
    }
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            o->visible = 1;
            func_80018DA4(o);
            break;
        case 1: {
            short t; unsigned int u;
            u = o->d30; u -= (*(unsigned short*)((char*)&SPAD.i174 + 2)); t = u;
            if (t < -0x100) {
                t = (u & 0xfff) >> 3;
            } else {
                t = t >> 3;
            }
            if ((short)t < 0x160) {
                o->a.p.whole = t;
                t = o->d34 - ((unsigned short*)&SPAD.s186)[0];
                if ((short)t < o->d38) t = o->d38;
                t = (t & 0xfff) >> 4; o->y.p.whole = t;
                o->visible = 1;
                func_80018DA4(o);
            }
            break;
        }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
