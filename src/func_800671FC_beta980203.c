// VERSION BETA_980203 beta980203 800671FC 192
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/PadGetState_scus94236.c (PadGetState, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
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
