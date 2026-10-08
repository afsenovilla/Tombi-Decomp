// FUNC 801295b0 124 X003
// MATCHING 801295b0 124
#include "TOBJ.H"

extern int AnimAdvance(TObj *);
extern void func_80129240(TObj *);
extern void func_80129430(TObj *);

void func_801295B0(TObj *o)
{
    switch (o->step) {
    case 0:
        AnimAdvance(o);
        break;
    case 1:
        func_80129240(o);
        break;
    case 2:
        func_80129430(o);
        break;
    }
}
