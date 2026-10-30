// near-twin of playerClingJumpRelease (PAL)
// VERSION SLPS_01144 x01 801211D0 324
#if defined(VERSION_SLPS_01144)
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8003C4;
extern unsigned short D_1F8003C6;
extern unsigned char D_8009EE5A;
#define GRABBED_OBJECT D_800A0C7C
#define PLAYER_EXT D_8009DEE0
#define GAME_AREA00_EVENT_CONTROL D_8009E52C
#define PAD_PRESSED D_8009F200
#include "gv2.h"
#include "spad.h"
#include "TOBJ.H"
typedef struct L { unsigned char active; char p[0x94 - 1]; struct L *next; } L;
extern TObj *PLAYER_EXT;
extern L *GRABBED_OBJECT;
extern unsigned int GAME_AREA00_EVENT_CONTROL[];
extern volatile unsigned short PAD_PRESSED[];

void func_801211D0_slps01144(TObj *o)
{
    L *q, *n;

    if (!(D_1F8001FC & D_1F8003C6)) return;
    ((unsigned char *)&o->wa8)[1] = 0;
    if (D_8009EE5A - 5 < 3) GRABBED_OBJECT->active = 1;
    PLAYER_EXT->b04 = 0;
    o->b9c = 1;
    o->timer = 10;
    o->velH = 0;
    o->velV = 0;
    o->velX = 0;
    o->velY = 0;
    n = GRABBED_OBJECT->next;
    if (n != 0) {
        q = n;
        if (q->next == 0) {
            q->active = 1;
        } else {
        loop:
            q->active = 3;
            q = q->next;
            if (q->next != 0) goto loop;
            q->active = 1;
        }
    }
    o->h->p.whole += (o->animFrame & 1) ? 0x10 : -0x10;
    if ((GAME_AREA00_EVENT_CONTROL[0] & 0x40) && (PAD_PRESSED[0] & D_1F8003C4)) o->ba7 = 1;
    o->step = 2;
    o->state = 0;
}
#endif
