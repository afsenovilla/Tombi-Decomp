// FUNC 8011a6a0 320 X009
// MATCHING 8011a6a0 320
#include "TOBJ.H"
extern TObj **D_1F800260;
extern unsigned short D_1F800250;
extern short D_1F80019E;
extern TObj D_800A6038;

int func_8011A6A0(TObj *o)
{
    TObj **list = D_1F800260;
    TObj *pl = &D_800A6038;
    TObj *n;

    if ((D_1F80019E = D_1F800250) == 0)
        goto out;
loop:
    {
        n = *list++;
        D_1F80019E--;
        if (!(n->active & 1))
            goto next;
        if (n->type != 0xa)
            goto next;
        if (n->subtype & 0x80)
            goto next;
        if ((unsigned short)(pl->d->p.whole - n->d->p.whole + 0x2d) >= 0x5b)
            goto next;
        if ((unsigned short)(pl->h->p.whole - n->h->p.whole + 0x60) >= 0xc1)
            goto next;
        if ((unsigned short)(pl->y.p.whole - n->y.p.whole + 0x40) >= 0x81)
            goto next;
        o->a.p.whole = n->a.p.whole;
        o->y.p.whole = n->y.p.whole - 0x10;
        o->b.p.whole = n->b.p.whole;
        o->d90 = (int)n;
        return 1;
    }
next:
    if (D_1F80019E != 0)
        goto loop;
out:
    return 0;
}
