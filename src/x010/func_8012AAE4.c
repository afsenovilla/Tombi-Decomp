// FUNC 8012aae4 356 X010
// MATCHING 8012aae4 356
#include "TOBJ.H"
extern char D_80077D0C[];
extern unsigned char D_8012F3E4, D_8012F3E5, D_8012F3E6, D_8012F3E7;
extern void *D_80132320[];
extern void AnimLoadDuration(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);

void func_8012AAE4(TObj *o)
{
    switch (o->state) {
    case 0:
        o->active = 2;
        o->b0b = 1;
        o->b0f = 4;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->d8c = 0;
        o->wac = 4;
        o->state++;
        o->animFrame &= 1;
        o->box0 = D_8012F3E4;
        o->box1 = D_8012F3E5;
        o->box2 = D_8012F3E6;
        o->box3 = D_8012F3E7;
        o->anim = D_80132320[0];
        AnimLoadDuration(o);
        break;
    case 1:
        FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1) o->d8c = (o->d8c + 0x14) & 0xff;
    else o->d8c = (o->d8c - 0x14) & 0xff;
}
