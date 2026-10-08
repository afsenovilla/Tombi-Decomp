// FUNC 80129f10 184 X004
// MATCHING 80129f10 184
#include "TOBJ.H"
extern void func_80129BE0(TObj *);
extern void func_80129E14(TObj *);
extern void AnimAdvance(TObj *);

void func_80129F10(TObj *o)
{
    switch (o->subtype) {
    case 0:
        switch (o->step) {
        case 0:
            if (o->b68) o->step++;
            break;
        case 1:
            func_80129BE0(o);
            break;
        case 3:
            AnimAdvance(o);
            break;
        }
        break;
    case 1:
        func_80129E14(o);
        break;
    }
}
