// FUNC 80121d2c 304 X009
// MATCHING 80121d2c 304
/* covers csv piece 80121D6C too. */
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
        do { o->anim = D_8012E9F8; FUN_8001fe6c(o); } while (0);
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
