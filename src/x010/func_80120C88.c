// FUNC 80120c88 824 X010
// MATCHING 80120c88 824
#include "gv2.h"
/* Match keys: the subtraction operands are split into temps with an empty asm("" ::: "$2") clobber between the loads and the subu (puts the e->... load in v1 and schedules box0 before the subu); velX/velY store the raw dx/dy. */
#include "TOBJ.H"
extern int SquareRoot0(int);
extern int ratan2(int, int);
extern int rsin(int);
extern int rcos(int);

#define HIT(o, e)        \
    {                    \
        e->b69 = 0;      \
        e->velX = 0;     \
        e->velY = 0;     \
        e->b69 = 0;      \
        o->wb0 = 0;      \
        o->y.p.frac = 0; \
        o->b69 = 1;      \
        o->velY = 0;     \
    }

void func_80120C88(TObj *o, TObj *e)
{
    short dx, dy;
    int x, y;
    int r;
    unsigned short a;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    { unsigned short t1 = o->h->p.whole, t2 = e->h->p.whole; asm("" ::: "$2"); dx = t1 - t2; }
    if ((unsigned short)(e->box0 + dx) >= e->box1) return;
    { unsigned short t1 = o->y.p.whole, t2 = e->y.p.whole; asm("" ::: "$2"); dy = t1 - t2; }
    if ((unsigned short)(e->box2 + dy) >= e->box3) return;
    x = dx;
    y = dy;
    r = SquareRoot0(x * x + y * y);

    if (y >= -7) r += 2;
    if (r >= e->box0) return;
    a = ratan2(y, x);
    o->y.raw = e->y.raw + ((rsin((short)a) * e->box0) << 4);
    o->h->raw = e->h->raw + ((rcos((short)a) * e->box0) << 4);
                    e->b69 = 1;
                    e->velX = dx;
                    e->velY = dy;
                    if (((unsigned char*)&GV2.bc3)[0] & 0x40) {
                        if (1 & o->b9c) return;
                        if ((unsigned short)((a & 0xfff) - 0x901) < 0x5ff) {
                            if (e->velH > 0) {
                                if (o->ba6 == 2) { HIT(o, e); return; }
                            } else {
                                if (o->ba6 == 3) { HIT(o, e); return; }
                            }
                            goto push;
                        }
                        if (o->velY < 0) o->velY = 0;
                    } else {
                        if (o->b9c & 1) return;
                        if (o->y.p.whole >= -0x3bb) return;
                        if ((unsigned short)((a & 0xfff) - 0x901) >= 0x5ff) return;
                        if (e->velH > 0) {
                            if (o->ba6 == 2) { HIT(o, e); return; }
                        } else {
                            if (o->ba6 == 3) { HIT(o, e); return; }
                        }
                    push:
                        o->h->raw += e->velH << 5;
                        o->wb0 = 0;
                        o->y.p.frac = 0;
                        o->b69 = 1;
                        o->velY = 0;
                    }
}
