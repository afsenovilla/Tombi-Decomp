// FUNC 80117bf0 344 X009
// MATCHING 80117bf0 344
#include "TOBJ.H"
extern unsigned char D_8009D2B1;
extern void FUN_8001e76c(int, int, int, int);
extern void FUN_8001e4f0(int);

void func_80117BF0(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_8009D2B1 == 0) {
            o->state = 1;
            o->b6a = 3;
            o->timer = 0x20;
            o->velV = 0x100;
        } else {
            if (o->b6a == 2) {
            } else if (o->b6a == 1) {
                o->state = 2;
                o->timer = 0x40;
                o->velV = 0x100;
                FUN_8001e76c(0x93, 1, 8, 0x78);
                FUN_8001e4f0(0x91);
            }
        }
        break;
    case 1:
        o->y.raw -= o->velV << 8;
        if (--o->timer == -1) {
            o->step = 0;
            o->state = 0;
        }
        break;
    case 2:
        o->y.raw -= o->velV << 8;
        if (--o->timer == -1) {
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
