// FUNC 80067658 32 MAIN0
// MATCHING 80067658 32
// Portado de psx_tomba (psyq/libcd/c_010.c, StSetMask); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern u_long ST_START_FRAME;
extern u_long ST_END_FRAME;
extern u_long ST_MASK;

void StSetMask(u_long mask, u_long start, u_long end)
{
    ST_MASK = mask;
    ST_START_FRAME = start;
    ST_END_FRAME = end;
}
