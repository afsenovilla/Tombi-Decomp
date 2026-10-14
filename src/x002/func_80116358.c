// FUNC 80116358 644 X002
// MATCHING 80116358 644
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
extern void (*D_80079AD0[])(TObj *);
extern void func_80027810(TObj *);
extern void tryStartItemInteraction(TObj *, int, int);
extern void FUN_8002a4d0(TObj *);

void func_80116358(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.b3a != 0) {
            o->step++;
            o->subtype = 0;
        }
        ((short *)o->d38)[1] = 0;
        func_80027810(o);
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
            if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0x128 && (*(short*)((char*)&SPAD.i16c + 2)) >= -59)
                tryStartItemInteraction(o, 1, 0);
            else if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x2bd)
                tryStartItemInteraction(o, 1, 2);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
