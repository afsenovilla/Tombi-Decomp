// FUNC 80116c10 140 X014
// MATCHING 80116c10 140
#include "TOBJ.H"
extern TObj *ObjAlloc(void);

void func_80116C10(TObj *o)
{
    TObj *n = ObjAlloc();
    if (n != 0) {
        n->active = 1;
        n->type = 0x15;
        n->b0c = o->step;
        n->b0f = o->b0f - 1;
        n->d90 = (int)o;
        n->a.p.whole = o->a.p.whole;
        n->y.p.whole = o->y.p.whole;
        n->b.p.whole = o->b.p.whole;
        n->wac = o->wac;
    }
}
