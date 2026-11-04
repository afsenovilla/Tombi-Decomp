// VERSION BETA_980203 beta980203 800324B4 156
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/playerEffect4EDispatchA.c (func_80030CDC_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
extern unsigned short TICK_COUNT;
extern void func_80030440_beta980203(TObj *);
extern void func_80030620_beta980203(TObj *);
extern void playerEffect4EStepD(TObj *);
extern void SfxPlay2(int, int);

void func_800324B4_beta980203(TObj *o)
{
    switch (o->subtype) {
    case 0:
        func_80030440_beta980203(o);
        break;
    case 1:
        func_80030620_beta980203(o);
        break;
    case 2:
        playerEffect4EStepD(o);
        break;
    }
    if ((TICK_COUNT & 3) == 0)
        SfxPlay2(3, 12);
}
#endif
