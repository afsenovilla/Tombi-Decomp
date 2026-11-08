// VERSION SCUS_94236 scus94236 8006ACB8 188
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// USA version of PAL padGetSendByte.c (src/padGetSendByte.c): #include "padlocal.h"=>#include "padlocal_scus94236.h".
/* func_8006ACB8 of US psx_tomba libpad/padportd.c: byte of the payload for the current command (PAL padPort: payload_2c_len/command one byte later). */
#include "padlocal_scus94236.h"

int padGetSendByte(padPort *arg0, int needsAck) {
    int index;
    int mode;
    u8 *ptr;

    index = arg0->payload_index - 3;
    mode = arg0->command;
    if (mode != 0) {
        if (mode == 0x4D) {
            goto mode4D;
        }

    }
    else
    {
mode0:
    if (index < 6) {
        if (arg0->actuatorEnabled[index] == 0) {
            return 0;
        }
    }

    if (index >= arg0->payload_28_len) {
        return 0;
    }
    ptr = arg0->payload_28 + index;

load:
    asm volatile("" : "=r"(ptr) : "0"(ptr));
    return *ptr;

mode4D:
    if (index < arg0->payload_2c_len) {
        ptr = arg0->payload_2c + index;
        goto load;
    }
    return 0xFF;

    }
other:
    if (index >= arg0->payload_2c_len) {
        return 0;
    }
    return arg0->payload_2c[index];
}
#endif
