// FUNC 80030afc 96 MAIN0
// MATCHING 80030afc 96
// Ported from psx_tomba (effect.c, updateL4Type1BObject); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
void updateScrollAnchoredEffect(u8* self);

extern u8 D_8009C1C0;

extern u8 D_8009C3F0;
extern u8 D_8009BCA6;

void func_800346A8(u8* self);

void updateL4Type1BObject(void)
{
    if (GAME.selectedArea == AREA00_VILLAGEOFALLBEGINNINGS) {
        l4Type1BArea0Update();
    } else if (GAME.selectedArea == AREA06_DIRTMOTOCROSS) {
        func_8011A87C();
    } else if (GAME.selectedArea == AREA11_VILLAGEOFCIVILIZATION){
        func_801163d4();
    }
    return;
}
