// VERSION SLPM_80165 x00 80101548 900
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepSlideDownSurface (src/playerStepSlideDownSurface_beta980203.c): field +0xE3 at +0xCF, the
// move by angle and speed takes them as arguments and the drift is added here (both directions), no fall lean, no
// timer on the step 4 landing.
#include "spad.h"
typedef struct {
    unsigned char b00[5];
    unsigned char step;
    unsigned char state;
    unsigned char b07[13];
    int y;
    unsigned char b18[8];
    short timer;
    unsigned char b22[12];
    unsigned short animFrame;
    unsigned char b30[0x39];
    unsigned char b69;
    unsigned char b6a;
    unsigned char b6b;
    unsigned char b6c[0x10];
    short velX;
    short velY;
    unsigned char b80[0xc];
    int d8c;
    unsigned char b90[0xc];
    unsigned char b9c, b9d, b9e, b9f;
    unsigned char ba0[4];
    unsigned char ba4;
    unsigned char ba5[7];
    unsigned char bac, bad;
    unsigned char bae[2];
    short wb0;
    unsigned short wb2;
    unsigned char bb4[10];
    unsigned char bbe;
    unsigned char bbf[7];
    unsigned char bc6, bc7;
    unsigned char bc8[0xf];
    unsigned char be3; /* 0xd7 (PAL 0xe3) */
} P;
typedef struct { unsigned char b0; unsigned char p1[7]; unsigned char b8; unsigned char p9[0x17]; short timer; } PL;

extern PL *PLAYER_EXT;
extern unsigned char SLOPE_TILT_TABLE[];

extern void AnimAdvance(P *);

extern void playerAccelerateDownhill(P *);
extern void playerMoveByAngleSpeed(P *);
extern void ObjTileCollide(P *, int, int);
extern void playerSetFallLean(P *);
extern void startPadVibration(int, int, int, int);


extern struct DemoAnimExt { char pad[0x2c]; unsigned short timer, frame; } *demoAnimExt asm("PLAYER_EXT");
extern void demoAnimReset(void *) asm("func_800EDA04");
extern void demoAnimRestart(void *) asm("func_80020554");

extern void dzMoveAt(void *, int, int) asm("func_800202FC");
extern void dzStick(void *) asm("func_800203CC");
extern short dzDriftH asm("D_8009A7F4"), dzDriftY asm("D_8009A7F6");
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

void func_80101548(P *o)
{
    switch (o->state) {
    case 0:
        o->bac = 0;
        PLAYER_EXT->b8 = o->wb2 >> 15;
        o->ba4 = 1;
        o->bc7 = 1;
        o->b9c = 0;
        o->b6b = 0;
        o->b9d = 0;
        o->bc6 = 0;
        *((unsigned char *)o + 0xcf) = 0;
        o->wb2 = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        o->animFrame = o->bbe & 1;
        PLAYER_EXT->b0 = 0;
        demoSetAnim(o, 0x11);
        o->state++;
    case 1:
        { unsigned char t = o->b6b; unsigned char r; if (t < 0xff) r = t + 1; else r = t; o->b6b = r; }
        AnimAdvance(o);
        demoSetAngleFromSlope(o);
        o->y += 0x80000;
        playerAccelerateDownhill(o);
        if (o->animFrame & 1) {
            dzMoveAt(o, *(short *)((char *)o + 0xb6), *(short *)((char *)o + 0xb2));
            **(int **)((char *)o + 0x40) += dzDriftH << 8;
            *(int *)((char *)o + 0x14) += dzDriftY << 8;
            dzStick(o);
            o->d8c = 0x100 - (o->b6b >> 2);
        } else {
            dzMoveAt(o, *(short *)((char *)o + 0xb6), *(short *)((char *)o + 0xb2));
            **(int **)((char *)o + 0x40) += dzDriftH << 8;
            *(int *)((char *)o + 0x14) += dzDriftY << 8;
            dzStick(o);
            o->d8c = o->b6b >> 2;
        }
        if (o->bbe == 0) {
            if (o->animFrame != PLAYER_EXT->b8) {
                o->wb2 = 0;
            }
            o->ba4 = 0;
            o->step = 0;
            o->state = 0;
        }
        if (o->bbe & 2) {
            o->step = 0x13;
            o->state = 0;
        }
        o->bad = 0;
        if (o->b69) {
            o->b69 = 0;
            o->velY = 0;
        } else {
            ObjTileCollide(o, 0, 0);
            if ((o->b69 | o->b9c | o->b9e | o->b9f | o->bbe) == 0) {
                o->bad = 1;
                o->velY += 0x223;
                o->y += o->velY << 8;
                PLAYER_EXT->timer = 0x21;
                if (o->velY >= 0x447) {
                    o->bad = 0;
                    o->velY = 0;
                    o->b9c = 2;
                    o->bac = 1;
                    if (o->step == 3) {
                        o->step = 4;
                        o->state = 1;
                    } else {
                        *(int *)((char *)o + 0x8c) = 0;
                        o->step = 2;
                        o->state = 3;
                    }
                }
            } else {
                o->velY = 0;
                o->b9c = 0;
            }
        }
        break;
    }
}
#endif
