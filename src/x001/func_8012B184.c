// FUNC 8012b184 336 X001
// MATCHING 8012b184 336
#include "TOBJ.H"
extern char D_80077D0C[];
extern void *D_8013FC94[];
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001faf4(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);

void func_8012B184(TObj *o)
{
    switch (o->state) {
    case 0:
        o->active = 2;
        o->b0b = 1;
        o->b0f = 4;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->wac = 9;
        o->animFrame &= 1;
        o->state++;
        o->anim = D_8013FC94[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->w98 == 0) FUN_8001faf4(o);
        else FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1) o->d8c = (o->d8c + 0x14) & 0xff;
    else o->d8c = (o->d8c - 0x14) & 0xff;
}
