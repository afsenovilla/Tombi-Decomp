// FUNC 800e964c 156 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
extern TObj *func_80018448(void);
extern unsigned short D_800FEBD4[];

void func_800E964C(TObj *o)
{
    TObj *n = func_80018448();
    if (n != 0) {
        n->active = 1;
        n->type = 0x3A;
        n->a.raw = o->a.raw;
        n->y.raw = o->y.raw;
        n->b.raw = o->b.raw;
        n->b0a = 0;
        n->w1e = D_800FEBD4[0];
        n->b0d = 0x80;
        n->b0f = o->b0f + 1;
        n->_pad0e[0] = 1;
        n->d90 = (int)o;
        n->b04 = 0;
        n->step = 0;
        n->state = 0;
        n->subtype = 0;
    }
}
