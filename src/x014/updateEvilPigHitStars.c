// FUNC 80116d1c 420 X014
// MATCHING 80116d1c 420
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
extern void *D_8001226C;
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void updateEvilPigHitStars(TObj *o)
{
    TObj *q;
    int dx, dy;
    unsigned char b = o->b04;

    switch (b) {
    case 0:
        o->w1e = 0x14;
        o->b0d = 1;
        o->anim = D_8001226C;
        o->w08 = 0x7ed7;
        o->animFrame = 0;
        o->d3c = ((int*)&SPAD.i2d8)[0];
        o->b04++;
        o->timer = 0x78;
        AnimLoadDuration(o);
        break;
    case 1:
        q = (TObj *)o->d90;
        if (q->b04 == 2) {
            if (o->b0c == q->step) {
                    if (o->b0c == 2) {
                        dy = -0x20;
                        if (q->animFrame & 1)
                            dx = -8;
                        else
                            dx = 8;
                    } else {
                        dy = -0x10;
                        if (q->animFrame & 1)
                            dx = -0xc;
                        else
                            dx = 0xc;
                    }
                    o->a.p.whole = q->a.p.whole + dx;
                    o->y.p.whole = q->y.p.whole + dy;
                    o->b.p.whole = q->b.p.whole;
                    if (((unsigned char*)&GVARS.b42)[0] == 0)
                        AnimAdvance(o);
                    FUN_800202b4(o);
            } else
                o->b04 = 3;
        } else
            o->b04 = 3;
        break;
    case 2:
        o->b04 = b + 1;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
