// VERSION SCUS_94236 scus94236 80069AB8 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padcmd.c, _padSetCmd); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
void _padSetCmd(padPort *arg0, unsigned char arg1, u8 *arg2, unsigned char arg3)
{
    arg0->command = arg1;
    arg0->payload_2c = arg2;
    arg0->payload_2c_len = arg3;
}
#endif
