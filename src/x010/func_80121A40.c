// FUNC 80121a40 364 X010
// MATCHING 80121a40 364
#include "TOBJ.H"
extern int SquareRoot0(int);
extern int ratan2(int, int);
extern int rsin(int);
extern int rcos(int);

void func_80121A40(TObj *o, TObj *e)
{
    unsigned short dx, dy;
    short x, y, a;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) < 0x5b) {
        dx = o->h->p.whole - e->h->p.whole;
        if ((unsigned short)(e->box0 + dx) <= e->box1) {
            dy = o->y.p.whole - e->y.p.whole;
            if ((unsigned short)(e->box2 + dy) <= e->box3) {
                x = dx;
                y = dy;
                if (SquareRoot0(x * x + y * y) < e->box0) {
                    a = ratan2(y, x);
                    o->y.raw = e->y.raw + ((rsin(a) * e->box0) << 4);
                    o->h->raw = e->h->raw + ((rcos(a) * e->box0) << 4);
                    o->b69 = 1;
                }
            }
        }
    }
}
