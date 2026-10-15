// FUNC 800ff074 116 X010
// MATCHING 800ff074 116
#include "gcd.h"
#include "gamestate.h"

int getHudExpBarMode(void)
{
    short t = GCD.bcffb != 0;
    short r;
    r = t;
    if (GCD.bcdac == 0xff) r = t + 1;
    if (GAME.selectedArea == 0 && GAME.selectedSection < 2) r = 0;
    if (GAME.displayExpBar == 1) r = 0;
    return r;
}
