// FUNC 8012d678 468 X010
// MATCHING 8012d678 468
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void FUN_800eea7c(TObj *, int, int);

void func_8012D678(TObj *o)
{
    switch (o->state) {
    case 0:
        if (PLAYER.y.p.whole >= -0xff && PLAYER.h->p.whole < 0xcb) {
            *(signed char *)&PLAYER.b0f = 8;
            GVARS.b3f = 1;
            PLAYER.y.p.whole = -0xe8;
            FUN_800eea7c(&PLAYER, 0x1e, 0);
            PLAYER.b04 = 5;
            PLAYER.d8c = 0;
            PLAYER.step = 0x65;
            PLAYER.state = 0;
            o->state++;
        }
        break;
    case 1:
        PLAYER.h->raw += -0x20000;
        if (PLAYER.h->p.whole < 0x96) {
            PLAYER.velY = 0;
            o->state++;
        }
        break;
    case 2:
        PLAYER.h->raw += -0x20000;
        PLAYER.y.raw += PLAYER.velY << 8;
        PLAYER.velY += 0x40;
        PLAYER.d8c++;
        if (PLAYER.y.p.whole >= -0x63) o->state = 9;
        break;
    case 9:
        GVARS.b3f = 0;
        break;
    }
}
