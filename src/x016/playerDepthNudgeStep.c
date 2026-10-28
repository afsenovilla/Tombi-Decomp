// FUNC 80116e60 144 X016
// MATCHING 80116e60 144
#include "TOBJ.H"

extern unsigned char D_800A60E0;
extern TObj PLAYER;

void playerDepthNudgeStep(TObj *o)
{
    char pad[16];

    switch (o->state) {
    case 0:
        D_800A60E0 = 8;
        break;
    case 1:
        PLAYER.d->p.whole--;
        break;
    case 2:
        PLAYER.d->p.whole++;
        break;
    }
}
