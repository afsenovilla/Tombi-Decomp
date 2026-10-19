// FUNC 8004985c 152 MAIN0
// MATCHING 8004985c 152
#include "gamestate.h"
extern void updateKomaPig(void);
extern void func_8012EFC4(void);
extern void func_80124B78(void);
extern void func_80123750(void);

void dispatchAreaEnemyDraw(void)
{
    switch (GAME.selectedArea) {
    case 0:
        updateKomaPig();
        break;
    case 1:
        func_8012EFC4();
        break;
    case 3:
        func_80124B78();
        break;
    case 4:
        func_80123750();
        break;
    }
}
