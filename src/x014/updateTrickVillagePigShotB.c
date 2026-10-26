// FUNC 801244d0 888 X014
// MATCHING 801244d0 888
// VERSION SLPS_01144 x14 801217A8 888
#include "camera.h"
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x14/nonmatchings/x14", func_801217A8_slps01144);
#else
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern void *D_80129FF0;
extern short D_8007A3F0[], D_8007A5F0[];
extern void FUN_8001fe6c(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void spawnRisingEffect(TObj *, int);
extern int FUN_8001f9e0(void);
extern short probeCollisionAtDepthB(TObj *, int, int);
extern void freeObjectLayer2(TObj *);

static __inline__ void setup(TObj *p)
{
    p->d3c = ((int*)&SPAD.i2dc)[0];
    p->wac = 0;
    p->anim = D_80129FF0;
    FUN_8001fe6c(p);
}

static __inline__ short out(TObj *o)
{
    if (o->h->p.whole < CAMERA_SCROLL.minX - 0xa0) return 1;
    if (CAMERA_SCROLL.maxX + 0xa0 < o->h->p.whole) return 1;
    if (CAMERA_SCROLL.maxY + 0x80 < o->y.p.whole) return 1;
    return o->y.p.whole < CAMERA_SCROLL.minY - 0x80;
}

void updateTrickVillagePigShotB(TObj *o)
{
    unsigned char t = o->b04;
    int r;
    short k;
    int a, v, x, y;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box3 = 0x10;
        *(signed char *)&o->b0f = -9;
        o->b0a = 2;
        o->box2 = 8;
        o->d84 = 0;
        o->d88 = 0;
        o->w1e = 1;
        o->animFrame = 1;
        o->b0d = 0;
        setup(o);
        break;
    case 1:
        if (GVARS.b42 == 1) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->velH = 0x80;
            o->velX = 8;
            o->step++;
            if (GAME.selectedSection != 7) spawnRisingEffect(o, 1);
            o->timer = 6;
            break;
        case 1:
            if (--o->timer == -1) {
                o->timer = (FUN_8001f9e0() & 3) + 6;
                r = FUN_8001f9e0() & 3;
                k = r;
                if (r == 0) k = 2;
                if (GAME.selectedSection != 7) spawnRisingEffect(o, k);
            }
            a = o->d8c;
            v = o->velH;
            x = v * D_8007A5F0[a];
            y = v * D_8007A3F0[a];
            o->h->raw += ((x << 4) >> 16) << 8;
            o->y.raw += ((y << 4) >> 16) << 8;
            o->velH += o->velX;
            if (o->velH > 0x300) o->velH = 0x300;
            if (o->velH > 0x140) o->velX = 0x10;
            if (probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole)) {
                o->active = 2;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            } else if (out(o)) {
                o->active = 2;
                o->b04 = 3;
            }
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        spawnRisingEffect(o, 0);
        o->b04++;
        ObjCullRegister(o);
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
