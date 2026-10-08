// FUNC 80117a9c 340 X009
// MATCHING 80117a9c 340
#include "TOBJ.H"

extern unsigned char D_8009D2B1;
extern void FUN_8001e76c(int, int, int, int);
extern void FUN_8001e4f0(int);

void func_80117A9C(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_8009D2B1 == 0) {
            o->state = 1;
            o->b6a = 3;
            o->timer = 0x20;
            o->velV = 0x100;
            break;
        }
        switch (o->b6a) {
        case 1:
            break;
        case 2:
            o->state = 2;
            o->timer = 0x40;
            o->velV = 0x100;
            FUN_8001e76c(0x94, 1, -8, 0x78);
            FUN_8001e4f0(0x92);
            break;
        }
        break;
    case 1:
        o->y.raw += o->velV << 8;
        if (--o->timer == -1) {
            o->step = 0;
            o->state = 0;
        }
        break;
    case 2:
        o->y.raw += o->velV << 8;
        if (--o->timer == -1) {
            o->step = 2;
            o->state = 0;
        }
        break;
    }
}
