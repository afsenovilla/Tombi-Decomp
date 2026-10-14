// FUNC 801278c0 1820 X001
// MATCHING 801278c0 1820
/* Matched (opusQ): itbrute priority bumps (cx/ax/cy), `/ (short)cx` in the s2 divide, and a local
   `Fix16 *oh = o->h;` pointer in the category==2 block. */
#include "TOBJ.H"

extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    return r;
}

int func_801278C0(TObj *o, TObj *e, unsigned char mode, int ang)
{
    short dx, dy, cy, ax, ay, s2, s7; int cx;
    short kind;
    unsigned int v;
    int q, r;
asm("" :: "r"(cx));

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = o->h->p.whole - e->h->p.whole;
    ax = e->box0 + dx;
asm("" : "=r"(ax) : "0"(ax));
    if ((unsigned short)ax > e->box1) return 0;
    dy = o->y.p.whole - e->y.p.whole;
    ay = e->box0 + dy;
    if ((unsigned short)ay > e->box1) return 0;
    switch (mode) {
    case 1:
    case 2:
        q = (dy < 0) ? 0xc00 : 0;
        r = q;
        if (dx > 0) {
            if (q == 0) r = 0x400;
            else r = q - 0x400;
asm("" :: "r"(cx));
        }
        if ((unsigned)(((ang - r) & 0xfff) - 0x500) > 0x900) return 1;
    case 0:
        cx = (unsigned)(rcos(ang) * e->box0) >> 12;
        cy = (unsigned)(rsin(ang) * e->box0) >> 12;
    }
    ax = cx;
    ay = cy;
    if ((short)cx < 0) ax = -cx;
    if (ay < 0) ay = -ay;
    kind = ax < ay;
    if (ax == 0) kind = 3;
    else if (ay == 0) kind = 2;
    switch (kind) {
    case 0:
        if (ax < sabs(dx)) return 2;
        s2 = dx * cy / (short)cx;
        break;
    case 1:
        if (ay < sabs(dy)) return 2;
        s7 = dy * (short)cx / -cy;
        break;
    case 2:
        s2 = 0;
        break;
    case 3:
        s7 = 0;
        break;
    }
    if (!(kind & 1)) {
        if (o->y.p.whole < e->y.p.whole - s2) {
            if (-(s2 + 4) + e->y.p.whole > o->y.p.whole + o->box3 - o->box2) return 3;
            o->y.p.whole = (-(s2 + 4) + e->y.p.whole) - (o->box3 - o->box2);
            o->b69 = 1;
            if (o->category != 2) return 2;
            if (o->b9c) return;
            if (o->type == 0 && o->step == 2) return;
            if ((unsigned)(ang - 0x401) < 0x3ff || ang >= 0xc01) v = (ang & 0x3ff) + 0xc00;
            else v = ang & 0x3ff;
            o->d8c = v >> 4;
            return;
        } else {
            if (-(s2 + -4) + e->y.p.whole < o->y.p.whole - o->box2) return 3;
            o->y.p.whole = o->box2 + (-(s2 + -4) + e->y.p.whole);
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
        Fix16 *oh = o->h;
        if (e->h->p.whole + s7 < oh->p.whole) {
            if (e->h->p.whole - -(s7 + 0xc) < oh->p.whole) return 3;
            oh->p.whole = e->h->p.whole - -(s7 + 0xc);
            o->b9d = 3;
            if (ang < 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
            else o->d8c = ang >> 4;
        } else {
            if (oh->p.whole < e->h->p.whole - -(s7 + -0xc)) return 3;
            oh->p.whole = e->h->p.whole - -(s7 + -0xc);
            o->b9d = 2;
            if (ang >= 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
            else o->d8c = ang >> 4;
        }
        return 3;
    }
    if (e->h->p.whole + s7 < o->h->p.whole) {
        if (e->h->p.whole - -(s7 + 0xc) < o->h->p.whole - o->box0) return 3;
        o->h->p.whole = o->box0 + (e->h->p.whole - -(s7 + 0xc));
        o->b9d = 3;
        if (o->category != 2) return 2;
        if (o->b9c) return;
        if (o->type == 0 && o->step == 2 && o->state != 2) return;
        if (ang < 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
        else o->d8c = ang >> 4;
    } else {
        if (e->h->p.whole - -(s7 + -0xc) > o->h->p.whole + (o->box1 - o->box0)) return 3;
        o->h->p.whole = (e->h->p.whole - -(s7 + -0xc)) - (o->box1 - o->box0);
        o->b9d = 2;
        if (o->category != 2) return 2;
        if (o->b9c) return;
        if (o->type == 0 && o->step == 2 && o->state != 2) return;
        if (ang >= 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
        else o->d8c = ang >> 4;
    }
    ay = q;
asm("" : "=r"(cy) : "0"(cy));
}
