// FUNC 8011add4 500 X003
// MATCHING 8011add4 500
#include "TOBJ.H"

void FUN_80025f40(int a, int b, int c, int d);
void FUN_8001e4f0(int a);

void func_8011ADD4(TObj *o)
{
    switch (o->step) {
    case 0:
        o->step++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
    case 1:
        if (o->b69) {
            o->velH = 0x400;
            o->w74 = 0x400;
            o->velX = -0x80;
            o->velV = -0x180;
            o->velY = 0xc0;
            o->timer = 4;
            o->step++;
        }
        break;
    case 2:
        o->d8c = (o->d8c + (o->velH >> 4)) & 0xfff;
        o->velH += o->velX;
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if ((unsigned short)(o->velV + 0xff) >= 0x1ff) {
            { int t = o->velY; o->velY = -t; }
            if (o->b69) {
                FUN_80025f40(0, 0, 0xff, 4);
            }
        }
        if (o->velH == o->w74) {
            if (--o->timer == 0) {
                o->step++;
                o->b0f -= 10;
                FUN_8001e4f0(0x7c);
                break;
            }
            { int t = o->velX; o->velX = -t; }
        } else if (o->velH == -o->w74) {
            { int t = o->velX; o->velX = -t; }
        }
        break;
    case 3:
        o->d8c = (o->d8c + (o->velH >> 4)) & 0xfff;
        o->y.p.whole += 2;
        if (o->visible == 0) {
            o->b04 = 3;
        }
        break;
    }
}
