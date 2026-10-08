// FUNC 80122f18 136 X014
// MATCHING 80122f18 136
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);

int func_80122F18(TObj *o)
{
    TObj *p = FUN_800183b8();

    if (p != 0) {
        p->active = 1;
        p->type = 0x49;
        p->b0a = 2;
        p->subtype = o->animFrame;
        p->wb4 = o->wb8;
        p->a.p.whole = o->a.p.whole;
        p->y.p.whole = o->y.p.whole;
        p->b.p.whole = o->b.p.whole;
    }
}
