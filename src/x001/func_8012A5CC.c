// FUNC 8012a5cc 456 X001
// MATCHING 8012a5cc 456
#include "TOBJ.H"

extern void *D_8013FC88[];
extern short D_1F80027E, D_1F800284, D_1F80016A;
extern void AnimLoadDuration(TObj *o);
extern int AnimAdvance(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);

void func_8012A5CC(TObj *o)
{
    int r;
    short t;

    switch (o->state) {
    case 0:
        o->b9c = 2;
        o->b69 = 0;
        o->b6a = 0;
        o->d8c = 0;
        o->velV = 0;
        o->wac = 6;
        o->state++;
        o->anim = D_8013FC88[0];
        AnimLoadDuration(o);
        break;
    case 1:
        AnimAdvance(o);
        o->velV += 0x10;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->b69 == 1) {
            r = 1;
            o->wae = -1;
            o->wb2 = 0;
            o->b69 = 0;
            o->wba = 1;
        } else if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            r = 1;
            o->b69 = 0;
            o->wba = 0;
            t = D_1F80027E;
            o->wb2 = t;
            o->wae = D_1F800284;
            o->d8c = -(t << 2) & 0xff;
        } else {
            r = 0;
        }
        if (r) {
            o->step = 1;
            o->state = 0;
            o->b69 &= 1;
            if (o->w22) o->animFrame = o->h->p.whole > o->wb4;
            else o->animFrame = D_1F80016A < o->h->p.whole;
        }
        break;
    }
}
