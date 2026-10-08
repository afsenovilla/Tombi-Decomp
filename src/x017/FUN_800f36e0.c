// FUNC 800f36e0 2060 X017
// MATCHING 800f36e0 2060
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[6];
    signed char b7;
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    unsigned short w20;
    unsigned char p22[6];
    unsigned short w28;
    unsigned short w2a;
    unsigned short w2c;
    unsigned short w2e;
} P800F36E0;

extern P800F36E0 *D_8009C330;
extern TObj *D_8009D2E8;
typedef struct { short x, y; } SXY;
extern short D_8009C944;
extern short D_8009C946;
extern volatile unsigned short D_8009D670[];
extern unsigned short D_1f8003c6;
extern unsigned char D_8009D2B2;
extern short D_8007A038[];
extern char *D_800A605C;
extern char D_80010E80[], D_80010EA0[], D_80010EC0[];
extern unsigned char D_801152E8[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern TObj *FUN_800182ac(void);
extern void FUN_8010f328(TObj *);
extern void FUN_80110fdc(TObj *);
extern void FUN_8010eaf8(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_80040278(TObj *, short, short);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8011133c(TObj *);
extern void FUN_801113bc(TObj *);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001e4f0(int);
extern void FUN_800eae0c(int, short, int, int);

#define SETANIM_IF(n) \
    D_8009C330->w2c = n; \
    if (D_8009C330->w2e != n) { \
        D_8009C330->w2c = n; \
        FUN_800efc04(o); \
        FUN_8001fe94(o, 0); \
        D_8009C330->w2e = D_8009C330->w2c; \
    }

void FUN_800f36e0(TObj *o)
{
    int d;
    signed char c;
    signed char b;
    TObj *q;

    switch (o->state) {
    case 0:
        d = 0x10;
        o->timer = 10;
        o->d84 = 0;
        if (o->animFrame & 1) d = 0xf0;
        o->b9d = 1;
        U8(o, 0xc8) = 0;
        U8(o, 0xa0) = 0;
        U8(o, 0xa1) = 0;
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        o->d88 = d;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        D_8009C330->b0 = 2;
        D_8009C330->w2e = -1;
        D_8009C330->w28 = -1;
        D_8009C330->w2a = -1;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3:
            D_800A605C = D_80010E80;
            break;
        case 4: case 5:
            D_800A605C = D_80010EA0;
            break;
        case 6: case 7:
            D_800A605C = D_80010EC0;
            break;
        }
        FUN_8001fe94(o, 0);
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = PTR(o, 0xe4);
            D_8009C330->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        U8(o, 0xac) = 0;
        c = S8(o, 0xe3);
        if (c < D_8007A038[D_8009D2B2]) {
            S8(o, 0xe3) = c + 1;
            q = FUN_800182ac();
            if (q != 0) {
                q->active = 1;
                q->type = D_8009D2B2;
                q->step = 0;
                q->state = 0;
            }
        }
        if (D_8009C330->b8 != 0) {
            o->state = 2;
        } else {
            o->state = 1;
        }
        break;
    case 1:
        if (o->velY >= 0) {
            D_8009C330->b8 = 1;
            o->state = 2;
        }
        if (D_8009D670[0] & D_1f8003c6) {
            if (D_8009C330->b8 == 0) {
                if (++D_8009C330->w20 >= 0xe) {
                    D_8009C330->b8 = 1;
                    o->state = 2;
                }
                FUN_8010f328(o);
            }
        } else {
            D_8009C330->b8 = 1;
            if (D_8009C330->w20 >= 5) {
                o->state = 2;
            } else {
                D_8009C330->w20++;
                FUN_8010f328(o);
            }
        }
    case 2:
        *(int *)o->h += D_8009C944 << 8;
        o->y.raw += D_8009C946 << 8;
        FUN_80110fdc(o);
        FUN_8010eaf8(o);
        o->h->raw += o->velX << 8;
        FUN_8010f0f4(o);
        FUN_8001fd94(o);
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = PTR(o, 0xe4);
            D_8009C330->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        if (o->velY > 0) {
            d = 0x10;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1) d = 0xf0;
            U8(o, 0xac) = 1;
            o->b9c = 2;
            o->d88 = d;
            o->d8c = 0;
            o->state = 3;
        }
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (FUN_8003facc(o)) {
            d = 0x10;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1) d = 0xf0;
            U8(o, 0xac) = 1;
            o->b9c = 2;
            o->d88 = d;
            o->d8c = 0;
            o->state = 3;
            o->velY = 0;
            o->velV = 0;
        }
        if (U8(o, 0xc8) != 0) {
            D_8009C330->b8 = 0;
            U8(o, 0xac) = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->step = 0x32;
            o->state = 0;
            break;
        }
        if (D_8009C330->b0 != 0) break;
        if (U8(o, 0xc6) != 0) break;
        U8(o, 0xac) = 0;
        SETANIM_IF(4);
        d = 0x10;
        o->b9d = 0;
        o->b9c = 1;
        o->d84 = 0;
        if (o->animFrame & 1) d = 0xf0;
        o->d88 = d;
        o->d8c = 0;
        o->step = 2;
        o->state = 2;
        break;
    case 3:
        *(int *)o->h += D_8009C944 << 8;
        o->y.raw += D_8009C946 << 8;
        FUN_80110fdc(o);
        o->d8c = 0;
        D_8009C330->b8 = 1;
        if (U8(o, 0xc9) != 0) {
            FUN_8001fec0(o);
            *(int *)o->h += D_8009C944 << 7;
            o->y.raw += D_8009C946 << 7;
            FUN_8011133c(o);
            o->h->raw += o->velX << 8;
            if (--o->timer <= 0) {
                o->timer = 0;
                FUN_801113bc(o);
                FUN_8001fd94(o);
            }
        } else {
            FUN_8010eaf8(o);
            o->h->raw += o->velX << 8;
            FUN_8010f0f4(o);
            FUN_8001fd94(o);
        }
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = PTR(o, 0xe4);
            D_8009C330->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        if (U8(o, 0xc8) != 0) {
            D_8009C330->b8 = 0;
            U8(o, 0xac) = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->step = 0x32;
            o->state = 0;
            break;
        }
        if (o->b69 == 1 || FUN_8003fd78(o, 0, 0) != 0) {
            FUN_8001e5f4(0x1c, 0x7f);
            D_8009C330->b8 = 0;
            o->step = 3;
            o->state = 1;
            o->ba7 = 0;
            U8(o, 0xac) = 0;
            o->b9c = 0;
            o->d8c = D_801152E8[o->wb0];
            if (o->bbe & 0x20) {
                FUN_8001e4f0(0x3e);
                FUN_800eae0c(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
                FUN_800eae0c(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
            }
            break;
        }
        if (D_8009C330->b0 != 0) break;
        if (U8(o, 0xc6) != 0) break;
        SETANIM_IF(4);
        d = 0x10;
        b = D_8009C330->b7;
        o->b9d = 0;
        U8(o, 0xac) = 1;
        o->d84 = 0;
        o->animFrame = b;
        o->b9c = 2;
        if (o->animFrame & 1) d = 0xf0;
        o->d88 = d;
        o->d8c = 0;
        o->step = 2;
        o->state = 3;
        break;
    }
}
