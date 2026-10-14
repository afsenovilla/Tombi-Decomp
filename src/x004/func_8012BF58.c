// FUNC 8012bf58 172 X004
// MATCHING 8012bf58 172
#include "spad.h"
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void func_8012BF58(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = 1;
        o->b0d = 0;
        o->d3c = ((int*)&SPAD.i2d0)[0];
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        AnimAdvance(o);
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
