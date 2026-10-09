// FUNC 80128a2c 2908 X001
// MATCHING 80128a2c 2908
#include "TOBJ.H"
typedef struct { short w0, w2, w4, w6, w8; } S;

extern char D_80077CDC[];
extern char D_80077CE8[];
extern void *D_8013FC70[];
extern void *D_8013FC74[];
extern void *D_8013FC78[];
extern void *D_8013FCC4[];
extern void *D_8013FCC8[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_1F80027E;
extern short D_1F800284;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern void FUN_8001faf4(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short func_8004065C(TObj *, short, short, int);
extern short FUN_80040278(TObj *, short, short);
extern short FUN_800408d8(TObj *, short, short);
extern void func_80128110(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

static __inline__ short chk(TObj *o)
{
    S *q = (S *)&o->wb4;
    short f;
    short d;

    if (o->d8c & 0x80) {
        d = -0x10;
        f = 1;
    } else {
        d = 0x10;
        f = 0;
    }
    if ((o->b9d & 2) && f == (o->b9d & 1)) {
        q->w6 = 1;
        return 1;
    }
    if (func_8004065C(o, o->h->p.whole + d, o->y.p.whole, f)) {
        q->w6 = 0;
        return 1;
    }
    return 0;
}

static __inline__ int land(TObj *o)
{
    if (o->b69 == 1) {
        o->wae = -1;
        o->wb2 = 0;
        o->b69 = 0;
        o->wba = 1;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        o->wba = 0;
        o->wb2 = D_1F80027E;
        o->wae = D_1F800284;
        o->d8c = (-D_1F80027E << 2) & 0xff;
        return 1;
    }
    return 0;
}

void func_80128A2C(TObj *o)
{
    S *p = (S *)&o->wb4;
    int dy, dx;

    if (o->visible && ((D_1F8001F8 + D_1F800198) & 0xf) == 0) playSFX(0x5e);
    switch (o->state) {
    case 0:
        o->b9c = 0;
        if (p->w8) {
            if (o->wb0 == 0) {
                if (o->animFrame & 1) o->d8c = 0x40;
                else o->d8c = 0xc0;
                o->movetab = D_80077CDC;
                if (p->w6) o->state = 10;
                else o->state = 7;
                o->wac = 0x16;
            } else {
                o->wac = 2;
                o->state++;
                o->animFrame = 1 - o->animFrame;
            }
        } else {
            if (o->wb0 == 0) {
                o->wac = 2;
                o->state++;
            } else {
                if (o->animFrame & 1) o->d8c = 0x40;
                else o->d8c = 0xc0;
                o->movetab = D_80077CDC;
                o->animFrame = 1 - o->animFrame;
                if (p->w6) o->state = 10;
                else o->state = 7;
                o->wac = 0x16;
            }
        }
        setAnim(o, D_8013FC70[o->wac]);
        break;
    case 1:
        if (!AnimAdvance(o)) break;
        if (p->w8) {
            o->animFrame = 1 - o->animFrame;
            p->w8 = 0;
        }
        o->state++;
        if (o->animFrame & 1) o->d8c = 0xc0;
        else o->d8c = 0x40;
        o->wac = 1;
        setAnim(o, D_8013FC74[0]);
        break;
    case 2:
        AnimAdvance(o);
        if (o->d8c & 0x80) {
            if (o->wb0 == 0) {
                dy = -0x80;
                o->animFrame = 1;
            } else {
                dy = 0x80;
                o->animFrame = 0;
            }
            dx = -0x300;
        } else {
            if (o->wb0 == 0) {
                dy = -0x80;
                o->animFrame = 0;
            } else {
                dy = 0x80;
                o->animFrame = 1;
            }
            dx = 0x300;
        }
        o->y.raw += dy << 8;
        o->h->raw += dx << 8;
        if (!chk(o)) {
            if (o->wb0 & 1) {
                if (p->w6) o->state = 13;
                else o->state = 11;
            } else {
                if (p->w6) o->state = 8;
                else o->state = 3;
            }
            break;
        }
        if (o->wb0 & 1) {
            if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
                p->w8 = 0;
                o->state = 5;
                break;
            }
        } else {
            if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
                p->w8 = 1;
                o->state = 5;
                break;
            }
        }
        if (--o->timer == -1) {
            o->timer = 0x80;
            func_80128110(o);
            if (o->step == 1) {
                o->step = 2;
                o->ba7 = 0;
                if (o->w22 == 2) {
                    if (o->y.p.whole > o->wb6) o->wb0 = 0;
                    else o->wb0 = 1;
                }
            } else {
                o->state = 0;
                o->ba7 = 0;
            }
        }
        break;
    case 3:
    case 8:
        o->movetab = D_80077CE8;
        o->d8c = 0;
        o->wac = 0x15;
        o->state++;
        setAnim(o, D_8013FCC4[0]);
        break;
    case 4:
        FUN_8001faf4(o);
        o->y.raw -= 0xc000;
        if (AnimAdvance(o)) {
            o->step = 1;
            o->y.raw += 0x40000;
            o->state = 0;
            land(o);
        }
        break;
    case 5:
        if (p->w8) {
            if (o->animFrame & 1) o->d8c = 0xc0;
            else o->d8c = 0x40;
        }
        o->movetab = D_80077CE8;
        o->b69 = 0;
        o->wac = 2;
        o->state++;
        setAnim(o, D_8013FC78[0]);
        break;
    case 6:
        if (p->w8) {
            FUN_8001fa88(o, 1 - o->animFrame);
            o->y.raw -= 0x8000;
            if (AnimAdvance(o)) {
                o->state = 0;
                if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
                    o->d8c = ((-D_1F80027E << 2) + 0x80) & 0xff;
                    p->w6 = 0;
                    o->step = 8;
                    o->wac = 1;
                    o->animFrame = 1 - o->animFrame;
                    setAnim(o, D_8013FC74[0]);
                } else {
                    p->w8 = 0;
                    o->step = 7;
                }
            } else {
                FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10);
            }
        } else {
            FUN_8001faf4(o);
            o->y.raw += 0x8000;
            if (AnimAdvance(o)) {
                o->state = 0;
                if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
                    o->wb2 = D_1F80027E;
                    o->d8c = (-D_1F80027E << 2) & 0xff;
                    o->wae = D_1F800284;
                    p->w6 = 0;
                    o->step = 1;
                    o->wac = 1;
                    setAnim(o, D_8013FC74[0]);
                } else {
                    o->step = 7;
                }
            } else {
                FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10);
            }
        }
        break;
    case 7:
        if (p->w8) {
            FUN_8001faf4(o);
            o->y.p.whole--;
        } else {
            FUN_8001fa88(o, 1 - o->animFrame);
            o->y.p.whole++;
        }
        if (!AnimAdvance(o)) break;
        if (p->w8) {
            p->w8 = 0;
            o->animFrame = 1 - o->animFrame;
        } else if (o->animFrame & 1) {
            o->d8c = 0xc0;
            o->animFrame = 0;
        } else {
            o->d8c = 0x40;
            o->animFrame = 1;
        }
        o->state = 2;
        o->wac = 1;
        setAnim(o, D_8013FC74[0]);
        break;
    case 9:
        FUN_8001faf4(o);
        if (AnimAdvance(o)) {
            o->step = 1;
            o->state = 0;
            o->y.p.whole += 4;
        }
        break;
    case 10:
        if (p->w8) o->y.p.whole--;
        else o->y.p.whole++;
        if (!AnimAdvance(o)) break;
        if (p->w8) {
            p->w8 = 0;
            if (o->animFrame & 1) {
                o->animFrame = 0;
                o->d8c = 0x40;
            } else {
                o->d8c = 0xc0;
                o->animFrame = 1;
            }
        } else if (o->animFrame & 1) {
            o->d8c = 0xc0;
            o->animFrame = 0;
            o->h->p.whole -= 3;
        } else {
            o->d8c = 0x40;
            o->animFrame = 1;
            o->h->p.whole += 3;
        }
        o->state = 2;
        o->wac = 1;
        setAnim(o, D_8013FC74[0]);
        break;
    case 11:
    case 13:
        o->d8c = 0x80;
        o->movetab = D_80077CE8;
        o->b69 = 0;
        o->wac = 0x16;
        o->animFrame = 1 - o->animFrame;
        o->state++;
        setAnim(o, D_8013FCC8[0]);
        break;
    case 12:
        FUN_8001faf4(o);
        o->y.raw += 0x5000;
        if (AnimAdvance(o)) {
            o->state = 0;
            o->y.p.whole -= 4;
            if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
                o->d8c = ((-D_1F80027E << 2) + 0x80) & 0xff;
                p->w6 = 0;
                o->step = 8;
            } else {
                o->step = 7;
            }
        }
        break;
    case 14:
        FUN_8001faf4(o);
        if (AnimAdvance(o)) {
            o->b69 = 0;
            o->y.p.whole -= 3;
            o->state++;
        }
        break;
    case 15:
        o->state = 0;
        if (o->b69 == 4) {
            p->w6 = 1;
            o->b69 = 0;
            o->step = 8;
        } else if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
            o->b69 = 0;
            o->d8c = ((-D_1F80027E << 2) + 0x80) & 0xff;
            p->w6 = 0;
            o->step = 8;
        } else {
            o->step = 7;
        }
        break;
    }
}
