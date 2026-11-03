// FUNC 80017798 164 MAIN0
// MATCHING 80017798 164
// VERSION SLPS_01144 slps01144 8001606C 128
#if defined(VERSION_SLPS_01144)
void func_8001606C(void)
{
    *(short *)0x1F8001F0 = 0;
    *(short *)0x1F8001F2 = 0;
    *(short *)0x1F8001EE = 0;
    *(short *)0x1F8001F6 = 0;
    *(short *)0x1F8001F8 = 0;
    *(char *)0x1F8001CC = 0;
    *(char *)0x1F8001D0 = 0;
    *(char *)0x1F8001D1 = 0;
    *(char *)0x1F8001CE = 0;
    *(char *)0x1F8001CF = 0;
    *(char *)0x1F8001D3 = 0;
    *(short *)0x1F8001C6 = 0;
    *(char *)0x1F8001C4 = 0;
    *(char *)0x1F8001C5 = 0;
    *(char *)0x1F8003CC = 0;
}
#else
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
#endif
