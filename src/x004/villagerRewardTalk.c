// FUNC 8012c884 364 X004
// MATCHING 8012c884 364
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern char D_80135908[];
extern char D_8013591C[];
extern char D_80135924[];
extern char D_8013592C[];
extern char D_80135934[];
extern char D_8013593C[];
extern char D_8013596C[];
extern char D_80135974[];
extern char D_80135984[];
extern char D_8013598C[];
extern char D_8013599C[];
static void *D_80135784[11] = {
    D_80135908, D_8013591C, D_80135924, D_8013592C,
    D_80135934, D_8013593C, D_8013596C, D_80135974,
    D_80135984, D_8013598C, D_8013599C,
};
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
