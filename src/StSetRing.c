// FUNC 80064bdc 48 MAIN0
// MATCHING 80064bdc 48
// Portado de psx_tomba (psyq/libcd/cdrom.c, StSetRing); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern StHEADER* D_800A3F04;
extern u32 D_800A3FD8;

void StSetRing(u_long* ring_addr, u_long ring_size)
{
    D_800A3F04 = (StHEADER*)ring_addr;
    D_800A3FD8 = ring_size;
    StClearRing();
}
