// FUNC 8002ffb0 132 MAIN0
// MATCHING 8002ffb0 132
#include "TOBJ.H"
#include "player.h"
extern TObj *ObjAlloc();

void spawnPlayerEffect4EStep1(int a, int b)
{
    TObj *o = ObjAlloc();
    if (o) {
        o->active = 1;
        o->type = 0x4e;
        o->subtype = a;
        o->b0c = b;
        o->a.raw = ((int*)&PLAYER.obj.a.raw)[0];
        o->y.raw = ((int*)&PLAYER.obj.a.raw)[1];
        o->b.raw = ((int*)&PLAYER.obj.a.raw)[2];
        o->step = 1;
    }
}
