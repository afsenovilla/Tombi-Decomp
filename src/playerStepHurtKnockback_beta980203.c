// VERSION BETA_980203 x00 801018A8 1536
#if defined(VERSION_BETA_980203)
// Beta version: the JP branch of playerStepHurtKnockback.c with the PAL step-1 order, the SPAD.b1a4 test, field 0xd7 (PAL 0xe3) and a pad vibration (0, 0x41, 0x41, 60).
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

extern P800FD358 *PLAYER_EXT;
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

extern char MOVE_TABLE_3[];
extern void spawnPlayerEffect4E(int, int, int);
extern void FUN_8001e4f0(int);
extern void applyAnimVelocityX(TObj *, int);

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

void func_800FCE48(TObj *o)
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
                GRABBED_OBJECT->active = 1;
            q = GRABBED_OBJECT;
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
        PLAYER_EXT->b8 = 0;
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
        U8(o, 0xd7) = 0;
        PLAYER_EXT->b0 = 0;
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
        *(signed char *)&o->b0f = -8;
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
        o->ba4 = 0;
        o->b69 = 0;
        o->velV = 0;
        o->movetab = MOVE_TABLE_3;
        SETANIM_IF(0x39);
        o->y.raw += o->velY << 8;
        if (o->active != 1)
            spawnImpactEffect(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e560(0x23, 0x24);
        FUN_8001e4f0(0x1f);
        startPadVibration(0, 0x41, 0x41, 0x3c);
        o->state++;
        break;
    case 2:
        BLINK();
        AnimAdvance(o);
        if (o->b69 == 0 && !(o->animFrame & 2))
            applyAnimVelocityX(o, o->animFrame ^ 1);
        o->h->raw += GVARS.driftH << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        o->velY += 0x40;
        o->y.raw += o->velY << 8;
        if (o->velY > 0) {
            SETANIM_IF(0x3a);
            o->b9c = 2;
            o->state++;
        }
        if (FUN_8003facc(o)) {
            SETANIM_IF(0x3a);
            o->b9c = 2;
            o->velY = 0;
            o->state = 3;
        }
        break;
    case 3:
        BLINK();
        AnimAdvance(o);
        if (o->b69 == 0 && !(o->animFrame & 2))
            applyAnimVelocityX(o, o->animFrame ^ 1);
        o->h->raw += GVARS.driftH << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (playerHurtCheckFastFall(o) == 0) {
            if (o->b69 == 1 || FUN_8003fd78(o, 0, 0)) {
                SETANIM_IF(0x3b);
                o->b04 = 2;
                o->step = 0;
                o->state = 4;
                o->substep = 0;
            }
        }
        break;
    case 4:
        BLINK();
        playerHurtLandRecover(o);
        break;
    }
}
#endif
