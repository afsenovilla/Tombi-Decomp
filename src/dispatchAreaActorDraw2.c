// FUNC 80045a14 60 MAIN0
// MATCHING 80045a14 60
#include "gamestate.h"
extern void func_80125984(void);
extern void func_8011FEC8(void);

void dispatchAreaActorDraw2(void)
{
    if (GAME.selectedArea == 0) func_80125984();
    else func_8011FEC8();
}
