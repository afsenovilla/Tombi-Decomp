// FUNC 801225bc 1008 X009
// MATCHING 801225bc 1008
#include "TOBJ.H"
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001faf4(TObj *);
extern void FUN_8001fb20(TObj *);
extern void playObjectSfx(TObj *, int);
extern void func_80121964(TObj *);
extern void func_801224AC(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);
extern void *D_8012E9F8[];
extern void *D_8012EA10[];
extern char D_80077CE8[];

static __inline__ short blocked(TObj *o)
{
    int d;
    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) {
        return 1;
    }
    d = (o->animFrame & 1) ? -16 : 16;
    return func_8004065C(o, o->h->p.whole + d, o->y.p.whole, o->animFrame) != 0;
}

static __inline__ short ground(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 16)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_801225BC(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->subtype & 0x80) {
            o->wac = 15;
            o->state = 4;
        } else {
            o->movetab = D_80077CE8;
            o->b9c = 1;
            o->velV = -0x200;
            o->wac = 4;
            o->state++;
            playObjectSfx(o, 0x99);
        }
        o->anim = D_8012E9F8[o->wac];
        AnimLoadDuration(o);
        break;
    case 1:
        FUN_8001faf4(o);
        blocked(o);
        func_80121964(o);
        AnimAdvance(o);
        o->velV += 0x38;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        FUN_8001faf4(o);
        blocked(o);
        func_80121964(o);
        AnimAdvance(o);
        o->velV += 0x38;
        if (o->velV > 0x600) {
            o->velV = 0x600;
        }
        o->y.raw += o->velV << 8;
        if (ground(o)) {
            ObjSetFacingToPlayer(o);
            o->b9c = 0;
            o->wac = 6;
            o->state++;
            o->anim = D_8012EA10[0];
            AnimLoadDuration(o);
        }
        break;
    case 3:
        if (o->visible && !ground(o)) {
            FUN_8001fb20(o);
        }
        if (AnimAdvance(o)) {
            o->state = 0;
        }
        func_801224AC(o);
        break;
    case 4:
        AnimAdvance(o);
        if (o->visible && !ground(o)) {
            FUN_8001fb20(o);
        }
        func_801224AC(o);
        break;
    }
    if (o->timer) {
        o->timer--;
    }
}
