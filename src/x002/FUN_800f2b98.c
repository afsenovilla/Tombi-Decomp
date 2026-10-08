// FUNC 800f2b98 2632 X002
// MATCHING 800f2b98 2632
#include "TOBJ.H"
typedef struct {
    unsigned char b0;
    unsigned char p1[3];
    unsigned char b4, b5;
    unsigned char p6[2];
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    unsigned short w20;
    unsigned char p22[0x28 - 0x22];
    unsigned short w28, w2a, w2c, w2e;
} P800F2B98;
typedef struct {
    unsigned char p0[0xc3];
    unsigned char c3;
    unsigned char p4[0xc9 - 0xc4];
    unsigned char c9;
    unsigned char pa[0xcc - 0xca];
    unsigned char cc;
    unsigned char pd[0xe4 - 0xcd];
    unsigned char *e4;
} X800F2B98;
#define X(o) ((X800F2B98 *)(o))
typedef struct { void *p[90]; } T800F2B98;

extern P800F2B98 *DAT_8009c330;
extern unsigned char *DAT_800a611c;
extern unsigned char *DAT_8009d2e8;
extern volatile unsigned short DAT_8009d670[];
extern unsigned short DAT_1f8003c6, DAT_1f8001fc, DAT_1f8001f8;
extern short DAT_1f800238;
extern short DAT_8009c944, DAT_8009c946[];
extern unsigned char DAT_8009d2b0, DAT_8009d2b1[], DAT_8009c990;
extern int DAT_8009f0ec;
extern unsigned char DAT_801152e8[];
extern short DAT_801151e0[];
extern T800F2B98 DAT_800e8404;
extern short FUN_8001fddc(int, int);
extern void FUN_8010f328(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e560(int, int);
extern void FUN_8001fec0(TObj *);
extern short FUN_800eef0c(void);
extern TObj *FUN_80018448(void);
extern void FUN_8010f400(TObj *);
extern void FUN_8010eaf8(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_800eec40(TObj *);
extern int FUN_8003facc(TObj *);
extern void FUN_80040278(TObj *, short, short);
extern void FUN_800f28a0(TObj *);
extern void FUN_800f2724(TObj *);
extern void FUN_800f2a30(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern int FUN_8004bbc0(TObj *, int);
extern void FUN_800efc8c(TObj *, int);

void FUN_800f2b98(TObj *o)
{
    T800F2B98 tab;
    short s;
    int d;
    unsigned char v;
    unsigned short m;
    unsigned char bb;

    switch (o->state) {
    case 0:
        d = o->wb2;
        o->timer = 10;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        *(unsigned char *)&o->waa = 0;
        X(o)->c3 = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        *(unsigned char *)&o->da0 = 0;
        *((unsigned char *)&o->da0 + 1) = 0;
        o->wb6 = 0;
        o->velX = FUN_8001fddc(0, d);
        o->velY = 0;
        DAT_8009c330->b8 = 0;
        DAT_8009c330->w2c = 4;
        DAT_8009c330->w20 = 0;
        DAT_8009c330->b5 = 0;
        if (*(unsigned char *)&o->wac >= 2) {
            DAT_8009d2e8 = DAT_800a611c;
            DAT_800a611c[4] = 2;
            DAT_8009d2e8[5] = 2;
            DAT_8009d2e8[6] = 0;
        }
        *(unsigned char *)&o->wac = 0;
        FUN_8010f328(o);
        tab = DAT_800e8404;
        o->anim = tab.p[DAT_8009c330->w2c];
        FUN_8001fe6c(o);
        DAT_8009c330->w2e = DAT_8009c330->w2c;
        FUN_8001e560(2, 4);
        o->state = 1;
    case 1:
        if (o->velY >= 0) {
            DAT_8009c330->b8 = 1;
            o->state = 2;
        }
        if (DAT_8009d670[0] & DAT_1f8003c6) {
            if (DAT_8009c330->b8 != 0) goto common;
            if (++DAT_8009c330->w20 >= 0xe) {
                DAT_8009c330->b8 = 1;
                o->state = 2;
            }
        } else {
            DAT_8009c330->b8 = 1;
            if (DAT_8009c330->w20 >= 5) {
                o->state = 2;
                goto common;
            }
            DAT_8009c330->w20++;
        }
        FUN_8010f328(o);
    case 2:
    common:
        o->h->raw += DAT_8009c944 << 8;
        o->y.raw += DAT_8009c946[0] << 8;
        FUN_8001fec0(o);
        if (o->ba7 != 0) {
            short a = o->a.p.whole, y = o->y.p.whole, b = o->b.p.whole;
            TObj *q;
            if (FUN_800eef0c() == 0 && DAT_1f800238 >= 6 && (q = FUN_80018448()) != 0) {
                short *t;
                q->active = 1;
                q->type = 0x31;
                q->subtype = 1;
                q->a.p.whole = a;
                q->y.p.whole = y;
                q->b.p.whole = b;
                t = &DAT_801151e0[(DAT_1f8001f8 & 7) * 2];
                q->h->p.whole += t[0];
                q->y.p.whole += t[1];
            }
        }
        FUN_8010f400(o);
        FUN_8010eaf8(o);
        o->h->raw += o->velX << 8;
        FUN_8010f0f4(o);
        FUN_8001fd94(o);
        if (o->velY >= -899) FUN_800eec40(o);
        if (o->velY > 0) {
            { int e;
            DAT_8009c330->b8 = 1;
            e = 0x10;
            o->velY = 0;
            o->velV = 0;
            o->d84 = 0;
            if (o->animFrame & 1) e = 0xf0;
            o->timer = 10;
            o->b9c = 2;
            o->d88 = e;
            *(unsigned char *)&o->wac = 1;
            o->state = 3; }
        }
        if (FUN_8003facc(o)) {
            { int e;
            DAT_8009c330->b8 = 1;
            e = 0x10;
            o->velY = 0;
            o->velV = 0;
            o->d84 = 0;
            if (o->animFrame & 1) e = 0xf0;
            o->timer = 10;
            o->b9c = 2;
            o->d88 = e;
            *(unsigned char *)&o->wac = 1;
            o->state = 3; }
        }
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10);
        o->b69 = 0;
        break;
    case 3:
        o->h->raw += DAT_8009c944 << 8;
        o->y.raw += DAT_8009c946[0] << 8;
        FUN_8001fec0(o);
        if (X(o)->c9 != 0) FUN_800f28a0(o);
        else FUN_800f2724(o);
        FUN_8010f400(o);
        DAT_8009c330->b8 = 1;
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        FUN_8003facc(o);
        if (*(unsigned char *)&o->wac == 2) {
            DAT_8009d2e8 = X(o)->e4;
            DAT_8009c330->b8 = 0;
            o->ba7 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            *((unsigned char *)&o->waa + 1) &= 0x7f;
        } else if (o->b69 == 1) {
            FUN_800f2a30(o);
            if (*((unsigned char *)&o->waa + 1) & 0x80) {
                o->ba4 = 0;
                switch (DAT_8009d2b1[0]) {
                case 1: o->step = 0x2a; o->state = 0; break;
                case 2: o->step = 0x2b; o->state = 0; break;
                default: o->state = 0; break;
                }
            } else if (o->bbe & 8) {
                DAT_8009c330->b8 = *(unsigned char *)&o->animFrame & 1;
                o->step = 0x1b;
                o->state = 0;
            } else {
                o->ba4 = 0;
                DAT_8009c330->w2e = 0xffff;
                DAT_8009c330->w28 = 0xffff;
                DAT_8009c330->w2a = 0xffff;
                o->d8c = DAT_801152e8[o->wb0];
                o->step = 1;
                o->state = 0;
                if (DAT_1f8001fc & DAT_1f8003c6) {
                    DAT_8009d2b0 = 0;
                    o->step = 2;
                    o->state = 0;
                }
            }
        } else if (FUN_8003fd78(o, 4, 0) != 0) {
            FUN_800f2a30(o);
            if (*((unsigned char *)&o->waa + 1) & 0x80) {
                o->ba4 = 0;
                o->ba5 = 0;
                DAT_8009c330->w20 = 0;
                switch (DAT_8009d2b1[0]) {
                case 1: o->step = 0x2a; o->state = 0; break;
                case 2: o->step = 0x2b; o->state = 0; break;
                default: o->state = 0; break;
                }
            } else if (o->bbe != 0) {
                o->step = 1;
                o->velY = 0;
                o->state = 0;
                if (o->step == 0x10 || o->step == 0x17 || o->step == 0x1b || o->step == 0x1e) goto l33e8;
                bb = o->bbe;
                if ((bb & 2) && o->b9c == 0) {
                    if (bb & 1) {
                        if (o->wb2 < -0x144) goto l3344;
                    } else if (o->wb2 > 0x144) {
                    l3344:
                        DAT_8009c330->b8 = *(unsigned char *)&o->animFrame & 1;
                        o->step = 0x10;
                        o->state = 0;
                    }
                } else if (bb & 4) {
                    DAT_8009c330->b8 = *(unsigned char *)&o->animFrame & 1;
                    o->step = 0x17;
                    o->state = 0;
                } else if (bb & 0x10) {
                    if (bb & 1) {
                        if (o->wb2 < -0x144) goto l33c8;
                    } else if (o->wb2 > 0x144) {
                    l33c8:
                        DAT_8009c330->b8 = *(unsigned char *)&o->animFrame & 1;
                        o->step = 0x1e;
                        o->state = 0;
                    }
                }
            l33e8:
                if (o->bbe & 0x20) {
                    DAT_8009c330->b8 = *(unsigned char *)&o->animFrame & 1;
                    o->step = 0x1f;
                    o->state = 0;
                }
            } else {
                o->ba4 = 0;
                o->ba5 = 0;
                DAT_8009c330->w20 = 0;
                DAT_8009c330->w2e = 0xffff;
                DAT_8009c330->w28 = 0xffff;
                DAT_8009c330->w2a = 0xffff;
                o->step = 1;
                o->state = 0;
                if (DAT_1f8001fc & DAT_1f8003c6) {
                    DAT_8009d2b0 = 0;
                    o->step = 2;
                    o->state = 0;
                }
            }
        } else {
            if (DAT_8009c990 == 3 && (DAT_1f8001fc & DAT_1f8003c6)) {
                DAT_8009d2b0 = 0;
                if (X(o)->cc == 1) {
                    *(unsigned char *)&o->wac = 0;
                    DAT_8009c330->b4 = 0;
                    s = o->wb2;
                    if (s < 0) {
                        short t = s;
                        if (!(o->animFrame & 1)) t = -s;
                        o->wb2 = t;
                    } else {
                        if (o->animFrame & 1) s = -s;
                        o->wb2 = s;
                    }
                    o->step = 0x45;
                    o->state = 0;
                    return;
                }
            }
        }
    tail:
        if (o->velY > 0) o->b9c = 2;
        break;
    }
    if (*(unsigned char *)&o->wac < 2 && (DAT_8009f0ec = FUN_8004bbc0(o, 0)) != 0) {
        DAT_8009c330->b8 = 0;
        DAT_8009c330->w20 = 0;
        *(unsigned char *)&o->wac = 0;
        o->ba7 = 0;
        o->b9c = 0;
        o->wb2 = 0;
        FUN_800efc8c(o, DAT_8009f0ec == 1);
    }
    FUN_800eef0c();
}
