// FUNC 80104040 144 X004
// MATCHING 80104040 144
#include "TOBJ.H"
extern TObj *DAT_8009d2e8;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_8001f96c(int, int, int, int);

void FUN_80104040(TObj *o)
{
    TObj *p;
    o->timer = 0;
    PlayerSetAnimIfChanged(o, 0xd);
    FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    *(char *)((char *)o + 0xac) = 3;
    o->b9c = 0;
    p = DAT_8009d2e8;
    o->velX = 0;
    o->velY = 0;
    p->h->p.whole = o->h->p.whole;
    *(short *)((char *)p + 0x16) = o->y.p.whole + *(short *)((char *)p + 0x70);
    o->state = 2;
}
