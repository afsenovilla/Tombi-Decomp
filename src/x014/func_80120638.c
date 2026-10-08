// FUNC 80120638 136 X014
// MATCHING 80120638 136
#include "TOBJ.H"

extern TObj *FUN_800183b8(void);

int func_80120638(TObj *o)
{
    TObj *n = FUN_800183b8();

    if (n) {
        n->active = 1;
        n->type = 0x43;
        n->subtype = o->animFrame & 1;
        n->wb4 = o->animFrame & 1;
        n->a.p.whole = o->a.p.whole;
        n->y.p.whole = o->y.p.whole;
        n->b.p.whole = o->b.p.whole;
    }
}
