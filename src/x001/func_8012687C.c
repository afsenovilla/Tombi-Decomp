// FUNC 8012687c 500 X001
// MATCHING 8012687c 500
#include "TOBJ.H"
#include "raw7.h"

extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short func_80043260(TObj *, TObj *);
extern short FUN_80043c74(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_8012687C(TObj *o, TObj *p)
{
    short r;

    if (o->b9e) return;
    if (p->active == 5) return;
    if (!(p->active & 2)) {
        r = func_80043260(o, p);
        if (r == 0) return;
        if (r == 1) {
            if (U8(o, 0xac) != 1) {
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
                U8(o, 0xac) = 2;
            }
        } else {
            if (p->subtype == 0) {
                if (D_1F8001A4) return;
                if (o->active & 2) return;
                o->active = 2;
                if (p->h->p.whole > o->h->p.whole) o->animFrame = 1;
                else o->animFrame = 0;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
                FUN_8004258c(o, 1);
            }
        }
    } else {
        if (!FUN_80043c74(o, p)) return;
        if (p->subtype) return;
        if (D_1F8001A4) return;
        if (o->active & 2) return;
        o->active = 2;
        if (p->h->p.whole > o->h->p.whole) o->animFrame = 1;
        else o->animFrame = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        FUN_8004258c(o, 1);
    }
    D_1F80019E = 0;
}
