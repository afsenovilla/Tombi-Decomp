// FUNC 801346c0 704 X000
// MATCHING 801346c0 704
#include "TOBJ.H"
extern void *DAT_8013ac2c;
extern void *DAT_8013ac30;
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_80040278(TObj *, short, short);

void FUN_801346c0(TObj *o)
{
    short r;
    short v;

    switch (o->state) {
    case 0:
        o->anim = DAT_8013ac2c;
        FUN_8001fe6c(o);
        o->animFrame = 1;
        o->velX = -0xc0;
        o->velY = -0x100;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state++;
        break;
    case 2:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        r = FUN_80040278(o, o->h->p.whole, o->y.p.whole + (o->box3 - o->box2));
        if (o->b69 | r) {
            o->active = 4;
            o->anim = DAT_8013ac30;
            FUN_8001fe6c(o);
            o->state++;
        }
        break;
    case 3:
        if (FUN_8001fec0(o)) {
            o->anim = DAT_8013ac2c;
            FUN_8001fe6c(o);
            v = 0x300;
            if (o->animFrame & 1)
                v = -0x300;
            o->velX = v;
            o->velY = -0x300;
            o->state++;
        }
        break;
    case 4:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state++;
        break;
    case 5:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        r = FUN_80040278(o, o->h->p.whole, o->y.p.whole + (o->box3 - o->box2));
        if (r)
            o->state = 3;
        break;
    }
    if (o->visible == 0) {
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
    }
}
