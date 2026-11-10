// VERSION PCPX_96084 tomba 8003A870 232
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Bounce: a half cosine wave of amplitude +0x82 (0x200) on the height, phase +0x88 in steps of 4; then the height
// is reset to +0x34 and the object goes back to state 0.
#include "TOBJ.H"
extern void *D_800140B4;
extern short D_800A168C[];

void func_8003A870(TObj *o_)
{
    register TObj *o asm("$5") = o_;
    switch (o->state) {
    case 0:
        o->anim = D_800140B4;
        o->d88 = 0;
        o->d8c = 0;
        o->velV = 0x200;
        o->b69 = 0;
        o->state++;
        break;
    case 1:
        o->d88 += 4;
        if (o->d88 & 0x80) {
            o->state++;
            break;
        }
        o->y.raw -= (D_800A168C[o->d88 & 0xff] * o->velV) >> 4;
        break;
    case 2:
        o->d88 = 0;
        o->step = 0;
        o->state = 0;
        o->y.raw = o->d34;
        break;
    }
}
#endif
