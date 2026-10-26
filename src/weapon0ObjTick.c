// FUNC 80033040 328 MAIN0
// MATCHING 80033040 328
#include "plx.h"
#include "TOBJ.H"
extern unsigned char *WEAPON_STATE;
extern unsigned char *PLAYER_EXT;
extern signed char D_800A611B;
extern void weapon0Init(TObj *);
extern void ObjCullRegister(TObj *);
extern void weapon0Swing(TObj *);
extern void freeObjectLayer1(TObj *);
void weapon0ObjTick(TObj *o)
{
    if (PLX.b60ff != 0)
        o->b04 = 2;
    switch (o->b04) {
    case 0:
        weapon0Init(o);
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            weapon0Swing(o);
            break;
        case 1:
            weapon0Swing(o);
            break;
        case 2:
            weapon0Swing(o);
            break;
        }
        break;
    case 2:
        WEAPON_STATE[0xc] = 0;
        *PLAYER_EXT = 0;
        if (--D_800A611B < 0)
            D_800A611B = 0;
        o->b04 = 3;
        break;
    case 3:
        freeObjectLayer1(o);
        break;
    }
}
