// FUNC 8012f3b0 372 X003
// MATCHING 8012f3b0 372
#include "TOBJ.H"
extern void FUN_8001fa88(TObj *, unsigned short);
extern void AnimLoadDuration(TObj *);
extern char DAT_80077d0c[];
extern void *D_8013A698[];

void func_8012F3B0(TObj *o)
{
    int x;

    switch (o->state) {
    case 0:
        o->active = 2;
        o->b0b = 1;
        o->b0f = 4;
        o->d8c = 0;
        o->animFrame = o->w7a & 1;
        if (o->step == 0) o->velV = 0;
        else o->velV = -0x400;
        o->movetab = DAT_80077d0c;
        o->state++;
        if (o->subtype == 0) o->wac = 0x1c;
        else o->wac = 0xb;
        o->anim = D_8013A698[o->wac];
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->step == 2) FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x20;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1) x = o->d8c + 20;
    else x = o->d8c - 20;
    o->d8c = x & 0xff;
}
