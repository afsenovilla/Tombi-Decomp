// FUNC 8002aa18 120 MAIN0
// MATCHING 8002aa18 120
#include "gamestate.h"
extern void func_800E8B28();
extern void func_80115EE4();

void dispatchSectionUpdate(void)
{
    switch (GAME.selectedSection) {
    case 0:
    case 2:
        func_800E8B28();
        break;
    case 1:
    case 3:
        func_80115EE4();
        break;
    }
}
