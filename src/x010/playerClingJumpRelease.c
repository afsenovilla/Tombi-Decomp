// FUNC 8011bd5c 336 X010
// MATCHING 8011bd5c 336
#include "gv2.h"
#include "spad.h"
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"

extern TObj *D_8009F0EC;
extern TObj *D_8009C330;

void playerClingJumpRelease(TObj *o)
{
    int e;
    Fix16 *h;

    if (SPAD.s1fc & SPAD.s3c6) {
        GV2.bb0 = 0;
        ((unsigned char *)&o->wa8)[1] = 0;
        if (GV2.bb2 - 5 < 3) D_8009F0EC->active = 1;
        o->b9c = 1;
        o->timer = 10;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        if (D_8009F0EC->d94) {
            e = D_8009F0EC->d94;
            if (!((TObj *)e)->d94) {
                ((TObj *)e)->active = 1;
            } else {
            loop:
                ((TObj *)e)->active = 3;
                e = ((TObj *)e)->d94;
                if (((TObj *)e)->d94) goto loop;
                ((TObj *)e)->active = 1;
            }
        }
        h = o->h;
        e = h->p.whole;
        if (o->animFrame & 1) h->p.whole = e + 0x10; else h->p.whole = e - 0x10;
        if (GAME.area00_eventControl & 0x40) {
            if (*(volatile unsigned short *)((unsigned short*)&GV6.s70) & SPAD.s3c4) o->ba7 = 1;
        }
        D_8009C330->b04 = 0;
        o->step = 2;
        o->state = 0;
    }
}
