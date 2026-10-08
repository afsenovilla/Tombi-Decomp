// FUNC 8012dcac 296 X003
// MATCHING 8012dcac 296
#include "TOBJ.H"
extern unsigned short D_8007A3F0[];
extern void *D_8013A700[], *D_8013A69C[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);

void func_8012DCAC(TObj *o)
{
    switch (o->state) {
    case 0:
        if (--o->timer == -1) {
            o->active = 3;
            o->timer = 0x78;
            o->wac = 0x1a;
            o->state++;
            o->anim = D_8013A700[0];
            AnimLoadDuration(o);
        }
        break;
    case 1:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->active = 1;
            o->state = 0;
            o->step++;
            FUN_8001f8e4(o);
            o->wac = 1;
            o->anim = D_8013A69C[0];
            AnimLoadDuration(o);
        }
        break;
    }
}
