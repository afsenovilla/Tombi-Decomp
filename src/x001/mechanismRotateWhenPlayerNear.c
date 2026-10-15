// FUNC 8011bc6c 280 X001
// MATCHING 8011bc6c 280
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

void mechanismRotateWhenPlayerNear(TObj *o)
{
    switch (o->state) {
    case 0:
        switch (GAME.selectedSection) {
        case 0:
        case 1:
            o->state++;
            break;
        case 2:
            o->d88 = 0x800;
            o->state = 4;
            break;
        }
        break;
    case 1:
        if ((*(short*)((char*)&SPAD.i16c + 2)) >= -0x140 && (unsigned short)((*(short*)((char*)&SPAD.i168 + 2)) - 0xb23) < 0x95)
            o->state++;
        break;
    case 2:
        o->d88 = (o->d88 + 8) & 0xfff;
        if (o->d88 >= 0x600) o->state++;
        break;
    case 3:
        if (o->visible == 0 && ((TObj *)o->d94)->visible == 0) {
            o->d88 = 0;
            o->state = 1;
        }
        break;
    case 4:
        break;
    }
}
