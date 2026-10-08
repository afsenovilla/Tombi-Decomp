// FUNC 8012d1bc 68 X010
// MATCHING 8012d1bc 68
#include "TOBJ.H"
extern unsigned short D_8009C962;
extern void func_8012CFCC(TObj *);

void func_8012D1BC(TObj *o)
{
    if (o->subtype == 0 && D_8009C962 == 0) func_8012CFCC(o);
}
