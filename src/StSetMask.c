// FUNC 80067658 32 MAIN0
// MATCHING 80067658 32
// Portado de psx_tomba (psyq/libcd/c_010.c, StSetMask); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern u_long D_8009D5F8;
extern u_long D_800A3CC4;
extern u_long D_800A3CF8;

void StSetMask(u_long mask, u_long start, u_long end)
{
    D_800A3CF8 = mask;
    D_8009D5F8 = start;
    D_800A3CC4 = end;
}
