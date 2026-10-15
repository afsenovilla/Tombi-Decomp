// FUNC 80120f4c 412 X001
// MATCHING 80120f4c 412
#include "spad.h"
#include "TOBJ.H"

typedef struct { char p[0x5c]; int d5c; } E;

extern void *D_8013E594[];
extern void func_80120E24(TObj *);
extern void freeObjectLayer7(TObj *);

void updateBackdropSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = 10;
        o->y.p.whole += 0x180;
        o->b0d = 0;
        o->anim = D_8013E594[o->b0c];
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->b0f = 0;
        if (o->subtype == 0) {
            ((E *)o)->d5c = o->d30 = o->a.raw;
        } else {
            o->d30 = o->a.p.whole;
        }
        o->d34 = o->y.p.whole;
        o->animFrame = o->b.p.whole;
        o->b.p.whole = 0;
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if ((*(short*)((char*)&SPAD.i174 + 2)) >= 0x42d) o->step++;
            func_80120E24(o);
            break;
        case 1:
            if ((*(short*)((char*)&SPAD.i174 + 2)) < 0x42c) {
                if (o->subtype == 0) o->d30 = ((E *)o)->d5c + 0x1400000;
                o->step--;
            }
            break;
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
