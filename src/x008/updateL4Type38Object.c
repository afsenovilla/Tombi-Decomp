// FUNC 800ee25c 72 X008
// MATCHING 800ee25c 72
#include "gamestate.h"
extern void l4Type38Update(void);
extern void func_8011C5CC(void);

void updateL4Type38Object(void)
{
    if (GAME.selectedArea == 0) {
        l4Type38Update();
    } else if (GAME.selectedArea == 9) {
        func_8011C5CC();
    }
}
