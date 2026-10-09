// FUNC 80129e14 252 X004
/* score 40: layout/branches match (inlines with snap block after the far compare); only register allocation of the inline copies differs: game v (velH) orig t1 / copy t2, t a1/a2, x v1/a3, y part v a1/a3; ours gives v-orig a3 first. Tried param orders, int/short types, test inside inline, pointer params, compare inline. */
#include "TOBJ.H"
extern unsigned char D_8009CF29;

static __inline__ void moveh(TObj *o, short v)
{
    Fix16 *h;
    short x;
    short t;

    h = o->h; t = o->velX; x = h->p.whole;
    if (x < t) {
        if (x + v < t) {
            h->p.whole = x + v;
            return;
        }
        goto snap;
    }
    if (t >= x - v) {
    snap:
        h->p.whole = t;
        o->velH = 0;
    } else {
        h->p.whole = x - v;
    }
}

static __inline__ void movey(TObj *o, short v)
{
    short y;
    short t;

    y = o->y.p.whole; t = o->velY;
    if (t >= y - v) {
        o->y.p.whole = t;
        o->velV = 0;
    } else {
        o->y.p.whole = y - v;
    }
}

void func_80129E14(TObj *o)
{
    if (*(int *)&o->velH == 0) return;
    if (o->velH) moveh(o, o->velH);
    if (o->velV) movey(o, o->velV);
    if (o->d->p.whole < 0x1c) o->d->p.whole += 2;
    if (*(int *)&o->velH == 0) D_8009CF29 = 0;
}
