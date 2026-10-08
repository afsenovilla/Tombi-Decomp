// FUNC 80123698 152 X014
// MATCHING 80123698 152
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);

void func_80123698(TObj *o)
{
    TObj *e = FUN_800183b8();
    short d;

    if (e != 0) {
        e->active = 2;
        e->type = 0x4b;
        *(signed char *)&e->b0f = -12;
        e->b0a = 2;
        e->d8c = 0;
        e->subtype = 1;
        e->b0c = 2;
        d = 8;
        if (o->animFrame) d = -8;
        e->a.p.whole = o->a.p.whole + d;
        e->y.p.whole = o->y.p.whole;
        e->b.p.whole = o->b.p.whole;
    }
}
