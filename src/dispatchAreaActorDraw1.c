// FUNC 800459d8 60 MAIN0
// MATCHING 800459d8 60
#include "gamestate.h"
void func_80124FA4(void);
void func_8011FE08(void);
void dispatchAreaActorDraw1(void)
{
    if (GAME.selectedArea == 0) func_80124FA4();
    else func_8011FE08();
}
