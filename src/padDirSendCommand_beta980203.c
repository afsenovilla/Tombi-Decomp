// VERSION BETA_980203 beta980203 80068B90 216
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/padDirSendCommand_scus94236.c (padDirSendCommand, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL padDirSendCommand.c (src/padDirSendCommand.c): #include "padlocal.h"=>#include "padlocal_scus94236.h"; D_800981EC == D_800981FC && D_800981E8 != 0=>_padSioChan == _padChanStart && _padGunExec != 0; D_800981DC();=>_padFuncGetGunPos();; D_800981D8();=>_padFuncSetGunPort();; D_8009822C=>D_80097590; D_800981C4(=>_padFuncSendAuto(.
// probably: libpad padif.c callback (US func_8006A44C, no US name): direct pad object table entry 80098234
/* func_8006A44C (US psx_tomba libpad/padif.c). */
#include "padlocal_scus94236.h"

void padDirSendCommand(padPort *obj) {
    if (_padSioChan == _padChanStart && _padGunExec != 0) {
        _padFuncGetGunPos();
        _padFuncSetGunPort();
    }

    if (D_80097590 != 0) {
        _padFuncSendAuto(obj->subPorts);
        _padFuncSendAuto(obj->subPorts + 1);
    }

    /* Read again for the argument: the original reloads the byte. */
    if (*(volatile u8*)&obj->command == 0) {
        func_8006979C(obj, 0x42);
    } else {
        func_8006979C(obj, obj->command);
    }
}
#endif
