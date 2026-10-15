// FUNC 8012687c 500 X001
// MATCHING 8012687c 500
#include "spad.h"
#include "TOBJ.H"
#include "raw7.h"

extern short func_80043260(TObj *, TObj *);
extern short pushOutOfBoxX(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void chainSegmentEnemyContact(TObj *o, TObj *p)
{
    short r;

    if (o->b9e) return;
    if (p->active == 5) return;
    if (!(p->active & 2)) {
        r = func_80043260(o, p);
        if (r == 0) return;
        if (r == 1) {
            if ((*(unsigned char*)&o->wac) != 1) {
                if ((short)(o->h->p.whole - p->h->p.whole) >= 0) {
                    o->bbe = 8;
                    o->wb0 = 2;
                } else {
                    o->bbe = 9;
                    o->wb0 = -2;
                }
            } else {
                p->active = 2;
                p->b04 = 2;
                p->step = 0;
                p->state = 0;
                p->b69 = 0;
                PTR(o, 0xe4) = p;
                (*(unsigned char*)&o->wac) = 2;
            }
        } else {
            if (p->subtype == 0) {
                if (SPAD.b1a4) return;
                if (o->active & 2) return;
                o->active = 2;
                if (p->h->p.whole > o->h->p.whole) o->animFrame = 1;
                else o->animFrame = 0;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
                decreaseObjectTimer(o, 1);
            }
        }
    } else {
        if (!pushOutOfBoxX(o, p)) return;
        if (p->subtype) return;
        if (SPAD.b1a4) return;
        if (o->active & 2) return;
        o->active = 2;
        if (p->h->p.whole > o->h->p.whole) o->animFrame = 1;
        else o->animFrame = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 1);
    }
    SPAD.s19e = 0;
}
