// FUNC 80104f2c 92 X000
// MATCHING 80104f2c 92
#include "TOBJ.H"
typedef struct { char pad[0x2c]; short w2c; } P;
extern P *D_8009C330;

void func_80104F2C(TObj *o)
{
    unsigned short m = *(unsigned short *)0x1F8001FC;
    if ((m & *(unsigned short *)0x1F8003C8) || (m & *(unsigned short *)0x1F8003C6)) {
        D_8009C330->w2c = 0xf;
        o->velY = 0;
        o->substep = 2;
    }
}
