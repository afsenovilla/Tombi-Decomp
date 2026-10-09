// FUNC 800673b8 96 MAIN0
// MATCHING 800673b8 96
// Portado de psx_tomba (psyq/libcd/c_003.c, StGetBackloc); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern volatile s8* D_80096340;
extern volatile s8* D_8009634C;
extern CdlLOC D_8009BF68;
extern int D_8009BF6C;
extern void (*D_8009C85C)(void);
extern int D_8009CA08;
extern int D_8009C914;
extern int D_800A15CC;
extern int D_800A15D0;
extern StHEADER* D_800A326C;

void StUnSetRing(void);

void data_ready_callback(void);

int StGetBackloc(CdlLOC* loc)
{
    if (D_8009C914 != 0) return -1;
    CdIntToPos(CdPosToInt(&D_8009BF68) + 1, loc);
    return D_8009BF6C;
}
