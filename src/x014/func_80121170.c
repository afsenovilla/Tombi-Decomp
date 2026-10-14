// FUNC 80121170 128 X014
// MATCHING 80121170 128
#include "spad.h"
#include "TOBJ.H"
extern TObj *allocObjectLayer2(void);

int func_80121170(TObj *o)
{
    TObj *n = allocObjectLayer2();
    if (n) {
        n->active = 1;
        n->type = 0x46;
        n->subtype = 0;
        n->b0c = 0;
        n->a.p.whole = (*(unsigned short*)((char*)&SPAD.i174 + 2)) + 0xa0;
        n->y.p.whole = ((unsigned short*)&SPAD.s17a)[0] - 0xf0;
        n->b.p.whole = o->b.p.whole;
    }
}
