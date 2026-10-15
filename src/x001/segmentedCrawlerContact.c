// FUNC 80126564 792 X001
// MATCHING 80126564 792
#include "spad.h"
#include "TOBJ.H"

typedef struct {
    TObj o;
    unsigned char pad[0x24];
    TObj *e4;
} P;

extern short objectBoxOverlapSideFacing(P *, TObj *);
extern void spawnImpactEffect(int, short, short, short);
extern void decreaseObjectTimer(P *, int);

void segmentedCrawlerContact(P *o, TObj *e)
{
    short r = objectBoxOverlapSideFacing(o, e);
    TObj *q;

    if (r == -1) return;
    switch (*(unsigned char *)&o->o.wac) {
    case 1:
        if (r < 3) {
            if (e->b6a == 0) {
                if (SPAD.b1a4) break;
                if (o->o.active & 2) break;
                *(unsigned char *)&o->o.wac = 0;
                o->o.active = 2;
                r = e->h->p.whole > o->o.h->p.whole;
                o->o.b04 = 2;
                o->o.step = 0;
                o->o.state = 0;
                o->o.animFrame = r;
                decreaseObjectTimer(o, 1);
                break;
            }
            e->active = 2;
            e->b04 = 2;
            e->b69 = 0;
            e->step = 1;
            e->state = 0;
            for (q = (TObj *)e->d90; q; q = (TObj *)q->d90) {
                q->active = 2;
                q->b69 = 0;
                q->b04 = 2;
                q->step = 3;
                q->state = 0;
            }
            for (q = (TObj *)e->d94; q; q = (TObj *)q->d94) {
                q->active = 2;
                q->b69 = 0;
                q->b04 = 2;
                q->step = 3;
                q->state = 0;
            }
            o->e4 = e;
            *(unsigned char *)&o->o.wac = 2;
            spawnImpactEffect(2, o->o.a.p.whole, o->o.y.p.whole, o->o.b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (e->active & 2) break;
        if (SPAD.b1a4) break;
        if (o->o.active & 2) break;
        o->o.active = 2;
        r = e->h->p.whole > o->o.h->p.whole;
        o->o.b04 = 2;
        o->o.step = 0;
        o->o.state = 0;
        o->o.animFrame = r;
        decreaseObjectTimer(o, 1);
        break;
    case 2:
        if (e->active == 3) break;
        e->active = 3;
        e->b68 = 1;
        if (o->o.h->p.whole > e->h->p.whole) r = 3;
        else r = 2;
        e->animFrame = r;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        for (q = (TObj *)e->d90; q; q = (TObj *)q->d90) {
            q->active = 3;
            q->b68 = 1;
            if (o->o.h->p.whole > e->h->p.whole) r = 3;
            else r = 2;
            q->animFrame = r;
            q->b04 = 2;
            q->step = 0;
            q->state = 0;
        }
        for (q = (TObj *)e->d94; q; q = (TObj *)q->d94) {
            q->active = 3;
            q->b68 = 1;
            if (o->o.h->p.whole > e->h->p.whole) r = 3;
            else r = 2;
            q->animFrame = r;
            q->b04 = 2;
            q->step = 0;
            q->state = 0;
        }
        break;
    }
}
