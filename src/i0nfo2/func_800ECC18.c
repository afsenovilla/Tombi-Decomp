// FUNC 800ecc18 1896 I0NFO2
// MATCHING 800ecc18 1896
#include "TOBJ.H"
extern unsigned short D_800B1320[];
extern unsigned short D_800A4678[];
extern unsigned long *D_800A4680;

#define C1 *d++ = *s++;
#define C8 C1 C1 C1 C1 C1 C1 C1 C1
#define C32 C8 C8 C8 C8

void func_800ECC18(unsigned char type, unsigned long *d)
{
    int rows;
    unsigned int v, w;
    unsigned long *s;
    int idx;
    int mode;
    int i;
    int stride;

    switch (type) {
    case 0: mode = 1; rows = 0x100; idx = 0; break;
    case 1: mode = 0; rows = 0x100; idx = 8; break;
    case 2: mode = 2; rows = 0x20; idx = 0x60; break;
    case 3: mode = 3; rows = 0x20; idx = 0x64; break;
    case 4: mode = 4; rows = 0x20; idx = 0x68; break;
    }
    v = D_800B1320[idx];
    w = D_800A4678[0];
    stride = w * 8;
    s = D_800A4680;
    s += ((v / w) * 32 - 24) * stride + (v % w) * 8;
    switch (mode) {
    case 0:
    case 2:
    case 3:
    case 4:
        stride = D_800A4678[0] * 8 - 32;
        for (i = 0; i < rows; i++) {
            C32
            s += stride;
        }
        break;
    case 1:
        stride -= 64;
        for (i = 0; i < rows; i++) {
            C32 C32
            s += stride;
        }
        break;
    }
}
