// FUNC 80111ae0 96 X004
// MATCHING 80111ae0 96
#include "gamestate.h"
extern void updateA00BitingPlant(void);
extern void updateA04BitingPlant(void);
extern void func_80121BAC(void);
void updateBitingPlantByArea(void)
{
    unsigned short m = GAME.selectedArea;
    if (m == 0) {
        updateA00BitingPlant();
    } else if (m == 4) {
        updateA04BitingPlant();
    } else if (m == 10) {
        func_80121BAC();
    }
}
