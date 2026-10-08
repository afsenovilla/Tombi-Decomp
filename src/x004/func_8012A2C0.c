// FUNC 8012a2c0 92 X004
// MATCHING 8012a2c0 92
#include "TOBJ.H"
extern unsigned char D_8009CF2D;
extern short D_1F800246;

void func_8012A2C0(TObj *o)
{
    if (D_8009CF2D == 0 && o->b68) {
        if (D_1F800246 < 2) {
            o->state = 0;
            o->step++;
        } else {
            o->b68 = 0;
        }
    }
}
