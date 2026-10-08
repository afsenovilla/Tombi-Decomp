// FUNC 80126770 464 X000
// MATCHING 80126770 464
#include "TOBJ.H"
extern unsigned char DAT_1f8001a4;
extern void FUN_8004258c(TObj *, int);

void FUN_80126770(TObj *o)
{
    short v;
    short r;
    short y;

    if (o->d->p.whole > 0xb4) return;
    if (o->d->p.whole == 0xb4) {
        y = o->y.p.whole;
        if (y >= -0x3a6) {
            v = o->h->p.whole - 0xc36;
            if (v >= 0) {
                r = (v * -132) / 138;
                if (r - 0x323 < y) {
                    o->y.p.whole = r - 0x323;
                    o->y.p.frac = 0;
                    o->velY = 0;
                    if (o->active & 2) {
                        o->bbe = 9;
                        o->wb0 = -2;
                        o->b69 = 1;
                        return;
                    }
                    if (DAT_1f8001a4 != 0) return;
                    o->active = 2;
                    o->animFrame = 1;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                    FUN_8004258c(o, 1);
                }
            }
        }
    }
    if (o->d->p.whole < 0x5b && o->h->p.whole >= 0xc35 && (unsigned short)(-0x23f - o->y.p.whole) < 0xfb) {
        o->h->p.whole = 0xc35;
        if (!(o->active & 2) && DAT_1f8001a4 == 0) {
            o->active = 2;
            o->animFrame = 1;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            FUN_8004258c(o, 1);
        }
    }
}
