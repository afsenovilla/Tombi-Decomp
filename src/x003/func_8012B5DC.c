// FUNC 8012b5dc 96 X003
// MATCHING 8012b5dc 96
#include "TOBJ.H"

extern unsigned char D_8009CE5D;

void func_8012B5DC(TObj *o)
{
    if (o->b68 != 0) o->step++;
    if (D_8009CE5D == 0xff && o->step == 0) {
        o->d88 = 1;
        o->step = 1;
        o->state = 0x46;
    }
}
