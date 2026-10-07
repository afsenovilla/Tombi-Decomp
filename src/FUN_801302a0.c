// FUNC 801302a0 116 X000
#include "TOBJ.H"
extern void FUN_80130314(TObj *);

void FUN_801302a0(TObj *o)
{
    TObj *c;
    int i;
    if (o->subtype == 0) {
        i = 1;
        FUN_80130314(o);
        c = (TObj *)o->d94;
        do {
            FUN_80130314(c);
            c->state = o->state;
            c = (TObj *)c->d94;
        } while (++i < 3);
    }
}
