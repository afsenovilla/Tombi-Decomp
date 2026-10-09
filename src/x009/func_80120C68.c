// FUNC 80120c68 716 X009
// MATCHING 80120c68 716
#include "TOBJ.H"

typedef struct { TObj o; char pad[0x24]; TObj *e4; } TObjX;
extern unsigned char D_1F8001A4;
extern short func_80043260(TObj *, TObj *);
extern short func_800435E0(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);
extern void FUN_8001f96c(int, short, short, short);

static __inline__ void hit(TObj *o, TObj *e)
{
    short r;
    o->active = 2;
    r = e->h->p.whole > o->h->p.whole;
    o->b04 = 2;
    o->step = 0;
    o->state = 0;
    o->animFrame = r;
    FUN_8004258c(o, 1);
}

void func_80120C68(TObj *o, TObj *e)
{
    short q;
    short w;
    short r;

    if (e->b6a) {
        q = func_80043260(o, e);
        if (q == 0) return;
        e->b69 = 0;
        if (D_1F8001A4) return;
        if ((o->active & 2) || (e->active & 2) || e->active == 7) {
            if (q != 1) return;
            if (o->h->p.whole > e->h->p.whole) {
                o->bbe = 8;
                o->wb0 = 1;
            } else {
                o->bbe = 9;
                o->wb0 = -1;
            }
            return;
        }
        if (q != 1) return;
        if (*(unsigned char *)&o->wac == q) *(unsigned char *)&o->wac = 0;
        hit(o, e);
        return;
        return;
    }
    r = func_800435E0(o, e);
    if (r == -1) return;
    switch (*(unsigned char *)&o->wac) {
    case 1:
        if (r < 3) {
            e->active = 4;
            e->b04 = 2;
            e->step = 1;
            e->state = 0;
            e->animFrame = o->animFrame & 1;
            ((TObjX *)o)->e4 = e;
            *(unsigned char *)&o->wac = 2;
            FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (e->active & 2) {
            if (r != 1) break;
            if (o->h->p.whole > e->h->p.whole) {
                o->bbe = 8;
                o->wb0 = 1;
            } else {
                o->bbe = 9;
                o->wb0 = -1;
            }
            break;
        }
        if (D_1F8001A4) break;
        if (o->active & 2) break;
        hit(o, e);
        break;
    case 2:
        if (e->active == 3) break;
        e->active = 3;
        w = o->h->p.whole > e->h->p.whole;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->w7a = w;
        break;
    }
}
