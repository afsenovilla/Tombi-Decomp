// JP slps01144 80050BD8 472
// Japanese version of PAL main0 80045800 (playerTouchCat2Type59.c): 228 -> 212.
#include "TOBJ.H"
extern unsigned char DAT_8009cdc7;
extern short DAT_1f80019e;
extern short pushObjectOutOfObjectBoxA(TObj *, TObj *);
extern void spawnImpactEffect(int, int, int, int);
#define B(o, n) (((unsigned char *)(o))[n])

void playerTouchCat2Type59(TObj *o, TObj *e)
{
    short r;
    if (o->b9e != 0 || B(o, 0xc8) != 0)
        return;
    if (e->b0c == 0x32) {
        if (DAT_8009cdc7 != 0xff)
            return;
        if ((unsigned short)(o->h->p.whole - e->h->p.whole + (o->box0 + e->box0)) > o->box1 + e->box1)
            return;
        if ((unsigned short)(o->y.p.whole - e->y.p.whole + (o->box2 + e->box2)) > o->box3 + e->box3)
            return;
        B(o, 0xa0) = 1;
        B(o, 0xa8) = 9;
        return;
    }
    r = pushObjectOutOfObjectBoxA(o, e);
    if (r == 0)
        return;
    if (o->ba6 & 2)
        o->ba6 += 2;
    if (r != 1)
        return;
    if (e->active & 4) {
        if (B(o, 0xac) == r) {
            e->active = 2;
            e->b04 = 2;
            e->step = 0;
            e->state = 0;
            e->b69 = 0;
            *(TObj **)((char *)o + 0xd4) = e;
            B(o, 0xac) = 2;
            spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        }
        DAT_1f80019e = 0;
    } else {
        if (o->h->p.whole > e->h->p.whole) {
            o->bbe = 8;
            o->wb0 = 2;
        } else {
            o->bbe = 9;
            o->wb0 = -2;
        }
    }
}
