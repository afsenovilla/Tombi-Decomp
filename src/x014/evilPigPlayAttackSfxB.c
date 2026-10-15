// FUNC 8011f888 56 X014
// MATCHING 8011f888 56
#include "gamestate.h"
extern int D_80125E50[];
extern void FUN_8001e4f0(int);

void evilPigPlayAttackSfxB(void)
{
    FUN_8001e4f0(D_80125E50[GAME.selectedSection]);
}
