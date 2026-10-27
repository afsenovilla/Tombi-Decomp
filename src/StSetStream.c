// FUNC 80067418 136 MAIN0
// MATCHING 80067418 136
// Portado de psx_tomba (psyq/libcd/c_005.c, StSetStream); licencia MIT del proyecto original.
#include "gv6.h"
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern int D_8009C334;
extern short D_8009C90C;
extern int ST_MODE;
extern int ST_FUNC1;
extern int ST_FUNC2;
extern int D_8009D5F4;
extern int D_800A3CC0;

void StSetStream(u_long mode, u_long start_frame, u_long end_frame, void (*func1)(), void (*func2)())
{
    StSetMask(1, start_frame, end_frame);
    D_800A3CC0 = 0;
    ST_FUNC1 = func1;
    ST_MODE = mode & 1;
    GV6.i94 = 0;
    D_8009D5F4 = 0;
    D_8009C90C = 0;
    D_8009C334 = 0;
    ST_FUNC2 = func2;
}
