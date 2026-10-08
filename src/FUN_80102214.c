// FUNC 80102214 2036 X000
// MATCHING 80102214 2036
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char pad[0x2c]; unsigned short w2c; } G330;
typedef struct { void *p[90]; } AnimTbl;
extern G330 *D_8009C330;
extern AnimTbl D_800E8404;
extern unsigned short D_8009D670, D_8009D674;
extern unsigned short D_1F8001FC, D_1F8001FE, D_1F8003C4;
extern unsigned char D_1F8001D2;
extern short D_1F80016A[], D_1F80016E[], D_1F800172[];
extern unsigned char D_8009C93E, D_8009C93F, D_8009C938, D_8009F7F0;
extern unsigned short D_8009D610;
extern int D_8009C984;
extern unsigned short D_8007A034, D_8007A036;
extern unsigned short D_800A6040, D_800A60FC;
extern signed char D_8009D2B0;
extern unsigned char D_8009C964;
extern void *memcpy(void *, void *, int);
extern void FUN_80025d90(void);
extern void FUN_800f0490(TObj *);
extern void FUN_80100788(TObj *);
extern void FUN_80101d74(TObj *);
extern void FUN_800fd358(TObj *);
extern void FUN_800fdb7c(TObj *);
extern void FUN_800fe13c(TObj *);
extern void FUN_800fe75c(TObj *);
extern void FUN_800fe98c(TObj *);
extern void FUN_800fa01c(TObj *);
extern void FUN_80109920(TObj *);
extern void FUN_8010b444(TObj *);
extern void FUN_8010b050(TObj *);
extern void FUN_800f8130(TObj *);
extern void FUN_8003f7cc(TObj *);
extern void FUN_8001fe94(TObj *, int);

#define RESET(o) \
    { \
        o->active = 3; \
        U8(o, 0xa1) = 0; \
        o->b04 = 2; \
        o->step = 2; \
        o->state = 2; \
        o->substep = 0; \
        o->timer = 0x78; \
        D_8009C330->w2c = 0x2f; \
        *(AnimTbl *)tp = D_800E8404; \
        o->anim = tp[D_8009C330->w2c]; \
        FUN_8001fe94(o, 1); \
    }

void FUN_80102214(TObj *o)
{
    unsigned char pad[8];
    unsigned short b0, b1;
    short f;
    void *t[90];
    void **tp;

    memcpy(pad, &D_8009D670, 8);
    b0 = D_1F8001FC;
    b1 = D_1F8001FE;
    U8(o, 0xe2) = 0;
    if (D_8009C93E) {
        o->visible = 1;
        return;
    }
    if (D_8009F7F0 == 0 && D_8009D610 == 7) {
        unsigned char a, b, c;
        FUN_80025d90();
        a = U8(o, 0xd6);
        b = U8(o, 0xd5);
        c = o->bbf;
        o->bbf = 0;
        U8(o, 0xd7) = a;
        U8(o, 0xd6) = b;
        U8(o, 0xd5) = c;
    }
    if (D_8009C93F) {
        *(volatile unsigned short *)&D_8009D670 = 0;
        D_1F8001FC = 0;
        D_8009D674 = 0;
        D_1F8001FE = 0;
        S16(o, 0xdc) = 0;
        S16(o, 0xde) = 0;
    }
    D_1F8001D2 = 0;
    U8(o, 0xc7) = 0;
    switch (o->subtype) {
    case 0:
        if ((D_8009C984 & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4) && o->step == 1)
            U8(o, 0xc1) = 1;
        else
            U8(o, 0xc1) = o->ba7;
        break;
    case 1:
        U8(o, 0xc1) = 1;
        o->ba7 = 1;
        D_800A6040 = D_8007A034;
        D_800A60FC = D_8007A036;
        break;
    case 2:
        U8(o, 0xc1) = 1;
        o->ba7 = 1;
        D_800A6040 = D_8007A034;
        D_800A60FC = D_8007A036;
        break;
    }
    {
        signed char *k = &D_8009D2B0;
        if (*k >= 5) goto z;
        if (*k < 3) {
        z:
            *k = 0;
        }
    }
    switch (o->b04) {
    case 0:
        FUN_800f0490(o);
        break;
    case 1:
        tp = t;
        if (D_8009C938) {
            RESET(o);
            break;
        }
        if (o->step != 0x30 && S16(o, 0xe0) > 0) {
            if (U8(o, 0xa3) & 2) {
                o->visible ^= 1;
            } else if (U8(o, 0xa2) & 2) {
                o->visible ^= 1;
            } else {
                S16(o, 0xe0) = S16(o, 0xe0) - 1;
                o->visible = S16(o, 0xe0) & 1;
                if (S16(o, 0xe0) <= 0) {
                    if (U8(o, 0xcc) != 2) o->active = 1;
                    S16(o, 0xe0) = 0;
                    o->visible = 1;
                }
            }
        }
        if (o->w22 > 0 && --o->w22 <= 0) {
            o->active = 1;
            o->visible = 1;
        }
        FUN_80100788(o);
        break;
    case 2:
        if (S16(o, 0xe0) > 0) {
            S16(o, 0xe0) = S16(o, 0xe0) - 1;
            if (U8(o, 0xd1) == 0) o->visible = S16(o, 0xe0) & 1;
            if (S16(o, 0xe0) <= 0) {
                o->active = 1;
                S16(o, 0xe0) = 0;
                o->visible = 1;
            }
        }
        switch (o->step) {
        case 0: FUN_800fd358(o); break;
        case 1: FUN_800fdb7c(o); break;
        case 2: FUN_800fe13c(o); break;
        case 3: FUN_800fe75c(o); break;
        case 4: FUN_800fe98c(o); break;
        }
        U8(o, 0xa8) = 0;
        o->ba6 = 0;
        f = 0;
        if (o->b9e == 0 && o->b04 == 5) {
            f = o->step == 0x40;
            if (o->step == 0x65) f++;
        }
        if (f == 0) FUN_8003f7cc(o);
        break;
    case 3:
        o->b04 = 1;
        break;
    case 5:
        tp = t;
        if (D_8009C938) {
            RESET(o);
            break;
        }
        if (o->w22 > 0 && --o->w22 <= 0) {
            o->active = 1;
            o->w22 = 0;
            o->visible = 1;
        }
        FUN_80101d74(o);
        break;
    case 6:
        tp = t;
        if (D_8009C938) {
            RESET(o);
            break;
        }
        if (o->w22 > 0 && --o->w22 <= 0) {
            o->active = 1;
            o->w22 = 0;
            o->visible = 1;
        }
        switch (o->step) {
        case 0:
            FUN_800fa01c(o);
            break;
        case 3:
            o->animFrame &= 1;
            FUN_80109920(o);
            break;
        case 4:
            o->animFrame &= 1;
            FUN_8010b444(o);
            break;
        case 5:
            o->animFrame &= 1;
            FUN_8010b050(o);
            o->d8c = 0;
            break;
        case 6:
            FUN_800f8130(o);
            break;
        case 7:
            o->d8c = 0;
            FUN_8010b050(o);
            if (o->b69) {
                o->b04 = 1;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        U8(o, 0xa8) = 0;
        o->ba6 = 0;
        if (o->b9e == 0) FUN_8003f7cc(o);
        break;
    }
    D_1F80016A[0] = o->h->p.whole;
    D_1F80016E[0] = o->y.p.whole;
    D_1F800172[0] = o->d->p.whole;
    D_8009C964 = o->d->p.whole / 90;
    memcpy(&D_8009D670, pad, 8);
    D_1F8001FC = b0;
    D_1F8001FE = b1;
}
