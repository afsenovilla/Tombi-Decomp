// FUNC 8001888c 84 MAIN0
// MATCHING 8001888c 84
#include "TOBJ.H"
extern unsigned short D_1f80023a;
extern int *D_1f80020c;

void FUN_8001888c(TObj *o)
{
    ((int *)o)[0] = 0;
    ((int *)o)[1] = 0;
    ((int *)o)[2] = 0;
    ((int *)o)[3] = 0;
    o->b9c = 0;
    o->b9d = 0;
    o->category &= 0x7f;
    D_1f80023a++;
    D_1f80020c--;
    *D_1f80020c = (int)o;
}
