// FUNC 80117d70 76 X001
// MATCHING 80117d70 76
#include "TOBJ.H"

void func_80117D70(TObj *o)
{
    TObj *p = (TObj *)o->d90;

    o->h->p.whole = p->h->p.whole - 0x1e;
    o->y.p.whole = p->y.p.whole - 10;
    o->d->p.whole = p->d->p.whole - 0x32;
}
