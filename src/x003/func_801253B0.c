// FUNC 801253b0 380 X003
// MATCHING 801253b0 380
#include "TOBJ.H"

extern void *D_80138E94;
extern void *D_80138E98;
extern void *D_80138E9C;

void func_801253B0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velV = 0x200;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velY = 0x20;
        o->state++;
        break;
    case 1:
        o->y.raw -= o->velV << 8;
        o->velV -= o->velY;
        if (o->velV < 0x100) {
            o->state++;
            o->anim = D_80138E98;
        }
        break;
    case 2:
        o->y.raw -= o->velV << 8;
        o->velV -= o->velY;
        if (o->velV < -0x100) {
            o->state++;
            o->anim = D_80138E9C;
        }
        break;
    case 3:
        o->y.raw -= o->velV << 8;
        o->velV -= o->velY;
        if (o->velV < -0x1ff) {
            o->state++;
        }
        break;
    case 4: {
        int d = o->d34;
        void *a = D_80138E94;
        o->velV = 0x200;
        o->velY = 0x20;
        o->step = 0;
        o->state = 0;
        o->y.raw = d << 16;
        o->anim = a;
        break;
    }
    }
}
