// FUNC 8011a6c4 432 X003
// MATCHING 8011a6c4 432
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"


int stepAreaSwitch(TObj *o)
{
    switch (o->state) {
    case 0:
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x20) > 0x40) return 0;
        if ((unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0x30) > 0x60) break;
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 1) return 1;
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2) {
            o->d30 = 0;
            o->state++;
            return 2;
        }
        if (GVARS.playerLanded) return 0;
        o->state = 3;
        o->d30 = 0;
        o->h->p.whole -= 0x20;
        return 2;
    case 1:
        o->h->raw += -0x18000;
        o->d30 += -0x18000;
        if ((o->d30 >> 16) < -0x1f) o->state++;
        break;
    case 2:
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 3)) == 2) o->state++;
        break;
    case 3:
        if (!(*(unsigned char*)((char*)&PLAYER.obj.da0 + 3))) o->state++;
        break;
    case 4:
        o->h->raw += 0x18000;
        o->d30 += 0x18000;
        if (o->d30 >= 0) o->state = 0;
        break;
    }
    return 0;
}
