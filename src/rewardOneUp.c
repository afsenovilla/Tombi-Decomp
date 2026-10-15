// FUNC 8003d23c 136 MAIN0
// MATCHING 8003d23c 136
// Ported from psx_tomba (reward.c, rewardOneUp); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"

extern int D_80077274;
extern int D_8013A44C;
extern int D_80134018;
extern u8 D_8009C263;
extern u8 D_8009C616;
extern int D_80131D84[];
extern int D_8007728C;

typedef struct {
    int x;
    int y;
    int z;
} VEC3;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    s16 unk6;
    int* unk8;
} unk_8007D6E0;

typedef struct {
    int unk0;
    s16 unk4;
    s16 unk6;
} unk_8007E868;

//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/reward", initItemObject);

void rewardOneUp(GameObject* arg0)
{
    u_char lives = GAME.playerLives;
    if (lives < 99) {
        GAME.playerLives = (u_char)(lives+1);
        printInfoMessage(MSG_ONEUP_ACQUIRED, MSG_TYPE_REWARD);
        playSFX(10);
    }
    if (!(arg0->unkC & 0x80)) {
        setAreaFlagBit(arg0->objectIndex);
    }
    arg0->state++;
}
