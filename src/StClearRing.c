// FUNC 80067270 96 MAIN0
// MATCHING 80067270 96
// Portado de psx_tomba (psyq/libcd/c_002.c, StClearRing); licencia MIT del proyecto original.
#include "gv6.h"
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern s32 Stframe_no;
extern s16 Stsector_offset;
extern s32 StCdIntrFlag;
extern s32 StRingIdx1;
extern s32 StRingIdx2;
extern s32 StRingIdx3;
extern s32 ST_RING_SIZE;

void StClearRing(void)
{
    StRingIdx3 = 0;
    StRingIdx2 = 0;
    StRingIdx1 = 0;
    GV6.ia0 = 0;
    init_ring_status(0, ST_RING_SIZE);
    StCdIntrFlag = 0;
    Stsector_offset = 0;
    Stframe_no = 0;
}
