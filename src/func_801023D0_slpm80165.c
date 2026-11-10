// VERSION SLPM_80165 x00 801023D0 428
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepDepthShift (src/x003/playerStepDepthShift.c): no wait for the camera, always towards
// +depth for 0xB4 / 5 frames, and anim 0x2C at the end (PlayerSetAnimIfChanged inline).
#include "gcd.h"
#include "TOBJ.H"
#include "raw7.h"
extern TObj *PLAYER_EXT;
extern unsigned short SPAWN_AXIS_SWAP;

extern void AnimAdvance(TObj *);

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

void func_801023D0(TObj *o)
{
    unsigned short f;
    switch (o->state) {
    case 0:
        (*(unsigned char*)&PLAYER_EXT->w08) = o->active;
        f = o->animFrame;
        o->velX = 0xb4;
        o->active = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        (*(unsigned char*)((char*)&o->wac + 1)) = 0;
        o->b69 = 0;
        (*(signed char*)&o->b0f) = -20;
        (*(unsigned char*)((char*)&o->da0 + 2)) = 2;
        o->animFrame = f & 1;
        demoSetAnim(o, 0x2b);
        o->state++;
    case 1:
        AnimAdvance(o);
        o->d->p.whole = o->d->p.whole + 5;
        if ((o->velX -= 5) == 0) {
            (*(signed char*)&o->b0f) = -8;
            demoSetAnim(o, 0x2c);
            o->active = (*(unsigned char*)&PLAYER_EXT->w08);
            o->b9c = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            PLAYER_EXT->timer = 0;
        }
        break;
    }
}
#endif
