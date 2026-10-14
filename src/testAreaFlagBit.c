// FUNC 80020a78 116 MAIN0
// MATCHING 80020a78 116
#include "gamestate.h"
extern int D_8009C994[][8];

int testAreaFlagBit(int n)
{
    int q = n / 32;
    int r = n % 32;
    int k;
    if (GAME.selectedArea == 0x10)
        k = 7;
    else if (GAME.selectedArea == 0x11)
        k = 0xc;
    else
        k = GAME.selectedArea;
    return D_8009C994[k][q] & (1 << r);
}
