// FUNC 80120f34 588 X009
// MATCHING 80120f34 588
#include "spad.h"
#include "TOBJ.H"
#include "raw7.h"

extern short func_80043464(TObj *, TObj *);
extern void spawnImpactEffect(int, int, int, int);
extern void decreaseObjectTimer(TObj *, int);

void jellyfishContact(TObj *o, TObj *p)
{
    short r;
    int t;

    if (o->b9e) return;
    r = func_80043464(o, p);
    if (r == -1) return;
    switch ((*(unsigned char*)&o->wac)) {
    case 1:
        if (r == 3) {
            p->active = 2;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->b6a = 0;
            PTR(o, 0xe4) = p;
            (*(unsigned char*)&o->wac) = 2;
            spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (p->active & 2) break;
        if (SPAD.b1a4) break;
        if (o->active & 2) break;
        if (r == 2 && p->b6a == 1) {
            if ((*(unsigned char*)&o->wac) == 1) (*(unsigned char*)&o->wac) = 0;
            p->active = 4;
            p->b6a = 2;
            o->active = 2;
            o->b04 = 2;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
            decreaseObjectTimer(o, 1);
            break;
        }
        if (o->b9c && r == (o->animFrame & 1)) break;
        o->active = 2;
        t = p->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = t;
        decreaseObjectTimer(o, 1);
        break;
    case 2:
        /* the dispatch's delay slot (addiu v0,3) runs before this case: the game compares active with 3 */
        if (p->active == 3) break;
        if (p->b6a == 2) break;
        p->active = 3;
        p->b6a = 0;
        p->b04 = 2;
        t = o->h->p.whole > p->h->p.whole;
        p->step = 0;
        p->state = 0;
        p->animFrame = t;
        break;
    }
    SPAD.s19e = 0;
}
