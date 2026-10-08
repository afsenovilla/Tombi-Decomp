/* wip: score 61. Falla: el juego calcula dy en v1 y lo copia a a1 (addu a1,v1,zero) antes del test de box2/box3,
   y carga b->box2 antes que a->box2; copiar dy a otra variable o pasarlo a un inline no lo reproduce. */
// FUNC 800439f4 640 MAIN0
#include "TOBJ.H"

int func_800439F4(TObj *a, TObj *b)
{
    short s, dx, ax, w, off, d0, sy;
    int dy;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) > 0x5a) return 0;
    s = b->box0 + a->box0;
    off = s;
    dx = a->h->p.whole - b->h->p.whole;
    ax = dx;
    if ((unsigned short)(dx + s) > b->box1 + a->box1) return 0;
    dy = (unsigned short)a->y.p.whole - (unsigned short)b->y.p.whole;
    if ((unsigned short)(dy + (b->box2 + a->box2)) > a->box3 + b->box3) return 0;
    w = off;
    d0 = ax;
    if (dx < 0) {
        ax = -dx;
        off = -s;
    } else {
        off = (b->box1 - b->box0) + (a->box1 - a->box0);
        w = off;
    }
    if ((unsigned short)(w - ax) < 4) {
        a->h->p.whole = b->h->p.whole + off;
        a->ba6 = off >= 0 ? 3 : 2;
        return 2;
    }
    sy = dy;
    if (sy <= 0) {
        if (a->b9c & 1) {
            a->h->p.whole = b->h->p.whole + off;
            return 2;
        }
        a->y.p.frac = 0;
        a->b69 = 1;
        a->y.p.whole = b->y.p.whole - (b->box2 + a->box2);
        b->b69 = 1;
        if (d0 >= 0) {
            a->bbe = 8;
            a->wb0 = 2;
        } else {
            a->bbe = 9;
            a->wb0 = -2;
        }
        return 1;
    }
    if ((b->box3 - b->box2) + (a->box3 - a->box2) - sy < 5) {
        a->y.p.whole = b->y.p.whole + ((b->box3 - b->box2) + (a->box3 - a->box2));
        if (a->velY < 0) {
            a->velY = 0;
            b->b69 = 1;
            if (b->type == 5) b->b69 = 4;
        }
        return 3;
    }
    a->h->p.whole = b->h->p.whole + off;
    a->ba6 = off >= 0 ? 3 : 2;
    return 2;
}
