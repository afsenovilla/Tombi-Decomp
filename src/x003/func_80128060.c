// FUNC 80128060 412 X003
// MATCHING 80128060 412
#include "TOBJ.H"
typedef struct B { unsigned char c[4]; } B;
extern char D_80077D0C[];
extern B D_80135CB0[];
extern void *D_80139500[];
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_8001fe6c(TObj *);

void func_80128060(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b0b = 1;
        o->active = 2;
        o->b0f = 4;
        o->b0a = 2;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->d8c = 0;
        o->d88 = 0;
        o->d84 = 0;
        o->animFrame &= 1;
        o->state++;
        if (o->subtype == 3)
            o->wac = 4;
        else
            o->wac = 10;
        o->anim = D_80139500[o->wac];
        { unsigned char *p = D_80135CB0[o->wac].c;
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1]; }
        FUN_8001fe6c(o);
        break;
    case 1:
        FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1)
        o->d8c = (o->d8c + 0x14) & 0xff;
    else
        o->d8c = (o->d8c - 0x14) & 0xff;
}
