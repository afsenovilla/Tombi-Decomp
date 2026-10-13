// FUNC 80114a08 64 X000
// MATCHING 80114a08 64
#include "gamestate.h"
extern void FUN_801300b0(void);
extern void FUN_8012d4ac(void);
void FUN_80114a08(void)
{
    if (GAME.selectedArea == 3)
        FUN_801300b0();
    else
        FUN_8012d4ac();
}
