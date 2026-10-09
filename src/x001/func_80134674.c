// FUNC 80134674 720 X001
// MATCHING 80134674 720
#include "TOBJ.H"

#define Q ((TObj *)o->d90)

extern void *D_8013F034[], *D_8013F074[];
extern short D_8007A5F0[], D_8007A3F0[];
extern void FUN_8001e4f0(int);

void func_80134674(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->state++;
        o->active = 2;
        o->velX = 0x10;
        o->b0f -= 0x1e;
        o->velH = -0x400;
        o->velY = 0x40;
        o->velV = -0x400;
        if (o->b0c == 0) {
            for (p = (TObj *)o->d94; p != 0; p = (TObj *)p->d94) {
                p->b04 = 2;
                p->step = 5;
                p->state = 0;
            }
            FUN_8001e4f0(0x54);
            o->d84 = o->d88;
            if ((unsigned int)(o->d88 - 0x40) < 0x80)
                o->animFrame = 1;
            else
                o->animFrame = 0;
            o->wac = ((o->d88 + 8) & 0xff) >> 4;
            o->anim = D_8013F034[o->wac];
        } else {
            if ((o->animFrame = Q->animFrame) == 0)
                o->d84 = (Q->d84 + 0x10) & 0xff;
            else
                o->d84 = (Q->d84 - 0x10) & 0xff;
            o->h->raw = Q->h->raw;
            o->y.raw = Q->y.raw;
            o->d->raw = Q->d->raw;
            o->h->raw -= D_8007A5F0[(unsigned char)o->d84] * 192;
            o->y.raw -= D_8007A3F0[(unsigned char)o->d84] * 192;
            o->wac = ((o->d84 + 8) & 0xff) >> 4;
            o->anim = D_8013F074[o->wac];
        }
        break;
    case 1:
        if (o->velH != 0) {
            if (o->animFrame)
                o->h->raw += o->velH << 8;
            else
                o->h->raw -= o->velH << 8;
            o->velH += o->velX;
        }
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if (o->velV > 0x400)
            o->velV = 0x400;
        break;
    }
}
