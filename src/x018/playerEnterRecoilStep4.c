// FUNC 800f3eec 80 X018
// MATCHING 800f3eec 80
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;

void playerEnterRecoilStep4(TObj *o)
{
    PLAYER_EXT[8] = 1;
    PLAYER_EXT[7] = o->animFrame;
    o->b9c = 2;
    o->timer = 10;
    o->step = 4;
    o->velY = 0;
    *(unsigned char *)&o->wac = 1;
    o->state = 3;
}
