// FUNC 800fe98c 1768 X008
// MATCHING 800fe98c 1768
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
} P800FE98C;

extern P800FE98C *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
extern TObj *GRABBED_OBJECT;
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001f110(int);
extern void playJingle(int);
extern void spawnImpactEffect(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void FUN_8001e4f0(int);
extern void startPadVibration(int, int, int, int);
extern int AnimAdvance(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void playerHurtLandRecover(TObj *);

#define SETANIM_IF(n) \
    PLAYER_EXT->w2c = n; \
    if (PLAYER_EXT->w2e != n) { \
        PLAYER_EXT->w2c = n; \
        FUN_800efc04(o); \
        FUN_8001fe94(o, 0); \
        PLAYER_EXT->w2e = PLAYER_EXT->w2c; \
    }

#define BLINK() \
    if (PLAYER_EXT->b8 == 0) \
        o->visible = (*(unsigned char*)&SPAD.s1f8) & 1;

static __inline__ void move(TObj *o)
{
    o->h->raw += GVARS.s44 << 8;
    o->y.raw += ((short*)&GVARS.s46)[0] << 8;
    BLINK();
    o->h->raw += o->velX << 8;
    o->y.raw += o->velY << 8;
}

#define LAND() { \
    int d = 0x10; int c3 = 3; int c2 = 2; int c1 = 1; \
    PLAYER_EXT->b8 = 0; \
    o->active = c3; \
    S16(o, 0xe0) = 0x8c; \
    o->b9c = c2; \
    (*(unsigned char*)&o->wac) = c1; \
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

void playerStepHurtSpin(TObj *o)
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
        if ((*(unsigned char*)&o->wac) >= 2) {
            BITTEN_OBJECT = (*(TObj **)&PLX.p611c);
            BITTEN_OBJECT->b04 = 2;
            BITTEN_OBJECT->step = 2;
            BITTEN_OBJECT->state = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        PLAYER_EXT->b0 = 0;
        o->d8c = 0;
        if (o->b9e != 0)
            GRABBED_OBJECT->b6a = 0;
        o->b9e = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->ba7 = 0;
        o->wb0 = 0;
        PLAYER_EXT->b8 = 0;
        *(signed char *)&o->b0f = -8;
        o->ba4 = 0;
        o->b69 = 0;
        o->velV = 0;
        o->state++;
        if (((unsigned char*)&SPAD.b1a4)[0] == 0 && o->w9a != o->w98) {
            o->w9a = o->w98;
            PLAYER_EXT->b8 = 1;
            ((unsigned char*)&GAME.playerHealth)[0] = o->w98;
            if (o->w98 <= 0) {
                GVARS.b42 = 1;
                GVARS.b38 = 1;
                FUN_8001f110(0);
                playJingle(3);
            }
        }
        PLAYER_EXT->b8 = GVARS.b38;
        spawnImpactEffect(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        startPadVibration(0, 0x81, 0x81, 0x3c);
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
        if (PLAYER_EXT->b8 != 0)
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
        if (PLAYER_EXT->b8 != 0)
            break;
        if (o->velY > 0) {
            LAND();
        }
        break;
    case 5:
        BLINK();
        playerHurtLandRecover(o);
        break;
    }
}
