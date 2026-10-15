// FUNC 8011d8c4 224 X004
// MATCHING 8011d8c4 224
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"

extern int D_8009F0EC;
extern void spawnCarriedGuide(TObj *);

void playerInitAreaHook(TObj *o)
{
    D_8009F0EC = 0;
    if (GAME.selectedSection == 2 && GAME.selectedSpawnPoint == 2) {
        o->b04 = 5;
        o->step = 9;
    }
    if (((unsigned char*)&GCD.bcdd6)[0] == 1) spawnCarriedGuide(o);
    if (((unsigned char*)&GCD.bcdd6)[0] == 0xff && GCD.bcdd5 == 1 && GCD.bcf00 && GCD.bcf04) spawnCarriedGuide(o);
}
