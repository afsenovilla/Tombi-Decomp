// FUNC 801278c0 1820 X001
/* score 345 (was 417; size now 1820 = game). slope/box push-out (o vs e, mode, angle). Found: ax/ay short (compared as
   short, == 0 tests), v unsigned with `(..)+0xc00 / else` form (srl, andi per arm), the b9c / type-step-state exits are
   plain `return;` (v0 = the loaded field in the game), quadrant base copied into a second var b, first x-block written
   `ang >= 0x800 ? .. : ang >> 4` so ang>>4 is the shared tail. Left: gcc here reassociates e->y - (ky + 4) into
   (e->y - 4) - ky and e->h + (kx + 12) into (e->h + 12) + kx (the game keeps ky+4 / kx+12 first); the same for the last
   x-block pair; register choice (box1 $8, dx sum $5, abs temps); last-block d8c tail order. -fno-expensive-optimizations
   gives 340 but moves e to $19. */
#include "TOBJ.H"
extern int rcos(int);
extern int rsin(int);

int func_801278C0(TObj *o, TObj *e, unsigned char mode, int ang)
{
    unsigned short dx, dy;
    int cx, cy;
    short ax, ay, t;
    short sel;
    short ky, kx;
    int base, b; unsigned int v;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = o->h->p.whole - e->h->p.whole;
    if ((unsigned short)(e->box0 + dx) > e->box1) return 0;
    dy = o->y.p.whole - e->y.p.whole;
    if ((unsigned short)(e->box0 + dy) > e->box1) return 0;
    switch (mode) {
    case 1:
    case 2:
        base = (short)dy < 0 ? 0xc00 : 0;
        b = base;
        if ((short)dx > 0) {
            if (base) b = base - 0x400;
            else b = 0x400;
        }
        if ((unsigned)(((ang - b) & 0xfff) - 0x500) >= 0x901) return 1;
    case 0:
        cx = (unsigned)(rcos(ang) * e->box0) >> 12;
        cy = (unsigned)(rsin(ang) * e->box0) >> 12;
        break;
    }
    ax = cx;
    if ((short)cx < 0) ax = -cx;
    ay = cy;
    if ((short)cy < 0) ay = -cy;
    sel = ax < ay;
    if (ax == 0) sel = 3;
    else if (ay == 0) sel = 2;
    switch (sel) {
    case 0:
        if ((short)dx < 0) t = -dx; else t = dx;
        if (ax < t) return 2;
        ky = (short)dx * (short)cy / (short)cx;
        break;
    case 1:
        if ((short)dy < 0) t = -dy; else t = dy;
        if (ay < t) return 2;
        kx = (short)dy * (short)cx / -(short)cy;
        break;
    case 2:
        ky = 0;
        break;
    case 3:
        kx = 0;
        break;
    }
    if (!(sel & 1)) {
        if (o->y.p.whole < e->y.p.whole - ky) {
            if (o->y.p.whole + o->box3 - o->box2 < e->y.p.whole - (ky + 4)) return 3;
            o->y.p.whole = (e->y.p.whole - (ky + 4)) - (o->box3 - o->box2);
            o->b69 = 1;
            if (o->category != 2) return 2;
            if (o->b9c) return;
            if (o->type == 0 && o->step == 2) return;
            if ((unsigned)(ang - 0x401) < 0x3ff || ang >= 0xc01) v = (ang & 0x3ff) + 0xc00;
            else v = ang & 0x3ff;
            o->d8c = v >> 4;
            return;
        } else {
            if (e->y.p.whole - (ky - 4) < o->y.p.whole - o->box2) return 3;
            o->y.p.whole = o->box2 + (e->y.p.whole - (ky - 4));
            if (o->category != 2) return 2;
            o->b69 = 4;
            if (o->b9c == 1) {
                if (o->velV < 0) o->velV = 0;
                return;
            }
            if (o->b9c) return;
            if (o->type == 0 && o->step == 2) return;
            if ((unsigned)(ang - 0x401) < 0x3ff || ang >= 0xc01) v = (ang & 0x3ff) + 0xc00;
            else v = ang & 0x3ff;
            o->d8c = ((v >> 4) + 0x80) & 0xff;
            return;
        }
    }
    if (o->category == 2 && o->type != 0) {
        if (e->h->p.whole + kx < o->h->p.whole) {
            if (e->h->p.whole + (kx + 0xc) < o->h->p.whole) return 3;
            o->h->p.whole = e->h->p.whole + (kx + 0xc);
            o->b9d = 3;
            if (ang < 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
            else o->d8c = ang >> 4;
        } else {
            if (o->h->p.whole < e->h->p.whole + (kx - 0xc)) return 3;
            o->h->p.whole = e->h->p.whole + (kx - 0xc);
            o->b9d = 2;
            if (ang >= 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
            else o->d8c = ang >> 4;
        }
        return 3;
    }
    if (e->h->p.whole + kx < o->h->p.whole) {
        if (e->h->p.whole + (kx + 0xc) < o->h->p.whole - o->box0) return 3;
        o->h->p.whole = o->box0 + (e->h->p.whole + (kx + 0xc));
        o->b9d = 3;
        if (o->category != 2) return 2;
        if (o->b9c) return;
        if (o->type == 0 && o->step == 2 && o->state != 2) return;
        if (ang < 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
        else o->d8c = ang >> 4;
    } else {
        if (o->h->p.whole + (o->box1 - o->box0) < e->h->p.whole + (kx - 0xc)) return 3;
        o->h->p.whole = (e->h->p.whole + (kx - 0xc)) - (o->box1 - o->box0);
        o->b9d = 2;
        if (o->category != 2) return 2;
        if (o->b9c) return;
        if (o->type == 0 && o->step == 2 && o->state != 2) return;
        if (ang < 0x800) o->d8c = ang >> 4;
        else o->d8c = ((ang + 0x800) & 0xfff) >> 4;
    }
}
