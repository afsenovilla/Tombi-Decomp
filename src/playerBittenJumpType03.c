// FUNC 80105bcc 2308 X000
// MATCHING 80105bcc 2308
#include "spad.h"
#include "gcd.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *BITTEN_OBJECT;
extern TObj *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern unsigned char D_8009D2B1;
extern unsigned char SLOPE_TILT_TABLE[];
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
extern void playerPlaceBittenObject(TObj *o);
extern void playerSetFallLean(TObj *o);
extern TObj *playerProbeGrab(TObj *, int);
extern void playerEnterGrabStep(TObj *, int);
extern void playerTryAttack(TObj *, int);

#define LAND() \
    o->d8c = SLOPE_TILT_TABLE[o->wb0]; \
    (*(unsigned char*)&PLAYER_EXT->w08) = 0; \
    o->b9c = 0; \
    o->ba7 = 0; \
    (*(unsigned char*)&o->wac) = 0; \
    o->d84 = 0; \
    o->wb2 = 0; \
    o->velY = 0; \
    if (BITTEN_OBJECT->type == 0x1c) { \
        switch (D_8009D2B1) { \
        case 1: \
            (*(unsigned char*)&o->wac) = 0; \
            o->step = 0x2a; \
            o->state = 0; \
            (*(unsigned char*)((char*)&o->waa + 1)) |= 0x80; \
            return; \
        case 2: \
            (*(unsigned char*)&o->wac) = 0; \
            o->step = 0x2b; \
            o->state = 0; \
            (*(unsigned char*)((char*)&o->waa + 1)) |= 0x80; \
            return; \
        default: \
            (*(unsigned char*)&o->wac) = 0; \
        } \
    } \
    o->step = 0; \
    o->state = 0;

#define TURN() \
    (*(unsigned char*)&PLAYER_EXT->w08) = 0; \
    (*(unsigned char*)((char*)&o->wac + 1)) = 0; \
    o->wb2 = 0; \
    PLAYER_EXT->timer = 0xe; \
    if (BITTEN_OBJECT->type == 0x1c && (t = D_8009D2B1) < 3 && t != 0) \
        (*(unsigned char*)((char*)&o->waa + 1)) |= 0x80; \
    if ((GAME.area00_eventControl & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4)) \
        o->ba7 = 1; \
    playerSetFallLean(o); \
    o->step = 2; \
    o->state = 3;

void playerBittenJumpType03(TObj *o)
{
    TObj *p;
    int t;

    switch (o->substep) {
    case 0:
        BITTEN_OBJECT->animFrame = o->animFrame & 1;
        BITTEN_OBJECT->d->p.whole = o->d->p.whole;
        BITTEN_OBJECT->step = 1;
        BITTEN_OBJECT->state = 6;
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        playerAirAccelerateX(o);
        if (BITTEN_OBJECT->type != 0x1c)
            o->h->raw += o->velX << 8;
        p = BITTEN_OBJECT;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        setFacingFromPad(o);
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->velY > 0) {
            o->d84 = 0;
            o->b9c = 2;
            o->velY = 0;
            o->substep = 1;
        }
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        if (ObjCheckHeadCollision(o)) {
            o->d84 = 0;
            o->b9c = 2;
            o->velY = 0;
            o->substep = 1;
        }
        if ((SPAD.s1fc & SPAD.s3c8) || (SPAD.s1fc & SPAD.s3c6)) {
            PLAYER_EXT->animTimer = 0xf;
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
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        playerAirAccelerateX(o);
        if (BITTEN_OBJECT->type != 0x1c)
            o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 != 0 || probeFloorPlaneAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x30)) != 0 ||
            (SPAD.s1fc & SPAD.s3c8) || (SPAD.s1fc & SPAD.s3c6) || ObjTileCollide(o, 4, 0) != 0) {
            o->velY = 0;
            o->substep = 2;
        }
        break;
    case 2:
        PlayerSetAnimIfChanged(o, 0xf);
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            o->velY = 0;
            PLAYER_EXT->animTimer = 0x1d;
            o->substep = 3;
        } else {
            playerPlaceBittenObject(o);
        }
        break;
    case 3:
        switch (BITTEN_OBJECT->type) {
        case 0x1f:
            BITTEN_OBJECT->b04 = 2;
            BITTEN_OBJECT->step = 1;
            break;
        case 3:
        case 0x2b:
            BITTEN_OBJECT->b04 = 2;
            BITTEN_OBJECT->step = 2;
            break;
        case 0x12:
            BITTEN_OBJECT->b04 = 2;
            BITTEN_OBJECT->step = 3;
            break;
        case 0x1c:
            (*(unsigned char*)((char*)&o->waa + 1)) = 0;
            switch (BITTEN_OBJECT->subtype) {
            case 0 ... 2:
                D_8009D2B1 = BITTEN_OBJECT->subtype;
                break;
            case 4:
                GCD.bcf06 = 1;
                break;
            }
            BITTEN_OBJECT->step = 3;
            BITTEN_OBJECT->state = 0;
            goto anim;
        default:
            t = (int)BITTEN_OBJECT;
            ((TObj *)t)->b04 = 3;
            BITTEN_OBJECT->step = 0;
            break;
        }
        BITTEN_OBJECT->state = 0;
    anim:
        ObjSetAnimFromTable(o);
        AnimLoadDuration(o);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
        o->d8c = 0x200;
        o->wb2 = 0x100;
        o->b9c = 2;
        (*(unsigned char*)&o->wac) = 1;
        o->velY = 0;
        o->substep = 4;
        break;
    case 4:
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        setFacingFromPad(o);
        ObjMotionStep(o);
        if ((*(unsigned char*)&o->wac) == 2) {
            BITTEN_OBJECT = (TObj *)S32(o, 0xe4);
            (*(unsigned char*)&PLAYER_EXT->w08) = 0;
            PLAYER_EXT->timer = 0;
            o->ba7 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            (*(unsigned char*)((char*)&o->waa + 1)) &= 0x7f;
            break;
        }
        if (o->b69 == 1) {
            LAND();
            break;
        }
        if (ObjTileCollide(o, 0, 0)) {
            LAND();
            break;
        }
        if (o->animFrame & 1) {
            o->d8c += 0x10;
            if (o->d8c >= 0x300) {
                o->d8c = 0x300;
                TURN();
            }
        } else {
            o->d8c -= 0x10;
            if (o->d8c <= 0x100) {
                o->d8c = 0x100;
                TURN();
            }
        }
        if ((*(unsigned char*)&o->wac) < 2) {
            GRABBED_OBJECT = playerProbeGrab(o, 0);
            if (GRABBED_OBJECT) {
                (*(unsigned char*)&PLAYER_EXT->w08) = 0;
                PLAYER_EXT->timer = 0;
                (*(unsigned char*)&o->wac) = 0;
                o->ba7 = 0;
                o->b9c = 0;
                o->wb2 = 0;
                playerEnterGrabStep(o, GRABBED_OBJECT == (TObj *)1);
            }
        }
        if (o->step == 0xf && o->b9e == 0)
            playerTryAttack(o, 1);
        break;
    }
}
