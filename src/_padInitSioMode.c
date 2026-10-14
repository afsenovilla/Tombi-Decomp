// FUNC 800698c4 820 MAIN0
// MATCHING 800698c4 820
// FLAGS -O2 -G0
// AS gnu
/* _padInitSioMode (US psx_tomba libpad/padmain.c) with the extra PAL tail (SIO reset handshake inlined). The asm clobber of $v0 only steers the register of the `command` load. */
#include "padlocal.h"

int _padInitSioMode(padPort* port)
{
    padSio* sio = D_80098210;
    sio->ctrl = 0x40;
    sio->ctrl = 0;
    sio->mode = 0xD;
    sio->baud = 0x88;
    setRC2wait(port->currentId == 8 ? 80 : 145);
    {
        int index = D_800981EC;
        {
            register padSio* regs = D_80098210;
            register int control = 0x1003;
            if (index) {
                control = 0x3003;
            }
            regs->ctrl = control;
        }
        {
            register int* address asm("$1") = D_80098204 + index;
            register int remaining asm("$2") = *address;
            if (remaining >= 0) {
                if (remaining > 0) {
                    register int* counts = D_80098204;
                    do {
                        padAddr count;
                        count.count = counts;
                        count.word = (D_800981EC * 4) + count.word;
                        (*count.count)--;
                        D_800981CC(port->subPorts + *count.count);
                    } while (counts[D_800981EC] > 0);
                }
                {
                    register int index = D_800981EC;
                    register int* table asm("$3");
                    register int* count;
                    table = D_80098204;
                    count = &table[index];
                    if (*count == 0) {
                        register int value asm("$3") = -1;
                        register padPort* arg asm("$4") = port;
                        void (*callback)(padPort*);
                        asm("" : : "r"(arg));
                        callback = D_800981CC;
                        *count = value;
                        callback(arg);
                        D_800981D0(port);
                    }
                }
            }
        }
    }
    if (D_80098210->stat & 0x200) {
        {
            register int control = D_80098210->ctrl;
            D_80098210->ctrl = control | 0x10;
        }
        if (D_80098210->stat & 0x200) {
            while (chkRC2wait() == 0) {
            }
            D_80098210->data = 1;
            setRC2wait(100);
            if (_padClrIntSio0() == 0) {
                return 0;
            }
            sioWaitRxReady();
            (void)D_80098210->data;
            setRC2wait(0x1ae);
            while (!(D_8009820C->stat & 0x80)) {
                if (chkRC2wait()) {
                    return 0;
                }
            }
            D_80098210->data = 0x42;
            setRC2wait(0x3c);
            if (_padClrIntSio0() == 0) {
                return 0;
            }
            sioWaitRxReady();
            (void)D_80098210->data;
            setRC2wait(0x1ae);
            while (!(D_8009820C->stat & 0x80)) {
                if (chkRC2wait()) {
                    return 0;
                }
            }
            D_80098210->data = 1;
            setRC2wait(0x3c);
            if (_padClrIntSio0() == 0) {
                return 0;
            }
            sioWaitRxReady();
            (void)D_80098210->data;
            return 0;
        }
        D_8009820C->stat = ~0x80;
    }
    if (port->pad_50[0]) {
        asm("" ::: "$2");
        if (port->command) {
            goto zero;
        }
    }
    return 1;
zero:
    return 0;
}
