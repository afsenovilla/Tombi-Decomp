// FUNC 801177b0 192 X016
// MATCHING 801177b0 192
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;
extern unsigned char D_8009CE05;
extern S2 *D_800A6078;
extern short D_800A604E;
extern unsigned char D_8009C940;
extern unsigned char D_8009C941;
extern unsigned char D_8009C93E;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001FC;

void func_801177B0(TObj *o)
{
    if (D_8009CE05 == 0xff) return;
    if ((unsigned short)(D_800A6078->s2 - 0x82) >= 0x28) return;
    if (D_800A604E != -0x3d) return;
    if (D_8009C940 != 0 && D_8009C941 == 0x3e) {
        o->step = 2;
        o->state = 0;
    } else if ((*(volatile unsigned short *)&D_8009D670 & 0x10) && (D_1F8001FC & 0x2000)) {
        D_8009C93E = 1;
        o->step = 1;
        o->state = 0;
    }
}
