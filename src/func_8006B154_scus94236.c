// VERSION SCUS_94236 scus94236 8006B154 612
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padseqd.c, func_8006B154); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
void func_8006B154_scus94236(padPort *port) {
    int old = port->currentId;
    int i;
    int mode = port->response_3c[0] >> 4;
    port->currentId = mode;
    if (mode == 15)
        port->currentId = old;
    else {
        port->output_30[0] = 0;
        port->output_30[1] = port->response_3c[0];
        for (i = 2; i < port->response_index; i++)
            port->output_30[i] = port->response_3c[i];
    }
    if ((port->response_3c[1] == 0 && (port->sequenceStep != 1 || port->fn_14 != 0) &&
        port->pad_50[0] == 0) ||
        (!func_8006B494(port) && !port->saved_command && !port->retryCount &&
        port->currentId != old))
        _padFuncClrInfo(port);
    port->retryCount = 0;
    if ((u8)(port->sequenceStep - 2) < 252 && port->response_3c[0] != 0xf3)
        _padFuncClrInfo(port);
    {
        register int state = port->sequenceStep;
        register int one;
        register int value asm("$2");
        if (state != 0 && state != 255 && !port->command)
            return;
        one = 1;
        if (state != one) {
        if (state < 2) {
            value = 254;
            asm("" : : "r"(value));
            if (state == 0)
                goto idle;
            goto advance;
        }
        value = 254;
        if (state == value)
            goto terminal;
        if (state == 255)
            return;
        goto advance;
    idle:
        if (!port->currentId)
            return;
        value = port->sequenceStep;
        port->communicationState = one;
        value++;
        port->sequenceStep = value;
        return;
        }
    initial:
        value = port->sequenceStep;
        port->infoRecordIndex = 0;
    increment:
        value++;
        port->sequenceStep = value;
        return;
    terminal:
        value = 255;
        port->sequenceStep = value;
        return;
    advance:
        if (port->fn_18)
            value = port->fn_18(port);
        else
            value = _padRecvAtLoadInfo(port);
        port->sequenceStep += value;
    }
}
#endif
