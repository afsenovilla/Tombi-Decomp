// FUNC 8011dc4c 252 X003
// MATCHING 8011dc4c 252
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern int D_8009F0EC;
extern unsigned char D_8009CDC3;

void func_8011DC4C(TObj *o)
{
    unsigned char *p;

    D_8009F0EC = 0;
    if ((GAME.selectedSection == 1 || GAME.selectedSection == 5) && GAME.selectedSpawnPoint == 1) {
        o->b04 = 5;
        o->step = 9;
    }
    if ((GAME.selectedSection == 0 || GAME.selectedSection == 4) && D_8009CDC3 != 0xff && GAME.selectedSpawnPoint == 4) {
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
