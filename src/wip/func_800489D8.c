// FUNC 800489d8 588 MAIN0
// score 167: reg alloc differs (game o=a3,e=t0, a0/a1 reused as temps)
#include "TOBJ.H"
extern short D_1F80019E[];
static __inline__ void body(TObj *o, TObj *e)
{
    short dx, w, r, ox, adx;
    char pad;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return;
    if (o->animFrame & 1) ox = o->box0;
    else ox = o->box1 - o->box0;
    w = e->box0 + ox;
    r = w;
    dx = o->h->p.whole - e->h->p.whole;
    adx = dx;
    if ((unsigned short)(dx + w) > e->box1 + o->box1)
        return;
    if ((unsigned short)(o->y.p.whole - e->y.p.whole + (e->box2 + (o->box3 - o->box2))) > o->box3 + e->box3)
        return;
    if (dx < 0) {
        adx = -dx;
        r = -r;
    } else {
        if (o->animFrame & 1) ox = o->box1 - o->box0;
        else ox = o->box0;
        r = ox + (e->box1 - e->box0);
    }
    if ((unsigned short)(r - adx) < 9) {
        if (o->b68) {
            e->b68 = o->b68;
            e->animFrame = o->animFrame & 1;
            o->b68 = 0;
        }
        o->h->p.whole = e->h->p.whole + r;
        if (r < 0) o->b9d = 2;
        else o->b9d = 3;
        return;
    }
    if (o->b9c & 1) return;
    D_1F80019E[0] = 0;
    o->b69 = 1;
    o->y.p.frac = 0;
    o->y.p.whole = e->y.p.whole - (e->box2 + (o->box3 - o->box2));
    if (o->category == 2) {
        switch (o->type) {
        case 2:
            e->b69 = 2;
            break;
        case 0x22:
            o->active = 2;
            o->step = 5;
            break;
        }
    }
}
void func_800489D8(TObj *o, TObj *e)
{
    body(o, e);
}
