// FUNC 800f1d44 2280 X014
// MATCHING 800f1d44 2280
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short x, y; } XY;
typedef struct {
    TObj o;
    char pc0[3];
    unsigned char c3;
    char pc4[0xcd - 0xc4];
    unsigned char cd, ce;
    char pcf[0xd5 - 0xcf];
    unsigned char d5, d6, d7;
} X;
#define XO(o) ((X *)(o))

extern TObj *D_8009C330;
extern unsigned char *D_8009D2E8;
extern unsigned char *D_800A611C;
extern volatile unsigned short D_8009D670[];
extern unsigned char D_8009D2B3;
extern unsigned char D_8009D2B1;
extern short D_8009C944[];
extern unsigned short D_8009D610;
extern unsigned int D_8009C984;
extern unsigned short D_1f8001f8, D_1f8003c4;
extern short D_1f800238;
extern XY D_801151E0[];
extern char D_80010DE0[], D_80010E2C[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8010f400(TObj *);
extern void FUN_800efa80(TObj *);
extern void FUN_8001fc14(TObj *, int, int);
extern void FUN_8001fce4(TObj *);
extern void FUN_8001e4f0(int);
extern int FUN_8005bdec(void);
extern short FUN_800eef0c(void);
extern TObj *FUN_80018448(void);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = D_8009C330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        D_8009C330->animFrame = D_8009C330->animTimer;
    }
}

static __inline__ void dust(TObj *o)
{
    short x, y, z;
    TObj *q;
    XY *t;
    x = o->a.p.whole;
    y = o->y.p.whole;
    z = o->b.p.whole;
    if (FUN_800eef0c() == 0 && D_1f800238 >= 6 && (q = FUN_80018448()) != 0) {
        q->active = 1;
        q->type = 0x31;
        q->subtype = 1;
        t = &D_801151E0[D_1f8001f8 & 7];
        q->a.p.whole = x;
        q->y.p.whole = y;
        q->b.p.whole = z;
        q->h->p.whole += t->x;
        q->y.p.whole += t->y;
    }
}

static __inline__ short odd(void)
{
    if (D_1f8001f8 & 0xf) return 1;
    return 0;
}

void func_800F1D44(TObj *o)
{
    unsigned char mode;
    short lim;
    short w;
    unsigned short t;

    if (U8(o, 0xac) >= 2) {
        D_8009D2E8 = D_800A611C;
        D_8009D2E8[4] = 2;
        D_8009D2E8[5] = 2;
        D_8009D2E8[6] = 0;
    }
    U8(o, 0xac) = 0;
    if (D_8009D670[0] & 0xa0) {
        switch (o->state) {
        case 0:
            XO(o)->cd = 0;
            XO(o)->ce = 0;
            U8(D_8009C330, 8) = 0;
            XO(o)->c3 = 0;
            o->velX = 0;
            o->state++;
        case 1:
            if (D_8009D2B3 >= 3) o->animTimer = 1;
            FUN_8001fec0(o);
            w = o->wb0;
            t = w;
            if (w < 0) {
                t = (t << 2) + 0x100;
                o->wb6 = t & 0xff;
            } else {
                t <<= 2;
                if (w > 0) o->wb6 = t & 0xff;
                else o->wb6 = 0;
            }
            FUN_8010f400(o);
            FUN_800efa80(o);
            FUN_8001fc14(o, o->wb6, o->wb2);
            o->h->raw += D_8009C944[0] << 8;
            o->y.raw += D_8009C944[1] << 8;
            FUN_8001fce4(o);
            if (U8(o, 0xad) == 0) {
                if (o->subtype != 0) {
                    if ((unsigned short)(o->wb0 + 5) >= 11) o->y.raw += 0x50000;
                    else o->y.raw += 0x30000;
                } else {
                    if ((unsigned short)(o->wb0 + 5) >= 11) o->y.raw += 0xc0000;
                    else o->y.raw += 0x80000;
                }
            }
            switch (o->ba6) {
            case 2:
                if (D_8009D670[0] & 0x20) {
                    if (o->wb2 >= 0x200) goto inc;
                    goto clr;
                }
                break;
            case 3:
                if (D_8009D670[0] & 0x80) {
                    if (o->wb2 < -0x1ff) {
                    inc:
                        U8(D_8009C330, 8)++;
                        if (U8(D_8009C330, 8) > 20) {
                            XO(o)->cd = 0;
                            XO(o)->ce = 0;
                            U8(D_8009C330, 8) = 0;
                            o->step = 0x1c;
                            o->state = 0;
                        }
                    } else {
                    clr:
                        U8(D_8009C330, 8) = 0;
                    }
                }
                break;
            case 4:
            case 5:
                o->wb2 = 0;
                break;
            default:
                if ((D_1f8001f8 & 7) == 0) FUN_8001e4f0(odd());
                break;
            }
            if (D_8009D610 == 7) {
                int f = o->animFrame & 3;
                if ((f == 0 && (XO(o)->d5 == 3 || XO(o)->d6 == 3 || XO(o)->d7 == 3 || o->bbf == 3)) ||
                    (f == 1 && (XO(o)->d5 == 2 || XO(o)->d6 == 2 || XO(o)->d7 == 2 || o->bbf == 2))) {
                    XO(o)->cd = 0;
                    XO(o)->ce = 0;
                    if ((o->ba6 & 6) == 0) {
                        o->wb2 = o->wbc;
                        setanim(o, 0x1a);
                        o->step = 0x13;
                        o->state = 0;
                    } else {
                        o->wb2 = 0;
                    }
                }
            } else if ((o->animFrame & 1) ? o->wb2 > 0x144 : o->wb2 < -0x144) {
                XO(o)->cd = 0;
                XO(o)->ce = 0;
                if (o->ba6 & 6) {
                    o->wb2 = 0;
                } else {
                    setanim(o, 0x1a);
                    o->step = 0x13;
                    o->state = 0;
                }
            }
            break;
        }
        mode = 0;
        if (D_8009D2B1 == 1) mode = 1;
        else if (D_8009D2B1 == 2) mode = 2;
        lim = 0x200;
        if (D_8009C984 & 0x40) {
            lim = 0x360;
            if (!(D_8009D670[0] & D_1f8003c4) && (unsigned)(o->subtype - 1) >= 2) lim = 0x200;
        }
        if (mode != 0) {
            if ((unsigned short)(o->wb2 + lim) >= lim * 2) {
                o->wb2 = (o->wb2 < 0) ? -lim : lim;
                if (XO(o)->cd == 0) {
                    XO(o)->cd = 1;
                    XO(o)->ce = (FUN_8005bdec() % 2) * 45;
                    if (XO(o)->ce == 0) XO(o)->ce = 12;
                } else {
                    XO(o)->ce--;
                }
                if (XO(o)->ce == 0) {
                    unsigned char st;
                    switch (mode) {
                    case 1: st = 42; break;
                    case 2: st = 43; break;
                    default: goto done;
                    }
                    XO(o)->cd = 0;
                    XO(o)->ce = 0;
                    o->step = st;
                    o->state = 0;
                }
            } else {
                XO(o)->cd = 0;
                XO(o)->ce = 0;
            }
        }
    done:
        if ((D_8009C984 & 0x40) && (D_8009D670[0] & D_1f8003c4)) {
            dust(o);
        }
        if (o->subtype != 0) {
            dust(o);
        }
    } else {
        FUN_800efa80(o);
        if (o->animFrame & 8) {
            if ((o->animFrame & 2) && (U8(o, 0xa0) & 2)) {
                D_8009C330->animFrame = 0xffff;
                o->anim = D_80010DE0;
                U8(o, 0xa2) = 1;
            } else if ((o->animFrame & 4) && (U8(o, 0xa0) & 0x10)) {
                D_8009C330->animFrame = 0xffff;
                o->anim = D_80010E2C;
                U8(o, 0xa3) = 1;
            }
        }
        XO(o)->cd = 0;
        XO(o)->ce = 0;
        o->step = 0;
        o->state = 0;
    }
    FUN_800eef0c();
}
