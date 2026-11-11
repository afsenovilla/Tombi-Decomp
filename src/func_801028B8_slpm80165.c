// VERSION SLPM_80165 x00 801028B8 732
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepWalkIntoDepth (src/playerStepWalkIntoDepth.c): no area 8 entry and no axis
// swap; PlayerSetAnimIfChanged inline. The volatile store of velX keeps it ahead of the area check (a plain
// store is scheduled into the branch delay slot).
#include "TOBJ.H"
#include "gamestate.h"
#define B(o, n) (((unsigned char *)(o))[n])
extern TObj *PLAYER_EXT;
extern unsigned short SPAWN_AXIS_SWAP;

extern int AnimAdvance(TObj *);


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

void func_801028B8(TObj *o)
{
    unsigned short f;
    switch (o->state) {
    case 0:
        B(PLAYER_EXT, 8) = o->active;
        B(PLAYER_EXT, 9) = 0;
        *(volatile short *)&o->velX = 0xb4;
        o->state++;
        if (((int*)&GAME.selectedArea)[0] == 0x40001) {
            B(PLAYER_EXT, 9) = 1;
            o->velX = 0x78;
            o->state = 2;
        }
        f = o->animFrame;
        o->active = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        B(o, 0xad) = 0;
        o->b69 = 0;
        *(signed char *)&o->b0f = -20;
        B(o, 0xa3) = 2;
        o->animFrame = f & 1;
        demoSetAnim(o, 0x2c);
    case 1:
        AnimAdvance(o);
        o->d->p.whole -= 2;
        o->velX -= 2;
        if (o->velX <= 0)
            o->state = 9;
        break;
    case 2:
        AnimAdvance(o);
        o->d->p.whole -= 1;
        o->velX -= 1;
        if (o->velX <= 0) {
            o->velX = 0x18;
            o->state = 3;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->y.p.whole += 2;
        o->velX -= 2;
        if (o->velX <= 0) {
            o->velX = 0x18;
            o->state = 4;
        }
        break;
    case 4:
        AnimAdvance(o);
        o->d->p.whole -= 1;
        o->velX -= 1;
        if (o->velX <= 0) {
            o->velX = 0x18;
            o->state = 3;
        }
        if (o->d->p.whole < 2000)
            o->state = 8;
        break;
    case 8:
        AnimAdvance(o);
        o->d->p.whole -= 1;
        break;
    case 9:
        AnimAdvance(o);
        *(signed char *)&o->b0f = -8;
        o->active = B(PLAYER_EXT, 8);
        o->b9c = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        PLAYER_EXT->timer = 0;
        break;
    }
}
#endif
