// FUNC 801196f8 320 X003
// MATCHING 801196f8 320
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

extern char D_8013924C[];
extern char D_80139254[];
extern char D_80139160[];
extern char D_80139168[];
extern char D_8013925C[];
extern char D_80139200[];
extern char D_801391F0[];
static void *D_80138EDC[7] = {
    D_8013924C, D_80139254, D_80139160, D_80139168,
    D_8013925C, D_80139200, D_801391F0,
};
extern void ObjListPush_1F800220(TObj *);
extern void func_801191C4(TObj *);
extern void func_801193D0(TObj *);
extern void ObjFree(TObj *);

void func_801196F8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 0x81;
        if (GAME.selectedSection == 3) {
            o->w1e = 8;
            o->w08 = 0x780c;
        } else {
            o->w1e = 9;
            o->w08 = 0x780b;
        }
        o->d3c = SPAD.i2d4;
        o->anim = D_80138EDC[o->b0c];
        o->d->p.whole -= o->subtype * 4;
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800220(o);
        if (GAME.selectedSection != 3 && GAME.selectedSection < 4) func_801191C4(o);
        else func_801193D0(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
