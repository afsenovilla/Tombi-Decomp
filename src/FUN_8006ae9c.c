// FUNC 8006ae9c 216 MAIN0
// MATCHING 8006ae9c 216
// FLAGS -O2 -G0
// AS gnu
/* func_8006A44C (US psx_tomba libpad/padif.c). */
#include "padlocal.h"

void FUN_8006ae9c(padPort *obj) {
    if (D_800981EC == D_800981FC && D_800981E8 != 0) {
        D_800981DC();
        D_800981D8();
    }

    if (D_8009822C != 0) {
        D_800981C4(obj->subPorts);
        D_800981C4(obj->subPorts + 1);
    }

    /* Read again for the argument: the original reloads the byte. */
    if (*(volatile u8*)&obj->command == 0) {
        func_8006979C(obj, 0x42);
    } else {
        func_8006979C(obj, obj->command);
    }
}
