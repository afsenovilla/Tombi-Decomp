// FUNC 80113cc8 624 X019
// MATCHING 80113cc8 624
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *);
extern short FUN_80040278(TObj *, int, int);

void FUN_80113cc8(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->w78 > 0)
            o->w7a = (o->d->p.whole + 90) / 90 * 90;
        else
            o->w7a = (o->d->p.whole - 90) / 90 * 90;
        o->b69 = 0;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        o->d->raw += o->w78 << 8;
        if (o->w78 > 0) {
            if (o->d->p.whole > o->w7a)
                o->d->p.whole = o->w7a;
        } else {
            if (o->d->p.whole < o->w7a)
                o->d->p.whole = o->w7a;
        }
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        o->d->raw += o->w78 << 8;
        if (o->w78 > 0) {
            if (o->d->p.whole > o->w7a)
                o->d->p.whole = o->w7a;
        } else {
            if (o->d->p.whole < o->w7a)
                o->d->p.whole = o->w7a;
        }
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + (o->box3 - o->box2)))) {
            o->b6a = 0;
            o->step = 0;
            o->state = 0;
            o->b69 = 0;
        }
        break;
    }
}
