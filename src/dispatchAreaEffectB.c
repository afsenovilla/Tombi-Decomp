// FUNC 80030ca8 176 MAIN0
// MATCHING 80030ca8 176
// Ported from psx_tomba (effect.c, dispatchAreaEffectB); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
void updateScrollAnchoredEffect(u8* self);

extern u8 D_8009C1C0;

extern u8 D_8009C3F0;
extern u8 D_8009BCA6;
void func_800346A8(u8* self);
/* The effect tables live in the retail .rodata (linked from splat by tools/build_full.py). */
#ifdef EFFECT_TABLES

extern u8 D_8009C3F0;
extern u8 D_8009BCA6;

void func_800346A8(u8* self);

#endif

void dispatchAreaEffectB(void)
{
    switch (GAME.selectedArea) {
        case AREA01_DWARFFOREST:
            func_80120A2C();
            return;
        case AREA02_DWARFVILLAGE:
            func_801178F4();
            return;
        case AREA03_PHOENIXMOUNTAIN:
            func_8011D84C();
            return;
        case AREA04_HAUNTEDMANSION:
            func_8011CEC0();
            return;
        case AREA09_MUSHROOMVILLAGE:
            func_8011CD84();
            return;
        case AREA10_DEEPJUNGLE:
            func_8011A778();
            return;
        case AREA16_VILLAGEOFCIVILIZATIONCLOCKTOWER:
            func_80116A60();
            // fallthrough
        default:
            return;
    }
}
