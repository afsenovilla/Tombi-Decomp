// FUNC 8002aa90 120 MAIN0
// MATCHING 8002aa90 120
#include "gamestate.h"
extern void func_800E8C98(void);
extern void func_80115EA8(void);

void dispatchSectionDraw(void)
{
    switch (GAME.selectedSection) {
    case 0:
    case 2:
        func_800E8C98();
        break;
    case 1:
    case 3:
        func_80115EA8();
        break;
    }
}
