// FUNC 8011dc4c 252 X003
// MATCHING 8011dc4c 252
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern int GRABBED_OBJECT;

void playerInitAreaHook(TObj *o)
{
    unsigned char *p;

    GRABBED_OBJECT = 0;
    if ((GAME.selectedSection == 1 || GAME.selectedSection == 5) && GAME.selectedSpawnPoint == 1) {
        o->b04 = 5;
        o->step = 9;
    }
    if ((GAME.selectedSection == 0 || GAME.selectedSection == 4) && GCD.bcdc3 != 0xff && GAME.selectedSpawnPoint == 4) {
        GVARS.b3a = 1;
        GV2.bb0 = 0;
        o->active = 1;
        o->b04 = 1;
        o->step = 2;
        o->state = 3;
        o->substep = 0;
    }
    p = &GV2.bc2;
    if (*p == 1) {
        o->active = 5;
        o->b04 = 5;
        o->step = 0x40;
        o->visible = 0;
        GVARS.b3a = 1;
        *p = 2;
    }
}
