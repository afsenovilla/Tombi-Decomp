// FUNC 800674a0 184 MAIN0
// MATCHING 800674a0 184
// Portado de psx_tomba (psyq/libcd/c_007.c, StFreeRing); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern int D_800A2524;
extern StHEADER* ST_RING_ADDR;
extern int ST_RING_SIZE;

u_long StFreeRing(u_long *base)
{
    int temp_a1;
    int i;
    short nSectors;
    StHEADER* temp_v0;
    StHEADER* temp_v0_2;

    temp_a1 = (base - (u_long*)&ST_RING_ADDR[ST_RING_SIZE]) / 504;
    temp_v0 = &ST_RING_ADDR[temp_a1];
    nSectors = ST_RING_ADDR[temp_a1].nSectors;
    if ((short)temp_v0->id != 4) {
        return 1;
    }
    for (i = 0; i < nSectors; i++) {
        temp_v0_2 = &ST_RING_ADDR[i+temp_a1];
        *(short*)temp_v0_2 = 0;
    }
    D_800A2524 = i+temp_a1;
    return 0;
}
