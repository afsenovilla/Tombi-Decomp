// FUNC 80126bbc 264 X001
// MATCHING 80126bbc 264
#include "TOBJ.H"
#include "raw7.h"

extern unsigned char D_8009CEB0;
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short func_80043260(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_80126BBC(TObj *o, TObj *p)
{
    p->b6a = 0;
    if (func_80043260(o, p) == 0) return;
    if (D_8009CEB0 != 0) return;
    if (p->w98 != 0) {
        if (D_1F8001A4 != 0) return;
        if (o->active & 2) return;
        o->active = 2;
        if (p->h->p.whole > o->h->p.whole) {
            o->animFrame = 1;
        } else {
            o->animFrame = 0;
        }
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        FUN_8004258c(o, 1);
        D_1F80019E = 0;
    } else if (U8(o, 0xc3) != 0) {
        p->b6a = o->ba6;
    }
}
