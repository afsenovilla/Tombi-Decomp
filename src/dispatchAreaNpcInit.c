// FUNC 80045ae8 64 MAIN0
// MATCHING 80045ae8 64
#include "gamestate.h"
extern void func_80124410(void);
extern void func_8011DE58(void);
void dispatchAreaNpcInit(void)
{
    if (GAME.selectedArea == 1) {
        func_80124410();
    } else {
        func_8011DE58();
    }
}
