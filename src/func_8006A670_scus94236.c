// VERSION SCUS_94236 scus94236 8006A670 892
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padif.c, func_8006A670); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int func_8006A670_scus94236(padPort *inPort) {
    register padPort *port = inPort;
    register int mode;
    register int index;
    register int offset;
    register padPort *other asm("$19");
    register int three;
    register int result;
    register padPort *arg;
    _padFuncCurrLimit(inPort);
    mode = 0;
    if (_padModeMtap) {
        register int id = port->response_3c[0];
        {
            register int eight = 8;
            if (id >> 4 == eight) {
                register int command = port->command;
                mode = command == 0;
            }
        }
    }
    if (mode) {
        index = -1;
        offset = -240;
        while (1) {
            if (--D_80097588 <= 0)
                break;
        /* Keep the incremented byte cursor: typed indexing changes scheduling. */
        if (index >= 0)
        {
            padAddr at;
            at.obj = port->subPorts;
            at.word += offset;
            _padFuncCurrLimit(at.obj);
        }
        result = func_8006979C(port, (u8)_padFuncGetTxd(port, 1));
        if (result < 0)
            return result;
        setRC2wait(60);
        if (!_padClrIntSio0())
            goto timeout;
        index++;
        offset += 240;
        if (index < 4)
                continue;
            break;
        }
    }
setup:
    {
        register int original = _padSioChan;
        register int opposite;
        opposite = !original;
        if (D_80097588 >= 2) {
            register int *count;
            register padAddr table;
            register int scaled;
            table.count = _padFixResult;
            scaled = opposite * 4;
            table.word = scaled + table.word;
            count = table.count;
            {
                register int product = opposite * 15;
                offset = product * 16;
            }
            three = 3;
            do {
                int remaining = *count;
                if (remaining < 0)
                    break;
                if (remaining > 0) {
                    /* Preserve the retail multiply and interior byte cursor. */
                    register int scaled;
                    register padAddr child asm("$3");
                    scaled = remaining * sizeof(padPort);
                    child.obj = _padInfoDir;
                    child.word = offset + child.word;
                    child.obj = child.obj->subPorts;
                    child.word += scaled;
                    other = child.obj - 1;
                    _padFuncRecvAuto(other);
                }
                {
                    register int value = *count;
                    if (value != three) {
                    if (value < 4) {
                        arg = port;
                        if (value >= 2)
                            goto call_transfer;
                        if (value < 0)
                            goto call_transfer;
                        goto primary;
                    }
                    arg = port;
                    if (value != 4)
                        goto call_transfer;
                    *count = three;
                    }
                    else
                    {
                paired:
                    do {
                    _padFuncRecvAuto(other - 1);
                    result = 1;
                    break;
                primary:
                    {
                        register padAddr base asm("$2");
                        base.obj = _padInfoDir;
                        other = (padPort *)(base.bytes + offset);
                    }
                    _padFuncRecvAuto(other);
                    _padFuncClrCmdNo(other);
                    result = -1;
                    } while (0);
                    *count = result;
                }
            transfer:
                arg = port;
                    }
            call_transfer:
                result = _padSioRW(port, (u8)_padFuncGetTxd(arg, mode));
                if (result < 0)
                    return result;
                setRC2wait(60);
                if (!_padClrIntSio0()) {
                timeout:
                    result = -3;
                    goto done;
                }
                --D_80097588;
            } while (D_80097588 >= 2);
        }
    }
    while (--D_80097588 > 0) {
        result = _padSioRW(port, (u8)_padFuncGetTxd(port, mode));
        if (result < 0)
            return result;
        setRC2wait(60);
        {
            int ready = _padClrIntSio0();
            if (!ready) {
                result = -3;
                return result;
            }
        }
    }
    func_80069A60();
    port->response_3c[port->response_index++] = *padif_sio;
    _padFuncNextPort(0);
    result = 0;
done:
    return result;
}
#endif
