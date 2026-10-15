// FUNC 800442b0 124 MAIN0
// MATCHING 800442b0 124
#include "TOBJ.H"
short objectBoxOverlapSide(TObj *o, TObj *e);

void tryAttachObjectOnResult3(TObj *o, TObj *e)
{
    short r = objectBoxOverlapSide(o, e);
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
