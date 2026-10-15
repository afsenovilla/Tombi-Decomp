// FUNC 8011c708 392 X004
// MATCHING 8011c708 392
#include "spad.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
extern void FUN_80018cf0(TObj *);
extern void FUN_80018838(TObj *);

void updateSceneryModel(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        switch (o->subtype) {
        case 0:
            break;
        case 1:
            o->d84 = 0;
            o->d88 = 0xc00;
            o->d8c = 0;
            break;
        case 2:
            o->d84 = 0;
            o->d88 = 0x800;
            o->d8c = 0;
            break;
        }
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            if (GAME.disableSelectMenu == 0 && GAME.selectedSection == 1) break;
            if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x32a) {
                if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x92) (*(short **)&PLAYER.obj.h)[1] = 0x92;
            }
            o->visible = 1;
            FUN_80018cf0(o);
            break;
        case 1:
            if (GAME.selectedSection == 2 && (*(short*)((char*)&SPAD.i174 + 2)) < 0x456) break;
            o->visible = 1;
            FUN_80018cf0(o);
            break;
        case 2:
            o->visible = 1;
            FUN_80018cf0(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
