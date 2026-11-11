// VERSION BETA_980203 beta980203 800681FC 16
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/_padSetCmd_scus94236.c (the USA pad layout: command at 0x36).
// Portado de psx_tomba (psyq/libpad/padcmd.c, _padSetCmd); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
void _padSetCmd(padPort *arg0, unsigned char arg1, u8 *arg2, unsigned char arg3)
{
    arg0->command = arg1;
    arg0->payload_2c = arg2;
    arg0->payload_2c_len = arg3;
}
#endif
