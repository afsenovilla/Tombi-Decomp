// FUNC 800f28a0 316 X014
// MATCHING 800f28a0 316
#include "TOBJ.H"
extern unsigned short GAME;
extern short GVARS_PLAYER_DRIFT_H, GVARS_PLAYER_DRIFT_Y;
extern TObj *PLAYER_EXT;
extern char DAT_80010ae8[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void setVelXFromPadLR(TObj *);
extern void adjustVelYFromPadDown(TObj *);
extern void FUN_8001fd94(TObj *);
extern void playerFloatDirFromPad(TObj *);

void playerJumpArea3Float(TObj *o)
{
    TObj *p;
    unsigned short *q;
    if (GAME == 3 && GVARS_PLAYER_DRIFT_H != 0) {
        p = PLAYER_EXT;
        q = &p->animFrame;
        p->animTimer = 0x35;
        if (*q != 0x35) {
            p->animTimer = 0x35;
            FUN_800efc04(o);
            FUN_8001fe94(o, 0);
            PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
        }
    } else {
        o->anim = DAT_80010ae8;
        FUN_8001fe94(o, 3);
    }
    FUN_8001fec0(o);
    *(volatile int *)&o->h->raw += GVARS_PLAYER_DRIFT_H * 0x80;
    o->y.raw += GVARS_PLAYER_DRIFT_Y * 0x80;
    setVelXFromPadLR(o);
    o->h->raw += o->velX * 0x100;
    o->timer--;
    if ((short)o->timer <= 0) {
        o->timer = 0;
        adjustVelYFromPadDown(o);
        FUN_8001fd94(o);
    }
    playerFloatDirFromPad(o);
}
