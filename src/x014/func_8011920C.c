// FUNC 8011920c 312 X014
// MATCHING 8011920c 312
#include "TOBJ.H"

extern short D_80125C30[];

void func_8011920C(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->state++;
        o->d8c = D_80125C30[o->b0c];
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->velH != 0) break;
        if (o->animFrame = p->animFrame) {
            o->state = 2;
            o->velV = 0;
            o->velY = 0x40;
        } else {
            o->state = 3;
            o->velV = 0;
            o->velY = -0x40;
        }
        break;
    case 2:
        o->velV += o->velY;
        o->d84 += o->velV >> 8;
        if (o->d84 >= 0x800) {
            o->d84 = 0x800;
            o->state = 1;
        }
        break;
    case 3:
        o->velV += o->velY;
        o->d84 += o->velV >> 8;
        if (o->d84 <= 0) {
            o->d84 = 0;
            o->state = 1;
        }
        break;
    }
}
