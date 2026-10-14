// JP x01 80116DB8 440
// Japanese version of PAL x001 80117DE0 (x001/func_80117DE0.c): 96 -> 40.
#include "TOBJ.H"

extern void cullObjectToDrawList(TObj *, int);
extern void freeObjectLayer8(TObj *);

void func_80117DE0(TObj *o)
{
    TObj *p;

    switch (o->b04) {
    case 0:
        o->box0 = 0x33;
        o->box1 = 0x66;
        o->box2 = 0x1a;
        o->box3 = 0x34;
        o->b04++;
        break;
    case 1:
        cullObjectToDrawList(o, 0x28);
        switch (o->subtype) {
        case 0:
            switch (o->step) {
            case 0:
                o->velY = 0;
                o->step++;
                break;
            case 1:
                break;
            case 2:
                o->y.raw += o->velY << 8;
                o->velY -= 0x30;
                if (o->velY < -0x780) o->velY = -0x780;
                break;
            }
            break;
        case 1:
            p = (TObj *)o->d90;
            o->h->p.whole = p->h->p.whole - 0x1e;
            o->y.p.whole = p->y.p.whole - 10;
            o->d->p.whole = p->d->p.whole - 0x32;
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
