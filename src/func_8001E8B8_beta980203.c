// VERSION BETA_980203 beta980203 8001E8B8 92
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/cdSeekStream.c (func_8001D584) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#include "globals.h"
extern short *D_80077F3C[];
extern char FILE_LINKS[];
extern int CdControl(int, void *, void *);
extern int CdControlF(int, void *);

void func_8001E8B8_beta980203(short a)
{
    if (CdControl(2, FILE_LINKS + *D_80077F3C[a] * 8, 0) != 0)
        CdControlF(0x15, 0);
}
#endif
