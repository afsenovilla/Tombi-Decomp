// FUNC 8004985c 152 MAIN0
// MATCHING 8004985c 152
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern void updateKomaPig(void);
extern void func_8012EFC4(void);
extern void func_80124B78(void);
extern void func_80123750(void);

void dispatchAreaEnemyDraw(void)
{
    switch (GAME.selectedArea) {
    case AREA00_VILLAGEOFALLBEGINNINGS:
        updateKomaPig();
        break;
    case AREA01_DWARFFOREST:
        func_8012EFC4();
        break;
    case AREA03_PHOENIXMOUNTAIN:
        func_80124B78();
        break;
    case AREA04_HAUNTEDMANSION:
        func_80123750();
        break;
    }
}
