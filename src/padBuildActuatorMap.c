// FUNC 8006b7ec 636 MAIN0
// MATCHING 8006b7ec 636
// probably: libpad padportd.c helper (US func_8006AD74, no US name) called from PadInitDirect: builds the actuator-enabled map
// FLAGS -O2 -G0
// AS gnu
/* func_8006AD74 (US psx_tomba libpad/padportd.c). */
#include "padlocal.h"

void padBuildActuatorMap(padPort *port) {
    register int limit, i, j, mask asm("$7"), active asm("$6");
    register int count;
    register int offset;
    register int one;
    register u8 *map, *data;
    bzero(port->actuatorEnabled, 6);
    if (port->currentExtendedId && port->payload_28) {
        limit = 6;
        if (port->payload_28_len < 7) {
            asm("" ::: "memory");
            limit = port->payload_28_len;
        }
        i = 0;
        if (port->actuatorCount) {
            one = 1;
            offset = 0;
            do {
                padAddr capability;
                active = 0;
                mask = 1;
                capability.capability = port->capabilities;
                capability.word = offset + capability.word;
                if (capability.capability->payloadBytes)
                    mask = 255;
                map = port->actuatorMap;
                data = port->payload_28;
                j = 0;
                if (limit) {
                    while (1) {
                        if (*map != i || !(*data & mask)) {
                            map++;
                            j++;
                            data++;
                            if (j < limit)
                                continue;
                        } else
                            goto selected;
                        break;
                    }
                }
            search_done:
                if (active) {
                    padAddr costRecord;
                    int total;
                    costRecord.capability = port->capabilities;
                    costRecord.word = offset + costRecord.word;
                    total = D_800981F4 + costRecord.capability->activationCost;
                    if (total < 61)
                        D_800981F4 = total;
                    else {
                        goto deny;
                    selected:
                        active = 1;
                        goto search_done;
                    deny:
                        active = 0;
                    }
                    if (active) {
                        map = port->actuatorMap;
                        data = port->actuatorEnabled;
                        j = 0;
                        if (limit)
                            do {
                                int same = *map == i;
                                map++;
                                if (same)
                                    *data = one;
                                j++;
                                data++;
                            } while (j < limit);
                    }
                }
                count = port->actuatorCount;
                asm("" : "=r"(i) : "0"(i), "r"(count));
                i++;
                offset += 5;
            } while (i < count);
        }
    } else {
        if (((unsigned)(port->currentId - 4) < 2 || port->currentId == 7) &&
            !port->currentExtendedId && port->payload_28_len >= 2) {
            if ((port->payload_28[0] & 192) == 64 && (port->payload_28[1] & 1) &&
                D_800981F4 + 10 < 61) {
                port->actuatorEnabled[1] = 1;
                port->actuatorEnabled[0] = 1;
                D_800981F4 += 10;
            }
        } else {
            asm("" ::: "memory");
            if (port->currentId == 3)
                port->actuatorEnabled[0] = 1;
            else if (!port->currentExtendedId) {
                register int value = 1;
                register int k asm("$3") = 5;
                /* Byte cursor over the object, offset by the flag index. */
                register padAddr cursor;
                cursor.obj = port;
                cursor.word += 5;
                do {
                    cursor.bytes[OFFSETOF(padPort, actuatorEnabled)] = value;
                    k--;
                    cursor.bytes--;
                } while (k >= 0);
            }
        }
    }
}
