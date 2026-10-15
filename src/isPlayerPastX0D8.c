// FUNC 801363c4 32 X000
// MATCHING 801363c4 32
#include "player.h"

int isPlayerPastX0D8(void)
{
    return (*(short **)&PLAYER.obj.h)[1] > 0xd8;
}
