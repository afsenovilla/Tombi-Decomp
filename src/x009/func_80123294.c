// FUNC 80123294 348 X009
// MATCHING 80123294 348
#include "TOBJ.H"

extern char D_80077CF4[];
extern void *D_8012EA40[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void FUN_8001fb20(TObj *);

static __inline__ int Ground(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_80123294(TObj *o)
{
    switch (o->state) {
    case 0:
        o->wb4 = 3;
        o->b6a = 1;
        o->movetab = D_80077CF4;
        o->wac = 0x12;
        o->state++;
        o->anim = D_8012EA40[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (AnimAdvance(o)) {
            o->timer = 0x78;
            o->state++;
        }
        break;
    case 2:
        if (--o->timer == 0) {
            o->step = 3;
            o->state = 0;
        }
        break;
    }
    if (o->visible && !Ground(o)) FUN_8001fb20(o);
}
