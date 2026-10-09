// FUNC 801261c0 672 X009
// MATCHING 801261c0 672
#include "TOBJ.H"

extern void *D_8012EE80[], *D_8012EE84[];
extern char D_80077D00[];
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern short FUN_80040278(TObj *, short, short);
extern short func_8004065C(TObj *, short, short, short);

#define LINK(o) (*(TObj **)((char *)(o) + 0xa8))

void func_801261C0(TObj *o)
{
    unsigned char t = o->state;
    short d;
    unsigned char f;
    unsigned short a;
    short sa;

    switch (t) {
    case 0:
        o->state = t + 1;
        o->wac = 0;
        o->anim = D_8012EE84[0];
        FUN_8001fe6c(o);
        if (LINK(o)->y.p.whole > o->y.p.whole) o->w74 = 0;
        else o->w74 = 1;
        o->velV = 0x200;
        o->velY = -8;
        break;
    case 1:
        AnimAdvance(o);
        if (o->w74 == 0) o->y.raw -= o->velV << 8;
        else o->y.raw += o->velV << 8;
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        o->velV += o->velY;
        if (o->velV > 0) break;
        o->timer = 0x18;
        o->wac = 0;
        o->anim = D_8012EE80[0];
        FUN_8001fe6c(o);
        o->movetab = D_80077D00;
        o->state++;
        break;
    case 2:
        AnimAdvance(o);
        FUN_8001fa88(o, o->animFrame);
        f = o->b9d;
        a = o->animFrame;
        if (!(f & 2) || a != (f & 1)) {
            sa = a;
            if (sa) d = -0x10;
            else d = 0x10;
            func_8004065C(o, o->h->p.whole + d, o->y.p.whole + 0x30, sa);
        }
        if (o->w74 == 0) o->y.raw -= o->velV << 8;
        else o->y.raw += o->velV << 8;
        if (o->velV >= -0x200) o->velV += o->velY;
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        if (--o->timer == -1) o->state++;
        break;
    case 3:
        o->step = 0;
        o->state = 0;
        break;
    }
}
