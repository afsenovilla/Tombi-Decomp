// FUNC 80115ea8 228 X008
// MATCHING 80115ea8 228
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
typedef void (*Fn)(TObj *);

extern Fn D_80079AB4[];
extern void FUN_8002a3d4(TObj *);
extern int tryStartItemInteraction(TObj *, int, int);
extern void FUN_8002a4d0(TObj *);

void updateAreaScroll(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.b3a != 0) o->step++;
        FUN_8002a3d4(o);
        break;
    case 1:
        D_80079AB4[o->subtype](o);
        if ((*(short*)((char*)&SPAD.i170 + 2)) < -0x59) tryStartItemInteraction(o, 1, 0);
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
