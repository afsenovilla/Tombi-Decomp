// FUNC 80121d2c 304 X009
/* score 28: whole function (covers csv piece 80121D6C). Only case 0 differs: o dies at the call-argument copy
   (a0 = o), so local-alloc's optimize_reg_copy substitutes a0 for o in every store of the block; the game keeps
   s0 for the stores and uses a0 only for the anim store in the jal delay slot. Tried: setAnim inline, block-local
   copies of o, raw store, return instead of break, statement orders. */
#include "TOBJ.H"
extern char D_80077CF4[];
extern void *D_8012E9F8;
extern void FUN_8001fe6c(TObj *);
extern short FUN_80040278(TObj *, short, short);
extern void FUN_8001fb20(TObj *);

static __inline__ void setAnim(TObj *p, void *a)
{
    p->anim = a;
    FUN_8001fe6c(p);
}

static __inline__ short land(TObj *o)
{
    if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_80121D2C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->active = 1;
        o->b6a = 1;
        o->timer = 60;
        o->wac = 0;
        o->state++;
        o->anim = D_8012E9F8;
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->visible != 0) o->state++;
        break;
    case 2:
        if (o->visible == 0) break;
        o->step++;
        o->state = 0;
        if (!land(o)) FUN_8001fb20(o);
        break;
    }
}
