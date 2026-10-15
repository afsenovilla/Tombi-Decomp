// FUNC 80121b58 532 X001
// MATCHING 80121b58 532
#include "gv2.h"
#include "TOBJ.H"
extern TObj *DAT_8009c330;
extern unsigned short DAT_8009d670;
extern int DAT_8009c984[];
extern unsigned short DAT_1f8001fc, DAT_1f8003c4, DAT_1f8003c6;
extern char D_80010748[];
extern void FUN_8001e4f0(int);
extern void startPadVibration(int, int, int, int);
extern void spawnGroundDebris(int, short, int, int);
extern void func_80110CAC(TObj *);
extern void playerMoveByAngleSpeed(TObj *);
extern void FUN_800f00ac(TObj *);
extern void playerContactStopCheckJump(TObj *);
extern void FUN_800ef4e4(TObj *);
extern void AnimLoadDuration(TObj *);

void playerStepContactStop(TObj *o)
{
    *(char *)&o->wac = 0;
    switch (o->state) {
    case 0:
        FUN_8001e4f0(0x3e);
        startPadVibration(0, 0, 0xff, 0x14);
        spawnGroundDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
        o->state++;
    case 1:
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb6 = 0;
        o->state++;
    case 2:
        func_80110CAC(o);
        playerMoveByAngleSpeed(o);
        FUN_800f00ac(o);
        playerContactStopCheckJump(o);
        if (o->wb2 == 0) {
            o->state = 3;
        }
        break;
    case 3:
        DAT_8009c330->animFrame = 0xffff;
        DAT_8009c330->animTimer = 0;
        o->anim = D_80010748;
        AnimLoadDuration(o);
        o->wb2 = 0;
        o->ba4 = 0;
        o->b9e = 0;
        o->velX = 0;
        o->velY = 0;
        o->velH = 0;
        o->velV = 0;
        o->state++;
    case 4:
        FUN_800ef4e4(o);
        if (DAT_1f8001fc & 0xa0) {
            FUN_8001e4f0(0x3e);
        }
        if (DAT_1f8001fc & DAT_1f8003c6) {
            GV2.bb0 = 0;
            FUN_8001e4f0(0x3e);
            spawnGroundDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
            o->b9c = 1;
            o->b04 = 1;
            o->y.p.whole -= 8;
            if ((DAT_8009c984[0] & 0x40) && (*(volatile unsigned short *)&DAT_8009d670 & DAT_1f8003c4)) {
                o->ba7 = 1;
            }
            o->step = 2;
            o->state = 0;
        }
        break;
    }
}
