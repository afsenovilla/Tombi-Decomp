// FUNC 80127d54 368 X004
// MATCHING 80127d54 368
#include "TOBJ.H"

extern void *D_80134D84;
void FUN_8001fe6c(TObj *o);
int FUN_8001fec0(TObj *o);

void func_80127D54(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->velV = 0x600;
        o->velH = -0x200;
        o->velX = 0x10;
        o->anim = D_80134D84;
        FUN_8001fe6c(o);
        break;
    case 1:
        o->y.raw += o->velV << 8;
        o->h->raw += o->velH << 8;
        o->velH += o->velX;
        if (o->velH == 0) {
            o->animFrame = 1;
            o->state++;
        }
        break;
    case 2:
        FUN_8001fec0(o);
        if (o->y.p.whole < -0x3c) {
            o->y.raw += o->velV << 8;
        } else {
            o->d88 = 0;
            o->state++;
        }
        break;
    case 3:
        o->h->p.whole = o->d30;
        o->y.p.whole = o->d34;
        o->d->p.whole = o->d38;
        o->step = 0;
        o->state = 0;
        break;
    }
}
