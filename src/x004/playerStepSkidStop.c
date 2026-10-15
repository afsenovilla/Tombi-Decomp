// FUNC 80106fec 784 X004
// MATCHING 80106fec 784
#include "gv2.h"
#include "spad.h"
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char p[0x2c]; unsigned short w2c; unsigned short w2e; } G;
extern G *D_8009C330;
void ObjSetAnimFromTable(TObj *o);
void AnimLoadDuration(TObj *o);
int AnimAdvance(TObj *o);
void setFacingAndUpDownFromPad(TObj *o);
void playerSetAngleFromSlope(TObj *o);
void func_8010FE70(TObj *o);
void playerMoveByAngleSpeed(TObj *o);
void playerStickToGround(TObj *o);
void FUN_800f00ac(TObj *o);
void ObjAddVelY7E(TObj *o);
short ObjTileCollide(TObj *, int, int);
void playerTryAttack(TObj *o, int);
void playerStepSkidStop(TObj *o)
{
    G *g;
    switch (o->state) {
    case 0:
        *((unsigned char *)o + 0xd5) = 0;
        *((unsigned char *)o + 0xd6) = 0;
        *((unsigned char *)o + 0xd7) = 0;
        *((unsigned char *)o + 0xbf) = 0;
        g = D_8009C330;
        o->velX = 0;
        o->velY = 0;
        g->w2c = 0x1a;
        if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
            g->w2c = 0x2a;
        if (D_8009C330->w2e != D_8009C330->w2c) {
            ObjSetAnimFromTable(o);
            AnimLoadDuration(o);
            D_8009C330->w2e = D_8009C330->w2c;
        }
        o->state++;
    case 1:
        setFacingAndUpDownFromPad(o);
        playerSetAngleFromSlope(o);
        func_8010FE70(o);
        if (o->b69 & 2) {
            AnimAdvance(o);
            func_8010FE70(o);
            o->b69 = 1;
        }
        playerMoveByAngleSpeed(o);
        playerStickToGround(o);
        FUN_800f00ac(o);
        if (AnimAdvance(o)) goto stop;
        if ((o->animFrame & 1) && o->wb2 < 0) {
            o->step = 1;
            o->state = 0;
        }
        if (!(o->animFrame & 1) && o->wb2 > 0) {
            o->step = 1;
            o->state = 0;
        }
        if ((o->animFrame & 2) && o->animFrame < 4) {
            D_8009C330->w2e = 0xffff;
            o->step = 0;
            o->state = 0;
        }
        if (SPAD.s1fc & SPAD.s3c6) {
            ((unsigned char*)&GV2.bb0)[0] = 0;
            o->b9c = 1;
            if ((((int*)&GAME.area00_eventControl)[0] & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 0;
        }
        playerTryAttack(o, 0);
        break;
    case 2:
        AnimAdvance(o);
        playerSetAngleFromSlope(o);
        ObjAddVelY7E(o);
        o->velY += 0x30;
        if (o->velY > 0) o->b9c = 2;
        if (o->b69 != 0) break;
        if (ObjTileCollide(o, 0, 0) == 0) break;
    stop:
        o->b9c = 0;
        o->velX = 0;
        o->velY = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
