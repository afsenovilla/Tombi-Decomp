// FUNC 8012c0dc 324 X010
// MATCHING 8012c0dc 324
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_801322F4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern TObj *showMessageBoxDirect(int, int, void *);

void func_8012C0DC(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->state++;
        ObjSetFacingToPlayer(o);
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wac = 5;
        o->anim = D_801322F4[0];
        AnimLoadDuration(o);
        o->d90 = (int)showMessageBoxDirect(2, 0xa, &o->a);
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->state++;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        }
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        break;
    }
}
