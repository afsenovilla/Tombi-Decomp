// FUNC 8012ce80 332 X010
// MATCHING 8012ce80 332
#include "spad.h"
#include "TOBJ.H"

extern void *D_80132310[];
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void func_8012CB50(TObj *);
extern void func_8012CCE8(TObj *);
extern void freeObjectLayer2(TObj *);

void func_8012CE80(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b6b = Rand() & 1;
        o->active = 3;
        o->box0 = 0xa;
        o->box1 = 0x14;
        o->box2 = 0x20;
        o->box3 = 0x3a;
        o->w1e = 1;
        o->wac = 0x1e;
        o->b0d = 0;
        o->d3c = ((int*)&SPAD.i2d0)[0];
        o->anim = D_80132310[o->wac];
        AnimLoadDuration(o);
        break;
    case 1:
        AnimAdvance(o);
        ObjCullRegister(o);
        switch (o->b6b) {
        case 0:
            func_8012CB50(o);
            break;
        case 1:
            func_8012CCE8(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
