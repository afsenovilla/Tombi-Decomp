// FUNC 800f44b0 2488 X001
// MATCHING 800f44b0 2488
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[6];
    signed char b7;
    unsigned char b8;
    unsigned char p9[0x28 - 9];
    unsigned short w28;
    unsigned short w2a;
    unsigned short w2c;
    unsigned short w2e;
} P800F44B0;

extern P800F44B0 *D_8009C330;
extern TObj *D_8009D2E8;
extern TObj *D_800A611C;
extern short D_8009C944;
extern short D_8009C946[];
extern unsigned short D_8009D670;
extern unsigned char D_8009D2B0;
extern int D_8009C984;
extern unsigned char D_8009D2B2;
extern short D_8007A038[];
extern char *D_800A605C;
extern char D_80010E80[], D_80010EA0[], D_80010EC0[];
extern unsigned char D_801152E8[];
extern void FUN_8001fe94(TObj *, int);
extern TObj *FUN_800182ac(void);
extern void FUN_80110fdc(TObj *);
extern void FUN_8001fc14(TObj *, int, int);
extern void FUN_8001fce4(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_800efa80(TObj *);
extern void FUN_8001fec0(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);

#define YADJ() \
    if (U8(o, 0xad) == 0) { \
        if (o->subtype) { \
            if ((unsigned short)(o->wb0 + 5) >= 0xb) o->y.raw += 0x50000; \
            else o->y.raw += 0x30000; \
        } else { \
            if ((unsigned short)(o->wb0 + 5) >= 0xb) o->y.raw += 0xc0000; \
            else o->y.raw += 0x80000; \
        } \
    }

#define DROP() \
    switch (D_8009C330->b7) { \
    case 0: case 2: \
        o->h->p.whole += 0x10; \
        o->y.p.whole += 4; \
        break; \
    case 1: case 3: \
        o->h->p.whole -= 0x10; \
        o->y.p.whole += 4; \
        break; \
    case 4: \
        o->h->p.whole -= 0xc; \
        o->y.p.whole -= 0xc; \
        break; \
    case 5: \
        o->h->p.whole -= 0xc; \
        o->y.p.whole -= 0xc; \
        break; \
    case 6: case 7: \
        o->y.p.whole -= 0x10; \
        break; \
    } \
    o->b69 = 0; \
    D_8009C330->b8 = 0; \
    U8(o, 0xac) = 0; \
    o->ba7 = 0; \
    o->b9c = 0; \
    o->step = 0x32; \
    o->state = 0;

#define LAND() \
    YADJ(); \
    FUN_8001e5f4(0x1c, 0x7f); \
    o->b9c = 0; \
    o->ba7 = 0; \
    U8(o, 0xac) = 0; \
    o->b9d = 0; \
    o->d8c = D_801152E8[o->wb0]; \
    if (o->bbe & 0x20) { \
        o->step = 0x1f; \
        o->state = 1; \
    } else { \
        o->step = 1; \
        o->state = 0; \
    }

void FUN_800f44b0(TObj *o)
{
    volatile unsigned short *k;
    unsigned short f;
    int d;
    signed char c;
    TObj *q;
    unsigned char b;
    short w;
    short x;

    if (o->b9d == 0) {
        D_8009C330->b7 = o->animFrame;
        f = o->animFrame & 1;
        o->animFrame = f;
        k = &D_8009D670;
        if (*k & 0x10) {
            if (*k & 0x80) o->animFrame = 5;
            else if (*k & 0x20) o->animFrame = 4;
            else if (f) o->animFrame = 7;
            else o->animFrame = 6;
        } else {
            if (*k & 0x80) o->animFrame = 3;
            else if (*k & 0x20) o->animFrame = 2;
            else if (f) o->animFrame = 3;
            else o->animFrame = 2;
        }
    }
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->b9d = 1;
        U8(o, 0xc8) = 0;
        if (U8(o, 0xac) >= 2) {
            D_8009D2E8 = D_800A611C;
            U8(D_800A611C, 4) = 2;
            U8(D_8009D2E8, 5) = 2;
            U8(D_8009D2E8, 6) = 0;
        }
        U8(o, 0xac) = 0;
        U8(o, 0xc3) = 0;
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        U8(D_8009C330, 0) = 2;
        U8(D_8009C330, 7) = o->animFrame;
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
        o->state = 1;
        c = S8(o, 0xe3);
        if (c < D_8007A038[D_8009D2B2]) {
            S8(o, 0xe3) = c + 1;
            q = FUN_800182ac();
            if (q != 0) {
                q->active = 1;
                b = D_8009D2B2;
                q->step = 0;
                q->state = 0;
                q->type = b;
            }
        }
        break;
    case 1:
        w = o->wb0;
        x = w;
        if (w < 0) { x <<= 2; x += 0x100; o->wb6 = x & 0xff; }
        else if (w > 0) { x <<= 2; o->wb6 = x & 0xff; }
        else o->wb6 = 0;
        FUN_80110fdc(o);
        FUN_8001fc14(o, o->wb6, o->wb2);
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_8001fce4(o);
        FUN_8010f0f4(o);
        FUN_8001fd94(o);
        if (D_8009C330->b0 == 0 && U8(o, 0xc6) == 0) {
            FUN_800efa80(o);
            o->b9d = 0;
            if (o->bbe & 0x20) {
                o->step = 0x1f;
                o->state = 1;
            } else {
                o->state = 2;
            }
        }
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = PTR(o, 0xe4);
            D_8009C330->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        if (o->b9c) {
            d = 0x10;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1) d = 0xf0;
            o->step = 4;
            o->d88 = d;
            o->d8c = 0;
            o->state = 1;
            o->b9c = 0;
        }
        if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
            D_8009D2B0 = 0;
            o->b9c = 1;
            if (D_8009C984 & 0x40) {
                if (*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C4) {
                    o->ba7 = 1;
                }
            }
            o->step = 10;
            o->state = 0;
        }
        if (U8(o, 0xc8)) {
            DROP();
            break;
        }
        YADJ();
        break;
    case 2:
        FUN_80110fdc(o);
        FUN_8010f0f4(o);
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_8001fce4(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (U8(o, 0xc8)) {
            DROP();
            break;
        }
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = PTR(o, 0xe4);
            o->ba7 = 0;
            D_8009C330->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        if (o->b69 == 1) {
            LAND();
        } else if (FUN_8003fd78(o, 0, 0)) {
            LAND();
        }
        break;
    }
}
