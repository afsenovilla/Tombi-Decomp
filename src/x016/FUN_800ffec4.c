// FUNC 800ffec4 2244 X016
// MATCHING 800ffec4 2244
#include "TOBJ.H"
typedef struct {
    unsigned char b0;
    unsigned char p1[7];
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    short w20;
    unsigned char p22[0x28 - 0x22];
    unsigned short w28, w2a, w2c, w2e;
} P800FFEC4;
typedef struct {
    unsigned char p0[0xc6];
    unsigned char c6, c7;
    unsigned char p8[0xcd - 0xc8];
    unsigned char cd, ce;
    unsigned char pf[0xd3 - 0xcf];
    unsigned char d3;
    unsigned char pd4[0xe0 - 0xd4];
    short e0;
    unsigned char e2, e3;
} X800FFEC4;
#define X(o) ((X800FFEC4 *)(o))

extern P800FFEC4 *DAT_8009c330;
extern unsigned char *DAT_800a611c;
extern unsigned char *DAT_8009d2e8;
extern int DAT_8009c934;
extern int DAT_8009c960[];
extern unsigned char DAT_8009c930[], DAT_8009c93e, DAT_8009c93f, DAT_8009c940[], DAT_8009c941, DAT_8009c942;
extern unsigned char DAT_8009d2b0, DAT_8009d2b1, DAT_8009d2c3;
extern unsigned char DAT_8009ce55, DAT_8009ce56, DAT_8009ce57, DAT_8009cee2, DAT_8009cf03, DAT_8009cdcc;
extern unsigned char DAT_801152e8[];
extern char DAT_80010748[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_800ec84c(int);
extern void FUN_80026e0c(int, int);
extern void FUN_80030b9c(int, int, int, int);
extern void FUN_8002c654(void);

#define SETANIM_IF(n) \
    DAT_8009c330->w2c = n; \
    if (DAT_8009c330->w2e != n) { \
        DAT_8009c330->w2c = n; \
        FUN_800efc04(o); \
        FUN_8001fe94(o, 0); \
        DAT_8009c330->w2e = DAT_8009c330->w2c; \
    }

static __inline__ void sub(TObj *o)
{
    X(o)->c7 = 1;
    o->b9d = 0;
    X(o)->c6 = 0;
    X(o)->e3 = 0;
    DAT_8009c330->b0 = 0;
}

static __inline__ void reset(TObj *o)
{
    if (*(unsigned char *)&o->wac >= 2) {
        DAT_8009d2e8 = DAT_800a611c;
        DAT_800a611c[4] = 2;
        DAT_8009d2e8[5] = 2;
        DAT_8009d2e8[6] = 0;
    }
    *(unsigned char *)&o->wac = 0;
    DAT_8009c934 = 0;
    sub(o);
}

void FUN_800ffec4(TObj *o)
{
    short s;
    int k;

    switch (o->state) {
    case 0:
        X(o)->cd = 0;
        X(o)->ce = 0;
        o->ba4 = 0;
        o->ba5 = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        DAT_8009c330->w20 = 0;
        o->w22 = 0;
        reset(o);
        DAT_8009c330->b8 = 0;
        o->b9c = 0;
        o->visible = 1;
        sub(o);
        if ((DAT_8009c960[0] & 0x3ffff) != 0x3000a) {
            o->anim = DAT_80010748;
            FUN_8001fe94(o, 0);
            DAT_8009c330->w2e = 0xffff;
            DAT_8009c330->w28 = 0xffff;
            DAT_8009c330->w2a = 0xffff;
        }
        o->state++;
    case 1:
        if (X(o)->e0 == 0) o->active = 1;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        if ((DAT_8009c960[0] & 0x3ffff) == 0x3000a) {
            s = -0x3c0;
            if (DAT_8009d2c3 & 0x40) s = -0x8c;
            if (o->y.p.whole > s) {
                o->b04 = 1;
                o->step = 0x3e;
                o->state = 0;
            }
        }
        if (DAT_8009c960[0] == 0x2000e) {
            o->b04 = 1;
            o->step = 0x3e;
            o->state = 0;
        }
        k = X(o)->d3;
        o->d8c = DAT_801152e8[o->wb0];
        if (k < 0x65) if (k >= 0x62) {
            FUN_800ec84c(k - 0x62);
            X(o)->d3 = 0;
            if (*((unsigned char *)&o->da0 + 1) == 2) {
                *((unsigned char *)&o->da0 + 1) = 0;
            } else if (DAT_8009c960[0] != 0x2000e) {
                DAT_8009c93f = 1;
                DAT_8009c942 = 1;
                o->b04 = 1;
                o->step = 0x30;
                o->state = 3;
                o->timer = 0x50;
            }
        }
        *((unsigned char *)&o->da0 + 1) = 0;
        if (DAT_8009c940[0] == 0) return;
        X(o)->e2 = DAT_8009c941;
        switch (DAT_8009c941) {
        case 4:
            DAT_8009c940[0] = 0;
            DAT_8009c930[0] = 3;
            SETANIM_IF(0);
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            break;
        case 0x7d:
            FUN_80026e0c(DAT_8009c941, 1);
            break;
        case 0x75:
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            FUN_80026e0c(DAT_8009c941, 1);
            break;
        case 3:
            if (*(unsigned short *)DAT_8009c960 != 0) break;
            SETANIM_IF(0);
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            break;
        case 9:
            DAT_8009c940[0] = 0;
            DAT_8009d2b1 = 0;
            FUN_80026e0c(DAT_8009c941, 1);
            break;
        case 10:
            DAT_8009c940[0] = 0;
            if (DAT_8009c960[0] == 0x30001 && DAT_8009ce55 < 3) {
                FUN_80030b9c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            }
            break;
        case 0x12:
            DAT_8009c940[0] = 0;
            if (DAT_8009c960[0] != 1) break;
            if (DAT_8009ce55 >= 2) break;
            if ((unsigned short)(o->h->p.whole - 0x74b) >= 0xf0) break;
            FUN_80030b9c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            break;
        case 5: case 0x7c: case 0x97: case 0x98:
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009d2b0 = 3;
            o->b04 = 5;
            o->step = 5;
            o->state = 0;
            o->substep = 1;
            o->d64 = 1;
            break;
        case 7:
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009d2b0 = 3;
            o->b04 = 5;
            o->step = 5;
            o->state = 0;
            o->substep = 0;
            FUN_80026e0c(DAT_8009c941, 1);
            o->d64 = 1;
            break;
        case 0x10:
            DAT_8009c940[0] = 0;
            if (DAT_8009ce56 != 0xff) {
                DAT_8009c942 = 1;
                DAT_8009c93f = 1;
                DAT_8009cee2 = 1;
            }
            break;
        case 0x82:
            *((unsigned char *)&o->waa + 1) |= 0x80;
            switch (DAT_8009d2b1) {
            case 1: o->step = 0x2a; break;
            case 2: o->step = 0x2b; break;
            }
            o->state = 0;
            break;
        case 0x3c:
            SETANIM_IF(0);
            break;
        case 0x11:
            DAT_8009ce57 = 4;
            FUN_80026e0c(DAT_8009c941, 1);
        case 0x3f: case 0x9a:
        c3f:
            DAT_8009c940[0] = 0;
            break;
        case 0x14:
            FUN_8002c654();
            goto c3f;
        case 0x8d:
            DAT_8009c940[0] = 0;
            FUN_80026e0c(DAT_8009c941, 1);
            DAT_8009cf03 = 3;
            break;
        case 0xd: case 0x8a:
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            break;
        case 0x5c:
            SETANIM_IF(0);
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            break;
        case 0xc:
            DAT_8009c940[0] = 0;
            if (DAT_8009c960[0] == 9 && DAT_8009cdcc != 0xff) {
                FUN_80030b9c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            }
            break;
        case 0xe:
            o->ba4 = 0;
            o->ba5 = 0;
            o->b9c = 1;
            o->visible = 1;
            o->ba7 = 0;
            o->active = 4;
            o->d64 = 1;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            DAT_8009c330->w20 = 0;
            o->d8c = 0;
            o->timer = 0x1e;
            o->w22 = 0;
            reset(o);
            DAT_8009d2b0 = 3;
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            o->step = 0x30;
            o->d8c = 0;
            o->b04 = 1;
            o->state = 2;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        if (--o->timer > 0) break;
        DAT_8009c93f = 0;
        DAT_8009c942 = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    }
}
