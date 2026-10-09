// FUNC 8011ecc8 392 X009
// MATCHING 8011ecc8 392
#include "TOBJ.H"
extern TObj **D_1F800268;
extern unsigned short D_1F800254;
extern short D_1F80019E;
extern short func_80121630(TObj *, short, short, unsigned char);

int func_8011ECC8(TObj *o)
{
    TObj **list = D_1F800268;
    TObj *e;
    short u;

    if ((D_1F80019E = D_1F800254) == 0)
        goto out;
loop:
    {
        e = *list++;
        D_1F80019E--;
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
        if (func_80121630(o, u, w, e->subtype))
            return 1;}
    }
next:
    if (D_1F80019E != 0)
        goto loop;
out:
    return 0;
}
