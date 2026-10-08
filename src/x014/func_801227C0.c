// FUNC 801227c0 140 X014
// MATCHING 801227c0 140
#include "TOBJ.H"

extern short D_1F800176[], D_1F80017A[];
extern TObj *FUN_800183b8(void);

int func_801227C0(TObj *o)
{
    TObj *n = FUN_800183b8();

    if (n) {
        n->active = 1;
        n->type = 0x48;
        n->subtype = 0;
        n->b0c = 0;
        n->a.raw = (D_1F800176[0] + 0xa0) << 16;
        n->y.raw = (D_1F80017A[0] - 0xf0) << 16;
        n->b.raw = o->b.p.whole << 16;
    }
}
