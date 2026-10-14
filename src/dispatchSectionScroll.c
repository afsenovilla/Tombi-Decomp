// FUNC 8002ab44 60 MAIN0
// MATCHING 8002ab44 60
#include "gamestate.h"
extern void func_800E8ED4();
extern void func_80115F88();

void dispatchSectionScroll(void)
{
    if (GAME.selectedSection == 0)
        func_800E8ED4();
    else
        func_80115F88();
}
