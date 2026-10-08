// FUNC 80128054 136 X001
// MATCHING 80128054 136
#include "TOBJ.H"
extern short func_800482EC(TObj *, TObj *);
extern void playSFX(int);

void func_80128054(TObj *o, TObj *e)
{
    if (o->type == 0 && o->b0c == 1 && func_800482EC(o, e)) {
        o->active = 2;
        o->b04 = 2;
        o->step = 2;
        o->state = 0;
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        playSFX(7);
    }
}
