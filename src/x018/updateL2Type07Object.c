// FUNC 80111ae0 96 X018
// MATCHING 80111ae0 96
#include "gamestate.h"
extern void func_8012ED60(void);
extern void func_80123B94(void);
extern void func_80121BAC(void);
void updateL2Type07Object(void)
{
    unsigned short m = GAME.selectedArea;
    if (m == 0) {
        func_8012ED60();
    } else if (m == 4) {
        func_80123B94();
    } else if (m == 10) {
        func_80121BAC();
    }
}
