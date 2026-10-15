// FUNC 8003ce14 244 MAIN0
// MATCHING 8003ce14 244
// Ported from psx_tomba (reward.c, rewardHeart); MIT licence of the original project.
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

void rewardHeart(GameObject* arg0)
{
    if (D_8009BCA0 == 0) {
        func_800E92D4(0x64, arg0->x, arg0->y, arg0->z);
        playSFX(9);
        if (!(arg0->unkC & 0x80)) {
            func_8002367C(arg0->objectIndex);
        }
        if (!(arg0->unkC & 0x7F)) {
            *(short*)&D_800A5430+=1;
            if ((short)GAME.playerHealthDisplayed < *(short*)&D_800A5430 ) {
                *(short*)&D_800A5430 = GAME.playerHealthDisplayed;
            }
        } else {
            *(short*)&D_800A5430+=2;
            if ((short)GAME.playerHealthDisplayed < *(short*)&D_800A5430 ) {
                *(short*)&D_800A5430 = GAME.playerHealthDisplayed;
            }
        }
        D_800A5432 = D_800A5430;
        GAME.playerHealth = (u_char) D_800A5430;
        arg0->state++;
    }
}
