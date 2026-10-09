// FUNC 80067328 144 MAIN0
// MATCHING 80067328 144
// Portado de psx_tomba (psyq/libcd/c_003.c, data_ready_callback); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern volatile s8* D_80096340;
extern volatile s8* D_8009634C;
extern CdlLOC D_8009BF68;
extern int D_8009BF6C;
extern void (*D_8009D4F4)(void);
extern int D_8009D6A0;
extern int D_8009BC7C;
extern int D_800A2264;
extern int D_800A2268;
extern StHEADER* D_800A3F04;

void StUnSetRing(void);

void data_ready_callback(void)
{
    StHEADER* ptr = &D_800A3F04[D_800A2268];
    do {
        ptr->id = 2;
    } while(0);
    D_8009BF68 = ptr->loc;
    D_8009BF6C = ptr->frameCount;
    D_800A2268 = D_800A2264;
    if (D_8009D4F4 != NULL) {
        D_8009D4F4();
    }
    D_8009D6A0 = 0;
}

int StGetBackloc(CdlLOC* loc);
