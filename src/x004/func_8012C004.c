// FUNC 8012c004 420 X004
// MATCHING 8012c004 420
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_80135748[];
extern void *D_80135750[];
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8012C004(TObj *o)
{
    TObj *p;
    int m, n;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        if (((unsigned char*)&GCD.bce24)[0] != 0 && ((unsigned char*)&GCD.bce24)[0] != 0xff) {
            o->state = 3;
            break;
        }
        o->state++;
        break;
    case 1:
        if (o->b68 == 0) break;
        m = 5;
        n = 9;
        goto talk;
    case 3:
        if (o->b68 == 0) break;
        m = 5;
        n = 10;
    talk:
        o->state++;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->d90 = showMessageBoxDirect(m, n, &o->a);
        o->wac = 2;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_80135750[0]);
        break;
    case 2:
    case 4:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->state = 0;
        o->wac = 0;
        o->anim = D_80135748[0];
        break;
    }
}
