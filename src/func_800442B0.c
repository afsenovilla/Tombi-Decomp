// FUNC 800442b0 124 MAIN0
// MATCHING 800442b0 124
#include "TOBJ.H"
short FUN_80043464(TObj *o, TObj *e);

void func_800442B0(TObj *o, TObj *e)
{
    short r = FUN_80043464(o, e);
    if (r != -1 && r == 3 && *(unsigned char *)&o->wac == 1) {
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->b69 = 0;
        *(TObj **)((char *)o + 0xe4) = e;
        *(unsigned char *)&o->wac = 2;
    }
}
