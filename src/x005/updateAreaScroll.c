// FUNC 80115ee4 252 X005
// MATCHING 80115ee4 252
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

extern void (*D_80079AD0[])(TObj *);
extern void cameraModeHandler6B(TObj *);
extern void tryStartItemInteraction(TObj *, int, int);
extern void cameraEventScrollHandler(TObj *);

void updateAreaScroll(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.playerLanded != 0) o->step++;
        cameraModeHandler6B(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x14 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b) tryStartItemInteraction(o, 1, 0);
        break;
    case 2:
        cameraEventScrollHandler(o);
        break;
    }
}
