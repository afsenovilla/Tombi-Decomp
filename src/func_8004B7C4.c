// FUNC 8004b7c4 740 MAIN0
// MATCHING 8004b7c4 740
#include "TOBJ.H"
typedef struct { TObj t; char c0[0x28]; short we8; short wea; } PO;
extern short D_1F80019E;
extern TObj *D_1F8003C0;

int func_8004B7C4(PO *o, TObj *e)
{
    short d, ad, dy, t;
    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 45) > 90)
        return 0;
    d = o->we8 - o->t.h->p.whole;
    ad = d;
    if (d < 0) ad = -d;
    d = o->we8 - e->h->p.whole;
    if (o->t.animFrame & 1) t = e->box0 + ad + d;
    else t = e->box0 + d;
    if ((unsigned short)t > e->box1 + ad)
        return 0;
    dy = o->wea - e->y.p.whole;
    ad = e->box2 + dy;
    if (e->box3 < (unsigned short)ad)
        return 0;
    switch (e->type) {
    case 0x19:
    case 0x37:
    case 0x3d:
        if (e->subtype == 0 && ad < 8) {
            o->t.b9e = 0xb;
            if (e->type == 0x3d && (e->animFrame & 2)) o->t.animFrame = 0;
            else o->t.animFrame = 1;
        } else {
            o->t.b9e = 0xa;
        }
        if (o->t.animFrame & 1) {
            o->t.wb8 = 8;
            o->t.h->p.whole = e->h->p.whole + 8;
        } else {
            o->t.wb8 = -5;
            o->t.h->p.whole = e->h->p.whole - 5;
        }
        break;
    case 0x11:
        if (ad < 8) o->t.b9e = 0xb;
        else o->t.b9e = 0xa;
        o->t.animFrame = 0;
        o->t.wb8 = -8;
        o->t.h->p.whole = e->h->p.whole - 8;
        break;
    case 0x10:
    case 0x3e:
        if (e->subtype == 0 && ad < 8) o->t.b9e = 0xc;
        else o->t.b9e = 0xa;
        if (o->t.animFrame & 1) {
            o->t.wb8 = 8;
            o->t.h->p.whole = e->h->p.whole + 8;
        } else {
            o->t.wb8 = -8;
            o->t.h->p.whole = e->h->p.whole - 8;
        }
        break;
    case 0x35:
        if (e->subtype != 0) return 0;
        if (o->t.animFrame & 1) {
            o->t.wb8 = 8;
            o->t.h->p.whole = e->h->p.whole + 8;
        } else {
            o->t.wb8 = -8;
            o->t.h->p.whole = e->h->p.whole - 8;
        }
        o->t.b9e = 0xa;
        e->b69 = 1;
        break;
    }
    o->t.wba = dy;
    o->t.velY = 0;
    D_1F80019E = 0;
    D_1F8003C0 = e;
    return 1;
}
