// FUNC 8012c6ec 436 X010
// MATCHING 8012c6ec 436
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_801322F0[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);

void func_8012C6EC(TObj *o)
{
    unsigned char s = o->state;
    char pad[16];

    switch (s) {
    case 0:
        o->state = s + 1;
        o->timer = 0x1e;
        o->wac = 4;
        o->anim = D_801322F0[0];
        AnimLoadDuration(o);
    case 1:
        AnimAdvance(o);
        if (--o->timer == -1) {
            PLAYER.obj.b04 = 6;
            PLAYER.obj.step = 5;
            PLAYER.obj.state = 0;
            PLAYER.obj.wb2 = 0;
            PLAYER.obj.velH = 0;
            PLAYER.obj.velV = 0;
            if (PLAYER.obj.animFrame & 1) PLAYER.obj.velX = 0xc0;
            else PLAYER.obj.velX = -0xc0;
            PLAYER.obj.b69 = 0;
            o->state++;
        }
        break;
    case 2:
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->b04 = 1;
        o->state = 0;
        if (GCD.bcde3 != 0xff) {
            if (GCD.bd07a) o->step = 2;
            else o->step = 1;
        } else {
            if (GCD.bcde2 == 0) o->step = 3;
            else if (GCD.bcde2 == 0xff) o->step = 6;
            else if (GCD.bce23) o->step = 5;
            else o->step = 4;
        }
        break;
    }
}
