// FUNC 8011da88 212 X001
// MATCHING 8011da88 212
#include "TOBJ.H"

void func_8011DA88(TObj *o)
{

    switch (o->subtype) {
    case 0: {
        int t = *(signed char *)&o->animFrame;
        o->box0 = 0x76;
        o->box1 = 0xee;
        o->box2 = 4;
        o->box3 = 8;
        o->b0a = 0x11;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->w7a = 0;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->animFrame = 2;
        o->b69 = 0;
        o->d38 = t << 12;
        o->b04++;
        break;
    }
    case 1: {
        int t = o->animFrame;
        o->box0 = 0x76;
        o->box1 = 0xec;
        o->box2 = 4;
        o->box3 = 8;
        o->b0a = 0x11;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->animFrame = 2;
        o->b69 = 0;
        o->d38 = t << 12;
        o->b04++;
        break;
    }
    }
}
