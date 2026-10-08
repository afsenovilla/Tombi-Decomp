// FUNC 80104dc8 216 X001
// MATCHING 80104dc8 216
#include "TOBJ.H"
extern TObj *DAT_8009d2e8;
extern void FUN_800ef490(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_80040278(TObj *, int, int);
extern int FUN_8003facc(TObj *);

void FUN_80104dc8(TObj *o)
{
    TObj *p = DAT_8009d2e8;

    p->h->p.whole = o->h->p.whole;
    p->y.p.whole = o->y.p.whole + p->box2;
    FUN_800ef490(o);
    FUN_8010f0f4(o);
    FUN_8001fd94(o);
    FUN_8001fec0(o);
    if (o->velY > 0) {
        o->b9c = 2;
        o->d84 = 0;
        o->velY = 0;
        o->substep = 1;
    }
    FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
    if (FUN_8003facc(o) != 0) {
        o->b9c = 2;
        o->d84 = 0;
        o->velY = 0;
        o->substep = 1;
    }
}
