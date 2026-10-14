// FUNC 80125f04 184 X001
// MATCHING 80125f04 184
#include "spad.h"
#include "TOBJ.H"
extern TObj PLAYER;
extern short boxesOverlapSigned(TObj *, TObj *);
extern int func_80042D5C(TObj *, TObj *);

void func_80125F04(TObj *p, TObj *o)
{
    if (boxesOverlapSigned(p, o) == -1)
        return;
    if (o->b6a == 2) {
        PLAYER.active = 1;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
    }
    o->b6a = 0;
    o->w7a = p->animFrame & 1;
    if (func_80042D5C(p, o)) {
        o->active = 3;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
    }
    SPAD.s19e = 0;
}
