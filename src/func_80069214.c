// FUNC 80069214 212 MAIN0
// MATCHING 80069214 212
// FLAGS -O2 -G0
// AS gnu
/* PadInfoAct (US psx_tomba libpad/padentry.c). */
#include "padlocal.h"

int func_80069214(int channel, int index, int field) {
    padPort *obj;
    PadCapabilityRecord *entry;

    obj = D_800981C0(channel);
    if (index < 0) {
        return obj->actuatorCount;
    }
    if (index >= obj->actuatorCount) {
        return 0;
    }

    entry = &obj->capabilities[index];
    switch (field) {
    case 1:
        return entry->protocol[0];
    case 2:
        return entry->protocol[1];
    case 3:
        return entry->payloadBytes;
    case 4:
        return entry->activationCost;
    case 5:
        return entry->high_bit;
    default:
        return 0;
    }
}
