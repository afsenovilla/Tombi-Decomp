// FUNC 800f20cc 152 I0NFO2
// MATCHING 800f20cc 152
#include "TOBJ.H"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void **D_800FEEC0[][3];

void topdownA11ActorIdleStep(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = D_800FEEC0[o->subtype][0][(short)o->animFrame];
        func_8001FE94(o, 0);
        o->state++;
    case 1:
        func_8001FEC0(o);
        break;
    }
}
