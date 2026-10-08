// FUNC 801252b8 676 X000
// MATCHING 801252b8 676
#include "TOBJ.H"

extern short func_80124FF4(TObj *o, TObj *e);

void func_801252B8(TObj *o, TObj *e)
{
    short dy0;
    int d;
    short dx, dx2, sx, px;
    short dy, r;
    short t;
    int v;
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
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) > 0x5a) return;
    dx = o->h->p.whole - e->h->p.whole;
    sx = e->box0 + o->box0;
    if ((unsigned short)(dx + sx) > e->box1 + o->box1) return;
    d = (unsigned short)o->y.p.whole - (unsigned short)e->y.p.whole;
    {
        short t = d + (e->box2 + o->box2);
        dy0 = d;
        if ((unsigned short)t > o->box3 + e->box3) return;
    }
    if (dx < 0) {
        dx = -dx;
        px = -sx;
        if ((unsigned short)(sx - dx) < 4) {
            o->h->p.whole = e->h->p.whole + px;
            return;
        }
    }
    if ((short)dy0 <= 0) {
        v = e->d30;
        dx2 = e->h->p.whole - v;
        dy = e->d34 - (unsigned short)e->y.p.whole;
        dx = o->h->p.whole - v;
        if (dx <= 0) {
            r = 0;
        } else if (dx2 < dx) {
            r = dy;
        } else {
            r = dx * (short)dy / dx2;
        }
        t = r + e->box2;
        if (e->d34 - t > o->y.p.whole + o->box2) return;
        o->y.p.whole = e->d34 - t - o->box2;
        o->y.p.frac = 0;
        o->velY = 0;
        o->b69 = 1;
        e->b69 = 1;
        return;
    }
    o->y.p.whole = e->y.p.whole + ((e->box3 - e->box2) + (o->box3 - o->box2));
    if (o->velY < 0) o->velY = 0;
}
