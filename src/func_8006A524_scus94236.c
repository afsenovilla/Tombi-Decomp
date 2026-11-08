// VERSION SCUS_94236 scus94236 8006A524 192
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padif.c, func_8006A524); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int func_8006A524_scus94236(padPort *obj) {
    int result;
    int blockCount;
    int value;

    if (D_80097590 != 0) {
        _padFuncSendAuto(obj->subPorts + 2);
        _padFuncSendAuto(obj->subPorts + 3);
    }

    value = 0;
    if (obj->command == 0) {
        value = _padModeMtap;
    }

    result = func_8006979C(obj, value);
    if (result < 0) {
        return result;
    }
    if ((result & 0xF0) == 0) {
        return -9;
    }

    blockCount = (result & 0xF) << 1;
    D_80097588 = blockCount;
    if (blockCount == 0) {
        D_80097588 = 0x20;
    }
    return 0;
}
#endif
