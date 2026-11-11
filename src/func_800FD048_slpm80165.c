// VERSION SLPM_80165 x00 800FD048 288
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerBiteEntryPullObject (src/x014/playerBiteEntryPullObject.c): also sets the angle from the
// slope and the mirrored angle of the bitten object; PlayerSetAnimIfChanged and playerSetAngleFromSlope inline.
#include "TOBJ.H"
extern TObj *BITTEN_OBJECT;

extern void spawnImpactEffect(int, int, int, int);


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

static __inline__ void demoSetAngleFromSlope(TObj *o)
{
    short v = o->wb0;
    if (v < 0) o->wb6 = (short)((v << 2) + 0x100) & 0xff;
    else if (v > 0) o->wb6 = (short)(v << 2) & 0xff;
    else o->wb6 = 0;
}

void playerBiteEntryPullObject(TObj *o)
{
    TObj *p;
    char pad[0];
    o->timer = 0;
    demoSetAnim(o, 0xd);
    spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    *(char *)((char *)o + 0xac) = 3;
    o->b9c = 0;
    p = BITTEN_OBJECT;
    o->velX = 0;
    o->velY = 0;
    p->h->p.whole = o->h->p.whole;
    *(short *)((char *)p + 0x16) = o->y.p.whole + *(short *)((char *)p + 0x70);
    demoSetAngleFromSlope(o);
    BITTEN_OBJECT->d8c = (0x100 - o->d8c) & 0xff;
    o->state = 2;
}
#endif
