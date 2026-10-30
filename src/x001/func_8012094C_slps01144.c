// near-twin of playerContactStopCheckJump (PAL)
// VERSION SLPS_01144 x01 8012094C 320
#if defined(VERSION_SLPS_01144)
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8003C4;
extern unsigned short D_1F8003C6;
#define PLAYER_EXT D_8009DEE0
#define playerSelectGroundAnim func_800EFBEC
#define playSFX func_8001E2D4
#define spawnGroundDebris func_800EC4DC
#define GAME_AREA00_EVENT_CONTROL D_8009E52C
#define PAD_PRESSED D_8009F200
#include "gv2.h"
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj o; char pc0[6]; unsigned char bc6; } PL;
extern TObj *PLAYER_EXT;
extern unsigned int GAME_AREA00_EVENT_CONTROL[];
extern volatile unsigned short PAD_PRESSED[];
extern void playerSelectGroundAnim(PL *);
extern void playSFX(int);
extern void spawnGroundDebris(int, int, int, int);

void func_8012094C_slps01144(PL *o)
{
    if (PLAYER_EXT->active == 0 && o->bc6 == 0) {
        playerSelectGroundAnim(o);
        o->o.b9d = 0;
        if (o->o.bbe & 0x20) {
            o->o.step = 0x1f;
            o->o.state = 1;
        } else {
            o->o.step = 0;
            o->o.state = 0;
        }
    }
    if (D_1F8001FC & D_1F8003C6) {
        playSFX(0x3e);
        spawnGroundDebris(o->o.h->p.whole, o->o.y.p.whole, o->o.d->p.whole, 0);
        spawnGroundDebris(o->o.h->p.whole, o->o.y.p.whole, o->o.d->p.whole, 0);
        o->o.b9c = 1;
        o->o.b04 = 1;
        o->o.y.p.whole -= 8;
        if ((GAME_AREA00_EVENT_CONTROL[0] & 0x40) && (PAD_PRESSED[0] & D_1F8003C4)) o->o.ba7 = 1;
        o->o.step = 2;
        o->o.state = 0;
    }
}
#endif
