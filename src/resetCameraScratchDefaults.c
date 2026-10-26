// FUNC 80017798 164 MAIN0
// MATCHING 80017798 164
#include "spad.h"
extern unsigned short AREA_VARIANT;
extern void getBaseMatrix();

void resetCameraScratchDefaults(void)
{
    getBaseMatrix(((char*)&SPAD.bc0));
    *(short *)0x1F8000EA = -0x220;
    *(short *)0x1F8000EE = 0xA0;
    *(short *)0x1F8000F2 = -0x80;
    *(int *)0x1F800200 = 0x45;
    *(short *)0x1F8000E2 = 0;
    *(short *)0x1F8000E6 = 0;
    *(short *)0x1F8000F6 = 0;
    *(char *)0x1F8003CE = 0;
    *(short *)0x1F8001C8 = 0;
    *(char *)0x1F8003D1 = 0;
    *(char *)0x1F8003D2 = 0xFF;
    *(char *)0x1F8003D3 = 0xFF;
    AREA_VARIANT = 0;
}
