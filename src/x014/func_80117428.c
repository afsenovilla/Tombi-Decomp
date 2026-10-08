// FUNC 80117428 104 X014
// MATCHING 80117428 104
#include "TOBJ.H"

extern TObj *ObjAlloc(void);

int func_80117428(TObj *o)
{
    TObj *p = ObjAlloc();
    if (p != 0) {
        p->active = 2;
        p->type = 0x4f;
        p->a.p.whole = o->a.p.whole;
        p->y.p.whole = o->y.p.whole;
        p->b.p.whole = o->b.p.whole;
    }
}
