// FUNC 8012e8c8 80 X010
// MATCHING 8012e8c8 80
#include "TOBJ.H"
extern unsigned short D_8009C962;
extern unsigned char D_8009D2C3;
extern short D_8009C944;
extern short D_8009C946;

void func_8012E8C8(TObj *o)
{
    if (D_8009C962 == 1) o->y.raw = -0xea;
    else if (D_8009D2C3 & 0x40) o->y.raw = -0x50;
    else o->y.raw = -0x3b8;
    D_8009C944 = 0;
    D_8009C946 = -0x40;
}
