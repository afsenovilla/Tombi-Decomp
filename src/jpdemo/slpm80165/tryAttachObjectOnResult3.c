// JPDEMO slpm80165 8004A7C8 124
// Demo version of PAL main0 800442B0 (tryAttachObjectOnResult3.c): attached object at +0xd0 (PAL +0xe4).
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
        *(TObj **)((char *)o + 0xd0) = e;
        *(unsigned char *)&o->wac = 2;
    }
}
