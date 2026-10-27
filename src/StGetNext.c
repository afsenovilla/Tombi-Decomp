// FUNC 80067594 196 MAIN0
// MATCHING 80067594 196
// Portado de psx_tomba (psyq/libcd/c_009.c, StGetNext); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern s32 D_800A2524;
extern s32 ST_END_FRAME;
extern StHEADER* ST_RING_ADDR;
extern volatile s32 ST_RING_SIZE;

u_long StGetNext(u_long** addr, u_long** header) {
    volatile StHEADER* ptr = &ST_RING_ADDR[D_800A2524];
    if (ptr->id == 1) {
        D_800A2524 = 0;
        if (ST_END_FRAME != 0) {
            ptr->id = 0;
        }
        ptr = &ST_RING_ADDR[D_800A2524];
    }
    if (ptr->id == 2) {
        ptr->id = 4;
        *addr = &ST_RING_ADDR[ST_RING_SIZE] + (D_800A2524 * 0x3F);
        *header = ptr;
        return 0;
    } else {
        return 1;
    }
}
