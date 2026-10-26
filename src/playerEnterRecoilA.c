// FUNC 800f29dc 84 X000
// MATCHING 800f29dc 84
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
void playerEnterRecoilA(TObj *o)
{
    int v;
    PLAYER_EXT[8] = 1;
    v = 0x10;
    o->velY = 0;
    o->velV = 0;
    o->d84 = 0;
    if (o->animFrame & 1) {
        v = 0xf0;
    }
    o->timer = 10;
    o->b9c = 2;
    o->d88 = v;
    *(unsigned char *)&o->wac = 1;
    o->state = 3;
}
