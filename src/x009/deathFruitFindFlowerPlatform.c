// FUNC 8011ecc8 392 X009
// MATCHING 8011ecc8 392
#include "spad.h"
#include "TOBJ.H"
extern short LIST_COUNTER;
extern short testCollisionSegmentsAtX(TObj *, short, short, unsigned char);

int deathFruitFindFlowerPlatform(TObj *o)
{
    TObj **list = (*(TObj ***)&SPAD.p268);
    TObj *e;
    short u;

    if ((LIST_COUNTER = SPAD.s254) == 0)
        goto out;
loop:
    {
        e = *list++;
        LIST_COUNTER--;
        if (!(e->active & 1))
            goto next;
        if (e->type != 0x1c)
            goto next;
        if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b)
            goto next;
        u = o->h->p.whole - e->h->p.whole + (o->box0 + e->box0);
        if ((unsigned short)u >= e->box1 + o->box1)
            goto next;
        if ((unsigned short)(o->y.p.whole - e->y.p.whole + (e->box2 + o->box2)) >= e->box3 + o->box3)
            goto next;
        u = o->h->p.whole - (e->h->p.whole - e->box0);
        if ((unsigned short)u >= e->box1)
            goto next;
        {short w = o->y.p.whole - (e->y.p.whole + (e->box3 - e->box2));
        if (testCollisionSegmentsAtX(o, u, w, e->subtype))
            return 1;}
    }
next:
    if (LIST_COUNTER != 0)
        goto loop;
out:
    return 0;
}
