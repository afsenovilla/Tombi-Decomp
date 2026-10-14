// FUNC 801184cc 120 X014
// MATCHING 801184cc 120
#include "spad.h"
#include "TOBJ.H"

extern TObj *ObjAlloc(void);

void func_801184CC(void)
{
    TObj *n = ObjAlloc();

    if (n) {
        n->active = 1;
        n->type = 0x61;
        n->subtype = 5;
        n->h->p.whole = ((unsigned short*)((char*)&SPAD.i174 + 2))[0];
        n->y.p.whole = ((unsigned short*)&SPAD.s17a)[0] - 0x104;
        n->d->p.whole = ((unsigned short*)&SPAD.s17e)[0];
    }
}
