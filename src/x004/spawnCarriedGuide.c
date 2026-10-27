// FUNC 8012b388 176 X004
// MATCHING 8012b388 176
#include "spad.h"
#include "player.h"
#include "TOBJ.H"
extern int PLAYER_Y_FIX[], D_800A6050[];
extern void *D_80134CE0;
extern TObj *allocObjectLayer2(void);
extern void AnimLoadDuration(TObj *);

void spawnCarriedGuide(void)
{
    TObj *e = allocObjectLayer2();

    if (e) {
        e->active = 1;
        e->type = 0x33;
        e->a.raw = ((int*)&PLAYER.obj.a.raw)[0];
        e->y.raw = PLAYER_Y_FIX[0];
        e->b.raw = D_800A6050[0];
        e->animFrame = ((unsigned short*)&PLAYER.obj.animFrame)[0] & 1;
        e->w1e = 6;
        e->subtype = 0;
        e->b0c = 0;
        e->b0a = 2;
        e->d8c = 0;
        e->d3c = ((int*)&SPAD.i2d4)[0];
        e->anim = D_80134CE0;
        AnimLoadDuration(e);
    }
}
