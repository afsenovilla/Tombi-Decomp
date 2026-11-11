// VERSION BETA_980203 beta980203 80017380 136
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/resetCameraScratchDefaults_scus94236.c: the 0x45 word stored later, no 0x3D2/0x3D3/AREA_VARIANT resets.
// USA version of PAL resetCameraScratchDefaults.c (src/resetCameraScratchDefaults.c): = -0x80;=>= -0x78;.
#include "spad.h"
extern unsigned short AREA_VARIANT;
extern void getBaseMatrix();

void resetCameraScratchDefaults(void)
{
    getBaseMatrix(((char*)&SPAD.bc0));
    *(short *)0x1F8000EA = -0x220;
    *(short *)0x1F8000EE = 0xA0;
    *(short *)0x1F8000F2 = -0x78;
    *(short *)0x1F8000E2 = 0;
    *(short *)0x1F8000E6 = 0;
    *(short *)0x1F8000F6 = 0;
    *(char *)0x1F8003CE = 0;
    *(short *)0x1F8001C8 = 0;
    *(int *)0x1F800200 = 0x45;
    *(char *)0x1F8003D1 = 0;
}
#endif
