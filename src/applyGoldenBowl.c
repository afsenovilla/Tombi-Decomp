// FUNC 80026fe8 184 MAIN0
// MATCHING 80026fe8 184
// Ported from psx_tomba (inventory.c, applyGoldenBowl); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

u8 applyGoldenBowl(void)
{
    u8 health;

    if ((u8) GAME.playerHealthDisplayed < 0x10U) {
        health = GAME.playerHealthDisplayed + GAME.bonusHealth;
        GAME.playerHealthDisplayed = health;
        if ((u32) (health & 0xFF) >= 0x11U) {
            GAME.playerHealthDisplayed = 0x10;
        }
        printInfoMessage(ITEM_GOLDENBOWL, MSG_TYPE_ITEM);
        playSFX(10);
        (u16*)D_800A5430 = GAME.playerHealthDisplayed;
        D_800A5432 = GAME.playerHealthDisplayed;
        GAME.playerHealth = GAME.playerHealthDisplayed;
    }
    GAME.goldenBowlState = 1;
    D_800B078C = (u8* ) &D_800121C8;
    return GAME.playerHealth;
}
