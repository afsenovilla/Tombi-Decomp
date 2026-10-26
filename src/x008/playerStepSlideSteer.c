// FUNC 80107c00 972 X008
// MATCHING 80107c00 972
#include "gv6.h"
#include "TOBJ.H"

#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern unsigned char *PLAYER_EXT;
extern int GRABBED_OBJECT;
extern char D_80010AE8[];
void playerSetAngleFromSlope(TObj *o);
void playerSetAnim(TObj *o, int a, int b);
void playerAccelerateDownhill(TObj *o);
void playerMoveByAngleSpeed(TObj *o);
int playerProbeGrab(TObj *o, int a);
void playerEnterGrabStep(TObj *o, int a);
void PlayerSetAnimIfChanged(TObj *o, int a);
void playerSetFallLean(TObj *o);
void AnimJump(TObj *o, int a);
void playerCheckGroundAndFall(TObj *o);

void playerStepSlideSteer(TObj *o)
{
    volatile unsigned short *k;
    switch (o->state) {
    case 0:
        PLAYER_EXT[0xb] = o->animFrame;
        if ((o->animFrame &= 1) != 0) {
            if (o->wb2 > 0) o->wb2 = -o->wb2;
        } else {
            if (o->wb2 < 0) o->wb2 = -o->wb2;
        }
        B(o, 0xac) = 0;
        o->b9c = 0;
        o->b6b = 0;
        B(o, 0xc7) = 1;
        o->b9d = 0;
        B(o, 0xc6) = 0;
        B(o, 0xe3) = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        PLAYER_EXT[0] = 0;
        o->ba4 = 1;
        playerSetAngleFromSlope(o);
        o->state++;
    case 1:
        k = &GV6.s70;
        if (*k & 0xa0) {
            if (*k & 0x80) o->animFrame = 1;
            if (*k & 0x20) o->animFrame = 0;
        }
        if (o->bbe & 1) {
            if (!(o->animFrame & 1)) {
                playerSetAnim(o, 8, 2);
                o->d8c = 0xe0;
            } else {
                playerSetAnim(o, 0x11, 0);
                o->d8c = 0;
            }
        } else {
            if (o->animFrame & 1) {
                playerSetAnim(o, 8, 2);
                o->d8c = 0x20;
            } else {
                playerSetAnim(o, 0x11, 0);
                o->d8c = 0;
            }
        }
        o->y.raw += 0x80000;
        playerAccelerateDownhill(o);
        playerMoveByAngleSpeed(o);
        if (o->bbe & 2) {
            o->step = 0x13;
            o->state = 0;
            o->substep = 0;
        }
        if (o->bbe == 0) {
            if (*(unsigned short *)(PLAYER_EXT + 0x2c) == 8 && B(o, 0xac) < 2) {
                GRABBED_OBJECT = playerProbeGrab(o, 1);
                if (GRABBED_OBJECT) {
                    PLAYER_EXT[8] = 0;
                    *(short *)(PLAYER_EXT + 0x20) = 0;
                    B(o, 0xac) = 0;
                    o->b9c = 0;
                    o->wb2 = 0;
                    playerEnterGrabStep(o, GRABBED_OBJECT == 1);
                }
            }
            if (o->animFrame != (o->bbe & 1)) o->wb2 = 0;
            o->ba4 = 0;
            o->wb6 = 0;
            PlayerSetAnimIfChanged(o, 4);
            if (PLAYER_EXT[0] == 0) {
                o->velX = 0;
                o->velY = 0;
                B(o, 0xac) = 1;
                o->y.p.whole -= 4;
                playerSetFallLean(o);
                k = &GV6.s70;
                if (*k & 0x80) o->animFrame = 1;
                if (*k & 0x20) o->animFrame = 0;
                o->anim = D_80010AE8;
                AnimJump(o, 3);
                o->step = 2;
                o->state = 3;
                o->substep = 0;
                break;
            }
            o->velX = 0;
            o->velY = 0;
            playerSetFallLean(o);
            k = &GV6.s70;
            if (*k & 0x80) o->animFrame = 1;
            if (*k & 0x20) o->animFrame = 0;
            o->step = 4;
            o->state = 3;
            o->substep = 0;
        }
        o->velX = 0;
        o->velY = 0;
        playerCheckGroundAndFall(o);
        break;
    }
}
