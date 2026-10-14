// FUNC 8012c3d0 420 X004
// MATCHING 8012c3d0 420
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_80135750[];
extern void *D_80135748[];
extern TObj *showMessageBoxDirect(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void homeSweetHomeTalk(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        if (((unsigned char*)&GCD.bce22)[0] && ((unsigned char*)&GCD.bce3d)[0] != 0xff) o->state = 3;
        else o->state = 1;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->d90 = (int)showMessageBoxDirect(5, 7, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135750[0];
            AnimLoadDuration(o);
        }
        break;
    case 3:
        if (o->b68) {
            o->state++;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->d90 = (int)showMessageBoxDirect(5, 8, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135750[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
    case 4:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->wac = 0;
            o->anim = D_80135748[0];
            o->state = 0;
        }
        break;
    }
}
