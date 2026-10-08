// FUNC 800fc414 3908 X018
// MATCHING 800fc414 3908
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[7];
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    short t20;
    unsigned char p22[0x2c - 0x22];
    unsigned short w2c;
    unsigned short w2e;
} P800FC414;

extern P800FC414 *D_8009C330;
extern unsigned char D_8009C938;
extern int D_8009C960;
extern unsigned char D_8009C960b[];
extern unsigned char D_8009CF06;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CE3D;
extern unsigned char D_801152E8[];
extern char D_80077CF4[];
extern char D_80010748[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001fd94(TObj *);
extern void ObjGravityStep(TObj *);
extern short FUN_800fc240(TObj *);
extern short FUN_80040278(TObj *, int, int);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_800ff610(TObj *, int);

#define SETANIM_IF(n) \
    D_8009C330->w2c = n; \
    if (D_8009C330->w2e != n) { \
        D_8009C330->w2c = n; \
        FUN_800efc04(o); \
        FUN_8001fe94(o, 0); \
        D_8009C330->w2e = D_8009C330->w2c; \
    }

#define ADD7C() \
    if (U8(o, 0xad) == 0) \
        o->y.raw += 0x7c000;

#define LAND() \
    if (U8(o, 0xd1) == 1) { \
        SETANIM_IF(0x55); \
    } else { \
        SETANIM_IF(0x3b); \
    }

#define GROUND() \
    ObjGravityStep(o); \
    FUN_8001fd94(o); \
    if (FUN_800fc240(o)) \
        return; \
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) { \
        o->b69 = 1; \
        o->velY = 0; \
        o->b9c = 0; \
    }

#define TURN() { \
    unsigned char x; unsigned int c; short d; unsigned short u; \
    x = D_801152E8[o->wb0]; \
    c = (unsigned char)(x - o->d8c); \
    d = c; \
    if (d == 0) return; \
    u = c; \
    if (u < 0x80) { \
        if (d >= 4) o->d8c = o->d8c + 4; \
        else if (d >= 2) o->d8c = o->d8c + 2; \
        else o->d8c = o->d8c + 1; \
    } else { \
        if (d < 0xfd) o->d8c = o->d8c - 4; \
        else if (d < 0xff) o->d8c = o->d8c - 2; \
        else o->d8c = o->d8c - 1; \
    } \
    o->d8c = *(unsigned char *)&o->d8c; }

static __inline__ int fall(TObj *o)
{
    U8(o, 0xad) = 0;
    FUN_8003fd78(o, 0, 0);
    if ((o->b69 | o->b9c | o->b9e | o->b9f | o->bbe) == 0) {
        U8(o, 0xad) = 1;
        o->velY += 0x223;
        o->y.raw += o->velY << 8;
        D_8009C330->t20 = 0x21;
        if (o->velY >= 0x447) {
            U8(o, 0xcd) = 0;
            U8(o, 0xce) = 0;
            U8(o, 0xad) = 0;
            o->velY = 0;
            o->b9c = 2;
            U8(o, 0xc3) = 0;
            o->d8c = 0;
            return 1;
        }
    } else {
        o->velY = 0;
        o->b9c = 0;
    }
    return 0;
}

void FUN_800fc414(TObj *o)
{
    int t, v;
    short r;

    switch (o->substep) {
    case 0:
        o->movetab = D_80077CF4;
        o->timer = 0x19;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        if (U8(o, 0xd1) == 1) {
            SETANIM_IF(0x57);
        } else {
            SETANIM_IF(0x10);
        }
        o->substep++;
    case 1:
        if (o->timer != 0)
            o->timer--;
        FUN_8001fec0(o);
        FUN_8001fa88(o, (o->animFrame & 1) ^ 1);
        {
            int d0 = o->d8c;
            if (o->animFrame & 1)
                t = (d0 - 0x10) & 0xff;
            else
                t = (d0 + 0x10) & 0xff;
        }
        o->d8c = t;
        if (o->ba6 != 0) {
            o->d8c = 0;
            LAND();
            o->timer = 10;
            o->substep++;
            break;
        }
        if (o->b69 == 0) {
            ObjGravityStep(o);
            FUN_8001fd94(o);
            if (o->timer == 0) {
                o->d8c = 0;
                LAND();
                o->timer = 10;
                o->substep++;
            }
            if (FUN_800fc240(o))
                return;
            if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) {
                o->b69 = 1;
                o->velY = 0;
                o->b9c = 0;
            }
        } else {
            ADD7C();
            if (o->timer == 0) {
                o->d8c = 0;
                LAND();
                o->timer = 10;
                o->substep++;
            } else {
                FUN_800ff610(o, 0);
            }
            fall(o);
        }
        break;
    case 2:
        FUN_8001fec0(o);
        if (o->timer != 0 && --o->timer <= 0)
            o->substep++;
        if (o->b69 == 0) {
            GROUND();
        } else {
            ADD7C();
            fall(o);
        }
        TURN();
        break;
    case 3:
        if (o->b69 == 0) {
            GROUND();
        } else {
            ADD7C();
            fall(o);
        }
        if (D_8009C938 == 0) {
            *(unsigned char *)&o->wac = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            SETANIM_IF(0x3d);
            o->substep++;
        } else {
            if (U8(o, 0xd1) == 1) {
                SETANIM_IF(0x56);
            } else {
                SETANIM_IF(0x3c);
            }
            o->timer = 0xb4;
            o->substep = 5;
        }
        TURN();
        break;
    case 4:
        if (o->b69 == 0) {
            GROUND();
        } else {
            ADD7C();
            ObjGravityStep(o);
            if (FUN_8003fd78(o, 0, 0)) {
                o->b69 = 1;
                o->velY = 0;
            }
            if (FUN_800fc240(o))
                return;
            if (FUN_8001fec0(o)) {
                D_8009C330->b8 = 0;
                o->active = 3;
                S16(o, 0xe0) = 0x8c;
                U8(o, 0xd1) = 0;
                S16(o, 0xb2) = 0;
                o->b04 = 1;
                if (D_8009C960 == 0x2000e) {
                    o->step = 0x3e;
                    o->state = 0;
                    o->substep = 0;
                } else if ((D_8009C960 & 0x3ffff) == 0x3000a) {
                    r = 0;
                    if (D_8009D2C3 & 0x40) {
                        if (o->y.p.whole > -0x8c) {
                            r = 1;
                            o->w56 = -0x8c;
                            if (o->y.p.whole > -0x7c) {
                                if (D_8009CE3D == 0xff)
                                    r = 2;
                                else
                                    o->y.p.whole = -0x8c;
                            }
                        }
                    } else {
                        if (o->y.p.whole > -0x3c0) {
                            r = 1;
                            o->w56 = -0x3c0;
                            if (o->y.p.whole > -0x3b0) {
                                if (D_8009CE3D == 0xff)
                                    r = 2;
                                else
                                    o->y.p.whole = -0x3c0;
                            }
                        }
                    }
                    switch (r) {
                    case 1:
                        o->step = 0x3d;
                        o->state = 0;
                        o->substep = 0;
                        break;
                    case 0:
                        goto reset;
                    case 2:
                        o->step = 0x3e;
                        o->state = 0;
                        o->substep = 0;
                        break;
                    }
                } else {
                reset:
                    o->anim = D_80010748;
                    FUN_8001fe6c(o);
                    o->step = 0;
                    o->state = 0;
                    o->substep = 0;
                }
            }
            fall(o);
        }
        TURN();
        break;
    case 5:
        o->visible = 1;
        FUN_8001fec0(o);
        if (o->b69 == 0) {
            GROUND();
        } else {
            ObjGravityStep(o);
            if (FUN_8003fd78(o, 0, 0)) {
                o->b69 = 1;
                o->velY = 0;
            }
            if (FUN_800fc240(o))
                return;
            ADD7C();
            fall(o);
        }
        if (--o->timer == 0) {
            D_8009C960b[0x951] = 0;
            D_8009CF06 = 0;
            if (*(unsigned short *)D_8009C960b != 10 || *(unsigned short *)(D_8009C960b + 2) != 0)
                D_8009C938 = 2;
            o->substep++;
        }
        TURN();
        break;
    case 6:
        if (o->b69 == 0) {
            GROUND();
        } else {
            ADD7C();
            fall(o);
        }
        break;
    }
}
