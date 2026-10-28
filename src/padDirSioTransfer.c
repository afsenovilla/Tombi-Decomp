// FUNC 8006b0ac 944 MAIN0
// MATCHING 8006b0ac 944
// probably: libpad padif.c callback (US func_8006A670): direct pad object table entry 80098240, PAL revision of the SIO byte loop
// FLAGS -O2 -G0
// AS gnu
/* func_8006A670 (US psx_tomba libpad/padif.c), PAL revision: the last byte loop skips the RC2 wait when the SIO status reads 0x22. */
#include "padlocal.h"

int padDirSioTransfer(padPort *inPort) {
    register padPort *port = inPort;
    register int mode;
    register int index;
    register int offset;
    register int off2;
    register padPort *other asm("$16");
    register int three;
    register int result;
    register padPort *arg;
    D_800981B8(inPort);
    mode = 0;
    if (D_800981F8) {
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
            if (--D_80098224 <= 0)
                break;
        /* Keep the incremented byte cursor: typed indexing changes scheduling. */
        if (index >= 0)
        {
            padAddr at;
            at.obj = port->subPorts;
            at.word += offset;
            D_800981B8(at.obj);
        }
        result = func_8006979C(port, (u8)D_800981B4(port, 1));
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
        register int original = D_800981EC;
        register int opposite;
        other = 0;
        opposite = !original;
        if (D_80098224 >= 2) {
            register int *count;
            register padAddr table;
            register int scaled;
            table.count = D_80098204;
            scaled = opposite * 4;
            table.word = scaled + table.word;
            count = table.count;
            {
                register int product = opposite * 15;
                off2 = product * 16;
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
                    child.obj = D_800981E0;
                    child.word = off2 + child.word;
                    child.obj = child.obj->subPorts;
                    child.word += scaled;
                    other = child.obj - 1;
                    D_800981CC(other);
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
                    D_800981CC(other - 1);
                    result = 1;
                    break;
                primary:
                    {
                        register padAddr base asm("$2");
                        base.obj = D_800981E0;
                        other = (padPort *)(base.bytes + off2);
                    }
                    D_800981CC(other);
                    D_800981D0(other);
                    result = -1;
                    } while (0);
                    *count = result;
                }
            transfer:
                arg = port;
                    }
            call_transfer:
                result = _padSioRW(port, (u8)D_800981B4(arg, mode));
                if (result < 0)
                    return result;
                setRC2wait(60);
                if (!_padClrIntSio0()) {
                timeout:
                    result = -3;
                    goto done;
                }
                --D_80098224;
            } while (D_80098224 >= 2);
        }
    }
    while (--D_80098224 > 0) {
        result = _padSioRW(port, (u8)D_800981B4(port, mode));
        if (result < 0)
            return result;
        if (*(unsigned short *)((char *)D_80098228 + 0xe) != 0x22) {
        setRC2wait(60);
        if (!_padClrIntSio0())
            return -3;
        }
    }
    func_80069A60();
    port->response_3c[port->response_index++] = *D_80098228;
    D_800981AC(0);
    result = 0;
done:
    return result;
}
