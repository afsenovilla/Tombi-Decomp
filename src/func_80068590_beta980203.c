// VERSION BETA_980203 beta980203 80068590 628
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/func_80069E4C_scus94236.c (func_80069E4C_scus94236, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// Portado de psx_tomba (psyq/libpad/padcmd.c, func_80069E4C); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int func_80069E4C_scus94236(padPort *inPort) {
    register padPort *port = inPort;
    switch (port->sequenceStep) {
    case 2:
        port->modeTable[port->infoRecordIndex] =
            port->response_3c[5] + (port->response_3c[4] << 8);
        port->infoRecordIndex++;
        if (port->infoRecordIndex >= port->modeCount) {
            port->infoRecordIndex = 0;
            break;
        }
        return 0;
    case 3: {
        PadCapabilityRecord *record =
            port->capabilities + port->infoRecordIndex;
        record->protocol[0] = port->response_3c[4];
        record->protocol[1] = port->response_3c[5] & 127;
        record->payloadBytes = port->response_3c[6];
        record->activationCost = port->response_3c[7];
        {
            register int high = port->response_3c[5];
            record->high_bit = high >> 7;
        }
        port->infoRecordIndex++;
        if (port->infoRecordIndex >= port->actuatorCount) {
            port->infoRecordIndex = 0;
            port->combinationBytesRemaining = 0;
            break;
        }
        return 0;
    }
    case 4: {
        PadCombinationRecord *record = port->combinations + port->infoRecordIndex;
        register u8 *source, *base;
        register int bytes asm("$4");
        unsigned offset;
        if (port->combinationBytesRemaining == 0) {
            {
                register int length = port->response_3c[4];
                bytes = 3;
                port->combinationBytesRemaining = length;
            }
            record->length = port->combinationBytesRemaining;
            {
                register u8 *response = port->response_3c;
                register int index = port->infoRecordIndex;
                source = response + 5;
                if (index == 0) {
                    base = (u8 *)port->combinations;
                    offset = port->combinationCount * 8;
                } else {
                    base = record[-1].data;
                    offset = (record[-1].length + 3) & 0x1fc;
                }
            }
            base = base + offset;
            record->data = base;
            D_8009B300 = base;
        } else {
            register u8 *response = port->response_3c;
            bytes = 6;
            source = response + 2;
        }
        bytes--;
        if (bytes != -1) {
            register u8 **destination = &D_8009B300;
            do {
                register int remaining = port->combinationBytesRemaining;
                bytes--;
                if (!remaining)
                    goto exhausted;
                {
                    register u8 *dst = *destination;
                    register int value = *source++;
                    asm("" : "=r"(source) : "0"(source));
                    *dst = value;
                    *destination = dst + 1;
                }
                port->combinationBytesRemaining--;
            } while (bytes != -1);
        }

        if (port->combinationBytesRemaining != 0) {
    zero:
        return 0;
        }
    exhausted:
        {
            port->infoRecordIndex++;
            if (port->infoRecordIndex >= port->combinationCount) {
                port->communicationState = 6;
                port->sequenceStep = 254;
                return 0;
            }
            port->combinationBytesRemaining = 0;
        }
        goto zero;
    }
    default:
        return 1;
    }
complete:
    return 1;
}
#endif
