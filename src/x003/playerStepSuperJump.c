// FUNC 8010e66c 684 X003
// MATCHING 8010e66c 684
#include "gvars.h"
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
extern TObj *PLAYER_EXT;
extern int MulCos(int a, int b);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimLoadDuration(TObj *);
extern void SfxPlay2(int a, int b);
extern void spawnPlayerPowerEffect(int);
extern int AnimAdvance(TObj *);
extern void playerUpdateSpeedFromPad(TObj *);
extern void playerAirAccelerateX(TObj *);
extern void ObjMotionStep(TObj *);
extern void ObjAddVelY7E(TObj *);
extern void ObjTileCollide(TObj *, int, int);
extern void playerAirSpinToFall(TObj *);
extern int ObjCheckHeadCollision(TObj *);

void playerStepSuperJump(TObj *o)
{
    int v;
    switch (o->state) {
    case 0:
        o->timer = 0x31;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        B(o, 0xc3) = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        B(o, 0xa0) = 0;
        B(o, 0xa1) = 0;
        o->wb6 = 0;
        o->velX = MulCos(0, o->wb2);
        o->velY = -10;
        B(PLAYER_EXT, 8) = 0;
        PLAYER_EXT->animTimer = 4;
        PLAYER_EXT->timer = 0;
        PLAYER_EXT->step = 0;
        PLAYER_EXT->b1d = 0;
        B(o, 0xac) = 0;
        ObjSetAnimFromTable(o);
        AnimLoadDuration(o);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
        SfxPlay2(2, 4);
        spawnPlayerPowerEffect(2);
        o->state = 1;
    case 1:
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        AnimAdvance(o);
        playerUpdateSpeedFromPad(o);
        playerAirAccelerateX(o);
        o->h->raw += o->velX << 8;
        o->velY -= 0x10;
        if (o->velY < -0x800)
            o->velY = -0x800;
        if (B(o, 0xcc) == 3)
            o->state = 2;
        ObjAddVelY7E(o);
        break;
    case 2:
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        AnimAdvance(o);
        playerUpdateSpeedFromPad(o);
        ObjMotionStep(o);
        break;
    default:
        return;
    }
    ObjTileCollide(o, 0, 0);
    if (o->velY >= -0x383)
        playerAirSpinToFall(o);
    if (ObjCheckHeadCollision(o) || o->velY > 0) {
        o->d84 = 0;
        v = 0x10;
        if (o->animFrame & 1)
            v = 0xf0;
        o->d88 = v;
        B(o, 0xac) = 1;
        o->b9c = 2;
        o->timer = 10;
        o->step = 2;
        o->state = 3;
    }
    o->b69 = 0;
}
