// FUNC 8011792c 368 X009
// MATCHING 8011792c 368
#include "TOBJ.H"

extern unsigned char D_8009D2B1;
extern void FUN_8001e76c(int, int, int, int);
extern void playSFX(int);

void func_8011792C(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_8009D2B1) {
            if (o->b6a == 1) {
                o->state = 1;
                o->timer = 0x20;
                o->velV = 0x100;
                FUN_8001e76c(0x93, 1, 8, 0x78);
                playSFX(0x91);
            } else if (o->b6a == 2) {
                o->state = 2;
                o->timer = 0x20;
                o->velV = 0x100;
                FUN_8001e76c(0x94, 1, -8, 0x78);
                playSFX(0x92);
            }
        }
        break;
    case 1:
        o->y.raw -= o->velV << 8;
        if (--o->timer == -1) {
            o->step = 1;
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
