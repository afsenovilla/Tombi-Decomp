// FUNC 80115ee4 252 X005
// MATCHING 80115ee4 252
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

extern void (*D_80079AD0[])(TObj *);
extern void cameraModeHandler6B(TObj *);
extern void tryStartItemInteraction(TObj *, int, int);
extern void cameraEventScrollHandler(TObj *);
#ifdef PORT2
extern int port_ws_player_margin(void);   /* runtime/ws_exits.c */
#endif

void updateAreaScroll(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.playerLanded != 0) o->step++;
        cameraModeHandler6B(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
#ifdef PORT2
        /* widescreen: fixed camera (1 unit = 1 column): the exit moves out to the wide frame's left edge */
        if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x14 - port_ws_player_margin() && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b) tryStartItemInteraction(o, 1, 0);
#else
        if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x14 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b) tryStartItemInteraction(o, 1, 0);
#endif
        break;
    case 2:
        cameraEventScrollHandler(o);
        break;
    }
}
