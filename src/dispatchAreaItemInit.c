// FUNC 8003e8d0 72 MAIN0
// MATCHING 8003e8d0 72
#include "gamestate.h"
void func_80123344(void);
void func_8011E154(void);

void dispatchAreaItemInit(void)
{
    unsigned short v = GAME.selectedArea;
    if (v == 0) {
        func_80123344();
    } else if (v == 4) {
        func_8011E154();
    }
}
