// VERSION SCUS_94236 scus94236 800695C4 472
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padmain.c, _padSioRW); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int _padSioRW(padPort* inObj, int inByte)
{
    register padPort* port = inObj;
    register int outgoing = inByte;
    u_char received;
    register int deviceId;
    register u_long baud;
    int replyByte;
    register int result asm("$2");

    if (outgoing < 0) {
        received = padmain_sio->data;
        port->response_index = 0xFF;
        port->payload_index = 1;
        *port->statusBuffer = ~outgoing;
        replyByte = received;
        if (!(padmain_sio->stat & 1)) {
            do {
            } while (!(padmain_sio->stat & 1));
        }
        do {
        } while (chkRC2wait() == 0);
        result = ~outgoing;
        padmain_sio->data = result;
        result = replyByte;
        return result;
    }
    {
        register int expected;
        {
            register u_char* response = port->response_3c;
            deviceId = *response;
        }
        expected = 8;
        baud = 0x88;
        if ((deviceId >> 4) == expected) {
            if ((u_char)port->response_index >= 9U) {
                baud = 0x22;
            }
        }
    }
    {
        register padSio* registers = padmain_sio;
        register u_long timer = *(volatile u_short*)0x1F801120;
        register u_long ready = registers->stat;
        register padSio* cursor;
        D_8009FD80 = 0x1AE;
        D_8009B6A4 = timer;
        if (!(ready & 2)) {
            cursor = registers;
            do {
            } while (!(cursor->stat & 2));
        } else {
            cursor = registers;
        }
    }
    {
        register padSio* sio = padmain_sio;
        register padIntr* irq = padmain_istat;
        received = sio->data;
        replyByte = received;
        sio->baud = baud;
        if (!(irq->stat & 0x80)) {
            do {
                if (chkRC2wait() != 0) {
                    result = -20;
                    return result;
                }
            } while ((padmain_istat->stat & 0x80) == 0);
        }
        padmain_sio->data = (u_char)outgoing;
        port->payload_index += 1;
        port->response_3c[port->response_index] = replyByte;
        result = port->response_index;
        result++;
        port->response_index = result;
        result = replyByte;
    }
    return result;
}
#endif
