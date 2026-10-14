// FUNC 800ee25c 72 X009
// MATCHING 800ee25c 72
#include "gamestate.h"
extern void FUN_80122118(void);
extern void func_8011C5CC(void);

void func_800EE25C(void)
{
    if (GAME.selectedArea == 0) {
        FUN_80122118();
    } else if (GAME.selectedArea == 9) {
        func_8011C5CC();
    }
}
