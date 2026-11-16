// FUNC 80116358 644 X002
// MATCHING 80116358 644
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
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
        if (GVARS.playerLanded != 0) {
            o->step++;
            o->subtype = 0;
        }
        ((short *)o->d38)[1] = 0;
        cameraModeHandler6B(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (GAME.selectedSection) {
        case 1:
            if ((*(short*)((char*)&SPAD.i170 + 2)) == 0) {
                if (PLAYER.obj.a.p.whole < 30 && PLAYER.obj.y.p.whole >= -59)
                    tryStartItemInteraction(o, 1, 0);
            } else {
                if ((*(short*)((char*)&SPAD.i168 + 2)) < 30 && (*(short*)((char*)&SPAD.i16c + 2)) >= -77)
                    tryStartItemInteraction(o, 1, 1);
            }
            break;
        case 2:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 30 && (*(short*)((char*)&SPAD.i16c + 2)) >= -59)
                tryStartItemInteraction(o, 1, 0);
            break;
        case 4:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 28 && (*(short*)((char*)&SPAD.i16c + 2)) >= -87)
                tryStartItemInteraction(o, 1, 0);
            break;
        case 5:
#ifdef PORT2
            /* widescreen: the camera is fixed here (1 unit = 1 column), so the exit moves out to the wide frame's
               right edge (runtime/ws_exits.c) */
            if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0x128 + port_ws_player_margin() && (*(short*)((char*)&SPAD.i16c + 2)) >= -59)
#else
            if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0x128 && (*(short*)((char*)&SPAD.i16c + 2)) >= -59)
#endif
                tryStartItemInteraction(o, 1, 0);
            else if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x2bd)
                tryStartItemInteraction(o, 1, 2);
            break;
        }
        break;
    case 2:
        cameraEventScrollHandler(o);
        break;
    }
}
