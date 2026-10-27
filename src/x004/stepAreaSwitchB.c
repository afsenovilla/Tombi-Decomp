// FUNC 8011b218 544 X004
// MATCHING 8011b218 544
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern signed char GAME_PLAYER_MODE;

int stepAreaSwitchB(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->subtype == 0xc) {
            if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x40) >= 0x61) return 0;
            if ((unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0x30) >= 0x61) return 0;
        } else {
            if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x30) >= 0x49) return 0;
            if ((unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0x20) >= 0x41) return 0;
        }
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 1) return 1;
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2) {
            o->d30 = 0;
            o->state++;
            return 2;
        }
        if (GVARS.playerLanded) return 0;
        if (GAME_PLAYER_MODE >= 3) break;
        o->state = 3;
        o->d30 = 0;
        o->h->p.whole -= 0x20;
        return 2;
    case 1:
        o->h->raw += -0x18000;
        o->d30 += -0x18000;
        if ((o->d30 >> 16) >= -0x1f) return 0;
        o->state++;
        break;
    case 3:
        o->h->raw += 0x18000;
        o->d30 += 0x18000;
        if ((o->d30 >> 16) < 0x20) return 0;
        o->state++;
        break;
    case 4:
        if (GVARS.playerLanded == 0) return 0;
        o->state = 0;
        break;
    case 2:
        break;
    }
    return 0;
}
