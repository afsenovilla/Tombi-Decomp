// VERSION SLPM_80165 x00 80106544 852
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepForcedLaunch (src/playerStepForcedLaunch.c, JP branch): no velocity clear, no
// release of a held object, anim 0x10 (PlayerSetAnimIfChanged inline), and the landing loads the idle anim
// without the slope tilt.
#include "gvars.h"
#include "TOBJ.H"
typedef struct { char p[8]; unsigned char b8; unsigned char b9; char q[0x20 - 0xa]; unsigned short w20; char r[0x2c - 0x22]; short w2c; short w2e; } G;
extern G *PLAYER_EXT;
extern TObj *PLAYER_HELD_OBJECT[];
extern char PLAYER_IDLE_ANIM[];
extern unsigned char SLOPE_TILT_TABLE[];
void playerReleaseBittenAndClearFlags(TObj *o);
void playerSetJumpRiseVelocity(TObj *o);

void ObjGravityStep(TObj *o);
void ObjAddVelY7E(TObj *o);
int AnimAdvance(TObj *o);
short TileCollideAt(TObj *, short, short);
int ObjCheckHeadCollision(TObj *o);
short ObjTileCollide(TObj *, int, int);
void AnimLoadDuration(TObj *o);

extern struct DemoAnimExt { char pad[0x2c]; unsigned short timer, frame; } *demoAnimExt asm("PLAYER_EXT");
extern void demoAnimReset(void *) asm("func_800EDA04");
extern void demoAnimRestart(void *) asm("func_80020554");
static __inline__ void demoSetAnim(void *o, unsigned short anim)
{
    struct DemoAnimExt *p = demoAnimExt;
    p->timer = anim;
    if (p->frame != anim) {
        demoAnimReset(o);
        demoAnimRestart(o);
        demoAnimExt->frame = demoAnimExt->timer;
    }
}

void func_80106544(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b69 = 0;
        o->b9e = 0;
        o->b9c = 1;
        o->wb2 = 0;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        PLAYER_EXT->b8 = 0;
        PLAYER_EXT->w20 = 0;
        *(unsigned char *)&o->wac = 0;
        playerSetJumpRiseVelocity(o);
        demoSetAnim(o, 0x10);
        o->state = 1;
    case 1:
        if (--PLAYER_EXT->b9 != 0) {
            if (PLAYER_EXT->b8 == 0) {
                PLAYER_EXT->w20++;
                playerSetJumpRiseVelocity(o);
            }
        } else {
            PLAYER_EXT->b8 = 1;
            if (PLAYER_EXT->w20 >= 5) {
                PLAYER_EXT->b8 = 0;
                o->state = 2;
            } else {
                PLAYER_EXT->w20++;
                playerSetJumpRiseVelocity(o);
            }
        }
    case 2:
        o->h->raw += ((short*)&GVARS.driftH)[0] << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 3;
        }
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 3;
        }
        o->b69 = 0;
        break;
    case 3:
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        o->h->raw += ((short*)&GVARS.driftH)[0] << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 == 1 || ObjTileCollide(o, 0, 0)) {
            PLAYER_EXT->b8 = 0;
            o->ba5 = 0;
            o->b9c = 0;
            *(unsigned char *)&o->wac = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            PLAYER_EXT->w20 = 0;
            PLAYER_EXT->w2c = 0;
            PLAYER_EXT->w2e = 0;
            o->anim = PLAYER_IDLE_ANIM;
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    case 4:
        break;
    }
}
#endif
