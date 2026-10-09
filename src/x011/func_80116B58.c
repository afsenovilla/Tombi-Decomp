// FUNC 80116b58 444 X011
// MATCHING 80116b58 444
#include "TOBJ.H"

extern void FUN_8003c980(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_800188e0(TObj *);

void func_80116B58(TObj *o)
{
    unsigned char t = o->b04;

    switch (t) {
    case 0:
        FUN_8003c980(o);
        o->velH = -0x80;
        o->velV = 0xd5;
        o->d30 = 0x3c0000;
        o->d34 = -0x500000;
        o->d38 = 0;
        o->h->raw = o->d30;
        o->y.raw = o->d34;
        o->d->raw = o->d38;
        o->velY = -0x500;
        o->timer = 0x3c;
        o->b0a = 0;
        o->b0f = 0;
        o->active = 4;
        o->b04++;
        break;
    case 1:
        if (ObjCullRegister(o)) {
            o->d30 += o->velH << 8;
            o->d34 += o->velV << 8;
            o->d34 += o->velY << 8;
            o->h->raw = o->d30;
            o->y.raw = o->d34;
            o->d->raw = o->d38;
            if (o->velY < 0x800) o->velY += 0x28;
            if (o->velY > 0) o->active = 1;
            if (--o->timer == 0) o->b04++;
        }
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
