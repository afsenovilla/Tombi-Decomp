// FUNC 8006af74 172 MAIN0
// MATCHING 8006af74 172
// probably: libpad padif.c callback (US func_8006A524): direct pad object table entry 80098238, stores the block count D_80098224
// FLAGS -O2 -G0
// AS gnu
/* func_8006A524 (US psx_tomba libpad/padif.c) without the US `(result & 0xF0) == 0 -> -9` check. */
#include "padlocal.h"

int padDirReadBlockCount(padPort *obj) {
    int result;
    int blockCount;
    int value;

    if (D_8009822C != 0) {
        D_800981C4(obj->subPorts + 2);
        D_800981C4(obj->subPorts + 3);
    }

    value = 0;
    if (obj->command == 0) {
        value = D_800981F8;
    }

    result = func_8006979C(obj, value);
    if (result < 0) {
        return result;
    }

    blockCount = (result & 0xF) << 1;
    D_80098224 = blockCount;
    if (blockCount == 0) {
        D_80098224 = 0x20;
    }
    return 0;
}
