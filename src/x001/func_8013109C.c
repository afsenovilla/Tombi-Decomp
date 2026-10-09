// FUNC 8013109c 604 X001
// MATCHING 8013109c 604
/* size 604: the csv piece 801312E4 (20 B) is this function's epilogue. */
#include "TOBJ.H"

extern short D_1F80027E;
extern void *D_8013E740[];
extern void *D_8013E74C[];
extern void *D_8013E750[];
extern unsigned char D_8009CEDF;
extern short FUN_80040278(TObj *, short, short);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001fab4(TObj *);

static __inline__ int land(TObj *o)
{
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->d84 = (-D_1F80027E << 2) & 0xff;
        return 1;
    }
    return 0;
}

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

void func_8013109C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->h->p.whole = 0x542;
        o->y.p.whole = -0x615;
        o->animFrame = 1;
        o->d8c = 0;
        o->timer = 10;
        o->wac = 0;
        o->state++;
        o->anim = D_8013E740[0];
        AnimLoadDuration(o);
    case 1:
        if (--o->timer == -1) {
            o->wac = 4;
            o->state++;
            setAnim(o, D_8013E750[0]);
        }
        break;
    case 2:
        AnimAdvance(o);
        if (--o->y.p.whole < -0x6dd) {
            o->y.p.whole = -0x6dc;
            o->wac = 3;
            o->state++;
            setAnim(o, D_8013E74C[0]);
        }
        break;
    case 3:
        AnimAdvance(o);
        FUN_8001fab4(o);
        land(o);
        if (o->h->p.whole < 0x4fe) o->state++;
        break;
    case 4:
        o->animFrame = 0;
        o->y.p.whole += 4;
        land(o);
        o->wac = 0;
        o->d8c = o->d84;
        o->state++;
        setAnim(o, D_8013E740[0]);
        { unsigned char *c = &D_8009CEDF; *c = *c + 0xfe; }
        break;
    case 5:
        break;
    }
}
