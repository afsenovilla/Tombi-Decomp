// FUNC 80116764 60 X006
// MATCHING 80116764 60
#include "gamestate.h"
extern void updateAreaScroll(void);
extern void func_801163B8(void);

void func_80116764(void)
{
    if (GAME.selectedSection) updateAreaScroll();
    else func_801163B8();
}
