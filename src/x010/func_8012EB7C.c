// FUNC 8012eb7c 36 X010
// MATCHING 8012eb7c 36
#include "TOBJ.H"
extern unsigned char D_8009CE1F;

void func_8012EB7C(TObj *o)
{
    if (D_8009CE1F == 0) {
        o->step = 1;
        o->state = 0;
    }
}
