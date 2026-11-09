// FUNC 800681d8 328 MAIN0
// MATCHING 800681d8 328
// Portado de psx_tomba (psyq/libetc/vsync.c, VSync); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libetc.h"

extern volatile s32* vsync_gpustat;
extern volatile s32* vsync_rcnt1;
extern volatile s32 Hcount; // HSync counter
extern volatile s32 D_800970B0; 
extern volatile s32 Vcount; // VSync counter

int VSync(int mode) {
    int syncFlag;
    int elapsed;
    int timeout;
    int v;
    int n;

    syncFlag = *vsync_gpustat;
    elapsed = (*vsync_rcnt1 - Hcount) & 0xFFFF;
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
    syncFlag = *vsync_gpustat;
    v_wait(Vcount + 1, 1);
    if (syncFlag & 0x400000 && (syncFlag ^ *vsync_gpustat) >= 0) {
        do {
        } while (!((syncFlag ^ *vsync_gpustat) & 0x80000000));
    }
    D_800970B0 = Vcount;
    Hcount = *vsync_rcnt1;
    return elapsed;
}

void v_wait(int v, int timeout);
