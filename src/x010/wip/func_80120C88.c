// FUNC 80120c88 824 X010
/* score 38 (o30: short dx,dy + int x,y; was 72): remaining diff at +0x60 box0/box1 load order. Old note, score 72: everything matches except the first two box tests: the dx temp gets a0 (prefers the dying o->h whole reg) instead of a1, so box0/box1 loads schedule differently and two load-delay nops shift all branches. Without the velX/velY stores that part matches (cf. func_80121A40). Tried: early returns, int/short/ushort dx/dy, separate sx/sy copies, operand orders, ratan2 short params, a/y reuse */
#include "TOBJ.H"
extern unsigned char D_8009D2C3[];
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
    dx = o->h->p.whole - e->h->p.whole;
    if ((unsigned short)(e->box0 + dx) >= e->box1) return;
    dy = o->y.p.whole - e->y.p.whole;
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
                    e->velY = y;
                    if (D_8009D2C3[0] & 0x40) {
                        if (o->b9c & 1) return;
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
