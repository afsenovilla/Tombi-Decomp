// FUNC 800ecbfc 160 I0NFO3
// MATCHING 800ecbfc 160
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void **D_800EED64[][3];

void tunnelActorIdleStep(TObj *o)
{
    short f;
    switch (o->state) {
    case 0:
        f = o->animFrame;
        o->anim = D_800EED64[o->subtype][0][f];
        func_8001FE94(o, 0);
        S16(o, 0xD2) = f;
        o->state++;
    case 1:
        func_8001FEC0(o);
        break;
    }
}
