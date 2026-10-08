// FUNC 80048c24 200 MAIN0
// MATCHING 80048c24 200
#include "TOBJ.H"

void func_80048C24(TObj *o, TObj *p)
{
    if (o->type == 0x22 || o->type == 0x2c) {
        if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) < 0x5b) {
            if ((unsigned short)(p->box0 + (o->h->p.whole - p->h->p.whole)) <= p->box1) {
                if ((unsigned short)(p->box2 + (o->y.p.whole - p->y.p.whole)) <= p->box3) {
                    o->active = 2;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                    p->state++;
                }
            }
        }
    }
}
