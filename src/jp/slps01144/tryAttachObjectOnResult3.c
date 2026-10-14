// JP slps01144 8004F6A4 124
// Japanese version of PAL main0 800442B0 (tryAttachObjectOnResult3.c): 228 -> 212.
#include "TOBJ.H"
short FUN_80043464(TObj *o, TObj *e);

void tryAttachObjectOnResult3(TObj *o, TObj *e)
{
    short r = FUN_80043464(o, e);
    if (r != -1 && r == 3 && *(unsigned char *)&o->wac == 1) {
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->b69 = 0;
        *(TObj **)((char *)o + 0xd4) = e;
        *(unsigned char *)&o->wac = 2;
    }
}
