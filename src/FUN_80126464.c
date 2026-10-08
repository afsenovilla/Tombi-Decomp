// FUNC 80126464 564 X000
// MATCHING 80126464 564
#include "TOBJ.H"
#include "raw7.h"
extern unsigned char D_1f8001a4;
extern unsigned char D_800a6038[];
extern short FUN_800435e0(TObj *, TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8004258c(TObj *, int);

void FUN_80126464(TObj *a, TObj *b)
{
    short r;

    r = FUN_800435e0(a, b);
    if (r == -1)
        return;
    switch (U8(a, 0xac)) {
    case 1:
        if (r < 3) {
            b->b69 = 0;
            b->active = 2;
            b->b04 = 2;
            b->step = 1;
            b->state = 0;
            b->animFrame = a->animFrame & 1;
            S32(a, 0xe4) = (int)b;
            U8(a, 0xac) = 2;
            FUN_8001f96c(2, a->a.p.whole, a->y.p.whole, a->b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (D_1f8001a4)
            return;
        if (b->active & 2)
            return;
        if (a->active & 2)
            return;
        if (U8(a, 0xac) != 3 && r == 3 && b->b6a == 1) {
            if (U8(a, 0xac) == 1)
                U8(a, 0xac) = 0;
            b->active = 4;
            b->b6a = 2;
            a->active = 2;
            D_800a6038[4] = 1;
            D_800a6038[5] = 0x40;
            D_800a6038[6] = 0;
            D_800a6038[7] = 0;
            return;
        }
        {
            int ah, bh;
            a->active = 2;
            bh = b->h->p.whole;
            ah = a->h->p.whole;
            a->b04 = 2;
            a->step = 0;
            a->state = 0;
            a->animFrame = ah < bh;
            FUN_8004258c(a, 1);
        }
        break;
    case 2:
        if (b->active == 3)
            return;
        b->active = 3;
        if (b->b6a == 2) {
            D_800a6038[0] = 1;
            D_800a6038[4] = 1;
            D_800a6038[5] = 0;
            D_800a6038[6] = 0;
        }
        b->b6a = 0;
        b->b04 = 2;
        b->step = 0;
        b->state = 0;
        break;
    }
}
