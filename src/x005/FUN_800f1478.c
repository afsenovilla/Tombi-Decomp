// FUNC 800f1478 2252 X005
// MATCHING 800f1478 2252
#include "TOBJ.H"
#include "raw7.h"
extern unsigned char *DAT_800a611c;
extern unsigned char *DAT_8009d2e8;
extern unsigned char *DAT_8009c330;
extern unsigned short DAT_8009d670;
extern unsigned char DAT_8009d00f;
extern unsigned char DAT_8009c93f;
extern unsigned char DAT_1f8003ce;
extern unsigned char DAT_8009d2b0[];
extern unsigned char DAT_8009c942;
extern unsigned char DAT_800a60f8;
extern unsigned char DAT_8009d004;
extern int DAT_8009c984;
extern short DAT_8009c944;
extern short DAT_8009c946[];
extern unsigned short DAT_1f8001fc, DAT_1f8003c4, DAT_1f8003c6, DAT_1f8003c8, DAT_1f8001c8;
extern char D_8001080c[];
extern char D_800113cc[];
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_800efa80(TObj *);
extern void FUN_800f1308(TObj *);
extern short func_800F0E7C(TObj *);
extern void FUN_8010f400(TObj *);
extern void FUN_8001fc14(TObj *, int, int);
extern void FUN_8001fce4(TObj *);
extern int func_8010E114(void);
extern void func_8010E328(TObj *, short);

typedef struct { unsigned char pad[0x28]; unsigned short w28, w2a, w2c, w2e; } Q;
#define PL ((Q *)DAT_8009c330)

#define YADJ(o)                                              \
    if (U8(o, 0xad) == 0) {                                  \
        if (o->subtype) {                                    \
            if ((unsigned short)(o->wb0 + 5) >= 0xb)         \
                o->y.raw += 0x50000;                         \
            else                                             \
                o->y.raw += 0x30000;                         \
        } else {                                             \
            if ((unsigned short)(o->wb0 + 5) >= 0xb)         \
                o->y.raw += 0xc0000;                         \
            else                                             \
                o->y.raw += 0x80000;                         \
        }                                                    \
    }

void FUN_800f1478(TObj *o)
{
    unsigned char c;
    unsigned char *p;
    int t;

    U8(o, 0xa2) = 0;
    U8(o, 0xa3) = 0;
    if (U8(o, 0xac) >= 2) {
        DAT_8009d2e8 = DAT_800a611c;
        DAT_800a611c[4] = 2;
        DAT_8009d2e8[5] = 2;
        DAT_8009d2e8[6] = 0;
    }
    U8(o, 0xac) = 0;
    c = 0;
    if (*(volatile unsigned short *)&DAT_8009d670 & 0xa0) {
        if (o->wb2 == 0) {
            if ((unsigned short)(*(unsigned short *)o->anim - 0x10d) >= 7)
                o->anim = D_8001080c;
            else
                o->anim = D_800113cc;
            FUN_8001fe6c(o);
            PL->w2c = 999;
            PL->w2e = 0xffff;
            PL->w28 = 0xffff;
            PL->w2a = 0xffff;
            if (o->animFrame & 1)
                o->h->raw += -0x10000;
            else
                o->h->raw += 0x10000;
        } else {
            FUN_800efa80(o);
        }
        YADJ(o);
        DAT_8009c330[0x1e] = 0;
        DAT_8009c330[0x1f] = 0;
        o->velX = 0;
        o->step = 1;
        o->state = 0;
        o->substep = 0;
    } else {
        switch (o->state) {
        case 0:
            U8(o, 0xc3) = 0;
            o->velX = 0;
            PL->w2c = 0;
            PL->w2e = 0xffff;
            o->substep = 0;
            o->state++;
        case 1:
            if (DAT_8009d00f) {
                FUN_800f1308(o);
            } else {
                c = func_800F0E7C(o);
                if (!c)
                    FUN_800efa80(o);
                if (*(unsigned short *)o->anim || (!DAT_8009c93f && !DAT_1f8003ce))
                    FUN_8001fec0(o);
            }
            {
                short u = o->wb0;
                if (u < 0)
                    o->wb6 = (short)((u << 2) + 0x100) & 0xff;
                else if (u > 0)
                    o->wb6 = (short)(u << 2) & 0xff;
                else
                    o->wb6 = 0;
            }
            FUN_8010f400(o);
            if (o->velY > 0x400) {
                U8(o, 0xac) = 1;
                DAT_8009c330[0x1e] = 0;
                DAT_8009c330[0x1f] = 0;
                t = 0x10;
                o->timer = 10;
                o->d84 = 0;
                if (o->animFrame & 1)
                    t = 0xf0;
                o->d88 = t;
                o->d8c = 0;
                DAT_8009d2b0[0] = 0;
                o->step = 2;
                o->state = 3;
                o->substep = 0;
            }
            YADJ(o);
            if (!(DAT_1f8001fc & (DAT_1f8003c6 | DAT_1f8003c8))) {
                FUN_8001fc14(o, o->wb6, o->wb2);
                o->h->raw += DAT_8009c944 << 8;
                o->y.raw += DAT_8009c946[0] << 8;
                FUN_8001fce4(o);
            }
            break;
        }
    }

    switch (c) {
    case 4:
        if (DAT_1f8001fc & DAT_1f8003c4) {
            o->b04 = 5;
            o->step = 0x61;
            o->state = 0;
            o->substep = 0;
            o->wb2 = 0;
            DAT_8009c93f = 1;
            DAT_8009d2b0[0] = 2;
            DAT_8009c942 = 1;
            o->visible = 1;
            DAT_8009c330[0x1e] = 0;
            DAT_8009c330[0x1f] = 0;
            DAT_800a60f8 = 1;
        }
        break;
    case 9:
        if (DAT_1f8001fc & DAT_1f8003c8) {
            o->b04 = 1;
            o->step = 0x3b;
            o->state = 0;
            o->substep = 0;
        }
        break;
    case 10:
        if (DAT_1f8001fc & DAT_1f8003c8) {
            o->b04 = 1;
            o->step = 0x3c;
            o->state = 0;
            o->substep = 0;
        }
        break;
    case 12:
        if (DAT_1f8001fc & DAT_1f8003c8) {
            o->b04 = 5;
            o->step = 0xb;
            o->state = 6;
            o->substep = 0;
        }
        break;
    default:
        if (func_8010E114() == 0)
            func_8010E328(o, 0);
        break;
    }

    if (DAT_1f8001fc & DAT_1f8003c6) {
        o->ba4 = 0;
        DAT_8009c330[0x1e] = 0;
        DAT_8009c330[0x1f] = 0;
        DAT_8009d2b0[0] = 0;
        o->substep = 0;
        switch (c) {
        case 0:
            o->b9c = 1;
            if ((DAT_8009c984 & 0x40) && (*(volatile unsigned short *)&DAT_8009d670 & DAT_1f8003c4))
                o->ba7 = 1;
            o->b04 = 1;
            o->step = 2;
            o->state = 0;
            break;
        case 1:
            o->b04 = 1;
            o->step = 0x11;
            o->state = 0;
            break;
        case 2: {
            unsigned short hx = o->h->p.whole;
            unsigned short k = DAT_1f8001c8;
            U16(o, 0xf2) = o->y.p.whole;
            U16(o, 0xee) = hx;
            if (k & 1)
                U16(o, 0xf6) = (o->d->p.whole + 1) / 90 * 90;
            else
                U16(o, 0xf6) = (o->d->p.whole - 1) / 90 * 90;
            o->b04 = 1;
            o->step = 0x12;
            o->state = 0;
            break; }
        case 5:
            o->b04 = 1;
            o->step = 0x28;
            o->state = 0;
            break;
        case 6:
            o->b04 = 1;
            o->step = 0x29;
            o->state = 0;
            break;
        case 8:
            o->b04 = 1;
            o->step = 0x3a;
            o->state = 0;
            break;
        case 13:
            o->b04 = 5;
            o->step = 0x65;
            o->state = 0;
            DAT_8009d004 = 1;
            break;
        }
    } else {
        p = DAT_8009c330;
        if (p[0x1e] >= 0x1f) {
            switch (c) {
            case 3:
                p[0x1e] = 0;
                DAT_8009c330[0x1f] = 0;
                o->b04 = 1;
                o->step = 0x20;
                o->state = 0;
                break;
            case 11:
                p[0x1e] = 0;
                DAT_8009c330[0x1f] = 0;
                o->b04 = 5;
                o->step = 0xb;
                o->state = 0;
                break;
            }
        } else if (p[0x1f] >= 0x1f) {
            o->substep = 0;
            if (c == 7) {
                DAT_8009c330[0x1e] = 0;
                DAT_8009c330[0x1f] = 0;
                o->b04 = 1;
                o->step = 0x31;
                o->state = 0;
            }
        }
    }
}
