// FUNC 8011f850 56 X014
// MATCHING 8011f850 56
#include "gamestate.h"
extern int D_80125E30[];
extern void FUN_8001e4f0(int);

void func_8011F850(void)
{
    FUN_8001e4f0(D_80125E30[GAME.selectedSection]);
}
