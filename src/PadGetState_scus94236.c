// VERSION SCUS_94236 scus94236 80068AA8 192
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padentry.c, PadGetState); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int PadGetState(int channel) {
    padPort *obj;

    obj = _padFuncPort2Info(channel);
    if ((*(u32 *)&obj->payload_28_len & 0xFFFF0000) == 0
        && (obj == obj->ownerPort || obj->pad_38[0] == 0)
        && obj->output_30[0] == 0) {
        return obj->communicationState;
    }

    switch (obj->communicationState) {
    case 3:
        return 1;
    case 2:
        return 1;
    case 6:
        return 4;
    default:
        return obj->communicationState;
    }
}
#endif
