// FUNC 801363c4 32 X000
// MATCHING 801363c4 32
#include "player.h"

int func_801363C4(void)
{
    return (*(short **)&PLAYER.obj.h)[1] > 0xd8;
}
