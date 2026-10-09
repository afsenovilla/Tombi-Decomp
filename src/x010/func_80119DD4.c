// FUNC 80119dd4 388 X010
// MATCHING 80119dd4 388
#include "TOBJ.H"

void func_80119DD4(TObj *o)
{
    switch (o->step) {
    case 0:
        o->step++;
        o->box0 = 4;
        o->box1 = 8;
        switch (o->animFrame) {
        case 0: o->box2 = 0x4e; o->box3 = 0x6e; break;
        case 1: o->box2 = 0x4c; o->box3 = 0x78; break;
        case 2: o->box2 = 0x60; o->box3 = 0x8a; break;
        case 3: o->box2 = 0x4e; o->box3 = 0x6e; break;
        case 4: o->box2 = 0x38; o->box3 = 0x58; break;
        case 5: o->box2 = 0x34; o->box3 = 0x54; break;
        case 6: o->box2 = 0x40; o->box3 = 0x60; break;
        }
        o->animFrame = 0;
        o->timer = 2;
        break;
    case 1:
        if (o->b69) {
            if (--o->timer == -1) {
                o->timer = 2;
                o->step++;
                o->animFrame = 1 - o->animFrame;
            }
        }
        break;
    case 2:
        if (--o->timer == -1) {
            o->timer = 2;
            o->step--;
            o->animFrame = 1 - o->animFrame;
        }
        break;
    }
}
