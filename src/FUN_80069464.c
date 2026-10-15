// FUNC 80069464 280 MAIN0
// MATCHING 80069464 280
// FLAGS -O2 -G0
// AS gnu
/* PadEnableCom (US psx_tomba libpad/padmain.c). */
#include "padlocal.h"

int FUN_80069464(int mask)
{
    int old;
    register int frames asm("$2");
    register int* p1 asm("$18");
    int* p2;

    old = (D_80098200 << 1) | (D_800981FC == 0);
    if (old != mask) {
        D_800981E4 = 0;

        if (mask & 1) {
            p1 = &D_8009BF88[0];
            frames = *p1;
            D_800981FC = 0;
            if (frames >= 150) {
                D_800981B0(D_800981E0);
            }
            *p1 = 0;
        } else {
            D_800981FC = 1;
        }

        if (mask & 2) {
            p2 = &D_8009BF88[1];
            frames = *p2;
            D_80098200 = 1;
            if (frames >= 150) {
                D_800981B0(D_800981E0 + 1);
            }
            *p2 = 0;
        } else {
            D_80098200 = 0;
        }

        D_800981E4 = 1;
    }

    return old;
}
