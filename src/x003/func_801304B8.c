// FUNC 801304b8 396 X003
// MATCHING 801304b8 396
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_8013A3B4[];
extern void *D_8013A368[];
extern TObj *showMessageBoxDirect(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void func_801304B8(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            PLAYER.obj.b04 = 5;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            o->d90 = (int)showMessageBoxDirect(2, 0xd, &o->a);
            o->wac = 3;
            o->animFrame = o->b68 & 1;
            o->anim = D_8013A3B4[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            o->wac = 0;
            o->anim = D_8013A368[0];
            o->state = 0;
            if (((unsigned char*)&GCD.bd00a)[0]) {
                GVARS.b3f = 0;
                GVARS.b42 = 0;
                PLAYER.obj.b04 = 1;
                PLAYER.obj.step = 0;
                PLAYER.obj.state = 0;
            } else {
                ((unsigned char*)&GCD.bd00a)[0] = 1;
            }
        }
        break;
    }
}
