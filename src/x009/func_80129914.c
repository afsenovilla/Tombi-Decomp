// FUNC 80129914 440 X009
// MATCHING 80129914 440
#include "TOBJ.H"

extern unsigned char D_8009C942[], D_8009C93F[];
extern TObj PLAYER;
extern void FUN_800eeae4(TObj *, int, int);

void func_80129914(TObj *o)
{
    switch (o->step) {
    case 0:
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        PLAYER.animFrame = 1;
        PLAYER.b04 = 5;
        PLAYER.step = 0;
        PLAYER.state = 0;
        PLAYER.visible = 1;
        FUN_800eeae4(&PLAYER, 2, 0);
        PLAYER.b69 = 0;
        *(signed char *)&PLAYER.b0f = -8;
        o->step++;
    case 1:
        if (PLAYER.b69) {
            PLAYER.b04 = 5;
            PLAYER.step = 100;
            PLAYER.state = 0;
            PLAYER.y.p.whole = -0x1d0;
            FUN_800eeae4(&PLAYER, 2, 0);
            o->step++;
        }
        break;
    case 2:
        PLAYER.h->raw -= 0x10000;
        if (PLAYER.h->p.whole < 0x1068) {
            PLAYER.h->p.whole = 0x1068;
            FUN_800eeae4(&PLAYER, 0x2c, 0);
            o->step++;
        }
        break;
    case 3:
        PLAYER.d->raw -= 0x10000;
        break;
    }
}
