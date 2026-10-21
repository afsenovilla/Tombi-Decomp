// FUNC 800e82f8 268 G0OVER
// MATCHING 800e82f8 268
#include "TOBJ.H"
extern short D_800E8C94[];
extern short D_800E8CA4[];

void gameOverLetterFlyIn(TObj *o)
{
    switch (o->state) {
    case 200: break;
    case 0:
        o->d30 = ((D_800E8C94[o->type] - o->a.p.whole) << 16) / 24;
        o->d34 = ((D_800E8CA4[o->type] - o->y.p.whole) << 16) / 24;
        o->velY = 0x200;
        o->velV = -0x1600;
        o->state++;
        break;
    case 1:
        o->a.raw += o->d30;
        o->y.raw += o->d34;
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        break;
    }
}
