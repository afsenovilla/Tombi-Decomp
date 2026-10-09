// FUNC 801305d0 2036 X001
/* score 198: only the two inlined side() probes differ: the game schedules `move a0,s0` before `lw v0,0x40(s0)` (+nop) at the d/f merge; everything else matches. Tried: void* param, separate probe inline, call per branch (worse) */
#include "TOBJ.H"
typedef struct { short w0, w2, w4, w6, w8; } S;
extern char D_80077CF4[];
extern char D_80077CE8[];
extern short D_1F80027E;
extern int AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001fb84(TObj *, char *);
extern short FUN_8004065c(TObj *, short, short, int);
extern short FUN_80040278(TObj *, short, short);
extern short FUN_800408d8(TObj *, short, short);

static __inline__ short side(TObj *o)
{
    short d;
    int f;

    if (o->wb6) {
        if (o->wb0 == 0) {
            if (o->animFrame & 1) goto neg;
            goto pos;
        } else {
            if (o->animFrame & 1) goto pos;
            goto neg;
        }
    }
    if (o->d8c & 0x80) {
    neg:
        d = -0x10;
        f = 1;
    } else {
    pos:
        d = 0x10;
        f = 0;
    }
    return FUN_8004065c(o, o->h->p.whole + d, o->y.p.whole, f);
}

static __inline__ int land(TObj *o)
{
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->d84 = (-D_1F80027E << 2) & 0xff;
        return 1;
    }
    return 0;
}

void func_801305D0(TObj *o)
{
    S *p;
    int t, k, dy, dx;

    AnimAdvance(o);
    p = (S *)&o->wb4;
    switch (o->state) {
    case 0:
        if (p->w2) {
            if (o->wb0 == 0) goto five;
        } else if (o->wb0) {
            goto five;
        }
        o->state++;
        break;
    five:
        o->state = 5;
        break;
    case 1:
        k = 0xc0;
        if (o->animFrame & 1) {
            t = o->d8c - 2;
        } else {
            k = 0x40;
            t = o->d8c + 2;
        }
        t &= 0xff;
        o->d8c = t;
        if (t == k) o->state++;
    case 2:
        if (o->d8c & 0x80) {
            if (o->animFrame & 1) {
                dy = -0x100;
                o->wb0 = 0;
            } else {
                dy = 0x100;
                o->wb0 = 1;
            }
            dx = -0x300;
        } else {
            if (o->animFrame & 1) {
                dy = 0x100;
                o->wb0 = 1;
            } else {
                dy = -0x100;
                o->wb0 = 0;
            }
            dx = 0x300;
        }
        o->y.raw += dy << 8;
        o->h->raw += dx << 8;
        if (!side(o)) {
            if (o->wb0 & 1) {
                o->timer = 0x10;
                o->state = 6;
            } else {
                o->state = 3;
            }
            break;
        }
        if (o->wb0 & 1) {
            if (!FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) break;
            p->w2 = 0;
            o->timer = 0x10;
            o->state = 4;
        } else {
            if (!FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) break;
            p->w2 = 1;
            o->timer = 0x10;
            o->state = 4;
        }
        break;
    case 3:
        FUN_8001fb84(o, D_80077CF4);
        o->y.raw += -0xc000;
        if (o->animFrame & 1)
            o->d8c += 2;
        else
            o->d8c -= 2;
        o->d8c = *(unsigned char *)&o->d8c;
        if (land(o)) {
            o->step = 1;
            o->state = 0;
            o->d8c = o->d84;
        }
        break;
    case 4:
        if (p->w2) {
            o->movetab = D_80077CE8;
            FUN_8001fa88(o, (unsigned short)(1 - o->animFrame));
            o->y.raw += -0x10000;
            if (o->animFrame & 1)
                o->d8c -= 4;
            else
                o->d8c += 4;
            o->d8c = *(unsigned char *)&o->d8c;
            if (o->timer) o->timer--;
            if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
                o->d84 = ((-D_1F80027E << 2) + 0x80) & 0xff;
                if (o->timer == 0 || o->d8c == o->d84) {
                    o->step = 3;
                    o->state = 0;
                }
            } else if (o->timer == 0) {
                o->step = 3;
                o->state = 0;
            }
        } else {
            FUN_8001fb84(o, D_80077CF4);
            o->y.raw += 0xc000;
            land(o);
            if (o->animFrame & 1) {
                o->d8c = (o->d8c - 2) & 0xff;
                if (o->d8c == 0x80) {
                    o->step = 1;
                    o->state = 0;
                }
            } else {
                o->d8c = (o->d8c + 2) & 0xff;
                if (o->d8c == 0) {
                    o->step = 1;
                    o->state = 0;
                }
            }
        }
        break;
    case 5:
        if (o->animFrame & 1)
            o->d8c += 2;
        else
            o->d8c -= 2;
        o->d8c = *(unsigned char *)&o->d8c;
        if (p->w2) {
            o->movetab = D_80077CF4;
            FUN_8001fa88(o, (unsigned short)(1 - o->animFrame));
            o->y.p.whole--;
        } else {
            FUN_8001fb84(o, D_80077CF4);
            o->y.p.whole++;
        }
        if (side(o)) {
            o->state = 2;
            if (p->w2) {
                p->w2 = 0;
                if (o->animFrame & 1)
                    o->d8c = 0xc0;
                else
                    o->d8c = 0x40;
            } else {
                if (o->animFrame & 1)
                    o->d8c = 0x40;
                else
                    o->d8c = 0xc0;
            }
        }
        break;
    case 6:
        o->movetab = D_80077CE8;
        FUN_8001fa88(o, (unsigned short)(1 - o->animFrame));
        o->y.raw += 0xc000;
        if (o->animFrame & 1)
            o->d8c -= 4;
        else
            o->d8c += 4;
        o->d8c = *(unsigned char *)&o->d8c;
        if (o->timer) o->timer--;
        if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
            o->d84 = ((-D_1F80027E << 2) + 0x80) & 0xff;
            if (o->timer == 0 || o->d8c == o->d84) {
                o->step = 3;
                o->state = 0;
            }
        } else if (o->timer == 0) {
            o->step = 3;
            o->state = 0;
        }
        break;
    }
}
