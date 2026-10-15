// FUNC 8003dbf4 144 MAIN0
// MATCHING 8003dbf4 144
// Ported from psx_tomba (reward.c, rewardAnimalDash); MIT licence of the original project.
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

void rewardAnimalDash(GameObject* arg0)
{
    printInfoMessage(MSG_ANIMALDASH_ACQUIRED, MSG_TYPE_REWARD);
    GAME.area00_eventControl |= 0x40;
    playSFX(10);
    if (!(arg0->unkC & 0x80)) {
        setAreaFlagBit(arg0->objectIndex);
    }
    GAME.unk6ad = 1;
    GAME.unk736 = 1;
    arg0->state++;
}
