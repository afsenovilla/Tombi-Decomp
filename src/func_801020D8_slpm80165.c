// VERSION SLPM_80165 x00 801020D8 760
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepRideSlopeContact (src/x001/playerStepRideSlopeContact.c): the +0xE3 byte is +0xCF,
// no fall lean (the first exit clears +0x8C); PlayerSetAnimIfChanged and playerSetAngleFromSlope inline.
#include "TOBJ.H"

typedef struct { TObj o; unsigned char pc0[6]; unsigned char c6, c7; unsigned char pc8[0x07]; unsigned char cf; } TX;
#define X(o) ((TX *)(o))
extern TObj *PLAYER_EXT;
extern void AnimAdvance(TObj *);


extern void playerSetFallLean(TObj *);
extern void playerCheckGroundAndFall(TObj *);


extern struct DemoAnimExt { char pad[0x2c]; unsigned short timer, frame; } *demoAnimExt asm("PLAYER_EXT");
extern void demoAnimReset(void *) asm("func_800EDA04");
extern void demoAnimRestart(void *) asm("func_80020554");

static __inline__ void demoSetAngleFromSlope(void *o)
{
    short v = *(short *)((char *)o + 0xb0);
    if (v < 0) *(short *)((char *)o + 0xb6) = (short)((v << 2) + 0x100) & 0xff;
    else if (v > 0) *(short *)((char *)o + 0xb6) = (short)(v << 2) & 0xff;
    else *(short *)((char *)o + 0xb6) = 0;
}

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

static __inline__ short inc(unsigned char x)
{
    if (x < 0xff) return x + 1;
    return x;
}

void playerStepRideSlopeContact(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER_EXT->b0b = o->animFrame;
        if (o->animFrame &= 1) {
            if (o->wb2 > 0) {
                o->wb2 = -o->wb2;
            }
        } else {
            if (o->wb2 < 0) {
                o->wb2 = -o->wb2;
            }
        }
        *(unsigned char *)&o->wac = 0;
        o->b9c = 0;
        o->b6b = 0;
        X(o)->c7 = 1;
        o->b9d = 0;
        X(o)->c6 = 0;
        X(o)->cf = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0x100;
        PLAYER_EXT->active = 0;
        o->ba4 = 1;
        if ((o->bbe & 1) != o->animFrame) {
            demoSetAnim(o, 8);
        } else {
            demoSetAnim(o, 0x11);
        }
        o->state++;
    case 1:
        o->b6b = inc(o->b6b);
        AnimAdvance(o);
        demoSetAngleFromSlope(o);
        o->y.raw += o->velY << 8;
        o->velY += 8;
        switch (PLAYER_EXT->animTimer) {
        case 8:
            o->d8c = (o->animFrame & 1) ? 0x20 : 0xe0;
            break;
        case 0x11:
            o->d8c = (o->animFrame & 1) ? 0x100 - (o->b6b >> 2) : o->b6b >> 2;
            break;
        }
        if (o->bbe == 0) {
            if (o->animFrame != (o->bbe & 1)) {
                o->wb2 = 0;
            }
            o->ba4 = 0;
            if (PLAYER_EXT->active == 0) {
                *(unsigned char *)&o->wac = 1;
                o->step = 2;
                o->d8c = 0;
                o->state = 3;
            } else {
                o->step = 4;
                o->state = 3;
            }
        }
        if (o->bbe & 2) {
            o->step = 0x13;
            o->state = 0;
        }
        playerCheckGroundAndFall(o);
        break;
    }
}
#endif
