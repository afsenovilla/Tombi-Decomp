// FUNC 80104f88 1804 X008
// MATCHING 80104f88 1804
#include "spad.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
extern TObj *BITTEN_OBJECT;
extern TObj *PLAYER_EXT;
extern void playerAirAccelerateX(TObj *o);
extern void setFacingFromPad(TObj *o);
extern void ObjGravityStep(TObj *o);
extern void ObjAddVelY7E(TObj *o);
extern int AnimAdvance(TObj *o);
extern void TileCollideAt(TObj *o, int x, int y);
extern int ObjCheckHeadCollision(TObj *o);
extern void ObjMotionStep(TObj *o);
extern short probeFloorPlaneAt(TObj *o, int x, int y);
extern short ObjTileCollide(TObj *o, int a, int b);
extern void ObjSetAnimFromTable(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern void PlayerSetAnimIfChanged(TObj *o, int n);
extern void SfxPlay2(int a, int b);
extern TObj *allocObjectLayer1(void);
extern void playerPlaceBittenObject(TObj *o);
extern void playerBittenJumpAirMove(TObj *o);

void playerBittenJumpGeneric(TObj *o)
{
    TObj *p;
    TObj *q;
    unsigned short t;

    switch (o->substep) {
    case 0:
        BITTEN_OBJECT->animFrame = o->animFrame & 1;
        BITTEN_OBJECT->d8c = o->d88 - 0xc0;
        playerAirAccelerateX(o);
        o->h->raw += o->velX << 8;
        BITTEN_OBJECT->h->p.whole = o->h->p.whole;
        BITTEN_OBJECT->y.p.whole = o->y.p.whole + BITTEN_OBJECT->box2;
        setFacingFromPad(o);
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
        if ((SPAD.s1fc & SPAD.s3c8) || (SPAD.s1fc & SPAD.s3c6)) {
            if (*(volatile unsigned short *)&GV6.s70 & 0x40) {
                PLAYER_EXT->animTimer = 0xf;
            } else {
                PLAYER_EXT->animTimer = 0xe;
            }
            o->b9c = 0;
            o->velY = 0;
            o->substep = 2;
        }
        break;
    case 1:
        setFacingFromPad(o);
        p = BITTEN_OBJECT;
        p->animFrame = o->animFrame & 1;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        p->d8c = o->d88 - 0xc0;
        o->h->raw += GVARS.driftH << 8;
        o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
        ObjMotionStep(o);
        AnimAdvance(o);
        if (o->b69 != 0 || probeFloorPlaneAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x30)) != 0 ||
            (SPAD.s1fc & SPAD.s3c8) || (SPAD.s1fc & SPAD.s3c6) || ObjTileCollide(o, 4, 0) != 0) {
            if (*(volatile unsigned short *)&GV6.s70 & 0x40) {
                PLAYER_EXT->animTimer = 0xf;
            } else {
                PLAYER_EXT->animTimer = 0xe;
            }
            o->b9c = 0;
            o->velY = 0;
            o->substep = 2;
        }
        break;
    case 2:
        if (PLAYER_EXT->animFrame != PLAYER_EXT->animTimer) {
            BITTEN_OBJECT->state = 7;
            ObjSetAnimFromTable(o);
            AnimLoadDuration(o);
            PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
        }
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            switch (BITTEN_OBJECT->type) {
            case 0x21:
            case 0x3b: case 0x3c: case 0x3d: case 0x3e:
            case 0x3f: case 0x40: case 0x41: case 0x42:
                break;
            default:
                {TObj *q = allocObjectLayer1();
                if (q) {
                    q->active = 1;
                    q->type = 1;
                    switch (BITTEN_OBJECT->type) {
                    case 0:
                        q->subtype = 3;
                        break;
                    case 2:
                        switch (GAME.selectedArea) {
                        case 0: case 1: case 2:
                            q->subtype = 0;
                            break;
                        case 3:
                            q->subtype = 8;
                            break;
                        }
                        break;
                    case 0x3a:
                        q->subtype = 0xc;
                        break;
                    case 0x1a:
                        q->type = 4;
                        q->subtype = 0;
                        break;
                    case 0x27:
                        q->subtype = 0xd;
                        break;
                    case 8:
                        q->subtype = 1;
                        break;
                    case 0x1e:
                        q->subtype = 9;
                        break;
                    case 0xa:
                        q->subtype = 4;
                        q->b0d = BITTEN_OBJECT->b0d;
                        break;
                    case 0x13:
                        q->subtype = 5;
                        break;
                    case 0x28:
                        q->subtype = 7;
                        break;
                    case 0xb:
                        q->subtype = BITTEN_OBJECT->subtype << 1;
                        break;
                    case 0x38:
                        q->subtype = BITTEN_OBJECT->subtype + 10;
                        break;
                    }
                    t = o->animFrame & 1;
                    q->animFrame = t;
                    if (*(volatile unsigned short *)&GV6.s70 & 0x40) {
                        q->animFrame = t | 2;
                    }
                    q->a.p.whole = o->a.p.whole;
                    q->y.p.whole = o->y.p.whole;
                    q->b.p.whole = o->b.p.whole;
                    {
int c = o->animFrame & 1; Fix16 *h = q->h; int w = h->p.whole; int r;
                        if (c) r = w - 0x10; else r = w + 0x10; h->p.whole = r;
                    }
                }}
                break;
            }
            o->velY = 0;
            PlayerSetAnimIfChanged(o, 0x1d);
            o->d8c = 0x200;
            o->substep = 3;
        } else {
            playerPlaceBittenObject(o);
        }
        break;
    case 3: {
        TObj *p = BITTEN_OBJECT;
        switch (p->type) {
        case 0x13:
            for (q = (TObj *)p->d90; q != 0; q = (TObj *)q->d90) {
                q->b04 = 3;
            }
            for (q = (TObj *)BITTEN_OBJECT->d94; q != 0; q = (TObj *)q->d94) {
                q->b04 = 3;
            }
            break;
        case 0x21:
            p->b04 = 2;
            BITTEN_OBJECT->step = 2;
            BITTEN_OBJECT->state = 0;
            break;
        case 0x3b: case 0x3c: case 0x3d: case 0x3e:
        case 0x3f: case 0x40: case 0x41: case 0x42:
            BITTEN_OBJECT->b04 = 2;
            BITTEN_OBJECT->step = 3;
            BITTEN_OBJECT->state = 0;
            {TObj *q = BITTEN_OBJECT; unsigned short t;
            t = o->animFrame & 1;
            q->animFrame = t;
            if (*(volatile unsigned short *)&GV6.s70 & 0x40) {
                q->animFrame = t | 2;
            }}
            break;
        default:
            BITTEN_OBJECT->b04 = 3;
            break;
        }
        o->wb2 = 0x100;
        o->b9c = 2;
        *(unsigned char *)&o->wac = 1;
        o->velY = 0;
        o->substep = 4;
        break; }
    case 4:
        playerBittenJumpAirMove(o);
        break;
    }
}
