// VERSION BETA_980203 beta980203 80067A3C 532
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/_padInitSioMode_scus94236.c (_padInitSioMode, twin score 0.900) [-O2 -G0] (tools/demo_sweep.py)
// Portado de psx_tomba (psyq/libpad/padmain.c, _padInitSioMode); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int _padInitSioMode(padPort* port)
{
    register padSio* sio = padmain_sio;
    sio->ctrl = 0x40;
    sio->ctrl = 0;
    sio->mode = 0xD;
    sio->baud = 0x88;
    setRC2wait(port->currentId == 8 ? 80 : 145);
    {
        int index = _padSioChan;
        {
            register padSio* regs = padmain_sio;
            register int control = 0x1003;
            if (index) {
                control = 0x3003;
            }
            regs->ctrl = control;
        }
        {
            register int* address asm("$1") = _padFixResult + index;
            register int remaining asm("$2") = *address;
            if (remaining >= 0) {
                if (remaining > 0) {
                    register int* counts = _padFixResult;
                    do {
                        padAddr count;
                        count.count = counts;
                        count.word = (_padSioChan * 4) + count.word;
                        (*count.count)--;
                        _padFuncRecvAuto(port->subPorts + *count.count);
                    } while (counts[_padSioChan] > 0);
                }
                {
                    register int index = _padSioChan;
                    register int* table asm("$3");
                    register int* count;
                    table = _padFixResult;
                    count = &table[index];
                    if (*count == 0) {
                        register int value asm("$3") = -1;
                        register padPort* arg asm("$4") = port;
                        void (*callback)(padPort*);
                        asm("" : : "r"(arg));
                        callback = _padFuncRecvAuto;
                        *count = value;
                        callback(arg);
                        _padFuncClrCmdNo(port);
                    }
                }
            }
        }
    }
    sio = padmain_sio;
    if (sio->stat & 0x200) {
        {
            register int control = sio->ctrl;
            sio->ctrl = control | 0x10;
        }
        if (sio->stat & 0x200) {
            sio->data = 1;
            func_80069A60();
            (void)padmain_sio->data;
            return 0;
        }
        padmain_istat->stat = ~0x80;
    }
    if (port->pad_50[0] && port->command) {
        return 0;
    }

    *port->response_3c = 0;
    return 1;
}
#endif
