// FUNC 80117de0 440 X001
// MATCHING 80117de0 440
#include "TOBJ.H"

extern void FUN_80020078(TObj *, int);
extern void FUN_8001888c(TObj *);

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
        FUN_80020078(o, 0x60);
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
        FUN_8001888c(o);
        break;
    }
}
