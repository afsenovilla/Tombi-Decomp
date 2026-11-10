// VERSION SLPM_80165 x00 8010257C 828
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8010834C (src/playerStepLaneShiftFall.c), JP: the landing restores the saved
// active byte, with no speed or timer setup; PlayerSetAnimIfChanged inline.
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *PLAYER_EXT;
extern unsigned short SPAWN_AXIS_SWAP;

extern void AnimAdvance(TObj *);
extern short probeFlaggedSegmentB(int, int, int);

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

void func_8010834C(TObj *o)
{
    unsigned short f;
    short r;
    switch (o->state) {
    case 0:
        (*(unsigned char*)&PLAYER_EXT->w08) = o->active;
        f = o->animFrame;
        o->velX = 0x5a;
        o->velY = -0x200;
        o->active = 2;
        o->d8c = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        (*(unsigned char*)((char*)&o->wac + 1)) = 0;
        o->b69 = 0;
        (*(signed char*)&o->b0f) = -20;
        (*(unsigned char*)((char*)&o->da0 + 2)) = 2;
        o->animFrame = f & 1;
        demoSetAnim(o, 0x15);
        o->state++;
    case 1:
        AnimAdvance(o);
        if (SPAWN_AXIS_SWAP & 1) {
            o->d->p.whole = o->d->p.whole - 5;
        } else {
            o->d->p.whole = o->d->p.whole + 5;
        }
        o->y.raw += o->velY << 8;
        o->velX -= 5;
        o->velY += 0x10;
        if (o->velX == 0) {
            (*(signed char*)&o->b0f) = -8;
            demoSetAnim(o, 0x16);
            o->d->p.whole = (short)(o->d->p.whole / 90) * 90;
            {
                int w = 2;
                unsigned char act = (*(unsigned char*)&PLAYER_EXT->w08);
                o->b9c = w;
                o->active = act;
                o->wb2 = 0;
                PLAYER_EXT->timer = 0;
                o->b04 = 1;
                o->step = w;
                o->state = 3;
            }
        } else {
            r = probeFlaggedSegmentB(o->h->p.whole, o->y.p.whole,
                             (short)(o->d->p.whole + ((SPAWN_AXIS_SWAP & 1) ? -5 : 5)));
            if (r != 0) {
                (*(unsigned char*)&o->waa) = 1;
                if (r == 1) {
                    o->d->p.whole += (SPAWN_AXIS_SWAP & 1) ? -5 : 5;
                    o->d->p.whole = (short)(o->d->p.whole / 90) * 90;
                    (*(unsigned char*)((char*)&o->da0 + 2)) = 3;
                    o->wb2 = 0;
                    o->step = 0x15;
                    o->state = 0;
                }
            }
        }
        break;
    }
}
#endif
