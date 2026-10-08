// FUNC 80125f04 184 X001
// MATCHING 80125f04 184
#include "TOBJ.H"
extern TObj D_800A6038;
extern short D_1F80019E;
extern short func_8004461C(TObj *, TObj *);
extern int func_80042D5C(TObj *, TObj *);

void func_80125F04(TObj *p, TObj *o)
{
    if (func_8004461C(p, o) == -1)
        return;
    if (o->b6a == 2) {
        D_800A6038.active = 1;
        D_800A6038.b04 = 1;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
    }
    o->b6a = 0;
    o->w7a = p->animFrame & 1;
    if (func_80042D5C(p, o)) {
        o->active = 3;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
    }
    D_1F80019E = 0;
}
