// FUNC 80113b6c 340 X000
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *);
extern short FUN_80040278(TObj *, int, int);

void FUN_80113b6c(TObj *o)
{
    short s;
    unsigned short u;
    switch (o->state) {
    case 0:
        o->b69 = 0;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->h->raw = o->h->raw + o->velX * 0x100;
        s = o->velY;
        o->y.raw = o->y.raw + s * 0x100;
        u = o->velY + 0x20;
        o->velY = u;
        if (0 < (int)((unsigned)u << 16)) o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        o->h->raw = o->h->raw + o->velX * 0x100;
        o->y.raw = o->y.raw + o->velY * 0x100;
        o->velY = o->velY + 0x20;
        if (o->b69 == 0 &&
            FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + (unsigned short)(o->box3 - o->box2))) == 0)
            return;
        o->b6a = 0;
        o->step = 0;
        o->state = 0;
        o->b69 = 0;
        break;
    }
}
