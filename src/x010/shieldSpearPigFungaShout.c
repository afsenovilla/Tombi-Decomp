// FUNC 8012cb50 408 X010
// MATCHING 8012cb50 408
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern char D_80132858[];
extern char D_8013286C[];
extern char D_8013287C[];
extern char D_80132884[];
static void *D_8013238C[4] = {
    D_80132858, D_8013286C, D_8013287C, D_80132884,
};
extern TObj *showMessageBoxDirect(int, int, Fix16 *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern unsigned int FUN_8001f9e0(void);

void shieldSpearPigFungaShout(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68 == 0)
            break;
        o->state++;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->d90 = (int)showMessageBoxDirect(2, 2, &o->a);
        o->wac = 0x1f;
        o->animFrame = o->b68 & 1;
        o->anim = D_8013238C[0];
        FUN_8001fe6c(o);
        break;
    case 2:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        PLAYER.obj.b04 = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->wac = 0x1f;
        o->anim = D_8013238C[0];
        o->state++;
        break;
    case 3:
        o->b6b = FUN_8001f9e0() & 1;
        o->state = 0;
        break;
    }
}
