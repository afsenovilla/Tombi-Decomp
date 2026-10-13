// FUNC 8004bb84 60 MAIN0
// MATCHING 8004bb84 60
#include "gamestate.h"
extern void func_80126940(void);
extern void func_8011FE38(void);
void dispatchAreaUnlayeredHandler(void)
{
    if (GAME.selectedArea == 0) {
        func_80126940();
    } else {
        func_8011FE38();
    }
}
