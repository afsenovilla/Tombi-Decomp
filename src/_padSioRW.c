// FUNC 80069ce8 528 MAIN0
// MATCHING 80069ce8 528
/* Older revision of _padSioRW (US psx_tomba / parasite-eve libpad/padmain.c): one SIO0 byte exchange. This revision also
   acks I_STAT bit 7 and sets SIO_CTRL bit 4 at the slow baud (0x22). Ported from the parasite-eve _padSioRW; the baud pin (s1) only steers allocation (crutch). */
// FLAGS -O2 -G0
// AS gnu
#include "padlocal.h"

int _padSioRW(padPort *inObj, int inByte)
{
    register padPort *port = inObj;
    register int outgoing = inByte;
    register int received asm("$4");
    register int deviceId;
    register u32 baud asm("$17");
    int replyByte;
    register int result asm("$2");

    if (outgoing < 0) {
        received = PAD_SIO->data;
        asm("" : "=r"(received) : "0"(received));
        port->response_index = 0xFF;
        port->payload_index = 1;
        *port->statusBuffer = ~outgoing;
        replyByte = received & 0xFF;
        if (!(PAD_SIO->stat & 1)) {
            do {
            } while (!(PAD_SIO->stat & 1));
        }
        do {
        } while (chkRC2wait() == 0);
        result = ~outgoing;
        PAD_SIO->data = result;
        result = replyByte;
        goto done;
    }
    {
        register int expected;
        {
            register u8 *response = port->response_3c;
            deviceId = *response;
        }
        expected = 8;
        baud = 0x88;
        if ((deviceId >> 4) == expected) {
            if ((u8)port->response_index >= 9U) {
                baud = 0x22;
            }
        }
    }
    {
        register padSio *registers = PAD_SIO;
        register u32 timer = *(volatile u16 *)0x1F801120;
        register u32 ready = registers->stat;
        register padSio *cursor;
        D_800A0A18 = 0x1AE;
        D_8009C33C = timer;
        if (!(ready & 2)) {
            cursor = registers;
            do {
            } while (!(cursor->stat & 2));
        } else
            cursor = registers;
    }
    {
        register padSio *sio = PAD_SIO;
        register padIntr *irq = D_8009820C;
        asm("" : "=r"(sio), "=r"(irq) : "0"(sio), "1"(irq));
        received = sio->data;
        asm("" : "=r"(received) : "0"(received));
        replyByte = received & 0xFF;
        sio->baud = baud;
        if (!(irq->stat & 0x80)) {
        waitAcknowledge:
            if (chkRC2wait() != 0) {
                result = -20;
                goto done;
            }
            if (D_8009820C->stat & 0x80) {
                goto writeByte;
            }
            goto waitAcknowledge;
        } else {
        writeByte:
            PAD_SIO->data = (u8)outgoing;
            if (baud == 0x22) {
                D_8009820C->stat = ~0x80;
                PAD_SIO->ctrl |= 0x10;
            }
            port->payload_index += 1;
            port->response_3c[port->response_index] = replyByte;
            result = port->response_index;
            result++;
            port->response_index = result;
            result = replyByte;
        }
    }
done:
    return result;
}
