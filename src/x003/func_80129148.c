// FUNC 80129148 124 X003
// MATCHING 80129148 124
#include "TOBJ.H"

extern void func_80128A1C(TObj *);
extern int AnimAdvance(TObj *);
extern void func_80128DF8(TObj *);

void func_80129148(TObj *o)
{
    switch (o->step) {
    case 0:
        func_80128A1C(o);
        break;
    case 1:
        AnimAdvance(o);
        break;
    case 2:
        func_80128DF8(o);
        break;
    }
}
