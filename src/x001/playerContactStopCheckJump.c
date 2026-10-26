// FUNC 80121a10 328 X001
// MATCHING 80121a10 328
#include "gv2.h"
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj o; char pc0[6]; unsigned char bc6; } PL;
extern TObj *D_8009C330;
extern unsigned int D_8009C984[];
extern volatile unsigned short PAD_PRESSED[];
extern void playerSelectGroundAnim(PL *);
extern void playSFX(int);
extern void spawnGroundDebris(int, int, int, int);

void playerContactStopCheckJump(PL *o)
{
    if (D_8009C330->active == 0 && o->bc6 == 0) {
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
    if (SPAD.s1fc & SPAD.s3c6) {
        GV2.bb0 = 0;
        playSFX(0x3e);
        spawnGroundDebris(o->o.h->p.whole, o->o.y.p.whole, o->o.d->p.whole, 0);
        spawnGroundDebris(o->o.h->p.whole, o->o.y.p.whole, o->o.d->p.whole, 0);
        o->o.b9c = 1;
        o->o.b04 = 1;
        o->o.y.p.whole -= 8;
        if ((D_8009C984[0] & 0x40) && (PAD_PRESSED[0] & SPAD.s3c4)) o->o.ba7 = 1;
        o->o.step = 2;
        o->o.state = 0;
    }
}
