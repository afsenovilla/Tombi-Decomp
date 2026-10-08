// FUNC 80121170 128 X014
// MATCHING 80121170 128
#include "TOBJ.H"
extern unsigned short D_1F800176;
extern unsigned short D_1F80017A[];
extern TObj *FUN_800183b8(void);

int func_80121170(TObj *o)
{
    TObj *n = FUN_800183b8();
    if (n) {
        n->active = 1;
        n->type = 0x46;
        n->subtype = 0;
        n->b0c = 0;
        n->a.p.whole = D_1F800176 + 0xa0;
        n->y.p.whole = D_1F80017A[0] - 0xf0;
        n->b.p.whole = o->b.p.whole;
    }
}
