// VERSION SLPM_80165 x00 80105FD4 1268
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepForcedJump (src/playerStepForcedJump.c, beta / JP form): playerAirSpinToFall is the demo
// helper func_800EC084 written inline (static __inline__ demoSpin), one jump sound, no slope tilt on landing.
#include "gvars.h"
#include "TOBJ.H"

#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
#define G PLAYER_EXT
extern unsigned char *PLAYER_EXT;
extern char PLAYER_IDLE_ANIM[];
extern unsigned char SLOPE_TILT_TABLE[];
void playerSetJumpRiseVelocity(TObj *o);

void SfxPlay2(int a, int b);
extern void SfxPlay(int) asm("func_8001E3D8");
void playerAirSpinToFall(TObj *o);
void AnimAdvance(TObj *o);
void ObjGravityStep(TObj *o);
void ObjAddVelY7E(TObj *o);
void TileCollideAt(TObj *o, short x, short y);
int ObjCheckHeadCollision(TObj *o);
short ObjTileCollide(TObj *o, int a, int b);
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

extern char D_800108BC[];
extern void func_8002057C(TObj *, int);
static __inline__ void demoSpin(TObj *o)
{
    if (o->animFrame & 1) {
        if (o->d8c > 0x80)
            o->d8c = (0x100 - o->d8c) & 0xff;
        o->d8c = (o->d8c + 4) & 0xff;
        if (o->d8c > 0x40) {
            o->anim = D_800108BC;
            func_8002057C(o, 3);
            o->d8c = 0;
        }
    } else {
        if (o->d8c < 0x80)
            o->d8c = (0x100 - o->d8c) & 0xff;
        o->d8c = (o->d8c - 4) & 0xff;
        if (o->d8c < 0xc0) {
            o->anim = D_800108BC;
            func_8002057C(o, 3);
            o->d8c = 0;
        }
    }
    if (*(unsigned short *)o->anim == 0x55)
        o->d8c = 0;
}

void func_80105FD4(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b69 = 0;
        B(o, 0x9e) = 0;
        o->b9c = 1;
        o->wb2 = 0;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        G[8] = 0;
        *(short *)(G + 0x20) = 0;
        B(o, 0xac) = 0;
        playerSetJumpRiseVelocity(o);
        demoSetAnim(o, 4);
        SfxPlay(2);
        o->state = 1;
    case 1:
        if (--G[9] != 0) {
            if (G[8] == 0) {
                (*(unsigned short *)(G + 0x20))++;
                playerSetJumpRiseVelocity(o);
            }
        } else {
            G[8] = 1;
            if (*(unsigned short *)(G + 0x20) >= 5) {
                G[8] = 0;
                o->state = 2;
            } else {
                (*(unsigned short *)(G + 0x20))++;
                playerSetJumpRiseVelocity(o);
            }
        }
    case 2:
        demoSpin(o);
        AnimAdvance(o);
        o->h->raw += ((short*)&GVARS.driftH)[0] << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 3;
        }
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            o->velY = 0;
            *(short *)((char *)o + 0x82) = 0;
            o->state = 3;
        }
        o->b69 = 0;
        break;
    case 3:
        demoSpin(o);
        AnimAdvance(o);
        B(o, 0x9e) = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        o->h->raw += ((short*)&GVARS.driftH)[0] << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        if (o->b69 == 1 || ObjTileCollide(o, 0, 0)) {
            G[8] = 0;
            B(o, 0xa5) = 0;
            o->b9c = 0;
            B(o, 0xac) = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            *(short *)(G + 0x20) = 0;
            *(unsigned short *)(G + 0x2e) = 0xffff;
            *(short *)(G + 0x2c) = 0;
            o->anim = PLAYER_IDLE_ANIM;
            AnimLoadDuration(o);
            o->state = 4;
        }
        break;
    case 4:
        break;
    }
}
#endif
