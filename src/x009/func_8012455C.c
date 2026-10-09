// FUNC 8012455c 372 X009
// MATCHING 8012455c 372
#include "TOBJ.H"

extern unsigned char D_8009CEF6;
extern unsigned char D_8009CFE5[];
extern void *D_8012E04C;
extern char D_80077D0C[];
void AnimLoadDuration(TObj *o);
void func_8004D620(int a, int b);
void FUN_8001fa88(TObj *o, unsigned short v);

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
            unsigned char *c = &D_8009CEF6;
            func_8004D620(*c + 0x29, 2);
            *c = *c + 1;
        }
        D_8009CFE5[o->b0c] = 1;
        break;
    case 1:
        FUN_8001fa88(o, 1 - o->animFrame);
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
