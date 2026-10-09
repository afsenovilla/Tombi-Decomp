// FUNC 80129b2c 640 X001
// MATCHING 80129b2c 640
#include "TOBJ.H"

extern void *D_8013FC84[];
extern short D_1F80027E, D_1F800284;
extern void AnimLoadDuration(TObj *o);
extern int AnimAdvance(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);
extern void FUN_8001e4f0(int);
extern void func_80128110(TObj *);

void func_80129B2C(TObj *o)
{
    int r;
    short t;

    switch (o->state) {
    case 0:
        o->b9c = 2;
        if (o->d8c == 0) o->state = 3;
        else if (o->d8c >= 0x80) o->state = 1;
        else o->state = 2;
        break;
    case 1:
        o->d8c += 4;
        if (o->d8c >= 0x100) {
            o->state = 3;
            o->d8c = 0;
        }
        o->velV += 8;
        o->y.raw += o->velV << 8;
        break;
    case 2:
        o->d8c -= 4;
        if (o->d8c <= 0) {
            o->state = 3;
            o->d8c = 0;
        }
        o->velV += 8;
        o->y.raw += o->velV << 8;
        break;
    case 3:
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->b69 = 0;
        o->b6a = 1;
        o->state++;
    case 4:
        if (o->b6a == 2) {
            *(signed char *)&o->b0f = -11;
            FUN_8001e4f0(0x5f);
            o->box0 = 0xc;
            o->box1 = 0x18;
            o->step = 6;
            o->state = 0;
            o->wac = 5;
            o->anim = D_8013FC84[0];
            AnimLoadDuration(o);
            break;
        }
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
            o->box0 = 0xc;
            o->box1 = 0x18;
            o->b9c = 0;
            o->b6a = 0;
            o->b69 &= 1;
            func_80128110(o);
            o->state = 0;
        }
        break;
    }
}
