// VERSION BETA_980203 beta980203 800693CC 16
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/func_8006ACA8_scus94236.c (func_8006ACA8_scus94236, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// Portado de psx_tomba (psyq/libpad/padportd.c, func_8006ACA8); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
void func_8006ACA8_scus94236(padPort *obj) {
    u8 command;

    command = obj->command;
    obj->command = 0;
    obj->saved_command = command;
}
#endif
