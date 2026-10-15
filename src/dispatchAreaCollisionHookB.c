// FUNC 80048d3c 80 MAIN0
// MATCHING 80048d3c 80
#include "gamestate.h"
extern void func_80126D04();
extern void func_8012038C();

void dispatchAreaCollisionHookB(void)
{
    switch (GAME.selectedArea) {
    case 0:
        func_80126D04();
        break;
    case 3:
        func_8012038C();
        break;
    }
}
