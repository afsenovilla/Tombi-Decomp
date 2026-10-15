// FUNC 80111ae0 96 X004
// MATCHING 80111ae0 96
#include "gamestate.h"
extern void l2Type07Update(void);
extern void updateArea4Type07Object(void);
extern void func_80121BAC(void);
void updateL2Type07Object(void)
{
    unsigned short m = GAME.selectedArea;
    if (m == 0) {
        l2Type07Update();
    } else if (m == 4) {
        updateArea4Type07Object();
    } else if (m == 10) {
        func_80121BAC();
    }
}
