// FUNC 80121934 48 X009
// MATCHING 80121934 48
#include "TOBJ.H"
extern int func_800482EC(TObj *, TObj *);

void func_80121934(TObj *o, TObj *p)
{
    if (o->type != 0x1c) func_800482EC(o, p);
}
