// near-twin of playerClingJumpRelease (PAL)
// VERSION SLPS_01144 x10 8011A8C8 328
#if defined(VERSION_SLPS_01144)
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8003C4;
extern unsigned short D_1F8003C6;
extern unsigned int D_8009E52C;
extern unsigned short D_8009F200;
extern unsigned char D_8009EE5A;
#define GRABBED_OBJECT D_800A0C7C
#define PLAYER_EXT D_8009DEE0
#include "gv2.h"
#include "spad.h"
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"

extern TObj *GRABBED_OBJECT;
extern TObj *PLAYER_EXT;

void func_8011A8C8_slps01144(TObj *o)
{
    int e;
    Fix16 *h;

    if (D_1F8001FC & D_1F8003C6) {
        ((unsigned char *)&o->wa8)[1] = 0;
        if (D_8009EE5A - 5 < 3) GRABBED_OBJECT->active = 1;
        o->b9c = 1;
        o->timer = 10;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        if (GRABBED_OBJECT->d94) {
            e = GRABBED_OBJECT->d94;
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
        if (D_8009E52C & 0x40) {
            if (*(volatile unsigned short *)((unsigned short*)&D_8009F200) & D_1F8003C4) o->ba7 = 1;
        }
        PLAYER_EXT->b04 = 0;
        o->step = 2;
        o->state = 0;
    }
}
#endif
