// FUNC 80067328 144 MAIN0
// MATCHING 80067328 144
// Portado de psx_tomba (psyq/libcd/c_003.c, data_ready_callback); licencia MIT del proyecto original.
#include "gv6.h"
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern volatile s8* D_80096340;
extern volatile s8* D_8009634C;
extern CdlLOC previous_frame_location;
extern int next_frameid;
extern void (*ST_FUNC1)(void);
extern int D_8009BC7C;
extern int StRingIdx1;
extern int StRingIdx2;
extern StHEADER* ST_RING_ADDR;

void StUnSetRing(void);

void data_ready_callback(void)
{
    StHEADER* ptr = &ST_RING_ADDR[StRingIdx2];
    do {
        ptr->id = 2;
    } while(0);
    previous_frame_location = ptr->loc;
    next_frameid = ptr->frameCount;
    StRingIdx2 = StRingIdx1;
    if (ST_FUNC1 != NULL) {
        ST_FUNC1();
    }
    GV6.ia0 = 0;
}

int StGetBackloc(CdlLOC* loc);
