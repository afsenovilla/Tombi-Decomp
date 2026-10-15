// FUNC 8011f850 56 X014
// MATCHING 8011f850 56
#include "gamestate.h"
static int D_80125E30[8] = {
    198, 203, 208, 214, 219, 224, 229, 235,
};
extern void FUN_8001e4f0(int);

void evilPigPlayAttackSfx(void)
{
    FUN_8001e4f0(D_80125E30[GAME.selectedSection]);
}
