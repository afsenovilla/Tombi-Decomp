// FUNC 80135850 452 X001
// MATCHING 80135850 452
#include "TOBJ.H"

extern unsigned short D_8009C960, D_8009C962;
extern Fix16 *D_800A6078;
extern char D_80077CDC[];
extern void *D_8013DDF0[], *D_8013DDF4[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001faf4(TObj *);

void func_80135850(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C960 == 1 && D_8009C962 < 2) break;
        switch (o->state) {
        case 0:
            o->timer = 0x3c;
            o->movetab = D_80077CDC;
            o->anim = D_8013DDF0[0];
            AnimLoadDuration(o);
            o->state++;
        case 1:
            AnimAdvance(o);
            FUN_8001faf4(o);
            if (--o->timer <= 0) {
                o->anim = D_8013DDF4[0];
                AnimLoadDuration(o);
                o->state++;
            }
            break;
        case 2:
            if (AnimAdvance(o)) {
                o->state = 0;
                o->animFrame ^= 1;
            }
            break;
        }
        break;
    case 1:
        if (D_8009C960 == 1 && D_8009C962 < 2) break;
        if (o->state == 0) {
            o->b6b = o->animFrame;
            o->animFrame = o->h->p.whole > D_800A6078->p.whole;
            o->anim = D_8013DDF4[0];
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    }
}
