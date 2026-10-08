// FUNC 80118314 124 X014
// MATCHING 80118314 124
#include "TOBJ.H"
extern TObj *ObjAlloc(void);

void func_80118314(short x, short y, short z)
{
    TObj *n = ObjAlloc();
    if (n != 0) {
        n->active = 1;
        n->type = 0x61;
        n->subtype = 3;
        n->h->p.whole = x;
        n->y.p.whole = y;
        n->d->p.whole = z;
        n->timer = 0x20;
    }
}
