// FUNC 8012c574 420 X004
// MATCHING 8012c574 420
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_80135784[];
extern void *D_8013577C[];
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);

void mermaidSightingTalk(TObj *o)
{
    int n, m;
    TObj *e;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        if (((unsigned char*)&GCD.bce22)[0] && ((unsigned char*)&GCD.bce3d)[0] != 0xff)
            o->state = 3;
        else
            o->state = 1;
        break;
    case 1:
        if (o->b68) {
            m = 5;
            n = 4;
            goto common;
        }
        break;
    case 3:
        if (o->b68) {
            m = 5;
            n = 5;
        common:
            o->state++;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->d90 = showMessageBoxDirect(m, n, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135784[0];
            FUN_8001fe6c(o);
        }
        break;
    case 2:
    case 4:
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
