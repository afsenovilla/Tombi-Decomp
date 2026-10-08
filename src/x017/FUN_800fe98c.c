// FUNC 800fe98c 1768 X017
// MATCHING 800fe98c 1768
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[7];
    unsigned char b8;
    unsigned char p9[0x2c - 9];
    unsigned short w2c;
    unsigned short w2e;
} P800FE98C;

extern P800FE98C *D_8009C330;
extern TObj *D_8009D2E8;
extern TObj *D_800A611C;
extern TObj *D_8009F0EC;
extern int D_8009C934;
extern unsigned char D_1F8001A4[];
extern unsigned char D_1F8001F8;
extern unsigned char D_8009C970[];
extern unsigned char D_8009C942;
extern unsigned char D_8009C938;
extern short D_8009C944;
extern short D_8009C946[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001f110(int);
extern void FUN_8001f2ec(int);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void FUN_8001e4f0(int);
extern void FUN_80025f40(int, int, int, int);
extern int AnimAdvance(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_800fc414(TObj *);

#define SETANIM_IF(n) \
    D_8009C330->w2c = n; \
    if (D_8009C330->w2e != n) { \
        D_8009C330->w2c = n; \
        FUN_800efc04(o); \
        FUN_8001fe94(o, 0); \
        D_8009C330->w2e = D_8009C330->w2c; \
    }

#define BLINK() \
    if (D_8009C330->b8 == 0) \
        o->visible = D_1F8001F8 & 1;

static __inline__ void move(TObj *o)
{
    o->h->raw += D_8009C944 << 8;
    o->y.raw += D_8009C946[0] << 8;
    BLINK();
    o->h->raw += o->velX << 8;
    o->y.raw += o->velY << 8;
}

#define LAND() { \
    int d = 0x10; int c3 = 3; int c2 = 2; int c1 = 1; \
    D_8009C330->b8 = 0; \
    o->active = c3; \
    S16(o, 0xe0) = 0x8c; \
    o->b9c = c2; \
    U8(o, 0xac) = c1; \
    U8(o, 0xd1) = 0; \
    o->timer = 10; \
    o->d84 = 0; \
    if (o->animFrame & 1) \
        d = 0xf0; \
    o->d88 = d; \
    o->d8c = 0; \
    o->b04 = c1; \
    o->step = c2; \
    o->state = c3; \
    o->substep = 0; }

void FUN_800fe98c(TObj *o)
{
    short vx;

    switch (o->state) {
    case 0:
        vx = 0x200;
        if (o->animFrame & 1)
            vx = -0x200;
        o->b9c = 1;
        o->velX = vx;
        o->velY = 0;
        U8(o, 0xc3) = 0;
        o->timer = 0x14;
        o->state++;
    case 1:
        o->visible = 1;
        SETANIM_IF(0x2e);
        FUN_8001e560(0x23, 0x24);
        FUN_8001e4f0(0x1f);
        o->b9c = 1;
        if (U8(o, 0xac) >= 2) {
            D_8009D2E8 = D_800A611C;
            D_8009D2E8->b04 = 2;
            D_8009D2E8->step = 2;
            D_8009D2E8->state = 0;
        }
        U8(o, 0xac) = 0;
        D_8009C934 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        D_8009C330->b0 = 0;
        o->d8c = 0;
        if (o->b9e != 0)
            D_8009F0EC->b6a = 0;
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        o->ba7 = 0;
        o->wb0 = 0;
        D_8009C330->b8 = 0;
        *(signed char *)&o->b0f = -8;
        o->ba4 = 0;
        o->b69 = 0;
        o->velV = 0;
        o->state++;
        if (D_1F8001A4[0] == 0 && o->w9a != o->w98) {
            o->w9a = o->w98;
            D_8009C330->b8 = 1;
            D_8009C970[0] = o->w98;
            if (o->w98 <= 0) {
                D_8009C942 = 1;
                D_8009C938 = 1;
                FUN_8001f110(0);
                FUN_8001f2ec(3);
            }
        }
        D_8009C330->b8 = D_8009C938;
        FUN_8001f96c(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_80025f40(0, 0x81, 0x81, 0x3c);
        break;
    case 2:
        move(o);
        if (AnimAdvance(o)) {
            SETANIM_IF(0x10);
            o->state = 3;
        }
        break;
    case 3:
        move(o);
        o->d8c = (o->d8c + (o->animFrame & 1 ? 0x10 : -0x10)) & 0xff;
        AnimAdvance(o);
        if (--o->timer <= 0)
            o->state++;
        if (D_8009C330->b8 != 0)
            break;
        if (o->velY > 0) {
            LAND();
        }
        if (FUN_8003facc(o)) {
            LAND();
        }
        break;
    case 4:
        move(o);
        AnimAdvance(o);
        o->velY += 0x40;
        if (o->velY > 0x680)
            o->velY = 0x680;
        if (o->b69 != 0 || FUN_8003fd78(o, 0, 0) != 0) {
            o->b9c = 0;
            o->substep = 0;
            o->animFrame ^= 1;
            o->state++;
        }
        if (D_8009C330->b8 != 0)
            break;
        if (o->velY > 0) {
            LAND();
        }
        break;
    case 5:
        BLINK();
        FUN_800fc414(o);
        break;
    }
}
