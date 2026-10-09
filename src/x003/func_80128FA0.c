// FUNC 80128fa0 424 X003
// MATCHING 80128fa0 424
#include "TOBJ.H"

typedef struct { void **p; int a, b; } T12;
extern T12 D_80135D84[];
extern short FUN_80040278(TObj *, int, int);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void func_80128FA0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->anim = D_80135D84[o->subtype].p[1];
        AnimLoadDuration(o);
        o->velX = -0x300;
        o->velY = 0x300;
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        if (!FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
            o->velX = -0x100;
            o->velY = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
            o->velX = -0x300;
            o->velY = 0x400;
            o->state = 1;
        }
        break;
    }
}
