// FUNC 8006a59c 212 MAIN0
// MATCHING 8006a59c 212
// FLAGS -O2 -G0
// AS gnu
/* _padLoadActInfo (US psx_tomba libpad/padcmd.c): same source, built with GNU as (the jal/branch delay slots are filled by the assembler, not by cc1). */
#include "padlocal.h"

int _padLoadActInfo(padPort *obj, unsigned char *buffer) {
    int cursor;
    int result;
    int state;
    register int (*processFn)(padPort *);
    int rowCount;
    int columnCount;

    if (buffer == 0) {
        goto return_zero;
    }
    if (obj->capabilities != 0) {
        return 0;
    }
    if (D_800981C8() != 0) {

return_zero:
    return 0;

    }
initialize:
    result = 1;
    asm volatile("" : "=r"(result) : "0"(result));
    state = 4;
    cursor = ((int)buffer + 3) >> 2;
    obj->communicationState = state;
    state = 1;
    obj->sequenceStep = state;
    obj->fn_14 = (void (*)(void *))padQueueActInfoCmd;
    asm volatile("" ::: "memory");
    rowCount = obj->modeCount;
        processFn = func_8006A718;
    obj->fn_18 = processFn;
    asm volatile("" ::: "memory");
    columnCount = obj->actuatorCount;
    asm volatile("" : "=r"(rowCount), "=r"(columnCount), "=r"(result) : "0"(rowCount), "1"(columnCount), "2"(result));

    cursor <<= 2;
    obj->modeTable = (u16 *)cursor;
    obj->infoRecordIndex = 0;
    asm volatile("" ::: "memory");
    cursor += ((rowCount + 1) >> 1) * 4;
    obj->capabilities = (PadCapabilityRecord *)cursor;
    cursor += (columnCount * 5 + 3) & 0xFFC;
    obj->combinations = (PadCombinationRecord *)cursor;
    return result;
}
