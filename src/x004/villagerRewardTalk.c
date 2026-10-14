// FUNC 8012c884 364 X004
// MATCHING 8012c884 364
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_80135784[];
extern void *D_8013577C[];
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);

void villagerRewardTalk(TObj *o)
{
    TObj *e;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->d90 = showMessageBoxDirect(5, 13, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135784[0];
            FUN_8001fe6c(o);
        }
        break;
    case 2:
        FUN_8001fec0(o);
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->wac = 0;
            o->anim = D_8013577C[0];
            o->state = 0;
        }
        break;
    }
}
