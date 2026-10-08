// FUNC 80129194 188 X004
// MATCHING 80129194 188
#include "TOBJ.H"

extern unsigned char D_8009CE22, D_8009C940, D_8009C941, D_8009C990, D_8009CE3D;
extern short D_800A604E;

void func_80129194(TObj *o)
{
    if (D_8009CE22 == 0) return;
    if (D_8009C940 && D_8009C941 == 0x94) {
        o->step = 2;
        o->state = 0;
    }
    if (D_800A604E >= -0xa8 && D_8009C990 != 1 && D_8009CE3D != 0 && D_8009CE3D != 0xff) {
        o->step = 3;
        o->state = 0;
    }
    if (o->b68) {
        o->animFrame = o->b68 & 1;
        o->step = 1;
        o->state = 0;
    }
}
