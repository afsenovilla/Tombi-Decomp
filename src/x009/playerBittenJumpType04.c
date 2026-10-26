// FUNC 80105694 928 X009
// MATCHING 80105694 928
#include "spad.h"
#include "TOBJ.H"
extern TObj *BITTEN_OBJECT;
extern TObj *PLAYER_EXT;
extern void setFacingFromPad(TObj *o);
extern void ObjGravityStep(TObj *o);
extern void ObjAddVelY7E(TObj *o);
extern int AnimAdvance(TObj *o);
extern void TileCollideAt(TObj *o, int x, int y);
extern int ObjCheckHeadCollision(TObj *o);
extern short probeFloorPlaneAt(TObj *o, int x, int y);
extern short ObjTileCollide(TObj *o, int a, int b);
extern void PlayerSetAnimIfChanged(TObj *o, int n);
extern void SfxPlay2(int a, int b);
extern TObj *allocObjectLayer1(void);
extern void playerPlaceBittenObject(TObj *o);
extern void playerBittenJumpAirMove(TObj *o);

void playerBittenJumpType04(TObj *o)
{
    TObj *p;
    switch (o->substep) {
    case 0:
        p = BITTEN_OBJECT;
        p->d8c = o->d88 - 0xc0;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
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
            PLAYER_EXT->animTimer = 0xf;
            o->velY = 0;
            o->substep = 2;
            break;
        }
        break;
    case 1:
        setFacingFromPad(o);
        p = BITTEN_OBJECT;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        p->d8c = o->d88 - 0xc0;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 != 0 || probeFloorPlaneAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x40)) != 0 ||
            (SPAD.s1fc & SPAD.s3c8) || (SPAD.s1fc & SPAD.s3c6) || ObjTileCollide(o, 4, 0) != 0) {
            o->b9c = 0;
            o->velY = 0;
            o->substep = 2;
            break;
        }
        break;
    case 2:
        PlayerSetAnimIfChanged(o, 0xf);
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
            BITTEN_OBJECT->b04 = 3;
            *(unsigned char *)&o->wac = 1;
            o->velY = 0;
            o->substep = 3;
        } else {
            playerPlaceBittenObject(o);
        }
        break;
    case 3:
        PlayerSetAnimIfChanged(o, 0x1d);
        o->d8c = 0x200;
        o->wb2 = 0x100;
        o->b9c = 2;
        o->velY = 0;
        o->substep = 4;
        break;
    case 4:
        playerBittenJumpAirMove(o);
        break;
    }
}
