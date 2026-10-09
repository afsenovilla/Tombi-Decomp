// FUNC 80067418 136 MAIN0
// MATCHING 80067418 136
// Portado de psx_tomba (psyq/libcd/c_005.c, StSetStream); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern int D_8009C334;
extern short D_8009C90C;
extern int D_8009C910;
extern int D_8009D4F4;
extern int D_8009D4F8;
extern int D_8009D5F4;
extern int D_8009D694;
extern int D_800A3CC0;

void StSetStream(u_long mode, u_long start_frame, u_long end_frame, void (*func1)(), void (*func2)())
{
    StSetMask(1, start_frame, end_frame);
    D_800A3CC0 = 0;
    D_8009D4F4 = func1;
    D_8009C910 = mode & 1;
    D_8009D694 = 0;
    D_8009D5F4 = 0;
    D_8009C90C = 0;
    D_8009C334 = 0;
    D_8009D4F8 = func2;
}
