// FUNC 8012e44c 264 X004
// MATCHING 8012e44c 264
#include "TOBJ.H"

extern unsigned char D_800A60DA;
extern Fix16 *D_800A6078;
extern short D_800A604E;
extern unsigned char D_8009CDDB;
extern unsigned char D_8009D0D9;
extern unsigned char D_8009CDD7;
extern unsigned char D_8009D0DA;

void func_8012E44C(TObj *o)
{
    if (D_800A60DA != 1) return;
    if ((unsigned short)(D_800A6078->p.whole - 0x370) < 0x28 && D_800A604E == -0x1d6) {
        if (D_8009CDDB == 0) {
            o->step = 1;
            o->state = 0;
        } else if (D_8009D0D9 != 0) {
            o->step = 1;
            o->state = 2;
        }
    }
    if ((unsigned short)(D_800A6078->p.whole - 0x3c8) < 0x28 && D_800A604E == -0x2e4) {
        if (D_8009CDD7 == 0) {
            o->step = 2;
            o->state = 0;
        } else if (D_8009D0DA != 0) {
            o->step = 2;
            o->state = 2;
        }
    }
}
