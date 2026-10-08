// FUNC 801260cc 244 X009
// MATCHING 801260cc 244
#include "TOBJ.H"

extern void *D_8012EEC4[];
extern void *D_8012EEC8[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);

void func_801260CC(TObj *o)
{
    o->h->raw = ((TObj *)o->d90)->h->raw;
    o->y.raw = ((TObj *)o->d90)->y.raw;
    o->d->raw = ((TObj *)o->d90)->d->raw;
    switch (o->state) {
    case 3:
        o->wac = 1;
        o->state++;
        o->b0f++;
        o->anim = D_8012EEC4[0];
        AnimLoadDuration(o);
        break;
    case 5:
        o->wac = 2;
        o->anim = D_8012EEC8[0];
        AnimLoadDuration(o);
        break;
    case 2:
    case 4:
    case 6:
    case 7:
        AnimAdvance(o);
        break;
    default:
        o->b04 = 3;
        break;
    }
}
