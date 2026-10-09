// FUNC 800681d8 328 MAIN0
// MATCHING 800681d8 328
// Portado de psx_tomba (psyq/libetc/vsync.c, VSync); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libetc.h"

extern volatile s32* D_800970A4;
extern volatile s32* D_800970A8;
extern volatile s32 D_800970AC; // HSync counter
extern volatile s32 D_800970B0; 
extern volatile s32 Vcount; // VSync counter

int VSync(int mode) {
    int syncFlag;
    int elapsed;
    int timeout;
    int v;
    int n;

    syncFlag = *D_800970A4;
    elapsed = (*D_800970A8 - D_800970AC) & 0xFFFF;
    if (mode < 0) {
        return Vcount;
    }
    if (mode == 1) {
        return elapsed;
    }
    n = 1;
    v = mode > 0 ? D_800970B0 - n + mode : D_800970B0;
    timeout = mode > 0 ? mode - n : 0;
    v_wait(v, timeout);
    syncFlag = *D_800970A4;
    v_wait(Vcount + 1, 1);
    if (syncFlag & 0x400000 && (syncFlag ^ *D_800970A4) >= 0) {
        do {
        } while (!((syncFlag ^ *D_800970A4) & 0x80000000));
    }
    D_800970B0 = Vcount;
    D_800970AC = *D_800970A8;
    return elapsed;
}

void v_wait(int v, int timeout);
