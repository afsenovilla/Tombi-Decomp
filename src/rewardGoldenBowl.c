// FUNC 8003d2c4 244 MAIN0
// MATCHING 8003d2c4 244
// Ported from psx_tomba (reward.c, rewardGoldenBowl); MIT licence of the original project.
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

void rewardGoldenBowl(GameObject* arg0)
{
    u_char health;

    if (D_8009BCA0 == 0) {
        health = GAME.playerMaxHealth;
        if (health < 16) {
            health += GAME.bonusHealth;
            GAME.playerMaxHealth = health;
            if ((u_long) ((byte)health & 0xFF) >= 17) {
                GAME.playerMaxHealth = 16;
            }
            playSFX(10);
            PLAYER_HEALTH = (u_short*)(*(char*)&GAME.playerMaxHealth);
            D_800A5432 = *(char*)&GAME.playerMaxHealth;
            *(char*)&GAME.playerHealth = *(char*)&GAME.playerMaxHealth;
        }
        GAME.goldenBowlState = 1;
        D_800B078C = &D_800121C8;
        playSFX(10);
        if (!(arg0->unkC & 0x80)) {
            func_8002367C(arg0->objectIndex);
        }
        arg0->state++;
    }
}
