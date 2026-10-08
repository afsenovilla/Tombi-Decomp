// FUNC 8012fbb0 724 X000
// MATCHING 8012fbb0 724
#include "TOBJ.H"

void func_8012FBB0(TObj *o)
{
    short a, b;
    int t;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->timer = 2;
        t = o->b68 & 1;
        o->b6b = t;
        if (t) {
            o->velH = 0xa0;
            o->velX = -0x20;
            o->state = 1;
        } else {
            o->velH = -0xa0;
            o->velX = 0x20;
            o->state = 2;
        }
        o->b68 = 0;
        break;
    case 1:
        if (o->b68) o->state = 0;
        if (o->timer < 8) {
            o->d8c = (o->d8c + (o->velH >> 4)) & 0xff;
            o->velH += o->velX;
            if (o->velH < 0) {
                if (o->b6b) o->timer++;
                a = -0xa0 / (o->timer >> 1);
                b = 0x20 / (*(volatile short *)&o->timer >> 1);
                o->d8c = 0;
                o->state = 2;
                o->velH = a;
                o->velX = b;
            }
        }
        if (o->timer >= 8) o->state = 3;
        break;
    case 2:
        if (o->b68) o->state = 0;
        if (o->timer < 8) {
            o->d8c = (o->d8c + (o->velH >> 4)) & 0xff;
            o->velH += o->velX;
            if (o->velH > 0) {
                if (!o->b6b) o->timer++;
                a = 0xa0 / (o->timer >> 1);
                b = -0x20 / (*(volatile short *)&o->timer >> 1);
                o->d8c = 0;
                o->state = 1;
                o->velH = a;
                o->velX = b;
            }
        }
        if (o->timer >= 8) o->state = 3;
        break;
    case 3:
        o->step = 0;
        o->state = 0;
        break;
    }
}
