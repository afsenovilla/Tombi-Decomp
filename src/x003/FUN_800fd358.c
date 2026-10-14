// FUNC 800fd358 2084 X003
// MATCHING 800fd358 2084
#include "spad.h"
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[7];
    unsigned char b8;
    unsigned char p9[0x2c - 9];
    unsigned short w2c;
    unsigned short w2e;
} P800FD358;

extern P800FD358 *D_8009C330;
extern TObj *D_8009D2E8;
extern TObj *D_8009F0EC;
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001f110(int);
extern void playJingle(int);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void startPadVibration(int, int, int, int);
extern int AnimAdvance(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern short FUN_800fc240(TObj *);
extern void FUN_800fc414(TObj *);

extern char D_80077D24[];
extern void FUN_8002ff20(int, int, int);
extern void FUN_8001e4f0(int);
extern void applyAnimVelocityX(TObj *, int);

#define SETANIM_IF(n) \
    D_8009C330->w2c = n; \
    if (D_8009C330->w2e != n) { \
        D_8009C330->w2c = n; \
        FUN_800efc04(o); \
        FUN_8001fe94(o, 0); \
        D_8009C330->w2e = D_8009C330->w2c; \
    }

#define BLINK() \
    if (U8(o, 0xd1) == 0 && D_8009C330->b8 == 0) \
        o->visible = (*(unsigned char*)&SPAD.s1f8) & 1;

void FUN_800fd358(TObj *o)
{
    TObj *q;

    switch (o->state) {
    case 0:
        o->velY = -0x500;
        o->animFrame = ~o->animFrame & 1;
        o->state++;
    case 1:
        o->d8c = 0;
        if (o->b9e != 0) {
            if (o->b9e == 4 || o->b9e == 7)
                D_8009F0EC->active = 1;
            q = D_8009F0EC;
            if (q->type == 0x21)
                q->ba7 = 0;
            else
                q->b6a = 0;
        }
        o->b9e = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->ba7 = 0;
        U8(o, 0xc3) = 0;
        (*(unsigned char*)((char*)&o->da0 + 2)) = 0;
        (*(unsigned char*)((char*)&o->da0 + 3)) = 0;
        o->wb0 = 0;
        D_8009C330->b8 = 0;
        if ((*(unsigned char*)&o->wac) >= 2) {
            D_8009D2E8 = (*(TObj **)&PLX.p611c);
            D_8009D2E8->b04 = 2;
            D_8009D2E8->step = 2;
            D_8009D2E8->state = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        D_8009C330->b0 = 0;
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
        *(signed char *)&o->b0f = -8;
        if (SPAD.b1a4 == 0 && o->w9a != o->w98) {
            o->w9a = o->w98;
            D_8009C330->b8 = 1;
            ((unsigned char*)&GAME.playerHealth)[0] = o->w98;
            if (o->w98 <= 0) {
                GVARS.b42 = 1;
                GVARS.b38 = 1;
                FUN_8001f110(0);
                playJingle(3);
            }
        }
        D_8009C330->b8 = GVARS.b38;
        o->ba4 = 0;
        o->b69 = 0;
        o->movetab = D_80077D24;
        o->velV = 0;
        o->timer = 0;
        D_8009C330->w2e = 0xff;
        switch (U8(o, 0xd1)) {
        case 1:
            SETANIM_IF(0x53);
            FUN_8002ff20(9, 0, 0);
            break;
        case 3:
            SETANIM_IF(0x50);
            o->timer = 0x3c;
            break;
        default:
            SETANIM_IF(0x39);
            break;
        }
        o->y.raw += o->velY << 8;
        if (o->active != 1)
            FUN_8001f96c(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e560(0x23, 0x24);
        FUN_8001e4f0(0x1f);
        startPadVibration(0, 0x81, 0x81, 0x3c);
        o->visible = 1;
        o->state++;
    case 2:
        if (U8(o, 0xd1) == 3) {
            AnimAdvance(o);
            if (--o->timer > 0)
                break;
            SETANIM_IF(0x39);
        }
        o->state++;
        break;
    case 3:
        BLINK();
        AnimAdvance(o);
        if (o->b69 == 0 && !(o->animFrame & 2))
            applyAnimVelocityX(o, o->animFrame ^ 1);
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        o->velY += 0x40;
        o->y.raw += o->velY << 8;
        if (o->velY > 0) {
            if (U8(o, 0xd1) == 1) {
                SETANIM_IF(0x54);
            } else {
                SETANIM_IF(0x3a);
            }
            o->b9c = 2;
            o->state++;
        }
        if (FUN_8003facc(o)) {
            if (U8(o, 0xd1) == 1) {
                SETANIM_IF(0x54);
            } else {
                SETANIM_IF(0x3a);
            }
            o->b9c = 2;
            o->velY = 0;
            o->state = 4;
        }
        break;
    case 4:
        BLINK();
        AnimAdvance(o);
        if (o->b69 == 0 && !(o->animFrame & 2))
            applyAnimVelocityX(o, o->animFrame ^ 1);
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (FUN_800fc240(o) == 0) {
            if (o->b69 == 1 || FUN_8003fd78(o, 0, 0)) {
                if (U8(o, 0xd1) == 1) {
                    SETANIM_IF(0x55);
                } else {
                    SETANIM_IF(0x3b);
                }
                o->b04 = 2;
                o->step = 0;
                o->state = 5;
                o->substep = 0;
            }
        }
        break;
    case 5:
        BLINK();
        FUN_800fc414(o);
        break;
    }
}
