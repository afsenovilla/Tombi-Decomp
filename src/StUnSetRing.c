// FUNC 800672d0 88 MAIN0
// MATCHING 800672d0 88
// Portado de psx_tomba (psyq/libcd/c_003.c, StUnSetRing); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern volatile s8* D_80096FDC;
extern volatile s8* D_80096FE8;
extern CdlLOC D_8009B2D0;
extern int D_8009B2D4;
extern void (*D_8009C85C)(void);
extern int D_8009CA08;
extern int D_8009BC7C;
extern int D_800A15CC;
extern int D_800A15D0;
extern StHEADER* D_800A326C;

void StUnSetRing(void)
{
    EnterCriticalSection();
    CdDataCallback(0);
    CdReadyCallback(0);
    *D_80096FDC = 0;
    *D_80096FE8 = 0;
    ExitCriticalSection();
}

void data_ready_callback(void);

int StGetBackloc(CdlLOC* loc);
