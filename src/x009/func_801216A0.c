// FUNC 801216a0 344 X009
// MATCHING 801216a0 344
#include "TOBJ.H"
extern short D_1F80019E;
extern short func_801215C0(TObj *, short, short, unsigned char);

void func_801216A0(TObj *o, TObj *e)
{
    short t;
    short u;
    if (o->active == 7) return;
    if (o->b9c & 1) return;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    if ((unsigned short)(e->box0 + (o->h->p.whole - e->h->p.whole)) >= e->box1) return;
    if ((unsigned short)(e->box2 + (o->y.p.whole - e->y.p.whole)) >= e->box3) return;
    u = o->h->p.whole - (e->h->p.whole - e->box0);
    t = o->y.p.whole - (e->y.p.whole + (e->box3 - e->box2));
    if (func_801215C0(o, u, t + o->box3 - o->box2, e->subtype)) {
        o->b69 = 1;
        o->h->p.whole += e->velX;
        { short w = o->y.p.whole + e->velY;
        D_1F80019E = 0;
        *(short *)((char *)o + 0x14) = 0;
        o->y.p.whole = w; }
    }
}
