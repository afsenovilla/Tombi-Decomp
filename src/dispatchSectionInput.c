// FUNC 8002ab08 60 MAIN0
// MATCHING 8002ab08 60
#include "gamestate.h"
extern void func_800E8D98();
extern void func_80115FC0();

void dispatchSectionInput(void)
{
    if (GAME.selectedSection == 0) {
        func_800E8D98();
    } else {
        func_80115FC0();
    }
}
