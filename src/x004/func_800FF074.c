// FUNC 800ff074 116 X004
// MATCHING 800ff074 116
#include "gamestate.h"
extern unsigned char D_8009CFFB;
extern unsigned char D_8009CDAC;

int func_800FF074(void)
{
    short t = D_8009CFFB != 0;
    short r;
    r = t;
    if (D_8009CDAC == 0xff) r = t + 1;
    if (GAME.selectedArea == 0 && GAME.selectedSection < 2) r = 0;
    if (GAME.displayExpBar == 1) r = 0;
    return r;
}
