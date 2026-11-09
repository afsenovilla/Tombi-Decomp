// FUNC 80067418 136 MAIN0
// MATCHING 80067418 136
// Portado de psx_tomba (psyq/libcd/c_005.c, StSetStream); licencia MIT del proyecto original.
#include "gv6.h"
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern int Stframe_no;
extern short Stsector_offset;
extern int ST_MODE;
extern int ST_FUNC1;
extern int ST_FUNC2;
extern int StCHANNEL;
extern int StEmu_Addr;

void StSetStream(u_long mode, u_long start_frame, u_long end_frame, void (*func1)(), void (*func2)())
{
    StSetMask(1, start_frame, end_frame);
    StEmu_Addr = 0;
    ST_FUNC1 = func1;
    ST_MODE = mode & 1;
    GV6.i94 = 0;
    StCHANNEL = 0;
    Stsector_offset = 0;
    Stframe_no = 0;
    ST_FUNC2 = func2;
}
