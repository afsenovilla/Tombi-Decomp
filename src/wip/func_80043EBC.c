// FUNC 80043ebc 544 MAIN0
/* score 8: only the dy test differs: game computes (dy-8) + (e->box2+o->box2) with dy=a0, o->box2=a1 and the dy copy (move a1,a0) late; the (dy-8)+(...) grouping gives dy=v1 (12), this form gives dy=a0 but adds in another order. Tried ~30 groupings, block-local short b = box sum (10), temps, types. */
#include "TOBJ.H"

typedef struct { short x, y; } Off;
extern Off D_8007B610[];

void func_80043EBC(TObj *o, TObj *e)
{
    short dx, dy, sx, px, cx, ax;
    short tx, ty;
    Off *t;
    short w;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return;
    { short *u = &D_8007B610[e->b0c].x;
    px = sx = e->box0 + o->box0 + 8;
    tx = *u++;
    dx = o->h->p.whole - (e->h->p.whole + tx);
    ty = *u; }
    if ((unsigned short)(dx + sx) > e->box1 + 16 + o->box1)
        return;
    dy = o->y.p.whole - (e->y.p.whole + ty);
    if ((unsigned short)(dy + e->box2 + o->box2 - 8) > o->box3 + e->box3 - 16)
        return;
    cx = px;
    ax = dx;
    if (dx < 0) {
        dx = -dx;
        px = -sx;
    } else {
        px = (e->box1 - e->box0) + (short)(o->box1 - o->box0 + 8);
        cx = px;
    }
    if ((unsigned short)(cx - dx) < 4) {
        o->h->p.whole = px + (e->h->p.whole + tx);
        return;
    }
    if (dy < 5) {
        if (o->b9c & 1) return;
        w = e->y.p.whole + ty + 8;
        o->y.p.whole = w - (e->box2 + o->box2);
        o->y.p.frac = 0;
        o->b69 = 1;
        e->b69 = 1;
        if (ax >= 0) {
            o->bbe = 8;
            o->wb0 = 2;
        } else {
            o->bbe = 9;
            o->wb0 = -2;
        }
        return;
    }
    o->y.p.whole = (e->y.p.whole + ty) + ((e->box3 - e->box2) + (o->box3 - o->box2));
    if (o->velY < 0) o->velY = 0;
}
