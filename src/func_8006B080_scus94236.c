// VERSION SCUS_94236 scus94236 8006B080 212
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padseqd.c, func_8006B080); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int func_8006B080_scus94236(padPort *obj) {
    register int compare asm("$2");
    int state;

    compare = 0xF3;
    if (obj->response_3c[0] == compare) {
        compare = obj->currentId;
        if (compare == 0) {
            goto emit_zero;
        }
    }

    state = obj->sequenceStep;
    compare = 1;
    if (state != compare) {
    compare = state < 2;
    if (compare != 0) {
    if (state == 0) {
        return 0;
    }
    goto dispatch;

    }
high_state:
    compare = 0xFE;
    if (state == compare) {
        goto emit_zero;
    }
    compare = 0xFF;
    if (state == compare) {
        return 0;
    }

    }
    else
    {
emit_one:
    asm volatile("" : : "r"(compare));
    _padCmdParaMode(obj, 1);
    return 0;

emit_zero:
    asm volatile("" : : "r"(compare));
    _padCmdParaMode(obj, 0);
    return 0;

    }
dispatch:
    if (obj->fn_14 != 0) {
        obj->fn_14(obj);
    } else {
        _padSendAtLoadInfo(obj);
    }

done:
    return 0;
}
#endif
