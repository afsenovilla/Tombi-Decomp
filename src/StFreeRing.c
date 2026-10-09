// FUNC 800674a0 184 MAIN0
// MATCHING 800674a0 184
// Portado de psx_tomba (psyq/libcd/c_007.c, StFreeRing); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern int D_800A2524;
extern StHEADER* D_800A3F04;
extern int D_800A3FD8;

u_long StFreeRing(u_long *base)
{
    int temp_a1;
    int i;
    short nSectors;
    StHEADER* temp_v0;
    StHEADER* temp_v0_2;

    temp_a1 = (base - (u_long*)&D_800A3F04[D_800A3FD8]) / 504;
    temp_v0 = &D_800A3F04[temp_a1];
    nSectors = D_800A3F04[temp_a1].nSectors;
    if ((short)temp_v0->id != 4) {
        return 1;
    }
    for (i = 0; i < nSectors; i++) {
        temp_v0_2 = &D_800A3F04[i+temp_a1];
        *(short*)temp_v0_2 = 0;
    }
    D_800A2524 = i+temp_a1;
    return 0;
}
