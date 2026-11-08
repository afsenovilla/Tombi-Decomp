// VERSION SCUS_94236 scus94236 8006979C 564
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padmain.c, func_8006979C); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int func_8006979C_scus94236(padPort* obj, int value)
{
    padSio* sio;
    padIntr* irq;
    volatile u_short* counter;
    volatile u_short* target;
    volatile u_short* mode;
    u_long start;
    u_long limit;
    u_long ticks;
    u_long wrap;
    int baud;
    int received;
    int first_byte;
    u_short poll_status;

    first_byte = obj->response_3c[0];
    baud = 0x88;
    if ((first_byte >> 4) == 8 && obj->response_index >= 9) {
        baud = 0x22;
    }

    {
        padSio* ready_sio = padmain_sio;
        do {
            poll_status = ready_sio->stat;
        } while ((poll_status & 2) == 0);
    }
    setRC2wait(400);

    sio = padmain_sio;
    received = sio->data;
    if (obj->response_index != 0 || (received >> 4) != 8) {
        sio->baud = baud;
    } else {
        sio->baud = 0x22;
    }

    irq = padmain_istat;
    if (!(irq->stat & 0x80)) {
        register padIntr* poll_irq;
        counter = (volatile u_short*)0x1F801120;
        target = (volatile u_short*)0x1F801128;
        wrap = 0x10000;
        mode = (volatile u_short*)0x1F801124;
        poll_irq = irq;
        start = D_8009B6A4;
        limit = D_8009FD80;
        do {
            ticks = *counter;
            if (ticks < start) {
                if (*target != 0) {
                    ticks += *target;
                } else {
                    ticks += wrap;
                }
            }
            if (*mode & 0x200) {
                if (ticks - start >= limit) {
                    return -2;
                }
            }
            if (((ticks - start) >> 3) >= limit) {
                return -2;
            }
        } while (!(poll_irq->stat & 0x80));
    }

    if (obj->currentId != 8 && _padSioState == 2) {
        setRC2wait(60);
        while (!chkRC2wait()) {
        }
    }

    padmain_sio->data = value;
    obj->payload_index++;
    if (obj->response_index != 0xFF) {
        /* The response cursor is read again after the payload update. */
        asm volatile("" ::: "memory");
        obj->response_3c[obj->response_index] = received;
    }
    obj->response_index++;
    return received;
}
#endif
