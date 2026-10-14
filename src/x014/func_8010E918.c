// FUNC 8010e918 100 X014
// MATCHING 8010e918 100
#include "gcd.h"
#include "gv2.h"
#include "gamestate.h"
typedef struct { char p[0xc1]; unsigned char c1; } O;
int func_8010E918(O *o)
{
    int b = o->c1;
    int u = GV2.bb3 + b * 4;
    if ((GAME.displayExpBar & 3) != 0)
        u = (unsigned char)b << 2 | 3;
    if (GCD.bcf06 != 0)
        u = 8;
    if (GCD.bd006 != 0)
        u = 8;
    return u;
}
