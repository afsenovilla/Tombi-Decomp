// FUNC 80068320 156 MAIN0
// MATCHING 80068320 156
// Portado de psx_tomba (psyq/libetc/vsync.c, v_wait); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libetc.h"

extern volatile s32* D_80096408;
extern volatile s32* D_8009640C;
extern volatile s32 D_80096410; // HSync counter
extern volatile s32 D_80096414; 
extern volatile s32 Vcount; // VSync counter

int VSync(int mode);

void v_wait(int v, int timeout) {
    volatile int t = timeout << 15;
    while (Vcount < v) {
        if (!t--) {
            puts("VSync: timeout\n");
            ChangeClearPAD(0);
            ChangeClearRCnt(3, 0);
            return;
        }
    }
}
