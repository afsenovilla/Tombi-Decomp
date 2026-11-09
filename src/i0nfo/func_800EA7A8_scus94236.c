// VERSION SCUS_94236 info 800EA7A8 1836
// USA version of PAL i0nfo 800eb4c8 (overheadBuildMapStrip.c)
#if defined(VERSION_SCUS_94236)
#include "TOBJ.H"
extern unsigned short D_800B0680[];
extern unsigned short D_800A39DC[];
extern unsigned long *D_800A39E4;

#define C1 *d++ = *s++;
#define C8 C1 C1 C1 C1 C1 C1 C1 C1
#define C32 C8 C8 C8 C8

void func_800EA7A8(unsigned char type, unsigned long *d)
{
    int rows;
    unsigned int v, w;
    unsigned long *s;
    unsigned int idx;
    int mode;
    int i;
    int stride;

    switch (type) {
    case 0: mode = 1; rows = 0x100; idx = 0; break;
    case 1: mode = 0; rows = 0x100; idx = 8; break;
    case 2: mode = 0; rows = 0x40; idx = 0x60; break;
    case 3: mode = 0; rows = 0x40; idx = 0x64; break;
    case 4: mode = 0; rows = 0x40; idx = 0x68; break;
    }
    idx = D_800B0680[idx];
    w = D_800A39DC[0];
    stride = w * 8;
    s = D_800A39E4;
    { register unsigned int q asm("$3") = idx / w * 32; s += q * stride + (idx % w) * 8; }
    if (mode != 0) {
        stride -= 64;
        for (i = 0; i < rows; i++) {
            C32 C32
            s += stride;
        }
    } else {
        stride -= 32;
        for (i = 0; i < rows; i++) {
            C32
            s += stride;
        }
    }
}
#endif
