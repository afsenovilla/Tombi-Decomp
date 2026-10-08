// FUNC 80124348 140 X001
// MATCHING 80124348 140
#include "TOBJ.H"
extern short func_800435E0(TObj *, TObj *);
extern void FUN_8004d620(int, int);

void func_80124348(TObj *p, TObj *o)
{
    short r = func_800435E0(p, o);
    if (r != -1 && r < 3) {
        o->active = 2;
        o->b04 = 2;
        o->b6a = 0;
        o->step = 3;
        o->state = 0;
        if (o->subtype == 0)
            FUN_8004d620(0x12, 2);
        o->animFrame = p->animFrame & 1;
    }
}
