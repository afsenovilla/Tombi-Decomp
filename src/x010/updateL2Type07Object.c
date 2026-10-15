// FUNC 80111ae0 96 X010
// MATCHING 80111ae0 96
#include "gamestate.h"
extern void l2Type07Update(void);
extern void func_80123B94(void);
extern void updateArea10Type07Object(void);
void updateL2Type07Object(void)
{
    unsigned short m = GAME.selectedArea;
    if (m == 0) {
        l2Type07Update();
    } else if (m == 4) {
        func_80123B94();
    } else if (m == 10) {
        updateArea10Type07Object();
    }
}
