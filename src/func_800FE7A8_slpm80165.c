// VERSION SLPM_80165 x00 800FE7A8 1120
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerBittenJumpType04 (src/playerBittenJumpType04_beta980203.c, the beta form: the bitten
// object is released in step 3): fixed button bits 0x60 instead of the two configurable masks; setFacingFromPad
// (d-pad 0x2000 / 0x8000) and PlayerSetAnimIfChanged inline.
#include "spad.h"
#include "TOBJ.H"
extern TObj *BITTEN_OBJECT;
extern TObj *PLAYER_EXT;

extern void ObjGravityStep(TObj *o);
extern void ObjAddVelY7E(TObj *o);
extern int AnimAdvance(TObj *o);
extern void TileCollideAt(TObj *o, int x, int y);
extern int ObjCheckHeadCollision(TObj *o);
extern short probeFloorPlaneAt(TObj *o, int x, int y);
extern short ObjTileCollide(TObj *o, int a, int b);

extern void SfxPlay2(int a, int b);
extern TObj *allocObjectLayer1(void);
extern void playerPlaceBittenObject(TObj *o);
extern void playerBittenJumpAirMove(TObj *o);


extern struct DemoAnimExt { char pad[0x2c]; unsigned short timer, frame; } *demoAnimExt asm("PLAYER_EXT");
extern void demoAnimReset(void *) asm("func_800EDA04");
extern void demoAnimRestart(void *) asm("func_80020554");

extern volatile unsigned short demoPadPressed asm("PAD_PRESSED");
static __inline__ void demoSetFacingFromPad(void *o)
{
    unsigned short v = *(unsigned short *)((char *)o + 0x2e) & 1;
    volatile unsigned short *p = &demoPadPressed;
    *(unsigned short *)((char *)o + 0x2e) = v;
    if (*p & 0x2000) {
        *(short *)((char *)o + 0x2e) = 0;
    } else {
        *(unsigned short *)((char *)o + 0x2e) = (*p & 0x8000) ? 1 : (v | 2);
    }
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

void func_800FE7A8(TObj *o)
{
    TObj *p;
    switch (o->substep) {
    case 0: {
        register TObj *b asm("$4") = BITTEN_OBJECT;
        b->d8c = o->d88 - 0xc0;
        b->h->p.whole = o->h->p.whole;
        b->y.p.whole = o->y.p.whole + b->box2;
    }
        demoSetFacingFromPad(o);
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->d84 = 0;
            o->velY = 0;
            o->substep = 1;
        }
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            o->d84 = 0;
            o->velY = 0;
            o->substep = 1;
        }
        if ((SPAD.s1fc & 0x60)) {
            PLAYER_EXT->animTimer = 0xf;
            o->velY = 0;
            o->substep = 2;
            break;
        }
        break;
    case 1:
        demoSetFacingFromPad(o);
        p = BITTEN_OBJECT;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        p->d8c = o->d88 - 0xc0;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 != 0 || probeFloorPlaneAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x40)) != 0 ||
            (SPAD.s1fc & 0x60) || ObjTileCollide(o, 4, 0) != 0) {
            o->b9c = 0;
            o->velY = 0;
            o->substep = 2;
            break;
        }
        break;
    case 2:
        demoSetAnim(o, 0xf);
        if (AnimAdvance(o)) {
            TObj *q;
            SfxPlay2(0x22, 0x23);
            q = allocObjectLayer1();
            if (q) {
                TObj *r;
                q->active = 1;
                q->type = 2;
                r = BITTEN_OBJECT;
                q->animFrame = (o->animFrame & 1) | 2;
                q->a.p.whole = o->a.p.whole;
                q->y.p.whole = o->y.p.whole + r->box2;
                q->b.p.whole = o->b.p.whole;
                q->b6b = r->b6b;
                q->b1d = BITTEN_OBJECT->b1d;
                q->subtype = BITTEN_OBJECT->subtype;
                q->b0c = BITTEN_OBJECT->b0c;
            }
            o->velY = 0;
            o->substep = 3;
        } else {
            playerPlaceBittenObject(o);
        }
        break;
    case 3:
        BITTEN_OBJECT->b04 = 3;
        demoSetAnim(o, 0x1d);
        o->d8c = 0x200;
        o->wb2 = 0x100;
        o->b9c = 2;
        *(unsigned char *)&o->wac = 1;
        o->velY = 0;
        o->substep = 4;
        break;
    case 4:
        playerBittenJumpAirMove(o);
        break;
    }
}
#endif
