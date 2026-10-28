// FUNC 80118b30 80 X017
// MATCHING 80118b30 80
#include "gcd.h"
#include "player.h"
typedef struct { short s0, s2; } S2;
typedef struct { char p[0x1e]; unsigned char b1e; } PLY;
extern PLY *PLAYER_EXT;

void func_80118B30(void)
{
    if ((unsigned short)((*(S2 **)&PLAYER.obj.h)->s2 - 0x100) < 0x90 && GCD.bcfd6 == 0) PLAYER_EXT->b1e = 0;
}
