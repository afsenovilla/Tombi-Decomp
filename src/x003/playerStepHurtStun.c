// FUNC 800fdb7c 1472 X003
// MATCHING 800fdb7c 1472
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
} P800FDB7C;

extern P800FDB7C *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
extern TObj *GRABBED_OBJECT;
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001f110(int);
extern void playJingle(int);
extern void spawnImpactEffect(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void startPadVibration(int, int, int, int);
extern int AnimAdvance(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern short playerHurtCheckFastFall(TObj *);
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

void playerStepHurtStun(TObj *o)
{
    TObj *q;
    short vx;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->wb0 = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->ba7 = 0;
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
        PLAYER_EXT->b8 = 0;
        U8(o, 0xc3) = 0;
        if (SPAD.b1a4 == 0 && o->w9a != o->w98) {
            o->w9a = o->w98;
            PLAYER_EXT->b8 = 1;
            ((unsigned char*)&GAME.playerHealth)[0] = o->w98;
            if (o->w98 <= 0) {
                GVARS.b42 = 1;
                GVARS.hurtState = 1;
                FUN_8001f110(0);
                playJingle(3);
            }
        }
        PLAYER_EXT->b8 = GVARS.hurtState;
        o->animFrame &= 1;
        if (o->b9e != 0) {
            if (o->b9e == 4 || o->b9e == 7)
                GRABBED_OBJECT->active = 1;
            q = GRABBED_OBJECT;
            if (q->type == 0x21)
                q->ba7 = 0;
            else
                q->b6a = 0;
        }
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
        *(signed char *)&o->b0f = -8;
        o->b9e = 0;
        o->ba4 = 0;
        o->b69 = 0;
        o->velY = 0;
        o->velV = 0;
        SETANIM_IF(0x1e);
        o->state = 1;
        spawnImpactEffect(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e560(0x23, 0x24);
        startPadVibration(0, 0x81, 0x81, 0x3c);
    case 1:
        BLINK();
        SETANIM_IF(0x1e);
        AnimAdvance(o);
        break;
    case 2:
        AnimAdvance(o);
        vx = 0x200;
        o->d8c = 0;
        o->velY = -0x400;
        o->velV = 0;
        if (o->animFrame & 1)
            vx = -0x200;
        o->velX = vx;
        o->state++;
        if (GVARS.hurtState == 1) {
            o->b04 = 2;
            o->step = 3;
            o->state = 0;
            break;
        }
    case 3:
        BLINK();
        o->h->raw += GVARS.driftH << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        AnimAdvance(o);
        o->velY += 0x20;
        o->y.raw += o->velY << 8;
        if (o->ba6)
            o->velX = 0;
        if (o->b69 == 0)
            o->h->raw += o->velX << 8;
        if (o->velY > 0) {
            o->state = 4;
            o->b9c = 2;
        }
        if (FUN_8003facc(o)) {
            o->b9c = 2;
            o->velY = 0;
            o->state = 4;
        }
        break;
    case 4:
        BLINK();
        o->h->raw += GVARS.driftH << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        AnimAdvance(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (playerHurtCheckFastFall(o) == 0) {
            if (o->ba6)
                o->velX = 0;
            if (o->b69 == 0) {
                o->h->raw += o->velX << 8;
                if (o->b69 == 0 && FUN_8003fd78(o, 0, 0) == 0)
                    break;
            }
            o->substep = 0;
            o->animFrame ^= 1;
            o->state++;
        }
        break;
    case 5:
        BLINK();
        playerHurtLandRecover(o);
        break;
    }
}
