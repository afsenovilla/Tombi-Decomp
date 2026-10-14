// FUNC 8012455c 372 X009
// MATCHING 8012455c 372
#include "gcd.h"
#include "TOBJ.H"

extern void *D_8012E04C;
extern char D_80077D0C[];
void AnimLoadDuration(TObj *o);
void printInfoMessage(int a, int b);
void applyAnimVelocityX(TObj *o, unsigned short v);

void func_8012455C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->active = 2;
        o->b0b = 1;
        o->b0f = 4;
        o->velV = -0x400;
        o->d8c = 0;
        o->movetab = D_80077D0C;
        o->state++;
        o->animFrame &= 1;
        o->anim = D_8012E04C;
        AnimLoadDuration(o);
        {
            unsigned char *c = &GCD.bcef6;
            printInfoMessage(*c + 0x29, 2);
            *c = *c + 1;
        }
        ((unsigned char*)&GCD.bcfe5)[o->b0c] = 1;
        break;
    case 1:
        applyAnimVelocityX(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1) {
        o->d8c = (o->d8c + 0x14) & 0xff;
    } else {
        o->d8c = (o->d8c - 0x14) & 0xff;
    }
}
