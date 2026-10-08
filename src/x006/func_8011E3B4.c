// FUNC 8011e3b4 352 X006
// MATCHING 8011e3b4 352
#include "TOBJ.H"
typedef struct { TObj t; char pc0[0xd0 - 0xc0]; short wd0; } TD;
extern unsigned char D_8009C93A;
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern TD *D_8009C94C;
extern void FUN_8001f96c(int, int, int, int);

static __inline__ int ovl(TObj *a, TObj *b)
{
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x14) >= 0x29) {
        return 0;
    }
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (a->box0 + b->box0)) > a->box1 + b->box1) {
        return 0;
    }
    return (unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) <= a->box3 + b->box3;
}

void func_8011E3B4(TObj *o, TObj *e)
{
    TD *p;

    if (D_8009C93A && ovl(o, e) && D_1F8001A4 == 0) {
        int eh, oh;
        e->active = 2;
        eh = e->h->p.whole;
        oh = o->h->p.whole;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->animFrame = eh < oh;
        FUN_8001f96c(0, e->a.p.whole, e->y.p.whole, e->b.p.whole);
        p = D_8009C94C;
        p->t.step = 4;
        p->t.state = 0;
        p->wd0 -= 0x10;
        if (p->wd0 < 0) {
            p->wd0 = 0;
        }
        D_1F80019E = 0;
    }
}
