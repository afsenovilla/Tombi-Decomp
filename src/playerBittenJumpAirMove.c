// FUNC 80104a04 724 X000
// MATCHING 80104a04 724
// VERSION SLPS_01144 x00 801046C0 756
// VERSION BETA_980203 x00 8010898C 724
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char p[8]; unsigned char b8; char q[0x20 - 9]; short w20; } G;
extern G *PLAYER_EXT;
extern int BITTEN_OBJECT;
extern int D_8009F0EC;
extern int GRABBED_OBJECT;
extern unsigned char SLOPE_TILT_TABLE[];
void setFacingFromPad(TObj *o);
void ObjMotionStep(TObj *o);
void ObjSetAnimFromTable(TObj *o);
void AnimJump(TObj *o, short n);
short ObjTileCollide(TObj *, int, int);
void playerSetFallLean(TObj *o);
int playerProbeGrab(TObj *o, int);
void playerEnterGrabStep(TObj *o, int);
void playerTryAttack(TObj *o, int);
void func_801046C0(TObj *o)
{
    int v;
    o->h->raw += GVARS.driftH << 8;
    o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
    setFacingFromPad(o);
    ObjMotionStep(o);
    if (*(unsigned char *)&o->wac == 2) {
        BITTEN_OBJECT = *(int *)((char *)o + 0xd4);
        PLAYER_EXT->b8 = 0;
        PLAYER_EXT->w20 = 0;
        o->d8c = 0;
        o->ba7 = 0;
        o->ba5 = 0;
        o->step = 0xe;
        o->state = 0;
        return;
    }
    if (o->b69 == 1) {
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        PLAYER_EXT->b8 = 0;
        goto reset;
    }
    if (ObjTileCollide(o, 0, 0) != 0) {
        PLAYER_EXT->b8 = 0;
        o->d84 = 0;
        PLAYER_EXT->b8 = 0;
    reset:
        o->b9c = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velY = 0;
        *(unsigned char *)&o->wac = 0;
        o->d8c = SLOPE_TILT_TABLE[o->wb0];
        o->step = 0;
        o->state = 0;
        return;
    }
    if (o->animFrame & 1) {
        o->d8c += 0x10;
        if (o->d8c >= 0x300) {
            o->d8c = 0x300;
            PLAYER_EXT->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            PLAYER_EXT->w20 = 0xe;
            if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
                o->ba7 = 1;
            v = 0x10;
            o->step = 2;
            o->state = 3;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1) v = 0xf0;
            o->d88 = v;
            o->d8c = 0;
        }
    } else {
        o->d8c -= 0x10;
        if (o->d8c <= 0x100) {
            o->d8c = 0x100;
            PLAYER_EXT->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            PLAYER_EXT->w20 = 0xe;
            if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
                o->ba7 = 1;
            v = 0x10;
            o->step = 2;
            o->state = 3;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1) v = 0xf0;
            o->d88 = v;
            o->d8c = 0;
        }
    }
    if (*(unsigned char *)&o->wac < 2) {
        if ((D_8009F0EC = playerProbeGrab(o, 0)) != 0) {
            PLAYER_EXT->b8 = 0;
            PLAYER_EXT->w20 = 0;
            *(unsigned char *)&o->wac = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->wb2 = 0;
            playerEnterGrabStep(o, GRABBED_OBJECT == 1);
        }
    }
    if (o->step == 0xf && !o->b9e) playerTryAttack(o, 1);
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL x000 80104A04 (playerBittenJumpAirMove.c): PAL C of src/playerBittenJumpAirMove.c, edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "spad.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char p[8]; unsigned char b8; char q[0x20 - 9]; short w20; } G;
extern G *PLAYER_EXT;
extern int BITTEN_OBJECT;
extern int D_8009F0EC;
extern int GRABBED_OBJECT;
extern unsigned char SLOPE_TILT_TABLE[];
void setFacingFromPad(TObj *o);
void ObjMotionStep(TObj *o);
void ObjSetAnimFromTable(TObj *o);
void AnimJump(TObj *o, short n);
short ObjTileCollide(TObj *, int, int);
void playerSetFallLean(TObj *o);
int playerProbeGrab(TObj *o, int);
void playerEnterGrabStep(TObj *o, int);
void playerTryAttack(TObj *o, int);
void playerBittenJumpAirMove(TObj *o)
{
    o->h->raw += GVARS.driftH << 8;
    o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
    setFacingFromPad(o);
    ObjMotionStep(o);
    if (*(unsigned char *)&o->wac == 2) {
        BITTEN_OBJECT = *(int *)((char *)o + 0xd8);
        PLAYER_EXT->b8 = 0;
        PLAYER_EXT->w20 = 0;
        o->d8c = 0;
        o->ba7 = 0;
        o->ba5 = 0;
        o->step = 0xe;
        o->state = 0;
        return;
    }
    if (o->b69 == 1) {
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        PLAYER_EXT->b8 = 0;
        goto reset;
    }
    if (ObjTileCollide(o, 0, 0) != 0) {
        PLAYER_EXT->b8 = 0;
        o->d84 = 0;
        PLAYER_EXT->b8 = 0;
    reset:
        o->b9c = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velY = 0;
        *(unsigned char *)&o->wac = 0;
        o->d8c = SLOPE_TILT_TABLE[o->wb0];
        o->step = 0;
        o->state = 0;
        return;
    }
    if (o->animFrame & 1) {
        o->d8c += 0x10;
        if (o->d8c >= 0x300) {
            o->d8c = 0x300;
            PLAYER_EXT->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            PLAYER_EXT->w20 = 0xe;
            if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 3;
            playerSetFallLean(o);
        }
    } else {
        o->d8c -= 0x10;
        if (o->d8c <= 0x100) {
            o->d8c = 0x100;
            PLAYER_EXT->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            PLAYER_EXT->w20 = 0xe;
            if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 3;
            playerSetFallLean(o);
        }
    }
    if (*(unsigned char *)&o->wac < 2) {
        if ((D_8009F0EC = playerProbeGrab(o, 0)) != 0) {
            PLAYER_EXT->b8 = 0;
            PLAYER_EXT->w20 = 0;
            *(unsigned char *)&o->wac = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->wb2 = 0;
            playerEnterGrabStep(o, GRABBED_OBJECT == 1);
        }
    }
    if (o->step == 0xf && !o->b9e) playerTryAttack(o, 1);
}
#else
#include "spad.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char p[8]; unsigned char b8; char q[0x20 - 9]; short w20; } G;
extern G *PLAYER_EXT;
extern int BITTEN_OBJECT;
extern int D_8009F0EC;
extern int GRABBED_OBJECT;
extern unsigned char SLOPE_TILT_TABLE[];
void setFacingFromPad(TObj *o);
void ObjMotionStep(TObj *o);
void ObjSetAnimFromTable(TObj *o);
void AnimJump(TObj *o, short n);
short ObjTileCollide(TObj *, int, int);
void playerSetFallLean(TObj *o);
int playerProbeGrab(TObj *o, int);
void playerEnterGrabStep(TObj *o, int);
void playerTryAttack(TObj *o, int);
void playerBittenJumpAirMove(TObj *o)
{
    o->h->raw += GVARS.driftH << 8;
    o->y.raw += ((short*)&GVARS.driftY)[0] << 8;
    setFacingFromPad(o);
    ObjMotionStep(o);
    if (*(unsigned char *)&o->wac == 2) {
        BITTEN_OBJECT = *(int *)((char *)o + 0xe4);
        PLAYER_EXT->b8 = 0;
        PLAYER_EXT->w20 = 0;
        o->d8c = 0;
        o->ba7 = 0;
        o->ba5 = 0;
        o->step = 0xe;
        o->state = 0;
        return;
    }
    if (o->b69 == 1) {
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        PLAYER_EXT->b8 = 0;
        goto reset;
    }
    if (ObjTileCollide(o, 0, 0) != 0) {
        PLAYER_EXT->b8 = 0;
        o->d84 = 0;
        PLAYER_EXT->b8 = 0;
    reset:
        o->b9c = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velY = 0;
        *(unsigned char *)&o->wac = 0;
        o->d8c = SLOPE_TILT_TABLE[o->wb0];
        o->step = 0;
        o->state = 0;
        return;
    }
    if (o->animFrame & 1) {
        o->d8c += 0x10;
        if (o->d8c >= 0x300) {
            o->d8c = 0x300;
            PLAYER_EXT->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            PLAYER_EXT->w20 = 0xe;
            if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 3;
            playerSetFallLean(o);
        }
    } else {
        o->d8c -= 0x10;
        if (o->d8c <= 0x100) {
            o->d8c = 0x100;
            PLAYER_EXT->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            PLAYER_EXT->w20 = 0xe;
            if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 3;
            playerSetFallLean(o);
        }
    }
    if (*(unsigned char *)&o->wac < 2) {
        if ((D_8009F0EC = playerProbeGrab(o, 0)) != 0) {
            PLAYER_EXT->b8 = 0;
            PLAYER_EXT->w20 = 0;
            *(unsigned char *)&o->wac = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->wb2 = 0;
            playerEnterGrabStep(o, GRABBED_OBJECT == 1);
        }
    }
    if (o->step == 0xf && !o->b9e) playerTryAttack(o, 1);
}
#endif
