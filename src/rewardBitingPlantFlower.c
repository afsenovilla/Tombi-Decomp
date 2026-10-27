// FUNC 8003d3b8 336 MAIN0
// MATCHING 8003d3b8 336
// Ported from psx_tomba (reward.c, rewardBitingPlantFlower); MIT licence of the original project.
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

void rewardBitingPlantFlower(GameObject* arg0)
{
    u_short temp_v0;
    u_char temp_v1;

    temp_v1 = arg0->subState;
    switch (temp_v1) {
        case 0:
            addItemToInventory(ITEM_BITINGPLANTFLOWER, 1, true);
            setEventStarted(EVENT_BITINGPLANTFLOWER, 0, 0);
            if (GAME.selectedArea == AREA00_VILLAGEOFALLBEGINNINGS) {
                GAME.bittingPlantFlowerState = 2;
            }
            if (!(arg0->unkC & 0x80)) {
                func_8002367C(arg0->objectIndex);
            }
            arg0->cooldownTimer = 0x12CU;
            asm("");
            D_8009BCA7 = 1;
            D_8009BCAA = 1;
            PLAYER_STATE = 5;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            D_800A539F = 0;
            arg0->subState++;
            return;
        case 1:
            temp_v0 = arg0->cooldownTimer - 1;
            arg0->cooldownTimer = temp_v0;
            if ((temp_v0 << 0x10) == 0) {
                D_8009BCA7 = 0;
                D_8009BCAA = 0;
                PLAYER_STATE = 1;
                PLAYER_SUBSTATE = 0;
                PLAYER_STEP = 0;
                D_800A539F = 0;
                arg0->state++;
            }
            return;
    }
}
