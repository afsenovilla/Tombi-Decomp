// FUNC 80101d74 924 X017
// MATCHING 80101d74 924
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *PLAYER_EXT;
extern unsigned char *BITTEN_OBJECT;
extern unsigned char TALK_POSE;
extern unsigned char D_1f8003ce;
extern unsigned char D_801152E8[];
extern char D_80010748[];
extern void playerEventStepLandIdle(TObj *);
extern void playerEventStepLaneJumpBack(TObj *);
extern void FUN_801231f4(TObj *);
extern void playerEventStepDropIn(TObj *);
extern void playerEventStepRiseFadeOut(TObj *);
extern void playerEventStepDepthShift(TObj *);
extern void playerEventStepJumpIn(TObj *);
extern void playerEventStepWalkIntoDepth(TObj *);
extern void func_8011DD48(TObj *);
extern void playerEventStepRiseUp(TObj *);
extern void playerAnimateTalkPose(TObj *);
extern void playerEventWalk(TObj *);
extern void AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern short ObjTileCollide(TObj *, int, int);
extern void objCollideSideWalls(TObj *);
void playerEventModeDispatch(TObj *o)
{
    short c;
    int v;
    switch (o->step) {
    case 1:
        if (--o->timer > 0) goto common;
        if ((*(int*)&GAME.selectedArea) == 0x40000) {
            (*(signed char*)&o->b0f) = 8;
        } else if ((*(int*)&GAME.selectedArea) == 0x50000) {
            (*(signed char*)&o->b0f) = -5;
        } else {
            (*(signed char*)&o->b0f) = -8;
        }
        v = D_801152E8[o->wb0];
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        o->timer = 0;
        o->d8c = v;
        ((unsigned char*)&GVARS.b3a)[0] = 1;
        break;
    case 2:
        playerEventStepLaneJumpBack(o);
        break;
    case 3:
        FUN_801231f4(o);
        break;
    case 4:
        playerEventStepDropIn(o);
        break;
    case 5:
        playerEventStepRiseFadeOut(o);
        break;
    case 0:
    case 6:
        playerEventStepLandIdle(o);
        break;
    case 7:
        playerEventStepDepthShift(o);
        break;
    case 8:
        playerEventStepJumpIn(o);
        break;
    case 9:
        playerEventStepWalkIntoDepth(o);
        break;
    case 10:
        if (--o->timer <= 0) goto anim;
    common:
        o->animFrame &= 1;
        if ((*(unsigned char*)&o->wac) >= 2) {
            BITTEN_OBJECT = (*(unsigned char **)&PLX.p611c);
            BITTEN_OBJECT[4] = 2;
            BITTEN_OBJECT[5] = 2;
            BITTEN_OBJECT[6] = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        PLAYER_EXT->active = 0;
        v = 0x128;
        if (o->animFrame & 1) {
            v = -0x128;
        }
        o->wb2 = v;
        playerEventWalk(o);
        if (o->b69 != 0) {
            o->y.p.whole += 2;
        }
        ObjTileCollide(o, 0, 0);
        o->d8c = D_801152E8[o->wb0];
        break;
    case 11:
        func_8011DD48(o);
        break;
    case 12:
        playerEventStepRiseUp(o);
        break;
    case 0x40:
        o->visible = 0;
        break;
    case 0x61:
        (*(unsigned char*)((char*)&o->da0 + 2)) = 0;
        (*(unsigned char*)((char*)&o->da0 + 3)) = 0;
        AnimAdvance(o);
        o->d8c = 0;
        break;
    case 0x62:
        o->anim = D_80010748;
        AnimLoadDuration(o);
        o->step++;
    case 0x63:
        o->anim = D_80010748;
        AnimLoadDuration(o);
        o->d8c = 0;
        break;
    case 0x64:
        o->d8c = 0;
    case 0x65:
        if (TALK_POSE != 0) {
            playerAnimateTalkPose(o);
        } else if (D_1f8003ce == 0 || *(unsigned short *)o->anim != 0) {
        anim:
            AnimAdvance(o);
        }
        break;
    }
    c = 0;
    (*(unsigned char*)&o->wa8) = 0;
    o->ba6 = 0;
    if (o->b9e == 0 && o->b04 == 5) {
        c = o->step == 0x40;
        if (o->step == 0x41) c++;
        if (o->step == 0x65) c++;
    }
    if (c == 0) objCollideSideWalls(o);
}
