// FUNC 800ee440 124 I0NFO2
// MATCHING 800ee440 124
#include "TOBJ.H"
extern unsigned short D_800A4674;
extern unsigned char *D_800FECB0;

void func_800EE440(TObj *o, short x, short y)
{
    if (!(o->active & 4)) {
        int r = ((y - 0x30) >> 3) * D_800A4674;
        o->b68 = 0;
        if (D_800FECB0[(short)(((x + 0x80) >> 3) + r)] & 0x80) {
            o->b68 = 1;
        }
    }
}
