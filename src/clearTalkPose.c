// FUNC 8002ce64 220 MAIN0
// MATCHING 8002ce64 220
#include "gamestate.h"
#include "player.h"
extern unsigned char TALK_POSE;
void playerSetAnim(unsigned char *p, int a, int b);
void func_800E8A98(unsigned char *p, int a, int b);
void func_800E96E8(unsigned char *p, int a, int b);
void func_800E940C(unsigned char *p, int a, int b);
void clearTalkPose(void)
{
    unsigned char *p = &PLAYER.obj.type;
    TALK_POSE = 0;
    switch (*p) {
    case 0:
        if (GAME.selectedArea == 9) playerSetAnim(p - 2, 0, 0);
        break;
    case 1:
        func_800E8A98(p - 2, 0, (*(short*)&PLAYER.obj.animFrame));
        break;
    case 2:
        func_800E96E8(p - 2, 0, (*(short*)&PLAYER.obj.animFrame));
        break;
    case 3:
        func_800E940C(p - 2, 0, (*(short*)&PLAYER.obj.animFrame));
        break;
    }
}
