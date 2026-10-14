// FUNC 8002abbc 60 MAIN0
// MATCHING 8002abbc 60
#include "gamestate.h"
void func_80116234(void);
void func_801162D8(void);

void dispatchSectionClose(void)
{
    if (GAME.selectedSection == 0) {
        func_80116234();
    } else {
        func_801162D8();
    }
}
