// FUNC 800ee25c 72 X016
// MATCHING 800ee25c 72
#include "gamestate.h"
extern void updateA00ApBox100k(void);
extern void func_8011C5CC(void);

void updateApBoxByArea(void)
{
    if (GAME.selectedArea == 0) {
        updateA00ApBox100k();
    } else if (GAME.selectedArea == 9) {
        func_8011C5CC();
    }
}
