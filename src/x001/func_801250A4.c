// FUNC 801250a4 268 X001
// MATCHING 801250a4 268
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short func_800439F4(TObj *, TObj *);
extern short func_80124C20(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_801250A4(TObj *a, TObj *b)
{
    short r;

    if (a->step == 0x22) return;
    if (b->subtype == 7) r = func_800439F4(a, b);
    else r = func_80124C20(a, b);
    if (r == 1 && !D_1F8001A4 && !(a->active & 2) && !(b->active & 2)) {
        a->active = 2;
        if (b->h->p.whole > a->h->p.whole) a->animFrame = 1;
        else a->animFrame = 0;
        a->b04 = 2;
        a->step = 0;
        a->state = 0;
        FUN_8004258c(a, 1);
        D_1F80019E = 0;
    }
}
