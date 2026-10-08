// FUNC 80126ea0 168 X010
// MATCHING 80126ea0 168
#include "TOBJ.H"

extern void func_80125E00(TObj *);
extern void func_801268D4(TObj *);
extern void func_80125FB4(TObj *);
extern void func_801264B4(TObj *);
extern int AnimAdvance(TObj *);

void func_80126EA0(TObj *o)
{
    switch (o->step) {
    case 0:
        func_80125E00(o);
        break;
    case 1:
        func_801268D4(o);
        break;
    case 2:
        func_80125FB4(o);
        break;
    case 3:
        func_801264B4(o);
        break;
    }
    AnimAdvance(o);
}
