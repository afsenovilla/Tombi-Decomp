// FUNC 80108fe0 88 X011
// MATCHING 80108fe0 88
#include "TOBJ.H"
extern unsigned char SLOPE_TILT_TABLE[];
extern TObj *PLAYER_EXT;

void playerResetAfterEvent(TObj *o)
{
    *(signed char *)&o->b0f = -8;
    o->ba5 = 0;
    ((unsigned char *)&o->da0)[3] = 0;
    o->b9c = 0;
    *(unsigned char *)&o->wac = 0;
    o->wb2 = 0;
    o->velX = 0;
    o->velY = 0;
    o->d8c = SLOPE_TILT_TABLE[o->wb0];
    PLAYER_EXT->timer = 0;
    o->b04 = 1;
    o->step = 0;
    o->state = 0;
}
