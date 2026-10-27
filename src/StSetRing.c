// FUNC 80064bdc 48 MAIN0
// MATCHING 80064bdc 48
// Portado de psx_tomba (psyq/libcd/cdrom.c, StSetRing); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern StHEADER* ST_RING_ADDR;
extern u32 ST_RING_SIZE;

void StSetRing(u_long* ring_addr, u_long ring_size)
{
    ST_RING_ADDR = (StHEADER*)ring_addr;
    ST_RING_SIZE = ring_size;
    StClearRing();
}
