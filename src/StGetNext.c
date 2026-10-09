// FUNC 80067594 196 MAIN0
// MATCHING 80067594 196
// Portado de psx_tomba (psyq/libcd/c_009.c, StGetNext); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern s32 D_800A2524;
extern s32 D_800A3CC4;
extern StHEADER* D_800A3F04;
extern volatile s32 D_800A3FD8;

u_long StGetNext(u_long** addr, u_long** header) {
    volatile StHEADER* ptr = &D_800A3F04[D_800A2524];
    if (ptr->id == 1) {
        D_800A2524 = 0;
        if (D_800A3CC4 != 0) {
            ptr->id = 0;
        }
        ptr = &D_800A3F04[D_800A2524];
    }
    if (ptr->id == 2) {
        ptr->id = 4;
        *addr = &D_800A3F04[D_800A3FD8] + (D_800A2524 * 0x3F);
        *header = ptr;
        return 0;
    } else {
        return 1;
    }
}
