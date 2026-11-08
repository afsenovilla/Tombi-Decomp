// VERSION SCUS_94236 scus94236 8006ACA8 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padportd.c, func_8006ACA8); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
void func_8006ACA8_scus94236(padPort *obj) {
    u8 command;

    command = obj->command;
    obj->command = 0;
    obj->saved_command = command;
}
#endif
