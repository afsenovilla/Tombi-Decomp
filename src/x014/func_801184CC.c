// FUNC 801184cc 120 X014
// MATCHING 801184cc 120
#include "TOBJ.H"

extern unsigned short D_1F800176[], D_1F80017A[], D_1F80017E[];
extern TObj *ObjAlloc(void);

void func_801184CC(void)
{
    TObj *n = ObjAlloc();

    if (n) {
        n->active = 1;
        n->type = 0x61;
        n->subtype = 5;
        n->h->p.whole = D_1F800176[0];
        n->y.p.whole = D_1F80017A[0] - 0x104;
        n->d->p.whole = D_1F80017E[0];
    }
}
