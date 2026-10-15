// FUNC 8011afa8 400 X004
// MATCHING 8011afa8 400
// VERSION SLPS_01144 x04 80119DD8 400
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x004 8011AFA8 (x004/stepAreaSwitch.c): 48 -> 32, 73 -> 65.
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern signed char D_8009D2B0;

int stepAreaSwitch(TObj *o)
{
    switch (o->state) {
    case 0:
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x20) >= 0x41) return 0;
        if ((unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0x20) >= 0x41) return 0;
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 1) return 1;
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2) {
            o->velY = 0;
            o->state++;
            return 2;
        }
        if (GVARS.b3a) return 0;
        if (D_8009D2B0 >= 3) break;
        o->state = 3;
        o->velY = 0xa00;
        o->d88 += 0xa00;
        return 2;
    case 1:
        o->d88 = (o->d88 - 0x20) & 0xfff;
        o->velY = (o->velY - 0x20) & 0xfff;
        if (o->velY > 0xa00) return 0;
        o->state++;
        break;
    case 3:
        o->d88 = (o->d88 + 0x20) & 0xfff;
        o->velY = (o->velY + 0x20) & 0xfff;
        if (o->velY != 0) return 0;
        o->state++;
        break;
    case 4:
        if (GVARS.b3a == 0) return 0;
        o->state = 0;
        break;
    case 2:
        break;
    }
    return 0;
}
#else
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern signed char D_8009D2B0;

int stepAreaSwitch(TObj *o)
{
    switch (o->state) {
    case 0:
        if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x30) >= 0x49) return 0;
        if ((unsigned short)(o->y.p.whole - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0x20) >= 0x41) return 0;
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 1) return 1;
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2) {
            o->velY = 0;
            o->state++;
            return 2;
        }
        if (GVARS.b3a) return 0;
        if (D_8009D2B0 >= 3) break;
        o->state = 3;
        o->velY = 0xa00;
        o->d88 += 0xa00;
        return 2;
    case 1:
        o->d88 = (o->d88 - 0x20) & 0xfff;
        o->velY = (o->velY - 0x20) & 0xfff;
        if (o->velY > 0xa00) return 0;
        o->state++;
        break;
    case 3:
        o->d88 = (o->d88 + 0x20) & 0xfff;
        o->velY = (o->velY + 0x20) & 0xfff;
        if (o->velY != 0) return 0;
        o->state++;
        break;
    case 4:
        if (GVARS.b3a == 0) return 0;
        o->state = 0;
        break;
    case 2:
        break;
    }
    return 0;
}
#endif
