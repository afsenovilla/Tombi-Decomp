// FUNC 801165bc 424 X006
// MATCHING 801165bc 424
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef void (*Fn)(TObj *);
extern Fn D_80079AD0[];
extern void cameraModeHandler6B(TObj *);
extern int tryStartItemInteraction(TObj *, int, unsigned char);
extern void cameraEventScrollHandler(TObj *);

void updateAreaScroll(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.playerLanded) {
            o->step++;
            o->subtype = 0;
        }
        cameraModeHandler6B(o);
        ((short *)o->d38)[1] = 0;
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (GAME.selectedSection) {
        case 1:
            if ((unsigned short)((*(unsigned short*)&PLAYER.obj.a.p.whole) - 0x2e7) < 0x80 && PLAYER.obj.b.p.whole >= 0x5b) {
                tryStartItemInteraction(o, 1, 0);
            } else if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0x3ad && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b) {
                tryStartItemInteraction(o, 1, 1);
            }
            break;
        case 2:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x1e && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b)
                tryStartItemInteraction(o, 1, 0);
            break;
        }
        break;
    case 2:
        cameraEventScrollHandler(o);
        break;
    }
}
