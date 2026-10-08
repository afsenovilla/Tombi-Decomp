// FUNC 80104ea0 140 X005
// MATCHING 80104ea0 140
#include "TOBJ.H"
extern unsigned short D_8009D670;
extern TObj *D_8009C330;
void func_80104EA0(TObj *o)
{
    unsigned short m = *(unsigned short *)0x1f8001fc;
    if ((m & *(unsigned short *)0x1f8003c8) || (m & *(unsigned short *)0x1f8003c6)) {
        if (*(volatile unsigned short *)&D_8009D670 & 0x40) {
            D_8009C330->animTimer = 15;
        } else {
            D_8009C330->animTimer = 14;
        }
        o->b9c = 0;
        o->velY = 0;
        o->substep = 2;
    }
}
