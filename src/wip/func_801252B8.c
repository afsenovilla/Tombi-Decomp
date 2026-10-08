// FUNC 801252b8 676 X000
// r11: score 28 (n reuses dy to fix dx/sx/dy regs). Left: e->y load reg (a1 in game) / sched of e->box2 load in the dy check, and n should be v1 (own var) in the slope block.
#include "TOBJ.H"

extern short func_80124FF4(TObj *o, TObj *e);

void func_801252B8(TObj *o, TObj *e)
{
    short dx, dy, sx, px, a, b, n;
    e->b69 = 0;
    if (e->b0c) {
        if (e->subtype == 2) {
            if (((unsigned char *)&o->da0)[2] == 3) return;
            if (func_80124FF4(o, e) == 1) *(unsigned char *)&o->da0 = e->animTimer;
        } else {
            if (func_80124FF4(o, e) == 1) *(unsigned char *)&o->da0 = e->animTimer;
        }
        return;
    }
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return;
    dx = o->h->p.whole - e->h->p.whole;
    sx = e->box0 + o->box0;
    if ((unsigned short)(dx + sx) > e->box1 + o->box1)
        return;
    dy = o->y.p.whole - e->y.p.whole;
    if ((unsigned short)(dy + (e->box2 + o->box2)) > o->box3 + e->box3)
        return;
    if (dx < 0) {
        dx = -dx;
        px = -sx;
        if ((unsigned short)(sx - dx) < 4) {
            o->h->p.whole = e->h->p.whole + px;
            return;
        }
    }
    if (dy <= 0) {
        a = e->h->p.whole - e->d30;
        b = e->d34 - e->y.p.whole;
        dy = o->h->p.whole - e->d30;
        if (dy <= 0) dy = 0;
        else if (a < dy) dy = b;
        else dy = dy * b / a;
        dy += e->box2;
        if (e->d34 - dy > o->y.p.whole + o->box2) return;
        o->y.p.whole = e->d34 - dy - o->box2;
        o->y.p.frac = 0;
        o->velY = 0;
        o->b69 = 1;
        e->b69 = 1;
    } else {
        o->y.p.whole = e->y.p.whole + ((e->box3 - e->box2) + (o->box3 - o->box2));
        if (o->velY < 0) o->velY = 0;
    }
}
