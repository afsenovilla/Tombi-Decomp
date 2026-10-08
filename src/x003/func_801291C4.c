// FUNC 801291c4 124 X003
// MATCHING 801291c4 124
#include "TOBJ.H"

extern void func_80128C08(TObj *);
extern int AnimAdvance(TObj *);
extern void func_80128FA0(TObj *);

void func_801291C4(TObj *o)
{
    switch (o->step) {
    case 0:
        func_80128C08(o);
        break;
    case 1:
        AnimAdvance(o);
        break;
    case 2:
        func_80128FA0(o);
        break;
    }
}
