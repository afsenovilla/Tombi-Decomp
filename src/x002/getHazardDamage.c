// FUNC 800ff404 524 X002
// MATCHING 800ff404 524
#include "TOBJ.H"
#include "gamestate.h"


short getHazardDamage(TObj *o, short r)
{
    switch (GAME.displayExpBar & 3) {
    case 0:
        r = 1;
        if ((*(int*)&GAME.selectedArea) == 0x20003) r = 2;
        if ((*(int*)&GAME.selectedArea) == 10) r = (o->h->p.whole < 0xc80) * 2;
        if ((*(int*)&GAME.selectedArea) == 0x3000a) r = 2;
        break;
    case 1:
        r = 2;
        if ((*(int*)&GAME.selectedArea) == 0x20000) r = 1;
        if ((*(int*)&GAME.selectedArea) == 0x30001) r = 1;
        if ((*(int*)&GAME.selectedArea) == 0x20003) r = 0;
        if ((*(int*)&GAME.selectedArea) == 10 && o->h->p.whole > 0xc80) r = 0;
        if ((*(int*)&GAME.selectedArea) == 0x3000a) r = 2;
        break;
    case 2:
        r = (*(int*)&GAME.selectedArea) == 0x20000;
        if ((*(int*)&GAME.selectedArea) == 0x30001) r = 1;
        if ((*(int*)&GAME.selectedArea) == 0x20003) r = 6;
        if ((*(int*)&GAME.selectedArea) == 10 && o->h->p.whole < 0xc80) r = 2;
        if ((*(int*)&GAME.selectedArea) == 0x3000a) r = 1;
        break;
    case 3:
        r = 2;
        if ((*(int*)&GAME.selectedArea) == 0x20003) r = 4;
        if ((*(int*)&GAME.selectedArea) == 10 && o->h->p.whole < 0xc80) r = 4;
        if ((*(int*)&GAME.selectedArea) == 0x3000a) r = 4;
        break;
    }
    return r;
}
