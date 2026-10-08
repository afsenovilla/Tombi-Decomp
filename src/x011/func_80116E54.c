// FUNC 80116e54 340 X011
// MATCHING 80116e54 340
#include "TOBJ.H"

extern short D_8007A5F0[];
extern char D_80077CD0[];
extern void *D_8011C420[];
extern int AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001fe6c(TObj *);

static inline void SetAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_80116E54(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->movetab = D_80077CD0;
        o->d88 = 0;
        o->velV = 0x60;
        o->wac = 0;
        SetAnim(o, D_8011C420[0]);
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, o->animFrame);
        o->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
        o->d88 = (o->d88 + 1) & 0xff;
        if (o->d88 == 0)
            o->state++;
        break;
    case 2:
        AnimAdvance(o);
        o->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
        o->d88 = (o->d88 + 1) & 0xff;
        if (o->d88 == 0)
            o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->state = 1;
        o->animFrame = 1 - o->animFrame;
        break;
    }
}
