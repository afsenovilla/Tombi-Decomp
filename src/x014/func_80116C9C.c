// FUNC 80116c9c 128 X014
// MATCHING 80116c9c 128
#include "TOBJ.H"

extern TObj *ObjAlloc(void);

void func_80116C9C(TObj *o)
{
    TObj *n = ObjAlloc();

    if (n) {
        n->active = 1;
        n->type = 0x60;
        n->b0c = o->step;
        n->b0f = o->b0f - 2;
        n->d90 = (int)o;
        n->a.p.whole = o->a.p.whole;
        n->y.p.whole = o->y.p.whole;
        n->b.p.whole = o->b.p.whole;
    }
}
