// FUNC 801288b8 576 X009
// MATCHING 801288b8 576
#include "TOBJ.H"
extern int D_1F8002E8[];
extern void *D_8012E9F0[];
extern short FUN_8005e420(int, int);
extern int FUN_800202b4(TObj *);
extern void FUN_80018790(TObj *);

void func_801288B8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0xb;
        o->b0d = 0x81;
        o->w08 = FUN_8005e420(0x120, 0x1ef);
        o->d3c = D_1F8002E8[0];
        o->anim = D_8012E9F0[0];
        o->b0a = 2;
        o->box0 = 2;
        o->box1 = 4;
        o->box2 = 8;
        o->box3 = 0xc;
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            o->step++;
            o->velV = 0x80;
            o->velH = 0x100;
            o->velX = 8;
            o->timer = 0x3c;
            break;
        case 1:
            o->y.raw += o->velV << 8;
            if (o->subtype == 0) {
                o->h->raw += o->velH << 8;
                o->d8c -= 2;
            } else {
                o->h->raw -= o->velH << 8;
                o->d8c += 2;
            }
            o->velH -= o->velX;
            if (o->velH == 0) {
                o->velV = 0x180;
                o->step++;
            }
            o->timer--;
            break;
        case 2:
            o->y.raw += o->velV << 8;
            if (--o->timer == -1) o->b04 = 3;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
