// FUNC 8011f888 56 X014
// MATCHING 8011f888 56
#include "gamestate.h"
static int D_80125E50[8] = {
    199, 204, 209, 215, 220, 225, 230, 236,
};
extern void FUN_8001e4f0(int);

void evilPigPlayAttackSfxB(void)
{
    FUN_8001e4f0(D_80125E50[GAME.selectedSection]);
}
