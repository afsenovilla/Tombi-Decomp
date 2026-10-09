// FUNC 801278c0 1820 X001
/* score 417 (draft, 1796 of 1820 B; o17: abs/sel block rewritten after the game: unsigned short ax/ay/t compared as (short), t = |dx| per case): slope/box push-out (o vs e, mode, angle); logic decoded from the asm, all branches present. Unsolved: 0x80 frame (0x48 B of locals unused here: an inlined helper with arrays?), register choice (dx/dy temps, abs copies), quadrant base code. Some paths return no value (v0 = stored d8c / velV in the game) */
#include "TOBJ.H"
extern int rcos(int);
extern int rsin(int);

int func_801278C0(TObj *o, TObj *e, unsigned char mode, int ang)
{
    unsigned short dx, dy;
    int cx, cy;
    unsigned short ax, ay, t;
    short sel;
    short ky, kx;
    int base, v;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = o->h->p.whole - e->h->p.whole;
    if ((unsigned short)(e->box0 + dx) > e->box1) return 0;
    dy = o->y.p.whole - e->y.p.whole;
    if ((unsigned short)(e->box0 + dy) > e->box1) return 0;
    switch (mode) {
    case 1:
    case 2:
        base = (short)dy < 0 ? 0xc00 : 0;
        if ((short)dx > 0) {
            if (base) base -= 0x400;
            else base = 0x400;
        }
        if ((unsigned)(((ang - base) & 0xfff) - 0x500) >= 0x901) return 1;
    case 0:
        cx = (unsigned)(rcos(ang) * e->box0) >> 12;
        cy = (unsigned)(rsin(ang) * e->box0) >> 12;
        break;
    }
    ax = cx;
    if ((short)cx < 0) ax = -cx;
    ay = cy;
    if ((short)cy < 0) ay = -cy;
    sel = (short)ax < (short)ay;
    if ((short)ax == 0) sel = 3;
    else if ((short)ay == 0) sel = 2;
    switch (sel) {
    case 0:
        t = dx;
        if ((short)dx < 0) t = -dx;
        if ((short)ax < (short)t) return 2;
        ky = (short)dx * (short)cy / (short)cx;
        break;
    case 1:
        t = dy;
        if ((short)dy < 0) t = -dy;
        if ((short)ay < (short)t) return 2;
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
            if (o->b9c) return 2;
            if (o->type == 0 && o->step == 2) return 2;
            v = ang & 0x3ff;
            if ((unsigned)(ang - 0x401) < 0x3ff || ang >= 0xc01) v += 0xc00;
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
            v = ang & 0x3ff;
            if ((unsigned)(ang - 0x401) < 0x3ff || ang >= 0xc01) v += 0xc00;
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
            if (ang < 0x800) o->d8c = ang >> 4;
            else o->d8c = ((ang + 0x800) & 0xfff) >> 4;
        }
        return 3;
    }
    if (e->h->p.whole + kx < o->h->p.whole) {
        if (e->h->p.whole + (kx + 0xc) < o->h->p.whole - o->box0) return 3;
        o->h->p.whole = o->box0 + (e->h->p.whole + (kx + 0xc));
        o->b9d = 3;
        if (o->category != 2) return 2;
        if (o->b9c) return 2;
        if (o->type == 0 && o->step == 2 && o->state != 2) return 2;
        if (ang < 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
        else o->d8c = ang >> 4;
    } else {
        if (o->h->p.whole + (o->box1 - o->box0) < e->h->p.whole + (kx - 0xc)) return 3;
        o->h->p.whole = (e->h->p.whole + (kx - 0xc)) - (o->box1 - o->box0);
        o->b9d = 2;
        if (o->category != 2) return 2;
        if (o->b9c) return 2;
        if (o->type == 0 && o->step == 2 && o->state != 2) return 2;
        if (ang < 0x800) o->d8c = ang >> 4;
        else o->d8c = ((ang + 0x800) & 0xfff) >> 4;
    }
}
