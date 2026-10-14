// FUNC 8006ac5c 152 MAIN0
// MATCHING 8006ac5c 152
// FLAGS -O2 -G0
// AS gnu
/* _padSetMainMode (US psx_tomba libpad/padcmd.c). */
#include "padlocal.h"

int FUN_8006ac5c(padPort *obj, int byte1, int byte2) {
    register int compareByte asm("$19");
    register int flag asm("$3");
    register int result;

    compareByte = byte1;
    result = D_800981C8(obj);
    if (result != 0) {
        return 0;
    }

    result = 1;
    asm("" : "=r"(result) : "0"(result));
    flag = 1;
    obj->sequenceStep = flag;
    obj->fn_14 = (void (*)(void *))func_8006ACF4;
    obj->fn_18 = func_8006AD48;
    obj->requestedModeIndex = byte1;
    obj->requestedModeLock = byte2;
    flag = (compareByte & 0xFF) ^ obj->currentModeIndex;
    flag = (unsigned int)flag < 1;
    obj->modeAlreadySelected = flag;
    return result;
}
